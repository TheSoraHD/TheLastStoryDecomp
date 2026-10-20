# Lote batch-auto-5kb - cola q5

Cola (de menor a mayor): `db/BgmIdTable_unk`, `db/ColliAttrTable_unk`, `db/HateParam_unk`, `db/AiTable_unk`,
`db/ArrowSet_unk`, `db/CharaMotionParam_unk`, `db/EventIdTable_unk`, `db/ChaseParam_unk`, `db/ChapterList_unk`,
`db/InitSetting_unk`, `db/CharaIdTable_unk`, `db/PaletteTable_unk`, `db/CharaParam_unk`, `db/EquipParam_unk`,
`db/ItemRare_unk`.

Patron comun: cada unidad es el cargador de un `/boot/*.csv` (`atn::Csv` en pila, `fn_8046DC5C`/`fn_8046DD20` del gestor
`lbl_8087F518`) mas accesores por id. Funciones `extern "C" fn_<addr>` (sin clase en el binario), como en
`db/GrowthParseGlobal.cpp`.

## db/BgmIdTable_unk (commit 1b6dae5)
- Resultado: 3/3 al 100%, matching si.
- NONMATCHING: ninguno.
- **Decisiones sin el usuario**
  - Splits sin cambios (el split ya cubria nombre de fichero, pool `""`/`"%s"`, `.sbss` y `.sdata2`).
  - `symbols.txt`: `lbl_8073EA50` -> `sFileNameStr`, `lbl_8073EA68` -> `@stringBase0`, `lbl_8087F188` -> `sEntries`,
    `lbl_8087F18C` -> `sCount`, `lbl_80882D70` -> `sFileName`, `lbl_80882D78` -> `sEmpty` (todos `scope:local`).
  - Tipo inferido `BgmIdEntry_unk` (0x28: id, nombre char[0x20], entero en 0x24) local al .cpp.
- Hallazgos MWCC
  - `static const char* const p = "";` pone el literal `""` en `.sdata2` (objeto de 1 byte `@N`), no en `@stringBase0`;
    un `static const char a[] = "";` va a `.sbss2`.

## db/ColliAttrTable_unk (commit d70b3a6)
- Resultado: 4/4 al 100%, matching si.
- NONMATCHING: ninguno.
- **Decisiones sin el usuario**
  - Splits sin cambios.
  - `symbols.txt`: `lbl_8073EEB0` -> `sSeKinds`, `lbl_8073EF10` -> `sEffKinds` (tamano 0x8 en symbols.txt aunque el objeto
    real es `char[1][0x20]`; no lo cambio), `lbl_8073EF30` -> `sFileNameStr`, `lbl_8073EF4C` -> `@stringBase0`,
    `lbl_8087F1C0` -> `sEntries`, `lbl_80882E20` -> `sFileName` (`scope:local`).
  - Tipo inferido `ColliAttrEntry_unk` (0x80: 3 nombres `ATR_*_SE` + 1 `ATR_EFF`, tabla fija de 0x21 filas).
- Hallazgos MWCC
  - `0 > id || id >= N` da dos `cmpwi` con signo (como `0 <= i`); `id < 0 || id >= N` se fusiona en `cmplwi`.

## db/HateParam_unk (commit 6111012)
- Resultado: 3/3 al 100%, matching si.
- NONMATCHING: ninguno.
- **Decisiones sin el usuario**
  - Splits sin cambios.
  - `symbols.txt`: `fn_8021021C` -> `__ct__18HateParamEntry_unkFv`, `lbl_8073F950` -> `sFileNameStr`,
    `lbl_8073F968` -> `@stringBase0`, `lbl_8087F210` -> `sEntries`, `lbl_8087F214` -> `sCount`, `lbl_80882EB0` -> `sFileName`.
  - Tipo inferido `HateParamEntry_unk` (0x6C, 27 campos int/float sin nombre), ctor vacio fuera de linea.
- Hallazgos MWCC: nada nuevo (ctor vacio no inline definido tras su uso => `__construct_new_array` con puntero al ctor).

## db/AiTable_unk (commit 532d3d1)
- Resultado: 2/2 al 100%, matching si.
- NONMATCHING: ninguno.
- **Decisiones sin el usuario**
  - Splits sin cambios.
  - `symbols.txt`: `lbl_8073E800` -> `sFileNameStr`, `lbl_8073E814` -> `@stringBase0`, `lbl_8087F158` -> `sEntries`,
    `lbl_8087F15C` -> `sCount`, `lbl_80882D28` -> `sFileName`.
  - Tipo inferido `AiTableEntry_unk` (0x90). Dos funciones reconstruidas que el original no conserva (`GetType_unk`,
    `GetNumber_unk`: tipo por prefijo "np/em/fr/pc/gn" y numero sin '_' via `std::string`): globales, se inlinean en el
    cargador y el enlazador las elimina; hacen falta para el orden del pool ("np".."gn", "_", "" y luego las cadenas de
    `basic_string`).
- Hallazgos MWCC
  - Literales de una funcion `static inline` entran en el pool al usarse (en el llamador); los de una funcion no inline
    definida antes (aunque se autoinlinee y luego se elimine) entran antes: sirve para reconstruir el orden del pool.
  - Una funcion que devuelve `bool` (o el tipo) inlinada deja un test repetido; asignar el resultado a un miembro y
    probar el miembro (`e->m = f(); if (e->m >= 0)`) da el salto directo.
  - `for (;;) { pos = find(); if (pos == npos) break; ... }` mantiene el test arriba (un `while` lo rota al final).

## db/ArrowSet_unk (commit PENDIENTE)
- Resultado: 3/4 al 100%, matching no.
- NONMATCHING: `fn_80207310` (cargador) 99.21% - camino rapido del `std::vector<int>::push_back` inline: el original carga
  el tamano antes que la capacidad (problema ya conocido de `include/std/vector.h`; probadas 8 formas de la comparacion,
  sin tocar la cabecera: una variable local mas saca `push_back` del autoinline).
- **Decisiones sin el usuario**
  - Splits sin cambios.
  - `symbols.txt`: `fn_802077F8` -> `__ct__17ArrowSetEntry_unkFv`, `fn_80207808` -> `__dt__17ArrowSetEntry_unkFv`,
    `lbl_8073E9C8` -> `sFileNameStr`, `lbl_8073E9DC` -> `@stringBase0`, `lbl_8087F178` -> `sEntries`,
    `lbl_8087F17C` -> `sCount`, `lbl_80882D60` -> `sFileName`.
  - Tipos inferidos `ArrowSetEntry_unk` (id + `atn::Buffer_unk<int>`; ctor/dtor implicitos) e `IntList_unk`
    (derivada vacia de `std::vector<int>`: el destructor del local comprueba `this` tres veces).
  - Las cadenas de `Buffer_unk<int>::Resize_unk` (`lbl_8087D6E8/EC`, `.sdata` compartido) y "Memory allocation
    failure" (`lbl_80775A88`) son de otros ficheros (deduplicadas): no se renombran.
- Hallazgos MWCC
  - Un `std::vector<T>` local cuyo destructor prueba `this` 3 veces = clase derivada de `std::vector` (con
    `std/vector.h` un `std::vector` local da 2).
  - `Buffer_unk::operator[](int)` con indice `unsigned` crea una segunda variable de induccion: indexar `mData` directo.
