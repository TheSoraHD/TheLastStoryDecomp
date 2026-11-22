#!/usr/bin/env python3
"""mutsweep.py <unit> <symbol> <function-name-in-source> [--rounds N] [--max M] [--keep] [--time S] [--only k1,k2] [--diff F] [--list]

Greedy search of small source mutations of one function body (C++), measured with objdiff-cli.
Each mutation is one natural rewrite (no asm, no volatile, no dead code):
  swap   - swap two adjacent simple statements at the same indent (declarations, assignments, calls)
  inl    - inline a local `T x = e;` used exactly once later in the body (value as an expression)
  tmp    - give a repeated `a->b` / `a.b` member chain a local of its own (auto type from a declaration
           is not available, so only for chains whose type appears in a declaration `T* v = chain;`)
  const  - add/remove `const` on a local pointer declaration
  ne0    - `if (x != 0)` <-> `if (x)`, `if (x == 0)` <-> `if (!x)`
  decl   - move a declaration without initializer one statement up/down
  mulsplit/divsplit/idx/ifid/ld/swret/cref - see mutations2 (barrido-20261121)
  lentmp - `Vector3 d = E; PSVECMag(&d)` -> `(E).Length_unk()` (bloqueos-20261122)
  val    - one or two member reads of a line through `Val_unk(...)` (bloqueos-20261122)
--diff F writes the unified diff of the best variant; --list prints the mutations without compiling.
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
plateau = opt("--plateau", 0, int)
difff = opt("--diff", None)
listonly = "--list" in args
args = [a for a in args if a not in ("--keep", "--list")]
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
    names = [fname]
    while names[-1].count("::") > 1:
        names.append(names[-1].split("::", 1)[1])
    if "::" in names[-1]:
        c, m_ = names[-1].split("::")
        if c != m_:
            names.append(m_)
    for fn in names:
        for i, l in enumerate(lines):
            if re.search(r"(?<![\w:])" + re.escape(fn) + r"\s*\(", l) and not l.startswith((" ", "\t", "//")) \
                    and not l.rstrip().endswith(";"):
                st = i; break
        if st is not None:
            break
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
            # do not swap statements that write a variable the other one reads or writes (root name of the
            # left side of `=`, `op=`, `++`/`--`), nor two calls (order of side effects)
            def lhs(l):
                m = re.match(r"^\s*(?:[\w:<>]+(?:\s*\*+|\s*&)?\s+)?(\**[\w.\[\]>-]+?)\s*(?:[-+*/%&|^]|<<|>>)?=(?!=)", l)
                if m:
                    return m.group(1).lstrip("*")
                m = re.match(r"^\s*(?:\+\+|--)?([\w.\[\]>-]+?)(?:\+\+|--)?;", l)
                return m.group(1) if m else None
            def conflict(w, other):
                if not w:
                    return False
                if w in other:
                    return True
                mr = re.match(r"\w+", w)
                if not mr:
                    return True
                r = mr.group(0)
                # the whole object (not one of its members) used in the other statement
                return re.search(r"\b%s\b(?!\s*(?:\.|->)\s*\w)" % r, other) is not None or (r == w and re.search(r"\b%s\b" % r, other))
            if conflict(lhs(lines[i]), lines[i + 1]) or conflict(lhs(lines[i + 1]), lines[i]):
                continue
            if re.search(r"\w\s*\(", lines[i]) and re.search(r"\w\s*\(", lines[i + 1]) and \
                    not (lhs(lines[i]) and lhs(lines[i + 1])):
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
            # only whole operands: `(a op b)`, `= a op b;`, `&& a op b)`... (precedence stays the same)
            pre = l[:m.start()].rstrip(" ")
            post = l[m.end():].lstrip(" ")
            if not (pre.endswith(("(", "=", ",", "return", "&&", "||", "?")) and
                    post.startswith((")", ";", ",", "&&", "||", "?", ":"))):
                continue
            if pre.endswith(("&&", "||", "?")) or post.startswith(("&&", "||", "?", ":")):
                if m.group(2) in ("+", "*"):
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
    out += mutations2(lines, a, b)
    return out

# ---- kinds added in barrido-20261121 (retouches that closed functions in grandes-20261120) ----
def top_ops(e):
    """top-level binary operators of expression e: list of (pos, op)"""
    res, depth, k = [], 0, 0
    while k < len(e):
        ch = e[k]
        if ch in "([":
            depth += 1
        elif ch in ")]":
            depth -= 1
        elif depth == 0 and k > 0:
            two = e[k:k + 2]
            if two in ("&&", "||", "==", "!=", "<=", ">=", "<<", ">>", "->"):
                if two != "->":
                    res.append((k, two))
                k += 2; continue
            if ch in "+-*/%<>&|^?:" and e[k - 1] == " " and e[k + 1:k + 2] == " ":
                res.append((k, ch))
        k += 1
    return res

ASG = re.compile(r"^(\s+)((?:const\s+)?[\w:<>]+\s+)?([\w.\[\]>-]+?) = (.+);\s*$")
CHAIN = r"[A-Za-z_]\w*(?:(?:\.|->)[A-Za-z_]\w*|\[[^\]\(]*\])+"

def nuses(name, text):
    return len(re.findall(r"(?<![\w.>])%s\b" % re.escape(name), text))

def mutations2(lines, a, b):
    out = []
    body = "\n".join(lines[a:b])
    # mulsplit: `x = E * K;` -> `x = E; x *= K;` (also `/`), when every top-level operator of the right side
    # is * or / (left associative, so the last factor is applied last: same value)
    for i in range(a, b):
        m = ASG.match(lines[i])
        if not m:
            continue
        ind, typ, lhs, rhs = m.groups()
        if typ and "&" in typ:
            continue
        ops = top_ops(rhs)
        if not ops or any(o not in ("*", "/") for _, o in ops):
            continue
        p, o = ops[-1]
        e1, k = rhs[:p].rstrip(), rhs[p + 1:].strip()
        if e1.startswith("(") and e1.endswith(")") and not top_ops(e1[1:-1]) is None:
            inner = e1[1:-1]; dep = 0
            for ch in inner:
                dep += (ch == "(") - (ch == ")")
                if dep < 0:
                    break
            else:
                e1 = inner
        mr = re.match(r"\w+", lhs)
        if not mr or re.search(r"\b%s\b" % re.escape(mr.group(0)), k):
            continue
        new = lines[:]
        new[i] = "%s%s%s = %s;\n%s%s %s= %s;" % (ind, typ or "", lhs, e1, ind, lhs, o, k)
        out.append(("mulsplit %d" % (i + 1), "\n".join(new).split("\n")))
    # divsplit: `... (v / E) ...` with v a local not used after this line -> `v /= E;` before it
    for i in range(a, b):
        l = lines[i]
        if l.lstrip().startswith(("//", "for", "while", "}")):
            continue
        for m in re.finditer(r"\(([A-Za-z_]\w*) ([/*]) ([^()]+|[^()]*\([^()]*\)[^()]*)\)", l):
            v, o, e = m.groups()
            if top_ops(e):
                continue
            dl = next((k for k in range(a, i) if re.match(r"^\s+[\w:<>]+\s+%s\b" % v, lines[k])), None)
            if dl is None or any(re.match(r"^\s+(for|while|do)\b", lines[k]) for k in range(dl, i)):
                continue
            if nuses(v, "\n".join(lines[i + 1:b])) or nuses(v, l) != 1 or re.search(r"\b%s\b" % v, e):
                continue
            ind = re.match(r"\s*", l).group(0)
            if l.lstrip().startswith(("if", "return", "else")):
                pass
            new = lines[:]
            new[i] = "%s%s %s= %s;\n%s" % (ind, v, o, e, l[:m.start()] + v + l[m.end():])
            out.append(("divsplit %d %s" % (i + 1, v), "\n".join(new).split("\n")))
    # idx: `a[f(x)] = c;` -> `int idx = f(x); a[idx] = c;`
    for i in range(a, b):
        l = lines[i]
        m = re.match(r"^(\s+)(.*?)\[([^\[\]]*\w\([^\[\]]*\))\](.*;)\s*$", l)
        if not m or l.lstrip().startswith(("if", "for", "while", "return", "//", "}")):
            continue
        ind, pre, e, post = m.groups()
        new = lines[:]
        new[i] = "%sint idx_%d = %s;\n%s%s[idx_%d]%s" % (ind, i, e, ind, pre, i, post)
        out.append(("idx %d" % (i + 1), "\n".join(new).split("\n")))
    # ifid: `if (E == K)` with E a call and K an integer literal or a name -> `int id = E; if (id == K)`
    for i in range(a, b):
        l = lines[i]
        m = re.match(r"^(\s+)(if|\} else if) \((.+?\w\(.*?\)) (==|!=) (-?\d+|0x[0-9A-Fa-f]+|[A-Za-z_]\w*)\)( \{)?\s*$", l)
        if not m or "&&" in m.group(3) or "||" in m.group(3) or m.group(2) != "if":
            continue
        ind, kw, e, op, k, br = m.groups()
        if top_ops(e):
            continue
        new = lines[:]
        new[i] = "%sint id_%d = %s;\n%sif (id_%d %s %s)%s" % (ind, i, e, ind, i, op, k, br or "")
        out.append(("ifid %d" % (i + 1), "\n".join(new).split("\n")))
    # ld: read a vector component (`X.y`, `p->m.z`) into a float local just before the statement that uses it
    for i in range(a, b):
        l = lines[i]
        if l.lstrip().startswith(("//", "for", "while", "}", "else", "case", "default")) or l.rstrip().endswith("{") \
                and not l.lstrip().startswith("if"):
            continue
        seen = set()
        for m in re.finditer(r"(?<![\w.>])(%s)" % CHAIN, l):
            c = m.group(1)
            if not re.search(r"(?:\.|->)[xyzw]$", c) or c in seen:
                continue
            seen.add(c)
            # not written in this statement
            if re.search(re.escape(c) + r"\s*(?:[-+*/]?=(?!=)|\+\+|--)", l):
                continue
            ind = re.match(r"\s*", l).group(0)
            dm = decl.match(l)
            if dm and dm.group(4).strip() == c:
                continue
            nm = "ld_%d" % len(seen) + "_%d" % i
            new = lines[:]
            new[i] = "%sfloat %s = %s;\n%s" % (ind, nm, c, l.replace(c, nm))
            out.append(("ld %d %s" % (i + 1, c), "\n".join(new).split("\n")))
    # swret: `return X;` repeated inside a switch followed by `return X;` -> `break;` (one shared return)
    for i in range(a, b):
        m = re.match(r"^(\s+)switch \(.*\) \{$", lines[i])
        if not m:
            continue
        ind = m.group(1)
        j = next((k for k in range(i + 1, b) if lines[k] == ind + "}"), None)
        if j is None:
            continue
        inner = lines[i + 1:j]
        # no loop or nested switch inside (a `break` there would mean something else)
        if any(re.match(r"^\s+(for|while|do|switch)\b", x) for x in inner):
            continue
        after = lines[j + 1] if j + 1 < b else ""
        rets = {}
        for k in range(i + 1, j):
            r = re.match(r"^\s+return\b\s*(.*?);$", lines[k])
            if r:
                rets.setdefault(r.group(1), []).append(k)
        for x, ks in rets.items():
            ra = re.match(r"^%sreturn\b\s*(.*?);$" % re.escape(ind), after)
            if ra and ra.group(1) == x:
                tail = []
            elif j + 1 == b and x:
                tail = [ind + "return %s;" % x]
            else:
                continue
            if len(ks) < 2 and not (ra and len(ks) >= 1):
                continue
            new = lines[:]
            for k in ks:
                new[k] = re.sub(r"return\b\s*.*?;$", "break;", new[k])
            new = new[:j + 1] + tail + new[j + 1:]
            out.append(("swret %d" % (i + 1), new))
    # cassign: `x op= E;` -> `x = x op E;` (and `x = E op x;` for the commutative + and *)
    for i in range(a, b):
        m = re.match(r"^(\s+)([\w.\[\]>-]+) ([-+*/|&^])= (.+);\s*$", lines[i])
        if not m:
            continue
        ind, x, o, e = m.groups()
        pe = e if not top_ops(e) else "(" + e + ")"
        new = lines[:]; new[i] = "%s%s = %s %s %s;" % (ind, x, x, o, pe)
        out.append(("cassign %d" % (i + 1), new))
        if o in "+*|&^":
            new = lines[:]; new[i] = "%s%s = %s %s %s;" % (ind, x, pe, o, x)
            out.append(("cassign %d rev" % (i + 1), new))
    # vassign: Vector3 copy through atn::VecAssign_unk (one more inline level: dst/src registers of __VecCopy)
    inc = next((k for k, l in enumerate(lines) if l.startswith("#include")), None)
    hasinc = any("math/VecAssign_unk.h" in l for l in lines)
    for i in range(a, b):
        m = re.match(r"^(\s+)((?:const )?(?:atn::)?Vector3) (\w+) = (%s|\w+);\s*$" % CHAIN, lines[i])
        m2 = re.match(r"^(\s+)(%s|\w+) = (%s|\w+);\s*$" % (CHAIN, CHAIN), lines[i])
        if m and "const" not in m.group(2):
            ind, typ, x, s = m.groups()
            rep = "%s%s %s;\n%satn::VecAssign_unk(%s, %s);" % (ind, typ, x, ind, x, s)
        elif m2 and not re.search(r"(?:\.|->)[xyzw]$", m2.group(3)) and not re.search(r"(?:\.|->)[xyzw]$", m2.group(2)):
            ind, x, s = m2.groups()
            rep = "%satn::VecAssign_unk(%s, %s);" % (ind, x, s)
        else:
            continue
        if inc is None:
            continue
        new = lines[:]
        new[i] = rep
        if not hasinc:
            new[inc] = '#include "math/VecAssign_unk.h"\n' + new[inc]
        out.append(("vassign %d" % (i + 1), "\n".join(new).split("\n")))
    # dtop: a declaration without initializer (or with literal constructor arguments) moved to the top of the
    # function (stack slots follow the order of the declarations); only if the name is declared once
    LITARGS = r"\((?:\s*-?[\d.]+f?\s*,?)+\)"
    for i in range(a + 1, b):
        m = re.match(r"^(\s+)((?:atn::)?[A-Za-z_][\w:]*) (\w+)(\[\w+\]|%s)?;\s*$" % LITARGS, lines[i])
        if not m or m.group(2) in ("return", "delete", "goto", "case", "else") or len(m.group(1)) <= 4:
            continue
        name = m.group(3)
        if m.group(2) not in ("Mtx", "Vector3", "atn::Vector3", "Matrix3x4", "atn::Matrix3x4", "float", "int", "Vec"):
            continue
        if m.group(4) and m.group(4).startswith("(") and any(re.match(r"^\s+(for|while|do)\b", l) for l in lines[a:i]):
            continue
        ndecl = sum(1 for l in lines[a:b] if re.match(r"^\s+(?:const\s+)?[A-Za-z_][\w:<>]*[\s*&]+%s\s*(?:;|\[|\(|=)" % re.escape(name), l))
        if ndecl != 1:
            continue
        new = lines[:]
        del new[i]
        new.insert(a, "    " + lines[i].strip())
        out.append(("dtop %s" % name, new))
    # byval: `p->m = atn::Vector3(e);` / `p->m = e;` through an inline taking the value by copy (setter of the
    # original: CosEditBase fn_801CD648); the helper is declared after the last #include
    lastinc = max((k for k, l in enumerate(lines) if l.startswith("#include")), default=None)
    hashelper = any("ByValAssign_unk" in l and "template" in l for l in lines)
    for i in range(a, b):
        m = re.match(r"^(\s+)(%s) = (?:atn::Vector3\((%s|\w+)\)|(%s));\s*$" % (CHAIN, CHAIN, CHAIN), lines[i])
        if not m or lastinc is None:
            continue
        ind, d, s1, s2 = m.groups()
        s = s1 or s2
        if re.search(r"(?:\.|->)[xyzw]$", d) or re.search(r"(?:\.|->)[xyzw]$", s):
            continue
        new = lines[:]
        new[i] = "%sByValAssign_unk<atn::Vector3>(%s, %s);" % (ind, d, s)
        if not hashelper:
            new[lastinc] += "\ntemplate <class T> inline void ByValAssign_unk(T& d, T v) { d = v; } // inferred: setter by value"
        out.append(("byval %d" % (i + 1), "\n".join(new).split("\n")))
    # cref: `dst = src;` (src a member chain) -> `const atn::Vector3& r = src; dst = r;`
    for i in range(a, b):
        m = re.match(r"^(\s+)(%s|\w+) = (%s);\s*$" % (CHAIN, CHAIN), lines[i])
        if not m:
            continue
        ind, d, s = m.groups()
        if re.search(r"(?:\.|->)[xyzw]$", s) or re.search(r"(?:\.|->)[xyzw]$", d):
            continue
        new = lines[:]
        new[i] = "%sconst atn::Vector3& cref_%d = %s;\n%s%s = cref_%d;" % (ind, i, s, ind, d, i)
        out.append(("cref %d" % (i + 1), "\n".join(new).split("\n")))
    # val: one or two member reads `a->mX` / `a.mX` of a line through an identity inline
    # (`Val_unk(a->mX)`): the inline call is evaluated first and changes the load order and the registers
    # (CosEditBase fn_801CDE14, RouteWalker_unk fn_80128930, bloqueos-20261122); rewrite a hit as a named
    # accessor of the class by hand
    hasval = any("Val_unk" in l and "template" in l for l in lines)
    RD = re.compile(r"(?<![\w.>&])((?:(?:this->)?[A-Za-z_]\w*(?:\[[^\]\(]*\])?(?:(?:\.|->)[A-Za-z_]\w*(?:\[[^\]\(]*\])?)*(?:\.|->))?m[A-Z0-9][A-Za-z0-9_]*)(?![\w(\[]|\s*(?:=[^=]|[-+*/|&^]=|\+\+|--|\.|->))")
    for i in range(a, b):
        l = lines[i]
        if l.strip().startswith("//") or lastinc is None:
            continue
        occ = [mm for mm in RD.finditer(l) if not re.match(r"\s*[\w:<>*& ]+\s+$", l[:mm.start()]) or "(" in l[:mm.start()]]
        occ = occ[:6]
        sets = [[o] for o in occ] + [[o1, o2] for k, o1 in enumerate(occ) for o2 in occ[k + 1:]]
        for st in sets:
            nl = l
            for o in sorted(st, key=lambda o: -o.start()):
                nl = nl[:o.start()] + "Val_unk(" + o.group(1) + ")" + nl[o.end():]
            new = lines[:]
            new[i] = nl
            if not hasval:
                new[lastinc] += "\ntemplate <class T> inline T Val_unk(const T& v) { return v; } // inferred: read through an inline"
            out.append(("val %d:%s" % (i + 1, "+".join(o.group(1) for o in st)), "\n".join(new).split("\n")))
    # lentmp: `atn::Vector3 d = E;` used once as `PSVECMag(&d)` / `PSVECSquareMag(&d)` -> `(E).Length_unk()` /
    # `(E).LengthSq_unk()`: the vector becomes a temporary (stack slot after the named locals; RouteWalker_unk
    # fn_801279E8, bloqueos-20261122)
    for i in range(a, b):
        m = re.match(r"^(\s+)(?:const\s+)?atn::Vector3 (\w+) = (.+);\s*$", lines[i])
        if not m:
            continue
        ind, name, e = m.groups()
        rest = "\n".join(lines[i + 1:b])
        if len(re.findall(r"\b%s\b" % name, rest)) != 1:
            continue
        for j in range(i + 1, min(b, i + 4)):
            mm = re.search(r"(PSVECMag|PSVECSquareMag)\(&%s\)" % name, lines[j])
            if mm:
                meth = "Length_unk" if mm.group(1) == "PSVECMag" else "LengthSq_unk"
                new = lines[:]
                new[j] = lines[j].replace(mm.group(0), "(%s).%s()" % (e, meth))
                del new[i]
                out.append(("lentmp %d" % (i + 1), new))
                break
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

if listonly:
    import difflib
    lines, a, b = body_range(orig)
    for name, new in mutations(lines, a, b):
        if only and name.split()[0] not in only.split(","):
            continue
        print("==", name)
        for d in list(difflib.unified_diff(lines, new, lineterm="", n=0))[2:]:
            if not d.startswith("@@"):
                print(d)
    raise SystemExit(0)
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
    # interleave the kinds (round robin) so that a time limit does not leave the last kinds untried
    byk = {}
    for m in muts:
        byk.setdefault(m[0].split()[0], []).append(m)
    muts = [q for grp in zip(*[v + [None] * (max(map(len, byk.values())) - len(v)) for v in byk.values()]) for q in grp if q] if byk else []
    print("round %d: %d mutations" % (r + 1, len(muts)), flush=True)
    rbest = (best, None, None)
    equal = []
    for name, new in muts:
        if tried >= mx or time.time() - t0 > tlim:
            break
        tried += 1
        v = pct("\n".join(new))
        if abs(v - best) < 1e-6:
            equal.append((name, "\n".join(new)))
        if v > rbest[0] + 1e-6:
            rbest = (v, name, "\n".join(new)); print("  better %.4f %s" % (v, name), flush=True)
        if v >= 100:
            break
    if rbest[1] is None:
        # --plateau K: take a random mutation with the same percent and go on (at most K times)
        if plateau > 0 and equal:
            plateau -= 1
            name, best_text = random.choice(equal)
            print("  plateau %s" % name, flush=True)
            continue
        break
    best, best_text = rbest[0], rbest[2]
    if best >= 100:
        break
print("best %.4f (tried %d, %.0f s)" % (best, tried, time.time() - t0))
if difff and best > base:
    import difflib
    open(difff, "w", encoding="latin-1").write("".join(difflib.unified_diff(
        orig.splitlines(True), best_text.splitlines(True), "a/" + src, "b/" + src)))
if keep and best > base:
    open(src, "w", encoding="latin-1").write(best_text)
    open(src + ".mut", "w", encoding="latin-1").write(best_text)
else:
    open(src, "w", encoding="latin-1").write(orig)
subprocess.run(["ninja", "-j1", "build/SLSP01/src/%s.o" % unit], capture_output=True)
