#!/usr/bin/env python3
# usage: addsplit.py <unitname> "<sec> <start> <end>" ... : insert a split block sorted by .text start
#        and a libs_ref entry (flags of OSMutex.c, or of --like X; MW=<version> overrides its compiler)
import os, sys, json, re
ROOT=os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)),'..','..'))
args=sys.argv[1:]; like='OSMutex.c'
if args[0]=='--like': like=args[1]; args=args[2:]
name=args[0]; secs=[a.split() for a in args[1:]]
p=ROOT+'/config/SLSP01/splits.txt'; s=open(p).read()
assert name+':\n' not in s
block=name+':\n'+''.join('\t%-11s start:0x%08X end:0x%08X\n'%('.'+a, int(b,16), int(c,16)) for a,b,c in secs)
tstart=int([x for x in secs if x[0]=='text'][0][1],16)
blocks=s.split('\n\n')
out=[];done=False
for b in blocks:
    m=re.search(r'\.text\s+start:0x([0-9A-F]+)',b)
    if not done and m and int(m.group(1),16)>tstart and not b.startswith('Sections:'):
        out.append(block.rstrip('\n')); done=True
    out.append(b)
if not done: out.append(block.rstrip("\n"))
open(p,'w').write('\n\n'.join(out))
p=ROOT+'/config/SLSP01/libs_ref.json'; d=json.load(open(p))
ref=[u for u in d if u['name'].endswith('/'+like) or u['name']==like][0]
e=dict(ref); e['name']=name; e['matching']=True
if os.environ.get('MW'): e['mw_version']=os.environ['MW']  # compiler other than the like unit's
d.append(e)
open(p,'w').write(json.dumps(d, indent=1, ensure_ascii=False))
print('ok')
