#!/usr/bin/env python3
# front.py minp: replace the manual front-direction block by an inline helper
import json,re,subprocess,os,sys
R=os.path.abspath(os.path.join(os.path.dirname(__file__),'..','..'))+'/'
d=json.load(open(R+'build/SLSP01/report.json'))
minp=float(sys.argv[1])
HS=['inline atn::Vector3 FrontTmp_unk(float ry, float z) {\n    atn::Vector3 v(0.0f, 0.0f, z);\n    Mtx r;\n    PSMTXRotRad(r, \'y\', ry);\n    PSMTXMultVec(r, &v, &v);\n    return v;\n}\n\n']
PAT=re.compile(r"^(\s*)atn::Vector3 (\w+)\(0\.0f, 0\.0f, ([\d\.]+f)\);\n\s*(?:Mtx|atn::Matrix3x4) (\w+);\n\s*PSMTXRotRad\(\4(?:\.m)?, 'y', ([^;]+?)\);\n\s*PSMTXMultVec\(\4(?:\.m)?, &\2, &\2\);\n",re.M)
for u in d['units']:
    un=u['name'][5:]
    src=R+'src/'+un+'.cpp'
    if not os.path.exists(src): continue
    s=open(src,encoding='latin-1').read()
    if 'PSMTXRotRad' not in s: continue
    for f in u.get('functions',[]):
        p=f.get('fuzzy_match_percent'); fn=f['name']; sz=int(f.get('size',0))
        if p is None or not (minp<=p<100) or sz>0x1400: continue
        meth=fn.split('__')[0] if not fn.startswith('__') else None
        if not meth: continue
        defs=list(re.finditer(r'^[^\n;{}()]*\b'+re.escape(meth)+r'\([^;{]*?\)\s*(?:const\s*)?(?::[^{;]*)?\{',s,re.M))
        if len(defs)!=1: continue
        st=defs[0].end(); depth=1; i=st
        while depth and i<len(s):
            if s[i]=='{': depth+=1
            elif s[i]=='}': depth-=1
            i+=1
        body=s[st:i]
        ls=s.rfind('\n',0,defs[0].start())+1
        while True:
            pl=s.rfind('\n',0,ls-1)+1
            if s[pl:ls].lstrip().startswith(('//','template')): ls=pl
            else: break
        for m in PAT.finditer(body):
            ind,x,z,mt,ry=m.groups()
            rest=body[m.end():]
            cands=[body[:m.start()]+ind+'atn::Vector3 '+x+' = FrontTmp_unk('+ry+', '+z+');\n'+rest]
            if len(re.findall(r'\b'+x+r'\b',rest))==1:
                r2=re.sub(r'\b'+x+r'\b','FrontTmp_unk('+ry+', '+z+')',rest,1)
                cands.append(body[:m.start()]+r2)
                m2=re.search(r'^\s*atn::Vector3 (\w+) = ([^;\n]*FrontTmp_unk[^;\n]*);\n',r2,re.M)
                if m2 and len(re.findall(r'\b'+m2.group(1)+r'\b',r2[m2.end():]))==1:
                    for e in (m2.group(2),'('+m2.group(2)+')'):
                        cands.append(body[:m.start()]+r2[:m2.start()]+re.sub(r'\b'+m2.group(1)+r'\b',e.replace('\\','\\\\'),r2[m2.end():],1))
            for H in HS:
              for nb in cands:
                t=s[:ls]+H+s[ls:st]+nb+s[i:]
                tmp=os.path.dirname(src)+'/.front.cpp'
                open(tmp,'w',encoding='latin-1').write(t)
                o=subprocess.run(['python3',R+'tools/decomp/trysym.py',un,tmp,fn],capture_output=True,text=True,env=dict(os.environ,V='1')).stdout.strip().split('\n')
                n=len(o)-1 if o[0].startswith('DIFF') else (0 if o[0].startswith('OK') else 999)
                o0=subprocess.run(['python3',R+'tools/decomp/trysym.py',un,src,fn],capture_output=True,text=True,env=dict(os.environ,V='1')).stdout.strip().split('\n')
                n0=len(o0)-1 if o0[0].startswith('DIFF') else (0 if o0[0].startswith('OK') else 999)
                print(un,fn[:60],round(p,2),x,n0,'->',n,'OK' if n==0 else '',flush=True)
                os.remove(tmp)
