"""Analyse the data of Event/EventCommandDef_unk using the simulated __sinit stores."""
import re, struct, json, sys
sys.path.insert(0, '/home/claude/scratch')
ROOT = '/home/claude/thesorahd/thelaststorydecomp'
b = open(ROOT + '/orig/SLSP01/sys/main.dol', 'rb').read()
offs = struct.unpack('>18I', b[0:72]); addrs = struct.unpack('>18I', b[72:144]); sizes = struct.unpack('>18I', b[144:216])


def rd(a, n):
    for o, ad, s in zip(offs, addrs, sizes):
        if ad <= a < ad + s:
            return b[o + a - ad:o + a - ad + n]
    raise KeyError(hex(a))


def u32(a):
    return struct.unpack('>I', rd(a, 4))[0]


def cstr(a):
    x = rd(a, 512)
    return x[:x.index(0)]


RANGES = {'.rodata': (0x8072D480, 0x8072F890), '.data': (0x807704A0, 0x807759B0),
          '.sdata2': (0x808805A0, 0x80880668), '.sdata': (0x8087D6C0, 0x8087D6C8)}
objs = {k: [] for k in RANGES}
for l in open(ROOT + '/config/SLSP01/symbols.txt'):
    m = re.match(r'(\S+) = (\.\w+):0x([0-9A-F]+); // .*?size:0x([0-9A-F]+)(.*)', l)
    if not m:
        continue
    a = int(m[3], 16)
    if m[2] in RANGES and RANGES[m[2]][0] <= a < RANGES[m[2]][1]:
        objs[m[2]].append((a, int(m[4], 16), m[1], m[5]))
for k in objs:
    objs[k].sort()

st = json.load(open('/home/claude/scratch/ecd_stores.json'))
stores = {int(k, 16): v for k, v in st['stores'].items()}


def secof(a):
    for k, (lo, hi) in RANGES.items():
        if lo <= a < hi:
            return k
    return None


# classify .data objects
data = []
for a, n, name, rest in objs['.data']:
    if a == 0x80775208:
        data.append(('cmd', a, n))
        continue
    if n % 0x18 == 0 and all((a + i + 4) in stores for i in range(0, n, 0x18)):
        data.append(('params', a, n))
    elif all((a + i) not in stores for i in range(0, n, 4)):
        data.append(('ptrs', a, n))
    else:
        data.append(('??', a, n))
cnt = {}
for d in data:
    cnt[d[0]] = cnt.get(d[0], 0) + 1
print(cnt)
for d in data:
    if d[0] == '??':
        print('??', hex(d[1]), hex(d[2]))

# check params: hash string == name string, store kinds
bad = 0
fields = {}
for kind, a, n in data:
    if kind != 'params':
        continue
    for e in range(a, a + n, 0x18):
        nm = u32(e)
        hs = stores[e + 4]
        nmstr = cstr(nm)
        if hs[1].encode('latin1') != nmstr or hs[2] != 0:
            bad += 1
            if bad < 5:
                print('name/hash differ', hex(e), nmstr, hs)
        dv = stores.get(e + 0xc)
        fields.setdefault('def', {}).setdefault(dv[0] if dv else None, 0)
        fields['def'][dv[0] if dv else None] += 1
        for off in (0x8, 0x10, 0x14):
            if (e + off) in stores:
                print('runtime store at', hex(e + off))
        if u32(e + 0xc) != 0:
            print('static default?', hex(e), hex(u32(e + 0xc)))
print('bad', bad, fields)
# command table
for i in range(49):
    e = 0x80775208 + i * 0x28
    hs = stores[e + 0xc]
    n4, n8 = cstr(u32(e + 4)), cstr(u32(e + 8))
    if hs[1].encode('latin1') not in (n4, n8):
        print('cmd hash?', i, hs, n4, n8)
    other = [hex(e + o) for o in range(0, 0x28, 4) if o != 0xc and (e + o) in stores]
    if other:
        print('cmd runtime', i, other)
print('cmd hash arg', set(stores[0x80775208 + i * 0x28 + 0xc][2] for i in range(49)))
print('cmd hash is field 8?', all(stores[0x80775208 + i * 0x28 + 0xc][1].encode('latin1') == cstr(u32(0x80775208 + i * 0x28 + 8)) for i in range(49)))

# rodata objects
def kind_ro(a, n, rest):
    if 'string' in rest:
        return 'str'
    w = [u32(a + i) for i in range(0, n - n % 4, 4)]
    if n % 8 == 0 and w and all(secof(x) in ('.rodata', '.sdata2') for x in w):
        return 'enum'
    return 'str?'
ro = [(a, n, kind_ro(a, n, rest)) for a, n, name, rest in objs['.rodata']]
from collections import Counter
print(Counter(k for _, _, k in ro))
for a, n, k in ro:
    if k == 'str?':
        print('str?', hex(a), n, rd(a, n))
