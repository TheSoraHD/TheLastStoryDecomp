#!/usr/bin/env python3
# (s12h-20261108) ifswap.py <maxsize hex> <minpct> [maxpct] [unitfilter]: for NM functions, swaps the branches of
# `if (c) { A } else { B }` into `if (!(c)) { B } else { A }` (only when both blocks are plain `{ }` blocks at the
# same indentation and the else is not an `else if`). Prints OK when trysym.py gives identical bytes.
# APPLY=1 keeps the OK variants; SKIPTO=<unit> starts from that unit.
import json,re,subprocess,os,sys
R=os.path.dirname(os.path.abspath(__file__))+'/../../'
d=json.load(open(R+'build/SLSP01/report.json'))
maxsz=int(sys.argv[1],16); minp=float(sys.argv[2]); maxp=float(sys.argv[3]) if len(sys.argv)>3 else 100
flt=sys.argv[4] if len(sys.argv)>4 else ''
APPLY=os.environ.get('APPLY'); SKIP=os.environ.get('SKIPTO')
IF=re.compile(r'^(\s+)(?:\} else )?if \((.*)\) \{\s*$')
def neg(c):
    c=c.strip()
    m=re.fullmatch(r'!\((.*)\)',c)
    if m and m.group(1).count('(')==m.group(1).count(')'): return m.group(1)
    if re.fullmatch(r'[\w\.\->\[\]]+',c): return '!'+c
    m=re.fullmatch(r'([\w\.\->\[\]\(\)]+) (==|!=|<|>|<=|>=) ([\w\.\->\[\]\(\)\-]+)',c)
    if m and '&&' not in c and '||' not in c:
        op={'==':'!=','!=':'==','<':'>=','>':'<=','<=':'>','>=':'<'}[m.group(2)]
        return '%s %s %s'%(m.group(1),op,m.group(3))
    return '!(%s)'%c
for u in d['units']:
    un=u['name'][5:]
    if SKIP:
        if SKIP!=un: continue
        SKIP=None
    if flt and flt not in un: continue
    src=R+'src/'+un+'.cpp'
    if not os.path.exists(src): continue
    s=open(src,encoding='latin-1').read()
    for f in u.get('functions',[]):
        p=f.get('fuzzy_match_percent'); fn=f['name']; sz=int(f.get('size',0))
        if p is None or not (minp<=p<maxp) or sz>maxsz: continue
        meth=fn.split('__')[0] if not fn.startswith('__') else None
        if not meth or '<' in meth: continue
        defs=list(re.finditer(r'^[^\n;{}()]*\b'+re.escape(meth)+r'\([^;{]*?\)\s*(?:const\s*)?(?::[^{;]*)?\{',s,re.M))
        if len(defs)!=1: continue
        st=defs[0].end(); depth=1; i=st
        while depth and i<len(s):
            if s[i]=='{': depth+=1
            elif s[i]=='}': depth-=1
            i+=1
        body=s[st:i]; lines=body.split('\n')
        for k in range(len(lines)):
            m=IF.match(lines[k])
            if not m or lines[k].strip().startswith('} else'): continue
            ind,cond=m.groups()
            # find "} else {" and the closing "}" at the same indentation
            e=None; c=None
            for q in range(k+1,len(lines)):
                lq=lines[q]
                if lq.startswith(ind+'}') and not lq.startswith(ind+' '):
                    if lq.strip()=='} else {' and e is None: e=q; continue
                    if lq.strip()=='}' and e is not None: c=q
                    break
            if e is None or c is None: continue
            A=lines[k+1:e]; B=lines[e+1:c]
            nl=lines[:k]+['%sif (%s) {'%(ind,neg(cond))]+B+['%s} else {'%ind]+A+['%s}'%ind]+lines[c+1:]
            t=s[:st]+'\n'.join(nl)+s[i:]
            tmp=os.path.dirname(src)+'/.is%d.cpp'%os.getpid()
            open(tmp,'w',encoding='latin-1').write(t)
            o=subprocess.run(['python3',R+'tools/decomp/trysym.py',un,tmp,fn],capture_output=True,text=True).stdout.strip()
            print(un,fn,round(p,2),k,lines[k].strip()[:50],(o.split()[0] if o else 'ERR'),flush=True)
            os.remove(tmp)
            if APPLY and o.startswith('OK'):
                open(src,'w',encoding='latin-1').write(t); s=t
                break
