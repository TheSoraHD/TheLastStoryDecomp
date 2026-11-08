#!/usr/bin/env python3
# hoist.py: in NM functions, hoist `EXPR` (env HEXPR regex, type env HTYPE) of a statement into a local declared just before
import json,re,subprocess,os,sys
R=os.path.abspath(os.path.join(os.path.dirname(__file__),'..','..'))+'/'
d=json.load(open(R+'build/SLSP01/report.json'))
HEXPR=os.environ.get('HEXPR',r'lbl_8087EFA8->m3A4'); HTYPE=os.environ.get('HTYPE','float')
SKIP=int(os.environ.get('SKIP','0')); SK=[0]
minp=float(sys.argv[1]) if len(sys.argv)>1 else 90
MAXP=float(sys.argv[2]) if len(sys.argv)>2 else 100
for u in d['units']:
    un=u['name'][5:]
    src=R+'src/'+un+'.cpp'
    if not os.path.exists(src): continue
    s=open(src,encoding='latin-1').read()
    if not re.search(HEXPR,s): continue
    for f in u.get('functions',[]):
        p=f.get('fuzzy_match_percent'); fn=f['name']; sz=int(f.get('size',0))
        if p is None or not (minp<=p<MAXP) or sz>0x2000: continue
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
            m=re.search(HEXPR,l)
            if not m or not l.rstrip().endswith(';') or l.strip().startswith(('if','for','while','return','}','else')): 
                if not m or not (l.strip().startswith('return') and l.rstrip().endswith(';')): continue
            ind=re.match(r'\s*',l).group(0)
            nl=lines[:k]+[ind+HTYPE+' hoist_unk = '+m.group(0)+';', l[:m.start()]+'hoist_unk'+l[m.end():]]+lines[k+1:]
            t=s[:st]+'\n'.join(nl)+s[i:]
            tmp=os.path.dirname(src)+'/.ho'+os.environ.get('TAG','')+'.cpp'
            open(tmp,'w',encoding='latin-1').write(t)
            SK[0]+=1
            if SK[0]<=SKIP: os.remove(tmp); continue
            o=subprocess.run(['python3',R+'tools/decomp/trysym.py',un,tmp,fn],capture_output=True,text=True).stdout.strip()
            print(un,fn,round(p,2),k,l.strip()[:50],(o.split()[0] if o else 'ERR'),flush=True)
            os.remove(tmp)
