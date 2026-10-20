# Lote batch-auto-5kb - cola q6

Cola (de menor a mayor): `db/TreasureList_unk`, `db/MarketPrice_unk`, `db/StrategyAiTable_unk`, `db/PlayerPriority_unk`,
`db/LevelEquipTable_unk`, `db/VoiceLengthTable_unk`, `db/TutorialTable_unk`, `db/MemberList_unk`,
`db/ReverbPresetParam_unk`, `db/SkillGrowth_unk`, `db/SkillParam_unk`, `db/HairBackControl_unk`, `db/PlayerParam_unk`,
`db/TreasureBoxTable_unk`, `db/ParseUtil_unk`, `db/MapEnvSeTable_unk`, `db/MapIdTable_unk`, `db/RandomItemTable_unk`.

Mismo patron que la cola q5 (`docs/batch-auto-5kb-q5.md`): cargador de `/boot/*.csv` con `atn::Csv` + accesores,
funciones `extern "C" fn_<addr>`.

## db/TreasureList_unk (commit 16e9845)
- Resultado: 2/2 al 100%, matching si.
- NONMATCHING: ninguno.
- **Decisiones sin el usuario**
  - Splits sin cambios.
  - `symbols.txt`: `lbl_80742740` -> `sFileNameStr`, `lbl_80742758` -> `@stringBase0`, `lbl_8087F338` -> `sEntries`,
    `lbl_8087F33C` -> `sCount`, `lbl_80882FD0` -> `sFileName` (`scope:local`).
  - Tipo inferido `TreasureListEntry_unk` (0x10, 4 enteros; filas cuya primera columna esta vacia).
- Hallazgos MWCC
  - Un indice `i` de bucle (`&sEntries[i]`) se reduce a un desplazamiento inicializado en el preheader (tras la llamada
    previa); si el original pone el `li rX, 0` antes de esa llamada, el desplazamiento es una variable del fuente
    (`(T*)((char*)p + ofs)`, `ofs += sizeof(T)`), declarada antes para el orden de registros.

## db/MarketPrice_unk (commit 3ffbd99)
- Resultado: 4/4 al 100%, matching si.
- NONMATCHING: ninguno.
- **Decisiones sin el usuario**
  - Splits sin cambios.
  - `symbols.txt`: `fn_802178C4` -> `__ct__15MarketPrice_unkFv`, `lbl_8073FC00` -> `sFileNameStr`, `lbl_8073FC18` ->
    `@stringBase0`, `lbl_8087F280` -> `sEntries`, `lbl_80882F28` -> `sFileName`. **Tamano cambiado**: `lbl_80882F30`
    (1.0f) de 0x8 a 0x4 (float + relleno: objdiff lo trataba como `double`).
  - Tipo inferido `MarketPrice_unk` (0xE8 = 58 floats a 100.0f; 10 columnas del CSV, sin cabecera ni comprobacion de carga
    previa); ctor inline cuya copia fuera de linea usa `__construct_new_array`.
- Hallazgos MWCC
  - Bucle con la prueba arriba y el `next()` del CSV abajo = `do { if (row >= N) break; ...; row++; } while (next());`.
  - `0 <= a && a < N && ...` con `return` del valor dentro y el literal al final reproduce `blt/bge` a la rama final.

## db/StrategyAiTable_unk (commit 39b8259)
- Resultado: 3/3 al 100%, matching si.
- NONMATCHING: ninguno.
- **Decisiones sin el usuario**
  - Splits sin cambios. **Propuesta**: la primera funcion (0x8021CC50, destructor `{size, T* data}` con
    `__destroy_new_array` de `fn_8021BE40`) va justo detras del `__sinit` de `db/SpText_unk` (0x8021CB7C, que la
    registra para sus 6 buffers globales): es la copia debil de `atn::Buffer_unk<T>::~Buffer_unk` del fichero de SpText.
    Mover el limite SpText/StrategyAiTable a 0x8021CCC4 toca una unidad ajena: pendiente. Mientras, se escribe aqui
    como clase no plantilla `SpTextBuffer_unk` con destructor no inline (la instanciacion explicita
    `template atn::Buffer_unk<X>::~Buffer_unk();` compila pero MWCC no emite nada).
  - `symbols.txt`: `fn_8021CC50` -> `__dt__16SpTextBuffer_unkFv`, **fuera de la unidad** `fn_8021BE40` (en
    `db/SpText_unk`) -> `__dt__15SpTextEntry_unkFv` (necesario para enlazar la llamada); datos `lbl_80742660` ->
    `sFileNameStr`, `lbl_8074268C` -> `sKinds`, `lbl_80742698` -> `@stringBase0`, `lbl_8087F308` -> `sEntries`,
    `lbl_8087F30C` -> `sCount`, `lbl_80882F98` -> `sFileName`. Las cadenas "ATTACK"/"GUARD" (objetos sueltos) quedan
    como `lbl_`.
  - Tipo inferido `StrategyAiEntry_unk` (0x20: id, tipo "ATTACK"/"GUARD"/2, 6 enteros).
- Hallazgos MWCC
  - El puntero de fila (`entry = &sEntries[i]`) declarado antes de `data` (resultado de la carga) en el mismo bloque
    reparte los registros salvados como el original (data en r31, entry en el mas bajo); declarado en el cuerpo del
    bucle deja `data` en el registro mas bajo.

## db/PlayerPriority_unk (commit 7ec63ad)
- Resultado: 4/4 al 100%, matching si.
- NONMATCHING: ninguno.
- **Decisiones sin el usuario**
  - Splits sin cambios.
  - `symbols.txt`: `lbl_80742370` -> `sProtectorFileNameStr`, `lbl_80742394` -> `sWeaponFileNameStr`, `lbl_807423B8` ->
    `@stringBase0` ("ATK".."AGI", ""), `lbl_8087F2B0` -> `sProtectors`, `lbl_8087F2B4` -> `sWeapons`, `lbl_80882F48` ->
    `sProtectorFileName`, `lbl_80882F4C` -> `sWeaponFileName`.
  - Tipos inferidos `PlayerPriority_unk` (0xF4: id + 30 pares `PlayerPriorityItem_unk` {tipo 1-6, valor}), 7 filas
    por tabla (armas y protectores). `fn_802188B4` recibe `atn::Csv*` (lee el resto de la fila).
- Hallazgos MWCC: nada nuevo (declarar `field` fuera del `for` para que el contador reciba el registro mas bajo).

## db/LevelEquipTable_unk (commit 7739345)
- Resultado: 4/4 al 100%, matching si.
- NONMATCHING: ninguno.
- **Decisiones sin el usuario**
  - Splits sin cambios (el inicio 0x8021414C, corte ambiguo segun `splits-auto-B`, cuadra: la unidad anterior
    `db/ItemRare_unk` ya esta decompilada hasta ahi).
  - `symbols.txt`: `lbl_8073FAB0` -> `sFileNameStr`, `lbl_8073FACC` -> `@stringBase0`, `lbl_8087F240` -> `sEntries`,
    `lbl_8087F244` -> `sCount`, `lbl_80882EF8` -> `sFileName`.
  - Tipo inferido `LevelEquipEntry_unk` (0x24: 2 ids con prefijo de 2 letras, 2x3 enteros, id final). Los accesores
    limitan el indice con macros `MIN(MAX(i, 0), sCount - 1)` (el valor limitado se calcula dos veces).
- Hallazgos MWCC
  - `x = cond ? f() : 0` deja `li r3,0` + un solo `stw`; si el original guarda un 0 de registro salvado en la rama
    `else` (`stw r31`), es `if/else` con una asignacion en cada rama.
  - `if (0 > k || k >= N) return 0;` da `blt fallo; cmpwi; blt ok` (dos `cmpwi` con signo); `k >= 0 && k < N` se fusiona
    en `cmplwi`.

## db/VoiceLengthTable_unk (commit 390b4de)
- Resultado: 2/2 al 100%, matching si.
- NONMATCHING: ninguno.
- **Decisiones sin el usuario**
  - Splits sin cambios.
  - `symbols.txt`: `lbl_807427A8` -> `sFileNameStr`, `lbl_807427C8` -> `@stringBase0` ("" + cadenas de `basic_string`),
    `lbl_8087F348` -> `sEntries`, `lbl_8087F34C` -> `sCount`, `lbl_80882FE0` -> `sFileName`.
  - Tipo inferido `VoiceLengthEntry_unk` (8: hash del nombre + duracion). `fn_8006B53C` declarado
    `extern "C" std::string (const std::string&)` (ruta con el sufijo de idioma "_xx" sustituido; inferido). Incluye
    `std/string_members.h` (el pool lleva las cadenas de error de `basic_string`).
- Hallazgos MWCC: nada nuevo (`std::string path(p); path = f(path);` reproduce el `operator=` inline de `std/string.h`).

## db/TutorialTable_unk (commit 3047ccf)
- Resultado: 5/6 al 100%, matching no.
- NONMATCHING: `fn_8021ED08` (cargador UTF-16) 97.71% - solo registros: el original guarda la direccion del
  `atn::CsvU` en un registro salvado mientras corre su ctor inline (mismo problema que `db/PaletteTable_unk` y
  `db/EquipParam_unk` de q5). Probado (en copia local de `include/atn/Csv.h`) llamar a `vf08` desde otro inline en el
  ctor de `CsvBase`: sin efecto. No se toca la cabecera compartida.
- **Decisiones sin el usuario**
  - Splits sin cambios.
  - `symbols.txt`: `fn_8021F040`/`fn_8021F054` -> `__ct__/__dt__17TutorialEntry_unkFv`; datos `lbl_80742760` ->
    `sFileNameStr`, `lbl_8074277C` -> `sDemoFileNameStr`, `lbl_807427A0` -> `@stringBase0`, `lbl_8087F340` -> `sEntries`,
    `lbl_8087F344` -> `sCount`, `lbl_80882FD8` -> `sFileName`, `lbl_80882FDC` -> `sDemoFileName`.
    (`pairnames --apply` emparejo mal el ctor con `vf08` y `lbl_8077A0A8`/`lbl_8087EEC8` con simbolos de funciones:
    corregido a mano; revisar siempre su salida en unidades con copias debiles.)
  - Tipos inferidos `TutorialEntry_unk` (0xEC: id, entero, `atn::FixedString_unk<0x20>`, `WFixedString_unk<0x40>`
    (wide, nuevo y local al .cpp), entero, `FixedString_unk<0x40>`); `TutorialSettings_unk` (solo `m194`, version demo,
    de `lbl_8087F0A8`). `fn_8006F5E8` declarado `extern "C" void (void* font, std::string*, const wchar_t*)` (convierte
    UTF-16 a `std::string`).
- Hallazgos MWCC: nada nuevo.

## db/MemberList_unk (commit 1b721e2)
- Resultado: 2/3 al 100%, matching no.
- NONMATCHING: `fn_80217D9C` (fila de un nivel) 98.39% - solo registros (`count - 1` y su desplazamiento `* 0x14`
  intercambiados entre r4 y r5); probados locales para el contador, la tabla y el primer nivel, puntero de fila y otros
  ordenes de comparacion. Se autoinlinea en el cargador (definida despues) y ahi si sale igual.
- **Decisiones sin el usuario**
  - Splits sin cambios.
  - `symbols.txt`: `fn_80217D64` -> `__ct__14MemberList_unkFv`; datos `sFileNameStr`, `sCampFileNameStr`, `@stringBase0`
    (0x8073FC54), `sEntries`, `sCount`, `sFileName`, `sCampFileName`.
  - Tipo inferido `MemberList_unk` (0x14: nivel, 7 `bool` de miembros + contador, 7 `bool` de campamento + contador;
    ctor con `memset`). Dos ficheros: `/boot/member_list.csv` y `/boot/member_camp_list.csv`.
- Hallazgos MWCC
  - `a[j] = *f() != 0;` calcula `&a[j]` antes de la llamada (registro salvado); leyendo antes el campo en un local
    (`const char* field = f(); a[j] = *field != 0;`) la direccion se calcula despues, como el original.

## db/ReverbPresetParam_unk (commit 1962d2a)
- Resultado: 3/3 al 100%, matching si.
- NONMATCHING: ninguno.
- **Decisiones sin el usuario**
  - Splits sin cambios.
  - `symbols.txt`: `lbl_80742418` -> `sMapIds`, `lbl_80742460` -> `sOtherIds`, `lbl_807424A8` -> `sFileNameStr`,
    `lbl_80742500` -> `sTypes`, `lbl_8074251C` -> `@stringBase0`, `lbl_8087F2C0` -> `sEntries`, `lbl_8087F2C4` -> `sCount`,
    `lbl_80882F58` -> `sFileName`. Las cadenas "normal".."cave" quedan como `lbl_`.
  - Tipo inferido `ReverbPreset_unk` (0x24: id, tipo por prefijo "normal".."cave" o -1, 7 floats). El cargador solo
    corre si existe el motor de sonido `lbl_8087EE90`, y le pasa cada fila (`fn_80049E54`) tras vaciarlo (`fn_8004A03C`).
    `fn_80219558` traduce un id de mapa a preset (tablas de 18 ids; casos especiales 0x1870E -> 1 y 0x30F7A -> 9).
- Hallazgos MWCC
  - 7 campos seguidos de un struct leidos con la misma llamada no salen como bucle: el original los escribe uno a uno
    (con un array y un `for` MWCC no desenrolla 7 iteraciones con llamadas).
  - Elemento de tabla usado dos veces en el cuerpo de un bucle de busqueda (`t = tabla[i]; strncmp(s, t, strlen(t))`):
    declararlo dentro del cuerpo le da el registro mas bajo (se crea despues del puntero reducido).

## db/SkillGrowth_unk (commit HASH)
- Resultado: 6/6 al 100%, matching si.
- NONMATCHING: ninguno.
- **Decisiones sin el usuario**
  - Splits sin cambios.
  - **Cabecera compartida `include/atn/Csv.h`** (cambios aditivos/equivalentes): nuevo ctor `atn::CsvU()` sin texto
    (separadores `lbl_807830B8`, otra copia de L" \t" que usan tambien `MP/Text`, `db/MapLink_unk`, `db/SpText_unk`;
    declarado `extern "C" wchar_t lbl_807830B8[3]`), y `CsvU::vf08` escrito con `m04 = length != 0 ? text + 1 : text;`
    (el original inlinado aqui usa `beq; addi r0; b; mr r0`). La copia debil `vf08__Q23atn4CsvUFPCwi` (0x8011BB90,
    `game/CampSaveLoad`) sigue al 100% y no baja ninguna funcion en `report.json`.
  - `symbols.txt`: `fn_8021AEF0`/`fn_8021AF10` -> `__ct__/__dt__15SkillGrowth_unkFv` (pairnames emparejaba el dtor con
    el de `ptr_string_base`: corregido a mano); datos `sFileNameStr`, `sTextFileNameStr`, `@stringBase0` (0x807425F8),
    `@wstringBase0` (0x807830F8, `.data`, L"%s"), `sEntries`, `sCount`, `sTexts`, `sFileName`, `sTextFileName`.
  - Tipo inferido `SkillGrowth_unk` (0x18: id, id de habilidad, dos `atn::ptr_string_base<wchar_t,0>` a textos de
    `/boot/skill_growth_text_xx.u16`). `atn::ptr_string_base` se redeclara igual que en
    `atn/ptr_string_base_wchar_t_0.cpp` (no hay cabecera; candidata a `include/atn/`).
- Hallazgos MWCC
  - Segundo caso (tras TreasureList) de fila indexada por desplazamiento en bytes explicito: si el `li rX, 0` del
    indice va antes de una llamada (o de un inline como `vf08`) previa al bucle, no es un indice reducido.
