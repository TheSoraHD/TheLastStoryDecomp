# callmap.py [--apply]: after sdkcheck (/tmp/sdkcheck_out.o), for every function whose target address is known, decode the target's branch at each of our REL24 relocations and propose fn_ -> our callee name
import sys,re,struct,subprocess
import os
os.chdir(os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "..")))
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection
apply='--apply' in sys.argv
syms={};byaddr={}
for l in open('config/SLSP01/symbols.txt'):
    m=re.match(r'(\S+) = \.text:0x([0-9A-F]+); // type:function size:0x([0-9A-F]+)',l)
    if m: syms.setdefault(m.group(1),int(m.group(2),16)); byaddr[int(m.group(2),16)]=m.group(1)
dol=open('orig/SLSP01/sys/main.dol','rb').read()
offs=struct.unpack('>18I',dol[0:72]); addrs=struct.unpack('>18I',dol[72:144]); sizes=struct.unpack('>18I',dol[144:216])
def rd(a):
    for o,ad,s in zip(offs,addrs,sizes):
        if ad<=a<ad+s: return struct.unpack('>I',dol[o+a-ad:o+a-ad+4])[0]
res={}
import os,re as _re
EXCL=os.environ.get('EXCL')
with open('/tmp/sdkcheck_out.o','rb') as f:
    e=ELFFile(f); st=e.get_section_by_name('.symtab'); symlist=list(st.iter_symbols())
    funcs=[(s['st_shndx'],s['st_value'],s['st_size'],s.name) for s in symlist if s['st_info']['type']=='STT_FUNC' and s['st_size']]
    for sec in e.iter_sections():
        if not isinstance(sec,RelocationSection): continue
        tsec=sec['sh_info']
        for r in sec.iter_relocations():
            typ=r['r_info_type']
            if typ not in (10,6,4): continue  # REL24, ADDR16_HA, ADDR16_LO
            off=r['r_offset']; tgt=symlist[r['r_info_sym']].name
            if typ in (6,4):
                if typ!=6: continue
                # pair: find the matching LO in same function later
                lo=[rr for rr in sec.iter_relocations() if rr['r_info_type']==4 and rr['r_info_sym']==r['r_info_sym'] and rr['r_offset']>off]
                if not lo: continue
                lo=min(lo,key=lambda x:x['r_offset'])
                for shn,v,sz,n in funcs:
                    if shn==tsec and v<=off<v+sz and n in syms and lo['r_offset']<v+sz and not (EXCL and _re.search(EXCL,n)):
                        a=syms[n]+off-v; b=syms[n]+lo['r_offset']-v
                        wa=rd(a&~3); wb=rd(b&~3)
                        if wa is None or wb is None: continue
                        hi=wa&0xffff; lw=wb&0xffff
                        if lw&0x8000: lw-=0x10000
                        dest=((hi<<16)+lw)&0xffffffff; dn=byaddr.get(dest)
                        if dn and dn!=tgt and dn.startswith('fn_'): res.setdefault(dn,set()).add(tgt)
                continue
            for shn,v,sz,n in funcs:
                if shn==tsec and v<=off<v+sz and n in syms and not (EXCL and _re.search(EXCL,n)):
                    a=syms[n]+off-v; w=rd(a)
                    if w is None or (w>>26)!=18: continue
                    li=w&0x03FFFFFC
                    if li&0x02000000: li-=0x04000000
                    dest=a+li; dn=byaddr.get(dest)
                    if dn and dn!=tgt and (dn.startswith('fn_')): res.setdefault(dn,set()).add(tgt)
for a,b in sorted(res.items()): print(a,' '.join(b))
if apply:
    al=[]
    for a,b in res.items():
        if len(b)==1: al+=[a,list(b)[0]]
    if al: print(subprocess.run(['python3','tools/decomp/rensym.py']+al,capture_output=True,text=True).stdout[-200:])
