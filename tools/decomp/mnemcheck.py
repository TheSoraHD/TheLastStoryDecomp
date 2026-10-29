#!/usr/bin/env python3
# mnemcheck.py <unit> [-a]: compares the mnemonic of every instruction (target vs ours) per function.
# objdiff can report 100% for a function whose only difference is the branch condition (beq/bne),
# so check the functions it gives at 100% with this too. Without -a only problems are printed.
import json, os, subprocess, sys

root = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..")
unit = sys.argv[1]
show_all = "-a" in sys.argv[2:]
out = subprocess.run(["build/tools/objdiff-cli", "diff", "-p", ".", "-u", "main/" + unit, "-o", "-", "--format", "json"],
                     capture_output=True, text=True, cwd=root).stdout
d = json.loads(out)
ours = {s["name"]: s for s in d["right"]["symbols"]}
for left in d["left"]["symbols"]:
    right = ours.get(left["name"])
    if "instructions" not in left or not right or "instructions" not in right:
        continue
    a = [i.get("instruction", {}).get("formatted", "").split(" ")[0] for i in left["instructions"]]
    b = [i.get("instruction", {}).get("formatted", "").split(" ")[0] for i in right["instructions"]]
    bad = [k for k, (x, y) in enumerate(zip(a, b)) if x != y]
    if len(a) != len(b):
        bad.append(-1)
    pct = right.get("match_percent")
    if show_all or (bad and pct == 100.0):
        print("%-60s %s %s" % (left["name"][:60], pct, "OK" if not bad else "MNEMONIC-DIFF at %s" % bad[:5]))
