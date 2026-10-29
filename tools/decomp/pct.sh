#!/bin/bash
# usage: pct.sh <unit> [symbol...]  -> objdiff match percent of each symbol (all symbols if none)
u=$1; shift
cd ${CMP_ROOT:-$(dirname "$0")/../..}
build/tools/objdiff-cli diff -p . -u main/$u -o - --format json-pretty 2>/dev/null | python3 -c "
import json,sys
want=set(sys.argv[1:])
d=json.load(sys.stdin)
for s in d.get('left',{}).get('symbols',[]):
    n=s.get('name'); p=s.get('match_percent')
    if p is None or 'instructions' not in s: continue
    if want and n not in want: continue
    print('%7.3f %s'%(p,n))
" "$@"
