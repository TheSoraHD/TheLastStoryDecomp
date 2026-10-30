import os,re,collections
ROOT=os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)),'..','..','..'))
LO,HI=0x8005DED8,0x80068DB0
def names():
    s=set()
    for l in open(f'{ROOT}/config/SLSP01/symbols.txt'):
        m=re.match(r'(fn_[0-9A-F]{8}) = \.text:0x([0-9A-F]+);',l)
        if m and LO<=int(m.group(2),16)<HI: s.add(m.group(1))
    return s
NAMES=names()
DECL_RE=re.compile(r'(?:extern\s+"C"\s+)?(?:void|int|unsigned\s+int|float|bool)\s+(fn_800[56][0-9A-F]{4})\s*\(([^;{)]*)\)\s*;')
def src_files():
    for d,_,fs in os.walk(ROOT+'/src'):
        for f in fs:
            if f.endswith(('.cpp','.c','.h')): yield os.path.join(d,f)
def split_params(s):
    out=[];depth=0;cur=''
    for ch in s:
        if ch in '(<[': depth+=1
        if ch in ')>]': depth-=1
        if ch==',' and depth==0: out.append(cur.strip());cur=''
        else: cur+=ch
    if cur.strip(): out.append(cur.strip())
    return out
def ptype(p):
    # returns (type, name)
    m=re.match(r'(.*?)([A-Za-z_]\w*)$',p.strip())
    if not m or m.group(1).strip()=='' : return (p.strip(),'')
    return (m.group(1).strip(),m.group(2))
def is_float(t):
    return re.sub(r'\bconst\b','',t).strip() in ('float','double')
