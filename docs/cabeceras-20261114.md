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

## Agentes e integraciones

- 14:00 primera tanda: A (push_back de vector.h), B (string.h/string_members.h), C (CharaStatus.h), D (Model_unk.h/Task.h).
  Referencia de las copias tras la fase 0: labelcheck 46, rep100 139.
- 15:16 integrada C (CharaStatus.h: m2CC FixedArray, CharaStatusTail_unk, MotionCommand_unk::m10 bool, ctores de
  ExpEntry_unk/LevelUpResult_unk en InventoryExp_unk.h, db/PlayerParam_unk.h = EquipSet/LevelSkills/GameUISkillTable):
  15424 -> 15426, sin conflictos. Notas: docs/cabeceras-20261114-qC.md.
- 16:04 integradas B (string.h), A (vector.h), D (Model_unk.h/Task.h) y F (GameCamera miembro). Conflictos de
  symbols.txt (B 32 bloques, A 3, D 32; solo @N salvo 0x80885700 y 0x8074A2A8): version de la integracion. Despues:
  `@enum$6929ControlAppear_cpp` (lo que emite el .o), `pdbatch.sh` de 384 unidades (13 min, ninguna baja), revertidos 25
  renombrados que no son @N (`sPath`, `sInstance`, `__vt__`, `@STRING@`..., como avisaron B y D), `lblmap.py` en 45
  unidades y a mano 0x80759740/48 de PacketPack (@18171/@18172). Total **15431**, lost 0 frente a la integracion
  anterior y a cada agente, sin bajadas, rep100 139 (= ref), labelcheck 49 -> 20 lineas (MapPartsBase 10 -> 11
  etiquetas, aceptado). `pairdata.py --min N` nuevo (filtra funciones por %).
