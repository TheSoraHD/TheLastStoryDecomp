# symreapply.py <branch>: re-apply the non-@N renames that <branch> made since the merge base with HEAD^1
import re,subprocess,sys
br=sys.argv[1]
b=subprocess.check_output(['git','merge-base','HEAD^1',br],text=True).strip()
def load(rev):
    d={}
    for l in subprocess.check_output(['git','show',rev+':config/SLSP01/symbols.txt'],text=True).splitlines(True):
        m=re.search(r'= (\S+:0x[0-9A-Fa-f]+);',l)
        if m: d.setdefault(m.group(1),[]).append(l)
    return d
B,G=load(b),load(br)
cur=open('config/SLSP01/symbols.txt').read().splitlines(True); idx={}
for i,l in enumerate(cur):
    m=re.search(r'= (\S+:0x[0-9A-Fa-f]+);',l)
    if m: idx.setdefault(m.group(1),[]).append(i)
n=0
for a in G:
    if B.get(a)!=G[a] and len(G[a])==1 and len(B.get(a,[]))==1 and not re.match(r'^@\d+ ',G[a][0]) and len(idx.get(a,[]))==1:
        i=idx[a][0]
        if cur[i]!=G[a][0]: print(cur[i].split(' = ')[0][:50],'->',G[a][0].split(' = ')[0][:70]); cur[i]=G[a][0]; n+=1
open('config/SLSP01/symbols.txt','w').writelines(cur); print('reapplied',n)
