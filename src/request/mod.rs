use crate::program::Program;

use self::prompt::Prompt;

pub mod openai;
pub mod prompt;

pub trait Handler {
    /// generate programs via a prefix string
    fn generate_by_str(&self, prefix: &str) -> eyre::Result<Vec<Program>>;

    /// generate programs via a formatted prompt
    fn generate(&self, prompt: &Prompt) -> eyre::Result<Vec<Program>>;

    /// infill programs via the context of inserting location (i.e., prefix and suffix)
    /// (deprecated)
    fn infill_by_str(&self, prefix: &str, suffix: &str) -> eyre::Result<Vec<String>>;

    /// infill programs via a formatted prompt
    /// (deprecated)
    fn infill(&self, prompt: &Prompt) -> eyre::Result<Vec<String>>;
}

/// concat the prefix, text and suffix.
pub fn program_infill(prefix: &str, text: &str, suffix: &str) -> String {
    let mut new_program = prefix.to_owned();
    new_program.push_str(text);
    new_program.push_str(suffix);
    new_program
}
