#!/usr/bin/env python3
# stk.py unit sym : list (orig, ours) r1 offsets for each instruction pair that differ in r1 offset
import json,subprocess,sys,re,os
os.chdir("/home/claude/work-N1")
u,sym=sys.argv[1],sys.argv[2]
out=subprocess.run(["build/tools/objdiff-cli","diff","-p",".","-u","main/"+u,sym,"-o","-","--format","json"],capture_output=True,text=True).stdout
d=json.loads(out); rows={}
for side in ("left","right"):
    for s in d[side]["symbols"]:
        if s["name"]==sym: rows[side]=s.get("instructions",[])
L,R=rows["left"],rows["right"]
pairs={}
for a,b in zip(L,R):
    fa=(a.get("instruction") or {}).get("formatted","") if isinstance(a.get("instruction"),dict) else ""
    fb=(b.get("instruction") or {}).get("formatted","") if isinstance(b.get("instruction"),dict) else ""
    ma=re.search(r"(-?0x[0-9a-f]+)\(r1\)|r1, (-?0x[0-9a-f]+)",fa); mb=re.search(r"(-?0x[0-9a-f]+)\(r1\)|r1, (-?0x[0-9a-f]+)",fb)
    if ma and mb:
        oa=int(ma.group(1) or ma.group(2),16); ob=int(mb.group(1) or mb.group(2),16)
        if oa!=ob: pairs.setdefault(ob,set()).add(oa)
for ob in sorted(pairs): print("ours %#x -> orig %s"%(ob,",".join("%#x"%x for x in sorted(pairs[ob]))))
