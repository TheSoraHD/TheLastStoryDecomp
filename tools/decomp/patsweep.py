#!/usr/bin/env python3
"""patsweep.py <regex> <replacement> [--units u1,u2] [--min P] [--helper TEXT] [--log F (resume)]: one rewrite at every match.

For each source line matching <regex> inside a function of the game below 100 % (report.json of the copy,
fuzzy >= --min, default 90), applies re.sub(<regex>, <replacement>) to that line only, adds --helper after the
last #include when given (default: the identity inline Val_unk), rebuilds the unit and prints the percent of
the function before -> after. The source is always restored. (bloqueos-20261122: the read of
lbl_8087F430->m10D8 through an inline with a parameter closed three functions.)
"""
import json, os, re, subprocess, sys
root = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))
os.chdir(root)
args = sys.argv[1:]
def opt(n, d):
    if n in args:
        i = args.index(n); v = args[i + 1]; del args[i:i + 2]; return v
    return d
units = opt("--units", None)
mn = float(opt("--min", 90))
mx = float(opt("--max", 100))
whole = "--all" in args
args = [a for a in args if a != "--all"]
logf = opt("--log", None)
done = set()
if logf and os.path.exists(logf):
    for l in open(logf):
        w = l.split()
        if len(w) > 2: done.add((w[0], w[2]))
logh = open(logf, "a") if logf else None
helper = opt("--helper", "template <class T> inline T Val_unk(const T& v) { return v; } // inferred: read through an inline")
pat, rep = re.compile(args[0]), args[1]
import signal
cur = [None, None]
def restore(*a):
    if cur[0]:
        open(cur[0], "w", encoding="latin-1").write(cur[1])
    raise SystemExit(1)
signal.signal(signal.SIGTERM, restore)
r = json.load(open("build/SLSP01/report.json"))

def pct(u, sym):
    if subprocess.run(["ninja", "-j1", "build/SLSP01/src/%s.o" % u], capture_output=True).returncode:
        return -1
    o = subprocess.run(["build/tools/objdiff-cli", "diff", "-p", ".", "-u", "main/" + u, sym, "-o", "-", "--format", "json"],
                       capture_output=True, text=True).stdout
    for s in json.loads(o)["left"]["symbols"]:
        if s["name"] == sym:
            return s.get("match_percent") or 0
    return -1

for u in r["units"]:
    un = u["name"][5:]
    src = "src/%s.cpp" % un
    if not os.path.exists(src) or (units and un not in units.split(",")):
        continue
    text = open(src, encoding="latin-1").read()
    if not pat.search(text):
        continue
    cur[0], cur[1] = src, text
    lines = text.split("\n")
    lastinc = max((k for k, l in enumerate(lines) if l.startswith("#include")), default=0)
    for f in u.get("functions", []):
        p = f.get("fuzzy_match_percent", 0)
        dn = f.get("metadata", {}).get("demangled_name", "")
        if not (mn <= p < min(mx, 100)) or not dn:
            continue
        name = dn.split("(")[0]
        short = name.split("::")[-1]
        # body of the function: the definition line not indented that contains Class::name( or name(
        cands = [name.split("::", 1)[1] if name.count("::") > 1 else name, short]
        st = None
        for c in cands:
            for k, l in enumerate(lines):
                if not l.startswith((" ", "\t", "/")) and re.search(r"(?<![\w:])%s\s*\(" % re.escape(c), l) and not l.rstrip().endswith(";"):
                    st = k; break
            if st is not None:
                break
        if st is None:
            continue
        en = next((k for k in range(st + 1, len(lines)) if lines[k].startswith("}")), None)
        if en is None:
            continue
        ks = [k for k in range(st, en) if pat.search(lines[k]) and (un, str(k + 1)) not in done]
        if whole and ks:
            ks = [ks]
        for k in ks:
            new = lines[:]
            for kk in (k if whole else [k]):
                new[kk] = pat.sub(rep, lines[kk])
            if whole:
                k = k[0]
            if helper and helper.split("(")[0] not in text:
                new[lastinc] += "\n" + helper
            open(src, "w", encoding="latin-1").write("\n".join(new))
            v = pct(un, f["name"])
            res = "%-28s %-50s %d %.3f -> %.3f" % (un, f["name"][:50], k + 1, p, v)
            print(res, flush=True)
            if logh: logh.write(res + "\n"); logh.flush()
        open(src, "w", encoding="latin-1").write(text)
    subprocess.run(["ninja", "-j1", "build/SLSP01/src/%s.o" % un], capture_output=True)
