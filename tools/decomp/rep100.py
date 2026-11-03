#!/usr/bin/env python3
"""rep100.py [unit...]: functions that report.json counts at 100% but objdiff-cli does not (rule 1:
they do not count). Usually stale labels (pd.sh) or symbol names (symdiff.py)."""
import json, sys
from scanlib import units, unit_diff

rep = {u["name"][5:]: u for u in json.load(open("build/SLSP01/report.json"))["units"]}
tot = 0
for u in units(sys.argv[1:]):
    r100 = set(f["name"] for f in rep.get(u, {}).get("functions", []) if f.get("fuzzy_match_percent", 0) == 100.0)
    d = unit_diff(u) if r100 else None
    if not d:
        continue
    for s in d["left"]["symbols"]:
        if "instructions" in s and s["name"] in r100 and s.get("match_percent") != 100.0:
            print(u, s["name"], s.get("match_percent"))
            tot += 1
print("total", tot)
