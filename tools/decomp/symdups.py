#!/usr/bin/env python3
"""symdups.py <backup symbols.txt>: restores from the backup every line of config/SLSP01/symbols.txt whose
name now appears twice (pairdata --relabel can give a .sdata label the @STRING@ name of another address,
which breaks the link with "multiply-defined"; accesores-20261126)."""
import sys, re, collections
p = 'config/SLSP01/symbols.txt'
cur = open(p).read().split('\n'); bak = open(sys.argv[1]).read().split('\n')
name = lambda l: l.split(' = ', 1)[0] if ' = ' in l else None
cnt = collections.Counter(name(l) for l in cur if name(l))
baddr = {}
for l in bak:
    m = re.search(r' = (\.\w+:0x[0-9A-F]+);', l)
    if m: baddr[m.group(1)] = l
n = 0
for i, l in enumerate(cur):
    nm = name(l)
    if nm and cnt[nm] > 1 and 'scope:local' not in l and not re.match(r'@\d+$', nm):
        m = re.search(r' = (\.\w+:0x[0-9A-F]+);', l)
        if m and m.group(1) in baddr and baddr[m.group(1)] != l:
            cur[i] = baddr[m.group(1)]; n += 1
open(p, 'w').write('\n'.join(cur))
print('symdups: restored', n)
