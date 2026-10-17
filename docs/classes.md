# Clases del juego (RTTI)

Generado a partir de las typeinfo y vtables del `.dol` (`SLSP01`). Las vtables tienen la forma `[typeinfo, 0, métodos…]`; en `symbols.txt` se llaman `__vt__<clase>` y `__RTTI__<clase>`.

- **Bases**: todas las clases base que lista la typeinfo (directas e indirectas).
- **Propias**: entradas de la vtable que no aparecen en la vtable de ninguna base: métodos nuevos o sobrescritos por la clase.

Total: 937 clases.

## (global)

| Clase | Bases | vtable | Métodos | Propias |
|---|---|---|---|---|
| `AIManager` | `atn::TaskBase`, `atn::Task` | `0x80775AE0` | 5 | 4 |
| `AiTarget` | — | `0x80775B18` | 2 | 1 |
| `AreaEffConfig` | `atn::TaskBase`, `atn::Task` | `0x807772E8` | 5 | 2 |
| `ArenaTask` | `atn::TaskBase`, `atn::Task` | `0x80777358` | 5 | 4 |
| `Arms` | — | `0x80777494` | 5 | 5 |
| `BattleCharaTask` | `atn::TaskBase`, `atn::Task`, `CharaTask` | `0x80779B68` | 59 | 12 |
| `BattleResult` | `atn::TaskBase`, `atn::Task` | `0x80779CA8` | 5 | 4 |
| `BattleTask` | `atn::TaskBase`, `atn::Task` | `0x80779DA0` | 5 | 4 |
| `BookTask` | `atn::TaskBase`, `atn::Task` | `0x8078FAB0` | 6 | 5 |
| `BootTask` | `atn::TaskBase`, `atn::Task` | `0x80779ED8` | 5 | 3 |
| `Bullet` | — | `0x80779F10` | 8 | 8 |
| `CampSaveLoad` | `atn::TaskBase`, `atn::Task` | `0x8077A030` | 7 | 6 |
| `CharaManager<EnemyTask>` | `atn::TaskBase`, `atn::Task` | `0x8077A1A8` | 6 | 5 |
| `CharaManager<NpcTask>` | `atn::TaskBase`, `atn::Task` | `0x8077A168` | 6 | 5 |
| `CharaManager<PlayerTask>` | `atn::TaskBase`, `atn::Task` | `0x8077A1E8` | 6 | 5 |
| `CharaStatus` | `CharaStatusBase` | `0x8077A6E8` | 1 | 1 |
| `CharaStatusBase` | — | `0x8077A708` | 2 | 1 |
| `CharaTask` | `atn::TaskBase`, `atn::Task` | `0x8077C460` | 55 | 52 |
| `ChaseManager` | `atn::TaskBase`, `atn::Task` | `0x8077CC10` | 5 | 4 |
| `ChaserGuard` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x8077C968` | 60 | 3 |
| `ChaserNpc` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x8077CAE0` | 60 | 3 |
| `ColliAttrManager` | `atn::TaskBase`, `atn::Task` | `0x8077CC48` | 5 | 4 |
| `CopyRobotTask` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x807824D8` | 60 | 5 |
| `CosEditBase` | `atn::TaskBase`, `atn::Task`, `NewCampPartsBase` | `0x80782608` | 28 | 12 |
| `CosEditProtector` | `atn::TaskBase`, `atn::Task`, `NewCampPartsBase`, `CosEditBase` | `0x807827A8` | 28 | 12 |
| `CostumeEditChara` | `atn::TaskBase`, `atn::Task` | `0x807828C8` | 5 | 4 |
| `CrossbowMenuTask` | `atn::TaskBase`, `atn::Task` | `0x8078298C` | 5 | 4 |
| `CrowdManager` | `atn::TaskBase`, `atn::Task` | `0x807829E0` | 5 | 4 |
| `CrowdSimulation` | `atn::TaskBase`, `atn::Task` | `0x80782A18` | 5 | 4 |
| `Em001Necromancer` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x80783D08` | 60 | 9 |
| `Em002Skelton` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x807840A8` | 60 | 5 |
| `Em002_01BlackSkelton` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x80783E48` | 60 | 6 |
| `Em002_02SkeltonKing` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x80783F78` | 60 | 5 |
| `Em003LesserDaemon` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x80784210` | 60 | 9 |
| `Em005DoppelGanger` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x807843B8` | 60 | 9 |
| `Em006DarkMaster` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask`, `SpecialEnemyBase` | `0x807844E8` | 69 | 10 |
| `Em101ElderZesya` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x807846A0` | 60 | 8 |
| `Em101ElderZesyaSecond` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x807847E0` | 60 | 7 |
| `Em101LittleZesya` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x80784958` | 60 | 5 |
| `Em101LittleZesyaSecond` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x80784AC8` | 60 | 7 |
| `Em201ZanGurg` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x80784BF8` | 60 | 6 |
| `Em201ZanGurgFinal` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x80784DD8` | 60 | 13 |
| `Em202ForestSpider` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x807850E0` | 92 | 42 |
| `Em202MothersSpider` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask`, `Em202ForestSpider` | `0x807852E8` | 96 | 17 |
| `Em202Spider` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x80785500` | 60 | 9 |
| `Em202SpiderBase` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x80785670` | 60 | 9 |
| `Em202_01_BSpider` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x80784F48` | 60 | 8 |
| `Em203GreaterDaemon` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x807857E0` | 60 | 19 |
| `Em203GreaterDaemonRed` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x80785978` | 61 | 24 |
| `Em203GreaterDaemonRedMp` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask`, `SpecialEnemyBase` | `0x80785B38` | 70 | 15 |
| `Em204BigMulue` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask`, `SpecialEnemyBase` | `0x80785F0C` | 69 | 18 |
| `Em204Mulue` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x80785D28` | 60 | 12 |
| `Em204MulueMp` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask`, `SpecialEnemyBase` | `0x80786204` | 69 | 15 |
| `Em205Ogre` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask`, `SpecialEnemyBase` | `0x807863B4` | 69 | 16 |
| `Em206CocoonMp` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask`, `SpecialEnemyBase` | `0x80786568` | 69 | 20 |
| `Em206EggMonster` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x807866F0` | 60 | 14 |
| `Em208Queen` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x80786860` | 60 | 12 |
| `Em209BigGolem` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask`, `Em209SmallGolem` | `0x80786990` | 60 | 6 |
| `Em209SmallGolem` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x80786B18` | 73 | 28 |
| `Em301LShade` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask`, `SpecialEnemyBase` | `0x80786C44` | 69 | 15 |
| `Em302Kraken` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x80786E20` | 60 | 20 |
| `Em302KrakenMp` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask`, `SpecialEnemyBase` | `0x80786F9C` | 69 | 22 |
| `Em303KrakenFoot` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x80787160` | 60 | 9 |
| `Em303KrakenFootMp` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask`, `SpecialEnemyBase` | `0x80787290` | 69 | 16 |
| `Em305Vampire` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask`, `SpecialEnemyBase` | `0x807873E8` | 70 | 13 |
| `Em305VampireMp` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask`, `SpecialEnemyBase`, `Em305Vampire` | `0x80787568` | 70 | 13 |
| `Em306GurgBeast` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask`, `SpecialEnemyBase` | `0x8078778C` | 69 | 11 |
| `Em308InvertCrab` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x80787938` | 60 | 13 |
| `Em308ParmCrab` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x80787AA8` | 60 | 11 |
| `Em309Ghoul` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x80787C10` | 60 | 12 |
| `Em309GhoulSimple` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask`, `SpecialEnemyBase` | `0x80787DF0` | 69 | 18 |
| `Em310Gargoyle` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x807880D0` | 60 | 9 |
| `Em312DarkServant` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask`, `SpecialEnemyBase` | `0x80788220` | 69 | 11 |
| `Em314Ivy` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask`, `SpecialEnemyBase` | `0x807883D0` | 69 | 12 |
| `Em315Coffin` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask`, `SpecialEnemyBase` | `0x807885D0` | 69 | 13 |
| `Em316LaserSphere` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x80788760` | 60 | 9 |
| `Em316LastLaser` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x80788890` | 60 | 8 |
| `Em401_01_WhiteLeoPard` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x80788A10` | 60 | 10 |
| `Em403WarLion` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x80788BD0` | 60 | 14 |
| `EnemyFieldMagician` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x80788D20` | 60 | 6 |
| `EnemyJill` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x80788E90` | 60 | 7 |
| `EnemyLizerdBlade` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x80788FC0` | 60 | 7 |
| `EnemyManager` | `atn::TaskBase`, `atn::Task`, `CharaManager<EnemyTask>` | `0x807890F0` | 6 | 2 |
| `EnemyQuarkFirst` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask`, `SpecialEnemyBase` | `0x807891D4` | 69 | 12 |
| `EnemyQuarkSecond` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x80789388` | 60 | 15 |
| `EnemyQuarkStand` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x80789550` | 60 | 18 |
| `EnemySeaLizerd` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x807896B8` | 60 | 8 |
| `EnemyTasha` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask`, `SpecialEnemyBase` | `0x807898B0` | 69 | 12 |
| `EnemyTask` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask` | `0x80789A08` | 61 | 4 |
| `EnemyTrista` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x80789B88` | 60 | 6 |
| `EnvSeBase` | `atn::TaskBase`, `atn::Task` | `0x80789E58` | 7 | 3 |
| `EnvSeBox` | `atn::TaskBase`, `atn::Task`, `EnvSeBase` | `0x80789D38` | 7 | 5 |
| `EnvSeGlobal` | `atn::TaskBase`, `atn::Task`, `EnvSeBase` | `0x80789CF0` | 7 | 6 |
| `EnvSeLine` | `atn::TaskBase`, `atn::Task`, `EnvSeBase` | `0x80789DC8` | 7 | 5 |
| `EnvSeManager` | `atn::TaskBase`, `atn::Task` | `0x80789CB8` | 5 | 4 |
| `EnvSePoint` | `atn::TaskBase`, `atn::Task`, `EnvSeBase` | `0x80789E10` | 7 | 5 |
| `EnvSeSphere` | `atn::TaskBase`, `atn::Task`, `EnvSeBase` | `0x80789D80` | 7 | 5 |
| `EquipItemData` | — | `0x80789E98` | 2 | 2 |
| `FriendArganan` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `PlayerTask` | `0x80789ED8` | 60 | 2 |
| `FriendManager` | `atn::TaskBase`, `atn::Task` | `0x8078A038` | 5 | 1 |
| `GameBgm` | `atn::TaskBase`, `atn::Task` | `0x8078A4F0` | 5 | 3 |
| `GameChoiceWindow` | `atn::TaskBase`, `atn::Task` | `0x8078B4D0` | 5 | 3 |
| `GameEventScript` | `atn::TaskBase`, `atn::Task` | `0x8078B128` | 5 | 4 |
| `GameLoadBackground` | `atn::TaskBase`, `atn::Task`, `GameSaveLoadBackgroundCommon` | `0x8078B400` | 5 | 2 |
| `GameLoadTask` | `atn::TaskBase`, `atn::Task`, `GameSaveLoadCommon` | `0x8078B3A0` | 5 | 4 |
| `GameMessageWindow` | `atn::TaskBase`, `atn::Task` | `0x8078B498` | 5 | 3 |
| `GameOverTask` | `atn::TaskBase`, `atn::Task` | `0x8078A4B8` | 5 | 4 |
| `GamePauseTask` | `atn::TaskBase`, `atn::Task` | `0x8078A394` | 5 | 2 |
| `GameSaveBackground` | `atn::TaskBase`, `atn::Task`, `GameSaveLoadBackgroundCommon` | `0x8078B5A8` | 5 | 3 |
| `GameSaveBanner` | `atn::TaskBase`, `atn::Task` | `0x8078B620` | 5 | 3 |
| `GameSaveLoadBackgroundCommon` | `atn::TaskBase`, `atn::Task` | `0x8078B548` | 14 | 9 |
| `GameSaveLoadCommon` | `atn::TaskBase`, `atn::Task` | `0x8078B588` | 6 | 1 |
| `GameSaveTask` | `atn::TaskBase`, `atn::Task`, `GameSaveLoadCommon` | `0x8078B508` | 5 | 4 |
| `GameScene` | `atn::TaskBase`, `atn::Task` | `0x8078BC48` | 5 | 4 |
| `GameSoundControl` | `atn::TaskBase`, `atn::Task` | `0x8078BDD0` | 5 | 2 |
| `GameTask` | `atn::TaskBase`, `atn::Task` | `0x8078A3D0` | 5 | 4 |
| `GameTextMessageWindow` | `atn::TaskBase`, `atn::Task` | `0x8078B460` | 5 | 3 |
| `GameTimer` | `atn::TaskBase`, `atn::Task` | `0x8078BE08` | 5 | 4 |
| `GameUI` | `atn::TaskBase`, `atn::Task` | `0x8078C584` | 5 | 4 |
| `GameVoice` | `atn::TaskBase`, `atn::Task` | `0x8078C690` | 5 | 3 |
| `GimmickCommon` | `atn::TaskBase`, `atn::Task` | `0x8078C9D0` | 27 | 26 |
| `GimmickManager` | `atn::TaskBase`, `atn::Task` | `0x8078CAB0` | 5 | 4 |
| `GimmickStateCommand` | `atn::TaskBase`, `atn::Task` | `0x8078CB40` | 5 | 2 |
| `HomeButtonMenu` | `atn::IRender` | `0x8078F258` | 6 | 6 |
| `HomingBullet` | `Bullet` | `0x8078F2B8` | 9 | 9 |
| `IAllocator` | — | — | — | — |
| `LaserTrace` | — | `0x807889C0` | 2 | 1 |
| `LastCocoonChrysalis` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x8078F500` | 60 | 6 |
| `LastCocoonEgg` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x8078F630` | 60 | 7 |
| `LastCocoonImago` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x8078F7F0` | 60 | 22 |
| `LastCocoonQuadruped` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x8078F980` | 60 | 13 |
| `LibraryTask` | `atn::TaskBase`, `atn::Task` | `0x8078FAF0` | 6 | 5 |
| `LoadSeTableTask` | `atn::TaskBase`, `atn::Task` | `0x8078CA78` | 5 | 2 |
| `LogMessageTask` | `atn::TaskBase`, `atn::Task` | `0x80790028` | 5 | 4 |
| `MapManager` | `atn::TaskBase`, `atn::Task` | `0x807901E0` | 5 | 2 |
| `MapPartsArea` | `atn::TaskBase`, `atn::Task`, `MapPartsBase` | `0x80790218` | 11 | 6 |
| `MapPartsBase` | `atn::TaskBase`, `atn::Task` | `0x807902E8` | 12 | 7 |
| `MapPartsColliTree` | `atn::TaskBase`, `atn::Task`, `MapPartsBase` | `0x80790320` | 11 | 3 |
| `MapPartsEfp` | `atn::TaskBase`, `atn::Task`, `MapPartsBase` | `0x80790378` | 11 | 4 |
| `MapPartsFlare` | `atn::TaskBase`, `atn::Task`, `MapPartsBase` | `0x807903F0` | 11 | 4 |
| `MapPartsGimmickLoc` | `atn::TaskBase`, `atn::Task`, `MapPartsBase` | `0x80790560` | 11 | 7 |
| `MapPartsLocatorSet` | `atn::TaskBase`, `atn::Task`, `MapPartsBase` | `0x807905D8` | 11 | 7 |
| `MapPartsModel` | `atn::TaskBase`, `atn::Task`, `MapPartsBase` | `0x80790630` | 11 | 8 |
| `MapPartsOcean` | `atn::TaskBase`, `atn::Task`, `MapPartsBase`, `MapPartsModel` | `0x80790688` | 11 | 4 |
| `MapPartsRain` | `atn::TaskBase`, `atn::Task`, `MapPartsBase` | `0x807906E8` | 11 | 5 |
| `MapTask` | `atn::TaskBase`, `atn::Task` | `0x80790740` | 5 | 4 |
| `MarketTask` | `atn::TaskBase`, `atn::Task` | `0x80790778` | 5 | 4 |
| `MenuCommonTask` | `atn::TaskBase`, `atn::Task` | `0x807907DC` | 5 | 4 |
| `MenuLoadTask` | `atn::TaskBase`, `atn::Task`, `CampSaveLoad` | `0x80779FA0` | 7 | 1 |
| `MenuSaveTask` | `atn::TaskBase`, `atn::Task`, `CampSaveLoad` | `0x80779FE8` | 7 | 2 |
| `MiniMapTask` | `atn::IRender`, `atn::TaskBase`, `atn::Task` | `0x80790818` | 5 | 4 |
| `NewCampPartsBase` | `atn::TaskBase`, `atn::Task` | `0x807932B8` | 24 | 22 |
| `NewCampPartsGmk` | `atn::TaskBase`, `atn::Task`, `NewCampPartsBase` | `0x80793320` | 23 | 7 |
| `NewCampPartsHint` | `atn::TaskBase`, `atn::Task`, `NewCampPartsBase` | `0x807933C8` | 23 | 13 |
| `NewCampPartsItem` | `atn::TaskBase`, `atn::Task`, `NewCampPartsBase` | `0x80793450` | 23 | 12 |
| `NewCampPartsMapSelect` | `atn::TaskBase`, `atn::Task`, `NewCampPartsBase` | `0x80793508` | 23 | 13 |
| `NewCampPartsOption` | `atn::TaskBase`, `atn::Task`, `NewCampPartsBase` | `0x80793610` | 23 | 13 |
| `NewCampPartsStatus` | `atn::TaskBase`, `atn::Task`, `NewCampPartsBase` | `0x80793698` | 23 | 13 |
| `NewCampPartsTop` | `atn::TaskBase`, `atn::Task`, `NewCampPartsBase` | `0x80793740` | 23 | 9 |
| `NewCampTask` | `atn::TaskBase`, `atn::Task` | `0x80793830` | 5 | 4 |
| `NewStaffRoll` | `atn::TaskBase`, `atn::Task` | `0x80793888` | 5 | 4 |
| `NewTitleTask` | `atn::TaskBase`, `atn::Task` | `0x80793A20` | 5 | 4 |
| `Np131LivingArmor` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x80794608` | 60 | 7 |
| `NpcManager` | `atn::TaskBase`, `atn::Task`, `CharaManager<NpcTask>` | `0x80794738` | 6 | 2 |
| `NpcTask` | `atn::TaskBase`, `atn::Task`, `CharaTask` | `0x80794780` | 55 | 4 |
| `PickTask` | `atn::TaskBase`, `atn::Task` | `0x807948E8` | 5 | 3 |
| `PlayerManager` | `atn::TaskBase`, `atn::Task`, `CharaManager<PlayerTask>` | `0x80794920` | 6 | 3 |
| `PlayerTask` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask` | `0x80794968` | 69 | 11 |
| `PopupTask` | `atn::TaskBase`, `atn::Task` | `0x80794A84` | 5 | 4 |
| `QuadrupedTask` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x807956F0` | 60 | 10 |
| `RandomMerchantChara` | `atn::TaskBase`, `atn::Task`, `CharaTask` | `0x8078ECA0` | 55 | 4 |
| `RecycleBuffer<gmk::ItemSlotAnime>` | `atn::TaskBase`, `atn::Task` | `0x8078F3C8` | 5 | 1 |
| `ReplayBuffer` | `atn::EffectManagerCallback` | `0x80795828` | 1 | 1 |
| `SelectMember` | `atn::TaskBase`, `atn::Task` | `0x80795850` | 5 | 4 |
| `SetConfig` | — | `0x80796790` | 2 | 1 |
| `ShopBase` | `atn::TaskBase`, `atn::Task` | `0x807967A8` | 15 | 9 |
| `ShopBuySell` | `atn::TaskBase`, `atn::Task`, `ShopBase` | `0x80796864` | 15 | 7 |
| `ShopColor` | `atn::TaskBase`, `atn::Task`, `ShopBase` | `0x807968E8` | 15 | 8 |
| `ShopIdentify` | `atn::TaskBase`, `atn::Task`, `ShopBase` | `0x80796994` | 15 | 9 |
| `ShopUpGrade` | `atn::TaskBase`, `atn::Task`, `ShopBase` | `0x80796A44` | 15 | 8 |
| `SkillEffectManager` | `atn::TaskBase`, `atn::Task` | `0x80796B28` | 5 | 4 |
| `SongManager` | `atn::TaskBase`, `atn::Task` | `0x80797330` | 5 | 3 |
| `SpecialEnemyBase` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x80797368` | 144 | 82 |
| `StrapTask` | `atn::TaskBase`, `atn::Task` | `0x807975B0` | 5 | 4 |
| `Sword` | `Arms` | `0x807975E8` | 5 | 3 |
| `TacView` | `atn::TaskBase`, `atn::Task` | `0x807976EC` | 5 | 4 |
| `TeamAIManager` | `atn::TaskBase`, `atn::Task` | `0x80797790` | 5 | 4 |
| `TownTask` | `atn::TaskBase`, `atn::Task`, `MapTask` | `0x807978C8` | 5 | 4 |
| `TownTpcLoader` | `atn::TaskBase`, `atn::Task` | `0x80797928` | 5 | 2 |
| `TutorialPopup` | `atn::TaskBase`, `atn::Task` | `0x80797974` | 5 | 3 |
| `TutorialTask` | `atn::TaskBase`, `atn::Task` | `0x807979B0` | 5 | 4 |

## AI

| Clase | Bases | vtable | Métodos | Propias |
|---|---|---|---|---|
| `AI::Script::AI_archer_attack01` | `AI::Script::CharaAi` | `0x80777230` | 15 | 15 |
| `AI::Script::AI_archer_guard01` | `AI::Script::CharaAi` | `0x807771D8` | 15 | 15 |
| `AI::Script::AI_arganan01` | `AI::Script::CharaAi` | `0x80777180` | 15 | 15 |
| `AI::Script::AI_buffer_attackstable01` | `AI::Script::CharaAi` | `0x80777128` | 15 | 15 |
| `AI::Script::AI_buffer_guard01` | `AI::Script::CharaAi` | `0x807770D0` | 15 | 15 |
| `AI::Script::AI_buffer_guard02` | `AI::Script::CharaAi` | `0x80777078` | 15 | 15 |
| `AI::Script::AI_buffer_pointguard01` | `AI::Script::CharaAi` | `0x80777020` | 15 | 15 |
| `AI::Script::AI_em_archer_attack02` | `AI::Script::CharaAi` | `0x80776FC8` | 15 | 15 |
| `AI::Script::AI_em_bridge_archer` | `AI::Script::CharaAi` | `0x80776F70` | 15 | 15 |
| `AI::Script::AI_em_monster_attack01` | `AI::Script::CharaAi` | `0x80776F18` | 15 | 15 |
| `AI::Script::AI_em_scout` | `AI::Script::CharaAi` | `0x80776EC0` | 15 | 15 |
| `AI::Script::AI_em_sword_attack01` | `AI::Script::CharaAi` | `0x80776E68` | 15 | 15 |
| `AI::Script::AI_em_sword_attack02` | `AI::Script::CharaAi` | `0x80776E10` | 15 | 15 |
| `AI::Script::AI_em_sword_pointguard01` | `AI::Script::CharaAi` | `0x80776DB8` | 15 | 15 |
| `AI::Script::AI_em_type003` | `AI::Script::CharaAi` | `0x80776D60` | 15 | 15 |
| `AI::Script::AI_em_type010` | `AI::Script::CharaAi` | `0x80776D08` | 15 | 15 |
| `AI::Script::AI_em_type102` | `AI::Script::CharaAi` | `0x80776CB0` | 15 | 15 |
| `AI::Script::AI_em_type103` | `AI::Script::CharaAi` | `0x80776C58` | 15 | 15 |
| `AI::Script::AI_em_type104` | `AI::Script::CharaAi` | `0x80776C00` | 15 | 15 |
| `AI::Script::AI_em_type105` | `AI::Script::CharaAi` | `0x80776BA8` | 15 | 15 |
| `AI::Script::AI_em_type106` | `AI::Script::CharaAi` | `0x80776B50` | 15 | 15 |
| `AI::Script::AI_fr_archer_pick01` | `AI::Script::CharaAi` | `0x80776AF8` | 15 | 15 |
| `AI::Script::AI_fr_buffer_pick01` | `AI::Script::CharaAi` | `0x80776AA0` | 15 | 15 |
| `AI::Script::AI_fr_follow_player` | `AI::Script::CharaAi` | `0x80776A48` | 15 | 15 |
| `AI::Script::AI_fr_magic_pick01` | `AI::Script::CharaAi` | `0x807769F0` | 15 | 15 |
| `AI::Script::AI_fr_magic_pick02` | `AI::Script::CharaAi` | `0x80776998` | 15 | 15 |
| `AI::Script::AI_fr_runaway` | `AI::Script::CharaAi` | `0x80776940` | 15 | 15 |
| `AI::Script::AI_fr_sword_attack01` | `AI::Script::CharaAi` | `0x807768E8` | 15 | 15 |
| `AI::Script::AI_fr_sword_pick01` | `AI::Script::CharaAi` | `0x80776890` | 15 | 15 |
| `AI::Script::AI_fr_sword_pointguard` | `AI::Script::CharaAi` | `0x80776838` | 15 | 15 |
| `AI::Script::AI_fr_sword_pointguard01` | `AI::Script::CharaAi` | `0x807767E0` | 15 | 15 |
| `AI::Script::AI_fr_type005` | `AI::Script::CharaAi` | `0x80776788` | 15 | 15 |
| `AI::Script::AI_fr_warppointescape` | `AI::Script::CharaAi` | `0x80776730` | 15 | 15 |
| `AI::Script::AI_jackal01` | `AI::Script::CharaAi` | `0x807766D8` | 15 | 15 |
| `AI::Script::AI_magic_attack01` | `AI::Script::CharaAi` | `0x80776680` | 15 | 15 |
| `AI::Script::AI_magic_attack01_2` | `AI::Script::CharaAi` | `0x80776628` | 15 | 15 |
| `AI::Script::AI_magic_attack02` | `AI::Script::CharaAi` | `0x807765D0` | 15 | 15 |
| `AI::Script::AI_magic_attackstable01` | `AI::Script::CharaAi` | `0x80776578` | 15 | 15 |
| `AI::Script::AI_magic_cure01` | `AI::Script::CharaAi` | `0x80776520` | 15 | 15 |
| `AI::Script::AI_magic_guard01` | `AI::Script::CharaAi` | `0x807764C8` | 15 | 15 |
| `AI::Script::AI_magic_guard01_2` | `AI::Script::CharaAi` | `0x80776470` | 15 | 15 |
| `AI::Script::AI_magic_guard02` | `AI::Script::CharaAi` | `0x80776418` | 15 | 15 |
| `AI::Script::AI_np_approach` | `AI::Script::CharaAi` | `0x807763C0` | 15 | 15 |
| `AI::Script::AI_np_approach_plus` | `AI::Script::CharaAi` | `0x80776368` | 15 | 15 |
| `AI::Script::AI_np_follow_player` | `AI::Script::CharaAi` | `0x80776310` | 15 | 15 |
| `AI::Script::AI_np_tagger_random` | `AI::Script::CharaAi` | `0x807762B8` | 15 | 15 |
| `AI::Script::AI_np_type001` | `AI::Script::CharaAi` | `0x80776260` | 15 | 15 |
| `AI::Script::AI_np_type002` | `AI::Script::CharaAi` | `0x80776208` | 15 | 15 |
| `AI::Script::AI_np_type003` | `AI::Script::CharaAi` | `0x807761B0` | 15 | 15 |
| `AI::Script::AI_np_type004` | `AI::Script::CharaAi` | `0x80776158` | 15 | 15 |
| `AI::Script::AI_np_type005` | `AI::Script::CharaAi` | `0x80776100` | 15 | 15 |
| `AI::Script::AI_np_wait_reaction` | `AI::Script::CharaAi` | `0x807760A8` | 15 | 15 |
| `AI::Script::AI_np_wait_reaction_minus` | `AI::Script::CharaAi` | `0x80776050` | 15 | 15 |
| `AI::Script::AI_np_wait_reaction_minus2` | `AI::Script::CharaAi` | `0x80775FF8` | 15 | 15 |
| `AI::Script::AI_np_wait_reaction_plus` | `AI::Script::CharaAi` | `0x80775FA0` | 15 | 15 |
| `AI::Script::AI_quark01` | `AI::Script::CharaAi` | `0x80775F48` | 15 | 15 |
| `AI::Script::AI_quark02` | `AI::Script::CharaAi` | `0x80775EF0` | 15 | 15 |
| `AI::Script::AI_return_guard01` | `AI::Script::CharaAi` | `0x80775C84` | 15 | 15 |
| `AI::Script::AI_silence_guard01` | `AI::Script::CharaAi` | `0x80775E98` | 15 | 15 |
| `AI::Script::AI_sword_guard01` | `AI::Script::CharaAi` | `0x80775E40` | 15 | 15 |
| `AI::Script::AI_sword_leaderguard01` | `AI::Script::CharaAi` | `0x80775DE8` | 15 | 15 |
| `AI::Script::AI_sword_leaderguard02` | `AI::Script::CharaAi` | `0x80775D90` | 15 | 15 |
| `AI::Script::AI_sword_leaderguard03` | `AI::Script::CharaAi` | `0x80775D38` | 15 | 15 |
| `AI::Script::AI_sword_pointguard01` | `AI::Script::CharaAi` | `0x80775CE0` | 15 | 15 |
| `AI::Script::CharaAi` | — | — | — | — |

## Event

| Clase | Bases | vtable | Métodos | Propias |
|---|---|---|---|---|
| `Event::Action` | `Event::Node` | `0x80793AB0` | 2 | 2 |
| `Event::ActionActorBehavior` | `atn::SingletonNoStatic<Event::ActionActorBehavior>`, `Event::ActionProcessor` | `0x80794AE0` | 2 | 2 |
| `Event::ActionActorEyes` | `atn::SingletonNoStatic<Event::ActionActorEyes>`, `Event::ActionProcessor` | `0x80794C98` | 2 | 2 |
| `Event::ActionActorFacial` | `atn::SingletonNoStatic<Event::ActionActorFacial>`, `Event::ActionProcessor` | `0x80794CD0` | 2 | 2 |
| `Event::ActionActorHeading` | `atn::SingletonNoStatic<Event::ActionActorHeading>`, `Event::ActionProcessor` | `0x80794C28` | 2 | 2 |
| `Event::ActionActorMaterial` | `atn::SingletonNoStatic<Event::ActionActorMaterial>`, `Event::ActionProcessor` | `0x80794B48` | 2 | 2 |
| `Event::ActionActorMotion` | `atn::SingletonNoStatic<Event::ActionActorMotion>`, `Event::ActionProcessor` | `0x80794D08` | 2 | 2 |
| `Event::ActionActorNode` | `atn::SingletonNoStatic<Event::ActionActorNode>`, `Event::ActionProcessor` | `0x80794BF0` | 2 | 2 |
| `Event::ActionActorPose` | `atn::SingletonNoStatic<Event::ActionActorPose>`, `Event::ActionProcessor` | `0x80794C60` | 2 | 2 |
| `Event::ActionActorPosition` | `atn::SingletonNoStatic<Event::ActionActorPosition>`, `Event::ActionProcessor` | `0x80794D40` | 2 | 2 |
| `Event::ActionActorSetting` | `atn::SingletonNoStatic<Event::ActionActorSetting>`, `Event::ActionProcessor` | `0x80794BB8` | 2 | 2 |
| `Event::ActionActorWind` | `atn::SingletonNoStatic<Event::ActionActorWind>`, `Event::ActionProcessor` | `0x80794B80` | 2 | 2 |
| `Event::ActionAmbientLight` | `atn::SingletonNoStatic<Event::ActionAmbientLight>`, `Event::ActionProcessor` | `0x807952A8` | 2 | 2 |
| `Event::ActionAnimationPlay` | `atn::SingletonNoStatic<Event::ActionAnimationPlay>`, `Event::ActionProcessor` | `0x807955F0` | 2 | 2 |
| `Event::ActionAudioEffect` | `atn::SingletonNoStatic<Event::ActionAudioEffect>`, `Event::ActionProcessor` | `0x807953F8` | 2 | 2 |
| `Event::ActionBrightness` | `atn::SingletonNoStatic<Event::ActionBrightness>`, `Event::ActionProcessor` | `0x80795190` | 2 | 2 |
| `Event::ActionCameraChange` | `atn::SingletonNoStatic<Event::ActionCameraChange>`, `Event::ActionProcessor` | `0x807950E8` | 2 | 2 |
| `Event::ActionCameraEffect` | `atn::SingletonNoStatic<Event::ActionCameraEffect>`, `Event::ActionProcessor` | `0x80794EE0` | 2 | 2 |
| `Event::ActionCameraFocus` | `atn::SingletonNoStatic<Event::ActionCameraFocus>`, `Event::ActionProcessor` | `0x80794E38` | 2 | 2 |
| `Event::ActionCameraInertia` | `atn::SingletonNoStatic<Event::ActionCameraInertia>`, `Event::ActionProcessor` | `0x80794DC8` | 2 | 2 |
| `Event::ActionCameraMode` | `atn::SingletonNoStatic<Event::ActionCameraMode>`, `Event::ActionProcessor` | `0x80794F18` | 2 | 2 |
| `Event::ActionCameraMotion` | `atn::SingletonNoStatic<Event::ActionCameraMotion>`, `Event::ActionProcessor` | `0x80794E00` | 2 | 2 |
| `Event::ActionCameraSetting` | `atn::SingletonNoStatic<Event::ActionCameraSetting>`, `Event::ActionProcessor` | `0x80794E70` | 2 | 2 |
| `Event::ActionCameraShake` | `atn::SingletonNoStatic<Event::ActionCameraShake>`, `Event::ActionProcessor` | `0x80794EA8` | 2 | 2 |
| `Event::ActionCheck` | `atn::SingletonNoStatic<Event::ActionCheck>`, `Event::ActionProcessor` | `0x807955B8` | 2 | 2 |
| `Event::ActionCloudMap` | `atn::SingletonNoStatic<Event::ActionCloudMap>`, `Event::ActionProcessor` | `0x80795270` | 2 | 2 |
| `Event::ActionColorGradation` | `atn::SingletonNoStatic<Event::ActionColorGradation>`, `Event::ActionProcessor` | `0x80795120` | 2 | 2 |
| `Event::ActionColorMatrix` | `atn::SingletonNoStatic<Event::ActionColorMatrix>`, `Event::ActionProcessor` | `0x80795158` | 2 | 2 |
| `Event::ActionCutAdjust` | `atn::SingletonNoStatic<Event::ActionCutAdjust>`, `Event::ActionProcessor` | `0x80794F98` | 2 | 2 |
| `Event::ActionDialogSpeaker` | `atn::SingletonNoStatic<Event::ActionDialogSpeaker>`, `Event::ActionProcessor` | `0x80795008` | 2 | 2 |
| `Event::ActionDialogText` | `atn::SingletonNoStatic<Event::ActionDialogText>`, `Event::ActionProcessor` | `0x80794FD0` | 2 | 2 |
| `Event::ActionEffectPlay` | `atn::SingletonNoStatic<Event::ActionEffectPlay>`, `Event::ActionProcessor` | `0x80795628` | 2 | 2 |
| `Event::ActionEnvSound` | `atn::SingletonNoStatic<Event::ActionEnvSound>`, `Event::ActionProcessor` | `0x80795468` | 2 | 2 |
| `Event::ActionGameSetting` | `atn::SingletonNoStatic<Event::ActionGameSetting>`, `Event::ActionProcessor` | `0x80795040` | 2 | 2 |
| `Event::ActionGodRay` | `atn::SingletonNoStatic<Event::ActionGodRay>`, `Event::ActionProcessor` | `0x80795318` | 2 | 2 |
| `Event::ActionLightSurround` | `atn::SingletonNoStatic<Event::ActionLightSurround>`, `Event::ActionProcessor` | `0x807951C8` | 2 | 2 |
| `Event::ActionLocatorHide` | `atn::SingletonNoStatic<Event::ActionLocatorHide>`, `Event::ActionProcessor` | `0x807950B0` | 2 | 2 |
| `Event::ActionMapReflect` | `atn::SingletonNoStatic<Event::ActionMapReflect>`, `Event::ActionProcessor` | `0x80795078` | 2 | 2 |
| `Event::ActionMessageChoice` | `atn::SingletonNoStatic<Event::ActionMessageChoice>`, `Event::ActionProcessor` | `0x80795548` | 2 | 2 |
| `Event::ActionMessagePrint` | `atn::SingletonNoStatic<Event::ActionMessagePrint>`, `Event::ActionProcessor` | `0x807954D8` | 2 | 2 |
| `Event::ActionMessageWindow` | `atn::SingletonNoStatic<Event::ActionMessageWindow>`, `Event::ActionProcessor` | `0x80795510` | 2 | 2 |
| `Event::ActionMoviePlay` | `atn::SingletonNoStatic<Event::ActionMoviePlay>`, `Event::ActionProcessor` | `0x80795350` | 2 | 2 |
| `Event::ActionMultiAudio` | `atn::SingletonNoStatic<Event::ActionMultiAudio>`, `Event::ActionProcessor` | `0x80795388` | 2 | 2 |
| `Event::ActionMusicPlay` | `atn::SingletonNoStatic<Event::ActionMusicPlay>`, `Event::ActionProcessor` | `0x80795430` | 2 | 2 |
| `Event::ActionNotify` | `atn::SingletonNoStatic<Event::ActionNotify>`, `Event::ActionProcessor` | `0x80795580` | 2 | 2 |
| `Event::ActionPointLight` | `atn::SingletonNoStatic<Event::ActionPointLight>`, `Event::ActionProcessor` | `0x80795238` | 2 | 2 |
| `Event::ActionProcessor` | — | — | — | — |
| `Event::ActionSoundPlay` | `atn::SingletonNoStatic<Event::ActionSoundPlay>`, `Event::ActionProcessor` | `0x807954A0` | 2 | 2 |
| `Event::ActionSpotLight` | `atn::SingletonNoStatic<Event::ActionSpotLight>`, `Event::ActionProcessor` | `0x80795200` | 2 | 2 |
| `Event::ActionSunLight` | `atn::SingletonNoStatic<Event::ActionSunLight>`, `Event::ActionProcessor` | `0x807952E0` | 2 | 2 |
| `Event::ActionVoicePlay` | `atn::SingletonNoStatic<Event::ActionVoicePlay>`, `Event::ActionProcessor` | `0x807953C0` | 2 | 2 |
| `Event::Actor` | `Event::CommonData` | `0x80775A20` | 3 | 3 |
| `Event::AnimationData` | `Event::CommonData` | `0x80782F60` | 3 | 3 |
| `Event::BehaviorFollow` | `atn::Singleton<Event::BehaviorFollow>`, `Event::ActionProcessor` | `0x80779E30` | 2 | 2 |
| `Event::BehaviorGesture` | `atn::Singleton<Event::BehaviorGesture>`, `Event::ActionProcessor` | `0x80779E68` | 2 | 2 |
| `Event::BehaviorMoveTo` | `atn::Singleton<Event::BehaviorMoveTo>`, `Event::ActionProcessor` | `0x80779EA0` | 2 | 2 |
| `Event::CameraData` | `Event::CommonData` | `0x80782EE8` | 3 | 3 |
| `Event::Channel` | `Event::Node` | `0x80793BA0` | 1 | 1 |
| `Event::CommonData` | — | — | — | — |
| `Event::Cut` | `Event::Node` | `0x80793D98` | 1 | 1 |
| `Event::Dialog` | `atn::TaskBase`, `atn::Task` | `0x807839E0` | 5 | 4 |
| `Event::DialogHead` | `atn::IRender`, `atn::TaskBase`, `atn::Task`, `CharaTask` | `0x807838A0` | 55 | 4 |
| `Event::DummyData` | `Event::CommonData` | `0x80794458` | 3 | 3 |
| `Event::EffectData` | `Event::CommonData` | `0x80782F10` | 3 | 3 |
| `Event::Label` | `Event::Node` | `0x80793E38` | 1 | 1 |
| `Event::Manager` | `atn::TaskBase`, `atn::Task` | `0x807900C0` | 5 | 4 |
| `Event::ModelData` | `Event::CommonData` | `0x80782F38` | 3 | 3 |
| `Event::MotionData` | `Event::CommonData` | `0x80782F88` | 3 | 3 |
| `Event::MovieData` | `Event::CommonData` | `0x80782E60` | 3 | 3 |
| `Event::MovieTask` | `atn::TaskBase`, `atn::Task` | `0x80782E88` | 5 | 4 |
| `Event::Node` | — | `0x80793AD8` | 2 | 1 |
| `Event::Path` | `Event::CommonData` | `0x80779F40` | 3 | 3 |
| `Event::PoseData` | `Event::CommonData` | `0x80782EC0` | 3 | 3 |
| `Event::PostSequence` | `atn::TaskBase`, `atn::Task` | `0x807900F8` | 5 | 4 |
| `Event::Puppeteer<CharaTask>` | `atn::TaskBase`, `atn::Task` | `0x807759E8` | 5 | 4 |
| `Event::Puppeteer<atn::Model>` | `atn::TaskBase`, `atn::Task` | `0x807759B0` | 5 | 4 |
| `Event::Section` | `Event::Node` | `0x80794008` | 1 | 1 |
| `Event::Sequence` | `Event::Node`, `atn::TaskBase`, `atn::Task` | `0x807943E0` | 5 | 4 |
| `Event::SequenceRoot` | `atn::TaskBase`, `atn::Task` | `0x80790130` | 5 | 4 |
| `Event::StringData` | `Event::CommonData` | `0x80794430` | 3 | 3 |

## MP

| Clase | Bases | vtable | Métodos | Propias |
|---|---|---|---|---|
| `MP::Chara<EnemyTask>` | `atn::TaskBase`, `atn::Task`, `CharaTask`, `BattleCharaTask`, `EnemyTask` | `0x80791570` | 60 | 4 |
| `MP::CryptMP` | `atn::net::Crypt` | `0x807908D0` | 6 | 6 |
| `MP::D2ACommonDialog` | `atn::TaskBase`, `atn::Task` | `0x80790888` | 5 | 4 |
| `MP::D2AMpMenu` | `atn::TaskBase`, `atn::Task` | `0x80790AB0` | 7 | 6 |
| `MP::D2AMpMenu::RANKING_DATA_LIST` | — | — | — | — |
| `MP::D2AMpMenu::RANKING_DATA_LIST_IMPL_FRIEND` | `MP::D2AMpMenu::RANKING_DATA_LIST` | `0x80790AF0` | 2 | 2 |
| `MP::D2AMpMenu::RANKING_DATA_LIST_IMPL_NEAR` | `MP::D2AMpMenu::RANKING_DATA_LIST` | `0x80790B40` | 2 | 2 |
| `MP::D2AMpMenu::RANKING_DATA_LIST_IMPL_TOP` | `MP::D2AMpMenu::RANKING_DATA_LIST` | `0x80790B18` | 2 | 2 |
| `MP::D2AMpMenuCockpit` | `atn::TaskBase`, `atn::Task` | `0x80790B78` | 7 | 6 |
| `MP::D2AMpMenuFriend` | `atn::TaskBase`, `atn::Task` | `0x80790BC8` | 7 | 6 |
| `MP::D2AMpMenuFriendRoom` | `atn::TaskBase`, `atn::Task` | `0x80790C08` | 7 | 6 |
| `MP::D2AMpMenuLobby` | `atn::TaskBase`, `atn::Task` | `0x80790CCC` | 7 | 6 |
| `MP::D2AMpMenuMessageList` | `atn::TaskBase`, `atn::Task` | `0x80790DB8` | 7 | 6 |
| `MP::D2AMpMenuRankMark` | `atn::TaskBase`, `atn::Task` | `0x80790A78` | 5 | 2 |
| `MP::D2AMpMenuResult` | `atn::TaskBase`, `atn::Task` | `0x80790EB0` | 7 | 6 |
| `MP::D2AMpMenuSetMessage` | `atn::TaskBase`, `atn::Task` | `0x80790EF0` | 7 | 6 |
| `MP::D2AMpMenuSetRule` | `atn::TaskBase`, `atn::Task` | `0x80790F30` | 7 | 6 |
| `MP::Manager2` | `atn::TaskBase`, `atn::Task` | `0x807916E0` | 5 | 4 |
| `MP::ManagerMP` | `atn::SingletonNoStatic<MP::ManagerMP>`, `atn::net::Manager` | `0x807916A0` | 4 | 4 |
| `MP::PacketPackMP` | `atn::SingletonNoStatic<MP::PacketPackMP>`, `atn::net::PacketPack` | `0x80792A28` | 5 | 5 |
| `MP::Popup` | `atn::TaskBase`, `atn::Task` | `0x80792A70` | 5 | 3 |
| `MP::SaveLoad` | `atn::TaskBase`, `atn::Task` | `0x80792AA8` | 5 | 2 |
| `MP::SoftKeyboard` | `atn::TaskBase`, `atn::Task` | `0x80792D38` | 5 | 4 |
| `MP::Text` | `atn::TaskBase`, `atn::Task` | `0x80792DC0` | 5 | 2 |
| `MP::func_ack` | `atn::net::FuncMessage` | `0x80792088` | 6 | 6 |
| `MP::func_apply_item` | `atn::net::FuncMessage` | `0x80792750` | 6 | 6 |
| `MP::func_apply_skill` | `atn::net::FuncMessage` | `0x80792788` | 6 | 6 |
| `MP::func_bot_on_team` | `atn::net::FuncMessage` | `0x807920C0` | 6 | 6 |
| `MP::func_bullet_register` | `atn::net::FuncMessage` | `0x80792910` | 6 | 6 |
| `MP::func_chat` | `atn::net::FuncMessage` | `0x80791EC8` | 6 | 6 |
| `MP::func_client_timeout` | `atn::net::FuncMessage` | `0x807921A0` | 6 | 6 |
| `MP::func_client_update` | `atn::net::FuncMessage` | `0x80792248` | 6 | 6 |
| `MP::func_damaged_enemy_sp` | `atn::net::FuncMessage` | `0x807922F0` | 6 | 6 |
| `MP::func_enemy_reinforce` | `atn::net::FuncMessage` | `0x80791E90` | 6 | 6 |
| `MP::func_enemy_status` | `atn::net::FuncMessage` | `0x80792408` | 6 | 6 |
| `MP::func_enemy_status_sp` | `atn::net::FuncMessage` | `0x807923D0` | 6 | 6 |
| `MP::func_enemy_sync` | `atn::net::FuncMessage` | `0x80792398` | 6 | 6 |
| `MP::func_enemy_update_loc` | `atn::net::FuncMessage` | `0x80792360` | 6 | 6 |
| `MP::func_enemy_update_loc_mot1` | `atn::net::FuncMessage` | `0x80792328` | 6 | 6 |
| `MP::func_enter_setting` | `atn::net::FuncMessage` | `0x80791C28` | 6 | 6 |
| `MP::func_explode_fx` | `atn::net::FuncMessage` | `0x80792830` | 6 | 6 |
| `MP::func_explode_fx_reliable` | `atn::net::FuncMessage` | `0x807927F8` | 6 | 6 |
| `MP::func_game_close` | `atn::net::FuncMessage` | `0x80792168` | 6 | 6 |
| `MP::func_game_end` | `atn::net::FuncMessage` | `0x807921D8` | 6 | 6 |
| `MP::func_game_open` | `atn::net::FuncMessage` | `0x80792130` | 6 | 6 |
| `MP::func_game_start` | `atn::net::FuncMessage` | `0x80792210` | 6 | 6 |
| `MP::func_gimmick_trigger` | `atn::net::FuncMessage` | `0x807928A0` | 6 | 6 |
| `MP::func_hit_force` | `atn::net::FuncMessage` | `0x80792868` | 6 | 6 |
| `MP::func_homing_bullet_register` | `atn::net::FuncMessage` | `0x807928D8` | 6 | 6 |
| `MP::func_notify_character` | `atn::net::FuncMessage` | `0x80791FE0` | 6 | 6 |
| `MP::func_play_message` | `atn::net::FuncMessage` | `0x807927C0` | 6 | 6 |
| `MP::func_player_beatable` | `atn::net::FuncMessage` | `0x80791FA8` | 6 | 6 |
| `MP::func_player_info` | `atn::net::FuncMessage` | `0x80792590` | 6 | 6 |
| `MP::func_player_info_for_host` | `atn::net::FuncMessage` | `0x80792558` | 6 | 6 |
| `MP::func_player_info_protector` | `atn::net::FuncMessage` | `0x80792520` | 6 | 6 |
| `MP::func_player_info_status` | `atn::net::FuncMessage` | `0x807924B0` | 6 | 6 |
| `MP::func_player_info_weapon` | `atn::net::FuncMessage` | `0x807924E8` | 6 | 6 |
| `MP::func_player_life` | `atn::net::FuncMessage` | `0x80791F70` | 6 | 6 |
| `MP::func_player_load_end` | `atn::net::FuncMessage` | `0x80792478` | 6 | 6 |
| `MP::func_player_notready` | `atn::net::FuncMessage` | `0x80792600` | 6 | 6 |
| `MP::func_player_play_end` | `atn::net::FuncMessage` | `0x80792440` | 6 | 6 |
| `MP::func_player_ready` | `atn::net::FuncMessage` | `0x80792638` | 6 | 6 |
| `MP::func_player_result_item_id` | `atn::net::FuncMessage` | `0x80791E20` | 6 | 6 |
| `MP::func_player_score` | `atn::net::FuncMessage` | `0x80791E58` | 6 | 6 |
| `MP::func_player_status` | `atn::net::FuncMessage` | `0x807929B8` | 6 | 6 |
| `MP::func_player_sync` | `atn::net::FuncMessage` | `0x80792980` | 6 | 6 |
| `MP::func_player_team` | `atn::net::FuncMessage` | `0x807925C8` | 6 | 6 |
| `MP::func_player_update_loc` | `atn::net::FuncMessage` | `0x807929F0` | 6 | 6 |
| `MP::func_player_update_loc_mot1` | `atn::net::FuncMessage` | `0x80792948` | 6 | 6 |
| `MP::func_player_warp` | `atn::net::FuncMessage` | `0x80791F00` | 6 | 6 |
| `MP::func_player_weapon_temp` | `atn::net::FuncMessage` | `0x807922B8` | 6 | 6 |
| `MP::func_query_team_request` | `atn::net::FuncMessage` | `0x80792050` | 6 | 6 |
| `MP::func_request_character` | `atn::net::FuncMessage` | `0x80792018` | 6 | 6 |
| `MP::func_request_game_start` | `atn::net::FuncMessage` | `0x80791CD0` | 6 | 6 |
| `MP::func_request_time` | `atn::net::FuncMessage` | `0x80791C60` | 6 | 6 |
| `MP::func_resolve_team_balance` | `atn::net::FuncMessage` | `0x80792280` | 6 | 6 |
| `MP::func_responce_game_start` | `atn::net::FuncMessage` | `0x80791C98` | 6 | 6 |
| `MP::func_rule_gamestart` | `atn::net::FuncMessage` | `0x80792670` | 6 | 6 |
| `MP::func_rule_match_type` | `atn::net::FuncMessage` | `0x80791F38` | 6 | 6 |
| `MP::func_rule_player_number` | `atn::net::FuncMessage` | `0x807920F8` | 6 | 6 |
| `MP::func_rule_score_limit` | `atn::net::FuncMessage` | `0x807926A8` | 6 | 6 |
| `MP::func_rule_stage` | `atn::net::FuncMessage` | `0x80792718` | 6 | 6 |
| `MP::func_rule_time_limit` | `atn::net::FuncMessage` | `0x807926E0` | 6 | 6 |
| `MP::func_start_result` | `atn::net::FuncMessage` | `0x80791D08` | 6 | 6 |
| `MP::func_sync_complete` | `atn::net::FuncMessage` | `0x80791D40` | 6 | 6 |
| `MP::func_sync_dummy` | `atn::net::FuncMessage` | `0x80791D78` | 6 | 6 |
| `MP::func_syncallmessage_host` | `atn::net::FuncMessage` | `0x80791DE8` | 6 | 6 |
| `MP::func_syncallmessage_lobby_host` | `atn::net::FuncMessage` | `0x80791DB0` | 6 | 6 |

## Metrowerks

| Clase | Bases | vtable | Métodos | Propias |
|---|---|---|---|---|
| `Metrowerks::bufferedbuf<char, std::char_traits<char>>` | `std::basic_streambuf<char, std::char_traits<char>>` | `0x807BBCC8` | 16 | 10 |
| `Metrowerks::bufferedbuf<wchar_t, std::char_traits<wchar_t>>` | `std::basic_streambuf<wchar_t, std::char_traits<wchar_t>>` | `0x807BBB90` | 16 | 10 |
| `Metrowerks::console_inputbuf<char, std::char_traits<char>>` | `std::basic_streambuf<char, std::char_traits<char>>` | `0x807BBD1C` | 13 | 8 |
| `Metrowerks::console_inputbuf<wchar_t, std::char_traits<wchar_t>>` | `std::basic_streambuf<wchar_t, std::char_traits<wchar_t>>` | `0x807BBBE4` | 13 | 8 |
| `Metrowerks::console_outputbuf<char, std::char_traits<char>>` | `std::basic_streambuf<char, std::char_traits<char>>`, `Metrowerks::bufferedbuf<char, std::char_traits<char>>` | `0x807BBC68` | 16 | 5 |
| `Metrowerks::console_outputbuf<wchar_t, std::char_traits<wchar_t>>` | `std::basic_streambuf<wchar_t, std::char_traits<wchar_t>>`, `Metrowerks::bufferedbuf<wchar_t, std::char_traits<wchar_t>>` | `0x807BBB30` | 16 | 5 |

## atn

| Clase | Bases | vtable | Métodos | Propias |
|---|---|---|---|---|
| `atn::AABBTree` | — | `0x807774F0` | 6 | 6 |
| `atn::AudioDevice` | — | `0x80777520` | 2 | 1 |
| `atn::BaseNode` | — | — | — | — |
| `atn::Bgm` | — | `0x80777570` | 2 | 1 |
| `atn::ColliBase` | — | `0x807776A0` | 6 | 6 |
| `atn::ColliBox` | `atn::ColliBase` | `0x807775F8` | 6 | 6 |
| `atn::ColliCapsule` | `atn::ColliBase` | `0x80777630` | 6 | 6 |
| `atn::ColliObject` | `atn::ColliBase` | `0x807775C0` | 6 | 6 |
| `atn::ColliSphere` | `atn::ColliBase` | `0x80777668` | 6 | 6 |
| `atn::ColliTree` | `atn::ColliBase` | `0x80777588` | 6 | 5 |
| `atn::CompositeIntensityTintMulti` | `atn::ITextureCompositor` | `0x80779830` | 5 | 5 |
| `atn::ConstantCurve` | `atn::Curve` | `0x80777728` | 4 | 4 |
| `atn::CosinusCurve` | `atn::Curve` | `0x80777758` | 4 | 4 |
| `atn::Csv` | `atn::CsvBase<char, 44, 13, 34>` | `0x807772B0` | 1 | 0 |
| `atn::CsvBase<char, 124, 13, 34>` | — | `0x80782FD0` | 2 | 1 |
| `atn::CsvBase<char, 44, 13, 34>` | — | `0x807772D0` | 2 | 1 |
| `atn::CsvBase<char, 9, 13, 34>` | — | `0x807774D8` | 2 | 1 |
| `atn::CsvBase<wchar_t, 44, 13, 34>` | — | `0x8077A090` | 2 | 1 |
| `atn::CsvBase<wchar_t, 9, 13, 34>` | — | `0x8078B608` | 2 | 1 |
| `atn::CsvPipe` | `atn::CsvBase<char, 124, 13, 34>` | `0x80782FB0` | 1 | 0 |
| `atn::CsvTab` | `atn::CsvBase<char, 9, 13, 34>` | `0x807774B8` | 1 | 0 |
| `atn::CsvTabU` | `atn::CsvBase<wchar_t, 9, 13, 34>` | `0x8078B5E8` | 1 | 1 |
| `atn::CsvU` | `atn::CsvBase<wchar_t, 44, 13, 34>` | `0x8077A070` | 1 | 1 |
| `atn::Curve` | — | — | — | — |
| `atn::D2A_Common` | — | `0x80782A50` | 8 | 7 |
| `atn::D2A_Data` | — | `0x80782A80` | 4 | 4 |
| `atn::D2A_Frame` | `atn::D2A_Common` | `0x80782AA0` | 7 | 2 |
| `atn::D2A_Line` | `atn::D2A_Common` | `0x80782AD8` | 7 | 2 |
| `atn::D2A_Manager` | `atn::TaskBase`, `atn::Task` | `0x80782B10` | 5 | 4 |
| `atn::D2A_Message` | `atn::D2A_Common` | `0x80782B68` | 7 | 4 |
| `atn::D2A_Reference` | `atn::D2A_Common` | `0x80782BD0` | 7 | 2 |
| `atn::D2A_Task` | `atn::TaskBase`, `atn::Task` | `0x80782C08` | 5 | 4 |
| `atn::D2A_TaskInstanceBase<32, 8>` | `atn::TaskBase`, `atn::Task`, `atn::ID2A_TaskInstance` | `0x80782CB0` | 5 | 4 |
| `atn::D2A_TaskInstanceBase<64, 8>` | `atn::TaskBase`, `atn::Task`, `atn::ID2A_TaskInstance` | `0x80782CF0` | 5 | 4 |
| `atn::D2A_TaskInstanceBase<8, 2>` | `atn::TaskBase`, `atn::Task`, `atn::ID2A_TaskInstance` | `0x80782C70` | 5 | 4 |
| `atn::D2A_Texture` | `atn::D2A_Common` | `0x80782D80` | 7 | 7 |
| `atn::D2A_Window` | `atn::D2A_Common` | `0x80782DB8` | 7 | 2 |
| `atn::D3Anime` | `atn::TaskBase`, `atn::Task` | `0x80782DF0` | 5 | 4 |
| `atn::D3AnimeInstance` | `atn::TaskBase`, `atn::Task` | `0x80782E28` | 5 | 4 |
| `atn::DLAllocator` | `IAllocator` | `0x807785D8` | 6 | 6 |
| `atn::DrawHelper` | — | `0x807777C0` | 2 | 1 |
| `atn::DrawHelper::ComDrawColliObject` | `atn::IRender`, `atn::DrawHelper::DrawCommand` | `0x80777898` | 6 | 6 |
| `atn::DrawHelper::ComDrawColliTree` | `atn::IRender`, `atn::DrawHelper::DrawCommand` | `0x80777858` | 6 | 6 |
| `atn::DrawHelper::ComDrawD3TextureMask` | `atn::IRender`, `atn::DrawHelper::DrawCommand` | `0x80777818` | 6 | 6 |
| `atn::DrawHelper::ComDrawFont` | `atn::IRender`, `atn::DrawHelper::DrawCommand` | `0x80777920` | 6 | 6 |
| `atn::DrawHelper::ComDrawFontSideFade` | `atn::IRender`, `atn::DrawHelper::DrawCommand`, `atn::DrawHelper::ComDrawFont` | `0x807778D8` | 6 | 2 |
| `atn::DrawHelper::ComDrawLine` | `atn::IRender`, `atn::DrawHelper::DrawCommand` | `0x80777AB0` | 6 | 6 |
| `atn::DrawHelper::ComDrawTexture` | `atn::IRender`, `atn::DrawHelper::DrawCommand` | `0x80777A70` | 6 | 6 |
| `atn::DrawHelper::ComDrawTextureAdd` | `atn::IRender`, `atn::DrawHelper::DrawCommand` | `0x80777A30` | 6 | 6 |
| `atn::DrawHelper::ComDrawTextureMask` | `atn::IRender`, `atn::DrawHelper::DrawCommand` | `0x807779F0` | 6 | 6 |
| `atn::DrawHelper::ComDrawTextureMaskMono` | `atn::IRender`, `atn::DrawHelper::DrawCommand`, `atn::DrawHelper::ComDrawTextureMask` | `0x80777960` | 6 | 2 |
| `atn::DrawHelper::ComDrawTextureMono` | `atn::IRender`, `atn::DrawHelper::DrawCommand`, `atn::DrawHelper::ComDrawTexture` | `0x807779A8` | 6 | 2 |
| `atn::DrawHelper::ComDrawWindow` | `atn::IRender`, `atn::DrawHelper::DrawCommand` | `0x807777D8` | 6 | 6 |
| `atn::DrawHelper::DrawCommand` | `atn::IRender` | — | — | — |
| `atn::EffectManager` | `atn::IRender`, `atn::TaskBase`, `atn::Task` | `0x80783BB8` | 5 | 3 |
| `atn::EffectManagerCallback` | — | — | — | — |
| `atn::FadeCapture` | `atn::TaskBase`, `atn::Task`, `atn::FadeCommon` | `0x80777B08` | 5 | 3 |
| `atn::FadeColor` | `atn::TaskBase`, `atn::Task`, `atn::FadeCommon` | `0x80777B48` | 5 | 2 |
| `atn::FadeCommon` | `atn::TaskBase`, `atn::Task` | `0x80777B88` | 5 | 4 |
| `atn::FileList` | — | `0x80777BC0` | 2 | 1 |
| `atn::ID2A_TaskInstance` | `atn::TaskBase`, `atn::Task` | — | — | — |
| `atn::IPostFx` | `atn::IRender` | — | — | — |
| `atn::IRender` | — | — | — | — |
| `atn::IRenderLayer` | — | — | — | — |
| `atn::ITextureCacheManager` | — | — | — | — |
| `atn::ITextureCompositor` | — | — | — | — |
| `atn::LH_Binary` | `atn::LoadHandle` | `0x8078FBB0` | 2 | 2 |
| `atn::LH_ColliObject` | `atn::LoadHandle` | `0x8078FC38` | 2 | 2 |
| `atn::LH_ColliTree` | `atn::LoadHandle` | `0x8078FC90` | 2 | 2 |
| `atn::LH_Effect` | `atn::LoadHandle` | `0x8078FCF8` | 2 | 2 |
| `atn::LH_Efp` | `atn::LoadHandle` | `0x8078FD48` | 2 | 2 |
| `atn::LH_Material` | `atn::LoadHandle` | `0x8078FDA8` | 2 | 2 |
| `atn::LH_Model` | `atn::LoadHandle` | `0x8078FE00` | 2 | 2 |
| `atn::LH_Motion` | `atn::LoadHandle` | `0x8078FE50` | 2 | 2 |
| `atn::LH_MotionCommandData` | `atn::LoadHandle` | `0x8078FEA0` | 2 | 2 |
| `atn::LH_MotionSet` | `atn::LoadHandle` | `0x8078FEF0` | 2 | 2 |
| `atn::LH_Scene` | `atn::LoadHandle` | `0x8078FF60` | 2 | 2 |
| `atn::LH_Texture` | `atn::LoadHandle` | `0x8078FFB0` | 2 | 2 |
| `atn::LH_Xsca` | `atn::LoadHandle` | `0x80790000` | 2 | 2 |
| `atn::Layer2D` | `atn::IRenderLayer` | `0x80779048` | 7 | 7 |
| `atn::Layer2D3D` | `atn::IRenderLayer`, `atn::Layer3D` | `0x80778F90` | 7 | 2 |
| `atn::Layer2DBlur` | `atn::IRenderLayer`, `atn::Layer2D` | `0x80778F30` | 7 | 3 |
| `atn::Layer3D` | `atn::IRenderLayer` | `0x8077906C` | 7 | 7 |
| `atn::LayerOcclusionCPU` | `atn::IRenderLayer` | `0x807790B0` | 7 | 7 |
| `atn::LayerPost3D` | `atn::IRenderLayer`, `atn::Layer3D` | `0x807790E8` | 7 | 2 |
| `atn::LayerPostFx` | `atn::IRenderLayer` | `0x80779128` | 7 | 7 |
| `atn::LayerPrerender` | `atn::IRenderLayer` | `0x80779010` | 7 | 7 |
| `atn::LayerReflect` | `atn::IRenderLayer`, `atn::Layer3D` | `0x80779160` | 7 | 3 |
| `atn::LayerShadow` | `atn::IRenderLayer`, `atn::Layer3D` | `0x807791A0` | 7 | 4 |
| `atn::LayerZPrePass` | `atn::IRenderLayer`, `atn::Layer3D` | `0x80778FD0` | 7 | 3 |
| `atn::LensFlare` | `atn::IRender` | `0x80777D08` | 6 | 6 |
| `atn::LinearAllocator` | `IAllocator` | `0x80778CA8` | 6 | 6 |
| `atn::LoadColliObject` | `atn::LoadCore` | `0x8078FC10` | 3 | 2 |
| `atn::LoadColliTree` | `atn::LoadCore` | `0x8078FC68` | 3 | 2 |
| `atn::LoadCore` | — | `0x8078FCB8` | 4 | 3 |
| `atn::LoadEffect` | `atn::LoadCore` | `0x8078FCD0` | 3 | 2 |
| `atn::LoadEfp` | `atn::LoadCore` | `0x8078FD20` | 3 | 2 |
| `atn::LoadHandle` | — | `0x8078FD70` | 2 | 1 |
| `atn::LoadMaterial` | `atn::LoadCore` | `0x8078FD80` | 3 | 2 |
| `atn::LoadModel` | `atn::LoadCore` | `0x8078FDD8` | 3 | 2 |
| `atn::LoadMotion` | `atn::LoadCore` | `0x8078FE28` | 3 | 2 |
| `atn::LoadMotionCommandData` | `atn::LoadCore` | `0x8078FE78` | 3 | 2 |
| `atn::LoadMotionSet` | `atn::LoadCore` | `0x8078FEC8` | 3 | 2 |
| `atn::LoadScene` | `atn::LoadCore` | `0x8078FF38` | 3 | 2 |
| `atn::LoadTexture` | `atn::LoadCore` | `0x8078FF88` | 3 | 2 |
| `atn::LoadXsca` | `atn::LoadCore` | `0x8078FFD8` | 3 | 2 |
| `atn::Loader` | `atn::TaskBase`, `atn::Task` | `0x8078FB70` | 5 | 4 |
| `atn::MindowsBool` | `atn::MindowsCommon` | `0x807786E8` | 10 | 3 |
| `atn::MindowsCommon` | — | `0x80778808` | 10 | 8 |
| `atn::MindowsFlag` | `atn::MindowsCommon` | `0x807786A0` | 10 | 3 |
| `atn::MindowsFloat` | `atn::MindowsCommon` | `0x80778730` | 10 | 5 |
| `atn::MindowsFolder` | `atn::MindowsCommon` | `0x80778610` | 10 | 7 |
| `atn::MindowsInt` | `atn::MindowsCommon` | `0x807787C0` | 10 | 5 |
| `atn::MindowsRadian` | `atn::MindowsCommon` | `0x80778658` | 10 | 5 |
| `atn::MindowsUInt` | `atn::MindowsCommon` | `0x80778778` | 10 | 5 |
| `atn::Model` | `atn::IRender`, `atn::ModelBase` | `0x80778888` | 8 | 3 |
| `atn::ModelBase` | `atn::IRender` | `0x807788D0` | 8 | 8 |
| `atn::ModelInstance` | `atn::IRender` | `0x80778958` | 6 | 6 |
| `atn::NandManager` | `atn::TaskBase`, `atn::Task` | `0x80778A9C` | 5 | 3 |
| `atn::OperatorNode` | `atn::BaseNode` | `0x80778B38` | 3 | 3 |
| `atn::PBoneBasic` | `atn::PBoneInterface` | `0x80778C10` | 12 | 12 |
| `atn::PBoneInterface` | — | — | — | — |
| `atn::PBoneOutForce` | `atn::PBoneInterface`, `atn::PBoneBasic` | `0x80778BB8` | 12 | 3 |
| `atn::ParseTree` | — | `0x80778B20` | 2 | 2 |
| `atn::Polynomial1Curve` | `atn::Curve` | `0x807776F8` | 4 | 4 |
| `atn::Polynomial2Curve` | `atn::Curve` | `0x807776C8` | 4 | 4 |
| `atn::PostFxBloom` | `atn::IRender`, `atn::IPostFx` | `0x80778CE0` | 6 | 6 |
| `atn::PostFxColorMatrix` | `atn::IRender`, `atn::IPostFx` | `0x80778D38` | 6 | 6 |
| `atn::PostFxDamage` | `atn::IRender`, `atn::IPostFx` | `0x80778D78` | 6 | 6 |
| `atn::PostFxDof` | `atn::IRender`, `atn::IPostFx` | `0x80778DB8` | 6 | 6 |
| `atn::PostFxGamma` | `atn::IRender`, `atn::IPostFx` | `0x80778DF8` | 6 | 6 |
| `atn::PostFxGodRay` | `atn::IRender`, `atn::IPostFx` | `0x80778E38` | 6 | 6 |
| `atn::PostFxMotionBlur` | `atn::IRender`, `atn::IPostFx` | `0x80778E78` | 6 | 6 |
| `atn::PostFxRain` | `atn::IRender`, `atn::IPostFx` | `0x80778EB8` | 6 | 6 |
| `atn::Rain` | `atn::IRender` | `0x80778EF8` | 6 | 6 |
| `atn::SequenceControl` | `atn::TaskBase`, `atn::Task` | `0x807792C0` | 5 | 5 |
| `atn::SequenceHolder::SequenceUnit` | `atn::SequenceHolder::SequenceUnitBase` | `0x80779308` | 2 | 2 |
| `atn::SequenceHolder::SequenceUnitBase` | — | `0x80779330` | 2 | 2 |
| `atn::SequenceHolder::SequenceUnitParam1<const GameLoadTask::Option>` | `atn::SequenceHolder::SequenceUnitBase` | `0x80796718` | 2 | 2 |
| `atn::SequenceHolder::SequenceUnitParam1<const GameTask::Option>` | `atn::SequenceHolder::SequenceUnitBase` | `0x80796740` | 2 | 2 |
| `atn::SequenceHolder::SequenceUnitParam1<const int>` | `atn::SequenceHolder::SequenceUnitBase` | `0x80796768` | 2 | 2 |
| `atn::Singleton<Event::BehaviorFollow>` | — | — | — | — |
| `atn::Singleton<Event::BehaviorGesture>` | — | — | — | — |
| `atn::Singleton<Event::BehaviorMoveTo>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionActorBehavior>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionActorEyes>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionActorFacial>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionActorHeading>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionActorMaterial>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionActorMotion>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionActorNode>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionActorPose>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionActorPosition>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionActorSetting>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionActorWind>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionAmbientLight>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionAnimationPlay>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionAudioEffect>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionBrightness>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionCameraChange>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionCameraEffect>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionCameraFocus>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionCameraInertia>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionCameraMode>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionCameraMotion>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionCameraSetting>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionCameraShake>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionCheck>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionCloudMap>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionColorGradation>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionColorMatrix>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionCutAdjust>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionDialogSpeaker>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionDialogText>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionEffectPlay>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionEnvSound>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionGameSetting>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionGodRay>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionLightSurround>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionLocatorHide>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionMapReflect>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionMessageChoice>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionMessagePrint>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionMessageWindow>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionMoviePlay>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionMultiAudio>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionMusicPlay>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionNotify>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionPointLight>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionSoundPlay>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionSpotLight>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionSunLight>` | — | — | — | — |
| `atn::SingletonNoStatic<Event::ActionVoicePlay>` | — | — | — | — |
| `atn::SingletonNoStatic<MP::ManagerMP>` | — | — | — | — |
| `atn::SingletonNoStatic<MP::PacketPackMP>` | — | — | — | — |
| `atn::SinusCurve` | `atn::Curve` | `0x80777788` | 4 | 4 |
| `atn::SmallAllocator<6>` | `IAllocator` | `0x80778560` | 6 | 6 |
| `atn::SmallAllocator<8>` | `IAllocator` | `0x80778598` | 6 | 6 |
| `atn::SoftKeyboardBase<wchar_t, 0>` | — | `0x807794D8` | 4 | 3 |
| `atn::SoftKeyboardU` | `atn::SoftKeyboardBase<wchar_t, 0>` | `0x80792D70` | 3 | 1 |
| `atn::Sound3dActor` | — | `0x80779510` | 2 | 1 |
| `atn::Sound3dEngine` | `nw4r::snd::detail::Sound3DEngineInterface`, `nw4r::snd::Sound3DEngine` | `0x807795C0` | 5 | 5 |
| `atn::Sound3dListener` | — | `0x80779528` | 2 | 1 |
| `atn::SoundCore` | — | `0x807794F8` | 2 | 1 |
| `atn::SoundManager` | — | `0x80779698` | 2 | 1 |
| `atn::Task` | `atn::TaskBase` | `0x807796D0` | 6 | 5 |
| `atn::TaskBase` | — | `0x807796F0` | 2 | 1 |
| `atn::TextAnimation` | `atn::TaskBase`, `atn::Task` | `0x80779868` | 5 | 4 |
| `atn::TextureCacheManager4x128` | `atn::ITextureCacheManager` | `0x80779728` | 2 | 2 |
| `atn::TextureCacheManager8x32` | `atn::ITextureCacheManager` | `0x80779700` | 2 | 2 |
| `atn::TextureComposite` | `atn::IRender` | `0x80779758` | 6 | 6 |
| `atn::TextureCompositeCommand` | — | — | — | — |
| `atn::TextureCompositeCopy` | `atn::TextureCompositeCommand` | `0x807797B8` | 2 | 2 |
| `atn::TextureCompositeCopyAlpha` | `atn::TextureCompositeCommand` | `0x80779790` | 2 | 2 |
| `atn::TextureCompositeMaskIntensitySingle` | `atn::TextureCompositeCommand` | `0x807797E0` | 2 | 2 |
| `atn::ValueNode` | `atn::BaseNode` | `0x80778B88` | 3 | 3 |
| `atn::VariableNode` | `atn::BaseNode` | `0x80778B60` | 3 | 3 |
| `atn::Video` | `atn::IRender` | — | — | — |
| `atn::VideoTHP` | `atn::IRender`, `atn::Video` | `0x807798D8` | 14 | 14 |
| `atn::net::Crypt` | — | `0x80790908` | 6 | 1 |
| `atn::net::FuncMessage` | — | — | — | — |
| `atn::net::Manager` | — | `0x807930F0` | 4 | 4 |
| `atn::net::PPDATA` | — | `0x80793168` | 2 | 1 |
| `atn::net::PPDATAR` | `atn::net::PPDATA` | `0x80793118` | 2 | 2 |
| `atn::net::PPDATAU` | `atn::net::PPDATA` | `0x80793140` | 2 | 2 |
| `atn::net::PacketPack` | — | `0x80791718` | 6 | 5 |
| `atn::ptr_string_base<char, 0>` | — | `0x807835B8` | 2 | 1 |
| `atn::ptr_string_base<wchar_t, 0>` | — | `0x80782FF0` | 2 | 1 |

## boost

| Clase | Bases | vtable | Métodos | Propias |
|---|---|---|---|---|
| `boost::bad_function_call` | `std::exception`, `std::runtime_error` | `0x80775B30` | 2 | 1 |
| `boost::bad_lexical_cast` | `std::exception`, `std::bad_cast` | `0x8078B160` | 2 | 2 |
| `boost::detail::sp_counted_base` | — | — | — | — |
| `boost::detail::sp_counted_impl_p<Event::Action>` | `boost::detail::sp_counted_base` | `0x80793B70` | 4 | 4 |
| `boost::detail::sp_counted_impl_p<Event::Channel>` | `boost::detail::sp_counted_base` | `0x80793FD8` | 4 | 4 |
| `boost::detail::sp_counted_impl_p<Event::Cut>` | `boost::detail::sp_counted_base` | `0x807943B0` | 4 | 4 |
| `boost::detail::sp_counted_impl_p<Event::Label>` | `boost::detail::sp_counted_base` | `0x80793D68` | 4 | 4 |
| `boost::detail::sp_counted_impl_p<Event::Section>` | `boost::detail::sp_counted_base` | `0x80793D38` | 4 | 4 |

## chr

| Clase | Bases | vtable | Métodos | Propias |
|---|---|---|---|---|
| `chr::ControlAbsorbed` | `chr::ControlBase` | `0x8077E9B8` | 22 | 2 |
| `chr::ControlAbsorbedHP` | `chr::ControlBase` | `0x8077E0D0` | 22 | 3 |
| `chr::ControlAbsorbedHPReleased` | `chr::ControlBase` | `0x8077E058` | 22 | 2 |
| `chr::ControlAlleyWay` | `chr::ControlBase` | `0x80781528` | 22 | 2 |
| `chr::ControlAppear` | `chr::ControlBase` | `0x8077CEB0` | 22 | 5 |
| `chr::ControlAppearCoffin` | `chr::ControlBase` | `0x8077CD48` | 22 | 2 |
| `chr::ControlAppearEnd` | `chr::ControlBase` | `0x8077CE38` | 22 | 3 |
| `chr::ControlAppearSkelton` | `chr::ControlBase` | `0x8077CDC0` | 22 | 4 |
| `chr::ControlAttack` | `chr::ControlBase`, `chr::ControlAttackBase` | `0x8077D4F8` | 23 | 2 |
| `chr::ControlAttackBase` | `chr::ControlBase` | `0x8077D578` | 23 | 10 |
| `chr::ControlAttackLastOne` | `chr::ControlBase`, `chr::ControlAttackBase` | `0x8077CF80` | 23 | 3 |
| `chr::ControlAttackNext` | `chr::ControlBase`, `chr::ControlAttackBase` | `0x8077D478` | 23 | 2 |
| `chr::ControlAttackSetup` | `chr::ControlBase` | `0x8077D400` | 22 | 2 |
| `chr::ControlAttractHateCharge` | `chr::ControlBase` | `0x80780F48` | 22 | 3 |
| `chr::ControlAvoid` | `chr::ControlBase` | `0x8077FF88` | 22 | 2 |
| `chr::ControlBackStep` | `chr::ControlBase` | `0x807803A8` | 22 | 3 |
| `chr::ControlBase` | — | `0x8077DC90` | 23 | 21 |
| `chr::ControlBoundShade` | `chr::ControlBase` | `0x8077E1C0` | 22 | 3 |
| `chr::ControlBreakOpen` | `chr::ControlBase` | `0x8077CC80` | 22 | 3 |
| `chr::ControlBumpHead` | `chr::ControlBase`, `chr::ControlInteractionBase` | `0x80781140` | 22 | 1 |
| `chr::ControlCannonLift` | `chr::ControlBase` | `0x8077F0C8` | 22 | 4 |
| `chr::ControlCannonTarget` | `chr::ControlBase`, `chr::ControlMoveCursor` | `0x8077EFD0` | 22 | 3 |
| `chr::ControlCannonThrow` | `chr::ControlBase` | `0x8077F050` | 22 | 2 |
| `chr::ControlCaught` | `chr::ControlBase` | `0x8077F210` | 22 | 4 |
| `chr::ControlCaughtSkelton` | `chr::ControlBase` | `0x8077E490` | 22 | 2 |
| `chr::ControlCdnBetray` | `chr::ControlBase` | `0x8077D980` | 22 | 4 |
| `chr::ControlCdnParalyzed` | `chr::ControlBase` | `0x8077DA70` | 22 | 4 |
| `chr::ControlCdnPiyo` | `chr::ControlBase` | `0x8077D818` | 22 | 3 |
| `chr::ControlCdnPsycho` | `chr::ControlBase` | `0x8077D728` | 22 | 3 |
| `chr::ControlCdnRestoreParalyzed` | `chr::ControlBase` | `0x8077D9F8` | 22 | 2 |
| `chr::ControlCdnRestorePiyo` | `chr::ControlBase` | `0x8077D7A0` | 22 | 3 |
| `chr::ControlCdnRestorePsycho` | `chr::ControlBase` | `0x8077D6B0` | 22 | 3 |
| `chr::ControlCdnRestoreSleep` | `chr::ControlBase` | `0x8077D890` | 22 | 3 |
| `chr::ControlCdnRestoreStone` | `chr::ControlBase` | `0x8077DAE8` | 22 | 2 |
| `chr::ControlCdnSleep` | `chr::ControlBase` | `0x8077D908` | 22 | 3 |
| `chr::ControlCdnStone` | `chr::ControlBase` | `0x8077DB60` | 22 | 3 |
| `chr::ControlChant` | `chr::ControlBase`, `chr::ControlMoveCursor` | `0x80781838` | 22 | 4 |
| `chr::ControlCharge` | `chr::ControlBase`, `chr::ControlMoveCursor` | `0x8077F578` | 22 | 3 |
| `chr::ControlChargeAttack` | `chr::ControlBase`, `chr::ControlChargeAttackBase` | `0x8077F480` | 22 | 2 |
| `chr::ControlChargeAttackBase` | `chr::ControlBase` | `0x8077F500` | 22 | 8 |
| `chr::ControlChaseAssist` | `chr::ControlBase` | `0x8077F770` | 22 | 3 |
| `chr::ControlChaseCatch` | `chr::ControlBase` | `0x8077F8D8` | 22 | 3 |
| `chr::ControlChaseCaught` | `chr::ControlBase` | `0x8077FA40` | 22 | 3 |
| `chr::ControlChaseFollow` | `chr::ControlBase` | `0x8077F6F8` | 22 | 3 |
| `chr::ControlChaseHeld` | `chr::ControlBase` | `0x8077F9C8` | 22 | 3 |
| `chr::ControlChaseHold` | `chr::ControlBase` | `0x8077F860` | 22 | 3 |
| `chr::ControlChaseRelease` | `chr::ControlBase` | `0x8077F950` | 22 | 3 |
| `chr::ControlChaseReleased` | `chr::ControlBase` | `0x8077F7E8` | 22 | 3 |
| `chr::ControlCliffhang` | `chr::ControlBase` | `0x8077FB60` | 22 | 4 |
| `chr::ControlCliffhangZanGurg` | `chr::ControlBase` | `0x8077DFE0` | 22 | 4 |
| `chr::ControlClimbDeck` | `chr::ControlBase` | `0x807814B0` | 22 | 2 |
| `chr::ControlConstrict` | `chr::ControlBase`, `chr::ControlInteractionBase` | `0x807811C0` | 22 | 1 |
| `chr::ControlCoverSlip` | `chr::ControlBase` | `0x80780510` | 22 | 2 |
| `chr::ControlCrawling` | `chr::ControlBase` | `0x80781438` | 22 | 2 |
| `chr::ControlDamage` | `chr::ControlBase` | `0x80780078` | 22 | 2 |
| `chr::ControlDamageLastOne` | `chr::ControlBase` | `0x8077FC40` | 22 | 3 |
| `chr::ControlDashAttack` | `chr::ControlBase`, `chr::ControlAttackBase` | `0x8077D380` | 23 | 2 |
| `chr::ControlDetect` | `chr::ControlBase` | `0x807802B8` | 22 | 3 |
| `chr::ControlDetectHateAtt` | `chr::ControlBase` | `0x80780240` | 22 | 2 |
| `chr::ControlDive` | `chr::ControlBase` | `0x80780600` | 22 | 3 |
| `chr::ControlDiveIntoWater` | `chr::ControlBase`, `chr::ControlInteractionBase` | `0x80780FC0` | 22 | 3 |
| `chr::ControlDragged` | `chr::ControlBase` | `0x80780690` | 22 | 2 |
| `chr::ControlDraggedStart` | `chr::ControlBase` | `0x80780708` | 22 | 3 |
| `chr::ControlDraggingStart` | `chr::ControlBase` | `0x80780780` | 22 | 2 |
| `chr::ControlDropDown` | `chr::ControlBase` | `0x80780898` | 22 | 2 |
| `chr::ControlDropDownQuadruped` | `chr::ControlBase`, `chr::ControlDropDown` | `0x80780818` | 22 | 2 |
| `chr::ControlDuckJoist` | `chr::ControlBase`, `chr::ControlInteractionBase` | `0x80781240` | 22 | 1 |
| `chr::ControlDuckUnderA` | `chr::ControlBase`, `chr::ControlInteractionBase` | `0x80781340` | 22 | 2 |
| `chr::ControlDuckUnderB` | `chr::ControlBase`, `chr::ControlInteractionBase` | `0x807812C0` | 22 | 1 |
| `chr::ControlEndCliffhangZanGurg` | `chr::ControlBase` | `0x8077DF68` | 22 | 3 |
| `chr::ControlFallDead` | `chr::ControlBase` | `0x8077FD30` | 22 | 2 |
| `chr::ControlGetUp` | `chr::ControlBase` | `0x8077FE98` | 22 | 2 |
| `chr::ControlGmkBreakArmor` | `chr::ControlBase` | `0x80780928` | 22 | 2 |
| `chr::ControlGmkCoffinGetUp` | `chr::ControlBase` | `0x80780A18` | 22 | 2 |
| `chr::ControlGmkCoffinOpen` | `chr::ControlBase` | `0x80780A90` | 22 | 2 |
| `chr::ControlGmkCoffinOpening` | `chr::ControlBase` | `0x80780B08` | 22 | 2 |
| `chr::ControlGmkTrapBat` | `chr::ControlBase` | `0x807809A0` | 22 | 3 |
| `chr::ControlGrapple` | `chr::ControlBase` | `0x80780D68` | 22 | 3 |
| `chr::ControlGrappleThrown` | `chr::ControlBase` | `0x80780C78` | 22 | 2 |
| `chr::ControlGrappled` | `chr::ControlBase` | `0x80780CF0` | 22 | 3 |
| `chr::ControlGrappledThrow` | `chr::ControlBase` | `0x80780C00` | 22 | 2 |
| `chr::ControlGuard` | `chr::ControlBase` | `0x80780000` | 22 | 2 |
| `chr::ControlGuardCounter` | `chr::ControlBase`, `chr::ControlAttackBase` | `0x8077D100` | 23 | 3 |
| `chr::ControlGuardMountle` | `chr::ControlBase` | `0x80781A00` | 22 | 3 |
| `chr::ControlGuarded` | `chr::ControlBase` | `0x8077FCB8` | 22 | 2 |
| `chr::ControlHighJumpAttack` | `chr::ControlBase`, `chr::ControlChargeAttackBase` | `0x8077F400` | 22 | 3 |
| `chr::ControlHitBackBullet` | `chr::ControlBase`, `chr::ControlAttackBase` | `0x8077D080` | 23 | 3 |
| `chr::ControlInteractionBase` | `chr::ControlBase` | `0x807813C0` | 22 | 2 |
| `chr::ControlJump` | `chr::ControlBase` | `0x80781910` | 22 | 4 |
| `chr::ControlJumpAt` | `chr::ControlBase` | `0x80780DE0` | 22 | 3 |
| `chr::ControlJumpAttack` | `chr::ControlBase`, `chr::ControlAttackBase` | `0x8077D300` | 23 | 3 |
| `chr::ControlJumpAttack2` | `chr::ControlBase`, `chr::ControlAttackBase` | `0x8077D280` | 23 | 3 |
| `chr::ControlKnockdown` | `chr::ControlBase` | `0x8077FF10` | 22 | 4 |
| `chr::ControlLadder` | `chr::ControlBase` | `0x807815A0` | 22 | 4 |
| `chr::ControlLookAround` | `chr::ControlBase` | `0x80781690` | 22 | 2 |
| `chr::ControlLookAroundEgg` | `chr::ControlBase` | `0x80781618` | 22 | 2 |
| `chr::ControlMagic` | `chr::ControlBase` | `0x807817C0` | 22 | 9 |
| `chr::ControlMagicEnd` | `chr::ControlBase`, `chr::ControlDropDown` | `0x80781740` | 22 | 1 |
| `chr::ControlMatHold` | `chr::ControlBase` | `0x8077E760` | 22 | 4 |
| `chr::ControlMatHoldDamage` | `chr::ControlBase` | `0x8077E670` | 22 | 2 |
| `chr::ControlMatHoldGuard` | `chr::ControlBase` | `0x8077E850` | 22 | 4 |
| `chr::ControlMatHoldPre` | `chr::ControlBase` | `0x8077E7D8` | 22 | 2 |
| `chr::ControlMatHoldRelease` | `chr::ControlBase` | `0x8077E6E8` | 22 | 2 |
| `chr::ControlMountle` | `chr::ControlBase` | `0x80781AF8` | 23 | 4 |
| `chr::ControlMountleQuadruped` | `chr::ControlBase`, `chr::ControlMountle` | `0x80781A78` | 23 | 2 |
| `chr::ControlMoveCursor` | `chr::ControlBase` | `0x80781B90` | 22 | 2 |
| `chr::ControlMultiAttack` | `chr::ControlBase`, `chr::ControlAttackBase` | `0x8077D200` | 23 | 3 |
| `chr::ControlOneMotion` | `chr::ControlBase` | `0x80781CE0` | 22 | 2 |
| `chr::ControlOneMotionTarget` | `chr::ControlBase` | `0x80781C68` | 22 | 2 |
| `chr::ControlOneMotionTransition` | `chr::ControlBase` | `0x80781BF0` | 22 | 2 |
| `chr::ControlOttotto` | `chr::ControlBase` | `0x80781DE0` | 22 | 2 |
| `chr::ControlOverdrive` | `chr::ControlBase` | `0x80780ED0` | 22 | 2 |
| `chr::ControlPushedAvoid` | `chr::ControlBase` | `0x80781D68` | 22 | 4 |
| `chr::ControlPushedDown` | `chr::ControlBase` | `0x80781E58` | 22 | 2 |
| `chr::ControlQuarkSeirenAttack` | `chr::ControlBase`, `chr::ControlChargeAttackBase` | `0x8077F380` | 22 | 4 |
| `chr::ControlReflectLaser` | `chr::ControlBase` | `0x8077DE00` | 22 | 2 |
| `chr::ControlRelease` | `chr::ControlBase` | `0x8077F198` | 22 | 2 |
| `chr::ControlRevival` | `chr::ControlBase` | `0x8077FE20` | 22 | 5 |
| `chr::ControlRevivalSkelton` | `chr::ControlBase` | `0x8077FDA8` | 22 | 4 |
| `chr::ControlRide` | `chr::ControlBase` | `0x80781F68` | 22 | 3 |
| `chr::ControlRideBeast` | `chr::ControlBase` | `0x80781EF0` | 22 | 4 |
| `chr::ControlShoot` | `chr::ControlBase` | `0x80782000` | 22 | 2 |
| `chr::ControlSideWallAttack` | `chr::ControlBase`, `chr::ControlAttackBase` | `0x8077D180` | 23 | 2 |
| `chr::ControlSitting` | `chr::ControlBase` | `0x80782258` | 22 | 2 |
| `chr::ControlSlip` | `chr::ControlBase` | `0x80782078` | 22 | 2 |
| `chr::ControlStagger` | `chr::ControlBase` | `0x807820F0` | 22 | 2 |
| `chr::ControlStepOn` | `chr::ControlBase`, `chr::ControlInteractionBase` | `0x807810C0` | 22 | 1 |
| `chr::ControlStickByQuark` | `chr::ControlBase` | `0x8077ED00` | 22 | 4 |
| `chr::ControlStickSword` | `chr::ControlBase` | `0x8077E940` | 22 | 2 |
| `chr::ControlStickSwordSp` | `chr::ControlBase` | `0x8077E8C8` | 22 | 2 |
| `chr::ControlStickZanGurgCliff` | `chr::ControlBase` | `0x8077DE78` | 22 | 4 |
| `chr::ControlStickZanGurgStand` | `chr::ControlBase` | `0x8077DEF0` | 22 | 4 |
| `chr::ControlStool` | `chr::ControlBase` | `0x80780498` | 22 | 2 |
| `chr::ControlSwapAttack` | `chr::ControlBase`, `chr::ControlAttackBase` | `0x8077D000` | 23 | 3 |
| `chr::ControlSwapPos` | `chr::ControlBase` | `0x80780420` | 22 | 2 |
| `chr::ControlSwatTurn` | `chr::ControlBase` | `0x80780588` | 22 | 3 |
| `chr::ControlThreadCatch` | `chr::ControlBase` | `0x8077E328` | 22 | 4 |
| `chr::ControlThreadDead` | `chr::ControlBase` | `0x8077E2B0` | 22 | 3 |
| `chr::ControlThreadRelease` | `chr::ControlBase` | `0x8077E238` | 22 | 2 |
| `chr::ControlThroughRope` | `chr::ControlBase`, `chr::ControlInteractionBase` | `0x80781040` | 22 | 1 |
| `chr::ControlThrown` | `chr::ControlBase` | `0x8077E5F8` | 22 | 3 |
| `chr::ControlThrownAvoid` | `chr::ControlBase` | `0x8077E508` | 22 | 4 |
| `chr::ControlThrownAway` | `chr::ControlBase` | `0x8077E580` | 22 | 3 |
| `chr::ControlThrownBreakSkelton` | `chr::ControlBase` | `0x8077E3A0` | 22 | 3 |
| `chr::ControlThrownSimple` | `chr::ControlBase` | `0x8077E148` | 22 | 3 |
| `chr::ControlThrownSkelton` | `chr::ControlBase` | `0x8077E418` | 22 | 3 |
| `chr::ControlThrust` | `chr::ControlBase` | `0x8077EC88` | 22 | 2 |
| `chr::ControlThrustForCocoon` | `chr::ControlBase` | `0x8077EA30` | 22 | 2 |
| `chr::ControlThrustForCrab` | `chr::ControlBase` | `0x8077EC10` | 22 | 2 |
| `chr::ControlThrustForGhoul` | `chr::ControlBase` | `0x8077EAA8` | 22 | 6 |
| `chr::ControlThrustForKraken` | `chr::ControlBase` | `0x8077EB98` | 22 | 3 |
| `chr::ControlThrustForQuarkStand` | `chr::ControlBase` | `0x8077EB20` | 22 | 4 |
| `chr::ControlTurnQuadruped` | `chr::ControlBase` | `0x80782168` | 22 | 2 |
| `chr::ControlUseSkill` | `chr::ControlBase` | `0x807821E0` | 22 | 2 |
| `chr::ControlWallDown` | `chr::ControlBase` | `0x80782388` | 22 | 3 |
| `chr::ControlWallMountle` | `chr::ControlBase` | `0x80781988` | 22 | 3 |
| `chr::ControlWallSlash` | `chr::ControlBase` | `0x80782310` | 22 | 8 |
| `chr::ControlWallUp` | `chr::ControlBase`, `chr::ControlMoveCursor` | `0x80782400` | 22 | 5 |
| `chr::ControlWarp` | `chr::ControlBase` | `0x80780330` | 22 | 3 |
| `chr::ControlZanTackle` | `chr::ControlBase` | `0x8077F308` | 22 | 3 |

## db

| Clase | Bases | vtable | Métodos | Propias |
|---|---|---|---|---|
| `db::GrowthParse` | — | `0x80783078` | 2 | 1 |
| `db::GrowthParseGlobal` | `atn::ParseTree` | `0x80783090` | 2 | 2 |
| `db::MotionCommandCommon` | `atn::TaskBase`, `atn::Task` | `0x807830C0` | 5 | 2 |

## gmk

| Clase | Bases | vtable | Métodos | Propias |
|---|---|---|---|---|
| `gmk::ArenaFloor` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078DEF0` | 27 | 9 |
| `gmk::AttractHatePoint` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078E210` | 27 | 11 |
| `gmk::Augur` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078DFB0` | 27 | 6 |
| `gmk::AutomaticDoor` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078CC30` | 27 | 13 |
| `gmk::Banana` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078DA78` | 27 | 11 |
| `gmk::Bat` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078D1F8` | 27 | 6 |
| `gmk::Brazier` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078D328` | 27 | 11 |
| `gmk::BulletPoint` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078DD20` | 27 | 7 |
| `gmk::Cannon` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078D3C0` | 27 | 11 |
| `gmk::Cannonball` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078CE90` | 27 | 14 |
| `gmk::Chain` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078F0C0` | 27 | 6 |
| `gmk::CharaLight` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078E2A8` | 27 | 11 |
| `gmk::CharaObj` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078E9A8` | 27 | 11 |
| `gmk::CharaPathMove` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078F178` | 27 | 11 |
| `gmk::ChickenManager` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078D500` | 27 | 5 |
| `gmk::Coffin` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078D818` | 27 | 11 |
| `gmk::Courtyard` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078DDB8` | 27 | 10 |
| `gmk::DoorSeal` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078E340` | 27 | 10 |
| `gmk::DropItem` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078D458` | 27 | 10 |
| `gmk::FallingTower` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078D6E8` | 27 | 10 |
| `gmk::Fireworks` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078DE58` | 27 | 8 |
| `gmk::FlamingArrow` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078DB10` | 27 | 9 |
| `gmk::FloatingBox` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078E808` | 27 | 12 |
| `gmk::FlyObjManager` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078E508` | 27 | 7 |
| `gmk::FragileObject` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078CB98` | 27 | 18 |
| `gmk::GmkChara` | `atn::TaskBase`, `atn::Task`, `CharaTask` | `0x8078E8A0` | 55 | 4 |
| `gmk::HealPoint` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078D650` | 27 | 10 |
| `gmk::InteractionSpot` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078E5A8` | 27 | 7 |
| `gmk::Item` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078CF28` | 27 | 9 |
| `gmk::ItemSlotAnimeBuffer` | `atn::TaskBase`, `atn::Task`, `RecycleBuffer<gmk::ItemSlotAnime>` | `0x8078F388` | 5 | 2 |
| `gmk::LandMine` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078E0E0` | 27 | 12 |
| `gmk::Leaf` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078D130` | 27 | 7 |
| `gmk::Lemon` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078D9E0` | 27 | 10 |
| `gmk::LemonBucket` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078EC08` | 27 | 15 |
| `gmk::LottaSummon` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078F008` | 27 | 12 |
| `gmk::MapLift` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078E640` | 27 | 10 |
| `gmk::MeetingSpot` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078CFE8` | 27 | 7 |
| `gmk::MotionObject` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078D8B0` | 27 | 18 |
| `gmk::Ocean` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078DBA8` | 27 | 11 |
| `gmk::OnOff` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078CD60` | 27 | 12 |
| `gmk::OpenBoard` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078E770` | 27 | 12 |
| `gmk::PassBoard` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078EA40` | 27 | 12 |
| `gmk::PigeonManager` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078D290` | 27 | 5 |
| `gmk::PlayEffect` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078CCC8` | 27 | 9 |
| `gmk::RainDrops` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078E6D8` | 27 | 8 |
| `gmk::RandomItem` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078EE40` | 27 | 9 |
| `gmk::RandomMerchant` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078EDA8` | 27 | 14 |
| `gmk::ReinforcePoint` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078CDF8` | 27 | 7 |
| `gmk::RoomLight` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078E048` | 27 | 9 |
| `gmk::RotaryRoom` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078E3D8` | 27 | 10 |
| `gmk::SavePoint` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078EED8` | 27 | 14 |
| `gmk::ShakeCamera` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078E178` | 27 | 8 |
| `gmk::SkeltonsMirror` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078D948` | 27 | 11 |
| `gmk::SparkCube` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078E470` | 27 | 17 |
| `gmk::SwingObject` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078D098` | 27 | 9 |
| `gmk::TodomeCountUp` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078EB70` | 27 | 10 |
| `gmk::Trap` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078D780` | 27 | 9 |
| `gmk::TreasureBox` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078D598` | 27 | 12 |
| `gmk::WaitObject` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078DC88` | 27 | 19 |
| `gmk::WaterLevel` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078EAD8` | 27 | 12 |
| `gmk::WindyItem` | `atn::TaskBase`, `atn::Task`, `GimmickCommon` | `0x8078EF70` | 27 | 9 |

## nw4r

| Clase | Bases | vtable | Métodos | Propias |
|---|---|---|---|---|
| `nw4r::snd::DvdSoundArchive` | `nw4r::snd::SoundArchive` | `0x80796BF0` | 6 | 6 |
| `nw4r::snd::DvdSoundArchive::DvdFileStream` | `nw4r::ut::IOStream`, `nw4r::ut::FileStream`, `nw4r::ut::DvdFileStream`, `nw4r::ut::DvdLockedFileStream` | `0x80796B60` | 23 | 23 |
| `nw4r::snd::Sound3DEngine` | `nw4r::snd::detail::Sound3DEngineInterface` | — | — | — |
| `nw4r::snd::SoundArchive` | — | `0x80796D18` | 6 | 1 |
| `nw4r::snd::SoundArchivePlayer` | `nw4r::snd::SoundStartable`, `nw4r::snd::detail::DisposeCallback` | `0x80796D38` | 3 | 3 |
| `nw4r::snd::SoundArchivePlayer::SeqNoteOnCallback` | `nw4r::snd::detail::NoteOnCallback` | `0x80796DB0` | 2 | 2 |
| `nw4r::snd::SoundArchivePlayer::WsdCallback` | `nw4r::snd::detail::WsdPlayer::WsdCallback` | `0x80796D88` | 2 | 2 |
| `nw4r::snd::SoundStartable` | — | — | — | — |
| `nw4r::snd::detail::DisposeCallback` | — | — | — | — |
| `nw4r::snd::detail::NoteOnCallback` | — | — | — | — |
| `nw4r::snd::detail::Sound3DEngineInterface` | — | — | — | — |
| `nw4r::snd::detail::WsdPlayer::WsdCallback` | — | — | — | — |
| `nw4r::ut::DvdFileStream` | `nw4r::ut::IOStream`, `nw4r::ut::FileStream` | — | — | — |
| `nw4r::ut::DvdLockedFileStream` | `nw4r::ut::IOStream`, `nw4r::ut::FileStream`, `nw4r::ut::DvdFileStream` | — | — | — |
| `nw4r::ut::FileStream` | `nw4r::ut::IOStream` | — | — | — |
| `nw4r::ut::IOStream` | — | — | — | — |

## std

| Clase | Bases | vtable | Métodos | Propias |
|---|---|---|---|---|
| `std::bad_alloc` | `std::exception` | `0x80793BC8` | 2 | 2 |
| `std::bad_cast` | `std::exception` | `0x8078B198` | 2 | 2 |
| `std::bad_exception` | `std::exception` | `0x807BBFE0` | 2 | 2 |
| `std::basic_ios<char, std::char_traits<char>>` | `std::ios_base` | `0x807799E0` | 1 | 1 |
| `std::basic_ios<wchar_t, std::char_traits<wchar_t>>` | `std::ios_base` | `0x807BBDC4` | 1 | 1 |
| `std::basic_iostream<char, std::char_traits<char>>` | `std::basic_ostream<char, std::char_traits<char>>`, `std::ios_base`, `std::basic_ios<char, std::char_traits<char>>`, `std::basic_istream<char, std::char_traits<char>>` | `0x8078B2BC` | 1 | 1 |
| `std::basic_istream<char, std::char_traits<char>>` | `std::ios_base`, `std::basic_ios<char, std::char_traits<char>>` | `0x8078B2E0` | 1 | 1 |
| `std::basic_istream<wchar_t, std::char_traits<wchar_t>>` | `std::ios_base`, `std::basic_ios<wchar_t, std::char_traits<wchar_t>>` | `0x807BBD98` | 1 | 1 |
| `std::basic_ostream<char, std::char_traits<char>>` | `std::ios_base`, `std::basic_ios<char, std::char_traits<char>>` | `0x80779AA0` | 1 | 1 |
| `std::basic_ostream<wchar_t, std::char_traits<wchar_t>>` | `std::ios_base`, `std::basic_ios<wchar_t, std::char_traits<wchar_t>>` | `0x807BBD68` | 1 | 1 |
| `std::basic_streambuf<char, std::char_traits<char>>` | — | `0x80779A58` | 14 | 13 |
| `std::basic_streambuf<wchar_t, std::char_traits<wchar_t>>` | — | `0x807BBC2C` | 13 | 13 |
| `std::basic_stringbuf<char, std::char_traits<char>, std::allocator<char>>` | `std::basic_streambuf<char, std::char_traits<char>>` | `0x80779A08` | 13 | 6 |
| `std::basic_stringstream<char, std::char_traits<char>, std::allocator<char>>` | `std::basic_ostream<char, std::char_traits<char>>`, `std::ios_base`, `std::basic_ios<char, std::char_traits<char>>`, `std::basic_istream<char, std::char_traits<char>>`, `std::basic_iostream<char, std::char_traits<char>>` | `0x8078B080` | 1 | 1 |
| `std::codecvt<char, char, int>` | `std::codecvt_base`, `std::locale::facet` | `0x807BBE40` | 8 | 8 |
| `std::codecvt<wchar_t, char, int>` | `std::codecvt_base`, `std::locale::facet` | `0x807BBE00` | 8 | 8 |
| `std::codecvt_base` | — | — | — | — |
| `std::ctype<char>` | `std::ctype_base`, `std::locale::facet` | `0x807BBECC` | 9 | 9 |
| `std::ctype<wchar_t>` | `std::ctype_base`, `std::locale::facet` | `0x807BBE7C` | 13 | 13 |
| `std::ctype_base` | — | — | — | — |
| `std::exception` | — | — | — | — |
| `std::ios_base` | — | `0x807BBB24` | 1 | 1 |
| `std::ios_base::failure` | `std::exception` | `0x807BBB08` | 2 | 2 |
| `std::length_error` | `std::exception`, `std::logic_error` | `0x807BBF10` | 2 | 1 |
| `std::locale::facet` | — | — | — | — |
| `std::logic_error` | `std::exception` | `0x807BBF38` | 2 | 2 |
| `std::num_put<char, std::ostreambuf_iterator<char, std::char_traits<char>>>` | `std::locale::facet` | `0x80783C20` | 9 | 9 |
| `std::numpunct<char>` | `std::locale::facet` | `0x80783C60` | 6 | 6 |
| `std::runtime_error` | `std::exception` | `0x80775B60` | 2 | 2 |
| `std::tr1::detail::shared_ptr_deleter<std::__locale_imp, std::detail::default_delete<std::__locale_imp>>` | `std::tr1::detail::shared_ptr_deleter_common` | `0x807BBDE0` | 3 | 3 |
| `std::tr1::detail::shared_ptr_deleter_common` | — | — | — | — |

