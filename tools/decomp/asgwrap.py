#!/usr/bin/env python3
# asgwrap.py <unit> <symbol> [--apply]
# Tries the "extra inline level" fix of the __VecCopy r3/r4 swap: wraps each plain assignment
# "lhs = rhs;" of the function in AsgW_unk(lhs, rhs) (template inline inserted before the function),
# one at a time, then greedily accumulates the ones that improve. Prints the percentages.
# Without --apply the source is always restored. With --apply the best set is left in place
# Default: atn::VecAssign_unk of include/math/VecAssign_unk.h (the include is added if missing; only for
# atn::Vector3: other types do not compile and are skipped). --template: the old template AsgW_unk.
# --name N --type T: named non-template inline (reused if already in the file); --only=i,j: build just that set.
import re, subprocess, sys, signal

unit, sym = sys.argv[1], sys.argv[2]
apply = '--apply' in sys.argv
path = 'src/%s.cpp' % unit
orig = open(path, encoding='latin-1').read()
method, mang = sym.split('__', 1)
# last class name of the mangled scope (Q<n><len><name>... or <len><name>)
if mang.startswith('Q'):
    n = int(mang[1]); k = 2; names = []
    for _ in range(n):
        l = re.match(r'\d+', mang[k:]).group(0); k += len(l); names.append(mang[k:k + int(l)]); k += int(l)
else:
    l = re.match(r'\d+', mang).group(0); names = [mang[len(l):len(l) + int(l)]]
cls = names[-1]


def restore(*a):
    open(path, 'w', encoding='latin-1').write(orig)
    subprocess.run(['ninja', 'build/SLSP01/src/%s.o' % unit], capture_output=True)
    if a:
        sys.exit(1)


signal.signal(signal.SIGTERM, restore)

# function definition: a line at column 0 containing "::method(" and ending with "{"
m = None
for mm in re.finditer(r'^[^\n;]*\b%s::%s\([^\n]*\{\s*$' % (re.escape(cls), re.escape(method)), orig, re.M):
    m = mm
    break
if not m:
    print('definition not found'); sys.exit(1)
start = m.start()
defstart = start
# move above the comment block of the function (and remember a NONMATCHING block to drop at 100%)
lines_before = orig[:start].split('\n')[:-1]
k = len(lines_before)
while k > 0 and lines_before[k - 1].startswith('//'):
    k -= 1
cblock = '\n'.join(lines_before[k:]) + '\n' if k < len(lines_before) else ''
start = start - len(cblock)
# body end: matching brace
i = orig.index('{', m.start()); depth = 0
for j in range(i, len(orig)):
    if orig[j] == '{': depth += 1
    elif orig[j] == '}':
        depth -= 1
        if depth == 0: end = j; break
body = orig[i:end]
asg = []
for a in re.finditer(r'^(\s*)([A-Za-z_][\w\.\->\[\]\(\)\*]*)\s=\s([^;]+);', body, re.M):
    lhs = a.group(2)
    if '(' in lhs and not lhs.startswith('('):
        continue
    asg.append((i + a.start(2), i + a.end(0), lhs, a.group(3)))
# declarations with an initializer of a Vector3: "atn::Vector3 v = expr;" -> "atn::Vector3 v; W(v, expr);"
for a in re.finditer(r'^(\s*)((?:const )?(?:atn::)?Vector3) (\w+) = ([^;]+);', body, re.M):
    asg.append((i + a.start(2), i + a.end(0), a.group(3), a.group(4), a.group(2), a.group(1).lstrip('\n')))
# --ops: also x++; x--; ++x; --x; and x op= y; through template inlines (OpInc_unk, OpAdd_unk...)
OPS = {'+=': 'OpAdd_unk', '-=': 'OpSub_unk', '*=': 'OpMul_unk', '/=': 'OpDiv_unk', '|=': 'OpOr_unk',
       '&=': 'OpAnd_unk', '++': 'OpInc_unk', '--': 'OpDec_unk'}
if '--ops' in sys.argv or '--opsonly' in sys.argv:
    for a in re.finditer(r'^(\s*)([A-Za-z_][\w\.\->\[\]\(\)\*]*)\s*(\+=|-=|\*=|/=|\|=|&=)\s*([^;]+);', body, re.M):
        asg.append((i + a.start(2), i + a.end(0), a.group(2), a.group(4), a.group(3), None, 'op'))
    for a in re.finditer(r'^(\s*)(?:(\+\+|--)([A-Za-z_][\w\.\->\[\]\(\)\*]*)|([A-Za-z_][\w\.\->\[\]\(\)\*]*)(\+\+|--));', body, re.M):
        lhs = a.group(3) or a.group(4); op = a.group(2) or a.group(5)
        asg.append((i + a.start(0) + len(a.group(1)), i + a.end(0), lhs, '', op, None, 'op'))
if '--opsonly' in sys.argv:
    asg = [a for a in asg if len(a) == 7]
if '--declonly' in sys.argv:
    asg = [a for a in asg if len(a) == 6]
print('assignments', len(asg))
NAME, TYPE = 'AsgW_unk', None
VECDEF = '--template' not in sys.argv and '--name' not in sys.argv and '--type' not in sys.argv
if VECDEF: NAME = 'atn::VecAssign_unk'
for k, a in enumerate(sys.argv):
    if a == '--name': NAME = sys.argv[k + 1]
    if a == '--type': TYPE = sys.argv[k + 1]
ONLY = None
for a in sys.argv:
    if a.startswith('--only='): ONLY = [int(x) for x in a[7:].split(',')]
if VECDEF:
    DECL = ''
elif TYPE:
    DECL = ('// inferred: copy through one more inline level (fixes the r3/r4 swap of the __VecCopy, see trucos-mwcc.md)\n'
            'static inline void %s(%s& dst, const %s& src) {\n    dst = src;\n}\n\n' % (NAME, TYPE, TYPE))
else:
    DECL = 'template <class T> static inline void %s(T& dst, const T& src) { dst = src; }\n\n' % NAME
if re.search(r'\b%s\(' % re.escape(NAME), orig):
    DECL = ''


drop = False


def build(sel):
    s = orig
    for k in sorted(sel, key=lambda k: asg[k][0], reverse=True):
        p0, p1, lhs, rhs = asg[k][:4]
        if len(asg[k]) == 7:
            op = asg[k][4]
            if op in ('++', '--'):
                s = s[:p0] + '%s(%s);' % (OPS[op], lhs) + s[p1:]
            else:
                s = s[:p0] + '%s(%s, %s);' % (OPS[op], lhs, rhs) + s[p1:]
            continue
        if len(asg[k]) == 6:
            s = s[:p0] + '%s %s;\n%s%s(%s, %s);' % (asg[k][4].replace('const ', ''), lhs, asg[k][5], NAME, lhs, rhs) + s[p1:]
        else:
            s = s[:p0] + '%s(%s, %s);' % (NAME, lhs, rhs) + s[p1:]
    if any(len(asg[k]) == 7 for k in sel):
        opdecl = ''
        for op, nm in OPS.items():
            if re.search(r'\b%s\(' % nm, s) and not re.search(r'inline \w+ %s\(' % nm, s):
                if op in ('++', '--'):
                    opdecl += 'template <class T> static inline void %s(T& v) { v%s; }\n' % (nm, op)
                else:
                    opdecl += 'template <class T, class U> static inline void %s(T& v, const U& x) { v %s x; }\n' % (nm, op)
        DECLX = opdecl + '\n'
    else:
        DECLX = ''
    if drop and '// NONMATCHING' in cblock:
        kept, skip = [], False
        for l in cblock.split('\n')[:-1]:
            if l.startswith('// NONMATCHING'): skip = True
            elif l.startswith('// inferred') or l.startswith('// DRAFT'): skip = False
            if not skip: kept.append(l)
        s = s[:start] + DECLX + DECL + ''.join(l + '\n' for l in kept) + s[defstart:]
    else:
        s = s[:start] + DECLX + DECL + s[start:]
    if VECDEF and '"math/VecAssign_unk.h"' not in s:
        m = list(re.finditer(r'^#include [^\n]*\n', s[:start], re.M))[-1]
        s = s[:m.end()] + '#include "math/VecAssign_unk.h"\n' + s[m.end():]
    open(path, 'w', encoding='latin-1').write(s)
    r = subprocess.run(['ninja', 'build/SLSP01/src/%s.o' % unit], capture_output=True)
    if r.returncode:
        return None
    out = subprocess.run(['tools/decomp/pct.sh', unit, sym], capture_output=True, text=True).stdout.split()
    return float(out[0]) if out else None


try:
    if ONLY is not None:
        v = build(ONLY)
        if apply and v == 100.0 and '// NONMATCHING' in cblock:
            drop = True; v = build(ONLY)
        print('only', ONLY, v)
        if v != 100.0 and '--keep' not in sys.argv: restore()
        if not apply: restore()
        sys.exit(0)
    base = build([])
    print('base', base)
    single = []
    for k in range(len(asg)):
        v = build([k])
        print(k, v, asg[k][2], '=', asg[k][3][:50], flush=True)
        if v is not None and v > base:
            single.append((v, k))
    single.sort(reverse=True)
    cur, best = [], base
    for v, k in single:
        t = build(cur + [k])
        if t is not None and t > best:
            cur, best = cur + [k], t
    print('best', best, cur)
    if apply and cur and best > base:
        build(cur)
        print('applied')
    else:
        restore()
except SystemExit:
    raise
except BaseException:
    restore()
    raise
