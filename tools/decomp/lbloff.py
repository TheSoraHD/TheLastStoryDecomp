#!/usr/bin/env python3
# (s12h-20261108) lbloff.py <unit> [--apply]: renames the @N labels of symbols.txt that our .o no longer has
# to the @N of our .o at the same offset of the same section (after a header change renumbered them and
# pairdata.py finds them ambiguous). Only when our label at that offset is not used by another line.
import re,subprocess,sys
u=sys.argv[1]; apply='--apply' in sys.argv
splits=open('config/SLSP01/splits.txt').read()
m=re.search(r'^'+re.escape(u)+r'\.(?:cpp|c):\n((?:\t.*\n)+)',splits,re.M)
rs={}
for l in m.group(1).split('\n'):
    mm=re.match(r'\t(\.\w+)\s+start:0x([0-9A-F]+) end:0x([0-9A-F]+)',l)
    if mm: rs[mm.group(1)]=(int(mm.group(2),16),int(mm.group(3),16))
o=subprocess.run(['build/binutils/powerpc-eabi-objdump','-t','build/SLSP01/src/%s.o'%u],capture_output=True,text=True).stdout
ours={}
for l in o.split('\n'):
    p=l.split()
    if len(p)>=6 and p[-1].startswith('@') and p[-3] in rs: ours.setdefault(p[-3],{})[int(p[0],16)]=p[-1]
have=set(n for d in ours.values() for n in d.values())
L=open('config/SLSP01/symbols.txt').read().split('\n')
used=set()
for i,l in enumerate(L):
    mm=re.match(r'(@\d+) = (\.\w+):0x([0-9A-F]+)',l)
    if mm and mm.group(2) in rs and rs[mm.group(2)][0]<=int(mm.group(3),16)<rs[mm.group(2)][1]: used.add(mm.group(1))
n=0
for i,l in enumerate(L):
    mm=re.match(r'(@\d+) = (\.\w+):0x([0-9A-F]+)',l)
    if not mm or mm.group(2) not in rs: continue
    s,e=rs[mm.group(2)]; a=int(mm.group(3),16)
    if not (s<=a<e) or mm.group(1) in have: continue
    new=ours.get(mm.group(2),{}).get(a-s)
    if not new or new in used: print('skip',mm.group(1),hex(a)); continue
    print(mm.group(1),'->',new); n+=1
    if apply: L[i]=new+l[len(mm.group(1)):]; used.add(new)
if apply: open('config/SLSP01/symbols.txt','w').write('\n'.join(L))
print(n,'renamed' if apply else 'to rename')
