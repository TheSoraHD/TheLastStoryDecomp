#!/usr/bin/env python3
"""Check that our .text functions come out in the same order as in the target object.

usage: ordercheck.py <unit> [--nobuild] [-v]

With -ipa file MWCC emits functions in source order, so a unit can only link if
the definitions are written in .dol order. This compares build/SLSP01/src/<unit>.o
against build/SLSP01/obj/<unit>.o (the split of the original):
  - functions of ours that are out of order (by the target offset of the same name),
  - target functions that have no body yet (missing),
  - functions of ours that the target does not have (extra; e.g. a name that differs).
Weak symbols (inline/template copies) are ignored.
Names are paired like cmp.py: identical name, or the fn_<ADDR> prefix.
Exit code 1 if anything is out of order.
"""
import os, re, subprocess, sys
from elftools.elf.elffile import ELFFile

ROOT = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))
os.chdir(ROOT)
args = [a for a in sys.argv[1:] if not a.startswith("-")]
unit = args[0]
verbose = "-v" in sys.argv

if "--nobuild" not in sys.argv:
    r = subprocess.run(["ninja", f"build/SLSP01/src/{unit}.o"], capture_output=True, text=True)
    if r.returncode:
        out = r.stdout
        i = out.find("###")
        print(out[i:i + 3000] if i >= 0 else out[-3000:])
        sys.exit(2)


def key(n):
    m = re.match(r"(fn_[0-9A-F]{8})", n)
    return m.group(1) if m else n


def funcs(path):
    with open(path, "rb") as f:
        elf = ELFFile(f)
        secs = list(elf.iter_sections())
        out = []
        for s in elf.get_section_by_name(".symtab").iter_symbols():
            if s["st_info"]["type"] != "STT_FUNC" or s["st_size"] == 0:
                continue
            shn = s["st_shndx"]
            if not isinstance(shn, int) or secs[shn].name != ".text":
                continue
            if s["st_info"]["bind"] == "STB_WEAK":
                continue  # inline/template copies, placed by the compiler
            out.append((s["st_value"], key(s.name), s.name))
        out.sort()
        return out


ours = funcs(f"build/SLSP01/src/{unit}.o")
tgt = funcs(f"build/SLSP01/obj/{unit}.o")
pos = {k: i for i, (_, k, _) in enumerate(tgt)}

seq = [(pos[k], n) for _, k, n in ours if k in pos]
extra = [n for _, k, n in ours if k not in pos]
have = {k for _, k, _ in ours}
missing = [n for _, k, n in tgt if k not in have]

# longest increasing subsequence: everything outside it has to move
import bisect
tails, idx, prev = [], [], [-1] * len(seq)
for i, (p, _) in enumerate(seq):
    j = bisect.bisect_left([seq[t][0] for t in tails], p)
    if j == len(tails):
        tails.append(i)
    else:
        tails[j] = i
    prev[i] = tails[j - 1] if j else -1
keep = set()
i = tails[-1] if tails else -1
while i >= 0:
    keep.add(i)
    i = prev[i]
moved = [seq[i][1] for i in range(len(seq)) if i not in keep]

print(f"{unit}: {len(seq)} paired, {len(missing)} missing, {len(extra)} extra, {len(moved)} out of order")
if moved:
    print("out of order (move these):")
    for n in moved:
        print("  ", n)
if verbose:
    for n in missing:
        print("missing ", n)
for n in extra:
    print("extra   ", n)
sys.exit(1 if moved else 0)
