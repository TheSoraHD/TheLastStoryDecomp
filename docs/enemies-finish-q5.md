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
  - vf40: `int state; if (...) state = 8; else state = 13; vf100(state);` da el 100% (el ternario y el if/else con
    dos llamadas no).

## Decisiones tomadas sin el usuario

- symbols.txt (renombrados): fn_80313E44 -> `vf10__11Em315CoffinFv`, fn_80313FC8 ->
  `vf10C__11Em315CoffinFP10HitMsg_unk`, fn_80313FE8 -> `vf40__11Em315CoffinFP11HitInfo_unk`, fn_80314130 ->
  `vf100__11Em315CoffinFi`, fn_80314384 -> `vf104__11Em315CoffinFv`, fn_80315654 -> `fn_80315654__11Em315CoffinFv`. Ningun otro .cpp los usaba.
- symbols.txt (renombrados): fn_80310C64 -> `fn_80310C64__8Em314IvyFPCci`, fn_80312EF8 ->
  `fn_80312EF8__8Em314IvyFv`, fn_8031319C -> `fn_8031319C__8Em314IvyFP9CharaTask`, fn_803132D0 ->
  `fn_803132D0__8Em314IvyFv`. Ningun otro .cpp los usaba.
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

## Pendientes para otras colas
