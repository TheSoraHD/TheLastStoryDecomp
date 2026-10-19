#!/usr/bin/env python3
"""List decompilation targets from build/SLSP01/report.json (run `ninja` first).

usage: targets.py [--max-unit 1024] [--max-func 0] [--limit 50] [--lib]
  default : split units (not auto_*) with NO source yet and .text <= --max-unit bytes,
            smallest first: name, code bytes, function count, classes (game_units.json).
  --funcs N : instead, functions <= N bytes at 0% inside units that already have source.
  --lib     : include library units (RVL_SDK, nw4r, dwc, gamespy, MSL...); default is game only.
A unit "has source" when build/SLSP01/src/<unit>.o exists (i.e. configure found a source file).
"""
import json, os, sys

ROOT = os.environ.get("CMP_ROOT", os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..")))
os.chdir(ROOT)
args = sys.argv[1:]
def opt(name, default):
    if name in args:
        return int(args[args.index(name) + 1], 0)
    return default
max_unit = opt("--max-unit", 1024)
max_func = opt("--funcs", 0)
limit = opt("--limit", 50)
want_lib = "--lib" in args

r = json.load(open("build/SLSP01/report.json"))
classes = {}
if os.path.exists("config/SLSP01/game_units.json"):
    for e in json.load(open("config/SLSP01/game_units.json")):
        classes[e["name"].rsplit(".", 1)[0]] = e.get("classes", [])
libnames = set()
if os.path.exists("config/SLSP01/libs_ref.json"):
    libnames = {e["name"].rsplit(".", 1)[0] for e in json.load(open("config/SLSP01/libs_ref.json"))}
LIBPREFIX = ("RVL_SDK/", "nw4r/", "dwc/", "gamespy/", "MSL", "PowerPC_EABI_Support/", "smg2/", "boost/", "Runtime")

rows = []
for u in r["units"]:
    name = u["name"].split("/", 1)[1]
    if name.startswith("auto_") or not u.get("functions"):
        continue
    is_lib = name.startswith(LIBPREFIX) or name in libnames
    if is_lib and not want_lib:
        continue
    has_src = os.path.exists(f"build/SLSP01/src/{name}.o")
    fs = u["functions"]
    if max_func:
        if not has_src:
            continue
        for f in fs:
            if f.get("fuzzy_match_percent", 0) == 0 and int(f.get("size", 0)) <= max_func:
                va = int(f.get("metadata", {}).get("virtual_address", 0))
                rows.append((int(f["size"]), f"{int(f['size']):5d}  {va:08X}  {name}  {f['name']}"))
    else:
        if has_src:
            continue
        code = int(u["measures"].get("total_code", 0))
        if code == 0 or code > max_unit:
            continue
        cl = ",".join(classes.get(name, []))[:80]
        rows.append((code, f"{code:5d}  {len(fs):3d}f  {name}  {cl}"))

rows.sort()
for _, line in rows[:limit]:
    print(line)
print(f"# {len(rows)} targets ({'functions' if max_func else 'units'}), showing {min(limit, len(rows))}")
