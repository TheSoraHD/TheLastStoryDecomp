#!/bin/bash
# pdbatch.sh <unidad>...: como pd.sh, pero para varias unidades con un solo dtk split.
# 1) compila y mide (fnstat) cada unidad; 2) pairdata --relabel --apply en todas seguidas (cada una
# toca solo las etiquetas de su propio rango, asi que no hace falta re-partir entre una y otra);
# 3) un configure + split; 4) mide otra vez y, si alguna baja, revierte solo sus lineas de
# symbols.txt y vuelve a partir (un segundo split solo si hubo reversiones).
# Rutas del entorno en la nube (docs/entorno-nube.md); ajustar con las variables B y T.
cd "$(dirname "$0")/../.."
B=${B:-/home/claude/tls-build}; T=${T:-/home/claude/tools}
conf() { python3 configure.py --compilers $B/build/compilers --dtk $T/dtk --objdiff $T/objdiff-cli --sjiswrap $B/build/tools/sjiswrap.exe --wrapper $T/wibo >/dev/null 2>&1; }
cnt() { python3 tools/decomp/fnstat.py $1 --nobuild 2>/dev/null | tail -1 | awk '{print $2}' | cut -d/ -f1; }
S=config/SLSP01/symbols.txt
W=$(mktemp -d)
cp $S $W/orig
declare -A before
for u in "$@"; do ninja build/SLSP01/src/$u.o >/dev/null 2>&1; before[$u]=$(cnt $u); done
for u in "$@"; do
  cp $S $W/pre
  python3 tools/decomp/pairdata.py $u --relabel --apply >/dev/null 2>&1
  diff $W/pre $S | grep '^>' | sed 's/^> //' > "$W/$(echo $u | tr / _).new"
done
if cmp -s $S $W/orig; then for u in "$@"; do echo "$u ${before[$u]} -> ${before[$u]} (sin cambios)"; done; rm -rf $W; exit 0; fi
conf
for u in "$@"; do ninja build/SLSP01/src/$u.o >/dev/null 2>&1; done
rev=0
for u in "$@"; do
  a=$(cnt $u); b=${before[$u]}
  if [ "$a" -lt "$b" ] 2>/dev/null; then
    # revert this unit's renamed lines: put back the original line with the same address
    python3 - "$S" "$W/orig" "$W/$(echo $u | tr / _).new" <<'E'
import sys, re
cur, orig, new = sys.argv[1:4]
addr = lambda l: (re.search(r'= (\S+:0x[0-9A-Fa-f]+);', l) or [None, None])[1]
o = {addr(l): l for l in open(orig) if addr(l)}
bad = {addr(l) for l in open(new) if addr(l)}
out = [o.get(addr(l), l) if addr(l) in bad else l for l in open(cur)]
open(cur, 'w').writelines(out)
E
    echo "$u $b -> $a REVERTED"; rev=1
  else
    echo "$u $b -> $a"
  fi
done
[ $rev = 1 ] && conf
rm -rf $W
