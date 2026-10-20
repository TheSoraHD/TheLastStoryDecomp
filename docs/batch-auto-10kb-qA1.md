# batch-auto-10kb qA1: atn (camara, frustum, PostFx, luces)

Cola: `atn/Camera_unk`, `atn/Frustum_unk`, `atn/PostFxGamma`, `atn/PostFxGodRay`, `atn/CharaLightData_unk`.

## atn/Camera_unk (commit a5d920c)
- Resultado: 2/4 al 100% (ctor, dtor), `matching: false`, `main.dol: OK`.
- NONMATCHING: `Camera_unk::fn_8004B378` 98.28% (orden en pila de los temporales de rot_z*rot_x*rot_y y de
  algunos temporales pequenos; FPR del caso con roll); `Camera_unk::fn_8004BF64` 99.94% (r3/r4 intercambiados en
  las dos copias de vector tras las rotaciones).
- **Decisiones sin el usuario**:
  - Splits sin cambios.
  - `symbols.txt`: `fn_8004BF64` -> `fn_8004BF64__10Camera_unkFff` (metodo `bool Camera_unk::fn_8004BF64(float, float)`,
    camara libre de depuracion movida con el pad; la llaman game/NewCampTask y game/NewTitleTask, en asm).
  - Cabecera nueva `include/atn/Camera_unk.h`: `Camera_unk` (0x1F4: tipo, proyeccion, pos/target/up/angulos, roll,
    limites de proyeccion, mView 0x58, mProj 0x88, near/far, mInvView 0xD0) y `Frustum_unk` (0xF4 en +0x100; sus
    funciones se quedan `extern "C"`, ver atn/Frustum_unk). Los `.cpp` que ya declaraban su propio `Camera_unk` local
    no se han tocado.
  - Cabecera compartida `include/math/Vec.h`: `Matrix44_unk::operator=` (copia de 8 paired singles). regress: lost 0.
  - Helpers inline inventados en el .cpp (LookAt_unk, Perspective_unk, Ortho_unk, Rotate_unk, RotZYX_unk, RotZXY_unk,
    SetView_unk, MultVec_unk, Direction_unk, Neg_unk, Cross_unk, CosF_unk/SinF_unk, RadToDeg_unk).
  - Nombre real propuesto del fichero: `atn_camera.cpp`.
- Hallazgos MWCC:
  - `A*B + C*D` se emite como `fmuls C*D` + `fmadds A,B`: el primer producto del fuente es el fmadds.
  - Pasar un `Vector3(...)` temporal construido con `-f()` (float devuelto) mete `frsp`; con locales `float` no.
  - Una matriz devuelta por valor por una inline y asignada (`m = F()`) va a la zona de locales de inline; con
    `T x = F()` (NRVO) va con los locales con nombre: sirve para ordenar matrices grandes en la pila.
  - Proyeccion escrita como `inline void F(Matrix44& out, ...) { Matrix44 m; ...; out = m; }` deja sus 0x40 al final de
    la zona de inline (como el original), devolviendola por valor no.
  - `Direction_unk(a, b)` inline con la resta dentro baja el temporal de la resta a la zona de inline.
- Pendientes para otras unidades: `lbl_8087EFB4` (camara global) es otra clase que contiene un `Camera_unk` en +0x104
  (GeoCamera_unk de atn/Geometry_unk: vista 0x15C = 0x104+0x58, proyeccion 0x18C = 0x104+0x88).

## atn/Frustum_unk (commit 013c918)
- Resultado: 7/7 al 100%, `matching: true`, `main.dol: OK`.
- NONMATCHING: ninguna.
- **Decisiones sin el usuario**:
  - Splits sin cambios (el split cubria justo el fichero: .text 0x8004F45C-0x80050900, .sdata2 0x80880930-0x80880940).
  - Simbolos sin renombrar: las 7 funciones quedan como `extern "C"` (`fn_8004F45C` bounds de n puntos,
    `fn_8004F50C`/`fn_8004F5B8` construccion, `fn_8004FF58` esfera, `fn_800500FC`/`fn_800502A8` caja, `fn_80050420`
    recorte de segmento) porque ya las llaman como `extern "C"` gmk/Brazier, atn/LensFlare, atn/ModelInstance,
    atn/D3Anime*, atn/AABBTree, atn/Rain y atn/LayerOcclusionCPU (seguramente eran metodos de una clase frustum).
  - Tipos inventados en `include/atn/Camera_unk.h`: `Frustum_unk` (vista 0x00, ortho 0x30, -near 0x34, -far 0x38,
    4 planos laterales en espacio vista 0x3C, bounds 0x7C/0x88, 6 planos en mundo 0x94; normales hacia fuera) y
    `Plane_unk` (normal, distancia). En el .cpp: `Segment_unk` y helpers inline (SetPlane_unk, ClipPlane_unk...).
  - Nombre real propuesto del fichero: quiza `atn_frustum.cpp` o parte de `atn_colli*` (esta entre ColliBaseFunc y
    ColliTest).
- Hallazgos MWCC:
  - `float t = TanF(x); t *= n;` da `frsp` + `fmuls t, n`; `TanF(x) * n` invierte los operandos.
  - Un plano con temporales `b - a`, `c - a` en orden ascendente en la pila = locales con nombre dentro de la inline
    (`Vector3 e1 = b - a; ...`); como argumentos directos salen en orden inverso.
  - Bucle sobre un array local con el puntero ya en registro: `Vector3* p = pts; for (...; p++)` evita el `mr` extra.
  - Dos resultados en ramas distintas de una inline que el original deja juntos en la pila (antes de los
    temporales de cada rama) = locales con nombre `Vector3 h = ...;` en cada rama.
  - `Lerp_unk(a, b, t)` de Vec.h no da el orden de temporales de `a + (b - a) * t` escrito en linea.
- Pendientes para otras unidades: ninguno.

## atn/PostFxGamma (commit 8b169ad)
- Resultado: 3/6 al 100% (`fn_800ACFC8`, ctor, `fn_800AD648`), `matching: false` (como el resto de PostFx: el .o
  emite copias debiles de `~IPostFx`/`~IRender`/`~PostFxGamma` que en el original estan en atn/LinearAllocator),
  `main.dol: OK`.
- NONMATCHING: `fn_800AD340` 81% (el original guarda el flag de cambio en un solo registro y tiene una variable de
  induccion muerta `+3`); `fn_800AD484` 99.19% (r30/r31 intercambiados); `vf10` 97.09% (registros de los log2
  inline y del bucle de reduccion; el original no reutiliza `cw * 2` entre viewport y copia).
- **Decisiones sin el usuario**:
  - Splits sin cambios.
  - `symbols.txt`: `fn_800AD484`/`fn_800AD648` -> metodos de `atn::PostFxGamma`; `fn_800AD834` ->
    `vf10__Q23atn11PostFxGammaFPv`; `lbl_80732C4C` -> `@stringBase0` (size 0x27); `lbl_8087D850`/`lbl_8087D854` ->
    `gGammaRampMin_unk`/`gGammaRampMax_unk` (floats escribibles de .sdata, limites de la rampa; nombres inventados);
    `fn_800BC2E0` (unidad ajena atn/RenderSetting_unk, en asm) -> `__ct__15GammaParams_unkFv` (constructor de los
    parametros del efecto, 0x34 bytes en lbl_8087EFA8+0x3DC; nombre de clase inventado).
  - Tipos inventados en el .cpp: `GammaParams_unk`, `GammaRenderParams_unk`, `GammaFrameBuffer_unk`, `GammaColor_unk`;
    clase `atn::PostFxGamma` (0x350: parametros 0x04, 8 GXTexObj 0x38, Texture_unk media 0x138, contadores
    0x168/0x16C, 8 Texture_unk de luminancia 0x170, LUT 0x2F0, rampa 0x320). `fn_800ACFC8` (normaliza una matriz
    indirecta 2x3) queda `extern "C"`.
- Hallazgos MWCC:
  - Macros MIN/MAX del original: llamadas duplicadas (`pow` dos veces, 4 expansiones de log2) y el orden de la
    comparacion (`a > b ? a : b` en float, `MAX(1, x)` en int) se ven en el asm.
  - Tablas locales `const int b[] = {...}` de una inline: se copian a la pila en cada expansion (lwz/stw desde .rodata).
  - Quitar una sentencia (`*p++` -> `data[y * 64 + x]`) puede hacer que una funcion pase a autoinlinearse en sus
    dos llamadoras: comprobar el `.o` tras cada cambio.
  - Un color con constructor de 4 floats deja los 4 `stfs` y un solo `fctiwz`; con inicializador `{...}` copia
    desde .rodata.
- Pendientes para otras unidades: ninguno.

## atn/PostFxGodRay (commit 1169ddf)
- Resultado: 2/6 al 100% (ctor, `fn_800AEF4C`), `matching: false` (mismo motivo que PostFxGamma: dtors debiles de
  IPostFx/IRender/PostFxGodRay), `main.dol: OK`.
- NONMATCHING: `fn_800AE568` 98.75% (registros y hueco en pila del temporal del color K); `fn_800AECD4` 92.5%
  (registro del ancho y momento de la conversion u16->int antes de fn_800C0D4C); `fn_800AEF68` 97.59% (huecos en
  pila de los temporales del caso 0 y FPR del tamano de pantalla); `vf10` 99.84% (FPR del lerp del color).
- **Decisiones sin el usuario**:
  - Splits sin cambios.
  - `symbols.txt`: `fn_800AF544` -> `vf10__Q23atn12PostFxGodRayFPv`; `lbl_80732C9C` -> `@stringBase0` (size 0x23);
    `lbl_8087D858..6C` -> `gGodRayRectMinX_unk`, `gGodRayRectMinY_unk`, `gGodRayRectMaxX_unk`, `gGodRayRectMaxY_unk`,
    `gGodRayIndScale_unk`, `gGodRayIndExp_unk` (globales escribibles de .sdata; nombres inventados).
    `fn_800AE568` (pasada de blur radial) y `fn_800AECD4` (limpia la rampa) quedan `extern "C"`. Los simbolos del
    static local `sRadial` (`lbl_807C7470`, registro de destructor `lbl_807C7460`, guarda `lbl_8087EF98`) no se han
    renombrado.
  - Tipos inventados en el .cpp: `GodRayParams_unk` (0x48 en lbl_8087EFA8+0x264 o en la escena +0x198),
    `GodRayRenderParams_unk`, `GodRayOverride_unk`, `GodRayFrameBuffer_unk` (ojo 0x10C, objetivo 0x118, vista 0x15C),
    `GodRayColor_unk`; clase `atn::PostFxGodRay` (0x78). Uso de `GimmickUvOwner_unk::fn_800D59B8` con cast, como
    gmk/AutomaticDoor (el simbolo ya tenia ese nombre).
- Hallazgos MWCC:
  - Registros de GPR en funciones grandes: declarar las variables sin inicializar al principio (`int fullW; ...`) y
    asignarlas despues ordena los registros salvados como en el original sin mover el codigo.
  - Constantes float en locales con nombre (`float rate = k;`) fijan el orden de operandos de `fmadds`
    (`d * rate + x`) y cargan la constante una sola vez.
  - Inicializar en la lista del ctor un miembro struct con constructor de 4 floats da los 4 `stfs` antes del ctor
    del siguiente miembro (Texture_unk), como el original.
- Pendientes para otras unidades: ninguno.

## atn/CharaLightData_unk (commit 36e6ed6)
- Resultado: 14/23 al 100%, `matching: false`, `main.dol: OK`. El fichero real es la escena del campamento/mapa
  (`NewCampScene2_unk`: LH_Scene + luces/camaras extra + dos `CharaLightData_unk` + plano de reflexion), con los
  metodos de `CharaLightData_unk` delante.
- NONMATCHING: ctor `NewCampScene2_unk` 67.54% (orden en pila de temporales y FPR); `fn_800C23E8`/`fn_800C2418`
  99.17% y `fn_800C2D88` 99.76% (orden de operandos del `add` final); `fn_800C258C` 99.4% (FPR del 0.5);
  `fn_800C2634` 85.55% y `fn_800C289C` 85.85% (el original indexa las texturas con `mulli` en el bucle desenrollado);
  `fn_800C2C20` 67.43% y `fn_800C2EE0` 71.86% (FPR/orden de la matriz de reflexion).
- **Decisiones sin el usuario**:
  - Split ampliado: `.sdata 0x8087D918-0x8087D920` (antes hueco `auto_09_8087D918_sdata`): son los dos pools `""`
    del `new (heap, "", "", 0) Texture_unk[n]` inline, que tambien usan NewCampTask/NewCampPartsGmk (el enlazador
    se queda la primera copia).
  - `symbols.txt`: `fn_800C1A1C` -> `__ct__17NewCampScene2_unkFv`; `lbl_807337B0` -> `@stringBase0` (size 0x3F);
    `lbl_8087D918`/`lbl_8087D91C` -> `@STRING@ResizeTextures_unk__FR16TextureArray_unkii@0` / `...unkii` (nombre de
    la inline inventada `ResizeTextures_unk`). Resto de funciones `extern "C"` (fn_800C18BC, fn_800C2448,
    fn_800C24B4 y fn_800C2C20 ya se llaman asi desde gmk/RoomLight, game/TownTask, atn/LensFlare, gmk/AutomaticDoor...).
  - Cabecera compartida `include/gmk/CharaLight.h`: `TextureArray_unk` con ctor `(0, 0)`; `CharaLightData_unk`:
    `m30[0xC]` -> `float m30, m34, m38`, `m44[0xC]` -> `int m44` (textura actual), `m48` (frames por textura), `m4C`
    (contador). regress: lost 0.
  - Tipos inventados en el .cpp: `NewCampScene2_unk` (0x2B4, nombre ya usado por el simbolo del dtor), `SceneColor_unk`,
    `SceneFog_unk`, `ScenePlane_unk`, `SceneRow_unk`, `SceneLightData_unk`; helpers inline `ResizeTextures_unk`,
    `SetWrap_unk`, `SetLight_unk`, `EnableLight_unk`, `GetLight_unk`, `NumLights_unk`, `Reflect_unk`,
    `SetIdentityRows_unk`, `Mul2_unk`/`Sub2_unk`, `WrapDeg_unk`. La clase `NewCampScene2_unk` de
    game/NewCampPartsGmk.cpp tiene el mismo layout (`SceneRec50_unk` = `CharaLightData_unk`).
  - Nombre real propuesto: fichero de la escena (quiza `atn_scene*.cpp`); `CharaLightData_unk` podria ser otra clase
    del mismo fichero.
- Hallazgos MWCC:
  - `busy = busy || f();` con `int busy` da el patron `cmpwi; li 0; bne; bl; beq; li 1` del original.
  - Temporales de argumentos de una inline: se reservan en el orden de los parametros (izquierda a derecha) aunque se
    evaluen de derecha a izquierda; para invertir su orden en la pila, invertir el orden de los parametros.
  - `i -= f(); return &v[i];` o `int j = i - f(); return &v[j];` cambian que registro recibe la resta (r0 frente al
    del parametro) sin tocar el resto.
  - Las stores enteras simples (`mOn = 1`) se programan tarde: escribirlas antes de las copias de vectores da el
    orden del original.
  - Un miembro asignado en el cuerpo (`m = v.GetNormalized_unk();`) deja temporal + copia psq; en la lista de
    inicializacion se normaliza en sitio.
- Pendientes para otras unidades: game/NewCampPartsGmk.cpp y game/NewCampTask/RenderSystem_unk podrian usar la
  `NewCampScene2_unk` de este .cpp (falta cabecera comun); los `.sdata` 0x8087D918 ya estan en esta unidad.
