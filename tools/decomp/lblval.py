#!/usr/bin/env python3
# lblval.py <unit...>: for each @N data label in both our .o and the original split .o, compare the bytes.
# Prints labels whose values differ (wrong pairing) and labels only in the original (unpaired) with candidates
# in our .o that have the same value and are not paired.
import sys
from elftools.elf.elffile import ELFFile

def syms(path):
    out = {}
    with open(path, 'rb') as f:
        e = ELFFile(f)
        st = e.get_section_by_name('.symtab')
        for s in st.iter_symbols():
            n = s.name
            if not n.startswith('@') or n.startswith('@string') or n.startswith('@STRING') or s['st_shndx'] in ('SHN_UNDEF', 'SHN_ABS', 'SHN_COMMON'):
                continue
            sec = e.get_section(s['st_shndx'])
            if sec.header['sh_type'] == 'SHT_NOBITS':
                continue
            d = sec.data()
            sz = s['st_size'] or 4
            k = n
            while k in out: k += '#'
            out[k] = (sec.name, d[s['st_value']:s['st_value'] + sz])
    return out

for u in sys.argv[1:]:
    a = syms('build/SLSP01/obj/%s.o' % u)
    b = syms('build/SLSP01/src/%s.o' % u)
    bad = [n for n in a if n in b and a[n][1] != b[n][1] and a[n][0] == b[n][0]]
    only = [n for n in a if n not in b and a[n][0] in ('.sdata2', '.rodata')]
    free = {n: v for n, v in b.items() if n not in a}
    print(u, 'mismatch', len(bad), 'unpaired', len(only))
    for n in bad:
        print('  BAD', n, a[n][0], a[n][1].hex(), 'ours', b[n][1].hex())
    for n in only:
        c = [m for m, v in free.items() if v == a[n]]
        print('  UNP', n, a[n][0], a[n][1][:8].hex(), 'cand', ' '.join(c[:6]))
