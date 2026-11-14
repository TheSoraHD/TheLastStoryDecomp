# port-decomp-20261114 - cola K (restos al 0 % de la partida)

Inicio: 2026-11-14 06:25 CET. Rama `agent-K`. Unidades: `game/AiTarget`, `game/BattleTask`, `game/GameTask`,
`game/MiniMapTask`.

## Resultado por funcion

- `AIManager::vf14` (0x80014A5C, 7724 bytes, antes `fn_80014A5C`): **escrita, NONMATCHING 93 %** (objdiff; fnstat
  98,9 %). Firma `virtual void AIManager::vf14()` (nueva en `AiTarget.h`; es la entrada 0x14 de `__vt__9AIManager`).
  Vista de depuracion de la IA: con "DebugMode" (`m50`) las teclas del teclado USB (puerto 0: 0xF1B8/0xF1B2 cambian
  `m78`, el tipo; puerto 4: 0xF1B4/0xF1B6 el personaje anterior/siguiente de su lista) eligen un personaje
  (`m88` TPC, `m8C` enemigo, `m80` amigo, `m7C` jugador, `m84` NPC) y se dibuja un panel con su estado de IA
  (`CharaC58_unk`: estado, modo, contadores, puntos de ruta, areas, `m140`, banderas), los circulos de sus puntos
  de ruta, una linea sobre la cabeza y el cono de vision (`CharaRes_unk::m4C/m50/m58`); con "DrawHate" (`m54`) la
  lista de odio de `BattleTask::mCharas`; con `m58` los 16 valores `m9C` de los scripts. Las tres tablas de nombres
  (estados, modos, tipos) son `static` de fichero al principio de `.rodata` (el original las direcciona desde el
  ancla `lbl_8072FA00`). Resto: asignacion de registros de los punteros (this/chara/scene/res), orden de evaluacion
  de los argumentos de las palabras de banderas ("Task", "Action") y la carga del angulo del cono.
- `AIManager::fn_8001794C` (0x8001794C, 1348 bytes): **escrita, NONMATCHING 97,8 %**. Firma `int
  fn_8001794C(CharaTask*)` (sin cambio). Paso de IA de los NPC (tipo 1 con `mC50 == 1`): solo cada cuarto frame
  (`m90 == mId % 4`; si no, `mC58.m04++` y 0), bloque de entrada reducido y vuelta del estado y de `m140`/`m9C`.
  Resto: el mismo bloqueo que `fn_80017128` (ver hallazgos).

## Cambios de firma / layout (para el port)

- `AIManager::vf14()` declarada (virtual) en `include/game/AiTarget.h`; `AIManager::m88` pasa de `int` a `NpcTask*`.
  Simbolo renombrado `fn_80014A5C` -> `vf14__9AIManagerFv`.
- `CharaC58_unk::m140` pasa de `unsigned char[0x60]` a `float m140[24]` (valores de los scripts; los `memset` de
  `CharaC58_unk.cpp` siguen igual).
- `CharaLinkTarget_unk` (`CharaTask.h`): `int m04` (id de grupo) dentro del relleno.
- `ResManager` (`game/ResHandle.h`, `lbl_8087EF10`): `int m04` (menu abierto; es el `Mindows_unk::m04` de
  `atn/MindowsCommon.h`, que declara `lbl_8087EF10` con otro tipo: pendiente unificar).
- `AiTarget.cpp`: `lbl_8087F048` declarado como `BattleTask*`.

## Decisiones y hallazgos

- Cadenas de formato del original seguidas de un `""` en `@stringBase0`: se escriben con `"\0"` final
  (`"%d\0"`), como en el original.
- Conversiones int -> float en bucles (`m140[i] = out->m18[i]`): MWCC aqui guarda la palabra 0x4330 una vez en el
  prologo (huecos propios) y desenrolla x8; el original la guarda antes de cada elemento en el hueco compartido con
  los float -> int (0x18) y desenrolla x6/x4. No sale con cast, inline, puntero, `unsigned` ni `do/while` (pruebas
  sueltas): bloquea `fn_80017128` y `fn_8001794C`.
