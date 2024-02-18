use super::Handler;
use crate::config::get_library_name;
use crate::program::Program;
use crate::request::format_server_url;
use crate::{config, Deopt};
use eyre::Result;
use once_cell::sync::OnceCell;
use reqwest::{Client, ClientBuilder};
use serde::{Deserialize, Serialize};
use std::process::{Child, Command, Stdio};
use std::time;

pub struct IncoderHanlder {
    child: Option<Child>,
    rt: tokio::runtime::Runtime,
}

impl Default for IncoderHanlder {
    fn default() -> Self {
        let rt = tokio::runtime::Builder::new_current_thread()
            .enable_all()
            .build()
            .unwrap_or_else(|_| panic!("Unable to build the incoder runtime."));
        Self {
            child: Some(
                rt.block_on(init_incoder_server())
                    .unwrap_or_else(|_| panic!("Unable to start the incoder server!")),
            ),
            rt,
        }
    }
}

impl IncoderHanlder {
    fn stop_incoder_server(&mut self) -> Result<()> {
        if let Some(child) = &mut self.child {
            child.kill()?;
            return Ok(());
        }
        eyre::bail!("The incoder server isn't running yet!")
    }
}

impl Drop for IncoderHanlder {
    fn drop(&mut self) {
        self.stop_incoder_server().unwrap();
    }
}

impl Handler for IncoderHanlder {
    fn generate_by_str(&self, prefix: &str) -> eyre::Result<Vec<Program>> {
        self.rt.block_on(generate_programs_by_prefix(prefix))
    }

    fn infill_by_str(&self, prefix: &str, suffix: &str) -> eyre::Result<Vec<String>> {
        self.rt
            .block_on(generate_infills_by_request(prefix, suffix))
    }

    fn stop(&mut self) -> eyre::Result<()> {
        //self.stop_incoder_server()
        Ok(())
    }
}

async fn init_incoder_server() -> Result<Child> {
    let server_path = crate::Deopt::get_incoder_path()?;
    if !server_path.exists() {
        eyre::bail!("The script of incoder server doesn't exist!");
    }
    let deopt = Deopt::new(get_library_name())?;
    let server_port = crate::config::LISTEN_PORT.to_string();
    let child = Command::new("python3")
        .arg(server_path)
        .arg("--port")
        .arg(server_port)
        .arg("--log_file")
        .arg(deopt.get_server_logger_path()?)
        .stdout(Stdio::null())
        .stderr(Stdio::inherit())
        .spawn()
        .expect("Fial to starat incoder server");
    test_server_status().await?;
    Ok(child)
}

/// Build the server client.
fn build_client() -> Client {
    ClientBuilder::new()
        .connect_timeout(time::Duration::from_secs(config::CONNECT_TIMEOUT))
        .timeout(time::Duration::from_secs(config::LISTEN_TIMEOUT))
        .build()
        .unwrap_or_else(|_| panic!("unable to build the server client!"))
}

/// Get the Incoder interface client.
fn get_client() -> Result<&'static Client> {
    // read OpenAI API key form the env var (OPENAI_API_KEY).
    pub static CLIENT: OnceCell<Client> = OnceCell::new();
    let client = CLIENT.get_or_init(build_client);
    Ok(client)
}

/// Send syn and check the server if is start.
async fn send_syn() -> Result<()> {
    let client = get_client()?;
    let syn: usize = rand::random();
    let url = format_server_url() + "/syn?id=" + &syn.to_string();
    log::trace!("request: {url}");
    let ack = client.get(url).send().await?.text().await?;
    let ack: usize = ack.parse()?;
    log::trace!("syn: {syn}, ack: {ack}");
    if ack == syn + 1 {
        Ok(())
    } else {
        eyre::bail!("Haven't receive a valid ack from server.");
    }
}

/// test the incoder server whether is started.
async fn test_server_status() -> Result<()> {
    for _ in 0..10 {
        if send_syn().await.is_ok() {
            log::info!("The server is start!");
            return Ok(());
        } else {
            log::info!("The server is starting, please wait 10 more seconds.");
        }
        tokio::time::sleep(time::Duration::from_secs(10)).await;
    }
    eyre::bail!("The server could not start within 100 seconds, please check config.")
}

#[derive(Debug, Builder, Serialize)]
#[builder(name = "IncoderRequestBuilder")]
#[builder(setter(into, strip_option))]
#[builder(pattern = "mutable")]
pub struct IncoderRequest {
    /// The prefix string or prompt to generate the related code.
    pub prefix: Option<String>,
    /// The suffix string behind the inserting location that you want to insert before.
    #[builder(default)]
    pub suffix: Option<String>,
    /// The max number of tokens that this request should generate with.
    #[builder(default = "Some(2048)")]
    pub max_tokens: Option<usize>,
    /// What [sampling temperature](https://towardsdatascience.com/how-to-sample-from-language-models-682bceb97277) to use. Higher values means the model will take more risks. Try 0.9 for more creative applications, and 0 (argmax sampling) for ones with a well-defined answer.
    ///
    /// We generally recommend altering this or `top_p` but not both.
    #[builder(default = "Some(0.0)")]
    pub temperature: Option<f64>,
    /// An alternative to sampling with temperature, called nucleus sampling, where the model considers the results of the tokens with top_p probability mass. So 0.1 means only the tokens comprising the top 10% probability mass are considered.
    #[builder(default = "Some(0.95)")]
    pub top_p: Option<f64>,
    /// How many completions/insertions to generate for each request.
    #[builder(default = "Some(1)")]
    pub n_samples: Option<usize>,
}

impl Default for IncoderRequest {
    fn default() -> Self {
        Self {
            prefix: None,
            suffix: None,
            max_tokens: Some(2048),
            temperature: Some(0.0),
            top_p: Some(0.95),
            n_samples: Some(1),
        }
    }
}

/// Create infill request with the provided prefix and suffix.
pub fn create_infill_request(prefix: &str, suffix: &str) -> Result<IncoderRequest> {
    let request = IncoderRequestBuilder::default()
        .prefix(prefix)
        .suffix(suffix)
        .max_tokens(config::MAX_INST_TOKENS)
        .temperature(config::get_config().temperature)
        .n_samples(config::get_sample_num())
        .build()?;
    Ok(request)
}

/// Create completion request with the provided prefix.
pub fn create_complete_request(prefix: &str) -> Result<IncoderRequest> {
    let request = IncoderRequestBuilder::default()
        .prefix(prefix)
        .max_tokens(config::MAX_TOKENS)
        .temperature(config::get_config().temperature)
        .n_samples(config::get_sample_num())
        .build()?;
    Ok(request)
}

/// Given a natural language prompt as prefix and generate the rest part of code.
pub async fn get_complete_response(request: &IncoderRequest) -> Result<IncoderResponse> {
    let client = get_client()?;
    let url = format_server_url() + "/generate/";
    let request = client.post(url).json::<IncoderRequest>(request).build()?;
    log::trace!(
        "Send incoder request body: {:#?}",
        String::from_utf8(request.body().unwrap().as_bytes().unwrap().to_vec())
    );
    let response = client.execute(request).await?;
    if !response.status().is_success() {
        let ret_code = response.status();
        let err_body = response.text().await?;
        eyre::bail!("[{ret_code}], body: {err_body}");
    }
    let response = response.json::<IncoderResponse>().await?;
    Ok(response)
}

/// Generate programs by the given prompt string.
pub async fn generate_programs_by_prefix(prefix: &str) -> Result<Vec<Program>> {
    let request = create_complete_request(prefix)?;
    let response = get_complete_response(&request).await?;
    let mut programs = Vec::new();
    for choice in response.choices {
        let program_str = prefix.to_owned() + &choice;
        let program = Program::new(&program_str);
        programs.push(program);
    }
    Ok(programs)
}

#[derive(Deserialize, Debug, Default)]
pub struct IncoderResponse {
    pub choices: Vec<String>,
    pub done_status: Vec<bool>,
}

/// Send infill request and get response.
async fn get_infill_response(request: &IncoderRequest) -> Result<IncoderResponse> {
    let client = get_client()?;
    let url = format_server_url() + "/infill/";
    let request = client.post(url).json(request).build()?;
    let response = client.execute(request).await?;
    if !response.status().is_success() {
        let ret_code = response.status();
        let err_body = response.text().await?;
        eyre::bail!("[{ret_code}], body: {err_body}");
    }
    let response = response.json::<IncoderResponse>().await?;
    Ok(response)
}

async fn generate_infills_by_request(prefix: &str, suffix: &str) -> Result<Vec<String>> {
    let request = create_infill_request(prefix, suffix)?;
    let response = get_infill_response(&request).await?;
    Ok(response.choices)
}

#[test]
fn test_incoder_server() -> Result<()> {
    let handler = IncoderHanlder::default();
    assert!(handler.rt.block_on(test_server_status()).is_ok());
    drop(handler);
    let rt = tokio::runtime::Builder::new_current_thread()
        .enable_all()
        .build()
        .unwrap_or_else(|_| panic!("Unable to build the incoder runtime."));
    assert!(rt.block_on(send_syn()).is_err());
    Ok(())
}
