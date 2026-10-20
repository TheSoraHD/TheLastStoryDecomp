#!/usr/bin/env python3
"""Compare two report.json files per function.

usage: regress.py <now/report.json> <ref1/report.json> [ref2 ...]
Prints matched-function totals and, for each reference, the functions at 100% there that are
below 100% (or missing) now. Use after integrating branches: "lost: 0" is the goal.
"""
import json,sys
def load(p):
    r=json.load(open(p)); d={}
    for u in r["units"]:
        for f in u.get("functions",[]):
            va=f.get("metadata",{}).get("virtual_address")
            d[va]=(f.get("fuzzy_match_percent",0),f["name"],u["name"])
    return d,r["measures"]
cur,cm=load(sys.argv[1])
print("now: matched funcs",cm.get("matched_functions"),"/",cm.get("total_functions"),"code%",round(cm.get("matched_code_percent",0),3))
for p in sys.argv[2:]:
    ref,rm=load(p); lost=[(ref[k],cur.get(k)) for k in ref if ref[k][0]==100 and (k not in cur or cur[k][0]<100)]
    print(p,"matched",rm.get("matched_functions"),"lost:",len(lost))
    for a,b in lost[:30]: print("   ",a[2],a[1],"->",b and round(b[0],2))
