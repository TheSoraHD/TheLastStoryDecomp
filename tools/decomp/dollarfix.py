# dollarfix.py <fichero con unidades>: tras refmap2.py, renombra en symbols.txt los nombres con $N (__arraydtor$N, @enum$N...) por posicion frente al build de $MAINREF (mixto2-20261120, cola M2)
import os,re,subprocess,sys
M=os.environ.get('MAINREF','/home/claude/mainref').rstrip('/')
O='build/binutils/powerpc-eabi-objdump'
def syms(f):
    out=subprocess.run([O,'-t',f],capture_output=True,text=True).stdout
    r={}
    for l in out.split('\n'):
        t=l.split()
        if len(t)>=5 and re.search(r'\$\d+',t[-1]):
            r.setdefault((t[-3],int(t[0],16),re.sub(r'\$\d+','$N',t[-1])),t[-1])
    return r
p='config/SLSP01/symbols.txt'; s=open(p).read(); n=0
for u in open(sys.argv[1]).read().split():
    a=syms(M+'/build/SLSP01/src/%s.o'%u); b=syms('build/SLSP01/src/%s.o'%u)
    if len(a)!=len(b): print('count differs',u,len(a),len(b))
    for k,old in a.items():
        new=b.get(k)
        if new and new!=old and old in s:
            c=s.count(old); s=s.replace(old,new); n+=c; print(u,old[:60],'->',new[:60],c)
open(p,'w').write(s); print('renamed',n)
