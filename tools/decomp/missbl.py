#!/usr/bin/env python3
"""missbl.py [unit...]: functions < 100% where the target calls (bl) a function of the same unit that
our object does not call (we inline it). Fix with __declspec(noinline) on that function (accepted
exception), the inlining-budget recipe, or, if later functions of the original inline it, look for a
file boundary (two .cpp in one split)."""
import sys
from scanlib import units, unit_diff


def bls(s):
    r = []
    for a in s.get("instructions", []):
        f = (a.get("instruction") or {}).get("formatted", "")
        if f.startswith("bl "):
            r.append(f[3:].strip().strip('"'))
    return r


for u in units(sys.argv[1:]):
    d = unit_diff(u)
    if not d:
        continue
    L = {s["name"]: s for s in d["left"]["symbols"] if "instructions" in s}
    R = {s["name"]: s for s in d["right"]["symbols"] if "instructions" in s}
    for n, s in L.items():
        if s.get("match_percent") == 100.0 or n not in R:
            continue
        ob = set(bls(R[n]))
        miss = sorted(x for x in set(bls(s)) if x in L and x not in ob and int(L[x].get("size", 0)) > 40)
        if miss:
            print("%s %s %.2f%% -> %s" % (u, n[:60], s.get("match_percent") or 0, " ".join(miss[:4])))
