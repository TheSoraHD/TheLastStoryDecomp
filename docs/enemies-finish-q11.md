# Lote enemies-finish, cola q11 (segunda pasada de la cola q2)

Inicio: 2026-10-22 16:10 CEST. Rama agent-q11 (referencia start-q11: 7139 funciones al 100%).
Cola: game/Em202Spider, game/Em202SpiderBase, game/Em205Ogre, game/Em208Queen, game/Em209SmallGolem.

Estado inicial (report.json, funciones al 100% / total): Em202Spider 11/25, Em202SpiderBase 14/23,
Em205Ogre 17/21, Em208Queen 21/29, Em209SmallGolem 14/22.

## Unidades

- `game/Em202Spider` (11/25 -> 15/25): escritas vf0C (lector CSV), vf10, fn_8027EB14 (movimiento),
  fn_8027F504 (eleccion de accion), fn_8027FA7C (estado 5, aguja), fn_80280184 (estado 6, embestida),
  fn_802805C8 (estado 9, giro) y fn_80281F88 (inicio del estado 8: pared mas cercana, punto exterior y
  dos puntos de aguja). Con las tablas estaticas (sSe_unk, sSeGroup_unk) al principio del fichero y
  sArmNodes_unk antes de fn_8027F69C los pools .sdata2/.rodata coinciden y suben al 100% vf3C, vf40, vf7C,
  vfEC, fn_8027F504, fn_8027F8A8, fn_8027FA7C, fn_80280184, fn_802805C8, fn_80281054/1344/1634,
  fn_80281ED8 y fn_802829DC. NONMATCHING: ctor 96.45% (registros de std::string), vf0C 99.90% (push_back,
  caso abierto; pool de __allocate en otra unidad), vf10 99.23% (slots de pila, ~20 variantes), vf14
  93.04%, vf54 99.31% (__VecCopy), fn_8027EB14 99.76% (slots del dash), fn_8027F69C 99.12%, fn_80281924
  99.84%, fn_80281F88 97.13%, fn_802826A0 99.88% (f0/f3 de ToRotationZ_unk).
- `game/Em202SpiderBase` (14/23 -> 15/23): escritas vf0C (lector CSV), vf10, fn_80285064 (estado 8, aguja
  con matriz z/y/x) y fn_80285FA0 (estado 11, mantiene la distancia). El fuente estaba en otro orden que el
  .dol: reordenado por direccion, el pool .sdata2 queda identico. fn_802857B4 al 100% (if anidado con el
  frame en local). Al 100% en objdiff las del lote anterior (vf3C, vf40, vf7C, fn_802847CC, ...), que ya
  contaban en report. NONMATCHING: ctor 99.81%, vf0C 99.94% (push_back), vf10 99.93% (__VecCopy), vf14
  93.04%, vf80 83.22%, fn_80285064 99.88% (f8/f9 del limite de giro, 8 variantes), fn_80285FA0 99.65% (solo
  pool), fn_802878EC 99.97%.

- `game/Em205Ogre` (17/21 -> 19/21): escrita vf108 (99.69%: f3/f4 del vector de paso del golpe, r3/r4 de la
  copia de la cabeza). Con el fuente en el orden del .dol (dtor justo tras el ctor y vfB4 al final, como en el
  .dol) el pool de cadenas y .sdata2 coincide y suben vf30 y vf104 al 100%. NONMATCHING: vf108 99.69%,
  fn_802B98B0 98.61% (__VecCopy y FPR del punto medio de los pies).
- `game/Em208Queen` (21/29 -> 22/29): fn_802CC4C8 al 100% leyendo la tabla del campo dentro del helper
  (`FindRoutePlace3_unk`); fn_802CAFB8 95.51% -> 99.92% asignando tipo y valor del registro de dano con
  un setter (`Set_unk`). El resto (vf10 99.86, vf14 99.58, vf20 99.81, fn_802CA8C0 97.24, fn_802CB43C 97.60,
  fn_802CC19C 97.29) son registros/__VecCopy ya documentados por q2; 3-5 variantes mas sin cambio.
- `game/Em209SmallGolem` (14/22 -> 16/22): escrita vf10 (99.53%; resucitar al golem con el boton, ataque de
  agujas, salto del estado 9, color de los materiales "em209_core"/"em209_core_light" y caida bajo el
  jugador). Suben fn_802CF728 y vfEC al 100%. Quedan al 99.8-99.9% fn_802CF054, fn_802CF3AC, fn_802CFB24,
  vf1C y el __sinit solo por el pool .sdata2: nuestro .o tiene un 5.0f de mas, el de la copia debil de
  `CharaTask::Item14_unk::Item14_unk()` (que vf50 emite al tomar su direccion para __construct_new_array).
  El original de esta unidad no la emite (Em205Ogre si: su 5.0f es la ultima constante). Comprobado
  cambiando a mano el 5.0f de la cabecera: las 5 funciones suben al 100%. No se toca la cabecera compartida
  (CharaTask.h): pendiente.

## Decisiones tomadas sin el usuario

- **symbols.txt (Em202Spider)**: fn_8027CF28/D838 -> vf0C/vf10__11Em202SpiderFv (ranuras 0x0C/0x10 de la
  vtable 0x80785500); fn_8027EB14/F504/FA7C/80280184/805C8/81F88 -> `<fn>__11Em202SpiderFv`. Datos locales:
  lbl_80744E60 -> sSe_unk, lbl_807854E8 -> sArmNodes_unk, lbl_80883940 -> sSeGroup_unk (scope:local; ningun
  otro fichero los usa, xref en build/SLSP01/asm).
- **symbols.txt (Em202SpiderBase)**: fn_80283004/837F4 -> vf0C/vf10__15Em202SpiderBaseFv;
  fn_80285064/85FA0 -> `<fn>__15Em202SpiderBaseFv`.
- **Em202Spider.h**: m14C0/m14CC son `std::vector<RoutePlace_unk*>` ("needle_point"/"outer_point"),
  m1508..m1518 `HitInfo_unk*` (skills del CSV), m14E0 float (radio del CSV), m14F4 como int[2].
- **Em202SpiderBase.h**: m14B8/m14FC como char[0x20] (nombres de sonido "MoveSE"/"NeedleChargeSE" copiados
  con sprintf). Los "BasePoints" se guardan como `SpiderBasePoint_unk*` haciendo cast de RoutePlace_unk*
  (mismo inicio: id y posicion).
- `mC58.mC8 = (int)mC58.mC4` en las vf10: mC8 es int en la cabecera compartida CharaTask.h (no se toca).
- Em202SpiderBase vf10: un helper `MovePos_unk` con una copia muerta de la posicion (`atn::Vector3 prev`),
  porque el original deja el psq_st de esa copia en la pila. Anotado como decision (copia sin uso).

## Hallazgos MWCC

- El pool .sdata2 de una unidad depende tambien de las tablas estaticas con punteros de .sdata2 (un `static
  const char* const p = "..."` define una palabra en .sdata2): declararla donde el original la tiene (al
  principio del fichero en Em202Spider) desplaza todo el pool.
- Una funcion de estado de ~20 sentencias que el original llama fuera de linea se autoinlinea en la vf10 si
  usa `if (fn_805F9920(&d) < k)`; con `float dist = fn_805F9920(&d); if (dist < k)` deja de inlinearse.
- `m620 = r; m614 = pos; m614.y += m620;` (miembro asignado antes que la copia) da el orden de stores
  m61C/m620 del original en la capsula de las aranas.
- `float f = frame; if (5.0f <= f && f <= 16.0f) {...}` dentro del else (en vez de `else if` con dos lecturas
  del miembro) da el orden de cargas del original (constante antes que el frame).
- Un `atn::Vector3 top = pos;` declarado despues del if del ataque (no al principio) coloca el temporal
  donde el original en las vf10 de las aranas base.

## Pendientes

- Em209SmallGolem: el ctor inline de `CharaTask::Item14_unk` emite en esta unidad una copia debil con su
  5.0f en .sdata2 que el original no tiene; resolverlo exige que esa unidad vea el ctor solo declarado
  (cambio en include/game/CharaTask.h, compartida). Ganaria 5 funciones (fn_802CF054, fn_802CF3AC,
  fn_802CFB24, vf1C, __sinit).

## Pendientes para otras colas
