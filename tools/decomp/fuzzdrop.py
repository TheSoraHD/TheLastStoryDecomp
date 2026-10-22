#!/usr/bin/env python3
"""fuzzdrop.py <now/report.json> <ref/report.json> [min_delta=0.005]
Lists functions whose fuzzy_match_percent went down (by address), and functions missing now."""
import json,sys
def load(p):
    r=json.load(open(p)); d={}
    for u in r["units"]:
        for f in u.get("functions",[]):
            va=f.get("metadata",{}).get("virtual_address")
            d[va]=(f.get("fuzzy_match_percent",0),f["name"],u["name"])
    return d
cur=load(sys.argv[1]); ref=load(sys.argv[2]); th=float(sys.argv[3]) if len(sys.argv)>3 else 0.005
drops=[(k,ref[k],cur.get(k)) for k in ref if k not in cur or cur[k][0] < ref[k][0]-th]
print("fuzzy drops:",len(drops))
for k,a,b in sorted(drops, key=lambda x: int(x[0] or 0)):
    print("   %s %s %s: %.2f -> %s"%(hex(int(k)) if k else k,a[2],a[1],a[0],"missing" if b is None else "%.2f"%b[0]))
