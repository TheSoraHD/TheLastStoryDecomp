#!/bin/bash
# pd.sh <unidad>: re-empareja las etiquetas de datos de la unidad (pairdata --relabel --apply) y
# revierte symbols.txt si baja el numero de funciones al 100% (fnstat).
# Un solo dtk split (antes: resetlocal + pairdata = dos splits de todo el .dol, ~1 min cada uno).
# Rutas del entorno en la nube (docs/entorno-nube.md); ajustar con las variables B y T.
cd "$(dirname "$0")/../.."
B=${B:-/home/claude/tls-build}; T=${T:-/home/claude/tools}
u=$1
conf() { python3 configure.py --compilers $B/build/compilers --dtk $T/dtk --objdiff $T/objdiff-cli --sjiswrap $B/build/tools/sjiswrap.exe --wrapper $T/wibo >/dev/null 2>&1; }
cp config/SLSP01/symbols.txt /tmp/pd_sym.bak
ninja build/SLSP01/src/$u.o >/dev/null 2>&1
b=$(python3 tools/decomp/fnstat.py $u --nobuild 2>/dev/null | tail -1 | awk '{print $2}' | cut -d/ -f1)
python3 tools/decomp/pairdata.py $u --relabel --apply >/dev/null 2>&1
if cmp -s config/SLSP01/symbols.txt /tmp/pd_sym.bak; then echo "$u $b -> $b (sin cambios)"; exit 0; fi
conf; ninja build/SLSP01/src/$u.o >/dev/null 2>&1
a=$(python3 tools/decomp/fnstat.py $u --nobuild 2>/dev/null | tail -1 | awk '{print $2}' | cut -d/ -f1)
if [ "$a" -lt "$b" ] 2>/dev/null; then cp /tmp/pd_sym.bak config/SLSP01/symbols.txt; conf; echo "$u $b -> $a REVERTED"; else echo "$u $b -> $a"; fi
