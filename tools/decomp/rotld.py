#!/usr/bin/env python3
"""rotld.py [regex]: for every line matching the pattern (default: PSMTXRotRad(m, 'y', a.b.y);) inside a
function below 100 %, try reading the angle into a float local just before it; print the functions that improve.
Runs in the main copy; restores every source."""
import json, os, re, subprocess, sys
os.chdir(os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..")))
PAT = re.compile(sys.argv[1] if len(sys.argv) > 1 else r"^(\s+)(.*PSMTXRotRad\([^,]*, '[xyz]', )([A-Za-z_][\w.>\[\]-]*\.[xyz])(\);.*)$")
d = json.load(open("build/SLSP01/report.json"))
fun = {}
for u in d["units"]:
    for f in u.get("functions", []):
        p = f.get("fuzzy_match_percent", 0)
        dm = f.get("metadata", {}).get("demangled_name") or f["name"]
        fun.setdefault(u["name"][5:], []).append((dm.split("(")[0], f["name"], p))

def pct(unit, sym):
    r = subprocess.run(["ninja", "-j1", "build/SLSP01/src/%s.o" % unit], capture_output=True, text=True)
    if r.returncode:
        return -1
    o = subprocess.run(["build/tools/objdiff-cli", "diff", "-p", ".", "-u", "main/" + unit, sym, "-o", "-", "--format", "json"],
                       capture_output=True, text=True).stdout
    try:
        for s in json.loads(o)["left"]["symbols"]:
            if s["name"] == sym:
                return s.get("match_percent") or 0
    except Exception:
        pass
    return -1

START = os.environ.get("START", "")
for unit in sorted(fun):
    if unit < START:
        continue
    src = "src/%s.cpp" % unit
    if not os.path.exists(src):
        continue
    text = open(src, encoding="latin-1").read()
    lines = text.split("\n")
    sites = [i for i, l in enumerate(lines) if PAT.match(l)]
    if not sites:
        continue
    for i in sites:
        # enclosing function: last top-level line with `name(` before i
        j = i
        while j >= 0 and not (re.match(r"^[A-Za-z_].*\b([\w:~]+)\s*\(", lines[j]) and not lines[j].rstrip().endswith(";")):
            j -= 1
        if j < 0:
            continue
        m = re.search(r"([\w:~]+)\s*\(", lines[j])
        name = m.group(1)
        cands = [x for x in fun[unit] if x[0] == name or x[0].endswith("::" + name) or x[1] == name]
        cands = [x for x in cands if x[2] < 100]
        if len(cands) != 1:
            continue
        sym, p0 = cands[0][1], cands[0][2]
        mm = PAT.match(lines[i])
        ind, pre, chain, post = mm.groups()
        new = lines[:]
        new[i] = "%sfloat ry_ld = %s;\n%s%sry_ld%s" % (ind, chain, ind, pre, post)
        base = pct(unit, sym)
        open(src, "w", encoding="latin-1").write("\n".join(new))
        v = pct(unit, sym)
        open(src, "w", encoding="latin-1").write(text)
        flag = "BETTER" if v > base + 1e-6 else ""
        print("%s %s %s line %d base %.3f -> %.3f %s" % (flag, unit, sym[:60], i + 1, base, v, chain), flush=True)
    subprocess.run(["ninja", "-j1", "build/SLSP01/src/%s.o" % unit], capture_output=True)
