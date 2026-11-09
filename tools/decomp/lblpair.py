# lblpair.py <unit> <func...>: renames in symbols.txt the @N labels that differ only by name in the diff of the functions
import re,subprocess,sys
u=sys.argv[1]
ren={}
for f in sys.argv[2:]:
    out=subprocess.run(['python3','tools/decomp/odiff2.py',u,f],capture_output=True,text=True).stdout
    for l in out.split('\n'):
        if 'ARG_MISMATCH' not in l: continue
        a,_,b=l.partition('|')
        ma=re.findall(r'(@\d+)@',a); mb=re.findall(r'(@\d+)@',b)
        if len(ma)==1 and len(mb)==1 and ma[0]!=mb[0]: ren[ma[0]]=mb[0]
print(ren)
p='config/SLSP01/symbols.txt'; s=open(p).read()
for a,b in ren.items():
    if re.search(r'^'+re.escape(b)+' = ',s,re.M): print('skip, exists',b); continue
    s=re.sub(r'^'+re.escape(a)+' = ',b+' = ',s,flags=re.M)
open(p,'w').write(s)
