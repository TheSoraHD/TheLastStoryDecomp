#!/usr/bin/env python3
# strend.py: @stringBase0 de symbols.txt cuyo tamano deja fuera un "" final referenciado como base+size
# (dtk recorta el "" final; objdiff da DIFF_REPLACE). Lista unidad, direccion, tamano y funciones candidatas.
import re, glob, os
syms = open('config/SLSP01/symbols.txt').read().split('\n')
sb = {}
for l in syms:
    m = re.match(r'@stringBase0 = \.rodata:0x([0-9A-F]+); // type:object size:0x([0-9A-F]+)', l)
    if m: sb[int(m.group(1), 16)] = int(m.group(2), 16)
for f in glob.glob('build/SLSP01/asm/**/*.s', recursive=True):
    t = open(f, errors='replace').read()
    m = re.search(r'\.obj "@stringBase0", local\n(.*?)\.endobj', t, re.S)
    if not m: continue
    # address of the object
    a = re.search(r'# \.rodata:0x[0-9A-F]+ \| 0x([0-9A-F]+) \| size: 0x([0-9A-F]+)\n\.obj "@stringBase0"', t)
    if not a: continue
    addr = int(a.group(1), 16); size = int(a.group(2), 16)
    hits = []
    for fn in re.finditer(r'\.fn (\S+),.*?\.endfn', t, re.S):
        body = fn.group(0)
        if '@stringBase0' not in body: continue
        # follow the registers that hold the pool address; a register is dropped when another instruction writes it
        regs = set()
        for ins in re.findall(r'\*/\t(\S+) ([^\n]*)', body):
            op, args = ins; a = [x.strip() for x in args.split(',')]
            m2 = re.match(r'addi (r\d+), r\d+, "@stringBase0"@l', op + ' ' + args)
            if m2: regs.add(m2.group(1)); continue
            if op == 'addi' and len(a) == 3 and a[1] in regs and a[2] == hex(size): hits.append(fn.group(1))
            if a and a[0] in regs and not op.startswith(('st', 'cmp', 'b')): regs.discard(a[0])
    if hits: print(os.path.relpath(f, 'build/SLSP01/asm'), hex(addr), hex(size), sorted(set(hits))[:4])
