#!/bin/bash
# symmerge.sh X: tras un merge de agent-X que choca en symbols.txt: HEAD + refmap2 (MAINREF = copia del agente) de las unidades
# (lote mixto2-20261120; supone las copias en /home/claude/work-X y nostr.py en /home/claude/agentes)
# que el agente toco y de las que labelcheck da mal tras compilar. Deja symbols.txt listo para git add.
X=$1; W=/home/claude/work-$X; R=/home/claude/TheLastStoryDecomp
cd $R; export TMPDIR=/home/claude/scratch/tmp
python3 tools/decomp/ourshunks.py
BASE=$(git merge-base HEAD MERGE_HEAD)
U=$(git diff --name-only $BASE MERGE_HEAD -- src | sed -E 's#^src/##; s#\.(cpp|c)$##' | sort -u | while read u; do grep -q "^$u\.\(cpp\|c\):" config/SLSP01/splits.txt && echo $u; done)
bash /home/claude/scratch/conf.sh >/dev/null 2>&1; timeout 590 ninja > /tmp/symmerge_ninja.log 2>&1
python3 tools/decomp/labelcheck.py $(cat /home/claude/ref/units.txt) 2>/dev/null | awk '{print $1}' > /tmp/symmerge_lc.txt
L=$(cat /tmp/symmerge_lc.txt | while read u; do grep -qx "$u" /home/claude/ref/labelcheck_units.txt 2>/dev/null || echo $u; done)
ALL=$(echo $U $L | tr ' ' '\n' | sort -u | tr '\n' ' ')
echo "refmap2: $ALL"
# la copia del agente puede tener .o viejos (parches probados y revertidos): recompilarlos antes
(cd $W && timeout 590 ninja -j2 $(for u in $ALL; do echo build/SLSP01/src/$u.o; done) >/dev/null 2>&1)
MAINREF=$W python3 tools/decomp/refmap2.py $ALL | tail -2
python3 /home/claude/agentes/nostr.py
