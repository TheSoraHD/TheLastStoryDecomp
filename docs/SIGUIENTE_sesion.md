# Siguiente sesion (tras datos-20261119)

> Mergeada en `main` (b0c6c0b). Sesion `datos-20261119` (hasta 4 agentes, 32 colas) del 2026-11-19 13:50 al 2026-11-20 ~00:15. Notas: `docs/datos-20261119.md`
> y `docs/datos-20261119-q*.md`; para el port: `docs/datos-20261119-port.md` y `claude/port-datos-20261119.md` (proyecto).

Estado: **16222 / 18113**, lost 0, rep100 23, labelcheck 10 lineas, `main.dol: OK`. Codigo 64,05 %, datos **76,72 %**
(1098638 B), **753** unidades completas. DRAFT 0. Funciones al 0 %: 45 (20 sin iostream/auto_03).

## Decisiones pendientes del usuario

- PBoneBasic::vf10 (`docs/datos-20261119-qR2-pbonebasic.patch`, 96,71 -> 98,47): necesita un `Vector3 axis(...)` que nunca se lee (el original tiene sus tres stores muertos); prohibido por la regla 2 sin permiso.
- Dos `goto` antiguos (vf10, vf40) en game/Em003LesserDaemon, que ahora enlaza; y el macro `REGISTER_AI_SCRIPT` de AI_archer_attack01 fn_8001C284 (la inline no cabe en el presupuesto de inlining con 64 llamadas).
- REP+ aceptado: Event::Manager vf10 da 100 en report.json y 99,93 en objdiff (copias debiles mal nombradas desde antes: `Vec2_unk`/`Vector2_unk`, `FixedArray_unk`/`Ring_unk`).
- Parches con arreglos de logica para el port, sin aplicar porque solo suben fuzzy: `docs/datos-20261119-qC4-vf40.patch` (Em201ZanGurgFinal::vf40) y `docs/datos-20261119-qG2b.patch` (ControlThrust::vf0C).

## Siguiente (decomp, datos)

- A una funcion de enlazar (todas probadas a fondo; buscar ideas nuevas): GameConfig_unk (AddLight_unk, push_back), ControlChaseCaught (Follow vf0C 99,97; 360 ordenaciones probadas), ShaderManager_unk (push_back), RenderSetting_unk (ctor 97,1), Em204MulueMp, PassBoard, Em309GhoulSimple, dwc_transport, SoundManager (99,82 con el parche qA), ControlDive (99,89 con el parche qB-dive), Event/Section (push_back de shared_ptr), ActionActorBehavior (Sequence fn_8054FBE0 99,77; 98 palabras a cero en `.sdata`), Em209SmallGolem, ControlCannonLift, NewCampPartsTop, Em101LittleZesyaSecond, EnemySeaLizerd, Em303KrakenFootMp, TextureDraw_unk, atn/Util_unk (`StrToInt_unk<wchar_t>` 92,7), D3Anime vf14 (99,80), AutomaticDoor (99,90 con parche), PostFxRain (99,72).
- El bloqueo que mas unidades frena es el cruce tamano/capacidad del `push_back` inline (qK; ~73 sitios). Una decision sobre `std/vector.h` abriria D2A_Manager, ShaderManager, GameConfig_unk, Section, Cut, FriendRoom, varios enemigos.
- Unidades grandes: GameEventScript 55 que faltan, ControlStickByQuark 8, GameUI 19, CharaTask 56, AI_archer_attack01 14 (+ pool de cadenas y `.sdata2` en el fuente), BattleTask 35, GameTask 16, PacketPack 54 (sus 20 `""` a cero de `.sdata` son `@STRING@` de inlines: no salen con el pragma).
- Trucos que mas rindieron en esta sesion (ver `docs/trucos-mwcc.md`, seccion datos-20261119): accesores y bloques en `static inline` del propio .cpp, orden de declaracion de locales (y dentro de las inlines), tablas `const`, comparaciones sueltas por una inline `bool`, arrays estaticos separados en vez de uno multidimensional, funcion devuelta por valor en vez de modificar en el sitio.

## Siguiente (port de PC; sigue de secuencial-20261119)

- Regla 10, `extern "C"` con el objeto como primer argumento: `atn::AudioDevice` (35), `atn::Font_unk` (12), `atn::TextAnimation` (6), `atn::EffectManager` (5), `atn::MindowsCommon` (3).
- Punteros guardados en `int`: `CharaTask::mF1C`, `Em303KrakenFoot::m1504`, `TacView::m54.mType`, `atn/DrawHelper` `m1C`, `owner` de `atn::EffectManager`/`EffectSystem`.
- Vistas locales de globales (~70: `MpSession_unk`, copias de `RenderSetting_unk`, tablas de texto de lbl_8087F1E4...) y las 77 firmas de `sigcheck.py`.
- De esta sesion: `CharaTask::m648` es `Arms*` (hoy con cast en CharaTask; unificar `chr::CharaPart_unk` con `Arms`); cambios de firmas y comportamiento en `docs/datos-20261119-port.md`.

## Herramientas

- Re-emparejar `@N` tras cambios que renumeran: `MAINREF=<build de referencia> python3 tools/decomp/refmap2.py <unidades>` (por direccion; `refmap.py` falla si symbols.txt cambia de numero de lineas). Al integrar ramas que renumeran la misma unidad (cabecera compartida + cambio local): HEAD + `refmap2.py` con MAINREF = la copia del agente que toco la unidad.
- Ojo: una `static inline` nueva en el propio .cpp TAMBIEN renumera los `@N` de esa unidad (no los de otras): re-emparejar siempre.
- `tools/decomp/sdz.py`: unidades sin enlazar con globales a cero en `.sdata` (candidatas al pragma). `tools/decomp/cand.py`: tabla de candidatas por datos (abajo).

## Candidatas por datos (report.json de datos-20261119)

Datos totales 1432078 B; enlazados 1098638 (76.72 %); 1 % = 14320 B.

## A: 0-3 funciones (>= 300 B)

| Unidad | Datos | Faltan | % de las que faltan | B/func | Z |
|---|---|---|---|---|---|
| game/GameConfig_unk | 2944 | 1/15 | 98.3 | 2944 |  |
| chr/ControlChaseCaught | 2456 | 1/39 | 100.0 | 2456 |  |
| atn/ShaderManager_unk | 2192 | 1/4 | 99.0 | 2192 |  |
| atn/RenderSetting_unk | 2168 | 1/15 | 97.1 | 2168 |  |
| atn/D2A_Manager | 6076 | 3/21 | 97.5, 98.6, 99.4 | 2025 |  |
| game/Em204MulueMp | 1596 | 1/31 | 98.7 | 1596 |  |
| gmk/PassBoard | 1552 | 1/13 | 98.7 | 1552 |  |
| game/Em309GhoulSimple | 1448 | 1/22 | 99.6 | 1448 |  |
| RVL_SDK/src/revolution/net/NETAES | 2800 | 2/9 | 61.3, 62.4 | 1400 |  |
| chr/ControlAppear | 1360 | 1/24 | 98.8 | 1360 |  |
| dwc/src/base/dwc_transport | 1256 | 1/24 | 99.8 | 1256 |  |
| atn/SoundManager | 1228 | 1/76 | 99.5 | 1228 |  |
| atn/Util_unk | 1176 | 1/16 | 92.7 | 1176 |  |
| chr/ControlDive | 1096 | 1/25 | 85.0 | 1096 |  |
| Event/Section | 1076 | 1/6 | 92.4 | 1076 |  |
| Event/ActionActorBehavior | 976 | 1/14 | 99.8 | 976 | 98 |
| PowerPC_EABI_Support/src/Runtime/Gecko_ExceptionPPC | 896 | 0/14 |  | 896 |  |
| game/Em209SmallGolem | 876 | 1/22 | 99.7 | 876 |  |
| chr/ControlCannonLift | 844 | 1/17 | 99.3 | 844 |  |
| game/NewCampPartsTop | 816 | 1/12 | 98.9 | 816 |  |
| game/Em101LittleZesyaSecond | 784 | 1/15 | 98.2 | 784 |  |
| chr/ControlDamage | 2280 | 3/41 | 99.4, 99.9, 99.9 | 760 |  |
| chr/ControlInteractionBase | 1464 | 2/21 | 97.6, 97.7 | 732 |  |
| game/EnemySeaLizerd | 720 | 1/13 | 98.4 | 720 |  |
| game/NewCampPartsStatus | 1392 | 2/29 | 98.4, 99.7 | 696 |  |
| game/ShopBase | 1360 | 2/48 | 95.3, 97.1 | 680 |  |
| game/Em204Mulue | 1352 | 2/39 | 99.3, 99.6 | 676 |  |
| game/Em303KrakenFootMp | 632 | 1/21 | 99.8 | 632 |  |
| chr/ControlChant | 1228 | 2/21 | 95.0, 99.2 | 614 |  |
| game/EnemyQuarkFirst | 1144 | 2/15 | 94.6, 95.6 | 572 |  |
| atn/TextureDraw_unk | 560 | 1/4 | 98.1 | 560 |  |
| atn/NetManager | 1064 | 2/73 | 97.9, 98.5 | 532 |  |
| game/LastCocoonChrysalis | 520 | 1/7 | 0.0 | 520 |  |
| Event/ActionCameraMode | 1528 | 3/20 | 91.3, 98.9, 99.1 | 509 |  |
| gmk/RotaryRoom | 996 | 2/18 | 96.4, 99.7 | 498 | 2 |
| game/Em305VampireMp | 976 | 2/14 | 96.9, 98.9 | 488 |  |
| game/Em001Necromancer | 968 | 2/29 | 99.0, 99.5 | 484 |  |
| db/ChaseParam_unk | 480 | 1/1 | 99.0 | 480 |  |
| game/Em302KrakenMp | 1436 | 3/39 | 82.5, 98.2, 99.3 | 478 |  |
| game/MapPartsLocatorSet | 456 | 1/13 | 87.9 | 456 | 2 |
| game/Em312DarkServant | 904 | 2/21 | 98.2, 98.4 | 452 |  |
| atn/EffectLoader_unk | 448 | 1/5 | 93.3 | 448 |  |
| atn/ptr_string_base_char_0 | 440 | 1/10 | 73.1 | 440 | 1 |
| game/ShopBuySell | 872 | 2/16 | 97.4, 99.2 | 436 |  |
| atn/PBoneBasic | 424 | 1/18 | 96.7 | 424 |  |
| gmk/HealPoint | 416 | 1/15 | 93.8 | 416 |  |
| game/Em101ElderZesyaSecond | 824 | 2/15 | 99.4, 99.9 | 412 |  |
| chr/ControlMountle | 1224 | 3/29 | 98.5, 99.7, 99.9 | 408 |  |
| game/PopupTask | 1208 | 3/14 | 97.3, 99.5, 99.7 | 402 |  |
| db/WeaponParam_unk | 400 | 1/16 | 99.2 | 400 |  |
| game/Em305Vampire | 1192 | 3/30 | 94.0, 98.8, 98.9 | 397 |  |
| MP/D2AMpMenuCockpit | 784 | 2/16 | 99.6, 99.8 | 392 |  |
| game/EnvSeManager | 384 | 1/44 | 99.4 | 384 | 6 |
| gmk/AutomaticDoor | 384 | 1/19 | 93.8 | 384 |  |
| gmk/Ocean | 376 | 1/15 | 81.2 | 376 |  |
| game/NewCampPartsGmk | 1128 | 3/21 | 96.7, 97.0, 98.4 | 376 |  |
| game/SelectMember | 368 | 1/12 | 99.6 | 368 |  |
| game/GameSaveBanner | 360 | 1/9 | 99.2 | 360 |  |
| MP/D2AMpMenuFriendRoom | 712 | 2/13 | 95.3, 99.6 | 356 |  |
| game/HomeButton_unk | 344 | 1/8 | 84.5 | 344 |  |
| game/Em310Gargoyle | 1012 | 3/21 | 94.1, 99.9, 99.9 | 337 |  |
| gmk/SwingObject | 984 | 3/20 | 98.4, 99.5, 99.9 | 328 |  |
| game/GimmickManager | 616 | 2/25 | 0.0, 0.0 | 308 | 6 |
| gmk/FallingTower | 304 | 1/13 | 99.7 | 304 | 1 |
| game/Em002_01BlackSkelton | 600 | 2/13 | 94.4, 96.1 | 300 |  |
| game/ShopColor | 584 | 2/15 | 97.9, 99.8 | 292 |  |
| game/Em202_01_BSpider | 868 | 3/19 | 89.3, 93.0, 99.3 | 289 |  |
| atn/LH_Model | 576 | 2/17 | 99.0, 99.7 | 288 | 2 |
| game/main | 560 | 2/14 | 76.2, 99.4 | 280 | 4 |
| game/Np131LivingArmor | 552 | 2/11 | 96.6, 97.0 | 276 |  |
| game/SpecialEnemyBase | 544 | 2/20 | 67.1, 98.1 | 272 |  |
| game/GameOverTask | 520 | 2/7 | 96.2, 99.6 | 260 |  |
| gmk/DropItem | 496 | 2/13 | 84.6, 94.1 | 248 |  |
| game/ShopIdentify | 736 | 3/20 | 61.7, 97.6, 99.7 | 245 |  |
| chr/ControlLadder | 712 | 3/14 | 83.8, 97.8, 98.5 | 237 |  |
| Event/Dialog | 472 | 2/17 | 89.1, 97.9 | 236 | 1 |
| gmk/OnOff | 440 | 2/18 | 99.2, 99.9 | 220 |  |
| MP/D2ACommonDialog | 656 | 3/18 | 93.1, 99.0, 99.7 | 218 | 1 |
| chr/ControlDropDown | 408 | 2/6 | 98.0, 98.7 | 204 |  |
| gmk/MotionObject | 384 | 2/29 | 97.8, 99.9 | 192 |  |
| game/GameSoundControl | 344 | 2/19 | 93.0, 99.2 | 172 |  |
| Event/Channel | 492 | 3/11 | 92.4, 93.8, 98.9 | 164 |  |
| db/OptionParts_unk | 488 | 3/16 | 99.3, 99.6, 99.9 | 162 |  |
| game/GimmickStateCommand | 480 | 3/39 | 94.8, 97.5, 98.3 | 160 | 8 |
| gmk/Lemon | 320 | 2/14 | 79.6, 85.7 | 160 |  |
| atn/NetFriend_unk | 472 | 3/30 | 94.0, 99.1, 99.3 | 157 |  |
| atn/PostFxDamage | 312 | 2/3 | 98.0, 98.1 | 156 |  |
| atn/atn_colli | 464 | 3/51 | 84.7, 98.2, 99.8 | 154 |  |
| game/MotionCommand_unk | 424 | 3/28 | 81.0, 94.7, 99.9 | 141 | 8 |
| atn/TextureComposite | 416 | 3/14 | 63.3, 74.7, 98.3 | 138 |  |
| gmk/FlamingArrow | 384 | 3/20 | 96.1, 96.1, 97.9 | 128 |  |
| gmk/ChickenManager | 376 | 3/15 | 96.2, 97.4, 97.9 | 125 | 2 |
| gmk/PigeonManager | 368 | 3/15 | 91.5, 96.2, 97.4 | 122 | 2 |
| game/LogMessageTask | 356 | 3/9 | 88.0, 89.7, 97.7 | 118 |  |
| RVL_SDK/src/revolution/ipc/ipcclt | 344 | 3/23 | 0.0, 95.2, 98.5 | 114 |  |
| gmk/MapLift | 336 | 3/15 | 88.7, 96.7, 99.2 | 112 | 2 |
| game/CrowdManager | 328 | 3/10 | 96.1, 98.3, 98.7 | 109 |  |
| atn/D2A_Message | 300 | 3/9 | 67.6, 93.0, 96.3 | 100 |  |

Suma: 88392 B en 98 unidades

## B: 4-6 funciones (>= 600 B)

| Unidad | Datos | Faltan | % de las que faltan | B/func | Z |
|---|---|---|---|---|---|
| smg2/src/RVL_SDK/wud/WUD | 9536 | 6/85 | 71.2, 72.4, 73.9, 83.2, 98.2, 98.5 | 1589 |  |
| game/CosEditProtector | 3452 | 4/25 | 97.1, 97.4, 98.2, 99.2 | 863 |  |
| atn/SoftKeyboardU | 2696 | 5/46 | 93.1, 95.9, 96.3, 98.3, 99.4 | 539 |  |
| game/Em201ZanGurgFinal | 1816 | 4/78 | 75.1, 96.5, 96.8, 98.7 | 454 | 1 |
| game/TacView | 2700 | 6/46 | 97.1, 97.4, 97.6, 98.4, 99.4, 99.5 | 450 | 2 |
| atn/THPPlayer_unk | 1552 | 4/22 | 89.3, 95.7, 97.6, 99.8 | 388 |  |
| game/EnemyTasha | 1520 | 4/25 | 90.8, 99.7, 99.9, 100.0 | 380 |  |
| db/SpText_unk | 1348 | 4/10 | 97.0, 98.0, 99.5, 99.9 | 337 | 2 |
| game/NewTitleTask | 1728 | 6/9 | 93.2, 94.7, 95.8, 98.7, 99.5, 99.7 | 288 |  |
| MP/D2AMpMenuResult | 1436 | 5/20 | 89.5, 92.9, 94.0, 98.3, 99.7 | 287 |  |
| game/Em202SpiderBase | 1092 | 4/23 | 83.2, 93.0, 99.9, 100.0 | 273 |  |
| MP/D2AMpMenuSetMessage | 1060 | 4/26 | 98.4, 99.4, 99.9, 99.9 | 265 |  |
| game/NewCampPartsHint | 1284 | 5/19 | 96.9, 96.9, 99.5, 99.5, 99.8 | 256 |  |
| atn/Loader | 1504 | 6/31 | 98.0, 98.6, 99.1, 99.9, 99.9, 99.9 | 250 |  |
| game/Em401_01_WhiteLeoPard | 1200 | 5/25 | 91.4, 97.9, 98.3, 99.8, 100.0 | 240 |  |
| game/EnemyTrista | 952 | 4/18 | 87.5, 98.9, 99.2, 99.6 | 238 |  |
| game/Em101LittleZesya | 928 | 4/25 | 94.5, 98.3, 99.0, 99.5 | 232 |  |
| game/Em101ElderZesya | 1132 | 5/20 | 95.1, 98.2, 98.9, 99.3, 99.9 | 226 |  |
| Event/Cut | 1108 | 5/15 | 91.9, 92.4, 97.5, 98.7, 98.9 | 221 |  |
| game/Em306GurgBeast | 880 | 4/22 | 97.6, 98.0, 99.9, 99.9 | 220 |  |
| game/Em308InvertCrab | 1096 | 5/29 | 94.6, 97.1, 98.8, 99.8, 99.9 | 219 |  |
| game/Em208Queen | 852 | 4/29 | 97.4, 98.3, 98.7, 99.6 | 213 |  |
| game/LibraryTask | 1056 | 5/23 | 99.1, 99.3, 99.5, 99.7, 99.8 | 211 | 2 |
| game/QuadrupedTask | 1056 | 5/26 | 90.6, 91.4, 97.6, 97.9, 100.0 | 211 |  |
| game/Em308ParmCrab | 1208 | 6/24 | 82.0, 94.2, 97.2, 98.8, 99.0, 100.0 | 201 |  |
| game/Em202MothersSpider | 944 | 5/18 | 92.3, 94.3, 94.8, 95.2, 96.5 | 188 |  |
| game/MapPartsGimmickLoc | 704 | 4/46 | 87.0, 88.6, 99.6, 99.9 | 176 | 4 |
| game/em005_doppel_ganger | 1052 | 6/17 | 96.3, 98.4, 99.1, 99.5, 99.5, 99.5 | 175 |  |
| game/Em314Ivy | 824 | 5/18 | 83.5, 97.8, 99.2, 99.2, 99.8 | 164 |  |
| game/ChaserGuard | 640 | 4/13 | 97.1, 98.3, 99.2, 99.8 | 160 |  |
| game/EnvSeBase | 792 | 5/41 | 15.3, 77.9, 86.1, 95.2, 99.9 | 158 |  |
| MP/D2AMpMenuFriend | 784 | 5/21 | 97.7, 99.8, 99.9, 99.9, 99.9 | 156 | 1 |
| game/TutorialTask | 608 | 4/31 | 94.8, 98.5, 99.6, 99.8 | 152 |  |
| game/MiniMapTask | 720 | 5/52 | 94.3, 97.8, 98.5, 99.6, 99.8 | 144 |  |
| atn/SmallAllocator_8 | 840 | 6/35 | 0.0, 0.0, 43.1, 85.6, 96.1, 96.1 | 140 |  |
| Event/DialogHead | 664 | 5/25 | 98.0, 98.2, 99.1, 99.4, 99.4 | 132 |  |
| game/MapTask | 664 | 5/27 | 5.2, 95.5, 97.4, 99.8, 99.9 | 132 | 1 |

Suma: 53428 B en 37 unidades

## C: 7 o mas funciones (>= 1500 B)

| Unidad | Datos | Faltan | % de las que faltan | B/func | Z |
|---|---|---|---|---|---|
| chr/ControlStickByQuark | 7408 | 8/159 | 89.1, 94.7, 95.6, 97.6, 98.2, 99.5, 99.9, 99.9 | 926 | 1 |
| AI/AI_archer_attack01 | 9844 | 14/510 | 0.0, 93.8, 94.1, 99.1, 99.3, 99.3, 99.5, 99.5 ... | 703 | 1 |
| game/GameUI | 8972 | 19/181 | 82.1, 89.0, 91.1, 93.4, 94.2, 96.5, 96.6, 96.7 ... | 472 |  |
| atn/MaterialParser_unk | 4056 | 9/35 | 90.6, 91.7, 95.9, 95.9, 97.3, 98.2, 98.3, 99.4 ... | 450 | 10 |
| Event/ActionEffectPlay | 5532 | 13/70 | 89.8, 94.0, 95.0, 96.3, 97.1, 97.2, 98.1, 98.2 ... | 425 |  |
| game/NewCampPartsOption | 2888 | 8/30 | 87.5, 93.5, 97.5, 97.6, 97.7, 98.1, 98.5, 99.2 | 361 | 1 |
| game/Em302Kraken | 2164 | 7/49 | 86.6, 90.9, 96.5, 97.7, 98.4, 99.3, 99.4 | 309 |  |
| chr/ControlAttackBase | 2432 | 8/53 | 83.7, 85.5, 94.6, 94.6, 95.5, 97.4, 98.0, 99.7 | 304 |  |
| chr/ControlCharge | 2120 | 7/38 | 90.5, 95.2, 97.7, 98.4, 99.2, 99.4, 99.9 | 302 |  |
| game/CharaTask | 12704 | 44/436 | 31.0, 78.0, 95.0, 95.1, 95.3, 95.6, 95.6, 96.0 ... | 288 | 2 |
| game/AiTarget | 2304 | 8/44 | 86.1, 90.0, 92.7, 98.8, 98.9, 99.1, 99.2, 99.5 | 288 |  |
| MP/D2AMpMenu | 2380 | 9/41 | 98.3, 98.4, 98.6, 98.6, 99.5, 99.6, 99.7, 99.9 ... | 264 | 3 |
| Event/DebugInfo_unk | 1916 | 9/58 | 51.3, 55.3, 88.9, 96.8, 97.0, 97.9, 98.2, 98.6 ... | 212 |  |
| atn/GraphicsDevice | 1812 | 9/41 | 96.2, 97.0, 98.7, 99.3, 99.3, 99.4, 99.6, 99.7 ... | 201 |  |
| PowerPC_EABI_Support/src/MSL_C++/iostream | 7656 | 41/115 | 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 ... | 186 |  |
| game/LastCocoonQuadruped | 1636 | 9/54 | 86.4, 92.8, 97.7, 98.1, 98.6, 99.2, 99.7, 99.8 ... | 181 |  |
| PowerPC_EABI_Support/src/MSL_C++/locale | 2820 | 16/54 | 76.2, 76.2, 80.5, 80.8, 80.8, 85.5, 88.2, 88.2 ... | 176 | 1 |
| game/MenuCommonTask | 1584 | 9/41 | 93.4, 93.9, 94.8, 98.3, 98.3, 98.4, 98.8, 99.6 ... | 176 | 1 |
| game/GameEventScript | 9404 | 55/224 | 34.5, 85.3, 88.5, 89.0, 89.4, 90.1, 90.1, 91.3 ... | 170 | 10 |
| game/Em203GreaterDaemonRedMp | 1648 | 10/63 | 86.1, 90.5, 91.7, 96.3, 97.4, 98.4, 99.3, 99.4 ... | 164 |  |
| game/GameTask | 2628 | 16/133 | 95.6, 97.3, 98.5, 98.6, 98.7, 98.9, 99.1, 99.1 ... | 164 | 2 |
| game/CampSaveLoad | 1632 | 10/31 | 80.5, 88.8, 94.6, 96.2, 98.0, 99.2, 99.3, 99.5 ... | 163 | 2 |
| Event/Sequence | 2428 | 15/119 | 88.7, 88.9, 92.7, 93.8, 95.2, 95.4, 95.9, 96.3 ... | 161 | 4 |
| game/Em206CocoonMp | 1504 | 10/39 | 0.0, 0.0, 95.1, 95.3, 96.5, 97.8, 99.1, 99.7 ... | 150 |  |
| game/NewCampPartsMapSelect | 1756 | 12/26 | 89.8, 91.8, 92.6, 95.3, 95.3, 95.8, 96.1, 96.4 ... | 146 |  |
| atn/PacketPack | 7844 | 54/380 | 0.0, 4.4, 72.1, 87.0, 89.8, 91.5, 94.1, 94.2 ... | 145 | 20 |
| game/Em204BigMulue | 1568 | 11/38 | 96.8, 97.6, 97.6, 98.3, 98.8, 99.1, 99.2, 99.4 ... | 142 |  |
| game/LastCocoonImago | 1556 | 11/36 | 73.6, 94.9, 95.3, 95.5, 96.3, 97.7, 98.6, 98.8 ... | 141 |  |
| game/BattleTask | 4860 | 35/212 | 89.1, 90.5, 91.3, 92.5, 93.0, 93.7, 94.7, 95.8 ... | 138 |  |
| atn/DrawHelper | 1512 | 11/68 | 92.3, 94.8, 96.0, 98.3, 98.3, 98.8, 98.9, 98.9 ... | 137 |  |
| MP/D2AMpMenuLobby | 1764 | 13/39 | 95.6, 95.6, 95.8, 98.3, 98.7, 98.9, 99.1, 99.3 ... | 135 |  |
| game/Em203GreaterDaemonRed | 2152 | 16/53 | 77.9, 86.1, 89.9, 91.4, 92.6, 92.7, 94.1, 95.7 ... | 134 |  |
| game/EnemyQuarkStand | 2568 | 20/80 | 89.0, 89.1, 95.3, 95.4, 96.0, 96.3, 96.3, 96.6 ... | 128 | 2 |
| Event/ActionActorPosition | 1812 | 15/51 | 93.7, 95.0, 96.1, 96.9, 97.2, 97.2, 97.5, 98.3 ... | 120 | 2 |
| game/Em202ForestSpider | 1528 | 14/64 | 72.1, 80.6, 92.3, 93.0, 94.2, 95.2, 97.5, 98.3 ... | 109 |  |
| game/Em203GreaterDaemon | 1744 | 16/66 | 79.1, 80.5, 85.6, 86.2, 89.2, 90.0, 94.4, 94.8 ... | 109 |  |
| game/GameScene | 1860 | 24/188 | 91.0, 93.5, 96.5, 96.5, 96.5, 96.6, 96.8, 97.5 ... | 77 | 80 |
| game/GameCamera_unk | 2240 | 29/90 | 32.5, 70.2, 83.2, 84.9, 85.0, 87.9, 90.6, 91.4 ... | 77 | 7 |
| game/EnemyQuarkSecond | 1848 | 24/70 | 80.0, 80.3, 87.4, 87.9, 91.0, 91.0, 91.0, 93.4 ... | 77 | 2 |
| Event/Manager | 1508 | 40/185 | 0.0, 0.0, 30.9, 30.9, 33.3, 33.3, 59.1, 62.3 ... | 37 | 6 |

Suma: 137592 B en 40 unidades

