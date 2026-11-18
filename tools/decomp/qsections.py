#!/usr/bin/env python3
# qsections.py <prefijo> <seccion>: une la seccion "## <seccion>" de las notas docs/<prefijo>-q*.md
# (una linea por punto, con la cola al final si no la lleva). Ej.: qsections.py enlazar2-20261117 Hallazgos
import glob,re,sys
pre,sec=sys.argv[1],sys.argv[2]
for f in sorted(glob.glob('docs/%s-q*.md'%pre)):
    q=re.search(r'-q(\w+)\.md$',f).group(1)
    t=open(f,encoding='utf-8',errors='replace').read()
    m=re.search(r'^## %s[^\n]*\n(.*?)(?=^## |\Z)'%re.escape(sec),t,re.M|re.S)
    if not m: continue
    item=None
    for l in m.group(1).split('\n')+['- ']:
        if l.startswith('- '):
            if item and item.strip('- ').strip():
                s=' '.join(item.split())
                if '(%s q'%pre not in s: s+=' (%s q%s)'%(pre,q)
                print(s)
            item=l
        elif item is not None and l.strip(): item+=' '+l.strip()
