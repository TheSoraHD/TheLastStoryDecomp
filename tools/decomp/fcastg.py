# (s12h-20261108) # fcastg.py <unit> <symbol> <src> <start> <end>: greedy (float) insertion (up to 3 rounds); keeps the file
# changed only when the function reaches 0 diff lines
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
chain=re.compile(r'(?<![\w.>\)\]])([A-Za-z_]\w*(?:(?:->|\.)[A-Za-z_]\w*|\[[^\]\[]*\])*)(?![\w\[]|->|\.|\s*\()')
skip={'float','int','return','if','else','for','while','const','unsigned','char','short','void','static','inline','sizeof','true','false','new','delete','this','case','switch','break','double','long','bool'}
cur=orig
open(src,'w').write(cur)
base=score(); print('base',base,flush=True)
done=False
try:
    for rnd in range(3):
        L=cur.split('\n'); best=(base,None)
        for ln in range(a-1,b):
            line=L[ln]; code=line.split('//')[0]
            for m in chain.finditer(code):
                tok=m.group(1)
                if tok.split('.')[0].split('->')[0].split('[')[0] in skip: continue
                pre=code[:m.start()].rstrip()
                if not pre or pre[-1] not in '*+-/(<>=,?:' or pre.endswith('::') or pre.endswith('(float)'): continue
                if pre.endswith(')') : continue
                L2=list(L); L2[ln]=line[:m.start()]+'(float)'+line[m.start():]
                open(src,'w').write('\n'.join(L2)); s=score()
                if s is not None and s<best[0]: best=(s,'\n'.join(L2)); print(' r%d'%rnd,ln+1,repr(tok),s,flush=True)
                if s==0: break
            if best[0]==0: break
        if best[1] is None: break
        cur=best[1]; base=best[0]
        if base==0: done=True; break
finally:
    if done:
        open(src,'w').write(cur); print('MATCH',flush=True)
    else:
        open(src,'w').write(orig); print('final',base,flush=True)
    subprocess.run('ninja %s >/dev/null 2>&1'%obj,shell=True)
