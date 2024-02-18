use crate::{
    config::{get_config, get_library_name},
    deopt::utils::get_file_dirname,
    feedback::clang_coverage::utils::{dump_fuzzer_coverage, sanitize_by_fuzzer_coverage},
    program::{serde::Serialize, transform::Transformer, Program},
    Deopt,
};
use eyre::Result;
use std::{
    path::{Path, PathBuf},
    process::{Command, Stdio},
};

use self::utils::{log_san_cost, cleanup_sanitize_dir};

use super::{ast::remove_duplicate_definition, logger::ProgramError, Executor};

impl Executor {
    /// check whether the c porgram is syntaxically and semantically correct.
    fn is_program_syntax_correct(&self, program_path: &Path) -> Result<Option<ProgramError>> {
        let start = std::time::Instant::now();
        let output: std::process::Output = Command::new("clang++")
            .stdout(Stdio::null())
            .arg("-fsyntax-only")
            .arg(&self.header_cmd)
            .arg(program_path.as_os_str())
            .output()
            .expect("failed to execute the syntax check process");
        log_san_cost(start, program_path)?;
        let success = output.status.success();
        if success {
            return Ok(None);
        }
        let err_msg = String::from_utf8_lossy(&output.stderr).to_string();
        Ok(Some(ProgramError::Syntax(err_msg)))
    }

    /// check whether the program is correct in compilation and linkage.
    fn is_program_link_correct(&self, program_path: &Path) -> Result<Option<ProgramError>> {
        let start = std::time::Instant::now();
        remove_duplicate_definition(program_path)?;
        let mut binary_out = PathBuf::from(program_path);
        binary_out.set_extension("out");

        let res = self.compile(vec![program_path], &binary_out, super::Compile::FUZZER);
        log_san_cost(start, program_path)?;

        if let Err(err) = res {
            let err_msg = err.to_string();
            return Ok(Some(ProgramError::Link(err_msg)));
        }
        Ok(None)
    }

    /// linked with AddressSanitizer, execute it to check whether code is correct.
    fn is_program_execute_correct(&self, program_path: &Path) -> Result<Option<ProgramError>> {
        let start = std::time::Instant::now();
        let mut transformer = Transformer::new(program_path, &self.deopt)?;
        transformer.add_fd_sanitizer()?;
        transformer.preprocess()?;

        let mut binary_out = PathBuf::from(program_path);
        binary_out.set_extension("out");

        self.deopt
            .copy_library_init_file(&get_file_dirname(program_path))?;

        self.compile(vec![program_path], &binary_out, super::Compile::FUZZER)?;

        // Execute the program on each corpus file and check error.
        let corpus = self.deopt.get_library_shared_corpus_dir()?;
        let has_err = self.execute_pool(&binary_out, &corpus);
        log_san_cost(start, program_path)?;
        Ok(has_err)
    }

    /// linked with LibFuzzer and AddressSanitizer, to check whether code is correct.
    pub fn is_program_fuzz_correct(&self, program_path: &Path) -> Result<Option<ProgramError>> {
        log::trace!("test program is fuzz correct: {program_path:?}");
        let start = std::time::Instant::now();
        let work_dir = get_file_dirname(program_path);

        let binary_out = program_path.with_extension("out");

        // execute fuzzer for duration timeout.
        let corpus_dir: PathBuf = [work_dir.clone(), "corpus".into()].iter().collect();
        crate::deopt::utils::copy_corpus(
            &self.deopt.get_library_shared_corpus_dir()?,
            &corpus_dir,
        )?;

        let res = self.execute_fuzzer(&binary_out, &corpus_dir);
        log_san_cost(start, program_path)?;
        if let Err(err) = res {
            return Ok(Some(ProgramError::Fuzzer(err.to_string())));
        }
        Ok(None)
    }

    pub fn is_program_coverage_correct(&self, program_path: &Path) -> Result<Option<ProgramError>> {
        log::trace!("test program is coverage correct: {program_path:?}");
        let start = std::time::Instant::now();
        let work_dir = get_file_dirname(program_path);

        // compile fuzzer with coverage instrumented library.
        let fuzzer_binary = program_path.with_extension("cov.out");
        self.compile(vec![program_path], &fuzzer_binary, super::Compile::COVERAGE)?;

        // Run the fuzzer on the previous synthesized corpus and collect coverage.
        let corpus_dir: PathBuf = [work_dir.clone(), "corpus".into()].iter().collect();
        let coverage =
            self.collect_code_coverage(Some(program_path), &fuzzer_binary, &corpus_dir)?;

        // Sanitize the fuzzer by its reached lines
        let has_err = sanitize_by_fuzzer_coverage(program_path, &self.deopt, &coverage)?;
        log_san_cost(start, program_path)?;
        self.update_corpus(program_path)?;
        std::fs::remove_dir_all(corpus_dir)?;

        if !has_err {
            return Ok(None);
        }
        let err_msg = dump_fuzzer_coverage(&fuzzer_binary)?;
        Ok(Some(ProgramError::Coverage(format!("The program cannot cover the callees along the path that contains maximum callees.\n{err_msg}"))))
    }

    pub fn check_program_is_correct(&self, seed_path: &Path) -> Result<Option<ProgramError>> {
        if let Some(err) = self.is_program_syntax_correct(seed_path)? {
            return Ok(Some(err));
        }
        if let Some(err) = self.is_program_link_correct(seed_path)? {
            return Ok(Some(err));
        }
        if let Some(err) = self.is_program_execute_correct(seed_path)? {
            return Ok(Some(err));
        }
        if let Some(err) = self.is_program_fuzz_correct(seed_path)? {
            return Ok(Some(err));
        }
        if let Some(err) = self.is_program_coverage_correct(seed_path)? {
            return Ok(Some(err));
        }
        Ok(None)
    }

    pub fn check_programs_are_correct(
        &self,
        programs: &[Program],
        deopt: &Deopt,
    ) -> Result<Vec<Option<ProgramError>>> {
        let mut program_paths = Vec::new();
        for (_i, program) in programs.iter().enumerate() {
            let temp_path = deopt.get_work_seed_by_id(program.id)?;
            let mut content = String::new();
            content.push_str(crate::deopt::utils::format_library_header_strings(deopt));
            content.push_str(&program.serialize());
            std::fs::write(&temp_path, content)?;
            program_paths.push(temp_path);
        }
        let res = self.concurrent_check(&program_paths, get_config().cores)?;
        // clean out the failure cache.
        for (i, has_err) in res.iter().enumerate() {
            let path = &program_paths[i];
            let dir = get_file_dirname(path);
            cleanup_sanitize_dir(&dir)?;
            if let Some(err) = has_err {
                // skip delete the hang and fuzzer error programs, those may contain true bugs.
                if let ProgramError::Hang(_) = err {
                    continue;
                }
                if let ProgramError::Fuzzer(_) = err {
                    continue;
                }
                std::fs::remove_dir_all(dir)?;
            }
        }
        Ok(res)
    }

    /// Using multi-process to run a fixed size of batch of programs, and check the program correctness.
    pub fn concurrent_check_batch(
        &self,
        programs: &Vec<PathBuf>,
        core: usize,
    ) -> Result<Vec<Option<ProgramError>>> {
        let mut childs = Vec::new();
        for i in 0..core {
            if i >= programs.len() {
                break;
            }
            let program = programs.get(i).unwrap();
            let child = Command::new("cargo")
                .env("RUST_BACKTRACE", "full")
                .arg("run")
                .arg("-q")
                .arg("--bin")
                .arg("harness")
                .arg("--")
                .arg(get_library_name())
                .arg("check")
                .arg(program)
                .stdout(Stdio::null())
                .stderr(Stdio::piped())
                .spawn()
                .expect("failed to execute the concurrent transform process");
            childs.push(child);
        }
        let mut has_errs: Vec<Option<ProgramError>> = Vec::new();
        // for each child process, wait output and log the error reason.
        for (i, child) in childs.into_iter().enumerate() {
            let output = child.wait_with_output().expect("command wasn't running");
            let program = programs.get(i).unwrap();
            if !output.status.success() {
                let err_msg = String::from_utf8_lossy(&output.stderr).to_string();
                let p_err = serde_json::from_str::<ProgramError>(&err_msg);
                if let Ok(err) = p_err {
                    has_errs.push(Some(err));
                } else {
                    has_errs.push(Some(ProgramError::Fuzzer(err_msg)));
                }
                log::trace!("error: {program:?}");
            } else {
                has_errs.push(None);
                log::trace!("correct: {program:?}");
            }
        }
        Ok(has_errs)
    }

    ///Utilize multi-process to check the correctness of programs concurrently.
    pub fn concurrent_check(
        &self,
        programs: &Vec<PathBuf>,
        core: usize,
    ) -> Result<Vec<Option<ProgramError>>> {
        let mut has_errs = Vec::new();
        let mut batch = Vec::new();
        for (i, program) in programs.iter().enumerate() {
            let i = i + 1;
            batch.push(program.clone());
            if i % core == 0 || i == programs.len() {
                let res = self.concurrent_check_batch(&batch, core)?;
                has_errs.extend(res);
                batch.clear();
            }
        }
        utils::print_san_cost(programs)?;
        Ok(has_errs)
    }

    // Find the new coverage corpus files and merge them in shared corpus.
    fn update_corpus(&self, program_path: &Path) -> Result<()> {
        log::debug!("update new corpora into shared corpus");
        let start = std::time::Instant::now();
        let work_dir = crate::deopt::utils::get_file_dirname(program_path);
        let fuzzer_binary = program_path.with_extension("out");
        let corpus_dir: std::path::PathBuf = [work_dir.clone(), "corpus".into()].iter().collect();
        let shared_corpus = self.deopt.get_library_shared_corpus_dir()?;

        self.minimize_corpus(&fuzzer_binary, &shared_corpus, &corpus_dir)?;
        log_san_cost(start, program_path)?;
        Ok(())
    }

    /// After the first half of converge, performs sanitization on the seeds again. It aims to:
    /// Sanitize the erroneous programs that were ignored due to no suitable fuzzing corpus to trigger the error.
    pub fn recheck_seed(&mut self, deopt: &mut Deopt) -> Result<()> {
        log::info!("Recheck the saved seeds and remove the error programs within them.");
        let succ_seed_dir = self.deopt.get_library_succ_seed_dir()?;
        let succ_seeds = crate::deopt::utils::read_sort_dir(&succ_seed_dir)?;
        for succ_seed in &succ_seeds {
            let seed_program = Program::load_from_path(succ_seed)?;
            let seed_id = seed_program.id;
            self.compile_seed(seed_id)?;
            // recheck the program
            let corpus = self.deopt.get_library_shared_corpus_dir()?;
            let work_seed_path = self.deopt.get_work_seed_by_id(seed_id)?;
            let binary_out = work_seed_path.with_extension("out");
            let has_err = self.execute_pool(&binary_out, &corpus);
            if let Some(err_msg) = has_err {
                log::warn!("seed: {} is rechecked as Error!", seed_id);
                let seed = self.deopt.get_seed_path_by_id(seed_id)?;
                self.deopt.save_err_program(&seed_program, &err_msg)?;
                std::fs::remove_file(succ_seed)?;
                if seed.exists() {
                    std::fs::remove_file(seed)?;
                    deopt.delete_seed_from_queue(&seed_program);
                }
            }
        }
        Ok(())
    }
}

pub mod utils {
    use std::io::Write;

    use eyre::Context;

    use crate::program::serde::Deserializer;

    use super::*;

    pub fn log_san_cost(start: std::time::Instant, program_path: &Path) -> Result<()> {
        let program_dir = get_file_dirname(program_path);
        let log_file: PathBuf = [program_dir, "san.cost".into()].iter().collect();
        let mut file = std::fs::OpenOptions::new()
            .create(true)
            .append(true)
            .open(log_file)?;
        file.write_fmt(format_args!("{}\n", start.elapsed().as_secs()))?;
        Ok(())
    }

    pub fn print_san_cost(program_paths: &Vec<PathBuf>) -> Result<()> {
        let mut syntax = 0;
        let mut link = 0;
        let mut execution = 0;
        let mut fuzzer = 0;
        let mut coverage = 0;
        let mut update = 0;
        for program_path in program_paths {
            let program_dir = get_file_dirname(program_path);
            let log_file: PathBuf = [program_dir, "san.cost".into()].iter().collect();
            if !log_file.exists() {
                continue;
            }
            let content = std::fs::read_to_string(&log_file)
                .context(format!("file {log_file:?} open error."))?;
            let mut de = Deserializer::from_input(&content);
            syntax += de.parse_number::<usize>()?;
            link += de.parse_number::<usize>().unwrap_or_default();
            execution += de.parse_number::<usize>().unwrap_or_default();
            fuzzer += de.parse_number::<usize>().unwrap_or_default();
            coverage += de.parse_number::<usize>().unwrap_or_default();
            update += de.parse_number::<usize>().unwrap_or_default();
        }
        let total = syntax + link + execution + fuzzer + coverage + update;
        log::debug!("Sanitization Time Cost: total: {total}s, syntax: {syntax}s, link: {link}s, exec: {execution}s, fuzz: {fuzzer}s, coverage: {coverage}s, update: {update}s");
        Ok(())
    }

    pub fn cleanup_sanitize_dir(sanitize_dir: &Path) -> Result<()> {
        let files = crate::deopt::utils::read_sort_dir(sanitize_dir)?;
        for file in files {
            if let Some(ext) = file.extension() {
                if ext == "log" || ext == "out" || ext == "cc" || ext == "profdata" || ext == "cost" {
                    continue;
                }
            }
            if file.is_dir() {
                std::fs::remove_dir_all(file)?;
            } else {
                std::fs::remove_file(file)?
            }
        }
        Ok(())
    }
}

#[cfg(test)]
mod tests {

    use super::*;

    #[test]
    fn test_coverage_sanitize() -> Result<()> {
        crate::config::Config::init_test("cJSON");
        let deopt = Deopt::new("cJSON")?;
        let executor = Executor::new(&deopt)?;

        // this should pass the sanitization.
        let cov_succ_program_path: std::path::PathBuf = [
            crate::Deopt::get_crate_dir()?,
            "testsuites",
            "sanitize",
            "cjson_cov_succ.cc",
        ]
        .iter()
        .collect();
        let work_path = deopt.get_work_seed_by_id(99999)?;
        std::fs::copy(cov_succ_program_path, &work_path)?;
        let has_err = executor.check_program_is_correct(&work_path)?;
        //println!("{has_err:#?}");
        assert!(has_err.is_none());

        // this should be sanitized by coverage.
        let cov_fail_program_path: std::path::PathBuf = [
            crate::Deopt::get_crate_dir()?,
            "testsuites",
            "sanitize",
            "cjson_cov_fail.cc",
        ]
        .iter()
        .collect();
        let work_path = deopt.get_work_seed_by_id(888888)?;
        std::fs::copy(cov_fail_program_path, &work_path)?;
        let has_err = executor.check_program_is_correct(&work_path)?;
        assert!(has_err.is_some());
        if let Some(err) = has_err {
            match err {
                ProgramError::Coverage(_) => return Ok(()),
                _ => panic!("Should not fail on other sanitization"),
            }
        }
        Ok(())
    }

    #[test]
    fn test_shared_corpus() -> Result<()> {
        crate::config::Config::init_test("libvpx");
        let deopt = Deopt::new("libvpx")?;
        let executor = Executor::new(&deopt)?;
        // this should pass the sanitization.
        let cov_succ_program_path: std::path::PathBuf = [
            crate::Deopt::get_crate_dir()?,
            "testsuites",
            "sanitize",
            "vpx_cov_succ.cc",
        ]
        .iter()
        .collect();
        let work_path = deopt.get_work_seed_by_id(99999)?;
        std::fs::copy(cov_succ_program_path, &work_path)?;
        let _ = executor.check_program_is_correct(&work_path)?;
        let work_dir = get_file_dirname(&work_path);

        let fuzzer_binary: PathBuf = [work_dir.clone(), "fuzz_driver.cov.out".into()]
            .iter()
            .collect();
        let prev_set =
            crate::deopt::utils::get_file_hash_set(&deopt.get_library_shared_corpus_dir()?);
        let prev_coverage = executor.collect_code_coverage(
            None,
            &fuzzer_binary,
            &deopt.get_library_shared_corpus_dir()?,
        )?;
        let mut observer =
            crate::feedback::observer::Observer::from_coverage(&prev_coverage, &deopt);
        println!("{}", observer.dump_global_states());

        let mut instresting_files = Vec::new();

        let corpus: PathBuf = [work_dir.clone(), "corpus".into()].iter().collect();
        for corpus_file in std::fs::read_dir(corpus)? {
            let file = corpus_file?.path();
            let file_name = file.file_name().unwrap().to_str().unwrap();
            if prev_set.contains(file_name) {
                continue;
            }
            log::debug!("evaluate file: {file:?}");
            let coverage = executor.collect_code_coverage(None, &fuzzer_binary, &file)?;
            if !observer.has_unique_branch(&coverage).is_empty() {
                println!("find instresting file: {file:?}");
                instresting_files.push(file);
            }
            let new_branches = observer.has_new_branch(&coverage);
            observer.merge_new_branch(&new_branches);
        }
        println!("{}", observer.dump_global_states());
        deopt.copy_file_to_shared_corpus(instresting_files)?;
        Ok(())
    }

    #[test]
    fn test_sanitization_for_a_program() -> Result<()> {
        crate::config::Config::init_test("libpng");
        let deopt = Deopt::new("libpng")?;
        let program_path: std::path::PathBuf =
            [crate::Deopt::get_crate_dir()?, "testsuites", "new_test.cc"]
                .iter()
                .collect();
        let work_path = deopt.get_work_seed_by_id(88888)?;
        std::fs::copy(program_path, &work_path)?;
        let executor = Executor::new(&deopt)?;
        let res = executor.check_program_is_correct(&work_path)?;
        println!("{res:?}");
        Ok(())
    }
}
