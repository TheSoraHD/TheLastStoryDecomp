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
