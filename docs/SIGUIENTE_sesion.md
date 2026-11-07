# Siguiente sesion (tras `msl-iostream-20261107c`, sin mergear)

Rama `msl-iostream-20261107c` (sobre `main` c918eb8): **14152 / 18116**, lost 0, `main.dol: OK`. Pendiente de
revisar y mergear. Prompt listo en el proyecto: `claude/prompt-msl-iostream-6.md`.

## MSL C++ (112/170)

- `PowerPC_EABI_Support/src/MSL_C++/`: `iostream.cpp` y `locale.cpp` sin enlazar, `shared_ptr_unk.cpp` enlazada.
- Detalle en `docs/msl-iostream-20261107.md`, "Sesion 5" (cambios, hallazgos, pendientes).
- `ios_base::Init`/`wios_base_Init_unk` (desbloquean ~19 funciones): el original no inlinea nada (presupuesto R3).
  El constructor de `console_inputbuf` (plantilla no `inline`) esta a ~2 sentencias de ser candidato auto-inline;
  aun siendolo faltan ~22 sentencias de coste. Las copias fuera de linea de los constructores de streams y de
  `console_outputbuf` ya son identicas al original.
- Despues: `sentry`/`flush` (solo el orden de operandos del `and.`), `__round_unk`, `throw_failure` (cadena de
  tipo en objeto propio), residuos de `locale.cpp`, y el orden de `.rodata`/`.bss`/funciones para enlazar.

## Otros (de `maraton-20261107`, siguen vigentes)

1. Unificar vistas con las clases reales: `Ledge_unk` = `GameSceneRoute_unk`; `Field_unk`/`GameField_unk`/
   `DiveField_unk` = `GameTask`; las vistas de `lbl_8087EFA8`. Hacerlo en un lote y un solo `pdbatch.sh`.
2. GameTask 0x8036555C-0x8036823C (primeras funciones, con cadenas): desbloquea el `@stringBase0` de los getters
   de flags (`fn_80370174`, `fn_80370A78`, `fn_80370AE4`, usados por ~50 unidades).
3. Constructores de `TacView` (0x805A7158) y `BattleTask` (0x800F1430): traen sus copias debiles.
4. `GameCamera_unk` 11/90 y `Event/DebugInfo_unk` 1/58: muchas funciones pequenas pendientes.
5. `ut_TextWriterBase`: unica unidad NW4R sin enlazar (los 2 bytes de `.sbss` son del propio objeto).
6. Bloqueos conocidos en `docs/mwcc-blockers.md`: no repetir.
