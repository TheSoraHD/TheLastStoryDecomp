#!/usr/bin/env python3
# combo.py <unit> <sym> <spec.py>: spec defines OLD (str) and SLOTS: list of lists of alternatives; NEW template with {0},{1}...
import sys,itertools,subprocess,os
R=os.path.abspath(os.path.join(os.path.dirname(__file__),'..','..'))+'/'
u,sym,spec=sys.argv[1:4]
g={}; exec(open(spec).read(),g)
s=open(R+'src/'+u+'.cpp',encoding='latin-1').read()
REPL=g.get('REPL') or [(g['OLD'],g['NEW'])]
for o,_ in REPL: assert s.count(o)==1, 'OLD '+o[:60]
p=os.path.dirname(R+'src/'+u+'.cpp')+'/.combo_tmp.cpp'
best=None
for c in itertools.product(*g['SLOTS']):
    t=s
    for o,n in REPL: t=t.replace(o,n.format(*c))
    open(p,'w',encoding='latin-1').write(t)
    r=subprocess.run(['python3',R+'tools/decomp/trysym.py',u,p,sym],capture_output=True,text=True,env=dict(os.environ,V='1')).stdout.strip().split('\n')
    n=(999 if 'None' in r[0] else len(r)-1) if r[0].startswith('DIFF') else (0 if r[0].startswith('OK') else 999)
    print(n,c,flush=True)
    if n==0: print('FOUND',c); break
os.remove(p)
