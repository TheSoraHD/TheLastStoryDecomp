"""Helpers shared by the scan tools (symdiff, missbl, sizepair, rep100)."""
import json, os, subprocess

ROOT = os.environ.get("CMP_ROOT", os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..")))
os.chdir(ROOT)


def units(args):
    """Units given on the command line, or every unlinked game unit with source (--all / none)."""
    names = [a for a in args if not a.startswith("--")]
    if names:
        return names
    out = []
    for u in json.load(open("config/SLSP01/game_units.json")):
        n = u["name"].rsplit(".", 1)[0]
        if not u.get("matching") and os.path.exists("src/" + u["name"]) and os.path.exists("build/SLSP01/src/%s.o" % n):
            out.append(n)
    return out


def unit_diff(u):
    """objdiff of a whole unit (left = target, right = ours), or None."""
    out = subprocess.run(["build/tools/objdiff-cli", "diff", "-p", ".", "-u", "main/" + u, "-o", "-",
                          "--format", "json"], capture_output=True, text=True).stdout
    try:
        return json.loads(out)
    except ValueError:
        return None


def nm(path):
    return subprocess.run(["build/binutils/powerpc-eabi-nm", "-S", "-n", path], capture_output=True, text=True).stdout
