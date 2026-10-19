#!/usr/bin/env python3
"""datarefs.py <unit> [section]: per function, the number of distinct objects referenced in a data
section (target vs ours), to find functions that emit a different number of anonymous constants."""
import os
import re, sys, os
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection
os.chdir(os.environ.get("CMP_ROOT", os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))))
unit = sys.argv[1]; secname = sys.argv[2] if len(sys.argv) > 2 else ".data"
def refs(p):
    e = ELFFile(open(p, "rb")); symtab = e.get_section_by_name(".symtab")
    secs = list(e.iter_sections())
    tidx = [i for i, x in enumerate(secs) if x.name == ".text"][0]
    didx = [i for i, x in enumerate(secs) if x.name == secname][0]
    funcs = sorted([(x["st_value"], x["st_size"], x.name) for x in symtab.iter_symbols()
                    if x["st_shndx"] == tidx and x["st_info"]["type"] == "STT_FUNC"])
    out = {}
    for r in e.iter_sections():
        if isinstance(r, RelocationSection) and r["sh_info"] == tidx:
            for x in r.iter_relocations():
                sym = symtab.get_symbol(x["r_info_sym"])
                if sym["st_shndx"] == didx:
                    off = sym["st_value"] + x["r_addend"]
                    f = [f for f in funcs if f[0] <= x["r_offset"] < f[0] + f[1]]
                    if f:
                        out.setdefault(f[0][2], set()).add(off)
    return funcs, out
ft, rt = refs("build/SLSP01/obj/%s.o" % unit)
fo, ro = refs("build/SLSP01/src/%s.o" % unit)
norm = lambda n: (re.match(r"(fn_[0-9A-F]{8})", n).group(1) if re.match(r"fn_[0-9A-F]{8}", n) else n)
ron = {norm(k): v for k, v in ro.items()}
tot = 0
for a, s, n in ft:
    t = len(rt.get(n, ())); o = len(ron.get(norm(n), ()))
    if t != o:
        tot += t - o
        print("%06X %3d %3d  (cum %+d)  %s" % (a, t, o, tot, n[:70]))
