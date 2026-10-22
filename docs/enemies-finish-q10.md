# Lote enemies-finish, cola q10 (segunda pasada sobre la cola q1)

Inicio: 2026-10-22 14:51 CEST. Rama agent-q10 (sale de e407184; referencia start-q10: 7110 funciones al 100%).
Cola: game/em005_doppel_ganger, game/Em003LesserDaemon, game/Em101ElderZesya, game/Em101ElderZesyaSecond,
game/Em101LittleZesya, game/Em101LittleZesyaSecond (todas con fuente empezada).

Estado inicial (report.json, funciones al 100% / total): em005_doppel_ganger 8/17, Em003LesserDaemon 10/22,
Em101ElderZesya 10/20, Em101ElderZesyaSecond 11/15, Em101LittleZesya 18/25, Em101LittleZesyaSecond 8/15.

## Unidades

- `game/em005_doppel_ganger` (8/17 -> 9/17): escrita vf10 (5.7 KB) y la clase anonima `ControlCall`
  (ctor inline en vf10, vf0C y dtor). El fuente se reordena al orden del .dol (ctor, dtor, vf0C, vf10, vf54, vf14,
  vf3C, vf40, vf44, fn_80252E6C, fn_802531F4, vfF0, fn_80253884, fn_80253C9C, ControlCall::vf0C). Al 100%: dtor de
  ControlCall. NONMATCHING: vf10 96.3% (las pruebas `(m54C & 8) != 8` del original salen materializadas con
  subfic/or/srwi., mismo bloqueo que RouteWalker_unk::fn_80127EF4 y CharaTask::fn_80149A30; ~25 formas probadas
  en ficheros sueltos: bool/int, inlines, ternarios, mascara por parametro, xor, resta...); ControlCall::vf0C
  99.44% (r7/r8 en el bucle de busqueda y r26/r27 de dos vectores de pila; 6 variantes).

## Decisiones tomadas sin el usuario

- **symbols.txt (renombrados)**: fn_80253F00 -> `vf0C__Q233@unnamed@em005_doppel_ganger_cpp@11ControlCallFv`
  (scope:local), fn_802544F0 -> `__dt__Q233@unnamed@em005_doppel_ganger_cpp@11ControlCallFv`, lbl_80784340 ->
  `__vt__Q233@unnamed@em005_doppel_ganger_cpp@11ControlCall` (scope:local), lbl_807843B0 -> `__RTTI__...ControlCall`.
  Ningun otro .cpp los usaba.
- Cabecera `include/game/Em005DoppelGanger.h` (solo la usan su unidad y la factoria): m14FC pasa a `CopyRobotTask*`
  (vf10 llama `CopyRobotTask::fn_801CCD00` y escribe su m14B8); el relleno 0x1530-0x1540 pasa a `atn::Vector3 m1530`
  (posicion de la trampa, duena de los efectos) y `float m153C` (radio); nueva inline `HpRate_unk`.
- Em005: vistas locales con nombres inventados: `DoppelAi_unk` (pesos por miembro del grupo de fn_80103F60, como
  AllyAi_unk de Em403WarLion), `Settings_unk` (solo m80 de lbl_8087F0A8), `ControlTime_unk` (m08 del control en
  curso), inline `IsShown_unk` / `IsControlTimeUp_unk`; `Color4f_unk` gana un ctor de 4 floats.

## Hallazgos MWCC

- `bool r = (x & 8) != 8` no se materializa con subfic/or/srwi. en ninguna forma de C++ probada (bool/int/inline/
  ternario/mascara variable); `(8 - x) | (x - 8)) >> 31` escrito a mano si lo da, pero seria codigo inventado.
- En un argumento de llamada, calcular `m153C / 15.0f` en un `float` local antes del color temporal coloca el
  fdivs donde el original.

## Pendientes (esta cola)

## Pendientes para otras colas
