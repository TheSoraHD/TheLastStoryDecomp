# Siguiente sesion (tras el merge de `nw4rmath-20261107` y `msl-iostream-20261107` en `main`)

`main` (2026-11-07): **14146 / 18116**, lost 0, build limpio `main.dol: OK`. Prompt listo en el proyecto:
`claude/prompt-msl-iostream-4.md` (crear la rama nueva desde `main`, ya no hace falta `msl-iostream-20261107`).

## MSL C++ (106/170)

- `PowerPC_EABI_Support/src/MSL_C++/`: `iostream.cpp` (0x8068B39C-0x80691E50), `locale.cpp` (0x80691E50-0x806952C4)
  y `shared_ptr_unk.cpp` (enlazada). Cabecera comun `msl_iostream_unk.h`.
- Pendientes (detalle en `docs/msl-iostream-20261107.md`, "Sesion 3"): `num_put` de `double` (3 funciones),
  `Init`/`wios_base_Init_unk` (presupuesto R3), `flush`/`sentry`, rangos de `ctype<wchar_t>` (recarga de `end()`),
  `replace` (8 temporales `stb 0`), `__global`, `__insert_one_unk`; despues orden y enlace de las dos TU.

## Otros (de `maraton-20261107`, siguen vigentes)

1. Unificar vistas con las clases reales: `Ledge_unk` = `GameSceneRoute_unk`; `Field_unk`/`GameField_unk`/
   `DiveField_unk` = `GameTask`; las vistas de `lbl_8087EFA8`.
2. GameTask 0x8036555C-0x8036823C (primeras funciones, con cadenas).
3. Constructores de `TacView` (0x805A7158) y `BattleTask` (0x800F1430).
4. `GameCamera_unk` 11/90 y `Event/DebugInfo_unk` 1/58.
5. `ut_TextWriterBase`: unica unidad NW4R sin enlazar.
