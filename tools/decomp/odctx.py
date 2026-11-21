#!/usr/bin/env python3
"""odctx.py <unit> <symbol> <index> [n=12]: target | ours instructions around an objdiff row index (both sides)."""
import json, subprocess, sys, os
os.chdir(os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..")))
u, sym, idx = sys.argv[1], sys.argv[2], int(sys.argv[3]); n = int(sys.argv[4]) if len(sys.argv) > 4 else 12
d = json.loads(subprocess.run(["build/tools/objdiff-cli", "diff", "-p", ".", "-u", "main/" + u, sym, "-o", "-",
                               "--format", "json"], capture_output=True, text=True).stdout)
rows = {}
for side in ("left", "right"):
    for s in d[side]["symbols"]:
        if s["name"] == sym: rows[side] = s.get("instructions", [])
L, R = rows.get("left", []), rows.get("right", [])
f = lambda x: (x.get("instruction") or {}).get("formatted", "") if isinstance(x.get("instruction"), dict) else ""
for i in range(max(0, idx - n), min(max(len(L), len(R)), idx + n + 1)):
    a = L[i] if i < len(L) else {}; b = R[i] if i < len(R) else {}
    k = a.get("diff_kind", "") or b.get("diff_kind", "")
    print("%-4d %s %-40s | %s" % (i, "*" if k else " ", f(a), f(b)))
