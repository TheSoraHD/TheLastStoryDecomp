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
- 19:24 integrada J (fn_802A4170/84 = ctor y operator[] de std::vector<EnemyTask*>, PostFx*::vf10(RenderManager*)
  con vtable de 0x20, una sola atn::VideoTHP y atn::TextureComposite.h; PathLengths_unk en WindyItem/PathData_unk/
  ActionActorPosition sigue sin salir: 0x8000D844 es el dtor de vector<float>, depende de vector.h): 15447, lost 0,
  sin bajadas, rep100 138, labelcheck 20.
- 19:24 integrada A4 (solo notas y docs/cabeceras-20261114-qA4.patch): cada nivel de inline invierte el orden
  de los huecos de pila (selector por tipo `__vec_grow_sel_unk`), `__recommend_ref_unk` y `__destroy_front_unk`
  aparte. +5 funciones al 100 % (push_back de CosEditItem_unk, CosEditWeaponItem_unk, KrakenCommand_unk,
  KrakenMpCommand_unk, gs::HideLink2_unk); quedan 4 bajadas de registros (DebugInfoColumn_unk copia 99,64 -> 99,00,
  NewCampPartsItem fn_80517B3C 99,40 -> 99,07, Manager fn_80484088 95,18 -> 94,97, GameEventScript fn_8039F08C
  99,97 -> 99,83) y 5 de <= 0,03. **Para revisar.** Lanzada A5 sobre esas bajadas.
- 20:04 integrada A5 (solo notas y docs/cabeceras-20261114-qA5.patch, el mejor: A3+A4 + __construct_at_end_unk
  por referencia + selector de U16RunPair_unk): 15450 con el parche (+5 push_back al 100 %, 2 de ControlAppear se
  recuperan re-emparejando) y 8 bajadas de registros (4 de 0,01-0,02). **Decision del usuario**: integrarlo aceptando
  las bajadas o seguir. No se lanzan mas pasadas de push_back.
- 20:54 integradas K (BattleTask 165 -> 168: fn_801002DC, fn_800F80B8, fn_800F8290; MeleeSort_unk 73 -> 92,5 %,
  fn_800FDF14 94,3 -> 96,0 % con el marco del original, fn_800FD658 -> 99,75 %; .data sin resolver: un L"" y un
  segundo L"%d" sin fusionar en @wstringBase0) y L (84+11 unidades usan BattleTask* lbl_8087F048 en vez de vistas
  Effect_unk/void*; BattleTask::fn_8013A19C(int); Event/MovieState_unk.h fuera: Manager usa atn::VideoTHP).
  Sin conflictos. 15450, lost 0, sin bajadas, rep100 138, labelcheck 19. Reinicio del servidor ~20:43 (K, L, M
  reanudados solos).
- 21:32 cola M **no integrada** (rama aparte `cabeceras-20261114-m`, sobre 470dd42): `Frustum_unk::mBounds` como
  subobjeto (la copia de atn::Camera usa un puntero base) y GameCamera::fn_8038C5B0 48,6 -> 99,6 %: +5 funciones al
  100 % (fn_80392D2C, copia debil de SetCamera_unk, Camera(const Camera&), NewCampTask fn_8052C6CC,
  Sequence::fn_8053E8D0), pero bajan NewCampTask fn_8052E1A0 71,31 -> 70,81 y fn_8052FA44 73,22 -> 73,12, y el merge
  exige cero bajadas. **Para revisar.** Renumera 253 unidades (+2 @N); notas: docs/cabeceras-20261114-qM.md (en esa rama).
