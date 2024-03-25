#!/bin/bash

echo "Generating bitcode for test program"
cd test-program
clang++ -emit-llvm test.cpp -c -fPIC -fno-builtin -o test.bc

cd ../
echo "Building pass"
mkdir -p build && cd build
cd build
cmake ..
make
echo "Running pass on test program"
opt -load-pass-plugin=AddToSubPass/LLVMAddToSubPass.so -passes=add-to-sub-pass < ../test-program/test.bc > output.bc
clang++ -S -g ../test-program/test.bc -o test.s
clang++ -S -g output.bc -o output.s

diff output.s test.s
