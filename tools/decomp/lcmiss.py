#!/usr/bin/env python3
# lcmiss.py <unidad>: etiquetas @N del split que faltan en nuestro .o y las @N del .o que no estan en symbols.txt (con seccion y offset)
import re,subprocess,sys
u=sys.argv[1]
splits=open('config/SLSP01/splits.txt').read();syms=open('config/SLSP01/symbols.txt').read().split('\n')
m=re.search(r'^'+re.escape(u)+r'\.(?:cpp|c):\n((?:\t.*\n)+)',splits,re.M)
rs=[(s,int(a,16),int(b,16)) for s,a,b in re.findall(r'\t(\.\w+)\s+start:0x([0-9A-F]+) end:0x([0-9A-F]+)',m.group(1))]
o=subprocess.run(['/home/claude/binutils/powerpc-eabi-objdump','-t','build/SLSP01/src/%s.o'%u],capture_output=True,text=True).stdout
have={}
for l in o.split('\n'):
    p=l.split()
    if len(p)>=5 and p[-1].startswith('@'): have[p[-1]]=(p[-3] if len(p)>=6 else p[-3],p[0],p[-2])
known=set()
for l in syms:
    mm=re.match(r'(@\d+) = (\.\w+):0x([0-9A-F]+)',l)
    if mm:
        a=int(mm.group(3),16)
        for s,b,e in rs:
            if s==mm.group(2) and b<=a<e:
                known.add(mm.group(1))
                if mm.group(1) not in have: print('falta',l[:80],'off 0x%X'%(a-b))
for n,(sec,off,size) in have.items():
    if n not in known and re.match(r'@\d+$',n): print('nueva',n,sec,off,size)
