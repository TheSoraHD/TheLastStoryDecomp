#!/usr/bin/env python3
"""unitcmp.py <unit> [ref/report.json]: per-function match of the current .o (objdiff-cli) against the reference
report (default $REF/report.json): prints functions that went up or down, and the bytes at 100 % before/after."""
import json, subprocess, sys, os
os.chdir(os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..")))
u = sys.argv[1]
ref = sys.argv[2] if len(sys.argv) > 2 else os.path.join(os.environ.get("REF", "/home/claude/ref"), "report.json")
R = {}
for x in json.load(open(ref))["units"]:
    if x["name"] == "main/" + u:
        R = {f["name"]: (f.get("fuzzy_match_percent", 0), int(f.get("size", 0))) for f in x.get("functions", [])}
o = subprocess.run(["build/tools/objdiff-cli", "diff", "-p", ".", "-u", "main/" + u, "-o", "-", "--format", "json"],
                   capture_output=True, text=True).stdout
d = json.loads(o)
N = {}
for s in d["left"]["symbols"]:
    if s.get("kind") == "SYMBOL_FUNCTION" or "instructions" in s:
        N[s["name"]] = s.get("match_percent", 0) or 0
b0 = b1 = 0
for n, (p, sz) in R.items():
    q = N.get(n)
    if p == 100: b0 += sz
    if q is not None and q == 100: b1 += sz
    if q is None: print("MISSING", n); continue
    if abs(q - p) > 1e-3: print("%s %8.3f -> %8.3f %6d %s" % ("UP  " if q > p else "DOWN", p, q, sz, n))
print("bytes@100 %d -> %d (%+d)" % (b0, b1, b1 - b0))
