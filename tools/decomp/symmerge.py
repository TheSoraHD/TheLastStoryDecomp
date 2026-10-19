#!/usr/bin/env python3
"""Resolve git conflict blocks in symbols.txt by address: prefer real names over lbl_/fn_/gap_."""
import re,sys
p=sys.argv[1]; s=open(p).read()
def auto(n): return re.match(r'(lbl|fn|gap)_[0-9A-F_]+',n) is not None or n.startswith('gap_')
def parse(block):
    out={}
    for l in block.strip('\n').split('\n'):
        if not l.strip(): continue
        m=re.match(r'(\S+) = \.(\w+):0x([0-9A-F]+);',l)
        out[(m.group(2),int(m.group(3),16))]=(m.group(1),l)
    return out
def fix(m):
    o,t=parse(m.group(1)),parse(m.group(2))
    res={}
    for k in set(o)|set(t):
        a,b=o.get(k),t.get(k)
        if a and b: res[k]=b[1] if (auto(a[0]) and not auto(b[0])) else a[1]
        else: res[k]=(a or b)[1]
    return '\n'.join(res[k] for k in sorted(res))+'\n'
s2=re.sub(r'<<<<<<< ours\n(.*?)=======\n(.*?)>>>>>>> theirs\n',fix,s,flags=re.S)
open(p,'w').write(s2)
print('blocks resolved:',s.count('<<<<<<< ours'))
