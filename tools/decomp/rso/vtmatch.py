#!/usr/bin/env python3
"""Pair RSO vtables with EU vtables through the RTTI class-name string; vote slot names."""
import json, struct, sys
from collections import defaultdict, Counter
from pathlib import Path
sys.path.insert(0, str(Path(__file__).parent))
from common import Dol, Rso, RSO_USA, SCR, eu_symbols, rso_functions
from rso import cstr

def main():
    r = Rso(RSO_USA); dol = Dol()
    rel = {}
    for off, sec, typ, add in r.relocs("int"):
        rel[off] = ("int", sec, typ, add)
    for off, imp, typ, add in r.relocs("ext"):
        rel[off] = ("ext", r.imports[imp][0], typ, add)
    toff = r.sections[1][0]
    fname = {off - toff: names[0] for names, off, _ in rso_functions(r)}
    # EU data words index
    word_at = defaultdict(list)
    d = dol.d
    for a, s, o in dol.secs:
        for i in range(0, s - 3, 4):
            w = struct.unpack(">I", d[o + i:o + i + 4])[0]
            if 0x80000000 <= w < 0x81800000:
                word_at[w].append(a + i)
    def eu_str(sname):
        b = sname.encode("latin1") + b"\0"
        res = []
        for a, s, o in dol.secs:
            seg = d[o:o + s]; i = seg.find(b)
            while i >= 0:
                if i == 0 or seg[i - 1] == 0:
                    res.append(a + i)
                i = seg.find(b, i + 1)
        return res
    votes = defaultdict(Counter)
    eunames = {x['name']: x['addr'] for x in eu_symbols()}
    paired = []
    for n, off, si in r.exports:
        if not n.startswith("__vt__") or si != 5:
            continue
        fo = r.sections[5][0] + off
        t = rel.get(fo)
        if not t or t[0] != "int":
            continue
        ti = r.sections[t[1]][0] + t[3]
        sn = rel.get(ti)
        if not sn or sn[0] != "int":
            continue
        cname = cstr(r.d, r.sections[sn[1]][0] + sn[3])
        cands = []
        for sa in eu_str(cname):
            for tia in word_at.get(sa, []):
                for vta in word_at.get(tia, []):
                    if dol.u32(vta + 4) == 0:
                        cands.append(vta)
        cands = sorted(set(cands))
        if len(cands) != 1 and n in eunames:
            cands = [eunames[n]]
        if len(cands) != 1:
            paired.append(dict(vt=n, cls=cname, eu=[hex(c) for c in cands], slots=0)); continue
        vta = cands[0]; k = 0; i = 2
        while True:
            x = rel.get(fo + 4 * i)
            ew = dol.u32(vta + 4 * i)
            if not x or ew is None or not (0x80004000 <= ew < 0x80800000):
                break
            if x[0] == "int" and x[1] == 1:
                nm = fname.get(x[3])
            elif x[0] == "ext":
                nm = x[1]
            else:
                nm = None
            if nm:
                votes[nm][ew] += 10; k += 1
            i += 1
        votes[n][vta] += 100
        paired.append(dict(vt=n, cls=cname, eu=[hex(vta)], slots=k))
    out = dict(vtables=paired, votes={k: {hex(a): c for a, c in v.items()} for k, v in votes.items()})
    (SCR / "vtmatch.json").write_text(json.dumps(out, indent=1))
    ok = [p for p in paired if len(p["eu"]) == 1]
    print("rso vtables", len(paired), "paired", len(ok), "slot names", sum(p["slots"] for p in ok), "distinct", len(votes))
    for p in paired[:12]: print(p)
main()
