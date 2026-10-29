"""Shared helpers: DOL reading, EU symbols, RSO functions, normalisation."""
import re
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).parent))
from rso import Rso  # noqa: E402

import os

ROOT = Path(__file__).resolve().parents[3]  # repo root
SCR = ROOT / "build" / "rso"  # outputs go to build/rso/
SCR.mkdir(parents=True, exist_ok=True)
DOL = ROOT / "orig/SLSP01/sys/main.dol"
SYMS = ROOT / "config/SLSP01/symbols.txt"
# USA LastWorld_tools.rso (only the NTSC-U disc has the full module); not committed
RSO_USA = Path(os.environ.get("RSO_USA", ROOT / "orig/SLSEXJ/LastWorld_tools.rso"))


class Dol:
    def __init__(self, path=DOL):
        d = self.d = path.read_bytes()
        offs = struct.unpack(">18I", d[0:0x48])
        addrs = struct.unpack(">18I", d[0x48:0x90])
        sizes = struct.unpack(">18I", d[0x90:0xD8])
        self.secs = [(a, s, o) for o, a, s in zip(offs, addrs, sizes) if s]

    def read(self, addr, size):
        for a, s, o in self.secs:
            if a <= addr < a + s:
                return self.d[o + addr - a: o + addr - a + size]
        return None

    def u32(self, addr):
        b = self.read(addr, 4)
        return struct.unpack(">I", b)[0] if b and len(b) == 4 else None


SYM_RE = re.compile(r"^(\S+) = (\.\w+):0x([0-9A-Fa-f]+); // (.*)$")


def eu_symbols():
    out = []
    for line in SYMS.read_text(encoding="utf-8", errors="replace").splitlines():
        m = SYM_RE.match(line)
        if not m:
            continue
        name, sec, addr, rest = m.groups()
        t = re.search(r"type:(\w+)", rest)
        s = re.search(r"size:0x([0-9A-Fa-f]+)", rest)
        out.append(dict(name=name, sec=sec, addr=int(addr, 16),
                        type=t.group(1) if t else "", size=int(s.group(1), 16) if s else 0,
                        line=line))
    return out


def words(b):
    n = len(b) // 4
    return list(struct.unpack(">%dI" % n, b[: n * 4]))


DFORM = set(range(32, 56)) | {14, 12, 13, 8, 7}  # loads/stores, addi/addic, subfic, mulli


def normalise(ws):
    """Mask relocatable parts symmetrically (works for DOL and RSO code)."""
    out = []
    lis = set()  # registers whose last write was a lis
    for w in ws:
        op = w >> 26
        if op == 18:  # b / bl
            out.append(w & 0xFC000003)
            continue
        rd = (w >> 21) & 31
        ra = (w >> 16) & 31
        if op == 15 and ra == 0:  # lis
            out.append(w & 0xFFFF0000)
            lis.add(rd)
            continue
        if op in DFORM and ra in lis:
            out.append(w & 0xFFFF0000)
            if op in (14, 32, 33, 34, 35, 40, 41, 42, 43, 46):  # writes rd (addi, loads)
                if rd != ra:
                    lis.discard(rd)
                else:
                    lis.discard(rd)
            continue
        if op == 24 and ((w >> 21) & 31) in lis:  # ori rA,rS,lo
            pass
        # any other write to a tracked register clears it (approximate: rd field)
        if op in (14, 15, 31, 32, 33, 34, 35, 40, 41, 42, 43, 46, 7, 8, 12, 13, 21, 28, 29, 24, 25):
            tgt = ra if op in (21, 28, 29, 24, 25) or (op == 31) and False else rd
            lis.discard(tgt)
        out.append(w)
    return tuple(out)


def rso_functions(r):
    """(name, file_offset, size) for exported code symbols in .text (section 1)."""
    toff, tsize = r.sections[1][0], r.sections[1][1]
    ex = sorted({(off, n) for n, off, si in r.exports if si == 1 and not n.startswith(".")})
    # group aliases at same offset
    by_off = {}
    for off, n in ex:
        by_off.setdefault(off, []).append(n)
    offs = sorted(by_off)
    fns = []
    for i, off in enumerate(offs):
        end = offs[i + 1] if i + 1 < len(offs) else tsize
        b = r.d[toff + off: toff + end]
        # trim trailing zero padding
        while len(b) >= 4 and b[-4:] == b"\0\0\0\0":
            b = b[:-4]
        fns.append((by_off[off], toff + off, b))
    return fns

