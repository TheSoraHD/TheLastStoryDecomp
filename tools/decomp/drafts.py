#!/usr/bin/env python3
"""drafts.py: lists the `// DRAFT:` functions (NONMATCHING code that is not equivalent to the original)
by unit, and marks the ones in the boot path (main() -> title screen, docs/ruta-arranque.md).

usage: drafts.py [--route] [--count] [unit...]
  --route  only the units of the boot path
  --count  only the totals (per unit and overall)
"""
import os, re, sys

ROOT = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..')

# units of the boot path (main() -> fn_8047A4C8 -> BootTask -> StrapTask -> NewTitleTask, and what main()
# calls every frame); keep in sync with docs/ruta-arranque.md
ROUTE = set('''
game/main atn/Task atn/Loader atn/FileList atn/FileUtil_unk atn/NandManager atn/LinearAllocator atn/ResRef_unk
game/GameConfig_unk game/SystemErrorMessage_unk game/GameSystem_unk game/SetConfig game/HeapInfo_unk
game/HomeButtonMenu atn/GraphicsDevice atn/RenderSystem_unk atn/DrawHelper atn/D2A_Manager atn/AudioDevice
atn/SoundManager atn/Pad_unk atn/MindowsCommon atn/SequenceHolder atn/SequenceUnitParam1_const_int
game/SequenceBoot_unk game/BootTask game/StrapTask game/NewTitleTask
'''.split())


def func_after(lines, i):
    """name of the function defined after the comment block at line i"""
    for j in range(i + 1, min(i + 40, len(lines))):
        s = lines[j].strip()
        if not s or s.startswith('//') or s.startswith('template'):
            continue
        m = re.search(r'([A-Za-z_][\w:<>,~ ]*?)\s*\(', s)
        if m:
            return m.group(1).split()[-1]
        return s[:60]
    return '?'


def scan(units):
    out = []
    src = os.path.join(ROOT, 'src')
    for dp, _, fs in os.walk(src):
        for f in sorted(fs):
            if not f.endswith(('.cpp', '.c')):
                continue
            p = os.path.join(dp, f)
            unit = os.path.relpath(p, src).rsplit('.', 1)[0].replace(os.sep, '/')
            if units and unit not in units:
                continue
            lines = open(p, encoding='latin-1').read().split('\n')
            for i, s in enumerate(lines):
                m = re.match(r'\s*// DRAFT: (.*)', s)
                if m:
                    out.append((unit, i + 1, func_after(lines, i), m.group(1)))
    return sorted(out)


def main():
    args = sys.argv[1:]
    route = '--route' in args
    count = '--count' in args
    units = set(a for a in args if not a.startswith('--'))
    if route:
        units = (units & ROUTE) if units else ROUTE
    res = scan(units)
    by = {}
    for u, n, fn, why in res:
        by.setdefault(u, []).append((n, fn, why))
    for u in sorted(by):
        tag = ' [ruta]' if u in ROUTE else ''
        print('%s: %d%s' % (u, len(by[u]), tag))
        if not count:
            for n, fn, why in by[u]:
                print('  %5d %s: %s' % (n, fn, why))
    inr = sum(len(v) for u, v in by.items() if u in ROUTE)
    print('total: %d DRAFT en %d unidades; en la ruta de arranque: %d' % (len(res), len(by), inr))


if __name__ == '__main__':
    main()
