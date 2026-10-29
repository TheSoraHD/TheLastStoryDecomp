#!/usr/bin/env python3
"""Rewrite src/<unit>.cpp so its function definitions are in .dol order.

usage: reorder.py <unit> [--dry]

With -ipa file MWCC emits functions in source order (see ordercheck.py). This
splits the file into top-level chunks (declarations, data, functions with the
comment block right above them), pairs every emitted function with the symbol
our build/SLSP01/src/<unit>.o has for it (same order as the source), looks the
symbol up in the target object build/SLSP01/obj/<unit>.o and writes the
functions sorted by target address. Every declaration/data chunk is written
before the first function that came after it originally, so nothing is used
before it is declared, and data keeps its relative order. Functions with no
target symbol keep their place relative to the previous paired function.
Namespaces and extern "C" { } blocks are reopened around each chunk.

Refuses (exit 2) if the number of emitted chunks does not match the object.
Check afterwards: ordercheck.py, cmp.py (no new DIFF), main.dol: OK.
"""
import os, re, subprocess, sys
from elftools.elf.elffile import ELFFile

ROOT = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))
os.chdir(ROOT)
args = [a for a in sys.argv[1:] if not a.startswith("-")]
unit = args[0]
dry = "--dry" in sys.argv
src = f"src/{unit}.cpp"


def build():
    r = subprocess.run(["ninja", f"build/SLSP01/src/{unit}.o"], capture_output=True, text=True)
    if r.returncode:
        out = r.stdout
        i = out.find("###")
        print(out[i:i + 3000] if i >= 0 else out[-3000:])
        sys.exit(2)


def key(n):
    m = re.match(r"(fn_[0-9A-F]{8})", n)
    return m.group(1) if m else n


def funcs(path):
    with open(path, "rb") as f:
        elf = ELFFile(f)
        secs = list(elf.iter_sections())
        out = []
        for s in elf.get_section_by_name(".symtab").iter_symbols():
            if s["st_info"]["type"] != "STT_FUNC" or s["st_size"] == 0:
                continue
            shn = s["st_shndx"]
            if not isinstance(shn, int) or secs[shn].name != ".text":
                continue
            if s["st_info"]["bind"] == "STB_WEAK":
                continue  # inline/template copies: placed by the compiler, not by the source order
            out.append((s["st_value"], s.name))
        out.sort()
        return [n for _, n in out]


# ---- tokenizer-light scanner: finds top-level chunks -------------------------------------------
text = open(src, encoding="utf-8", errors="surrogateescape").read()


def skip_noncode(t, i):
    """If t[i] starts a comment/string/char, return index after it, else None."""
    if t.startswith("//", i):
        j = t.find("\n", i)
        return len(t) if j < 0 else j
    if t.startswith("/*", i):
        return t.index("*/", i) + 2
    if t[i] in "\"'":
        q = t[i]
        j = i + 1
        while t[j] != q:
            j += 2 if t[j] == "\\" else 1
        return j + 1
    return None


# Each item: (kind, text, scope) where kind in {"open", "close", "chunk"}; scope = tuple of openers
items = []
scope = []
i = 0
start = 0
depth = 0          # depth inside the current chunk (not counting namespaces)
n = len(text)
while i < n:
    j = skip_noncode(text, i)
    if j is not None:
        i = j
        continue
    c = text[i]
    if c == "#" and depth == 0 and (i == 0 or text[i - 1] == "\n"):
        # preprocessor line belongs to the current chunk; skip to end of line (with continuations)
        while True:
            e = text.find("\n", i)
            if e < 0:
                i = n
                break
            if text[e - 1] == "\\":
                i = e + 1
                continue
            i = e + 1
            break
        if depth == 0 and re.sub(r"//[^\n]*|/\*.*?\*/", "", text[start:i], flags=re.S).strip().startswith("#"):
            items.append(("chunk", text[start:i], tuple(scope)))
            start = i
        continue
    if c == "{":
        head = text[start:i]
        hs = re.sub(r"//[^\n]*|/\*.*?\*/", "", head, flags=re.S).strip()
        if depth == 0 and (re.fullmatch(r"namespace(\s+\w+)?", hs) or hs == 'extern "C"'):
            # keep leading comments of the opener as their own chunk
            m = re.search(r'(namespace(\s+\w+)?|extern "C")\s*$', head)
            pre = head[:m.start()]
            if pre.strip():
                items.append(("chunk", pre, tuple(scope)))
            scope.append(hs)
            items.append(("open", hs, tuple(scope)))
            i += 1
            start = i
            continue
        depth += 1
    elif c == "}":
        if depth == 0:
            if text[start:i].strip():
                items.append(("chunk", text[start:i], tuple(scope)))
            items.append(("close", scope[-1], tuple(scope)))
            scope.pop()
            i += 1
            # swallow trailing comment on the same line (e.g. "} // namespace atn")
            e = text.find("\n", i)
            e = n if e < 0 else e + 1
            i = e
            start = i
            continue
        depth -= 1
        if depth == 0:
            # end of a braced chunk; a ';' may follow (class/struct/array init)
            k = i + 1
            while k < n and text[k] in " \t":
                k += 1
            if k < n and text[k] == ";":
                i = k
            e = text.find("\n", i)
            e = n if e < 0 else e + 1
            items.append(("chunk", text[start:e], tuple(scope)))
            i = e
            start = i
            continue
    elif c == ";" and depth == 0:
        e = text.find("\n", i)
        e = n if e < 0 else e + 1
        items.append(("chunk", text[start:e], tuple(scope)))
        i = e
        start = i
        continue
    i += 1
if text[start:].strip():
    items.append(("chunk", text[start:], tuple(scope)))
if scope:
    print("unbalanced namespaces", scope)
    sys.exit(2)


def code_of(t):
    return re.sub(r"//[^\n]*|/\*.*?\*/", "", t, flags=re.S)


def split_comment(t):
    """Split a chunk into (leading text that is only blank lines/comments not touching the code,
    code with the comment block directly above it)."""
    lines = t.split("\n")
    k = 0
    while k < len(lines) and not code_of(lines[k]).strip():
        k += 1
    # walk back from the first code line over contiguous comment lines
    m = k
    while m > 0 and lines[m - 1].strip().startswith("//"):
        m -= 1
    return "\n".join(lines[:m]) + ("\n" if m else ""), "\n".join(lines[m:])


def is_function(t):
    c = code_of(t).strip()
    if c.startswith("#") or not c.endswith("}"):
        return False
    h = c[:c.find("{")] if "{" in c else c
    if re.match(r"(class|struct|union|enum|typedef|template)\b", h):
        return False
    if "=" in re.sub(r"operator\s*\S+", "", h).split("(")[0]:
        return False
    if "(" not in h:
        return False
    if re.search(r"\binline\b", h.split("(")[0]):
        return False
    return True


chunks = [(k, t, s) for k, t, s in items]

# ---- pair emitted functions with our object ---------------------------------------------------
build()
ours = [nm for nm in funcs(f"build/SLSP01/src/{unit}.o") if not nm.startswith("__sinit_")]
tgt = funcs(f"build/SLSP01/obj/{unit}.o")
tpos = {key(nm): idx for idx, nm in enumerate(tgt)}

fidx = [idx for idx, (k, t, s) in enumerate(chunks) if k == "chunk" and is_function(t)]
# static functions that were fully inlined are not in the object: drop them from pairing by name
emitted = []
oi = 0
for idx in fidx:
    t = code_of(chunks[idx][1])
    name = re.search(r"([~\w:]+|operator\s*\S+)\s*\(", t[:t.find("{")])
    name = name.group(1) if name else "?"
    last = name.split("::")[-1]
    if oi < len(ours):
        sym = ours[oi]
        ok = (last in sym) or (last.startswith("~") and "__dt__" in sym) or \
             ("operator" in last and "__" in sym) or ("__ct__" in sym and "::" in name and
                                                      name.split("::")[-2] == last)
        if ok:
            emitted.append((idx, sym))
            oi += 1
            continue
    if re.match(r"\s*static\b", t):
        emitted.append((idx, None))
        continue
    print(f"cannot pair source function {name!r} with object symbol {ours[oi] if oi < len(ours) else None!r}")
    sys.exit(2)
if oi != len(ours):
    print(f"object has {len(ours) - oi} more functions than the source chunks: {ours[oi:oi + 5]}")
    sys.exit(2)

# target position for every function chunk; unpaired ones follow the previous paired one
pos = {}
last = -1.0
for idx, sym in emitted:
    if sym is not None and key(sym) in tpos:
        last = float(tpos[key(sym)])
        pos[idx] = last
    else:
        last += 0.001
        pos[idx] = last

order = sorted(pos, key=lambda x: (pos[x], x))

# leading blank/comment text not attached to a function stays where it was
new_chunks, remap = [], {}
for idx, (k, t, sc) in enumerate(chunks):
    if idx in pos:
        pre, body = split_comment(t)
        if pre.strip():
            new_chunks.append(("chunk", pre, sc))
        remap[idx] = len(new_chunks)
        new_chunks.append((k, body, sc))
    else:
        new_chunks.append((k, t, sc))
chunks = new_chunks
pos = {remap[i]: v for i, v in pos.items()}
order = sorted(pos, key=lambda x: (pos[x], x))

# free functions whose rank changes get a prototype at their old place, so every use that saw
# the definition before still sees a declaration (with -ipa file this does not change the code)
orig_rank = {idx: r for r, idx in enumerate(sorted(pos))}
protos = {}
for r, idx in enumerate(order):
    if orig_rank[idx] == r:
        continue
    t = code_of(chunks[idx][1])
    head = t[:t.find("{")].strip()
    name = re.search(r"([~\w:]+|operator\s*\S+)\s*\(", head)
    if name is None or "::" in name.group(1):
        continue
    protos[idx] = re.sub(r"\s+", " ", head) + ";\n"

# ---- write ----------------------------------------------------------------------------------
out = []
cur = ()


def set_scope(target):
    global cur
    k = 0
    while k < len(cur) and k < len(target) and cur[k] == target[k]:
        k += 1
    for _ in range(len(cur) - k):
        opener = cur[-1]
        out.append("} // " + opener + "\n" if opener.startswith("namespace") else "}\n")
        cur = cur[:-1]
    for sname in target[k:]:
        if out and not "".join(out[-2:]).endswith("\n\n"):
            out.append("\n")
        out.append(sname + " {\n")
        cur = cur + (sname,)


done = set()
fset = set(pos)


def emit_until(limit):
    for idx in range(limit):
        if idx in fset and idx in protos and idx not in done:
            set_scope(chunks[idx][2])
            out.append(protos.pop(idx))
            continue
        if idx in done or idx in fset:
            continue
        k, t, s = chunks[idx]
        done.add(idx)
        if k != "chunk":
            continue
        set_scope(s)
        out.append(t)


for idx in order:
    emit_until(idx)
    k, body, s = chunks[idx]
    protos.pop(idx, None)
    set_scope(s)
    if not body.startswith("\n") and out and not out[-1].endswith("\n\n"):
        out.append("\n")
    out.append(body if body.endswith("\n") else body + "\n")
    done.add(idx)
emit_until(len(chunks))
set_scope(())
res = "".join(out)
res = re.sub(r"\n{3,}", "\n\n", res)

if dry:
    sys.stdout.write(res)
else:
    open(src, "w", encoding="utf-8", errors="surrogateescape").write(res)
    print(f"{src}: {len(order)} functions written in target order")
