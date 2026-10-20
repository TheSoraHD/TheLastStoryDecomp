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
