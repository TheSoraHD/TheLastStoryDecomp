#!/usr/bin/env python3
"""pairnames.py <unit> [--apply]: pair the functions of our object with the target's by order
(only when both have the same count) and rename target fn_XXXXXXXX symbols in symbols.txt."""
import os
import re, subprocess, sys, os
os.chdir(os.environ.get("CMP_ROOT", os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))))
unit = sys.argv[1]
def funcs(p):
    out = subprocess.run(["build/binutils/powerpc-eabi-nm", "-S", "-n", p], capture_output=True, text=True).stdout
    r = []
    for l in out.splitlines():
        q = l.split()
        if len(q) == 4 and q[2] in "TtWw":
            r.append((int(q[0], 16), int(q[1], 16), q[3]))
    return r
t = funcs("build/SLSP01/obj/%s.o" % unit)
o = funcs("build/SLSP01/src/%s.o" % unit)
t = [x for x in t if not x[2].startswith("gap_")]
print("target %d, ours %d" % (len(t), len(o)))
pairs = []
if len(t) == len(o):
    pairs = list(zip(t, o))
else:
    # in-order alignment on sizes (the linker strips unused functions of the source); sizes may
    # differ a little when the source is an older version
    j = 0
    for a in t:
        k = next((k for k in range(j, len(o)) if o[k][1] == a[1] or o[k][2] == a[2]), None)
        if k is None:
            print("  no pair for %s %x" % (a[2], a[1])); continue
        pairs.append((a, o[k])); j = k + 1
ren = {}
for a, b in pairs:
    flag = "" if a[1] == b[1] else "  (size %x vs %x)" % (a[1], b[1])
    if a[2] != b[2]:
        print("  %s -> %s%s" % (a[2], b[2], flag))
        if re.match(r"^fn_[0-9A-F]{8}$", a[2]):
            ren[a[2]] = b[2]
if "--apply" in sys.argv and ren:
    p = "config/SLSP01/symbols.txt"
    L = open(p).read().split("\n")
    for i, l in enumerate(L):
        m = re.match(r"(\S+) = (.*)", l)
        if m and m.group(1) in ren:
            L[i] = ren[m.group(1)] + " = " + m.group(2)
    open(p, "w").write("\n".join(L))
    print("renamed", len(ren))

# ---- data symbols: from the relocations of paired functions (same offset in both) ----
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection
def relocs(p):
    e = ELFFile(open(p, "rb")); st = e.get_section_by_name(".symtab"); secs = list(e.iter_sections())
    tidx = [k for k, x in enumerate(secs) if x.name == ".text"][0]
    r = {}
    for rs in e.iter_sections():
        if isinstance(rs, RelocationSection) and rs["sh_info"] == tidx:
            for x in rs.iter_relocations():
                sym = st.get_symbol(x["r_info_sym"])
                r[x["r_offset"]] = (sym.name, sym["st_info"]["bind"], x["r_addend"], sym["st_info"]["type"])
    return r
if "--data" in sys.argv:
    rt = relocs("build/SLSP01/obj/%s.o" % unit); ro = relocs("build/SLSP01/src/%s.o" % unit)
    dren = {}
    for a, b in pairs:
        for k in range(0, min(a[1], b[1]), 2):
            x = rt.get(a[0] + k); y = ro.get(b[0] + k)
            if x and y and re.match(r"^lbl_[0-9A-F]{8}$", x[0]) and not y[0].startswith(("@", ".")) \
                    and x[2] == 0 and y[2] == 0 and y[3] != "STT_FUNC":
                if x[0] in dren and dren[x[0]][0] != y[0]:
                    print("  conflict", x[0], dren[x[0]][0], y[0]); continue
                dren[x[0]] = (y[0], y[1] == "STB_LOCAL")
    for k, v in dren.items():
        print("  %s -> %s%s" % (k, v[0], " (local)" if v[1] else ""))
    if "--apply" in sys.argv and dren:
        p = "config/SLSP01/symbols.txt"
        L = open(p).read().split("\n")
        for i, l in enumerate(L):
            m = re.match(r"(\S+) = (.*)", l)
            if m and m.group(1) in dren:
                n, loc = dren[m.group(1)]
                rest = m.group(2)
                if loc and "scope:" not in rest:
                    rest = rest + " scope:local"
                L[i] = n + " = " + rest
        open(p, "w").write("\n".join(L))
        print("renamed data", len(dren))
