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
- 16:10 segunda tanda: G (regla 10 en cabeceras, fn_800F8C6C miembro), H (funciones desbloqueadas: GameTask,
  Inventory, CosEditItemList_unk, Em305Vampire, ShopUpGrade), N (cerca de enlazar), A2 (forma original de push_back y
  copias debiles de Event). E (.rodata de BattleTask) espera a que acabe G, que tambien toca BattleTask.
- 17:01 integrada N (HomeButton::draw, HBMBase enlaza; 147 unidades cerca de enlazar probadas, casi todas
  registros): 15434, lost 0, sin bajadas, rep100 139. Reinicio del servidor a las ~17:00: G, H y A2 se reanudan solos.
- 17:22 integrada A2 solo con notas y parche (docs/cabeceras-20261114-qA2.patch): la forma original de
  push_back (fast + slow + expand_by + realloc) sube 37 funciones (Event/Manager fn_80481B9C 58 -> 96 %, Section/Cut/
  Channel 69 -> 92 %) pero baja RenderManager::RenderManager 88,7 -> 46,3 % (presupuesto de inline), ParsePATH_unk
  72,7 -> 16,5 %, ParseACID_unk y DataList_unk<Actor>::Add_unk. **Para revisar:** aceptar esas bajadas o otra pasada.
- 18:18 integradas H (sorts de MslSort_unk.h, CosEditItemList_unk sin funciones al 0 %, ItemManager_unk +6,
  ShopBase +1), G (fn_800F8C6C miembro de BattleTask, AiScriptIO_unk.h, RenderNode/RenderObj en LayerOcclusionCPU,
  sin copias locales de IRender, ctor de PathLengths_unk) e I (fn_80392D2C miembro de GameCamera,
  MenuCommonTask::sInstance en CostumeEditChara; LevelUpResult_unk != CharaStatusData_unk). Conflictos @N: version de
  la integracion; **ojo**: en los bloques en conflicto se perdian renombrados no @N de la rama del agente
  (fn_800F8C6C -> undefined al enlazar): `scratch/reapply.py <rama>` los vuelve a aplicar. pdbatch en 85 unidades y
  lblmap en 22. Total **15446**, lost 0, sin bajadas, rep100 138, labelcheck 20.
- 18:41 integrada A3 (solo notas y docs/cabeceras-20261114-qA3.patch): sin las 4 bajadas grandes de A2
  (construct(mData + mSize++, x); 20 asserts vacios en vector(const vector&); memmove en copias de vectores de
  punteros/int/float). Suben 45 funciones (Sequence::fn_8054204C 10 -> 93 %, ParsePATH_unk 73 -> 95 %); quedan 8
  bajadas de registros/huecos de pila (<= 0,72 %). Lanzada A4 para esas 8.
- 19:01 integrada E (datos de BattleTask en el fuente, .rodata igual al original, BattleTask 164 -> 165):
  15447, lost 0, sin bajadas, rep100 138, labelcheck 20, build limpio OK. **Cambio de split (para revisar):** la
  cadena "Event::ActionProcessor" (.rodata 0x80736530) y `__RTTI__Q25Event15ActionProcessor` (.data 0x80779E28, hueco
  auto_07) pasan a Event/Behavior_unk (aparece una sola vez en el DOL; la emite Behavior_unk.o, que enlaza). Commit f1ce542.
