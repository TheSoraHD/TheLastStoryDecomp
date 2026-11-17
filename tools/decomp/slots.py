#!/usr/bin/env python3
# slots.py <unit> <sym>: pares de desplazamientos de pila orig -> nuestro (de odiff2), agrupados
import subprocess, sys, re, collections
out = subprocess.run(['python3', 'tools/decomp/odiff2.py', sys.argv[1], sys.argv[2]], capture_output=True, text=True, cwd=__import__('os').environ.get('CMP_ROOT','.')).stdout
pairs = collections.Counter()
for l in out.split('\n'):
    if '|' not in l: continue
    a, b = l.split('|', 1)
    ra = re.findall(r'r1, (0x[0-9a-f]+)|(0x[0-9a-f]+)\(r1\)', a)
    rb = re.findall(r'r1, (0x[0-9a-f]+)|(0x[0-9a-f]+)\(r1\)', b)
    if ra and rb:
        x = int(ra[0][0] or ra[0][1], 16); y = int(rb[0][0] or rb[0][1], 16)
        if x != y: pairs[(x, y)] += 1
for (x, y), n in sorted(pairs.items(), reverse=True):
    print('orig 0x%x -> ours 0x%x  (%d)' % (x, y, n))
