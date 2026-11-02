# padfix.py [--apply]: auto_ gaps made only of zero padding -> extend the .text end of the unit that ends at the gap
# usage: padfix.py [--apply] (run with a reference report.json of a build that still has the pad_ gaps; edit the path below)
import json,re,struct,sys
apply='--apply' in sys.argv
d=open('orig/SLSP01/sys/main.dol','rb').read()
offs=struct.unpack('>18I',d[0:72]);addr=struct.unpack('>18I',d[72:144]);sz=struct.unpack('>18I',d[144:216])
def rd(a,n):
  for o,b,s in zip(offs,addr,sz):
    if b<=a<b+s: return d[o+a-b:o+a-b+n]
r=json.load(open('/home/claude/ref/report.json'))
p='config/SLSP01/splits.txt'; s=open(p).read()
n=0
gu=json.load(open('config/SLSP01/game_units.json'))+json.load(open('config/SLSP01/libs_ref.json'))
match={u['name']:bool(u.get('matching')) for u in gu}
for u in r['units']:
  if 'auto_' not in u['name']: continue
  fs=u.get('functions',[])
  if len(fs)!=1 or not fs[0]['name'].startswith('pad_'): continue
  a=int(fs[0]['metadata']['virtual_address']); size=int(fs[0]['size'])
  b=rd(a,size)
  if b is None or any(b): print('NONZERO',u['name'],b.hex() if b else None); continue
  if (a+size)%16: print('NOTALIGNED',u['name']); continue
  m=re.search(r'^(\S+):\n((?:\t.*\n)*?)\t\.text       start:0x([0-9A-F]+) end:0x%08X\n'%a,s,re.M)
  if not m: print('NOPRED',u['name']); continue
  pred=m.group(1)
  nm=re.search(r'^(\S+):\n(?:\t.*\n)*?\t\.text       start:0x%08X '%(a+size),s,re.M)
  nxt=nm.group(1) if nm else None
  if match.get(pred) and not match.get(nxt):
    print('SKIP',pred,'->',nxt); continue
  old='\t.text       start:0x%s end:0x%08X\n'%(m.group(3),a)
  new='\t.text       start:0x%s end:0x%08X\n'%(m.group(3),a+size)
  print('EXT',m.group(1),'%08X->%08X'%(a,a+size)); n+=1
  if apply: s=s.replace(old,new,1)
if apply: open(p,'w').write(s)
print(n)
