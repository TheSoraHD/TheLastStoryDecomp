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

## db/PlayerPriority_unk (commit HASH)
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
