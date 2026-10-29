#!/usr/bin/env python3
"""Map LastWorld_tools.rso (USA) names onto the EU main.dol.

Pass 1: exported RSO .text functions whose masked code equals an EU function.
Pass 2: through those pairs, every bl / lis+addi that the RSO relocates against
        an import (DOL symbol) or another RSO function gives the EU address of
        that name (votes; conflicts reported).
Output: build/rso/rsomatch.json
"""
import json
import struct
import sys
from collections import Counter, defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).parent))
from common import Dol, Rso, RSO_USA, SCR, eu_symbols, normalise, rso_functions, words  # noqa

MIN_SIZE = int(sys.argv[1]) if len(sys.argv) > 1 else 16


def sext16(v):
    return v - 0x10000 if v & 0x8000 else v


def main():
    r = Rso(RSO_USA)
    dol = Dol()
    syms = eu_symbols()
    funcs = [s for s in syms if s["type"] == "function" and s["sec"] == ".text"]
    by_addr = defaultdict(list)
    for s in syms:
        by_addr[s["addr"]].append(s)

    # --- relocations by file offset
    rel = {}
    for off, sec, typ, add in r.relocs("int"):
        rel.setdefault(off, []).append(("int", sec, typ, add))
    for off, imp, typ, add in r.relocs("ext"):
        rel.setdefault(off, []).append(("ext", r.imports[imp][0], typ, add))

    rfns = rso_functions(r)
    text_off = r.sections[1][0]
    rso_name_at = {off - text_off: names for names, off, _ in rfns}

    # --- pass 1
    eu_idx = defaultdict(list)
    for s in funcs:
        b = dol.read(s["addr"], s["size"])
        if b:
            eu_idx[normalise(words(b))].append(s)
    rso_idx = defaultdict(list)
    for f in rfns:
        rso_idx[normalise(words(f[2]))].append(f)

    pairs = []  # (rso names, rso off, eu sym, size, unique)
    for key, rl in rso_idx.items():
        el = eu_idx.get(key)
        if not el:
            continue
        uniq = len(rl) == 1 and len(el) == 1
        for names, off, b in rl:
            for e in el:
                pairs.append(dict(rso=names, off=off, eu=e, size=len(b), unique=uniq,
                                  neu=len(el), nrso=len(rl)))

    # --- pass 1b: RSO block = exported function + unexported locals -> EU function is a prefix
    matched_r = {p["off"] for p in pairs}
    pre = defaultdict(list)
    for s_ in funcs:
        b = dol.read(s_["addr"], s_["size"])
        if b and len(b) >= 32:
            k = normalise(words(b))
            pre[k[:8]].append((k, s_))
    for names, off, b in rfns:
        if off in matched_r or len(b) < 32:
            continue
        k = normalise(words(b))
        c = [s_ for kk, s_ in pre.get(k[:8], []) if len(kk) < len(k) and k[:len(kk)] == kk
             and words(b)[len(kk) - 1] == 0x4E800020]
        if len(c) == 1:
            pairs.append(dict(rso=names, off=off, eu=c[0], size=c[0]["size"], unique=True,
                              neu=1, nrso=1, prefix=True))

    # --- pass 2: propagate through relocations of reliable pairs
    votes = defaultdict(Counter)  # name -> Counter(eu addr)
    kinds = {}
    for p in pairs:
        if not p["unique"] or p["size"] < MIN_SIZE:
            continue
        base_r, base_e, n = p["off"], p["eu"]["addr"], p["size"] // 4
        ha = {}
        for i in range(n):
            ro = base_r + 4 * i
            ew = dol.u32(base_e + 4 * i)
            for kind, tgt, typ, add in rel.get(ro, []) + rel.get(ro + 2, []):
                if kind == "int":
                    if tgt != 1:
                        continue  # own data, not interesting
                    names = rso_name_at.get(add)
                    if not names:
                        continue
                    name = names[0]
                else:
                    name = tgt
                if typ == 10 and (ew >> 26) == 18:
                    d = ew & 0x03FFFFFC
                    if d & 0x02000000:
                        d -= 0x04000000
                    addr = base_e + 4 * i + d
                    votes[name][addr] += 1
                    kinds[name] = "code"
                elif typ == 6:
                    ha[(name, add)] = ew & 0xFFFF
                elif typ == 4 and (name, add) in ha:
                    addr = ((ha[(name, add)] << 16) + sext16(ew & 0xFFFF) - (add if kind == "ext" else 0)) & 0xFFFFFFFF
                    votes[name][addr] += 1
                    kinds.setdefault(name, "data")

    # --- names from pass 1 (unique pairs) count as strong votes
    names_out = {}
    for p in pairs:
        if p["unique"] and p["size"] >= MIN_SIZE:
            for nm in p["rso"]:
                votes[nm][p["eu"]["addr"]] += 100
                kinds[nm] = "code"

    vt = json.loads((SCR / "vtmatch.json").read_text())["votes"]
    for nm, c in vt.items():
        for a, v in c.items():
            votes[nm][int(a, 16)] += v
            kinds.setdefault(nm, "code" if not nm.startswith("__vt__") else "data")

    conflicts = []
    for nm, c in votes.items():
        (addr, cnt), *rest = c.most_common()
        if rest:
            conflicts.append(dict(name=nm, votes={hex(a): v for a, v in c.most_common()}))
            if rest[0][1] * 2 > cnt:
                continue
        cur = by_addr.get(addr, [])
        names_out[nm] = dict(addr=hex(addr), votes=cnt, kind=kinds.get(nm),
                             eu=[s["name"] for s in cur])

    # one name per address: if several RSO names map to one address keep all (aliases)
    out = dict(pairs=[dict(rso=p["rso"], eu=p["eu"]["name"], addr=hex(p["eu"]["addr"]), size=p["size"],
                           unique=p["unique"], neu=p["neu"], nrso=p["nrso"], prefix=p.get("prefix", False)) for p in pairs],
               names=names_out, conflicts=conflicts)
    (SCR / "rsomatch.json").write_text(json.dumps(out, indent=1))
    up = [p for p in pairs if p["unique"] and p["size"] >= MIN_SIZE]
    print("pairs", len(pairs), "unique>=%d" % MIN_SIZE, len(up))
    print("names located", len(names_out), "conflicts", len(conflicts))
    imp = {n for n, _, _ in r.imports}
    print("imports located", sum(1 for n in names_out if n in imp), "/", len(imp))


if __name__ == "__main__":
    main()
