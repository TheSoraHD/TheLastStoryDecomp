"""Identifiers (class/namespace tokens) that only exist in the tools RSO."""
import re, sys
from pathlib import Path
sys.path.insert(0, str(Path(__file__).parent))
from common import Rso, RSO_USA, Dol, eu_symbols

def tokens(name):
    """Length-prefixed identifiers in a CodeWarrior mangled name."""
    out = set(); j = 0; n = len(name)
    while j < n:
        c = name[j]
        if c == 'Q' and j + 1 < n and name[j + 1].isdigit():
            j += 2; continue
        if c.isdigit() and (j == 0 or not name[j - 1].isalnum() or name[j - 1] in 'FPRCUSQ' or name[j-1] == '_'):
            k = j
            while k < n and name[k].isdigit(): k += 1
            ln = int(name[j:k]); s_ = name[k:k + ln]
            if ln and len(s_) == ln and (s_[0].isalpha() or s_[0] == '_') and all(ch.isalnum() or ch in '_<>,' for ch in s_.split('<')[0]):
                out.add(s_.split('<')[0]); j = k + ln if '<' not in s_ else k + len(s_.split('<')[0]); continue
            j = k; continue
        j += 1
    return out

def tools_only():
    r = Rso(RSO_USA); dol = Dol()
    imp = set().union(*(tokens(n) for n, _, _ in r.imports))
    eu = set().union(*(tokens(s['name']) for s in eu_symbols()))
    exp = set().union(*(tokens(n) for n, _, si in r.exports))
    cand = exp - imp - eu
    d = dol.d
    return {t for t in cand if len(t) < 4 or d.find(t.encode()) < 0}

if __name__ == "__main__":
    t = sorted(tools_only()); print(len(t)); print(' '.join(t))
