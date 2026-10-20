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

## db/HateParam_unk (commit PENDIENTE)
- Resultado: 3/3 al 100%, matching si.
- NONMATCHING: ninguno.
- **Decisiones sin el usuario**
  - Splits sin cambios.
  - `symbols.txt`: `fn_8021021C` -> `__ct__18HateParamEntry_unkFv`, `lbl_8073F950` -> `sFileNameStr`,
    `lbl_8073F968` -> `@stringBase0`, `lbl_8087F210` -> `sEntries`, `lbl_8087F214` -> `sCount`, `lbl_80882EB0` -> `sFileName`.
  - Tipo inferido `HateParamEntry_unk` (0x6C, 27 campos int/float sin nombre), ctor vacio fuera de linea.
- Hallazgos MWCC: nada nuevo (ctor vacio no inline definido tras su uso => `__construct_new_array` con puntero al ctor).
