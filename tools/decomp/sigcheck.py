#!/usr/bin/env python3
"""sigcheck.py: extern "C" fn_XXXXXXXX declared with different argument classes in different files.

For the PC port: on the Wii, ints/pointers and floats go in separate register files, so a declaration
that moves a float among the ints, or uses double for float, still compiles to the same code; on x86-64
(Windows above all) it does not. This lists the functions whose declarations disagree in the sequence of
argument classes (I = int-sized, P = pointer/reference, F = float, D = double, S = struct by value,
V = variadic) or in the return class. A file marked * defines the function (its signature is the
reference).

usage: python3 tools/decomp/sigcheck.py [--all] [--func fn_XXXXXXXX]
  default: only mismatches that matter on x86-64 (pointer vs int, float vs double, a float moved among
  the ints, different argument count, different non-void returns; void against a value is ignored); --all also lists pure type-name differences.
"""
import collections
import glob
import re
import sys

INTS = {'int', 'unsigned int', 'long', 'unsigned long', 'short', 'unsigned short', 'char', 'unsigned char',
        'signed char', 'bool', 'unsigned', 'signed', 'u8', 'u16', 'u32', 's8', 's16', 's32', 'BOOL',
        'wchar_t', '__msl_size_t', 'size_t'}


def split_args(args):
    out, depth, cur = [], 0, ''
    for ch in args:
        if ch in '(<':
            depth += 1
        elif ch in ')>':
            depth -= 1
        if ch == ',' and depth == 0:
            out.append(cur)
            cur = ''
        else:
            cur += ch
    if cur.strip():
        out.append(cur)
    return [a.strip() for a in out]


def arg_class(a):
    a = re.sub(r'/\*.*?\*/', '', a).strip()
    if a == '...':
        return 'V'
    if a in ('', 'void'):
        return None
    if '(*' in a or '[' in a:
        return 'P'
    a = re.sub(r'\b(const|volatile|struct|class|enum)\b', ' ', a)
    a = re.sub(r'\s+', ' ', a).strip()
    if '*' in a or '&' in a:
        return 'P'
    # drop the parameter name (last identifier after a type)
    m = re.match(r'^(.*\S)\s+[A-Za-z_]\w*$', a)
    t = m.group(1) if m else a
    t = t.strip()
    if t == 'float' or t == 'f32':
        return 'F'
    if t == 'double' or t == 'f64':
        return 'D'
    if t in INTS or t.startswith('unsigned ') or t.startswith('signed '):
        return 'I'
    if t in ('long long', 'unsigned long long', 's64', 'u64'):
        return 'L'
    if re.search(r'(Func|Callback|Ptr)(_unk)?$', t):
        return 'P'  # typedef of a function or data pointer (MindowsFunc_unk, MtxPtr...)
    return 'S:' + t.split('::')[-1]


def ret_class(r):
    r = re.sub(r'\b(inline|static|const|struct|class|virtual)\b', ' ', r)
    r = re.sub(r'\s+', ' ', r).strip()
    if r == 'void':
        return 'v'
    return arg_class(r + ' x') or 'v'


def main():
    show_all = '--all' in sys.argv
    only = None
    if '--func' in sys.argv:
        only = sys.argv[sys.argv.index('--func') + 1]
    decl = collections.defaultdict(lambda: collections.defaultdict(list))
    pat = re.compile(r'^\s*extern "C" (?:inline )?([^\n;(]*?)\b(fn_[0-9A-F]{8})\(([^;{]*?)\)\s*[;{]', re.M | re.S)
    files = sorted(glob.glob('src/**/*.cpp', recursive=True) + glob.glob('include/**/*.h', recursive=True))
    for f in files:
        s = open(f, encoding='utf-8', errors='replace').read()
        s = re.sub(r'//[^\n]*', '', s)
        for m in pat.finditer(s):
            ret, name, args = m.groups()
            if only and name != only:
                continue
            if '=' in ret or '"' in ret:
                continue
            cls = tuple(c for c in (arg_class(a) for a in split_args(re.sub(r'\s+', ' ', args))) if c)
            key = (ret_class(ret), cls)
            isdef = s[m.end() - 1] == '{'
            decl[name][key].append(f + ('*' if isdef else ''))
    n = 0
    for name in sorted(decl):
        v = decl[name]
        if len(v) < 2:
            continue
        keys = list(v)
        lens = {len(k[1]) for k in keys}
        rets = {k[0] for k in keys}

        def abi(k):
            # what x86-64 sees: the sequence with struct types folded into S
            return tuple(c[:1] for c in k[1])
        # a void declaration of a function that returns a value is harmless (the value is ignored)
        nonvoid = {k[0][:1] for k in keys if k[0] != 'v'}
        relevant = len({abi(k) for k in keys}) > 1 or len(nonvoid) > 1
        if not relevant and not show_all:
            continue
        n += 1
        why = []
        if len(lens) > 1:
            why.append('argument count')
        if len(nonvoid) > 1:
            why.append('return')
        if not why:
            why.append('argument classes' if relevant else 'type names')
        print('%s (%s):' % (name, ', '.join(why)))
        for k in sorted(keys, key=lambda k: -len(v[k])):
            fs = v[k]
            print('    %-4d %s(%s)  %s' % (len(fs), k[0], ' '.join(k[1]), ' '.join(sorted(set(fs))[:3])
                                         + (' ...' if len(set(fs)) > 3 else '')))
    print('# %d functions' % n)


if __name__ == '__main__':
    main()
