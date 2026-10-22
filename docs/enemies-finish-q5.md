# Lote enemies-finish, cola q5

Inicio: 2026-10-22 09:05 CEST. Rama agent-q5 (sale de main 64df7a6; referencia start-q5: 6806 funciones al 100%).
Cola: game/Em309Ghoul, game/Em309GhoulSimple, game/Em310Gargoyle, game/Em312DarkServant, game/Em314Ivy,
game/Em315Coffin.

## Unidades

- `game/Em315Coffin` (7/21 -> 11/21): vf10 (0x80313E44), vf10C (0x80313FC8), vf40 (0x80313FE8) y vf100
  (0x80314130) al 100%. fn_80315654 (capsula de colision) 99.46%: cruce r3/r4 de las copias de Vector3 desde
  temporales (familia `__VecCopy`, bloqueo documentado; 4 variantes).
  - Los seis contadores 0x15C8-0x15F4 son tres `CoffinSkill_unk` {id, ?, intervalo, cuenta}; 0x803140B4 es la copia
    debil de su `Reset_unk` (`mCount = mInterval`), sin escribir aun (la emite el primer llamador sin inlinear).
  - vf104 (0x80314384, maquina de estados) escrita: 99.81%. Queda la numeracion de FPR del inicio/actualizacion
    inline del fundido `CoffinFade_unk` (0x1638: tiempo, duracion, velocidad, activo, desde, hasta, valor, delta);
    ~16 variantes. Claves que si ayudaron: `ColorF_unk(GXColor)` como en GimmickCommon, `FadeTo_unk` que lee
    mValue dentro del inline, `range`/`d` en locales en la interpolacion, inline de enlace que relee mC58.mC4.

- `game/Em314Ivy` (4/18 -> 7/18): lector del CSV fn_80310C64, vf40 y vfF8 al 100%. Escritas tambien
  fn_80312EF8 (eleccion del jugador mas cercano, 99.96%, solo el offset de 1e16f en .sdata2), fn_8031319C (atrapar,
  99.94%, offset de 10.0f), fn_803132D0 (soltar, 99.50%: registros salvados y pila del bucle de efectos) y vf10
  (99.34%: pila y copias de Vector3). vf14 sigue 83.25% (el original carga el punto antes de elegir el color; ~12
  variantes). vf0C, vf100/104/108 grandes sin escribir.
  - En el report completo fn_80312EF8 y fn_8031319C estan al 100% (fnstat.py suelto las daba al 99.9% por el
    desplazamiento del pool de .sdata2): 9/18.
  - vf30 al 100% con el `\0` final del formato "Em314 Ivy[%d]\0" (10/18).
  - vf40: `int state; if (...) state = 8; else state = 13; vf100(state);` da el 100% (el ternario y el if/else con
    dos llamadas no).

- `game/Em312DarkServant` (12/21 -> 14/21): vf0C y el lector del CSV fn_8030DEB0 pasan al 100% al renombrar el
  pool `lbl_807490AC` a `@stringBase0`. Escritas vf100 (0x8030E710, 95.88%: recursion de fn_8030E5C8 inlineada
  tres niveles, bloqueo documentado, y recarga de m54C antes del store de m14A8) y fn_8030F380 (escudo, 99.92%:
  solo el offset de 36.0f segun fnstat; 100% en el report completo). vf104/vf108 (2.4 KB cada una) sin escribir.

- `game/Em312DarkServant` (segunda vuelta, 14/21): vf30 al 100% con un bloque `{ void* sub = ...; }` por
  sub-grupo (el original da un registro nuevo a cada `sub`) y el `\0` final de "Em312 DarkServant[%d]\0" (el
  pool guarda un terminador extra detras). Mismo arreglo aplicado a Em315Coffin::vf30 y Em309GhoulSimple::vf30.
- `game/Em309GhoulSimple` (9/22, sin nuevas al 100%): escritas vf10 (0x8030646C, 99.57%: registros volatiles del
  CanAct_unk inline), vf20 (0x80308D14, capsula, 99.76%: r4/r5 de la copia del cuello) y vf30 (0x80309E6C,
  99.92%: offsets del pool). Las grandes (vf100/104/108, fn_803067E0 vf40, fn_80308E6C) y fn_803097AC (slot 0xB0:
  CharaTask::vfB0 esta declarada `void vfB0()` en la cabecera compartida, devuelve int y recibe un Vector3*) sin
  escribir; fn_80309D20/fn_80309D50 son invoker/manager de boost::function (bind de ControlThrustForGhoul).
- `game/Em310Gargoyle` (2/21 -> 3/21): vf40 (0x8030B5B0, derrota: banderas 0xCF/0x66 del campo y efecto
  ef_uc037) al 100%. vf3C (0x8030B41C, modos de dano "DamageCalcMode") 99.99%: solo el offset de "head" en el
  pool. vf0C (0x8030A6A0) 98.41%: r3/r4 de la palabra de banderas y del pool, r3/r4/r5 de los stores finales
  (~30 variantes). Ctor (tabla de jinetes lbl_80787F68 de 0x78 bytes por tipo y rutas del campo) y el resto
  (vf10/vf14 grandes, fn_8030B408/fn_8030C190 copias debiles sin tipo) sin escribir.
  - Cabecera: campos sacados del relleno (m14F1, m14F4[3] ids de jinete, m1500[3], m15E0, m1604, m1608, m160C).
- `game/Em309Ghoul` (8/33 -> 10/33): vf0C (0x802FD134, espera de 10 efectos y 2 xsca, busca el gimmick 0x80A)
  y fn_802FF6C4 (eleccion del objetivo: el lider del grupo cuando m1524 llega a m16F8; pesos por jugador del
  efecto) al 100%. vf20 (0x802FF604, capsula) 99.48%: r5/r6 de las copias de temporales (familia `__VecCopy`).
  Las 20 restantes son grandes (0.5-4.6 KB).
  - Segunda vuelta (11/33): vf40 (0x802FE3E4, dano acumulado y muerte con dos motions, inline `Die_unk`) al 100%;
    vf54 (0x802FF2E4, tres esferas de dano en "spine1") 93.27% (r6/r7 de los temporales de suma).
- Pools de cadenas renombrados a `@stringBase0` en Em309Ghoul (0x80748A14), Em309GhoulSimple (0x80748DC0),
  Em310Gargoyle (0x80748FE8) y Em315Coffin (0x80749390): sin cambio en el recuento, pero las llamadas con
  cadenas ya comparan el nombre real.
- Nota: `fnstat.py` compila la unidad suelta y muestra al 99.9% funciones que solo difieren en el offset de una
  constante de .sdata2; el report completo las da al 100%. Fiarse del report.

## Decisiones tomadas sin el usuario

- symbols.txt (renombrados): fn_80313E44 -> `vf10__11Em315CoffinFv`, fn_80313FC8 ->
  `vf10C__11Em315CoffinFP10HitMsg_unk`, fn_80313FE8 -> `vf40__11Em315CoffinFP11HitInfo_unk`, fn_80314130 ->
  `vf100__11Em315CoffinFi`, fn_80314384 -> `vf104__11Em315CoffinFv`, fn_80315654 -> `fn_80315654__11Em315CoffinFv`. Ningun otro .cpp los usaba.
- symbols.txt (renombrados): fn_80310C64 -> `fn_80310C64__8Em314IvyFPCci`, fn_80312EF8 ->
  `fn_80312EF8__8Em314IvyFv`, fn_8031319C -> `fn_8031319C__8Em314IvyFP9CharaTask`, fn_803132D0 ->
  `fn_803132D0__8Em314IvyFv`. Ningun otro .cpp los usaba.
- symbols.txt: `lbl_807490AC` -> `@stringBase0` (scope:local, pool de cadenas de Em312DarkServant);
  fn_8030E710 -> `vf100__16Em312DarkServantFi`, fn_8030F380 -> `fn_8030F380__16Em312DarkServantFv`.
- symbols.txt: fn_8030646C -> `vf10__16Em309GhoulSimpleFv`, fn_80308D14 -> `vf20__16Em309GhoulSimpleFv`,
  fn_80309E6C -> `vf30__16Em309GhoulSimpleFPv`; include/game/Em309GhoulSimple.h declara vf20 y vf30.
- symbols.txt: fn_8030A6A0 -> `vf0C__13Em310GargoyleFv`, fn_8030B41C -> `vf3C__13Em310GargoyleFP11HitInfo_unk`,
  fn_8030B5B0 -> `vf40__13Em310GargoyleFP11HitInfo_unk`.
- symbols.txt: fn_802FD134 -> `vf0C__10Em309GhoulFv`, fn_802FF604 -> `vf20__10Em309GhoulFv`, fn_802FF6C4 ->
  `fn_802FF6C4__10Em309GhoulFv`, fn_80304B40 -> `fn_80304B40__10Em309GhoulFv`, fn_803057A8 ->
  `fn_803057A8__10Em309GhoulFv` (las dos ultimas solo declaradas, sin escribir), fn_802FE3E4 ->
  `vf40__10Em309GhoulFP11HitInfo_unk`, fn_802FF2E4 -> `vf54__10Em309GhoulFv`.
- include/game/Em309Ghoul.h: campos sacados del relleno (m14EC, m150C, m1518, m1524, m1614 `GimmickCommon*`,
  m16F8); solo la usa Em309Ghoul.cpp.
- symbols.txt: pools `lbl_80748A14`, `lbl_80748DC0`, `lbl_80748FE8`, `lbl_80749390` -> `@stringBase0`
  (scope:local).
- include/game/Em314Ivy.h: m14E8 pasa de int a `void*` (matriz del nodo "catch_point") y se anaden las
  declaraciones de las cuatro funciones; solo la usan Em314Ivy.cpp y la factoria.
- include/game/Em315Coffin.h (solo la usan Em315Coffin.cpp y la factoria de SpecialEnemyBase.cpp): campos 0x15C8-0x15F4
  agrupados en `CoffinSkill_unk` (nombre inventado), 0x15F8/0x1604 Vector3 y 0x161C `CharaTask*` (destino del drenaje);
  0x1638-0x1654 agrupados en `CoffinFade_unk` (nombre inventado).

## Hallazgos MWCC
- `if (!(s != 9 && s != 10))` evita que MWCC fusione `s == 9 || s == 10` en `subi/cmplwi` (Em314Ivy::fn_80312EF8;
  ya estaba en la guia para 2/3, confirmado).
- Un argumento elegido con `?:` entre dos constantes se evalua en otro orden que `int x; if (...) x = A; else
  x = B; f(x);` (este ultimo da el orden del original en Em314Ivy::vf40).

- En los menus de depuracion (vf30) de los enemigos especiales, cada sub-grupo en su propio bloque
  `{ void* sub = fn_8008937C(...); ... }` reproduce el reparto de registros salvados; reutilizar una variable
  `sub` lo cambia.

## Pendientes para otras colas
- `CharaTask::vfB0` (cabecera compartida include/game/CharaTask.h) esta declarada `void vfB0()`; en los enemigos
  (Em309GhoulSimple 0x803097AC, Em205Ogre 0x802BA64C) devuelve int y recibe un `Vector3*`. Cambiarla afecta a
  todas las unidades: no tocado.
