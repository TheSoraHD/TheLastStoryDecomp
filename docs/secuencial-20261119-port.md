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
- GameOverTask y NewCampPartsTop con `CampSaveLoad.h`: no llevado. Al incluirla entra `atn/D2A_Task.h`, cuyas
  declaraciones chocan con las locales: `fn_801F4E8C` es `atn::D2A_DrawRect GetRect(atn::D2A_Task*, unsigned int)` y
  aqui `NewCampPartsPoint_unk (atn::Task*, ...)` (struct de game/NewCampPartsBase.h que usan todas las NewCampParts*),
  ademas de las vistas `NewCampTopLayout_unk`/`GameOverD2A_unk`. Para el port: `NewCampPartsPoint_unk` = `atn::D2A_DrawRect`
  (x, y, w, h, z) y el primer argumento de `fn_801F4E8C` es un `atn::D2A_Task*`.

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
