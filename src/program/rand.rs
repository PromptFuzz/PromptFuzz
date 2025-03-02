use eyre::Context;
use once_cell::sync::Lazy;
use rand::rngs::StdRng;
use rand::{
    distributions::WeightedIndex, prelude::Distribution, seq::SliceRandom, Rng, SeedableRng,
};
use std::sync::Mutex;

// Initialize the global random number generator (RNG)
pub static GLOBAL_RNG: Lazy<Mutex<StdRng>> = Lazy::new(|| {
    let seed = rand::random();
    Mutex::new(StdRng::seed_from_u64(seed))
});

pub fn get_global_rng() -> std::sync::MutexGuard<'static, StdRng> {
    GLOBAL_RNG.lock().unwrap()
}

pub fn random_select<T>(seq: &[T]) -> &T {
    let len = seq.len();
    let rand_num: usize = get_global_rng().gen();
    let rand_num: usize = rand_num % len;
    &seq[rand_num]
}

pub fn random_sample<T: Clone>(seq: &[T], n: usize) -> Vec<&T> {
    if seq.len() <= n {
        let sample: Vec<&T> = seq.iter().collect();
        return sample;
    }
    let mut rng = get_global_rng();
    let sample: Vec<&T> = seq.choose_multiple(&mut *rng, n).collect();
    sample
}

pub fn weighted_choose(weights: Vec<f32>) -> usize {
    if weights.iter().all(|x| *x == 0.0_f32) {
        return 0;
    }
    let dist = WeightedIndex::new(&weights)
        .context(format!("InvalidWeight: {weights:?}"))
        .unwrap();
    let mut rng = get_global_rng();
    dist.sample(&mut *rng)
}

pub fn prob_coin(prob: f32) -> bool {
    if prob == 0.0_f32 {
        return false;
    }
    assert!(prob < 1.0, "{prob} should < 1.0");
    assert!(prob > 0.0, "{prob} should > 0.0");
    let random: f32 = get_global_rng().gen_range(0.0..1.0);
    random < prob
}

pub fn rand_comb_len() -> usize {
    let lens: Vec<usize> = (crate::config::DEFAULT_COMB_LEN..10).collect();
    let select_len = random_select(&lens);
    *select_len
}
