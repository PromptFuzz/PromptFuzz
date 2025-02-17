use std::path::PathBuf;

use crate::{deopt::Deopt, feedback::observer::Observer, program::Program};
use eyre::Result;

/// Minimize seed programs by unique branches
pub fn minimize(deopt: &Deopt) -> Result<()> {
    let seeds_dir = deopt.get_library_succ_seed_dir()?;
    // first sort seeds by coverge.
    let mut program_coverage: Vec<(PathBuf, f32)> = Vec::new();
    for file in crate::deopt::utils::read_sort_dir(&seeds_dir)? {
        let program = Program::load_from_path(&file)?;
        let coverage_rate = Observer::compute_coverage_for_program(program.id, deopt)?;
        program_coverage.push((file, coverage_rate));
    }
    program_coverage.sort_by(|a, b| b.1.partial_cmp(&a.1).unwrap());

    // iterate the sorted seeds, only the seeds still triger unique branch(s) are retained.
    let mut observer = Observer::new(deopt);
    for (program_path, _) in program_coverage {
        let program = Program::load_from_path(&program_path)?;
        let seed = deopt.get_seed_path_by_id(program.id)?;
        let coverage = deopt.get_seed_coverage(program.id)?;
        let unique_branches = observer.has_unique_branch(&coverage);
        if unique_branches.is_empty() {
            if seed.exists() {
                log::info!(
                    "Program Seed triggers no unique branch and has been removed: {program_path:?}"
                );
                std::fs::remove_file(seed)?;
            }
            continue;
        }
        log::info!("{program_path:?} is an unique seed");
        if !seed.exists() {
            std::fs::copy(program_path, seed)?;
        }
        let new_exp_branches = observer.has_new_branch(&coverage);
        observer.merge_new_branch(&new_exp_branches);
    }

    log::info!("{}", observer.dump_global_states());
    Ok(())
}
