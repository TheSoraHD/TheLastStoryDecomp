# ssauto.py [obj] : after ssfind (functions renamed in symbols.txt), map the data of our object
# (/tmp/sdkcheck_out.o) to target addresses by decoding the DOL at our relocations (HA/LO, SDA21,
# ADDR32 inside data), and print the split ranges per section (text from the named functions).
import sys, re, struct, os
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection
os.chdir(os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', '..')))
obj = sys.argv[1] if len(sys.argv) > 1 else '/tmp/sdkcheck_out.o'
SDA, SDA2 = 0x808856C0, 0x808885A0
dol = open('orig/SLSP01/sys/main.dol', 'rb').read()
offs = struct.unpack('>18I', dol[0:72]); addrs = struct.unpack('>18I', dol[72:144]); sizes = struct.unpack('>18I', dol[144:216])
def rw(a):
    for o, ad, s in zip(offs, addrs, sizes):
        if ad <= a < ad + s: return struct.unpack('>I', dol[o + a - ad:o + a - ad + 4])[0]
syms = {}; tsyms = []
for l in open('config/SLSP01/symbols.txt'):
    m = re.match(r'(\S+) = \.(\w+):0x([0-9A-F]+); // type:(\w+)(?: size:0x([0-9A-F]+))?', l)
    if m:
        syms.setdefault(m.group(1), (m.group(2), int(m.group(3), 16), int(m.group(5) or '0', 16)))
        tsyms.append((m.group(2), int(m.group(3), 16), int(m.group(5) or '0', 16), m.group(1)))
e = ELFFile(open(obj, 'rb'))
secs = {i: s for i, s in enumerate(e.iter_sections())}
st = e.get_section_by_name('.symtab'); sl = list(st.iter_symbols())
addr = {}  # our symbol index -> target address
where = {}  # our (secidx, value) -> target
funcs = []
for i, s in enumerate(sl):
    if s['st_info']['type'] == 'STT_FUNC' and s['st_size'] and s.name in syms and syms[s.name][0] == 'text':
        addr[i] = syms[s.name][1]; funcs.append((syms[s.name][1], syms[s.name][2], s.name, s['st_info']['bind']))
def symat(secidx, off):
    best = None
    for i, s in enumerate(sl):
        if s['st_shndx'] == secidx and s['st_info']['type'] in ('STT_OBJECT', 'STT_FUNC', 'STT_NOTYPE') and s.name and s['st_value'] <= off < s['st_value'] + max(s['st_size'], 1):
            best = i
    return best
rels = {}
for s in e.iter_sections():
    if isinstance(s, RelocationSection): rels[s['sh_info']] = list(s.iter_relocations())
def base_of(i):  # target address of the start of the containing our-symbol, for a section offset
    return addr.get(i)
changed = True
while changed:
    changed = False
    for secidx, rl in rels.items():
        for r in rl:
            off = r['r_offset']; typ = r['r_info_type']; ts = r['r_info_sym']; add = r['r_addend']
            src = symat(secidx, off)
            if src is None or src not in addr: continue
            a = addr[src] + off - sl[src]['st_value']
            tgt = sl[ts]
            if tgt['st_shndx'] in ('SHN_UNDEF', 'SHN_ABS') or not isinstance(tgt['st_shndx'], int): continue
            # the target symbol may be a section symbol: find the real symbol at value+addend
            if tgt['st_info']['type'] == 'STT_SECTION':
                ti = symat(tgt['st_shndx'], add)
                if ti is None: continue
                extra = add - sl[ti]['st_value']
            else:
                ti = ts; extra = add
            if ti in addr: continue
            w = rw(a & ~3)
            if w is None: continue
            val = None
            if typ == 6:  # HA: find LO in same section after
                lo = [x for x in rl if x['r_info_type'] == 4 and x['r_info_sym'] == ts and x['r_addend'] == add and x['r_offset'] > off]
                if not lo: continue
                lo = min(lo, key=lambda x: x['r_offset'])
                lsrc = symat(secidx, lo['r_offset'])
                if lsrc != src: continue
                wl = rw((addr[src] + lo['r_offset'] - sl[src]['st_value']) & ~3)
                hi = w & 0xFFFF; l2 = wl & 0xFFFF
                if l2 & 0x8000: l2 -= 0x10000
                val = ((hi << 16) + l2) & 0xFFFFFFFF
            elif typ == 109:  # SDA21
                ra = (w >> 16) & 0x1F; d = w & 0xFFFF
                if d & 0x8000: d -= 0x10000
                val = (SDA if ra == 13 else SDA2 if ra == 2 else 0) + d
                if ra not in (2, 13): continue
            elif typ == 1:  # ADDR32
                val = w
            if val is None: continue
            addr[ti] = val - extra; changed = True
for i, s_ in enumerate(sl):
    if i in addr or s_['st_info']['type'] != 'STT_OBJECT' or not isinstance(s_['st_shndx'], int): continue
    if s_.name in syms and '.' + syms[s_.name][0] == secs[s_['st_shndx']].name: addr[i] = syms[s_.name][1]
import json
json.dump({sl[i].name: a for i, a in addr.items() if sl[i]['st_info']['type'] == 'STT_OBJECT'}, open('/tmp/ssauto_map.json', 'w'))
res = {}
for i, a in addr.items():
    s = sl[i]
    if not isinstance(s['st_shndx'], int): continue
    sec = secs[s['st_shndx']].name
    res.setdefault(sec, []).append((a, s['st_size'], s.name))
fn = sorted(f for f in funcs if f[3] != 'STB_WEAK')
args = []
if fn:
    lo, hi = fn[0][0], max(a + z for a, z, n, b in fn)
    wk = [f for f in funcs if f[3] == 'STB_WEAK' and lo - 0x100 <= f[0] <= hi + 0x100]
    if wk: lo = min(lo, min(f[0] for f in wk)); hi = max(hi, max(f[0] + f[1] for f in wk))
    args.append('text 0x%X 0x%X' % (lo, hi))
    foreign = [t for t in tsyms if t[0] == 'text' and lo <= t[1] < hi and t[3] not in {f[2] for f in funcs}]
    print('text %X %X' % (lo, hi), ('FOREIGN ' + ' '.join('%s@%X' % (t[3], t[1]) for t in foreign[:6])) if foreign else '')
for sec, l in sorted(res.items()):
    if sec == '.text': continue
    ours = [s for s in e.iter_sections() if s.name == sec][0]['sh_size']
    a = min(x[0] for x in l); b = max(x[0] + x[1] for x in l)
    print('%s %X %X (span %X ours %X) %s' % (sec[1:], a, b, b - a, ours, ' '.join('%s@%X' % (n[:40], x) for x, z, n in sorted(l)[:3])))
    last = max(l)[0]
    tz = [t for t in tsyms if t[0] == sec[1:] and t[1] == last]
    end = max(b, last + tz[0][2]) if tz else b
    args.append('%s 0x%X 0x%X' % (sec[1:], a, end))
    if abs((b - a) - ours) > 8: print('  WARN span/ours mismatch', sec)
unm = [s.name for i, s in enumerate(sl) if isinstance(s['st_shndx'], int) and s['st_shndx'] in secs and secs[s['st_shndx']].name in ('.data', '.rodata', '.sdata', '.sdata2', '.bss', '.sbss') and s['st_info']['type'] == 'STT_OBJECT' and i not in addr]
if unm: print('unmapped:', ' '.join(unm[:12]))
sin = [f for f in funcs if f[2].startswith('__sinit_')]
for f in sin:
    for a in range(0x8072D2A0, 0x8072D600, 4):
        if rw(a) == f[0]: print('ctors %X %X' % (a, a + 4)); args.append('ctors 0x%X 0x%X' % (a, a + 4))
print('ARGS', ' '.join('"%s"' % x for x in args))
