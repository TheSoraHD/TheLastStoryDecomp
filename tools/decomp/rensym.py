#!/usr/bin/env python3
"""Rename symbols in config/SLSP01/symbols.txt (rule 3 allows renames).

usage: rensym.py <old> <new> [<old> <new> ...]
Only whole symbol names at the start of a line are replaced; fails if a name is not found or the new
name already exists. Run configure.py afterwards.
"""
import re, sys

p = "config/SLSP01/symbols.txt"
args = sys.argv[1:]
assert args and len(args) % 2 == 0, __doc__
lines = open(p).read().split("\n")
names = {l.split(" = ", 1)[0]: i for i, l in enumerate(lines) if " = " in l}
for old, new in zip(args[::2], args[1::2]):
    if old not in names:
        sys.exit(f"not found: {old}")
    if new in names:
        sys.exit(f"already exists: {new}")
    i = names.pop(old)
    lines[i] = new + lines[i][len(old):]
    names[new] = i
    print(f"{old} -> {new}")
open(p, "w").write("\n".join(lines))
