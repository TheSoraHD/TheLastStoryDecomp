#!/usr/bin/env python3
"""mirrorscan.py [--lo P] [--hi P] [--min SIZE] [--units a,b]: stack groups in mirrored order (bloqueos-20261122).

Runs slotmap.py on every function of the game in [lo, hi) and reports the ones where three or more moved
stack groups of the same size are in reverse order in our build (the original keeps them ascending, as the
locals of an inline, and we keep them descending, as temporaries, or the reverse). Such a function usually
closes by moving the statements that create those objects into an inline (EnemyQuarkStand fn_8034F788:
AimDir_unk) or out of one.
"""
import json, os, re, subprocess, sys
root = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))
os.chdir(root)
a = sys.argv[1:]
def opt(n, d, t=str):
    if n in a:
        i = a.index(n); v = a[i + 1]; del a[i:i + 2]; return t(v)
    return d
lo = opt("--lo", 97.0, float); hi = opt("--hi", 100.0, float); mins = opt("--min", 200, int)
units = opt("--units", None)
r = json.load(open("build/SLSP01/report.json"))
G = re.compile(r"^<> 0x([0-9a-f]+)-0x([0-9a-f]+)\s+0x([0-9a-f]+)-0x([0-9a-f]+)\s+[-+]\d+\s+(\d+)")
for u in r["units"]:
    un = u["name"][5:]
    if units and un not in units.split(","):
        continue
    if not re.match(r"(game|gmk|chr|atn|MP|Event|AI|db)/", un):
        continue
    for f in u.get("functions", []):
        p = f.get("fuzzy_match_percent", 0); sz = int(f.get("size", 0))
        if not (lo <= p < hi) or sz < mins:
            continue
        o = subprocess.run(["python3", "tools/decomp/slotmap.py", un, f["name"]], capture_output=True, text=True).stdout
        gs = []
        for l in o.splitlines():
            m = G.match(l)
            if m:
                gs.append((int(m.group(1), 16), int(m.group(3), 16), int(m.group(5))))
        best = 0
        for size in set(g[2] for g in gs):
            s = sorted([g for g in gs if g[2] == size])
            run = 1
            for k in range(1, len(s)):
                run = run + 1 if s[k][1] < s[k - 1][1] else 1
                best = max(best, run)
        if best >= 3:
            print("%-32s %-60s %5d %.3f mirrored %d of %d groups" % (un, f["name"][:60], sz, p, best, len(gs)), flush=True)
