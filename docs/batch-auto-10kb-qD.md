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

## db/ItemParam_unk (commit c701ab6)
- Resultado: 12/17 al 100%, `matching: false`, `main.dol: OK`.
- NONMATCHING:
  - `fn_80211244` (textos UTF-16) 94.95% - bloqueo conocido del ctor inline de `atn::CsvU`.
  - `fn_80211770` (nombre "<nombre>+<nivel>" de un objeto mejorado) 97.28% - numeracion de registros (slot/entry/level).
  - `fn_80211940` (crea el objeto mejorado en un hueco libre) 90.59% - el original copia la parte `BulletParam_unk`
    con bloques (0x14-0x74 en bucle, parejas en 0x90 y 0xB4): sub-agregados que `include/db/ParseUtil_unk.h` no
    tiene (cambiarlo toca ParseUtil/SkillParam/HitInfo: pendiente); mas lo de los dos inline de abajo.
  - `fn_80212840` (arma mejorada) 90.90% y `fn_80212C34` (equipo mejorado) 85.25% - reparto de registros y orden de
    planificacion alrededor de la copia (el original guarda `base` en un quinto registro salvado y calcula el bonus
    antes del id nuevo).
- **Decisiones sin el usuario**
  - Splits sin cambios.
  - Unidad nueva en `game_units.json`.
  - **Cabeceras nuevas** `include/db/WeaponParam_unk.h` y `include/db/EquipParam_unk.h`: se mueven ahi los tipos de las
    filas (`WeaponParamEntry_unk`, `EquipParamEntry_unk` y sus listas/quintetos) y se declaran sus accesores
    `extern "C"`. **Se toca `src/db/EquipParam_unk.cpp`** (unidad de otra cola ya integrada) solo para incluir la cabecera
    en vez de definir los tipos (objeto identico salvo numeracion de etiquetas `@N`; EquipParam sigue 13/14).
  - Cambios de layout en esas cabeceras para que las copias de estructura salgan como en el original: `m70`/`m74` ->
    `int m70[2]` (se copian como pareja) y `operator=` explicito en `EquipParamLists_unk`/`WeaponParamLists_unk` que
    copia cada lista de 7 floats como bloque propio (sin el, MWCC copia las dos listas como un unico bloque; con las
    listas como miembros directos del registro, el ctor cambia de orden). Inventado e indicado con comentario.
  - `symbols.txt`: `fn_8021154C` -> `__ct__18ItemParamEntry_unkFv`; datos `sKinds` (0x8073FA10), `sUsers`,
    `sTextFileNameStr`, `@stringBase0` (0x8073FA60), `@wstringBase0` (0x8087DB40, `.sdata`, `data:wstring`),
    `sEntries`, `sCount`, `sBuf`, `sTexts`, `sNames` (**tamano 0x8 -> 0x4**), `sTextFileName`.
  - Tipo inferido `ItemParamEntry_unk` (0x138, deriva de `BulletParam_unk`; las ultimas 0x100 filas son huecos para
    las mejoras); macros locales `IP_MIN`/`IP_MAX` (el original calcula dos veces el `MAX` dentro del `MIN`).
  - Nombre real propuesto: `db/ItemParam.cpp` (mismo esquema que `db/SkillParam_unk.cpp`).
- Hallazgos MWCC
  - La copia implicita de una estructura con ctor copia por miembros, pero un miembro struct trivialmente copiable
    (aunque tenga ctor propio) se copia como un unico bloque de palabras: dos arrays de 7 floats dentro de un struct
    salen fusionados en parejas; como miembros directos (o con `operator=` propio) sale cada array por separado
    (3 parejas + 1).
  - Una funcion no inline de ~16 sentencias no se autoinlinea; agrupar el calculo en una expresion con macros
    `MIN/MAX` la deja por debajo del umbral y el llamador la inlinea como el original.
  - `p[2] >= L'0' && p[2] <= L'9'` sale como `addis +1`/`subi 0x30`/`clrlwi 16`/`cmplwi 9`.
  - El orden de operandos de `add` de `f() + j` (j unsigned) se consigue con `int n = f(); g(n + j - 0x80);`, y
    `sEntries[sCount - 0x100 + i]` da `add rCount, ri` (no `sCount + i - 0x100`).
  - Asignar `-1` a traves de punteros locales con nombre (`T* p = f(); p->m = -1;`) evita que MWCC guarde la
    constante en un registro salvado.
- Pendientes para otras unidades: completar `BulletParam_unk` (`include/db/ParseUtil_unk.h`) con los sub-agregados
  que delata la copia de `fn_80211940` (0x14-0x73 como bloque, parejas 0x90/0x94 y 0xB4/0xB8).
