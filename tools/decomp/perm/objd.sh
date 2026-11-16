#!/bin/bash
# objd.sh <args...> file.o : objdump solo de la funcion $PERM_FN (variable de entorno)
f="${@: -1}"
/home/claude/binutils/powerpc-eabi-objdump -dr -EB -mpowerpc -M broadway "$f" | awk -v fn="<$PERM_FN>:" 'NR<=6{print;next} index($0,fn){p=1} p&&/^$/{exit} p'
