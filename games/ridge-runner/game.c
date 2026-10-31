// Ridge Runner: tiny two-wheel hill-climb physics, compiled to WebAssembly.
// State at byte 64 of memory (float[8]), written by JS at start:
//   rear x,y  front x,y  rear prev x,y  front prev x,y
#define st ((float *)64)
__attribute__((import_name("s"))) float S(float);

// Terrain height; hills grow taller the further you go.
__attribute__((export_name("h"))) float ground(float x) {
  float a = 15 + x * .01f;
  return a * S(x * .009f) + a * .5f * S(x * .025f);
}

// One Verlet step for one wheel. (ex, ey) is the gas-driven push used to tilt the bike in the air.
static int wheel(float *p, float ex, float ey, float gas) {
  float vx = p[0] - p[4], vy = p[1] - p[5] - .15f;
  p[4] = p[0]; p[5] = p[1];
  p[0] += vx + ex; p[1] += vy + ey;
  float g = ground(p[0]) + 6;
  if (p[1] >= g) return 0;
  p[1] = g;                       // on the ground: push out and drive forward
  p[4] = p[0] - vx * .9f - gas * .2f;
  return 1;
}

// Advance one frame with throttle gas (-1..1). Returns how many wheels touch the ground.
__attribute__((export_name("t"))) int step(float gas) {
  float k = gas * .0005f, dx = (st[2] - st[0]) * k, dy = (st[3] - st[1]) * k;
  int hit = wheel(st, dy, -dx, gas) + wheel(st + 2, -dy, dx, gas);
  dx = st[2] - st[0]; dy = st[3] - st[1];   // keep wheels 40 apart
  float d = dx * dx + dy * dy, r = (800 - d * .5f) / (d + 1600);
  st[0] -= dx * r; st[1] -= dy * r; st[2] += dx * r; st[3] += dy * r;
  return hit;
}
