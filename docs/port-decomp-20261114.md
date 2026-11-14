# port-decomp-20261114

Inicio: 2026-11-14 02:23 (Europe/Madrid). Funciones al 0 % al empezar (report.json de 0f33107): 253. Partida: 15389 / 18109, rep100 139 (ref mkref), labelcheck 57 lineas.

Sesion de 10 h centrada en el port (claude/prompt-10h-port.md). Permisos confirmados por el usuario al empezar: push de la rama y merge en main si el cierre es limpio; hasta 4 agentes; repos tls-build y port (fase3-20261113, solo lectura en /home/claude/port); LayerOcclusionCPU sin asm GQR6 (C equivalente NONMATCHING).

## Fase 0

Hecho en 30 min (02:24-02:55), un solo re-emparejado (`pdbatch.sh` de 290 unidades + `lblmap.py` en 34 + `pdbatch.sh` de 10). Resultado: 15389 / 18109, lost 0, sin bajadas de fuzzy, labelcheck 57 -> 45 lineas, rep100 139 -> 143 (ver abajo). Commits por punto + uno del re-emparejado (a1e3f0c).

1. **Hecho.** `IRender::vf14(const Matrix3x4*, const Vector3*, const Frustum_unk*, AABB_unk*)` en `include/atn/IRender.h` (declaraciones adelantadas, sin includes nuevos), `ModelBase` (`atn/Model_unk.h`) y `ModelInstance` (`atn/ModelInstance.cpp`). `atn/PostFx.h` ya no tiene su `IRender`: incluye `atn/IRender.h` (`atn/LensFlare.h` ya lo incluia). Quedan copias locales en `atn/VideoTHP.cpp`, `Rain.cpp`, `LinearAllocator.cpp`, `TextureComposite.cpp`, `CompositeIntensityTintMulti.cpp` (no las usa el port). Renombrados: `vf14__Q23atn7IRender...`, `vf14__Q23atn13ModelInstance...`.
2. **Hecho.** `ModelBase::fn_8008D78C(RenderManager*)`; `vf18` le pasa la vista y lee `view->mCamera.mView.m` (vf18 sigue al 100 %).
3. **Hecho.** `include/atn/LayerShadow.h` (deriva del `Layer3D` de `atn/Layer3D.h`). Para que su `vf10` sobrescriba, `IRenderLayer::vf10` pasa a `vf10(void*)` (la lista de dibujo; `RenderManager::fn_800BDBC8` ya la pasaba): `Layer2D`, `Layer3D`, `LayerPrerender`, `LayerPostFx` renombrados (`...FPv`). `RenderSystem_unk.cpp` usa la cabecera; `GraphicsDevice.cpp` (`RenderTextures_unk` eliminado: `mTex`, `mDepthTex`, `mDepthMtx`, `mTexMtx`) y `ModelDraw_unk.cpp` (`DrawPass_unk::m7C` -> `LayerShadow::mCamera.mPos`) usan sus miembros. `LayerOcclusionCPU.cpp` conserva su copia local de `IRenderLayer` (su `vf10` toma un `int`): pendiente.
4. **Hecho.** `TexProjector_unk` eliminado: `fn_80073194`, `fn_80074EC8`, `lbl_8087EEE8/EC` usan `CharaLightData_unk` (`m00` mezcla, `m04` centro, `m1C` angulo, `mPos` desplazamiento, `mTextures[m44]`). `GraphicsDevice.h` solo lo declara adelantado.
5. **Hecho.** `GXStateBits_unk` en `include/atn/GXStateBits_unk.h`; `Material_unk` tiene una union anonima con los campos `m14_*` (los usan MaterialParser/Shader/Model) y `atn::GXStateBits_unk mState`; `ModelDraw_unk.cpp` llama `fn_80076A28(rs, &mat->mState)` en los dos sitios.
6. **Hecho.** `include/atn/CompositeCache_unk.h` (`CompositeLayerDesc_unk` con `atn::Color`, la clave y `CompositeCacheEntry_unk`); `CompositeCacheHandle_unk::fn_800D8808` devuelve `&mEntry->mTexture`.
7. **No hecho (no cabe en el tope).** Las 43 vistas `CollResult_unk` tienen el ctor fuera de linea `__ct__14CollResult_unkFv` (0x80140500, una copia debil que en el original seria `__ct__Q23atn15ColliResult_unkFv`) y funciones con `CollResult_unk*` en la firma llamadas entre unidades (`ColliAttrManager::fn_80184864`, `Bullet::fn_80110858`, `gmk::Lemon::fn_80416268`...): cambiar las 6 del port obliga a cambiar los nombres mangled y todos los llamadores a la vez. Queda como cola propia (G) si sobran agentes.

Re-emparejado: `pairdata` llamo `__vt__...`/`sInstance__9SetConfig` a cadenas y floats (0x80783CA8, 0x8078CB00/08/10, 0x80885CB0): devueltos a mano. `@enum$6816ControlAppear_cpp` -> `@enum$6811` (dos funciones de boost::function de ControlAppear). rep100 +4 (Em204BigMulue, Em312DarkServant, GameCamera_unk x2, GameEventScript, NewCampTask x2; -SwingObject x3): floats de `.sdata2` con dos etiquetas del mismo nombre en la unidad; pendiente de arreglar a mano.

## Colas y agentes

- 02:59: lanzados A (HUD), B (combate), C (camaras), D (modelos) desde a1e3f0c.
- 04:25 integrada B (merge limpio). 04:30 integrada C: conflictos de `symbols.txt` solo de etiquetas `@N` (C renumero -10 las 148 unidades que incluyen `GameCamera.h`; B re-emparejo BattleTask): version de la integracion + `pdbatch.sh` de BattleTask/AiTarget/AiCommon_unk + `pairdata99.py` de la cola B en BattleTask (rep100 183 -> 138). 04:38 integrada D (merge limpio). Total 15389 -> 15392, lost 0, sin bajadas, rep100 143 -> 138, labelcheck 45 -> 44 (ff62b19, push).
- 04:42: lanzados E (carga/dibujo de modelos), F (oclusion, LH_Motion_cont) y H (segunda pasada de combate: BattleCharaTask fn_800E4278 y BattleTask) desde ff62b19.
- 05:45 integrada A sin aviso del agente (paro a las 04:09 esperando una aprobacion de herramienta que nadie podia dar; se perdio en el reinicio del servidor de las ~06:08): 7 funciones escritas, re-emparejado de GameUI/PopupTask/TacView + lblmap. 05:51 lanzada I (segunda pasada HUD y camaras).
- 05:58 integrada E (merge limpio). 06:06 lanzada G (CollResult_unk, sin BattleTask).
- ~06:08 reinicio del servidor: I y G se reanudan solos; F reanudada a mano (tope 07:30); H habia terminado (informe perdido; notas en qH).
- 06:12 integrada H: total 15394; rep100 +1 (`BattleCharaTask::fn_800EBD00` 99,95 %, etiqueta float sin pareja tras re-emparejar). 06:18 lanzada J (segunda pasada de modelos).
- 06:22 integrada F: total 15395; rep100 +1 aceptado: `fn_800B5994` codigo identico, sus cuatro limites a cero (`lbl_8087D898..A4`) los pone MWCC en `.sbss` y el original en `.sdata` (como `fn_800B6218`, que ya estaba). 06:26 lanzada K (restos al 0 % de AiTarget, BattleTask, GameTask, MiniMapTask).
