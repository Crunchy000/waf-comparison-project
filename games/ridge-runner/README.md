# Ridge Runner

A small original hill-climb driving game. The physics (terrain, two Verlet wheels, a rigid
frame, ground contact, throttle and mid-air tilt) lives in `game.c`, compiled to a
WebAssembly module under 512 bytes. `index.html` is the front end: rendering, input,
fuel, crash detection and score.

## Build

    ./build.sh        # needs clang 15+ with the wasm32 target and wasm-ld

## Play

Serve the folder (WebAssembly can't load from `file://`) and open it:

    python3 -m http.server -d games/ridge-runner 8000

Right arrow / D or the GAS button drives forward and tilts the bike back in the air.
Left arrow / A or BRAKE reverses and tilts it forward. R restarts. Grab red fuel cans
to keep going, and don't land on your head.

## WebAssembly interface

| Export | Meaning |
| --- | --- |
| `memory` | 8 floats at byte 64: rear x,y, front x,y, then their previous positions |
| `t(gas)` | advance one physics step, gas in -1..1; returns wheels on the ground |
| `h(x)` | terrain height at x |

Import: `env.s` = `Math.sin`.
