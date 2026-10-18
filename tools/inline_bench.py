#!/usr/bin/env python3
"""Probe MWCC's inlining decisions with synthetic code (rules in docs/inlining.md).

Each argument is a comma-separated list of statement counts: one inline function per count,
all called once by a function f<i>. An optional "own=N" prefix gives f<i> N statements of its own
("own=120:150,100"). Prints, per case, whether each callee was inlined (I) or called (c).
Compiles with the flags of game/GimmickManager.cpp (Wii/1.3, -ipa file).

    python3 tools/inline_bench.py 150,100 150,5 own=120:150,100
"""
import os, re, subprocess, sys, tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def body(name, k, off):
    return f"inline void {name}(int* p) {{\n" + "".join(
        f"    p[{i}] = p[{i + 1}] * {i + off} + p[{i + 2}];\n" for i in range(k)) + "}\n"


def main():
    cases = []
    for a in sys.argv[1:]:
        own = 0
        if a.startswith("own="):
            own, a = a[4:].split(":")
            own = int(own)
        cases.append((own, [int(x) for x in a.split(",")]))
    src = "extern int g[100000];\nextern int ext(int);\n"
    for ci, (own, sizes) in enumerate(cases):
        for j, k in enumerate(sizes):
            src += body(f"F{ci}_{j}", k, 3 + j)
        src += f"void f{ci}() {{\n" + "".join(f"    g[{90000 + i}] = ext(g[{i}]) + {i};\n" for i in range(own))
        src += "".join(f"    F{ci}_{j}(g + {j * 300});\n" for j in range(len(sizes))) + "}\n"
    tmp = tempfile.mkdtemp()
    cpp = os.path.join(tmp, "bench.cpp")
    open(cpp, "w").write(src)
    cmd = subprocess.run(["ninja", "-t", "commands", "build/SLSP01/src/game/GimmickManager.o"], cwd=ROOT,
                         capture_output=True, text=True).stdout.strip().splitlines()[-1]
    cmd = cmd.split("&&")[0].replace("src/game/GimmickManager.cpp", cpp).replace("-o build/SLSP01/src/game", "-o " + tmp)
    cmd = cmd.replace("-MMD", "")
    subprocess.run(cmd, shell=True, cwd=ROOT, capture_output=True)
    asm = os.path.join(tmp, "bench.s")
    subprocess.run([os.path.join(ROOT, "build/tools/dtk"), "elf", "disasm", os.path.join(tmp, "bench.o"), asm],
                   capture_output=True)
    calls = set()
    fn = None
    for line in open(asm):
        if line.startswith(".fn "):
            fn = line.split()[1].rstrip(",")
        m = re.search(r"\bbl\s+(\S+)", line)
        if m and fn:
            calls.add((fn, m.group(1).split("__")[0]))
    for ci, (own, sizes) in enumerate(cases):
        res = ["c" if (f"f{ci}__Fv", f"F{ci}_{j}") in calls else "I" for j in range(len(sizes))]
        print(f"own={own} {sizes} -> {' '.join(res)}")


if __name__ == "__main__":
    main()
