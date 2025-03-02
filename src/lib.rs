pub mod analysis;
pub mod ast;
pub mod config;
pub mod deopt;
pub mod execution;
pub mod feedback;
pub mod fuzzer;
pub mod minimize;
pub mod mutation;
pub mod program;
pub mod request;
use async_openai::error::OpenAIError;
use config::get_library_name;
use deopt::Deopt;
use eyre::Result;
use flexi_logger::{opt_format, FileSpec, Naming};
use once_cell::sync::OnceCell;

pub fn init_logger() -> Result<()> {
    let deopt = Deopt::new(get_library_name())?;
    let log_file = FileSpec::default()
        .directory(deopt.get_library_output_dir()?)
        .basename("fuzzer")
        .use_timestamp(true);
    flexi_logger::Logger::try_with_env_or_str("debug")?
        .format_for_files(opt_format)
        .log_to_file(log_file)
        .duplicate_to_stdout(flexi_logger::Duplicate::Debug)
        .rotate(
            flexi_logger::Criterion::Size(1 << 30),
            Naming::Timestamps,
            flexi_logger::Cleanup::KeepLogFiles(3),
        )
        .start()?;
    Ok(())
}

pub fn init_debug_logger() -> Result<()> {
    flexi_logger::Logger::try_with_env_or_str("debug")?
        .log_to_stdout()
        .start()?;
    Ok(())
}

pub static mut PCH_NAMES: OnceCell<Vec<String>> = OnceCell::new();

#[derive(Debug, thiserror::Error)]
pub enum FuzzerError {
    #[error("This target API `{0}` is not found in the seed.")]
    TargetNotFound(String),
    #[error("Trying to mutate line (`{0}`) with len (`{1}`) out of the bound of `{2}`.")]
    MutationOutBound(usize, usize, usize),
    #[error("Unable to get respond with `{0}` times retry from: `{1}`.")]
    RetryError(String, u8),
    #[error("Cannot find `input_data.size();`")]
    FuzzerInputError,
}

pub enum Critical {
    Normal,
    NonCritical,
    Critical,
}

/// Wether this err is critical to exit? 0: no error, 1: non-critical error, 2: critical error
pub fn is_critical_err<T>(err: &Result<T>) -> Critical {
    if let Err(err) = err {
        match err.downcast_ref::<FuzzerError>() {
            Some(FuzzerError::TargetNotFound(_)) => {
                log::warn!("{:#?}", err);
                return Critical::NonCritical;
            }
            Some(FuzzerError::MutationOutBound(_, _, _)) => {
                log::warn!("{:#?}", err);
                return Critical::NonCritical;
            }
            _ => (),
        };
        match err.downcast_ref::<OpenAIError>() {
            Some(OpenAIError::JSONDeserialize(_)) => {
                log::warn!("{:#?}", err);
                std::thread::sleep(std::time::Duration::from_secs(30));
                return Critical::NonCritical;
            }
            Some(OpenAIError::ApiError(api_err)) => {
                if api_err.message.contains("Rate limit")
                    || api_err.message.contains("Too Many Requests")
                {
                    log::warn!("Rate limit reached! Sleep 1 minute for API weak up.");
                    std::thread::sleep(std::time::Duration::from_secs(60));
                    return Critical::NonCritical;
                }
                if api_err.r#type == "cf_bad_gateway" {
                    log::warn!("502 Bad gateway!");
                    return Critical::NonCritical;
                }
                if api_err
                    .message
                    .contains("That model is currently overloaded with other requests")
                    || api_err.message.contains("Service Unavailable")
                {
                    log::warn!("That model is currently overloaded with other requests. You can retry your request, or contact us through our help center at help.openai.com if the error persists. (Please include the request ID d22597b1df46cc134d2c33aa6ba71828 in your message)");
                    std::thread::sleep(std::time::Duration::from_secs(30));
                    return Critical::NonCritical;
                }
                if api_err.message.contains(
                    "The server had an error while processing your request. Sorry about that!",
                ) {
                    log::warn!(
                        "The server had an error while processing your request. Sorry about that!"
                    );
                    std::thread::sleep(std::time::Duration::from_secs(10));
                    return Critical::NonCritical;
                }
            }
            Some(OpenAIError::Reqwest(req_err)) => {
                if req_err
                    .to_string()
                    .contains("connection closed before message completed")
                {
                    log::warn!("{:?}", err);
                    return Critical::NonCritical;
                }
                log::warn!("{:?}", err);
                return Critical::NonCritical;
            }
            _ => (),
        }
        log::error!("{:#?}", err);
        return Critical::Critical;
    }
    Critical::Normal
}
