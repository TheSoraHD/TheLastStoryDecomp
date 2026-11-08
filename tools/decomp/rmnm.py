#!/usr/bin/env python3
# rmnm.py <unit> <fn>: removes the NONMATCHING comment (with its continuation lines) above the definition of fn
import re,sys,os
R=os.path.dirname(os.path.abspath(__file__))+'/../../'
un,fn=sys.argv[1:3]
p=R+'src/'+un+'.cpp'
L=open(p,encoding='latin-1').read().split('\n')
meth=fn.split('__')[0] if not fn.startswith('__') else None
cls=None
m=re.match(r'^\w+?__(?:Q\d)?(\d+)(\w+?)F',fn)
if fn.startswith('__ct__'):
    mm=re.match(r'__ct__(?:Q\d(?:\d+\w+?)*?)?(\d+)(\w+?)F',fn); meth=mm.group(2)[:int(mm.group(1))]
idx=[i for i,l in enumerate(L) if re.search(r'\b'+re.escape(meth)+r'\(',l) and not l.rstrip().endswith(';') and not l.startswith(' ') and not l.startswith('//')]
if m and not fn.startswith('__ct__'):
    c=m.group(2)[:int(m.group(1))]
    idx2=[i for i in idx if c+'::'+meth in L[i]]
    if idx2: idx=idx2
if len(idx)!=1: print('AMBIG',un,fn,idx); sys.exit(1)
d=idx[0]; j=d-1
while j>=0 and L[j].startswith('//'): j-=1
blk=list(range(j+1,d))
k=[i for i in blk if L[i].startswith('// NONMATCHING')]
if not k: print('NONE',un,fn); sys.exit(0)
k=k[0]; e=k+1
while e<d and not (L[e].startswith('// inferred') or L[e].startswith('// NONMATCHING')): e+=1
print('REMOVED',un,fn,'|',' '.join(L[k:e])[:120])
del L[k:e]
open(p,'w',encoding='latin-1').write('\n'.join(L))
