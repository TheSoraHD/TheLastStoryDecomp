#!/usr/bin/env python3
"""odiff2.py <unit> <symbol>: print the differing instructions of one function (objdiff)."""
import os
import json, subprocess, sys, os
os.chdir(os.environ.get("CMP_ROOT", os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))))
u, sym = sys.argv[1], sys.argv[2]
out = subprocess.run(["build/tools/objdiff-cli", "diff", "-p", ".", "-u", "main/" + u, sym, "-o", "-", "--format", "json"],
                     capture_output=True, text=True).stdout
d = json.loads(out)
rows = {}
for side in ("left", "right"):
    for s in d[side]["symbols"]:
        if s["name"] == sym:
            rows[side] = s.get("instructions", [])
L, R = rows.get("left", []), rows.get("right", [])
for i in range(max(len(L), len(R))):
    a = L[i] if i < len(L) else {}; b = R[i] if i < len(R) else {}
    if a.get("diff_kind") or b.get("diff_kind"):
        fa = (a.get("instruction") or {}).get("formatted", "") if isinstance(a.get("instruction"), dict) else ""
        fb = (b.get("instruction") or {}).get("formatted", "") if isinstance(b.get("instruction"), dict) else ""
        print("%-4d %-22s %-40s | %s" % (i, a.get("diff_kind", "") or b.get("diff_kind", ""), fa, fb))
