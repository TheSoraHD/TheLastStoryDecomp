#!/usr/bin/env python3
"""resetlocal.py <unit>: give back the lbl_<addr> names (default scope) to the @NNNN symbols of the
unit's data sections in symbols.txt, so that pairdata.py can pair them again after an edit."""
import re, sys
unit = sys.argv[1]
splits = open("config/SLSP01/splits.txt").read()
m = re.search(r"^%s\.cpp:\n((?:\t.*\n)+)" % re.escape(unit), splits, re.M)
ranges = [(int(a, 16), int(b, 16)) for a, b in re.findall(r"start:(0x\w+) end:(0x\w+)", m.group(1))]
out = []
n = 0
for l in open("config/SLSP01/symbols.txt"):
    mm = re.match(r'^(@\d+|"@\d+") = (\.\w+):(0x\w+);(.*)$', l.rstrip("\n"))
    if mm and any(a <= int(mm.group(3), 16) < b for a, b in ranges) and mm.group(2) != ".text":
        rest = mm.group(4).replace(" scope:local", "")
        l = "lbl_%08X = %s:%s;%s\n" % (int(mm.group(3), 16), mm.group(2), mm.group(3), rest)
        n += 1
    out.append(l)
open("config/SLSP01/symbols.txt", "w").write("".join(out))
print("reset", n)
