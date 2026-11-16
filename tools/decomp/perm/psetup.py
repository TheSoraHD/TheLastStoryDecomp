#!/usr/bin/env python3
# psetup.py <unidad sin libs/ ni .c (p.ej. dwc/src/base/dwc_transport)> <funcion> <dir>
# Monta un directorio de decomp-permuter para una funcion de C: base.c (-E con los cflags de la unidad en
# build.ninja y clean2.py), compile.sh (mw_version de la unidad, sin includes), target.o y settings.toml.
# Quita 'static' a la funcion (si no, el permuter la deja sin usar y el compilador no la emite). Despues prun.sh.
# Verificar: expand.py base.c exp.c && ./compile.sh exp.c -o e.o y comparar con build/SLSP01/src/<unidad>.o.
import sys,re,os,subprocess,shutil
unit,fn,d=sys.argv[1:4]
H=os.path.dirname(os.path.abspath(__file__))
W=os.path.abspath(H+'/../../..')  # raiz del repo
nj=open(W+'/build.ninja').read()
m=re.search(r'build build/SLSP01/src/'+re.escape(unit)+r'\.o: (.*?)\n  basedir',nj,re.S)
blk=m.group(1)
mwv=re.search(r'mw_version = (\S+)',blk).group(1)
cf=re.search(r'cflags = (.*?)\n  \w',blk+'\n  x',re.S).group(1)
cf=cf.replace('$\n','').split()
src=re.search(r'(libs/\S+\.c)',blk).group(1)
# tokens: keep quoted pragmas
import shlex
args=shlex.split(' '.join(cf))
os.makedirs(d,exist_ok=True)
base=['/home/claude/tools/wibo','/home/claude/tls-build/build/compilers/Wii/1.3/mwcceppc.exe']  # -E con 1.3 (comprobado igual para unidades de 1.0); se compila con mwv
out=subprocess.run(base+args+['-E',src],cwd=W,capture_output=True)
open(d+'/base.c','wb').write(out.stdout)
subprocess.run(['python3',H+'/clean2.py',d+'/base.c',fn],check=True)
b=open(d+'/base.c').read()
b=re.sub(r'(?m)^static ([^;{(]*\b'+re.escape(fn)+r'\s*\()',r'\1',b)
open(d+'/base.c','w').write(b)
noinc=[];i=0
while i<len(args):
    if args[i] in ('-i','-I','-ir'): i+=2; continue
    noinc.append(args[i]); i+=1
open(d+'/compile.sh','w').write('#!/bin/bash\n/home/claude/tools/wibo /home/claude/tls-build/build/compilers/'+mwv+'/mwcceppc.exe '+' '.join(shlex.quote(a) for a in noinc)+' -c -o "$3" "$1" >/dev/null 2>&1\n')
os.chmod(d+'/compile.sh',0o755)
shutil.copy(W+'/build/SLSP01/obj/'+unit+'.o',d+'/target.o')
open(d+'/settings.toml','w').write('func_name = "%s"\ncompiler_type = "mwcc"\nobjdump_command = "%s/objd.sh"\n'%(fn,H))
print(src, len(out.stdout), out.stderr[:300])
