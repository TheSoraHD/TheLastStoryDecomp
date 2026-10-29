import re, bisect
from pathlib import Path
SPL = Path(__file__).resolve().parents[3] / "config/SLSP01/splits.txt"
def text_units():
    cur=None; rng=[]
    for l in SPL.read_text().splitlines():
        if l and not l[0].isspace() and l.endswith(':'): cur=l[:-1]
        m=re.match(r'\s+\.text\s+start:0x([0-9A-F]+) end:0x([0-9A-F]+)',l)
        if m and cur: rng.append((int(m.group(1),16),int(m.group(2),16),cur))
    rng.sort(); starts=[r[0] for r in rng]
    def unit(a):
        i=bisect.bisect_right(starts,a)-1
        return rng[i][2] if i>=0 and rng[i][0]<=a<rng[i][1] else '?'
    return unit
