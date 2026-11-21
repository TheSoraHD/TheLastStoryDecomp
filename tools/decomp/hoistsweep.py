#!/usr/bin/env python3
"""hoistsweep.py <unit> <symbol> <function-name-in-source> [--rounds N] [--time S] [--keep]

Greedy search of "value in a local" / "local back to an expression" rewrites in one function body,
measured with objdiff-cli. Mutations (one per variant):
  arg   - an argument of a call (not a plain name or literal) moved into a local just before the statement:
          `__typeof__(e) hN = e;` (the type is written by hand afterwards if the variant is kept)
  rhs   - the right-hand side of an assignment/compare inside a statement or `if` condition, same way
  inl   - a local `T x = e;` used once later replaced by its expression (see mutsweep.py)
Review every kept variant by hand: a hoisted argument passed to a non-const reference changes the meaning.
"""
import sys, re, os, subprocess, json, time
root = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))
os.chdir(root)
args = sys.argv[1:]
def opt(n, d, conv=str):
    if n in args:
        i = args.index(n); v = args[i + 1]; del args[i:i + 2]; return conv(v)
    return d
rounds = opt("--rounds", 2, int); tlim = opt("--time", 540, float); src = opt("--file", None)
keep = "--keep" in args
args = [a for a in args if a != "--keep"]
unit, sym, fname = args[:3]
src = src or "src/%s.cpp" % unit
orig = open(src, encoding="latin-1").read()
t0 = time.time()

def pct(text):
    open(src, "w", encoding="latin-1").write(text)
    r = subprocess.run(["ninja", "-j1", "build/SLSP01/src/%s.o" % unit], capture_output=True, text=True)
    if r.returncode != 0:
        return -1
    o = subprocess.run(["build/tools/objdiff-cli", "diff", "-p", ".", "-u", "main/" + unit, sym, "-o", "-",
                        "--format", "json"], capture_output=True, text=True).stdout
    try:
        d = json.loads(o)
    except Exception:
        return -1
    for s in d.get("left", {}).get("symbols", []):
        if s.get("name") == sym:
            return s.get("match_percent") or 0
    return -1

def body_range(text):
    lines = text.split("\n")
    st = None
    for i, l in enumerate(lines):
        if re.search(r"\b" + re.escape(fname) + r"\s*\(", l) and not l.startswith((" ", "\t", "//")) \
                and not l.rstrip().endswith(";"):
            st = i; break
    if st is None:
        raise SystemExit("function not found: " + fname)
    while not lines[st].rstrip().endswith("{"):
        st += 1
    en = next(i for i in range(st + 1, len(lines)) if lines[i].startswith("}"))
    return lines, st + 1, en

def calls(line):
    """(start, end) spans of the top-level arguments of every call in the line."""
    out = []
    for m in re.finditer(r"[\w\]\)]\s*\(", line):
        i = m.end()  # after '('
        depth = 1; start = i; j = i
        while j < len(line) and depth:
            c = line[j]
            if c in "([":
                depth += 1
            elif c in ")]":
                depth -= 1
                if depth == 0:
                    if line[start:j].strip():
                        out.append((start, j))
                    break
            elif c == "," and depth == 1:
                out.append((start, j)); start = j + 1
            j += 1
    return out

simple_tok = re.compile(r"^\s*(&?[A-Za-z_]\w*|-?[0-9.]+f?|0x[0-9A-Fa-f]+|\"[^\"]*\"|L\"[^\"]*\"|'.')\s*$")
ctrl = re.compile(r"^\s*(if|for|while|switch|return|case|default|else|do|\}|//)")

def mutations(lines, a, b, nxt):
    out = []
    for i in range(a, b):
        l = lines[i]
        if not l.strip() or l.strip().startswith("//"):
            continue
        ind = re.match(r"^(\s*)", l).group(1)
        is_if = re.match(r"^\s*(\} else )?if \(", l)
        if ctrl.match(l) and not is_if and not re.match(r"^\s*return ", l):
            continue
        if not (l.rstrip().endswith(";") or l.rstrip().endswith("{")):
            continue
        if is_if and l.strip().startswith("}"):
            continue  # an else-if cannot get a statement before it
        for (s, e) in calls(l):
            arg = l[s:e].strip()
            if simple_tok.match(arg) or not arg or arg.startswith("&") and re.fullmatch(r"&[\w.>\-\[\]]+", arg):
                continue
            if "++" in arg or "--" in arg or "=" in arg.replace("==", "").replace("!=", "").replace("<=", "").replace(">=", ""):
                continue
            name = "h%d" % nxt[0]; nxt[0] += 1
            new = lines[:]
            new[i] = l[:s] + (" " if l[s:s+1] == " " else "") + name + l[e:]
            new.insert(i, "%s__typeof__(%s) %s = %s;" % (ind, arg, name, arg))
            out.append(("arg %d %s" % (i + 1, arg[:40]), new))
        m = re.match(r"^(\s*)([\w.>\-\[\]\*]+) (=|\+=|-=|\*=) (.+);\s*$", l)
        if m and not simple_tok.match(m.group(4)) and "(" not in m.group(2):
            rhs = m.group(4)
            name = "h%d" % nxt[0]; nxt[0] += 1
            new = lines[:]
            new[i] = "%s%s %s %s;" % (m.group(1), m.group(2), m.group(3), name)
            new.insert(i, "%s__typeof__(%s) %s = %s;" % (ind, rhs, name, rhs))
            out.append(("rhs %d %s" % (i + 1, rhs[:40]), new))
    # inline a local used once
    decl = re.compile(r"^(\s+)((?:const\s+)?[\w:<>]+(?:\s*\*+)?)\s+(\w+)\s*=\s*(.+);\s*$")
    for i in range(a, b):
        m = decl.match(lines[i])
        if not m:
            continue
        ind, typ, name, expr = m.groups()
        rest = "\n".join(lines[i + 1:b])
        uses = re.findall(r"(?<![\w.>])%s\b" % name, rest)
        if len(uses) == 1 and not re.search(r"(?<![\w.>])%s\s*(=[^=]|\+=|-=|\+\+|--)|&%s\b" % (name, name), rest):
            new = lines[:]
            e = expr.strip()
            if not re.fullmatch(r"[\w.>\-\[\]]+", e):
                e = "(" + e + ")"
            del new[i]
            for j in range(i, b - 1):
                if re.search(r"(?<![\w.>])%s\b" % name, new[j]):
                    new[j] = re.sub(r"(?<![\w.>])%s\b" % name, lambda _: e, new[j], count=1); break
            out.append(("inl %s@%d" % (name, i + 1), new))
    return out

base = pct(orig)
print("base %.4f" % base, flush=True)
best_text, best = orig, base
nxt = [0]
tried = 0
for r in range(rounds):
    lines, a, b = body_range(best_text)
    muts = mutations(lines, a, b, nxt)
    print("round %d: %d mutations" % (r + 1, len(muts)), flush=True)
    rbest = (best, None, None)
    for name, new in muts:
        if time.time() - t0 > tlim:
            break
        tried += 1
        v = pct("\n".join(new))
        if v > rbest[0] + 1e-6:
            rbest = (v, name, "\n".join(new)); print("  better %.4f %s" % (v, name), flush=True)
        if v >= 100:
            break
    if rbest[1] is None:
        break
    best, best_text = rbest[0], rbest[2]
    if best >= 100 or time.time() - t0 > tlim:
        break
print("best %.4f (tried %d, %.0f s)" % (best, tried, time.time() - t0))
if best > base:
    open("/home/claude/scratch/hoist_best.cpp", "w", encoding="latin-1").write(best_text)
if keep and best > base:
    open(src, "w", encoding="latin-1").write(best_text)
else:
    open(src, "w", encoding="latin-1").write(orig)
subprocess.run(["ninja", "-j1", "build/SLSP01/src/%s.o" % unit], capture_output=True)
