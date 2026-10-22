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
- `game/Em308InvertCrab` (9/29 -> 11/29 en report): escritas vf10 (0x802F2294, maquina de estados 6-18, recuperacion
  con la skill m1550 sobre agua y el aviso de tutorial 6/17 del estado 17 con "SE_UICOM_011") y fn_802F3BE8
  (estado 8: carga, espera m14E4 frames y dispara la aguja desde "hips" con el giro limitado a +-m14F0/2).
  fn_802F2C50 se autoinlineaba en vf10: con 4 `((void)0)` sale fuera de linea como en el original. Con el fuente en
  el orden del .dol suben vf40, fn_802F2C50, fn_802F566C, fn_802F58A0 y vfEC. NONMATCHING: vf10 99.43% (slots de
  pila de los estados 13/17 y del aviso, registros de la busqueda en la tabla de rutas), fn_802F3BE8 99.93% (solo
  desplazamientos de .sdata2: faltan los estados fn_802F33A0-fn_802F5370, cuyas constantes van antes).
- `game/Em308ParmCrab` (5/24, sin cambio): escrita vf10 (0x802F7E0C) 98.42% (slots de pila y planificacion del
  radio min(30, 0.8 * m152C), ~10 formas). Cabecera: m1564 pasa a `RoutePlace_unk*`.
- `game/Em306GurgBeast` (11/22 -> 14/22): escrita vf108 (0x802F0328, movimiento de los estados 6-16) con sus copias
  debiles fn_802F0964/fn_802F0988/fn_802F0990 (al 100%) y fn_802F0998 (56%: el original carga mFps antes del `lis`
  de la constante 0x43300000; 6 formas). vf108 97.84% (r5/r7 de las dos llamadas a fn_802F0964). Cabecera:
  m14D4 pasa a `CharaTask*` y m1518 a `RoutePlace_unk*`. Las copias debiles se definen `extern "C" inline` en el
  .cpp (los llamadores de gmk/Cannonball.cpp, gmk/FlamingArrow.cpp y game/Em312DarkServant.cpp siguen con sus
  propias declaraciones).
