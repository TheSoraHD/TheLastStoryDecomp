#!/usr/bin/env python3
# usage: datafind.py [obj, default /tmp/sdkcheck_out.o]: for every data section of the object
# (.data .rodata .bss .sdata .sdata2 .sbss), find where its bytes are in main.dol (relocated words
# masked); prints the candidate ranges. Run sdkcheck.py first. .bss/.sbss: only the size is printed.
import os, sys, struct
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection
ROOT=os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)),'..','..')); os.chdir(ROOT)
obj=sys.argv[1] if len(sys.argv)>1 else '/tmp/sdkcheck_out.o'
d=open('orig/SLSP01/sys/main.dol','rb').read()
offs=struct.unpack('>18I',d[0:72]);addr=struct.unpack('>18I',d[72:144]);sz=struct.unpack('>18I',d[144:216])
secs=[(addr[i],d[offs[i]:offs[i]+sz[i]]) for i in range(18) if sz[i]]
e=ELFFile(open(obj,'rb'))
for s in e.iter_sections():
    if s.name not in ('.data','.rodata','.sdata','.sdata2','.bss','.sbss'): continue
    if s.name in ('.bss','.sbss'): print(s.name,'size',hex(s['sh_size'])); continue
    data=bytearray(s.data()); mask=[True]*len(data)
    rs=e.get_section_by_name('.rela'+s.name)
    if rs:
        for r in rs.iter_relocations():
            o=r['r_offset']
            for k in range(o,min(o+4,len(data))): mask[k]=False
    n=len(data)
    if n==0: continue
    # anchor: longest run of unmasked bytes
    best=(0,0);i=0
    while i<n:
        if mask[i]:
            j=i
            while j<n and mask[j]: j+=1
            if j-i>best[1]-best[0]: best=(i,j)
            i=j
        else: i+=1
    a0,a1=best
    hits=[]
    if a1-a0>=4:
        anchor=bytes(data[a0:a1])
        for base,sec in secs:
            k=sec.find(anchor)
            while k>=0:
                st=k-a0
                if st>=0 and st+n<=len(sec) and all((not mask[t]) or sec[st+t]==data[t] for t in range(n)):
                    hits.append(base+st)
                k=sec.find(anchor,k+1)
    print(s.name,'size',hex(n),'at',' '.join(hex(h) for h in hits[:5]) if hits else 'NOT FOUND (anchor %d bytes)'%(a1-a0))
