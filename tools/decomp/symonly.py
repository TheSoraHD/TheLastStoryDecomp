#!/usr/bin/env python3
"""symonly.py [report.json] [--min N]: functions below 100 % whose objdiff rows differ only in symbol names
(same mnemonic and registers, another relocation target): candidates for a rename in symbols.txt.
Prints unit, function, size, number of differing rows, rows that differ only in a symbol, and examples."""
import json, subprocess, sys, os, re
os.chdir(os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..")))
args = sys.argv[1:]
mn = 0
if "--min" in args:
    i = args.index("--min"); mn = int(args[i + 1]); del args[i:i + 2]
rep = args[0] if args else "build/SLSP01/report.json"
d = json.load(open(rep))
units = {}
for u in d["units"]:
    for f in u.get("functions", []):
        if int(f.get("size", 0)) >= mn and 0 < f.get("fuzzy_match_percent", 0) < 100:
            units.setdefault(u["name"][5:], []).append((f["name"], int(f["size"])))
fmt = lambda x: (x.get("instruction") or {}).get("formatted", "") if isinstance(x.get("instruction"), dict) else ""
sym = re.compile(r"[@A-Za-z_$.][^\s,()]*(@(ha|l|sda21|h))?$")
def strip(s):
    return re.sub(r"(lbl_|fn_|@|jumptable_)[^\s,()]*|[A-Za-z_][\w<>,:$.]*__[^\s,()]*|\"[^\"]*\"", "SYM", s)
for un, fl in sorted(units.items()):
    o = subprocess.run(["build/tools/objdiff-cli", "diff", "-p", ".", "-u", "main/" + un, "-o", "-", "--format", "json"],
                       capture_output=True, text=True).stdout
    try: dd = json.loads(o)
    except Exception: continue
    S = {side: {s["name"]: s.get("instructions", []) for s in dd.get(side, {}).get("symbols", [])} for side in ("left", "right")}
    for fn, sz in fl:
        L = S["left"].get(fn); R = S["right"].get(fn)
        if L is None or R is None: continue
        tot = only = 0; ex = []
        for i in range(max(len(L), len(R))):
            a = L[i] if i < len(L) else {}; b = R[i] if i < len(R) else {}
            if not (a.get("diff_kind") or b.get("diff_kind")): continue
            tot += 1
            fa, fb = fmt(a), fmt(b)
            if fa and fb and fa != fb and strip(fa) == strip(fb):
                only += 1
                if len(ex) < 2: ex.append("%s | %s" % (fa, fb))
        if tot and only >= tot - int(os.environ.get("SLACK", "0")) and only:
            print("%s %s %d rows=%d  %s" % (un, fn, sz, tot, " ;; ".join(ex)))
