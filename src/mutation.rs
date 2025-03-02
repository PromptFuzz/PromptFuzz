use std::usize;

use rand::Rng;

use crate::{
    config::DEFAULT_COMB_LEN,
    deopt::Deopt,
    feedback::schedule::{rand_choose_combination, Schedule},
    program::{
        gadget::get_func_gadget,
        rand::{get_global_rng, random_select},
    },
    request::prompt::Prompt,
};

#[derive(Clone, Debug, PartialEq)]
pub enum MutationOperator {
    // Add an API in prompt
    Add,
    // Replace an API in prompt
    Replace,
    // Change several APIs with another prompt
    CrossOver,
}

pub fn prompt_shuffle(prompt: &mut Prompt) {
    log::info!("Shuffle the prompt.");
    let combination = rand_choose_combination(crate::program::rand::rand_comb_len());
    prompt.set_combination(combination);
}

/// Insert an high energy API into combination
fn prompt_insertion(prompt: Vec<String>, schedule: &Schedule) -> Vec<String> {
    let mut combination = prompt;
    let mut insert_num = 0;
    loop {
        let choose_api = schedule.choose_api_by_energy().to_string();
        if combination.contains(&choose_api) {
            continue;
        }
        let ins_idx: usize = get_global_rng().gen::<usize>() % combination.len();
        log::info!("Insert {choose_api} into prompt.");
        combination.insert(ins_idx, choose_api);
        insert_num += 1;
        if insert_num >= 3 {
            break;
        }
    }
    combination
}

fn prompt_energy_deletion(prompt: Vec<String>, schedule: &Schedule) -> Vec<String> {
    let mut combination = prompt;
    if combination.len() < DEFAULT_COMB_LEN {
        let mut should_infill = DEFAULT_COMB_LEN - combination.len();
        while should_infill > 0 {
            let api = schedule.choose_api_by_energy().to_string();
            combination.push(api);
            should_infill -= 1;
        }
    }
    let should_delete = 3;
    let mut deleted = 0;
    while deleted < should_delete {
        let delete_idx = schedule.choose_low_energy_api(&combination);
        let deleted_api = combination.remove(delete_idx);
        log::info!("Delete low energy {} from prompt.", deleted_api);
        deleted += 1;
    }
    combination
}

/// Replace an uncovered API with a high energy API
fn prompt_replacement(prompt: Vec<String>, schedule: &Schedule) -> Vec<String> {
    let prompt = prompt_energy_deletion(prompt, schedule);
    prompt_insertion(prompt, schedule)
}

fn crossover(
    prompt1: Vec<String>,
    prompt2: Vec<String>,
    index1: usize,
    index2: usize,
    len: usize,
) -> Vec<String> {
    let mut combination = Vec::new();
    let first_part = &prompt1[0..index1];
    combination.extend_from_slice(first_part);
    let second_part = &prompt2[index2..(index2 + len)];
    combination.extend_from_slice(second_part);
    let third_part = &prompt1[(index1 + len)..];
    combination.extend_from_slice(third_part);
    combination
}

/// Retain the uncovered APIs, and replace with covered APIs with those chosed from High Quality Prompt.
fn prompt_crossover(prompt1: Vec<String>, prompt2: Vec<String>) -> Vec<String> {
    log::info!("Crossover {prompt1:#?} with {prompt2:#?}");
    if prompt1.len() >= prompt2.len() {
        // crossover with lentgh of 3
        if prompt1.len() <= 3 {
            return [prompt1, prompt2].concat();
        }
        if prompt2.len() <= 3 {
            let max_index = prompt1.len() - prompt2.len();
            let prompt2_len = prompt2.len();
            let index: usize = get_global_rng().gen::<usize>() % max_index;
            crossover(prompt1, prompt2, index, 0, prompt2_len)
        } else {
            let max_index1 = prompt1.len() - 3;
            let max_index2 = prompt2.len() - 3;
            let index1 = get_global_rng().gen::<usize>() % max_index1;
            let index2 = get_global_rng().gen::<usize>() % max_index2;
            crossover(prompt1, prompt2, index1, index2, 3)
        }
    } else {
        prompt_crossover(prompt2, prompt1)
    }
}

pub fn mutate_prompt(prompt: &mut Prompt, schedule: &Schedule, deopt: &mut Deopt) {
    let mutators = vec![
        MutationOperator::Add,
        MutationOperator::Replace,
        MutationOperator::CrossOver,
    ];
    let mutator = random_select(&mutators);
    let seed = deopt.select_seed_from_queue().clone();
    let prompt_comb = seed.convert_to_prompt();
    log::info!("Select seed: {}, comb: {:?}", seed.id, prompt_comb);

    if prompt_comb.is_empty() {
        let combination = schedule.assemble_high_energy_combiantion();
        prompt.set_combination(combination);
        return;
    }

    let combination = match mutator {
        MutationOperator::Add => {
            log::info!("[Mutation Insert]: select seed {}", seed.id);
            prompt_insertion(prompt_comb, schedule)
        }
        MutationOperator::Replace => {
            log::info!("[Mutation Replace]: select seed {}", seed.id);
            prompt_replacement(prompt_comb, schedule)
        }
        MutationOperator::CrossOver => {
            let seed2 = deopt.select_seed_exclude(&seed);
            let prompt_comb2 = seed2.convert_to_prompt();
            log::info!(
                "[Mutation CrossOver] select seed1: {}, seed2: {}",
                seed.id,
                seed2.id
            );
            prompt_crossover(prompt_comb, prompt_comb2)
        }
    };
    let combination = combination
        .iter()
        .map(|api| get_func_gadget(api).unwrap())
        .collect();
    prompt.set_combination(combination);
}

#[test]
fn test_seed_to_prompt() -> eyre::Result<()> {
    crate::config::Config::init_test("cJSON");
    let mut deopt = Deopt::new("cJSON".to_string())?;
    deopt.load_programs_from_seeds()?;
    let seed_id = 24;
    let seed = deopt.get_seed_from_queue(seed_id);
    let critical_calls = seed.convert_to_prompt();
    println!("{critical_calls:?}");
    Ok(())
}
