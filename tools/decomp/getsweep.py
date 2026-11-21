#!/usr/bin/env python3
"""getsweep.py <regex> <replacement> [unit...]: for every match of <regex> in src/<unit>.cpp (each one alone),
rewrite it (re.sub replacement, e.g. bitfield read -> getter) and compare the objdiff percents of the whole unit
with the original: prints the variants that raise a function (and what they lower). Nothing is kept.
Without units: every unit whose source matches."""
import json, re, os, subprocess, sys, glob
os.chdir(os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..")))
pat, rep = re.compile(sys.argv[1]), sys.argv[2]
units = sys.argv[3:]
if not units:
    for p in glob.glob("src/**/*.cpp", recursive=True):
        if pat.search(open(p, encoding="latin-1").read()):
            units.append(p[4:-4])

def unit_pcts(u):
    o = subprocess.run(["build/tools/objdiff-cli", "diff", "-p", ".", "-u", "main/" + u, "-o", "-", "--format", "json"],
                       capture_output=True, text=True).stdout
    try:
        d = json.loads(o)
    except Exception:
        return {}
    return {s["name"]: (s.get("match_percent") or 0, int(s.get("size", 0))) for s in d["left"]["symbols"]
            if "instructions" in s}

for u in units:
    p = "src/%s.cpp" % u
    s = open(p, encoding="latin-1").read()
    ms = list(pat.finditer(s))
    if not ms:
        continue
    subprocess.run(["ninja", "-j1", "build/SLSP01/src/%s.o" % u], capture_output=True)
    base = unit_pcts(u)
    for m in ms:
        new = s[:m.start()] + m.expand(rep) + s[m.end():]
        open(p, "w", encoding="latin-1").write(new)
        r = subprocess.run(["ninja", "-j1", "build/SLSP01/src/%s.o" % u], capture_output=True, text=True)
        line = s.count("\n", 0, m.start()) + 1
        if r.returncode != 0:
            print("%s:%d build fail" % (u, line)); continue
        now = unit_pcts(u)
        up = [(k, base[k][0], v[0]) for k, v in now.items() if k in base and v[0] > base[k][0] + 1e-6]
        dn = [(k, base[k][0], v[0]) for k, v in now.items() if k in base and v[0] < base[k][0] - 1e-6]
        if up:
            print("UP %s:%d %s | down %s" % (u, line, " ".join("%s %.3f->%.3f" % x for x in up),
                                             " ".join("%s %.3f->%.3f" % x for x in dn)[:300]), flush=True)
    open(p, "w", encoding="latin-1").write(s)
    subprocess.run(["ninja", "-j1", "build/SLSP01/src/%s.o" % u], capture_output=True)
