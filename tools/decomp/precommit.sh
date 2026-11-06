#!/bin/bash
# precommit.sh [--pd] <units...>: every check before a commit, against the reference of mkref.sh ($REF).
#   --pd: first relabels the data of the units (pd.sh for one, pdbatch.sh for several).
# Then: ninja (main.dol: OK), regress (lost), fuzzdrop, ordercheck, mnemcheck, poolcheck of the units, and
# labelcheck + rep100 against the reference. Last line: OK or FAIL with the reasons.
cd "$(dirname "$0")/../.."
REF=${REF:-/home/claude/ref}
if [ "$1" = "--pd" ]; then
    shift
    if [ $# -eq 1 ]; then timeout 590 tools/decomp/pd.sh "$1" | tail -1; else timeout 590 tools/decomp/pdbatch.sh "$@" | tail -$#; fi
fi
fail=""
rm -f build/SLSP01/main.dol
timeout 590 ninja > /tmp/precommit_ninja.log 2>&1
grep -q "main.dol: OK" /tmp/precommit_ninja.log && echo "main.dol: OK" || { grep -E "FAILED|error" /tmp/precommit_ninja.log | head -5; fail="$fail ninja"; }
r=$(python3 tools/decomp/regress.py build/SLSP01/report.json $REF/report.json | tail -2); echo "$r"
echo "$r" | grep -q "lost: 0" || fail="$fail regress"
f=$(python3 tools/decomp/fuzzdrop.py build/SLSP01/report.json $REF/report.json | tail -1); echo "$f"
echo "$f" | grep -q "drops: 0" || fail="$fail fuzzdrop"
for u in "$@"; do
    o=$(python3 tools/decomp/ordercheck.py $u | grep "out of order"); echo "$o"
    echo "$o" | grep -q " 0 out of order" || fail="$fail ordercheck:$u"
    m=$(python3 tools/decomp/mnemcheck.py $u | grep -v "^$"); [ -n "$m" ] && { echo "$m" | head -3; fail="$fail mnemcheck:$u"; }
done
if [ $# -gt 0 ]; then
    p=$(python3 tools/decomp/poolcheck.py "$@" | grep -v "units with findings: 0")
    [ -n "$p" ] && { echo "$p" | head -4; echo "(poolcheck: compare with the known warnings)"; }
fi
python3 tools/decomp/labelcheck.py $(cat $REF/units.txt) > /tmp/precommit_lc.txt 2>&1
python3 tools/decomp/rep100.py > /tmp/precommit_rep.txt 2>&1
python3 - "$REF" <<'P' || fail="$fail labelcheck/rep100"
import sys
ref=sys.argv[1]
def load(f): return set(l.strip() for l in open(f) if l.strip())
def units(s): return {x.split()[0]: x for x in s}
a=units(load(ref+'/labelcheck.txt')); b=units(load('/tmp/precommit_lc.txt'))
worse=[]
for u,x in b.items():
    nb=int(x.split()[-1]); na=int(a[u].split()[-1]) if u in a else 0
    if nb>na: worse.append(x)
ra=load(ref+'/rep100.txt'); rb=load('/tmp/precommit_rep.txt')
newr=[x for x in rb-ra if not x.startswith('REP total') and not x.startswith('total')]
for x in sorted(worse): print('LC+',x)
for x in sorted(newr): print('REP+',x)
print('labelcheck/rep100:', 'peor' if (worse or newr) else 'igual')
sys.exit(1 if (worse or newr) else 0)
P
python3 -c "import json;m=json.load(open('build/SLSP01/report.json'))['measures'];print('funcs',m['matched_functions'],'units',m['complete_units'])"
[ -z "$fail" ] && echo "PRECOMMIT OK" || echo "PRECOMMIT FAIL:$fail"
