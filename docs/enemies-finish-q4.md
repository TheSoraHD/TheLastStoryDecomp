# Lote enemies-finish, cola q4

Inicio: 2026-10-22 11:59 CEST. Rama agent-q4 (sale de main 3fe5bd1; referencia start-q4: 6869 funciones al 100%).
Cola: game/Em303KrakenFoot, game/Em303KrakenFootMp, game/Em305Vampire, game/Em306GurgBeast, game/Em308InvertCrab,
game/Em308ParmCrab.

Estado inicial (report.json, funciones al 100% / total): Em303KrakenFoot 13/26, Em303KrakenFootMp 15/21,
Em305Vampire 15/30, Em306GurgBeast 9/22, Em308InvertCrab 0/29 (sin fuente), Em308ParmCrab 3/24.

## Unidades

- `game/Em303KrakenFoot` (13/26 -> 20/26, primera pasada): al 100% vf54 (0x802E315C, esfera de dano con
  `Vector_unk<Item14_unk>::push_back`), fn_802E3604 (eleccion de objetivo: lider libre o jugador mas cercano),
  fn_802E37DC (movimiento por ruta; en el estado 11 se para cerca del kraken m1504), fn_802E3B24 (maquina de
  acciones m14C0: pide la siguiente accion al kraken con fn_802DC8B0 y la arranca cerca del objetivo) y los tres
  inicios de ataque fn_802E4D6C/fn_802E500C/fn_802E52AC (estados 12/13/14, motions 0x142/0x14D/0x145). Con el pool
  renombrado a @stringBase0 suben al 100% el ctor, el lector del CSV y vf24.
  NONMATCHING: vf10 (0x802E289C) 99.63% (r3/r4 de la copia del nodo "reference", familia __VecCopy; 5 formas),
  vf40 (0x802E2F24) 99.40% y fn_802E55B0 (reaccion al dano, la llama Em302Kraken) 99.72%: registros del
  CanAct_unk en linea del personaje atrapado (r0/r4/r6 rotados; ~12 formas del inline, misma familia que
  Em314Ivy::vf104 y Em309GhoulSimple::vf10). vf0C sigue 99.71% (ya investigada).
  Quedan sin escribir fn_802E3EE8 (estado 8, atrapar, 2 KB) y fn_802E470C (estado 11, 1.6 KB).
- `game/Em303KrakenFootMp` (15/21 -> 17/21, primera pasada): vf54 (0x802E7298, igual que la de KrakenFoot) y
  vf100 (0x802E6228; estados 7/8/9/2) al 100%. vf10 sube de ~97.2% a 99.23% (helper `UpdateColli_unk` no inline
  que el compilador autoinlinea; quedan los registros del CanAct_unk y el r3/r4 del nodo). Fuente reordenada por
  direccion. Quedan vf104 (0x802E683C, 2.2 KB) y fn_802E7740 (2.3 KB, la llama vf104).

- `game/Em305Vampire` (15/30 -> 26/30): al 100% vf10 (0x802E9590; los estados 6/14/15 van inline en helpers
  `Tackle_unk`/`Rolling_unk`/`Stun_unk` y sus copias fuera de linea se escriben aparte con el cuerpo directo),
  vf3C, vf40, vf11C, vf14/vf20/fn_802EC38C (menu de depuracion "em305") al completar el pool, fn_802EAC70 (estado
  6), fn_802EAF68 (estado 7: barrido con fn_8004D388 a lo largo de m1518), fn_802EB1F4 (estado 8: abre la puerta
  con un `GimmickMsg_unk(4)` y la cruza), fn_802EBBA8 (inicio del estado 7). NONMATCHING: ctor 93.8% (registros
  salvados y planificacion de los stores de flags, como el de Em305VampireMp), vf0C 93.9% (push_back de
  std::vector en el bucle de rutas, bloqueo documentado; copia de Em305VampireMp::vf0C), fn_802EA490 98.8%
  (lwzux del punto de ruta + r3/r4 de copias de Vector3; ~12 formas) y fn_802EB61C 99.4% (f3/f4/f5 del cociente de
  vida y r28/r29 del bucle; ~20 formas). Em305VampireMp sigue 12/14.

- `game/Em306GurgBeast` (9/22 -> 11/22): al 100% vf100 (0x802EF16C, switch de estados; el caso 2 va entre el 13
  y el 14 como en el original y usa mF14) y fn_802F0F60. Con el pool `lbl_80748488` renombrado a @stringBase0
  (tamano 0xA9) no cambia nada mas. NONMATCHING: vf10 97.62%, fn_802F1120 96.3% y fn_802F1320 97.8%
  (planificacion/registros, ver los comentarios en la fuente). include/game/Em306GurgBeast.h: m152C pasa a
  `BattleCharaTask*`. Quedan vf104 (fn_802EF5FC, 3.3 KB), vf108 (fn_802F0328, 1.6 KB: llama fuera de linea a
  las copias debiles fn_802F0964/0988/0990/0998, que solo aparecen al escribirla) y fn_802F09D4 (1.4 KB).
- `game/Em308InvertCrab` (0/29 -> 9/29, fuente nueva): cabecera include/game/Em308InvertCrab.h (deriva de
  EnemyTask, 0x15D8 bytes; vf68/vf6C/vf90 inline, sus copias debiles salen al final del fichero) y
  src/game/Em308InvertCrab.cpp. La factoria (SpecialEnemyBase.cpp, caso 38) pasa a `new (3, "", "", 0)
  Em308InvertCrab(parent, info)`. Al 100%: dtor, vf14, vf40, vf68, vf6C, vf90, vfEC (0x802F59D8, muerte con
  cuenta atras m1434), fn_802F2C50 (rayo al suelo: atributos 0x14/0x15/0x1A-0x1D) y fn_802F58A0.
  NONMATCHING: ctor 99.81% (r26/r27 del segundo constructor de std::string, bloqueo conocido), vf20 99.77%
  (r3/r4 del __VecCopy), vf3C 98.80% (r5/r6), vf80 94.64% (pila del destino del salto) y fn_802F66CC 99.86%
  (r3/r4 del __VecCopy de la traslacion del nodo "hit_top" del jugador). Quedan fn_802F1A2C/fn_802F2294 (vf0C y
  vf10, 2 KB cada una) y los estados fn_802F33A0-fn_802F693C. fn_802F6E5C (slot vf60, devuelve 8.0f) no se
  escribe: CharaTask::vf60 esta declarada void en la cabecera compartida.
- `game/Em308ParmCrab` (3/24 -> 5/24): ctor escrito (99.83%, mismo bloqueo de std::string que InvertCrab) y la
  factoria (caso 37) pasa a `new`; cabecera completada con los campos que tocan las funciones escritas. Al 100%:
  vf40 (0x802F8A60; el efecto se escribe en linea con sus locales en el orden del original y lbl_8087F048 en un
  local), fn_802FBE00 (giro hacia el objetivo: `ToRotationZ_unk` y la comparacion `-0.3927f < d && d < 0.3927f`
  dan el 100%), ademas del dtor, vf90 y fn_802FC188 ya existentes. NONMATCHING: vf14 94.14% (temporales del
  color), vf20 99.78% (r3/r4 del __VecCopy), vf3C 98.84%, vf80 82.04%, vfEC 98.67% (el original pasa &rot desde
  r31), fn_802FB6AC y fn_802FBA30 99.96% (slots de pila de dir y del temporal de ToRotation_unk intercambiados).
  Quedan vf0C/vf10 y los estados (fn_802F8EBC-fn_802FB0C4, fn_802FC2E0).

## Decisiones tomadas sin el usuario

- **symbols.txt**: `lbl_8087D9E0`/`lbl_8087D9E4` (.sdata) -> `@STRING@reserve__Q23atn36Vector_unk<Q29CharaTask10Item14_unk>FUii@0`
  y `...FUii`: son los "" del `new (0, "", "", 0)` del reserve inline de `Vector_unk<CharaTask::Item14_unk>`
  (CharaTask::vf50 lo menciona en un comentario; los usan muchos enemigos en asm). Mismo criterio que q1 con
  `Vector_unk<Vector3>`.
- **symbols.txt**: pools `lbl_80747D90` (Em303KrakenFoot, tamano 0xF8 -> 0xF3 con el contenido real) y
  `lbl_80747EA0` (Em303KrakenFootMp) -> `@stringBase0` (scope:local, string_table).
- **symbols.txt (renombrados)**: Em303KrakenFoot fn_802E289C -> vf10, fn_802E2F24 -> vf40, fn_802E315C -> vf54,
  fn_802E3604/37DC/3B24/3EE8/470C/4D6C/500C/52AC -> `<fn>__15Em303KrakenFootFv`, fn_802E55B0 ->
  `fn_802E55B0__15Em303KrakenFootFi`; Em303KrakenFootMp fn_802E6228 -> vf100, fn_802E7298 -> vf54. Ninguno se
  llamaba como `extern "C"` desde otro .cpp (Em302Kraken sigue en asm).
- **splits.txt (Em305Vampire)**: `.rodata` de game/Em305Vampire pasa de 0x80747F60-0x80747F70 (solo el nombre
  RTTI) a 0x80747F58-0x80748238: absorbe `auto_06_80747F58_rodata` (double 0x4330000080000000 de la conversion
  int->float, solo lo usa Em305Vampire) y `auto_06_80747F70_rodata` (el pool de cadenas de la unidad). El pool
  `lbl_80747F70` pasa a `@stringBase0` (tamano 0x2C2, contando hasta "basic_string: length_error"). Build limpio OK.
- include/game/Em305Vampire.h (solo la usan Em305Vampire.cpp, Em305VampireMp.cpp y la factoria): m1518-m1520 pasa a
  `atn::Vector3 m1518` (movimiento de la embestida), m1544-m1558 a dos `atn::Vector3` (puerta), m155C a
  `VampireDoor_unk*`. Se declaran los helpers inline (nombres inventados).
- **symbols.txt (renombrados)**: Em305Vampire fn_802E8B88 -> vf0C, fn_802E9590 -> vf10, fn_802E9DEC -> vf3C,
  fn_802E9FC8 -> vf40.
- include/game/Em303KrakenFoot.h: m14D0 pasa a `CharaTask*` (personaje atrapado), m14CC (objetivo) y m1504 (el
  kraken) siguen como int con comentario (se guardan/leen como int en funciones ya al 100%). Struct local
  `FootAction_unk` {kind, arg} para el resultado de `fn_802DC8B0` (Em302Kraken, sin fuente).

- **symbols.txt (Em308)**: pools `lbl_80748558` (InvertCrab, 0x205) y `lbl_80748780` (ParmCrab, 0x278) ->
  `@stringBase0` (scope:local, string_table; sin scope:local el enlazador da multiply-defined). Renombrados: ctor
  de ParmCrab (fn_802F6E64), vf14/vf20/vf3C/vf40/vf80/vfEC de ParmCrab (fn_802F83E4/8678/87E0/8A60/8CA4/B39C),
  fn_802FB6AC/fn_802FBA30/fn_802FBE00 con firma; InvertCrab vf20 (fn_802F2AF8), fn_802F2C50, fn_802F58A0.
- **SpecialEnemyBase.cpp** (compartido, cambio minimo): casos 37 y 38 de la factoria con `new` de las clases
  nuevas; se quitan los `extern "C"` de fn_802F1524 y fn_802F6E64 y se anaden los dos includes.

## Hallazgos MWCC

- Si el original llama fuera de linea a inlines diminutas en una funcion (presupuesto superado), repartir el
  codigo repetido en helpers inline (`SetState_unk`, `PlayMotion_unk`) puede devolver el presupuesto y dar el
  100% (Em303KrakenFoot::fn_802E3B24 paso de 34% a 100%).
- Un bloque copiado en dos casos de un switch con el mismo orden de pila del original sale con un helper inline
  (Em303KrakenFootMp::vf100); escrito en linea dos veces, los slots quedan repartidos distinto.
- `bool up = (x & 0x40U) == 0x40U; if (up)` (con el radio asignado antes que el centro) fija el orden de cargas
  de vf54 (Item14_unk); el if directo o el orden centro/radio no.
- `float z = b.z - a.z; d.x = ...; d.z = z; d.y = 0` antes de `ToRotationZ_unk().y` en un `float` fija el orden
  de cargas de los dos operandos de la resta.

- `float ry = rot.y; PSMTXRotRad(m, 'y', ry)` dentro de un helper inline del vector frontal fija la
  planificacion de los argumentos de PSMTXRotRad (Em308 vf20); sin el local el compilador lee mRot.y por r3.
- En Em308ParmCrab::fn_802FBE00, `ToRotationZ_unk` (z leida una vez) da el 100% donde `ToRotation_unk` deja un
  frsp de mas; en fn_802FBA30/fn_802FB6AC es al reves.

## Pendientes para otras colas
