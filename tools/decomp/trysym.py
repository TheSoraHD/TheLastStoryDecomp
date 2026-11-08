#!/usr/bin/env python3
# (s12h-20261107) compiles a copy with try.sh and compares function bytes with build/SLSP01/obj (relocation fields masked)
# trysym.py <unit> <file.cpp> <sym...>: compile file.cpp standalone (try.sh) and compare syms with original obj
import sys,subprocess,os
R=os.path.dirname(os.path.abspath(__file__))+'/../../'
sys.path.insert(0,R+'tools/decomp')
u,f=sys.argv[1],sys.argv[2]; SYMS=sys.argv[3:]
out='/tmp/trysym_%d.o'%os.getpid()
env=dict(os.environ,TRY_OUT=out)
for _i in range(3):
    r=subprocess.run([R+'tools/decomp/try.sh',f],env=env,capture_output=True,text=True)
    if os.path.exists(out): break
if not os.path.exists(out): print(r.stdout[:2000]); sys.exit(1)
sys.argv=['cmpsym.py',u]
import importlib.util
from elftools.elf.elffile import ELFFile
spec=importlib.util.spec_from_file_location('c',R+'tools/decomp/cmpsym.py')
src=open(R+'tools/decomp/cmpsym.py').read().split("u=sys.argv[1]")[0]
ns={"__file__":R+"tools/decomp/cmpsym.py"};exec(src,ns)
from elftools.elf.relocation import RelocationSection
def load2(path):
    f=ELFFile(open(path,'rb')); syms={}
    st=f.get_section_by_name('.symtab')
    secs={i:x for i,x in enumerate(f.iter_sections())}
    for x in st.iter_symbols():
        if x['st_info']['type']=='STT_FUNC': syms[x.name]=(x['st_shndx'],x['st_value'],x['st_size'])
    rel={}
    for x in f.iter_sections():
        if isinstance(x,RelocationSection):
            tgt=x['sh_info']
            for r in x.iter_relocations():
                t=r['r_info_type']
                m={4:0xFFFF,5:0xFFFF,6:0xFFFF,109:0x1FFFFF,10:0x03FFFFFC,11:0xFFFC,18:0x03FFFFFC,1:0xFFFFFFFF}.get(t,0xFFFFFFFF)
                rel.setdefault(tgt,{})
                rel[tgt][r['r_offset']&~3]=rel[tgt].get(r['r_offset']&~3,0)|m
    return f,secs,syms,rel
a=load2(out);b=load2(R+'build/SLSP01/obj/'+u+'.o')
def get(x,n):
    f_,secs,syms,rel=x
    if n not in syms: return None
    sh,v,sz=syms[n]; d=secs[sh].data()[v:v+sz]; m=bytearray(d)
    for o,mk in rel.get(sh,{}).items():
        if v<=o<v+sz:
            w=int.from_bytes(m[o-v:o-v+4],'big')&~mk&0xFFFFFFFF
            m[o-v:o-v+4]=w.to_bytes(4,'big')
    return bytes(m)
import sys as _s
if not SYMS: SYMS=sorted(set(a[2])&set(b[2]))
for n in SYMS:
    n,_,t=n.partition('=')
    t=t or n
    x,y=get(a,n),get(b,t)  # ALIAS
    print('OK ' if x is not None and x==y else 'DIFF', n, None if x is None else len(x), None if y is None else len(y))
    if os.environ.get('V') and x and y:
        import subprocess as sp
        def dis(path):
            o=sp.run([R+'build/binutils/powerpc-eabi-objdump','-d','--no-show-raw-insn','--disassemble='+n,path],capture_output=True,text=True).stdout
            return [l.split(':',1)[1].strip() for l in o.splitlines() if l.startswith('  ') and ':' in l]
        def dis2(path,nm):
            o=sp.run([R+'build/binutils/powerpc-eabi-objdump','-d','--no-show-raw-insn','--disassemble='+nm,path],capture_output=True,text=True).stdout
            return [l.split(':',1)[1].strip() for l in o.splitlines() if l.startswith('  ') and ':' in l]
        A=dis(out);B=dis2(R+'build/SLSP01/obj/'+u+'.o',t)
        for i,(p_,q_) in enumerate(zip(A,B)):
            import re as _re
            if _re.sub(r'[0-9a-f]+ <.*','',p_)!=_re.sub(r'[0-9a-f]+ <.*','',q_): print('  %4d %-36s | %s'%(i,p_[:36],q_[:40]))
os.remove(out)
if os.environ.get('V'):
    pass
