#!/usr/bin/env python3
"""Resolve git conflict blocks in symbols.txt by address.

usage: symresolve.py config/SLSP01/symbols.txt
Merges both sides of every conflict block, keeps one line per address (a real name wins over
fn_/lbl_), sorts by address and prints real-name clashes (HEAD kept). Use when symmerge.py
reports 0 blocks resolved.
"""
import re,sys
p=sys.argv[1]; L=open(p).read().split("\n"); out=[]; i=0; rep=[]
key=lambda l: re.search(r"= (\.\w+):0x(\w+)",l)
def auto(n): return bool(re.match(r"(fn|lbl)_[0-9A-F]{8}$",n))
while i<len(L):
    if L[i].startswith("<<<<<<<"):
        a=[];b=[];i+=1
        while not L[i].startswith("======="): a.append(L[i]); i+=1
        i+=1
        while not L[i].startswith(">>>>>>>"): b.append(L[i]); i+=1
        i+=1; d={}
        for side,ls in (("H",a),("T",b)):
            for l in ls:
                m=key(l); k=(m[1],int(m[2],16))
                if k not in d: d[k]=l; continue
                n0=d[k].split(" =")[0]; n1=l.split(" =")[0]
                if n0!=n1:
                    if auto(n0) and not auto(n1): d[k]=l
                    elif not auto(n0) and not auto(n1): rep.append((n0,n1))
        out+= [d[k] for k in sorted(d,key=lambda k:k[1])]
    else: out.append(L[i]); i+=1
open(p,"w").write("\n".join(out)); print("real-name clashes (kept HEAD):",rep)
