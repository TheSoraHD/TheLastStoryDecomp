# tools/decomp

Helper scripts used while decompiling. All of them work on the repo that contains them
(or on `$CMP_ROOT` if set, e.g. a sandbox copy). Run `ninja` first: they read `build/SLSP01/`.

| Script | Use |
|---|---|
| `targets.py` | List targets from `report.json`: unsourced split units <= 1 KB (`--max-unit`), or `--funcs N` small 0% functions in units with source; `--lib` includes libraries. |
| `fasm.py <unit> <addr\|name>...` | Compact target asm of functions. |
| `cmp.py <unit> [filter] [-v] [--nobuild]` | Compile the unit and compare every function (names/pools normalised). Does not compare branch targets: final check is objdiff/report.json. |
| `fnstat.py <unit> [filter]` | Per-function status of a unit. |
| `odiff2.py <unit> <symbol>` | Differing instructions of one function (objdiff-cli). |
| `variants.py <unit> <filter> <file>` | Try alternative snippets (blocks separated by `====`), keep the first that matches. |
| `try.sh <file.cpp>` | Standalone compile with the game flags, prints disassembly. |
| `xref.py build` / `xref.py <name> [ctx] [max]` | Cross-reference index of the target asm (`build/xref.pkl`). |
| `callers.py <name> [ctx] [max]` | Call sites of a function in the target asm. |
| `datarefs.py <unit> [section]` | Per function, number of distinct data objects referenced (ours vs target). |
| `pairnames.py <unit> [--data] [--apply]` | Rename target `fn_`/`lbl_` symbols in symbols.txt from our object (check pairings). |
| `symmerge.py config/SLSP01/symbols.txt` | Resolve git conflict blocks in symbols.txt by address, preferring real names. |
| `refmerge.py <other libs_ref.json>` | Add missing libs_ref.json entries from another copy. |

`maps/map_<unit>.txt`: optional address -> mangled-name map used by `cmp.py` for calls whose target
symbol is still `fn_<ADDR>` but our code uses a real name.
