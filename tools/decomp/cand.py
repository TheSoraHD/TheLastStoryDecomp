# cand.py [report] > md: tabla de candidatas por datos (unidades sin enlazar) con la marca Z de sdz.py (/home/claude/scratch/sdz.tsv). Ejecutar desde la raiz.
import json,sys
import os
R=os.getcwd()
r=json.load(open(sys.argv[1] if len(sys.argv)>1 else R+'/build/SLSP01/report.json'))
Z={l.split('\t')[0]:l.split('\t')[1] for l in open('/home/claude/scratch/sdz.tsv')}
m=r['measures'];T=int(m['total_data']);C=int(m['complete_data'])
print('Datos totales %d B; enlazados %d (%.2f %%); 1 %% = %d B.\n'%(T,C,100*C/T,T//100))
rows=[]
for u in r['units']:
    mm=u['measures']
    if mm.get('complete_units',0)==1: continue
    n=u['name'][5:];fs=u.get('functions') or []
    below=sorted([(f.get('fuzzy_match_percent') or 0) for f in fs if (f.get('fuzzy_match_percent') or 0)<100 or False])
    # objdiff 100 but not counted? use matched count
    miss=int(mm.get('total_functions',0))-int(mm.get('matched_functions',0))
    d=int(mm.get('total_data',0))
    if d<300: continue
    rows.append((n,d,miss,int(mm.get('total_functions',0)),below,Z.get(n,'')))
for title,lo,hi,mind in (('A: 0-3 funciones (>= 300 B)',0,3,300),('B: 4-6 funciones (>= 600 B)',4,6,600),('C: 7 o mas funciones (>= 1500 B)',7,99999,1500)):
    sel=[x for x in rows if lo<=x[2]<=hi and x[1]>=mind]
    sel.sort(key=lambda x:-x[1]/max(x[2],1))
    print('## %s\n\n| Unidad | Datos | Faltan | %% de las que faltan | B/func | Z |\n|---|---|---|---|---|---|'%title)
    for n,d,mi,t,b,z in sel:
        ps=', '.join('%.1f'%p for p in b[:8])+(' ...' if len(b)>8 else '')
        print('| %s | %d | %d/%d | %s | %d | %s |'%(n,d,mi,t,ps,d//max(mi,1),z))
    print('\nSuma: %d B en %d unidades\n'%(sum(x[1] for x in sel),len(sel)))
