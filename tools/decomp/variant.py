#!/usr/bin/env python3
# variant.py <unit> <sym> <old-file> <new-file>: replaces the text of old-file by new-file in src/<unit>.cpp (copy) and tests
import sys,subprocess,os
R=os.path.dirname(os.path.abspath(__file__))+'/../../'
u,sym=sys.argv[1:3]
s=open(R+'src/'+u+'.cpp',encoding='latin-1').read()
pairs=sys.argv[3:]
for i in range(0,len(pairs),2):
    old=open(pairs[i],encoding='latin-1').read().rstrip('\n'); new=open(pairs[i+1],encoding='latin-1').read().rstrip('\n')
    assert s.count(old)==1, 'old not unique/found '+pairs[i]
    s=s.replace(old,new)
d=os.path.dirname(R+'src/'+u+'.cpp')
p=d+'/.variant_tmp.cpp'
open(p,'w',encoding='latin-1').write(s)
r=subprocess.run(['python3',''+R+'tools/decomp/trysym.py',u,p,sym],capture_output=True,text=True)
print(r.stdout.strip() or r.stderr[-1500:]); os.remove(p)
