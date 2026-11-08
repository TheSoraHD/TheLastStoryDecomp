# (s12h-20261108) # fswap.py <unit> <symbol> <src> <start> <end>: tries swapping the operands of each simple `A op B` (op: + * == !=)
# one at a time (greedy, up to 3 rounds); keeps the file only on a 0-diff match
import re,subprocess,sys,os
os.chdir('/home/claude/thelaststorydecomp')
unit,sym,src,a,b=sys.argv[1],sys.argv[2],sys.argv[3],int(sys.argv[4]),int(sys.argv[5])
orig=open(src).read()
obj='build/SLSP01/src/%s.o'%unit
def score():
    r=subprocess.run('ninja %s 2>&1 | grep -c Error'%obj,shell=True,capture_output=True,text=True)
    if r.stdout.strip()!='0': return None
    r=subprocess.run(['python3','tools/decomp/odiff2.py',unit,sym],capture_output=True,text=True)
    return len([x for x in r.stdout.split('\n') if x.strip()])
op_ch=r'(?:\(float\))?(?:[A-Za-z_]\w*(?:(?:->|\.)[A-Za-z_]\w*|\[[^\]\[]*\])*(?:\(\))?|\d+\.?\d*f?)'
pat=re.compile(r'(?<![\w.>\]\)*/%])('+op_ch+r')\s*(\+|\*|==|!=)\s*('+op_ch+r')(?![\w.\[(*/%]|->)')
cur=orig; open(src,'w').write(cur); base=score(); print('base',base,flush=True); done=False
try:
    for rnd in range(3):
        L=cur.split('\n'); best=(base,None)
        for ln in range(a-1,b):
            line=L[ln]; code=line.split('//')[0]
            for m in pat.finditer(code):
                x,op,y=m.group(1),m.group(2),m.group(3)
                if x==y: continue
                new=line[:m.start()]+y+' '+op+' '+x+line[m.end():]
                L2=list(L); L2[ln]=new
                open(src,'w').write('\n'.join(L2)); s=score()
                if s is not None and s<best[0]: best=(s,'\n'.join(L2)); print(' r%d'%rnd,ln+1,repr(m.group(0)),s,flush=True)
                if s==0: break
            if best[0]==0: break
        if best[1] is None: break
        cur=best[1]; base=best[0]
        if base==0: done=True; break
finally:
    if done: open(src,'w').write(cur); print('MATCH',flush=True)
    else: open(src,'w').write(orig); print('final',base,flush=True)
    subprocess.run('ninja %s >/dev/null 2>&1'%obj,shell=True)
