#!/usr/bin/env python3
# fvar.py <unit> <symbol> <variants-file> --after <marker>: as var.py, but each old text is replaced at its first
# occurrence after <marker> (e.g. the definition line of the function), so texts repeated in other functions work.
# var.py <unit> <symbol> <variants-file>: run from the root of your copy.
# variants-file: blocks "old\n===\nnew\n%%%" ; each block is ONE variant (may contain several old/new pairs
# separated by lines "+++"). Every variant starts from src/<unit>.cpp.bak (created from the .cpp if missing),
# builds only the unit (ninja -j1) and prints the match percent of <symbol>. The .cpp is restored from .bak at the end
# unless --keep N (keep variant N, 1-based).
import sys,subprocess,os,shutil
u,sym,vf=sys.argv[1:4]; keep=int(sys.argv[sys.argv.index('--keep')+1]) if '--keep' in sys.argv else 0
src=sys.argv[sys.argv.index('--file')+1] if '--file' in sys.argv else 'src/%s.cpp'%u; bak=src+'.bak'
mark=sys.argv[sys.argv.index('--after')+1].replace('\\n','\n')
def rep(t,old,new):
    k=t.find(mark)
    if k<0: return None
    j=t.find(old,k)
    if j<0: return None
    return t[:j]+new+t[j+len(old):]
if not os.path.exists(bak): shutil.copy(src,bak)
base=open(bak,encoding='latin-1').read()
blocks=[b.strip('\n') for b in open(vf,encoding='latin-1').read().split('%%%') if b.strip()]
def pct():
    r=subprocess.run(['build/tools/objdiff-cli','diff','-p','.','-u','main/'+u,'-o','-','--format','json-pretty'],capture_output=True,text=True)
    import json
    try: d=json.loads(r.stdout)
    except Exception: return None
    for s in d.get('left',{}).get('symbols',[]):
        if s.get('name')==sym: return s.get('match_percent')
def build(text):
    open(src,'w',encoding='latin-1').write(text)
    r=subprocess.run(['ninja','-j1','build/SLSP01/src/%s.o'%u],capture_output=True,text=True)
    return r.returncode==0, r.stdout[-400:]
ok,_=build(base); print('base', pct() if ok else 'BUILD FAIL')
for i,b in enumerate(blocks,1):
    t=base; bad=None
    for pair in b.split('\n+++\n'):
        old,new=pair.split('\n===\n')
        t2=rep(t,old,new)
        if t2 is None: bad='old not found'; break
        t=t2
    if bad: print(i,bad); continue
    ok,out=build(t); print(i, pct() if ok else 'BUILD FAIL '+out.strip().split('\n')[-1][:150])
final=base
if keep:
    t=base
    for pair in blocks[keep-1].split('\n+++\n'):
        old,new=pair.split('\n===\n'); t=rep(t,old,new)
    final=t
build(final); os.remove(bak)
print('kept',keep if keep else 'base')
