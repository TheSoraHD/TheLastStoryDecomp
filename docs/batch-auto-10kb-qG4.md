# batch-auto-10kb qG4: game/MP (Gun, HomingBullet, main, MapPartsLocatorSet, MP/Text)

Cola: `game/Gun`, `game/HomingBullet`, `game/main`, `game/MapPartsLocatorSet`, `MP/Text`.

## game/Gun + game/HColliData_unk (commit 9c49c28)
- Resultado: `game/Gun` 5/6 al 100%, `matching: false`; `game/HColliData_unk` (nueva) 4/11 al 100%,
  `matching: false`. `main.dol: OK`, regress lost: 0.
- NONMATCHING:
  - `Gun::vf14` 99.91%: el original carga primero la z de la traslacion (pool .sdata2 3.7, 0.5, 0.0)
    pero guarda el vector empezando por x; probados vector con nombre, asignaciones z/y/x, Set_unk,
    helpers inline por valor/referencia/componentes. Por eso `.sdata2` queda al 50% en objdiff.
  - `ColliObjectData_unk::fn_8043F694` 38%, `fn_8043F908` 73%, `ColliTreeData_unk::ColliTreeData_unk`
    49%, `fn_8043FEE8` 60%, `fn_80440184` 45%, `fn_80440280` 62%, `fn_80440360` 56%: el original
    desenrolla los bucles de intercambio de bytes (`stwbrx`) y usa punteros de induccion separados
    para cargas y stores; Wii/1.3 (y 1.0/1.1/GC 3.0a5.2) nunca desenrolla un bucle cuyo cuerpo usa
    `__stwbrx` (ni con inline, static, plantilla ni puntero). Ademas el original inlinea una vez la
    recursion de `fn_80440360`.
  - `__sinit` de HColliData_unk (0x804405CC) no escrito: solo pone a 1 los guards compartidos
    0x8087F3CC/0x8087F3CD (ids de facetas `std::num_put`/`numpunct` de MSL, como el `__sinit` de
    `game/GameScene` 0x803CCF38); el fichero real incluye cabeceras de locale de MSL.
- **Decisiones sin el usuario**:
  - splits.txt: `game/Gun` cortada. Antes: .text 0x8043F028-0x804405F8, .ctors 0x8072D370-0x8072D374,
    .rodata 0x80754498-0x807544C8. Despues: `game/Gun` .text 0x8043F028-0x8043F5C4, .rodata
    0x80754498-0x807544B8 (.data/.sdata2 igual); nueva `game/HColliData_unk.cpp` .text
    0x8043F5C4-0x804405F8, .ctors 0x8072D370-0x8072D374, .rodata 0x807544B8-0x807544C8.
    Prueba: el pool de cadenas de Gun (`@stringBase0` 0x80754498) va al principio de .rodata y las
    constantes magicas "@HCB"/"@HOC" detras, y el `__sinit` cierra el tramo ColliData: dos ficheros.
  - Nombre inventado `game/HColliData_unk.cpp`: el orden de enlace de esta zona es alfabetico por
    nombre de fichero (CharaPathMove, Gun, ?, HomeButtonMenu...), asi que el real empieza entre
    "Gun" y "HomeButtonMenu". Clases `ColliObjectData_unk`/`ColliTreeData_unk` (ya declaradas en
    atn/LH_ColliObject.cpp y atn/LH_ColliTree.cpp, sin cambiar).
  - Firma de `Arms::vf18`/`Sword::vf18` cambiada a `(const atn::Vector3&, const atn::Vector3&, int, int)`
    (la de `chr::CharaPart_unk::vf18`, como la llama CharaTask): `Gun::vf18` lee el cuarto argumento
    (`fn_80219E6C(d)`). Cabeceras compartidas `include/game/Arms.h`, `include/game/Sword.h`,
    `src/game/Arms.cpp`, `src/game/Sword.cpp` tocadas; symbols.txt `vf18__4ArmsFv` ->
    `vf18__4ArmsFRCQ23atn7Vector3RCQ23atn7Vector3ii`, `vf18__5SwordFv` -> igual con `5Sword`. Sword
    sigue `matching: true` y el DOL OK.
  - symbols.txt: funciones de Gun por pairnames (+ `vf14__3GunFv`), `__vt__3Gun`, `__RTTI__3Gun`,
    `@stringBase0` 0x80754498 (scope:local, 0x1C); funciones de ColliObjectData_unk/ColliTreeData_unk
    0x8043F694-0x8044058C con nombres `fn_<addr>__<clase>`; lector de chunks del tramo HomingBullet
    renombrado: 0x80442B50 `__ct__13ChunkFile_unkFPvUi`, 0x80442E64 `__dt__13ChunkFile_unkFv`,
    0x80442EA4 `IsSwapped__13ChunkFile_unkCFv`, 0x80442EAC `GetChunkCount__13ChunkFile_unkCFv`,
    0x80442ECC `FindChunk__13ChunkFile_unkFiUi`.
  - Cabeceras nuevas: `include/game/Gun.h`, `include/game/ChunkFile_unk.h` (clase inventada
    `ChunkFile_unk`: lector de ficheros de chunks con offsets relativos y cambio de endianness).
  - game_units.json: entradas nuevas `game/Gun.cpp` y `game/HColliData_unk.cpp`.
- Hallazgos MWCC:
  - Wii/1.3 no desenrolla bucles cuyo cuerpo usa `__stwbrx`/`__lwbrx` (si los de stores normales).
  - Miembro con destructor llamado desde el dtor de la clase: `li r4,-1` (no 0 como la base).
  - `float sz = Col(2); float sy = Col(1); float sx = Col(0); Vector3 s(sx, sy, sz);` dentro de un
    inline da el orden de pila del original (s debajo de los tres temporales de columna).
- Pendientes: `__sinit` con guards de facetas MSL (si se quiere enlazar HColliData_unk).

## game/HomingBullet + game/IffChunkFile_unk (commit 7e04e15)
- Resultado: `game/HomingBullet` 8/10 al 100%, `matching: false`; `game/IffChunkFile_unk` (nueva) 4/5 al
  100%, `matching: false`. `main.dol: OK`, regress lost: 0.
- NONMATCHING:
  - `HomingBullet::vf28` 84.3%: el original carga todo el bloque de parametros antes de guardar los campos
    del dueno (planificacion; probados parametro por valor, copia a local, struct sin ctor) y elige
    otros registros en el evento de replay.
  - `HomingBullet::vf10` 99.93%: r3/r4 de la copia de vector inline (familia `__VecCopy`).
  - `HomingBullet::vf1C` (0x80441F24, 0xB78) sin escribir (declarado): reaccion al impacto, rebote
    con matrices 4x4, `atan2`, `chr::Control` nuevo, sonidos (pool "SE_DG036_015"/"SE_BTCOM_015").
  - `ChunkFile_unk::ChunkFile_unk` 86.6%: registros de la rama con cambio de endianness; el original
    calcula las direcciones de la cabecera con `addi` donde aqui sale `lwzu`.
- **Decisiones sin el usuario**:
  - splits.txt: `game/HomingBullet` .text 0x80441590-0x80442F10 -> 0x80441590-0x80442B50; nueva
    `game/IffChunkFile_unk.cpp` .text 0x80442B50-0x80442F10 (solo .text). Prueba: las 5 funciones del
    lector de chunks no usan datos de HomingBullet y se llaman fuera de linea desde otro fichero
    (HColliData_unk). Nombre inventado: el orden alfabetico de enlace exige un nombre entre
    "HomingBullet" y el del gestor de objetos (`ItemManager_unk`, tambien inventado).
  - Cabeceras nuevas: `include/game/Bullet.h` (clase base `Bullet` inferida: layout desde HomingBullet,
    metodos no virtuales como `extern "C"` con `Bullet*`, `BulletShot_unk`), `include/game/HomingBullet.h`
    (`HomingParam_unk`, 0x24 bytes en mA0).
  - symbols.txt: `fn_8010ED4C` -> `__ct__6BulletFv`, `fn_8010EE38` -> `__dt__6BulletFv` (unidad asm
    game/Bullet), `fn_800F1EE0` -> `__dt__12HomingBulletFv` (copia debil del dtor implicito, unidad asm
    game/BattleTask); funciones de HomingBullet por pairnames y a mano (vf0C, vf10, vf1C, vf28),
    `lbl_80754608` -> `@stringBase0` (scope:local, 0x1D).
  - game_units.json: entradas nuevas `game/HomingBullet.cpp`, `game/IffChunkFile_unk.cpp`.
- Hallazgos MWCC:
  - `int r = ((m04 >> 2) & 1) ^ 1;` da `rlwinm` + `xori`; `!((m04 >> 2) & 1)` da `cntlzw/srwi`.
  - `cos()` llamado desde un inline `float CosF(float x)` deja la carga del argumento antes de guardar
    el resultado anterior (`lfs f0; fmr f28,f1; fmr f1,f0`).
  - Comparacion de un `unsigned char` con `cmpwi` (no `cmplwi`): `(int)c == 6`.
- Pendientes: `HomingBullet::vf1C`; `game/Bullet` (asm) tiene la clase base completa para decompilar.

## game/main (commit 8395d0f)
- Resultado: 11/14 al 100%, `matching: false`, `main.dol: OK`, regress lost: 0.
- NONMATCHING:
  - `main` 98.6%: reparto de registros salvados (el original deja el desplazamiento del bucle de
    callbacks en r31); probados bucle sin local, referencia e inline.
  - `fn_8047A4C8` (arranque) 39.7%: el original inlinea los ctores de `boost::function` desde
    `boost::bind`, las `std::string`, `LightDesc_unk` y `ResRef_unk` pero llama a
    `Vector_unk::push_back` fuera de linea (0x8047AEBC). Aqui los tres `push_back` sacan de linea a
    todas las candidatas (todo o nada). Probablemente el `push_back` original recibe el elemento por
    valor (nunca se inlinea); no se cambia `include/atn/Array_unk.h` porque lo usan otras unidades con
    tipos POD. Ademas empieza con `OSInitFastCast` (asm inline de la cabecera del SDK, `mtspr GQR2-5`),
    no escrito (regla 2): pendiente de permiso si se quiere usar `revolution/os/OSFastCast.h`.
  - `atn::Vector_unk<boost::function<void()>>::push_back` (copia debil, 0x8047AEBC) 99.4%: huecos de
    pila de los temporales del `operator=` de `boost::function` en el bucle de `reserve`.
- **Decisiones sin el usuario**:
  - Splits sin cambios.
  - Cabecera nueva `include/game/HeapInfo_unk.h` (struct `HeapInfo_unk`/`FrameTimer_unk` sacados de
    `src/game/HeapInfo_unk.cpp`, que ahora la incluye; anadidos los inline `Start_unk`/`Stop_unk`).
    HeapInfo_unk sigue `matching: true` y el DOL OK.
  - Clases con nombre real por los nombres de tipo de boost en .rodata: `ResetShutdown` (es el
    `GameSystem_unk` de `src/game/GameSystem_unk.cpp`) y `ErrorManager` (lbl_8087F420).
    `main` llama a `fn_80570CF8` como `extern "C"` (asi lo define GameSystem_unk.cpp) y el bind usa
    `ResetShutdown::fn_80570CF8` (simbolo sin definir: el puntero a miembro de .data 0x80790068 no
    empareja). Pendiente: renombrar `GameSystem_unk` -> `ResetShutdown` con `fn_80570CF8` como metodo.
  - symbols.txt: `fn_80363DBC` -> `fn_80363DBC__12ErrorManagerFv`, `fn_803646C4` ->
    `fn_803646C4__12ErrorManagerFv` (unidad asm, solo referenciados por los punteros a miembro),
    `fn_800BD94C` -> `__ct__18RenderCallback_unkFQ25boost36function<FPv_v,Q23std12allocator<v>>i`
    (unidad asm atn/RenderSystem_unk), `fn_8047A4C8` -> `fn_8047A4C8__Fv`, invokers/managers de boost
    y `fn_80479EA0`/`fn_8047A158` (`RenderSystem_unk`, inline del sistema de render: tipos y nombres
    inventados) por pairnames y a mano.
  - game_units.json: entrada nueva `game/main.cpp`.
- Hallazgos MWCC:
  - Un llamador con varias llamadas a una candidata grande (push_back) saca de linea tambien
    `boost::bind` y el ctor plantilla de `boost::function` (todo o nada); con una sola llamada se
    inlinea todo.
  - Las inline con una sola sentencia `m8B0 = Callback(func, arg);` (ctor de otro fichero que recibe
    `boost::function` por valor) salen como copias debiles identicas a las del original.

## game/MapPartsLocatorSet (commit 0ba230e)
- Resultado: 9/13 al 100%, `matching: false`, `main.dol: OK`, regress lost: 0.
- NONMATCHING:
  - ctor 99.91%: r27/r28 (longitud y cadena del `std::string`) cambiados en el segundo bloque
    "merge_%s_loc.model" (el primero sale igual); probado un inline comun para los dos bloques (peor).
  - `vf24` 87.9%: el original lee la caja de cada instancia dos veces (dos copias de min/max, la z
    pasa por `frsp`) y coloca los temporales de las esquinas de otra forma. Reutiliza el patron
    `ProjectBox_unk` de atn/Geometry_unk con matriz 3x4 (`TransformBox_unk`) y el asm aprobado
    `__VecMinMax`.
  - `fn_8049B1F0` 97.3%: el original calcula `i * 4` del hueco de la instancia en cada iteracion (aqui
    se reduce a otra variable de induccion y gasta un registro salvado mas).
  - `vf2C` 99.45%: registros del puntero del nombre y su longitud.
- **Decisiones sin el usuario**:
  - Splits sin cambios (el pool 0x80756B84 ya estaba dentro del split .rodata).
  - Cabecera nueva `include/game/MapPartsLocatorSet.h` (layout completo 0x350, `aligned(8)` como
    MapPartsModel; bitfield 0x340 con 6 flags `int x : 1`).
  - Cabecera nueva `include/game/MapInstance_unk.h`: las declaraciones (`MapLod_unk`, `MapLocator_unk`,
    `MapLocatorShear_unk`, `MapInstance_unk`) salen de `src/game/MapInstance_unk.cpp`, que la incluye
    (`MapLod_unk::m00` pasa a `const char* mName`).
  - `include/gmk/Lemon.h` (vista de `atn::ModelInstance`): `m08` -> `atn::Sphere mBound`, nombres
    `m6C` (flags) y `m7C` (textura de luz); mismos offsets.
  - `include/gmk/SeTable.h` (`atn::FixedString_unk`): anadidos ctor desde `const char*` y
    `Equals_unk(const char*)` (comparacion inline del original).
  - La textura de 0xDC es `atn::Texture_unk`, pero su metodo 0x800D59B8 esta declarado en
    `GimmickUvOwner_unk` (game/GimmickCommon.h): se llama con un cast. Pendiente: mover
    `fn_800D59B8` a `atn::Texture_unk` (renombrar simbolo y actualizar GimmickCommon).
  - El creador `fn_8049A018` queda `extern "C"` (asi lo declara game/MapPartsFactory.cpp).
  - symbols.txt: metodos de MapPartsLocatorSet renombrados a sus nombres C++; `lbl_80756B84` ->
    `@stringBase0` (scope:local, 0x142).
- Hallazgos MWCC:
  - Bucles "todos listos": `ready = ready && f();` da `cmpwi; li 0; beq` como el original
    (`int prev = ready; ready = 0; if (prev && f()) ready = 1;` da `mr.`).
  - `delete p;` sin `if (p)` previo: el `if` explicito anade un `beq` duplicado.
  - Copia de un `atn::Sphere` campo a campo (xy primero) frente al ctor de copia (z primero).
- Pendientes para otras unidades: al renombrar simbolos hay que buscar tambien llamadores
  `extern "C"` en unidades enlazadas (aqui `atn/LH_Motion.cpp` usa `fn_804714A4/B0` = `Loader_unk`).
