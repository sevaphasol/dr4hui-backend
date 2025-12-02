#!/bin/bash

cmake -DCMAKE_INSTALL_PREFIX=install -DBUILD_EXAMPLES=OFF -B build
cmake --build build -j$(nproc)
cmake --install build --prefix install
ln -sf build/compile_commands.json
