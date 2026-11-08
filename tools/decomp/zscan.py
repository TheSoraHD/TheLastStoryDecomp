#!/usr/bin/env python3
# zscan.py: NM functions whose count of "z stored before the paired x/y store" differs between target and ours
import json,re,glob,subprocess,os,sys
import os
R=os.path.dirname(os.path.abspath(__file__))+'/../..'
d=json.load(open(R+'/build/SLSP01/report.json'))
def pat(lines):
    zf=0; xyf=0
    for i in range(len(lines)-1):
        a,b=lines[i],lines[i+1]
        if re.search(r'\bstfs\b',a) and re.search(r'\bpsq_st\b',b): zf+=1
        if re.search(r'\bpsq_st\b',a) and re.search(r'\bstfs\b',b): xyf+=1
    return zf,xyf
res=[]
for u in d['units']:
    un=u['name'][5:]
    nm=[f for f in u.get('functions',[]) if f.get('fuzzy_match_percent') is not None and 30<=f['fuzzy_match_percent']<100]
    if not nm: continue
    asm=R+'/build/SLSP01/asm/'+un+'.s'
    obj=R+'/build/SLSP01/src/'+un+'.o'
    if not os.path.exists(asm) or not os.path.exists(obj): continue
    tgt={}; cur=None
    for line in open(asm,errors='ignore'):
        if line.startswith('.fn '): cur=line.split()[1].rstrip(','); tgt[cur]=[]
        elif line.startswith('.endfn'): cur=None
        elif cur and '*/' in line: tgt[cur].append(line.split('*/')[1].strip())
    out=subprocess.run([R+'/build/binutils/powerpc-eabi-objdump','-d','--no-show-raw-insn',obj],capture_output=True,text=True).stdout
    ours={}; cur=None
    for line in out.split('\n'):
        m=re.match(r'^[0-9a-f]+ <(.+)>:',line)
        if m: cur=m.group(1); ours[cur]=[]
        elif cur and '\t' in line: ours[cur].append(line.split('\t',1)[1].replace('xsaddsp','psq_st'))
    for f in nm:
        n=f['name']
        if n in tgt and n in ours:
            a=pat(tgt[n]); b=pat(ours[n])
            if a!=b: res.append((f['fuzzy_match_percent'],un,n,a,b))
for r in sorted(res,reverse=True): print('%6.2f %s %s tgt=%s ours=%s'%r)
