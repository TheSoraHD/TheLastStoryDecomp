#!/usr/bin/env python3
"""Compare our functions against the target, independent of symbol names and pool order.

usage: cmp.py <unit> [name-or-addr ...] [-v] [--nobuild]

Pairs our symbol with the target symbol by identical name, by the fn_<ADDR>
prefix, or through tools/map_<unit>.txt. Instructions are compared after
normalising relocations:
  - calls/branches to functions: compared by name (fn_ADDR prefix only)
  - references to data: compared by *content* (the bytes of the referenced
    object around the addend: float/double values, strings, pointer tables),
    so the result does not depend on the order of the literal pools.
OK means: same instructions and same referenced data.
"""
import os
import difflib, os, re, struct, subprocess, sys
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection

ROOT = os.environ.get("CMP_ROOT", os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..")))
os.chdir(ROOT)
args = [a for a in sys.argv[1:] if not a.startswith("-")]
unit = args[0]
want = args[1:]
verbose = "-v" in sys.argv
OBJDUMP = "build/binutils/powerpc-eabi-objdump"

if "--nobuild" not in sys.argv:
    r = subprocess.run(["ninja", f"build/SLSP01/src/{unit}.o"], capture_output=True, text=True)
    if r.returncode:
        out = r.stdout
        i = out.find("###")
        print(out[i:i + 3000] if i >= 0 else out[-3000:]); sys.exit(1)

def normname(n):
    return re.sub(r"(fn_[0-9A-F]{8})(__\w*?)(?=@|$)", r"\1", n) if "fn_" in n else n


RELTYPES = {109: "SDA21", 6: "HA", 4: "LO", 5: "HI", 10: "REL24", 1: "ADDR32", 11: "REL14"}


class Obj:
    def __init__(self, path):
        self.f = open(path, "rb")
        self.elf = ELFFile(self.f)
        self.secs = list(self.elf.iter_sections())
        self.symtab = self.elf.get_section_by_name(".symtab")
        self.syms = list(self.symtab.iter_symbols())
        self.data = {}
        for i, s in enumerate(self.secs):
            if s["sh_type"] == "SHT_PROGBITS":
                self.data[i] = s.data()
        self.rel = {}
        for s in self.secs:
            if isinstance(s, RelocationSection):
                tgt = s["sh_info"]
                self.rel.setdefault(tgt, {})
                for r in s.iter_relocations():
                    self.rel[tgt][r["r_offset"]] = (r["r_info_type"], r["r_info_sym"], r["r_addend"])
        self.by_sec = {}
        for sym in self.syms:
            sh = sym["st_shndx"]
            if isinstance(sh, int):
                self.by_sec.setdefault(sh, []).append(sym)
        for v in self.by_sec.values():
            v.sort(key=lambda s: s["st_value"])

    def describe_data(self, symidx, addend, depth=0):
        """Content-based description of a data reference."""
        sym = self.syms[symidx]
        sh = sym["st_shndx"]
        name = sym.name
        if not isinstance(sh, int) or sh == 0:
            return "ext:" + name
        sec = self.secs[sh]
        secname = sec.name
        if sym["st_info"]["type"] == "STT_FUNC" or secname.startswith(".text"):
            return "fn:" + re.sub(r"(fn_[0-9A-F]{8})\w*", r"\1", name)
        if name.startswith("@STRING@"):
            return "sym:anon+%d" % addend  # literal of an inline function (pool of the first emitter)
        if not re.match(r"^(@\d+|lbl_[0-9A-Fa-f]+|@stringBase\d*|\.\w+|jumptable_\w+|s[A-Z]\w*_unk|gap_\w+)$", name) \
                and sym["st_info"]["bind"] != "STB_LOCAL" or name.startswith(("@LOCAL@", "@GUARD@", "__vt__", "__RTTI__")):
            return "sym:%s+%d" % (normname(name), addend)
        off = sym["st_value"] + addend
        if sh not in self.data:  # bss: named statics keep the name, anonymous ones do not
            return "sym:anon+%d" % addend
        d = self.data[sh]
        objs = [s for s in self.by_sec.get(sh, []) if s["st_value"] <= off < s["st_value"] + max(s["st_size"], 1)
                and s["st_info"]["type"] in ("STT_OBJECT", "STT_NOTYPE") and s["st_size"] > 0]
        if secname in (".sdata2", ".sbss2") and off + 4 <= len(d):
            sz = objs[0]["st_size"] if objs else 4
            if sz >= 8:
                return "lit:" + d[off:off + 8].hex()
            # the "" string literal is also 4 zero bytes in the pool: tell it apart by its size
            return "lit%d:" % sz + d[off:off + 4].hex()
        end = d.find(b"\0", off)
        if end >= off and end - off < 256 and all(32 <= c < 127 or c >= 0x80 for c in d[off:end]) and (
                end > off or not objs or objs[0]["st_size"] <= 8 or "string" in objs[0].name or objs[0].name.startswith("@")):
            if end > off or not objs or objs[0]["st_size"] < 16:
                return "str:" + d[off:end].decode("latin1")
        o = objs[0] if objs else None
        if o is not None:
            start, sz = o["st_value"], o["st_size"]
            raw = bytearray(d[start:start + sz])
            rels = self.rel.get(sh, {})
            desc = []
            for ro in range(start, start + sz, 4):
                if ro in rels:
                    t, si, ad = rels[ro]
                    desc.append(self.describe_data(si, ad, depth + 1) if depth < 1 else "ptr")
                    raw[ro - start:ro - start + 4] = b"\0\0\0\0"
            return "obj%d+%d:%s|%s" % (sz, off - start, bytes(raw).hex()[:96], ",".join(desc)[:300])
        return "data:%s+%d" % (secname, addend)


def load(path):
    o = Obj(path)
    txt = subprocess.run([OBJDUMP, "-d", "--no-show-raw-insn", path], capture_output=True, text=True).stdout
    funcs = {}
    cur = None
    for l in txt.split("\n"):
        m = re.match(r"^([0-9a-f]+) <(.+)>:$", l)
        if m:
            cur = m.group(2)
            funcs[cur] = []
            continue
        if cur is None:
            continue
        m = re.match(r"^\s+([0-9a-f]+):\s+(.*)$", l)
        if m:
            ins = re.sub(r"\s+", " ", m.group(2).split("#")[0].strip())
            ins = re.sub(r"\b[0-9a-f]+ <.*>$", "<T>", ins)
            funcs[cur].append([int(m.group(1), 16), ins])
    tsec = None
    for i, s in enumerate(o.secs):
        if s.name == ".text":
            tsec = i
            break
    rels = o.rel.get(tsec, {})
    out = {}
    for name, body in funcs.items():
        lines = []
        for addr, ins in body:
            r = rels.get(addr) or rels.get(addr + 2)
            if r:
                t, si, ad = r
                tn = RELTYPES.get(t, str(t))
                sym = o.syms[si]
                is_fn = sym["st_info"]["type"] == "STT_FUNC" or tn in ("REL24", "REL14")
                if not is_fn and not isinstance(sym["st_shndx"], int):
                    is_fn = False
                undef = sym["st_shndx"] == "SHN_UNDEF" or sym["st_shndx"] == 0
                if tn in ("REL24", "REL14"):
                    ins = re.sub(r"<T>|\b0x[0-9a-f]+\b|\b\d+\b$", "", ins).strip()
                    ins = "%s ~%s:%s" % (ins, tn, normname(sym.name))
                else:
                    ins = re.sub(r"(-?0x[0-9a-f]+|\b\d+\b)(\(r\d+\))?$", r"<rel>\2", ins)
                    if undef:
                        nm = normname(sym.name)
                        if re.match(r"^lbl_[0-9A-F]+$", nm):
                            nm = "anon"
                        ins = "%s ~%s:sym:%s+%d" % (ins, tn, nm, ad)
                    elif sym["st_info"]["type"] == "STT_FUNC":
                        ins = "%s ~%s:sym:%s+%d" % (ins, tn, normname(sym.name), ad)
                    else:
                        ins = "%s ~%s:%s" % (ins, tn, o.describe_data(si, ad))
            lines.append(ins)
        # drop trailing alignment padding (-func_align 16 emits zero words after the function)
        while lines and re.match(r"^\.long 0x0$|^\.word 0x0+$", lines[-1]):
            lines.pop()
        out[name] = lines
    return out


ours = load(f"build/SLSP01/src/{unit}.o")
tgt = load(f"build/SLSP01/obj/{unit}.o")
tgt_by_addr = {}
tgt_by_vf = {}
for n in tgt:
    m = re.match(r"fn_([0-9A-F]{8})", n)
    if m:
        tgt_by_addr[m.group(1)] = n
    m = re.match(r"(vf[0-9A-F]+__\d+\w+?)F", n)
    if m:
        tgt_by_vf[m.group(1)] = n

extra = {}
mp = os.path.join(os.path.dirname(os.path.abspath(__file__)), "maps", "map_%s.txt") % unit.replace("/", "_")
if os.path.exists(mp):
    for l in open(mp):
        p = l.split()
        if len(p) == 2:
            extra[p[1]] = p[0]
rename = [("fn_" + a, n) for n, a in extra.items()]

ok = bad = 0
for n, body in ours.items():
    m = re.match(r"fn_([0-9A-F]{8})", n)
    t = n if n in tgt else (tgt_by_addr.get(m.group(1)) if m else None)
    if t is None and n in extra:
        t = tgt_by_addr.get(extra[n])
    if t is None:
        mv = re.match(r"(vf[0-9A-F]+__\d+\w+?)F", n)
        if mv:
            t = tgt_by_vf.get(mv.group(1))
    if t is None:
        continue
    if want and not any(w in n or w in t for w in want):
        continue
    a = []
    for l in tgt[t]:
        for x, y in rename:
            l = re.sub(r"\b%s\b" % re.escape(x), y, l)
        a.append(l)
    b = body
    if a == b:
        ok += 1
        print("OK    %s" % n)
    else:
        bad += 1
        nd = sum(1 for x in difflib.ndiff(a, b) if x[0] in "+-")
        print("DIFF  %s  (%d/%d lines differ, %d vs %d instrs)" % (n, nd, len(a), len(a), len(b)))
        if verbose:
            sm = difflib.SequenceMatcher(None, a, b, autojunk=False)
            for op, i1, i2, j1, j2 in sm.get_opcodes():
                if op == "equal":
                    for k in range(i1, i2):
                        print("      %-58s %s" % (a[k][:58], b[j1 + k - i1][:58]))
                else:
                    for k in range(max(i2 - i1, j2 - j1)):
                        x = a[i1 + k] if i1 + k < i2 else ""
                        y = b[j1 + k] if j1 + k < j2 else ""
                        print("  !!  %-58s %s" % (x[:58], y[:58]))
print("# ok %d, diff %d" % (ok, bad))
