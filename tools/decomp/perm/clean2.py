# clean2.py base.c <fn>: como clean.py, pero las funciones con asm se envuelven en
# PERM_PRETEND(prototipo;) PERM_IGNORE(cuerpo) para que se sigan compilando (inlines asm).
import re,sys
t=open(sys.argv[1]).read()
# pycparser escribe __typeof__ como typeof, que el MWCC 1.0 no acepta
t=re.sub(r'__typeof__\(sizeof\(0\)\)','unsigned long',t)
# identificadores con PERM_ dentro (NAND_PERM_RUSR) confunden al permuter
t=re.sub(r'(\w)PERM_',r'\1PRM_',t)
t=re.sub(r'__declspec\(noinline\)','PERM_IGNORE(__declspec(noinline))',t)
t=re.sub(r'__declspec\((?!noinline)[^)]*\)','',t)
res=[];pos=0
pat=re.compile(r'(?m)^[^\n;{}#]*\([^;{}]*\)\s*\{')
while True:
    m=pat.search(t,pos)
    if not m: res.append(t[pos:]);break
    d=0;j=m.end()-1
    while True:
        if t[j]=='{':d+=1
        elif t[j]=='}':
            d-=1
            if d==0:break
        j+=1
    body=t[m.start():j+1]
    res.append(t[pos:m.start()])
    if re.search(r'\basm\b',body) and sys.argv[2] not in body[:200]:
        proto=body[:body.index('{')].rstrip()
        res.append('PERM_PRETEND('+proto+';)\nPERM_IGNORE('+body+')')
    else: res.append(body)
    pos=j+1
t=''.join(res)
# pycparser lee 'sizeof(T) + sizeof(T)' como sizeof de un cast: se anulan, quitarlos
t=re.sub(r'- *sizeof\((\w+)\) *\+ *sizeof\(\1\)','',t)
open(sys.argv[1],'w').write(t)
