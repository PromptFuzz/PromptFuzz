#!/bin/bash
## Usage: ./test_hang.sh xxx.cc

mkdir -p test
clang++ -fsanitize=address -I/home/loydlv/vbd/llm_fuzz/data/libaom/include -L/home/loydlv/vbd/llm_fuzz/data/libaom/lib -laom -g $1 -o test/$1.out
CORPUS=/home/loydlv/vbd/llm_fuzz/output/libaom/codec_corpus
export LD_LIBRARY_PATH=/home/loydlv/vbd/llm_fuzz/data/libaom/lib
for ENTRY in $(ls ${CORPUS});do
        if [[ -d ${CORPUS}/$ENTRY ]]; then
                for INNER in $(ls ${CORPUS}/${ENTRY}); do
                        ARG=${CORPUS}/${ENTRY}/${INNER}
                        echo  $ARG 
                        time ./test/$1.out $ARG
                done
                break
        fi
        ARG=${CORPUS}/${ENTRY}
        echo $ARG "\n"
        time ./test/$1.out $ARG

done