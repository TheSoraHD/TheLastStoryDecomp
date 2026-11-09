# Ruta de arranque: de `main()` a la pantalla de titulo

Estado de todo lo que se ejecuta desde `main()` (game/main.cpp) hasta la pantalla de titulo
(game/NewTitleTask), al cierre de `arranque-20261109`. Las tablas salen de `tools/decomp/ruta_arranque.py`
(volver a generarlas con `python3 tools/decomp/ruta_arranque.py` tras un `ninja` completo).

## Recorrido

1. `main` (game/main.cpp): `OSInitFastCast`, heaps (atn/LinearAllocator, game/HeapInfo_unk), configuracion
   (game/SetConfig, game/GameConfig_unk), sistema de ficheros (atn/Loader, atn/FileList, atn/FileUtil_unk,
   atn/NandManager, atn/ResRef_unk), video (atn/GraphicsDevice, atn/RenderSystem_unk = `atn::RenderManager`,
   atn/DrawHelper), sonido (atn/AudioDevice, atn/SoundManager), mando (atn/Pad_unk), menu de depuracion
   (atn/MindowsCommon), menu HOME (game/HomeButtonMenu), reset/apagado (game/GameSystem_unk =
   `ResetShutdown`), errores (game/SystemErrorMessage_unk = `ErrorManager`) y layouts (atn/D2A_Manager).
2. Bucle de `main`: por frame `atn::Task` (arbol de tareas), audio (`AudioDevice::fn_80047DCC`,
   `SoundManager::fn_800CE6E0`/`fn_800CECF0`), `fn_80071D60`, `ResetShutdown::fn_80570CF8` y los callbacks
   de frame (`ResetShutdown`, `ErrorManager`), y el dibujo (`RenderManager::fn_800BDBC8`).
3. Secuencias (atn/SequenceHolder, atn/SequenceUnitParam1_const_int): game/BootTask -> game/StrapTask (avisos
   de correa) -> game/NewTitleTask (titulo).

## Unidades de la ruta

| Unidad | 100 % | casi (>= 99 %) | total | DRAFT | enlaza | 100 % antes |
|---|---|---|---|---|---|---|
| `game/main` | 11 | 2 | 14 | 0 | no | 11 |
| `atn/Task` | 13 | 3 | 25 | 0 | no | 12 |
| `atn/Loader` | 23 | 5 | 31 | 0 | no | 21 |
| `atn/FileList` | 21 | 2 | 27 | 0 | no | 20 |
| `atn/FileUtil_unk` | 10 | 1 | 16 | 0 | no | 9 |
| `atn/NandManager` | 32 | 1 | 34 | 0 | no | 32 |
| `atn/LinearAllocator` | 24 | 0 | 24 | 0 | si | 24 |
| `atn/ResRef_unk` | 5 | 0 | 5 | 0 | si | 5 |
| `game/GameConfig_unk` | 11 | 0 | 15 | 0 | no | 11 |
| `game/SystemErrorMessage_unk` | 9 | 1 | 11 | 0 | no | 9 |
| `game/GameSystem_unk` | 6 | 1 | 7 | 0 | no | 10 |
| `game/SetConfig` | 8 | 0 | 8 | 0 | si | 8 |
| `game/HeapInfo_unk` | 4 | 0 | 4 | 0 | si | 4 |
| `game/HomeButtonMenu` | 5 | 0 | 5 | 0 | si | 5 |
| `atn/GraphicsDevice` | 30 | 6 | 41 | 0 | no | 26 |
| `atn/RenderSystem_unk` | 30 | 2 | 37 | 0 | no | 20 |
| `atn/DrawHelper` | 54 | 6 | 68 | 0 | no | 54 |
| `atn/D2A_Manager` | 18 | 0 | 21 | 0 | no | 18 |
| `atn/AudioDevice` | 49 | 3 | 56 | 0 | no | 44 |
| `atn/SoundManager` | 69 | 5 | 76 | 0 | no | 68 |
| `atn/Pad_unk` | 15 | 0 | 17 | 0 | no | 15 |
| `atn/MindowsCommon` | 51 | 6 | 57 | 0 | no | 50 |
| `atn/SequenceHolder` | 8 | 1 | 11 | 0 | no | 7 |
| `atn/SequenceUnitParam1_const_int` | 34 | 3 | 37 | 0 | no | 34 |
| `game/BootTask` | 4 | 0 | 4 | 0 | si | 0 |
| `game/StrapTask` | 5 | 0 | 6 | 0 | no | 5 |
| `game/NewTitleTask` | 2 | 3 | 9 | 0 | no | 1 |
| **total** | 551 | 51 | 666 | 0 | 6 de 27 | 523 |

## Librerias que llama la ruta

| Objeto de libreria | enlaza | funciones llamadas desde la ruta |
|---|---|---|
| `PowerPC_EABI_Support/src/MSL_C/MSL_Common/printf.c` | si | `fprintf`, `sprintf`, `vsprintf` |
| `PowerPC_EABI_Support/src/MSL_C/MSL_Common/scanf.c` | si | `sscanf` |
| `PowerPC_EABI_Support/src/MSL_C/MSL_Common/string.c` | si | `strcat`, `strcmp`, `strcpy`, `strncmp`, `strncpy`, `strrchr`, `strstr`, `strtok` |
| `PowerPC_EABI_Support/src/MSL_C/MSL_Common/wstring.c` | si | `wcscpy`, `wcslen`, `wcsncmp` |
| `PowerPC_EABI_Support/src/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_cos.c` | si | `cos` |
| `PowerPC_EABI_Support/src/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_sin.c` | si | `sin` |
| `PowerPC_EABI_Support/src/MSL_C/PPC_EABI/abort_exit_ppc_eabi.c` | si | `abort` |
| `PowerPC_EABI_Support/src/Runtime/global_destructor_chain.c` | si | `__register_global_object` |
| `PowerPC_EABI_Support/src/Runtime/ptmf.c` | si | `__ptmf_scall`, `__ptmf_test` |
| `PowerPC_EABI_Support/src/Runtime/runtime.c` | si | `__cvt_fp2unsigned`, `_restgpr_14`, `_restgpr_17`, `_restgpr_18`, `_restgpr_19`, `_restgpr_20`, `_restgpr_21`, `_restgpr_22`, `_restgpr_23`, `_restgpr_24`, `_restgpr_25`, `_restgpr_26` (+13) |
| `RVL_SDK/src/revolution/ai/ai.c` | si | `AIInit` |
| `RVL_SDK/src/revolution/ax/AX.c` | si | `AXInit` |
| `RVL_SDK/src/revolution/ax/AXCL.c` | si | `AXSetCompressor` |
| `RVL_SDK/src/revolution/dvd/dvd.c` | si | `DVDCheckDisk`, `DVDGetDriveStatus`, `DVDInit` |
| `RVL_SDK/src/revolution/gx/GXAttr.c` | si | `GXClearVtxDesc`, `GXInvalidateVtxCache`, `GXSetNumTexGens`, `GXSetTexCoordGen2`, `GXSetVtxAttrFmt`, `GXSetVtxAttrFmtv`, `GXSetVtxDesc`, `GXSetVtxDescv` |
| `RVL_SDK/src/revolution/gx/GXBump.c` | si | `GXSetIndTexCoordScale`, `GXSetIndTexOrder`, `GXSetNumIndStages`, `GXSetTevDirect`, `GXSetTevIndirect` |
| `RVL_SDK/src/revolution/gx/GXGeometry.c` | si | `GXBegin`, `GXEnableTexOffsets`, `GXSetCoPlanar`, `GXSetCullMode`, `GXSetLineWidth`, `GXSetPointSize` |
| `RVL_SDK/src/revolution/gx/GXLight.c` | si | `GXInitLightAttn`, `GXInitLightColor`, `GXInitLightDistAttn`, `GXInitLightPos`, `GXInitSpecularDir`, `GXLoadLightObjImm`, `GXSetChanAmbColor`, `GXSetChanCtrl`, `GXSetChanMatColor`, `GXSetNumChans` |
| `RVL_SDK/src/revolution/gx/GXTransform.c` | si | `GXGetViewportv`, `GXLoadNrmMtxImm`, `GXLoadPosMtxImm`, `GXLoadTexMtxImm`, `GXProject`, `GXSetClipMode`, `GXSetCurrentMtx`, `GXSetProjection`, `GXSetScissor`, `GXSetScissorBoxOffset`, `GXSetViewport` |
| `RVL_SDK/src/revolution/hbm/HBMCommon.cpp` | si | `HBMStartBlackOut` |
| `RVL_SDK/src/revolution/mtx/mtx.c` | si | `PSMTXConcat`, `PSMTXIdentity`, `PSMTXInvXpose`, `PSMTXInverse`, `PSMTXRotAxisRad`, `PSMTXRotRad`, `PSMTXScale`, `PSMTXTrans`, `PSMTXTranspose` |
| `RVL_SDK/src/revolution/mtx/mtx44.c` | si | `C_MTXOrtho` |
| `RVL_SDK/src/revolution/mtx/mtxvec.c` | si | `PSMTXMultVec` |
| `RVL_SDK/src/revolution/nwc24/NWC24Schedule.c` | si | `NWC24SuspendScheduler` |
| `RVL_SDK/src/revolution/os/OS.c` | si | `OSInit` |
| `RVL_SDK/src/revolution/os/OSCache.c` | si | `DCFlushRange`, `DCInvalidateRange`, `LCDisable`, `LCEnable` |
| `RVL_SDK/src/revolution/os/OSError.c` | si | `OSPanic`, `OSReport` |
| `RVL_SDK/src/revolution/os/OSFont.c` | si | `OSGetFontEncode`, `OSGetFontTexture`, `OSInitFont` |
| `RVL_SDK/src/revolution/os/OSInterrupt.c` | si | `OSDisableInterrupts`, `OSRestoreInterrupts` |
| `RVL_SDK/src/revolution/os/OSMutex.c` | si | `OSInitMutex`, `OSLockMutex`, `OSUnlockMutex` |
| `RVL_SDK/src/revolution/os/OSStateTM.c` | si | `OSSetPowerCallback`, `OSSetResetCallback` |
| `RVL_SDK/src/revolution/os/OSThread.c` | si | `OSInitThreadQueue`, `OSSleepThread`, `OSSleepTicks`, `OSWakeupThread` |
| `RVL_SDK/src/revolution/os/OSTime.c` | si | `OSGetTime` |
| `RVL_SDK/src/revolution/sc/scapi.c` | si | `SCGetAspectRatio`, `SCGetEuRgb60Mode`, `SCGetLanguage`, `SCGetProgressiveMode`, `SCGetSoundMode` |
| `msc/src/RVL_SDK/kpad/KPAD.c` | no | `KPADEnableDPD` |
| `nw4r/DvdSoundArchive.cpp` | ? | `Close__Q34nw4r3snd15DvdSoundArchiveFv`, `LoadHeader__Q34nw4r3snd15DvdSoundArchiveFPvUl`, `LoadLabelStringData__Q34nw4r3snd15DvdSoundArchiveFPvUl`, `Open__Q34nw4r3snd15DvdSoundArchiveFPCc`, `__ct__Q34nw4r3snd15DvdSoundArchiveFv`, `__dt__Q34nw4r3snd15DvdSoundArchiveFv` |
| `nw4r/SoundArchive.cpp` | ? | `ConvertLabelStringToGroupId__Q34nw4r3snd12SoundArchiveCFPCc`, `ConvertLabelStringToSoundId__Q34nw4r3snd12SoundArchiveCFPCc`, `GetSoundType__Q34nw4r3snd12SoundArchiveCFUl`, `detail_ReadGroupInfo__Q34nw4r3snd12SoundArchiveCFUlPQ44nw4r3snd12SoundArchive9GroupInfo` |
| `nw4r/src/snd/snd_AxManager.cpp` | si | `AppendEffect__Q44nw4r3snd6detail9AxManagerFQ34nw4r3snd6AuxBusPQ34nw4r3snd6FxBase`, `ClearEffect__Q44nw4r3snd6detail9AxManagerFQ34nw4r3snd6AuxBusi`, `GetInstance__Q44nw4r3snd6detail9AxManagerFv`, `GetOutputMode__Q44nw4r3snd6detail9AxManagerFv`, `SetMasterVolume__Q44nw4r3snd6detail9AxManagerFfi`, `SetOutputMode__Q44nw4r3snd6detail9AxManagerFQ34nw4r3snd10OutputMode` |
| `nw4r/src/snd/snd_FrameHeap.cpp` | si | `GetCurrentLevel__Q44nw4r3snd6detail9FrameHeapCFv`, `GetSize_unk__Q44nw4r3snd6detail9FrameHeapCFv` |
| `nw4r/src/snd/snd_SoundHeap.cpp` | si | `Clear__Q34nw4r3snd9SoundHeapFv`, `Create__Q34nw4r3snd9SoundHeapFPvUl`, `Destroy__Q34nw4r3snd9SoundHeapFv`, `LoadState__Q34nw4r3snd9SoundHeapFi`, `SaveState__Q34nw4r3snd9SoundHeapFv`, `__ct__Q34nw4r3snd9SoundHeapFv`, `__dt__Q34nw4r3snd9SoundHeapFv` |
| `nw4r/src/snd/snd_SoundSystem.cpp` | si | `InitSoundSystem__Q34nw4r3snd11SoundSystemFll`, `ShutdownSoundSystem__Q34nw4r3snd11SoundSystemFv` |
| `nw4r/src/ut/ut_LinkList.cpp` | si | `__dt__Q44nw4r2ut6detail12LinkListImplFv` |
| `smg2/src/RVL_SDK/dvd/dvdFatal.c` | si | `DVDSetAutoFatalMessaging` |
| `smg2/src/RVL_SDK/dvd/dvdfs.c` | si | `DVDClose`, `DVDConvertPathToEntrynum`, `DVDFastOpenDir`, `DVDOpen`, `DVDReadAsyncPrio`, `DVDReadDir`, `DVDReadPrio` |
| `smg2/src/RVL_SDK/gx/GXFifo.c` | si | `GXDisableBreakPt`, `GXEnableBreakPt`, `GXGetCPUFifo`, `GXGetFifoPtrs`, `GXResetOverflowCount`, `GXSetBreakPtCallback` |
| `smg2/src/RVL_SDK/gx/GXFrameBuf.c` | si | `GXAdjustForOverscan`, `GXCopyDisp`, `GXCopyTex`, `GXGetYScaleFactor`, `GXSetCopyClear`, `GXSetCopyFilter`, `GXSetDispCopyDst`, `GXSetDispCopyGamma`, `GXSetDispCopySrc`, `GXSetDispCopyYScale`, `GXSetTexCopyDst`, `GXSetTexCopySrc` |
| `smg2/src/RVL_SDK/gx/GXInit.c` | si | `GXInit` |
| `smg2/src/RVL_SDK/gx/GXMisc.c` | si | `GXFlush`, `GXPixModeSync`, `GXReadDrawSync`, `GXSetDrawSync` |
| `smg2/src/RVL_SDK/gx/GXPixel.c` | si | `GXSetAlphaUpdate`, `GXSetBlendMode`, `GXSetColorUpdate`, `GXSetDither`, `GXSetDstAlpha`, `GXSetFog`, `GXSetFogRangeAdj`, `GXSetPixelFmt`, `GXSetZCompLoc`, `GXSetZMode` |
| `smg2/src/RVL_SDK/gx/GXTev.c` | si | `GXSetAlphaCompare`, `GXSetNumTevStages`, `GXSetTevAlphaIn`, `GXSetTevAlphaOp`, `GXSetTevColorIn`, `GXSetTevColorOp`, `GXSetTevKAlphaSel`, `GXSetTevKColor`, `GXSetTevKColorSel`, `GXSetTevOp`, `GXSetTevOrder`, `GXSetTevSwapMode` (+2) |
| `smg2/src/RVL_SDK/gx/GXTexture.c` | si | `GXGetTexBufferSize`, `GXGetTexObjFmt`, `GXGetTexObjHeight`, `GXGetTexObjWidth`, `GXInitTexObj`, `GXInitTexObjLOD`, `GXInvalidateTexAll`, `GXLoadTexObj`, `GXSetTexCoordCylWrap`, `GXSetTexCoordScaleManually`, `GXSetTexRegionCallback` |
| `smg2/src/RVL_SDK/mtx/vec.c` | si | `PSVECCrossProduct`, `PSVECDotProduct`, `PSVECMag`, `PSVECNormalize`, `PSVECSquareMag` |
| `smg2/src/RVL_SDK/nand/NANDCheck.c` | si | `NANDCheckAsync` |
| `smg2/src/RVL_SDK/nand/NANDCore.c` | si | `NANDChangeDir`, `NANDGetCurrentDir`, `NANDGetHomeDir`, `NANDGetType` |
| `smg2/src/RVL_SDK/nand/NANDErrorMessage.c` | si | `NANDSetAutoErrorMessaging` |
| `smg2/src/RVL_SDK/nand/NANDOpenClose.c` | si | `NANDClose`, `NANDCloseAsync`, `NANDOpen`, `NANDOpenAsync`, `NANDSafeCancelAsync_unk`, `NANDSimpleSafeCloseAsync`, `NANDSimpleSafeOpenAsync` |
| `smg2/src/RVL_SDK/nand/nand.c` | si | `NANDCreate`, `NANDCreateAsync`, `NANDCreateDir`, `NANDCreateDirAsync`, `NANDDeleteAsync`, `NANDGetLength`, `NANDGetUserData`, `NANDMoveAsync`, `NANDReadAsync`, `NANDSeek`, `NANDSeekAsync`, `NANDSetUserData` (+2) |
| `smg2/src/RVL_SDK/os/OSReset.c` | si | `OSRestart`, `OSReturnToDataManager`, `OSReturnToMenu`, `OSShutdownSystem` |
| `smg2/src/RVL_SDK/vi/vi.c` | si | `VIConfigure`, `VIFlush`, `VIGetDTVStatus`, `VIGetTvFormat`, `VIInit`, `VISetBlack`, `VISetNextFrameBuffer`, `VISetPostRetraceCallback`, `VISetPreRetraceCallback`, `VIWaitForRetrace` |
| `smg2/src/RVL_SDK/vi/vi3in1.c` | si | `VISetGamma` |
| `ss_nw4r/src/snd/snd_BasicSound.cpp` | si | `GetRemainingFadeFrames__Q44nw4r3snd6detail10BasicSoundCFv`, `Stop__Q44nw4r3snd6detail10BasicSoundFi` |
| `ss_nw4r/src/snd/snd_FxChorus.cpp` | si | `__ct__Q34nw4r3snd8FxChorusFv` |
| `ss_nw4r/src/snd/snd_FxDelay.cpp` | si | `__ct__Q34nw4r3snd7FxDelayFv` |
| `ss_nw4r/src/snd/snd_FxReverbStdDpl2.cpp` | si | `GetRequiredMemSize__Q34nw4r3snd15FxReverbStdDpl2Fv`, `SetParam__Q34nw4r3snd15FxReverbStdDpl2FRCQ44nw4r3snd6detail16FxReverbStdParam`, `__ct__Q34nw4r3snd15FxReverbStdDpl2Fv` |
| `ss_nw4r/src/snd/snd_RemoteSpeaker.cpp` | si | `Setup__Q34nw4r3snd13RemoteSpeakerFPFll_v`, `Shutdown__Q34nw4r3snd13RemoteSpeakerFPFll_v` |
| `ss_nw4r/src/snd/snd_RemoteSpeakerManager.cpp` | si | `GetInstance__Q44nw4r3snd6detail20RemoteSpeakerManagerFv`, `GetRemoteSpeaker__Q44nw4r3snd6detail20RemoteSpeakerManagerFi` |
| `ss_nw4r/src/snd/snd_Sound3DListener.cpp` | si | `SetInteriorSize__Q34nw4r3snd15Sound3DListenerFf`, `SetMaxVolumeDistance__Q34nw4r3snd15Sound3DListenerFf`, `SetUnitDistance__Q34nw4r3snd15Sound3DListenerFf` |
| `ss_nw4r/src/snd/snd_Sound3DManager.cpp` | si | `GetRequiredMemSize__Q34nw4r3snd14Sound3DManagerFPCQ34nw4r3snd12SoundArchive`, `SetEngine__Q34nw4r3snd14Sound3DManagerFPQ34nw4r3snd13Sound3DEngine`, `Setup__Q34nw4r3snd14Sound3DManagerFPCQ34nw4r3snd12SoundArchivePvUl`, `__ct__Q34nw4r3snd14Sound3DManagerFv` |
| `ss_nw4r/src/snd/snd_SoundArchivePlayer.cpp` | si | `GetRequiredMemSize__Q34nw4r3snd18SoundArchivePlayerFPCQ34nw4r3snd12SoundArchive`, `GetRequiredStrmBufferSize__Q34nw4r3snd18SoundArchivePlayerFPCQ34nw4r3snd12SoundArchive`, `LoadGroup_unk__Q34nw4r3snd18SoundArchivePlayerFUlPCvPCv`, `Setup__Q34nw4r3snd18SoundArchivePlayerFPCQ34nw4r3snd12SoundArchivePvUlPvUl`, `Shutdown__Q34nw4r3snd18SoundArchivePlayerFv`, `Update__Q34nw4r3snd18SoundArchivePlayerFv`, `__ct__Q34nw4r3snd18SoundArchivePlayerFv`, `__dt__Q34nw4r3snd18SoundArchivePlayerFv` |

## Asm aprobado que usa la ruta

Para una version portable (no PowerPC) estas piezas necesitan una version en C bajo `#ifdef`:

- `include/math/Vec.h`: copias paired-single `__VecCopy`, `__VecCopyZFirst`, `__Vec4Copy`, `__Vec2Copy`,
  `__MtxCopy`, `__Mtx44Copy` y `__VecMin`/`__VecMax`/`__VecMinMax`. Las usan los constructores de copia y
  `operator=` de `atn::Vector3`, `Vector2_unk`, `atn::Matrix3x4`: casi todas las unidades de la ruta
  (GraphicsDevice, RenderSystem_unk, DrawHelper, NewTitleTask...). Equivalente en C: copia de 2/3/4/12/16
  floats y min/max por componente.
- `include/sdk/OSFastCast.h` (`OSInitFastCast`, game/main.cpp): configura los GQR 2-5 para las conversiones
  rapidas; en C, nada (o las conversiones normales).
- atn/Model `fn_80099F74` y `fn_8009A018` (skinning en la cache bloqueada, `nofralloc`): los usan los modelos
  del mapa del titulo; en C, el bucle de mezcla de matrices por pesos (2 y N huesos).
- atn/ModelDraw_unk `SetGQR6_unk` y `SetMatColor_unk` (paired-single cuantizado GQR6): color de material de
  los modelos; en C, conversion u8 -> float y producto por el color.

El asm de NW4R math, MetroTRK, `VPanic` y boost (`sp_counted_base_cw_ppc.hpp`) no esta en la ruta de juego
(MetroTRK y `VPanic` solo en depuracion; boost `shared_ptr` si: `sp_counted_base` usa `lwarx`/`stwcx.`, en C
un contador atomico normal).

## Que falta para enlazar la ruta

- Unidades que no enlazan: todas las de la tabla con "no". Casi todas tienen funciones al 99,x % por
  registros o por el orden de las instrucciones (bloqueos de MWCC en `docs/mwcc-blockers.md`).
- `game/main` `fn_8047A4C8` (76,1 %): con `RegistFrameCallbacks_unk` inline (Create + 3 `push_back` de
  `boost::bind`); faltan los constructores de `boost::function`, que el original deja fuera de linea.
- atn/SequenceHolder: ya usa `boost::lambda` de Boost 1.35.0 (`libs/boost`); `fn_800C3D74` al 100 %,
  `fn_800C3E84` al 98,07 %; el constructor y `fn_800C3990` siguen por `std::vector`.
- atn/RenderSystem_unk: `fn_800BC618` al 97,92 % (registros guardados); `fn_800BDBC8`: constantes del
  area segura plegadas por MWCC.
- atn/GraphicsDevice `fn_80072BDC` (93,97 %), atn/FileUtil_unk `fn_80069060` (82,56 %) y `fn_80068DB0`
  (93,12 %), atn/Loader `fn_8046E410` (99,10 %), atn/AudioDevice `fn_80046228`/GetLabel (99,3/98,7 %),
  atn/NandManager `fn_800A4244` (98,02 %: orden de la carga de `rect.w` frente a `data()`).
- atn/SoundManager `fn_800CB800`/`fn_800CBE8C`/`fn_800CC518` (99,90 %): la segunda inline (volumen) intercambia
  indice y gestor (r30/r31); una inline libre con el indice primero arregla los registros pero cambia el orden de
  los temporales de `boost::bind` en la pila.
- Bloqueos conocidos: GameSystem `fn_80570CF8` (99,93), Pad `fn_800A58D0`/`fn_800A58F8` (orden de
  registros), Util `fn_800DC12C` (`b` inicial), Task `fn_800D2C50` (registros de la recursion inline).
- Las capas de render (`IRenderLayer`) tienen copias locales con firmas distintas en atn/LayerShadow y
  atn/LayerOcclusionCPU; unificarlas en `include/atn/Layer3D.h` permitiria llamar a `vf10`/`vf14`/`vf18`
  sin cast desde `RenderManager::fn_800BDBC8`.
