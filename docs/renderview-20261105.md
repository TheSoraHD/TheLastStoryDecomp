# renderview-20261105: vistas de `lbl_8087EFB4` -> `atn::RenderManager` (antes `atn::RenderView_unk`)

**Nombre real:** el volcado de simbolos del RSO USA (`docs/lastworld_tools_symbols.txt` de
Aelbannan/last-story-decomp, tabla de importaciones) da `atn::RenderManager` con `instance_`
(lbl_8087EFB4), `AddRender(IRender*, int, float)` (0x800BDB58), `AddResolveTexture(Texture*, int)`
(0x800C1660), `GetScreenPosition(const Vector3&) const` (0x800BFAC8), y en su dwarf `Order`,
`GetScene`, `GetKeyLight`, `FrameCallbackEntry` y `static_vector<ResolveCommand, 8>`. La clase se ha
renombrado (`include/atn/RenderManager.h`) y `lbl_8087EFB4` es ahora `atn::RenderManager::instance_`.

Rama `renderview-20261105` desde `main` b07abc3 (12679 / 18119 en `report.json`). Referencias en
`/home/claude/ref` (report.json, labelcheck 204 lineas, rep100 145).

## Inventario (inicio)

99 ficheros con `lbl_8087EFB4`: 22 ya con `RenderView_unk.h`; 65 vistas propias:

- **Solo la camara** (0x104..0x2F8): posicion (0x10C: `mEye`/`mPos`/`m10C`), objetivo (0x118:
  `mTarget`/`mPos` de MapPartsModel y CrowdManager), `m104` como `Camera_unk`, vista 0x15C, proyeccion
  0x18C, frustum 0x204 (`GetFrustum_unk`, `mFrustum`, `m204`), `m134` (= `mRot.y`). Unidades: gmk (12),
  BattleCharaTask, ChaseManager, CopyRobotTask, CostumeEditChara, CrowdManager, Em306/309/401/403,
  QuadrupedTask, EnemyQuarkStand, FinishCamera_unk, GameOverTask, GimmickCommon, MapPartsModel,
  NewCampPartsBase, ActionActorPosition, CharaTask, AutomaticDoor, D3Anime*, AABBTree, atn_colli,
  Geometry_unk, LayerOcclusionCPU, MapPartsLocatorSet.
- **Resto del sistema de render** (atn): GraphicsDevice, LayerShadow, ModelDraw_unk, ModelInstance,
  EffectDesc_unk, Layer3D, Layer2DBlur, TextureComposite, VideoTHP, PostFx* (`PostFxFrameBuffer_unk`),
  LensFlare, Rain, game/main (`RenderSystem_unk`), MapManager (`SetMapHit_unk`), TownTask.
- **Metodos `extern "C"` con `self`** de atn/RenderSystem_unk.cpp (0x800BC468-0x800C16BC): unos 25;
  los mas llamados fn_800BDB58 (57 llamadas, 12 ficheros), fn_800BFAC8 (51, 22), fn_800C16B4 (35, 8),
  fn_800BFB70 (26, 11).
- Desplazamientos cruzados sin conflicto real: 0x00C (`RenderTextures_unk*` de GraphicsDevice) y 0x048
  (`Drawer_unk*` de VideoTHP) son capas de `mLayers[20]` (3: LayerShadow, 18: Layer2D "2D Object");
  `m054` de Model es `mPassHeads[1]`; 0x118 es `mTarget` aunque dos vistas lo llamen `mPos`.

## Disposicion de RenderView_unk

Del constructor 0x800BC874 (`include/atn/RenderView_unk.h`, comprobada con `offsetof`; 0x54968 bytes):
`mLayers[20]` 0x000, `mPassHeads[20]` 0x050, `mPassCounts[20]` 0x0A0, cola 0x0F0-0x0F8, `m0FC`/`m100`,
`mCamera` (atn::Camera) 0x104, `m2F8` pasada, `mScene` 0x2FC (`NewCampScene2_unk*`), `mMapHit`
(boost::function, 0x14) 0x300 y `mMapHitArg` 0x314, `mShadowCamera` 0x318, `GXTexObj` 0x50C/0x530,
`m558` (tercera camara), `Texture_unk m74C`, `GXTexObj m77C`, `m79C`, `Matrix3x4 m7A0[5]`
(fn_800C0900), niebla 0x890-0x8AC, `RenderCallback_unk` x4 0x8B0-0x8F8, copias pendientes 0x910/0x914[8],
`m954`, `mKeys` 0x958 y `mCmds` 0x895C (`DrawQueue_unk`, `include/atn/DrawCmd_unk.h`), `m54960`,
`m54964`.

## Resultado

- Tanda 1 (29 unidades de solo camara): sin bajadas tras `pdbatch.sh`; labelcheck 204 -> 194.
- Tanda 2 (metodos): 25 funciones `extern "C"` de atn/RenderSystem_unk.cpp pasadas a miembros (y el
  constructor 0x800BC874 como constructor real), mas `SetMapHit_unk` (MapManager) y los setters
  0x80479EA0/0x8047A158 (game/main); 57 unidades llamadoras cambiadas. Sin bajadas en objdiff; suben al
  100%: GimmickCommon x7 (enlazada), RoomLight vf10, LayerPostFx y LayerPost3D (constructores).
  Mejoran: CharaTask fn_80144F4C 49 -> 98.8 (camara ortografica con `atn::Camera` real),
  AutomaticDoor fn_803F53E0 66 -> 91, PostFxBloom vf10 86 -> 91, RenderSystem ctor 77 -> 81,
  fn_800BFB70 82 -> 90, fn_800BFC18 95 -> 99.5. labelcheck 204 -> 177, rep100 145 -> 145.

- Funciones de RenderManager (atn/RenderSystem_unk.cpp) con los miembros reales: `AddRender` (asignaciones
  sobre una reserva del buffer de cola `AllocQueue_unk`, entrada `RenderEntry_unk` de 0x10),
  `fn_800C0590`, `fn_800C122C`, `fn_800C1424`, `fn_800BD9C0` al 100%: RenderSystem_unk 15 -> 20/37; fn_800BC468
  81 -> 88 (rampa I8 en mosaicos 8x4 con un clamp que lee dos veces cada limite).
- `atn::Sphere` sin constructor de copia propio (copia implicita): sube fn_800C122C a 100, y
  MapManager fn_80491D50/fn_80491DF8 71 -> 78, DrawEntry_unk ctor 88 -> 90; sin bajadas en `report.json`
  (307 unidades la incluyen). El `@enum$` de ControlAppear pasa a 6741.

## Cambios de splits

Ninguno.

## Decisiones tomadas sin el usuario

- **`atn::RenderView_unk` renombrada a `atn::RenderManager`** (nombre del RSO) y `lbl_8087EFB4` a
  `atn::RenderManager::instance_` (`instance___Q23atn13RenderManager` en symbols.txt). El prompt
  hablaba de `RenderView_unk`; se ha preferido el nombre real. Revertirlo es un `sed` en los fuentes y en
  symbols.txt (`Q23atn13RenderManager` -> `Q23atn14RenderView_unk`).
- Metodos con nombre real: `AddRender` (los llamadores pasan ahora `(render, pass, order)`, el orden de la
  firma del RSO; sin cambios de codigo), `AddResolveTexture`, `FrameCallbackEntry` (antes
  `RenderCallback_unk`), `mResolveCount`/`mResolve[8]` (`ResolveCommand`) y `GetScreenPosition` (miembro `const` que devuelve
  `Vector3` por valor: los 51 llamadores declaran ahora el resultado en la llamada; en GimmickCommon la
  distancia a la camara pasa a ser un temporal para que `screen` ocupe el hueco del original, y en
  GameUI `a = GetScreenPosition(a)`). `GetScene`, `GetKeyLight` y `Order` no se han asignado: no hay
  pruebas de a que funcion corresponde cada una.
- 0x80116BD4 se llamaba `__as__Q23atn6CameraFRCQ23atn6Camera`, pero escribe en 0x10C(r3): es
  `RenderManager::SetCamera_unk(const Camera&)` (copia debil en Event/Path; Path, TutorialTask y
  GameCamera_unk la llaman sobre `fn_8008B964()`). Renombrada.
- 0x800F7258 (`addi r3, r3, 0x104`) es un accesor de la camara: `RenderManager::fn_800F7258()` (copia
  debil en game/BattleTask). PlayerInput_unk y ControlMoveCursor lo definen inline (y `fn_8008B964`)
  porque el original los llama fuera de linea y gastan presupuesto de inlining: sin ellos
  `Input_unk::fn_80124BE4` se inlineaba y desaparecia.

- `Plane_unk` estaba definida en `atn/Camera_unk.h` y en `gmk/WaterLevel.h`: movida a
  `include/atn/Plane_unk.h` (las dos la incluyen).
- `RenderView_unk.h` incluye `std/string.h` (fn_800BD9C0 recibe un `std::string`), `boost/function.hpp`,
  `atn/Texture_unk.h`, `atn/Color.h` y `atn/DrawCmd_unk.h`.
- `DrawCmd_unk` / `DrawQueue_unk::push_back`: declarados en la cabecera, definidos en
  `atn/ModelDraw_unk.cpp` (su unico usuario); el constructor por defecto, en RenderSystem_unk.
- `mScene` y los metodos que devuelven la escena usan `NewCampScene2_unk*`: es `atn::Scene` (MapArea la
  tiene como base en 0x4C y el callback `MapHitFunc_unk` devuelve `atn::Scene*`), pero renombrarla es
  la propuesta 5 y se deja para despues; las vistas locales de la escena (`DrawLens_unk`,
  `LightTarget_unk`...) siguen en su unidad.

## Hallazgos

- Con un miembro que devuelve un struct por valor, MWCC construye el resultado en la variable solo si se
  inicializa en la declaracion (`Vector3 s = f()`); `s = f()` con `s` declarada antes saca un temporal y
  una copia. El hueco de pila sigue el orden en que se crean las variables: un temporal (`&(a - b)`) en
  vez de una variable con nombre deja el hueco a la siguiente (GimmickCommon fn_803ED774/fn_803EDCF4).
- pairdata no resuelve dos floats iguales en `.sdata2` de una unidad cuando la renumeracion las cruza
  (GameUI 0x80885DE4/0x80885E68): sale un `@N` repetido. Se arregla sumando a mano el desfase de la
  renumeracion a todas las etiquetas del rango.

- fn_800BFAC8 (proyeccion a pantalla) no es un miembro que devuelve `Vector3` por valor: compila igual
  (NRVO), pero los llamadores declaran el resultado antes y pasan su direccion; con
  `screen = view->fn(...)` sale un temporal y una copia. Se deja libre con la vista como 2o parametro.
- fn_800C0900 si devuelve `Matrix3x4` por valor (ModelDraw recibe el resultado en su variable).
- La esfera que recibe fn_800C122C por valor se copia en el llamador con copia miembro a miembro
  (centro con psq y luego radio), no con el constructor de copia de `atn::Sphere` (z primero): en
  ModelInstance se construia con una inline local; al final se ha quitado el constructor de copia de
  `atn::Sphere` (la copia implicita da exactamente eso y nada baja; MapManager fn_80491D50 sube).
- `atn::Color` con constructor de copia por miembros da el mismo codigo que las copias con temporales
  de las vistas `RainColor_unk`/`MotionBlurColor_unk`: se han sustituido por `atn::Color`.
- `fn_800C0A8C`/`fn_800C0A98` son las texturas de `mLayers[4]`/`[5]` (`LayerReflect::mTexture`, 0x5C).

## Pendientes

- `atn::Camera` tiene en el RSO `__ct__Q23atn6CameraFQ33atn6Camera8CAM_TYPEQ33atn6Camera14CAM_PROJECTION`
  y metodos `SetPosition`, `GetTarget`, `CalcMatrix`... ; `atn::Frustum` es el nombre real de
  `Frustum_unk`; `atn::Scene` el de `NewCampScene2_unk` (importa `__ct__Q23atn5SceneFv`).

- Vistas de la escena (`NewCampScene2_unk`, que es `atn::Scene`) que siguen en sus unidades y se
  convierten con un cast marcado `// pending: view of the scene`: `RainOverride_unk`,
  `GodRayOverride_unk`, `ColorMatrixOwner_unk` (PostFx), `ShadowLight_unk` (LayerShadow),
  `ScreenInfo_unk` (CharaTask, ColliAttrManager, WaterLevel, AutomaticDoor), `Lens_unk` (CharaTask),
  `InstLens_unk` (ModelInstance), `DrawLens_unk` (ModelDraw_unk). Unificarlas es la propuesta 5.
- `DrawPass_unk` de ModelDraw_unk es una vista de las capas (`mLayers`, LayerShadow en 3): cast.
- Copias locales de `atn::IRender` (PostFx.h, TextureComposite, VideoTHP, Rain...) con `vf10(void*)`:
  `RenderView_unk.h` ya no incluye `atn/IRender.h` (solo lo declara) para no chocar con ellas.
- Funciones del frustum (`fn_8004FF58`, `fn_800502A8`...) son `extern "C"` con `Frustum_unk*`: los
  llamadores que las declaraban con `void*` ahora hacen cast.
