#!/usr/bin/env python3
"""bigcands.py [report.json] [--min 2000] [--fuzzy 99] [--nodiff]: large functions not at 100 %.

Lists functions with size >= --min and fuzzy in [--fuzzy, 100), with the share of the code
percent each one is worth and the number of differing instructions from odiff2.py.
Sorted by bytes per diff line (most profitable first). Markdown table on stdout.
"""
import json, subprocess, sys, os
root = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))
os.chdir(root)
args = sys.argv[1:]
def opt(n, d):
    if n in args:
        i = args.index(n); v = args[i + 1]; del args[i:i + 2]; return v
    return d
mn = int(opt("--min", "2000")); fz = float(opt("--fuzzy", "99"))
nodiff = "--nodiff" in args
args = [a for a in args if a != "--nodiff"]
rep = args[0] if args else "build/SLSP01/report.json"
d = json.load(open(rep))
total = int(d["measures"]["total_code"])
rows = []
for u in d["units"]:
    for f in u.get("functions", []):
        sz = int(f.get("size", 0)); p = f.get("fuzzy_match_percent", 0)
        if sz >= mn and fz <= p < 100:
            rows.append([u["name"][5:], f["name"], sz, p])
for r in rows:
    if nodiff:
        r.append(0); continue
    o = subprocess.run(["python3", "tools/decomp/odiff2.py", r[0], r[1]], capture_output=True, text=True).stdout
    r.append(len([l for l in o.splitlines() if l.strip()]))
rows.sort(key=lambda r: -r[2] / max(r[4], 1))
print("| # | Bytes | %% cod | Fuzzy | diff | Unidad | Funcion |")
print("|---|---|---|---|---|---|---|")
for i, (un, fn, sz, p, n) in enumerate(rows, 1):
    print("| %d | %d | %.3f | %.3f | %d | %s | `%s` |" % (i, sz, 100.0 * sz / total, p, n, un, fn))
print("\nTotal: %d funciones, %d B (%.2f %% del codigo)." % (len(rows), sum(r[2] for r in rows), 100.0 * sum(r[2] for r in rows) / total))
