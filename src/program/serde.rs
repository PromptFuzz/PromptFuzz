use core::str::FromStr;
use std::path::PathBuf;

use crate::request::prompt::combination_to_str;
use eyre::Result;

use super::{
    gadget::{get_func_gadget, FuncGadget},
    Program, Quality,
};

pub trait Serialize: Sized {
    fn serialize(&self) -> String;
}

pub trait Deserialize: Sized {
    fn deserialize(de: &mut Deserializer) -> Result<Self>;
}

impl Serialize for Program {
    fn serialize(&self) -> String {
        let combination: Vec<String> = self
            .combination
            .iter()
            .map(|x| x.get_func_name().to_string())
            .collect();
        let mut out = format!("//<ID> {}\n", self.id);
        out.push_str(&format!(
            "//<Prompt> {}\n",
            serde_json::to_string(&combination).unwrap()
        ));
        out.push_str("/*<Combination>: [");
        out.push_str(&combination_to_str(&self.combination));
        out.push_str("\n*/\n");
        out.push_str(&format!(
            "//<score> {}, nr_unique_branch: {}\n",
            self.get_quality().compute_quality_score(),
            self.get_quality().get_nr_unique_branches(),
        ));
        out.push_str(&format!(
            "//<Quality> {}\n",
            serde_json::to_string(&self.quality).unwrap()
        ));
        out.push_str(&self.statements);
        out
    }
}

impl Deserialize for Program {
    fn deserialize(de: &mut Deserializer) -> Result<Self> {
        let mut program = Program::default();
        de.consume_token_until("<ID> ")?;
        let id: usize = de.parse_number()?;
        program.id = id;
        de.consume_token_until("//<Prompt> ")?;
        let combination = de.eat_token_until("\n")?;
        let combination: Vec<String> = serde_json::from_str(combination)?;
        let combination: Vec<&'static FuncGadget> = combination
            .iter()
            .map(|func| get_func_gadget(func).unwrap())
            .collect();
        program.combination = combination;

        de.consume_token_until("/*<Combination>:")?;
        de.consume_token_until("*/")?;
        de.consume_token_until("<Quality> ")?;
        let quality = de.consume_token_until("\n")?;
        let quality: Quality = serde_json::from_str(quality).unwrap_or_default();
        let statements = de.input.to_string();
        program.statements = statements;
        program.set_quality(quality);
        Ok(program)
    }
}

pub struct Deserializer<'de> {
    // This string starts with the input data and characters are truncated off
    // the beginning as data is parsed.
    pub input: &'de str,
}

impl<'de> Deserializer<'de> {
    /// construct a default deserializer from the input buf.
    pub fn from_input(input: &'de str) -> Self {
        Deserializer { input }
    }
}

/// Simple parser function implementation
impl<'de> Deserializer<'de> {
    // Look at the first character in the input without consuming it.
    pub fn peek_char(&self) -> Result<char> {
        self.input
            .chars()
            .next()
            .ok_or_else(|| eyre::eyre!("end of input"))
    }

    // Consume the first character in the input.
    pub fn next_char(&mut self) -> Result<char> {
        let ch = self.peek_char()?;
        self.input = &self.input[ch.len_utf8()..];
        Ok(ch)
    }

    pub fn is_empty(&self) -> bool {
        self.input.is_empty()
    }

    pub fn next_token(&mut self) -> Result<&'de str> {
        let mut index = 0;
        for c in self.input.chars() {
            match c {
                '0'..='9' | 'a'..='z' | 'A'..='Z' | '_' | ':' => index += 1,
                _ => break,
            }
        }

        let token = &self.input[..index];
        self.input = self.input[index..].trim();
        Ok(token)
    }

    pub fn eat_token(&mut self, token: &str) -> Result<()> {
        if self.input.starts_with(token) {
            self.input = self.input[token.len()..].trim();
            return Ok(());
        }
        eyre::bail!("input `{}` should start with {token}.", self.input)
    }

    pub fn is_next_token(&self, token: &str) -> bool {
        self.input.starts_with(token)
    }

    /// eat tokens until meet this `token`. It does not consume `token`.
    pub fn eat_token_until(&mut self, token: &str) -> Result<&'de str> {
        match self.input.find(token) {
            Some(len) => {
                let eaten_tokens = &self.input[..len];
                self.input = &self.input[len..];
                Ok(eaten_tokens)
            }
            None => Err(eyre::eyre!("input `{}` not found {token}", self.input)),
        }
    }

    /// eat tokens until meet this `token`. It consumes `token`.
    pub fn consume_token_until(&mut self, token: &str) -> Result<&'de str> {
        match self.input.find(token) {
            Some(len) => {
                let eaten_tokens = &self.input[..len];
                self.input = &self.input[len..];
                self.eat_token(token)?;
                Ok(eaten_tokens)
            }
            None => Err(eyre::eyre!("input `{}` not found {token}", self.input)),
        }
    }

    // Parse a string until the next '"' character.
    pub fn parse_string(&mut self) -> Result<&'de str> {
        if self.next_char()? != '"' {
            return Err(eyre::eyre!("expect a string: {}.", self.input));
        }
        match self.input.find('"') {
            Some(len) => {
                let s = &self.input[..len];
                self.input = &self.input[len + 1..];
                Ok(s)
            }
            None => Err(eyre::eyre!("parsing until end of input: {}", self.input)),
        }
    }

    pub fn parse_number<T: FromStr>(&mut self) -> eyre::Result<T> {
        let mut len = 0;
        for c in self.input.chars() {
            match c {
                '0'..='9' | '-' | '.' => {}
                _ => {
                    break;
                }
            }
            len += 1;
        }
        eyre::ensure!(len > 0, "`{}` is not number`", self.input);
        let token = &self.input[..len];
        self.input = self.input[len..].trim_start();
        match token.parse() {
            Ok(v) => Ok(v),
            Err(_) => Err(eyre::eyre!("fail to parse `{}` as number", token)),
        }
    }

    pub fn parse_vec<T: Deserialize>(&mut self) -> Result<Vec<T>> {
        let mut v = Vec::new();
        if self.next_char()? != '[' {
            return Err(eyre::eyre!("Seserializer error: expect a `[`."));
        }
        loop {
            let item = T::deserialize(self)?;
            v.push(item);
            if self.peek_char()? == ']' {
                break;
            }
            self.eat_token(",")?;
        }
        self.eat_token("]")?;
        Ok(v)
    }

    pub fn parse_path(&mut self) -> Result<PathBuf> {
        let path = self.consume_token_until("\n")?;
        let path = PathBuf::from_str(path)?;
        Ok(path)
    }

    pub fn remain(&self) -> &str {
        self.input
    }
}
