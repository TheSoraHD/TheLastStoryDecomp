#!/usr/bin/env python3
"""Show call sites of a function in the target asm (all units), with context.

usage: callers.py <name-substring> [context=6] [max=5]
"""
import os
import glob, re, sys
name = sys.argv[1]
ctx = int(sys.argv[2]) if len(sys.argv) > 2 else 6
mx = int(sys.argv[3]) if len(sys.argv) > 3 else 5
n = 0
for path in sorted(glob.glob(os.path.join(os.environ.get("CMP_ROOT", os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))), "build/SLSP01/asm/**/*.s"), recursive=True)):
    lines = open(path).read().split("\n")
    cur = None
    for i, l in enumerate(lines):
        m = re.match(r"\.fn ([^\s,]+)", l)
        if m:
            cur = m.group(1)
        if re.search(r"\b(bl|b|lis|addi)\s.*\b%s\b" % re.escape(name), l) and not l.startswith(".fn"):
            n += 1
            if n > mx:
                continue
            print("=== %s in %s (%s)" % (name, cur, path.split("asm/")[1]))
            for k in range(max(0, i - ctx), min(len(lines), i + 3)):
                mm = re.match(r"/\* ([0-9A-F]{8}) [0-9A-F]{8}  (?:[0-9A-F]{2} ){4}\*/\t(.*)", lines[k])
                print("  " + ("%s  %s" % (mm.group(1)[-4:], mm.group(2)) if mm else lines[k]))
print("# total refs:", n)
