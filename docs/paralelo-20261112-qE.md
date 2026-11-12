# Lote paralelo 2026-11-12, cola E (Last Cocoon)

Inicio: 2026-11-12 16:10 (reloj del contenedor). Rama `agent-E`.

## game/LastCocoonQuadruped

- Escritas (antes `-`): ctor (100 %), `QuadRoutes_unk` ctor 0x8045FE2C (100 %), vf0C 0x8045FE70 (100 %),
  invoker/manager de boost::function 0x80460374/0x804603A0 (100 %/99,6), vf10 0x80460458 (92 %),
  fn_80463174 (100 %), fn_804635A4/AC (100 %), fn_80463610 (99,96: cadena), fn_80463F20/fn_80464608
  (99,9: ranuras de pila), fn_80466CA4/fn_804672CC (100 %), fn_80467F50 (99,9: pool), fn_80468B68 (19,6 %).
- Segunda tanda: vf40 0x80460B48 (99,9: r5/r6), fn_80462CCC callback de nodos (99,2: como
  QuadrupedTask::fn_805688CC), fn_80468534 (99,6: r28/r29).
- 100 %: 25 -> 38 de 54. Sin escribir: 9 -> 6 (fn_80461838, fn_80462038, fn_80464CF0, fn_804655AC,
  fn_80465B88, fn_80468038).
- NONMATCHING: vf10 (presupuesto de inlining R3 del original), fn_80468B68 (el original queda por encima
  del presupuesto: llama a las copias de Vector3 y a FindPlace_unk; los auxiliares de std::vector son
  copias debiles de Em403WarLion con nombre C).

- Tercera tanda: fn_80468038 (97,7 %: `m560 == 4 || m560 == 5` sale como prueba de rango), fn_80464CF0
  (modo 12, 99,96 %: ranuras de PlayEfp_unk invertidas; el original construye un QuadCollResult_unk sin
  usar en cada rama, el de la tercera despues de la matriz), fn_804655AC (modo 15, 99,7 %: registros).

## game/LastCocoonImago

- Escritas: fn_8045E548 (modo de los motions 0x14B-0x14D), vf20 0x8045B594 (98,8 %: pila). Tablas de
  .rodata `sImagoAttacks_unk` (0x80754C24) y `sImagoCapsules_unk` (0x80754D30) definidas en el fuente;
  `m18D4` pasa a `AttackTimer_unk[3]` (la copia de la espera de la tabla 0 sale plegada a 0, como en el
  original).
- vf0C 0x80458590 (100 %; `m18F4` = lugar 2 de la tabla de rutas, `Field_unk` local con `m10D8`),
  vf54 0x8045A8F0 (98,6 %: ranuras de pila de las dos ramas), vf3C 0x80459358 (100 %; `IsDown_unk` y
  `!Check(0x80000)`), vf80 0x8045C83C (98,5 %: pila; rotacion en el sitio con `fn_80139F60`),
  fn_8045E780 (100 %: el giro `t * (0.15f * d)` en su propia sentencia evita el `fmadds`).

## game/LastCocoonEgg

- fn_8045817C (menu de depuracion, 99,5: solo offsets de cadenas, falta el resto del pool). Las copias
  debiles fn_80453C88/fn_80453DB4/fn_80453DBC (y fn_80454554/64) solo existen porque vf0C/vf10 estan
  por encima del presupuesto: hay que escribirlas con esas funciones.
- Etiquetas @11773/@11775/@11998 renombradas a mano (@11800/@11802/@12025): pairdata las da por ambiguas
  aunque el pool coincide por posicion.

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

## Resumen (cierre 2026-11-12 17:45)

| Unidad | Funciones al 0 % / sin escribir | 100 % |
|---|---|---|
| game/LastCocoonQuadruped | 25 -> 3 (fn_80461838, fn_80462038, fn_80465B88) | 25 -> 38 / 54 |
| game/LastCocoonImago | 21 -> 14 (ctor 0 % por presupuesto, vf10/vf40/vf50, fn_8045ABB4...) | 14 -> 18 / 36 |
| game/LastCocoonEgg | 18 -> 17 | 20 / 42 (fn_8045817C al 99,5 % por el pool) |
| game/LastCocoonChrysalis | 1 -> 1 (ctor, bloqueo documentado; no repetido) | 6 / 7 |
| game/QuadrupedTask | 1 -> 1 (no tocada) | sin cambios |

report.json: 15091 -> 15108 funciones al 100 %, lost 0, sin bajadas. Ninguna unidad nueva enlaza.

## Pendientes

- Quadruped: fn_80461838, fn_80462038 (modo 0/7), fn_80465B88 (modo 16); vf10 al 92 % y fn_80468B68 al
  20 % (presupuesto de inlining del original).
- Imago: ctor (presupuesto), vf10 0x8045883C, vf40 0x804596E8, vf50 0x80459FD0, fn_8045ABB4, fn_8045B82C,
  fn_8045BFFC, fn_8045CB24, fn_8045D3A4, fn_8045DF34, fn_8045EDEC; 0x80459FB8 es un vfBC con argumento
  (`return arg != &m18FC`), pero `CharaTask::vfBC()` no tiene parametros en la cabecera compartida.
- Egg: vf0C (por encima del presupuesto; sus copias debiles fn_80453C88/DB4/DBC) y el resto de modos.
- Unificar `QuadRoutes_unk`, `PathSpline_unk` (game/PathData_unk.cpp) y `gmk::PathData_unk`.
