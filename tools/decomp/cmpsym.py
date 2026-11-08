#!/usr/bin/env python3
# cmpsym.py <unit> ours=target ... : compares function bytes (masking reloc'd words) between our .o and target .o
import sys
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection
import os
R=os.path.dirname(os.path.abspath(__file__))+'/../../build/SLSP01/'
def load(p):
    f=ELFFile(open(p,'rb')); syms={}; 
    st=f.get_section_by_name('.symtab')
    secs={i:s for i,s in enumerate(f.iter_sections())}
    for s in st.iter_symbols():
        if s['st_info']['type']=='STT_FUNC': syms[s.name]=(s['st_shndx'],s['st_value'],s['st_size'])
    rel={}
    for s in f.iter_sections():
        if isinstance(s,RelocationSection):
            tgt=s['sh_info']
            for r in s.iter_relocations(): rel.setdefault(tgt,set()).add(r['r_offset']&~3)
    return f,secs,syms,rel
u=sys.argv[1]
a=load(R+'src/'+u+'.o'); b=load(R+'obj/'+u+'.o')
def get(x,n):
    f,secs,syms,rel=x
    if n not in syms: return None
    sh,v,sz=syms[n]; d=secs[sh].data()[v:v+sz]
    m=bytearray(d)
    for o in rel.get(sh,()):
        if v<=o<v+sz: m[o-v:o-v+4]=b'\0\0\0\0'
    return bytes(m)
for pair in sys.argv[2:]:
    o,t=pair.split('=')
    x,y=get(a,o),get(b,t)
    print('OK ' if x is not None and x==y else 'DIFF', t, o, None if x is None else len(x), None if y is None else len(y))
