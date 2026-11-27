# stackonly.py lo hi: functions of the game whose objdiff differences are only stack offsets (r1) or labels
import json, subprocess, sys, re, os
lo, hi = float(sys.argv[1]), float(sys.argv[2])
r = json.load(open('build/SLSP01/report.json'))
units = {}
for u in r['units']:
    un = u['name'][5:]
    if not os.path.exists('src/%s.cpp' % un): continue
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
        if len(a) != len(b): continue
        bad = 0; stk = 0
        for x, y in zip(a, b):
            k = x.get('diff_kind', '') or y.get('diff_kind', '')
            if not k: continue
            fa = x.get('instruction', {}).get('formatted', ''); fb = y.get('instruction', {}).get('formatted', '')
            if '@' in fa or '@' in fb: continue
            if 'r1)' in fa or 'r1, ' in fa: stk += 1; continue
            bad += 1
        if bad == 0 and stk > 0:
            print('%-30s %-50s %5d %.3f stack-rows %d' % (un, name[:50], sz, p, stk)); sys.stdout.flush()
