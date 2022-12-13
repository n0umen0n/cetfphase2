#!/bin/bash

echo "compiling..."
eosio-cpp -abigen -I ./include -contract cetf -o cetf.wasm src/cetf.cpp 
shasum -a 256 cetf.wasm


