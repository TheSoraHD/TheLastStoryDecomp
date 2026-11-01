#!/bin/bash
# sscall.sh snd_X.cpp [EXCL regex]: compile with ssfind, propose callee renames, warn if used in source, apply
cd $(cd $(dirname $0)/../../.. && pwd)
case $1 in */*) r=$1;; *) r=snd/$1;; esac; python3 $(dirname $0)/ssfind.py libs/ss_nw4r/src/$r > /dev/null
EXCL="$2" python3 $(dirname $0)/callmap.py > /tmp/cm_$(basename $1).txt
for f in $(awk '{print $1}' /tmp/cm_$(basename $1).txt); do grep -rlw "$f" src include libs --include=*.cpp --include=*.c --include=*.h | sed "s/^/WARN $f used in /"; done
awk 'NF>2{print "MULTI", $0}' /tmp/cm_$(basename $1).txt
[ -z "$NOAPPLY" ] && EXCL="$2" python3 $(dirname $0)/callmap.py --apply | tail -1
wc -l < /tmp/cm_$(basename $1).txt
