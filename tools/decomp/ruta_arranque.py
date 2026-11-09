#!/usr/bin/env python3
# ruta_arranque.py [report.json]: tables of the boot route (main() -> title screen) for docs/ruta-arranque.md.
#  1. per unit of the route: functions at 100%, almost equal (>= 99%), DRAFT, linked (matching in game_units.json);
#  2. library objects called from the route (bl targets in the asm of the route units) and whether they link.
# Needs build/SLSP01/asm (dtk split) and build/SLSP01/report.json.
import json, os, re, sys, collections

ROUTE = ("game/main atn/Task atn/Loader atn/FileList atn/FileUtil_unk atn/NandManager atn/LinearAllocator "
         "atn/ResRef_unk game/GameConfig_unk game/SystemErrorMessage_unk game/GameSystem_unk game/SetConfig "
         "game/HeapInfo_unk game/HomeButtonMenu atn/GraphicsDevice atn/RenderSystem_unk atn/DrawHelper "
         "atn/D2A_Manager atn/AudioDevice atn/SoundManager atn/Pad_unk atn/MindowsCommon atn/SequenceHolder "
         "atn/SequenceUnitParam1_const_int game/BootTask game/StrapTask game/NewTitleTask").split()
GAME = ('game', 'atn', 'Event', 'AI', 'gmk', 'chr', 'MP', 'db')

rep = json.load(open(sys.argv[1] if len(sys.argv) > 1 else 'build/SLSP01/report.json'))
units = {u['name'].replace('main/', ''): u for u in rep['units']}
gu = {u['name'].rsplit('.', 1)[0]: u.get('matching') for u in json.load(open('config/SLSP01/game_units.json'))}
libs = {l['name']: l.get('matching') for l in json.load(open('config/SLSP01/libs_ref.json'))}


def drafts(u):
    for ext in ('.cpp', '.c'):
        p = 'src/%s%s' % (u, ext)
        if os.path.exists(p):
            return sum(1 for l in open(p, encoding='latin-1') if '// DRAFT:' in l)
    return 0


print('| Unidad | 100 % | casi (>= 99 %) | total | DRAFT | enlaza |')
print('|---|---|---|---|---|---|')
tot = collections.Counter()
for u in ROUTE:
    fs = units.get(u, {}).get('functions', [])
    p = [f.get('fuzzy_match_percent', 0) for f in fs]
    full = sum(1 for x in p if x == 100)
    near = sum(1 for x in p if 99 <= x < 100)
    d = drafts(u)
    link = 'si' if gu.get(u) else 'no'
    tot.update(full=full, near=near, n=len(p), d=d, link=1 if gu.get(u) else 0)
    print('| `%s` | %d | %d | %d | %d | %s |' % (u, full, near, len(p), d, link))
print('| **total** | %d | %d | %d | %d | %d de %d |' % (tot['full'], tot['near'], tot['n'], tot['d'], tot['link'],
                                                       len(ROUTE)))

# library calls of the route
splits = open('config/SLSP01/splits.txt').read()
ranges = [(int(m.group(2), 16), int(m.group(3), 16), m.group(1)) for m in
          re.finditer(r'^(\S+):\n\t\.text\s+start:0x([0-9A-F]+) end:0x([0-9A-F]+)', splits, re.M)]
addr = {}
for l in open('config/SLSP01/symbols.txt'):
    m = re.match(r'(\S+) = \.text:0x([0-9A-F]+);', l)
    if m:
        addr[m.group(1)] = int(m.group(2), 16)


def unit_of(a):
    for s, e, n in ranges:
        if s <= a < e:
            return n
    return '?'


res = collections.defaultdict(set)
for u in ROUTE:
    p = 'build/SLSP01/asm/%s.s' % u
    if not os.path.exists(p):
        continue
    for t in re.findall(r'\bbl\s+"?([^"\s]+)"?', open(p).read()):
        a = addr.get(t)
        if a is None:
            continue
        n = unit_of(a)
        if n.split('/')[0] in GAME or n == '?':
            continue
        res[n].add(t)
print()
print('| Objeto de libreria | enlaza | funciones llamadas desde la ruta |')
print('|---|---|---|')
for n in sorted(res):
    m = libs.get(n)
    st = 'si' if m else ('no' if m is not None else '?')
    names = sorted(res[n])
    s = ', '.join('`%s`' % x for x in names[:12]) + (' (+%d)' % (len(names) - 12) if len(names) > 12 else '')
    print('| `%s` | %s | %s |' % (n, st, s))
