#!/bin/bash

set -e

cmake . -B build-clang/ \
	-DCMAKE_EXPORT_COMPILE_COMMANDS=1 \
	-DCMAKE_BUILD_TYPE="Debug" \
	-DCMAKE_C_COMPILER="clang" \
	-DCMAKE_C_FLAGS="-Wall -Wextra -Wpedantic -stdlib=libc++" \
	-DCMAKE_CXX_COMPILER="clang++" \
	-DCMAKE_CXX_FLAGS="-Wall -Wextra -Wpedantic -stdlib=libc++"

cmake --build build-clang/ -j$(nproc)

ln -sf build-clang/compile_commands.json .

echo "============================================================================"
echo "============================================================================"

build-clang/main
