#!/usr/bin/env python3
# gap.py <report.json>: units with more functions at 100% in report.json than in objdiff (fnstat)
# (name-only failures: pairdata candidates)
import json,subprocess,sys
r=json.load(open(sys.argv[1]))
gu={u['name'].rsplit('.',1)[0]:u for u in json.load(open('config/SLSP01/game_units.json'))}
out=[]
for u in r['units']:
    n=u['name'][5:]
    if n not in gu or gu[n].get('matching'): continue
    fs=[f for s in u.get('sections',[]) for f in s.get('functions',[])] + u.get('functions',[])
    full=sum(1 for f in fs if f.get('fuzzy_match_percent',0)==100)
    if not full: continue
    o=subprocess.run(['python3','tools/decomp/fnstat.py',n,'--nobuild'],capture_output=True,text=True).stdout.strip().splitlines()
    try: a=int(o[-1].split()[1].split('/')[0])
    except: a=-1
    if full-a>0: out.append((full-a,n,a,full))
for x in sorted(out,reverse=True): print(*x)
