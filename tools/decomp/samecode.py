#!/usr/bin/env python3
# (s12h-20261108) samecode.py [unit...]: for each target function that objdiff leaves unpaired (unnamed weak
# copies, renamed drafts), lists the unpaired functions of our .o with the same or similar code (relocations
# and branch targets masked; >=60% equal lines, +-3 instructions). Exact copies are printed without a
# percentage. Renaming the symbol in symbols.txt pairs them (NewCampPartsGmk: +3).
import json,os,subprocess,re,sys
R='/home/claude/thelaststorydecomp/'
OBJ=R+'build/binutils/powerpc-eabi-objdump'
def funcs(o):
    out=subprocess.run([OBJ,'-dr','--no-show-raw-insn',o],capture_output=True,text=True).stdout
    fs={};cur=None
    for l in out.splitlines():
        m=re.match(r'^[0-9a-f]+ <(.+)>:$',l)
        if m: cur=m.group(1); fs[cur]=[]; continue
        if cur and '\t' in l and not 'R_PPC' in l:
            ins=l.split('\t',1)[1].split('<')[0].strip()
            ins=re.sub(r'\b(b|bl|beq|bne|blt|bgt|ble|bge|bdnz|bdz)\s+[0-9a-f]+','\\1 X',ins)
            fs[cur].append(ins)
    return fs
d=json.load(open(R+'build/SLSP01/report.json'))
for u in d['units']:
    un=u['name'][5:]
    if un.startswith('auto') or not os.path.exists(R+'src/'+un+'.cpp'): continue
    if len(sys.argv)>1 and un not in sys.argv[1:]: continue
    fsr=u.get('functions',[])
    miss=[f['name'] for f in fsr if f.get('fuzzy_match_percent') is None]
    if not miss: continue
    a=R+'build/SLSP01/obj/'+un+'.o'; b=R+'build/SLSP01/src/'+un+'.o'
    if not os.path.exists(a) or not os.path.exists(b): continue
    fa=funcs(a); fb=funcs(b)
    tnames=set(f['name'] for f in fsr)
    ours={k:v for k,v in fb.items() if k not in tnames and v}
    for m in miss:
        ca=fa.get(m)
        if not ca: continue
        hits=[]
        for k,v in ours.items():
            if v==ca:
                hits.append((100,k)); continue
            if abs(len(v)-len(ca))>3 or len(v)<6: continue
            same=sum(1 for x,y in zip(v,ca) if x==y)
            if same>=0.6*len(ca): hits.append((same*100//len(v),k))
        hits.sort(reverse=True)
        if hits: print(un, m, len(ca), '->', hits[:2])
