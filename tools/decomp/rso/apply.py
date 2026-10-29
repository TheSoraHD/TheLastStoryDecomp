#!/usr/bin/env python3
"""Rename symbols.txt entries from build/rso/classified.json.

Usage: python3 tools/decomp/rso/apply.py [category ...] [--dry]
Default category: apply (anonymous fn_/lbl_ symbols not referenced from src/ or include/).
Skips HBM library names (nw4hbm/homebutton/HBM) that land in .text outside the HBM range:
identical nw4r code, not the same function.
"""
import json
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).parent))
from common import SCR, SYMS  # noqa

HBM_TEXT = (0x805C0000, 0x805E5000)


def skip(rec):
    a = int(rec["addr"], 16)
    hbm = re.search(r"nw4hbm|homebutton|^HBM|__HBM", rec["name"])
    return bool(hbm) and a < 0x80770000 and not (HBM_TEXT[0] <= a < HBM_TEXT[1])


def main():
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    dry = "--dry" in sys.argv
    cats = args or ["apply"]
    res = json.loads((SCR / "classified.json").read_text())
    ren = {}
    for c in cats:
        for rec in res[c]:
            if skip(rec):
                print("skip", rec["addr"], rec["name"])
                continue
            ren[int(rec["addr"], 16)] = (rec["eu"][0], rec["name"])
    lines = SYMS.read_text(encoding="utf-8").split("\n")
    done = 0
    for i, l in enumerate(lines):
        m = re.match(r"^(\S+) = (\.\w+):0x([0-9A-Fa-f]+);", l)
        if not m:
            continue
        a = int(m.group(3), 16)
        if a in ren and ren[a][0] == m.group(1):
            lines[i] = ren[a][1] + l[len(m.group(1)):]
            done += 1
            del ren[a]
    if ren:
        print("not found:", len(ren))
    print("renamed", done)
    if not dry:
        SYMS.write_text("\n".join(lines), encoding="utf-8")


if __name__ == "__main__":
    main()
