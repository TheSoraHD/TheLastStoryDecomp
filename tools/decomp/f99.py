#!/usr/bin/env python3
"""f99.py [report.json] [--min 99] [--max 100] [--out tsv]: functions between min and max % with unit,
functions the unit lacks to be fully matched, and kind of diff (reg/stack/const/order/other)."""
import json, subprocess, sys, os, re, collections
R = "/home/claude/TheLastStoryDecomp"
os.chdir(os.environ.get("CMP_ROOT", R))
args = sys.argv[1:]
rep = args[0] if args and not args[0].startswith("--") else "build/SLSP01/report.json"
lo = float(args[args.index("--min") + 1]) if "--min" in args else 99.0
hi = float(args[args.index("--max") + 1]) if "--max" in args else 100.0
out = args[args.index("--out") + 1] if "--out" in args else None
r = json.load(open(rep))
REG = re.compile(r"^[rf]\d+$|^cr\d$")

def classify(unit, names):
    p = subprocess.run(["build/tools/objdiff-cli", "diff", "-p", ".", "-u", unit, "-o", "/tmp/f0.json", "--format", "json"],
                       capture_output=True, text=True)
    try:
        d = json.load(open("/tmp/f0.json"))
    except Exception:
        return {}
    L = {s["name"]: s for s in d["left"]["symbols"]}
    Rr = {s.get("name"): s for s in d["right"]["symbols"]}
    res = {}
    for n in names:
        s = L.get(n)
        if not s:
            res[n] = "?"; continue
        t = Rr.get(n) or {}
        li = s.get("instructions") or []; ri = t.get("instructions") or []
        kinds = collections.Counter()
        for i in range(max(len(li), len(ri))):
            a = li[i] if i < len(li) else {}; b = ri[i] if i < len(ri) else {}
            k = a.get("diff_kind") or b.get("diff_kind")
            if not k: continue
            if k in ("DIFF_INSERT", "DIFF_DELETE", "DIFF_REPLACE", "DIFF_OP_MISMATCH"):
                kinds["order"] += 1; continue
            fa = ((a.get("instruction") or {}).get("formatted") or "")
            fb = ((b.get("instruction") or {}).get("formatted") or "")
            ta = re.split(r"[ ,()]+", fa.strip()); tb = re.split(r"[ ,()]+", fb.strip())
            if len(ta) != len(tb):
                kinds["other"] += 1; continue
            sub = "reg"
            for x, y in zip(ta, tb):
                if x == y: continue
                if REG.match(x) and REG.match(y): continue
                if "r1" in ta and re.match(r"^-?(0x)?[0-9a-fA-F]+$", x):
                    sub = "stack"; continue
                sub = "const"; break
            kinds[sub] += 1
        res[n] = ",".join("%s%d" % (k, v) for k, v in kinds.most_common()) or "none"
    return res

rows = []
for u in r["units"]:
    fs = u.get("functions") or []
    if not fs: continue
    below = [f for f in fs if (f.get("fuzzy_match_percent") or 0) < 100]
    if not below: continue
    sel = [f for f in below if lo <= (f.get("fuzzy_match_percent") or 0) < hi]
    if not sel: continue
    cl = classify(u["name"], [f["name"] for f in sel])
    for f in sel:
        rows.append((u["name"][5:], len(below), int(f["size"]), f["fuzzy_match_percent"],
                     f["metadata"].get("demangled_name", f["name"]), f["name"], hex(int(f["metadata"]["virtual_address"])), cl.get(f["name"], "?")))
rows.sort(key=lambda x: (x[1], x[0], x[6]))
lines = ["%s\t%d\t%d\t%.2f\t%s\t%s\t%s" % (a, b, c, d, g, h, e) for a, b, c, d, e, f2, g, h in rows]
if out:
    open(out, "w").write("unit\tfaltan\tsize\tpct\taddr\tdiff\tfunc\n" + "\n".join(lines) + "\n")
print(len(rows), "funciones;", len(set(x[0] for x in rows)), "unidades")
c = collections.Counter(x[1] for x in rows)
print("por faltan:", sorted(c.items())[:10])
k = collections.Counter(x[7].split(",")[0].rstrip("0123456789") for x in rows)
print("por tipo dominante:", k.most_common())
