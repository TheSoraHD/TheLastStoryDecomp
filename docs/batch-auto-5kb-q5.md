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

## db/ArrowSet_unk (commit 6298f72)
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

## db/CharaMotionParam_unk (commit e7cfa76)
- Resultado: 3/3 al 100%, matching si.
- NONMATCHING: ninguno.
- **Decisiones sin el usuario**
  - Splits sin cambios.
  - `symbols.txt`: `lbl_8073EB38` -> `sFileNameStr`, `lbl_8073EB58` -> `@stringBase0`, `lbl_8087F1A8` -> `sMoveParams`,
    `lbl_8087F1AC` -> `sMoveCount`, `lbl_8087F1B0` -> `sAttackParams`, `lbl_8087F1B4` -> `sAttackCount`,
    `lbl_80882D90` -> `sFileName`.
  - Tipos inferidos `CharaMoveParam_unk` (8: hash + float) y `CharaAttackParam_unk` (0x58), secciones "@move"/"@attack".
    Este cargador usa `fn_8006BA8C`/`fn_8006BB6C` (carga/libera) en vez del gestor `lbl_8087F518`.
  - `fn_8006B174` declarado `extern "C" std::string fn_8006B174(const std::string&)` (devuelve una copia normalizada del
    nombre; inferido).
- Hallazgos MWCC: nada nuevo (una asignacion a una variable salvada se planifica donde esta en el fuente respecto a la llamada virtual).

## db/EventIdTable_unk (commit aa4e5bf)
- Resultado: 5/7 al 100%, matching no.
- NONMATCHING:
  - `EventIdTable_unk::fn_8020F248` (cargador) 97.86% - solo reparto de registros (el original deja `this` en r24 y el
    cero de size/i en r23, y el 0 de `mFlags` es copia de ese registro); probado mover declaraciones de count, i, entry.
  - `EventIdTable_unk::fn_8020F75C` (id por subcadena del nombre) 99.24% - el original incrementa i antes del offset;
    probados for/while, orden de declaraciones y aritmetica de punteros.
- **Decisiones sin el usuario**
  - Splits sin cambios. Pendiente: el pool original (`@stringBase0` 0x8073F890) incluye las cadenas de error de
    `basic_string`, asi que el fichero real usaba `std::string` en algo que no se ha reconstruido (funcion eliminada o
    inline); con la unidad no enlazada no afecta al build.
  - `symbols.txt`: funciones a metodos de la clase inventada `EventIdTable_unk` (`fn_8020F130__16EventIdTable_unkFv`
    singleton, `fn_8020F248__...Fv` cargador, `fn_8020F71C__...Fi`, `fn_8020F75C__...FPCc`), `fn_8020F190` ->
    `__dt__16EventIdEntry_unkFv`, `fn_8020F1D0` -> `__dt__16EventIdTable_unkFv`, `fn_8020F710` -> `__ct__16EventIdEntry_unkFv`;
    datos: `sFileNameStr`, `@stringBase0`, `sFileName`, `@LOCAL@/@GUARD@fn_8020F130...@sInstance` (0x807C7FA8/0x8087F200),
    `@STRING@Resize_unk__Q23atn30Buffer_unk<16EventIdEntry_unk>Fii(@0)` (0x8087DB3C/0x8087DB38, en el `.sdata` de la unidad).
  - Tipos inferidos: `EventIdEntry_unk` (0x60, nombre `atn::FixedString_unk<0x40>` de `gmk/SeTable.h`, destructor vacio
    inline) y `EventIdTable_unk` (`atn::Buffer_unk<EventIdEntry_unk>`; singleton con static local).
- Hallazgos MWCC
  - `strcmp(a, b) ? 0 : N` da `neg/or/srawi/andc` (`== 0 ? N : 0` da `cntlzw`); un `if/else` que asigna en cada rama a
    un miembro sale como valor en r0 y un solo `stw` cuando es un inline que devuelve el valor.

## db/ChaseParam_unk (commit 8b057fc)
- Resultado: 0/1 al 100%, matching no.
- NONMATCHING: `fn_8020A90C` 97.94% - solo reparto de registros (data/cero y nombre/puntero de configuracion
  intercambiados; el original guarda la parte alta del pool en r29 desde la primera llamada). Probado el orden de
  declaracion de path, size, data y name.
- **Decisiones sin el usuario**
  - Splits sin cambios (la unidad no tiene `.sbss`: escribe en el objeto global `lbl_8087F0A8`).
  - `symbols.txt`: `lbl_8073ECD8` -> `@stringBase0` (el nombre del fichero esta en el pool, no en un array aparte).
  - Tipo inferido `ChaseConfig_unk` (solo los miembros 0x4E0-0x580 que se escriben aqui, nombres de las claves del CSV)
    para `lbl_8087F0A8` (en `game/EnvSeBase.cpp` el mismo simbolo se declara como `EnvSeConfig_unk*`; no se unifica).
  - `lbl_80882E18` (grados a radianes) tiene tamano 8 en `symbols.txt` (float + relleno): sin cambiar.
- Hallazgos MWCC
  - `p->m = f(x)` con `p` global: MWCC carga `p` despues de la llamada; con un accesor inline (`Config()->m = f(x)`) lo
    carga antes y lo guarda en un registro salvado. Con `k * f(x)` el original lo carga despues (asignacion directa).

## db/ChapterList_unk (commit 5fbed0f)
- Resultado: 9/9 al 100%, matching si.
- NONMATCHING: ninguno.
- **Decisiones sin el usuario**
  - Splits sin cambios. La primera funcion (`fn_80207F80`) es la que llama a todos los cargadores `/boot/*.csv`
    (48 llamadas); queda en esta unidad como en el split (podria ser de otro fichero: sin pruebas para cortar).
  - `symbols.txt`: `fn_802085B4` -> `__ct__16ChapterEntry_unkFv`, `lbl_8073EA70` -> `sFileNameStr`,
    `lbl_8073EA88` -> `@stringBase0`, `lbl_8087F190` -> `sEntries`, `lbl_8087F194` -> `sCount`, `lbl_80882D80` -> `sFileName`.
  - Cabecera nueva `include/db/EventIdTable_unk.h` (clases de `db/EventIdTable_unk.cpp`, usada aqui para llamar al
    singleton). `TextTable_unk` se declara localmente (solo `static fn_8020C028`); los cargadores de otras unidades
    `db/` se declaran `extern "C" void fn_XXXXXXXX()`: si otra cola les da nombre con clase, hay que actualizar esta lista.
  - Tipo inferido `ChapterEntry_unk` (0x64, enlazada con mPrev/mNext; ctor inline cuya copia debil es `fn_802085B4`).
- Hallazgos MWCC
  - Comparar `entry->m == x` a traves de un puntero local da `cmpw rCampo, rParam`; con `a[i].m == x` sale
    `cmpw rParam, rCampo` aunque se inviertan los operandos.
  - Una carga que el original sube por encima de un `if` previo (`lwz` antes del primer `bgt`) = valor leido en un
    local antes del `if` (`int v = last->m; if (...) return; if (v <= x)`).

## db/InitSetting_unk (commit 4214155)
- Resultado: 2/2 al 100%, matching si.
- NONMATCHING: ninguno.
- **Decisiones sin el usuario**
  - Splits sin cambios.
  - `symbols.txt`: `lbl_8073F970` -> `sProtectorKinds`, `lbl_8073F984` -> `sWeaponKinds`, `lbl_8073F990` -> `sFileNameStr`,
    `lbl_8073F9A8` -> `@stringBase0`, `lbl_8087F218` -> `sInstance`, `lbl_80882ED8` -> `sFileName`.
  - Tipos inferidos `InitSetting_unk` (0x434), `InitItem_unk`, `InitDrop_unk` (miembro objeto), `InitEquip_unk`.
- Hallazgos MWCC
  - Inicializaciones que el original hace entre la construccion de dos arrays miembro = un miembro objeto con su propio
    ctor (el cuerpo del ctor de la clase iria al final); un `for` de 10 asignaciones constantes se desenrolla entero.
  - `static const char* const t[] = {"qu", ...}`: los literales cortos van a `.sdata2` y el array a `.rodata`.

## db/CharaIdTable_unk (commit 4d70d57)
- Resultado: 14/16 al 100%, matching no.
- NONMATCHING:
  - `fn_80208BE8` (cargador UTF-16) 0% (tamano distinto): el original llama fuera de linea a `fn_802088A4`, `fn_802089C4`,
    `fn_80208A3C`, `fn_80209184` y al ctor de `atn::CsvU`; aqui el presupuesto de inline no se supera y se inlinea todo
    (bastan 3 llamadas mas a `fn_80208A3C` para superarlo: probablemente la reconstruccion de `std::basic_string` tiene
    menos sentencias que la MSL real). El cuerpo es el mismo.
  - `__ct__Q23atn4CsvUFPCwi` (0x80209048, copia debil del ctor de `atn::CsvU`): no se emite porque aqui se inlinea.
- **Decisiones sin el usuario**
  - Splits sin cambios.
  - `symbols.txt`: `fn_80209048` -> `__ct__Q23atn4CsvUFPCwi`; datos `sFileNameStr`, `@stringBase0` (0x8073EAB8),
    `@wstringBase0` (0x8087DB28, `.sdata`, `scope:local data:wstring`), `sEntries`, `sCount`, `sWideTexts`, `sTexts`,
    `sFileName`. (`pairnames --data` empareja mal por el cargador distinto: renombrado a mano.)
  - Tipo inferido `CharaIdEntry_unk` (0x30). `fn_80208814` declarado `extern "C" std::string (const std::basic_string<wchar_t>&)`
    (conversion sin fuente: devuelve siempre ""). `lbl_8087EEC8` es la fuente (`fn_8006F420` convierte UTF-16).
- Hallazgos MWCC
  - Una funcion definida despues de su llamadora tambien se autoinlinea con `-ipa file` (`fn_8020924C` en `fn_802091E8`).
  - Para que un inline reciba el mismo registro que el original a veces hay que pasar `*out` recargado tras
    `*out = buf + pos` en vez de un local (`fn_80208A3C`).

## db/PaletteTable_unk (commit PENDIENTE)
- Resultado: 12/14 al 100%, matching no.
- NONMATCHING:
  - `fn_8020B2BC` (paletas: ids, colores y nombres UTF-16, 4 ficheros) 96.32% - solo reparto de registros (constante 0,
    puntero de datos, direccion del `CsvU` en registro en el original). Probado separar cada bloque en una funcion
    estatica (entonces no se inlinean).
  - `fn_8020BB0C` (presets) 99.41% - el original tiene locales sin usar bajo el path (1.0f en 0x14, 0x28, 0x3c y 0x40 del
    marco, pila 0x7B0): tipo desconocido; aqui un array de 4 colores no usado da otro tamano de pila.
- **Decisiones sin el usuario**
  - Splits sin cambios.
  - `symbols.txt`: `fn_8020BAC0`/`fn_8020BACC` -> `__ct__/__dt__11Palette_unkFv`; datos `sGroupNames`, `sColorNames`,
    `sEffNames`, `sInvNames` (`.data`), `@stringBase0` (0x8073F4A0), `@wstringBase0` (0x8087DB30, `.sdata`), `sPalettes`,
    `sEffPalettes`, `sPresets`, `sPaletteIds`, `sEffPaletteIds`.
  - Tipos inferidos `Palette_unk` (0x54: id, color, nombre `wchar_t[0x20]`), `PaletteSet_unk`/`EffPaletteSet_unk`
    (arrays de 64/32 construidos con `__construct_array`), `PaletteIds_unk`, `PaletteSlots_unk` (7x8).
  - `fn_8020BD78`/`fn_8020BDAC` devuelven `sEffNames[32]` aunque la tabla tiene 32 entradas (31 nombres + ""): lectura
    fuera de rango en el original, reproducida tal cual.
- Hallazgos MWCC
  - `*wfield == L';'` compara con `cmplwi` (wchar_t sin signo); con `';'` sale `cmpwi`.
  - Literales de inicializadores de tablas de punteros: los de > 4 bytes son objetos sueltos en `.rodata` (no van al
    pool), los de <= 4 bytes van a `.sdata2`; un array de punteros no `const` va a `.data`.
  - Un bucle de busqueda de 64/32 iteraciones constantes se desenrolla x8 con `ctr`.
