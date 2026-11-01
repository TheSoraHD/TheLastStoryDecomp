# ssfind.py <ss src (relative to libs/ss_nw4r or ref_ss)> [--assert] [--apply] [--mw Wii/1.0]
# Compile a SS-decomp nw4r source with the lib-zone NW4R flags (snd_AxManager) and the SS include
# tree, then for every function: OK/DIFF if its name is in symbols.txt, else look for fn_ symbols
# in the NW4R range with identical masked bytes (RENAME) or the same size (SIZE).
import sys, subprocess, re, struct, os
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection
os.chdir(os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', '..')))
args = sys.argv[1:]
apply = '--apply' in args; args = [a for a in args if a != '--apply']
useassert = '--assert' in args; args = [a for a in args if a != '--assert']
mw = 'Wii/1.0'
if '--mw' in args: i = args.index('--mw'); mw = args[i + 1]; del args[i:i + 2]
extra = []
if '--flags' in args: i = args.index('--flags'); extra = args[i + 1].split(); del args[i:i + 2]
src = args[0]
S = 'libs/ss_nw4r'
if not os.path.exists(src):
    for base in (S + '/src/', '/home/claude/ref_ss/src/nw4r/'):
        if os.path.exists(base + src): src = base + src; break
if src.startswith(os.environ.get('SS_REF', '/home/claude/ref_ss')):
    R = os.environ.get('SS_REF', os.environ.get('SS_REF', '/home/claude/ref_ss')); M = R + '/src/PowerPC_EABI_Support/MSL'
    inc = ['-i', R + '/include', '-i', M + '/MSL_C++/MSL_Common/Include', '-i', M + '/MSL_C/MSL_Common/Include',
           '-i', M + '/MSL_C/PPC_EABI/Include', '-i', M + '/MSL_C/MSL_Common_Embedded/Math/Include', '-i', R + '/src']
else:
  inc = ['-i', S + '/include', '-i', S + '/msl/MSL_C++/MSL_Common/Include', '-i', S + '/msl/MSL_C/MSL_Common/Include',
       '-i', S + '/msl/MSL_C/PPC_EABI/Include', '-i', S + '/msl/MSL_C/MSL_Common_Embedded/Math/Include']
fl = ['-nodefaults', '-proc', 'gekko', '-align', 'powerpc', '-enum', 'int', '-fp', 'hardware', '-Cpp_exceptions', 'off',
      '-O4,p', '-inline', 'auto', '-pragma', 'cats off', '-pragma', 'warn_notinlined off', '-maxerrors', '1',
      '-nosyspath', '-RTTI', 'off', '-str', 'reuse', '-enc', 'SJIS', '-DNDEBUG=1', '-ipa', 'file',
      '-fp_contract', 'off', '-func_align', '16', '-lang=c++'] + extra
if useassert: fl.append('-DNW4R_ASSERT')
out = '/tmp/sdkcheck_out.o'
if os.path.exists(out): os.remove(out)
r = subprocess.run(['build/tools/wibo', 'build/tools/sjiswrap.exe', 'build/compilers/%s/mwcceppc.exe' % mw] + fl + inc +
                   ['-c', src, '-o', out], capture_output=True, text=True)
if not os.path.exists(out):
    print((r.stdout + r.stderr)[-1500:]); sys.exit(1)
tsym = []; byname = {}
for l in open('config/SLSP01/symbols.txt'):
    m = re.match(r'(\S+) = \.text:0x([0-9A-F]+); // type:function size:0x([0-9A-F]+)', l)
    if m:
        a = int(m.group(2), 16)
        tsym.append((a, m.group(1), int(m.group(3), 16))); byname.setdefault(m.group(1), (a, int(m.group(3), 16)))
dol = open('orig/SLSP01/sys/main.dol', 'rb').read()
offs = struct.unpack('>18I', dol[0:72]); addrs = struct.unpack('>18I', dol[72:144]); sizes = struct.unpack('>18I', dol[144:216])
def rd(a, n):
    for o, ad, s in zip(offs, addrs, sizes):
        if ad <= a < ad + s: return dol[o + a - ad:o + a - ad + n]
lo, hi = int(os.environ.get('LO', '0x80703F30'), 16), int(os.environ.get('HI', '0x807263DC'), 16)
cands = [(a, n, s) for a, n, s in tsym if lo <= a < hi and (n.startswith('fn_') or n.startswith('auto_'))]
props = []; used = set(); nok = nbad = 0; amb = []; okaddr = []; anch = []
with open(out, 'rb') as f:
    e = ELFFile(f); relocs = {}
    for s in e.iter_sections():
        if isinstance(s, RelocationSection):
            for rr in s.iter_relocations(): relocs.setdefault(s['sh_info'], set()).add(rr['r_offset'] & ~3)
    st = e.get_section_by_name('.symtab')
    for sym in sorted(st.iter_symbols(), key=lambda s: (s['st_shndx'] if isinstance(s['st_shndx'], int) else 0, s['st_value'])):
        if sym['st_info']['type'] != 'STT_FUNC' or sym['st_size'] == 0: continue
        name = sym.name
        sec = e.get_section(sym['st_shndx']); data = sec.data()[sym['st_value']:sym['st_value'] + sym['st_size']]
        rel = relocs.get(sym['st_shndx'], set())
        def same(t):
            return all(sym['st_value'] + i in rel or data[i:i + 4] == t[i:i + 4] for i in range(0, len(data), 4))
        if name in byname:
            a, s = byname[name]
            ok = s == len(data) and same(rd(a, s))
            print('%-5s %08X %5X %s' % ('OK' if ok else 'DIFF', a, len(data), name[:110]))
            nok += ok; nbad += not ok
            if ok: okaddr.append(a); anch.append((sym['st_value'], a))
            continue
        hits = [(a, n) for a, n, s in cands if s == len(data) and same(rd(a, s)) and n not in used]
        if len(hits) == 1:
            props.append((hits[0][1], name, hits[0][0])); used.add(hits[0][1]); anch.append((sym['st_value'], hits[0][0]))
            print('RENAME %08X %5X %s' % (hits[0][0], len(data), name[:110]))
        elif len(hits) > 1:
            amb.append((name, hits, len(data), sym['st_info']['bind'] == 'STB_WEAK', sym['st_value']))
            print('AMBIG  %d %5X %s' % (len(hits), len(data), name[:110]))
        else:
            sz = [hex(a) for a, n, s in cands if s == len(data)]
            print('NONE  %5X %s %s' % (len(data), name[:100], ('size:' + ','.join(sz[:4])) if 0 < len(sz) <= 4 else ''))
for name, hits, sz, weak, off in amb:
    lower = [x for x in anch if x[0] < off]
    if not lower: continue
    ao, ta = max(lower)
    pred = ta + (off - ao)
    h = [x for x in hits if x[0] == pred and x[1] not in used]
    if len(h) == 1:
        props.append((h[0][1], name, h[0][0])); used.add(h[0][1]); print('RENAME* %08X %5X %s' % (h[0][0], sz, name[:110]))
print('ok %d diff %d rename %d' % (nok, nbad, len(props)))
used_src = subprocess.run(['grep', '-rhoE', r'\b(fn|lbl)_80[0-9A-F]{6}\b', 'src', 'include', 'libs'], capture_output=True, text=True).stdout.split()
used_src = set(used_src)
for n, new, a in props:
    if n in used_src: print('WARN used in source:', n, '->', new[:80])
if apply and props:
    al = []
    for n, new, a in props: al += [n, new]
    print(subprocess.run(['python3', 'tools/decomp/rensym.py'] + al, capture_output=True, text=True).stdout[-300:])
