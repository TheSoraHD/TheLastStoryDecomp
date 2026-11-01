#!/bin/bash
# ssdo.sh snd_X.cpp | ut/ut_X.cpp : vendor, ssfind (apply unless WARN; FORCE=1 applies anyway), ssauto
f=$1; case $f in */*) r=$f;; *) r=snd/$f;; esac
t=$(basename $f)
python3 $(dirname $0)/vendor.py $r > /dev/null
python3 $(dirname $0)/ssfind.py $(cd $(dirname $0)/../../.. && pwd)/libs/ss_nw4r/src/$r > /tmp/ssf_$t.txt
grep -v "^OK\|^RENAME " /tmp/ssf_$t.txt
if [ -z "$FORCE" ] && grep -q WARN /tmp/ssf_$t.txt; then echo "STOP: WARN"; exit 1; fi
python3 $(dirname $0)/ssfind.py $(cd $(dirname $0)/../../.. && pwd)/libs/ss_nw4r/src/$r --apply > /dev/null
python3 $(dirname $0)/ssauto.py
