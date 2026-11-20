# ourshunks.py: resuelve los bloques en conflicto de symbols.txt con el lado HEAD (ours), conservando lo que git mezclo solo
p='config/SLSP01/symbols.txt'; L=open(p).read().split('\n'); out=[]; st=0; n=0
for l in L:
    if l.startswith('<<<<<<<'): st=1; n+=1; continue
    if st==1 and l=='=======': st=2; continue
    if st==2 and l.startswith('>>>>>>>'): st=0; continue
    if st==2: continue
    out.append(l)
open(p,'w').write('\n'.join(out)); print('hunks ours:',n)
