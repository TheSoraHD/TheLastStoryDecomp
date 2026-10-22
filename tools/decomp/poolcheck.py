#!/usr/bin/env python3
"""Check literal VALUES against the original split objects (objdiff only compares labels and
positions, so a wrong float or a typo in a string can still show 100%).

usage: poolcheck.py [substring ...]     (run from the repo root after a build; default: all game units)

For every unit with both build/SLSP01/obj/<unit>.o (original) and build/SLSP01/src/<unit>.o (ours):
  - floats: 4-byte words of our .sdata2 that do not appear in the original .sdata2 (wrong literal or
    a constant the original does not emit in that file);
  - strings: printable strings (>= 5 chars) in our .rodata/.data/.sdata/.sdata2 that are neither in
    the original unit nor anywhere in orig/SLSP01/sys/main.dol (typos; weak RTTI names that the
    linker deduplicates are skipped because they exist elsewhere in the DOL).
"""
import glob, os, re, struct, sys
from collections import Counter
from elftools.elf.elffile import ELFFile

SECS = ('.rodata', '.data', '.sdata', '.sdata2')


def sections(path):
    out = {}
    with open(path, 'rb') as f:
        e = ELFFile(f)
        for n in SECS:
            s = e.get_section_by_name(n)
            if s is not None and s.header['sh_type'] != 'SHT_NOBITS':
                out[n] = s.data()
    return out


def strings(secs):
    out = set()
    for d in secs.values():
        for m in re.finditer(rb'[\x20-\x7e]{5,}', d):
            out.add(m.group())
    return out


def main():
    pats = sys.argv[1:]
    dol = open('orig/SLSP01/sys/main.dol', 'rb').read()
    found = 0
    for orig in sorted(glob.glob('build/SLSP01/obj/**/*.o', recursive=True)):
        rel = os.path.relpath(orig, 'build/SLSP01/obj')
        if pats and not any(p in rel for p in pats):
            continue
        ours = os.path.join('build/SLSP01/src', rel)
        if not os.path.exists(ours):
            continue
        a, b = sections(orig), sections(ours)
        wa = Counter(a.get('.sdata2', b'')[i:i + 4] for i in range(0, len(a.get('.sdata2', b'')) - 3, 4))
        db = b.get('.sdata2', b'')
        wb = Counter(db[i:i + 4] for i in range(0, len(db) - 3, 4))
        extra = [w for w in wb if wb[w] > wa.get(w, 0) and w != b'\0\0\0\0']
        sa = strings(a)
        bad = sorted(s for s in strings(b) - sa if not any(s in x for x in sa) and s not in dol)
        if extra or bad:
            found += 1
            msg = rel
            if extra:
                msg += '  floats: ' + ' '.join('%s(%g)' % (w.hex(), struct.unpack('>f', w)[0]) for w in extra)
            if bad:
                msg += '  strings: ' + repr([s.decode() for s in bad][:10])
            print(msg)
    print('units with findings:', found)


if __name__ == '__main__':
    main()
