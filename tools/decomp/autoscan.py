#!/usr/bin/env python3
"""Inventory of the auto_ (unsplit) .text ranges, with hints for where to split them.

usage: autoscan.py [--min 0x400] [--md out.md] [--tsv out.tsv] [--unit auto_03_XXXXXXXX_text]
       autoscan.py --data [--tsv out.tsv]

Reads the extracted asm of every unit (build/SLSP01/asm/**.s, run `ninja` first) and prints, for each
auto_*_text range of the game/engine code:
  - size, functions, units before/after (with source or not);
  - classes: vtables (any .data) whose methods are in the range, named from their RTTI string;
    ctor/dtor candidates (functions that store the vtable address);
  - TU boundary candidates from duplicated constants: CodeWarrior keeps one copy of each .sdata2/.rodata
    constant per file, so two functions that load the same value from different addresses are in
    different files (a boundary lies between them);
  - __sinit functions next to the range (a __sinit is always the last function of its file).
With --data, lists the data objects still in auto_ data ranges (.data/.rodata/.sdata/.sdata2/.bss/.sbss)
and who references them: objects used only by the functions of one split unit are candidates to be
added to that unit's split (pools, constants, tables, statics), grouped by unit.
With --unit, prints per function: address, size, class (vtable), constant copies used, and data objects
it references that live in auto_ data ranges (candidates for that file's data).
"""
import os, re, sys, collections, bisect

ROOT = os.environ.get("CMP_ROOT", os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..")))
ASM = os.path.join(ROOT, "build", "SLSP01", "asm")
args = sys.argv[1:]
def opt(name, default=None):
    return args[args.index(name) + 1] if name in args else default
MIN = int(opt("--min", "0"), 0)
ONE = opt("--unit")

HDR = re.compile(r"^# \.(\w+):0x[0-9A-Fa-f]+ \| 0x([0-9A-Fa-f]+) \| size: 0x([0-9A-Fa-f]+)")
FN = re.compile(r"^\.fn (\S+),")
OBJ = re.compile(r"^\.obj (\S+),")
REF = re.compile(r"\b([A-Za-z_@$][\w@$.]*)@(?:ha|l|sda21)\b")
CALL = re.compile(r"\t(?:bl|b)\s+([A-Za-z_][\w@$.]*)\s*$")
CTORS = re.compile(r"^\.section \.ctors")

funcs = {}      # name -> (addr, size, unit)
objs = {}       # name -> dict(addr, size, sec, unit, words[], vals[])
frefs = collections.defaultdict(list)  # func -> [referenced names] (in order)
ctor_fns = set()
unit_src = {}

for dp, dn, fns in os.walk(ASM):
    for f in fns:
        if not f.endswith(".s"):
            continue
        path = os.path.join(dp, f)
        unit = os.path.relpath(path, ASM)[:-2]
        cur_f = cur_o = None; pend = None; in_ctors = False
        for line in open(path, encoding="utf-8", errors="replace"):
            m = HDR.match(line)
            if m:
                pend = (m.group(1), int(m.group(2), 16), int(m.group(3), 16)); continue
            if CTORS.match(line):
                in_ctors = True; continue
            if line.startswith(".section") or line.startswith(".text") or line.startswith(".data"):
                in_ctors = False
            m = FN.match(line)
            if m and pend:
                cur_f = m.group(1); funcs[cur_f] = (pend[1], pend[2], unit); continue
            if line.startswith(".endfn"):
                cur_f = None; continue
            m = OBJ.match(line)
            if m and pend:
                cur_o = m.group(1)
                objs[cur_o] = dict(addr=pend[1], size=pend[2], sec=pend[0], unit=unit, words=[], vals=[])
                continue
            if line.startswith(".endobj"):
                cur_o = None; continue
            if in_ctors and line.strip().startswith(".4byte"):
                ctor_fns.add(line.split()[1]); continue
            if cur_f:
                for r in REF.findall(line):
                    frefs[cur_f].append(r)
                m = CALL.search(line)
                if m and not m.group(1).startswith(".L"):
                    frefs[cur_f].append(m.group(1))
            elif cur_o:
                s = line.strip()
                if s.startswith(".4byte"):
                    objs[cur_o]["words"].append(s.split()[1])
                if s.startswith((".4byte", ".float", ".double", ".2byte", ".byte", ".string", ".8byte")):
                    objs[cur_o]["vals"].append(s)

# units with source: build/SLSP01/src/<unit>.o exists
srcdir = os.path.join(ROOT, "build", "SLSP01", "src")
def has_src(unit):
    return os.path.exists(os.path.join(srcdir, unit + ".o"))

def is_auto(unit):
    return os.path.basename(unit).startswith("auto_")

# class names from vtables: obj whose first word is an RTTI object (first word -> string)
def string_of(name):
    o = objs.get(name)
    if not o: return None
    for v in o["vals"]:
        if v.startswith(".string"):
            return v[len(".string"):].strip().strip('"')
    return None

fn_class = {}
vt_of_class = {}
for name, o in objs.items():
    if o["sec"] != "data" or len(o["words"]) < 3:
        continue
    w = o["words"]
    rt = objs.get(w[0])
    if not rt or not rt["words"]:
        continue
    cname = string_of(rt["words"][0])
    if not cname or not re.match(r"^[A-Za-z_][\w:<>, *&]*$", cname):
        continue
    methods = [x for x in w[2:] if x in funcs]
    if len(methods) < 1:
        continue
    vt_of_class[cname] = name
    for x in methods:
        fn_class.setdefault(x, cname)

# who references each vtable (ctor/dtor candidates)
vt_users = collections.defaultdict(set)
vtnames = {v: c for c, v in vt_of_class.items()}
for f, refs in frefs.items():
    for r in refs:
        if r in vtnames:
            vt_users[f].add(vtnames[r])

# duplicated constants (same section kind and same value at different addresses)
const_key = {}
groups = collections.defaultdict(set)
for name, o in objs.items():
    if o["sec"] not in ("sdata2", "rodata") or o["size"] > 8 or not o["vals"]:
        continue
    if any(v.startswith(".string") for v in o["vals"]):
        continue
    k = (o["sec"], o["size"], tuple(o["vals"]))
    const_key[name] = k
    groups[k].add(name)
dup = {n for k, ns in groups.items() if len(ns) > 1 for n in ns}

units_by_addr = sorted({(funcs[f][0], funcs[f][2]) for f in funcs})
fn_sorted = sorted(funcs, key=lambda f: funcs[f][0])
fn_addrs = [funcs[f][0] for f in fn_sorted]

# text ranges per unit
urange = {}
for f, (a, s, u) in funcs.items():
    lo, hi = urange.get(u, (a, a + s))
    urange[u] = (min(lo, a), max(hi, a + s))
uorder = sorted(urange, key=lambda u: urange[u][0])

def neighbours(u):
    i = uorder.index(u)
    return (uorder[i - 1] if i else None, uorder[i + 1] if i + 1 < len(uorder) else None)

def analyse(u):
    lo, hi = urange[u]
    fl = [f for f in fn_sorted if funcs[f][2] == u]
    classes = collections.OrderedDict()   # classes with own methods (vtable entries) in the range
    built = collections.OrderedDict()     # classes whose vtable is stored here but whose methods are elsewhere
    for f in fl:
        c = fn_class.get(f)
        if c: classes.setdefault(c, []).append(f)
    for f in fl:
        for c in vt_users.get(f, ()):
            if c not in classes: built.setdefault(c, [])
    # boundary candidates: walk functions, remember for each const value which copy was last used
    last = {}; bounds = []
    for f in fl:
        for r in frefs.get(f, ()):
            if r in dup:
                k = const_key[r]
                if k in last and last[k][1] != r:
                    bounds.append((funcs[last[k][0]][0], funcs[f][0], r))
                last[k] = (f, r)
    # merge to ranges (boundary is somewhere after func A and up to func B)
    rng = []
    for a, b, r in sorted(bounds):
        if rng and a < rng[-1][1]:
            rng[-1] = (max(rng[-1][0], a), min(rng[-1][1], b))
        else:
            rng.append((a, b))
    prev, nxt = neighbours(u)
    sinit = [f for f in fl if f in ctor_fns]
    return dict(lo=lo, hi=hi, n=len(fl), fl=fl, classes=classes, built=built, bounds=rng, prev=prev, next=nxt, sinit=sinit)

def short(u):
    if u is None: return "-"
    return u + ("" if is_auto(u) else (" (src)" if has_src(u) else " (asm)"))

if "--data" in args:
    users = collections.defaultdict(set)
    for f, refs in frefs.items():
        for r in refs:
            if r in objs: users[r].add(funcs[f][2])
    for name, o in objs.items():   # data -> data references (vtables, tables)
        for w in o["words"]:
            if w in objs and not is_auto(o["unit"]): users[w].add(o["unit"])
    per_unit = collections.defaultdict(list); shared = unused = 0; tot = collections.Counter()
    for name, o in objs.items():
        if not is_auto(o["unit"]) or o["sec"] in ("text", "ctors", "dtors", "init") or name.startswith("gap_"):
            continue
        us = users.get(name, set())
        tot[o["sec"]] += o["size"]
        if not us: unused += o["size"]; continue
        named = {u for u in us if not is_auto(u)}
        if len(us) == 1 and named:
            per_unit[next(iter(named))].append(name)
        else:
            shared += o["size"]
    print("bytes in auto_ data ranges:", {k: hex(v) for k, v in sorted(tot.items())})
    print(f"objects used by a single split unit: {sum(len(v) for v in per_unit.values())} in {len(per_unit)} units; "
          f"shared/auto-only 0x{shared:X} bytes; unreferenced 0x{unused:X} bytes")
    rows = sorted(per_unit.items(), key=lambda kv: -sum(objs[n]["size"] for n in kv[1]))
    tsv = opt("--tsv")
    fo = open(tsv, "w") if tsv else None
    if fo: fo.write("unit\tsource\tobjects\tbytes\tsections\tfirst\n")
    for u, ns in rows:
        ns.sort(key=lambda n: objs[n]["addr"])
        b = sum(objs[n]["size"] for n in ns)
        secs = ",".join(sorted({objs[n]["sec"] for n in ns}))
        line = f"{u}\t{'src' if has_src(u) else 'asm'}\t{len(ns)}\t0x{b:X}\t{secs}\t" + ",".join(ns[:6])
        if fo: fo.write(line + "\n")
    if not fo:
        for u, ns in rows[:40]:
            b = sum(objs[n]["size"] for n in ns)
            print(f"  {u:45s} {'src' if has_src(u) else 'asm'} {len(ns):4d} obj 0x{b:5X}  " + ",".join(sorted({objs[n]['sec'] for n in ns})))
    sys.exit()

if ONE:
    a = analyse(ONE)
    print(f"{ONE}: 0x{a['lo']:08X}-0x{a['hi']:08X} size 0x{a['hi']-a['lo']:X}, {a['n']} functions")
    print(f"  before: {short(a['prev'])}\n  after:  {short(a['next'])}")
    for f in a["fl"]:
        ad, sz, _ = funcs[f]
        tags = []
        if f in fn_class: tags.append("vt:" + fn_class[f])
        if f in vt_users: tags.append("uses vt:" + ",".join(sorted(vt_users[f])))
        if f in ctor_fns: tags.append("__sinit")
        cs = sorted({r for r in frefs.get(f, ()) if r in dup}, key=lambda r: objs[r]["addr"])
        if cs: tags.append("dupconst:" + ",".join(cs[:6]) + ("..." if len(cs) > 6 else ""))
        ad_auto = sorted({r for r in frefs.get(f, ()) if r in objs and is_auto(objs[r]["unit"]) and objs[r]["sec"] != "text"},
                         key=lambda r: objs[r]["addr"])
        if ad_auto: tags.append("autodata:" + ",".join(ad_auto[:8]) + ("..." if len(ad_auto) > 8 else ""))
        print(f"  {ad:08X} {sz:6X} {f}  " + "  ".join(tags))
    for lo_, hi_ in a["bounds"]:
        print(f"  boundary between 0x{lo_:08X} and 0x{hi_:08X} (different copies of a constant)")
    sys.exit()

rows = []
for u in uorder:
    if not (is_auto(u) and u.endswith("_text")):
        continue
    lo, hi = urange[u]
    if hi - lo < MIN or lo >= 0x805BFAC0:  # stop at the RVL SDK
        continue
    rows.append((u, analyse(u)))

tsv = opt("--tsv")
md = opt("--md")
out = []
for u, a in rows:
    cl = ", ".join(list(a["classes"])[:12]) + (" ..." if len(a["classes"]) > 12 else "")
    if not cl and a["built"]:
        cl = "(solo construye: " + ", ".join(list(a["built"])[:6]) + ")"
    out.append((u, a["lo"], a["hi"], a["n"], len(a["classes"]), len(a["bounds"]), a["prev"], a["next"], cl))
out.sort(key=lambda r: r[1])
if tsv:
    with open(tsv, "w") as fo:
        fo.write("unit\tstart\tend\tsize\tfuncs\tclasses\tboundaries\tbefore\tafter\tclass_names\n")
        for u, lo, hi, n, nc, nb, p, nx, cl in out:
            fo.write(f"{u}\t0x{lo:08X}\t0x{hi:08X}\t0x{hi-lo:X}\t{n}\t{nc}\t{nb}\t{short(p)}\t{short(nx)}\t{cl}\n")
if md:
    with open(md, "w") as fo:
        fo.write("| Tramo | Rango | Tamaño | Func. | Clases (vtables) | Cortes por constantes | Antes / después |\n|---|---|---|---|---|---|---|\n")
        for u, lo, hi, n, nc, nb, p, nx, cl in out:
            fo.write(f"| `{u}` | `0x{lo:08X}–0x{hi:08X}` | 0x{hi-lo:X} | {n} | {cl or '-'} | {nb} | {short(p)} / {short(nx)} |\n")
tot = sum(r[2] - r[1] for r in out)
print(f"{len(out)} auto text ranges (game/engine, >= 0x{MIN:X} bytes), 0x{tot:X} bytes; "
      f"{sum(r[4] for r in out)} classes with vtables inside; {sum(r[5] for r in out)} boundary candidates")
if not (tsv or md):
    for u, lo, hi, n, nc, nb, p, nx, cl in sorted(out, key=lambda r: r[1] - r[2])[:40]:
        print(f"0x{hi-lo:6X} {n:4d}f {nc:3d}cl {nb:3d}cut  {u}  {cl[:90]}")
