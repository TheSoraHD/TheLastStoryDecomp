# lblremap.py <unit...>: exact re-pairing after a header change that only renumbers @N. BASEDIR=<dir> holds copies
# of the .o files from before the change (<dir>/<unit>.o); every @N of symbols.txt in the unit ranges is renamed
# to the label of the new build/SLSP01/src/<unit>.o at the same section and offset. Run configure afterwards.
# Check first that the code is equal apart from label names (objdump -d | sed 's/@[0-9]*//g').
import sys, re, subprocess
S='config/SLSP01/symbols.txt'
import os
BASE=lambda u: os.environ['BASEDIR']+'/'+u+'.o'
def syms(o):
    out=subprocess.run(['build/binutils/powerpc-eabi-objdump','-t',o],capture_output=True,text=True).stdout
    d={}
    for l in out.splitlines():
        m=re.match(r'([0-9a-f]{8}) (.{7}) (\S+)\s+([0-9a-f]{8}) (\S+)$',l)
        if m and m[5].startswith('@'): d[m[5]]=(m[3],int(m[1],16))
    return d
ranges={}
cur=None
for l in open('config/SLSP01/splits.txt'):
    if l and not l[0].isspace() and l.strip().endswith(':'): cur=l.strip()[:-1].replace('.cpp','').replace('.c','')
    m=re.search(r'start:0x([0-9A-F]+) end:0x([0-9A-F]+)',l)
    if m and cur: ranges.setdefault(cur,[]).append((int(m[1],16),int(m[2],16)))
lines=open(S).read().splitlines(True)
tot=0
for u in sys.argv[1:]:
    old=syms(BASE(u)); new=syms('build/SLSP01/src/'+u+'.o')
    inv={v:k for k,v in new.items()}
    mp={k:inv[v] for k,v in old.items() if v in inv}
    n=0
    for i,l in enumerate(lines):
        m=re.match(r'(@\d+) = \S+:0x([0-9A-F]+);',l)
        if not m: continue
        a=int(m[2],16)
        if not any(s<=a<e for s,e in ranges.get(u,[])): continue
        if m[1] in mp and mp[m[1]]!=m[1]:
            lines[i]=l.replace(m[1]+' =',mp[m[1]]+' =',1); n+=1
    print(u,n,'renamed'); tot+=n
open(S,'w').writelines(lines)
