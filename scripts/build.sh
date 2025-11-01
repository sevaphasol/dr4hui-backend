#!/bin/bash

mkdir -p build

pushd build
cmake ..
make -j$(nproc) install
popd

ln -sf build/compile_commands.json
