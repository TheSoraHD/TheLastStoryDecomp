#!/usr/bin/env python3
"""Try alternative bodies for one snippet of a source file and report which match.

usage: variants.py <unit> <func-filter> <variants-file>
The variants file holds blocks separated by lines '====': the first block is the
exact text to replace in src/<unit>.cpp, the following blocks are candidates.
The file is restored afterwards unless a candidate matches (then the first
matching candidate is kept).
"""
import os, subprocess, sys

ROOT = os.environ.get("CMP_ROOT", os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..")))
unit, flt, vf = sys.argv[1], sys.argv[2], sys.argv[3]
import os
src = os.environ.get("VFILE") or f"{ROOT}/src/{unit}.cpp"
blocks = open(vf).read().split("\n====\n")
orig_snip, cands = blocks[0], blocks[1:]
text = open(src).read()
assert orig_snip in text, "snippet not found"
kept = None
for i, c in enumerate(cands):
    open(src, "w").write(text.replace(orig_snip, c, 1))
    r = subprocess.run(["python3", os.path.join(os.path.dirname(os.path.abspath(__file__)), "cmp.py"), unit, flt], capture_output=True, text=True)
    lines = [l for l in r.stdout.split("\n") if l.startswith(("OK", "DIFF", "#"))]
    status = " | ".join(l for l in lines if flt in l) or r.stdout[-400:]
    print(f"[{i}] {status}")
    if kept is None and any(l.startswith("OK") and flt in l for l in lines):
        kept = c
if kept is not None:
    open(src, "w").write(text.replace(orig_snip, kept, 1))
    print("kept first matching variant")
else:
    open(src, "w").write(text)
