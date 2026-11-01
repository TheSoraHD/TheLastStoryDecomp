# variants_try.py <unit> <symbol> <file> <variants.txt>: try source variants of one function.
# variants.txt: blocks "OLD\n-----\nNEW" separated by lines "=====". Each block is applied alone to
# <file>, the unit is rebuilt and the objdiff percent of <symbol> printed; the file is restored at the end.
# var.py unit symbol file variants.txt : variants separated by lines "=====" ; each variant is "OLD\n-----\nNEW"
import sys,subprocess,os
u,sym,f,vf=sys.argv[1:5]
orig=open(f,encoding='latin-1').read()
vs=open(vf,encoding='latin-1').read().split('\n=====\n')
try:
  for i,v in enumerate(vs):
    old,new=v.split('\n-----\n')
    old=old.strip('\n'); new=new.strip('\n')
    if old not in orig: print(i,'OLD NOT FOUND'); continue
    open(f,'w',encoding='latin-1').write(orig.replace(old,new,1))
    r=subprocess.run(['ninja','build/SLSP01/src/%s.o'%u],capture_output=True,text=True)
    if r.returncode: print(i,'BUILD FAIL'); continue
    p=subprocess.run(['tools/decomp/pct.sh',u,sym],capture_output=True,text=True).stdout.strip()
    print(i,p.split()[0] if p else '?', new.split('\n')[0][:90])
finally:
  open(f,'w',encoding='latin-1').write(orig)
  subprocess.run(['ninja','build/SLSP01/src/%s.o'%u],capture_output=True)
