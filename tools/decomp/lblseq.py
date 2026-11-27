# lblseq.py <unit> [--apply]: align .sdata2 labels of the original asm and our .o by value sequence (difflib)
import re,subprocess,sys,difflib,struct
u=sys.argv[1]; apply='--apply' in sys.argv
asm=open('build/SLSP01/asm/%s.s'%u).read()
sec=asm[asm.index('.section .sdata2'):]
sec=sec[:sec.find('.section',10)] if sec.find('.section',10)>0 else sec
orig=[]
for m in re.finditer(r'# \.sdata2:0x[0-9A-F]+ \| 0x([0-9A-F]+) \| size: 0x(\w+)\n\.obj "?([^",]+)"?, \w+\n\t\.(float|double|4byte|8byte) ([^\n]+)',sec):
    orig.append((int(m.group(1),16),m.group(3),m.group(4),m.group(5)))
o=subprocess.run(['/home/claude/binutils/powerpc-eabi-objdump','-t','build/SLSP01/src/%s.o'%u],capture_output=True,text=True).stdout
d=subprocess.run(['/home/claude/binutils/powerpc-eabi-objcopy','-O','binary','-j','.sdata2','build/SLSP01/src/%s.o'%u,'/dev/stdout'],capture_output=True).stdout
ours=[]
for l in o.split('\n'):
    p=l.split()
    if len(p)>=5 and p[-3]=='.sdata2' and p[-1].startswith('@'):
        off=int(p[0],16); sz=int(p[-2],16)
        raw=d[off:off+sz]
        ours.append((off,p[-1],raw))
ours.sort()
def oval(t,v):
    if t=='float': return struct.pack('>f',float(v))
    if t=='double': return struct.pack('>d',float(v))
    if t=='4byte':
        try: return struct.pack('>I',int(v,16))
        except ValueError: return ('rel',v)
    return None
A=[oval(t,v) for a,n,t,v in orig]; B=[r for _,_,r in ours]
sm=difflib.SequenceMatcher(None,A,B,autojunk=False)
ren={}
for tag,i1,i2,j1,j2 in sm.get_opcodes():
    if tag=='equal':
        for k in range(i2-i1):
            a,n,_,_=orig[i1+k]; on=ours[j1+k][1]
            if n!=on: ren[a]=(n,on)
print(len(ren),'renames', [(hex(a),x) for a,x in list(ren.items())[:50]])
if apply:
    L=open('config/SLSP01/symbols.txt').readlines()
    for i,l in enumerate(L):
        mm=re.match(r'(@\d+) = \.sdata2:0x([0-9A-F]+);',l)
        if mm and int(mm.group(2),16) in ren:
            L[i]=l.replace(mm.group(1)+' =',ren[int(mm.group(2),16)][1]+' =',1)
    open('config/SLSP01/symbols.txt','w').writelines(L)
