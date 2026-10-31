#!/usr/bin/env python3
# usage: libpair.py <src> [<libs_ref unit for flags>] [--apply] : after compiling with sdkcheck.py, find
# for every function of our object without a symbol the fn_ symbols of main.dol with the same
# size and the same code (relocated words masked); with --apply renames the unique ones in symbols.txt.
import os, sys, re, struct, subprocess
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection
ROOT=os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)),'..','..')); os.chdir(ROOT)
args=[a for a in sys.argv[1:] if a!='--apply']; apply='--apply' in sys.argv
subprocess.run(['python3',os.path.join(os.path.dirname(os.path.abspath(__file__)),'sdkcheck.py')]+args,capture_output=True)
out=os.environ.get('SDK_OUT','/tmp/sdkcheck_out.o')
txt=open('config/SLSP01/symbols.txt').read()
syms={}; fns=[]
for m in re.finditer(r'^(\S+) = \.text:0x([0-9A-F]+); // type:function size:0x([0-9A-F]+)',txt,re.M):
    syms.setdefault(m.group(1),[]).append(int(m.group(2),16))
    if m.group(1).startswith('fn_'): fns.append((m.group(1),int(m.group(2),16),int(m.group(3),16)))
dol=open('orig/SLSP01/sys/main.dol','rb').read()
offs=struct.unpack('>18I',dol[0:72]); addrs=struct.unpack('>18I',dol[72:144]); sizes=struct.unpack('>18I',dol[144:216])
def rd(a,n):
    for o,ad,s in zip(offs,addrs,sizes):
        if ad<=a<ad+s: return dol[o+a-ad:o+a-ad+n]
e=ELFFile(open(out,'rb')); relocs={}
for s in e.iter_sections():
    if isinstance(s,RelocationSection):
        for r in s.iter_relocations(): relocs.setdefault(s['sh_info'],set()).add(r['r_offset']&~3)
bysize={}
for f in fns: bysize.setdefault(f[2],[]).append(f)
ren=[]
for sym in e.get_section_by_name('.symtab').iter_symbols():
    if sym['st_info']['type']!='STT_FUNC' or sym['st_size']==0 or sym.name in syms: continue
    sec=e.get_section(sym['st_shndx']); data=sec.data()[sym['st_value']:sym['st_value']+sym['st_size']]
    rel=relocs.get(sym['st_shndx'],set())
    mask=[i for i in range(0,len(data),4) if sym['st_value']+i not in rel]
    c=[f for f in bysize.get(len(data),[]) if all(rd(f[1]+i,4)==data[i:i+4] for i in mask)]
    print(sym.name, [ (f[0]) for f in c][:4])
    if len(c)==1: ren.append((c[0][0],sym.name))
if apply:
    for old,new in ren:
        if new+' =' in txt: continue
        txt=re.sub(r'^'+old+r' =', new+' =', txt, flags=re.M)
    open('config/SLSP01/symbols.txt','w').write(txt)
    print('renamed',len(ren))
