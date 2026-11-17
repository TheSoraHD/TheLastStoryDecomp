#!/usr/bin/env python3
# side.py <unit> <sym> <from> <to>: both sides of objdiff (target | ours) for an instruction range
import json,subprocess,sys
u,sym,a,b=sys.argv[1],sys.argv[2],int(sys.argv[3]),int(sys.argv[4])
d=json.loads(subprocess.run(['build/tools/objdiff-cli','diff','-p','.','-u','main/'+u,sym,'-o','-','--format','json'],capture_output=True,text=True,cwd=__import__('os').environ.get('CMP_ROOT','.')).stdout)
r={}
for side in ('left','right'):
    for s in d[side]['symbols']:
        if s['name']==sym: r[side]=s.get('instructions',[])
f=lambda x:(x.get('instruction') or {}).get('formatted','') if isinstance(x.get('instruction'),dict) else ''
for i in range(a,b):
    L=r['left'][i] if i<len(r['left']) else {}; R=r['right'][i] if i<len(r['right']) else {}
    print('%-4d %-40s | %s'%(i,f(L),f(R)))
