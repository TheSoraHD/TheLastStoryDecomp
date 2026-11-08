#!/usr/bin/env python3
# usage: [FREE=1] paramtry.py <maxsize hex> [minsize hex]: toggles `const` on one pointer/reference parameter
# of each non-matching function at a time and reports OK when the bytes match (FREE=1: C functions fn_XXXXXXXX).
# paramtry.py <maxsize>: for NM methods/functions, toggle const on one pointer/reference parameter at a time
import json,re,subprocess,os,glob,sys
import os
R=os.path.dirname(os.path.abspath(__file__))+'/../../'
d=json.load(open(R+'build/SLSP01/report.json'))
maxsz=int(sys.argv[1],16); minsz=int(sys.argv[2],16) if len(sys.argv)>2 else 0
cands=[]
for u in d['units']:
    un=u['name'][5:]
    if not re.match(r'(game|Event|atn|gmk|chr|MP|db)/',un): continue
    for f in u.get('functions',[]):
        p=f.get('fuzzy_match_percent'); n=f['name']; sz=int(f.get('size',0))
        if p is None or not (40<=p<100) or sz>maxsz or sz<=minsz: continue
        if '__' not in n and re.match(r'^fn_[0-9A-F]+$',n) and os.environ.get('FREE'):
            cands.append((un,n,n,p)); continue
        if os.environ.get('FREE'): continue
        if os.environ.get('CTOR'):
            mc=re.match(r'^__ct__(?:Q\d(?:\d+\w+?)*?)?(\d+)(\w+?)F',n)
            if mc:
                k=int(mc.group(1)); rest=n[mc.start(2):]; cls=rest[:k]
                cands.append((un,n,cls,p))
            continue
        m=re.match(r'^(\w+?)__',n)
        if not m or n.startswith('__') or '__' not in n: continue
        cands.append((un,n,m.group(1),p))
hdrs=glob.glob(R+'include/**/*.h',recursive=True)
def nm_syms(obj,meth):
    out=subprocess.run([R+'build/binutils/powerpc-eabi-nm',obj],capture_output=True,text=True).stdout
    return [l.split()[-1] for l in out.split('\n') if l.split() and (l.split()[-1].startswith(meth+'__') or ('__ct__' in l and str(len(meth))+meth+'F' in l))]
for un,n,meth,p in cands:
    src=R+'src/'+un+'.cpp'
    if not os.path.exists(src): continue
    s=open(src,encoding='latin-1').read()
    defs=list(re.finditer(r'(\w+::)?'+re.escape(meth)+r'\(([^)]*)\)(\s*const)?\s*[{:]',s))
    if len(defs)!=1: continue
    dm=defs[0]; params=dm.group(2)
    if not params.strip(): continue
    plist=[x.strip() for x in params.split(',')]
    # find decl
    hdr=None;hm=None
    for h in hdrs+[src]:
        t=open(h,encoding='latin-1').read()
        ms=[mm for mm in re.finditer(r'\b'+re.escape(meth)+r'\(([^)]*)\)(\s*const)?\s*;',t)]
        if len(ms)==1: hdr=h;hm=ms[0];break
    obj=R+'build/SLSP01/src/'+un+'.o'
    before=set(nm_syms(obj,meth))
    for i,pp in enumerate(plist):
        if not re.search(r'[*&]',pp): continue
        if pp.startswith('const '): np_=pp[6:]
        elif re.match(r'(atn::|chr::|std::)?[A-Z]\w*[*&]|(atn::)?\w+_unk[*&]|(atn::)?\w+[ ]*[*&] *\w+',pp) and not re.match(r'(int|float|char|void|unsigned|short|wchar_t|bool)\b',pp): np_='const '+pp
        else: continue
        nl=list(plist); nl[i]=np_
        ns=s[:dm.start(2)]+', '.join(nl)+s[dm.end(2):]
        open(src,'w',encoding='latin-1').write(ns)
        if hdr and hdr!=src:
            ht=open(hdr,encoding='latin-1').read()
            dp=[x.strip() for x in hm.group(1).split(',')]
            if len(dp)==len(plist):
                if dp[i].startswith('const '): dp[i]=dp[i][6:]
                else: dp[i]='const '+dp[i]
                open(hdr,'w',encoding='latin-1').write(ht[:hm.start(1)]+', '.join(dp)+ht[hm.end(1):])
        r=subprocess.run(['ninja','build/SLSP01/src/'+un+'.o'],cwd=R,capture_output=True,text=True)
        res='ERR'
        if r.returncode==0:
            after=[x for x in nm_syms(obj,meth) if x not in before]
            if '__' not in n: after=[n]
            if len(after)==1:
                res=subprocess.run(['python3',''+R+'tools/decomp/cmpsym.py',un,after[0]+'='+n],cwd=R,capture_output=True,text=True).stdout.split()[0]
            else: res='NOSYM'
        print('%-28s %-55s p%d %6.2f -> %s'%(un,n[:55],i,p,res),flush=True)
        open(src,'w',encoding='latin-1').write(s)
        if hdr and hdr!=src: open(hdr,'w',encoding='latin-1').write(ht)
subprocess.run(['ninja'],cwd=R,capture_output=True)
