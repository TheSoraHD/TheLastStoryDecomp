#!/usr/bin/env python3
# bvconst.py: for NM functions whose definition has by-value class params (Vector3 etc), try top-level const on them
import json,re,subprocess,os,sys
R=os.path.dirname(os.path.abspath(__file__))+'/../../'
d=json.load(open(R+'build/SLSP01/report.json'))
nm={}
for u in d['units']:
    un=u['name'][5:]
    for f in u.get('functions',[]):
        p=f.get('fuzzy_match_percent')
        if p is not None and 40<=p<100: nm.setdefault(un,[]).append((f['name'],p))
TYPES=os.environ.get('TYPES') or r'(?:atn::)?(?:Vector3|Vector2_unk|Matrix3x4|Color|Sphere|Quaternion_unk)'
for un,funcs in sorted(nm.items()):
    src=R+'src/'+un+'.cpp'
    if not os.path.exists(src): continue
    s=open(src,encoding='latin-1').read()
    # definitions with a by-value param of these types
    for m in re.finditer(r'^[^\n;{}]*?\b(\w+)(?:::(\w+))?\(([^()]*\b'+TYPES+r'\s+\w+[^()]*)\)\s*(?:const\s*)?(?::[^{;]*)?\{',s,re.M):
        params=m.group(3)
        if not re.search(r'(?<![\w:&*])'+TYPES+r'\s+\w+\s*(?:,|$)',params): continue
        name=m.group(2) or m.group(1)
        cls=m.group(1) if m.group(2) else None
        cand=[(fn,p) for fn,p in funcs if fn==name or fn.startswith(name+'__') or (name==cls and fn.startswith('__ct__'))]
        if not cand: continue
        newp=re.sub(r'(?<![\w:&*])(?<!const )('+TYPES+r'\s+\w+\s*(?:,|$))',r'const \1',params)
        if newp==params: continue
        t=s[:m.start(3)]+newp+s[m.end(3):]
        tmp=os.path.dirname(src)+'/.bv.cpp'
        open(tmp,'w',encoding='latin-1').write(t)
        for fn,p in cand:
            o=subprocess.run(['python3',''+R+'tools/decomp/trysym.py',un,tmp,fn],capture_output=True,text=True).stdout.strip()
            print(un,fn,round(p,2),o.split()[0] if o else 'ERR',flush=True)
        os.remove(tmp)
