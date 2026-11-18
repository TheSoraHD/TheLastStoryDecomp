#!/usr/bin/env python3
# lcfix.py <unidad> [--apply]: renombra en symbols.txt las @N que faltan en nuestro .o por la @N nueva del .o en la misma seccion y desplazamiento
import re,subprocess,sys
u=sys.argv[1]; ap='--apply' in sys.argv
out=subprocess.run(['python3','tools/decomp/lcmiss.py',u],capture_output=True,text=True).stdout.split('\n')
new={}
for l in out:
    m=re.match(r'nueva (@\d+) (\.\w+) ([0-9a-f]+) ([0-9a-f]+)',l)
    if m: new[(m.group(2),int(m.group(3),16))]=m.group(1)
p='config/SLSP01/symbols.txt';L=open(p).read().split('\n')
splits=open('config/SLSP01/splits.txt').read()
for l in out:
    m=re.match(r'falta (@\d+) = (\.\w+):0x([0-9A-F]+);.* off 0x([0-9A-F]+)',l)
    if not m: continue
    k=(m.group(2),int(m.group(4),16))
    if k in new:
        for i,s in enumerate(L):
            if s.startswith(m.group(1)+' = %s:0x%s;'%(m.group(2),m.group(3))):
                print(m.group(1),'->',new[k],m.group(3)); L[i]=new[k]+s[len(m.group(1)):]
    else: print('sin pareja',m.group(1),m.group(2),m.group(3))
if ap: open(p,'w').write('\n'.join(L))
