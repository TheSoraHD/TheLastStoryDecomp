# Para el port: cambios de `secuencial-20261119`

Sesion secuencial del 2026-11-19 (rama `secuencial-20261119`, notas `docs/secuencial-20261119.md`). Todo neutro para MWCC
(mismo codigo) salvo donde se dice. Cada punto: lo que el port tiene que cambiar o puede quitar de su rama de PC.

## 2.1 Vistas locales de GameTask (`lbl_8087F430`)

### Grupo 1 (commit 76f3f7d): el accesor `fn_8013C504` y los getters debiles de GameUI

- `GameTask::fn_8013C504()` (0x8013C504, definido en game/CharaTask.cpp; antes `Field_unk::fn_8013C504`, con
  vistas que devolvian `GameScene*`, `atn::Task*`, `void*` o `PartyTable_unk*`): declarado en `game/GameTask.h`.
  Em403WarLion y PlayerManager lo definen `inline` (como el original, que lo inlinea alli).
- Getters debiles de game/GameUI declarados en las cabeceras y definidos inline en las unidades que los usan:
  `GameTask::GetState_unk() const` (m54E4, 0x803D6E24), `GameTask::GetRoute_unk()` (&m550C, 0x803D7058),
  `GameTaskTable_unk::HasState_unk() const` (mState != 0, 0x803D7044; antes `GameUIRoute_unk::HasM2C_unk`),
  `GameCamera::GetCamera_unk()` (&mCamera, 0x803D6EEC) y `GameCamera::GetMode_unk() const` (m7FC, 0x803D6F70;
  antes `GameUIPlayerInput_unk`). `fn_80122550` devuelve `GameCamera*` (= &field->mCamera).
- Vistas sustituidas por `GameTask*` (desplazamiento de la Wii -> miembro real):
  - `m48/m4C/m50` -> `mOption.mType/mMap/mSub`; `m74`/`mEye` -> `mCamera.mCamera.mPos`; `m80` -> `mCamera.mCamera.mTarget`;
    `m260` -> `mCamera.m1F4`; `m4D8` -> `mCamera.m46C` (GameCameraModeParam_unk; `.mMode`); `m568..m584` ->
    `mCamera.m4FC/m508/m514/m518` (camara fija); `m588` -> `mCamera.m51C`; `m868`/`m86C`/`m870` -> `mCamera.m7FC/m800/m804`;
    `m8A0` -> `mCamera.m834` (personaje seguido); `m910` (GameUI) -> `mCamera.m8A4`; `m96C..m978` -> `mCamera.mShake`
    (mFrame/mLength/mWidth/mHeight); `m97C..` (punto mirado) -> `mCamera.m910/m911/m914/m91C/m934`; `m988` -> `mCamera.m91C`;
    `m54F4` (CharaTask) -> `mMessages`; `m5744` es `unsigned int`.
  - Metodos de las vistas pasados a inlines libres: `SetCameraLook_unk(GameCamera&, pos, flag)` (antes `*Look_unk::Set_unk`
    en Em203GreaterDaemon(+Red/RedMp), Em302Kraken, EnemyQuarkStand), `SetCameraLookPos_unk`, `SetCameraMode_unk(
    GameCameraModeParam_unk&, int, const GameCameraParam_unk&)` (EnemyQuarkStand, antes `QuarkFieldCamera_unk::Set_unk`;
    `QuarkCamParam_unk` = `GameCameraParam_unk`), `Shake_unk(GameTask*, int, float, float)` (LastCocoonEgg/Quadruped).
  - PlayerManager: `PartyTable_unk`/`PartySlot_unk` -> `GameScene::m148` (`SimpleArray<gs::SceneGroup_unk>`,
    `mLeader`/`mCharas`).
  - PacketPack: `PPField_unk` -> `GameTask`; `mgr->mGame->fn_8013C504()` (`GameScene*`).
  - Unidades: CharaTask, GameUI, PacketPack, PlayerManager, Cannonball, FlamingArrow, GameCamera_unk, GameEventScript,
    Em001Necromancer, Em203GreaterDaemon(+Red/RedMp), Em301LShade, Em302Kraken(+Mp), Em309Ghoul, Em310Gargoyle,
    Em403WarLion, EnemyQuarkSecond, EnemyQuarkStand, LastCocoonEgg, LastCocoonQuadruped.
  - Queda: `QuarkField_unk` de EnemyQuarkSecond::fn_803412AC (el original guarda la direccion de la sub-vista del
    punto mirado; con `GameCamera&` cambia la base del `this`, patron T).

### Grupo 2 (commit b8be23f): 55 unidades con vistas de miembros reales

- Vistas de `lbl_8087F430` cuyos miembros son miembros reales de GameTask (`m10D0`, `m10D4`, `m10D8`, `m54E4`, `m5590`,
  `m5634`, `m5660`, `m5674`, `m5694`, `m5744`...) o la camara (`m868` -> `mCamera.m7FC`, `m8A0` -> `mCamera.m834`,
  `m96C..m978` -> `mCamera.mShake`, `m06C` -> `mCamera.mCamera`) -> `GameTask*` (`extern "C" GameTask* lbl_8087F430`,
  `fn_80121F00` devuelve `GameTask*`). Em006DarkMaster: el local `routes` es `GameScene*` (antes `void*` con cast).
- Unidades: AI/AiCommon_unk MP/D2AMpMenuResult chr/ControlChant chr/ControlCharge chr/ControlDamage chr/ControlPushedDown game/ArenaTask game/Arms game/BattleResult game/CampSaveLoad game/CharaC58_unk game/ChaseManager game/ChaserGuard game/ChaserNpc game/CrowdSimulation game/DamageCalc_unk game/Em003LesserDaemon game/Em006DarkMaster game/Em101ElderZesyaSecond game/Em101LittleZesya game/Em201ZanGurgFinal game/Em202ForestSpider game/Em202MothersSpider game/Em202Spider game/Em202SpiderBase game/Em202_01_BSpider game/Em204MulueMp game/Em206CocoonMp game/Em208Queen game/Em303KrakenFoot game/Em303KrakenFootMp game/Em305Vampire game/Em305VampireMp game/Em308InvertCrab game/Em308ParmCrab game/Em309GhoulSimple game/Em401_01_WhiteLeoPard game/EnemyFieldMagician game/EnvSeBase game/GameChoiceWindow game/LastCocoonChrysalis game/LogMessageTask game/PlayerInput_unk game/QuadrupedTask game/RouteWalker_unk game/SelectMember game/ShopBuySell game/em005_doppel_ganger gmk/AttractHatePoint gmk/BulletPoint gmk/InteractionSpot gmk/LottaSummon gmk/MeetingSpot gmk/RandomItem gmk/ReinforcePoint 

### Grupo 3 (f630a94): 33 unidades

- Vistas opacas (`struct X_unk;` solo declarado, usado con casts `(GameTask*)`) -> `GameTask*` directo, sin casts.
- `mChapter` (CosEditProtector, ShopBase) -> `m10D0`; `mFlags` (ControlDropDown) es `atn::Task::mFlags`; `m20`
  (GameOverTask, NewCampPartsTop) es `atn::Task::m20` (`void*`, el padre; cast a `atn::Task*` para FadeColor::Create).
- `GameTask::SetFadeMode_unk(int)` (m563C, copia debil 0x805BB594 de TutorialTask; antes `TutorialField_unk`).
- Unidades: Event/ActionEffectPlay chr/ControlCannonLift chr/ControlDropDown chr/ControlStagger db/ItemParam_unk db/WeaponParam_unk game/CosEditProtector game/Em002_01BlackSkelton game/Em002_02SkeltonKing game/Em201ZanGurg game/Em204BigMulue game/Em205Ogre game/Em209SmallGolem game/Em314Ivy game/EnemyJill game/EnvSeManager game/GameOverTask game/LibraryTask game/MapManager game/NewCampPartsHint game/NewCampPartsStatus game/NewCampPartsTop game/NewCampTask game/Np131LivingArmor game/ShopBase game/SkillEffectManager game/SystemErrorMessage_unk game/TutorialTask gmk/Coffin gmk/FragileObject gmk/MapLift gmk/PassBoard gmk/WaitObject 

### Grupo 4 (5bd87f0): camaras de jefes y BattleCharaTask

- Em101ElderZesya, Em101LittleZesyaSecond, EnemyTrista, Em204Mulue, Em206EggMonster: vistas `*Camera_unk m4D8` ->
  `SetCameraMode_unk(GameCameraModeParam_unk&, ...)` sobre `mCamera.m46C` (`m04` -> `mWeight`, `m08` -> `mStep`),
  `*CamParam_unk` -> `GameCameraParam_unk` (`m588` -> `mCamera.m51C`), `*Look_unk m97C` -> `SetCameraLook_unk`.
  Em206EggMonster: `EggFieldCam_unk` (vista de GameCamera) fuera: `m08/m14` -> `mCamera.mPos/mTarget`, `mD0` ->
  `mCamera.mInvView.m`, `m900..m90C` -> `mShake`; `m54F4` -> `mMessages`.
- LastCocoonImago: `m006C` -> `mCamera.mCamera`, `m54F4` -> `mMessages`. Em306GurgBeast: `mCamera.fn_8037EF30` sin cast.
- BattleCharaTask: `CameraSub_unk` fuera (`m08/m14` -> `mCamera.mCamera.mPos/mTarget`, `m1FC/m208` -> `mCamera.m1F4.mPos/mTarget`),
  `m868` -> `mCamera.m7FC`, `m94C` -> `mCamera.m8E0`, `BattleArea_unk` -> `GameScene` (`m64` es `MapTask*`, `m48` -> `mType`).
  `fn_800E62AC`, `fn_800E73EC`, `fn_800E7650` reciben `GameTask*` (antes `Field_unk*`; BattleCharaTask.h y symbols.txt).
- No llevado: EnemyQuarkFirst (el local de parametros es un POD sin ctor y `SetCamera_unk` es de la tarea entera; con
  `GameCameraParam_unk` el ctor de la cabecera seria una llamada: ver qV, sigue igual).

### Grupo 5 (eaa7ea4): TownTask (punto 2.2) y sub-vistas de `GameScene`

- TownTask: `m38` -> `atn::Task::mFlags`, `m553C` -> `m550C.m30` (GameTaskTable_unk), `m868` -> `mCamera.m7FC`.
- ControlCliffhang: `CliffTable_unk`/`CliffPoint_unk`/`CliffLink_unk` -> `GameScene::mC4` (`gs::InteractionPoint`: `m00` -> `mId`,
  `mType` -> `m18`) y `gs::InteractionConnection` (`mA/mB` -> `m00/m04`).
- PickTask: `PickAreaTable_unk` -> `GameScene::mF4` (`gs::AiArea`: `mRotY` -> `m14`, `mId` -> `m24`).
- Augur: `AugurRoutes_unk::mA4 + (i - 1) * 8` -> `&GameScene::mA0.mData[i - 1]` (`fn_803CD958(U16Table_unk*, int)`).
- GameBgm: `GameBgmTable_unk::m140` -> `GameScene::m140` (`gs::BgmBox*`). TownTpcLoader: `CharaList_unk` -> `GameScene`
  (`mNum` -> `m80.mSize`, `m134` es `GameEventScript*`).
- CharaStatus: `FieldInfo_unk` -> `ChapterEntry_unk` (db/ChapterList_unk.h). CrowdManager: `mMap/mArea/mRoutes/mMode` ->
  `mOption.mMap/mOption.mSub/m10D8/m54E4`. GameScene (su vista del campo): `m54F4` -> `mMessages`, `m5514..m551C` ->
  `m550C.mType/mId/mSub`, `m5538` -> `m550C.mState` (igual en GameVoice). RandomMerchant: `fn_8021DE88(int level)`
  (la declaracion local decia `void*`), `m10D8->fn_803CC6B4` sin cast. MarketTask: `m10D8` es `GameScene*`.
- Unidades: chr/ControlCliffhang game/CharaStatus game/CrowdManager game/GameBgm game/GameScene game/GameVoice game/MarketTask game/PickTask game/TownTask game/TownTpcLoader gmk/Augur gmk/RandomMerchant 

## 2.3 `atn::AudioDevice` con cabecera: no llevado

- La clase real esta en atn/AudioDevice.cpp (unidad enlazada) con ~600 lineas de tipos locales (`AudioMap_unk` con
  miembros de plantilla fuera de linea, vistas de nw4r::snd, `AudioBank_unk`), y Sound3dEngine.cpp tiene su propia
  `AudioDevice` parcial; sus metodos son `extern "C" fn_800xxxxx`. Pasarla a cabecera (y los `SoundEngine_unk` de
  PacketPack, Sound3dListener, SoundManager, BootTask, GameVoice, Event/Manager) es un refactor de nombres en una unidad
  enlazada: queda para una sesion propia. El port sigue con su vista de `SoundEngine_unk` (atn/SoundCore.h).

## 2.4 Tabla de personajes (f1e72a8)

- `CharaIdEntry_unk` en `include/db/CharaIdTable_unk.h` (antes local de db/CharaIdTable_unk.cpp; `m2C` -> `mIcon`) con
  `fn_8020924C(int)`. Usada por ShopBuySell (antes `ShopChara_unk`; `mName` -> `mShortName` 0x08), NewCampPartsStatus
  (antes `NewCampStatusName_unk`; `m14` es `const char*` y `fn_8020A81C(const char* name)`, antes `(int)`),
  CharaParam_unk y MenuCommonTask (`fn_802091E8` y `fn_804A4AEC` con `CharaIdEntry_unk*`; la otra tabla de `MenuItem_unk`,
  la de `fn_80211480`, es `ItemParamEntry_unk` de db/ItemParam_unk.h: `mName` 0x08).
- Para el port: `CharaInfo_unk` de game/CharaTask.h (`CharaTask::m60`) es otra vista de la misma fila: `m08` es
  `const wchar_t* mShortName` (alli `const char*`), `m14` es `const char*` (alli `int`, se pasa a `fn_8020A81C`), `m04` es
  `mName`. Commit siguiente: `CharaInfo_unk::m14` pasa a `const char*` y `fn_8020A81C(const char* name)` en CharaTask y GameEventScript
  (GameTask quita su cast): en 64 bits el `int` truncaba el puntero. `m08` sigue `const char*` (sin cambiar).
- GameOverTask y NewCampPartsTop incluyen `CampSaveLoad.h` (commit d66a8b7): fuera sus `MenuLoadTask` locales y la
  vista de solo metodos de `atn::D2A_Task` de NewCampPartsTop (`mLayout` -> `&mData.m10`). Hizo falta sacar las funciones C
  de D2A_Task de `atn/D2A_Task.h` (ver MenuD2A_unk) y que CampSaveLoad.h no incluya `D2A_TaskApi_unk.h`. Para el port:
  `NewCampPartsPoint_unk` (game/NewCampPartsBase.h) = `atn::D2A_DrawRect` (x, y, w, h, z) y el primer argumento de
  `fn_801F4E8C` es un `atn::D2A_Task*` (las NewCampParts* siguen con su declaracion local).

## 2.5 `Settings_unk` (lbl_8087F0A8) -> `GameConfig_unk` (commit 09a7051)

- 31 unidades: la vista local de `lbl_8087F0A8` es `GameConfig_unk` (game/GameConfig_unk.h); miembros por desplazamiento:
  - (solo el tipo): MapPartsModel, SetConfig, AttractHatePoint
  - `m18` -> `mDrawMap`: AutomaticDoor, Chain, ChickenManager, Courtyard, DoorSeal, FloatingBox, FragileObject, LandMine, LemonBucket, MotionObject, OpenBoard, PigeonManager, SparkCube, SwingObject, WaitObject, WaterLevel
  - `m254` -> `mHateAttackedDelayFrame`, `m304` -> `mOverdriveExplosion`, `m3CC` -> `mParamIndex`: ControlAttractHateCharge
  - `m30` -> `m030`: EnemySeaLizerd
  - `m30` -> `m030`, `mDrawDebug` -> `mDrawCharaColli`: Em403WarLion
  - `m314` -> `mTacViewWind`: ControlCharge
  - `m40` -> `m040`, `m2BC` -> `mMagicCutFrame`, `m2C4` -> `mFlickMagic`: ControlChant
  - `m74` -> `mDrawGimmickName`: CharaObj, CharaPathMove
  - `m80` -> `m080`: em005_doppel_ganger
  - `mDrawDebug` -> `mDrawCharaColli`: CopyRobotTask, Np131LivingArmor
  - `mDrawGimmickState` -> `mDrawGimmickName`: PlayEffect
  - `mFps` -> `m030`: Banana
- Quedan 16: Event/Path, ControlChaseCaught, ControlStickByQuark, CharaStatus, CharaTask, ChaserGuard, ChaserNpc,
  Em315Coffin, GameCamera_unk, GameUI, PlayerInput_unk, TutorialTask, ReinforcePoint (metodos o structs propios en la vista:
  `fn_80116BAC`, `StatusRate_unk`, `ChaseParam_unk`, campos dentro de `m53C`...), ControlCdnStone y Trap (su `Input_unk`
  local choca con el de GameConfig_unk.h) y GimmickCommon (con GameConfig_unk.h fn_803EDB18 cambia de registros).
- Commit c424b8f: ControlChaseCaught (`m520` -> `m4DC.m44`), ChaserNpc (`m53C..m578` -> `m53C.m00..m3C`,
  struct GameConfig53C_unk), ControlCdnStone y Trap (su `Input_unk` local fuera: es el de GameConfig_unk.h).

## 2.5 `EffectMgr_unk::mB8` -> `atn::EffectManager::mSystem.mLayer` (commits c424b8f y 4d47070)

- `lbl_8087F3C0` es `atn::EffectManager`; `mB8` = `mSystem.mLayer` (EffectSystem_unk 0x6C, capa de dibujo de los efectos
  nuevos). Fuera la vista local `EffectMgr_unk` en 34 unidades: ActionEffectPlay, Arms, AttractHatePoint, AutomaticDoor, CharaTask, CopyRobotTask, DoorSeal, DropItem, Em101ElderZesyaSecond, Em201ZanGurgFinal, EnemyTasha, EnemyTrista, FallingTower, Fireworks, FragileObject, GameUI, GimmickStateCommand, HealPoint, LandMine, LastCocoonEgg, LastCocoonImago, LastCocoonQuadruped, LottaSummon, MapPartsEfp, MotionObject, PlayEffect, RainDrops, SavePoint, SkeltonsMirror, SparkCube, SwingObject, TreasureBox, WaitObject, WaterLevel.
- `atn/EffectManager.h` ya no declara `fn_80232B7C`, `fn_8023A680`, `fn_8023A02C` ni `fn_80239DAC`: pasan a
  `include/atn/EffectApi_unk.h` (incluida por las 11 unidades que las usaban con la firma de la cabecera). Las demas
  unidades las declaran con `void* owner` (y otras listas de argumentos en fn_8023A680). Para el port: el "owner" es un
  puntero; en la cabecera es `unsigned int` (trunca en 64 bits): mejor `void*`/`uintptr_t` en EffectApi_unk.h.
- Quedan con vista propia: EnemyQuarkStand (declara su propia `atn::EffectManager` parcial) y Em204Mulue(+Mp),
  MotionCommand_unk, NewCampPartsMapSelect, SkillEffect, Em209SmallGolem, NewCampPartsGmk, em005_doppel_ganger (su
  `EffectMgr_unk` tiene otra forma).

## 2.6 Bugs de 64 bits

- Commit e0b50cc: `EffectSlot_unk::m0C` (include/atn/EffectSystem_unk.h) es `atn::Texture*` (la segunda
  textura del par, `&EffectTexturePair_unk::m30`, o 0); antes `int` con `(int)&...` (truncaba en 64 bits). El port puede quitar
  su arreglo local.
- Fuera (como dice el prompt): el color por valor de GXSetFog y `HitInfo_unk::m48`.

## 2.5 `ActionOwner_unk` = `Event::Section` (commit 60ffa1a)

- `Event::Action::fn_805381A0()` devuelve `Section*` (include/Event/Action.h); fuera el struct `ActionOwner_unk` de
  Event/Behavior.h (`m10` = `Section::m10`, id del actor). Unidades: Node, BehaviorFollow, BehaviorGesture, BehaviorMoveTo,
  ActionEffectPlay, ActionActorPosition (`fn_80224618(Section*)`), ActionCameraMode (`GetCamera_unk(Section*)`).

## 2.5 Structs de fichero en cabeceras (commit 9cac0ae)

- `SceneCameraData_unk` y `SceneHeader_unk` (antes locales de atn/LH_Scene.cpp) en `include/atn/SceneData_unk.h`;
  `MotionNode_unk` y `MotionData_unk` (antes locales de atn/Motion_unk.cpp) en `include/atn/MotionData_unk.h`. Mismas
  definiciones: el port solo tiene que cambiar ahi los punteros de 32 bits (`PTR32()`) y quitar sus `#if TARGET_PC`.
- No llevados: `GimmickLocTable_unk` (MapPartsGimmickLoc) y `BuildingFile_unk` (MapPartsLocatorSet) son el mismo fichero
  pero la primera vista empieza 0x10 bytes despues (cabecera) y tiene un getter debil con su nombre (0x80498E7C);
  vistas de oclusion (`atn/ModelOcclusion_unk.h`): no buscadas.
- `PathData_unk` (game/PathData_unk.cpp) = `Event::Path` (commit 27a692d): fuera sus copias locales de `PathPoint_unk`,
  `PathSpline_unk` y `PathVec_unk`; `mOwner` -> `CommonData::m04`, `mPoints` es el `std::vector<PathPoint_unk>` de Event/Path.h,
  `GetPoint_unk` pasa a inline libre; `Event::PathSpline_unk::Clear_unk()` declarado en Event/PathSpline.h y definido (como
  `Build_unk`) en la unidad. fn_80547EA0/fn_8054856C/fn_8054875C con el mismo porcentaje.

## 2.5 `MenuD2A_unk` = `atn::D2A_Task` (commit 5b0c35d)

- `include/game/MenuCommonTask.h`: `typedef atn::D2A_Task MenuD2A_unk;` con declaracion adelantada (antes un struct con
  relleno). MenuCommonTask.cpp y GameOverTask.cpp incluyen atn/D2A_Task.h: `m58` (layout) -> `&mData.m10`, `mCallback`
  (0xE4) -> `mData.m9C` (callback `(D2A_Common*, D2A_DrawColor*, void*)`), `m108` -> `mBlend`, `mLastFrame` -> `mData.mEndFrame`.
  MenuCommonTask::vf14 prueba el final con una inline no const `IsAnimeEnd_unk` (la `IsEnd_unk() const` de D2A_Task
  relee el frame alli).
- Las funciones C de D2A_Task (`fn_801F3FF8`, `fn_801F4484`, `fn_801F465C`, `fn_801F4728`, `fn_801F4E8C`) pasan de
  atn/D2A_Task.h a `include/atn/D2A_TaskApi_unk.h` (incluida justo detras por los 28 que incluian D2A_Task.h).
- Quedan: `MP::MpD2A_unk` (include/MP/D2AMpMenu_unk.h, ~10 unidades MP, 357 usos de `mLayout`) y las vistas
  `atn::D2A_Task` de solo metodos de MP/D2AMpMenu_unk.h, SelectMember y ShopUpGrade.
- atn/PostFxMotionBlur: la vista `MotionBlurParams_unk` de `lbl_8087EFA8` -> `RenderSetting_unk::m240`
  (`MotionBlurParam_unk`: mEnabled, mDebug, mFrozen, mScale, mShift, mStartBlend, mEndBlend, mStartBlendSpeed, mEndBlendSpeed).
- Commit 243e05b: `MP::MpD2A_unk` = `atn::D2A_Task` (include/MP/D2AMpMenu_unk.h incluye atn/D2A_Task.h; fuera
  la vista de solo metodos y el struct con relleno). `mLayout` -> `&mData.m10` (~380 usos), `mLastFrame` -> `mData.mEndFrame`,
  `m108` -> `mBlend`. Unidades: MP/D2AMpMenuCockpit MP/D2ACommonDialog MP/D2AMpMenu MP/D2AMpMenuMessageList MP/D2AMpMenuSetMessage MP/D2AMpMenuFriend MP/D2AMpMenuFriendRoom MP/D2AMpMenuLobby MP/D2AMpMenuSetRule MP/D2AMpMenuResult game/CosEditPalette_unk Event/Dialog atn/SoftKeyboardU . Quedan las vistas propias de MP/Popup
  (`PopupTask_unk`), game/PopupTask.h, Em208Queen y Em001Necromancer (`GetSlash_unk`), SelectMember y ShopUpGrade.
- Commit 072aa2e: gmk/ReinforcePoint: `Settings_unk::mE4[mD4].m00` -> `GameConfig_unk::mDifficulty[mCurrentLevel].mLevelAddBoss`.
- Para el port (no llevado; mapas medidos): CharaStatus `StatusRate_unk mD8[4]` = `mDifficulty[4]` (`m00..m1C` -> `mLevelMul,
  mLevelAdd, mLevelMulBoss, mLevelAddBoss, mHPMul, mHPAdd, mAttack, mAttackAdd`) y `m5A0` cae en `mTitle` (0x584); ChaserGuard
  `ChaseParam_unk mChase` = `GameConfig4DC_unk m4DC` (`m4DC..m518` -> `m00..m3C`); PlayerInput_unk `SettingsOpt_unk m244`
  cae dentro de `mSceneDebug` (`m244.m34/m38` = `m278/m27C`) y su `DialogConfig_unk` local (de SetConfig) choca de nombre
  con el de GameConfig_unk.h; CharaTask y Em315Coffin: `fn_8013C434` devuelve `&m454` (bloque Surround 0x454-0x46C, sin struct
  en GameConfig_unk: hace falta uno para tiparlo).
- Commit 1ef554e: game/Bullet (`HomingField_unk` de game/Bullet.h fuera: `m5590` = `GameTask::m5590` ReplayBuffer*)
  y NewCampPartsOption (vista vacia) -> `GameTask*`, sin casts.

## 2.5 Vistas de `lbl_8087EFA8` -> `RenderSetting_unk` (commit a8e9425)

- 51 unidades: la vista local (`GameState_unk`, `FrameTime_unk`, `Timer_unk`, `Params_unk`...) es `RenderSetting_unk`
  (atn/RenderSetting_unk.h); miembros simples por desplazamiento (casi todos `m3A4`, velocidad de frame). Los accesores de las
  vistas (`GetTime_unk`/`GetDelta_unk` = m3A4) son `RenderSetting_unk::GetFrameSpeed_unk() const` (declarado en la cabecera,
  definido inline en AiTarget, CharaStatus, Em001Necromancer, Em003LesserDaemon, MotionCommand_unk, ReplayBuffer).
- Para el port: estas unidades ya no tienen relleno con desplazamientos de la Wii hacia RenderSetting_unk (su guia: la clase real
  con `PTR32` en sus dos punteros). Unidades: atn/LayerReflect atn/PostFxColorMatrix atn/PostFxRain chr/ControlAttackBase chr/ControlAttractHateCharge chr/ControlCdnStone chr/ControlChant chr/ControlCharge chr/ControlCliffhang chr/ControlDamage chr/ControlDive chr/ControlDropDown chr/ControlMountle chr/ControlPushedDown chr/ControlTurnQuadruped chr/ControlWallUp game/AiTarget game/Bullet game/CharaFaceControl game/CharaStatus game/Em001Necromancer game/Em003LesserDaemon game/Em201ZanGurgFinal game/Em203GreaterDaemon game/Em203GreaterDaemonRedMp game/Em205Ogre game/Em206CocoonMp game/Em306GurgBeast game/Em309Ghoul game/Em316LaserSphere game/Em316LastLaser game/Em401_01_WhiteLeoPard game/Em403WarLion game/EnemyFieldMagician game/EnemyLizerdBlade game/EnemyQuarkFirst game/EnemyQuarkStand game/EnemyTasha game/EnemyTrista game/GimmickStateCommand game/HomingBullet game/LastCocoonImago game/LastCocoonQuadruped game/MotionCommand_unk game/NewStaffRoll game/QuadrupedTask game/ReplayBuffer game/SetConfig game/TownTask gmk/ArenaFloor gmk/SparkCube 
- Quedan ~24 vistas con sub-structs (Bloom/Dof/Gamma/GodRay/ColorMatrix/Damage, `m20C[]`, `mFog`, `mSlow`, `mCamera`...).

## 2.5 Resto de vistas de `lbl_8087F0A8` -> `GameConfig_unk` (commit e51a120)

- 47 unidades con vistas de otro nombre (`AiSettings_unk`, `BatDebug_unk`, `BeastSettings_unk`, `ChaseConfig_unk`...): miembros
  simples por desplazamiento; los casts `((Input_unk*)lbl_8087F0A8->mPad)` y `->mInput` son `&lbl_8087F0A8->mInput` (el
  `Input_unk` local de Dialog/Em201ZanGurgFinal/EnemyQuarkStand/MP... es el de GameConfig_unk.h). Unidades:
  AI/AiCommon_unk AI/AiUtil_unk MP/D2AMpMenuFriend MP/D2AMpMenuMessageList MP/D2AMpMenuSetMessage atn/SequenceUnitParam1_const_int chr/ControlCannonLift chr/ControlDive chr/ControlMountle chr/ControlTurnQuadruped chr/ControlWallUp db/PlayerParam_unk db/TutorialTable_unk game/Bullet game/Em201ZanGurgFinal game/Em202SpiderBase game/Em206EggMonster game/Em208Queen game/Em306GurgBeast game/Em308InvertCrab game/Em308ParmCrab game/Em316LaserSphere game/Em316LastLaser game/Em401_01_WhiteLeoPard game/EnemyQuarkSecond game/EnemyQuarkStand game/EnemyTask game/EnvSeBase game/GameSaveBackground game/GameSaveLoadBackgroundCommon game/GameSaveLoadCommon game/GameSystem_unk game/LogMessageTask game/QuadrupedTask game/ShopIdentify game/ShopUpGrade game/TownTask gmk/Bat gmk/Brazier gmk/BulletPoint gmk/Cannonball gmk/DropItem gmk/Fireworks gmk/FlyObjManager gmk/Leaf gmk/Lemon gmk/OnOff gmk/SavePoint 
- No llevados: GameBgm (con GameConfig_unk.h fn_8037D4C0 cambia de registros), BootTask (`m1B4` = `mStrap`, ConfigString_unk),
  Event/Dialog y MenuCommonTask (`fn_80124C6C` devuelve un UV por valor con otro tipo), y las vistas con sub-structs.

## 2.5 Vistas de `lbl_8087EEE0` -> `atn::GraphicsDevice_unk` (commit 818dfde)

- 16 unidades: `RenderState_unk`, `RenderMgr_unk`, `DialogRender_unk`... -> `atn::GraphicsDevice_unk` (atn/GraphicsDevice.h):
  `mWidth`, `mHeight`, `mWide` (`m44`), `mClearColor`. Unidades: Event/Dialog MP/D2AMpMenuCockpit atn/D3Anime atn/D3AnimeInstance atn/FadeCommon atn/Geometry_unk atn/LayerOcclusionCPU atn/LayerReflect atn/Loader atn/NandManager atn/RenderSetting_unk atn/WPad_unk game/CharaTask game/MenuCommonTask game/NewStaffRoll game/SystemErrorMessage_unk 
- No llevados: DrawHelper, Layer2DBlur, Layer3D, LayerShadow, LensFlare, TextureComposite (sus declaraciones locales de
  `fn_800763FC`, `fn_80071D68`, `fn_80071D74`, `fn_800763C0` chocan con las de atn/GraphicsDevice.h), HomeButtonMenu y StrapTask
  (`mFbWidth/mFbHeight` de 16 bits en 0xE/0x10), VideoTHP (`m10`), LayerPostFx (`atn::Vec2f m58`).

## 2.5 Otras vistas de globales (commit 9c00cc9)

- `lbl_8087F490` -> `GameUI`: D2AMpMenuCockpit (`m255C`), CrossbowMenuTask y AutomaticDoor (`m10FC/m1100`), Trap (`m728`).
- `lbl_8087F540` -> `Event::Manager`: EnvSeBase, GameVoice, RouteWalker_unk, SongManager, main.
- `lbl_8087F098` -> `ChaseManager`: ControlChaseCaught, CharaTask, CosEditBase, GameUI, LemonBucket, SavePoint, SystemErrorMessage_unk.
- `lbl_8087F628` (`atn::net::Manager`, 11 vistas) y `lbl_8087F610`: sus vistas tienen sub-structs; sin llevar.
- Commit 956b4da: `lbl_8087F8A0` -> `PlayerManager` (`mHead` del `CharaManager<PlayerTask>` base) en CharaTask,
  Em202Spider, Em202_01_BSpider, Em209SmallGolem, Em308InvertCrab, Em308ParmCrab, Em403WarLion, EnemyJill, Np131LivingArmor,
  TownTask; `lbl_8087F018` -> `WPad_unk` (SoftKeyboardBase_wchar_t_0) y `lbl_8087EFE8` -> `atn::SoundManager` (GameChoiceWindow).

## Para el port: vistas encontradas y no llevadas

- `CharaListNode_unk` y similares (`{ unsigned char m0000[0x14AC]; CharaTask* mNext; }`): leen el siguiente de las listas Convertidos a `static_cast<PlayerTask*>(c)->mNext` en Em403WarLion, Em302Kraken y Em202MothersSpider (todas sus llamadas recorren `lbl_8087F8A0`); el resto mezcla listas de jugadores y enemigos.
  de `CharaManager<T>` (`PlayerTask::mNext`/`EnemyTask::mNext`, 0x14AC en la Wii) desde un `CharaTask*`. En PC el
  desplazamiento es otro: hay que leer `static_cast<PlayerTask*>(chara)->mNext` (o `EnemyTask`) segun la lista. Unidades:
  game/AiTarget game/BattleTask game/CharaTask game/Em002_01BlackSkelton game/Em202ForestSpider game/Em202MothersSpider game/Em202Spider game/Em202_01_BSpider game/Em301LShade game/Em302Kraken game/Em302KrakenMp game/Em403WarLion game/EnemyQuarkSecond gmk/Brazier gmk/BulletPoint gmk/Cannonball gmk/FlamingArrow gmk/FloatingBox gmk/FragileObject gmk/Leaf gmk/LottaSummon gmk/MotionObject gmk/OpenBoard 
- Vistas grandes de una sola variable de otros globales (sin cabecera real en la decomp): `lbl_8087EE98` (colisiones),
  `lbl_8087F610` (sesion MP, `MP::Manager2` local de PacketPack), `lbl_8087F138` (`atn::D2A_Manager`), `lbl_8087F1E4`
  (`TextTable_unk`: sus vistas leen `const wchar_t* mText[]` de 4 bytes, la tabla real son `ptr_string_base<wchar_t, 0>` de 8).
- `DebugMessages_unk` (`lbl_8087EEB8`): 17 copias locales sustituidas por `atn/DebugMessages_unk.h` (ahora con `fn_800698DC` y `fn_80069E50`); la disposicion completa sigue en `atn/FileUtil_unk.cpp` (para el port: mover alli la clase a la cabecera si hace falta `sizeof`).
- `lbl_8087EEE0` (2a pasada): DrawHelper, Layer2DBlur, LensFlare, TextureComposite, Layer3D, LayerShadow (sin sus `extern "C"` locales de `fn_800763FC`/`fn_800763C0`/`fn_80071D68`/`fn_80071D74`; se pasa `proj.m`), HomeButtonMenu y StrapTask (`mRMode.viWidth/viHeight`), VideoTHP (`mRMode.viHeight`, `mWide`). Queda LayerPostFx (`atn::Vec2f m58` = `mOffsetX/Y`).
- `lbl_8087F3C0` (2a pasada): 12 vistas mas a `atn::EffectManager`: Bullet, Brazier, OnOff, RandomItem, WindyItem (`mB8` = `mSystem.mLayer`), Em204Mulue(+Mp), MotionCommand_unk (`mC4`/`mC8` = `mSystem.m78`/`m7C`), Em209SmallGolem, NewCampPartsMapSelect (`mBC` = `mSystem.m70`), SkillEffect (`mD0` = `mSystem.mReplaying`), em005_doppel_ganger (`mEC` = `mFlags`). Quedan EnemyQuarkStand (define su propia `atn::EffectManager` parcial) y NewCampPartsGmk (`SetBC_unk` inline local).
- `GameConfig_unk::m4DC`/`m53C` (`GameConfig4DC_unk`/`GameConfig53C_unk`): campos con los nombres de las claves CSV de db/ChaseParam_unk (`visChaseDistance`... `chPlayerHit`); fuera la vista `ChaseConfig_unk` de db/ChaseParam_unk y `ChaseParam_unk`/`Settings_unk` de ChaserGuard (`SwingHead_unk(const GameConfig4DC_unk*)`). Campos sin clave: `m4DC.m00` (dibujo de depuracion), `m50`-`m5C`, `m53C.m00`, `m30`, `m34`.
- `lbl_8087EFE8` (2a pasada): Event/Path (`SoundMgr_unk::SetListener_unk` -> `atn::SoundManager::SetListener_unk`, copia debil 0x80116E74, declarada en atn/SoundManager.h y definida inline en la unidad; renombrado en symbols.txt) y GameScene (`m2984` -> `mListener`).
- `lbl_8087F8A0` (2a pasada): GimmickStateCommand (`m48->m528` -> `mHead->mPos`), SongManager (`m48` -> `mHead`, mPos/mRot), LemonBucket (`mRotY` -> `mRot.y`) con `CharaManager<PlayerTask>`/`PlayerTask`. Queda CrossbowMenuTask (`m5C->m11C`, `mAA4`, `mAAC` sin nombre en CharaTask.h).
- `lbl_8087F9F8`: PlayerInput_unk con `TacView` (`mA70`). Queda ControlCharge (`TacCursor_unk` local con ctor = `TacViewAim_unk`, ctor inline en TacView.cpp).
- `lbl_8087F9E8`: Em203GreaterDaemonRed(+Mp): `RedSkillMgr_unk`/`RedSkill_unk` -> `SkillEffectManager::mBullets[32]`/`SkillEffect` (`m08->m48` -> `m08->mType`).
- Para el port (no llevado; bug de 64 bits): `ResManager` de game/ResHandle.h (y `RsDebug_unk`/`DebugMenu_unk` de RenderSetting_unk/VideoTHP) es una vista de `atn::Mindows_unk` (atn/MindowsCommon.h: `void* mOwner` en 0, `mRoot` en 0x10). En 64 bits `m04` y `m10` se mueven: el port debe usar `Mindows_unk::m04`/`mRoot`. `fn_8008937C` es AddFolder (`MindowsFolder*`, nombre) y `fn_800897D8` quita la carpeta: ResHandle es la carpeta de depuracion de un objeto, no un recurso. No se cambio aqui porque ~40 unidades declaran `fn_8008937C(void*, const char*)` en local y chocarian con la declaracion de MindowsCommon.h.

## 2.5 `TextTable_unk` (lbl_8087F1E4) en cabecera (commit d51cb4e)

- `include/atn/TextTable_unk.h`: la clase real de atn/ptr_string_base_wchar_t_0.cpp (`atn::ptr_string_base<wchar_t, 0>
  mStrings[0x203]`, entradas de 8 bytes {vptr, texto}) con dos accesores inline:
  - `GetPair_unk(id)` = `mStrings[id].mPtr` (o el texto vacio `lbl_808813D0`): las vistas `{int, const wchar_t*}`.
  - `Get_unk(id)` lee la tabla como palabras de 32 bits (`reinterpret_cast<const wchar_t* const*>(mStrings)[id]`):
    las vistas `const wchar_t* mText[1]`; la palabra `2 * i + 1` es el texto de la entrada `i`. **Para el port**: en 64
    bits este accesor debe ser `mStrings[id >> 1].mPtr` (un solo sitio que cambiar).
- 30 unidades sin su vista local: CampSaveLoad CosEditPalette_unk CosEditProtector Em001Necromancer Em101ElderZesya
  Em101ElderZesyaSecond Em101LittleZesya Em202ForestSpider Em203GreaterDaemon(+Red/RedMp) Em204Mulue Em208Queen
  Em301LShade Em302Kraken(+Mp) Em309Ghoul GameOverTask LastCocoonChrysalis MenuCommonTask NewCampPartsHint
  NewCampPartsItem NewCampPartsTop PopupTask SelectMember ShopBase (con un `GetText_unk` local escrito con `if`)
  ShopBuySell ShopColor ShopIdentify ShopUpGrade TacView.
- Quedan con vista: CharaTask (con la cabecera cambian los registros de fn_80149A30 y el marco de fn_8014C540), StrapTask
  (vf14 92,87 -> 92,66), MP/MpCampMsgTable_unk.h (con `typedef TextTable_unk` baja atn/PacketPack fn_804F50DC 96,39 ->
  96,36), BattleTask y CosEditItemList_unk (leen pares con cast), GameUI (`GetEntry_unk`, copia debil 0x803D7084),
  ItemManager_unk (union), MenuCommon_unk (`const`), SoftKeyboardU, ControlCdnStone (`mDC`), MP/D2AMpMenuSetMessage
  (`MP::MpMsgTable_unk` de lbl_8087F86C, otra tabla).
- `lbl_8087F0A8` (3a pasada): CharaStatus y GameTask (`StatusRate_unk mD8[4]`/`mD4` -> `mDifficulty[mCurrentLevel]` con `mLevelMul`... `mAttackAdd`; `m5A0` -> `mTitle.mEasyModeType`; en GameTask `m184` mDrawGameInfo, `m280` mAddExpImmediately, `m290` mAlwaysBirdview, `m2BC` mMagicCutFrame, `m320` mTouchHelp, `m3DC` mBattleEndReset), GameOverTask (`m5A4` -> `mTitle.mLogoFadeStart`, "GameOver/Logo Fade Start" del menu), MapPartsLocatorSet (`m5B0`/`m5B4` -> `mTitle.m2C`/`m30`, comentados en la cabecera). Quedan: ChaseManager (`atn::Vector3 m52C` = `m4DC.m50..m58` floats), CharaTask y TutorialTask (accesor `fn_80116BAC`), PlayerInput_unk (`SettingsOpt_unk`), chr/ControlCaught/JumpAt/StickByQuark, Event/Dialog, MenuCommonTask (su `Input_unk` local choca), BootTask, GameBgm/GimmickCommon (registros).
- chr/ControlCaught, ControlJumpAt, ControlStickByQuark: fuera `Unk_8087F0A8` e `Input_unk` locales (`GameConfig_unk::mInput`, `mFrameRate`/`mFps` -> `m030`).
- MenuCommonTask y Event/Dialog: fuera `MenuSettings_unk`/`DialogSettings_unk` e `Input_unk` locales: `lbl_8087F0A8->mInput.fn_80124C6C` (devuelve `ButtonUv_unk` = `atn::Vector4_unk`; `DialogUv_unk` fuera).
- `lbl_8087EFA8` (3a pasada): PostFxBloom (`m58..mC8` -> `m054.mDebug..mScale`), PostFxGamma y PostFxGodRay (copias locales de `GammaParams_unk`/`GodRayParam_unk` fuera; `m3DC`/`m264`), PostFxDamage (`m374.mDebug/mColor/mAngle/mScale/mZoom`; `DamageColor_unk` queda como copia local con ctor desde `atn::Color`), Em301LShade y Em203GreaterDaemonRed (`m240` = `MotionBlurParam_unk`: la copia se escribe `MotionBlurParam_unk blur = lbl->m240; blur.mEnabled = x; lbl->m240 = blur;` como el original; Em203GreaterDaemonRed fn_8029A0A8 81,60 -> 94,13). No llevado: Em302Kraken (`m2AC` = `RainParam_unk`, pero su ctor por defecto cambia vf10 y fn_802DC618).
- MenuCommonTask (regla 10): `fn_804A4264`, `fn_804A4280`, `fn_804A5B9C`, `fn_804A5CD8`, `fn_804A5D34`, `fn_804A65A0` pasan de `extern "C"` con `MenuCommonTask* menu` a miembros (declarados en game/MenuCommonTask.h sin nombres de parametro; renombrados en symbols.txt). Llamadores: CosEditProtector, NewCampPartsItem, GameOverTask, MP/D2AMpMenuLobby, D2AMpMenuSetRule, D2AMpMenu. Despues tambien `fn_804A4300`, `fn_804A62C8` y `fn_804A7DD4` (NewCampPartsTop, ShopIdentify y NewCampPartsOption incluyen ya MenuCommonTask.h en vez de su clase local). Y `fn_804A3C24` (12 llamadores; ShopColor y NewCampPartsStatus incluyen MenuCommonTask.h). Con `ShopD2A_unk` = `D2A_Task`, tambien `fn_804A5824`. Quedan 3 (`fn_804A53D4`, `fn_804A55FC`, `fn_804A5E40`; este ultimo con el desplazamiento como `atn::Vector3` por valor, `atn::Vector3*` o `const float*` segun la unidad): sus llamadores declaran el rectangulo con 5 vistas distintas de `atn::D2A_DrawRect` (`MpRect_unk`, `NewCampPartsPoint_unk`, `ShopPoint_unk`, `SelectMemberPoint_unk`, `D2APoint_unk`) o no incluyen MenuCommonTask.h.
- `ShopD2A_unk` (game/ShopBase.h) = `typedef atn::D2A_Task` (antes un struct con relleno y bitfields `unsigned`): `mLayout` -> `&mData.m10` (133 usos), `mLastFrame` -> `mData.mEndFrame`; fuera las clases `atn::D2A_Task` de solo metodos de ShopBase, ShopIdentify, ShopUpGrade y ShopBuySell, y `CosEditTaskLayout_unk`/`CosEditD2A_unk` de CosEditProtector. Renombrados en symbols.txt `ShopBase::fn_805846C0`/`fn_80584750` (`P11ShopD2A_unk` -> `PQ23atn8D2A_Task`). Para el port: los bitfields de D2A_Task son `int x : 1` (orden distinto en PC, ya conocido).
- `ShopPoint_unk` = `typedef atn::D2A_DrawRect` en game/ShopBase.h (fuera las 5 copias locales de las tiendas, mismo ctor a cero). `MP::MpRect_unk` es igual pero sale en nombres mangled y `@STRING@` de symbols.txt (`D2AMpMenuRankMark::fn_804B49C0`, `D2AMpMenuLobby::Player_unk::SetPos_unk`): no tocado. `NewCampPartsPoint_unk`, `SelectMemberPoint_unk` y `D2APoint_unk` (MenuCommonTask, CosEditBase, Loader) tienen ctor vacio: cambiar a `D2A_DrawRect` meteria ceros.
- `lbl_8087F138` = `atn::D2A_Manager` (nueva `include/atn/D2A_Manager.h`, clase sacada de atn/D2A_Manager.cpp; `m1AC` -> `ID2A_TaskInstance* mInstance`) y `D2A_MessageTable_unk` real (nueva `include/atn/D2A_MessageTable_unk.h` con `D2A_TextEntry_unk`/`D2A_TextEntries_unk`; antes D2A_Manager.cpp tenia una vista de 0x18 bytes). Fuera las vistas de D2A_Task (`m168` -> `&mTexts`, `mCurrent`), D2A_TaskInstanceBase_64_8 (`mInstance`), MenuCommonTask y NewCampPartsBase (`m1A8`/`m1AC` -> `mCurrent`/`mInstance`) y MenuCommon_unk (`&mTexts`).
- `lbl_8087F018` (2a pasada): GameTask, HomeButtonMenu, StrapTask (`m40E0` -> `m40E0[0]`) y main (`m40D4/D8/DC` -> `m40D0[1..3]`) con `WPad_unk`. Quedan atn/Pad_unk (`m3C00[p].mBit0/mBit1` como bitfields frente a `mFlags[4]`) y SystemErrorMessage_unk (`IsNoPad_unk`/`GetExt_unk` locales: `mConnected[0]`, `mStatus[0][0].mDevType`).
- `lbl_8087F490` (2a pasada): EnemyJill (`JillArena_unk` -> `GameUI`: `m263C`/`m2640` son `GameMessageWindow*`/`GameTextMessageWindow*` y `m50` es su `mState`). No llevado: Em308InvertCrab (`CrabTutorial_unk` = `GameUIElem1C_unk`, pero el ctor de -1 de la cabecera cambia vf10; igual `DropTutorial_unk` de gmk/DropItem), MenuCommonTask (`MenuColors_unk` = `m288C`/`m289C` `GameUIColor_unk`, con un `MenuColor_unk` local que se construye desde `float[4]`), TownTpcLoader (`Clear_unk` local), AttractHatePoint y LottaSummon (`m7C0` = `GameUIElem1C_unk`).
- `EffectMgr_unk` opaco (solo `struct EffectMgr_unk;` y punteros, 48 unidades): ahora `atn::EffectManager` (declaracion adelantada) en `lbl_8087F3C0`, `fn_800F7FA0`, `fn_8023A8B4`, `fn_80239DAC`, `fn_80239D58`, `fn_8023A680`, `fn_80276AE4`, `fn_8026607C`... (nombres C sin cambio; `Em101ElderZesya::PlayBarrierM_unk` en su cabecera). Quedan las dos vistas con miembros (EnemyQuarkStand, NewCampPartsGmk).
- `Unk_8087F518` opaco (35 unidades: `lbl_8087F518`, `fn_8046DC5C`/`fn_8046DD20`/`fn_8046EED8`) -> `atn::Loader` (declaracion adelantada; definido en atn/Loader.cpp). Las funciones siguen siendo `extern "C"` con el cargador como primer argumento (regla 10, sin tocar).
- Otros tipos opacos de globales: `GraphicsDevice_unk`/`Screen_unk`/`DrawRender_unk`/`RenderState_unk` (DialogHead, FinishCamera_unk, Event/Path, ModelDraw_unk, Rain) -> `atn::GraphicsDevice_unk`; `ChaseEffectMgr_unk`/`CannonEffectMgr_unk` -> `atn::EffectManager`; `D2A_Icons_unk` (D2A_Message) -> `atn::D2A_Manager`.
- Globales declarados `extern "C" void*` con su clase (declaracion adelantada) en 36 unidades: lbl_8087F430 `GameTask`, lbl_8087F518 `atn::Loader`, lbl_8087F9E8 `SkillEffectManager`, lbl_8087EEE0 `atn::GraphicsDevice_unk`, lbl_8087F0A8 `GameConfig_unk`, lbl_8087F540 `Event::Manager`, lbl_8087F138 `atn::D2A_Manager`. Siguen como `void*`: lbl_8087EE98 (colisiones, 87; la clase `ColliManager_unk` solo esta en atn/ColliBaseFunc_unk.cpp), lbl_8087EF70 (pad, 62), lbl_8087EEC8 (fuente, 35), lbl_8087F610 (MP, 17), lbl_8087EE90 (sonido, 12).
- Tambien `void*` -> clase: lbl_8087EEC8 `atn::Font_unk`, lbl_8087EF68 `atn::NandManager`, lbl_8087EF8C `atn::PostFxManager` (unas 40 unidades). lbl_8087EF70 sigue `void*` (su `Pad_unk` de atn/Pad_unk.cpp es un struct vacio).
- `tools/decomp/sigcheck.py` (nuevo): lista las funciones `extern "C"` declaradas con distinta secuencia de clases de argumento (int/puntero/float/double/struct) o distinto retorno no void entre unidades: en la Wii compilan igual, en x86-64 (Windows sobre todo) no. 162 -> 110 -> 98 en la sesion.
- Corregidas: `fn_800DC288` devuelve `float` (dos unidades decian `double`), `fn_800C31F4(name, int, int, float)` (dos lo tenian con el float delante), y las funciones del menu de depuracion de MindowsCommon.h (`fn_800874C8` AddInt, `fn_800875F8`, `fn_80087994` AddBool, `fn_80087AA0`, `fn_8008771C` AddFloat, `fn_80087858`, `fn_80087BB4`, `fn_80087E9C`, `fn_8008826C`, `fn_80088724`, `fn_80088AF4`, `fn_80088FAC`) en 97 unidades: el callback y su argumento son `void*` al final, detras de los floats (antes `int, int` delante de los floats en ~50 unidades). Las llamadas se reordenaron; el codigo de la Wii es el mismo.
- `fn_8023A8B4` (efecto EFP sobre un modelo): el cuarto argumento es un puntero (nodo; `void* node` en atn/EffectManager.cpp) y 98 unidades lo declaraban `int`: ahora `void* node` en 100 unidades (las llamadas pasan 0).
- `fn_8004ED34`/`fn_8004ECC0`/`fn_8004D388` (rayos/barridos de colision): el resultado y el objeto ignorado son punteros; 7 unidades los declaraban `int` (ControlAppear, ControlCliffhang, RouteWalker_unk, DamageCalc_unk, FinishCamera_unk, Rain, DropItem): ahora `void*`. No llevado: `fn_8006EF48` (ancho de texto) en Loader, NandManager y MenuCommonTask declara `(font, text, size, ...)` frente a `(font, size, text, ...)` de las otras 24: reordenar los argumentos de la llamada cambia la planificacion (Loader fn_8046ECDC 99,92 -> 96,77); el port tiene que corregir esas tres declaraciones a mano.
- Punteros declarados `int` en una sola unidad (las llamadas pasan 0): RouteWalker_unk `fn_80019E88`, DialogHead `fn_8007642C` (luz), Layer2DBlur `fn_800A981C`, RainDrops `fn_8023A8B4` (modelo), Em315Coffin y FlamingArrow `fn_80276AE4` (modelo), D2AMpMenuLobby `fn_804DE560`. No cambiados (pasan un entero de verdad, la firma sigue sin aclarar): D2AMpMenuSetRule `fn_804D824C`, MapPartsEfp `fn_8023A8B4` (tercer argumento `id`).
- `fn_800CB5B4` (volumen): la definicion (atn/SoundManager.cpp) es `(SoundRef*, float, int)`; 6 unidades la declaraban `(SoundRef*, int, float)`: corregidas con las llamadas reordenadas (mismo codigo). CharaTask la declara `(SoundRef*, int)` sin el float (el original no carga f1 ahi): sin tocar, para el port es un bug latente.
- Con la firma de la definicion como referencia (`sigcheck.py` marca con `*` el fichero que define), los argumentos que la definicion tiene como puntero y otras unidades declaraban `int` pasan a `void*` donde las llamadas pasan 0: `fn_804A53D4` (nombre del cursor, 8 unidades MP/SoftKeyboardU/SelectMember), `fn_8004D314` (Em202SpiderBase, Em308InvertCrab/ParmCrab, Brazier), `fn_80444020` (BattleResult, GameEventScript), `fn_8036F268` (GameEventScript, NewCampPartsMapSelect). Donde las llamadas pasan un entero de verdad (14 unidades) no se toco: queda en `docs/secuencial-20261119-sigcheck.txt`.
- Retornos puntero declarados `int` (solo se comparan con 0): atn/Model (`fn_80476194`, `fn_804761B0`), BattleTask/CharaTask/GameUI (`fn_804EB7C4`, `fn_804EB814`), NewCampPartsBase (`fn_8021946C`) -> `void*`. Siguen como `int` porque guardan el valor en un campo `int` (cambiarlos es cambiar esos campos): `fn_800EFD04` (EnemyQuarkStand, BulletPoint: `shot.m04`), `fn_8021AFF4`/`fn_8021AF50` (CharaStatus), y PacketPack.
- `BulletShot_unk::m04`/`m08` y `Bullet::m60`/`m64` (game/Bullet.h) eran `int` con punteros dentro (bug de 64 bits): ahora `atn::EffFile*`/`atn::EfpFile*`; fuera los casts `(int)` de BattleTask y Em204*; `fn_800EFD04`/`fn_800EFDA0` devuelven puntero tambien en BulletPoint, EnemyQuarkStand y PacketPack; `QuadShot_unk` de LastCocoonQuadruped con los mismos tipos.
- `lbl_8087F628` (`atn::net::Manager`): DamageCalc_unk sin su `MpState_unk` (`mDisabled` -> `mFlags.mB12`). Item, DropItem, ControlCannonLift y PPDATAU usan ya `GetMyAid_unk`/`GetServerAid_unk` de atn/net/Manager.h (sus `Get1_unk`/`Get2_unk` locales eran copias). Quedan las vistas MP con sub-structs (D2AMpMenuCockpit, FriendRoom, MessageList, SetMessage, SoftKeyboardU).
- ChaserGuard: `ChaseState_unk` -> `ChaseManager` (`m48` mState, `m58` mFrames, `m70` mCatcher, `m74` = `(int)mGuards.size()`: con `unsigned` cambia la conversion a float, `mF8` mCaught).
- `lbl_8087F518` = `atn::Loader` (atn/LoadManager.h) en GameSaveBackground (`m3EF4`) y main (`m3ED8`; su `fn_8046D7D8` local devolvia void).
- HomeButtonMenu: `HbmSaveLoad_unk` -> `atn::NandManager` (`m174` = `mIdle`).
- SystemErrorMessage_unk: `SysMovie_unk` -> `atn::VideoTHP` (lbl_8087F010, `m08`).
- `lbl_8087EE98` (colisiones): `ColliManager_unk*` (declaracion adelantada de la clase de atn/ColliBaseFunc_unk.cpp) en vez de `void*` en 75 unidades. Las funciones de consulta (`fn_8004ED34`...) siguen recibiendo `void*` en las unidades del juego.
- `lbl_8087EF70` (pad del juego, atn/Pad_unk.cpp): `Pad_unk*` (declaracion adelantada) en vez de `void*` en 56 unidades. No en MenuCommonTask (fn_804A4494 98,43 -> 98,08), NewCampPartsTop y CosEditPalette_unk (cambian registros: guardan el global en un `void* pad` local) ni PacketPack (no compila con el tipo).
- (Hecho despues, ver abajo) `Settings_unk` de CharaTask, Em315Coffin y TutorialTask con sus metodos debiles `fn_80116BAC` (`&mInput`), `fn_8013C41C` (`m418`), `fn_8013C434` (`&m454`) y `fn_8013C50C` (`m048`): hace falta un struct para el bloque "Surround" de GameConfig_unk (0x454-0x46C: `mSurround`, `mSurroundMode`, `mPlayerDamageRatio`, `mEnemyDamageRatio`, `mForceRatio`, `mBattleWaitRatio`, `mMoveSpeedRatio`) y declarar los cuatro metodos en GameConfig_unk (renombrando `fn_*__12Settings_unkFv` en symbols.txt).

## 2.5 y 2.6 despues del punto de control 5 (commits 9ad7f65 ... 082973f)

- GameConfig_unk: `GameConfigSurround_unk mSurround` (0x454) y `fn_80116BAC`/`fn_8013C41C`/`fn_8013C434`/`fn_8013C50C` miembros; fuera las vistas `Settings_unk` de CharaTask (con su `Block454_unk`), Em315Coffin, TutorialTask, Event/Path, GameCamera_unk y ControlStickByQuark. Queda GameUI (`GetFlags_unk` 0x803D6F60 sobre 0x470) y PlayerInput_unk (`fn_801231B0` sobre 0x244). APLICADO.
- GameBgm (`m15C`), GimmickCommon (`mDrawGimmickName`, `m07C`) y BootTask (`mStrap.mBuf`) leen `GameConfig_unk`. APLICADO.
- ChaseManager: la vista `ChaseSettings_unk` (0x524-0x538) se queda; `GameConfig4DC_unk::m50/m54/m58` como `atn::Vector3 mCameraOffset` baja el constructor de GameConfig_unk a 93 % (los tres floats se inicializan uno a uno). DESCARTADO. Para el port basta saber que 0x524/0x528 = `m4DC.mReleaseSwingCount`/`mCapturedTime` y 0x52C-0x538 = `m4DC.m50..m5C`.
- GameSoundControl: `lbl_8087F0A8` como `GameConfig_unk*` (`mInput`, `m113C`, `m1140`) en vez de `unsigned char*` + 0x48C y la vista `SaveSystem_unk`. APLICADO. Pendiente: su `Field_unk` ve `GameTask::m5620/m5624` como un stream (m48-m74), y `GameTask.h` los declara `atn::FadeCommon*`/`atn::FadeCapture*`: uno de los dos tipos esta mal (revisar en el port antes de usarlo).
- `lbl_8087F018` = `WPad_unk` tambien en SystemErrorMessage_unk (`GetExt_unk`/`IsNoPad_unk` pasan a `WPad_unk.h`: `mStatus[0][0].mDevType`, `mConnected[0] == 0`) y HomeButton_unk (`mStatus[i]` en vez de `+ i * 0xF00`). Fuera las vistas `SysInput_unk` y `unsigned char*`. APLICADO.
- `lbl_8087F490` = `GameUI` tambien en AttractHatePoint (`m7C0.m00..m18`) y LottaSummon (`GameUI::Request_unk(int)` y el constructor `GameUIElem1C_unk(int type)` nuevos en `GameUI.h`). APLICADO. Em308InvertCrab con `GameUIElem1C_unk` local cambia la pila de vf10: DESCARTADO (se queda `CrabTutorial_unk`, mismos campos). MenuCommonTask (`m288C`/`m289C` como `float[4]`) y TownTpcLoader (`Target_unk::Clear_unk`) siguen con vista.
- El constructor con parametro en `GameUI.h` renumera los `@N` de las 52 unidades que lo incluyen (re-emparejado con refmap; Trap a mano porque no esta en mainref).
- atn/LinearAllocator: `lbl_8087EFA8` = `RenderSetting_unk` (`m054.mEnabled`, `mDof.mEnabled`, `m238`/`m23C`, `m324.mEnabled`, `m374.mEnabled`, `m3DC.mDepth`: los "alpha" de la vista son las prioridades de las capas). APLICADO.
- atn/Layer2DBlur: con `GetBlurWidth_unk`/`GetBlurHeight_unk` en `RenderSetting_unk.h` la unidad queda al 100 %, pero cualquier inline nueva en esa cabecera renumera los `@N` de sus 88 unidades y refmap no los re-empareja todos (quedan ~150 unidades con etiquetas sin pareja). Se hizo con dos inline `static` del propio .cpp (`BlurWidth_unk`/`BlurHeight_unk`): APLICADO (commit aparte).
- atn/ModelDraw_unk: `lbl_8087EFA8` = `RenderSetting_unk`; `m104/m108/m10C/m110[4]` de la vista = `m104.mEnabled/mMode/mCullDist/mColor` (`ShadowParam_unk`). APLICADO.
- atn/ModelInstance: `lbl_8087EFA8` = `RenderSetting_unk`; `m208[mLod]` de la vista (float[8]) = `m20C[mLod - 1]` (distancias LOD 1-3 del menu; `mLod` >= 1 en ese bucle), mismo codigo. APLICADO.
- `lbl_8087EE60`/`lbl_8087EE64` (AI/AiUtil_unk) pasan de `void*` a `CharaTask*`/`CharaLink_unk*` (es `CharaC58_unk::mD4`, que AiTarget pasa a `fn_800142CC`); `CharaLink_unk` gana `mCount` (0x24C) y `mCharas[16]` (0x250); fuera la vista `AiList_unk` de AiCommon_unk. APLICADO.
- `GameUIMarker_unk::m10` era un `int` con un `CharaTask*` dentro (GameEventScript lo guardaba con `(int)chara`, GameUI lo leia con un cast): ahora `CharaTask*` (bug de 64 bits). TownTpcLoader limpia `GameUI::mD40` con una inline `static` del .cpp en vez de la vista `Target_unk`. APLICADO.
- `atn::ColliBase::m0C` (dueno del colisionador) pasa de `int` a `void*`: fuera los `m0C = (int)this`/`(int)mOwner` de 22 unidades y la comparacion `== (int)this` de LastCocoonQuadruped; `BattleTask::m3E2E0` (el dueno copiado de ahi) tambien `void*`. Bug de 64 bits. APLICADO.
- `GimmickMsg_unk::m10` (game/GimmickCommon.h) pasa de `int` a `CharaTask*` (el personaje que manda el mensaje): fuera los `(int)chara`/`(int)this` de Cannonball, CharaTask, BattleCharaTask, BattleTask, GameTask, Em208Queen, ControlCannonLift, ControlAttractHateCharge y PacketPack (solo tipos). Bug de 64 bits. APLICADO.
- `CharaTask::mF1C` (direccion de la matriz que sigue el personaje) es un `int` con un puntero: como `void*` compila en 7 unidades quitando el cast, pero arrastra firmas con nombre mangled (`ControlCaught(Chara_unk*, int arg, ...)`, `ControlMatHold(Chara_unk*, int mtx)`, `Em310Gargoyle::SeatMtx_unk` que devuelve `int`, `Em306GurgBeast::m1530`, `ControlStickByQuark` `mMtx` x3). NO HECHO (pendiente para una sesion: renombrar los simbolos de los constructores).
- `EnemyQuarkStand::m16F0` (personaje agarrado) y `GameEventScript::m98` (personaje que se acerco) pasan de `int` a `CharaTask*` (fuera los casts). Bug de 64 bits. APLICADO.
- `NewCampPair_unk::m04` (`CharaTask*` del miembro; NewCampTask y CosEditItemList_unk) y `WindySlot_unk::m184` (`WindyArea_unk*`) pasan de `int` a puntero. Bug de 64 bits. APLICADO. Quedan como `int` con puntero: `CharaTask::mF1C`/`m13B0`, `TacView::m54.mType` (`(int)chara`), `atn/DrawHelper` `m1C`. (`PopupEntry_unk::mB0` ya es `MtxPtr`: commit aparte.)
- Em101ElderZesya/Em101LittleZesya: `m16A0` (`int` -> `Em101LittleZesya*`) y `Em101LittleZesya::m1530` (`int` -> `Em101ElderZesya*`); fuera `*(Em101ElderZesya**)((char*)m16A0 + 0x1530)` y `*(int*)(m16A0 + 0x14EC)`; `fn_8025D6D0`/`fn_8025D704`/`fn_8025FE30` pasan de `extern "C"` con `zesya` a miembros (simbolos renombrados), `ElderOrder_unk` = `ZesyaWarp_unk`. APLICADO.
- `SpecialEnemyBase::m14B0` (`EnemyInfo_unk*`), `EnemyQuarkStand::m14B0` y `Em303KrakenFootMp::m14F4` (`CharaTask*`, objetivo) pasan de `int` a puntero; fuera ~20 casts. APLICADO. `Em303KrakenFoot::m1504` (el kraken) se queda `int`: lo pasa a `CharaTask::fn_8016C4D8(int, const Vector3&)` (cambiar la firma renombra el simbolo y sus otras llamadas).
- `CharaHit_unk::m10` (atacante, `int`) pasa a `CharaTask*` y `BattleTask::fn_800FDE60(int effect, ...)` a `(CharaTask* attacker, ...)` (simbolo renombrado): fuera 8 casts en CharaTask, CharaStatus y BattleTask. APLICADO.
- `CharaTask::m13B0` (quien lo derroto) pasa de `int` a `CharaTask*`, y `Inventory::fn_80449A18`/`fn_80449ABC` reciben `CharaTask* killer` (simbolos renombrados): fuera los casts de BattleTask, Em301LShade, BattleCharaTask e ItemManager_unk. APLICADO.
- `atn::net::Manager::fn_80509A88/fn_80509A90` (bloqueo del mutex) pasan de `extern "C"` con `mgr` a miembros (simbolos renombrados); DwcAllocator_unk declara `lbl_8087F628` como `atn::net::Manager*` (antes `void*` "mutex"). APLICADO.
- `lbl_8087FA20` (`TownTask*`, NewCampTask) y `lbl_8087F418` (`EnvSeManager*`, GameSoundControl) dejan de ser `void*`. APLICADO.
- ChaseManager: `fn_80182788`/`fn_80182874`/`fn_80183860` pasan de `extern "C"` con `mgr` a miembros (simbolos renombrados; ChaserGuard llama `lbl_8087F098->...`). Las declaraciones nuevas en `ChaseManager.h` renumeran los `@N` de sus 14 unidades (re-emparejado con refmap). APLICADO.
- GameTask: `fn_8036DBC0`/`fn_8036DDD4`/`fn_8036E624`/`fn_8036E6D4`/`fn_8036FE74` (`extern "C"` con `task`) como miembros: `fn_8036DDD4` cambia registros y GameEventScript cambia la pila de 3 funciones (solo por las declaraciones nuevas en `GameTask.h`). DESCARTADO; para el port basta con tratarlas como miembros (el primer argumento es `this`).
- `atn::EffectManager` y `EffectSystem`: el dueno de cada efecto (`owner`) es un `unsigned int` en todas las funciones (`fn_80239D58`, `fn_80239DAC`, `fn_8023A02C`...) y en `EffectEntry_unk::mOwner`, pero los llamadores pasan `this`: para 64 bits hace falta `uintptr_t` (o `void*`) en toda la familia. NO HECHO (toca ~50 unidades y las firmas de EffectSystem).
- Pendiente (regla 10, no empezado por tiempo): `atn::TextAnimation` (6 `extern "C"` con `self`, clase en el .cpp; Event/Dialog, Event/Manager y Event/ActionEffectPlay la ven como `DialogTextMgr_unk`), `atn::Font_unk` (12, ~40 llamadores), `atn::AudioDevice` (35, ver 2.3), `atn::EffectManager` (5, con el `owner` como `unsigned int`), `atn::MindowsCommon` (3).
