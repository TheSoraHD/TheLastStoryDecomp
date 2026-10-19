#!/usr/bin/env python3
"""Per-function match status for one unit.

usage: fnstat.py <unit, e.g. game/CharaTask> [filter-substring] [--nobuild]
Compiles build/SLSP01/src/<unit>.o and diffs every target function with
objdiff-cli. Prints address, match %, size, name.
"""
import os
import json, re, subprocess, sys, os

ROOT = os.environ.get("CMP_ROOT", os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..")))
args = [a for a in sys.argv[1:] if not a.startswith("--")]
unit = args[0]
flt = args[1] if len(args) > 1 else ""
os.chdir(ROOT)

base = 0
cur = None
for l in open("config/SLSP01/splits.txt"):
    if l.strip().endswith(":") and not l.startswith("\t"):
        cur = l.strip()[:-1]
    m = re.match(r"\s+\.text\s+start:0x([0-9A-F]+)", l)
    if m and cur and cur.rsplit(".", 1)[0] == unit:
        base = int(m.group(1), 16)

if "--nobuild" not in sys.argv:
    r = subprocess.run(["ninja", f"build/SLSP01/src/{unit}.o"], capture_output=True, text=True)
    if r.returncode:
        print(r.stdout[-6000:], r.stderr[-2000:]); sys.exit(1)
out = subprocess.run(["build/tools/objdiff-cli", "diff", "-p", ".", "-u", f"main/{unit}",
                      "-o", "-", "--format", "json"], capture_output=True, text=True)
d = json.loads(out.stdout)
rows = []
for s in d["left"]["symbols"]:
    if "instructions" not in s:
        continue
    rows.append((base + int(s.get("address", 0)), s.get("match_percent"), int(s["size"]), s["name"]))
rows.sort()
tot = ok = 0
for a, p, sz, n in rows:
    if flt and flt not in n:
        continue
    tot += 1
    ok += (p == 100.0)
    print("%08X %7s %6d %s" % (a, "-" if p is None else "%.2f" % p, sz, n))
print(f"# {ok}/{tot} at 100%")
