#!/usr/bin/env python3
# permrun.py <unit> <sym> <funcname-in-source>  : permutes runs of simple assignments in the function body
import sys, re, itertools, subprocess, os
unit, sym, fname = sys.argv[1:4]
src = '/home/claude/work-D3/src/%s.cpp' % unit
orig = open(src, encoding='latin-1').read()
import signal
def _restore(*a):
    open(src, 'w', encoding='latin-1').write(orig); sys.exit(1)
signal.signal(signal.SIGTERM, _restore)
lines = orig.split('\n')
start = next(i for i, l in enumerate(lines) if (fname + '(') in l and not l.startswith(' ') and not l.rstrip().endswith(';'))
while not lines[start].rstrip().endswith('{'): start += 1
end = next(i for i in range(start, len(lines)) if lines[i] == '}')
pat = re.compile(r'^(\s+)[\w\.\->\[\]\(\)\*]+ *(=|\+=|-=|\*=|/=|\|=|&=) [^;{}]*;$')
runs = []; i = start + 1
while i < end:
    m = pat.match(lines[i])
    if m:
        ind = m.group(1); j = i
        while j < end and pat.match(lines[j]) and pat.match(lines[j]).group(1) == ind: j += 1
        if j - i >= 2: runs.append((i, j))
        i = j
    else: i += 1
def pct():
    subprocess.run(['rm', '-f', '/home/claude/work-D3/build/SLSP01/src/%s.o' % unit])
    r = subprocess.run(['bash', '-c', 'cd %s && timeout 300 ninja -j1 build/SLSP01/src/%s.o >/dev/null 2>&1 && tools/decomp/pct.sh %s %s' % (os.getcwd(), unit, unit, sym)], capture_output=True, text=True).stdout.split()
    return float(r[0]) if r else -1
base = pct(); print('base', base, 'runs', runs); best = (base, None)
for (a, b) in runs:
    blk = lines[a:b]
    if len(blk) > 5: continue
    for p in itertools.permutations(range(len(blk))):
        if list(p) == list(range(len(blk))): continue
        new = lines[:a] + [blk[k] for k in p] + lines[b:]
        open(src, 'w', encoding='latin-1').write('\n'.join(new))
        v = pct()
        if v > best[0]: best = (v, (a, p)); print('  better', v, a + 1, p, flush=True)
open(src, 'w', encoding='latin-1').write(orig)
print('best', best)
