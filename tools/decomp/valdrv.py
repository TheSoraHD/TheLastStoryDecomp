#!/usr/bin/env python3
# valdrv.py <lo> <hi> <minsize[-maxsize]> <kinds> <tag> [mutsweep args]: runs mutsweep (kinds) over the functions
# of the game with fuzzy in [lo, hi) and size in [minsize, maxsize) from build/SLSP01/report.json of the copy $SW;
# log in /home/claude/scratch/<tag>.log (resumable), best variant of each function in
# /home/claude/scratch/diffs/<symbol>.diff (bloqueos-20261122, accesores-20261126).
# Hits: awk '{split($0,a,"-> "); split(a[2],b," "); if (b[1]+0 > $4+0.3) print}' /home/claude/scratch/<tag>.log
import json, re, subprocess, os, sys, time
lo, hi, sz_arg, kinds, tag = float(sys.argv[1]), float(sys.argv[2]), sys.argv[3], sys.argv[4], sys.argv[5]
mins, maxs = (int(sz_arg.split('-')[0]), int(sz_arg.split('-')[1])) if '-' in sz_arg else (int(sz_arg), 1 << 30)
extra = sys.argv[6:]
os.chdir(os.environ.get('SW', '/home/claude/scratch/sweep'))
os.makedirs('/home/claude/scratch/diffs', exist_ok=True)
r = json.load(open('build/SLSP01/report.json'))
skip = ('fn_8013CB68', 'fn_803729B4', 'fn_800B47AC', 'auto_', 'ut_TextWriterBase', 'Gecko', 'NETAES', 'ControlAppear',
        'game/CharaTask')
c = []
for u in r['units']:
    un = u['name'][5:]
    if not os.path.exists('src/%s.cpp' % un):
        continue
    if not re.match(r'(game|gmk|chr|atn|MP|Event|AI|db|ev|ui|map)/', un):
        continue
    for f in u.get('functions', []):
        p = f.get('fuzzy_match_percent', 0); sz = int(f.get('size', 0))
        dn = f.get('metadata', {}).get('demangled_name', '')
        if not (lo <= p < hi) or not (mins <= sz < maxs) or not dn or any(s in f['name'] or s in un for s in skip):
            continue
        fn = dn.split('(')[0]
        if '<' in fn or 'operator' in fn or '~' in fn:
            continue
        c.append((p, sz, un, f['name'], fn))
c.sort(key=lambda x: (-x[0]))
done = set()
lg = '/home/claude/scratch/%s.log' % tag
if os.path.exists(lg):
    for l in open(lg):
        done.add(l.split()[1] if len(l.split()) > 1 else '')
log = open(lg, 'a')
for p, sz, un, sym, fn in c:
    if sym in done:
        continue
    t = time.time()
    o = subprocess.run(['timeout', '300', 'python3', 'tools/decomp/mutsweep.py', un, sym, fn, '--only', kinds,
                        '--rounds', '3', '--time', '240', '--diff', '/home/claude/scratch/diffs/%s.diff' % sym] + extra,
                       capture_output=True, text=True).stdout
    m = re.search(r'best ([\d.]+)', o)
    b = m.group(1) if m else 'ERR'
    log.write('%s %s %s %.3f -> %s %ds\n' % (un, sym, sz, p, b, time.time() - t)); log.flush()
    subprocess.run(['git', 'checkout', '-q', 'src/'])
log.write('DONE\n'); log.flush()
