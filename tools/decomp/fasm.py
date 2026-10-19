#!/usr/bin/env python3
"""Print target asm of functions from build/SLSP01/asm/<unit>.s, compactly.

usage: fasm.py <unit> <addr-or-name> [...]
"""
import os
import re, sys
unit = sys.argv[1]
path = os.path.join(os.environ.get("CMP_ROOT", os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))), "build/SLSP01/asm", unit) + ("" if unit.endswith(".s") else ".s")
want = sys.argv[2:]
lines = open(path).read().split("\n")
i = 0
out = []
while i < len(lines):
    l = lines[i]
    m = re.match(r"\.fn (\S+), (\w+)", l)
    if m:
        name = m.group(1)
        j = i + 1
        body = []
        addr = None
        while not lines[j].startswith(".endfn"):
            b = lines[j]
            mm = re.match(r"/\* ([0-9A-F]{8}) [0-9A-F]{8}  (?:[0-9A-F]{2} ){4}\*/\t(.*)", b)
            if mm:
                if addr is None:
                    addr = mm.group(1)
                body.append("%s  %s" % (mm.group(1)[-4:], mm.group(2)))
            elif b.strip():
                body.append(b)
            j += 1
        if any(w.upper() == (addr or "") or w == name or (w.lower() in name.lower() and not re.match(r"^[0-9a-fA-F]{8}$", w)) for w in want):
            out.append(f"### {name} @ {addr}")
            out.extend(body)
        i = j
    i += 1
print("\n".join(out))
