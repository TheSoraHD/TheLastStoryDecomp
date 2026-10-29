"""Generate src/Event/EventCommandDef_unk.cpp from the DOL data and the simulated __sinit."""
import re, struct, json, sys
exec(open('/home/claude/scratch/ecd_an.py').read())

CMD = 0x80775208
NCMD = 0x7A8 // 0x28


def f32s(v):
    # shortest decimal that rounds to the same float32
    import struct as S
    bits = S.pack('>f', v)
    for p in range(1, 12):
        s = '%.*g' % (p, v)
        if S.pack('>f', float(s)) == bits:
            if 'e' in s:
                m, e = s.split('e')
                s = s  # keep exponent form
            elif '.' not in s:
                s += '.0'
            return s + 'f'
    raise Exception(v)


def lit(bs):
    """C literal for raw bytes (SJIS): plain ASCII, or UTF-8 text when cp932 round-trips."""
    try:
        t = bs.decode('cp932')
        if t.encode('cp932') == bs:
            out = ''
            for ch in t:
                if ch == '"' or ch == '\\':
                    out += '\\' + ch
                elif ord(ch) < 0x20:
                    out += '\\x%02X' % ord(ch)
                else:
                    out += ch
            return '"' + out + '"'
    except UnicodeDecodeError:
        pass
    raise Exception(bs)


def strlit(a):
    return lit(cstr(a))


def camel(s):
    parts = re.split(r'[^0-9A-Za-z]+', s)
    return ''.join(p[:1].upper() + p[1:] for p in parts if p)


# --- names of the data objects -------------------------------------------------------------
names = {}
cmds = []
for i in range(NCMD):
    e = CMD + i * 0x28
    w = [u32(e + o) for o in range(0, 0x28, 4)]
    cmds.append(w)
    base = camel(cstr(w[2]).decode())
    if w[6]:
        names.setdefault(w[6], 's%sParams_unk' % base)
    if w[9]:
        names.setdefault(w[9], 's%sVariants_unk' % base)
        for j in range(w[8]):
            p = u32(w[9] + j * 4)
            if p:
                names.setdefault(p, 's%sParams%d_unk' % (base, j))
names[CMD] = 'sCommands_unk'
names.setdefault(0x807704A0, 'sCommonParams_unk')  # inferred: attributes shared by every command
for k, a, n in data:
    if a not in names:
        names[a] = 'sParams_%08X_unk' % a
        print('unreferenced', hex(a), file=sys.stderr)

# enum arrays: name after the first parameter that uses them
ro_kind = {a: k for a, n, k in ro}
ro_size = {a: n for a, n, k in ro}
enum_names = {}
for k, a, n in data:
    if k not in ('params', '??'):
        continue
    for e in range(a, a + n, 0x18):
        cnt, ptr = u32(e + 0x10), u32(e + 0x14)
        if ptr and ptr not in enum_names:
            nm = names[a][:-len('_unk')] + camel(cstr(u32(e)).decode()) + 'Names_unk'
            if nm in enum_names.values():
                nm = nm[:-4] + '%X_unk' % ptr
            enum_names[ptr] = nm

# --- order: data objects in .data order, enum arrays before the first data object whose new
# literals come after them in .rodata ------------------------------------------------------------
def lits_of_params(a, n):
    out = []
    for e in range(a, a + n, 0x18):
        if u32(e) or (e + 4) in stores:
            out.append(u32(e))
    return out


def lits_of_enum(a):
    n = ro_size[a]
    return [u32(a + i) for i in range(0, n, 4)]


all_enum_lits = set()
for _e in enum_names:
    all_enum_lits.update(lits_of_enum(_e))
seen = set()
sequence = []
enum_list = sorted(enum_names)
ei = 0
for k, a, n in data:
    if k in ('params', '??'):
        ls = lits_of_params(a, n)
    elif k == 'cmd':
        ls = []
        for w in cmds:
            ls += [w[1], w[2]]
    else:
        ls = []
    new = [x for x in ls if x not in seen and secof(x) == '.rodata' and x not in all_enum_lits]
    first = min(new) if new else None
    while ei < len(enum_list) and (first is None or enum_list[ei] < first):
        # only place the enum now if this data object (or an earlier one) needs it, or its address
        # is before this object's literals
        if first is None:
            # does this object use it?
            uses = k in ('params', '??') and any(u32(e + 0x14) == enum_list[ei] for e in range(a, a + n, 0x18))
            if not uses:
                break
        sequence.append(('enum', enum_list[ei]))
        for x in lits_of_enum(enum_list[ei]):
            seen.add(x)
        ei += 1
    sequence.append((k, a, n))
    for x in ls:
        seen.add(x)
assert ei == len(enum_list), (ei, len(enum_list))

# --- emit -------------------------------------------------------------------------------------
out = []
P = out.append
P('// inferred file name (splits-auto-A): definitions of the event commands (effect.play, actor.motion,')
P('// dialog.text...) and of their attributes; the readers look them up by command id. Only the two')
P('// lookups and the __sinit that hashes the names are code.')
P('// Generated from the DOL data by a script (see docs/batch-auto-20kb-game.md).')
P('#include "Event/Node.h"')
P('')
P('namespace Event {')
P('')
P('// inferred: definition of an event command (0x28 bytes, table 0x80775208)')
P('struct CommandDef_unk {')
P('    int mId;                       // 0x00')
P('    const char* mLabel;            // 0x04: inferred: name shown by the tools')
P('    const char* mName;             // 0x08: xml name')
P('    unsigned int mHash;            // 0x0C: fn_800DC880(mName, 789456)')
P('    int m10;                       // 0x10: inferred: category (1 effect/audio, 2 camera...)')
P('    int mParamCount;               // 0x14')
P('    ParamDef_unk* mParams;         // 0x18')
P('    int mVariantParam;             // 0x1C: inferred: attribute that selects the variant, -1 none')
P('    int mVariantCount;             // 0x20')
P('    ParamDef_unk** mVariants;      // 0x24: inferred: attributes of each variant')
P('};')
P('')


def value(v):
    if v is None:
        return None
    if v[0] == 'int':
        x = v[1]
        if x >= 0x80000000:
            x -= 1 << 32
        if x == -0x1000000:
            return '(int)0xFF000000'
        if abs(x) >= 0x10000:
            return '(int)0x%X' % (x & 0xffffffff)
        return str(x)
    if v[0] == 'float':
        return f32s(v[1])
    raise Exception(v)


def ptrname(p):
    if p == 0:
        return '0'
    return names.get(p) or enum_names.get(p) or ('/*?%08X*/0' % p)


for item in sequence:
    if item[0] == 'enum':
        a = item[1]
        n = ro_size[a] // 8
        P('static const ParamEnum_unk %s[%d] = {' % (enum_names[a], n))
        for i in range(n):
            P('    {%s, %s},' % (strlit(u32(a + i * 8)), strlit(u32(a + i * 8 + 4))))
        P('};')
        P('')
        continue
    k, a, n = item
    if k in ('params', '??'):
        cnt = n // 0x18
        P('static ParamDef_unk %s[%d] = {' % (names[a], cnt))
        rows = []
        for e in range(a, a + n, 0x18):
            if u32(e) == 0 and (e + 4) not in stores:
                rows.append(None)
                continue
            nm = cstr(u32(e))
            hs = stores[e + 4]
            assert hs[1].encode('latin1') == nm and hs[2] == 0
            assert u32(e + 4) == 0 and u32(e + 0xc) == 0
            typ = u32(e + 8)
            dv = value(stores.get(e + 0xc))
            ec, ep = u32(e + 0x10), u32(e + 0x14)
            s = lit(nm)
            fields = [s, 'fn_800DC880(%s)' % s, '0x%X' % typ if typ >= 10 else str(typ)]
            if ec or ep:
                fields += [dv if dv is not None else 'ParamValue_unk()', str(ec), ptrname(ep)]
            elif dv is not None:
                fields += [dv]
            rows.append('    {' + ', '.join(fields) + '},')
        while rows and rows[-1] is None:
            rows.pop()
        assert None not in rows
        for r in rows:
            P(r)
        P('};')
        P('')
    elif k == 'ptrs':
        cnt = n // 4
        ps = [u32(a + i) for i in range(0, n, 4)]
        while ps and ps[-1] == 0:
            ps.pop()
        P('static ParamDef_unk* %s[%d] = {' % (names[a], cnt))
        for p in ps:
            P('    %s,' % ptrname(p))
        P('};')
        P('')
    elif k == 'cmd':
        P('static CommandDef_unk sCommands_unk[%d] = {' % NCMD)
        for i, w in enumerate(cmds):
            e = CMD + i * 0x28
            hs = stores[e + 0xc]
            assert hs[1].encode('latin1') == cstr(w[2]) and hs[2] == 789456
            nmv = strlit(w[2])
            def iv(x):
                return str(x - (1 << 32)) if x >= 0x80000000 else ('0x%X' % x if x >= 10 else str(x))
            P('    {%s, %s, %s, fn_800DC880(%s, 789456), %s, %s, %s, %s, %s, %s},' % (
                iv(w[0]), strlit(w[1]), nmv, nmv, iv(w[4]), iv(w[5]), ptrname(w[6]), iv(w[7]), iv(w[8]), ptrname(w[9])))
        P('};')
        P('')
P('static int sCommandCount_unk = sizeof(sCommands_unk) / sizeof(sCommands_unk[0]);')
P('')
P('} // namespace Event')
P('')
P('// inferred: definition of the command `id`')
P('extern "C" Event::CommandDef_unk* fn_800081C0(int id) {')
P('    for (int i = 0; i < Event::sCommandCount_unk; i++) {')
P('        if (id == Event::sCommands_unk[i].mId) {')
P('            return &Event::sCommands_unk[i];')
P('        }')
P('    }')
P('    return 0;')
P('}')
P('')
P('// inferred: attributes of the command `id` (of its variant `variant` when it is not negative);')
P('// `count` receives the number of attributes')
P('extern "C" Event::ParamDef_unk* fn_80008210(int id, int* count, int variant) {')
P('    for (int i = 0; i < Event::sCommandCount_unk; i++) {')
P('        if (id == Event::sCommands_unk[i].mId) {')
P('            if (count != 0) {')
P('                *count = Event::sCommands_unk[i].mParamCount;')
P('            }')
P('            if (variant >= 0) {')
P('                return Event::sCommands_unk[i].mVariants[variant];')
P('            }')
P('            return Event::sCommands_unk[i].mParams;')
P('        }')
P('    }')
P('    return 0;')
P('}')
open(ROOT + '/src/Event/EventCommandDef_unk.cpp', 'w', encoding='utf-8').write('\n'.join(out) + '\n')
print('written', len(out), 'lines;', len(enum_names), 'enums')
