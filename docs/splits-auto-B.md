# splits-auto, agente B (rango .text 0x800F7250 - 0x8033C3D8)

Herramientas propias usadas (scratchpad, no en el repo): vista por funcion de constantes duplicadas
(`cc.py`), propietario de cada objeto de datos = unidad que contiene todas las funciones que lo usan
(directa o via vtable/tablas) (`gen.py`), y aplicacion de bloques a `splits.txt`.

## Grupo 1 (prioridad 1 + 0x800F7250)

| Tramo | Unidad | Prueba |
|---|---|---|
| 0x800F7250-0x8010D98C | `game/BattleTask.cpp` ampliada (antes acababa en 0x800F7250) | Las 191 funciones usan las mismas copias de `.sdata2` que BattleTask (0.0f `lbl_80881478`, 1.0f `lbl_80881494`, 1.5707964, 1e16...): ninguna constante duplicada cambia de copia entre 0x800F1430 y 0x8010D98C; la `.sdata2` propia sigue sin hueco hasta 0x808815F8 (BehaviorFollow). `.rodata` 0x807355C8-0x80736040 (tablas de SE, double int->float `lbl_80735EB0` usada por ambas partes). |
| 0x80116EB8-0x8011770C | `game/MenuCommon_unk.cpp` (nuevo, nombre inventado) + `.ctors` 0x8072D2D0 | `__sinit` 0x801175EC al final; 1.0f propia (`lbl_80881720`) distinta de la de Event/Path; funciones de sonido de menu (`fn_80117228` = sonido de sistema SE_UICOM_*), tabla de stats (atk_/def_/agi_...). |
| 0x8011BD14-0x8011D24C | `game/CharaC58_unk.cpp` (nuevo, nombre inventado por el struct `CharaC58_unk`) | 0.0f/1.0f/3.0f propios (0x80881798...) distintos de CampSaveLoad; termina donde -1.0f y 1.0f cambian de copia (0x808817BC->0x808817C0, 0x8088179C->0x808817C4). Contiene ctor/metodos de `CharaC58_unk` y `CharaD74_unk`. |
| 0x8011D24C-0x8011F6B0 | `game/CharaFaceControl.cpp` (nuevo) | Cambio de copia de constantes en 0x8011D24C; cadena RTTI de boost::function `bind_t<..., CharaFaceControl, atn::MotionBuffer*, int>` (0x80736EF0) usada aqui. Clase principal sin vtable: `Chara1188_unk` en el repo = probablemente `CharaFaceControl`. |
| 0x80204958-0x80207C00 | `atn/CsvBase_char_124_13_34.cpp` ampliada hacia atras (+23 funciones) | Ya decia el plan: el fichero real empieza en 0x80204958 (tras Event/MotionData). Mismo `.rodata` contiguo hasta la vtable/RTTI de CsvPipe y CsvBase<char,'|'>; pool `""` 0x8073EA48 al final. Cargadores de CSV de /boot (ai_table, weapon_param, weapon_effect, arrow_set, battle_eff_se_table). |
| 0x80263CF0-0x80264DB4 | `game/Em201ZanGurg.cpp` ampliada | Mismas copias (0.1, 1.0, 0.0 en 0x808835F8...) que Em201ZanGurg; la `.sdata2` 0x80883638-0x80883670 solo la usan estas funciones. Corte con ZanGurgFinal en 0x80264DB4: todas las constantes cambian de copia. |
| 0x80267F88-0x8026FC0C | `game/Em201ZanGurgFinal.cpp` ampliada | Mismas copias (0x80883670..) que ZanGurgFinal; funciones finales 0x8026FBB8/0x8026FBD4 en la vtable de Em201ZanGurgFinal. Datos: tabla de cadenas op_* (0x807444D0-), `lbl_80744608`, jumptables 0x80784D28-0x80784DD8. |
| 0x8031B09C (`__sinit`, 0xC) | `game/Em316LaserSphere.cpp` ampliada + `.ctors` 0x8072D350 | `__sinit` usa `lbl_80884DE8` (sdata2 de LaserSphere): ultima funcion de LaserSphere. |
| 0x8031CA30-0x8031EB3C | `game/Em316LastLaser.cpp` ampliada (incluye `LaserTrace`) | Mismas copias (0.0f 0x80884DF0, 1.0f 0x80884E04...) que LastLaser; WhiteLeoPard empieza con copias nuevas (0x80884E48...). |

Datos: cada unidad recibe los objetos `auto_` que solo usan sus funciones, pegados a su rango.

### Decisiones sin el usuario
- Nombres inventados: `game/MenuCommon_unk.cpp`, `game/CharaC58_unk.cpp`. `game/CharaFaceControl.cpp` sale de la cadena RTTI de boost (`CharaFaceControl`), sin vtable propia.
- `game/BattleTask.cpp` (sin fuente) crece de 0x5E20 a 0x1D55C bytes: no hay ninguna prueba de corte (constantes compartidas en todo el tramo, ninguna `__sinit`). Las vtables de `gmk::Cannon`, `chr::ControlCdnRestorePsycho` y `Bullet` solo tienen funciones debiles/heredadas en el tramo (vf58/vf44 de GimmickCommon, vf2C de ControlBase, `fn_80101330`), no son ficheros de esas clases. Revisar si se quiere partir por otra via.
- `symbols.txt`: `lbl_8073EA48` -> `@stringBase0` (pool `""` de CsvBase_char_124_13_34).

## Hallazgos
- dtk acepta los bloques `.ctors` de 4 bytes de las `__sinit` aisladas junto a la unidad anterior sin `align:`.

## Grupo 2 (chr::Control*)

| Tramo | Unidad | Prueba |
|---|---|---|
| 0x8018EEC4-0x80192908 | `chr/ControlCdnStone.cpp` (nuevo) | Constantes nuevas (0.0f 0x80881F70, 1.0f 0x80881F7C...) frente a ControlAttackBase; todas las clases `chr::ControlCdn*` (Stone, RestoreStone, Paralyzed, RestoreParalyzed, Betray, Sleep, RestoreSleep, Piyo, RestorePiyo, Psycho, RestorePsycho) con sus RTTI contiguos en `.rodata` 0x807395B4-0x807396D4 en orden inverso (Stone ultima = primera del codigo). Incluye al final los debiles `vf14/vf18/vf10__Q23chr11ControlBaseFv` (0x80192578-0x80192908, con su propia copia 0.0f/1.0f 0x80881FB0) y la vtable de `chr::ControlBase` (0x8077DC90): primer fichero que los usa. ControlStickByQuark empieza en 0x80192908 (nuevas copias 0.5f/1.0f/0.0f 0x80881FB8...). |
| 0x80192908-0x801963F4 | `chr/ControlStickByQuark.cpp` ampliada hacia atras | Mismas copias de constantes que el resto de ControlStickByQuark (0x80881FBC=1.0, 0x80881FCC=0.0...) hasta 0x801A0BA8; clases StickByQuark, Thrust, ThrustForCrab/Kraken/QuarkStand/Ghoul. |
| 0x801AAAE4-0x801AAB64 | `chr/ControlChaseCaught.cpp` ampliada | Dos destructores debiles en las vtables de ControlChaseAssist/ControlChaseFollow (clases de ControlChaseCaught). |
| 0x801C041C-0x801C3414 | `chr/ControlMountle.cpp` (nuevo) | Constantes nuevas frente a ControlChant (0x80882698...); clases Mountle, MountleQuadruped, GuardMountle, WallMountle, Jump. |
| 0x801C3414-0x801C3DCC | `chr/ControlMoveCursor.cpp` (nuevo) | 0.0f/1.0f cambian de copia en 0x801C3414 (0x80882748) y otra vez en ControlOneMotion (0x801C3DCC); vtable `chr::ControlMoveCursor` 0x80781B90 justo antes de la de ControlOneMotion. |

### Decisiones sin el usuario
- `chr::ControlBase` (vtable y vf10/14/18 debiles) queda dentro de `chr/ControlCdnStone.cpp` (no es un fichero aparte: solo funciones debiles con su copia de constantes; a revisar).
- `.bss` 0x807C7C78 (usado solo por ControlMountle) se queda en `auto_`: asignarlo creaba un ciclo de orden de enlace (ControlMountle -> ControlMoveCursor -> ControlOneMotion -> ControlPushedDown -> auto bss), porque el `.bss` de ControlPushedDown esta antes. Igual con `.bss` 0x807C7C30/0x807C7C38 de ControlChaseCaught (detras del bss de ControlPushedDown).
- `.sbss` de ControlCdnStone extendido hasta 0x8087F0C0 para incluir el guard compartido `@GUARD@assign_to<...mf0<v,9CharaTask>...>` (0x8087F0BE, 1 byte): dejarlo solo en un trozo `auto_` de 2 bytes da `undefined` al enlazar.

## Hallazgos (grupo 2)
- Si un objeto solo usado por la unidad X queda detras de datos de otra unidad Y y X va antes que Y en .text, dtk falla por ciclo de orden de enlace: hay que dejar ese objeto en `auto_`.
- Un guard `@GUARD@` (1 byte) compartido no puede quedarse solo en un hueco `auto_` minimo entre dos unidades: el enlace da `undefined`; meterlo en la unidad vecina.

## Grupo 3 (CosEdit y cargadores de tablas `db/`)

Metodo nuevo (herramienta `part.py` en el scratchpad): pruebas duras de corte = (1) mismo valor (constante float/double o
cadena, p. ej. "basic_string: out_of_range") en dos direcciones distintas usado por dos funciones; (2) el pool de cadenas
de un fichero (objeto `.rodata` con varias cadenas, o `""`) es el ultimo objeto de su `.rodata`: un objeto privado detras
de el es de otro fichero. El corte se pone en la posicion donde los datos privados quedan ordenados en todas las secciones
y que menos llamadas cruza. Validado sobre unidades ya partidas (chr/Control*, Em002-Em006, Em202-Em204, gmk 0x8040A000-0x80440000,
0x80338000-0x80370000, 0x804F0000-0x80530000, 0x80125320-0x80184A64): 0 cortes falsos (solo omite cortes sin pruebas).

| Tramo | Unidades | Prueba |
|---|---|---|
| 0x801CFC58-0x801CFD68 | `game/CosEditBase.cpp` ampliada (+`__sinit` 0x801CFC58, `.ctors` 0x8072D2E4) | `__sinit` usa la `.sdata2` de CosEditBase; 0x801CFD68 (creador `new CosEditProtector`) ya usa el pool de CosEditProtector. |
| 0x801CFD68-0x801CFDCC, 0x801D5420-0x801D54C0 | `game/CosEditProtector.cpp` ampliada (`.ctors` 0x8072D2E8) | creador + `__sinit` 0x801D54A0 con `.bss` de CosEditProtector; cadenas compartidas con el pool 0x8073C56C. |
| 0x801D54C0-0x801D80A0 | `game/CosEditPalette_unk.cpp` (nuevo, `.ctors` 0x8072D2EC) | 1.0f/3.0f/0.0f nuevos (0x80882AD0...), cadenas "base_pos"/"reference" duplicadas con CosEditProtector; `__sinit` 0x801D8080; lo usa CosEditProtector (paleta de colores: "slt_palette%d%d", "paletteColor"). Nombre inventado. |
| 0x801D80A0-0x801E7FF8 | `game/CosEditItemList_unk.cpp` (nuevo, `.ctors` 0x8072D2F0) | double int->float 0x8073CAA0 y 1.0f 0x80882AF4 nuevos; cadenas "uv0001"/"mask_uv0001"/"basePos" duplicadas; `__sinit` 0x801E7FD8. Listas de protectores/armas ("protector01", "mainIconUv"). Nombre inventado. Tramo de 0xFF38 sin mas pruebas internas: queda entero. |
| 0x80204958-0x8021F47C | 39 unidades `db/*_unk.cpp` (nombres inventados por el CSV que cargan) y `atn/CsvBase_char_124_13_34.cpp` reducida a 0x802078C4-0x80207CC0 | Cada cargador de `/boot/*.csv` es un fichero: copias distintas de las cadenas de `std::basic_string` ("basic_string::reserve length_error"...) y de 0.0f/1.0f, y pools de cadenas separados por objetos no-cadena. `atn/CsvBase_char_124_13_34` = cargador de battle_eff_se_table (CsvPipe, vtable/RTTI de CsvBase<char,'|'>, pool `""`) + los 3 accesores 0x80207C00-0x80207CC0 (usan las variables de esa tabla). |

Unidades `db/` (texto): AiTable 0x80204958, WeaponParam 0x80204E54, ArrowSet 0x80207310, BgmIdTable 0x80207CC0, ChapterList 0x80207F80,
CharaIdTable 0x80208748, CharaMotionParam 0x80209300, CharaParam 0x802098E0, ChaseParam 0x8020A90C, ColliAttrTable 0x8020AFF4,
PaletteTable 0x8020B2BC, OptionParts 0x8020C4C8, EquipParam 0x8020DF88, EventIdTable 0x8020F130, HateParam 0x8020FDEC,
InitSetting 0x80210244, ItemParam 0x80210ACC, ItemRare 0x80212F68, LevelEquipTable 0x8021414C, MapEnvSeTable 0x802144A8,
MapLink 0x802150F0 (+`__sinit`, `.ctors` 0x8072D304), MapIdTable 0x80216A2C, MarketPrice 0x80217778, MemberList 0x802179FC,
HairBackControl 0x80217E18, PlayerPriority 0x802188B4, PlayerParam 0x80218BD4, ReverbPresetParam 0x80219544, SkillParam 0x8021996C,
ParseUtil 0x80219F84 (sin CSV propio: funciones de parseo/consulta, `fn_80219E6C` "bullet data by id" queda en SkillParam),
SkillGrowth 0x8021AAB8, SpText 0x8021B060 (+`__sinit`, `.ctors` 0x8072D308), StrategyAiTable 0x8021CC50, RandomItemTable 0x8021CF4C,
TreasureBoxTable 0x8021E0B0, TreasureList 0x8021EB04, TutorialTable 0x8021ED08, VoiceLengthTable 0x8021F100.

### Decisiones sin el usuario
- Todos los nombres `db/*_unk.cpp`, `game/CosEditPalette_unk.cpp`, `game/CosEditItemList_unk.cpp` son inventados (por el CSV/uso).
- `atn/CsvBase_char_124_13_34.cpp` (con fuente, no enlazada): en el grupo 1 la amplie hasta 0x80204958 siguiendo el plan; con las pruebas nuevas el tramo son 6 ficheros y la unidad queda en 0x802078C4-0x80207CC0. Contradice la nota del plan ("23 funciones mas"): a revisar. Los 3 accesores 0x80207C00-0x80207CC0 podrian ser del fichero siguiente (BgmIdTable); los dejo con la tabla cuyas variables usan.
- Posiciones de corte ambiguas (varias posiciones validas, elegida la que menos llamadas cruza): CharaParam/ChaseParam (0x802098E0 vs 0x80209960), EventIdTable (0x8020F0AC..0x8020F248), LevelEquipTable (0x80213B78..0x8021414C), HairBackControl, SkillParam/ParseUtil/SkillGrowth (0x80219EC4.., 0x8021A4CC..0x8021AAB8), RandomItemTable (0x8021CF4C vs 0x8021CF8C).

## Grupo 4 (colas de enemigos `game/Em*`)

Tramos `auto_` justo detras de una unidad de enemigo sin fuente: en todos, `part.py` no encuentra ninguna prueba de corte
y la cola comparte constantes privadas de `.sdata2`/`.rodata` (misma direccion) con la unidad anterior, asi que es el mismo fichero.
Se amplian (texto y datos `auto_` propios):

| Unidad | Nuevo `.text` | Prueba (objetos privados compartidos cabeza/cola) |
|---|---|---|
| `game/Em003LesserDaemon.cpp` | 0x8024A650-0x80250A24 | 17 (p. ej. sdata2 0x808832AC) |
| `game/Em005DoppelGanger.cpp` | 0x80250A24-0x8025457C (+`__sinit`, `.ctors` 0x8072D324) | 4 |
| `game/Em101ElderZesya.cpp` | 0x802558B0-0x8025A060 (+`__sinit`, `.ctors` 0x8072D328) | 6 |
| `game/Em202_01_BSpider.cpp` | 0x8026FC0C-0x8027278C (+`__sinit`, `.ctors` 0x8072D32C) | 4 |
| `game/Em202Spider.cpp` | 0x8027C990-0x80282AE8 | 10 |
| `game/Em202SpiderBase.cpp` | 0x80282AE8-0x80287E7C (+`__sinit`, `.ctors` 0x8072D330) | 11 |
| `game/Em203GreaterDaemon.cpp` | 0x80287E7C-0x80293738 | 16 |
| `game/Em204Mulue.cpp` | 0x802A4BBC-0x802AC364 | 17 |
| `game/Em401_01_WhiteLeoPard.cpp` | 0x8031EB3C-0x80322C68 | 10 |

## Grupo 5 (0x80220168-0x80244228: Event debug, DialogHead, dlmalloc, sistema de efectos)

| Tramo | Unidad | Prueba |
|---|---|---|
| 0x80220168-0x802251F8 | `Event/DebugInfo_unk.cpp` (nuevo, `.ctors` 0x8072D30C) | Pool propio 0x807428F4 ("Event Info:", "Cut List:", estados "playing"/"paused"...: volcado de depuracion de eventos); copias propias de las cadenas de basic_string; `__sinit` 0x80224A2C + destructores de arrays estaticos 0x8022513C-0x802251DC (registrados por ese `__sinit`). Nombre inventado. |
| 0x802251F8-0x8022526C, 0x8022808C-0x802280B4 | `Event/DialogHead.cpp` ampliada | 0x802251F8 (creador `new DialogHead`) usa el pool 0x80742D7C de DialogHead; 0x8022808C-0x802280B4 son 5 funciones de su vtable. |
| 0x80229DF0-0x8022B850 | `atn/DLMalloc_unk.cpp` (nuevo) | Sin datos propios salvo `.bss` 0x807C80B0; funciones llamadas solo desde `atn::DLAllocator` (malloc de Doug Lea: create_mspace/mspace_malloc/free). 0.0f/1.0f del siguiente fichero cambian de copia. Nombre inventado. |
| 0x8022B850-0x80234A8C | `atn/EffectSystem_unk.cpp` (nuevo, `.ctors` 0x8072D310) | Metodos `EffectSystem_unk` (nombre del repo); constantes propias (0x808830D0...); `__sinit` 0x80234A60. |
| 0x80234A8C-0x802371BC | `atn/EffectDesc_unk.cpp` (nuevo, `.ctors` 0x8072D314) | Pool "Effect"/"FadeNear"; 0.0f/1.0f nuevos; `__sinit` 0x802371AC. Nombre inventado. |
| 0x802371BC-0x8023793C | `atn/EfpHandle_unk.cpp` (nuevo, `.ctors` 0x8072D318) | ctor `atn::EfpHandle_unk`, dtor `atn::LH_Effect`; `__sinit` 0x802378E0. |
| 0x8023793C-0x80238808, 0x80238808-0x80239798 | `atn/EffectRes_unk.cpp`, `atn/EffectLoader_unk.cpp` (nuevos) | 0.0f/1.0f cambian de copia en 0x80238CA0 (posicion elegida 0x80238808 entre 0x802386FC y 0x80238CA0); pool "old eff version!" del cargador. Nombres inventados. |
| 0x8023B9D0-0x8024372C | `std/char_traits_char_8023C080.cpp` ampliada (`.ctors` 0x8072D31C) | Plantillas `std::num_put`/`numpunct` y `__sinit` 0x80243700 (guards de ids de facetas). |
| 0x8024372C-0x80243758 | `atn/EffectManager_cont.cpp` (nuevo) | Thunks de la vtable de `atn::EffectManager` (`fn_8024372C`, `fn_80243734`). |
| 0x80243758-0x80244228 | `atn/EffectSequencer_unk.cpp` (nuevo, `.ctors` 0x8072D320) | Pool propio 0x80743140 ("EffectSequencer", "EffectLine", ...) pegado al de EffectManager (dos pools = dos ficheros); cadena "Effect" duplicada. |

### Decisiones sin el usuario
- **EffectManager**: las pruebas indican que el fichero real `EffectManager.cpp` va de 0x80239798 a 0x80243758 (incluye las plantillas std de 0x8023B9D0-0x8024372C y los thunks): su pool de cadenas 0x8074308C ("EfpInfoList", ...) esta detras de los nombres RTTI de num_put/numpunct, su `.sdata` 0x8087DC18 va detras del `.sdata` "inf"/"nan" de la parte std (dtk da ciclo de orden si se separan), y la vtable apunta a los thunks de 0x8024372C (el propio `src/atn/EffectManager.cpp` lo dice). No he fusionado porque `std/char_traits_char_8023C080.cpp` es una unidad existente de `game_units.json` (no se puede borrar sin tocarlo): la amplio a todo el tramo std y dejo los thunks como `atn/EffectManager_cont.cpp`. A revisar: fusionar las tres unidades en EffectManager. Por la misma razon el `.sdata` 0x8087DC10 ("inf"/"nan") y el pool 0x8074308C quedan en `auto_`.
- `.sbss` de la unidad std ampliado a 0x8087F3D0 para incluir los guards compartidos 0x8087F3CC/0x8087F3CD (si no, `undefined` al enlazar).

## Grupo 6 (0x8010E3F0-0x80125320)

| Tramo | Unidad | Prueba |
|---|---|---|
| 0x8010E3F0-0x8010E60C | `Event/BehaviorGesture.cpp` (nuevo) | Metodos de la vtable `Event::BehaviorGesture`; 0.0f/1.0f propios (0x80881618); datos: nombre RTTI + `Singleton<BehaviorGesture>` (rodata 0x80736588, data 0x80779E68). |
| 0x8010E60C-0x8010EB58 | `Event/BehaviorMoveTo.cpp` (nuevo) | vtable `Event::BehaviorMoveTo`; 1.0f cambia de copia en 0x8010E60C. |
| (datos) | `Event/BehaviorFollow.cpp`: `.rodata`/`.data` ampliados con el RTTI de `atn::Singleton<Event::BehaviorFollow>` | Patron de los tres ficheros: nombre de la clase y despues el del Singleton. |
| (datos) | `game/Bullet.cpp`: `.rodata` hasta 0x807366B0 | cadena "spine" usada solo por Bullet. |
| 0x80112220-0x80112254 | `Event/Path.cpp` ampliada hacia atras (`.sdata2` desde 0x808816A8, `.rodata` desde 0x807366B0) | `fn_80112220` usa las mismas copias de 1.0f/-1.0f/0.0f que Path (0x808816A8-0x808816B0) y solo la llama Path. |
| 0x8011FF78-0x80121F00 | `game/MotionCommand_unk.cpp` (nuevo, `.ctors` 0x8072D2D4) | `__sinit` 0x80121EB0 al final; empieza tras las plantillas de `CharaManager<T>` (funciones debiles = final del fichero anterior). Metodos de `MotionCommand_unk` (nombre del repo) y tabla "EFF_ATR". Nombre inventado. |
| 0x80121F00-0x80125320 | `game/PlayerInput_unk.cpp` (nuevo, `.ctors` 0x8072D2D8) | `__sinit` 0x80124FFC al final; metodos de `Input_unk` (botones, iconos de botones). Nombre inventado. |

### Decisiones sin el usuario
- `game/MotionCommand_unk.cpp` empieza su `.rodata` en 0x807370B4 (4 bytes de relleno, align 8 esperado): la unidad anterior `game/CharaManager_PlayerTask.cpp` esta enlazada y no se puede ampliar, y dejar el relleno sin unidad rompe objdiff ("gap symbol out of section bounds"). dtk solo avisa (WARN de alineacion).
- `Event/BehaviorFollow.cpp` (con fuente) recibe el RTTI de `Singleton<BehaviorFollow>`: su fuente no lo genera aun.

## Grupo 7 (0x80184A64 y zona D2A 0x801F0364-0x80201E78)

| Tramo | Unidad | Prueba |
|---|---|---|
| 0x80184A64-0x80187D04 | `game/GameConfig_unk.cpp` (nuevo) | 0.0f/0.01f/1.0f/0.5f cambian de copia frente a ColliAttrManager y ControlBreakOpen; tabla de nombres de parametros ("Dialog", "UseWaitMotion", "Cam Pos", "Fog Mode", "LevelMul", "BGM Start", "Title Fade Start", "DebugMenu", "Config"). Nombre inventado. Su `.sbss` 0x8087EED0 queda en `auto_` (esta antes del de ColliAttrManager: ciclo de orden). |
| 0x801F0364-0x801F0AC0 | `atn/D2A_ParamTable_unk.cpp` (nuevo) | La double int->float 0x8073CDC8 que usa esta detras del nombre RTTI de D2A_Frame (en un fichero va antes del RTTI) y su `.sdata2` 0x80882C28 entre las de Frame y Line; metodo `atn::D2A_ParamTable`. Por el orden alfabetico de `atn_*` el nombre real deberia ir entre D2A_Frame y D2A_Line. |
| 0x801F0D80-0x801F2398 | `atn/D2A_Manager.cpp` (nuevo, `.ctors` 0x8072D2F8) | vtable/clase `atn::D2A_Manager`, `__sinit` 0x801F1EA0; pool y tablas "icon_skull.d2a", "/data/d2anime/d2a_text_xx.u16". Absorbe el bloque explicito `auto_fn_801F1EA0_text.cpp` que habia en splits.txt (era solo el `__sinit`). |
| 0x801F3490-0x801F3EF4 | `atn/D2A_MessageTable_unk.cpp` (nuevo) | Pool propio 0x8073E380 con otra copia de las cadenas de basic_string (D2A_Message tiene la suya en 0x8073E314). Lee un CsvU de textos (`fn_801F3E20` = texto por id, lo usan los menus). Nombre inventado. |
| 0x801F4FF4-0x801F51C8, 0x801F8E6C-0x801F94F4 | `atn/D2A_TaskInstanceBase_64_8.cpp` ampliada | 0x801F4FF4 usa el pool 0x8073E508 que esta detras de los nombres RTTI de `D2A_TaskInstanceBase<..>`/`ID2A_TaskInstance` (final de la `.rodata` de ese fichero) y copias de constantes distintas de D2A_Task; la cola comparte datos (0x80782D50). |
| 0x801FDE94-0x801FF1B8 | `atn/D2A_Texture.cpp` ampliada | la cola comparte `.sdata` 0x8087DB00/0x8087DB04 con D2A_Texture. |
| 0x801FFA04-0x80201E78 | `atn/D2Anime_unk.cpp` (nuevo, `.ctors` 0x8072D300) | 0.0f en otra copia (0x80882CD0) que D2A_Window (0x80882CBC); `__sinit` 0x80201E1C; cargador de ficheros d2a llamado por `D2A_Task::vf0C`. Nombre inventado (entre D2A_Window y D3Anime por orden alfabetico, de ahi "D2Anime"). Su `.sdata` queda en `auto_` (orden). |

### Decisiones sin el usuario
- Borrado el bloque `auto_fn_801F1EA0_text.cpp` de splits.txt (lo absorbe `atn/D2A_Manager.cpp`).
- `atn/D2A_MessageTable_unk.cpp`: el string "Memory allocation failure" que usa (0x80782BB0) cae dentro del `.data` de `atn/D2A_Message.cpp` (con fuente): no lo he movido; contradice en parte el corte (a revisar).
