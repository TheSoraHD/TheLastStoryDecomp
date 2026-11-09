#!/usr/bin/env python3
# declperm.py <unit> <symbol> <start> <end> [--apply]
# Register order of locals: MWCC gives the volatile registers in the order the variables are declared.
# Takes the lines start..end (1-based, inclusive) of src/<unit>.cpp, which must all be
# "<type> <name> = <expr>;" declarations, splits them into uninitialized declarations followed by the
# assignments (same order as now) and tries every order of the declarations, building only the unit
# and printing the match percent of <symbol>. With --apply it writes the best order (if it is 100%).
import itertools, os, re, subprocess, sys

unit, sym, start, end = sys.argv[1], sys.argv[2], int(sys.argv[3]), int(sys.argv[4])
apply = '--apply' in sys.argv
src = 'src/%s.cpp' % unit
orig = open(src).read()
lines = orig.split('\n')
block = lines[start - 1:end]
decls = []
for l in block:
    m = re.match(r'^(\s*)((?:const\s+)?[\w:<>]+(?:\s*\*+)?)\s+(\w+)\s*=\s*(.*);\s*$', l)
    if not m:
        sys.exit('not a declaration: ' + l)
    decls.append(m.groups())
indent = decls[0][0]


def pct():
    import json
    out = subprocess.run(['build/tools/objdiff-cli', 'diff', '-p', '.', '-u', 'main/' + unit, sym, '-o', '-',
                          '--format', 'json-pretty'], capture_output=True, text=True).stdout
    for s in json.loads(out)['right']['symbols']:
        if s.get('name') == sym:
            return float(s.get('match_percent') or 0)
    return -1.0


best = (-1, None)
try:
    for perm in itertools.permutations(range(len(decls))):
        new = ['%s%s %s;' % (indent, decls[i][1], decls[i][2]) for i in perm]
        new += ['%s%s = %s;' % (indent, d[2], d[3]) for d in decls]
        open(src, 'w').write('\n'.join(lines[:start - 1] + new + lines[end:]))
        r = subprocess.run(['ninja', 'build/SLSP01/src/%s.o' % unit], capture_output=True, text=True)
        p = pct() if r.returncode == 0 else -2.0
        names = [decls[i][2] for i in perm]
        print('%-10s %s' % (p, ' '.join(names)), flush=True)
        if p > best[0]:
            best = (p, perm)
        if p == 100.0:
            break
finally:
    if apply and best[0] == 100.0:
        perm = best[1]
        new = ['%s%s %s;' % (indent, decls[i][1], decls[i][2]) for i in perm]
        new += ['%s%s = %s;' % (indent, d[2], d[3]) for d in decls]
        open(src, 'w').write('\n'.join(lines[:start - 1] + new + lines[end:]))
    else:
        open(src, 'w').write(orig)
    subprocess.run(['ninja', 'build/SLSP01/src/%s.o' % unit], capture_output=True)
print('best', best[0], [decls[i][2] for i in best[1]] if best[1] else None)
