#!/bin/bash
# mkref.sh: saves the reference of the current build in $REF (default /home/claude/ref) before starting a batch:
# report.json, the units with .o and split (units.txt), labelcheck of those units and rep100.
# Run it after a clean build of main (rm -rf build/SLSP01; configure; ninja).
cd "$(dirname "$0")/../.."
REF=${REF:-/home/claude/ref}
mkdir -p $REF
cp build/SLSP01/report.json $REF/report.json
python3 - > $REF/units.txt <<'P'
import re,os
sp=open('config/SLSP01/splits.txt').read()
for m in re.finditer(r'^(\S+)\.(cpp|c):\n',sp,re.M):
    u=m.group(1)
    if os.path.exists('build/SLSP01/src/%s.o'%u): print(u)
P
python3 tools/decomp/labelcheck.py $(cat $REF/units.txt) > $REF/labelcheck.txt 2>&1
python3 tools/decomp/rep100.py > $REF/rep100.txt 2>&1
echo "ref: $(wc -l < $REF/units.txt) units, labelcheck $(wc -l < $REF/labelcheck.txt) lines, rep100 $(tail -1 $REF/rep100.txt)"
