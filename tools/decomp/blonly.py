#!/usr/bin/env python3
# blonly.py: functions of unlinked game units (95-99.99%) whose only differences are call targets
# (bl X | bl Y) and label names: prints unit, function and the (original, ours) call pairs
import subprocess,sys,re,json
gu=[u['name'].rsplit('.',1)[0] for u in json.load(open('config/SLSP01/game_units.json')) if not u.get('matching') and 'CharaTask' not in u['name']]
SYM=r'(lbl_[0-9A-F]+|@\S+?@|@\d+|\S*@STRING@\S*)'
for u in gu:
    o=subprocess.run(['build/tools/objdiff-cli','diff','-p','.','-u','main/'+u,'-o','-','--format','json'],capture_output=True,text=True).stdout
    if not o: continue
    d=json.loads(o)
    for s in d['left']['symbols']:
        p=s.get('match_percent')
        if p is None or p>=100 or p<95 or 'instructions' not in s: continue
        r=subprocess.run(['python3','tools/decomp/odiff2.py',u,s['name']],capture_output=True,text=True).stdout.splitlines()
        bl=set();ok=bool(r)
        for x in r:
            m=re.match(r'\s*\d+\s+(\S+)\s+(.*?)\s+\|\s+(.*)$',x)
            if not m or m.group(1)!='DIFF_ARG_MISMATCH': ok=False;break
            a,b=m.group(2),m.group(3)
            mb=re.match(r'bl (\S+)$',a.strip()); nb=re.match(r'bl (\S+)$',b.strip())
            if mb and nb: bl.add((mb.group(1),nb.group(1))); continue
            if re.sub(SYM,'S',a).split()!=re.sub(SYM,'S',b).split(): ok=False;break
        if ok and bl: print(u,s['name'],sorted(bl),flush=True)
