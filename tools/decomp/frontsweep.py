#!/usr/bin/env python3
"""frontsweep.py [--min P] [--log F]: the rotated unit vector through an inline (bloqueos-20261122).

In the functions of the game below 100 % (fuzzy >= --min), finds the block
    atn::Vector3 d(0.0f, 0.0f, 1.0f);            (or `atn::Vector3 d = atn::Vector3(0.0f, 0.0f, 1.0f);`)
    Mtx m; / atn::Matrix3x4 m;
    PSMTXRotRad(m, 'y', A); / m.SetRotationY_unk(A);
    PSMTXMultVec(m(.m), &d, &d);
and tries (1) `atn::Vector3 d = FrontY_unk(A);` and, when d is used once afterwards, (2) the call in place of
that use; both with the angle by value and by const reference. Prints the percent before -> after; the
source is restored. ControlCliffhang vf0C: the vector of the inline goes to the bottom of the stack frame.
"""
import json, os, re, subprocess, sys, signal
root = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))
os.chdir(root)
args = sys.argv[1:]
def opt(n, d):
    if n in args:
        i = args.index(n); v = args[i + 1]; del args[i:i + 2]; return v
    return d
mn = float(opt("--min", 97)); logf = opt("--log", None)
done = set()
if logf and os.path.exists(logf):
    done = set(" ".join(l.split()[:3]) for l in open(logf))
logh = open(logf, "a") if logf else None
HELP = {
    "v": "static inline atn::Vector3 FrontY_unk(float yaw) { atn::Vector3 d(0.0f, 0.0f, 1.0f); Mtx m; PSMTXRotRad(m, 'y', yaw); PSMTXMultVec(m, &d, &d); return d; } // inferred: unit vector of a facing",
    "r": "static inline atn::Vector3 FrontY_unk(const float& yaw) { atn::Vector3 d(0.0f, 0.0f, 1.0f); Mtx m; PSMTXRotRad(m, 'y', yaw); PSMTXMultVec(m, &d, &d); return d; } // inferred: unit vector of a facing",
}
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
D = re.compile(r"^(\s+)atn::Vector3 (\w+)(?:\(0\.0f, 0\.0f, 1\.0f\)| = atn::Vector3\(0\.0f, 0\.0f, 1\.0f\));\s*$")
for u in r["units"]:
    un = u["name"][5:]
    src = "src/%s.cpp" % un
    if not os.path.exists(src):
        continue
    text = open(src, encoding="latin-1").read()
    if "0.0f, 0.0f, 1.0f)" not in text:
        continue
    cur[0], cur[1] = src, text
    lines = text.split("\n")
    lastinc = max((k for k, l in enumerate(lines) if l.startswith("#include")), default=0)
    for f in u.get("functions", []):
        p = f.get("fuzzy_match_percent", 0)
        dn = f.get("metadata", {}).get("demangled_name", "")
        if not (mn <= p < 100) or not dn:
            continue
        name = dn.split("(")[0]
        cands = [name.split("::", 1)[1] if name.count("::") > 1 else name, name.split("::")[-1]]
        st = None
        for c in cands:
            for k, l in enumerate(lines):
                if not l.startswith((" ", "\t", "/")) and re.search(r"(?<![\w:])%s\s*\(" % re.escape(c), l) and not l.rstrip().endswith(";"):
                    st = k; break
            if st is not None:
                break
        if st is None:
            continue
        en = next((k for k in range(st + 1, len(lines)) if lines[k].startswith("}")), len(lines))
        for k in range(st, en - 3):
            m = D.match(lines[k])
            if not m:
                continue
            ind, d = m.groups()
            mm = re.match(r"^\s+(?:Mtx|atn::Matrix3x4) (\w+);\s*$", lines[k + 1])
            if not mm:
                continue
            mx = mm.group(1)
            ma = re.match(r"^\s+(?:PSMTXRotRad\(%s, 'y', (.+)\)|%s\.SetRotationY_unk\((.+)\));\s*$" % (mx, mx), lines[k + 2])
            mv = re.match(r"^\s+PSMTXMultVec\(%s(?:\.m)?, &%s, &%s\);\s*$" % (mx, d, d), lines[k + 3])
            if not ma or not mv:
                continue
            ang = ma.group(1) or ma.group(2)
            rest = "\n".join(lines[k + 4:en])
            uses = [j for j in range(k + 4, en) if re.search(r"\b%s\b" % d, lines[j])]
            vars_ = [("named", None)]
            if len(re.findall(r"\b%s\b" % d, rest)) == 1:
                vars_.append(("inplace", uses[0]))
            for kind, j in vars_:
                for h in ("v", "r"):
                    key = "%s %s %d%s%s" % (un, f["name"][:40], k + 1, kind[0], h)
                    if " ".join(key.split()[:3]) in done:
                        continue
                    new = lines[:]
                    if kind == "named":
                        new[k] = "%satn::Vector3 %s = FrontY_unk(%s);" % (ind, d, ang)
                        new[k + 1] = new[k + 2] = new[k + 3] = None
                    else:
                        new[j] = re.sub(r"\b%s\b" % d, lambda _m: "FrontY_unk(%s)" % ang, lines[j], 1)
                        new[k] = new[k + 1] = new[k + 2] = new[k + 3] = None
                    if "FrontY_unk(" not in text:
                        new[lastinc] += "\n" + HELP[h]
                    open(src, "w", encoding="latin-1").write("\n".join(x for x in new if x is not None))
                    v = pct(un, f["name"])
                    res = "%s %.3f -> %.3f" % (key, p, v)
                    print(res, flush=True)
                    if logh: logh.write(res + "\n"); logh.flush()
        open(src, "w", encoding="latin-1").write(text)
    subprocess.run(["ninja", "-j1", "build/SLSP01/src/%s.o" % un], capture_output=True)
