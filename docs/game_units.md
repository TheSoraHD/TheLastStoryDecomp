# Unidades del juego (inferidas)

Unidades de traducción del código del juego deducidas automáticamente. Todas están en `NonMatching` (ensamblador) hasta que se descompilen.

## Método

1. **Clases por fichero:** dentro de un fichero, CodeWarrior emite las vtables en orden inverso al código; entre ficheros siguen el orden de enlace. Recorriendo las vtables en el orden de `.data`, un salto hacia delante en el código de la clase indica un fichero nuevo.
2. **Rango de código:** métodos propios de las clases del grupo (según la vtable), funciones que usan sus vtables/RTTI (constructores, destructores) y bloques atados por constantes de `.sdata2`/`.rodata` compartidas.
3. **Descartes:** si dentro del rango aparece la misma constante en dos direcciones distintas (CodeWarrior guarda una copia por fichero), el rango cruza un límite y la unidad se descarta.
4. **Datos:** bloque de vtables y RTTI en `.data`, nombres de clase en `.rodata`, constantes de `.sdata2` usadas solo por la unidad.
5. **Comprobaciones:** ningún solape entre unidades; el orden de `.data`, `.rodata` y `.sdata2` sigue al de `.text` en todas; el `.dol` reconstruido es idéntico.

**Limitaciones:** las unidades son conservadoras. Pueden faltarles funciones auxiliares o datos (cadenas, tablas, variables) en los bordes, que quedan en las unidades automáticas vecinas. Los nombres de fichero son inferidos a partir de la clase principal; la unidad piloto `atn/atn_colli.cpp` se delimitó a mano.

## Lista (346 unidades)

| Unidad | .text | Clases |
|---|---|---|
| `Event/Puppeteer_CharaTask.cpp` | `0x8000CBD0–0x80013D60` | `Event::Puppeteer<CharaTask>`, `Event::Puppeteer<atn::Model>` |
| `game/AiTarget.cpp` | `0x800144FC–0x800185B4` | `AiTarget`, `AIManager` |
| `std/runtime_error.cpp` | `0x8001A268–0x8001A34C` | `std::runtime_error`, `boost::bad_function_call` |
| `AI/AI_archer_attack01.cpp` | `0x80025DE0–0x80035EB4` | `AI::Script::AI_archer_attack01`, `AI::Script::AI_archer_guard01`, `AI::Script::AI_arganan01`, `AI::Script::AI_buffer_attackstable01`, `AI::Script::AI_buffer_guard01`, `AI::Script::AI_buffer_guard02`, `AI::Script::AI_buffer_pointguard01`, `AI::Script::AI_em_archer_attack02`, `AI::Script::AI_em_bridge_archer`, `AI::Script::AI_em_monster_attack01`, `AI::Script::AI_em_scout`, `AI::Script::AI_em_sword_attack01`, `AI::Script::AI_em_sword_attack02`, `AI::Script::AI_em_sword_pointguard01`, `AI::Script::AI_em_type003`, `AI::Script::AI_em_type010`, `AI::Script::AI_em_type102`, `AI::Script::AI_em_type103`, `AI::Script::AI_em_type104`, `AI::Script::AI_em_type105`, `AI::Script::AI_em_type106`, `AI::Script::AI_fr_archer_pick01`, `AI::Script::AI_fr_buffer_pick01`, `AI::Script::AI_fr_follow_player`, `AI::Script::AI_fr_magic_pick01`, `AI::Script::AI_fr_magic_pick02`, `AI::Script::AI_fr_runaway`, `AI::Script::AI_fr_sword_attack01`, `AI::Script::AI_fr_sword_pick01`, `AI::Script::AI_fr_sword_pointguard`, `AI::Script::AI_fr_sword_pointguard01`, `AI::Script::AI_fr_type005`, `AI::Script::AI_fr_warppointescape`, `AI::Script::AI_jackal01`, `AI::Script::AI_magic_attack01`, `AI::Script::AI_magic_attack01_2`, `AI::Script::AI_magic_attack02`, `AI::Script::AI_magic_attackstable01`, `AI::Script::AI_magic_cure01`, `AI::Script::AI_magic_guard01`, `AI::Script::AI_magic_guard01_2`, `AI::Script::AI_magic_guard02`, `AI::Script::AI_np_approach`, `AI::Script::AI_np_approach_plus`, `AI::Script::AI_np_follow_player`, `AI::Script::AI_np_tagger_random`, `AI::Script::AI_np_type001`, `AI::Script::AI_np_type002`, `AI::Script::AI_np_type003`, `AI::Script::AI_np_type004`, `AI::Script::AI_np_type005`, `AI::Script::AI_np_wait_reaction`, `AI::Script::AI_np_wait_reaction_minus`, `AI::Script::AI_np_wait_reaction_minus2`, `AI::Script::AI_np_wait_reaction_plus`, `AI::Script::AI_quark01`, `AI::Script::AI_quark02`, `AI::Script::AI_silence_guard01`, `AI::Script::AI_sword_guard01`, `AI::Script::AI_sword_leaderguard01`, `AI::Script::AI_sword_leaderguard02`, `AI::Script::AI_sword_leaderguard03`, `AI::Script::AI_sword_pointguard01`, `AI::Script::AI_return_guard01` |
| `game/AreaEffConfig.cpp` | `0x80041A80–0x80042E20` | `AreaEffConfig`, `atn::CsvBase<char, 44, 13, 34>` |
| `game/ArenaTask.cpp` | `0x8004318C–0x80043D58` | `ArenaTask` |
| `game/Arms.cpp` | `0x80043EAC–0x800457A4` | `Arms` |
| `atn/AABBTree.cpp` | `0x800458D4–0x80046214` | `atn::AABBTree` |
| `atn/AudioDevice.cpp` | `0x800463FC–0x80046EE0` | `atn::AudioDevice` |
| `atn/Bgm.cpp` | `0x8004AD9C–0x8004AE84` | `atn::Bgm` |
| `atn/SinusCurve.cpp` | `0x8005C248–0x8005DD60` | `atn::SinusCurve`, `atn::CosinusCurve`, `atn::ConstantCurve`, `atn::Polynomial1Curve`, `atn::Polynomial2Curve` |
| `atn/DrawHelper.cpp` | `0x8005DD60–0x8005DED8` | `atn::DrawHelper` |
| `atn/ComDrawTexture.cpp` | `0x8005E000–0x80068BF0` | `atn::DrawHelper::ComDrawTexture`, `atn::DrawHelper::ComDrawTextureAdd` |
| `atn/FadeCommon.cpp` | `0x8006A09C–0x8006A950` | `atn::FadeCommon`, `atn::FadeColor`, `atn::FadeCapture` |
| `atn/FileList.cpp` | `0x8006BBCC–0x8006BD78` | `atn::FileList` |
| `atn/LensFlare.cpp` | `0x80077160–0x80079044` | `atn::LensFlare` |
| `atn/SmallAllocator_8.cpp` | `0x80082AC8–0x80084E50` | `atn::SmallAllocator<8>`, `atn::SmallAllocator<6>` |
| `atn/DLAllocator.cpp` | `0x80084E50–0x80084FD8` | `atn::DLAllocator` |
| `atn/MindowsCommon.cpp` | `0x80084FD8–0x800899DC` | `atn::MindowsCommon`, `atn::MindowsInt`, `atn::MindowsUInt`, `atn::MindowsFloat`, `atn::MindowsBool`, `atn::MindowsFlag` |
| `atn/ModelInstance.cpp` | `0x8009E5A0–0x8009F70C` | `atn::ModelInstance` |
| `atn/NandManager.cpp` | `0x800A2C48–0x800A5508` | `atn::NandManager` |
| `atn/ParseTree.cpp` | `0x800A5D14–0x800A7314` | `atn::ParseTree` |
| `atn/PBoneBasic.cpp` | `0x800A787C–0x800A8FF0` | `atn::PBoneBasic`, `atn::PBoneOutForce` |
| `atn/LinearAllocator.cpp` | `0x800A8FF0–0x800A981C` | `atn::LinearAllocator` |
| `atn/Rain.cpp` | `0x800B1B28–0x800B2E2C` | `atn::Rain` |
| `atn/Layer2DBlur.cpp` | `0x800B2E2C–0x800B326C` | `atn::Layer2DBlur` |
| `atn/Layer3D.cpp` | `0x800B326C–0x800B38B8` | `atn::Layer3D` |
| `atn/LayerOcclusionCPU.cpp` | `0x800B3A94–0x800B543C` | `atn::LayerOcclusionCPU` |
| `atn/LayerPost3D.cpp` | `0x800B66A4–0x800B675C` | `atn::LayerPost3D` |
| `atn/LayerPostFx.cpp` | `0x800B675C–0x800B68BC` | `atn::LayerPostFx` |
| `atn/LayerReflect.cpp` | `0x800B68BC–0x800B6BEC` | `atn::LayerReflect` |
| `atn/LayerShadow.cpp` | `0x800B6BEC–0x800B95C4` | `atn::LayerShadow` |
| `atn/SequenceControl.cpp` | `0x800C3610–0x800C37FC` | `atn::SequenceControl` |
| `atn/SoftKeyboardBase_wchar_t_0.cpp` | `0x800C63D8–0x800C6E60` | `atn::SoftKeyboardBase<wchar_t, 0>` |
| `atn/Sound3dListener.cpp` | `0x800C7DC4–0x800C8278` | `atn::Sound3dListener`, `atn::Sound3dActor`, `atn::SoundCore` |
| `atn/Sound3dEngine.cpp` | `0x800CAD9C–0x800CB360` | `atn::Sound3dEngine` |
| `atn/TextureCacheManager8x32.cpp` | `0x800D69A8–0x800D6BE0` | `atn::TextureCacheManager8x32` |
| `atn/TextureComposite.cpp` | `0x800D71A4–0x800D810C` | `atn::TextureComposite` |
| `atn/CompositeIntensityTintMulti.cpp` | `0x800D8894–0x800D9410` | `atn::CompositeIntensityTintMulti` |
| `atn/TextAnimation.cpp` | `0x800D9410–0x800DBF68` | `atn::TextAnimation` |
| `atn/VideoTHP.cpp` | `0x800DCA70–0x800DD1EC` | `atn::VideoTHP` |
| `std/char_traits_char.cpp` | `0x800E0AC0–0x800E0B18` | `std::basic_ios<char, std::char_traits<char>>` |
| `std/allocator_char.cpp` | `0x800E0B90–0x800E24CC` | `std::basic_stringbuf<char, std::char_traits<char>, std::allocator<char>>` |
| `game/BattleCharaTask.cpp` | `0x800E2D4C–0x800EF3EC` | `BattleCharaTask` |
| `game/BattleResult.cpp` | `0x800EFF68–0x800F1378` | `BattleResult` |
| `game/BattleTask.cpp` | `0x800F1430–0x800F7250` | `BattleTask` |
| `Event/BehaviorFollow.cpp` | `0x8010D98C–0x8010E3F0` | `Event::BehaviorFollow` |
| `game/Bullet.cpp` | `0x8010EB58–0x80112220` | `Bullet`, `BootTask` |
| `Event/Path.cpp` | `0x80112254–0x80116EB8` | `Event::Path` |
| `game/CampSaveLoad.cpp` | `0x8011770C–0x8011BD14` | `MenuSaveTask`, `MenuLoadTask`, `CampSaveLoad`, `atn::CsvBase<wchar_t, 44, 13, 34>`, `atn::CsvU` (antes dos unidades: `MenuSaveTask` y `CsvBase_wchar_t_44_13_34`) |
| `game/CharaManager_PlayerTask.cpp` | `0x8011F6FC–0x8011FF78` | `CharaManager<PlayerTask>`, `CharaManager<EnemyTask>`, `CharaManager<NpcTask>` |
| `game/CharaTask.cpp` | `0x801354B4–0x8017CB9C` | `CharaTask` (un solo fichero: un pool de `.sdata2`, 436 funciones) |
| `game/ChaserGuard.cpp` | `0x8017CB9C–0x8018059C` | `ChaserGuard` |
| `game/ChaserNpc.cpp` | `0x8018059C–0x801823A8` | `ChaserNpc` |
| `game/ChaseManager.cpp` | `0x801823A8–0x80183588` | `ChaseManager` |
| `game/ColliAttrManager.cpp` | `0x801839FC–0x80184A64` | `ColliAttrManager` |
| `chr/ControlBreakOpen.cpp` | `0x80187D04–0x801880D0` | `chr::ControlBreakOpen` |
| `chr/ControlAppear.cpp` | `0x801880D0–0x80189634` | `chr::ControlAppear`, `chr::ControlAppearEnd`, `chr::ControlAppearSkelton`, `chr::ControlAppearCoffin` |
| `chr/ControlAttackBase.cpp` | `0x80189634–0x8018EEC4` | `chr::ControlAttackBase`, `chr::ControlAttack`, `chr::ControlAttackNext`, `chr::ControlAttackSetup`, `chr::ControlDashAttack`, `chr::ControlJumpAttack`, `chr::ControlJumpAttack2`, `chr::ControlMultiAttack`, `chr::ControlSideWallAttack`, `chr::ControlGuardCounter`, `chr::ControlHitBackBullet`, `chr::ControlSwapAttack`, `chr::ControlAttackLastOne` |
| `chr/ControlStickByQuark.cpp` | `0x801963F4–0x801A0BA8` | `chr::ControlStickByQuark`, `chr::ControlThrust`, `chr::ControlThrustForCrab`, `chr::ControlThrustForKraken`, `chr::ControlThrustForQuarkStand`, `chr::ControlThrustForGhoul`, `chr::ControlThrustForCocoon`, `chr::ControlAbsorbed`, `chr::ControlMatHoldPre`, `chr::ControlMatHold`, `chr::ControlMatHoldRelease`, `chr::ControlMatHoldDamage`, `chr::ControlThrown`, `chr::ControlThrownAway`, `chr::ControlThrownAvoid`, `chr::ControlCaughtSkelton`, `chr::ControlThrownSkelton`, `chr::ControlThrownBreakSkelton`, `chr::ControlThreadCatch`, `chr::ControlThreadDead`, `chr::ControlThreadRelease`, `chr::ControlBoundShade`, `chr::ControlAbsorbedHP`, `chr::ControlAbsorbedHPReleased`, `chr::ControlCliffhangZanGurg`, `chr::ControlEndCliffhangZanGurg`, `chr::ControlStickZanGurgStand`, `chr::ControlStickZanGurgCliff`, `chr::ControlReflectLaser` |
| `chr/ControlCannonLift.cpp` | `0x801A0BA8–0x801A226C` | `chr::ControlCannonLift`, `chr::ControlCannonThrow`, `chr::ControlCannonTarget` |
| `chr/ControlCaught.cpp` | `0x801A2290–0x801A2CEC` | `chr::ControlCaught`, `chr::ControlRelease` |
| `chr/ControlCharge.cpp` | `0x801A2CEC–0x801A77BC` | `chr::ControlCharge`, `chr::ControlChargeAttackBase`, `chr::ControlChargeAttack`, `chr::ControlHighJumpAttack`, `chr::ControlQuarkSeirenAttack`, `chr::ControlZanTackle` |
| `chr/ControlChaseCaught.cpp` | `0x801A77BC–0x801AAAE4` | `chr::ControlChaseCaught`, `chr::ControlChaseHeld`, `chr::ControlChaseRelease`, `chr::ControlChaseCatch`, `chr::ControlChaseHold`, `chr::ControlChaseReleased` |
| `chr/ControlCliffhang.cpp` | `0x801AAB64–0x801AC434` | `chr::ControlCliffhang` |
| `chr/ControlDamage.cpp` | `0x801AC434–0x801B0710` | `chr::ControlDamage`, `chr::ControlGuard`, `chr::ControlAvoid`, `chr::ControlKnockdown`, `chr::ControlGetUp`, `chr::ControlRevival`, `chr::ControlRevivalSkelton`, `chr::ControlFallDead`, `chr::ControlGuarded`, `chr::ControlDamageLastOne` |
| `chr/ControlDetect.cpp` | `0x801B0710–0x801B0D88` | `chr::ControlDetect`, `chr::ControlDetectHateAtt` |
| `chr/ControlDive.cpp` | `0x801B0D88–0x801B2888` | `chr::ControlDive`, `chr::ControlSwatTurn`, `chr::ControlCoverSlip`, `chr::ControlStool`, `chr::ControlSwapPos`, `chr::ControlBackStep`, `chr::ControlWarp` |
| `chr/ControlDraggingStart.cpp` | `0x801B2888–0x801B2EDC` | `chr::ControlDraggingStart`, `chr::ControlDraggedStart`, `chr::ControlDragged` |
| `chr/ControlDropDown.cpp` | `0x801B2EDC–0x801B409C` | `chr::ControlDropDown`, `chr::ControlDropDownQuadruped` |
| `chr/ControlGmkCoffinOpening.cpp` | `0x801B409C–0x801B529C` | `chr::ControlGmkCoffinOpening`, `chr::ControlGmkCoffinOpen`, `chr::ControlGmkCoffinGetUp`, `chr::ControlGmkTrapBat`, `chr::ControlGmkBreakArmor` |
| `chr/ControlJumpAt.cpp` | `0x801B529C–0x801B73F0` | `chr::ControlJumpAt`, `chr::ControlGrapple`, `chr::ControlGrappled`, `chr::ControlGrappleThrown`, `chr::ControlGrappledThrow` |
| `chr/ControlAttractHateCharge.cpp` | `0x801B73F0–0x801B7CD0` | `chr::ControlAttractHateCharge`, `chr::ControlOverdrive` |
| `chr/ControlInteractionBase.cpp` | `0x801B7CD0–0x801B9A14` | `chr::ControlInteractionBase`, `chr::ControlDuckUnderA`, `chr::ControlDuckUnderB`, `chr::ControlDuckJoist`, `chr::ControlConstrict`, `chr::ControlBumpHead`, `chr::ControlStepOn`, `chr::ControlThroughRope` |
| `chr/ControlLadder.cpp` | `0x801B9A54–0x801BC288` | `chr::ControlLadder`, `chr::ControlAlleyWay`, `chr::ControlClimbDeck`, `chr::ControlCrawling` |
| `chr/ControlLookAround.cpp` | `0x801BC288–0x801BC5B8` | `chr::ControlLookAround`, `chr::ControlLookAroundEgg` |
| `chr/ControlChant.cpp` | `0x801BC5B8–0x801C041C` | `chr::ControlChant`, `chr::ControlMagic`, `chr::ControlMagicEnd` |
| `chr/ControlOneMotion.cpp` | `0x801C3DCC–0x801C4738` | `chr::ControlOneMotion`, `chr::ControlOneMotionTarget`, `chr::ControlOneMotionTransition` |
| `chr/ControlPushedDown.cpp` | `0x801C48B4–0x801C6F90` | `chr::ControlPushedDown`, `chr::ControlOttotto`, `chr::ControlPushedAvoid` |
| `chr/ControlRide.cpp` | `0x801C6F90–0x801C7918` | `chr::ControlRide`, `chr::ControlRideBeast` |
| `chr/ControlShoot.cpp` | `0x801C7918–0x801C82F8` | `chr::ControlShoot` |
| `chr/ControlStagger.cpp` | `0x801C82F8–0x801C8F10` | `chr::ControlStagger`, `chr::ControlSlip` |
| `chr/ControlTurnQuadruped.cpp` | `0x801C8F10–0x801C93C4` | `chr::ControlTurnQuadruped` |
| `chr/ControlUseSkill.cpp` | `0x801C93C4–0x801C9B24` | `chr::ControlUseSkill` |
| `chr/ControlSitting.cpp` | `0x801C9B24–0x801C9F6C` | `chr::ControlSitting` |
| `chr/ControlWallUp.cpp` | `0x801C9F6C–0x801CC0AC` | `chr::ControlWallUp`, `chr::ControlWallDown`, `chr::ControlWallSlash` |
| `game/CopyRobotTask.cpp` | `0x801CC0AC–0x801CCE50` | `CopyRobotTask` |
| `game/CosEditBase.cpp` | `0x801CCE50–0x801CFC58` | `CosEditBase` |
| `game/CosEditProtector.cpp` | `0x801CFDCC–0x801D5420` | `CosEditProtector` |
| `game/CostumeEditChara.cpp` | `0x801E7FF8–0x801E9218` | `CostumeEditChara` |
| `game/CrossbowMenuTask.cpp` | `0x801E9398–0x801EA4B0` | `CrossbowMenuTask` |
| `game/CrowdManager.cpp` | `0x801EA4B0–0x801EB7C4` | `CrowdManager` |
| `game/CrowdSimulation.cpp` | `0x801EB93C–0x801EC158` | `CrowdSimulation` |
| `atn/D2A_Common.cpp` | `0x801EC158–0x801EEE54` | `atn::D2A_Common` |
| `atn/D2A_Data.cpp` | `0x801EEE54–0x801EFB4C` | `atn::D2A_Data` |
| `atn/D2A_Frame.cpp` | `0x801EFB4C–0x801F0364` | `atn::D2A_Frame` |
| `atn/D2A_Line.cpp` | `0x801F0AC0–0x801F0D80` | `atn::D2A_Line` |
| `atn/D2A_Message.cpp` | `0x801F2398–0x801F33A8` | `atn::D2A_Message` |
| `atn/D2A_Reference.cpp` | `0x801F3EF4–0x801F3FF8` | `atn::D2A_Reference` |
| `atn/D2A_Task.cpp` | `0x801F3FF8–0x801F4F84` | `atn::D2A_Task` |
| `atn/D2A_TaskInstanceBase_64_8.cpp` | `0x801F51C8–0x801F8E6C` | `atn::D2A_TaskInstanceBase<64, 8>`, `atn::D2A_TaskInstanceBase<32, 8>`, `atn::D2A_TaskInstanceBase<8, 2>` |
| `atn/D2A_Texture.cpp` | `0x801F94F4–0x801FDE94` | `atn::D2A_Texture` |
| `atn/D2A_Window.cpp` | `0x801FF1B8–0x801FF8EC` | `atn::D2A_Window` |
| `atn/D3Anime.cpp` | `0x80201E78–0x80202A6C` | `atn::D3Anime` |
| `atn/D3AnimeInstance.cpp` | `0x80202A6C–0x80203930` | `atn::D3AnimeInstance` |
| `Event/MotionData.cpp` | `0x80203930–0x80204958` | `Event::MotionData`, `Event::AnimationData`, `Event::ModelData`, `Event::EffectData`, `Event::CameraData`, `Event::PoseData`, `Event::MovieTask`, `Event::MovieData` |
| `atn/CsvBase_char_124_13_34.cpp` | `0x80207BEC–0x80207C00` | `atn::CsvBase<char, 124, 13, 34>` |
| `atn/ptr_string_base_wchar_t_0.cpp` | `0x8020C028–0x8020C4B0` | `atn::ptr_string_base<wchar_t, 0>` |
| `db/GrowthParseGlobal.cpp` | `0x8020F7D0–0x8020FDEC` | `db::GrowthParseGlobal`, `db::GrowthParse` |
| `db/MotionCommandCommon.cpp` | `0x80218720–0x802188B4` | `db::MotionCommandCommon` |
| `atn/ptr_string_base_char_0.cpp` | `0x8021F47C–0x80220108` | `atn::ptr_string_base<char, 0>` |
| `Event/DialogHead.cpp` | `0x8022526C–0x8022808C` | `Event::DialogHead` |
| `Event/Dialog.cpp` | `0x802280B4–0x80229DF0` | `Event::Dialog` |
| `atn/EffectManager.cpp` | `0x80239798–0x8023A8B4` | `atn::EffectManager` |
| `std/char_traits_char_8023C080.cpp` | `0x8023C080–0x802411B8` | `std::num_put<char, std::ostreambuf_iterator<char, std::char_traits<char>>>` |
| `game/Em001Necromancer.cpp` | `0x80244228–0x80247674` | `Em001Necromancer` |
| `game/Em002_01BlackSkelton.cpp` | `0x802476D0–0x80247FB0` | `Em002_01BlackSkelton` |
| `game/Em002_02SkeltonKing.cpp` | `0x80248E88–0x8024A004` | `Em002_02SkeltonKing` |
| `game/Em002Skelton.cpp` | `0x8024A004–0x8024A650` | `Em002Skelton` |
| `game/Em003LesserDaemon.cpp` | `0x8024A650–0x8024E7E4` | `Em003LesserDaemon` |
| `game/Em005DoppelGanger.cpp` | `0x80250A24–0x80253C9C` | `Em005DoppelGanger` |
| `game/Em006DarkMaster.cpp` | `0x8025457C–0x8025589C` | `Em006DarkMaster` |
| `game/Em101ElderZesya.cpp` | `0x802558B0–0x802598DC` | `Em101ElderZesya` |
| `game/Em101ElderZesyaSecond.cpp` | `0x8025A060–0x8025C998` | `Em101ElderZesyaSecond` |
| `game/Em101LittleZesya.cpp` | `0x8025C998–0x8025FE30` | `Em101LittleZesya` |
| `game/Em101LittleZesyaSecond.cpp` | `0x8025FE60–0x80262938` | `Em101LittleZesyaSecond` |
| `game/Em201ZanGurg.cpp` | `0x80262938–0x80263CF0` | `Em201ZanGurg` |
| `game/Em201ZanGurgFinal.cpp` | `0x80264DB4–0x80267F88` | `Em201ZanGurgFinal` |
| `game/Em202_01_BSpider.cpp` | `0x8026FC0C–0x80272660` | `Em202_01_BSpider` |
| `game/Em202ForestSpider.cpp` | `0x8027278C–0x80279860` | `Em202ForestSpider` |
| `game/Em202MothersSpider.cpp` | `0x80279860–0x8027C990` | `Em202MothersSpider` |
| `game/Em202Spider.cpp` | `0x8027C990–0x80281054` | `Em202Spider` |
| `game/Em202SpiderBase.cpp` | `0x80282AE8–0x80286CB0` | `Em202SpiderBase` |
| `game/Em203GreaterDaemon.cpp` | `0x80287E7C–0x802914F8` | `Em203GreaterDaemon` |
| `game/Em203GreaterDaemonRed.cpp` | `0x80293738–0x8029CB00` | `Em203GreaterDaemonRed` |
| `game/Em203GreaterDaemonRedMp.cpp` | `0x8029CB00–0x802A4BBC` | `Em203GreaterDaemonRedMp` |
| `game/Em204Mulue.cpp` | `0x802A4BBC–0x802AABD8` | `Em204Mulue` |
| `game/Em204BigMulue.cpp` | `0x802AC364–0x802B23E8` | `Em204BigMulue` |
| `game/Em204MulueMp.cpp` | `0x802B23E8–0x802B753C` | `Em204MulueMp` |
| `game/Em205Ogre.cpp` | `0x802B77AC–0x802BA92C` | `Em205Ogre` |
| `game/Em206CocoonMp.cpp` | `0x802BA92C–0x802C0C38` | `Em206CocoonMp` |
| `game/Em206EggMonster.cpp` | `0x802C0C38–0x802C7BC8` | `Em206EggMonster` |
| `game/Em208Queen.cpp` | `0x802C7BE4–0x802CCAFC` | `Em208Queen` |
| `game/Em209BigGolem.cpp` | `0x802CCB20–0x802CCFD8` | `Em209BigGolem` |
| `game/Em209SmallGolem.cpp` | `0x802CCFD8–0x802D0038` | `Em209SmallGolem` |
| `game/Em301LShade.cpp` | `0x802D0038–0x802D73E4` | `Em301LShade` |
| `game/Em302Kraken.cpp` | `0x802D7480–0x802DDCFC` | `Em302Kraken` |
| `game/Em302KrakenMp.cpp` | `0x802DDDB8–0x802E20FC` | `Em302KrakenMp` |
| `game/Em303KrakenFoot.cpp` | `0x802E211C–0x802E59E0` | `Em303KrakenFoot` |
| `game/Em303KrakenFootMp.cpp` | `0x802E59E0–0x802E8580` | `Em303KrakenFootMp` |
| `game/Em305Vampire.cpp` | `0x802E8580–0x802EC670` | `Em305Vampire` |
| `game/Em305VampireMp.cpp` | `0x802EC670–0x802EE604` | `Em305VampireMp` |
| `game/Em306GurgBeast.cpp` | `0x802EE604–0x802F1524` | `Em306GurgBeast` |
| `game/Em308InvertCrab.cpp` | `0x802F1524–0x802F6E64` | `Em308InvertCrab` |
| `game/Em308ParmCrab.cpp` | `0x802F6E64–0x802FC850` | `Em308ParmCrab` |
| `game/Em309Ghoul.cpp` | `0x802FC850–0x80305C74` | `Em309Ghoul` |
| `game/Em309GhoulSimple.cpp` | `0x80305C74–0x8030A144` | `Em309GhoulSimple` |
| `game/Em310Gargoyle.cpp` | `0x8030A144–0x8030DA1C` | `Em310Gargoyle` |
| `game/Em312DarkServant.cpp` | `0x8030DA20–0x80310314` | `Em312DarkServant` |
| `game/Em314Ivy.cpp` | `0x80310330–0x80313544` | `Em314Ivy` |
| `game/Em315Coffin.cpp` | `0x80313544–0x803161F8` | `Em315Coffin` |
| `game/Em316LaserSphere.cpp` | `0x803161F8–0x8031B09C` | `Em316LaserSphere` |
| `game/Em316LastLaser.cpp` | `0x8031B0A8–0x8031CA30` | `Em316LastLaser` |
| `game/Em401_01_WhiteLeoPard.cpp` | `0x8031EB3C–0x803227A0` | `Em401_01_WhiteLeoPard` |
| `game/Em403WarLion.cpp` | `0x80322C68–0x8032C52C` | `Em403WarLion` |
| `game/EnemyFieldMagician.cpp` | `0x8032C52C–0x8032DEA0` | `EnemyFieldMagician` |
| `game/EnemyJill.cpp` | `0x8032DEA0–0x803335C4` | `EnemyJill` |
| `game/EnemyLizerdBlade.cpp` | `0x803335C4–0x80334B38` | `EnemyLizerdBlade` |
| `game/EnemyManager.cpp` | `0x80334B38–0x80334CD8` | `EnemyManager` |
| `game/EnemyQuarkFirst.cpp` | `0x80334CD8–0x803388A4` | `EnemyQuarkFirst` |
| `game/EnemyQuarkSecond.cpp` | `0x803388A4–0x8033C3D8` | `EnemyQuarkSecond` |
| `game/EnemyQuarkStand.cpp` | `0x80352EC8–0x803545E0` | `EnemyQuarkStand` |
| `game/EnemySeaLizerd.cpp` | `0x803545E0–0x803559F8` | `EnemySeaLizerd` |
| `game/EnemyTrista.cpp` | `0x8035BC84–0x8035DF08` | `EnemyTrista` |
| `game/EnvSeManager.cpp` | `0x8035DFB8–0x8035E720` | `EnvSeManager` |
| `game/EnvSeBase.cpp` | `0x80360C5C–0x80363284` | `EnvSeBase`, `EnvSePoint`, `EnvSeLine`, `EnvSeSphere`, `EnvSeBox`, `EnvSeGlobal` |
| `game/EquipItemData.cpp` | `0x80363284–0x80363C28` | `EquipItemData` |
| `game/FriendArganan.cpp` | `0x80365390–0x8036545C` | `FriendArganan` |
| `game/FriendManager.cpp` | `0x80365464–0x8036554C` | `FriendManager` |
| `game/GameTask.cpp` | `0x803658B8–0x8036CA60` | `GameTask`, `GamePauseTask` |
| `game/GameOverTask.cpp` | `0x80376BE8–0x80378474` | `GameOverTask` |
| `game/GameBgm.cpp` | `0x8037C8C4–0x8037D8B8` | `GameBgm` |
| `std/bad_cast.cpp` | `0x803936A4–0x8039BDB4` | `std::bad_cast`, `boost::bad_lexical_cast`, `GameEventScript` |
| `game/GameLoadBackground.cpp` | `0x803B33F8–0x803B35FC` | `GameLoadBackground` |
| `game/GameChoiceWindow.cpp` | `0x803B434C–0x803B6970` | `GameChoiceWindow`, `GameMessageWindow`, `GameTextMessageWindow` |
| `game/GameSaveBackground.cpp` | `0x803B8C6C–0x803B9044` | `GameSaveBackground` |
| `game/GameSaveBanner.cpp` | `0x803B9044–0x803B9804` | `GameSaveBanner`, `atn::CsvBase<wchar_t, 9, 13, 34>`, `atn::CsvTabU` |
| `game/GameScene.cpp` | `0x803BEC40–0x803C3664` | `GameScene` |
| `game/GameSoundControl.cpp` | `0x803CD9A8–0x803CE070` | `GameSoundControl` |
| `game/GameTimer.cpp` | `0x803CE4B0–0x803CE688` | `GameTimer` |
| `game/GameUI.cpp` | `0x803CE980–0x803CFC78` | `GameUI` |
| `game/GimmickManager.cpp` | `0x803EDFBC–0x803EF590` | `GimmickManager`, `LoadSeTableTask` |
| `game/GimmickStateCommand.cpp` | `0x803F11F8–0x803F1A38` | `GimmickStateCommand` |
| `gmk/FragileObject.cpp` | `0x803F1C38–0x803F471C` | `gmk::FragileObject` |
| `gmk/AutomaticDoor.cpp` | `0x803F47A0–0x803F69E4` | `gmk::AutomaticDoor` |
| `gmk/PlayEffect.cpp` | `0x803F69E4–0x803F744C` | `gmk::PlayEffect` |
| `gmk/OnOff.cpp` | `0x803F75B8–0x803F9420` | `gmk::OnOff` |
| `gmk/ReinforcePoint.cpp` | `0x803F9420–0x803F9F34` | `gmk::ReinforcePoint` |
| `gmk/Cannonball.cpp` | `0x803F9F34–0x803FBFA8` | `gmk::Cannonball` |
| `gmk/Item.cpp` | `0x803FBFA8–0x803FC8D8` | `gmk::Item` |
| `gmk/MeetingSpot.cpp` | `0x803FC8D8–0x803FDFC0` | `gmk::MeetingSpot` |
| `gmk/SwingObject.cpp` | `0x803FE390–0x80400728` | `gmk::SwingObject` |
| `gmk/Leaf.cpp` | `0x80400728–0x80402978` | `gmk::Leaf` |
| `gmk/Bat.cpp` | `0x80402978–0x804047A4` | `gmk::Bat` |
| `gmk/PigeonManager.cpp` | `0x804048CC–0x80406754` | `gmk::PigeonManager` |
| `gmk/Brazier.cpp` | `0x80406754–0x80408324` | `gmk::Brazier` |
| `gmk/ChickenManager.cpp` | `0x8040B668–0x8040DC30` | `gmk::ChickenManager` |
| `gmk/TreasureBox.cpp` | `0x8040DC30–0x8040EC30` | `gmk::TreasureBox` |
| `gmk/HealPoint.cpp` | `0x8040EC30–0x8040FAC0` | `gmk::HealPoint` |
| `gmk/FallingTower.cpp` | `0x8040FAC0–0x80410A68` | `gmk::FallingTower` |
| `gmk/Trap.cpp` | `0x80410A68–0x80411440` | `gmk::Trap` |
| `gmk/Coffin.cpp` | `0x80411440–0x80412AFC` | `gmk::Coffin` |
| `gmk/MotionObject.cpp` | `0x80412B80–0x80414AC4` | `gmk::MotionObject` |
| `gmk/SkeltonsMirror.cpp` | `0x80414AC4–0x8041524C` | `gmk::SkeltonsMirror` |
| `gmk/Lemon.cpp` | `0x8041524C–0x804166B0` | `gmk::Lemon` |
| `gmk/Banana.cpp` | `0x804166B0–0x80416F74` | `gmk::Banana` |
| `gmk/FlamingArrow.cpp` | `0x80416F74–0x80418198` | `gmk::FlamingArrow` |
| `gmk/Ocean.cpp` | `0x80418198–0x80418EAC` | `gmk::Ocean` |
| `gmk/WaitObject.cpp` | `0x80419010–0x8041B284` | `gmk::WaitObject` |
| `gmk/BulletPoint.cpp` | `0x8041B970–0x8041C09C` | `gmk::BulletPoint` |
| `gmk/Courtyard.cpp` | `0x8041C09C–0x8041CD24` | `gmk::Courtyard` |
| `gmk/Fireworks.cpp` | `0x8041CD54–0x8041DDE4` | `gmk::Fireworks` |
| `gmk/ArenaFloor.cpp` | `0x8041DDE4–0x8041EB80` | `gmk::ArenaFloor` |
| `gmk/Augur.cpp` | `0x8041EBF4–0x8041FED8` | `gmk::Augur` |
| `gmk/RoomLight.cpp` | `0x8041FF08–0x80420ADC` | `gmk::RoomLight` |
| `gmk/LandMine.cpp` | `0x80420ADC–0x80421454` | `gmk::LandMine` |
| `gmk/ShakeCamera.cpp` | `0x80421454–0x80421AB8` | `gmk::ShakeCamera` |
| `gmk/AttractHatePoint.cpp` | `0x80421AB8–0x804225E0` | `gmk::AttractHatePoint` |
| `gmk/CharaLight.cpp` | `0x80422610–0x80423340` | `gmk::CharaLight` |
| `gmk/DoorSeal.cpp` | `0x8042335C–0x80423F64` | `gmk::DoorSeal` |
| `gmk/RotaryRoom.cpp` | `0x80423F64–0x80426108` | `gmk::RotaryRoom` |
| `gmk/SparkCube.cpp` | `0x8042618C–0x80427730` | `gmk::SparkCube` |
| `gmk/FlyObjManager.cpp` | `0x80427730–0x80428B0C` | `gmk::FlyObjManager` |
| `gmk/InteractionSpot.cpp` | `0x80428B0C–0x80429668` | `gmk::InteractionSpot` |
| `gmk/MapLift.cpp` | `0x804296DC–0x8042AB58` | `gmk::MapLift` |
| `gmk/RainDrops.cpp` | `0x8042AFE4–0x8042B56C` | `gmk::RainDrops` |
| `gmk/OpenBoard.cpp` | `0x8042B56C–0x8042C21C` | `gmk::OpenBoard` |
| `gmk/FloatingBox.cpp` | `0x8042C21C–0x8042CE30` | `gmk::FloatingBox` |
| `gmk/CharaObj.cpp` | `0x8042CE30–0x8042D628` | `gmk::CharaObj`, `gmk::GmkChara` |
| `gmk/PassBoard.cpp` | `0x8042D628–0x8042E36C` | `gmk::PassBoard` |
| `gmk/WaterLevel.cpp` | `0x8042E36C–0x8042F0B4` | `gmk::WaterLevel` |
| `gmk/TodomeCountUp.cpp` | `0x8042F0B4–0x8042F7EC` | `gmk::TodomeCountUp` |
| `gmk/LemonBucket.cpp` | `0x8042F860–0x804310B0` | `gmk::LemonBucket` |
| `gmk/RandomMerchant.cpp` | `0x8043119C–0x80432CF0` | `gmk::RandomMerchant`, `RandomMerchantChara` |
| `gmk/RandomItem.cpp` | `0x80432CF0–0x8043465C` | `gmk::RandomItem` |
| `gmk/SavePoint.cpp` | `0x80435CE4–0x80436E34` | `gmk::SavePoint` |
| `gmk/WindyItem.cpp` | `0x80436EB8–0x804392C0` | `gmk::WindyItem` |
| `gmk/LottaSummon.cpp` | `0x8043A770–0x8043B778` | `gmk::LottaSummon` |
| `gmk/Chain.cpp` | `0x8043B7E8–0x8043D594` | `gmk::Chain` |
| `gmk/CharaPathMove.cpp` | `0x8043D594–0x8043F028` | `gmk::CharaPathMove` |
| `RecycleBuffer_gmk/ItemSlotAnime.cpp` | `0x8044D9BC–0x80452710` | `RecycleBuffer<gmk::ItemSlotAnime>`, `gmk::ItemSlotAnimeBuffer` |
| `game/LastCocoonChrysalis.cpp` | `0x80452710–0x80452F14` | `LastCocoonChrysalis` |
| `game/LastCocoonEgg.cpp` | `0x80452F14–0x804576D4` | `LastCocoonEgg` |
| `game/LastCocoonImago.cpp` | `0x80458270–0x8045FAB0` | `LastCocoonImago` |
| `game/LastCocoonQuadruped.cpp` | `0x8045FAB0–0x80468F48` | `LastCocoonQuadruped` |
| `game/LibraryTask.cpp` | `0x80468FBC–0x8046ACDC` | `LibraryTask`, `BookTask` |
| `atn/LH_ColliObject.cpp` | `0x80472344–0x80472C90` | `atn::LH_ColliObject`, `atn::LoadColliObject` |
| `atn/LH_ColliTree.cpp` | `0x80472CAC–0x80472FAC` | `atn::LH_ColliTree`, `atn::LoadColliTree` |
| `atn/LH_Effect.cpp` | `0x804731C0–0x80473B20` | `atn::LH_Effect`, `atn::LoadEffect` |
| `atn/LH_Efp.cpp` | `0x80473B20–0x80473E74` | `atn::LH_Efp`, `atn::LoadEfp` |
| `atn/LH_Material.cpp` | `0x80474224–0x80474860` | `atn::LH_Material`, `atn::LoadMaterial` |
| `atn/LH_Model.cpp` | `0x80474888–0x80476130` | `atn::LH_Model`, `atn::LoadModel` |
| `atn/LH_Motion.cpp` | `0x80476220–0x804766B0` | `atn::LH_Motion`, `atn::LoadMotion` |
| `atn/LH_MotionCommandData.cpp` | `0x804768EC–0x80476CE4` | `atn::LH_MotionCommandData`, `atn::LoadMotionCommandData` |
| `atn/LH_MotionSet.cpp` | `0x80476D64–0x80477694` | `atn::LH_MotionSet`, `atn::LoadMotionSet` |
| `atn/LH_Scene.cpp` | `0x80477778–0x80477FA4` | `atn::LH_Scene`, `atn::LoadScene` |
| `atn/LH_Texture.cpp` | `0x80478054–0x804783DC` | `atn::LH_Texture`, `atn::LoadTexture` |
| `atn/LH_Xsca.cpp` | `0x804783DC–0x804786F8` | `atn::LH_Xsca`, `atn::LoadXsca` |
| `game/LogMessageTask.cpp` | `0x80478714–0x80479998` | `LogMessageTask` |
| `Event/Manager.cpp` | `0x80487184–0x80488050` | `Event::Manager` |
| `Event/PostSequence.cpp` | `0x8048FF60–0x8048FFCC` | `Event::PostSequence` |
| `Event/SequenceRoot.cpp` | `0x8048FFCC–0x8049003C` | `Event::SequenceRoot` |
| `game/MapManager.cpp` | `0x80490EB8–0x80491418` | `MapManager` |
| `game/MapPartsBase.cpp` | `0x80491EA0–0x80496420` | `MapPartsBase`, `MapPartsArea` |
| `game/MapPartsColliTree.cpp` | `0x80496420–0x804965EC` | `MapPartsColliTree` |
| `game/MapPartsEfp.cpp` | `0x804965EC–0x80496A28` | `MapPartsEfp` |
| `game/MapPartsFlare.cpp` | `0x80496C3C–0x80496EE8` | `MapPartsFlare` |
| `game/MapPartsGimmickLoc.cpp` | `0x80496F6C–0x8049A018` | `MapPartsGimmickLoc` |
| `game/MapPartsLocatorSet.cpp` | `0x8049A09C–0x8049B9B8` | `MapPartsLocatorSet` |
| `game/MapPartsModel.cpp` | `0x8049B9B8–0x8049C8DC` | `MapPartsModel` |
| `game/MapPartsOcean.cpp` | `0x8049C8DC–0x8049CDA0` | `MapPartsOcean` |
| `game/MapPartsRain.cpp` | `0x8049CDBC–0x8049D264` | `MapPartsRain` |
| `game/MapTask.cpp` | `0x8049D7E4–0x8049ED90` | `MapTask` |
| `game/MarketTask.cpp` | `0x804A1DDC–0x804A24C4` | `MarketTask` |
| `game/MenuCommonTask.cpp` | `0x804A2A34–0x804A39EC` | `MenuCommonTask` |
| `game/MiniMapTask.cpp` | `0x804A7F18–0x804A9E44` | `MiniMapTask` |
| `MP/D2ACommonDialog.cpp` | `0x804AC488–0x804ADCAC` | `MP::D2ACommonDialog` |
| `MP/CryptMP.cpp` | `0x804ADCAC–0x804AE4C4` | `MP::CryptMP` |
| `MP/D2AMpMenu.cpp` | `0x804AE888–0x804B4400` | `MP::D2AMpMenu`, `MP::D2AMpMenuRankMark` |
| `MP/D2AMpMenuCockpit.cpp` | `0x804B4CB8–0x804B6988` | `MP::D2AMpMenuCockpit` |
| `MP/D2AMpMenuFriend.cpp` | `0x804B69F0–0x804B9014` | `MP::D2AMpMenuFriend` |
| `MP/D2AMpMenuFriendRoom.cpp` | `0x804B9DAC–0x804BBB60` | `MP::D2AMpMenuFriendRoom` |
| `MP/D2AMpMenuLobby.cpp` | `0x804BBC28–0x804C18A8` | `MP::D2AMpMenuLobby` |
| `MP/D2AMpMenuMessageList.cpp` | `0x804C3060–0x804C50C0` | `MP::D2AMpMenuMessageList` |
| `MP/D2AMpMenuResult.cpp` | `0x804C56D4–0x804C7408` | `MP::D2AMpMenuResult` |
| `MP/D2AMpMenuSetMessage.cpp` | `0x804C87E0–0x804CC618` | `MP::D2AMpMenuSetMessage` |
| `MP/D2AMpMenuSetRule.cpp` | `0x804CD9E4–0x804CF190` | `MP::D2AMpMenuSetRule` |
| `atn/PacketPack.cpp` | `0x804F7F54–0x804FC7E0` | `atn::net::PacketPack`, `MP::Manager2`, `MP::ManagerMP`, `MP::Chara<EnemyTask>` |
| `MP/func_enter_setting.cpp` | `0x804FFE04–0x80500734` | `MP::func_enter_setting` |
| `MP/Popup.cpp` | `0x805012C4–0x805020FC` | `MP::Popup` |
| `MP/SaveLoad.cpp` | `0x805020FC–0x80502708` | `MP::SaveLoad` |
| `atn/SoftKeyboardU.cpp` | `0x805029DC–0x80505DD0` | `atn::SoftKeyboardU`, `MP::SoftKeyboard` |
| `atn/PPDATAU.cpp` | `0x8050D7EC–0x8050DDDC` | `atn::net::PPDATAU`, `atn::net::PPDATAR` |
| `game/NewCampPartsGmk.cpp` | `0x805128C0–0x80512C90` | `NewCampPartsGmk` |
| `game/NewCampPartsHint.cpp` | `0x80514784–0x805169F8` | `NewCampPartsHint` |
| `game/NewCampPartsItem.cpp` | `0x80516A5C–0x8051B900` | `NewCampPartsItem` |
| `game/NewCampPartsMapSelect.cpp` | `0x8051B984–0x80521DF8` | `NewCampPartsMapSelect` |
| `game/NewCampPartsOption.cpp` | `0x80521E80–0x80526358` | `NewCampPartsOption` |
| `game/NewCampPartsStatus.cpp` | `0x8052692C–0x805290F8` | `NewCampPartsStatus` |
| `game/NewCampPartsTop.cpp` | `0x80529118–0x8052A888` | `NewCampPartsTop` |
| `game/NewCampTask.cpp` | `0x8052A8EC–0x8052C108` | `NewCampTask` |
| `game/NewStaffRoll.cpp` | `0x8053108C–0x80531E80` | `NewStaffRoll` |
| `game/NewTitleTask.cpp` | `0x80531F44–0x80536CA8` | `NewTitleTask` |
| `Event/Node.cpp` | `0x80537708–0x8053D3F8` | `Event::Node`, `Event::Action` |
| `Event/StringData.cpp` | `0x805430E8–0x805430F8` | `Event::StringData` |
| `Event/DummyData.cpp` | `0x805430F8–0x80543108` | `Event::DummyData` |
| `game/Np131LivingArmor.cpp` | `0x80546000–0x80547468` | `Np131LivingArmor` |
| `game/NpcManager.cpp` | `0x805477C8–0x80547980` | `NpcManager` |
| `game/NpcTask.cpp` | `0x80547C28–0x80547DF8` | `NpcTask` |
| `game/PickTask.cpp` | `0x80548D54–0x80549D8C` | `PickTask` |
| `game/PopupTask.cpp` | `0x8054A750–0x8054D794` | `PopupTask` |
| `Event/ActionActorBehavior.cpp` | `0x8054E574–0x80551108` | `Event::ActionActorBehavior` |
| `Event/ActionActorPosition.cpp` | `0x80552460–0x8055A400` | `Event::ActionActorPosition`, `Event::ActionActorMotion`, `Event::ActionActorFacial`, `Event::ActionActorEyes`, `Event::ActionActorPose`, `Event::ActionActorHeading`, `Event::ActionActorNode`, `Event::ActionActorSetting`, `Event::ActionActorWind`, `Event::ActionActorMaterial` |
| `Event/ActionCameraMode.cpp` | `0x8055A49C–0x8055D6B4` | `Event::ActionCameraMode`, `Event::ActionCameraEffect` |
| `Event/ActionEffectPlay.cpp` | `0x8055DBF4–0x805634D0` | `Event::ActionEffectPlay`, `Event::ActionAnimationPlay`, `Event::ActionCheck`, `Event::ActionNotify`, `Event::ActionMessageChoice`, `Event::ActionMessageWindow`, `Event::ActionMessagePrint`, `Event::ActionSoundPlay`, `Event::ActionEnvSound`, `Event::ActionMusicPlay`, `Event::ActionAudioEffect`, `Event::ActionVoicePlay`, `Event::ActionMultiAudio`, `Event::ActionMoviePlay`, `Event::ActionGodRay`, `Event::ActionSunLight`, `Event::ActionAmbientLight`, `Event::ActionCloudMap`, `Event::ActionPointLight`, `Event::ActionSpotLight`, `Event::ActionLightSurround`, `Event::ActionBrightness`, `Event::ActionColorMatrix`, `Event::ActionColorGradation` |
| `game/QuadrupedTask.cpp` | `0x80566420–0x80568EE0` | `QuadrupedTask` |
| `game/ReplayBuffer.cpp` | `0x8056BF8C–0x8056F5B4` | `ReplayBuffer` |
| `game/SelectMember.cpp` | `0x8057162C–0x805727A4` | `SelectMember` |
| `atn/SequenceUnitParam1_const_int.cpp` | `0x80574750–0x80575018` | `atn::SequenceHolder::SequenceUnitParam1<const int>`, `atn::SequenceHolder::SequenceUnitParam1<const GameTask::Option>`, `atn::SequenceHolder::SequenceUnitParam1<const GameLoadTask::Option>` |
| `game/SetConfig.cpp` | `0x8057E83C–0x8057F0F8` | `SetConfig` |
| `game/ShopBase.cpp` | `0x8057F280–0x80584FE4` | `ShopBase` |
| `game/ShopBuySell.cpp` | `0x80585080–0x80586DB0` | `ShopBuySell` |
| `game/ShopColor.cpp` | `0x80586E3C–0x80588684` | `ShopColor` |
| `game/ShopIdentify.cpp` | `0x80588710–0x8058A3AC` | `ShopIdentify` |
| `game/ShopUpGrade.cpp` | `0x8058AC90–0x8058CA44` | `ShopUpGrade` |
| `game/SkillEffectManager.cpp` | `0x8059AB98–0x8059C084` | `SkillEffectManager` |
| `nw4r/DvdSoundArchive.cpp` | `0x8059D7D8–0x8059E21C` | `nw4r::snd::DvdSoundArchive`, `nw4r::snd::DvdSoundArchive::DvdFileStream` |
| `nw4r/SoundArchive.cpp` | `0x8059E21C–0x8059E280` | `nw4r::snd::SoundArchive` |
| `nw4r/SeqNoteOnCallback.cpp` | `0x8059E7EC–0x805A21C0` | `nw4r::snd::SoundArchivePlayer::SeqNoteOnCallback`, `nw4r::snd::SoundArchivePlayer::WsdCallback`, `nw4r::snd::SoundArchivePlayer` |
| `game/StrapTask.cpp` | `0x805A6208–0x805A6A80` | `StrapTask` |
| `game/Sword.cpp` | `0x805A6A80–0x805A6B54` | `Sword` |
| `game/TacView.cpp` | `0x805A6D20–0x805A96D4` | `TacView` |
| `game/TeamAIManager.cpp` | `0x805B3420–0x805B4D90` | `TeamAIManager` |
| `game/TownTask.cpp` | `0x805B6C5C–0x805B7B24` | `TownTask` |
| `game/TownTpcLoader.cpp` | `0x805B9B08–0x805B9EC8` | `TownTpcLoader` |
| `game/TutorialTask.cpp` | `0x805B9FC4–0x805BD4B8` | `TutorialTask`, `TutorialPopup` |
