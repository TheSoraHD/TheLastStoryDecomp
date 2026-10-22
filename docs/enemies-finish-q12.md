# Lote enemies-finish, cola q12 (segunda pasada de la cola q4)

Inicio: 2026-10-22 17:54 CEST. Rama agent-q12 (referencia start-q12: 7149 funciones al 100%).
Cola: game/Em303KrakenFoot, game/Em303KrakenFootMp, game/Em305Vampire, game/Em306GurgBeast, game/Em308InvertCrab,
game/Em308ParmCrab.

Estado inicial (report.json, funciones al 100% / total): Em303KrakenFoot 20/26, Em303KrakenFootMp 17/21,
Em305Vampire 26/30, Em306GurgBeast 11/22, Em308InvertCrab 9/29, Em308ParmCrab 5/24.

## Unidades

- `game/Em303KrakenFoot` (20/26, sin cambio en el recuento): escritas las dos que faltaban, fn_802E3EE8 (estado 8:
  gira hacia el objetivo, lo atrapa en el frame 22 con fn_801595BC, escribe el CatchNotifyFlag y lanza la voz
  0x2A; si se suelta, estado 16 o 10) y fn_802E470C (estado 11: arrastra al atrapado hasta el kraken y gira
  10 grados por frame hacia el). NONMATCHING 99.15% y 99.58%: registros del CanAct_unk en linea (misma familia
  que vf40/fn_802E55B0) y slots de pila de los temporales (~12 formas cada una: helpers Drop_unk/Release_unk/
  GetBack_unk/RotateY_unk, orden de declaracion, copias en linea). fn_802E3B24 llama ahora a
  `Em302Kraken::fn_802DC8B0` con su nombre real (se incluye game/Em302Kraken.h) y sigue al 100%.
- `game/Em303KrakenFootMp` (17/21 -> 18/21): escritas vf104 (0x802E683C, maquina de estados 7/8/9/10/2/12 y
  la de acciones) y fn_802E7740 (acciones m14E4: ataque cercano, eleccion aleatoria 8/9 con rand() % 2, vuelta
  al punto base) -> fn_802E7740 al 100%. vf104 NONMATCHING 99.82%: slots de pila de los dos bloques de
  movimiento en linea (escritos con goto; con un helper inline sale 99.0%).
- `game/Em308InvertCrab` (9/29 -> 16/29; detalle de la primera tanda): escritas vf10 (0x802F2294, maquina de estados 6-18, recuperacion
  con la skill m1550 sobre agua y el aviso de tutorial 6/17 del estado 17 con "SE_UICOM_011") y fn_802F3BE8
  (estado 8: carga, espera m14E4 frames y dispara la aguja desde "hips" con el giro limitado a +-m14F0/2).
  fn_802F2C50 se autoinlineaba en vf10: con 4 `((void)0)` sale fuera de linea como en el original. Con el fuente en
  el orden del .dol suben vf40, fn_802F2C50, fn_802F566C, fn_802F58A0 y vfEC. NONMATCHING: vf10 99.43% (slots de
  pila de los estados 13/17 y del aviso, registros de la busqueda en la tabla de rutas), fn_802F3BE8 99.93% (solo
  desplazamientos de .sdata2: faltan los estados fn_802F33A0-fn_802F5370, cuyas constantes van antes).
- `game/Em308ParmCrab` (5/24 -> 10/24; primera tanda): escrita vf10 (0x802F7E0C) 98.42% (slots de pila y planificacion del
  radio min(30, 0.8 * m152C), ~10 formas). Cabecera: m1564 pasa a `RoutePlace_unk*`.
- `game/Em306GurgBeast` (11/22 -> 14/22): escrita vf108 (0x802F0328, movimiento de los estados 6-16) con sus copias
  debiles fn_802F0964/fn_802F0988/fn_802F0990 (al 100%) y fn_802F0998 (56%: el original carga mFps antes del `lis`
  de la constante 0x43300000; 6 formas). vf108 97.84% (r5/r7 de las dos llamadas a fn_802F0964). Cabecera:
  m14D4 pasa a `CharaTask*` y m1518 a `RoutePlace_unk*`. Las copias debiles se definen `extern "C" inline` en el
  .cpp (los llamadores de gmk/Cannonball.cpp, gmk/FlamingArrow.cpp y game/Em312DarkServant.cpp siguen con sus
  propias declaraciones).

- `game/Em308InvertCrab` (segunda tanda): escritas fn_802F33A0 (movimiento; reutilizar el local `v` de la ruta
  para la direccion al objetivo da el 100%), fn_802F389C (eleccion de accion), fn_802F42CC (estado 9, giro en
  12 pasos y pesos del efecto a 1), fn_802F469C (estado 10, 99.22%: carga de m15A8 antes del -1.0f),
  fn_802F4A98 (estado 11, 97.05%: pila y orden de la rotacion de huida), fn_802F5370 (estado 12) y el trio
  fn_802F5BCC/fn_802F5F20/fn_802F62A0 (copias de los gemelos de Em308ParmCrab; 99.96/99.96/100). Con el pool
  .sdata2 ya completo quedan al 100% fn_802F33A0, fn_802F389C, fn_802F3BE8, fn_802F42CC, fn_802F5370 y
  fn_802F62A0. Queda sin escribir fn_802F693C (huida, 1.3 KB) y el slot 0x60 (fn_802F6E5C, ver pendientes).
- `game/Em308ParmCrab` (segunda tanda): escritas fn_802F8EBC (movimiento, 100%: `rot` declarado sin
  inicializar y asignado en cada rama, como en el original), fn_802F93F0 (eleccion de accion, 100%),
  fn_802F97B8 (estado 8, aguja: la direccion es (0, 0, 3), 100%), fn_802F9F3C (estado 9, 100%), fn_802FA2F4
  (estado 10, 99.30%: li r5 del radio de persecucion, como vf10), fn_802FA764 (estado 11, 97.19%, como el de
  InvertCrab) y fn_802FB0C4 (estado 12, 100%: se autoinlineaba en vf10; 4 `((void)0)` la sacan). Quedan
  fn_802FC2E0 (huida) y el slot 0x60.

## Resumen final

report.json: 7149 -> 7165 funciones al 100% (lost 0, fuzzdrop sin bajadas contra start-q12; main.dol OK en
todos los commits). Por unidad (inicio -> final): Em303KrakenFoot 20 -> 20/26, Em303KrakenFootMp 17 -> 18/21,
Em305Vampire 26 -> 26/30 (sin tocar: no quedaban funciones por escribir y sus NONMATCHING son bloqueos
documentados por q4), Em306GurgBeast 11 -> 14/22, Em308InvertCrab 9 -> 16/29, Em308ParmCrab 5 -> 10/24.
Ninguna unidad enlazada (`matching: true`): todas tienen NONMATCHING (registros del CanAct_unk en linea,
__VecCopy, push_back, std::string del ctor) o funciones sin escribir.

## Decisiones tomadas sin el usuario

- **symbols.txt (renombrados)**: Em303KrakenFootMp fn_802E683C -> vf104, fn_802E7740 ->
  `fn_802E7740__17Em303KrakenFootMpFv`; Em306GurgBeast fn_802F0328 -> vf108 (ranura 0x108); Em308InvertCrab
  fn_802F2294 -> vf10, fn_802F33A0/389C/3BE8/42CC/469C/4A98/5370 -> `<fn>__15Em308InvertCrabFv`,
  fn_802F5BCC/5F20 -> `...FP9CharaTask`, fn_802F62A0 -> `...FP9CharaTaski`; Em308ParmCrab fn_802F7E0C -> vf10,
  fn_802F8EBC/93F0/97B8/9F3C/A2F4/B0C4 -> `<fn>__13Em308ParmCrabFv`. Ningun otro .cpp los llamaba como
  `extern "C"`.
- **Em303KrakenFoot.cpp** incluye game/Em302Kraken.h para llamar a `Em302Kraken::fn_802DC8B0` y
  `fn_802DC5B4` con su nombre real (antes un `extern "C"` con struct local FootAction_unk).
- Cabeceras (solo de la cola): Em303KrakenFoot.h declara los helpers inline Drop_unk/GetBack_unk/DistSq_unk;
  Em306GurgBeast.h m14D4 -> CharaTask*, m1518 -> RoutePlace_unk*; Em308InvertCrab.h m1530 int (estado tras el
  giro), m1560/m1564 (lugar y punto de huida), m1588 CharaTask*, m15A4 int; Em308ParmCrab.h m1564
  RoutePlace_unk*, m159C/m15A0 (lugar y punto de huida). Nombres de helpers inventados (`_unk`).
- Em308InvertCrab::fn_802F2C50 y Em308ParmCrab::fn_802FB0C4: 4 `((void)0)` (asserts vacios aprobados) para que
  no se autoinlineen en vf10.
- Em308InvertCrab/ParmCrab: las funciones de estado 10/11/12 calculan `rot = d.ToRotation_unk()` y no lo usan:
  el original conserva el calculo (llamadas a atan2 visibles). Anotado como calculo sin uso del original, no
  inventado para mover constantes.
- Em308ParmCrab::fn_802F8EBC deja `rot` sin inicializar en la rama de ruta cuando no hay movimiento, como el
  binario (lectura de un valor indefinido en el original).
- Em306GurgBeast: las copias debiles fn_802F0964/0988/0990/0998 se definen `extern "C" inline` en el .cpp de la
  unidad (los demas llamadores siguen con sus declaraciones propias); structs locales BeastSettings_unk,
  BeastParams_unk y CollResult_unk.
- Em308InvertCrab::fn_802F62A0: el bucle de pesos del efecto se escribe con un array de 72 floats (el original
  lo desenrolla 8 x 9); struct local CrabWeights_unk.

## Hallazgos MWCC

- Una funcion pequena que el original llama fuera de linea y aqui se autoinlinea en una funcion grande: 4
  `((void)0)` la sacan (Em308InvertCrab::fn_802F2C50).
- Limitar un angulo con `d >= h / 2.0f ... d <= -h / 2.0f` da el `fneg f0, f8; fmuls f0, f0, f7` del original;
  con `* 0.5f` los operandos salen cruzados (Em308 fn_802F3BE8/fn_802F97B8).
- Reutilizar el Vector3 local de la ruta (`v = target->mPos - mPos;`) en vez de declarar otro reproduce la pila
  del original en los movimientos de los cangrejos.
- `float reach = 30.0f; if (p) reach = p->m40;` da el `lfs` de la constante antes del `cmpwi`; el `if/else`
  deja un salto de mas.
- `if (++a, b = 1, a >= c)` fija el orden de los dos stores (contador y bandera) del original.
- Un `atn::Vector3 rot;` declarado sin inicializar y asignado en cada rama (no `= mRot` al principio) quita la
  copia adelantada de mRot.

## Pendientes

- CharaTask::vf60 sigue declarada `void` (cabecera compartida): los slots 0x60 de los cangrejos
  (0x802F6E5C, 0x802FC848, devuelven 8.0f) no se pueden escribir sin cambiarla.
- Em306GurgBeast: vf104 (fn_802EF5FC, 3.3 KB) y fn_802F09D4 (1.4 KB) sin escribir.
- Em308InvertCrab fn_802F693C y Em308ParmCrab fn_802FC2E0 (la huida, ~1.3 KB cada una) sin escribir.
