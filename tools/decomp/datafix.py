# datafix.py <unit...>: for @N in .data/.rodata of the unit (symbols.txt) missing in our .o, rename to our .o @N at the same offset if same size
import re,sys,subprocess
from elftools.elf.elffile import ELFFile
sp=open('config/SLSP01/splits.txt').read(); S='config/SLSP01/symbols.txt'; L=open(S).read().split('\n')
n=0
for u in sys.argv[1:]:
    m=re.search(r'^'+re.escape(u)+r'\.(?:cpp|c):\n((?:\t.*\n)+)',sp,re.M)
    st={mm.group(1):(int(mm.group(2),16),int(mm.group(3),16)) for mm in re.finditer(r'\t(\.\w+)\s+start:0x([0-9A-F]+) end:0x([0-9A-F]+)',m.group(1))}
    e=ELFFile(open('build/SLSP01/src/%s.o'%u,'rb')); ours={}; names=set()
    for s in e.get_section_by_name('.symtab').iter_symbols():
        names.add(s.name)
        if s.name.startswith('@') and s['st_shndx'] not in ('SHN_UNDEF','SHN_ABS','SHN_COMMON'):
            sec=e.get_section(s['st_shndx']).name
            if sec in ('.data','.rodata'): ours[(sec,s['st_value'])]=(s.name,s['st_size'])
    for i,l in enumerate(L):
        mm=re.match(r'(@\d+) = (\.data|\.rodata):0x([0-9A-F]+);.*size:0x([0-9A-F]+)',l)
        if not mm or mm.group(1) in names: continue
        sec=mm.group(2); a=int(mm.group(3),16)
        if sec not in st or not (st[sec][0]<=a<st[sec][1]): continue
        o=ours.get((sec,a-st[sec][0]))
        if o and o[1]==int(mm.group(4),16): L[i]=o[0]+l[len(mm.group(1)):]; n+=1; print(u,mm.group(1),'->',o[0])
open(S,'w').write('\n'.join(L)); print('renamed',n)
