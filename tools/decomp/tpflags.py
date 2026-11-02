# tpflags.py <unit>...: give libs_ref units the cflags of the TP homebuttonLib sources (libs/tp_revolution)
import json,sys
p='config/SLSP01/libs_ref.json'; d=json.load(open(p))
M='libs/tp_revolution/msl'
add=['-i libs/tp_revolution/include','-i libs/tp_revolution/tpinc','-ir libs/tp_revolution/homebuttonLib',
 '-i %s/MSL/MSL_C/MSL_Common/Include'%M,'-i %s/MSL/MSL_C/MSL_Common_Embedded/Math/Include'%M,'-i %s/MSL/MSL_C/PPC_EABI/Include'%M,
 '-i %s/MSL/MSL_C++/MSL_Common/Include'%M,'-i %s/Runtime/Inc'%M,'-D__REVOLUTION_SDK__','-D__GEKKO__','-DVERSION=0']
for u in d:
  if u['name'] in sys.argv[1:]:
    fl=[f for f in u['cflags'] if f!='-i include' and 'libs/RVL_SDK' not in f and 'HBM_VERSION' not in f and 'PowerPC_EABI_Support' not in f]
    u['cflags']=fl+add; print('ok',u['name'])
open(p,'w').write(json.dumps(d, indent=1, ensure_ascii=False))
