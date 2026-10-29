#!/usr/bin/env python3
"""pairdata.py <unit> [--apply]: rename the target's data labels (lbl_XXXXXXXX of the unit's own
.rodata/.sdata2/.data...) to the names our object uses (@NNNN pool constants, @stringBase0, static
objects), by comparing the relocations of the same instruction in every paired function (objdiff).

Only labels referenced the same way everywhere are renamed; compiler-local names (@NNNN,
@stringBase0) get scope:local (template weak objects @STRING@/@LOCAL@/@GUARD@ do not). Run it when the source is stable: @NNNN numbers change when the
code before them changes."""
import json, os, re, subprocess, sys

os.chdir(os.environ.get("CMP_ROOT", os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))))
unit = sys.argv[1]
SYM = "config/SLSP01/symbols.txt"


def target_funcs():
    out = subprocess.run(["build/binutils/powerpc-eabi-nm", "-n", "build/SLSP01/obj/%s.o" % unit],
                         capture_output=True, text=True).stdout
    return [l.split()[2] for l in out.splitlines() if len(l.split()) == 3 and l.split()[1] in "TtWw"]


def reloc_name(formatted):
    # mangled names have commas but no spaces: the operand is the last ", " field
    op = formatted.split(", ")[-1] if ", " in formatted else formatted.split(" ", 1)[-1]
    op = re.sub(r"\(r\d+\)$", "", op)
    m = re.match(r'^"?(.*?)"?@(?:ha|l|sda21)$', op)
    return m.group(1) if m else None


def relocs(sym):
    out = subprocess.run(["build/tools/objdiff-cli", "diff", "-p", ".", "-u", "main/" + unit, sym, "-o", "-",
                          "--format", "json"], capture_output=True, text=True).stdout
    try:
        d = json.loads(out)
    except ValueError:
        return []
    rows = {}
    names = {}
    for side in ("left", "right"):
        names[side] = [x["name"] for x in d[side]["symbols"]]
        for s in d[side]["symbols"]:
            if s["name"] == sym:
                rows[side] = s.get("instructions", [])
    if "left" not in rows or "right" not in rows:
        return []
    res = []
    for a, b in zip(rows["left"], rows["right"]):
        ra = (a.get("instruction") or {}).get("relocation") if isinstance(a.get("instruction"), dict) else None
        rb = (b.get("instruction") or {}).get("relocation") if isinstance(b.get("instruction"), dict) else None
        if not ra or not rb or ra.get("type") != rb.get("type"):
            continue
        ta = reloc_name(a["instruction"]["formatted"])
        tb = reloc_name(b["instruction"]["formatted"])
        if ta and tb and re.match(r"^lbl_[0-9A-F]{8}$", ta):
            res.append((ta, tb))
    return res


pairs = {}
for f in target_funcs():
    for t, o in relocs(f):
        pairs.setdefault(t, set()).add(o)
ren = {}
# a name that appears in an ambiguous pairing is not trusted for any label
bad = set()
for t, os_ in pairs.items():
    if len(os_) > 1:
        bad |= os_
for t, os_ in sorted(pairs.items()):
    if len(os_) == 1:
        o = next(iter(os_))
        if o in bad:
            print("  %s -> %s: name also in an ambiguous pairing, skipped" % (t, o))
        elif o != t and not o.startswith("lbl_") and not o.startswith("..."):
            ren[t] = o
            print("  %s -> %s" % (t, o))
    else:
        print("  %s: ambiguous %s" % (t, sorted(os_)))
used = {}
for t, o in ren.items():
    used.setdefault(o, []).append(t)
for o, ts in used.items():
    if len(ts) > 1:
        print("  %s used for several labels %s: skipped" % (o, ts))
        for t in ts:
            del ren[t]
if "--apply" in sys.argv and ren:
    lines = open(SYM).read().split("\n")
    for i, l in enumerate(lines):
        m = re.match(r"(\S+) = ", l)
        if m and m.group(1) in ren:
            new = ren[m.group(1)]
            l = new + l[len(m.group(1)):]
            # only compiler-local names: @NNNN pool constants and the string pools (the weak objects
            # of templates, @STRING@/@LOCAL@/@GUARD@, keep the default scope)
            if re.match(r"^@(\d+|stringBase\d+|wstringBase\d+)$", new) and "scope:local" not in l:
                l = re.sub(r"(type:object size:0x[0-9A-F]+)", r"\1 scope:local", l, count=1)
            lines[i] = l
    open(SYM, "w").write("\n".join(lines))
    print("renamed %d" % len(ren))
