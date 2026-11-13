# Lote en paralelo 2026-11-13, cola B (camara)

Rama `agent-B`. Inicio: 2026-11-13 01:05 CET. Referencia: `/home/claude/ref-B` (15232 / 18109).

## Resultado por unidad

### game/GameCamera_unk

- `fn_8038E23C` (empuja el ojo fuera de las paredes): escrita, NONMATCHING ~96,5 % (orden de la carga de
  `param->m10` y de las copias de los extremos del rayo; r27/r28 del punto de impacto y `result.m10`).
  Tiene un cuarto argumento sin usar (el objetivo): los llamadores lo pasan en r7. Firma cambiada en
  `GameCamera.h` (sin nombres de parametro) y nombre mangled nuevo en `symbols.txt`.
- `fn_803884B8` (camara lateral m648): **100 %** (mnemcheck limpio).
- `fn_8038A5F0` (camara de la cornisa, CharaTask::mC14-mC44): escrita, NONMATCHING 99,8 % (una carga de m594.m0C).
- `fn_8038378C` (cornisa sin mezcla; vista de pajaro con `fn_80387DE4`): escrita, NONMATCHING 99,8 % (igual).
- `fn_8037EF48` (seguir al lider): escrita, NONMATCHING 99,5 % (orden en la pila de los temporales de
  fn_800109E0, como fn_8038C070).
- Funciones al 0 %: 26 -> 21.

## NONMATCHING nuevas

- `fn_8038E23C` 96,5 %: ver arriba; probado `float height` antes/despues de `to`, `result` arriba, `VecAssign_unk`,
  copia campo a campo, `m89C++` despues de la copia, otra variable para la segunda copia.
- `fn_8038A5F0`/`fn_8038378C` 99,8 %: el original carga dos veces `m594.m0C` en la primera mezcla del
  desplazamiento lateral (`t * (m0C - -m0C) + -m0C`) y aqui se comparte la carga; probado una inline de mezcla
  de floats, otros ordenes de operandos y los parametros por puntero.

## Decisiones tomadas sin el usuario

- `GameCamera.h`: `float m7F8` (giro del lider) y `float mCA8` (velocidad de giro) en lugar de relleno.
- `#include "game/SetConfig.h"` subido al principio de `GameCamera_unk.cpp` (estaba antes de las curvas xsca).

- `GameCamera.h`: `m958[0x98C - 0x958]` partido para `float m96C, m970` (miembros de datos, no renumeran);
  `fn_8038E23C` con un cuarto `atn::Vector3*` (cambio de firma, sin nombres de parametro).
- `lbl_8087DCF0`..`lbl_8087DD00` (`.sdata` de esta unidad: floats globales, 0.0 salvo `lbl_8087DCF8` = 50.0)
  quedan `extern "C"` en `GameCamera_unk.cpp`: un cero solo queda en `.sdata` con `explicit_zero_data`
  (no aprobado aqui). Pendiente para cuando la unidad vaya a enlazar.
- `lbl_808859B0` (90.0) y `lbl_80885A6C` (2.5) de `.sdata2` pasan a `@N` locales (solo los usa GameCamera).
- `pd.sh` dejo 6 etiquetas ambiguas en GameCamera_unk; renombradas a mano en `symbols.txt` con el mismo
  desplazamiento (+653) que el resto (`labelcheck/rep100: igual`).
- `GameCamera.h`: declarado `fn_80387DE4(const CharaTask*)` (sin nombre de parametro; labelcheck igual) y
  renombrado en `symbols.txt`. `fn_800109E0` (inline) y las declaraciones de sin/cos movidas mas arriba en el `.cpp`.
- `Cross_unk(a, b)` (producto vectorial inline local) y `MotionFrame_unk(c)` (frame de la pista 0) en el `.cpp`.

## Hallazgos

- Un argumento que el llamador pone en r7 sin que la funcion lo lea: la funcion tiene un parametro sin usar
  (fn_8038E23C); sin el, el llamador no carga r7.
- `float t = MIN(...); h = t * (g - h0) + h0;` da el orden de `fmadds` del original; la expresion en linea no.
- Copias de vectores en un `case`: `VecAssign_unk(dst, expr)` arregla el cruce r3/r4 y r5/r6.
- El giro en y con `atn::Matrix3x4::fn_8013A13C` en lugar de `PSMTXRotRad` arreglo la planificacion del
  principio de fn_803884B8 (el original lee el angulo con r4).

- `side.ToRotation_unk()` (no copia + `fn_80139F60()`), `eye *= -d; eye += target;` (sin temporal) y
  `float yaw;` declarado antes de `height` dieron la pila y los registros de fn_8038378C/fn_8038A5F0.

## Pendientes

- `fn_80387DE4` (vista de pajaro, 1748 bytes): sin escribir (mando `lbl_8087EF70`, `fn_800A56A8`).
