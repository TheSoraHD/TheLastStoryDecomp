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

## Decisiones tomadas sin el usuario

- symbols.txt (renombrados): fn_80313E44 -> `vf10__11Em315CoffinFv`, fn_80313FC8 ->
  `vf10C__11Em315CoffinFP10HitMsg_unk`, fn_80313FE8 -> `vf40__11Em315CoffinFP11HitInfo_unk`, fn_80314130 ->
  `vf100__11Em315CoffinFi`, fn_80315654 -> `fn_80315654__11Em315CoffinFv`. Ningun otro .cpp los usaba.
- include/game/Em315Coffin.h (solo la usan Em315Coffin.cpp y la factoria de SpecialEnemyBase.cpp): campos 0x15C8-0x15F4
  agrupados en `CoffinSkill_unk` (nombre inventado), 0x15F8/0x1604 Vector3 y 0x161C `CharaTask*` (destino del drenaje).

## Hallazgos MWCC

## Pendientes para otras colas
