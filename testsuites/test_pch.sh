#!/bin/sh

clang -x c-header -I../data/cJSON/include test.h -o test.h.pch

function with_pch() {
    number=0
    while [ "$number" -lt 1000 ]; do
    clang -fsyntax-only -include-pch test.h.pch\
         -I../data/cJSON/include -Xclang -ast-dump test.c >/dev/null
    number=$((number + 1))
    done
}

function without_pch() {
    number=0
    while [ "$number" -lt 1000 ]; do
    clang -fsyntax-only -I../data/cJSON/include -Xclang -ast-dump -Xclang -ast-dump-filter=main test.c > /dev/null
    number=$((number + 1))
    done
}

echo "with_pch:"
time with_pch
echo "without_pch:"
time without_pch

# experimently, using pch is 2x faster than without using pch.
