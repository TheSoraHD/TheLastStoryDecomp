# Lote batch-auto-5kb - cola q3

Cola (de menor a mayor): `atn/EffectManager_cont`, `atn/D2A_ParamTable_unk`, `atn/EfpHandle_unk`,
`atn/NumberFormat_unk`, `atn/Util_unk`, `atn/D2A_MessageTable_unk`, `atn/EffectSequencer_unk`, `atn/EffectRes_unk`,
`atn/EffectLoader_unk`, `atn/LoadManager_unk`.

## Arreglo previo fuera de la cola (commit d0c4289)

- La copia integrada (q1+q4+q5+q7+q9) no compilaba: `src/MP/SaveLoad.cpp` (q9) declaraba `fn_8004B20C` con otro tipo
  que `include/game/GameSaveLoadBackgroundCommon.h` (otra cola) y llamaba a `fn_803BE8C0` como `extern "C"`, que la
  otra cola habia renombrado a `GameSaveSlot_unk::fn_803BE8C0`. Arreglo: `typedef SaveTime_unk CalendarTime_unk` y
  llamada como metodo sobre la cabecera de 0xB8 de `SaveWork_unk`. `MP/SaveLoad` sigue 8/8 al 100%.

## atn/EffectManager_cont (commit bc7b982)

- Resultado: 2/2 al 100%, unidad propia `matching: true` (no fusionada). `main.dol: OK`.
- NONMATCHING: ninguna.
- **Decisiones sin el usuario**
  - No se fusiona con `atn/EffectManager`: entre ambas esta `std/char_traits_char_8023C080` (0x8023B9D0-0x8024372C,
    unidad ajena en `game_units.json`), que segun `splits-auto-B` es parte del mismo fichero real. Ademas
    `atn/EffectManager` esta en `matching: false` (no esta enlazada). **Pendiente para el usuario**: fusionar las
    tres unidades en `atn/EffectManager` (0x80239798-0x80243758) cuando esa unidad y la parte std esten al 100%.
  - Las dos funciones son los thunks de ajuste de `this` de `EffectManager` para su base `IRender` en 0x48
    (`@72@vf10__Q23atn13EffectManagerFP10Camera_unk` y `@72@__dt__Q23atn13EffectManagerFv`; nuestro
    `EffectManager.o` los emite debiles al final). Como unidad aparte se escriben como llamadas de cola normales
    (`extern "C"`, nombres `fn_8024372C`/`fn_80243734` sin cambiar en `symbols.txt`); al fusionar habra que borrarlas.
  - Splits: `atn/EffectManager_cont` .text 0x8024372C-0x80243758 -> 0x8024372C-0x8024373C; los 0x1C bytes de
    `EfpData_unk::EfpData_unk()` (0x8024373C) pasan a `atn/EffectSequencer_unk` (0x80243758 -> 0x8024373C): los
    thunks cierran el fichero de EffectManager y `EfpData_unk` tiene mas metodos dentro de EffectSequencer
    (`fn_802437B4__11EfpData_unkFPCc`).
  - `game_units.json`: entrada nueva `atn/EffectManager_cont.cpp` tras `std/char_traits_char_8023C080.cpp`.
- Hallazgos MWCC
  - MWCC convierte en salto de cola (`addi r3,r3,-0x48; b f`) una funcion que solo llama a otra con el puntero
    ajustado: sirve para reproducir thunks `@N@` fuera de su fichero. Un parametro `short` reenviado anade `extsh`:
    usar `int`.

## atn/D2A_ParamTable_unk (commit d6e5655)

- Resultado: 6/6 al 100%, `matching: true`, `main.dol: OK`.
- NONMATCHING: ninguna.
- **Decisiones sin el usuario**
  - Splits: sin cambios (el tramo ya cubre `.text`, el double int->float de `.rodata` y su `.sdata2`).
  - `symbols.txt`: `lbl_80882C28` (0.0f) pasa de `size:0x8` a `size:0x4` (los 4 bytes siguientes son relleno;
    con 8 objdiff lo trataba como double y no casaba el `lfs`).
  - Contenido: tablas `{clave, valor}` de los elementos D2A con setter (busca la clave exacta) y getter interpolado
    linealmente entre las claves vecinas, para `int` (`fn_801F0364/03AC`), `float` (`D2A_ParamTable::fn_801F04FC`,
    `fn_801F0544`) y `std::wstring` (`fn_801F0670/0758`, sin interpolar: valor de la clave anterior). Tipos
    inferidos locales `D2A_IntParamTable_unk` y `D2A_StringParamTable_unk`. Los nombres `extern "C"` se mantienen
    (los llaman asi D2A_Common, D2A_Message, D2A_Data, D2A_Texture y MenuCommonTask); `fn_801F04FC` conserva su
    firma `(int key, int arg, float value)` aunque por los llamadores parece `(key, value, flag)` (mismos registros).
  - **Cabecera compartida `include/std/string.h`**: `operator=` copia dos cadenas cortas con `mRaw = str.mRaw` en vez
    de `mLong = str.mLong` (el original carga la palabra 1 antes de guardar la 0, como el ctor copia). Efecto en el
    resto (report.json): suben `db/CharaParam_unk` tokenizer::assign (95.9 -> 99.7) y `LibraryTask`
    `BookTask::fn_8046B798` (96.2 -> 97.0); bajan un poco `D2A_Message` ctor (66.54 -> 66.31) y `vf18`
    (91.59 -> 91.58), ambas ya NONMATCHING. Ninguna funcion al 100% se pierde.
  - Nombre real propuesto: entre `atn_d2a_frame` y `atn_d2a_line` (ver splits-auto-B), p. ej. `atn_d2a_param.cpp`.
- Hallazgos MWCC
  - Locales de un bucle de busqueda (`prev`, `next`, `found`): el orden de declaracion decide r7/r8/r9; probar las
    6 permutaciones con un script es mas rapido que razonarlo.
  - Temporales (tags de iterador) en orden ascendente en la pila = el codigo esta en una inline (aqui la seleccion
    del valor anterior/siguiente): sacarlo a un helper inline los reordena.
