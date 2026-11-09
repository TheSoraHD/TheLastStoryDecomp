#!/usr/bin/env python3
"""blorder.py <unit> <symbol>: sequence of the call targets (bl) of a function, original vs ours, with
the moves needed to align them (difflib). A block of calls that appears in another place points at
case bodies or if/else branches written in another order."""
import difflib
import json
import os
import subprocess
import sys

os.chdir(os.environ.get("CMP_ROOT", os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))))
unit, sym = sys.argv[1], sys.argv[2]
out = subprocess.run(["build/tools/objdiff-cli", "diff", "-p", ".", "-u", "main/" + unit, sym, "-o", "-", "--format", "json"],
                     capture_output=True, text=True).stdout
d = json.loads(out)


def calls(side):
    for s in d[side]["symbols"]:
        if s["name"] == sym:
            r = []
            for ins in s.get("instructions", []):
                f = (ins.get("instruction") or {}).get("formatted", "") if isinstance(ins.get("instruction"), dict) else ""
                if f.startswith("bl "):
                    r.append(f[3:].split("__")[0][:40])
            return r
    return []


a, b = calls("left"), calls("right")
sm = difflib.SequenceMatcher(a=a, b=b, autojunk=False)
print("calls: original %d, ours %d, ratio %.3f" % (len(a), len(b), sm.ratio()))
for tag, i1, i2, j1, j2 in sm.get_opcodes():
    if tag != "equal":
        print("%-7s orig[%d:%d] %s | ours[%d:%d] %s" % (tag, i1, i2, a[i1:i2][:6], j1, j2, b[j1:j2][:6]))
