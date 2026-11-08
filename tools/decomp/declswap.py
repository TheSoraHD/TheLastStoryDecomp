#!/usr/bin/env python3
# declswap.py <maxsize hex> <minpct>: swaps adjacent independent local declarations in NM functions
import json,re,subprocess,os,sys
R=os.path.dirname(os.path.abspath(__file__))+'/../../'
d=json.load(open(R+'build/SLSP01/report.json'))
maxsz=int(sys.argv[1],16); minp=float(sys.argv[2]); maxp=float(sys.argv[3]) if len(sys.argv)>3 else 100; minsz=int(sys.argv[4],16) if len(sys.argv)>4 else 0
DECL=re.compile(r'^(\s+)(?:const\s+)?[A-Za-z_][\w:<>]*(?:\s*[\*&])*\s+(\w+)(?:\s*=\s*[^;]*|\([^;]*\))?;\s*$')
for u in d['units']:
    un=u['name'][5:]
    src=R+'src/'+un+'.cpp'
    if not os.path.exists(src): continue
    s=open(src,encoding='latin-1').read()
    for f in u.get('functions',[]):
        p=f.get('fuzzy_match_percent'); fn=f['name']; sz=int(f.get('size',0))
        if p is None or not (minp<=p<maxp) or sz>maxsz or sz<minsz: continue
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
        for k in range(len(lines)-1):
            a=DECL.match(lines[k]); b=DECL.match(lines[k+1])
            if not a or not b or a.group(1)!=b.group(1): continue
            if re.search(r'\b'+re.escape(a.group(2))+r'\b',lines[k+1].split('=',1)[-1] if '=' in lines[k+1] else lines[k+1][b.end(2):]): continue
            if lines[k].strip().startswith(('return','delete','goto')) or lines[k+1].strip().startswith(('return','delete','goto')): continue
            nl=lines[:]; nl[k],nl[k+1]=nl[k+1],nl[k]
            t=s[:st]+'\n'.join(nl)+s[i:]
            tmp=os.path.dirname(src)+'/.ds.cpp'
            open(tmp,'w',encoding='latin-1').write(t)
            o=subprocess.run(['python3',''+R+'tools/decomp/trysym.py',un,tmp,fn],capture_output=True,text=True).stdout.strip()
            print(un,fn,round(p,2),k,lines[k].strip()[:30],'<->',lines[k+1].strip()[:30],(o.split()[0] if o else 'ERR'),flush=True)
            os.remove(tmp)
