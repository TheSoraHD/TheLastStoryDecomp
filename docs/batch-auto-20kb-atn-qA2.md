# batch-auto-20kb-atn qA2: atn (sombras y ajustes de render)

Cola: `atn/LayerShadow`, `atn/RenderSetting_unk`.

## atn/LayerShadow (commit c36055a, notas y nombres en el commit final)
- Resultado: 2/7 al 100% (ctor 0x800B6BEC y dtor 0x800B955C), `matching: false`, `main.dol: OK`.
- NONMATCHING:
  - `fn_800B6C70` (caja ajustada a una rejilla) 99.88%: las dos ultimas conversiones `(int)step` usan huecos de
    pila nuevos en vez de reutilizar 0x38/0x30.
  - `fn_800B7090` (camara ortografica de la luz que cubre la caja) 82.01%: el original tiene un GPR salvado mas
    y 0x10 mas de pila (los temporales de las esquinas empiezan en 0x30).
  - `vf10` 22.90%: supera el presupuesto de inline (tres copias de `Camera_unk`, copia del frustum, helpers de
    `Vector3`/`AABB_unk`): aqui salen como copias debiles y el original lo inlinea todo. Probado: escribir a mano
    lo propio (setters, normalizar, esquinas) no basta; con solo una copia de camara menos ya entra. Falta saber
    que construccion del original es mas barata (posible helper inline que agrupe parte del cuerpo).
  - `vf1C` 86.69% y `vf20` 93.95%: registros de la copia del frustum dentro de la copia de camara (punteros base
    y de bucle intercambiados: r4/r5 en el original, r6/r7 aqui); probado `operator=` explicito en
    `Frustum_unk`/`Plane_unk` sin cambio (o peor, por presupuesto).
- **Decisiones sin el usuario**
  - Splits: sin cambios.
  - `symbols.txt`: renombrados ctor/dtor, `vf10/vf1C/vf20__Q23atn11LayerShadow`, `fn_800B6C70__...` y
    `fn_800B7090__...` (miembros), `lbl_80732EF4` -> `@stringBase0` ("Shadow", local).
  - Copias locales de `atn::IRenderLayer`/`atn::Layer3D` en el `.cpp` (como `LayerOcclusionCPU`): el vtable
    muestra que `vf10` recibe la lista de render (`void*`); `include/atn/Layer3D.h` sigue con `vf10()` sin
    argumentos para no cambiar los nombres de las copias debiles ya emparejadas.
  - Tipos inventados: `AABB_unk` local (igual que el de `atn/Geometry_unk.cpp`), `ShadowRenderSystem_unk`
    (lbl_8087EFB4: camara actual en 0x104, camara de sombra en 0x318, buffer de profundidad en 0x79C),
    `ShadowLight_unk` (resultado de fn_800C1424), `ShadowObj_unk`/`ShadowNode_unk` (lista de render: vf14 devuelve
    la caja del objeto que proyecta sombra), `ShadowColorF_unk`, helpers inline `CopyHalf_unk`, `MtxMul_unk`,
    `DepthMtx_unk`, `SnapDown_unk`/`SnapUp_unk`, `TransformBox_unk` (copia del de `game/MapPartsLocatorSet.cpp`).
  - Layout: `mBox` 0x5C, `mCamera` 0x74, `mLightCamera` 0x268, `mImage` 0x45C, dos `GXTexObj` 0x460/0x480 y dos
    matrices 0x4A0/0x4D0 (tamano 0x500).
- Hallazgos MWCC
  - `(int)ceilf(x)` con el inline de MSL (`lfs` + `fmr f1`) frente a `(int)(float)ceil(x)` (carga directa a f1).
  - Una caja devuelta por valor rellenada campo a campo (`AABB_unk r; r.mMin.x = ...; return r;`) no usa
    temporales `Vector3`; con `AABB_unk(Vector3(...), Vector3(...))` el marco crece 0x20.
  - Matriz identidad con stores descendentes (0x124 ... 0xFC, luego los unos) = `Matrix3x4::SetIdentity()`.
- Pendientes: presupuesto de inline de `vf10`; registros de la copia de `Camera_unk` (afecta tambien a
  cualquier otra copia de camara del juego).

## atn/RenderSetting_unk (commit d90d56f)
- Resultado: 14/15 al 100%, `matching: false`, `main.dol: OK`.
- NONMATCHING: ctor `RenderSetting_unk` (0x800B9618) 96.81%: los tres colores blancos del bloom tienen sus
  temporales de pila en orden inverso (0x80/0x70/0x60 frente a 0x60/0x70/0x80) y el planificador coloca
  distinto algunas cargas de constantes; probado: inline que devuelve el color, array con ctor por defecto y un
  sub-objeto Bloom (peor).
- **Decisiones sin el usuario**
  - Splits: sin cambios.
  - Unidad nueva en `config/SLSP01/game_units.json`.
  - `symbols.txt`: pairnames (ctor y metodos de los bloques de parametros), `lbl_80732F00` -> `@stringBase0`
    (pool de 0x7A8, local). `fn_800B95C4` (crea el objeto) y `fn_800BC438` (clave de orden de dibujo, la llama
    `atn/ModelDraw_unk`) siguen `extern "C"`.
  - Cabecera nueva `include/atn/RenderSetting_unk.h` (nombres inventados): `RenderSetting_unk` (0x424, el objeto
    de lbl_8087EFA8 con los ajustes del menu de debug "Render"; miembros `mXXX` con la etiqueta del menu en el
    comentario) y los bloques de parametros que tambien cargan los map parts desde su csv (`game/MapPartsBase`
    llama a sus metodos): `GodRayParam_unk` (0x264), `RainParam_unk` (0x2AC), `HazeParam_unk` (0x2BC),
    `ShadowParam_unk` (0x104), `ColorMatrixParam_unk` (0x324), `DamageParam_unk` (0x374) y `GammaParams_unk`
    (0x3DC; el mismo layout que la copia local de `atn/PostFxGamma.cpp`). Tipos `RsColor_unk` (4 floats POD) y
    `RsVector4_unk` (copia con `__Vec4Copy`).
  - Los metodos `AddMenu` de los bloques (fn_800BB4FC, fn_800BB8D8, fn_800BBA64, fn_800BBEFC, fn_800BC194) se
    autoinlinean en el ctor; el menu de Gamma no tiene funcion propia.
  - `LayerShadow` usa ahora esta cabecera para lbl_8087EFA8 (direccion de sombra = `m104.mDir`).
- Hallazgos MWCC
  - `fn_800DC288` (atof del juego) devuelve `float`: declarada `double` aparecen `frsp` y el prologo pasa de
    `stmw` a `_savegpr`.
  - `x = (c && d) ? (m = f()) : 0;` materializa el booleano en registros salvados (alarga la vida de las
    constantes 0/1 y cambia toda la asignacion de registros del ctor); con `if/else` sale como en el original.
  - Bits `(p >> 4 & 0x7FFFFF) << 6 | (q & 0x3F)` = `clrlwi` + `rlwimi p, 2, 3, 25` con los registros del
    original; con `p << 2 & 0x1FFFFFC0` los registros salen intercambiados.
  - Temporal `RsColor_unk(r, g, b, 1.0f)` con `r/g/b` `float` locales: el temporal guarda los valores tal cual y
    el miembro recibe la copia; con locales `double` aparecen `frsp` antes del temporal.
- Pendientes: el orden de los temporales de color del ctor; nombre real del fichero desconocido.
