#!/usr/bin/env python3
"""pairdata.py <unit> [--apply] [--local]: rename the target's data labels (lbl_XXXXXXXX of the unit's own
.rodata/.sdata2/.data...) to the names our object uses (@NNNN pool constants, @stringBase0, static
objects), by comparing the relocations of the same instruction in every paired function (objdiff).

Only labels referenced the same way everywhere are renamed; compiler-local names (@NNNN,
@stringBase0) get scope:local (template weak objects @STRING@/@LOCAL@/@GUARD@ do not). Run it when the source is stable: @NNNN numbers change when the
code before them changes.

One objdiff call for the whole unit (2026-11-03; before: one call per function, ~40 min for
CharaTask, now seconds). --local: only rename to compiler-local names (@NNNN, @stringBase0,
@wstringBase0); use it when running over many units at once. --relabel: also re-pair labels that
already have a stale @NNNN name (no resetlocal / second dtk split needed) (other names it proposes, such as
statics or vtables, can be wrong and break the link)."""
import json, os, re, subprocess, sys

os.chdir(os.environ.get("CMP_ROOT", os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))))
unit = sys.argv[1]
ONLYLOCAL = "--local" in sys.argv
# --relabel: also re-pair target labels that already have a compiler-local name (@NNNN) which no
# longer matches ours, so resetlocal (and the extra dtk split it costs) is not needed first
RELABEL = "--relabel" in sys.argv
TNAME = r"^(lbl_[0-9A-F]{8}|@\d+)$" if RELABEL else r"^lbl_[0-9A-F]{8}$"
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


_ALL = None


def relocs(sym):
    # one objdiff call for the whole unit (faster than one call per function)
    global _ALL
    if _ALL is None:
        out = subprocess.run(["build/tools/objdiff-cli", "diff", "-p", ".", "-u", "main/" + unit, "-o", "-",
                              "--format", "json"], capture_output=True, text=True).stdout
        try:
            d = json.loads(out)
        except ValueError:
            d = {"left": {"symbols": []}, "right": {"symbols": []}}
        _ALL = ({s["name"]: s.get("instructions", []) for s in d["left"]["symbols"]},
                {s["name"]: s.get("instructions", []) for s in d["right"]["symbols"]})
    rows = {"left": _ALL[0].get(sym), "right": _ALL[1].get(sym)}
    if not rows["left"] or not rows["right"]:
        return []
    res = []
    for a, b in zip(rows["left"], rows["right"]):
        ra = (a.get("instruction") or {}).get("relocation") if isinstance(a.get("instruction"), dict) else None
        rb = (b.get("instruction") or {}).get("relocation") if isinstance(b.get("instruction"), dict) else None
        if not ra or not rb or ra.get("type") != rb.get("type"):
            continue
        ta = reloc_name(a["instruction"]["formatted"])
        tb = reloc_name(b["instruction"]["formatted"])
        if ta and tb and re.match(TNAME, ta):
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
        elif o != t and not o.startswith("lbl_") and not o.startswith("...") and (ONLYLOCAL is False or re.match(r"^@(\d+|stringBase\d+|wstringBase\d+)$", o)):
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
def unit_ranges():
    txt = open("config/SLSP01/splits.txt").read()
    m = re.search(r"^" + re.escape(unit) + r"\.(?:cpp|c):\n((?:\t.*\n)+)", txt, re.M)
    r = []
    if m:
        for sec, a, b in re.findall(r"\t(\.\w+)\s+start:0x([0-9A-F]+) end:0x([0-9A-F]+)", m.group(1)):
            if sec != ".text":
                r.append((int(a, 16), int(b, 16)))
    return r


if "--apply" in sys.argv and ren:
    lines = open(SYM).read().split("\n")
    # @NNNN names repeat across units: rename only the line inside this unit's data ranges, and only
    # when the name is unique there (lbl_XXXXXXXX names are unique anyway)
    rs = unit_ranges()
    where = {}
    for i, l in enumerate(lines):
        m = re.match(r"(\S+) = \.\w+:0x([0-9A-F]+);", l)
        if m and any(a <= int(m.group(2), 16) < b for a, b in rs):
            where.setdefault(m.group(1), []).append(i)
    done = 0
    for t, new in sorted(ren.items()):
        idx = where.get(t, [])
        if not idx and t.startswith("lbl_"):
            idx = [i for i, l in enumerate(lines) if l.startswith(t + " = ")]
        if len(idx) != 1:
            print("  %s: %d lines in the unit, skipped" % (t, len(idx)))
            continue
        # the new name must not stay on another line of the unit that is not renamed itself
        others = [i for i in where.get(new, []) if i != idx[0] and new not in ren]
        if others:
            print("  %s -> %s: name already used in the unit, skipped" % (t, new))
            continue
        i = idx[0]
        l = new + lines[i][len(t):]
        # only compiler-local names: @NNNN pool constants and the string pools (the weak objects
        # of templates, @STRING@/@LOCAL@/@GUARD@, keep the default scope)
        if re.match(r"^@(\d+|stringBase\d+|wstringBase\d+)$", new) and "scope:local" not in l:
            l = re.sub(r"(type:object size:0x[0-9A-F]+)", r"\1 scope:local", l, count=1)
        lines[i] = l
        done += 1
    open(SYM, "w").write("\n".join(lines))
    print("renamed %d" % done)
