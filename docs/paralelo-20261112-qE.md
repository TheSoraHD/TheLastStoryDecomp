# Lote paralelo 2026-11-12, cola E (Last Cocoon)

Inicio: 2026-11-12 16:10 (reloj del contenedor). Rama `agent-E`.

## game/LastCocoonQuadruped

- Escritas (antes `-`): ctor (100 %), `QuadRoutes_unk` ctor 0x8045FE2C (100 %), vf0C 0x8045FE70 (100 %),
  invoker/manager de boost::function 0x80460374/0x804603A0 (100 %/99,6), vf10 0x80460458 (92 %),
  fn_80463174 (100 %), fn_804635A4/AC (100 %), fn_80463610 (99,96: cadena), fn_80463F20/fn_80464608
  (99,9: ranuras de pila), fn_80466CA4/fn_804672CC (100 %), fn_80467F50 (99,9: pool), fn_80468B68 (19,6 %).
- 100 %: 25 -> 35 de 54.
- NONMATCHING: vf10 (presupuesto de inlining R3 del original), fn_80468B68 (el original queda por encima
  del presupuesto: llama a las copias de Vector3 y a FindPlace_unk; los auxiliares de std::vector son
  copias debiles de Em403WarLion con nombre C).

## Decisiones tomadas sin el usuario

- `QuadRoutes_unk` es el mismo spline que `PathSpline_unk` de game/PathData_unk.cpp y que
  `gmk::PathData_unk` (CharaPathMove.h, 0x54 alli): no se unifican (otras familias); se le dan los campos
  reales (`mNumSegments`, `mLength`, `mStart`, `mEnd`, `mX/mY/mZ` de `QuadCubic_unk`, `mLengths`). Pendiente.
- `Field_unk` local (como gmk/FlamingArrow.cpp) para `fn_8013C504`.
- `fn_804635A4`/`fn_804635AC` (copias debiles de accesores de GimmickCommon) definidas con nombre C.

## Hallazgos

- El ctor de LastCocoonQuadruped sale al 100 % a la primera con todos los miembros tipados (no hay
  bloqueo de presupuesto como en Chrysalis/Imago).
- `ToRotationZ_unk` (z redondeada una vez) en vez de `ToRotation_unk` cuando el asm reusa el `frsp` de z
  para atan2 (`fmr f2, f4`).
