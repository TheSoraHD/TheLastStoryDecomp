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

## Pendientes para otras colas
