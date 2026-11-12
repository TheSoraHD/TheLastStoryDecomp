#!/bin/bash
# noinl.sh <unidad> <Clase::metodo|funcion>: prueba __declspec(noinline) en esa definicion (excepcion
# aceptada si el original la llama con bl) y muestra el diff de fnstat; deja el cambio (revisar y
# git checkout del .cpp si no compensa). Candidatas: tools/decomp/missbl.py.
cd "$(dirname "$0")/../.."
u=$1; pat=$2; f=src/$u.cpp
python3 tools/decomp/fnstat.py $u > ${TMPDIR:-/tmp}/noinl.before
python3 - "$f" "$pat" <<'PY'
import sys, re
f, pat = sys.argv[1], sys.argv[2]
s = open(f, encoding="latin-1").read()
m = re.search(r"^([^\n]*\b" + re.escape(pat) + r"\([^\n]*\) \{)$", s, re.M)
if not m:
    sys.exit("definition not found")
s = s[:m.start()] + "__declspec(noinline) " + s[m.start():]
open(f, "w", encoding="latin-1").write(s)
PY
ninja build/SLSP01/src/$u.o 2>&1 | grep -i error
python3 tools/decomp/fnstat.py $u > ${TMPDIR:-/tmp}/noinl.after
diff ${TMPDIR:-/tmp}/noinl.before ${TMPDIR:-/tmp}/noinl.after
