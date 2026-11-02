#!/usr/bin/env python3
"""selfnames.py <unit> [--apply] [addr=name ...]: rename target fn_XXXXXXXX symbols of a unit to our
mangled names that start with the same fn_XXXXXXXX (member functions named after their address), plus
explicit pairs given as ADDR=NAME (e.g. 803C1950=vf10__9GameSceneFv)."""
import os, re, subprocess, sys
os.chdir(os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..")))
unit = sys.argv[1]
out = subprocess.run(["build/binutils/powerpc-eabi-nm", "build/SLSP01/src/%s.o" % unit], capture_output=True, text=True).stdout
ours = {}
for l in out.splitlines():
    n = l.split()[-1]
    m = re.match(r"(fn_[0-9A-F]{8})__", n)
    if m:
        ours[m.group(1)] = n
for a in sys.argv[2:]:
    if "=" in a:
        k, v = a.split("=", 1)
        ours["fn_" + k.upper()] = v
p = "config/SLSP01/symbols.txt"
L = open(p).read().split("\n")
names = set(l.split(" = ")[0] for l in L if " = " in l)
n = 0
for i, l in enumerate(L):
    m = re.match(r"(fn_[0-9A-F]{8}) = (.*)", l)
    if m and m.group(1) in ours and ours[m.group(1)] not in names:
        print("  %s -> %s" % (m.group(1), ours[m.group(1)]))
        if "--apply" in sys.argv:
            L[i] = ours[m.group(1)] + " = " + m.group(2); n += 1
if "--apply" in sys.argv:
    open(p, "w").write("\n".join(L))
    print("renamed", n)
