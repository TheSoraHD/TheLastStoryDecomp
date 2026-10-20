# batch-auto-10kb qD: cargadores db/ (tablas /boot y cosedit)

Cola (de menor a mayor direccion): `db/WeaponParam_unk`, `db/OptionParts_unk`, `db/ItemParam_unk`, `db/MapLink_unk`,
`db/SpText_unk`.

Patron comun: como las colas q5/q6 del lote anterior (`docs/batch-auto-5kb-q5.md`, `-q6.md`): cargador de un CSV con
`atn::Csv` en pila + accesores `extern "C" fn_<addr>`.

## db/WeaponParam_unk (commit f5bdba9)
- Resultado: 14/16 al 100%, `matching: false` (no enlazada), `main.dol: OK`.
- NONMATCHING:
  - `fn_8020689C` (nombres/textos UTF-16) 96.22% - bloqueo conocido del ctor inline de `atn::CsvU` (direccion del csv
    en registro salvado), igual que `db/EquipParam_unk`.
  - copia debil `__pp<token_iterator...>` (0x80205BE8, `it++` del `boost::tokenizer`) 99.23% - solo numeracion de
    registros dentro de `boost::char_separator::operator()` (libs/boost); no se toca la libreria. El resto de copias
    debiles (vf08, tokenizer, basic_string) son de otros ficheros.
- **Decisiones sin el usuario**
  - Splits sin cambios (el split cubre `.rodata` con las dos tablas, los tres nombres de fichero y el pool, `.sbss` y
    `.sdata2`).
  - Unidad nueva en `game_units.json` (`db/WeaponParam_unk.cpp`, antes de EquipParam).
  - `symbols.txt`: `fn_80205A78`/`fn_8020685C` -> `__ct__/__dt__20WeaponParamEntry_unkFv`, `fn_80207250`/`fn_8020726C`
    -> `__ct__/__dt__16WeaponEffect_unkFv`; datos `sRanks` (0x8073E880), `sFileNameStr`, `sTextFileNameStr`, `sTypes`
    (0x8073E900), `sEffectFileNameStr`, `@stringBase0` (0x8073E938), `sEntries`, `sTexts`, `sCount`, `sEffects`,
    `sEffectCount` (**tamano 0x8 -> 0x4**, lo que sigue es relleno de `.sbss`), `sFileName`, `sTextFileName`,
    `sEffectFileName` (todos `scope:local`).
  - Tipos inferidos: `WeaponParamEntry_unk` (0x130, mismo prefijo 0x00-0x78 que `EquipParamEntry_unk`; `mFlags` con un
    bit por letra de la columna de usuarios y 0x8000..0x200000 para "elza".."jackal"), `WeaponEffect_unk` (0x20),
    `WeaponParty_unk` (solo el id de arma de los miembros en `lbl_8087F4F0` + 0x82B0), helper inline `ParseId_unk`
    (prefijo "ws"/"wg"/otro + numero) y `FindName_unk` (indice en tabla o 0).
  - Nombre real propuesto: `db/WeaponParam.cpp` (lee `/boot/weapon_param.csv`, `weapon_param_text_xx.u16` y
    `weapon_effect.csv`).
- Hallazgos MWCC
  - Un `if/else` que asigna constantes a un campo de un struct local se convierte en select sin saltos
    (`cntlzw/extrwi/neg/addi`); escrito a traves de un inline que recibe punteros (`*kind = 0;`) quedan los saltos y las
    constantes en registros salvados como en el original.
  - Destructor inline vacio declarado en la clase y definido (`inline X::~X() {}`) tras la funcion que lo usa: su copia
    debil sale despues de las copias debiles de plantillas que esa funcion instancia (ctor, `__pp<...>`, dtor).
  - Pasar los argumentos de una llamada inline por locales (`int kind = ...; int id = ...; return f(kind, id);`) cambia
    el orden de carga de los globales dentro del inline (sCount antes que sEntries).
- Pendientes para otras unidades: ninguno.

## db/OptionParts_unk (commit ce42b1a)
- Resultado: 12/16 al 100%, `matching: false`, `main.dol: OK`.
- NONMATCHING:
  - `fn_8020C5D4` (lector de `database/cosedit/option/*.csv` sobre `OptionParts_unk`) 99.26% - numeracion de registros
    en las busquedas de "hide"/"base_hide" (el original da registros distintos a las dos ramas identicas) y operandos
    del `std::equal` inline.
  - `fn_8020CC64` (texturas tintadas por grupo con `atn::CompositeIntensityTintMulti`) 99.61% - numeracion de
    registros de las busquedas de grupo/capa (~12 variantes probadas).
  - `fn_8020D688` (cargador de `option_parts.csv` + nombres UTF-16) 97.57% - bloqueo conocido del ctor inline de
    `atn::CsvU` + registros de la busqueda del slot.
  - `fn_8020DE0C` (nivel de las partes desde el nombre del modelo) 99.79% - el original incrementa el puntero de
    niveles antes que el contador (con `*p++` salen los registros intercambiados).
- **Decisiones sin el usuario**
  - Splits sin cambios. El hueco `.sbss` 0x8087F1E0-0x8087F1E8 es de `atn/ptr_string_base_wchar_t_0` (TextTable) y
    `MP/D2AMpMenu`, no de esta unidad.
  - Unidad nueva en `game_units.json`.
  - `symbols.txt`: `fn_8020D4E8`/`fn_8020D5A0` -> `__ct__/__dt__15OptionGroup_unkFv`, `fn_8020D548`/`fn_8020D560` ->
    `__ct__/__dt__15OptionLayer_unkFv`, `fn_8020DA08`/`fn_8020DAB4` -> `__ct__/__dt__19OptionLevelList_unkFv`,
    `fn_8020DA68`/`fn_8020DA74` -> `__ct__/__dt__15OptionLevel_unkFv`; datos `sSlots` (0x8073F600), `sLetters`,
    `sColorSlots`, `sSlotNames`, `@stringBase0` (0x8073F678), `sLevels` (0x8087F1E8), `sSets` (0x8087F1EC).
  - Tipos inferidos (locales al .cpp): `OptionParts_unk` (0x10E0: 16 partes, 24 colores, 8 "hide", 4 "base_hide"),
    `OptionPart_unk`, `OptionColor_unk`, `OptionHide_unk`, `OptionColorOut_unk` (0x14; el original lee 4 floats desde
    +0x8, el cuarto es la palabra 0 del elemento siguiente: reproducido con un cast), `OptionGroup_unk`/`OptionLayer_unk`
    (array local de 32 grupos), `OptionLevel_unk`/`OptionLevelList_unk`/`OptionLevelTable_unk` (11 letras x 4 slots),
    `OptionSetTable_unk` (bool[10][11][4][16]), `WFixedString_unk`. `atn::CompositeIntensityTintMulti` y
    `CompositeLayerDesc_unk` se redeclaran en minimo (estan definidos en `src/atn/CompositeIntensityTintMulti.cpp`, sin
    cabecera); `Model_unk` minimo con `fn_8008B130`. Externos nuevos declarados: `fn_8006AFF8` (union de rutas),
    `fn_80095750` (pone el compositor en el modelo).
  - Tablas sin uso `sRanks` {"－","S","A","B","C"} y `sKinds` {"qu",..,"qs"} anadidas antes de `sSlots`: el enlazador
    las elimina pero fijan el orden de `.sdata2` del original ("S","A","B","qs","up",...). Inferidas por el orden.
  - Nombre real propuesto: `cosedit/OptionParts.cpp` (editor de trajes).
- Hallazgos MWCC
  - Una tabla `static const char* const t[]` sin usar no se emite, pero sus literales que tambien usa otra tabla
    posterior salen en `.sdata2` en el orden de la tabla sin usar: explica ordenes de `.sdata2` "imposibles".
  - `p[row]` con `bool* p = t->m[a][b][c];` reproduce la reasociacion del original (`stbx` con la fila aparte);
    `t->m[a][b][c][row]` la pliega distinto.
  - `group = groups + count; count++;` (unsigned) da puntero inductivo; `&groups[count++]` da un desplazamiento.
  - Dos `""` sin fusionar en el pool tras otro `""` ya usado = `strcmp(x, "\0")`.
- Pendientes para otras unidades: `src/atn/CompositeIntensityTintMulti.cpp` podria exportar su clase a una cabecera.
