#!/usr/bin/env python3
# refmap.py unit...: rename the @N labels of the unit in symbols.txt by going through clean main:
# main symbols name -> main .o (section, offset) -> our .o name at the same (section, offset)
import re,subprocess,sys
R='/home/claude/TheLastStoryDecomp/'; M='/home/claude/mainref/'
sp=open(R+'config/SLSP01/splits.txt').read()
def rng(u):
    m=re.search(r'^'+re.escape(u)+r'\.(?:cpp|c):\n((?:\t.*\n)+)',sp,re.M); r=[]
    for l in m.group(1).split('\n'):
        mm=re.match(r'\t(\.\w+)\s+start:0x([0-9A-F]+) end:0x([0-9A-F]+)',l)
        if mm and mm.group(1)!='.text': r.append((mm.group(1),int(mm.group(2),16),int(mm.group(3),16)))
    return r
def labels(o):
    out=subprocess.run(['/home/claude/binutils/powerpc-eabi-objdump','-t',o],capture_output=True,text=True).stdout
    byname={}; bypos={}
    for l in out.split('\n'):
        t=l.split()
        if len(t)>=5 and t[-1].startswith('@'):
            sec=t[-3]; off=int(t[0],16); byname.setdefault(t[-1],(sec,off)); bypos.setdefault((sec,off),t[-1])
    return byname,bypos
msyms=open(M+'config/SLSP01/symbols.txt').read().split('\n')
cur=open(R+'config/SLSP01/symbols.txt').read().split('\n')
n=0
for u in sys.argv[1:]:
    rs=rng(u)
    mb,_=labels(M+'build/SLSP01/src/%s.o'%u); _,nb=labels(R+'build/SLSP01/src/%s.o'%u)
    for i,l in enumerate(msyms):
        m=re.match(r'(@\d+) = (\.\w+):0x([0-9A-F]+);',l)
        if not m: continue
        a=int(m.group(3),16)
        if not any(s==m.group(2) and lo<=a<hi for s,lo,hi in rs): continue
        if m.group(1) not in mb: continue
        new=nb.get(mb[m.group(1)])
        if not new: continue
        # find same address line in current
        for j in (i,):
            c=cur[j]
            mm=re.match(r'(\S+) = (\.\w+):0x([0-9A-F]+);',c)
            if mm and int(mm.group(3),16)==a and mm.group(1)!=new:
                cur[j]=new+c[len(mm.group(1)):]; n+=1
open(R+'config/SLSP01/symbols.txt','w').write('\n'.join(cur)); print('renamed',n)
