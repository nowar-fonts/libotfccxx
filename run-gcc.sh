#!/bin/bash

set -e

cmake . -B build-gcc/ \
	-DCMAKE_EXPORT_COMPILE_COMMANDS=1 \
	-DCMAKE_BUILD_TYPE="Debug" \
	-DCMAKE_C_COMPILER="gcc" \
	-DCMAKE_C_FLAGS="-Wall -Wextra -Wpedantic" \
	-DCMAKE_CXX_COMPILER="g++" \
	-DCMAKE_CXX_FLAGS="-Wall -Wextra -Wpedantic"

cmake --build build-gcc/ -j$(nproc)

ln -sf build-clang/compile_commands.json .

echo "============================================================================"
echo "============================================================================"

build-gcc/main
