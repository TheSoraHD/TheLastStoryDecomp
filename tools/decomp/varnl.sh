#!/bin/bash
# varnl.sh <unit> <symbol> <variants-file>: like var.py, but counts the diff lines of odiff2.py without @N labels (variants that add inlines renumber them); restores the source
cd "$(dirname "$0")/../.."
u=$1; s=$2; vf=$3
src=src/$u.cpp; cp $src /tmp/vg.orig
n=$(python3 -c "print(len([b for b in open('$vf').read().split('%%%') if b.strip()]))")
echo "base $(python3 tools/decomp/odiff2.py $u "$s" 2>/dev/null | grep -v '@' | grep -c .)"
for k in $(seq 1 $n); do
  python3 - $k "$vf" <<'P'
import sys
s=[b for b in open(sys.argv[2]).read().split('%%%\n') if b.strip()]
open('/tmp/vg1.txt','w').write(s[int(sys.argv[1])-1]+'%%%\n')
P
  timeout 900 python3 tools/decomp/var.py $u "$s" /tmp/vg1.txt --keep 1 >/dev/null 2>&1; rm -f $src.bak
  echo "$k $(python3 tools/decomp/odiff2.py $u "$s" 2>/dev/null | grep -v '@' | grep -c .)"
  cp /tmp/vg.orig $src
done
ninja build/SLSP01/src/$u.o >/dev/null 2>&1
