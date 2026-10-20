# Lote batch-auto-5kb - cola q4

Cola (de menor a mayor): `atn/LH_Core`, `atn/LH_Handle`, `atn/DwcAllocator_unk`, `atn/THPDraw_unk`,
`atn/PPDATAU`, `atn/FileSystem_unk`, `atn/XsfParser_unk`, `atn/Bezier_unk`, `atn/NetFriend_unk`.

## atn/LH_Core (commit 6ecfb23)

- Resultado: 7/7 al 100%, `matching: true`, `main.dol: OK`.
- NONMATCHING: ninguna.
- **Decisiones sin el usuario**
  - Splits: sin cambios (la unidad no tiene `.rodata`/`.sdata2`; la RTTI de `atn::LoadCore` la emite `LH_ColliTree`).
  - `symbols.txt` (pairnames): `fn_804730E4` -> `fn_804730E4__Q23atn8LoadCoreFv` (quita una referencia; lo usan
    `LoadHandle` y `FileSystem_unk`), `fn_80473184` -> `vf0C__Q23atn8LoadCoreFv` (slot 0x0C de la vtable).
  - `include/atn/LoadCore.h` (compartida): `mState` y `m05` pasan a `volatile` (cargador con hilos: 0x80473104
    relee `mState` tras el store y el ctor intercala el store de la vtable entre los dos `stb`); nuevo
    `fn_804730E4`. El ultimo 0 de la vtable (0x18 bytes) es relleno: las vtables hijas miden 0x14.
  - `include/atn/LoadManager.h`: nuevo miembro `m3EDC` (bandera que pone 0x804730E4 al quedar sin referencias).
  - Nombre real propuesto del fichero: se mantiene `LH_Core` (sin pruebas nuevas).
- Hallazgos MWCC
  - Store de un miembro seguido de su relectura (`stb` + `lbz` en vez de reusar el registro) = miembro `volatile`;
    dos miembros `volatile` dejan el store de la vtable entre sus stores en el ctor.

## atn/LH_Handle (commit 769538a)

- Resultado: 8/9 al 100%, `matching: false` (`main.dol: OK`).
- NONMATCHING: `fn_80473FCC__Q23atn10LoadHandleFPCc` 99.2% - solo reparto de registros (`this` r20 y desplazamiento
  del bucle r19 en el original; aqui al reves). 8 variantes probadas (anotadas en el fuente).
- **Decisiones sin el usuario**
  - Splits: sin cambios (sin `.rodata` propio; RTTI de `LoadHandle` en `LH_Binary`).
  - `symbols.txt` (pairnames): `fn_80473F18`/`fn_80473F34` -> metodos de `LoadHandle` (ruta y hash del recurso),
    `fn_80473FCC` -> `fn_80473FCC__Q23atn10LoadHandleFPCc` (carga y espera: corre el cargador, la lista de
    callbacks `boost::function<void()>` de `lbl_8087EE94` y `OSSleepTicks(1 ms)` mientras esta ocupado).
  - `include/atn/LoadHandle.h`: `operator=` devuelve `void` (0x804741C0 no devuelve `this`); nuevas
    declaraciones `fn_80473F18/F34/FCC`.
  - `include/atn/LoadCore.h`: `GetState_unk()` inline que devuelve `int` (0x80473F50 compara con `cmpwi`).
  - `include/atn/LoadManager.h`: `LoadManager_unk` tiene vtable (0x80473FCC llama al slot 0x10): tres virtuales
    `vf08/vf0C/vf10` declaradas, `m00[0x48]` pasa a `m04[0x44]` (mismos desplazamientos).
  - Tipo inventado `LoadCallbacks_unk` (cuenta en 0x0, array de `boost::function<void()>` en 0x8) para
    `lbl_8087EE94` (tambien lo recorren `atn::AudioDevice` 0x80046228 y `main`).
- Hallazgos MWCC
  - Leer un `u8` a traves de un inline que devuelve `int` da `cmpwi` en vez de `cmplwi` al compararlo.

## atn/DwcAllocator_unk (commit de252cc)

- Resultado: 6/6 al 100%, `matching: true`, `main.dol: OK`.
- NONMATCHING: ninguna.
- **Decisiones sin el usuario**
  - Splits: sin cambios (el split ya cubre `.text`, pool y `.sbss`).
  - `symbols.txt`: `lbl_8075B180` -> `@stringBase0` (size 0x17, `scope:local data:string_table`).
  - Funciones como `extern "C"` sin clase (callbacks de DWC: init 0x8050D3E8, fin 0x8050D484, `DWCAllocEx`/`DWCFreeEx`
    0x8050D500/0x8050D5B8 y otro par alloc/free alineado a 32 0x8050D67C/0x8050D728). El primer parametro
    (nombre/contexto) no se usa.
  - Tipos locales repetidos de `atn/SmallAllocator_8.cpp` (`NamedAllocator_unk` con `vf0C(size, align, heap, name)`,
    alias `extern "C"` de `DLAllocator::fn_80084EE8` con el nombre) y `MemoryManager_unk` minimo (`mUseMutex` 0x1B0).
    Mutex de red `lbl_8087F628` con lock/unlock `fn_80509A88/90`.
  - Nombre real propuesto: sin pruebas (podria ser parte del fichero de `atn/NetManager`).
- Hallazgos MWCC
  - `f()->m = 1;` carga la constante en un registro salvado antes de la llamada; con `T* p = f(); p->m = 1;` sale
    `li r0` despues (como el original).

## atn/THPDraw_unk (commit 0b9fa5f)

- Resultado: 3/3 al 100%, `matching: true`, `main.dol: OK`.
- NONMATCHING: ninguna.
- **Decisiones sin el usuario**
  - Splits y `symbols.txt`: sin cambios.
  - Es `THPDraw.c` del reproductor de ejemplo del SDK de THP (THPGXRestore 0x805B4DF0, THPGXYuv2RgbSetup 0x805B4F08,
    THPGXYuv2RgbDraw 0x805B53CC), adaptado: las texturas se cargan con `fn_800763FC(lbl_8087EEE0, obj, map)` del
    renderer del motor en vez de `GXLoadTexObj`. Funciones `extern "C"` con su nombre `fn_` (nombres del SDK
    propuestos, no aplicados). Declaraciones GX locales en el `.cpp` (no se amplia `atn/GXDecl.h`).
  - Nombre real propuesto del fichero: `THPDraw.c` (o `.cpp`); se mantiene la unidad `atn/THPDraw_unk`.
- Hallazgos MWCC
  - Los colores locales `const GXColor c = {...}` pasados por valor van a `.sdata2` (los de 8 bytes, `GXColorS10`, a
    `.rodata`) y salen antes que los floats del cuerpo aunque se usen despues.

## atn/PPDATAU (commit 2b9d288)

- Resultado: 14/18 al 100%, `matching: false` (`main.dol: OK`).
- NONMATCHING (todas solo por reparto de registros/planificacion, anotadas en el fuente):
  - `PacketPack::fn_8050D7EC` 97.7% (alta de un peer: `id` en r28 y vtable en r31 en el original).
  - `PacketPack::fn_8050DC58` 99.7% (orden de los dos primeros `addi`).
  - `PacketPack::fn_8050DEA8` 99.5% (cuenta de aids y fin de lista intercambian r30/r31).
  - `PacketPack::fn_8050E094` 95.5% (bucle exterior con registros bajos en el original).
- **Decisiones sin el usuario**
  - Splits: sin cambios (el split ya cubre `.text`, `.rodata`, `.data`, `.sdata`; el layout de datos de nuestro
    `.o` coincide: RTTI, vtables R/U/PPDATA, `@STRING@` de `__msl_error` en `.data`, `.sdata` 5/3 + literales).
  - La unidad son metodos de `atn::net::PacketPack` (lista de peers `std::list<PPUser_unk>` en 0x4, `Crypt*` en
    0x10) mas `PPDATA/PPDATAU/PPDATAR` (colas por peer). Nueva cabecera `include/atn/net/PPDATA.h`; ampliada
    `include/atn/net/PacketPack.h` (miembros, `PPUser_unk` inventado de 0x920 bytes con ctor/dtor inline,
    `__Deactivate_unk` inline; `vf10/vf14` devuelven `unsigned char`). `PPDATA` declara los datos antes que las
    virtuales: asi MWCC pone el puntero a vtable al final (0x460) y el objeto mide 0x480.
  - `PacketPack::vf0C` deja de estar definida en la clase: se define `inline` en `MP/CryptMP.cpp` (si no, este
    fichero emite una copia debil). `CryptMP` sube de 8/11 a 9/11 (la copia 0x804AE3C4 ahora casa).
  - `include/MP/FuncMessage.h`: `vf18` devuelve `unsigned int` y `vf1C` pasa a `unsigned int vf1C(const char* data,
    const void* arg)` en las 65 clases (0x8050E094 la llama con dos argumentos y compara con `cmplwi`). Renombrado
    en `symbols.txt` `vf1C__Q22MP18func_enter_settingFv` -> `vf1C__Q22MP18func_enter_settingFPCcPCv`.
  - `include/std/list.h`: nuevo `back()`.
  - `symbols.txt`: pairnames (vf08/vf0C de las tres clases y metodos de `PacketPack`) y a mano
    `fn_8050D7EC/DC58/E094/E510/E530/E598` -> `...__Q33atn3net10PacketPackF...`; `lbl_8075B1D4` -> `@stringBase0`
    (0x22). Se dejan `extern "C"` `fn_8050E410` (lo usa `MP/CryptMP.cpp` con ese nombre) y `fn_8050E618`.
  - Los literales de la reserva del bufer de trabajo compartido (`lbl_8087F600`) son de `atn/NetManager`
    (`lbl_8087E1B0/1AC`): se referencian como `extern` en vez de literales para no emitir copias.
  - Nombre real propuesto: sin pruebas; la unidad podria ser el final de `atn/PacketPack.cpp` (`PacketPack.cpp`)
    o un `PPDATA.cpp` propio (los nombres RTTI `atn::net::PPDATA*` son de este fichero).
- Hallazgos MWCC
  - Data miembro declarado antes que las virtuales en una clase sin bases => vptr al final de la clase.
  - Literales `""` de una funcion inline van a `.sdata` como objetos debiles `@STRING@<func>` (uno por literal, sin
    fusionar entre inlines); en direcciones crecientes en orden inverso de uso.
  - Iterar con `iterator end = l.end();` local da `addi r3, r3, 8` (fin en r3) como el original; con
    `it != l.end()` en la condicion sale en r0.
  - Un indice `a[n++] = x` sobre un array local (en vez de un puntero que avanza) crea la variable de induccion
    tarde y le da el registro mas bajo.
  - Las copias debiles no referenciadas (dtor de `PPUser_unk`, holder de `std::list`) las quita el enlazador.

## atn/FileSystem_unk (commit HASH)

- Resultado: 11/22 al 100%, `matching: false` (`main.dol: OK`).
- NONMATCHING (detalle en el fuente):
  - `LoadManager_unk::fn_8046F460` 98.3%, `fn_8046F4D4` 91.8%, `fn_8046F544` 98.8%, `fn_8046F8B8` 93.2%,
    `fn_8046F96C` 97.6% (reparto de registros), `fn_8046F5CC` 95.0% (insercion ordenada en la cache: una recarga).
  - `PfsArchive_unk::fn_8046FD7C` 92.3% (listado de directorio con `std::string`; registros y el pool empieza por
    un `"pfs"` que ninguna funcion usa).
  - `PfsArchive_unk::fn_8046FD2C` 35.8%, `ResRef_unk::fn_80470364` 34.8%, `fn_8047043C` 32.4%, `fn_8047054C` 0%:
    el original llama fuera de linea a funciones que aqui se inlinean porque parecen de **otro fichero**.
- **Decisiones sin el usuario**
  - **Pendiente (corte de unidad):** 0x80470364-0x80470580 (`ResRef_unk::fn_80470364/8047043C/80470528/8047054C`
    y `fn_80470564`) parece un fichero propio entre `FileSystem_unk` y `LH_Binary`: llama fuera de linea a
    funciones de este fichero que se autoinlinearian y su cadena `"%sh"` (0x80755BF0) es un objeto `.rodata` aparte
    tras el pool. Haria falta una unidad nueva (p. ej. `atn/ResRef_unk`, `.text` 0x80470364-0x80470580,
    `.rodata` 0x80755BF0-0x80755BF8); no esta permitido crearla en este lote.
  - Tipos inventados: `PfsArchive_unk` (archivo pfs: indice + datos, entradas de 16 bytes ordenadas por hash;
    ctor 0x8046FA18), `PathName_unk` (ruta fija de 0x40 con ctor que pone `[0] = 0` y `operator=` que copia solo si
    no es el mismo bufer: con eso el ctor casa), `PfsEntry_unk`, `PfsDir_unk`, `PfsTable_unk`, `PfsHandle_unk`,
    `FileListOut_unk` (salida de `fn_8006BD78`). `ResRef_unk` local (ya existe en `game/CampSaveLoad.h` y
    `atn/LH_MotionSet.cpp` con otros metodos).
  - `include/atn/LoadManager.h`: `LoadCache_unk` (cuenta + 4000 `LoadCore*` ordenados; `size/operator[]/begin/
    push_back/insert` inline) sustituye a `m48[]` (mismos desplazamientos; `Find_unk` pasa `&m48`); nuevas listas
    `mArchives` (0x4020) y `mMounted` (0x402C) y metodos `fn_8046F460/F4D4/F544/F8B8/F96C`.
  - `include/atn/LoadCore.h`: `vf10(char* out)` (hace `sprintf(out, "size=%d", m50)`); renombrado en
    `symbols.txt` `vf10__Q23atn8LoadCoreFv` -> `vf10__Q23atn8LoadCoreFPc` (las vtables de los `LH_*` enlazados
    siguen casando). El formato es un `static char[]` (esta en `.data`).
  - `symbols.txt`: metodos de `LoadManager_unk`/`PfsArchive_unk`/`ResRef_unk` (a mano; `fn_8046FBB0` y
    `fn_8046FD1C` son `const`), `lbl_80755B88` -> `@stringBase0` (0x66).
  - Nombre real propuesto: sin pruebas (podria ser `Pfs.cpp`/`FileSystem.cpp`; el principio del tramo,
    0x8046F460-0x8046F9FC, son metodos de `LoadManager_unk` y quiza sean el final de `atn/LoadManager_unk`).
- Hallazgos MWCC
  - Un metodo que carga `this->mX` antes de guardar r31 casa al declararlo `const` (confirmado:
    `fn_8046FBB0`); ademas una relectura de `mEntries` tras un bucle sale de un accesor (`fn_8046FD1C(i)->m`).
  - Un indice `(n - 1)` sin plegar en el desplazamiento (`slwi`+`stwx` en vez de `stw -0x10`) sale pasando el
    indice por un accesor inline `At(int i)`.
  - Una busqueda binaria que relee `a[mid]` en cada rama casa con un `operator[]` inline sobre una referencia
    (`T& c = *(T*)p; c[mid]`).
  - `tolower` de MSL (`<ctype.h>`) da el patron `li 1 / li 0 / cmpwi` de la conversion a minusculas.
