#!/usr/bin/env python3
"""twins.py [--min 2000] [--fuzzy 99] [--top 3]: for each function of size >= --min below 100 % (fuzzy >= --fuzzy),
lists the most similar target functions of the whole DOL (mnemonic sequence, difflib ratio) and their match
percent: a twin at 100 % is a source to copy the shape from (family of enemies, template copies...).
Cache of the mnemonics in build/twins.pkl (delete it after a new split)."""
import json, os, subprocess, sys, pickle, difflib, glob, re
os.chdir(os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..")))
a = sys.argv[1:]
def opt(n, d, c=str):
    if n in a:
        i = a.index(n); v = a[i + 1]; del a[i:i + 2]; return c(v)
    return d
mn = opt("--min", 2000, int); fz = opt("--fuzzy", 99.0, float); top = opt("--top", 3, int)
only = opt("--func", None)
OBJ = "build/binutils/powerpc-eabi-objdump"
cache = "build/twins.pkl"
if os.path.exists(cache):
    M = pickle.load(open(cache, "rb"))
else:
    M = {}
    for o in glob.glob("build/SLSP01/obj/**/*.o", recursive=True):
        u = o[len("build/SLSP01/obj/"):-2]
        out = subprocess.run([OBJ, "-d", "--no-show-raw-insn", o], capture_output=True, text=True).stdout
        cur = None
        for l in out.splitlines():
            m = re.match(r"^[0-9a-f]+ <(.+)>:$", l)
            if m:
                cur = (u, m.group(1)); M[cur] = []; continue
            m = re.match(r"^\s+[0-9a-f]+:\s+(\S+)", l)
            if m and cur:
                M[cur].append(m.group(1))
    pickle.dump(M, open(cache, "wb"))
d = json.load(open("build/SLSP01/report.json"))
P = {}
for u in d["units"]:
    for f in u.get("functions", []):
        P[(u["name"][5:], f["name"])] = (f.get("fuzzy_match_percent", 0), int(f.get("size", 0)))
cands = [k for k, (p, s) in P.items() if s >= mn and fz <= p < 100 and (only is None or only in k[1])]
cands.sort(key=lambda k: -P[k][1])
byLen = sorted(M.items(), key=lambda kv: len(kv[1]))
for k in cands:
    seq = M.get(k)
    if not seq:
        continue
    n = len(seq); res = []
    for k2, s2 in byLen:
        tol = float(os.environ.get("TWLEN", "0.15"))
        if k2 == k or not ((1 - tol) * n <= len(s2) <= (1 + tol) * n):
            continue
        sm = difflib.SequenceMatcher(None, seq, s2, autojunk=False)
        if sm.real_quick_ratio() < float(os.environ.get("TWMIN", "0.8")) or sm.quick_ratio() < float(os.environ.get("TWMIN", "0.8")):
            continue
        r = sm.ratio()
        if r >= float(os.environ.get("TWMIN", "0.8")):
            res.append((r, k2))
    res.sort(reverse=True)
    if res:
        print("%s %s (%d B, %.2f):" % (k[0], k[1][:60], P[k][1], P[k][0]))
        for r, k2 in res[:top]:
            p = P.get(k2, (None, 0))[0]
            print("    %.3f %s %s %s" % (r, k2[0], k2[1][:70], "%.2f" % p if p is not None else "?"))
