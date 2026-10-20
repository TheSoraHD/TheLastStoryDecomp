# Lote batch-auto-5kb (2026-10-20): resumen y decisiones a revisar

Lote de decompilacion sobre las unidades creadas por `splits-auto`: las 98 unidades partidas sin fuente con `.text` <= 5 KB (`targets.py --max-unit 5120`) mas los 11 `_cont` de esa lista (los 4 de 1-3 funciones y, a peticion del usuario, los 7 de 4-7 funciones), fusionandolos con su unidad enlazada cuando se pudo. 9 colas (q1-q4 `atn`, q5-q6 `db`, q7-q8 `game`/`Event`/`MP`/`AI`/`chr`/`gmk`, q9 los 7 `_cont` grandes), repartidas por familia y por tramos de direcciones contiguos, con 4 agentes a la vez, cada uno en su copia y rama `agent-qN`, integradas con `git merge --no-ff`. Notas completas de cada cola (resultado por unidad, NONMATCHING con motivo, decisiones y hallazgos MWCC) en `docs/batch-auto-5kb-q{1..9}.md`.

Permisos del usuario para este lote: ampliar splits de las unidades del lote, renombrar en `symbols.txt` y hacer push de la rama (no de `main`).

## Resultado

- Build limpio (`rm -rf build/SLSP01`) con `main.dol: OK`; `regress.py` contra `main` (8c2a8fd): **lost: 0**.
- Funciones al 100%: **4758 -> 5356** (+598) al cerrar el lote, **5375** tras las excepciones aprobadas. Codigo 15,12% -> 16,69%; fuzzy 24,72% -> 27,45%; datos 15,21% -> 15,89%; ficheros enlazados 299 -> 335.
- Unico descenso de fuzzy: `atn::D2A_Message` ctor 66,54% -> 66,31% y `vf18` 91,59% -> 91,54%, por el cambio de `std::basic_string::operator=` de q3 (ver decisiones).
- De las 109 unidades del lote: 108 con fuente (queda en asm `Event/Manager_cont`), 44 enlazadas (`matching: true`, contando las unidades enlazadas que absorbieron un `_cont`).
- `_cont`: fusionados 8 (`FriendManager`, `MapPartsOcean`, `SetConfig`, `GameTimer`, `MP/SaveLoad`, `NpcManager`, `atn/Bgm`, `MarketTask`); quedan 3: `atn/EffectManager_cont` (unidad propia enlazada, 2/2), `atn/LH_Motion_cont` (unidad propia 3/4), `Event/Manager_cont` (sin fuente).
- Casi todos los NONMATCHING son reparto de registros o huecos de pila (96-99,9%). Bloqueos repetidos: el ctor inline de `atn::CsvU` en registro salvado (q5, q6; ya conocido), `push_back` de `std::vector`, plantillas de MSL/boost sin reconstruir (`std::map` en XsfParser, `boost::lambda` en SequenceHolder).

## Arreglos de integracion (hechos al juntar ramas)

Renombrados de una cola que otra seguia usando como `extern "C"` (fallo de enlace `undefined: fn_...`); se cambio el llamador por la clase/miembro real, sin cambiar codigo:
- `DebugMessages_unk::fn_800698DC` (q1) en `atn/LH_Core` y `atn/FileSystem_unk` (q4).
- `atn::LoadHandle::fn_80473F18` (q4) en `game/BattleEfp_unk` (q7) y en `CharaTask`, `gmk/MapLift`, `LibraryTask` (sin enlazar).
- `atn::LoadHandle::fn_80473FCC` (q4) en `db/HairBackControl_unk` (q6).
- `fn_8004B20C` y `GameSaveSlot_unk::fn_803BE8C0` (q7) en `MP/SaveLoad` (q9).
- `std::ios_base::Init` y el init de streams anchos de MSL: q3 y q8 los nombraron distinto con dos cabeceras; se deja la de q8 (`include/std/ios_init.h`, `wios_base_Init_unk`) y se borra `std/ios_init_unk.h`.
- Conflictos de `game_units.json` resueltos por union de entradas; de `symbols.txt` solo nombres de MSL y un tamano de float.

## Decisiones a revisar

**Excepciones a la regla 2 aprobadas por el usuario (2026-10-20) y aplicadas tras el lote** (ramas `agent-pz`, `agent-pv`, `agent-ps`; build `main.dol: OK`, `regress.py` lost: 0, ninguna funcion baja de fuzzy; 5356 -> 5375 funciones al 100%):
1. `#pragma explicit_zero_data on` (con `push`/`pop`, solo alrededor de las definiciones) para los cinco `0.0f` de `.sdata` 0x8087D828-0x8087D850 de `atn/PostFxColorMatrix` (3/4). En `atn/LipSync_unk` y `atn/PostFxRain` no hizo falta: esos ceros de `.sdata` eran pools `@STRING@` de los `""` de un `new (heap, "", "", 0) T[n]` dentro de una inline (renombrados en `symbols.txt` con tamano 0x1); LipSync 2/4, PostFxRain 2/8 -> 7/8.
2. Asm paired-single de `include/math/Vec.h` ampliado con min/max por componentes (`ps_sel`/`fsel`): `__VecMin`/`__VecMax` (f0-f7, ejemplo 0x80070C98) y `__VecMinMax` (f0-f9, ejemplo 0x8006FEBC). Patron en 10 funciones de 6 ficheros (atn_colli, Model, ModelInstance, LayerOcclusionCPU, LayerShadow, MapPartsLocatorSet). `atn/Geometry_unk` 4/10 -> 7/10; el inline de proyeccion de 8 esquinas queda como `ProjectBox_unk` local en Geometry_unk.cpp (moverlo a cabecera al decompilar las demas).
3. `boost::shared_ptr` real de Boost 1.35.0 (`boostorg/smart_ptr` tag `boost-1.35.0`, con `detail/sp_counted_base_cw_ppc.hpp`: atomicos `lwarx/stwcx.`) en `libs/boost/include`, sustituye a `include/Event/SharedPtr_unk.h`; unico cambio, el include relativo de `sp_typeinfo.hpp` (anotado en `libs/boost/README.md`). `Channel::~Channel` 49% -> 98,9%; `Event/Cut` decompilada (10/15). Lo que queda (~40%) es el `push_back` inline de `std::vector`.

**Splits pendientes (fuera de las unidades del lote, no tocados):**
- `atn/FileSystem_unk`: el tramo 0x80470364-0x80470580 (`ResRef_unk`, `fn_80470564`) parece otro fichero entre FileSystem_unk y LH_Binary (llama fuera de linea a lo que aqui se inlinea; `"%sh"` en objeto `.rodata` aparte). Unidad nueva propuesta.
- `atn/CompositeCache_unk`: su `.text` se recorto a 0x800D87A4; 0x800D87A4-0x800D8894 queda `auto_` (probable inicio de `atn/CompositeIntensityTintMulti`).
- `db/SpText_unk`: su dtor debil 0x8021CC50-0x8021CCC4; `db/MapLink_unk`: dos dtors debiles 0x80216A2C-0x80216AFC (escritos de momento en StrategyAiTable/MapIdTable como clases no plantilla).
- `atn/EffectManager_cont`: no fusionada; `std/char_traits_char_8023C080` esta en medio y es del mismo fichero real, y `atn/EffectManager` tiene `matching: false`. Propuesta: fusionar las tres. Su ctor de `EfpData_unk` paso a EffectSequencer_unk.
- `Event/Manager_cont`: su pool y dos constantes de su `__sinit` son de `Event/Manager` (sin fuente). Propuesta: `Event/Manager` + `PostSequence` + `Manager_cont` en un fichero al decompilar `Event::Manager`.
- `atn/LH_Motion_cont`: no fusionada porque su primera funcion es `LoadManager_unk::Get_unk<LoadMotion>` (~96% en los doce `LH_*`); fusionar bajaria `LH_Motion` del 100%.
- `SequenceControl::fn_800C37FC` (primera del split de SequenceHolder) se definio en `SequenceHolder.cpp`; puede ser de `SequenceControl.cpp` (enlazada).

**Splits cambiados dentro del lote** (todos sobre huecos `auto_` o de la propia unidad): `.sbss` de singletons en Pad/LightUtil/FileUtil (q1) y AiUtil (q7), `.bss` de MenuCommon desde 0x807C7898 (q7), `.data` de EffectLoader hasta 0x80783BB0 y recorte de EffectManager_cont a 0x8024372C-0x8024373C (q3), `.sdata2` 0x80880890-98 a `atn/Bgm` y `.rodata` de MarketTask desde 0x807570C0 (q9), y las 8 fusiones de `_cont`.

**`symbols.txt` mas alla de renombrar:**
- Simbolos nuevos/partidos: `@237` en MenuCommon (registro de dtor de 0xC, sacado de `lbl_807C78C8`); `lbl_80793E58` partido en 4 tablas de 0x60 (Event/Section); `lbl_80732930` partido en dos tablas de 0x28 (q1).
- Tamanos: varios floats de 8 -> 4 bytes (float + relleno que objdiff leia como double) en q1, q2, q3, q6, q7, q9; pool de LoadManager_unk 0x8 -> 0x5 (q3).
- Renombrados que cambian el mangling (firmas): `LoadCore::vf10(char*)`, `func_enter_setting::vf1C(const char*, const void*)` (q4), `CharaTask::vf68`/`vf6C` (q7), `CharaTask::fn_801546F4(int, Vector3, float)` (q8). Tambien se nombraron funciones de unidades ajenas en asm (MSL iostream, Event/Node y Cut, `SpTextEntry_unk` dtor, ctor `basic_string(const char*)`, `c_str`, `token_iterator`, `Model_unk::fn_80097A20`).

**Cabeceras compartidas cambiadas** (todo con `regress.py` sin perdidas):
- `std/string.h`: `operator=` copia con `mRaw = str.mRaw` (q3). Sube `tokenizer::assign` de CharaParam (q5: 95,9% -> 99,7%) y baja 0,2 y 0,06 puntos el ctor y `vf18` de `atn/D2A_Message`. Revisar si se mantiene.
- `MP/FuncMessage.h`: `vf18` devuelve `unsigned int` y `vf1C(const char*, const void*)` en las 65 clases (q4).
- `atn/LoadCore.h` (`mState`/`m05` volatile), `atn/LoadManager.h` (vtable, `LoadCache_unk`, listas pfs, `m3ED8` int, `mPending`), `atn/LoadHandle.h`, `atn/PacketPack.h`, `atn/ResLoader_unk.h`, `atn/Csv.h` (ctor `CsvU()`, `vf08` con ternario), `atn/Texture_unk.h`, `atn/Model_unk.h`, `atn/SoundRef.h` (`SoundObject` 0xA8 -> 0xAC), `std/list.h`, `std/vector.h`, `game/CharaTask.h`, `CharaStatus.h`, `NpcManager.h` (array `mNpcs[6]`), `MP/SaveLoad.h`, `GameSaveLoadBackgroundCommon.h`, `gmk/Lemon.h`. `PacketPack::vf0C` pasa a definirse inline en `MP/CryptMP.cpp` (CryptMP sube 8/11 -> 9/11).

**Codigo que no esta en el DOL (solo para ordenar pools):** `fn_Stripped_unk` en Event/Label, Section, Channel y EventLoader (q8); `GetType_unk`/`GetNumber_unk` en `db/AiTable_unk` (q5); `Reader_unk::Get_unk` en LoadManager_unk, que queda como funcion de mas en nuestro `.o` (q3).

**Nombres reales propuestos (no aplicados):** `atn/THPDraw_unk` = `THPDraw.c` (SDK THP); `atn/Bezier_unk` = `animEngine.c` (devkit de Maya); `atn/Math_unk` = `atn_math.cpp`; `db/HairBackControl_unk` = tabla de nombres de motion; `db/ParseUtil_unk` = parametros de bala (`db/BulletParam`); `game/TargetPos_unk` parte de AiTarget; `game/HomeButton_unk` parte de `HomeButtonMenu`; helpers iniciales de `GameSystem_unk` de `game/ReplayBuffer`.

**Pendientes en otros ficheros:** `src/auto_03_803B27AC_text.cpp` (GameLoadTask, sin configuracion) declara con nombre C funciones que ya tienen nombre C++; `db/ChapterList_unk` llama a 48 cargadores `/boot` como `extern "C" fn_...` (actualizar si se les da clase); `lbl_8087F0A8` declarado con dos tipos distintos (ChaseParam / EnvSeBase).

## Resultado por unidad

Funciones al 100% / total del `report.json` final; en los `_cont` fusionados la cifra es la de la unidad enlazada completa.

**q1**

| Unidad | 100% / total | matching |
|---|---|---|
| `atn/Math_unk` | 1/1 | sí |
| `atn/LipSync_unk` | 2/4 | no |
| `atn/Pad_unk` | 15/17 | no |
| `atn/LightUtil_unk` | 4/7 | no |
| `atn/Light_unk` | 7/10 | no |
| `atn/PostFxBloom` | 1/3 | no |
| `atn/PostFxColorMatrix` | 2/4 | no |
| `atn/FileUtil_unk` | 9/16 | no |
| `atn/Geometry_unk` | 4/10 | no |
| `atn/BoneCalc_unk` | 1/3 | no |

**q2**

| Unidad | 100% / total | matching |
|---|---|---|
| `atn/Texture_unk` | 8/9 | no |
| `atn/Se_unk` | 11/11 | sí |
| `atn/CompositeCache_unk` | 5/7 | no |
| `atn/PostFxDof` | 2/3 | no |
| `atn/SequenceHolder` | 7/11 | no |
| `atn/PostFxMotionBlur` | 2/3 | no |
| `atn/TextureDraw_unk` | 3/4 | no |
| `atn/PostFxDamage` | 1/3 | no |
| `atn/PostFxRain` | 2/8 | no |

**q3**

| Unidad | 100% / total | matching |
|---|---|---|
| `atn/EffectManager_cont` | 2/2 | sí |
| `atn/D2A_ParamTable_unk` | 6/6 | sí |
| `atn/EfpHandle_unk` | 14/15 | no |
| `atn/NumberFormat_unk` | 3/4 | no |
| `atn/Util_unk` | 6/12 | no |
| `atn/D2A_MessageTable_unk` | 5/6 | no |
| `atn/EffectSequencer_unk` | 13/14 | no |
| `atn/EffectRes_unk` | 7/14 | no |
| `atn/EffectLoader_unk` | 4/5 | no |
| `atn/LoadManager_unk` | 3/8 | no |

**q4**

| Unidad | 100% / total | matching |
|---|---|---|
| `atn/LH_Core` | 7/7 | sí |
| `atn/LH_Handle` | 8/9 | no |
| `atn/DwcAllocator_unk` | 6/6 | sí |
| `atn/THPDraw_unk` | 3/3 | sí |
| `atn/PPDATAU` | 14/18 | no |
| `atn/FileSystem_unk` | 11/22 | no |
| `atn/XsfParser_unk` | 9/24 | no |
| `atn/Bezier_unk` | 3/7 | no |
| `atn/NetFriend_unk` | 19/30 | no |

**q5**

| Unidad | 100% / total | matching |
|---|---|---|
| `db/BgmIdTable_unk` | 3/3 | sí |
| `db/ColliAttrTable_unk` | 4/4 | sí |
| `db/HateParam_unk` | 3/3 | sí |
| `db/AiTable_unk` | 2/2 | sí |
| `db/ArrowSet_unk` | 3/4 | no |
| `db/CharaMotionParam_unk` | 3/3 | sí |
| `db/EventIdTable_unk` | 5/7 | no |
| `db/ChaseParam_unk` | 0/1 | no |
| `db/ChapterList_unk` | 9/9 | sí |
| `db/InitSetting_unk` | 2/2 | sí |
| `db/CharaIdTable_unk` | 14/16 | no |
| `db/PaletteTable_unk` | 12/14 | no |
| `db/CharaParam_unk` | 9/11 | no |
| `db/EquipParam_unk` | 13/14 | no |
| `db/ItemRare_unk` | 6/8 | no |

**q6**

| Unidad | 100% / total | matching |
|---|---|---|
| `db/TreasureList_unk` | 2/2 | sí |
| `db/MarketPrice_unk` | 4/4 | sí |
| `db/StrategyAiTable_unk` | 3/3 | sí |
| `db/PlayerPriority_unk` | 4/4 | sí |
| `db/LevelEquipTable_unk` | 4/4 | sí |
| `db/VoiceLengthTable_unk` | 2/2 | sí |
| `db/TutorialTable_unk` | 5/6 | no |
| `db/MemberList_unk` | 2/3 | no |
| `db/ReverbPresetParam_unk` | 3/3 | sí |
| `db/SkillGrowth_unk` | 6/6 | sí |
| `db/SkillParam_unk` | 6/7 | no |
| `db/HairBackControl_unk` | 8/8 | sí |
| `db/PlayerParam_unk` | 4/7 | no |
| `db/TreasureBoxTable_unk` | 6/7 | no |
| `db/ParseUtil_unk` | 11/15 | no |
| `db/MapEnvSeTable_unk` | 10/11 | no |
| `db/MapIdTable_unk` | 5/7 | no |
| `db/RandomItemTable_unk` | 16/17 | no |

**q7**

| Unidad | 100% / total | matching |
|---|---|---|
| `game/FriendManager_cont` (fusionada en game/FriendManager) | 3/3 | sí |
| `game/BattleCalc_unk` | 1/1 | sí |
| `game/TargetPos_unk` | 2/2 | sí |
| `Event/BehaviorGesture` | 2/2 | sí |
| `AI/AiUtil_unk` | 4/4 | sí |
| `game/HomeButtonMenu` | 4/5 | no |
| `Event/BehaviorMoveTo` | 1/2 | no |
| `game/EnemyTask` | 8/8 | sí |
| `game/GameSaveTask` | 7/7 | sí |
| `game/MenuCommon_unk` | 7/10 | no |
| `chr/ControlMoveCursor` | 3/6 | no |
| `game/U16Table_unk` | 2/5 | no |
| `game/GameSaveLoadBackgroundCommon` | 13/16 | no |
| `game/BattleEfp_unk` | 17/17 | sí |
| `gmk/Cannon` | 13/13 | sí |

**q8**

| Unidad | 100% / total | matching |
|---|---|---|
| `game/MapPartsOcean_cont` (fusionada en game/MapPartsOcean) | 7/7 | sí |
| `MP/NetGlobals_unk` | 1/1 | sí |
| `Event/Label` | 1/2 | no |
| `game/HeapInfo_unk` | 4/4 | sí |
| `MP/CharaListeners_unk` | 3/3 | sí |
| `game/MapPartsFactory` | 2/2 | sí |
| `game/PlayerTask` | 6/6 | sí |
| `Event/EventLoader_unk` | 3/5 | no |
| `game/MapInstance_unk` | 3/5 | no |
| `game/PlayerManager` | 6/8 | no |
| `Event/Manager_cont` (sin fuente) | 0/3 | no |
| `Event/Section` | 5/6 | no |
| `game/HomeButton_unk` | 6/8 | no |
| `Event/Channel` | 8/11 | no |
| `game/GameSystem_unk` | 10/12 | no |
| `game/PathData_unk` | 1/4 | no |

**q9**

| Unidad | 100% / total | matching |
|---|---|---|
| `game/SetConfig_cont` (fusionada en game/SetConfig) | 8/8 | sí |
| `game/GameTimer_cont` (fusionada en game/GameTimer) | 12/12 | sí |
| `MP/SaveLoad_cont` (fusionada en MP/SaveLoad) | 8/8 | sí |
| `atn/LH_Motion_cont` | 3/4 | no |
| `game/NpcManager_cont` (fusionada en game/NpcManager) | 7/7 | sí |
| `atn/Bgm_cont` (fusionada en atn/Bgm) | 8/8 | sí |
| `game/MarketTask_cont` (fusionada en game/MarketTask) | 13/13 | sí |

