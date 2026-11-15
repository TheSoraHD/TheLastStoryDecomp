#!/usr/bin/env python3
# datasplit.py <unit> [--apply]: split @N objects of .data/.rodata in symbols.txt that our .o emits as several
# consecutive @N objects (e.g. ptmf constants of 12 bytes merged by dtk). Only when our pieces tile it exactly.
import re, sys
from elftools.elf.elffile import ELFFile
u = sys.argv[1]; apply = '--apply' in sys.argv
def objs(path):
    e = ELFFile(open(path, 'rb')); out = {}
    for s in e.get_section_by_name('.symtab').iter_symbols():
        if not s.name.startswith('@') or s['st_shndx'] in ('SHN_UNDEF', 'SHN_ABS', 'SHN_COMMON'): continue
        sec = e.get_section(s['st_shndx']).name
        if sec in ('.data', '.rodata'): out.setdefault(sec, []).append((s['st_value'], s['st_size'], s.name))
    return out
A = objs('build/SLSP01/obj/%s.o' % u); B = objs('build/SLSP01/src/%s.o' % u)
sp = open('config/SLSP01/splits.txt').read()
m = re.search(r'^' + re.escape(u) + r'\.(?:cpp|c):\n((?:\t.*\n)+)', sp, re.M)
start = {mm.group(1): int(mm.group(2), 16) for mm in re.finditer(r'\t(\.\w+)\s+start:0x([0-9A-F]+)', m.group(1))}
L = open('config/SLSP01/symbols.txt').read().split('\n')
idx = {}
for i, l in enumerate(L):
    mm = re.match(r'(@\d+) = (\.\w+):0x([0-9A-F]+);', l)
    if mm: idx.setdefault((mm.group(2), int(mm.group(3), 16)), []).append(i)
rep = {}
for sec, lst in A.items():
    ours = sorted(B.get(sec, []))
    for off, size, name in lst:
        if size <= 0: continue
        pieces = [p for p in ours if off <= p[0] < off + size]
        if len(pieces) < 2 or pieces[0][0] != off: continue
        pos = off; ok = True
        for p in pieces:
            if p[0] != pos or p[1] <= 0: ok = False; break
            pos += p[1]
        if not ok or pos != off + size: continue
        addr = start[sec] + off
        lines = [i for i in idx.get((sec, addr), []) if re.search(r'size:0x%X ' % size, L[i])]
        if len(lines) != 1: continue
        i = lines[0]; old = L[i]; new = []
        for p in pieces:
            l = re.sub(r'^@\d+ = (\.\w+):0x[0-9A-F]+;', '%s = %s:0x%08X;' % (p[2], sec, start[sec] + p[0]), old)
            new.append(re.sub(r'size:0x[0-9A-F]+ ', 'size:0x%X ' % p[1], l))
        rep[i] = new
print(u, 'objects split:', len(rep), 'new symbols:', sum(len(v) for v in rep.values()))
if apply:
    out = []
    for i, l in enumerate(L): out += rep.get(i, [l])
    open('config/SLSP01/symbols.txt', 'w').write('\n'.join(out))
