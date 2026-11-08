#!/usr/bin/env python3
# opidx.py <maxsize hex> [minpct]: in NM functions, replace X.mData[i] / X->mData[i] by X[i] / (*X)[i] (each occurrence and all)
import json,re,subprocess,os,sys
R=os.path.dirname(os.path.abspath(__file__))+'/../../'
d=json.load(open(R+'build/SLSP01/report.json'))
maxsz=int(sys.argv[1],16); minp=float(sys.argv[2]) if len(sys.argv)>2 else 80
for u in d['units']:
    un=u['name'][5:]
    src=R+'src/'+un+'.cpp'
    if not os.path.exists(src): continue
    s=open(src,encoding='latin-1').read()
    for f in u.get('functions',[]):
        p=f.get('fuzzy_match_percent'); fn=f['name']; sz=int(f.get('size',0))
        if p is None or not (minp<=p<100) or sz>maxsz: continue
        meth=fn.split('__')[0] if not fn.startswith('__') else None
        if fn.startswith('__ct__'):
            mm=re.match(r'__ct__(?:Q\d(?:\d+\w+?)*?)?(\d+)(\w+?)F',fn)
            meth=mm.group(2)[:int(mm.group(1))] if mm else None
        if not meth: continue
        defs=list(re.finditer(r'^[^\n;{}()]*\b'+re.escape(meth)+r'\([^;{]*?\)\s*(?:const\s*)?(?::[^{;]*)?\{',s,re.M))
        if len(defs)!=1: continue
        st=defs[0].end(); depth=1; i=st
        while depth and i<len(s):
            if s[i]=='{': depth+=1
            elif s[i]=='}': depth-=1
            i+=1
        body=s[st:i]
        occ=list(re.finditer(r'(\b[\w\.]+)\.mData\[',body))
        if not occ: continue
        variants=[]
        for o in occ: variants.append([o])
        if len(occ)>1: variants.append(occ)
        for vs in variants:
            b=body
            for o in sorted(vs,key=lambda x:-x.start()):
                b=b[:o.start()]+o.group(1)+'['+b[o.end():]
            t=s[:st]+b+s[i:]
            tmp=os.path.dirname(src)+'/.oi.cpp'
            open(tmp,'w',encoding='latin-1').write(t)
            o2=subprocess.run(['python3',''+R+'tools/decomp/trysym.py',un,tmp,fn],capture_output=True,text=True).stdout.strip()
            print(un,fn,round(p,2),'all' if len(vs)>1 else vs[0].group(0),(o2.split()[0] if o2 else 'ERR'),flush=True)
            os.remove(tmp)
