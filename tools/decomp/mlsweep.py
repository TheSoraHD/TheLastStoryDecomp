#!/usr/bin/env python3
"""mlsweep.py <regex> <replacement> --helper TEXT [--units u1,u2] [--min P] [--max P] [--log F]: like patsweep.py
but the regex may span several lines (re.M over the whole source). Each match is rewritten alone; the helper is
added after the last #include; the unit is rebuilt and every function of the unit whose percent changes is
printed (before -> after; @N renumbering can lower other functions by a few hundredths). The source is always
restored. (accesores-20261126: `Vector3 d = a - b; PSVECSquareMag(&d)` through an inline closed the stack
of ChaserGuard::fn_8017E650.)"""
import json, os, re, subprocess, sys
args = sys.argv[1:]
def opt(n, d):
    if n in args:
        i = args.index(n); v = args[i + 1]; del args[i:i + 2]; return v
    return d
units = opt("--units", None)
mn = float(opt("--min", 95)); mx = float(opt("--max", 100))
helper = opt("--helper", "")
logf = opt("--log", None)
rx, rep = args[0], args[1]
done = set()
if logf and os.path.exists(logf):
    for l in open(logf):
        if l.startswith("#"):
            done.add(l.split()[1])
logh = open(logf, "a") if logf else None
def out(s):
    print(s, flush=True)
    if logh:
        logh.write(s + "\n"); logh.flush()
rep_ = json.load(open("build/SLSP01/report.json"))
P = {}
for u in rep_["units"]:
    un = u["name"][5:]
    P[un] = {f["name"]: f.get("fuzzy_match_percent", 0) for f in u.get("functions", [])}
def pcts(un):
    o = subprocess.run(["build/tools/objdiff-cli", "diff", "-p", ".", "-u", "main/" + un, "-o", "-", "--format", "json"],
                       capture_output=True, text=True).stdout
    try:
        d = json.loads(o)
    except Exception:
        return {}
    return {s["name"]: (s.get("match_percent") or 0) for s in d.get("left", {}).get("symbols", []) if "match_percent" in s}
cands = [un for un, fs in P.items() if any(mn <= p < mx for p in fs.values()) and os.path.exists("src/%s.cpp" % un)]
if units:
    cands = [u for u in cands if u in units.split(",")]
R = re.compile(rx, re.M)
for un in sorted(cands):
    src = "src/%s.cpp" % un
    orig = open(src, encoding="latin-1").read()
    ms = list(R.finditer(orig))
    if not ms:
        continue
    base = None
    for k, m in enumerate(ms):
        key = "%s@%d" % (un, m.start())
        if key in done:
            continue
        if base is None:
            subprocess.run(["ninja", "-j1", "build/SLSP01/src/%s.o" % un], capture_output=True)
            base = pcts(un)
        new = orig[:m.start()] + m.expand(rep) + orig[m.end():]
        if helper:
            incs = [x for x in re.finditer(r"^#include .*$", new, re.M)]
            p = incs[-1].end() if incs else 0
            new = new[:p] + "\n" + helper + new[p:]
        open(src, "w", encoding="latin-1").write(new)
        r = subprocess.run(["ninja", "-j1", "build/SLSP01/src/%s.o" % un], capture_output=True, text=True)
        line = orig.count("\n", 0, m.start()) + 1
        if r.returncode != 0:
            out("# %s line %d BUILD FAIL" % (key, line))
        else:
            now = pcts(un)
            ch = ["%s %.3f->%.3f" % (n[:40], base.get(n, 0), v) for n, v in now.items()
                  if abs(v - base.get(n, 0)) > 0.05 and base.get(n, 0) < 100]
            out("# %s line %d %s" % (key, line, " | ".join(ch)))
        open(src, "w", encoding="latin-1").write(orig)
    if base is not None:
        subprocess.run(["ninja", "-j1", "build/SLSP01/src/%s.o" % un], capture_output=True)
