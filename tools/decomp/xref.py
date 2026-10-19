#!/usr/bin/env python3
"""Cross-reference index over the target asm.

usage: xref.py build            (re-index)
       xref.py <name> [ctx=4] [max=3]
"""
import glob, os, pickle, re, sys
IDX = os.path.join(os.environ.get("CMP_ROOT", os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))), "build", "xref.pkl")
if sys.argv[1] == "build":
    refs = {}
    for path in sorted(glob.glob(os.path.join(os.environ.get("CMP_ROOT", os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))), "build/SLSP01/asm/**/*.s"), recursive=True)):
        lines = open(path).read().split("\n")
        cur = None
        clean = []
        for l in lines:
            mm = re.match(r"/\* ([0-9A-F]{8}) [0-9A-F]{8}  (?:[0-9A-F]{2} ){4}\*/\t(.*)", l)
            clean.append("%s  %s" % (mm.group(1)[-4:], mm.group(2)) if mm else l)
        for i, l in enumerate(lines):
            m = re.match(r"\.fn ([^\s,]+)", l)
            if m:
                cur = m.group(1)
                continue
            if l.startswith(".") or not l.startswith("/*"):
                continue
            ins = l.split("*/", 1)[-1]
            found = set(re.findall(r"([^\s,()]+?)@(?:ha|h|l|sda21)\b", ins))
            mb = re.match(r"\s*(?:bl|b|bne|beq|blt|bgt|ble|bge)\s+([^\s,]+)\s*$", ins)
            if mb and not mb.group(1).startswith(".L"):
                found.add(mb.group(1))
            found |= set(re.findall(r"\b(fn_[0-9A-F]{8}\w*|lbl_[0-9A-F]{8})", ins))
            for s in found:
                s = s.strip('"')
                refs.setdefault(s, []).append((path.split("asm/")[1], cur, clean[max(0, i - 8):i + 3]))
    pickle.dump(refs, open(IDX, "wb"))
    print(len(refs))
    sys.exit()
refs = pickle.load(open(IDX, "rb"))
name = sys.argv[1]
ctx = int(sys.argv[2]) if len(sys.argv) > 2 else 4
mx = int(sys.argv[3]) if len(sys.argv) > 3 else 3
hits = refs.get(name, [])
for path, fn, lines in hits[:mx]:
    print("=== %s in %s (%s)" % (name, fn, path))
    for l in lines[8 - ctx:]:
        print("  " + l)
print("# %s: %d refs" % (name, len(hits)))
