# fixbad.py <unit> [--apply]: for each BAD label of lblval, rename the symbols.txt entry (among same-named ones)
# whose bytes in the original DOL equal our label's value... picks entry by DOL value.
import sys, struct, re, subprocess
sys.argv_save = list(sys.argv)
unit = sys.argv[1]; apply = '--apply' in sys.argv
src = open(__import__('os').path.join(__import__('os').path.dirname(__file__), 'lblval.py')).read().split('for u in sys.argv')[0]
exec(src)
dol = open('orig/SLSP01/sys/main.dol', 'rb').read()
offs = struct.unpack('>18I', dol[0:72]); addrs = struct.unpack('>18I', dol[72:144]); sizes = struct.unpack('>18I', dol[144:216])
def rd(a, n=4):
    for o, ad, sz in zip(offs, addrs, sizes):
        if ad <= a < ad + sz:
            return dol[o + a - ad:o + a - ad + n]
a = syms('build/SLSP01/obj/%s.o' % unit); b = syms('build/SLSP01/src/%s.o' % unit)
lines = open('config/SLSP01/symbols.txt').read().split('\n')
bad = [n for n in a if n in b and a[n][1] != b[n][1] and a[n][0] == b[n][0]]
used = set(a)
for n in bad:
    # candidates in symbols with name n
    idx = [i for i, l in enumerate(lines) if l.startswith(n + ' = ')]
    for i in idx:
        m = re.match(r'\S+ = (\.\w+):0x([0-9A-Fa-f]+)', lines[i]); ad = int(m.group(2), 16)
        v = rd(ad)
        if v == b[n][1][:4]:
            continue  # this one already right
        # find our label with value v not in orig names
        c = [k for k, x in b.items() if x[1][:4] == v and x[0] == m.group(1) and k not in a]
        print(n, hex(ad), v.hex(), 'cand', c)
        if apply and len(c) == 1:
            lines[i] = c[0] + lines[i][len(n):]
if apply:
    open('config/SLSP01/symbols.txt', 'w').write('\n'.join(lines))
