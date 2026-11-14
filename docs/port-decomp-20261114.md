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
- 07:25 integrada I (merge limpio; 15396). 07:31 lanzada L (push_back de `std/vector.h`, TacView, Event/Sequence).
- 07:45 integrada G (conflictos de `@N`: version de la integracion + `pdbatch.sh`/`lblmap.py` de GameCamera_unk y FinishCamera_unk; dos floats duplicados de GameCamera_unk a mano: 0x80885974 = @21368, 0x8088597C = @21370). 07:56 lanzada M (`CharaTask::vfBC`).
- 08:05 integrada K (limpio). 08:12 lanzada N (tercera pasada de combate + vistas de colision de BattleTask/BattleCharaTask). 08:15 integrada J (limpio; 15399). 08:19 lanzada O (camaras y MiniMapTask).
- 08:55 integrada L (`std/vector.h` sin cambio: ninguna forma del push_back sin bajadas; re-emparejado de Event/Manager). 09:00 integrada M (`CharaTask::vfBC(atn::ColliBase*)`, 218 unidades renumeradas): conflictos de `@N` resueltos con la integracion + `pdbatch.sh` de 10 unidades + `pairdata99.py` en BattleTask; total 15401, rep100 140, labelcheck 43. 09:10 lanzadas P (eventos de la primera escena) y Q (unidades cerca de enlazar), tope 11:30.
- ~09:55 segundo reinicio del servidor: P y Q se reanudan solos. 10:00 integradas O y N (conflictos de `@N` con M: version de la integracion + `pdbatch.sh` de 117 unidades + `pairdata99.py` en BattleTask + `lblmap.py` en 19 + `align.py` de la cola M en 8; `@enum$6812ControlAppear_cpp` -> `6816`). Total 15418, rep100 141, labelcheck 50. 10:20 lanzada R (MiniMapTask::vf14).
- 10:40 integrada Q: enlazan `db/RandomItemTable_unk`, `game/PlayerInput_unk`, `game/CopyRobotTask`. Q sigue con una segunda lista (dos funciones por debajo del 100 %): sin resultados.
- 11:00 integradas P (`Event/Actor.h` renumero 33 unidades: `pdbatch.sh` + `lblmap.py` de 8) y R (**cambio de splits**: el double u32->float de 0x80757640 pasa de la `.rodata` de `game/MenuCommonTask` a la de `game/MiniMapTask`; lo usa `MiniMapTask::vf14`, MenuCommonTask no; build limpio con `main.dol: OK`). Total 15424, rep100 141, labelcheck 49. 11:03 lanzadas S (HUD) y T (combate), tope 12:15.
- 11:35 integrada S (`GameUI::fn_803E0E10` es estatica: el original no recibe `this`). 11:46 integrada T (`BattleTask::vf14` 85 -> 90 %).

## Resumen

Inicio 02:23, fin ~12:30. 20 colas (A-T) con hasta 4 agentes a la vez (mismo modelo). Dos reinicios del servidor (~06:08 y ~09:55): los agentes se reanudaron; el agente A se quedo parado de 04:09 a 06:08 esperando una aprobacion de herramienta (su trabajo hasta entonces se integro).

- **15389 -> 15424 / 18109**, lost 0 frente a 0f33107, sin bajadas de fuzzy, rep100 139 -> 139, labelcheck 57 -> 49 lineas, build limpio y `main.dol: OK`.
- Funciones al 0 % (report.json): **253 -> 201**. DRAFT: 38 -> 34 (0 en la ruta).
- Enlazan 140 -> **143**: `db/RandomItemTable_unk`, `game/PlayerInput_unk`, `game/CopyRobotTask` (cola Q).
- Funciones de las colas del prompt (A-F): las 25 escritas y equivalentes, sin DRAFT; al 100 % en objdiff solo `fn_8009DE60` (y `fn_8009BF3C`/`fn_800B5994` a 99,9x %: datos, no codigo). Estado y firma de cada una: `claude/port-decomp-20261114.md` (proyecto).
- Al 100 % nuevas por unidad (destacadas): BattleTask ctor/dtor y copias debiles, MiniMapTask (ctor, dtor, vf0C, vf10(), `atn::Camera::SetPosition/SetTarget`), FinishCamera_unk (`fn_8037865C`, `fn_80378D34`), GameCamera_unk (`fn_8037F830`), Model (`fn_80097A9C`), ModelDraw_unk (`fn_8009DE60`, `fn_8009D1A4`, `fn_8009B93C`), Sequence (`fn_8053ED4C`), LastCocoonImago (`vfBC`), AiTarget (`fn_80018078`), BattleCharaTask (`fn_800EBD00`, `fn_800EE360`), Puppeteer_CharaTask (3), chr/ControlBase `vf2C`, BattleTask `fn_80100BF0`/`fn_801061BC`, las tres unidades enlazadas.

## Cambios de splits, game_units y symbols

- `splits.txt` (cola R): el double u32->float de 0x80757640 pasa de la `.rodata` de `game/MenuCommonTask` (acaba en 0x80757640) a la de `game/MiniMapTask` (empieza en 0x80757640). Pruebas: lo usa `MiniMapTask::vf14`; MenuCommonTask ni lo usa ni lo emite y su `@stringBase0` acaba justo ahi; sin el cambio labelcheck empeoraba en las dos. Build limpio `main.dol: OK`.
- `game_units.json`: `matching: true` en RandomItemTable_unk, PlayerInput_unk, CopyRobotTask.
- `symbols.txt`: solo renombrados (firmas nuevas, mangled de vistas unificadas, re-emparejados de `@N`).

## Decisiones tomadas sin el usuario (para revisar)

- Fase 0 punto 7 no hecho en la fase 0 (no cabia en el tope); lo hicieron las colas G y N.
- `IRenderLayer::vf10(void*)` en `atn/Layer3D.h` (necesario para `LayerShadow.h`): renombra cinco `vf10` de capas.
- rep100: `fn_800B5994` (datos en `.sbss`/`.sdata`) aceptado; el resto de las subidas temporales de rep100 se arreglaron al integrar.
- Funciones con bajadas aceptadas dentro de una cola por la firma real (GameUI `fn_803DBD44` 97,87 -> 97,62 y recuperada a 97,89).
- `GameUI::fn_803E0E10` estatica (el original no recibe `this`).
- `std/vector.h` sin cambios (cola L): el `push_back` del original no sale sin bajadas.
- poolcheck: un aviso nuevo en `game/BattleTask` (float 2.44346 sin pareja), el resto igual que la base.
