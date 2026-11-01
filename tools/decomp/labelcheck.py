# labelcheck.py <unidad...>: etiquetas @N de symbols.txt (datos del split) que no existen en nuestro .o
# (tras cambiar una cabecera compartida se renumeran: hay que repetir pd.sh en esas unidades)
import re,subprocess,sys
splits=open('config/SLSP01/splits.txt').read()
syms=[l for l in open('config/SLSP01/symbols.txt')]
def ranges(u):
    m=re.search(r'^'+re.escape(u)+r'\.(?:cpp|c):\n((?:\t.*\n)+)',splits,re.M)
    r=[]
    for l in m.group(1).split('\n'):
        mm=re.match(r'\t(\.\w+)\s+start:0x([0-9A-F]+) end:0x([0-9A-F]+)',l)
        if mm and mm.group(1) in ('.sdata2','.rodata','.data','.bss','.sbss','.sdata'): r.append((int(mm.group(2),16),int(mm.group(3),16)))
    return r
for u in sys.argv[1:]:
    rs=ranges(u)
    names=[]
    for l in syms:
        mm=re.match(r'(@\d+) = \.\w+:0x([0-9A-F]+)',l)
        if mm:
            a=int(mm.group(2),16)
            if any(s<=a<e for s,e in rs): names.append(mm.group(1))
    o=subprocess.run(['/home/claude/binutils/powerpc-eabi-nm','build/SLSP01/src/%s.o'%u],capture_output=True,text=True).stdout
    have=set(x.split()[-1] for x in o.split('\n') if x.strip())
    miss=[n for n in names if n not in have]
    if miss: print(u,len(names),'missing',len(miss))
