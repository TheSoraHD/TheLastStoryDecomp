#!/usr/bin/env python3
# opidx_apply.py <unit> <fn>: tries all, then each occurrence, applies the first variant that makes fn OK and keeps all other funcs
import re,subprocess,os,sys
R=os.path.dirname(os.path.abspath(__file__))+'/../../'
un,fn=sys.argv[1:3]
src=R+'src/'+un+'.cpp'
s=open(src,encoding='latin-1').read()
meth=fn.split('__')[0] if not fn.startswith('__') else None
if fn.startswith('__ct__'):
    mm=re.match(r'__ct__(?:Q\d(?:\d+\w+?)*?)?(\d+)(\w+?)F',fn); meth=mm.group(2)[:int(mm.group(1))]
defs=list(re.finditer(r'^[^\n;{}()]*\b'+re.escape(meth)+r'\([^;{]*?\)\s*(?:const\s*)?(?::[^{;]*)?\{',s,re.M))
assert len(defs)==1
st=defs[0].end(); depth=1; i=st
while depth and i<len(s):
    if s[i]=='{': depth+=1
    elif s[i]=='}': depth-=1
    i+=1
body=s[st:i]
occ=list(re.finditer(r'(\b[\w\.]+)\.mData\[',body))
variants=[occ]+[[o] for o in occ]
def run(f):
    o=subprocess.run(['python3',''+R+'tools/decomp/trysym.py',un,f],capture_output=True,text=True).stdout
    return {l.split()[1]:l.split()[0] for l in o.splitlines() if l[:2] in('OK','DI')}
base=run(src)
for vs in variants:
    b=body
    for o in sorted(vs,key=lambda x:-x.start()):
        b=b[:o.start()]+o.group(1)+'['+b[o.end():]
    t=s[:st]+b+s[i:]
    tmp=os.path.dirname(src)+'/.oa.cpp'
    open(tmp,'w',encoding='latin-1').write(t)
    r=run(tmp); os.remove(tmp)
    if r.get(fn)=='OK' and all(not(base.get(k)=='OK' and r.get(k)!='OK') for k in base if 'sinit' not in k):
        open(src,'w',encoding='latin-1').write(t)
        gained=[k for k in r if r[k]=='OK' and base.get(k)!='OK']
        print('APPLIED',un,fn,len(vs),'gained',gained); sys.exit(0)
print('FAILED',un,fn)
