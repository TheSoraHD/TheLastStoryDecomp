#!/usr/bin/env python3
# usage: sdkauto.py <src.c> [--like X] : runs sdkcheck.py and, if every function matches, proposes
# the split ranges (text from the functions, data from the symbols they reference). Prints
# 'ADD sec start end | ...' or 'SKIP reason'. Check the ranges against symbols.txt before using them
# (sizes include padding; weak copies and stripped objects are not detected).
import os, sys, re, subprocess
from elftools.elf.elffile import ELFFile
ROOT=os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)),'..','..')); os.chdir(ROOT)
src=sys.argv[1]; like=sys.argv[3] if len(sys.argv)>3 else None
r=subprocess.run(['python3',os.path.join(os.path.dirname(os.path.abspath(__file__)),'sdkcheck.py'),src]+([like] if like else []),capture_output=True,text=True).stdout
last=r.strip().split('\n')[-1]
if not last.startswith('ok') or not last.endswith('bad 0'): print('SKIP code',last); sys.exit()
out=os.environ.get('SDK_OUT','/tmp/sdkcheck_out.o')
syms=[]; byname={}
for l in open('config/SLSP01/symbols.txt'):
    m=re.match(r'(\S+) = \.(\w+):0x([0-9A-F]+); // type:(\w+)(?: size:0x([0-9A-F]+))?',l)
    if m:
        s=(m.group(1),m.group(2),int(m.group(3),16),int(m.group(5) or '0',16),m.group(4)); syms.append(s); byname.setdefault(s[0],[]).append(s)
e=ELFFile(open(out,'rb'))
fn=[];objs={};secsz={}
for s in e.iter_sections():
    if s.name in ('.data','.rodata','.bss','.sdata','.sbss','.sdata2','.sbss2') and s['sh_size']>0: secsz[s.name[1:]]=s['sh_size']
names=set()
for sym in e.get_section_by_name('.symtab').iter_symbols():
    if sym['st_info']['type']=='STT_FUNC' and sym['st_size']>0 and sym.name in byname:
        names.add(sym.name)
        if sym['st_info']['bind']!='STB_WEAK' or os.environ.get('WEAK'):
            fn.append([x for x in byname[sym.name] if x[1]=='text'][0])
fn.sort(key=lambda s:s[2])
lo=fn[0][2]; hi=fn[-1][2]+fn[-1][3]
foreign=[s for s in syms if s[1]=='text' and s[4]=='function' and lo<=s[2]<hi and s[0] not in names]
if foreign: print('SKIP foreign',[x[0] for x in foreign][:5]); sys.exit()
for m_ in re.finditer(r'\.text\s+start:0x([0-9A-F]+) end:0x([0-9A-F]+)',open('config/SLSP01/splits.txt').read()):
    a_,b_=int(m_.group(1),16),int(m_.group(2),16)
    if a_<hi and lo<b_: print('SKIP split exists'); sys.exit()
asmfile=subprocess.run(['grep','-rl','.fn %s,'%fn[0][0],'build/SLSP01/asm'],capture_output=True,text=True).stdout.split()[0]
refs=set(); inr=False
for l in open(asmfile):
    m=re.match(r'\.fn (\S+),',l)
    if m:
        a=[s for s in byname.get(m.group(1),[]) if s[1]=='text']
        inr = bool(a) and lo<=a[0][2]<hi
    if inr:
        for x in re.findall(r'"?([A-Za-z_@$.0-9]+)"?@(?:ha|l|sda21)',l): refs.add(x)
# follow pointers stored in the data objects found
import glob
objblocks={}
def objrefs(name):
    if not objblocks:
        for f in glob.glob('build/SLSP01/asm/**/*.s',recursive=True):
            cur=None
            for l in open(f,errors='ignore'):
                m=re.match(r'\.obj (\S+),',l)
                if m: cur=m.group(1); objblocks[cur]=[]; continue
                if l.startswith('.endobj'): cur=None; continue
                if cur:
                    for x in re.findall(r'\.4byte ([A-Za-z_@$.0-9]+)',l): objblocks[cur].append(x)
    return objblocks.get(name,[])
todo=list(refs)
while todo:
    x=todo.pop()
    for y in objrefs(x):
        if y not in refs: refs.add(y); todo.append(y)
res=['text %X %X'%(lo,hi)]
for sec,sz in secsz.items():
    rs=[]
    for x in refs:
        base=re.sub(r'_[0-9A-F]{8}$','',x)
        c=[s for s in byname.get(x,[])+byname.get(base,[]) if s[1]==sec]
        if x.startswith('lbl_') or re.search(r'_[0-9A-F]{8}$',x):
            a=int(x[-8:],16); c=[s for s in c if s[2]==a]
        rs+=c
    rs=sorted(set(rs),key=lambda s:s[2])
    if not rs: res.append('?'+sec+'(unref)'); continue
    a=rs[0][2]; b=max(s[2]+s[3] for s in rs)
    al = 8 if sec in ('data','rodata','bss') else 4
    span=b-a
    if not (0<=span-sz<8 or 0<=sz-span<16):
        print('SKIP size',sec,hex(a),hex(b),'ours',hex(sz),[(q[0],hex(q[2]),hex(q[3])) for q in rs]); res.append('?'+sec); continue
    end=max(a+sz,b)
    if end%4:
        nx=[q[2] for q in syms if q[1]==sec and q[2]>=end]
        end=min(nx) if nx else (end+3)&~3
    res.append('%s %X %X'%(sec,a,end))
print('ADD',' | '.join(res))
