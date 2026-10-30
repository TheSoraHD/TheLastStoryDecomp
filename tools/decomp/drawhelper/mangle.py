import os
import json,re,sys
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
from common import ptype
C=json.load(open(os.path.join(os.path.dirname(os.path.abspath(__file__)),'canon.json')))
STR='Q23std59basic_string<c,Q23std14char_traits<c>,Q23std12allocator<c>>'
def q(name):
    parts=name.split('::')
    if len(parts)==1: return f'{len(name)}{name}'
    return f'Q{len(parts)}'+''.join(f'{len(p)}{p}' for p in parts)
BASE={'int':'i','unsigned int':'Ui','float':'f','void':'v','char':'c','wchar_t':'w','std::string':STR}
def m(t):
    t=t.strip(); out=''
    ref=t.endswith('&'); 
    if ref: t=t[:-1].strip(); out+='R'
    ptr=t.endswith('*')
    if ptr: t=t[:-1].strip(); out+='P'
    const=t.startswith('const ')
    if const: t=t[6:].strip(); out+='C'
    out+=BASE.get(t) or q(t)
    return out
res={}
for n,ps in C.items():
    res[n]=n+'__Q23atn10DrawHelperF'+''.join(m(ptype(p)[0]) for p in ps)
json.dump(res,open('/tmp/mangled.json','w'),indent=1)
for k,v in res.items(): print(v)
