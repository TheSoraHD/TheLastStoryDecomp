#!/usr/bin/env python3
# tabla.py [report] [out]: por unidad no completa: bytes de codigo sin enlazar, funciones <100 y sus %
import json,sys
rep=sys.argv[1] if len(sys.argv)>1 else '/home/claude/TheLastStoryDecomp/build/SLSP01/report.json'
out=sys.argv[2] if len(sys.argv)>2 else '/home/claude/scratch/tabla.tsv'
r=json.load(open(rep));rows=[]
for u in r['units']:
    m=u['measures'];fs=u.get('functions') or []
    below=[f for f in fs if (f.get('fuzzy_match_percent') or 0)<100]
    if not below: continue
    rows.append((u['name'][5:],int(m.get('total_code',0)),len(below),' '.join('%s:%.1f'%(f['metadata'].get('demangled_name',f['name']).split('(')[0][-40:],f.get('fuzzy_match_percent') or 0) for f in sorted(below,key=lambda f:-(f.get('fuzzy_match_percent') or 0)))))
rows.sort(key=lambda x:(x[2],-x[1]))
open(out,'w').write('unit\tcode\tfaltan\tfuncs\n'+'\n'.join('%s\t%d\t%d\t%s'%x for x in rows)+'\n')
m=r['measures'];print('tabla',len(rows),'unidades; funcs',m['matched_functions'],'code%',round(m['matched_code_percent'],2),'data%',round(m['complete_data_percent'],2),'units',m['complete_units'])
