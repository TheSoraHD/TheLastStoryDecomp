"""Minimal parser for Wii RSO modules (LastWorld_tools.rso)."""
import struct


def cstr(d, off):
    e = d.find(b"\x00", off)
    return d[off:e].decode("latin1", "replace")


class Rso:
    def __init__(self, path):
        d = self.d = open(path, "rb").read()
        u = lambda o: struct.unpack(">I", d[o:o + 4])[0]
        self.nsec = u(0x08)
        secoff = u(0x0C)
        self.sections = []
        for i in range(self.nsec):
            off, size = struct.unpack(">II", d[secoff + i * 8: secoff + i * 8 + 8])
            self.sections.append((off & ~1, size, off & 1))
        self.name = cstr(d, u(0x10)) if u(0x10) else ""
        self.bss_size = u(0x18)
        self.int_rel = (u(0x30), u(0x34))
        self.ext_rel = (u(0x38), u(0x3C))
        eo, es, en = u(0x40), u(0x44), u(0x48)
        io, is_, inn = u(0x4C), u(0x50), u(0x54)
        self.exports = []  # (name, offset, section)
        for i in range(es // 16):
            no, off, si, h = struct.unpack(">4I", d[eo + i * 16: eo + i * 16 + 16])
            self.exports.append((cstr(d, en + no), off, si))
        self.imports = []  # (name, offset, reloc_off)
        for i in range(is_ // 12):
            no, off, ro = struct.unpack(">3I", d[io + i * 12: io + i * 12 + 12])
            self.imports.append((cstr(d, inn + no), off, ro))

    def relocs(self, which):
        off, size = self.int_rel if which == "int" else self.ext_rel
        out = []
        for i in range(size // 12):
            o, info, add = struct.unpack(">3I", self.d[off + i * 12: off + i * 12 + 12])
            out.append((o, info >> 8, info & 0xFF, add))
        return out
