#!/usr/bin/env python3
"""libs_ref.json: take ours, add entries from a sandbox's libs_ref.json that are missing (after the last entry with the same top dir)."""
import json,sys
p='config/SLSP01/libs_ref.json'; d=json.load(open(p)); nn=json.load(open(sys.argv[1]))
have={e['name'] for e in d}
for e in nn:
    if e['name'] in have: continue
    top=e['name'].split('/')[0]
    idx=[k for k,x in enumerate(d) if x['name'].split('/')[0]==top]
    d.insert((max(idx)+1) if idx else len(d),e); print('added',e['name'],e['matching'])
open(p,'w').write(json.dumps(d,indent=1,ensure_ascii=False))
