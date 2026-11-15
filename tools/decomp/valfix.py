# valfix.py <unit...>: missing .sdata2 @N (labelcheck) -> our .o @N with the same value (from the original obj) if unique and unused
import re,sys,subprocess
from elftools.elf.elffile import ELFFile
exec(open('tools/decomp/labelcheck.py').read().split('for u in sys.argv')[0])
S='config/SLSP01/symbols.txt'
def vals(p):
    e=ELFFile(open(p,'rb')); out=[]
    for s in e.get_section_by_name('.symtab').iter_symbols():
        if s.name.startswith('@') and s['st_shndx'] not in ('SHN_UNDEF','SHN_ABS','SHN_COMMON'):
            sec=e.get_section(s['st_shndx'])
            if sec.name=='.sdata2': out.append((s.name,s['st_value'],sec.data()[s['st_value']:s['st_value']+4]))
    return out
n=0
for u in sys.argv[1:]:
    rs=ranges(u); st=[a for a,b in rs]
    O=vals('build/SLSP01/obj/%s.o'%u); B=vals('build/SLSP01/src/%s.o'%u)
    have=set(x[0] for x in B)
    sd2=[r for r in rs]
    used=set()
    for i,l in enumerate(syms):
        mm=re.match(r'(@\d+) = \.sdata2:0x([0-9A-F]+)',l)
        if mm and any(s<=int(mm.group(2),16)<e for s,e in rs): used.add(mm.group(1))
    base=min(s for s,e in rs if any(s<=int(re.match(r'@\d+ = \.sdata2:0x([0-9A-F]+)',l).group(1),16)<e for l in syms if re.match(r'@\d+ = \.sdata2:0x',l) and False)) if False else None
    for i,l in enumerate(syms):
        mm=re.match(r'(@\d+) = \.sdata2:0x([0-9A-F]+)',l)
        if not mm or mm.group(1) in have or not any(s<=int(mm.group(2),16)<e for s,e in rs): continue
        a=int(mm.group(2),16)
        ov=[v for nm,off,v in O if nm==mm.group(1)]
        if len(ov)!=1: print(u,'skip',mm.group(1)); continue
        c=[nm for nm,off,v in B if v==ov[0] and nm not in used]
        if len(c)==1: syms[i]=c[0]+l[len(mm.group(1)):]; used.add(c[0]); n+=1; print(u,mm.group(1),'->',c[0])
        else: print(u,mm.group(1),'cands',c)
open(S,'w').write(''.join(syms)); print('renamed',n)
