#!/usr/bin/env python3
"""offscan.py <unit> [symbol...]: compare the multiset of memory offsets and immediates of each
non-matching function (original vs ours). Register allocation and scheduling do not change the
multiset; a different member (0x388 instead of 0x3A8), a missing store or a wrong constant does, so
the output points at semantic errors in functions that are "only registers" away.

For every function below 100% it prints the offsets present only in the original (-) or only in
ours (+), e.g. `- 0x3a8(` means the original reads or writes offset 0x3A8 once more than ours."""
import collections
import json
import os
import re
import subprocess
import sys

os.chdir(os.environ.get("CMP_ROOT", os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))))
unit = sys.argv[1]
only = set(sys.argv[2:])
out = subprocess.run(["build/tools/objdiff-cli", "diff", "-p", ".", "-u", "main/" + unit, "-o", "-", "--format", "json"],
                     capture_output=True, text=True).stdout
d = json.loads(out)

MEM = re.compile(r"(-?0x[0-9a-f]+|-?\d+)\((r\d+)\)")
IMM = re.compile(r"^(addi|li|lis|ori|oris|cmpwi|cmplwi|mulli|subi|andi\.|xori|rlwinm|clrlwi|slwi|srwi)\b")


def keys(instrs):
    c = collections.Counter()
    for ins in instrs:
        f = (ins.get("instruction") or {}).get("formatted", "") if isinstance(ins.get("instruction"), dict) else ""
        if not f:
            continue
        op = f.split()[0]
        for m in MEM.finditer(f):
            off = m.group(1)
            if m.group(2) == "r1":
                continue  # stack slots move with the allocation
            c["%s(" % off] += 1
        regs = f.replace(",", " ").split()[1:]
        if op in ("addi", "subi") and len(regs) > 1 and regs[1] == "r1":
            continue  # stack addresses
        if IMM.match(f) and "(" not in f and "@" not in f:
            last = f.replace(",", " ").split()[-1]
            if re.match(r"^-?(0x[0-9a-f]+|\d+)$", last):
                c["%s %s" % (op.rstrip("."), last)] += 1
    return c


def syms(side):
    r = {}
    for s in d[side]["symbols"]:
        r[s["name"]] = s
    return r


L, R = syms("left"), syms("right")
for name, s in L.items():
    if only and name not in only:
        continue
    m = s.get("match_percent")
    if m is None or m >= 100 or name not in R:
        continue
    a, b = keys(s.get("instructions", [])), keys(R[name].get("instructions", []))
    minus, plus = a - b, b - a
    if not minus and not plus:
        continue
    print("%s %.2f%%" % (name, m))
    for k, v in sorted(minus.items()):
        print("   - %s x%d" % (k, v))
    for k, v in sorted(plus.items()):
        print("   + %s x%d" % (k, v))
