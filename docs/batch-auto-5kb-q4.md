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

## atn/DwcAllocator_unk (commit HASH)

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
