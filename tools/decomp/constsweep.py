#!/usr/bin/env python3
"""constsweep.py [--min-fuzzy 90] [--max N] [func...]: for `extern "C"` functions below 100 % with `const` pointer or
reference parameters, try each parameter without its `const` (and all of them at once) and report the variants
that raise the function. A `const` parameter lets MWCC move its loads above stores (D2A_Texture fn_801FE090).
Only the declarations of the same file are changed; nothing is kept."""
import json, re, os, subprocess, sys
os.chdir(os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..")))
args = sys.argv[1:]
def opt(n, d, c=str):
    if n in args:
        i = args.index(n); v = args[i + 1]; del args[i:i + 2]; return c(v)
    return d
mf = opt("--min-fuzzy", 90.0, float); mx = opt("--max", 999, int)
d = json.load(open("build/SLSP01/report.json"))
P = {}
for u in d["units"]:
    for f in u.get("functions", []):
        P[f["name"]] = (u["name"][5:], f.get("fuzzy_match_percent", 0), int(f.get("size", 0)))

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

def split_params(p):
    out, depth, cur = [], 0, ""
    for c in p:
        if c in "(<":
            depth += 1
        elif c in ")>":
            depth -= 1
        if c == "," and depth == 0:
            out.append(cur); cur = ""
        else:
            cur += c
    out.append(cur)
    return out

cands = []
for un in sorted(set(v[0] for v in P.values())):
    p = "src/%s.cpp" % un
    if not os.path.exists(p):
        continue
    s = open(p, encoding="latin-1").read()
    for m in re.finditer(r'^extern "C" [^\n;{]*?\b(fn_[0-9A-F]{8})\(([^)]*)\)\s*\{', s, re.M):
        name = m.group(1)
        if name in P and P[name][0] == un and mf <= P[name][1] < 100 and "const " in m.group(2):
            if not args or name in args:
                cands.append((P[name][2], un, name))
cands.sort(reverse=True)
for sz, un, name in cands[:mx]:
    p = "src/%s.cpp" % un
    orig = open(p, encoding="latin-1").read()
    subprocess.run(["ninja", "-j1", "build/SLSP01/src/%s.o" % un], capture_output=True)
    base = pct(un, name)
    pat = re.compile(r'(extern "C" [^\n;{]*?\b%s\()([^)]*)(\))' % name)
    params = None
    for m in pat.finditer(orig):
        params = split_params(m.group(2)); break
    idx = [i for i, q in enumerate(params) if "const " in q and ("*" in q or "&" in q)]
    variants = [[i] for i in idx] + ([idx] if len(idx) > 1 else [])
    res = []
    for v in variants:
        def rep(m):
            ps = split_params(m.group(2))
            if len(ps) != len(params):
                return m.group(0)
            for i in v:
                ps[i] = ps[i].replace("const ", "", 1)
            return m.group(1) + ",".join(ps) + m.group(3)
        new = pat.sub(rep, orig)
        open(p, "w", encoding="latin-1").write(new)
        r = subprocess.run(["ninja", "-j1", "build/SLSP01/src/%s.o" % un], capture_output=True, text=True)
        val = pct(un, name) if r.returncode == 0 else -1
        res.append((val, v))
    open(p, "w", encoding="latin-1").write(orig)
    subprocess.run(["ninja", "-j1", "build/SLSP01/src/%s.o" % un], capture_output=True)
    best = max(res) if res else (-1, None)
    flag = "UP" if best[0] > base + 1e-6 else "  "
    print("%s %6d %-28s %-12s base %.3f best %.3f %s" % (flag, sz, un, name, base, best[0], best[1]), flush=True)
