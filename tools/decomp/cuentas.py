#!/usr/bin/env python3
# cuentas.py [report]: unidades completas, funciones al 100 %, codigo/datos, 0 % (total y sin iostream/auto_03) y DRAFT
import json,sys,subprocess,os
import os; R=os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)),'..','..'))
rep=sys.argv[1] if len(sys.argv)>1 else R+'/build/SLSP01/report.json'
r=json.load(open(rep));m=r['measures']
z=0;zr=0
for u in r['units']:
    for f in u.get('functions') or []:
        if not f.get('fuzzy_match_percent'):
            z+=1
            n=u['name']
            if 'auto_03' in n or 'iostream' in n.lower() or '/MSL_C++' in n or 'msl_cpp' in n.lower(): continue
            zr+=1
d=subprocess.run(['python3',R+'/tools/decomp/drafts.py','--count'],capture_output=True,text=True,cwd=R).stdout.strip().split('\n')[-1]
print('unidades %d | 100%% %d / %d | codigo %.2f %% | datos %.2f %% | 0%% %d (sin iostream/auto_03 %d) | DRAFT: %s'%(m['complete_units'],m['matched_functions'],m['total_functions'],m['matched_code_percent'],m['complete_data_percent'],z,zr,d))
