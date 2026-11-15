# symkeepat.py <old symbols>: in config/SLSP01/symbols.txt keep only renames to @N/@stringBase/@wstringBase; revert the rest by line
import sys,re
old=open(sys.argv[1]).read().splitlines(True); cur=open('config/SLSP01/symbols.txt').read().splitlines(True)
assert len(old)==len(cur)
ok=re.compile(r'^@(\d+|stringBase\d+|wstringBase\d+)$'); n=0
for i,(a,b) in enumerate(zip(old,cur)):
    if a!=b and not ok.match(b.split(' = ')[0]):
        print('revert',b.split(' = ')[0],'->',a.split(' = ')[0]); cur[i]=a; n+=1
open('config/SLSP01/symbols.txt','w').writelines(cur); print('reverted',n)
