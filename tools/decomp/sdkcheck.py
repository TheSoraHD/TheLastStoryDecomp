#!/usr/bin/env python3
# usage: sdkcheck.py <src.c> [<libs_ref unit suffix for the flags, default OSMutex.c>]
# Compiles a library source with the flags of that libs_ref unit and compares every function with
# the same-named symbol of symbols.txt in main.dol (relocated words masked). V=1 prints the first
# differing words. COMPILERS (default build/compilers) and WIBO (default build/tools/wibo).
import os, sys, json, re, struct, subprocess
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection
ROOT=os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)),'..','..')); os.chdir(ROOT)
src=sys.argv[1]
d=json.load(open('config/SLSP01/libs_ref.json'))
ref=[u for u in d if u['name'].endswith('OSMutex.c')][0]
if len(sys.argv)>2: ref=[u for u in d if u['name'].endswith(sys.argv[2])][0]
fl=ref['cflags']; mw=ref['mw_version']
out=os.environ.get('SDK_OUT','/tmp/sdkcheck_out.o')
# keep pragma quoting
cmd=[os.environ.get('WIBO','build/tools/wibo'),os.path.join(os.environ.get('COMPILERS','build/compilers'),mw,'mwcceppc.exe')]
for f in fl:
    cmd+= f.split(' ',1) if f.startswith('-pragma') or f.startswith('-i ') or f.startswith('-O') else f.split()
cmd=[c.replace('"','') for c in cmd]+['-c',src,'-o',out]
r=subprocess.run(cmd,capture_output=True,text=True)
if r.returncode: print(r.stdout[-3000:],r.stderr[-2000:]); sys.exit(1)
syms={}
for l in open('config/SLSP01/symbols.txt'):
    m=re.match(r'(\S+) = \.text:0x([0-9A-F]+); // type:function size:0x([0-9A-F]+)',l)
    if m: syms.setdefault(m.group(1),(int(m.group(2),16),int(m.group(3),16)))
for kv in os.environ.get('OVR','').split(','):
    if '=' in kv:
        k,v=kv.split('='); a=int(v,16)
        szs=[int(m.group(1),16) for m in [re.search(r'0x%X; // type:function size:0x([0-9A-F]+)'%a, open('config/SLSP01/symbols.txt').read())] if m]
        syms[k]=(a,szs[0] if szs else 0)
dol=open('orig/SLSP01/sys/main.dol','rb').read()
offs=struct.unpack('>18I',dol[0:72]); addrs=struct.unpack('>18I',dol[72:144]); sizes=struct.unpack('>18I',dol[144:216])
def rd(a,n):
    for o,ad,s in zip(offs,addrs,sizes):
        if ad<=a<ad+s: return dol[o+a-ad:o+a-ad+n]
with open(out,'rb') as f:
    e=ELFFile(f); relocs={}
    for s in e.iter_sections():
        if isinstance(s,RelocationSection):
            for rr in s.iter_relocations(): relocs.setdefault(s['sh_info'],set()).add(rr['r_offset']&~3)
    st=e.get_section_by_name('.symtab'); ok=bad=0
    for sym in st.iter_symbols():
        if sym['st_info']['type']!='STT_FUNC' or sym['st_size']==0: continue
        sec=e.get_section(sym['st_shndx']); data=sec.data()[sym['st_value']:sym['st_value']+sym['st_size']]
        name=sym.name
        if name not in syms: print('NOSYM',name,hex(sym['st_size'])); continue
        a,sz=syms[name]; t=rd(a,sz)
        rel=relocs.get(sym['st_shndx'],set())
        nd=0
        for i in range(0,max(len(data),sz),4):
            if sym['st_value']+i in rel: continue
            if data[i:i+4]!=t[i:i+4]:
                nd+=1
                if os.environ.get('V') and nd<6: print('   +%x ours %s target %s'%(i,data[i:i+4].hex(),t[i:i+4].hex()))
        st_='OK ' if nd==0 and len(data)==sz else 'DIFF'
        if nd==0 and len(data)==sz: ok+=1
        else: bad+=1
        print(st_,name,hex(len(data)),hex(sz),nd)
    print('ok',ok,'bad',bad)
