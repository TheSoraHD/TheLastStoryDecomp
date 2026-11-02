#!/usr/bin/env python3
# callpair.py <unit> [--apply] [--unique]: propose renames of target fn_/lbl_ symbols
#  1) identical masked bytes with a single candidate (--unique)
#  2) relocations at the same offsets inside functions that already have the same name
import sys, re
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection
import os
R = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))
unit = sys.argv[1]
def load(p):
    e = ELFFile(open(p, "rb"))
    secs = list(e.iter_sections())
    tidx = [k for k, x in enumerate(secs) if x.name == ".text"][0]
    data = secs[tidx].data()
    st = e.get_section_by_name(".symtab")
    rel = {}
    for rs in e.iter_sections():
        if isinstance(rs, RelocationSection) and rs["sh_info"] == tidx:
            for x in rs.iter_relocations():
                s = st.get_symbol(x["r_info_sym"])
                rel[x["r_offset"]] = (s.name, s["st_info"]["type"], x["r_addend"])
    fns = []
    for s in st.iter_symbols():
        if s["st_info"]["type"] == "STT_FUNC" and s["st_shndx"] == tidx and s["st_size"]:
            fns.append((s["st_value"], s["st_size"], s.name))
    fns.sort()
    out = {}
    for a, sz, n in fns:
        w = []
        for o in range(a, a + sz, 4):
            v = int.from_bytes(data[o:o+4], "big")
            if (o & ~3) in rel or (o + 2) in rel:
                op = v >> 26
                v = v & 0xFC000003 if op == 18 else v & 0xFFFF0000
            w.append(v)
        out[n] = (a, sz, tuple(w), {k - a: v for k, v in rel.items() if a <= k < a + sz})
    return out
t = load(f"{R}/build/SLSP01/obj/{unit}.o")
o = load(f"{R}/build/SLSP01/src/{unit}.o")
ren = {}
if "--unique" in sys.argv:
    byw = {}
    for n, (a, sz, w, r) in o.items():
        byw.setdefault(w, []).append(n)
    for n, (a, sz, w, r) in t.items():
        if re.match(r"^fn_[0-9A-F]{8}$", n):
            c = [x for x in byw.get(w, []) if x not in t]
            if len(c) == 1:
                ren[n] = c[0]
for n in t:
    if n in o and t[n][2] == o[n][2]:
        rt = t[n][3]; ro = o[n][3]
        for k, (sn, ty, ad) in rt.items():
            if k in ro and re.match(r"^(fn|lbl)_[0-9A-F]{8}$", sn):
                on = ro[k][0]
                if on != sn and not on.startswith(("@", ".")) and ad == 0 and ro[k][2] == 0:
                    if sn in ren and ren[sn] != on:
                        print("  conflict", sn, ren[sn], on); continue
                    ren[sn] = on
names_t = set(t)
for k, v in sorted(ren.items()):
    print("  %s -> %s" % (k, v))
if "--apply" in sys.argv and ren:
    p = f"{R}/config/SLSP01/symbols.txt"
    L = open(p).read().split("\n")
    names = set(l.split(" = ")[0] for l in L if " = " in l)
    n = 0
    for i, l in enumerate(L):
        m = re.match(r"(\S+) = (.*)", l)
        if m and m.group(1) in ren:
            new = ren[m.group(1)]
            if new in names:
                print("  skip (exists):", new); continue
            L[i] = new + " = " + m.group(2); names.add(new); n += 1
    open(p, "w").write("\n".join(L))
    print("renamed", n)
