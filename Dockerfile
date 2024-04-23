FROM ubuntu:22.04

ENV PATH=/lib/llvm-15/bin:/usr/local/cargo/bin:/root/.cargo/bin:$PATH \ 
    LD_LIBRARY_PATH=/lib/llvm-15/lib:$LD_LIBRARY_PATH \
    RUSTUP_HOME=/usr/local/rustup \
    CARGO_HOME=/usr/local/cargo \
    DEBIAN_FRONTEND=noninteractive \
    DOCKER_CONTAINER=1

RUN apt-get update \
    && apt-get -y install build-essential wget curl cmake git unzip patchelf graphviz python3 python3-pip lsb-release software-properties-common gnupg file libssl-dev openssl pkg-config libfontconfig libfontconfig1-dev zip \
    && apt-get clean \
    && pip3 install wllvm

# build llvm and clang dependency
RUN wget https://apt.llvm.org/llvm.sh \
    && chmod +x llvm.sh \
    && ./llvm.sh 15 \
    && ln -s /usr/bin/clang-15 /usr/bin/clang \
    && ln -s /usr/bin/clang++-15 /usr/bin/clang++

RUN curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y --default-toolchain stable && rustup default stable



RUN mkdir -p /prompt_fuzz
ADD ./data/ /prompt_fuzz/data
ADD ./src/ /prompt_fuzz/src
ADD ./testsuites /prompt_fuzz/testsuites
ADD Cargo.lock Cargo.toml /prompt_fuzz/
ADD ban_all.txt /prompt_fuzz/ban_all.txt
WORKDIR /prompt_fuzz