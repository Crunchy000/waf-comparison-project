#!/bin/sh
# Compile game.c to game.wasm (needs clang + wasm-ld, python3) and check the 512-byte budget.
set -e
cd "$(dirname "$0")"
clang --target=wasm32 -Oz -ffast-math -nostdlib -fno-builtin -mno-bulk-memory \
  -Wl,--no-entry -Wl,--strip-all -Wl,--compress-relocations -Wl,-z,stack-size=0 \
  -Wl,--global-base=16 -Wl,--initial-memory=131072 -o game.wasm game.c
python3 strip.py game.wasm
size=$(wc -c < game.wasm)
echo "game.wasm: $size bytes"
[ "$size" -le 512 ] || { echo "over 512-byte budget"; exit 1; }
