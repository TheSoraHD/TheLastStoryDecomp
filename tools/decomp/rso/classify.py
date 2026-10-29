#!/usr/bin/env python3
"""Classify located RSO names against the EU symbols and the EU sources."""
import json
import re
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).parent))
from common import ROOT, SCR, eu_symbols  # noqa
from toolsonly import tokens, tools_only  # noqa

NOT_TOOLS = {"Iterator", "node_base", "rep_s", "sentry", "value_compare", "__generic_iterator",
             "D_T", "PC", "RC", "No_Op", "WPADInfo", "_GXTevBias", "_GXTevScale", "_GXTexObj",
             "TPLPalette", "HBMKPadData"}
ANON = re.compile(r"^(fn_|lbl_|func_|auto_|dtor_|jumptable_|gap_|@)")


def main():
    d = json.loads((SCR / "rsomatch.json").read_text())
    tools = tools_only() - NOT_TOOLS
    syms = eu_symbols()
    at = {}
    for s in syms:
        at.setdefault(s["addr"], []).append(s)
    eu_names = {s["name"] for s in syms}
    src = subprocess.run(["grep", "-rhoE", r"(fn|lbl|dtor|func|gap|jumptable)_8[0-9A-Fa-f]{7}", str(ROOT / "src"), str(ROOT / "include"), str(ROOT / "libs")],
                         capture_output=True, text=True).stdout.split()
    used = {x.upper()[-8:] for x in src}
    res = {"same": [], "apply": [], "in_source": [], "differs": [], "tools_class": [], "taken": [], "nosym": []}
    for nm, v in d["names"].items():
        a = int(v["addr"], 16)
        rec = dict(name=nm, addr=v["addr"], votes=v["votes"], kind=v["kind"], eu=v["eu"])
        if nm in v["eu"]:
            res["same"].append(rec); continue
        tk = {t for t in tools if re.search(r"(?:(?<![0-9])|(?<=Q[0-9]))%d%s" % (len(t), re.escape(t)), nm)}
        if tk:
            rec["tools"] = sorted(tk); res["tools_class"].append(rec); continue
        cur = [s for s in at.get(a, [])]
        if not cur:
            res["nosym"].append(rec); continue
        if nm in eu_names:
            res["taken"].append(rec); continue
        s = cur[0]
        rec["size"] = s["size"]
        if not ANON.match(s["name"]):
            res["differs"].append(rec)
        elif ("%08X" % a) in used:
            res["in_source"].append(rec)
        else:
            res["apply"].append(rec)
    (SCR / "classified.json").write_text(json.dumps(res, indent=1))
    for k, v in res.items():
        print(k, len(v))


if __name__ == "__main__":
    main()
