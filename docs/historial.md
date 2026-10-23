# Historial de lotes

Una fila por rama mergeada en `main`. El detalle (resultado por unidad, decisiones, hallazgos) está en el documento de cada lote; el relato completo anterior a 2026-10-22, en `docs/archivo/CLAUDE_completo_2026-10-22.md` (sección "Estado actual").

| Fecha | Rama | Merge | Funciones al 100% | Código / fuzzy | Enlazados | Detalle |
|---|---|---|---|---|---|---|
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
