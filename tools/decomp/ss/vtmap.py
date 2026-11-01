# vtmap.py [--apply] : for the data objects of /tmp/sdkcheck_out.o whose name is in symbols.txt (vtables),
# map every ADDR32 relocation to a function onto the target word at the same offset and propose
# fn_ -> our function name (thunks and weak inline virtuals emitted in other units).
import sys, re, struct, os, subprocess
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection
os.chdir(os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', '..')))
apply = '--apply' in sys.argv
dol = open('orig/SLSP01/sys/main.dol', 'rb').read()
offs = struct.unpack('>18I', dol[0:72]); addrs = struct.unpack('>18I', dol[72:144]); sizes = struct.unpack('>18I', dol[144:216])
def rw(a):
    for o, ad, s in zip(offs, addrs, sizes):
        if ad <= a < ad + s: return struct.unpack('>I', dol[o + a - ad:o + a - ad + 4])[0]
syms = {}; byaddr = {}
for l in open('config/SLSP01/symbols.txt'):
    m = re.match(r'(\S+) = \.(\w+):0x([0-9A-F]+);', l)
    if m:
        syms.setdefault(m.group(1), int(m.group(3), 16)); byaddr.setdefault(int(m.group(3), 16), m.group(1))
import json
for k, v in json.load(open('/tmp/ssauto_map.json')).items(): syms[k] = v
e = ELFFile(open('/tmp/sdkcheck_out.o', 'rb'))
sl = list(e.get_section_by_name('.symtab').iter_symbols())
props = {}
for s in e.iter_sections():
    if not isinstance(s, RelocationSection): continue
    tsec = e.get_section(s['sh_info'])
    if tsec.name not in ('.data', '.rodata'): continue
    objs = [x for x in sl if x['st_shndx'] == s['sh_info'] and x['st_info']['type'] == 'STT_OBJECT' and x.name in syms]
    for r in s.iter_relocations():
        if r['r_info_type'] != 1: continue
        o = [x for x in objs if x['st_value'] <= r['r_offset'] < x['st_value'] + x['st_size']]
        if not o: continue
        o = o[0]; tgt = sl[r['r_info_sym']]
        if tgt['st_info']['type'] != 'STT_FUNC': continue
        a = syms[o.name] + r['r_offset'] - o['st_value']
        w = rw(a); n = byaddr.get(w)
        if n and n != tgt.name and (n.startswith('fn_') or n.startswith('auto_')):
            props.setdefault(n, set()).add(tgt.name)
        elif n and n != tgt.name:
            print('NAMEDIFF %s @%X: target %s ours %s' % (o.name[:40], a, n[:60], tgt.name[:60]))
al = []
for n, b in sorted(props.items()):
    print(n, ' '.join(b))
    if len(b) == 1 and list(b)[0] not in syms: al += [n, list(b)[0]]
if apply and al: print(subprocess.run(['python3', 'tools/decomp/rensym.py'] + al, capture_output=True, text=True).stdout[-300:])
