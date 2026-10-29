"""Simulate the __sinit of Event/EventCommandDef_unk (fn_800082AC) and dump the runtime stores."""
# usage: tools/decomp/fasm.py Event/EventCommandDef_unk 0x800082AC > build/eventcmd/ecd.s; then run ecd_sim.py and ecd_gen.py
import re, struct, json, sys

import os
ROOT = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', '..'))
TMP = os.path.join(ROOT, 'build', 'eventcmd')
os.makedirs(TMP, exist_ok=True)
b = open(ROOT + '/orig/SLSP01/sys/main.dol', 'rb').read()
offs = struct.unpack('>18I', b[0:72]); addrs = struct.unpack('>18I', b[72:144]); sizes = struct.unpack('>18I', b[144:216])


def rd(a, n):
    for o, ad, s in zip(offs, addrs, sizes):
        if ad <= a < ad + s:
            return b[o + a - ad:o + a - ad + n]
    raise KeyError(hex(a))


def u32(a):
    return struct.unpack('>I', rd(a, 4))[0]


def f32(a):
    return struct.unpack('>f', rd(a, 4))[0]


def cstr(a):
    x = rd(a, 256)
    return x[:x.index(0)]


syms = {}
for l in open(ROOT + '/config/SLSP01/symbols.txt'):
    m = re.match(r'(\S+) = (\.\w+):0x([0-9A-F]+); // .*?(?:size:0x([0-9A-F]+))?', l)
    if m:
        syms[m[1]] = int(m[3], 16)

regs = {}
fregs = {}
stack = {}
stores = {}  # addr -> ('hash', str, arg) | ('int', v) | ('float', v, label)
order = []
pending_hash_r3 = None
for line in open(os.path.join(TMP, 'ecd.s')):
    m = re.match(r'([0-9A-F]{4})\s+(\w+)\s*(.*)', line.strip())
    if not m:
        continue
    op, args = m[2], [x.strip() for x in m[3].split(',')] if m[3] else []
    if op == 'lis':
        r, v = args
        if '@ha' in v:
            regs[r] = ('sym', v.split('@')[0])
        else:
            regs[r] = ('val', (int(v, 0) << 16) & 0xffffffff)
    elif op == 'li':
        r, v = args
        regs[r] = ('val', int(v, 0) & 0xffffffff)
    elif op == 'addi':
        r, s, v = args
        if '@l' in v:
            regs[r] = ('addr', syms[v.split('@')[0]])
        elif s == 'r1':
            regs[r] = None
        else:
            src = regs[s]
            iv = int(v, 0)
            if src[0] == 'addr':
                regs[r] = ('addr', src[1] + iv)
            elif src[0] == 'val':
                regs[r] = ('val', (src[1] + iv) & 0xffffffff)
            else:
                raise Exception(line)
    elif op == 'bl':
        if args[0] == 'fn_800DC880':
            s = regs['r3']; a4 = regs['r4']
            assert s[0] == 'addr' and a4[0] == 'val', line
            regs['r3'] = ('hash', cstr(s[1]), a4[1], s[1])
        elif args[0].startswith('_savegpr') or args[0].startswith('_restgpr'):
            pass
        else:
            raise Exception(line)
    elif op == 'lfs':
        f, src = args
        lab = src.split('@')[0]
        fregs[f] = (f32(syms[lab]), lab)
    elif op == 'stfs':
        f, dst = args
        stack[dst] = fregs[f]
    elif op == 'lwz':
        r, src = args
        if src in stack:
            regs[r] = ('float',) + stack[src]
        else:
            regs[r] = None
    elif op == 'stw':
        r, dst = args
        mm = re.match(r'(-?0x[0-9a-f]+|\d+)\((r\d+)\)', dst)
        off, base = int(mm[1], 0), mm[2]
        if base == 'r1':
            continue
        bv = regs[base]
        assert bv[0] == 'addr', line
        addr = bv[1] + off
        v = regs[r]
        if v[0] == 'hash':
            stores[addr] = ('hash', v[1].decode('latin1'), v[2], v[3])
        elif v[0] == 'val':
            stores[addr] = ('int', v[1])
        elif v[0] == 'float':
            stores[addr] = ('float', v[1], v[2])
        else:
            raise Exception(line)
        order.append(addr)
    elif op in ('stwu', 'mflr', 'mtlr', 'blr'):
        pass
    else:
        raise Exception(line)

json.dump({'stores': {hex(k): v for k, v in stores.items()}, 'order': [hex(a) for a in order]},
          open(os.path.join(TMP, 'ecd_stores.json'), 'w'), indent=0)
print(len(stores), 'stores;', 'monotonic' if order == sorted(order) else 'NOT monotonic')
kinds = {}
for v in stores.values():
    kinds[v[0]] = kinds.get(v[0], 0) + 1
print(kinds)
