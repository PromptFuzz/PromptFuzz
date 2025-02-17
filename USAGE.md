# Usage
## 🔧 Build Pre-requisites

### 1. 🐳Using Docker (Recommend)
You can use the [Dockerfile](Docekrfile) to build the environment:
```
docker build -t promptfuzz .
docker run -it promptfuzz bash
```

### 2. Library build scripts
Before you apply this fuzzer for a new project, you **must** have a automatic build script to build your project to prepare the required data (e.g., headers, link libraries, fuzzing corpus and etc.), like [OSS-Fuzz](https://github.com/google/oss-fuzz). See [Preparation](data/README.md).


We have prepared the build scripts for some popular open source libraries, you can refer to the **data** directory.



### 3. Build Environment Locally (Optional)
If you prefer to set up the environment locally instead of using Docker, you can follow the instructions below:

**Requirements:**
- Rust stable
- LLVM and Clang (built with compiler-rt)
- wllvm (installed by `pip3 install wllvm`)

You can download llvm and clang from this [link](https://github.com/llvm/llvm-project/releases/tag/llvmorg-15.0.0) or install by [llvm.sh](https://apt.llvm.org/).

Explicit dependency see [Dockerfile](Dockerfile).

If you prefer build llvm manually, you can build clang with compiler-rt and libcxx from source code following the config:
```
cmake -S llvm -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_PROJECTS="clang;lld" \
 -DLLVM_ENABLE_RUNTIMES="libcxx;libcxxabi;compiler-rt;" \
 -DCMAKE_BUILD_TYPE=Release -DLIBCXX_ENABLE_STATIC_ABI_LIBRARY=ON \
 -DLIBCXXABI_ENABLE_SHARED=OFF  -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ 
 ```

### 4. LLM dependency (Optional)
> Those custom LLMs have not been fully supported and tested in PromptFuzz. If you just want to use PromptFuzz, please choose ChatGPT or GPT4.

Currently, the primary programming language used for implementation is **Rust**, while a few **Python** scripts are utilized to invoke specific LLM models.

If you want to invoke the self-build LLMs (i.e., Incoder), the following is the requirements for building **Python** dependency:
```
- pytorch (pip3 install torch)
- transformers (pip3 install transformers)
- yaml (pip3 install PyYAML)
- fastapi (pip3 install fastapi[all])
```

## 🦄Basic Usage

### 1. Build library
Run the script in the **prompt_fuzz/data** directory, to prepare the required data of this library.

After the build process is finished, the data of this library is stored under **prompt_fuzz/output/build/**.

### 2. Export the LLM API KEY

You must have an `OPENAI_API_KEY` in advance if you choice ChatGPT and GPT4. If you don't have that key, apply it from [OpenAI](https://openai.com/api/) in advance.
```
user@ubuntu$ export OPENAI_API_KEY=$(your_key)
```

### 3. Export the OpenAI Proxy Base (Optional)
If you encounter difficulties in accessing the OPENAI service from your IP location, you can utilize a proxy to redirect your requests as follows:
```
user@ubuntu$ export OPENAI_PROXY_BASE=https://openai.proxy.com/v1
```

Here, `openai.proxy.com` should be the location of your personal openai service proxy.

### 4. Generate Fuzz drivers

PromptFuzz generates fuzz drivers in a fuzz loop. There are several options that can be tuned in the configuration of promptfuzz.

Typically, the only options that need to be actively set are `-c` and `-r`. The `-c` option determines the number of cores to be used for sanitization. Enabling the `-r` option will periodically re-check the correctness of the seed programs, reducing false positives but also introducing some extra overhead.

For instance, the following command is sufficient to perform fuzzing on libaom:
```
cargo run --bin fuzzer -- libaom -c $(nproc) -r
```

The detailed configurations of promptfuzz:

```
user@ubuntu$ cargo run --bin fuzzer -- --help
```

### 5. Run fuzz drivers
Once the fuzz drivers generated finish, you should follow the follow steps to run the fuzz drivers and detect bugs.

Take libaom is an example, you can run this command to fuse the programs into a fuzz driver that can be fuzzed:

`cargo run --bin harness -- libaom fuse-fuzzer`

And, you can execute the fuzzers you fused:

`cargo run --bin harness -- libaom fuzzer-run`

> Note that, promptfuzz implements the mechanism to detect the crashed program inside the fused fuzz driver.
 If a crash of a program has detected, promptfuzz will disable the code of the crashed program, which enables an continuously fuzzing. So, ensure that executing the fuzz drivers in PromptFuzz.

After 24 hours execution(s), you should deduplicate the reported crashes by PromptFuzz:

`cargo run --bin harness -- libaom sanitize-crash`


Then, you can collect and verbose the code coverage of your fuzzers by:

`cargo run --bin harness -- libaom coverage collect`

and 

`cargo run --bin harness -- libaom coverage report`


## 🎈Advance Usage
We also provide a harness named `harness` to facilitate you access some core components of PromptFuzz.

Here is the command input of `harness`:
```
#[derive(Subcommand, Debug)]
enum Commands {
    /// check a program whether is correct.
    Check { program: PathBuf },
    /// Recheck the seeds whether are correct.
    ReCheck,
    /// transform a program to a fuzzer.
    Transform {
        program: PathBuf,
        #[arg(short, default_value = "true")]
        use_cons: bool,
        /// corpora used to perform transform check
        #[arg(short = 'p', default_value = "None")]
        corpora: Option<PathBuf>,
    },
    /// Fuse the programs in seeds to fuzzers.
    FuseFuzzer {
        /// transform fuzzer with constraints
        #[arg(short, default_value = "true")]
        use_cons: bool,
        /// the number of condensed fuzzer you want to fuse
        #[arg(short, default_value = "1")]
        n_fuzzer: usize,
        /// the count of cpu cores you could use
        #[arg(short, default_value = "10")]
        cpu_cores: usize,
        seed_dir: Option<PathBuf>,
    },
    /// Run a synthesized fuzzer in the fuzz dir.
    FuzzerRun {
        /// which fuzzer you want to run. default is "output/$Library/fuzzers"
        #[arg(short = 'u', default_value = "true")]
        use_cons: bool,
        /// the amount of time you wish your fuzzer to run. The default is 86400s (24 hours), the unit is second. 0 is for unlimit.
        time_limit: Option<u64>,
        /// whether minimize the fuzzing corpus before running
        minimize: Option<bool>,
    },
    /// collect code coverage
    Coverage {
        /// Coverage kind to collect
        kind: CoverageKind,
        /// -u means the exploit fuzzers
        #[arg(short = 'u', default_value = "true")]
        exploit: bool,
    },
    Compile {
        kind: Compile,
        #[arg(short = 'u', default_value = "true")]
        exploit: bool,
    },
    /// infer constraints
    Infer,
    /// Minimize the seeds by unique branches.
    Minimize,
    /// Sanitize duplicate and spurious crashes
    SanitizeCrash {
        #[arg(short = 'u', default_value = "true")]
        exploit: bool,
    },
    /// archive the results
    Archive { suffix: Option<String> },
    ///  Build ADG from seeds
    Adg {
        /// ADG kind to build: sparse or dense
        kind: ADGKind,
        /// The path of target programs to build the ADG.
        target: Option<PathBuf>,
    },
}

```