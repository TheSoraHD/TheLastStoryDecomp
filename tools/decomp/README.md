# tools/decomp

Helper scripts used while decompiling. All of them work on the repo that contains them
(or on `$CMP_ROOT` if set, e.g. a sandbox copy). Run `ninja` first: they read `build/SLSP01/`.

| Script | Use |
|---|---|
| `targets.py` | List targets from `report.json`: unsourced split units <= 1 KB (`--max-unit`), or `--funcs N` small 0% functions in units with source; `--lib` includes libraries. |
| `fasm.py <unit> <addr\|name>...` | Compact target asm of functions. |
| `cmp.py <unit> [filter] [-v] [--nobuild]` | Compile the unit and compare every function (names/pools normalised). Does not compare branch targets: final check is objdiff/report.json. |
| `fnstat.py <unit> [filter]` | Per-function status of a unit. |
| `poolorder.py [--all] [--funcs [--mixed]] [unit...]` | Order of the `@stringBase0` strings vs the original object (same / order / content, first differing index); `--funcs` lists functions < 100% whose only differences are pool offsets. |
| `ordercheck.py <unit> [-v] [--nobuild]` | Functions of our object out of `.dol` order (with `-ipa file` the source order is the emission order), missing and extra; weak copies ignored. Exit 1 if anything is out of order. |
| `reorder.py <unit> [--dry]` | Rewrite `src/<unit>.cpp` with its functions in `.dol` order; keeps declarations/data before their uses and adds prototypes for moved free functions. Check with `cmp.py` afterwards. |
| `odiff2.py <unit> <symbol>` | Differing instructions of one function (objdiff-cli). |
| `variants.py <unit> <filter> <file>` | Try alternative snippets (blocks separated by `====`), keep the first that matches. |
| `try.sh <file.cpp>` | Standalone compile with the game flags, prints disassembly. |
| `xref.py build` / `xref.py <name> [ctx] [max]` | Cross-reference index of the target asm (`build/xref.pkl`). |
| `callers.py <name> [ctx] [max]` | Call sites of a function in the target asm. |
| `datarefs.py <unit> [section]` | Per function, number of distinct data objects referenced (ours vs target). |
| `pairnames.py <unit> [--data] [--apply]` | Rename target `fn_`/`lbl_` symbols in symbols.txt from our object (check pairings). |
| `symmerge.py config/SLSP01/symbols.txt` | Resolve git conflict blocks in symbols.txt by address, preferring real names. |
| `regress.py <report.json> <ref report.json>...` | Per-function comparison of two builds: functions at 100% in a reference that are not any more (integration check). |
| `symresolve.py config/SLSP01/symbols.txt` | Resolve symbols.txt conflict blocks by address (real name over `fn_`/`lbl_`); fallback when `symmerge.py` finds nothing. |
| `refmerge.py <other libs_ref.json>` | Add missing libs_ref.json entries from another copy. |
| `autoscan.py [--min N] [--md f] [--tsv f]` / `--unit <auto_unit>` / `--data` | Inventory of the auto_ (unsplit) .text ranges with split hints: classes whose vtable methods are in the range, TU boundary candidates from constants duplicated per file, neighbour units; per-function view of one range; data objects left in auto_ data ranges that only one split unit uses. |

`maps/map_<unit>.txt`: optional address -> mangled-name map used by `cmp.py` for calls whose target
symbol is still `fn_<ADDR>` but our code uses a real name.
- `variants_try.py <unidad> <símbolo> <fichero> <variantes.txt>`: aplica cada bloque `VIEJO\n-----\nNUEVO` (separados por `=====`) por separado, compila la unidad (~2 s) e imprime el porcentaje de objdiff del símbolo; restaura el fichero al final.
- `pct.sh <unidad> [símbolo...]`: porcentaje de objdiff por función (todas si no se dan símbolos); más fiable que el recuento de líneas de `cmp.py` cuando cambian nombres de datos.
- `rso/` (`vtmatch.py`, `rsomatch.py`, `classify.py`, `apply.py`): real names from the USA `LastWorld_tools.rso` (`orig/SLSEXJ/LastWorld_tools.rso` or `$RSO_USA`, not committed) mapped onto EU addresses by identical code and relocations; outputs in `build/rso/`. See `docs/rso-names.md`.
- `resetlocal.py <unidad>`: devuelve el nombre `lbl_<dir>` a los `@NNNN` de los datos de la unidad en `symbols.txt` (antes de repetir `pairdata.py --apply` tras editar el fuente).

## Library sources (SDK, nw4hbm, smg2)

- `sdkcheck.py <src> [<libs_ref unit for the flags>]`: compiles a `libs/` source and compares each
  function with main.dol (relocations masked). `COMPILERS=/path/to/compilers` if they are not in
  `build/compilers`.
- `sdkauto.py <src> [--like <unit>]`: if everything matches, proposes the split ranges.
- `addsplit.py [--like <unit>] <name> "text A B" "data A B" ...`: inserts the split (sorted by
  `.text`) and the `libs_ref.json` entry. Then configure + `ninja` (main.dol: OK is the check).

- `pd.sh <unidad>`: `pairdata --relabel --apply` con reversión si baja fnstat (auto-20261101b). Desde 2026-11-03 sin `resetlocal`: un solo `dtk split` (~1 min) en vez de dos; vale también para CharaTask, GameScene y Model (antes 40 min o por pasos).
- `pdbatch.sh <unidad...>`: lo mismo que `pd.sh` para varias unidades con un solo `dtk split` y reversión por unidad (solo sus líneas de `symbols.txt`); 10 unidades en ~1,5 min (solo-20261103b). Revisar después `__vt__`/`sInstance` en el diff.
- `pairdata.py <unidad> [--apply] [--local] [--relabel]`: una sola llamada a objdiff por unidad (segundos; antes una por función). `--relabel` re-empareja también las `@NNNN` desfasadas (solo dentro de los rangos de datos de la unidad); `--local` solo propone nombres `@NNNN`/`@stringBase` (usarlo en lotes de muchas unidades).
- `labelcheck.py <unidad...>`: etiquetas `@N` de los datos del split que faltan en nuestro `.o` (tras tocar una cabecera, repetir `pd.sh` en esas unidades; report.json no lo detecta).
- `gap.py <report.json>`: unidades con más 100% en report.json que en objdiff (candidatas a pairdata).
- `blonly.py`: funciones que solo fallan por el destino de llamadas (`bl X | bl Y`) y nombres de etiquetas.
- `tomethod.py FN fichero...`: convierte llamadas `extern "C" FN(obj, ...)` en `obj->FN(...)`.

## Escaneos rápidos (2026-11-03, `solo-20261103`)

Sin argumentos recorren todas las unidades del juego sin enlazar que tienen fuente (~1 min cada uno; una llamada a objdiff por unidad). Repetirlos tras cada lote.

- `symdiff.py [unidad...]`: funciones < 100% cuyas diferencias son solo nombres de símbolos (llamadores `extern "C"` de métodos con nombre mangled en `symbols.txt`, `@N` desfasadas → `pd.sh`, copias débiles aún `fn_`).
- `missbl.py [unidad...]`: el original llama con `bl` a una función del mismo tramo que nosotros inlineamos → `noinl.sh`, receta del presupuesto de inlining o corte de fichero si las funciones siguientes del original sí la inlinean.
- `sizepair.py [unidad...]`: propone nombres para `fn_XXXXXXXX` sin pareja emparejando por tamaño con nuestros símbolos sin pareja (solo propuestas: aplicar, compilar y quedarse con las que dan 100%).
- `rep100.py [unidad...]`: funciones que `report.json` da al 100% y objdiff no (no cuentan, regla 1).
- `tomember.py <func> <ficheros...>`: convierte llamadas `func(obj, ...)` de una `extern "C"` con `self` en llamadas a método y quita la declaración (revisar el diff).
- `noinl.sh <unidad> <Clase::metodo>`: prueba `__declspec(noinline)` en esa definición y muestra el cambio de `fnstat`.
- `scanlib.py`: funciones comunes.
