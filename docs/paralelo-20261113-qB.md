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
- `fn_8037E9E0` (menu de depuracion "GameCamera"): escrita; el codigo coincide, solo cambian los
  desplazamientos de `@stringBase0` (faltan las cadenas "gun".."fixRotY" de fn_8037E12C, sin escribir).
- `~GameCamera` (0x8037DC68): escrito, NONMATCHING 99,97 %: el original destruye la camara de 0x00 con -1
  (como miembro, no como base): GameCamera tendria un `atn::Camera` en 0x00 en lugar de derivar de el.
- `fn_8038EF94` (fija los parametros de un modo; switch con tabla): **100 %**.
- `fn_8038E9B0` (colision del ojo de la vista de pajaro): escrita, NONMATCHING 90,6 % (orden de las cargas de los
  extremos del rayo, como fn_8038E540, y registros float).
- `fn_80387DE4` (vista de pajaro: giro con el stick del mando clasico, seguimiento del suelo): escrita,
  NONMATCHING 91,2 % (huecos de pila de los extremos del rayo del suelo y orden de las primeras cargas).
- Funciones al 0 %: 26 -> 16; al 100 %: 30 -> 32 / 90.

## NONMATCHING nuevas

- `fn_8038E23C` 96,5 %: ver arriba; probado `float height` antes/despues de `to`, `result` arriba, `VecAssign_unk`,
  copia campo a campo, `m89C++` despues de la copia, otra variable para la segunda copia.
- `fn_8038A5F0`/`fn_8038378C` 99,8 %: el original carga dos veces `m594.m0C` en la primera mezcla del
  desplazamiento lateral (`t * (m0C - -m0C) + -m0C`) y aqui se comparte la carga; probado una inline de mezcla
  de floats, otros ordenes de operandos y los parametros por puntero.

## Decisiones tomadas sin el usuario

- `GameCamera.h`: `float m7F8` (giro del lider) y `float mCA8` (velocidad de giro) en lugar de relleno.
- `GameCamera.h`: `GameCameraParam_unk m7B0` ("WallUp"; antes m7B4/m7BC/m7C0/m7C4 sueltos, ahora
  `m7B0.m04`...), `ResHandle m7EC` (y `#include "game/ResHandle.h"`: las 153 unidades que incluyen
  GameCamera.h ya la incluian; labelcheck igual), campos del menu m8D8, m8DC, m9EC, m9F0, m9F4, mA04, mA08 y
  la declaracion de `fn_8037E9E0()` y de `fn_8038EF94(const GameCameraParam_unk*, int)`.
- `GameCamera.h`: `atn::Vector3 m8B0` (copia de m8A4 de la vista de pajaro).
- `HasCeiling_unk`/`ViewHeight_unk`/`AddViewHeight_unk` (inlines locales) subidas antes de fn_80387DE4; una sola
  declaracion local de `fn_8004D388` (posicion, movimiento, mascara, ignorado, id, radio).
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

- Constructor (0x8037D8B8): inicializa `m34` de cada `GameCameraParam_unk` y los temporizadores: hace falta un
  constructor por defecto en `GameCameraParam_unk` (inline nueva en una cabecera de 153 unidades).
- Herencia de `GameCamera`: el destructor original trata la camara de 0x00 como miembro (ver arriba).
- `fn_8037E12C` (lee el csv de camaras del mapa, 1904 bytes): copia y reasigna campo a campo los ajustes de
  jugador de `GameConfig_unk` 0x244-0x330 como un struct (hoy campos sueltos en `GameConfig_unk.h`, otra
  familia); sus cadenas "gun".."fixRotY" arreglarian los desplazamientos de fn_8037E9E0.
- `fn_8038F52C` (3656 bytes, cambio de modo segun la accion del personaje, tabla de 0x8A casos) y
  `fn_80390A88` (3684 bytes): sin empezar. Los weak de 8-36 bytes (fn_8037F640 cos, fn_80382A00, fn_803830A0,
  fn_80387540, fn_8038B648, fn_803903C4, fn_803903CC) saldran al escribir las funciones grandes que los llaman.
- `fn_80382A1C`: sigue bloqueada (TownTask::fn_805B81F0 y `atn::Camera::SetUpVector` en cabeceras de otras
  familias).
- game/FinishCamera_unk: sin cambios (su unica funcion al 0 % tiene 13216 bytes).
