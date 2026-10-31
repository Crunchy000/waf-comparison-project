#!/bin/sh
# Compile game.c to game.wasm (needs clang + wasm-ld) and check the 512-byte budget.
set -e
cd "$(dirname "$0")"
clang --target=wasm32 -Oz -ffast-math -nostdlib -fno-builtin -mno-bulk-memory \
  -Wl,--no-entry -Wl,--strip-all -Wl,-z,stack-size=0 -Wl,--initial-memory=65536 \
  -o game.wasm game.c
size=$(wc -c < game.wasm)
echo "game.wasm: $size bytes"
[ "$size" -lt 512 ] || { echo "over 512-byte budget"; exit 1; }
