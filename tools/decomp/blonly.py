# blonly.py lo hi: functions whose differences include call-target name mismatches (bl A | bl B)
import json, subprocess, sys, os
lo, hi = float(sys.argv[1]), float(sys.argv[2])
r = json.load(open('build/SLSP01/report.json'))
units = {}
for u in r['units']:
    un = u['name'][5:]
    if not os.path.exists('src/%s.cpp' % un) and not os.path.exists('src/%s.c' % un): continue
    for f in u.get('functions', []):
        p = f.get('fuzzy_match_percent', 0)
        if lo <= p < hi: units.setdefault(un, []).append((f['name'], p, int(f.get('size', 0))))
for un, fs in sorted(units.items()):
    o = subprocess.run(['build/tools/objdiff-cli', 'diff', '-p', '.', '-u', 'main/' + un, '-o', '-', '--format', 'json-pretty'], capture_output=True, text=True).stdout
    try: d = json.loads(o)
    except Exception: continue
    L = {s['name']: s for s in d.get('left', {}).get('symbols', [])}
    R = {s['name']: s for s in d.get('right', {}).get('symbols', [])}
    for name, p, sz in fs:
        if name not in L or name not in R: continue
        a = L[name].get('instructions', []); b = R[name].get('instructions', [])
        bls = []; other = 0
        for x, y in zip(a, b):
            k = x.get('diff_kind', '') or y.get('diff_kind', '')
            if not k: continue
            fa = x.get('instruction', {}).get('formatted', ''); fb = y.get('instruction', {}).get('formatted', '')
            if fa.startswith('bl ') and fb.startswith('bl ') and fa != fb: bls.append((fa[3:], fb[3:]))
            else: other += 1
        if bls:
            print('%-28s %-44s %5d %.3f other %d: %s' % (un, name[:44], sz, p, other, ' ; '.join('%s|%s' % (x[:40], y[:40]) for x, y in bls[:2]))); sys.stdout.flush()
