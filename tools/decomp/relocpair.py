#!/usr/bin/env python3
# (s12h-20261108) relocpair.py [--apply] [unit...]: pairs data labels through the relocations of the functions
# both objects have: when instruction i of function F refers to label X in the original and to label Y in our
# .o (X absent from our .o, Y absent from symbols.txt), X is renamed to Y. Only consistent one-to-one mappings
# are applied (local @N labels only: lbl_ names can be referenced by other units). Solves the labels pd.sh leaves out because the value is ambiguous (two 0.0f in the pool).
import json, os, re, subprocess, sys, collections
R = os.path.dirname(os.path.abspath(__file__)) + '/../../'
OBJ = R + 'build/binutils/powerpc-eabi-objdump'
SYM = R + 'config/SLSP01/symbols.txt'

def relocs(o):
    out = subprocess.run([OBJ, '-dr', '--no-show-raw-insn', o], capture_output=True, text=True).stdout
    fs = {}; cur = None; idx = 0
    for l in out.splitlines():
        m = re.match(r'^[0-9a-f]+ <(.+)>:$', l)
        if m:
            cur = m.group(1); fs[cur] = []; idx = 0; continue
        if cur is None: continue
        if 'R_PPC' in l:
            p = l.split()
            fs[cur].append((idx, p[-1].split('+')[0]))
        elif '\t' in l:
            idx += 1
    return fs

def syms(o):
    out = subprocess.run([R + 'build/binutils/powerpc-eabi-nm', o], capture_output=True, text=True).stdout
    return set(l.split()[-1] for l in out.splitlines() if l.strip())

apply = '--apply' in sys.argv
funcs = '--funcs' in sys.argv  # pair unnamed functions (fn_XXXXXXXX) with our named weak copies instead
units = [a for a in sys.argv[1:] if not a.startswith('--')]
d = json.load(open(R + 'build/SLSP01/report.json'))
symtxt = open(SYM).read().split('\n')
symnames = set(l.split(' = ')[0] for l in symtxt if ' = ' in l)
renames = {}
for u in d['units']:
    un = u['name'][5:]
    if units and un not in units: continue
    a = R + 'build/SLSP01/obj/' + un + '.o'; b = R + 'build/SLSP01/src/' + un + '.o'
    if not os.path.exists(a) or not os.path.exists(b): continue
    ra = relocs(a); rb = relocs(b); sb = syms(b)
    m = collections.defaultdict(set)
    for f, la in ra.items():
        lb = rb.get(f)
        if not lb or len(lb) != len(la): continue
        for (ia, xa), (ib, xb) in zip(la, lb):
            if ia != ib: break
            if xa == xb: continue
            if funcs:
                if not re.match(r'^fn_[0-9A-F]{8}$', xa) or xa in sb or xb in symnames or xb.startswith('@'): continue
            else:
                if not re.match(r'^@\d+$', xa): continue
                if not re.match(r'^@\d+$', xb) or xa in sb or xb in symnames: continue
            m[xa].add(xb)
    used = collections.Counter(next(iter(v)) for v in m.values() if len(v) == 1)
    for xa, v in m.items():
        if len(v) == 1 and used[next(iter(v))] == 1:
            renames[(un, xa)] = next(iter(v))
for (un, xa), xb in sorted(renames.items()):
    print(un, xa, '->', xb)
if apply and renames:
    # only lines whose address belongs to the unit's sections are renamed: the label names repeat
    out = []
    import bisect
    splits = open(R + 'config/SLSP01/splits.txt').read()
    ranges = collections.defaultdict(list)
    cur = None
    for l in splits.split('\n'):
        if l and not l.startswith('\t') and l.endswith(':'):
            cur = l[:-1].rsplit('.', 1)[0]
        elif cur and l.startswith('\t'):
            mm = re.search(r'start:(0x[0-9A-F]+) end:(0x[0-9A-F]+)', l)
            if mm: ranges[cur].append((int(mm.group(1), 16), int(mm.group(2), 16)))
    n = 0
    for i, l in enumerate(symtxt):
        if ' = ' not in l: continue
        name = l.split(' = ')[0]
        ma = re.search(r':0x([0-9A-F]+);', l)
        if not ma: continue
        addr = int(ma.group(1), 16)
        for (un, xa), xb in renames.items():
            if name == xa and any(s <= addr < e for s, e in ranges.get(un, [])):
                symtxt[i] = xb + l[len(name):]; n += 1
    open(SYM, 'w').write('\n'.join(symtxt))
    print(n, 'renamed')
