#!/usr/bin/env python3
# sdata2pair.py <unit>: rename the @N/.lbl labels of the unit's .sdata2 in symbols.txt to the @N of our .o when the
# value at the same offset is the same (pairdata leaves them when ambiguous)
import sys,re,struct
from elftools.elf.elffile import ELFFile
u=sys.argv[1]
import os
R=os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..'))+'/'
f=ELFFile(open(R+'build/SLSP01/src/%s.o'%u,'rb'))
sec=f.get_section_by_name('.sdata2'); data=sec.data()
idx=[i for i,s in enumerate(f.iter_sections()) if s.name=='.sdata2'][0]
ours={}
for s in f.get_section_by_name('.symtab').iter_symbols():
    if s['st_shndx']==idx and s['st_size']==4 and s.name.startswith('@'):
        ours[s['st_value']]=(s.name,data[s['st_value']:s['st_value']+4])
splits=open(R+'config/SLSP01/splits.txt').read()
m=re.search(r'^'+re.escape(u)+r'\.(?:cpp|c):\n((?:\t.*\n)+)',splits,re.M)
mm=re.search(r'\t\.sdata2\s+start:0x([0-9A-F]+) end:0x([0-9A-F]+)',m.group(1))
start,end=int(mm.group(1),16),int(mm.group(2),16)
asm=open(R+'build/SLSP01/asm/%s.s'%u).read()
tgt={}
for m in re.finditer(r'# \.sdata2:0x([0-9A-F]+) \| 0x([0-9A-F]+) \| size: 0x4\n\.obj "?([^",]+)"?, \w+\n(?:\.hidden.*\n)?\t\.(float|4byte) (\S+)',asm):
    tgt[int(m.group(2),16)]=(int(m.group(1),16),m.group(3),m.group(5))
p=R+'config/SLSP01/symbols.txt'
lines=open(p).read().split('\n')
used=set()
for l in lines:
    mm2=re.match(r'(\S+) = \.sdata2:0x([0-9A-F]+);',l)
    if mm2 and start<=int(mm2.group(2),16)<end: used.add(mm2.group(1))
n=0
for i,l in enumerate(lines):
    mm=re.match(r'(\S+) = \.sdata2:0x([0-9A-F]+); // (.*)',l)
    if not mm: continue
    a=int(mm.group(2),16)
    if not (start<=a<end) or a not in tgt: continue
    off=tgt[a][0]
    o=ours.get(off)
    if not o: continue
    val=tgt[a][2]
    try: tv=struct.pack('>f',float(val)) if tgt[a][2] and not val.startswith('0x') else struct.pack('>I',int(val,16))
    except: continue
    if tv!=o[1] or mm.group(1)==o[0]: continue
    if o[0] in used: print('skip (used)',o[0],hex(a)); continue
    attrs=mm.group(3)
    if 'scope:local' not in attrs: attrs=attrs.replace('size:0x4 ','size:0x4 scope:local ')
    print(mm.group(1),'->',o[0],hex(a))
    used.discard(mm.group(1)); used.add(o[0])
    lines[i]='%s = .sdata2:0x%s; // %s'%(o[0],mm.group(2),attrs); n+=1
open(p,'w').write('\n'.join(lines))
print(n,'renamed')
