# CLAUDE.md — Decompilación de The Last Story (Wii, PAL `SLSP01`)

Repo `TheSoraHD/TheLastStoryDecomp` (plantilla decomp-toolkit + objdiff + ninja). El `main.dol` original (`orig/SLSP01/sys/main.dol`, SHA-1 `d4275869483fbc681f2ce6eddfbd9742e40c6b64`), los compiladores (`build/compilers/`) y `sjiswrap.exe` están en el repo privado `TheSoraHD/tls-build`: el entorno en la nube no necesita el PC. Comunicación con el usuario en español.

**Objetivo:** C++ que, compilado con el compilador original, da un `.dol` idéntico byte a byte. Una función solo cuenta si objdiff la da al **100%**.

> Este fichero se carga en cada sesión y en cada agente: **mantenerlo corto**. Nada de relatos de ramas ni listas de trucos aquí; van en `docs/` (ver abajo).

## Estado

- `auto-20261030` (2026-10-31, sesion autonoma, **sin mergear**): **9113 / 18215 funciones al 100%** (codigo 32,93%), 380 unidades enlazadas, `main.dol: OK`. Unidades nuevas sin enlazar: `BattleCharaTask` (58/88), `NewCampPartsBase`, `FragileObject`, `QuadrupedTask`, `NewCampPartsOption`, `TutorialTask`, `ControlAttackBase`; `GameUI` 10 -> 33. Detalle y pendientes en `docs/auto-20261030.md`.
- `close-one-func-2` (2026-10-30, mergeada en `main`): 8770 funciones, +`game/MapPartsEfp`, `game/Gun` (`docs/close-one-func-2.md`).
- `close-one-func` (2026-10-30, mergeada): 8768 funciones, +`gmk/TodomeCountUp`, `gmk/CharaObj`, `atn/TaskBase`; `list-const-iter` (8765, `docs/list-const-iter.md`).
- Anteriores: `k2-close-units` + `vector-push-back` (K2, `push_back`; `docs/k2-close-units.md`), `i2-poolorder` (I2, mergeada; `poolorder.py`, `docs/i2-poolorder.md`), `fb2-materialparser` (F-b2), `fb3-drawhelper-paso3` (`atn/DrawHelper` 53/68), `fb3-comdrawtexture`, `fb-materialparser`, `batch-auto-50kb-atn` (F), `batch-auto-20kb-game` (E), `close-near-units` (K), `rso-names*`; revisión de Svenchu/Codex en `docs/revision-2026-10-29.md`.
- objdiff da 100% aunque cambie la condición de un salto (`beq`/`bne`): pasar `tools/decomp/mnemcheck.py <unidad>` a las funciones al 100%. Tras editar una unidad ya emparejada: `resetlocal.py`, `pairdata.py --apply`, `configure`.
- Al cerrar un lote: sustituir estas tres líneas, añadir **una** fila a `docs/historial.md`, el resumen y las decisiones en `docs/<lote>.md`, y los hallazgos generales (una línea cada uno) en `docs/trucos-mwcc.md`.

## Dónde está cada cosa (leer solo lo que haga falta)

- `docs/trucos-mwcc.md`: trucos de MWCC, una línea cada uno. Buscar con `grep` cuando una función se atasque; no leerlo entero.
- `docs/entorno-nube.md`: montaje del entorno en la nube y receta de lotes en paralelo con agentes.
- `docs/historial.md`: lotes hechos, cifras y dónde están sus decisiones.
- `docs/mwcc-blockers.md`: bloqueos recurrentes investigados (qué **no** repetir).
- `docs/fuente-enlazable.md`: por qué un fuente con muchos 100% puede no enlazar nunca y cómo evitarlo (regla 10).
- `docs/inlining.md` (reglas R1–R5, `tools/inline_bench.py`), `docs/msl_cpp.md` (`basic_string`/`vector`/`list`), `libs/boost/README.md`, `docs/game_units.md`, `docs/classes.md`, `docs/splits-plan.md`, `tools/decomp/README.md`.
- `docs/archivo/CLAUDE_completo_2026-10-22.md`: el CLAUDE.md largo anterior (estado por unidad, hallazgos con ejemplos, notas de red/DWC). Solo para `grep`.
- **Ficheros grandes: nunca enteros, solo `grep` o rangos.** `config/SLSP01/symbols.txt` (3 MB), `splits.txt` (160 KB), `game_units.json` (60 KB), `docs/classes.md` (85 KB), `docs/game_units.md` (36 KB), las notas de lotes `docs/*-q*.md` (20–120 KB cada una), `build/SLSP01/report.json` y los `.s` de `build/SLSP01/asm` (usar `fasm.py`). Filtrar también las salidas largas de `ninja`, `regress.py` o `targets.py` (`| tail`, `grep`).

## Reglas innegociables

1. Match solo si objdiff da **100%**. Si no, `// NONMATCHING: X% - motivo` y seguir. Nunca contarla como hecha. `report.json` puede dar 100% donde `objdiff-cli diff` da 99,5–99,9%: no cuenta.
2. Prohibido `asm` inline y `#pragma` para forzar código; el C++ debe ser algo que un desarrollador habría escrito. Excepciones aprobadas (no ampliar sin permiso):
   - asm paired-single de `include/math/Vec.h` (copias, sumas, min/max `__VecMin`/`__VecMax`/`__VecMinMax`): `inline` genéricas de cabecera, con `// inferred:` y una dirección de ejemplo;
   - `#pragma explicit_zero_data on` (push/pop) solo en los `0.0f` de `.sdata` de `atn/PostFxColorMatrix`;
   - asm de librería de `boost/detail/sp_counted_base_cw_ppc.hpp` (Boost 1.35.0 en `libs/boost`);
   - `OSInitFastCast` (`include/sdk/OSFastCast.h`, usada en `game/main`);
   - paired-single cuantizado GQR6 solo en `atn/ModelDraw_unk fn_8009BF3C` (`SetGQR6_unk`, `SetMatColor_unk`); el de `LH_Binary` **no** está aprobado;
   - `#pragma ppc_iro_level 0` en `nw4r::snd::detail::AxManager::Init`; `-opt nostrength` solo en `dwc/src/base/dwc_base64.c`.
   - Aceptados (no son pragma): `__declspec(noinline) inline` (`MP/CryptMP`), `__declspec(noinline)` sin `inline` en funciones fuertes que el original llama con `bl` y MWCC inlinearía (AI_archer_attack01, 2026-10-29) y asserts vacíos `((void)0)` para el umbral de autoinline. `dont_inline`/`inline_depth`/`auto_inline` no sirven con `-ipa file`.
3. Avisar y explicar antes de tocar `splits.txt`, `symbols.txt` (salvo renombrados), `configure.py`, `config.yml` o flags.
4. No tocar ni subir ficheros originales (`orig/`, `.dol`, ISO, assets).
5. Trabajar en rama, nunca en `main`; un commit por función o grupo pequeño. Mensaje: `Match Clase::metodo (0x<dirección>)` + líneas de atribución del sistema. `git push` solo con permiso explícito en la sesión.
6. Nada destructivo (`reset --hard`, `push --force`, `rm -rf` fuera de `build/`) sin confirmación. No salir del repo.
7. No inventar: tipos y nombres deducidos con `// inferred: motivo`. Comentarios de código en ASCII (sjiswrap).
8. Tras 10–15 intentos distintos sin 100%, dejar `NONMATCHING` y pasar a la siguiente.
9. No reescribir código ya matched sin necesidad; no cambiar compilador ni flags globales para forzar un match; no cambiar firmas/layouts compartidos sin comprobar que nada baja.
10. **El fuente tiene que poder enlazar**, no solo dar porcentaje (ejemplos y motivos en `docs/fuente-enlazable.md`):
    - Orden del `.dol`: `ordercheck.py <unidad>` con 0 fuera de orden en cada commit (`reorder.py` lo arregla).
    - Prohibido declarar o definir con `extern "C"` un nombre mangled (`__ct__…`, `__dt__…`, `__vt__…`, `Func__FR…`) y escribir el puntero de vtable a mano (`*(void**)this = __vt__…`): declarar la clase (`_unk` con relleno si hace falta) o incluir su cabecera; vtable y RTTI las emite el compilador.
    - Métodos y virtuales como miembros de su clase (`vfXX`, `fn_<addr>`), no como `extern "C"` con un `self` (`extern "C"` solo para funciones sin clase); un mismo objeto, un solo tipo en una cabecera, no una vista parcial por fichero.
    - Datos del propio split (`.data/.rodata/.bss/.sdata/.sdata2`) definidos en el fuente; `extern lbl_` solo para datos de otras unidades.
    - Un commit que solo sube el fuzzy no es avance; no bajar a `NONMATCHING` funciones para "cubrir" la unidad.
11. Codex y cualquier otro agente siguen estas mismas reglas (`AGENTS.md` remite aquí): nada de commits directos en `main`; rama, checklist y merge.
12. **Trabajo secuencial por defecto: no lanzar agentes/subagentes sin preguntar antes al usuario** (aunque `docs/entorno-nube.md` tenga la receta de lotes en paralelo).

## Entorno

- Juego: `Wii/1.3`, `-ipa file`, `-func_align 4`, `-RTTI on`, `-str reuse,readonly`, `-sdata 4 -sdata2 4`, `-use_lmw_stmw on`, `-enc SJIS`. Con `-ipa file` las funciones se emiten en el orden del fuente: escribir los `.cpp` en el orden del `.dol`.
- Enlazador `GC/1.3.2` (único que da el `.dol` exacto). Runtime `GC/3.0a5.2`. SDK `Wii/1.1`; red (NCD/NET/NHTTP/NWC24/SO/SSL) `GC/3.0a5.2`; DWC y GameSpy `Wii/1.3 -func_align 16 -sdata 0 -sdata2 0`; NW4R SND `GC/3.0a5.2`.
- Unidades del juego: `config/SLSP01/game_units.json` (`name`, `classes`, `matching`), fuente en `src/<name>`. Librerías: `config/SLSP01/libs_ref.json` (escribir con `json.dumps(d, indent=1, ensure_ascii=False)` sin salto de línea final).
- `"matching": true` solo si todas sus funciones están al 100% **y** `ninja` da `main.dol: OK`. Una unidad con `matching: false` no se enlaza: `main.dol: OK` no la valida, y objdiff no compara valores de `.sdata2` ni cadenas: pasar `poolcheck.py`.
- Nombres: `<familia>/<Clase>.cpp`; `_unk` = nombre inventado; `<ruta>_cont.cpp` = continuación de una unidad enlazada (fusionar al decompilarla).
- Montaje en la nube: `docs/entorno-nube.md`.

## Comandos

```bash
python3 configure.py <flags>                       # tras tocar splits/symbols/libs_ref/game_units/configure
ninja                                              # todo + CHECK SHA-1 (main.dol: OK)
ninja build/SLSP01/src/<unidad>.o                  # una unidad (NonMatching no se recompila con ninja a secas)
build/tools/objdiff-cli diff -p . -u main/<unidad> <símbolo> -o - --format json-pretty
python3 tools/decomp/targets.py [--max-unit N] [--funcs 256]   # objetivos
python3 tools/decomp/fasm.py <unidad> <addr|nombre>            # asm objetivo compacto
python3 tools/decomp/cmp.py <unidad> [filtro] [-v]             # OK/DIFF por función (no mira destinos de salto)
python3 tools/decomp/mnemcheck.py <unidad> [-a]                # funciones al 100% con algún mnemónico distinto (beq/bne)
python3 tools/decomp/odiff2.py <unidad> <símbolo>              # instrucciones distintas
python3 tools/decomp/fnstat.py <unidad>                        # estado por función
python3 tools/decomp/xref.py <nombre>                          # referencias en todo el DOL
python3 tools/decomp/pairnames.py <unidad> [--apply]           # renombra fn_/lbl_ desde nuestro .o (revisar antes)
python3 tools/decomp/regress.py <now/report.json> <ref/report.json>   # funciones perdidas (lost: 0)
python3 tools/decomp/fuzzdrop.py <now/report.json> <ref/report.json>  # bajadas de fuzzy
python3 tools/decomp/poolcheck.py [unidad...]                  # floats de .sdata2 y cadenas frente al .o original
python3 tools/decomp/poolorder.py [--funcs] [unidad...]        # orden de @stringBase0 frente al original
python3 tools/decomp/ordercheck.py <unidad> [-v]               # orden del fuente frente al .dol (fuera de orden / faltan)
python3 tools/decomp/reorder.py <unidad> [--dry]               # reescribe el .cpp en orden del .dol (luego cmp.py)
python3 tools/decomp/autoscan.py --min 0x100 [--unit X] [--data]      # tramos auto_ (borrar antes build/SLSP01/asm)
tools/decomp/try.sh test.cpp                                   # compila un fichero suelto con flags del juego
```

## Flujo por unidad/función

1. `targets.py` → objetivo. **Antes de escribir, comprobar que el split cubre el fichero real** (pool `@stringBase0`, `.sdata2`, `.sbss`, funciones vecinas): si no, casi nada llega al 100%.
2. `fasm.py` para el asm; `grep` en `symbols.txt` y `docs/classes.md` por la clase o la dirección; cabeceras de `include/` y funciones vecinas ya hechas.
3. Escribir el C++ en el orden del `.dol` (comprobar con `ordercheck.py`) con las clases reales (regla 10); compilar la unidad; `cmp.py`/`odiff2.py`; iterar.
4. Si se atasca: `grep -i <palabra> docs/trucos-mwcc.md docs/mwcc-blockers.md`.
5. Al 100%: `pairnames.py --apply` (revisar), `matching: true` si toda la unidad lo está, `configure` + `ninja`, `main.dol: OK`, `regress.py` lost: 0. Commit.

## Convenciones

- Virtuales sin nombre `vfXX` (offset en la vtable); no virtuales `fn_<addr>` dentro de su clase; externos sin clase `extern "C"` con nombre automático (`fn_8005B9CC`, `lbl_8087EF68`); clases desconocidas `<Algo>_unk` con `// inferred:`; miembros `mXX` (`m04`) o `field_0x10[4]` si no se conoce el tipo.
- `chr::Chara_unk` = `CharaTask`. Tipos reales del motor: `atn::Vector3`, `atn::Matrix3x4` (`include/math/Vec.h`), `atn::CsvBase<...>`/`Csv`/`CsvU`/`CsvTabU` (`include/atn/Csv.h`), `atn::Loader`.

## Antes de dar una tarea por terminada

- [ ] Función al 100% en objdiff; proyecto compila sin warnings nuevos; `main.dol: OK`.
- [ ] `regress.py` lost: 0 y `fuzzdrop.py` sin bajadas contra el `report.json` guardado **antes** de empezar (nunca contra uno regenerado tras el cambio); `poolcheck.py` sin avisos nuevos en lo tocado.
- [ ] `ordercheck.py` sin funciones fuera de orden; sin `extern "C"` con nombres mangled ni vtables escritas a mano (regla 10).
- [ ] Sin asm ni pragmas fuera de las excepciones; comentarios `// inferred:` puestos.
- [ ] Commit en la rama; sección "Estado" y `docs/historial.md` al día si cambió el estado.

## Mapa del `.dol` (`.text`)

| Rango | Contenido |
|---|---|
| `0x80004000–0x800062F8` | `.init`: `__start.c`, `__ppc_eabi_init.c`, `__mem.c` |
| `0x800081C0–0x800DE000` | Motor `atn::` (ficheros `atn_*.cpp` en orden alfabético) |
| `0x800DE000–0x805BFAC0` | Juego (`Event::`, `AI::`, `gmk::`, `chr::`, `MP::`…), todo partido en unidades con nombre |
| `0x805BFAC0–0x8067A034` | RVL SDK, NdevExi2A, MetroTRK |
| `0x8067AA78–0x8068B610` | MSL_C |
| `0x80695358–0x80697D34` | Runtime |
| `0x80697BB8–0x80705300` | `Gecko_ExceptionPPC.cp`, red del SDK, DWC, GameSpy |
| `0x80703F30–0x807263DC` | NW4R (snd, lyt, db, ut) |
