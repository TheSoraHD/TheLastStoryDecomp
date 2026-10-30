#!/usr/bin/env python3
"""Compare the ORDER of the strings in @stringBase0 against the original split objects.

poolcheck.py only checks that our literals exist somewhere in the original; MWCC emits the pool in
the order in which it first meets each literal, so the same strings in another order shift every
`addi rX, rY, @stringBase0@l + N` and keep otherwise perfect functions below 100%.

usage: poolorder.py [--all] [--funcs] [substring ...]   (run from the repo root after a build)

For every unit with both build/SLSP01/obj/<unit>.o (original) and build/SLSP01/src/<unit>.o (ours),
or only those whose path contains one of the substrings, it prints one of:
  same            identical list of strings (trailing alignment padding ignored)
  order  @i       same strings (as a multiset) in another order; i = first differing index
  content @i      different strings; i = first differing index, then missing/extra counts
  no-pool         the original has no @stringBase0 (or ours has none)
Only the `order`/`content` units are printed unless --all is given.

--funcs: for the units in `order`/`content`, list the functions < 100% (build/SLSP01/report.json)
whose only differing instructions in objdiff are references to @stringBase0 (offsets that move with
the order of the pool), i.e. functions that would match once the pool is fixed (`only`), and
how many have pool offsets among other differences (`mixed`, listed with --mixed).
"""
import glob, json, os, subprocess, sys
from collections import Counter
from elftools.elf.elffile import ELFFile

ROOT = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..'))


def pool(path, exact=False):
    """Return the list of strings of @stringBase0 (None if the object has none). With exact (our
    objects, whose symbol size is exact) only the terminator of the last string is dropped; the
    split objects of the original include the alignment padding, see trim()."""
    with open(path, 'rb') as f:
        e = ELFFile(f)
        st = e.get_section_by_name('.symtab')
        if st is None:
            return None
        for s in st.iter_symbols():
            if s.name == '@stringBase0':
                sec = e.get_section(s['st_shndx'])
                if sec.header['sh_type'] == 'SHT_NOBITS':
                    return None
                d = sec.data()[s['st_value']:s['st_value'] + s['st_size']]
                parts = d.split(b'\0')
                if exact:
                    if parts and parts[-1] == b'':
                        parts.pop()
                else:
                    # the pool ends with the terminator of its last string plus alignment padding
                    while parts and parts[-1] == b'':
                        parts.pop()
                return parts
    return None


def compare(a, b):
    if a is None or b is None:
        return 'no-pool', None
    if a == b:
        return 'same', None
    i = next((k for k in range(min(len(a), len(b))) if a[k] != b[k]), min(len(a), len(b)))
    if Counter(a) == Counter(b):
        return 'order', i
    return 'content', i


def report_funcs():
    r = json.load(open(os.path.join(ROOT, 'build/SLSP01/report.json')))
    out = {}
    for u in r['units']:
        name = u['name'].split('/', 1)[1] if '/' in u['name'] else u['name']
        out[name] = [(f['name'], f.get('fuzzy_match_percent') or 0.0) for f in u.get('functions', [])]
    return out


def pool_only_diff(unit, sym):
    """'only' if every differing instruction of sym is a reference to @stringBase0 on both sides,
    'mixed' if some are and others are not, None otherwise."""
    p = subprocess.run([os.path.join(ROOT, 'build/tools/objdiff-cli'), 'diff', '-p', ROOT, '-u', 'main/' + unit,
                        sym, '-o', '-', '--format', 'json'], capture_output=True, text=True)
    try:
        d = json.loads(p.stdout)
    except ValueError:
        return False
    rows, syms = {}, {}
    for side in ('left', 'right'):
        syms[side] = d.get(side, {}).get('symbols', [])
        for s in syms[side]:
            if s.get('name') == sym:
                rows[side] = s.get('instructions', [])
    L, R = rows.get('left'), rows.get('right')
    if not L or not R or len(L) != len(R):
        return None

    # objdiff marks `addi rX, rBase, N` on a register holding @stringBase0 with a synthetic
    # R_PPC_NONE relocation (addend N) whose target_symbol is the pool's index on that side
    pidx = {}
    for side, rows_ in (('left', L), ('right', R)):
        for x in rows_:
            ins = x.get('instruction') or {}
            if '@stringBase0' in ins.get('formatted', '') and ins.get('relocation'):
                pidx[side] = ins['relocation'].get('target_symbol')
                break
    if len(pidx) != 2:
        return None

    def strip(ins):
        return [q for q in ins.get('parts', []) if not ('arg' in q and ('signed' in q['arg'] or 'unsigned' in q['arg']))]

    npool = nother = 0
    for a, b in zip(L, R):
        if not (a.get('diff_kind') or b.get('diff_kind')):
            continue
        ia, ib = a.get('instruction') or {}, b.get('instruction') or {}
        ra, rb = ia.get('relocation') or {}, ib.get('relocation') or {}
        if ra.get('target_symbol') == pidx['left'] and rb.get('target_symbol') == pidx['right'] and \
                strip(ia) == strip(ib):
            npool += 1
        else:
            nother += 1
    if not npool:
        return None
    return 'mixed' if nother else 'only'



def main():
    args = sys.argv[1:]
    show_all = '--all' in args
    funcs = '--funcs' in args
    pats = [a for a in args if not a.startswith('--')]
    os.chdir(ROOT)
    rep = report_funcs() if funcs else {}
    counts = Counter()
    for orig in sorted(glob.glob('build/SLSP01/obj/**/*.o', recursive=True)):
        rel = os.path.relpath(orig, 'build/SLSP01/obj')
        if pats and not any(p in rel for p in pats):
            continue
        ours = os.path.join('build/SLSP01/src', rel)
        if not os.path.exists(ours):
            continue
        a, b = pool(orig), pool(ours, exact=True)
        if a is not None and b is not None:
            # "" literals at the end of our pool: the original keeps as many as fit in its padding
            k = 0
            while k < len(b) and b[len(b) - 1 - k] == b'':
                k += 1
            if k and len(a) == len(b) - k:
                a = a + [b''] * k
        kind, i = compare(a, b)
        counts[kind] += 1
        if kind in ('same', 'no-pool') and not show_all:
            continue
        unit = os.path.splitext(rel)[0]
        msg = '%-8s %s' % (kind, unit)
        if kind in ('order', 'content'):
            msg += '  @%d/%d' % (i, len(a))
            if i < len(a):
                msg += '  orig=%r' % a[i][:40].decode('latin-1')
            if i < len(b):
                msg += ' ours=%r' % b[i][:40].decode('latin-1')
            if kind == 'content':
                ca, cb = Counter(a), Counter(b)
                msg += '  missing=%d extra=%d' % (sum((ca - cb).values()), sum((cb - ca).values()))
        if funcs and kind in ('order', 'content'):
            cand = [f for f, pct in rep.get(rel, rep.get(unit + '.cpp', rep.get(unit, []))) if 0.0 < pct < 100.0]
            res = [(f, pool_only_diff(unit, f)) for f in cand]
            good = [f for f, k in res if k == 'only']
            mixed = [f for f, k in res if k == 'mixed']
            msg += '  funcs<100=%d pool-only=%d mixed=%d' % (len(cand), len(good), len(mixed))
            for f in good:
                msg += '\n    only  ' + f
            if '--mixed' in args:
                for f in mixed:
                    msg += '\n    mixed ' + f
        print(msg, flush=True)
    print('summary:', ' '.join('%s=%d' % kv for kv in sorted(counts.items())))


if __name__ == '__main__':
    main()
