#!/usr/bin/env python3
"""constsweep2.py [--min-fuzzy 98] [--max N] [--start K]: like constsweep.py, for C++ functions (members or
mangled free functions) whose name cannot change cheaply: the test does not touch the signature, it gives the body
a non-const copy of each `const T*` parameter (`T* p_nc = const_cast<T*>(p);`, uses renamed) and measures.
A winner means the real parameter is not const: change the declaration and rename the symbol by hand."""
import json, re, os, subprocess, sys
os.chdir(os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..")))
args = sys.argv[1:]
def opt(n, d, c=str):
    if n in args:
        i = args.index(n); v = args[i + 1]; del args[i:i + 2]; return c(v)
    return d
mf = opt("--min-fuzzy", 98.0, float); mx = opt("--max", 999, int); st = opt("--start", 0, int)
d = json.load(open("build/SLSP01/report.json"))
F = []
for u in d["units"]:
    for f in u.get("functions", []):
        p = f.get("fuzzy_match_percent", 0)
        if mf <= p < 100 and int(f.get("size", 0)) >= 200:
            F.append((int(f["size"]), u["name"][5:], f["name"], p))
F.sort(reverse=True)

def pct(unit, sym):
    o = subprocess.run(["build/tools/objdiff-cli", "diff", "-p", ".", "-u", "main/" + unit, sym, "-o", "-",
                        "--format", "json"], capture_output=True, text=True).stdout
    try:
        dd = json.loads(o)
    except Exception:
        return -1
    for s in dd["left"]["symbols"]:
        if s["name"] == sym:
            return s.get("match_percent") or 0
    return -1

n = 0
for sz, un, sym, p in F:
    m = re.match(r"^(\w+?)__(\d+)(\w+?)F", sym)
    if not m or not (sym.startswith("fn_") or sym.startswith("vf")):
        continue
    fname = m.group(3)[:int(m.group(2))] + "::" + m.group(1)
    path = "src/%s.cpp" % un
    if not os.path.exists(path):
        continue
    s = open(path, encoding="latin-1").read()
    dm = re.search(r"^[^\n;]*\b" + re.escape(fname) + r"\(([^)]*)\)\s*(const\s*)?\{", s, re.M)
    if not dm:
        continue
    params = [q.strip() for q in dm.group(1).split(",")]
    cps = []
    for q in params:
        mm = re.match(r"^const ([\w:<>]+)\s*\*\s*(\w+)$", q)
        if mm:
            cps.append((mm.group(1), mm.group(2)))
    if not cps:
        continue
    n += 1
    if n <= st:
        continue
    if n > st + mx:
        break
    body_start = dm.end()
    end = s.index("\n}\n", body_start)
    subprocess.run(["ninja", "-j1", "build/SLSP01/src/%s.o" % un], capture_output=True)
    base = pct(un, sym)
    res = []
    for typ, name in cps:
        body = s[body_start:end]
        body2 = re.sub(r"(?<![\w.>])%s\b" % name, name + "_nc", body)
        new = s[:body_start] + "\n    %s* %s_nc = const_cast<%s*>(%s);" % (typ, name, typ, name) + body2 + s[end:]
        open(path, "w", encoding="latin-1").write(new)
        r = subprocess.run(["ninja", "-j1", "build/SLSP01/src/%s.o" % un], capture_output=True, text=True)
        res.append((pct(un, sym) if r.returncode == 0 else -1, name))
    open(path, "w", encoding="latin-1").write(s)
    subprocess.run(["ninja", "-j1", "build/SLSP01/src/%s.o" % un], capture_output=True)
    best = max(res)
    print("%s %6d %-26s %-40s base %.3f best %.3f %s" % ("UP" if best[0] > base + 1e-6 else "  ", sz, un, fname[:40],
          base, best[0], best[1]), flush=True)
