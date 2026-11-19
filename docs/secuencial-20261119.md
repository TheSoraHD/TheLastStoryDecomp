Inicio 2026-11-19 02:12 | Cuentas: unidades 742 | 100% 16074 / 18109 | codigo 62.39 % | datos 76.18 % | 0% 45 (20) | DRAFT 0

# secuencial-20261119

Sesion secuencial de 12 h (sin agentes). Parte de `main` 3197b85. Prioridad: port de PC; despues, enlazar.
Referencia: `/home/claude/ref` (1077 unidades con .o, labelcheck 11, rep100 23).

## Puntos de control

| Hora | Fase | Cuentas |
|---|---|---|
| 02:30 | 0 | 742 u, 16074, 62.39 / 76.18, 0% 45, DRAFT 0 |
| 03:55 | 2 | 742 u, 16075, 62.40 / 76.18, 0% 45, DRAFT 0 (D1 +1; fase 2 sin cambios de codigo) |
| 05:20 | 2 | 742 u, 16075, 62.40 / 76.18, 0% 45, DRAFT 0 |
| 13:05 | cierre | 742 u, 16075, 62.40 / 76.18, 0% 45, DRAFT 0; build limpio `main.dol: OK`, lost 0, sin bajadas, labelcheck 10 (Sound3dListener arreglada), rep100 23, poolcheck igual que main (134 lineas), 16 avisos de alineacion de dtk (splits sin cambios); clon limpio `main.dol: OK` |

## Fase 0: listas

Unidades con 1-3 funciones < 100 % y todas >= 97 % (86): `docs/secuencial-20261119-f3list.txt` (con las notas de enlazar2/mwcc-blockers que las citan).

Inventario de vistas del port (`grep -rlw` en src + include):

| Vista | Ficheros antes | Despues |
|---|---|---|
| `Field_unk` | 99 | 6 (03:55), 5 (05:20), 5 (08:45) |
| `BulletField_unk` | 1 | 0 (03:55), 0 (05:20), 0 (08:45) |
| `SoundEngine_unk` | 7 | 7 (03:55), 7 (05:20), 7 (08:45) |
| `MenuD2A_unk` | 6 | 6 (03:55), 5 (05:20), 4 (08:45) |
| `PathData_unk` | 11 | 11 (03:55), 10 (05:20), 10 (08:45) |
| `ActionOwner_unk` | 9 | 9 (03:55), 0 (05:20), 0 (08:45) |
| `Settings_unk` | 47 | 16 (03:55), 11 (05:20), 9 (08:45) |
| `EffectMgr_unk` | 92 | 92 (03:55), 58 (05:20), 51 (08:45) |
| `GameTaskTable_unk` | 2 | 3 (03:55), 3 (05:20), 3 (08:45) |
| `CharaIdEntry_unk` | 2 | 7 (03:55), 7 (05:20), 7 (08:45) |
| `ShopChara_unk` | 1 | 0 (03:55), 0 (05:20), 0 (08:45) |
| `NewCampStatusName_unk` | 1 | 0 (03:55), 0 (05:20), 0 (08:45) |
| `MenuItem_unk` | 3 | 2 (03:55), 2 (05:20), 2 (08:45) |
| `SelectMemberAnim_unk` | 2 | 2 (03:55), 2 (05:20), 2 (08:45) |
| `SceneData_unk` | 0 | 0 (03:55), 1 (05:20), 1 (08:45) |
| `MotionData_unk` | 1 | 1 (03:55), 2 (05:20), 2 (08:45) |
| `ModelOcclusion_unk` | 0 | 0 (03:55), 0 (05:20), 0 (08:45) |
| `fn_8013C504__9Field_unkFv` | 1 | 0 (03:55), 0 (05:20), 0 (08:45) |

Vistas locales de globales (un `struct X` del propio .cpp y `extern "C" X* lbl_8087....`): 147 (07:15) -> 96 (08:45).
Por global a las 08:45: lbl_8087F610 20 (`MP::Manager2`), lbl_8087EFA8 11, lbl_8087F628 10 (`atn::net::Manager`),
lbl_8087F1E4 9, lbl_8087F0A8 7, lbl_8087F490 6, lbl_8087F018 6, lbl_8087EE98 5, lbl_8087F430 3, lbl_8087EE90 3.
A las 10:20: 77 (lbl_8087F610 20, lbl_8087EFA8 11, lbl_8087F1E4 9, lbl_8087F0A8 7...). Globales declarados `void*`: lbl_8087EE98 87 -> 12,
lbl_8087EF70 62 -> 6, lbl_8087EEC8 36 -> 0, lbl_8087F430 15 -> 0.

## Decisiones

- D3 (aprobada): lo de "Para revisar" de enlazar2 se queda como esta (`int& cur = m84`, `float zero`, `FindPlace_unk(id = ...)`, `GXWGFifo` directo, `GetSaveFileName_unk`, alias `self`, las dos `const` de C2).
- Repo del port (TheLastStory-PortTest): no se toca; solo notas (respuesta del usuario al empezar).

## Fase 1: D1 y D2

- **D1** (commit 685d6c2): `block_relocations` de OSInit (0x805EB77C, 0x805EB7E4) en `config.yml`. Build limpio `main.dol: OK`;
  OSInit 99,97 -> 100 (+1, 16075), lost 0, sin bajadas, poolcheck de OS limpio, labelcheck/rep100 iguales.
- **D2: revertido** (medicion sin re-emparejar sobre 685d6c2; parche aplicado + los tres sitios que no compilaban con la
  firma nueva: `vector_vec3_unk.h` push_back con `c`, `EventVector_unk.h` x2 y `Sequence.cpp` AddSlow_unk con `this->mCap`;
  diff completo en `docs/secuencial-20261119-D2.patch`).
  - Ganadas 8: ChaserNpc ctor, Em202Spider vf0C, Em202SpiderBase vf0C, Em308InvertCrab vf0C, Em308ParmCrab vf0C,
    EnemyQuarkSecond fn_80338D14, GameUI fn_803E2110 (y ShopBase fn_805849E8 no: queda igual).
  - Perdidas 22: copias debiles de push_back 100 -> 99,84 (Em001Necromancer x2, Em302Kraken x2, Em302KrakenMp x2, GameScene,
    CharaPathMove, LastCocoonEgg, MapPartsGimmickLoc, Puppeteer_CharaTask `vector<int>`), `__expand_by_unk` 0x8049003C
    (firma), y por presupuesto de inlining CosEditItemList_unk 7 (fn_801E07A4, size, `operator[]`, clear, push_back x2,
    CosEditBegin/End_unk dejan de salir fuera de linea) y chr/ControlAppear 2 (invoke/manage de boost::function a 0).
  - Bajadas grandes: Event/Sequence fn_8054204C 97,82 -> 91,54 (aqui no 10,3), `__push_back_slow_unk` de Sequence y
    EventParser 96,3/90,4 -> 0 (firma), Event/Manager fn_80485028/5964/62C4 ~98 -> ~90, Cut/Channel/Section ~92 -> ~87,
    EventParser ParseDUID/POIN ~98 -> ~87, MapPartsBase push_back x2 98,3 -> 87,3, Em403WarLion push_back 98,6 -> 87.
  - Neto -14. La prueba de 45 min de recuperar las copias debiles no se hizo: aunque volvieran las 11 copias, las 10
    perdidas por presupuesto/firma dejan el neto en -2 (< +5). fn_8054204C: no aplica (sin parche sigue en 97,82).

## Fase 2: port

Detalle de cada cambio (para el port): `claude/port-secuencial-20261119.md` del proyecto (copia en
`docs/secuencial-20261119-port.md`). Todos con `PRECOMMIT OK` (lost 0, sin bajadas, labelcheck/rep100 iguales,
poolcheck igual que main) salvo el `ordercheck` de GameEventScript, que ya tiene 1 fuera de orden en main.

| Commit | Punto | Cambio |
|---|---|---|
| 76f3f7d | 2.1 | `GameTask::fn_8013C504` y getters debiles de GameUI en las cabeceras; 22 unidades (todas las del accesor) |
| b8be23f | 2.1 | 55 unidades con vistas de miembros reales |
| f630a94 | 2.1 | 33 unidades: vistas opacas, `mChapter`, `GameTask::SetFadeMode_unk` |
| 5bd87f0 | 2.1/2.2 | camaras de jefes (m46C/m910/m4FC) y BattleCharaTask (firmas con `GameTask*`); EnemyTrista fn_8035D294 60,90 -> 87,46 |
| eaa7ea4 | 2.2 | TownTask; sub-vistas de `GameScene` (Cliffhang, PickTask, Augur, GameBgm, TownTpcLoader, CharaStatus...) |
| f1e72a8 | 2.4 | `CharaIdEntry_unk` en `db/CharaIdTable_unk.h` (fuera ShopChara_unk, NewCampStatusName_unk, MenuItem_unk de personajes) |
| 34d8804 | 2.4 | `CharaInfo_unk::m14` = `const char*` |
| 09a7051 | 2.5 | 31 `Settings_unk` -> `GameConfig_unk` |
| c424b8f | 2.5 | 4 `Settings_unk` mas (m4DC/m53C, `Input_unk` comun); EffectMgr_unk en FallingTower/WaterLevel |
| e0b50cc | 2.6 | `EffectSlot_unk::m0C` = `atn::Texture*` |
| 60ffa1a | 2.5 | `ActionOwner_unk` = `Event::Section` |
| 9cac0ae | 2.5 | `atn/SceneData_unk.h`, `atn/MotionData_unk.h` |
| 4d47070 | 2.5 | `EffectMgr_unk::mB8` -> `mSystem.mLayer` en 41 unidades (`atn/EffectApi_unk.h`) |
| 5b0c35d | 2.5 | `MenuD2A_unk` = `atn::D2A_Task` (`atn/D2A_TaskApi_unk.h`); PostFxMotionBlur con `RenderSetting_unk` |
| 243e05b | 2.5 | `MP::MpD2A_unk` = `atn::D2A_Task` (13 unidades MP) |
| 072aa2e | 2.5 | ReinforcePoint con `mDifficulty` |
| 1ef554e | 2.1 | Bullet y NewCampPartsOption con `GameTask` |
| d66a8b7 | 2.4 | GameOverTask y NewCampPartsTop con `CampSaveLoad.h` |
| 27a692d | 2.5 | `PathData_unk` = `Event::Path` |
| a8e9425 | 2.5 | vistas de `lbl_8087EFA8` -> `RenderSetting_unk` (51 unidades) |
| e51a120 | 2.5 | resto de vistas de `lbl_8087F0A8` -> `GameConfig_unk` (47 unidades) |
| 818dfde | 2.5 | vistas de `lbl_8087EEE0` -> `atn::GraphicsDevice_unk` (16 unidades) |
| 9c00cc9 | 2.5 | GameUI, `Event::Manager`, ChaseManager (16 unidades) |
| 956b4da | 2.5 | PlayerManager, WPad_unk, SoundManager (12 unidades) |
| 2905a11 | 2.5 | RenderSetting_unk en 7 vistas con sub-structs |
| 3120cf3 | 2.5 | `NextChara_unk` por `PlayerTask::mNext` (3 unidades) |
| b9f5304 | 2.5 | una sola `DebugMessages_unk` (17 unidades) |
| c8031d3 | 2.5 | GraphicsDevice_unk en 9 vistas mas (`mRMode.viWidth/viHeight`, declaraciones de la cabecera) |
| 63d7fc6 | 2.5 | `atn::EffectManager` en 12 vistas mas (`mSystem.m70/m78/m7C/mReplaying`, `mFlags`) |
| ddc1a34 | 2.5 | `GameConfig_unk::m4DC/m53C` con nombres de las claves CSV de db/ChaseParam_unk |
| 572c18d | 2.5 | `atn::SoundManager` en Event/Path (`SetListener_unk` miembro) y GameScene |
| 0e5f032 | 2.5 | `CharaManager<PlayerTask>`/`PlayerTask` en GimmickStateCommand, SongManager, LemonBucket |
| 1dcc25b | 2.5 | `TacView` en PlayerInput_unk; `SkillEffectManager` en Em203GreaterDaemonRed(+Mp) |
| d51cb4e | 2.5 | `TextTable_unk` (lbl_8087F1E4) en `atn/TextTable_unk.h`; 30 vistas fuera |
| 1dad3c9 | 2.5 | `GameConfig_unk` en CharaStatus, GameTask (`mDifficulty`), GameOverTask, MapPartsLocatorSet |
| 927dacc | 2.5 | `GameConfig_unk`/`Input_unk` en ControlCaught, ControlJumpAt, ControlStickByQuark |
| d2df91b | 2.5 | `GameConfig_unk::mInput` en MenuCommonTask y Event/Dialog |
| b83d38f | 2.5 | `RenderSetting_unk` en PostFxBloom/Gamma/GodRay/Damage, Em301LShade, Em203GreaterDaemonRed (fn_8029A0A8 81,60 -> 94,13) |
| 070519a, b200f44, f662477, f97fa52 | regla 10 | 11 funciones `extern "C"` de MenuCommonTask con `menu` pasan a miembros |
| efa4ee3 | 2.5 | `ShopD2A_unk` = `atn::D2A_Task`; fuera las vistas de D2A_Task de las tiendas y CosEditProtector |
| fe81365 | 2.5 | `ShopPoint_unk` = `atn::D2A_DrawRect` en ShopBase.h |
| c91c026 | 2.5 | `atn::D2A_Manager` y `D2A_MessageTable_unk` en cabeceras; 5 vistas de lbl_8087F138 fuera |
| 5da5afc, 15e69c0 | 2.5 | `WPad_unk` (GameTask, HomeButtonMenu, StrapTask, main) y `GameUI` (EnemyJill) |
| 014478c, da8a5f6, 97ac04f | 2.5 | tipos opacos de globales -> clase real (`atn::EffectManager` 48 unidades, `atn::Loader` 35, otros 8) |
| 5de5f08, 856f6e2, dc70760 | 2.5 | globales `void*` -> clase (GameTask, Loader, SkillEffectManager, GraphicsDevice_unk, GameConfig_unk, Event::Manager, D2A_Manager, Font_unk, NandManager, PostFxManager; ~76 unidades) |
| b96f79d ... a9cb196 | firmas | `tools/decomp/sigcheck.py`; firmas `extern "C"` corregidas: menu de depuracion (97 unidades, callback al final), `fn_8023A8B4` (100), rayos de colision, `fn_800CB5B4`, punteros declarados `int` (162 -> 82 funciones distintas) |
| f5118ef, 25290be, 0e3ab2a | 64 bits | retornos puntero declarados `int`; `BulletShot_unk::m04/m08` y `Bullet::m60/m64` como punteros |
| 479e5d2, 4ff7155 | 2.5 | `atn::net::Manager` (DamageCalc, Item, DropItem, ControlCannonLift, PPDATAU) |
| 87bbbaa, 3c247af, 31ca307, c593860 | 2.5 | ChaseManager (ChaserGuard), Loader (GameSaveBackground, main), NandManager (HomeButtonMenu), VideoTHP (SystemErrorMessage) |
| 04309f2, 60bc31f | 2.5 | `lbl_8087EE98` -> `ColliManager_unk*` (75 unidades), `lbl_8087EF70` -> `Pad_unk*` (56) |
| 57299b2 | 2.5 | `lbl_8087EE98` -> `ColliManager_unk*` en 9 unidades mas |
| 9ad7f65 | 2.5 | `GameConfig_unk::mSurround` (0x454) y accesores miembro; fuera las vistas `Settings_unk` de CharaTask, Em315Coffin, TutorialTask, Event/Path, GameCamera_unk, ControlStickByQuark |
| 93fe121, 93c134a | 2.5 | `GameConfig_unk` en GameBgm, GimmickCommon, BootTask y GameSoundControl (`mInput`, `m113C`, `m1140` en vez de bytes) |
| 1e4a41b | 2.5 | `WPad_unk` en SystemErrorMessage_unk y HomeButton_unk (`mStatus[i]` en vez de `+ i * 0xF00`) |
| bf4a3b4 | 2.5 | `GameUI` en AttractHatePoint y LottaSummon (`Request_unk`, constructor `GameUIElem1C_unk(int)`) |
| 4813e92, 6ca6827, f9c2260, 9c11016 | 2.5 | `RenderSetting_unk` en LinearAllocator, ModelDraw_unk (`m104` = `ShadowParam_unk`), Layer2DBlur, ModelInstance |
| 6323082 | 2.5 | `lbl_8087EE60/EE64` de `void*` a `CharaTask*`/`CharaLink_unk*` (lista de 16 en `CharaLink_unk`); fuera `AiList_unk` |
| 3430212 | 2.6 | `GameUIMarker_unk::m10` = `CharaTask*`; TownTpcLoader limpia `GameUI::mD40` (inline `static` del .cpp) |
| 822c7cc | 2.6 | `atn::ColliBase::m0C` (dueno) = `void*`: fuera `(int)this` en 22 unidades; `BattleTask::m3E2E0` igual |
| 0100abd | 2.6 | `GimmickMsg_unk::m10` = `CharaTask*` (9 unidades) |
| 060e436, ddaec9f, a0afe75 | 2.6 | `EnemyQuarkStand::m16F0`, `GameEventScript::m98`, `NewCampPair_unk::m04`, `WindySlot_unk::m184`, `PopupEntry_unk::mB0` (`MtxPtr`) |
| 0c51c4e | regla 10 | Em101LittleZesya: 3 `extern "C"` con `zesya` a miembros; `Em101ElderZesya::m16A0`/`m1530` tipados |
| e077c2b | 2.6 | `SpecialEnemyBase::m14B0` (`EnemyInfo_unk*`), `EnemyQuarkStand::m14B0`, `Em303KrakenFootMp::m14F4` |
| b1a6996, 9b80141 | 2.6 | atacante (`CharaHit_unk::m10`) y quien derroto (`CharaTask::m13B0`) como `CharaTask*`; `BattleTask::fn_800FDE60` e `Inventory::fn_80449A18/ABC` con puntero (simbolos renombrados) |
| bf2c660, 082973f | regla 10 | `atn::net::Manager` lock/unlock y ChaseManager (3) a miembros |
| 7d10fd3 | 2.5 | `lbl_8087FA20` (`TownTask*`), `lbl_8087F418` (`EnvSeManager*`) |
| bcd23c6 | regla 10 | Em303KrakenFoot: 3 `extern "C"` con `foot` a miembros |

Re-emparejado: `pdbatch.sh` y, para lo que queda (pools iguales que main), `refmap.py` (scratch): nombre en symbols.txt de main ->
(seccion, desplazamiento) en el .o de main -> nombre en nuestro .o. Mas fiable que lblseq/valfix cuando el pool no cambia;
`@STRING@` y etiquetas vigiladas restauradas por linea tras cada lote.

GameEventScript fn_803A26F4 98,75 -> 99,55 (llama al getter debil y a `GameScene::FindPlace_unk`, como el original).

## Fase 3: enlazar

- Lista de candidatas en `docs/secuencial-20261119-f3list.txt` (unidades con 1-3 funciones por debajo del 100 %): todas las
  que quedan cerca estan en `docs/mwcc-blockers.md` con muchas variantes ya probadas (ControlChaseFollow vf0C 99,97,
  ControlGrappled vf10 99,89, ControlSlip vf0C 99,92, PostFxMotionBlur vf10 99,90...). Sin unidades nuevas enlazadas.
- Unidades con todas las funciones al 100 % y sin enlazar: game/Em315Coffin y game/MapPartsRain (el `.sdata` a 0.0f,
  fuera de alcance).
- GameEventScript fn_803A26F4 98,75 -> 99,55 (fase 2). PostFxMotionBlur vf10 sigue en 99,90 con `RenderSetting_unk`.

## Fase 4: patrones en lote

- GameEventScript fn_803A2378 / fn_803A2210 / fn_803A7D94 (97,3-97,8 %, las "sin probar"): la unica diferencia es el
  `stw` de `mWait` dos instrucciones antes que en el original (entre `lis` y `addi` de `lbl_8074ED24`; el original lo pone
  tras el `addi`). 10 variantes en fn_803A2378 (expandir `SetNext_unk` a mano con tabla/indice/referencia en varios ordenes,
  `bool`, ternario, ifs anidados, dos llamadas): el `stw` no se mueve. Misma familia que los 21 `SetNext_unk`.
- `m97C.Set_unk` (patron T): no repetido; las vistas de la fase 2 ya usan `SetCameraLook_unk` libre y el bloqueo sigue.

## Cambios de splits

## Bloqueos

- 2.1: `QuarkField_unk` de EnemyQuarkSecond::fn_803412AC y EnemyQuarkFirst (el original guarda la direccion de la
  sub-vista; con `GameCamera&`/`GameCameraParam_unk` cambia la base o entra el ctor de la cabecera). TeamAIManager
  (`TeamTargets_unk::Find_unk` sobre tres `SimpleArray` de tipos distintos), GameSoundControl (`FieldStream_unk` es una vista
  de los fades m5620/m5624), Bullet (`HomingField_unk`) y NewCampPartsOption (`class Field_unk`): sin tocar.
- 2.3: `atn::AudioDevice` a cabecera: la clase lleva ~700 lineas de tipos locales (vistas de nw4r::snd con tamanos,
  `AudioMap_unk`, `Sound3dEngine m900` de otra unidad); incluirla en Sound3dListener/SoundManager/BootTask/GameVoice/PacketPack/
  Event/Manager choca con sus vistas locales de nw4r. No empezado.
- 2.4: GameOverTask/NewCampPartsTop con `CampSaveLoad.h`: choca `fn_801F4E8C` (`D2A_DrawRect GetRect(D2A_Task*, ...)` frente
  a `NewCampPartsPoint_unk (atn::Task*, ...)` de NewCampPartsBase.h). `SelectMemberAnim_unk`: `mAnims[9]` se recorre como
  array; separarlo cambia el tipo de todos los accesos. No probados.
