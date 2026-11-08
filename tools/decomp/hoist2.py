#!/usr/bin/env python3
# (copied from the s12h-20261107 session scratch; paths relative to the repo)
import json,re,subprocess,os,sys
R=os.path.abspath(os.path.join(os.path.dirname(__file__),'..','..'))+'/'
d=json.load(open(R+'build/SLSP01/report.json'))
SKIP=int(os.environ.get('SKIP','0')); SK=[0]
minp=float(sys.argv[1]) if len(sys.argv)>1 else 95
MAXP=float(sys.argv[2]) if len(sys.argv)>2 else 100
OPND=r'(?:\w+(?:->|\.|\[\d+\]\.?))+\w+'
PAT=re.compile(r'(?<![\w\.>])('+OPND+r') ([*+]) ('+OPND+r')(?![\w\(\[])')
for u in d['units']:
    un=u['name'][5:]
    src=R+'src/'+un+'.cpp'
    if not os.path.exists(src): continue
    s=open(src,encoding='latin-1').read()
    for f in u.get('functions',[]):
        p=f.get('fuzzy_match_percent'); fn=f['name']; sz=int(f.get('size',0))
        if p is None or not (minp<=p<MAXP) or sz>0x1000: continue
        meth=fn.split('__')[0] if not fn.startswith('__') else None
        if fn.startswith('__ct__'):
            mm=re.match(r'__ct__(?:Q\d(?:\d+\w+?)*?)?(\d+)(\w+?)F',fn); meth=mm.group(2)[:int(mm.group(1))] if mm else None
        if not meth: continue
        defs=list(re.finditer(r'^[^\n;{}()]*\b'+re.escape(meth)+r'\([^;{]*?\)\s*(?:const\s*)?(?::[^{;]*)?\{',s,re.M))
        if len(defs)!=1: continue
        st=defs[0].end(); depth=1; i=st
        while depth and i<len(s):
            if s[i]=='{': depth+=1
            elif s[i]=='}': depth-=1
            i+=1
        body=s[st:i]; lines=body.split('\n')
        for k,l in enumerate(lines):
            if not l.rstrip().endswith(';') or l.strip().startswith(('if','for','while','}','else')): continue
            for m in PAT.finditer(l):
                ind=re.match(r'\s*',l).group(0)
                for order in ((1,3),(3,1)):
                    for swap in (False,True):
                        decl=[ind+'float h%d_unk = %s;'%(j,m.group(j)) for j in order]
                        expr=('h3_unk %s h1_unk' if swap else 'h1_unk %s h3_unk')%m.group(2)
                        nl=lines[:k]+decl+[l[:m.start()]+expr+l[m.end():]]+lines[k+1:]
                        t=s[:st]+'\n'.join(nl)+s[i:]
                        tmp=os.path.dirname(src)+'/.h2.cpp'
                        open(tmp,'w',encoding='latin-1').write(t)
                        SK[0]+=1
                        if SK[0]<=SKIP: os.remove(tmp); continue
                        o=subprocess.run(['python3',R+'tools/decomp/trysym.py',un,tmp,fn],capture_output=True,text=True).stdout.strip()
                        print(un,fn,round(p,2),k,order,swap,m.group(0)[:40],(o.split()[0] if o else 'ERR'),flush=True)
                        os.remove(tmp)
