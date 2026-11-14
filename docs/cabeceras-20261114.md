Inicio: 2026-11-14 13:20 | funciones al 0 %: 201 (report.json) | base main e992ba3: 15424/18109, lost 0, rep100 139, labelcheck 49

# cabeceras-20261114

Sesion de 10 h centrada en cabeceras compartidas, en paralelo con la fase 4 del port. Hasta 4 agentes.
Decisiones del usuario al empezar: cola G se hace (cambio de firma de fn_800F8C6C apuntado para el port);
merge final aunque el port siga activo; notas para el port solo en el proyecto (claude/port-cabeceras-20261114.md).

## Fase 0

1. `HitInfo_unk::m0C` -> `const wchar_t*`, `m50/m54` -> `float` (game/HitInfo_unk.h). Fuera los casts de BattleTask,
   TacView, Em202Spider y Em309Ghoul. Las inlines `HitArc_unk`/`HitHeight_unk` de BattleTask se quedan (ahora leen
   los miembros): sin ellas `vf14` baja 90,37 -> 90,29. lblmap en BattleTask: labelcheck 2 -> 0.
2. Fuera `typedef atn::ColliBase RayObj_unk` (BattleTask.h); GameUI ya usaba `atn::ColliBase*`.
3. `fn_801F6E78(inst, name, atn::D2A_DrawRect& r)` (no const: con `const&` la definicion baja a 99,28 %; con `&` sigue
   al 100 %). `GameUI::fn_803E0E10` 85 -> 93,5 %. Los temporales de la copia consumian `@N`: lblmap en GameUI.
