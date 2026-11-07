# Siguiente sesion (tras el merge de `msl-iostream-20261107b` en `main`)

`main` (2026-11-07, merge de `msl-iostream-20261107b`): **14151 / 18116**, lost 0, `main.dol: OK`. Prompt listo en
el proyecto: `claude/prompt-msl-iostream-5.md` (rama nueva desde `main`).

## MSL C++ (111/170)

- `PowerPC_EABI_Support/src/MSL_C++/`: `iostream.cpp` y `locale.cpp` sin enlazar, `shared_ptr_unk.cpp` enlazada.
- Detalle en `docs/msl-iostream-20261107.md`, "Sesion 4" (decisiones, hallazgos, pendientes).
- La pieza grande es `ios_base::Init`/`wios_base_Init_unk`: el original no inlinea nada (presupuesto R3); al
  nuestro le faltan ~75 sentencias de coste de candidatos (20-40 si el constructor de `console_inputbuf` fuera
  `inline`). Si se resuelve, salen ~19 funciones (constructores de streams y de `console_outputbuf`, destructores
  de `wostream`/`wios`/`wistream`, 0x8068E270, `__nw__FUlPv`, `setf`, `tie`, `pubsetbuf`...).
- Despues: `sentry`/`flush` (operandos del `and.` de `clear()`), `__round_unk` (registro), residuos de `locale.cpp`,
  y el orden de `.rodata`/`.bss` y de las funciones para enlazar.

## Otros (de `maraton-20261107`, siguen vigentes)

1. Unificar vistas con las clases reales: `Ledge_unk` = `GameSceneRoute_unk`; `Field_unk`/`GameField_unk`/
   `DiveField_unk` = `GameTask`; las vistas de `lbl_8087EFA8`. Hacerlo en un lote y un solo `pdbatch.sh`.
2. GameTask 0x8036555C-0x8036823C (primeras funciones, con cadenas): desbloquea el `@stringBase0` de los getters
   de flags (`fn_80370174`, `fn_80370A78`, `fn_80370AE4`, usados por ~50 unidades).
3. Constructores de `TacView` (0x805A7158) y `BattleTask` (0x800F1430): traen sus copias debiles.
4. `GameCamera_unk` 11/90 y `Event/DebugInfo_unk` 1/58: muchas funciones pequenas pendientes.
5. `ut_TextWriterBase`: unica unidad NW4R sin enlazar (los 2 bytes de `.sbss` son del propio objeto).
6. Bloqueos conocidos en `docs/mwcc-blockers.md`: no repetir.
