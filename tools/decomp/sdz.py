# sdz.py: unidades sin enlazar con globales a cero en .sdata (symbols.txt + main.dol original): candidatas a explicit_zero_data. Ejecutar desde la raiz del repo.
import re,json,os,subprocess
import os
R=os.getcwd()
sp=open(R+'/config/SLSP01/splits.txt').read()
rep=json.load(open(R+'/build/SLSP01/report.json'))
done={u['name'][5:] for u in rep['units'] if u['measures'].get('complete_code_percent',0)==100 and u['measures'].get('complete_data_percent',0)==100}
syms=[]
for l in open(R+'/config/SLSP01/symbols.txt'):
    m=re.match(r'(\S+) = \.sdata:0x([0-9A-F]+); // type:object size:0x([0-9A-F]+)',l)
    if m: syms.append((int(m.group(2),16),int(m.group(3),16),m.group(1)))
dol=open(R+'/orig/SLSP01/sys/main.dol','rb').read()
import struct
offs=struct.unpack('>18I',dol[0:72]);addr=struct.unpack('>18I',dol[72:144]);size=struct.unpack('>18I',dol[144:216])
def rd(a,n):
    for o,ad,sz in zip(offs,addr,size):
        if ad<=a<ad+sz: return dol[o+a-ad:o+a-ad+n]
for m in re.finditer(r'^(\S+)\.(cpp|c):\n((?:\t.*\n)+)',sp,re.M):
    u=m.group(1); sd=re.search(r'\.sdata\s+start:0x([0-9A-F]+) end:0x([0-9A-F]+)',m.group(3))
    if not sd or u in done: continue
    a,b=int(sd.group(1),16),int(sd.group(2),16)
    z=[(n,hex(x)) for x,s,n in syms if a<=x<b and rd(x,s)==b'\0'*s]
    if z: print('%s\t%d\t%s'%(u,len(z),' '.join(n for n,_ in z[:6])+(' ...' if len(z)>6 else '')))
