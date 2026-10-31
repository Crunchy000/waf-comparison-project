# Ridge Runner

A small original hill-climb driving game where the whole game lives in a WebAssembly
module of at most 512 bytes: terrain, driving physics, tilting in the air, crash and
restart, and drawing every pixel. `index.html` is only a wrapper that loads the module,
passes the keys in and copies each finished frame to a canvas.

## Build

    ./build.sh        # needs clang 15+ (wasm32 target), wasm-ld and python3

`build.sh` compiles `game.c`, then `strip.py` removes two empty sections the linker
always adds, and the script fails if `game.wasm` is over 512 bytes.

## Play

Serve the folder (WebAssembly can't load from `file://`) and open it:

    python3 -m http.server -d games/ridge-runner 8000

Hold Right arrow (or the right half of the screen) for gas, Left arrow (left half) to
brake. In the air, gas tips the nose up and brake tips it down. Land at an angle that
doesn't match the ground and you start over. The hills keep getting taller.

## WebAssembly interface

| Export | Meaning |
| --- | --- |
| `f(gas)` | run one frame; gas is 1, -1 or 0 |
| `memory` | after `f`, a 160x96 RGBA image at byte 4096 |

No imports.
