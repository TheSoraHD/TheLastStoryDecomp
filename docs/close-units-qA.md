# Lote close-units, cola A (atn y std)

Inicio: 2026-10-22 04:21 CEST. Rama agent-A. Nota: la herramienta Projects no estaba disponible en esta sesion;
las reglas se tomaron del prompt del lote y de las notas de docs/.

## Unidades

- `std/char_traits_char` (1/1 al 100%): NO enlaza. Fuente reescrita como especializacion completa
  `basic_ios<char, char_traits<char> >` con destructor fuera de linea (funcion clave): el .o emite exactamente
  el dtor 0x58, la vtable y las dos RTTI (antes salia ademas una funcion auxiliar de 0x38 que no esta en el
  original). Con `matching: true` el .dol falla: MWCC alinea la .rodata del .o (nombres RTTI) a 8 y el split
  empieza en 0x807350A4 (alineado a 4), asi que todo lo posterior se desplaza 4 bytes (0x807350A4 -> 0x807350A8).
  No se puede arreglar sin tocar `atn/WordWrap_unk` (unidad con fuente, su .rodata acaba en 0x807350A4): se queda
  `matching: false`. Solo enlazara fusionada con el fichero anterior (WordWrap_unk + char_traits_char + allocator_char,
  ver batch-auto-10kb-qA2.md).
- `atn/D2A_Frame` (2/3): sin cambios. `vf18` sigue 99.85% tras 10 intentos (orden de declaraciones, copia
  del color, temporales del alfa, forma de fn_ToU8, bloques): solo cambia el reparto de registros flotantes.
- `atn/D2A_MessageTable_unk` (5/6): saltada, bloqueo documentado (`push_back` en bucle: tamano/capacidad).
- `atn/EffectLoader_unk` (4/5): `fn_80239030` 90.51% -> 93.20% (10 intentos). Las tablas de curvas de
  versiones antiguas se reservan como `new EffectCurveData_unk[n][22]` (un solo `mulli 0xB0`, como el
  original). Queda reparto de registros (cabecera r26/recurso r27 en el original).
  - Hallazgo MWCC: `(T(*)[N]) new T[n * N]` sale con `mulli N` + `slwi`; `new T[n][N]` con un solo `mulli N*sizeof(T)`.
- `atn/EffectSequencer_unk` (13/14): `fn_802440F0` sigue 95.55% tras 10 intentos (referencia, `continue`,
  helper `fn_802438C4`, indices directos, max con `if`...): solo r30/r31 intercambiados.
- `atn/EfpHandle_unk` (14/15): `fn_802371BC` sigue 74.74% (6 intentos mas sobre los ~20 previos). Con
  `__sthbrx` en el bucle de 16 bits salen menos lineas distintas pero no se desenrolla (67%): descartado.
- `atn/LH_ColliTree`, `atn/LH_Efp`, `atn/LH_Motion_cont`, `atn/LH_Texture` (y las demas `LH_*` de la cola):
  saltadas, bloqueo documentado de la plantilla `Loader::Get_unk<T>` (registros, ver `include/atn/LoadManager.h`).
  - symbols.txt (decision sin el usuario): las 11 copias debiles `fn_` de `Get_unk<T>` renombradas a su nombre
    decorado `Get_unk<Q23atnN<T>>__Q23atn6LoaderFPCci_PQ23atnN<T>` (0x80472A9C ColliObject, 0x80472DCC ColliTree,
    0x8047392C Effect, 0x80473C88 Efp, 0x80474678 Material, 0x80475F20 Model, 0x80476AC8 MotionCommandData,
    0x8047749C MotionSet, 0x80477DB8 Scene, 0x80478204 Texture, 0x80478518 Xsca). Pasan de 0% a ~96.8% en el
    report (ahora emparejadas); `fn_804766B0` (LoadMotion) no se toca porque `LH_Motion.cpp` la llama `extern "C"`.
- `atn/LH_Handle` (8/9): `fn_80473FCC` sigue 99.22% (7 intentos nuevos sobre los 8 del comentario).
- `atn/NumberFormat_unk` (3/4): `fn_800DD3FC` sigue 99.39% (10 intentos: permutaciones de declaraciones,
  `number` fuera, `num` partido, bucle de copia con puntero aparte).
- `atn/ParseTree`: `fn_800A6E8C` 95.33% -> 100% (25/25). El indice 1-based se escribe como desplazamiento en bytes
  `*(float**)((unsigned char*)vars->mVars + (idx - 1) * sizeof(float*))`.
  - Hallazgo MWCC: `p[i - 1]` pliega el -1 en el desplazamiento (`slwi; add; lwz -4`); con el desplazamiento en
    bytes escrito a mano sale `subi; slwi; lwzx`.
  - ENLAZADA (`matching: true`). Para enlazar: el pool `@stringBase0` del original acaba con "tree::insert length
    error" de una insercion de `std::map` que el enlazador elimino; se reconstruye minima como
    `atn::fn_Stripped_unk(unsigned long)` (patron de Event/Label) para que la .rodata mida 0x98. Nuestro .o
    emite ademas `__vt__Q23atn8BaseNode`/`__dt__Q23atn8BaseNodeFv` debiles que el original no tiene: el
    enlazador los descarta (sin referencias) y el DOL sale OK.
- `atn/RenderSetting_unk` (14/15): ctor 96.81% -> 96.92% (9 intentos): los tres blancos del bloom con un helper
  `sWhite_unk()` que devuelve el color por valor. Sigue el orden inverso de los temporales en pila y el
  planificador.
- `atn/Layer2DBlur`: saltada (marcada en la cola: sin/cos).
- `atn/PostFxColorMatrix`, `atn/PostFxDof`, `atn/PostFxMotionBlur`, `atn/PostFxRain`: saltadas (marcadas: emiten ~IPostFx).
- `atn/TaskBase` (3/4): ctor sigue 61.09% tras 9 intentos (locales, palabras sueltas del id/etiqueta, struct de
  dos palabras, puntero a la etiqueta): el planificador coloca las cargas de `sTaskTag` despues del incremento.
- `atn/TextureDraw_unk` (3/4): saltada, `fn_800D5F68` (0x930) sin escribir; demasiado grande para este lote.
- `atn/Texture_unk` (8/9): `fn_800D5A3C` sigue 99.69%. Pasando el tamano de `GXGetTexBufferSize` directamente
  al reservar se arregla r28/r31 (queda solo el orden ancho/alto), pero `fn_800D5B58` baja de 100% a 99.87%:
  descartado. 10 intentos.
- `atn/VideoTHP` (12/13): `vf10` sigue 99.28% tras 9 intentos (registros volatiles). No se fusiona
  `std/char_traits_char`.
- `atn/AABBTree` (11/13): `vf18` sigue 97.37% (6 intentos: orden de `mr r3` del frustum en el inline de
  `fn_80045C88`); `fn_80045D7C` 85% saltada (bucle de ctores de `AABB_unk[4]`, ya investigado).
- `atn/Camera_unk` (2/4): saltada (bloqueo `__VecCopy` r3/r4).
- `atn/ColliBaseFunc_unk` (15/17): saltada (borradores de 85.8%/93.2% de 6.4 KB/1.8 KB; fuera del alcance del lote).
- `atn/CompositeCache_unk` (5/7): saltada (`push_back` en Get_unk-like y bucle de lista ya investigado, ~10 variantes).
- `atn/LayerPostFx`: ENLAZADA (6/6). `Vec2f` (include/atn/LayerPostFx.h, solo la usa este fichero) copia con
  `__Vec2Copy` (asm aprobado de math/Vec.h, como DialogVec2_unk): vf20 47% -> 100%. vf1C 78% -> 100% guardando
  `mSaved` con `__Vec2Copy` explicito (con el operator= salian r4/r5 intercambiados). Las copias debiles de
  IRenderLayer que emite el .o las descarta el enlazador (como en Layer3D/LayerReflect).
  - CABECERA (solo de esta unidad): `include/atn/LayerPostFx.h` incluye math/Vec.h y da a Vec2f ctor de copia y operator=.
- `atn/D3Anime`, `atn/D3AnimeInstance`, `atn/LipSync_unk`: saltadas (solo registros, ya investigadas; D3Anime vf14 es el
  mismo intercambio r4/r5 del inline de carga).
- `atn/Pad_unk` (15/17): `fn_800A58D0`/`fn_800A58F8` siguen 97% (10 intentos: el original deja el gestor en r3 y la tabla en r5).
- `atn/LightUtil_unk` (4/7): sin cambios. `fn_8007A154` 98.73%: con el borrado en un inline `Erase_unk` definido despues se
  relee mCount como en el original pero se intercambian dst/src (r6/r7) - descartado; `fn_8007A36C`/`A41C` 99.5%
  (10 intentos cada una).
- `atn/TextureCacheManager8x32` (6/7): `ITextureCacheManager::fn_800D6898` 0% -> 100% (nueva: region de cache de una
  textura; `int mipmap`, con `unsigned char` sale el clrlwi en cada prueba). Ctores 99.07% sin cambios (4 intentos
  nuevos). Nombres de SDK declarados localmente: `GXGetTexObjFmt`, `GXGetTexObjMipMap`, `fn_806163B0` (GXGetTexObjData).
- `atn/D2A_Data` (10/13): `D2A_Track_unk` ctor sigue 87.22% (9 intentos: inline con rango, limites por &mValue,
  while, copia de struct): el original no pliega begin/end del buffer vacio.
- `atn/Geometry_unk`, `atn/LensFlare`, `atn/SequenceHolder` (fn_800C3D74/E84 son find_if con boost::lambda),
  `atn/D2Anime_unk`, `atn/D2A_Message`, `atn/Light_unk`, `atn/PostFxGamma`, `atn/Rain`, `atn/Shader_unk`,
  `atn/ptr_string_base_char_0`, `atn/ColliCalc_unk`: saltadas por tiempo/bloqueos documentados (contract de fmadds,
  inline no deseado, pools, solo registros ya investigados).
- `atn/D2A_TaskInstanceBase_64_8` (47/51): las 4 `vf10`/`Update` siguen 97% (10 intentos: el original relee mTask
  tras `mFrame += mSpeed`).
- `atn/CsvBase_char_124_13_34` (4/5, era 1/5): reconstruida la tabla /boot/battle_eff_se_table.csv
  (`fn_802078C4` cargador 99.48%, `fn_80207C00` cuenta, `fn_80207C08` fila, `fn_80207C34` prueba de nivel: las
  tres al 100%). NONMATCHING del cargador: r24/r26 (fila / cadena de niveles) intercambiados.
  - CABECERA COMPARTIDA `include/atn/Csv.h`: `atn::CsvPipe` recibe el ctor `(text, length)` que pone los
    separadores `lbl_8087DB20` (" \t", .sdata de la unidad) como `atn::Csv`; regress lost 0, fuzzdrop 0.
  - Hallazgo MWCC: con declaraciones C al principio, el orden decide la numeracion de los GPR salvados
    (la primera declarada recibe el registro mas alto entre las que viven en todo el bucle).
- Accesores pequenos de las `LH_*` (0% -> 100%, 20 funciones):
  - `atn/LH_ColliObject`: `fn_80472C90` (esfera envolvente, `extern "C"` como lo declara atn_colli).
  - `atn/LH_Material`: `LH_Material::fn_80474860` (lista de materiales) y `fn_8047486C` (cuenta); symbols.txt
    renombrados a `fn_80474860__Q23atn11LH_MaterialFv` / `fn_8047486C__Q23atn11LH_MaterialFv`; cabecera
    `include/atn/LH_Material.h` (solo la usa esta unidad y quien la incluye para el tipo).
  - `atn/LH_Xsca`: `LH_Xsca::fn_804786F8` (animacion cargada); symbols.txt `fn_804786F8__Q23atn7LH_XscaFv`;
    cabecera `include/atn/LH_Xsca.h`.
  - `atn/LH_MotionCommandData`: `fn_80476CE4` (ya declarada en su cabecera).
  - `atn/LH_Scene` (11/12): `fn_80477FA4/FC0/FDC/FF8/80478014/80478034` (luces, camaras y registros de luz;
    `extern "C"` como los declara CharaLightData_unk). `SceneHeader_unk` gana `m18`/`m1C`.
  - `atn/LH_Model` (15/17): `fn_80476130`..`fn_80476204` (8 accesores, `extern "C"` como los declaran
    ModelDraw_unk/LayerOcclusionCPU).
  - `atn/LH_MotionSet` (5/9): `fn_80477694` (carga sin compartir) al 100%; `fn_804776F4` ~98% (contador/desplazamiento
    r30/r31, 7 intentos).
  - Decision sin el usuario: los nuevos `extern "C"` mantienen el nombre `fn_` de symbols.txt; solo se renombran a
    miembro los que nadie llama desde fuente (Material, Xsca).
- `std/char_traits_char`: probado tambien `align:4` en su .rodata de splits.txt: no sirve (la alineacion a 8 la
  impone el .o de MWCC); revertido.
- `atn/NetFriend_unk` (22/30, era 19/30): `fn_8050EF14` (borrar amigo y guardar en "online/playerinfo7"),
  `fn_8050F404` (primer hueco libre) y `fn_8050F464` al 100%; `fn_8050F470` (clave de amigo como texto) 99.85%
  (solo desplazamientos del pool de cadenas anchas, como fn_8050F734).
- Saltadas por bloqueo documentado: `atn/D2A_Manager` (presupuesto de inline), `atn/CompositeIntensityTintMulti`
  (presupuesto de inline de vf14), `atn/SmallAllocator_8` (ciclo de inline del gestor), `atn/Geometry_unk`
  fn_80070D04 (`__VecCopy` r3/r4), `atn/FileList` y `atn/TextureDraw_unk` (sin escribir, grandes).
