#!/bin/bash

set -e

cmake . -B build/ \
	-DCMAKE_EXPORT_COMPILE_COMMANDS=1 \
	-DCMAKE_BUILD_TYPE="RelWithDebugInfo" \
	-DCMAKE_C_FLAGS="-Wall -Wextra" \
	-DCMAKE_CXX_FLAGS="-Wall -Wextra"

cmake --build build/ -j$(nproc)

ln -sf build/compile_commands.json .

echo "============================================================================"
echo "============================================================================"

build/main
