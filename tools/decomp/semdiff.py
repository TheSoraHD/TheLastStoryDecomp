#!/usr/bin/env python3
"""semdiff.py <unit> <symbol>: objdiff rows that still differ after normalizing registers, stack offsets
(x(r1)) and branch targets; also the multiset of calls and of member offsets on both sides."""
import json, subprocess, sys, re, collections, os
os.chdir(os.environ.get("CMP_ROOT", os.getcwd()))
u, sym = sys.argv[1], sys.argv[2]
out = subprocess.run(["build/tools/objdiff-cli", "diff", "-p", ".", "-u", "main/" + u, sym, "-o", "-", "--format", "json"],
                     capture_output=True, text=True).stdout
d = json.loads(out)
rows = {}; base = {}
for side in ("left", "right"):
    for s in d[side]["symbols"]:
        if s["name"] == sym:
            rows[side] = s.get("instructions", []); base[side] = int(s["address"])
def fmt(x):
    i = x.get("instruction")
    return i.get("formatted", "") if isinstance(i, dict) else ""
def norm(t):
    t = re.sub(r"-?0x[0-9a-f]+\(r1\)", "S(r1)", t)
    t = re.sub(r"^addi (r\d+), r1, -?0x[0-9a-f]+", r"addi \1, r1, S", t)
    t = re.sub(r"\b(r|f|cr)\d+\b", r"\1", t)
    t = re.sub(r"^(b\w*)\s+0x[0-9a-f]+", r"\1 T", t)
    t = re.sub(r"^(b\w*\s+cr,)\s*0x[0-9a-f]+", r"\1 T", t)
    t = re.sub(r"@\d+", "@N", t)
    return t
L, R = rows.get("left", []), rows.get("right", [])
A = [norm(fmt(x)) for x in L if fmt(x)]; B = [norm(fmt(x)) for x in R if fmt(x)]
import difflib
sm = difflib.SequenceMatcher(None, A, B, autojunk=False)
for op, a1, a2, b1, b2 in sm.get_opcodes():
    if op == "equal": continue
    print("%s orig[%d:%d] ours[%d:%d]" % (op, a1, a2, b1, b2))
    for k in range(max(a2 - a1, b2 - b1)):
        x = A[a1 + k] if a1 + k < a2 else ""; y = B[b1 + k] if b1 + k < b2 else ""
        print("    %-45s | %s" % (x, y))
# branch targets: map original instruction indices to ours through the aligned blocks
def real(X):
    return [x for x in X if fmt(x)]
LL, RR = real(L), real(R)
m = {}
for op, a1, a2, b1, b2 in sm.get_opcodes():
    if op == "equal" or (op == "replace" and a2 - a1 == b2 - b1):
        for k in range(a2 - a1): m[a1 + k] = b1 + k
def dest(x, side):
    i = x.get("instruction") or {}
    bd = i.get("branch_dest")
    return None if bd is None else (int(bd) - base[side]) // 4
bad = 0
for ia, ib in sorted(m.items()):
    da, db = dest(LL[ia], "left"), dest(RR[ib], "right")
    if da is None and db is None: continue
    if da is None or db is None or m.get(da, -999) != db:
        if da is not None and da not in m and db is not None: continue  # target inside an unaligned block
        bad += 1; print("BRANCH orig %d -> %s | ours %d -> %s (mapped %s)  %s" % (ia, da, ib, db, m.get(da), A[ia]))
print("branches with a different target: %d" % bad)
def bag(X, pat):
    return collections.Counter(m.group(0) for t in X for m in [re.search(pat, t)] if m)
for name, pat in (("calls", r"^bl .*"), ("mem", r"^(l|st)\w+ \w+, -?0x[0-9a-f]+\(r\)$")):
    ca, cb = bag(A, pat), bag(B, pat)
    if ca != cb:
        print("== %s only orig:" % name, dict(ca - cb)); print("== %s only ours:" % name, dict(cb - ca))
print("rows orig %d ours %d" % (len(A), len(B)))
