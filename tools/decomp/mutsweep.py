#!/usr/bin/env python3
"""mutsweep.py <unit> <symbol> <function-name-in-source> [--rounds N] [--max M] [--keep] [--time S]

Greedy search of small source mutations of one function body (C++), measured with objdiff-cli.
Each mutation is one natural rewrite (no asm, no volatile, no dead code):
  swap   - swap two adjacent simple statements at the same indent (declarations, assignments, calls)
  inl    - inline a local `T x = e;` used exactly once later in the body (value as an expression)
  tmp    - give a repeated `a->b` / `a.b` member chain a local of its own (auto type from a declaration
           is not available, so only for chains whose type appears in a declaration `T* v = chain;`)
  const  - add/remove `const` on a local pointer declaration
  ne0    - `if (x != 0)` <-> `if (x)`, `if (x == 0)` <-> `if (!x)`
  decl   - move a declaration without initializer one statement up/down
Keeps the best variant of each round and starts the next round from it. Prints the best percent.
The source is restored unless --keep is given (then the best variant stays).
"""
import sys, re, os, subprocess, json, time, random
root = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))
os.chdir(root)
args = sys.argv[1:]
def opt(n, d, conv=str):
    if n in args:
        i = args.index(n); v = args[i + 1]; del args[i:i + 2]; return conv(v)
    return d
rounds = opt("--rounds", 2, int); mx = opt("--max", 200, int); tlim = opt("--time", 1500, float)
keep = "--keep" in args
only = opt("--only", None)
args = [a for a in args if a != "--keep"]
unit, sym, fname = args[:3]
src = "src/%s.cpp" % unit
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

simple = re.compile(r"^(\s+)(?!if\b|for\b|while\b|switch\b|case\b|default\b|return\b|break\b|continue\b|else\b|do\b|\}|//)[^{};]*;\s*$")
decl = re.compile(r"^(\s+)((?:const\s+)?[\w:<>]+(?:\s*\*+|\s*&)?)\s+(\w+)\s*=\s*(.+);\s*$")
declnoinit = re.compile(r"^(\s+)((?:const\s+)?[\w:<>]+(?:\s*\*+)?)\s+(\w+)(\s*\[[^\]]*\])?;\s*$")

def mutations(lines, a, b):
    out = []
    for i in range(a, b - 1):
        m1, m2 = simple.match(lines[i]), simple.match(lines[i + 1])
        if m1 and m2 and m1.group(1) == m2.group(1):
            # do not swap a declaration below a use of its name
            n1 = decl.match(lines[i]) or declnoinit.match(lines[i])
            if n1 and re.search(r"\b%s\b" % n1.group(3), lines[i + 1]):
                continue
            new = lines[:]; new[i], new[i + 1] = lines[i + 1], lines[i]
            out.append(("swap %d" % (i + 1), new))
    body = "\n".join(lines[a:b])
    for i in range(a, b):
        m = decl.match(lines[i])
        if not m:
            continue
        ind, typ, name, expr = m.groups()
        if "&" in typ:
            continue
        rest = "\n".join(lines[i + 1:b])
        uses = re.findall(r"(?<![\w.>])%s\b" % name, rest)
        assigned = re.search(r"(?<![\w.>])%s\s*(=[^=]|\+=|-=|\+\+|--)|&%s\b|\+\+%s\b|--%s\b" % ((name,) * 4), rest)
        if len(uses) == 1 and not assigned:
            new = lines[:]
            e = expr.strip()
            if not re.fullmatch(r"[\w.>\-\[\]]+", e):
                e = "(" + e + ")"
            del new[i]
            for j in range(i, b - 1):
                if re.search(r"(?<![\w.>])%s\b" % name, new[j]):
                    new[j] = re.sub(r"(?<![\w.>])%s\b" % name, lambda _: e, new[j], count=1); break
            out.append(("inl %s@%d" % (name, i + 1), new))
        if typ.endswith("*"):
            new = lines[:]
            if typ.startswith("const "):
                new[i] = ind + typ[6:] + " " + name + " = " + expr + ";"
            else:
                new[i] = ind + "const " + typ + " " + name + " = " + expr + ";"
            out.append(("const %s@%d" % (name, i + 1), new))
    for i in range(a, b):
        l = lines[i]
        for pat, rep in ((r"if \((\w[\w.>\-\[\]]*) != 0\)", r"if (\1)"), (r"if \((\w[\w.>\-\[\]]*) == 0\)", r"if (!\1)"),
                         (r"if \(!(\w[\w.>\-\[\]]*)\)", r"if (\1 == 0)")):
            if re.search(pat, l):
                new = lines[:]; new[i] = re.sub(pat, rep, l, count=1)
                out.append(("ne0 %d" % (i + 1), new))
        m = declnoinit.match(l)
        if m:
            for d in (-1, 1):
                j = i + d
                if a <= j < b and simple.match(lines[j]) and not re.search(r"\b%s\b" % m.group(3), lines[j]):
                    new = lines[:]; new[i], new[j] = lines[j], lines[i]
                    out.append(("decl %s %+d" % (m.group(3), d), new))

    # split / ternary forms of declarations
    for i in range(a, b):
        m = decl.match(lines[i])
        if not m:
            continue
        ind, typ, name, expr = m.groups()
        if "&" in typ or "[" in name:
            continue
        new = lines[:]; new[i] = "%s%s %s;\n%s%s = %s;" % (ind, typ, name, ind, name, expr)
        out.append(("split %s@%d" % (name, i + 1), "\n".join(new).split("\n")))
        t = re.fullmatch(r"\(?([^?]+?)\)?\s*\?\s*([^:]+?)\s*:\s*(.+)", expr.strip())
        if t and expr.count("?") == 1:
            c, x, y = t.groups()
            new = lines[:]; new[i] = "%s%s %s = %s;\n%sif (%s) {\n%s    %s = %s;\n%s}" % (ind, typ, name, y, ind, c, ind, name, x, ind)
            out.append(("tern %s@%d" % (name, i + 1), "\n".join(new).split("\n")))
        # move the declaration past the next declaration (both pure declarations)
        if i + 1 < b:
            m2 = decl.match(lines[i + 1])
            if m2 and m2.group(1) == ind and not re.search(r"\b%s\b" % name, lines[i + 1]):
                pass  # covered by swap
    # if (x) v = a; patterns: `T v = a; if (c) { v = b; }` -> ternary
    for i in range(a, b - 3):
        m = decl.match(lines[i])
        if not m:
            continue
        ind, typ, name, expr = m.groups()
        c = re.match(r"^%sif \((.+)\) \{$" % re.escape(ind), lines[i + 1])
        asg = re.match(r"^%s    %s = (.+);$" % (re.escape(ind), name), lines[i + 2])
        if c and asg and lines[i + 3] == ind + "}":
            new = lines[:i] + ["%s%s %s = %s ? %s : %s;" % (ind, typ, name, c.group(1), asg.group(1), expr)] + lines[i + 4:]
            out.append(("toternary %s@%d" % (name, i + 1), new))
    # nest `if (A && B) {` without else (top-level && only, no || in the condition)
    for i in range(a, b):
        m = re.match(r"^(\s+)if \((.+)\) \{$", lines[i])
        if not m or "||" in m.group(2):
            continue
        ind, cond = m.groups()
        parts = split_and(cond)
        if len(parts) < 2:
            continue
        j = next((k for k in range(i + 1, b) if lines[k].startswith(ind + "}")), None)
        if j is None or lines[j].strip() != "}":
            continue
        inner = [("    " + l if l else l) for l in lines[i + 1:j]]
        new = lines[:i] + ["%sif (%s) {" % (ind, parts[0]), "%s    if (%s) {" % (ind, " && ".join(parts[1:]))] + inner + \
              [ind + "    }", ind + "}"] + lines[j + 1:]
        out.append(("nest %d" % (i + 1), new))
    # float literal operand order and x * 0.5f <-> x / 2.0f (exact for powers of two)
    LIT = r"\d+\.\d*f"
    SIMPLE = r"[A-Za-z_][\w]*(?:(?:\.|->)[A-Za-z_]\w*|\[[^\]]*\]|\(\))*"
    for i in range(a, b):
        l = lines[i]
        for m in re.finditer(r"(?<![*/] )(?<![\w.>\])])(%s) \* (%s)(?![\w.\[(])" % (LIT, SIMPLE), l):
            new = lines[:]; new[i] = l[:m.start()] + m.group(2) + " * " + m.group(1) + l[m.end():]
            out.append(("fswap %d lit*x" % (i + 1), new))
        for m in re.finditer(r"(?<![*/] )(?<![\w.>\])])(%s) \* (%s)(?![\w.\[(])" % (SIMPLE, LIT), l):
            new = lines[:]; new[i] = l[:m.start()] + m.group(2) + " * " + m.group(1) + l[m.end():]
            out.append(("fswap %d x*lit" % (i + 1), new))
            k = {"0.5f": "2.0f", "0.25f": "4.0f", "0.125f": "8.0f"}.get(m.group(2))
            if k:
                new = lines[:]; new[i] = l[:m.start()] + m.group(1) + " / " + k + l[m.end():]
                out.append(("fswap %d x/k" % (i + 1), new))
    # commutative operand swap: a == b <-> b == a (also !=), a < b <-> b > a (and <=, >=), a + b, a * b
    OPS = {"==": "==", "!=": "!=", "<": ">", ">": "<", "<=": ">=", ">=": "<=", "+": "+", "*": "*"}
    for i in range(a, b):
        l = lines[i]
        if l.lstrip().startswith(("//", "#")):
            continue
        for m in re.finditer(r"(?<![\w.>\])\-+*/&|])(%s) (==|!=|<=|>=|<|>|\+|\*) (%s)(?![\w.\[(])" % (SIMPLE, SIMPLE), l):
            if m.group(1) == m.group(3):
                continue
            new = lines[:]; new[i] = l[:m.start()] + m.group(3) + " " + OPS[m.group(2)] + " " + m.group(1) + l[m.end():]
            out.append(("cswap %d %s" % (i + 1, m.group(2)), new))
    # split `if (A || B) {` whose body is a single return/continue/break into two ifs
    for i in range(a, b - 2):
        m = re.match(r"^(\s+)if \((.+)\) \{$", lines[i])
        if not m or "&&" in m.group(2):
            continue
        ind, cond = m.groups()
        parts = split_or(cond)
        if len(parts) < 2 or lines[i + 2] != ind + "}":
            continue
        body = lines[i + 1]
        if not re.match(r"^\s+(return\b[^;]*|continue|break);$", body):
            continue
        new = lines[:i]
        for q in parts:
            new += ["%sif (%s) {" % (ind, q), body, ind + "}"]
        new += lines[i + 3:]
        out.append(("splitor %d" % (i + 1), new))
    return out

def split_or(c):
    parts, depth, cur, k = [], 0, "", 0
    while k < len(c):
        ch = c[k]
        if ch in "([":
            depth += 1
        elif ch in ")]":
            depth -= 1
        if depth == 0 and c[k:k + 4] == " || ":
            parts.append(cur); cur = ""; k += 4; continue
        cur += ch; k += 1
    parts.append(cur)
    return parts

def split_and(c):
    parts, depth, cur, k = [], 0, "", 0
    while k < len(c):
        ch = c[k]
        if ch in "([":
            depth += 1
        elif ch in ")]":
            depth -= 1
        if depth == 0 and c[k:k + 4] == " && ":
            parts.append(cur); cur = ""; k += 4; continue
        cur += ch; k += 1
    parts.append(cur)
    return parts

base = pct(orig)
print("base %.4f" % base, flush=True)
best_text, best = orig, base
tried = 0
for r in range(rounds):
    lines, a, b = body_range(best_text)
    muts = mutations(lines, a, b)
    if only:
        muts = [m for m in muts if m[0].split()[0] in only.split(",")]
    random.seed(r)
    print("round %d: %d mutations" % (r + 1, len(muts)), flush=True)
    rbest = (best, None, None)
    for name, new in muts:
        if tried >= mx or time.time() - t0 > tlim:
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
    if best >= 100:
        break
print("best %.4f (tried %d, %.0f s)" % (best, tried, time.time() - t0))
if keep and best > base:
    open(src, "w", encoding="latin-1").write(best_text)
    open(src + ".mut", "w", encoding="latin-1").write(best_text)
else:
    open(src, "w", encoding="latin-1").write(orig)
subprocess.run(["ninja", "-j1", "build/SLSP01/src/%s.o" % unit], capture_output=True)
