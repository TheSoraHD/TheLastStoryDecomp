#!/usr/bin/env python3
# remap.py <copia_agente> <unidad...>: renombra las @N de symbols.txt del split de cada unidad con el nombre que tiene
# en nuestro .o el simbolo que, en el .o de la copia del agente, lleva el nombre que el agente puso a esa direccion
# (misma seccion, desplazamiento y tamano). Para re-emparejar tras un merge en conflicto.
import subprocess,re,sys
W=sys.argv[1]
def syms(path):
    o=subprocess.run(['/home/claude/binutils/powerpc-eabi-objdump','-t',path],capture_output=True,text=True).stdout
    d={}
    for l in o.split('\n'):
        p=l.split()
        if len(p)>=5 and p[-1].startswith('@'): d[p[-1]]=(p[-3],p[0],p[-2])
    return d
pl={}
for l in open(W+'/config/SLSP01/symbols.txt'):
    mm=re.match(r'^(@\d+) = (\.\w+):0x([0-9A-F]+);',l)
    if mm: pl[(mm.group(2),mm.group(3))]=mm.group(1)
splits=open('config/SLSP01/splits.txt').read()
L=open('config/SLSP01/symbols.txt').read().split('\n')
for u in sys.argv[2:]:
    P=syms(W+'/build/SLSP01/src/%s.o'%u); N=syms('build/SLSP01/src/%s.o'%u)
    inv={}
    for k,v in N.items(): inv.setdefault(v,[]).append(k)
    m=re.search(r'^'+re.escape(u)+r'\.(?:cpp|c):\n((?:\t.*\n)+)',splits,re.M)
    rs=[(s,int(a,16),int(b,16)) for s,a,b in re.findall(r'\t(\.\w+)\s+start:0x([0-9A-F]+) end:0x([0-9A-F]+)',m.group(1))]
    ch=0
    for i,s in enumerate(L):
        mm=re.match(r'^(@\d+) = (\.\w+):0x([0-9A-F]+);',s)
        if not mm: continue
        a=int(mm.group(3),16)
        if not any(sec==mm.group(2) and b<=a<e for sec,b,e in rs): continue
        pn=pl.get((mm.group(2),mm.group(3)))
        if not pn or pn not in P: continue
        c=inv.get(P[pn])
        if c and len(c)==1 and c[0]!=mm.group(1): L[i]=c[0]+s[len(mm.group(1)):]; ch+=1
    print(u,'cambios',ch)
open('config/SLSP01/symbols.txt','w').write('\n'.join(L))
