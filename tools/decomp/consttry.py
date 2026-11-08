#!/usr/bin/env python3
# usage: consttry.py <maxsize hex> [minsize hex]: tries `const` on each non-matching method (definition and
# declaration), compiles the unit and compares the bytes with cmpsym.py; prints OK when it matches. Restores the files.
# consttry.py: for small NM non-const methods, try making them const (definition + declaration); report gains
import json,re,subprocess,os,glob,sys
import os
R=os.path.dirname(os.path.abspath(__file__))+'/../../'
d=json.load(open(R+'build/SLSP01/report.json'))
cands=[]
for u in d['units']:
    un=u['name'][5:]
    if not re.match(r'(game|Event|atn|gmk|chr|MP|db)/',un): continue
    for f in u.get('functions',[]):
        p=f.get('fuzzy_match_percent'); n=f['name']; sz=int(f.get('size',0))
        lo=int(sys.argv[2],16) if len(sys.argv)>2 else 0
        if p is None or not (40<=p<100) or sz>int(sys.argv[1] if len(sys.argv)>1 else '0x100',16) or sz<=lo: continue
        m=re.match(r'^(\w+?)__((?:Q\d(?:\d+\w+?)+)|(?:\d+\w+?))F(.*)$',n)
        if not m or n.startswith('__'): continue
        cands.append((un,n,m.group(1),p))
def pct(un,sym):
    out=subprocess.run([R+'tools/decomp/pct.sh',un,sym],cwd=R,capture_output=True,text=True).stdout.split()
    return float(out[0]) if out else None
for un,n,meth,p in cands:
    src=R+'src/'+un+'.cpp'
    if not os.path.exists(src): continue
    s=open(src,encoding='latin-1').read()
    defs=[mm for mm in re.finditer(r'\b(\w+)::'+re.escape(meth)+r'\(([^)]*)\)\s*\{',s)]
    if len(defs)!=1: continue
    dm=defs[0]; cls=dm.group(1)
    # header decl
    hdr=None
    for h in glob.glob(R+'include/**/*.h',recursive=True)+[src]:
        t=open(h,encoding='latin-1').read()
        if re.search(r'class '+cls+r'\b|struct '+cls+r'\b',t) and re.search(r'\b'+re.escape(meth)+r'\([^)]*\)\s*;',t):
            hdr=h;break
    if not hdr: continue
    ht=open(hdr,encoding='latin-1').read()
    hm=list(re.finditer(r'\b'+re.escape(meth)+r'\(([^)]*)\)\s*;',ht))
    if len(hm)!=1: continue
    ns=s[:dm.end()-1]+'const '+s[dm.end()-1:] if not s[dm.start():dm.end()].rstrip('{').rstrip().endswith('const') else None
    if ns is None: continue
    hh=hm[0]; nh=ht[:hh.end()-1]+' const'+ht[hh.end()-1:]
    open(src,'w',encoding='latin-1').write(ns)
    if hdr!=src: open(hdr,'w',encoding='latin-1').write(nh)
    r=subprocess.run(['ninja','build/SLSP01/src/'+un+'.o'],cwd=R,capture_output=True,text=True)
    newsym=n.replace('__'+n.split('__',1)[1].split('F',1)[0]+'F','__'+n.split('__',1)[1].split('F',1)[0]+'CF',1)
    q='ERR' if r.returncode else subprocess.run(['python3',''+R+'tools/decomp/cmpsym.py',un,newsym+'='+n],cwd=R,capture_output=True,text=True).stdout.split()[0]
    print('%-30s %-60s %6.2f -> %s'%(un,n[:60],p,q),flush=True)
    open(src,'w',encoding='latin-1').write(s)
    if hdr!=src: open(hdr,'w',encoding='latin-1').write(ht)
subprocess.run(['ninja'],cwd=R,capture_output=True)
