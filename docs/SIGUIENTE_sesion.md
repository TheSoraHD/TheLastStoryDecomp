# Siguiente sesion (tras `gametask-20261107`, mergeada en `main`)

`main` 339e680 (merge de `gametask-20261107`): **14167 / 18116**, lost 0, `main.dol: OK`. Crear rama nueva desde
`main`. Notas: `docs/gametask-20261107.md`.

## GameTask (57/133)

- Pool `@stringBase0` ordenado hasta 0x1AF; accesores de flags (`fn_80370174`, `fn_80370A78`, `fn_80370AE4`) al
  100%. Ya cuadran las funciones con esas cadenas (`dg`, `GetGlobalFlag`...).
- Siguiente en el pool: `fn_803712BC` (`Title`), una instanciacion de `vector` (`"vector length error"`) antes de
  `fn_80373FC4` (`SE_UICOM_022`). Despues, `fn_803737C0`/`fn_803743AC` (`iconOn`).
- `fn_803729B4` (1,7 KB) rechaza todos sus inline (R3): al escribirlo aparecen ~10 copias debiles pequenas
  (`fn_80373084`..`fn_80373108`, `GetCurrentMap`, `fn_80372574`, `fn_803727D4`, `fn_8036E428`, `fn_8036EE04`,
  los de `RenderConfig` tras el dtor). Ojo con el orden: cada copia va detras de la primera funcion que la usa.
- Casi iguales: ctor 99,96%, `vf0C` 99%, `fn_80376324` 99%, `fn_80375D0C` 99,85%, `fn_80370C2C` 98,9%.
- Tras `precommit.sh --pd`: devolver `lbl_8087D6E8`/`lbl_8087D6EC` en `symbols.txt` (ver notas).

## Otros (de `maraton-20261107`, siguen vigentes)

1. Unificar vistas con las clases reales: `Ledge_unk` = `GameSceneRoute_unk`; `Field_unk`/`GameField_unk`/
   `DiveField_unk` = `GameTask` (ahora con cabecera completa); los `extern "C"` de flags (`fn_80370174`...)
   pasarian a metodos. Hacerlo en un lote y un solo `pdbatch.sh`.
2. Constructores de `TacView` (0x805A7158) y `BattleTask` (0x800F1430): traen sus copias debiles.
3. `GameCamera_unk` 11/90 y `Event/DebugInfo_unk` 1/58: muchas funciones pequenas pendientes.
4. `ut_TextWriterBase`: unica unidad NW4R sin enlazar.
5. MSL C++ en rendimientos decrecientes (`docs/msl-iostream-20261107.md`).
6. Bloqueos conocidos en `docs/mwcc-blockers.md`: no repetir.
