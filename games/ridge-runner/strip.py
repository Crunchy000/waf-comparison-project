# Drop the table and global sections that wasm-ld always emits but this game never uses.
import sys
d = open(sys.argv[1], 'rb').read(); out = d[:8]; i = 8
while i < len(d):
    t = d[i]; j = i + 1; n = s = 0
    while True:
        b = d[j]; j += 1; n |= (b & 127) << s; s += 7
        if b < 128: break
    if t not in (4, 6): out += d[i:j + n]
    i = j + n
open(sys.argv[1], 'wb').write(out)
