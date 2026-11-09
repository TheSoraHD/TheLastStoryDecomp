# lblmap.py <unit>: renames the @N labels of the data of the unit in symbols.txt from the label pairs seen in
# the diffs of all its functions (when lblfix.py, which maps by offset, cannot: our pool differs in layout)
import re,subprocess,sys,json,collections
unit=sys.argv[1]
exec(open('tools/decomp/labelcheck.py').read().split('for u in sys.argv')[0])
rs=ranges(unit)
out=subprocess.run(["build/tools/objdiff-cli","diff","-p",".","-u","main/"+unit,"-o","-","--format","json"],capture_output=True,text=True).stdout
d=json.loads(out)
def ins(side):
    r={}
    for s in d[side]['symbols']:
        r[s['name']]=[(i.get('instruction') or {}).get('formatted','') for i in s.get('instructions',[])]
    return r
L,R=ins('left'),ins('right')
pairs=collections.Counter()
for n in L:
    if n not in R: continue
    for a,b in zip(L[n],R[n]):
        ma=re.findall(r'(@\d+)@',a); mb=re.findall(r'(@\d+)@',b)
        if ma and mb and len(ma)==len(mb):
            for x,y in zip(ma,mb):
                if x!=y: pairs[(x,y)]+=1
m={}
for (x,y),c in pairs.most_common():
    if x not in m and y not in m.values(): m[x]=y
lines=open('config/SLSP01/symbols.txt').read().split('\n')
n=0
for i,l in enumerate(lines):
    mm=re.match(r'(@\d+) = (\.\w+):0x([0-9A-F]+)',l)
    if mm and mm.group(1) in m and any(s<=int(mm.group(3),16)<e for s,e in rs):
        lines[i]=m[mm.group(1)]+l[len(mm.group(1)):]; n+=1
open('config/SLSP01/symbols.txt','w').write('\n'.join(lines))
print('renamed',n,'of',len(m))
