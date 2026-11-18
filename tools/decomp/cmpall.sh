#!/bin/bash
# cmpall.sh <unit> <sed-expr>: apply one change of form to the whole src/<unit>.cpp at once (G5), build only the unit
# and print the functions whose percent changes (before -> after). Restores the file unless KEEP=1.
# Run from the root of your copy.
u=$1; e=$2; s=src/$u.cpp
cp $s /tmp/cmpall.$$.bak
ninja -j1 build/SLSP01/src/$u.o >/dev/null 2>&1; bash tools/decomp/pct.sh $u > /tmp/cmpall.$$.a
sed -i -E "$e" $s; n=$(diff /tmp/cmpall.$$.bak $s | grep -c '^>'); echo "lineas cambiadas: $n"
if ninja -j1 build/SLSP01/src/$u.o >/tmp/cmpall.$$.log 2>&1; then bash tools/decomp/pct.sh $u > /tmp/cmpall.$$.b
 join -1 2 -2 2 <(sort -k2 /tmp/cmpall.$$.a) <(sort -k2 /tmp/cmpall.$$.b) | awk '$2!=$3{print $2" -> "$3"  "$1}'
else echo BUILD FAIL; grep -m3 -i error /tmp/cmpall.$$.log; fi
[ "$KEEP" = 1 ] || { cp /tmp/cmpall.$$.bak $s; ninja -j1 build/SLSP01/src/$u.o >/dev/null 2>&1; }
rm -f /tmp/cmpall.$$.*
