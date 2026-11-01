# tools/decomp/ss: NW4R units from the Skyward Sword sources (libs/ss_nw4r)

Needs a sparse clone of `zeldaret/ss` (CC0) in `$SS_REF` (default `/home/claude/ref_ss`):
`git clone --depth 1 --filter=blob:none --sparse https://github.com/zeldaret/ss.git` and
`git sparse-checkout set --no-cone '/include/' '/src/nw4r/' '/src/PowerPC_EABI_Support/MSL/'`.

Per unit (`snd_X.cpp` or `ut/ut_X.cpp`), see docs/auto-20261031c.md:
1. `ssdo.sh snd_X.cpp`: `vendor.py` (copies the source and missing headers, drops the `snd_`/`ut_`
   prefix), `ssfind.py` (compiles with the lib NW4R flags, OK/DIFF by name, renames `fn_` with the
   same masked bytes; stops if a `fn_` is used by our sources unless `FORCE=1`), `ssauto.py` (split
   ranges from the relocations; prints `ARGS`).
2. `vtmap.py [--apply]`: names thunks and weak virtuals from the vtables (checks wrong names too).
3. `ssadd.py snd_X.cpp "text a b" ...` (`MATCH=0` for an unlinked unit), `sscall.sh snd_X.cpp`
   (renames the callees), then configure + ninja: `main.dol: OK` is the real check (functions with
   identical bytes can get the wrong name).
4. `pairdata.py ss_nw4r/src/snd/snd_X --apply` for the data labels.
Game code that calls a renamed `fn_` with `extern "C"` must call the method (see `atn/SoundCore.h`).
