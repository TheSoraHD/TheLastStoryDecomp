#!/bin/bash
# prun.sh <dir> <fn> [segundos]: un tramo (590 s por defecto, -j1), log en <dir>/run.log (anexado)
# PERMUTER: ruta de decomp-permuter (MIT); por defecto /home/claude/scratch/permuter
export PATH=/home/claude/binutils:$PATH PERM_FN=$2
cd $1
timeout ${3:-590} python3 -u ${PERMUTER:-/home/claude/scratch/permuter}/permuter.py . -j1 --stop-on-zero >> run.log 2>&1
tr '\r' '\n' < run.log | grep -a -E "base score|new best|score = 0 |wrote|rror" | grep -av "^ *iteration" | tail -8
ls -d output-* 2>/dev/null
