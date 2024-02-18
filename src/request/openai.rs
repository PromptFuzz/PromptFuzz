use std::process::Child;

use crate::{
    config::{self, get_config, LLMModel},
    is_critical_err,
    program::Program,
    FuzzerError,
};
use async_openai::{
    types::{
        ChatCompletionRequestMessage, CreateChatCompletionRequest, CreateChatCompletionRequestArgs,
        CreateChatCompletionResponse, CreateCompletionRequestArgs,
    },
    types::{CreateCompletionRequest, CreateCompletionResponse},
    Client,
};
use eyre::Result;
use once_cell::sync::OnceCell;

use self::openai_billing::{load_openai_usage, log_openai_usage};

use super::{prompt::PromptKind, Handler};

pub struct OpenAIHanler {
    _child: Option<Child>,
    rt: tokio::runtime::Runtime,
}

impl Default for OpenAIHanler {
    fn default() -> Self {
        let rt = tokio::runtime::Builder::new_current_thread()
            .enable_all()
            .build()
            .unwrap_or_else(|_| panic!("Unable to build the openai runtime."));
        //log_account_balance(&rt).unwrap();
        load_openai_usage().unwrap();
        Self { _child: None, rt }
    }
}

impl Handler for OpenAIHanler {
    fn generate_by_str(&self, prefix: &str) -> eyre::Result<Vec<Program>> {
        self.rt.block_on(generate_programs_by_prefix(prefix))
    }

    fn generate(&self, prompt: &super::prompt::Prompt) -> eyre::Result<Vec<Program>> {
        let config = config::get_config();
        match config.generative {
            config::LLMModel::Codex => Handler::generate(self, prompt),
            config::LLMModel::ChatGPT | config::LLMModel::GPT4 => {
                let start = std::time::Instant::now();
                let chat_msgs = prompt.to_chatgpt_message();
                let mut programs = self.rt.block_on(generate_programs_by_chat(chat_msgs))?;
                for program in programs.iter_mut() {
                    program.combination = prompt.get_combination()?;
                }
                log::debug!("LLM Generate time: {}s", start.elapsed().as_secs());
                Ok(programs)
            }
            config::LLMModel::Incoder => unreachable!(),
        }
    }

    fn infill_by_str(&self, prefix: &str, suffix: &str) -> eyre::Result<Vec<String>> {
        self.rt
            .block_on(generate_infills_by_request(prefix, suffix))
    }

    fn infill(&self, prompt: &super::prompt::Prompt) -> eyre::Result<Vec<String>> {
        let config = config::get_config();
        match config.infill {
            config::LLMModel::Codex => Handler::infill(self, prompt),
            config::LLMModel::ChatGPT | config::LLMModel::GPT4 => {
                let chat_msgs = prompt.to_chatgpt_message();
                if let PromptKind::Infill(_, suffix) = &prompt.kind {
                    let stop = suffix[..10].to_string();
                    self.rt
                        .block_on(generate_infills_by_chat(chat_msgs, Some(stop)))
                } else {
                    self.rt.block_on(generate_infills_by_chat(chat_msgs, None))
                }
            }
            config::LLMModel::Incoder => unreachable!(),
        }
    }

    fn stop(&mut self) -> eyre::Result<()> {
        //log_account_balance(&self.rt)?;
        Ok(())
    }
}


fn get_openai_proxy() -> Option<String> {
    match std::env::var("OPENAI_PROXY_BASE") {
        Ok(base) => Some(base),
        Err(_) => None,
    }
}

/// Get the OpenAI interface client.
fn get_client() -> Result<&'static Client> {
    // read OpenAI API key form the env var (OPENAI_API_KEY).
    pub static CLIENT: OnceCell<Client> = OnceCell::new();
    let client = CLIENT.get_or_init(|| {
        if let Some(proxy) = get_openai_proxy() {
            Client::new().with_api_base(proxy)
        } else {
            Client::new()
        }
    });
    Ok(client)
}

pub mod openai_billing {
    use std::path::PathBuf;

    use crate::{config::*, deopt::Deopt};
    /// Get the accont billing banlance
    use chrono::{Datelike, Duration, Utc};
    use reqwest::header::{HeaderMap, HeaderValue, AUTHORIZATION, CONTENT_TYPE};
    use serde_json::Value;

    use super::*;

    fn _log_account_balance(rt: &tokio::runtime::Runtime) -> Result<()> {
        let api_key = std::env::var("OPENAI_API_KEY")
            .expect("Your OPENAI_API_KEY is unset, please set it first!");
        let (total_amount, total_usage, remaining) =
            rt.block_on(_check_account_billing(&api_key)).unwrap();
        // Output total usage, total amount and remaining balance information
        log::info!("OpenAI billing Total Amount: {:.4}", total_amount);
        log::info!("OpenAI billing Used: {:.4}", total_usage);
        log::info!("OpenAI billing Remaining: {:.4}", remaining);
        Ok(())
    }

    fn _get_openai_base() -> &'static str {
        "https://api.openai.com/v1"
    }

    // This method has failed since 2023/07/24 OPENAI closed the API of accessing account balling via API KEY.
    async fn _check_account_billing(
        api_key: &str,
    ) -> Result<(f64, f64, f64), Box<dyn std::error::Error>> {
        // Calculate start and end date
        let now = Utc::now();
        let start_date = (now - Duration::days(90)).date_naive();
        let end_date = (now + Duration::days(1)).date_naive();

        // Set API request URLs and headers
        let url_subscription = format!("{}/dashboard/billing/subscription", _get_openai_base()); // Check if subscribed
        let url_usage = format!(
            "{}/dashboard/billing/usage?start_date={}&end_date={}",
            _get_openai_base(),
            start_date,
            end_date
        ); // Check usage

        let mut headers = HeaderMap::new();
        headers.insert(CONTENT_TYPE, HeaderValue::from_static("application/json"));
        headers.insert(
            AUTHORIZATION,
            HeaderValue::from_str(&format!("Bearer {}", api_key))?,
        );

        let client = reqwest::Client::new();

        // Get API limits
        let response = client
            .get(url_subscription)
            .headers(headers.clone())
            .send()
            .await?;

        if !response.status().is_success() {
            log::error!("Your account has been blocked, please log in to OpenAI for assistance.");
            return Ok((0.0, 0.0, 0.0));
        }

        let subscription_data: Value = response.json().await?;

        // Check if expired
        let timestamp_now = Utc::now().timestamp();
        let timestamp_expire = subscription_data["access_until"].as_i64().unwrap_or(0);

        if timestamp_now > timestamp_expire {
            println!(
                "Your account usage limit has expired, please log in to OpenAI for assistance."
            );
        }

        let total_amount = subscription_data["hard_limit_usd"].as_f64().unwrap_or(0.0);
        let is_subscribed = subscription_data["has_payment_method"]
            .as_bool()
            .unwrap_or(false);

        // Get usage amount
        let response = client
            .get(&url_usage)
            .headers(headers.clone())
            .send()
            .await?;
        //println!("response: {response:#?}");
        let usage_data: Value = response.json().await?;
        let mut total_usage = usage_data["total_usage"].as_f64().unwrap_or(0.0) / 100.0;

        // If the user's card is bound, the usage limit is refreshed monthly
        if is_subscribed {
            let day = now.day();
            let start_date = (now - Duration::days((day - 1).into())).date_naive();
            let url_usage = format!(
                "{}/dashboard/billing/usage?start_date={}&end_date={}",
                _get_openai_base(),
                start_date,
                end_date
            ); // Check usage
            let response = client
                .get(&url_usage)
                .headers(headers.clone())
                .send()
                .await?;
            let usage_data: Value = response.json().await?;
            total_usage = usage_data["total_usage"].as_f64().unwrap_or(0.0) / 100.0;
        }

        // Calculate remaining usage
        let remaining = total_amount - total_usage;
        Ok((total_amount, total_usage, remaining))
    }

    static mut PROMPT_USAGE: u32 = 0;
    static mut COMPLETION_USAGE: u32 = 0;
    pub static mut QUOTA_COST: f32 = 0.0;

    fn get_prompt_token_usage() -> u32 {
        unsafe { PROMPT_USAGE }
    }

    fn get_completion_token_usage() -> u32 {
        unsafe { COMPLETION_USAGE }
    }

    pub fn get_quota_cost() -> f32 {
        unsafe { QUOTA_COST }
    }

    fn get_usage_log_path() -> Result<PathBuf> {
        let config = get_config();
        let deopt = Deopt::new(&config.target)?;
        let misc_path = deopt.get_library_misc_dir()?;
        let prompt_usage_path: PathBuf = [misc_path, "openai_usage".into()].iter().collect();
        Ok(prompt_usage_path)
    }

    pub fn load_openai_usage() -> Result<()> {
        let log_path = get_usage_log_path()?;
        if log_path.exists() {
            let content = std::fs::read_to_string(log_path)?;
            let parts: Vec<&str> = content.split(' ').collect();
            assert_eq!(parts.len(), 3);
            let prompt_usage: u32 = parts[0].parse()?;
            let completion_usage: u32 = parts[1].parse()?;
            let quota_cost: f32 = parts[2].parse()?;
            unsafe { PROMPT_USAGE = prompt_usage };
            unsafe { COMPLETION_USAGE = completion_usage };
            unsafe { QUOTA_COST = quota_cost };
        }
        Ok(())
    }

    pub fn log_openai_usage(response: &CreateChatCompletionResponse) -> Result<()> {
        if let Some(usage) = &response.usage {
            let prompt_token = usage.prompt_tokens;
            let complete_token = usage.completion_tokens;
            let log_path = get_usage_log_path()?;

            unsafe {
                PROMPT_USAGE += prompt_token;
            }
            unsafe {
                COMPLETION_USAGE += complete_token;
            }
            count_billing(&response.model, prompt_token, complete_token)?;
            let content: String = [
                get_prompt_token_usage().to_string(),
                " ".into(),
                get_completion_token_usage().to_string(),
                " ".into(),
                get_quota_cost().to_string(),
            ]
            .concat();
            std::fs::write(log_path, content)?;
        }
        Ok(())
    }

    fn count_billing(model: &str, prompt_usage: u32, completion_usage: u32) -> Result<()> {
        let (prompt_price, completion_price) = match model {
            CHATGPT_MODEL => (CHATGPT_INPUTR_PRICE, CHATGPT_OUTPUT_PRICE),
            CHATGPT_MODEL_LONG => (CHATGPT_LONG_INPUT_PRICE, CHATGPT_LONG_OUTPUT_PRICE),
            GPT4_MODEL => (GPT4_INPUT_PRICE, GPT4_OUTPUT_PRICE),
            _ => unimplemented!("Model {model} is not supported!"),
        };

        let curr_fee =
            (prompt_price * prompt_usage as f32) + (completion_price * completion_usage as f32);
        let curr_fee = curr_fee / 1000_f32;
        unsafe {
            QUOTA_COST += curr_fee;
        }
        let current_cost = get_quota_cost();
        log::info!("Total OPENAI corpora cost: ${current_cost}");
        Ok(())
    }
}

/// Create an requst of code completion task via OpenAI interface client.
fn create_complete_request(prompt: &str) -> Result<CreateCompletionRequest> {
    let request = CreateCompletionRequestArgs::default()
        .model(config::CODEX_MODEL)
        .prompt(prompt)
        .max_tokens(config::MAX_TOKENS)
        .n(config::get_sample_num())
        .temperature(config::get_config().temperature)
        .stream(false)
        .echo(true)
        .build()?;
    Ok(request)
}

async fn get_complete_response(
    request: CreateCompletionRequest,
) -> Result<CreateCompletionResponse> {
    let client = get_client().unwrap();
    for _retry in 0..config::RETRY_N {
        let response = client
            .completions()
            .create(request.clone())
            .await
            .map_err(eyre::Report::new);
        match is_critical_err(&response) {
            crate::Critical::Normal => return response,
            crate::Critical::NonCritical => continue,
            crate::Critical::Critical => return Err(response.err().unwrap()),
        }
    }
    Err(FuzzerError::RetryError(format!("{request:?}"), config::RETRY_N).into())
}

/// Create a request for a chat prompt
fn create_chat_request(
    msgs: Vec<ChatCompletionRequestMessage>,
    stop: Option<String>,
) -> Result<CreateChatCompletionRequest> {
    let config = get_config();
    let tokens = count_request_token_len(&msgs);
    let mut binding = CreateChatCompletionRequestArgs::default();
    let binding = if matches!(config.generative, LLMModel::GPT4) {
        binding.model(config::GPT4_MODEL)
    }
    else if tokens < config::CHATGPT_CONTEXT_LIMIT {
        binding.model(config::CHATGPT_MODEL)
    } else {
        binding.model(config::CHATGPT_MODEL_LONG)
    };

    let mut request = binding
        .messages(msgs)
        .n(config::get_sample_num())
        .temperature(config::get_config().temperature)
        .stream(false);
    if let Some(stop) = stop {
        request = request.stop(stop);
    }
    let request = request.build()?;
    Ok(request)
}

/// Get a response for a chat request
async fn get_chat_response(
    request: CreateChatCompletionRequest,
) -> Result<CreateChatCompletionResponse> {
    let client = get_client().unwrap();
    for _retry in 0..config::RETRY_N {
        let response = client
            .chat()
            .create(request.clone())
            .await
            .map_err(eyre::Report::new);
        match is_critical_err(&response) {
            crate::Critical::Normal => {
                let response = response?;
                log_openai_usage(&response)?;
                return Ok(response);
            }
            crate::Critical::NonCritical => {
                continue;
            }
            crate::Critical::Critical => return Err(response.err().unwrap()),
        }
    }
    Err(FuzzerError::RetryError(format!("{request:?}"), config::RETRY_N).into())
}

fn create_infill_request(prefix: &str, suffix: &str) -> Result<CreateCompletionRequest> {
    let request = CreateCompletionRequestArgs::default()
        .model(config::CODEX_MODEL)
        .prompt(prefix)
        .suffix(suffix)
        .max_tokens(config::MAX_TOKENS)
        .n(config::get_sample_num())
        .temperature(config::get_config().temperature)
        .stream(false)
        .build()?;
    Ok(request)
}

/// Generate `SAMPLE_N` programs by completing the prefix.
pub async fn generate_programs_by_prefix(prefix: &str) -> Result<Vec<Program>> {
    let request = create_complete_request(prefix)?;
    let respond = get_complete_response(request).await?;
    if let Some(usage) = &respond.usage {
        log::trace!("Corpora usage: {usage:?}");
    }
    let mut programs: Vec<Program> = Vec::new();
    for choice in respond.choices {
        let program = Program::new(&choice.text);
        programs.push(program);
    }
    Ok(programs)
}

/// Generate `SAMPLE_N` programs by chatting with instructions.
pub async fn generate_programs_by_chat(
    chat_msgs: Vec<ChatCompletionRequestMessage>,
) -> Result<Vec<Program>> {
    let request = create_chat_request(chat_msgs, None)?;
    let respond = get_chat_response(request).await?;
    if let Some(usage) = &respond.usage {
        log::trace!("Corpora usage: {usage:?}");
    }
    let mut programs: Vec<Program> = Vec::new();
    for choice in respond.choices {
        let content = choice.message.content;
        let content = strip_code_wrapper(&content);
        let program = Program::new(&content);
        programs.push(program);
    }
    Ok(programs)
}

/// Generate `INFILL_N` infills for the given prefix and suffix.
async fn generate_infills_by_request(prefix: &str, suffix: &str) -> Result<Vec<String>> {
    let request = create_infill_request(prefix, suffix)?;
    let respond = get_complete_response(request).await?;
    if let Some(usage) = &respond.usage {
        log::trace!("Corpora usage: {usage:?}");
    }
    let mut infills: Vec<String> = Vec::new();
    for choice in respond.choices {
        infills.push(choice.text.clone())
    }
    Ok(infills)
}

/// Generate `INFILL_N` infills by chatting with instructions.
async fn generate_infills_by_chat(
    chat_msgs: Vec<ChatCompletionRequestMessage>,
    stop: Option<String>,
) -> Result<Vec<String>> {
    let request = create_chat_request(chat_msgs, stop)?;
    let respond = get_chat_response(request).await?;
    if let Some(usage) = &respond.usage {
        log::trace!("Corpora usage: {usage:?}");
    }
    let mut infills = vec![];
    for choice in respond.choices {
        let content = choice.message.content;
        let content = strip_code_wrapper(&content);
        infills.push(content);
    }
    Ok(infills)
}

fn strip_code_prefix<'a>(input: &'a str, pat: &str) -> &'a str {
    let pat = String::from_iter(["```", pat]);
    if input.starts_with(&pat) {
        if let Some(p) = input.strip_prefix(&pat) {
            return p;
        }
    }
    input
}

/// strip the code wrapper that ChatGPT generated with code.
fn strip_code_wrapper(input: &str) -> String {
    let mut input = input.trim();
    let mut event = "";
    if let Some(idx) = input.find("```") {
        event = &input[..idx];
        input = &input[idx..];
    }
    let input = strip_code_prefix(input, "cpp");
    let input = strip_code_prefix(input, "CPP");
    let input = strip_code_prefix(input, "C++");
    let input = strip_code_prefix(input, "c++");
    let input = strip_code_prefix(input, "c");
    let input = strip_code_prefix(input, "C");
    let input = strip_code_prefix(input, "\n");
    if let Some(idx) = input.rfind("```") {
        let input = &input[..idx];
        let input = ["/*", event, "*/\n", input].concat();
        return input;
    }
    ["/*", event, "*/\n", input].concat()
}

fn count_request_token_len(msgs: &[ChatCompletionRequestMessage]) -> usize {
    let bpe = tiktoken_rs::cl100k_base().unwrap();
    let msg_str: String = msgs.iter().map(|x| x.content.to_string()).collect();
    let tokens = bpe.encode_with_special_tokens(&msg_str);
    // 11 is align to openai's rule
    tokens.len() + 11
}