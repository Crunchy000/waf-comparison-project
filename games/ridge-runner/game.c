// Ridge Runner: the whole game (physics, crashes, restart and drawing) in WebAssembly.
// Each call to f(gas) runs one frame (gas: 1 = gas, -1 = brake, 0 = coast) and
// leaves a 160x96 RGBA image at byte 4096 of memory for the page to show.

static float V, U, X, Y, A;   // vertical speed, forward speed, position x/y, tilt (as a slope)

// Rolling hills that start flat and grow taller the further you drive.
// t*(.5-|t|) is a cheap sine-like wave, so no maths library is needed.
__attribute__((noinline)) static float ground(float x) {
  float t = x * .002f;
  t -= __builtin_rintf(t);
  return (99 + x * .25f) * t * (.5f - __builtin_fabsf(t));
}

__attribute__((export_name("f"))) void frame(float gas) {
  V -= .1f; X += U; Y += V;
  A += gas * .04f;                                  // gas tips the nose up, brake tips it down
  float g = ground(X), s = ground(X + 1) - g;       // ground height and slope under the car
  if (Y <= g) {                                     // touching the ground
    if (__builtin_fabsf(A - s) > .8f) X = U = 0;    // landed at a bad angle: back to the start
    Y = g; A = s;
    U = (U + (gas + gas - s) * .03f) * .99f;            // drive, minus the pull of the slope and drag
    V = U * s;
  }
  unsigned *px = (unsigned *)4096;
  for (int j = 0; j < 160 * 96; j++) {
    float dx = (j % 160 - 60) * 2, dy = (48 - j / 160) * 2;
    float a = dx + A * dy, b = dy - A * dx;         // pixel in the car's tilted frame
    px[j] = __builtin_fabsf(a) < 16 && __builtin_fabsf(b - 7) < 7 ? b < 4 ? 0xff222222 : 0xff41a5f2  // car: dark wheels, orange body
          : Y + dy < ground(X + dx) ? 0xff2f7a4f                                  // grass
          : 0xffd08a50;                                                           // sky
  }
}
