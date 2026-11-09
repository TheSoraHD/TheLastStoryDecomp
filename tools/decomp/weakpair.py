#!/usr/bin/env python3
# weakpair.py [units...]: target fn_XXXXXXXX functions whose bytes (relocs masked) and size equal exactly ONE
# function of our .o that has no symbol of the same name in the target; prints "unit fn_ -> ourname"
import subprocess, sys, json, os, re
R=os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..')
os.chdir(R)
def syms(p):
    out=subprocess.run(['build/binutils/powerpc-eabi-nm','-S',p],capture_output=True,text=True).stdout
    r={}
    for l in out.splitlines():
        q=l.split(None,3)
        if len(q)==4 and q[2] in 'TtWw': r[q[3]]=(int(q[0],16),int(q[1],16))
    return r
def code(p):
    # bytes of .text with relocated fields zeroed (objdump -dr)
    out=subprocess.run(['build/binutils/powerpc-eabi-objdump','-dr','--no-show-raw-insn',p],capture_output=True,text=True).stdout
    fn={};cur=None;ins=[]
    for l in out.splitlines():
        m=re.match(r'^[0-9a-f]+ <(.*)>:$',l)
        if m: cur=m.group(1); fn[cur]=[]; continue
        if cur is None: continue
        m=re.match(r'^\s+[0-9a-f]+:\s+(\S+)\s*(.*)$',l)
        if m:
            a=m.group(2)
            a=re.sub(r'\b[0-9a-f]+ <[^>]*>','T',a)
            fn[cur].append(m.group(1)+' '+a)
        elif 'R_PPC' in l and fn.get(cur): fn[cur][-1]+=' R'
    return fn
units=sys.argv[1:]
if not units:
    units=[u['name'][5:] for u in json.load(open('build/SLSP01/report.json'))['units'] if u['name'].startswith('main/')]
for u in units:
    t='build/SLSP01/obj/%s.o'%u; o='build/SLSP01/src/%s.o'%u
    if not (os.path.exists(t) and os.path.exists(o)): continue
    ts=syms(t); os_=syms(o)
    extra=[n for n in os_ if n not in ts]
    cand=[n for n in ts if re.match(r'^fn_[0-9A-F]{8}$',n) and n not in os_]
    if not extra or not cand: continue
    tc=code(t); oc=code(o)
    for c in cand:
        m=[e for e in extra if os_[e][1]==ts[c][1] and oc.get(e)==tc.get(c)]
        if len(m)==1: print(u,c,'->',m[0])
        elif len(m)>1: print(u,c,'AMBIG',len(m),m[:3])
