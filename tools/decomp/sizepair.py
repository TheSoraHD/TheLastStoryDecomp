#!/usr/bin/env python3
"""sizepair.py [unit...]: target functions still named fn_XXXXXXXX with no counterpart in our object,
paired in address order with our unpaired symbols of the same size (proposals only: rename in
symbols.txt, rebuild and keep the ones fnstat shows at 100%)."""
import re, sys
from scanlib import units, unit_diff, nm

for u in units(sys.argv[1:]):
    d = unit_diff(u)
    if not d:
        continue
    Rn = set(s["name"] for s in d["right"]["symbols"])
    Ln = set(s["name"] for s in d["left"]["symbols"])
    tgt = sorted((s for s in d["left"]["symbols"] if "instructions" in s and re.fullmatch(r"fn_[0-9A-F]{8}", s["name"])
                  and s["name"] not in Rn), key=lambda s: int(s["name"][3:], 16))
    ours = []
    for l in nm("build/SLSP01/src/%s.o" % u).split("\n"):
        p = l.split()
        if len(p) == 4 and p[2] in "TW" and p[3] not in Ln:
            ours.append((int(p[1], 16), p[3]))
    j = 0
    for s in tgt:
        k = j
        while k < len(ours) and ours[k][0] != int(s["size"]):
            k += 1
        if k < len(ours):
            print(u, s["name"], ours[k][1])
            j = k + 1
