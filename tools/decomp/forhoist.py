#!/usr/bin/env python3
# (s12h-20261108) forhoist.py <maxsize hex> <minpct> [maxpct] [unitfilter]: for NM functions, moves the declaration
# of a local (a `for` loop variable or `T x = e;`) to the top of the function body (`T x;` ... `x = e;`).
# MWCC colours registers by declaration order: in the MSL __sort a `T* it;` declared before `result` fixed the
# swap of the two pointers. Prints OK when trysym.py gives identical bytes (relocations masked).
import json,re,subprocess,os,sys
R=os.path.dirname(os.path.abspath(__file__))+'/../../'
d=json.load(open(R+'build/SLSP01/report.json'))
maxsz=int(sys.argv[1],16); minp=float(sys.argv[2]); maxp=float(sys.argv[3]) if len(sys.argv)>3 else 100
flt=sys.argv[4] if len(sys.argv)>4 else ''
APPLY=os.environ.get('APPLY')  # 1: keep the OK variants in the source
SKIP=os.environ.get('SKIPTO')  # unit name to start from
MODE=os.environ.get('MODE','top')  # top: function body; block: innermost enclosing block; prev: before the previous declaration; split: in place
TY=r'(?:const\s+)?(?:unsigned\s+|signed\s+)?[A-Za-z_][\w:]*(?:<[^;()]*?>)?(?:\s*\*)*'
FOR=re.compile(r'^(\s+)for \(('+TY+r')\s+(\w+) = ([^;]+);')
DEF=re.compile(r'^(\s+)('+TY+r')\s+(\w+) = ([^;]+);\s*$')
BAD=('return','delete','goto','else','case')
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
        for k in range(1,len(lines)):
            m=FOR.match(lines[k]) or DEF.match(lines[k])
            if not m: continue
            ind,ty,name,expr=m.groups()
            if ty.split()[0] in BAD or ty in ('return','delete'): continue
            if k==1 and m.re is DEF: continue
            if len(re.findall(r'\b'+TY+r'\s+'+re.escape(name)+r'\b\s*[=;(\[]',body))!=1: continue
            nl=lines[:]
            if m.re is FOR: nl[k]=lines[k].replace('for (%s %s = '%(ty,name),'for (%s = '%name,1)
            else: nl[k]='%s%s = %s;'%(ind,name,expr)
            if MODE=='split':
                if m.re is FOR: continue
                nl.insert(k,'%s%s %s;'%(ind,ty,name))
            elif MODE=='top':
                nl.insert(1,'    %s %s;'%(ty,name))
            else:
                at=None
                for q in range(k-1,0,-1):
                    lq=lines[q]
                    if MODE=='block' and lq.rstrip().endswith('{') and len(lq)-len(lq.lstrip())==len(ind)-4:
                        at=q+1; break
                    if MODE=='prev' and (len(lq)-len(lq.lstrip()))<len(ind) and lq.strip(): break
                    if MODE=='prev' and len(lq)-len(lq.lstrip())==len(ind) and DEF.match(lq):
                        at=q; break
                if at is None or at==1 or at==k: continue
                nl.insert(at,'%s%s %s;'%(ind,ty,name))
            t=s[:st]+'\n'.join(nl)+s[i:]
            tmp=os.path.dirname(src)+'/.fh%d.cpp'%os.getpid()
            open(tmp,'w',encoding='latin-1').write(t)
            o=subprocess.run(['python3',R+'tools/decomp/trysym.py',un,tmp,fn],capture_output=True,text=True).stdout.strip()
            print(un,fn,round(p,2),k,lines[k].strip()[:50],(o.split()[0] if o else 'ERR'),flush=True)
            os.remove(tmp)
            if APPLY and o.startswith('OK'):
                open(src,'w',encoding='latin-1').write(t); s=t
                break
