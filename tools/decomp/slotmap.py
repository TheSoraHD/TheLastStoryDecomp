#!/usr/bin/env python3
"""slotmap.py <unit> <symbol> [-v]: stack slots of one function, ours against the original (bloqueos-20261122).

Pairs the rows of objdiff (same row = same instruction) and, for each operand `N(r1)` or `addi rX, r1, N`,
maps the offset of the original to ours. Offsets that move by the same amount and are contiguous are
grouped as one object (a local or a temporary of 4/8/12/16/0x30... bytes). For each group: original range,
our range, delta, size, first row and the instructions of the first use, sorted by the original offset (the
lowest slot first). Rule of thumb (docs/trucos-mwcc.md): named locals in reverse order of declaration, then
the temporaries; the big ones first; the ones of an inline go below, in ascending order.
-v prints every paired operand.
"""
import json, os, re, subprocess, sys

os.chdir(os.environ.get("CMP_ROOT", os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))))
u, sym = sys.argv[1], sys.argv[2]
verbose = "-v" in sys.argv
out = subprocess.run(["build/tools/objdiff-cli", "diff", "-p", ".", "-u", "main/" + u, sym, "-o", "-", "--format", "json"],
                     capture_output=True, text=True).stdout
d = json.loads(out)
rows = {}
for side in ("left", "right"):
    for s in d[side]["symbols"]:
        if s["name"] == sym:
            rows[side] = s.get("instructions", [])
L, R = rows.get("left", []), rows.get("right", [])

def fmt(x):
    i = x.get("instruction") if isinstance(x, dict) else None
    return i.get("formatted", "") if isinstance(i, dict) else ""

# skip the prologue/epilogue saves (stfd/psq_st of f14-f31, stw of r0/r31 above the frame...)
def stackops(t):
    if re.match(r"(stwu|stmw|lmw)\b", t) or "r1, r1" in t:
        return []
    res = []
    for m in re.finditer(r"(-?0x[0-9a-f]+|\d+)\(r1\)", t):
        res.append(int(m.group(1), 16) if "x" in m.group(1) else int(m.group(1)))
    m = re.match(r"addi r\d+, r1, (0x[0-9a-f]+|\d+)", t)
    if m:
        res.append(int(m.group(1), 16) if "x" in m.group(1) else int(m.group(1)))
    return res

def width(t):
    op = t.split()[0] if t else ""
    if op.startswith("psq"):
        return 8
    if op in ("stfd", "lfd"):
        return 8
    if op in ("stb", "lbz", "lbzu", "stbu"):
        return 1
    if op in ("sth", "lhz", "lha", "sthu"):
        return 2
    return 4

pairs = {}   # orig offset -> {ours offset: [rows]}
first = {}
unpaired = []
n = max(len(L), len(R))
# frame size: the stwu of the prologue, to skip the register saves at the top of the frame
def frame(rs):
    for x in rs[:3]:
        m = re.match(r"stwu r1, -(0x[0-9a-f]+)\(r1\)", fmt(x))
        if m:
            return int(m.group(1), 16)
    return 1 << 30
fa_, fb_ = frame(L), frame(R)
for i in range(n):
    ta = fmt(L[i]) if i < len(L) else ""
    tb = fmt(R[i]) if i < len(R) else ""
    oa, ob = stackops(ta), stackops(tb)
    if oa and ob and len(oa) == len(ob):
        for x, y in zip(oa, ob):
            if x >= fa_ - 0x120 and re.match(r"(stfd|lfd|psq_st|psq_l) f(1[4-9]|2\d|3[01])", ta):
                continue   # saved FPRs
            pairs.setdefault(x, {}).setdefault(y, []).append(i)
            first.setdefault((x, y), (i, ta, tb, width(ta)))
            if verbose:
                print("%4d  0x%-4x -> 0x%-4x  %-36s | %s" % (i, x, y, ta, tb))
    elif oa or ob:
        unpaired.append((i, ta, tb))

# groups: orig offsets sorted; contiguous (gap <= 4 bytes past the last access) with the same delta
items = []
for x in sorted(pairs):
    for y, rs in pairs[x].items():
        items.append((x, y, rs))
groups = []
for x, y, rs in items:
    w = first[(x, y)][3]
    dl = y - x
    if groups and groups[-1]["d"] == dl and x <= groups[-1]["end"] + 4 and x >= groups[-1]["start"]:
        g = groups[-1]
        g["end"] = max(g["end"], x + w); g["rows"] += rs
    else:
        groups.append({"start": x, "end": x + w, "d": dl, "rows": list(rs), "first": first[(x, y)]})
print("%-14s %-14s %7s %5s  %s" % ("orig", "ours", "delta", "size", "first use (row: orig | ours)"))
for g in groups:
    i, ta, tb, w = g["first"]
    mark = "  " if g["d"] == 0 else "<>"
    print("%s 0x%03x-0x%03x  0x%03x-0x%03x  %+6d  %4d  %d: %s | %s" % (mark, g["start"], g["end"], g["start"] + g["d"],
          g["end"] + g["d"], g["d"], g["end"] - g["start"], i, ta, tb))
moved = [g for g in groups if g["d"]]
print("groups %d, moved %d; rows with r1 only on one side: %d" % (len(groups), len(moved), len(unpaired)))
if verbose:
    for i, ta, tb in unpaired:
        print("  %4d %-36s | %s" % (i, ta, tb))
