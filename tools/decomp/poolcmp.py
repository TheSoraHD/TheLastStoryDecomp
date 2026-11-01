#!/usr/bin/env python3
"""poolcmp.py <obj.o> <lo> <hi>: compares the .sdata2 pool of our object with the DOL range lo..hi
(hex) and prints the common prefix (objdiff compares .sdata2 loads by position)."""
import sys,struct,subprocess

from elftools.elf.elffile import ELFFile
obj,lo,hi=sys.argv[1],int(sys.argv[2],16),int(sys.argv[3],16)
import os
R=os.path.dirname(os.path.abspath(__file__))+'/../..'
d=open(R+'/orig/SLSP01/sys/main.dol','rb').read()
offs=struct.unpack('>18I',d[0:72]);addrs=struct.unpack('>18I',d[72:144]);sizes=struct.unpack('>18I',d[144:216])
def rd(a,n):
    for o,ad,s in zip(offs,addrs,sizes):
        if ad<=a<ad+s: return d[o+a-ad:o+a-ad+n]
tgt=[rd(a,4) for a in range(lo,hi,4)]
f=ELFFile(open(obj,'rb'))
data=f.get_section_by_name('.sdata2').data()
ours=[data[i:i+4] for i in range(0,len(data),4)]
n=0
for i,(a,b) in enumerate(zip(tgt,ours)):
    if a!=b:
        print('first diff at index',i,hex(lo+4*i),struct.unpack('>f',a)[0],struct.unpack('>f',b)[0]); break
    n+=1
print('ours',len(ours),'target',len(tgt),'equal prefix',n)
