import os
import sys,re,json,os,collections
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
from common import *
C=json.load(open(os.path.join(os.path.dirname(os.path.abspath(__file__)),'canon.json')))
dry='--dry' in sys.argv
report=collections.Counter(); flags=[]
def norm(t): return re.sub(r'\s+',' ',re.sub(r'\bconst\b','',t)).replace(' *','*').replace(' &','&').strip()
def simple(e): return re.fullmatch(r'[\w.]+(->[\w.]+)*(\[[^\]]*\])?',e.strip()) is not None
def find_call_end(t,i):
    d=0
    for j in range(i,len(t)):
        if t[j]=='(': d+=1
        elif t[j]==')':
            d-=1
            if d==0: return j
    raise Exception('unbalanced')
def split_args(s):
    out=[];d=0;cur='';q=None
    k=0
    while k<len(s):
        ch=s[k]
        if q:
            cur+=ch
            if ch=='\\': cur+=s[k+1];k+=1
            elif ch==q: q=None
        elif ch in '"\'': q=ch;cur+=ch
        elif ch in '([{': d+=1;cur+=ch
        elif ch in ')]}': d-=1;cur+=ch
        elif ch==',' and d==0: out.append(cur);cur=''
        else: cur+=ch
        k+=1
    out.append(cur); return out
def clean(a): return re.sub(r'\\\n',' ',a).strip()
for path in sorted(src_files()):
    t=open(path,encoding='latin1').read(); orig=t
    rel=os.path.relpath(path,ROOT)
    cdecl={}
    # remove declarations
    def rm(m):
        n=m.group(1)
        if n not in C: return m.group(0)
        cdecl[n]=[re.sub(r'\s+',' ',x) for x in split_params(m.group(2))]
        return '\x00'
    t=DECL_RE.sub(rm,t)
    if not cdecl: continue
    # drop the now-empty lines (decl + trailing same-line comment)
    t=re.sub(r'[ \t]*\x00[ \t]*(//[^\n]*)?\n','',t)
    t=t.replace('\x00','')
    # calls
    pos=0
    out=''
    pat=re.compile(r'\b(fn_800[56][0-9A-F]{4})\s*\(')
    while True:
        m=pat.search(t,pos)
        if not m: out+=t[pos:]; break
        n=m.group(1)
        if n not in C or n not in cdecl:
            if n in C: flags.append(f'{rel}: call to {n} without local declaration')
            out+=t[pos:m.end()]; pos=m.end(); continue
        op=m.end()-1; cp=find_call_end(t,op)
        args=[clean(a) for a in split_args(t[op+1:cp])]
        ctys=[ptype(p)[0] for p in cdecl[n]]
        if len(args)!=len(ctys):
            flags.append(f'{rel}: {n} call has {len(args)} args, decl {len(ctys)}'); out+=t[pos:m.end()]; pos=m.end(); continue
        obj=args[0]; oty=ctys[0]
        iq=[(a,ty) for a,ty in zip(args[1:],ctys[1:]) if not is_float(ty)]
        fq=[(a,ty) for a,ty in zip(args[1:],ctys[1:]) if is_float(ty)]
        canon=C[n]; new=[]
        ci=[p for p in canon if not is_float(ptype(p)[0])]; cf=[p for p in canon if is_float(ptype(p)[0])]
        if len(iq)!=len(ci) or len(fq)>len(cf):
            flags.append(f'{rel}: {n} int/float count {len(iq)}/{len(fq)} vs {len(ci)}/{len(cf)}'); out+=t[pos:m.end()]; pos=m.end(); continue
        if len(fq)<len(cf): flags.append(f'{rel}: {n} MISSING float args filled with FIXME')
        ii=fi=0
        for p in canon:
            cty=ptype(p)[0]
            if is_float(cty):
                if fi<len(fq): new.append(fq[fi][0])
                else: new.append('0.0f /* FIXME: original leaves this register unset */')
                fi+=1; continue
            a,ty=iq[ii]; ii+=1
            if '&' in ty and '*' in cty:
                a='&'+a if simple(a) else '&('+a+')'
            elif '*' in cty and '*' in ty and norm(ty)!=norm(cty) and a!='0' and norm(cty)!='void*':
                a='('+cty+')'+(a if simple(a) else '('+a+')'); report['cast']+=1
            elif '&' in cty and '*' in ty:
                if a.startswith('&') and not a.startswith('&&'): a=a[1:].strip()
                elif a.startswith('(') and False: pass
                else: a='*'+a if simple(a) else '*('+a+')'
                if a.startswith('(') and a.endswith(')') and a.count('(')==1: a=a[1:-1]
            new.append(a)
        if obj=='lbl_8087EEB0' or norm(oty)=='atn::DrawHelper*':
            recv=obj if simple(obj) else '('+obj+')'
        else:
            recv='((atn::DrawHelper*)'+(obj if simple(obj) else '('+obj+')')+')'
        multiline='\\\n' in t[op:cp]
        call=f'{recv}->{n}('+', '.join(new)+')'
        out+=t[pos:m.start()]+call; pos=cp+1; report['calls']+=1
    t=out
    # singleton declaration and include
    t=re.sub(r'[ \t]*extern\s+(?:"C"\s+)?void\s*\*\s*lbl_8087EEB0\s*;[^\n]*\n','',t)
    if '"atn/DrawHelper.h"' not in t:
        incs=list(re.finditer(r'^#include[^\n]*\n',t,re.M))
        at=incs[-1].end() if incs else 0
        t=t[:at]+'#include "atn/DrawHelper.h"\n'+t[at:]
    report['files']+=1
    if not dry and t!=orig: open(path,'w',encoding='latin1').write(t)
print(dict(report))
for f in flags: print('FLAG',f)
