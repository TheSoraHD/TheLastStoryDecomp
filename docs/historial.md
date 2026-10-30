# Historial de lotes

Una fila por rama mergeada en `main`. El detalle (resultado por unidad, decisiones, hallazgos) está en el documento de cada lote; el relato completo anterior a 2026-10-22, en `docs/archivo/CLAUDE_completo_2026-10-22.md` (sección "Estado actual").

| Fecha | Rama | Merge | Funciones al 100% | Código / fuzzy | Enlazados | Detalle |
|---|---|---|---|---|---|---|
| 2026-10-30 | `vector-push-back` (sobre K2) | merge en `main` 2026-10-30 | 8759 (+3 que `report.json` ya contaba) | 31,44% / 54,15% fuzzy | 375 | `docs/msl_cpp.md` ("Element kinds": despacho POD con `__builtin_is_pod`; `__allocate` común de vectores de punteros/int/float y nodos de `list` de punteros, símbolos 0x80775A88/0x80779810 renombrados; 2 bajadas aceptadas por el usuario en `Loader`; residuo r3/r4 de `push_back` en bucles documentado con sonda aislada) |
| 2026-10-30 | `k2-close-units` (propuesta K2) | merge en `main` 2026-10-30 | 8752 → 8759 | 31,37 → 31,44% / 54,12 → 54,15% fuzzy | 371 → 375 | `docs/k2-close-units.md` (enlazadas `db/EventIdTable_unk`, `game/SpecialEnemyFactory_unk`, `Event/BehaviorMoveTo`, `Event/BehaviorFollow`; `atn::Task` alineada a 8 con ids `unsigned long long`; índice `unsigned` en bucles inline; `fn_800D5F68` escrita al 98%; `std/bad_cast*`/`runtime_error` necesitan fusión de splits) |
| 2026-10-30 | `i2-poolorder` (propuesta I2) | merge en `main` 2026-10-30 | 8750 → 8752 | 31,36 → 31,37% | 371 (1302 ficheros) | `docs/i2-poolorder.md` (`poolorder.py`; pools `@stringBase0` idénticos 279 → 317 unidades: NUL de formatos `sprintf`, `std/string_members.h`, instanciación explícita tras el primer uso; `fn_805A5220` e `InsertNode` al 100%; split `game/SpecialEnemyFactory_unk` y 12 pools `lbl_` renombrados) |
| 2026-10-30 | `fb2-materialparser` (propuesta F-b2) | merge en `main` 2026-10-30 | 8747 → 8750 | 31,33 → 31,36% | 371 | `docs/fb2-materialparser.md` (`atn/MaterialParser_unk` 21/35; pool de cadenas idéntico al original: ayudante del menú no `static` y `std/string_members.h`; `SwapTable_unk` para `m38`; 9 funciones suben 1–9 puntos) |
| 2026-10-30 | `fb3-drawhelper-paso3` (propuesta F-b3, paso 3) | rama, sin mergear | 8724 → 8747 | 31,17 → 31,33% | 371 | `docs/fb3-comdrawtexture.md` (paso 3: formas 3D, ventana, 12 `vf10` GX y destructores; `atn/DrawHelper` 53/68, faltan 15 casi-100%) |
| 2026-10-30 | `fb3-comdrawtexture` (propuesta F-b3, pasos 1 y 2) | merge en `main` 2026-10-30 | 8696 → 8724 | 31,03 → 31,17% / 53,78% fuzzy | 371 | `docs/fb3-comdrawtexture.md` (opción A: 31 funciones de dibujo como métodos de `atn::DrawHelper`, 302 llamadas en 67 ficheros; `atn/ComDrawTexture` fusionada en `atn/DrawHelper`, 30/68 al 100%, deja de enlazarse hasta completarla) |
| 2026-10-30 | `fb-materialparser` (propuesta F-b, primera parte, sobre `batch-auto-50kb-atn`) | 387e48a2 | 8678 → 8696 | 31,03% | 372 | `docs/fb-materialparser.md` (opción A: `atn/MaterialParser_unk` como métodos de `Material_unk`/`TexStage_unk`, 18/35 al 100%; cabecera común `include/atn/Material_unk.h` que sustituye cuatro vistas; `SafeDelete_unk` y macro `FIND_NAME` por el presupuesto de inline) |
| 2026-10-29 | `batch-auto-50kb-atn` (propuesta F) | 387e48a2 (dentro de F-b) | 8571 → 8614 | 30,79 → 30,96% / 52,67 → 52,99% fuzzy | 372 | `docs/batch-auto-50kb-atn.md` (`atn/ColliTest_unk` 16/28 y `atn/EffectSystem_unk` 27/58 con fuente nueva; `ComDrawTexture` y `MaterialParser_unk` aplazadas: métodos llamados como `extern "C"` con `self`, decisión del usuario; cabeceras `AABB_unk.h` y `EffectSystem_unk.h`; `tools/decomp/resetlocal.py`) |
| 2026-10-29 | `batch-auto-20kb-game` (propuesta E, primera mitad) | 161fd9c | 8180 → 8571 | 29,76 → 30,79% / 49,68 → 52,67% fuzzy | 372 | `docs/batch-auto-20kb-game.md` (17 unidades de 10–20 KB con fuente nueva más `chr/ControlBase`; `FinishCamera_unk::fn_803792F0` sin escribir; `tools/decomp/pairdata.py` y `mnemcheck.py`; objdiff no distingue `beq`/`bne`) |
| 2026-10-29 | `close-near-units` (propuesta K) | 161fd9c (dentro de E) | 8180 → 8180 | 29,75% / 49,68% fuzzy | 370 → 372 | `docs/close-near-units.md` (SparkCube y Banana enlazadas; `MP/Popup::vf10` 97,6 → 99,8%; bloqueos de `db/EventIdTable_unk`, `std/bad_cast` y enemigos) |
| 2026-10-29 | `rso-names-3` (`MemoryTag` en `operator new`, SparkCube 48xxx) | — | 8180 → 8180 | 29,75% | 370 | `docs/rso-names.md` (sección "Cuarta tanda") |
| 2026-10-29 | `rso-names-2` (nombres reales del RSO en el fuente, 3 colas + tercera tanda secuencial) | — | 8180 → 8180 | 29,75% / — | 370 | `docs/rso-names.md` (sección "Segunda tanda"), `docs/rso-names-q{A,B,C}.md`; fusión `atn/WordWrap_unk`+`char_traits_char`+`allocator_char`; thunks de iostream a `GameEventScript_cont2` (lost 2 aceptado); tercera tanda: `balance_insert`, `Inventory`, `SimpleArray`, `CharaFaceControl`, `AllocFromMem2`, `EffFile`, `GimmickManager`, vistas parciales |
| 2026-10-29 | `rso-names` (nombres del `LastWorld_tools.rso` USA) | — | 8180 → 8180 | 29,75% / 49,67% fuzzy | 370 | `docs/rso-names.md` (299 renombrados en `symbols.txt`, `GetNormalize`/`GetStrings`, `tools/decomp/rso/`; tabla de nombres reales pendientes por unidad) |
| 2026-10-29 | `fix-svenchu-linkage` (funciones que faltan: GraphicsDevice, SoundManager, AI, allocator_char) | merge en `main` 2026-10-29 | 7830 → 8180 | 27,92 → 29,75% / 47,50 → 49,67% fuzzy | 370 | `docs/revision-2026-10-29.md` (sección "Continuación"; AI 479/510, SoundManager 64/76, GraphicsDevice 25/41; split AiCommon/AI a 0x8001C088; bloqueos en `docs/mwcc-blockers.md`) |
| 2026-10-29 | `fix-svenchu-linkage` (revisión de los commits de Svenchu/Codex) | — | 7825 → 7830 | 27,92% / 47,48 → 47,50% fuzzy | 370 | `docs/revision-2026-10-29.md` (orden del `.dol` en 6 unidades, clases reales en AI/RenderSystem/GraphicsDevice/SoundManager/HBMRso, regla 10, `AGENTS.md`, `ordercheck.py`/`reorder.py`) |
| 2026-10-29 | `enemies-third` (propuesta H) | merge en `main` 2026-10-29 | 7564 → 7825 | 26,21 → 27,92% / 43,46 → 47,48% fuzzy | 370 | `docs/enemies-third.md` (8 unidades con fuente nueva, 13 segundas pasadas; cierre anticipado; 1 bajada y 1 aviso de poolcheck aceptados en Em310Gargoyle) |
| 2026-10-25 | `codex/d2atexture-decomp` | — | 7550 → 7564 | 26,18 → 26,21% / 43,17 → 43,29% fuzzy | 370 | `docs/D2A-Texture-q1.md` (14/30 al 100%; las 30 funciones >=50%; `vf18` al 95,72%; build limpio y checks sin pérdidas, bajadas ni hallazgos) |
| 2026-10-25 | `codex/rendersystem-decomp` (avance local) | — | 7537 → 7550 | 26,17 → 26,18% / 42,94 → 43,17% fuzzy | 370 | `docs/RenderSystem-q1.md` (13/37 al 100%, 24 parciales; build limpio `main.dol: OK`, regress/fuzzdrop/poolcheck sin hallazgos; sin push) |
| 2026-10-24 | `codex/graphicsdevice-decomp` (avance local) | — | 7528 → 7537 | 26,16 → 26,17% / 42,94 → 42,94% | 370 | `docs/GraphicsDevice-q1.md` (9 funciones al 100%; dos NONMATCHING; build limpio y checks sin pérdidas, bajadas ni hallazgos; sin push) |
| 2026-10-24 | `codex/soundmanager-decomp` (avance local) | — | 7501 → 7528 | 26,17% / 42,93% | 370 | `docs/SoundManager-q1.md` (27/76 en SoundManager; build limpio `main.dol: OK`, regress lost: 0, fuzzdrop 0 bajadas, poolcheck 0 hallazgos; sin push) |
| 2026-10-24 | `codex/rodata-unassigned-2026-10-24` | — | 7501 → 7501 | 26,16% / 42,92% | 370 | `docs/rodata-sin-asignar-q1.md` (cuatro tramos y tres unidades de datos `.rodata`; build limpio `main.dol: OK`, regress/fuzzdrop/poolcheck sin hallazgos; sin push) |
| 2026-10-24 | `codex/ai-archer-attack01-q2` (continuación local) | — | 7501 → 7501 | 26,16% / 42,91% | 370 | `docs/AI-archer-attack01.md` (`fn_8003AD38` al 70,00% y dos stubs al 99,32%, NONMATCHING; build/checks sin pérdidas, bajadas ni hallazgos; sin push) |
| 2026-10-24 | `codex/ai-archer-attack01-q2` (AI/AI_archer_attack01) | — | 7500 → 7501 | 26,15 → 26,16% / 42,80 → 42,90% | 370 | `docs/AI-archer-attack01.md` (186/517; `fn_8001C284` 92,27%, `fn_8001E1D0` 47,60%, cuatro stubs de rango al 98,78%, `fn_8003E530` 88,17%; `ninja`, regress/fuzzdrop/poolcheck sin pérdidas) |
| 2026-10-23/24 | `codex/ai-archer-attack01-next` (AI/AI_archer_attack01) | — | 7491 → 7500 | 26,14 → 26,15% / 42,78 → 42,80% | 370 | `docs/AI-archer-attack01.md` (185/517; build limpio; regress lost: 0, fuzzdrop 0 bajadas, poolcheck 0 hallazgos) |
| 2026-10-23 | `codex/std-allocator-char` (AI/AI_archer_attack01) | — | 7315 → 7491 | 26,02 → 26,14% / 42,62 → 42,78% | 370 | `docs/AI-archer-attack01.md` (176/517; build limpio; regress/fuzzdrop/poolcheck sin pérdidas, bajadas ni hallazgos) |
| 2026-10-23 | `codex/std-allocator-char` (revisión `game/main`) | — | 7315 → 7315 | 26,02% / 42,62% | 370 | `docs/game-main.md` (11/14; sin match nuevo; validación limpia) |
| 2026-10-23 | `codex/std-allocator-char` (local) | — | 7301 → 7315 | 26,0 → 26,02% / 42,6 → 42,62% | 370 | `docs/std-allocator-char.md` (iostream/stringbuf parcial; build limpio; regress/fuzzdrop/poolcheck sin pérdidas; unidad no enlazable) |
| 2026-10-23 | `codex/std-bad-cast` (local) | — | 7299 → 7301 | 26,0 → 26,0% / 42,6 → 42,6% | 370 | `docs/std-bad-cast.md` (split refinado: 2 funciones MSL aisladas) |
| 2026-10-22 | `enemies-finish` (propuesta G) | c7606b1 | 6806 → 7299 | 22,6 → 26,0% / 35,5 → 42,6% | 370 | `docs/enemies-finish.md`, `-q{1..12}.md` |
| 2026-10-22 | `close-units` (D) | 64df7a6 | 6476 → 6806 | 21,3 → 22,6% / 34,0 → 35,5% | 342 → 370 | `docs/close-units.md`, `-q{A..F}.md` |
| 2026-10-21/22 | `batch-auto-20kb-atn` (B) | 6111ca3 | 5961 → 6476 | 19,5 → 21,3% / 30,9 → 34,0% | 341 → 342 | `docs/batch-auto-20kb-atn.md`, `-qA{1..10}.md` |
| 2026-10-20/21 | `batch-auto-10kb` (A) | b015150 | 5487 → 5961 | 18,1 → 19,5% / 30,9% | 338 → 341 | `docs/batch-auto-10kb.md`, `-q*.md` |
| 2026-10-21 | `mwcc-blockers` (C) | e4a1847 | 5375 → 5487 | 16,7 → 18,0% | 335 → 338 | `docs/mwcc-blockers.md`, `-q{1..4}.md` |
| 2026-10-20 | `batch-auto-5kb` | 6ace171 | 4758 → 5375 | 15,1 → 16,7% / 24,7 → 27,4% | 299 → 335 | `docs/batch-auto-5kb.md`, `-q{1..9}.md` |
| 2026-10-20 | `splits-auto` | 8c2a8fd | 4757 → 4758 | — | — | `docs/splits-auto.md`, `-{A..D}.md`, `docs/splits-plan.md` |
| 2026-10-20 | `batch-10kb-units` | 6e7fd82 | 3997 → 4757 | 12,6 → 15,1% / 19,5 → 24,7% | 297 → 299 | `docs/batch-10kb-units.md` |
| 2026-10-20 | `batch-5kb-units` | 616531a | 3179 → 3997 | 10,4 → 12,6% / 15,7 → 19,5% | — | archivo |
| 2026-10-19 | `batch-small-units` | a15deb0 | 3090 → 3179 | — | — | archivo |
| 2026-10-19 | `splits-80697BB8` (red, DWC, GameSpy) | b44d2ab | 2041 → 3090 | — | 272 | archivo |
| 2026-10-19 | `chara-task` (CharaTask completo) | ff2a473 | 1777 → 2041 | — | — | archivo |

## Pendientes abiertos (decisión del usuario)

- `CharaTask::vf60` debería devolver `float` (los cangrejos devuelven 8.0f).
- El ctor inline de `CharaTask::Item14_unk` emite un 5.0f que el original de Em209SmallGolem no tiene (+5 funciones si se resuelve sin romper Em205Ogre).
- `psq_l` cuantizado de `atn/LH_Binary`: sin aprobar.
- `Gecko_ExceptionPPC.cp`: aparcada (tamaño de extab).

## Pendientes técnicos (sin decisión)

- Splits: las tres `.bss` grandes (`auto_08_807E7800`, `auto_08_80837140`, `auto_08_80862F30`); `Event/Manager` + `PostSequence` + `Manager_cont`; fusión `atn/WordWrap_unk` + `std/char_traits_char` + `std/allocator_char`; corte `Xsca_unk`/`XsfParser_unk`; prioridad 1 de `splits-auto` que queda (`EnvSeManager`, `NewCampTask`, `ShopUpGrade`).
- Al 100% sin enlazar: `std/char_traits_char`, `db/EventIdTable_unk`, `gmk/SparkCube`; `MP/Popup` solo le falta `vf10`.
- Avisos de `poolcheck.py` en 25 unidades fuera de los lotes (propuesta I).
- No rinde: rondas de pulido de NONMATCHING de registros (`__VecCopy` r3/r4, `push_back` en bucles, `SetPaneText` en menús MP).
