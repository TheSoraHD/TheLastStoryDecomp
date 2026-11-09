# lblfix.py <unit>: renames the @N labels of the unit data in symbols.txt to the names in our .o at the same offsets
# (only right when the pools of our .o are laid out as the original; otherwise use lblpair.py)
import re,subprocess,sys
u=sys.argv[1]
splits=open('config/SLSP01/splits.txt').read()
m=re.search(r'^'+re.escape(u)+r'\.(?:cpp|c):\n((?:\t.*\n)+)',splits,re.M)
secs={s:(int(a,16),int(b,16)) for s,a,b in re.findall(r'\t(\.\w+)\s+start:0x([0-9A-F]+) end:0x([0-9A-F]+)',m.group(1)) if s!='.text'}
o=subprocess.run(['powerpc-linux-gnu-objdump','-t','build/SLSP01/src/%s.o'%u],capture_output=True,text=True).stdout
ours={}
for l in o.split('\n'):
    f=l.split()
    if len(f)>=6 and f[-1].startswith('@') and f[-3] in secs: ours[(f[-3],int(f[0],16))]=f[-1]
p='config/SLSP01/symbols.txt';L=open(p).read().split('\n');n=0
for i,l in enumerate(L):
    mm=re.match(r'(@\d+) = (\.\w+):0x([0-9A-F]+);',l)
    if mm and mm.group(2) in secs:
        a=int(mm.group(3),16); s,e=secs[mm.group(2)]
        if s<=a<e:
            nm=ours.get((mm.group(2),a-s))
            if nm and nm!=mm.group(1): L[i]=nm+l[len(mm.group(1)):]; n+=1
open(p,'w').write('\n'.join(L)); print('renamed',n)
