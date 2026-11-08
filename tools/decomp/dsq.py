#!/usr/bin/env python3
# dsq.py minp: replace `atn::Vector3 X = A - B; ... PSVECSquareMag(&X)` (X used once) by a static inline DistSq helper
import json,re,subprocess,os,sys
R=os.path.abspath(os.path.join(os.path.dirname(__file__),'..','..'))+'/'
d=json.load(open(R+'build/SLSP01/report.json'))
minp=float(sys.argv[1])
H='static inline float DsqTmp_unk(const atn::Vector3& a, const atn::Vector3& b) {\n    atn::Vector3 d = a - b;\n    return PSVECSquareMag(&d);\n}\n\n'
DECL=re.compile(r'^(\s*)atn::Vector3 (\w+)(?: = |\()([^;]*?) - ([^;]*?)\)?;\n',re.M)
for u in d['units']:
    un=u['name'][5:]
    src=R+'src/'+un+'.cpp'
    if not os.path.exists(src): continue
    s=open(src,encoding='latin-1').read()
    if 'PSVECSquareMag' not in s: continue
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
        # line start of the definition (include preceding comment lines? insert before the def line)
        ls=s.rfind('\n',0,defs[0].start())+1
        while True:
            pl=s.rfind('\n',0,ls-1)+1
            if s[pl:ls].lstrip().startswith('//') or s[pl:ls].lstrip().startswith('template'): ls=pl
            else: break
        for m in DECL.finditer(body):
            x=m.group(2); a=m.group(3).strip(); b=m.group(4).strip()
            if a.count('(')!=a.count(')') or b.count('(')!=b.count(')'): continue
            rest=body[m.end():]
            uses=re.findall(r'\b'+x+r'\b',rest)
            if len(uses)!=1 or ('PSVECSquareMag(&'+x+')') not in rest: continue
            nb=body[:m.start()]+rest.replace('PSVECSquareMag(&'+x+')','DsqTmp_unk('+a+', '+b+')',1)
            t=s[:ls]+H+s[ls:st]+nb+s[i:]
            tmp=os.path.dirname(src)+'/.dsq.cpp'
            open(tmp,'w',encoding='latin-1').write(t)
            o=subprocess.run(['python3',R+'tools/decomp/trysym.py',un,tmp,fn],capture_output=True,text=True,env=dict(os.environ,V='1')).stdout.strip().split('\n')
            n=len(o)-1 if o[0].startswith('DIFF') else (0 if o[0].startswith('OK') else 999)
            o0=subprocess.run(['python3',R+'tools/decomp/trysym.py',un,src,fn],capture_output=True,text=True,env=dict(os.environ,V='1')).stdout.strip().split('\n')
            n0=len(o0)-1 if o0[0].startswith('DIFF') else (0 if o0[0].startswith('OK') else 999)
            print(un,fn[:60],round(p,2),x,n0,'->',n,'OK' if n==0 else '',flush=True)
            os.remove(tmp)
