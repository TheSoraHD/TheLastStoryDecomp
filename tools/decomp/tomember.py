#!/usr/bin/env python3
"""tomember.py <func> <file.cpp...>: turns calls `func(obj, args...)` of an extern "C" with a `self`
into member calls (`obj->func(args)` / `x.func(args)` for `&x`) and removes the extern declaration.
Review the diff (casts of the object stay as written), rebuild and pd.sh the units."""
import re, sys


def split_args(s):
    depth, out, cur = 0, [], ""
    for ch in s:
        if ch in "([{":
            depth += 1
        if ch in ")]}":
            depth -= 1
        if ch == "," and depth == 0:
            out.append(cur.strip())
            cur = ""
        else:
            cur += ch
    out.append(cur.strip())
    return out


fn = sys.argv[1]
for f in sys.argv[2:]:
    s = open(f, encoding="latin-1").read()
    s = re.sub(r'^extern "C"[^\n;]*\b' + fn + r"\([^;]*;[^\n]*\n", "", s, flags=re.M)
    res, i, n = "", 0, 0
    while True:
        j = s.find(fn + "(", i)
        if j < 0:
            res += s[i:]
            break
        if j > 0 and (s[j - 1].isalnum() or s[j - 1] in "_>.:"):
            res += s[i:j + len(fn)]
            i = j + len(fn)
            continue
        k, depth = j + len(fn) + 1, 1
        while depth:
            depth += {"(": 1, ")": -1}.get(s[k], 0)
            k += 1
        args = split_args(s[j + len(fn) + 1:k - 1])
        obj = args[0]
        if re.fullmatch(r"&[\w.\[\]>-]+", obj):
            call = obj[1:] + "."
        elif re.fullmatch(r"[\w.\[\]>-]+(\(\))?", obj):
            call = obj + "->"
        else:
            call = "(" + obj + ")->"
        res += s[i:j] + call + fn + "(" + ", ".join(args[1:]) + ")"
        i, n = k, n + 1
    open(f, "w", encoding="latin-1").write(res)
    print(f, n)
