# Historial de lotes

Una fila por rama mergeada en `main`. El detalle (resultado por unidad, decisiones, hallazgos) está en el documento de cada lote; el relato completo anterior a 2026-10-22, en `docs/archivo/CLAUDE_completo_2026-10-22.md` (sección "Estado actual").

| Fecha | Rama | Merge | Funciones al 100% | Código / fuzzy | Enlazados | Detalle |
|---|---|---|---|---|---|---|
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
