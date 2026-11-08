#!/usr/bin/env python3
# effz.py minp: replace the inline zero-origin effect block by an inline helper
import json,re,subprocess,os,sys
R=os.path.abspath(os.path.join(os.path.dirname(__file__),'..','..'))+'/'
d=json.load(open(R+'build/SLSP01/report.json'))
minp=float(sys.argv[1])
LINE=re.compile(r'^\s*(Color4f_unk color;|atn::Vector3 rot;|atn::Vector3 pos;|(rot|pos)\.[xyz] = 0\.0f;|color\.[rgba] = 1\.0f;)\s*$')
CALL=re.compile(r'^(\s*)fn_8023A8B4\(([^,]+), ([^,]+), ([^,]+), 0, pos, rot, color, -1, -1, 1, 1\.0f\);\s*$')
ORD=['    atn::Vector3 pos;\n    atn::Vector3 rot;\n    Color4f_unk color;','    Color4f_unk color;\n    atn::Vector3 rot;\n    atn::Vector3 pos;']
def helper(o):
    return ('inline void EffOrigin_unk(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model) {\n'+o+
      '\n    rot.x = 0.0f;\n    rot.y = 0.0f;\n    rot.z = 0.0f;\n    pos.x = 0.0f;\n    pos.y = 0.0f;\n    pos.z = 0.0f;\n'
      '    color.r = 1.0f;\n    color.g = 1.0f;\n    color.b = 1.0f;\n    color.a = 1.0f;\n'
      '    fn_8023A8B4(mgr, efp, model, 0, pos, rot, color, -1, -1, 1, 1.0f);\n}\n\n')
def score(un,f,fn):
    o=subprocess.run(['python3',R+'tools/decomp/trysym.py',un,f,fn],capture_output=True,text=True,env=dict(os.environ,V='1')).stdout.strip().split('\n')
    return len(o)-1 if o[0].startswith('DIFF') else (0 if o[0].startswith('OK') else 999)
for u in d['units']:
    un=u['name'][5:]
    src=R+'src/'+un+'.cpp'
    if not os.path.exists(src): continue
    s=open(src,encoding='latin-1').read()
    if '0, pos, rot, color, -1, -1, 1, 1.0f' not in s: continue
    for f in u.get('functions',[]):
        p=f.get('fuzzy_match_percent'); fn=f['name']; sz=int(f.get('size',0))
        if p is None or not (minp<=p<100): continue
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
        ls=s.rfind('\n',0,defs[0].start())+1
        while True:
            pl=s.rfind('\n',0,ls-1)+1
            if s[pl:ls].lstrip().startswith(('//','template')): ls=pl
            else: break
        for k,l in enumerate(lines):
            m=CALL.match(l)
            if not m: continue
            j=k
            while j>0 and LINE.match(lines[j-1]): j-=1
            if k-j!=13: continue
            nl=lines[:j]+[m.group(1)+'EffOrigin_unk(%s, %s, %s);'%(m.group(2),m.group(3),m.group(4))]+lines[k+1:]
            n0=score(un,src,fn)
            for o in ORD:
                t=s[:ls]+helper(o)+s[ls:st]+'\n'.join(nl)+s[i:]
                tmp=os.path.dirname(src)+'/.effz.cpp'
                open(tmp,'w',encoding='latin-1').write(t)
                n=score(un,tmp,fn); os.remove(tmp)
                print(un,fn[:60],round(p,2),k,n0,'->',n,'OK' if n==0 else '',flush=True)
