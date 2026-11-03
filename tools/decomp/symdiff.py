#!/usr/bin/env python3
"""symdiff.py [unit...]: functions < 100% whose differing instructions only differ in symbol operands
(relocation targets). Without units: every unlinked game unit with source (~1 min).
Typical causes: callers that use an extern "C" for a method already named mangled in symbols.txt,
stale @NNNN labels (pd.sh), weak copies still named fn_XXXXXXXX."""
import re, sys
from scanlib import units, unit_diff


def norm(x):
    p = x.split(None, 1)
    if len(p) < 2:
        return x
    o = []
    for t in re.split(r"(,\s*)", p[1]):
        if re.fullmatch(r"(r|f|cr|qr)\d+|-?0x[0-9a-f]+(\((r\d+)\))?|-?\d+|,\s*", t.strip()) or t.strip() == "":
            o.append(t)
        else:
            o.append(re.sub(r"^[^(]+", "S", t))
    return p[0] + " " + "".join(o)


for u in units(sys.argv[1:]):
    d = unit_diff(u)
    if not d:
        continue
    R = {s["name"]: s for s in d["right"]["symbols"]}
    for s in d["left"]["symbols"]:
        if "instructions" not in s or s.get("match_percent") == 100.0:
            continue
        r = R.get(s["name"])
        if not r or "instructions" not in r or len(s["instructions"]) != len(r["instructions"]):
            continue
        pairs = []
        for a, b in zip(s["instructions"], r["instructions"]):
            if a.get("diff_kind") or b.get("diff_kind"):
                fa = (a.get("instruction") or {}).get("formatted", "")
                fb = (b.get("instruction") or {}).get("formatted", "")
                if norm(fa) != norm(fb):
                    pairs = None
                    break
                pairs.append((fa, fb))
        if pairs:
            print("%s %s %.2f%%" % (u, s["name"][:80], s.get("match_percent") or 0))
            for fa, fb in sorted(set(pairs))[:4]:
                print("    target: %s | ours: %s" % (fa[:90], fb[:90]))
