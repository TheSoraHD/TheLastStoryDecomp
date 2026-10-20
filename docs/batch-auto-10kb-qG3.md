# batch-auto-10kb qG3: game/gmk

Cola: `game/SystemErrorMessage_unk`, `game/GameOverTask`, `game/GimmickStateCommand` (prioridad 1),
`gmk/DropItem`.

## game/GimmickStateCommand (commit 30298ea)
- Resultado: 9/39 al 100%, `matching: false`, `main.dol: OK` (regress lost: 0).
- NONMATCHING:
  - `GimmickCommonB0_unk::fn_803EF780` 0%: el original supera el presupuesto de inline y llama fuera
    de linea a todas las inline (sus 25 copias debiles 0x803EFCA8-0x803F069C van detras); aqui se
    inlinean todas. Con una sonda de ~16 sentencias extra el codigo casa salvo registros (this en r27,
    contadores r29-r31): las inline reconstruidas de MSL/atn (vector, string, CsvTab, Buffer_unk) son
    mas pequenas que las originales. Por eso faltan las 25 copias debiles y el pool de cadenas cambia.
  - `fn_803F0764` 94.61% (huecos de pila de los temporales int->float y de los vectores/color de los
    efectos, registros de motion/model/loop, retorno tras vf38 con bne + b).
  - `fn_803F11F8` 99.57% y `fn_803F1310` 99.78%: solo desplazamientos del pool (consecuencia de lo anterior).
  - `GimmickStateCommand::vf10` 94.46%: registros del bucle de la tabla de sonidos.
- Decisiones sin el usuario:
  - Split sin cambios (ya cubria el fichero, ver splits-auto-C.md grupo 1).
  - symbols.txt: fn_803F13D4/fn_803F1418 -> ctor/dtor de `GimmickStateCommand::Entry_unk`,
    fn_803F1474 -> `__dt__19GimmickStateCommandFv`, fn_803F1508 -> `vf10__19GimmickStateCommandFv`,
    lbl_80752040 -> `sCommandNames` (scope:local), lbl_80752084 -> `@stringBase0`.
  - include/game/GimmickCommon.h (compartida): `GimmickCommonB0_unk` con su layout real (5 `atn::Buffer_unk`
    de comandos/ResLoader/EfpHandle/SoundRef/MotionCmdElemD + mMotion/mEnabled/mSoundOn), struct nueva
    `GimmickMotionCmd_unk` (0x28), `GimmickStateCommand` completa (ctor, dtor y vf10 virtuales, Entry_unk
    {SeTable*, frame, done, SoundRef}, `EntryList_unk` derivada de FixedArray_unk como LogList_unk). Incluye
    ahora atn/FixedArray.h y atn/SoundRef.h. GimmickCommon.o y el resto sin cambios (regress 0).
  - include/atn/Array_unk.h: `Buffer_unk::Resize_unk(n, heap, name)` (nombre del dueno sin usar).
  - include/atn/Csv.h: ctor `CsvTab(const void*, int)` que pone los separadores (copia debil 0x803EFCA8)
    y `extern lbl_8087D710`.
  - include/std/vector_string.h (nueva): `std::vector<std::string>` mueve los strings como bytes
    (memcpy/memset) y destruye el buffer viejo y el temporal (la copia debil de push_back sale con el
    tamano del original 0x801207D4, 0x44C; no comparada instruccion a instruccion).
  - Nombres inventados: GimmickMotionCmd_unk y sus campos, Entry_unk, EntryList_unk, VecIter_unk (iterador
    de vector con copias debiles propias), MapInfo_unk/Field_unk/SoundCfg_unk/ShakePlayer*_unk locales.
  - Literal `"%s %s\0"` en el menu de depuracion: el pool tiene un cero extra tras "%s %s" (0x44).
  - Nombre real del fichero: GimmickStateCommand.cpp (por la clase); contiene tambien GimmickCommonB0_unk.
- Hallazgos MWCC:
  - `for (int i...; i < (unsigned)buf.mSize; ...)` (indice con signo, comparacion sin signo) pone el
    incremento del indice detras del del puntero; con `unsigned i` va delante.
  - Un FixedArray_unk de elementos con destructor como miembro: dos `beq` sobre this antes de
    __destroy_arr = struct derivado de FixedArray_unk (como LogList_unk). Cambiar FixedArray_unk a
    base+derivada rompe LogMessageTask (3 niveles).
  - La copia debil de un template no inline definido fuera de la clase se emite al final del .o (sus
    literales al final del pool); en el original push_back sale justo tras su llamadora.
- Pendientes para otras unidades:
  - game/MotionCommand_unk (0x8011FF78) tiene las copias debiles de vector<string> (0x801206EC,
    0x80120700, 0x801207D4, 0x80120D34) y MotionCmdElemD_unk::operator= (0x8012044C): sirve
    include/std/vector_string.h. Probablemente tiene el mismo problema de presupuesto.
  - gmk/Cannonball usa fn_803EFDA4/fn_803EFDAC (accesores de MapInfo_unk) como extern "C" inline.

## gmk/DropItem (commit 9e33934)
- Resultado: 10/13 al 100%, `matching: false` (unidad nueva en game_units.json), `main.dol: OK` (regress lost: 0).
- NONMATCHING:
  - `vf10` 90.61%: asignacion de registros (el original tiene el jugador en r22 y usa r16-r31), orden de
    los stores del mensaje de tutorial y de los locales de la prueba de contacto con las balas.
  - `vf14` 84.49%: el original construye un local muerto con m9C del campo y compara las 4 componentes
    del color con 0 y 1 sin usar el resultado (44 instrucciones; no se encontro el fuente).
  - `vf40` 99.92%: posicion en pila del temporal de la animacion del item (debajo de los temporales del rayo).
- Decisiones sin el usuario:
  - Split sin cambios (ver splits-auto-C.md grupo 5). Unidad anadida a game_units.json (`gmk/DropItem.cpp`,
    `gmk::DropItem`, matching false). No enlazable igual que gmk/Item: define el static local de
    fn_8014F8F4 (CharaListeners) y su guarda.
  - symbols.txt: ctor, vf0C, vf10, vf14, vf1C, vf20, vf40, vf68, vf6C, dtor y fn_8040B200 de gmk::DropItem
    con nombre real; lbl_80752A5C -> @stringBase0, lbl_8078D4F0 -> @wstringBase0, lbl_808862B8/BC ->
    sGetSound/sPosPane, lbl_80752A18/40 -> sTextPanes/sColorPanes (scope:local).
  - include/gmk/DropItem.h ampliada con el layout completo (0x2A8; m2A4 de relleno inferido) y
    DropLayout_unk/DropItemData_unk; los nombres antiguos usados por Item/Cannonball se conservan.
  - include/gmk/ItemSlotAnime.h (compartida): 5 metodos declarados, `IsState56_unk`, y la palabra de
    flags en +0xEC como campos de bits (mFlag31 raro, mFlag26 aviso visible, ...). Sin cambios en otras unidades.
  - fn_8040B394 queda `extern "C"` (efecto por tipo de item). El pool ancho del original tiene L"" en +6
    y +0xA (y ceros en +4/+8): aqui solo L"?" + L"".
- Hallazgos MWCC:
  - `const T& r = a + b;` (referencia a temporal) se coloca como local con nombre, no como temporal.
  - Pasar el vector como `const Vector3&` a una funcion (fn_8004ECC0) hace que los `a + b` de los
    argumentos sean temporales en orden de evaluacion (derecha a izquierda).
  - Test `(x & 8) != 8` materializado con subfic/or/srwi. = bool devuelto por un inline.
- Pendientes: gmk/Item (decompilada) y DropItem comparten el static local de CharaTask; para enlazar
  hay que resolver como referenciar el de CharaTask.cpp sin definirlo.

## game/SystemErrorMessage_unk (commit 7bd3fca)
- Resultado: 9/11 al 100%, `matching: false` (unidad nueva en game_units.json), `main.dol: OK` (regress lost: 0).
- NONMATCHING:
  - `fn_80363DBC` 99.47%: el original relee lbl_8087F018 para m40C0 y mError al conservarlo; orden de
    registros de las constantes 3/0 y de los contadores del fade.
  - `fn_803646C4` 98.04%: huecos de pila del color y del test del puntero a miembro, registros del bucle
    de la fuente (constante 1.0 y doble de conversion intercambiados).
- Decisiones sin el usuario:
  - Split sin cambios (splits-auto-C.md grupo 2). Clase `SystemErrorMessage_unk` (0x4C, nombre inventado;
    la misma que GameFlow_unk de game/GameSystem_unk.cpp, instancia lbl_8087F420). Miembros con nombres inventados.
  - symbols.txt: ctor y metodos fn_80363DBC/fn_803646C4/fn_80364BE8/fn_80364DB4 (Fiiii)/fn_80364E7C/
    fn_8036503C/fn_80365320 con mangling de la clase; fn_80364DAC -> `get__Q25boost14scoped_ptr<Uc>CFv`
    (copia debil, apuntada por el puntero a miembro de 0x80789EC8); lbl_8074D8C0 -> @stringBase0;
    lbl_8074CCD0 -> sMessages, lbl_80789EB0/EC0 -> sLinePanes/sChoicePanes (scope:local).
  - Tabla de mensajes `sMessages[6][31][4]` generada desde los datos del DOL (escapes \x de SJIS); el orden
    de primera aparicion coincide con el orden de los literales en .rodata.
  - fn_80363C28 y fn_80363CE4 quedan `extern "C"`. m48 es `boost::scoped_ptr<unsigned char>` (operator
    unspecified_bool_type de Boost para MWCC: ptmf a get()).
  - Literal `"%s\0"` en el sprintf (cero extra en el pool tras "%s", como en GimmickStateCommand).
- Hallazgos MWCC:
  - `extsh` de todos los parametros al principio = parametros int copiados a locales `short` (GX s16);
    `x1 = x0 + w` con el local corto y `s1 = s + w` con el parametro.
  - Un setter inline que recibe el puntero ya calculado (`&task->mData.m10`) fija el orden: direccion del
    layout antes del hash (con la tarea como argumento, la direccion se calcula despues).
  - Conversion float->u8 con `__cvt_fp2unsigned` = cast a `unsigned int`, no a `unsigned char` (fctiwz).
  - Comprobacion "visible" que relee mFlags antes del `|= 4` = `Task::IsFlag_unk(4)`.
- Pendientes: nada.

## game/GameOverTask (commit fa40541)
- Resultado: 2/7 al 100% (ctor 99.59%, fn_80377298 99.45%), `matching: false`, `main.dol: OK` (regress lost: 0).
- NONMATCHING:
  - ctor 99.59%: registros de la copia del nombre (origen/destino intercambiados).
  - `vf0C` 48.93%: la preparacion de la escena (luz, entorno y estado de camara de lbl_8087EFA8) se copia
    con structs temporales cuyo layout no se reconstruyo (muchos stores muertos en pila).
  - `fn_80377298` 99.45%: registro de la direccion de la unica opcion en .sdata2.
  - `vf10` 89.58%: el original mantiene las seis tablas de textos en registros, pasa un tercer argumento a
    MenuLoadTask::fn_80117914 (la declaracion conocida tiene dos, como en NewCampPartsTop) y otros stores del fade.
  - `vf14` 84.74%: posicion/objetivo de la camara via copias locales y planificacion de la copia de Camera_unk.
- Decisiones sin el usuario:
  - Split sin cambios (ya cubre .rodata 0x8074DDF8-0x8074DF88).
  - symbols.txt: ctor, dtor, vf0C, vf10, vf14 y fn_80377298 con nombre de clase; lbl_8074DEC0 -> @stringBase0.
  - Clase definida en el .cpp (sin cabecera): tamano 0x298 (relleno m290 inferido), FixedArray_unk<int, 5>
    de opciones, Camera_unk en 0x94. Tablas de mensajes/opciones por modo como arrays const separados
    (el bucle de textos lee 5 entradas de tablas de 4: lee la primera opcion, como el original).
  - Literal `"\0"` para el nombre vacio del CampSaveArg (dos ceros en el pool en 0x69/0x6A).
  - Declaraciones locales copiadas de game/NewCampPartsTop.cpp (CampSaveData_unk, MenuLoadTask, MenuCommonTask...).
- Pendientes: reconstruir los structs de escena de lbl_8087EFA8 (0x3C, 0x54-0xD0, 0x324, 0x374) para vf0C.
