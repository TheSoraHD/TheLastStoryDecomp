# refmap2.py <unidades>: re-empareja los @N de symbols.txt por direccion con un build de referencia en $MAINREF (su symbols.txt y sus .o); sirve aunque symbols.txt tenga otro numero de lineas (datos-20261119, cola G6)
import os,re,subprocess,sys
R=os.getcwd()+'/'; M=os.environ.get('MAINREF','/home/claude/mainref').rstrip('/')+'/'
OBJDUMP='build/binutils/powerpc-eabi-objdump'
sp=open(R+'config/SLSP01/splits.txt').read()
def rng(u):
    m=re.search(r'^'+re.escape(u)+r'\.(?:cpp|c):\n((?:\t.*\n)+)',sp,re.M); r=[]
    for l in m.group(1).split('\n'):
        mm=re.match(r'\t(\.\w+)\s+start:0x([0-9A-F]+) end:0x([0-9A-F]+)',l)
        if mm and mm.group(1)!='.text': r.append((mm.group(1),int(mm.group(2),16),int(mm.group(3),16)))
    return r
def labels(o):
    out=subprocess.run([OBJDUMP,'-t',o],capture_output=True,text=True).stdout
    byname={}; bypos={}
    for l in out.split('\n'):
        t=l.split()
        if len(t)>=5 and t[-1].startswith('@'):
            sec=t[-3]; off=int(t[0],16); byname.setdefault(t[-1],(sec,off)); bypos.setdefault((sec,off),t[-1])
    return byname,bypos
maddr={}
for l in open(M+'config/SLSP01/symbols.txt'):
    m=re.match(r'(@\d+) = (\.\w+):0x([0-9A-F]+);',l)
    if m: maddr[(m.group(2),int(m.group(3),16))]=m.group(1)
cur=open(R+'config/SLSP01/symbols.txt').read().split('\n')
n=0
for u in sys.argv[1:]:
    rs=rng(u)
    mo=M+'build/SLSP01/src/%s.o'%u
    if not os.path.exists(mo): print('no mainref .o',u); continue
    mb,_=labels(mo); _,nb=labels(R+'build/SLSP01/src/%s.o'%u)
    for j,c in enumerate(cur):
        mm=re.match(r'(@\d+) = (\.\w+):0x([0-9A-F]+);',c)
        if not mm: continue
        sec=mm.group(2); a=int(mm.group(3),16)
        if not any(s==sec and lo<=a<hi for s,lo,hi in rs): continue
        mname=maddr.get((sec,a))
        if not mname or mname not in mb: continue
        new=nb.get(mb[mname])
        if new and new!=mm.group(1):
            cur[j]=new+c[len(mm.group(1)):]; n+=1
open(R+'config/SLSP01/symbols.txt','w').write('\n'.join(cur)); print('renamed',n)
