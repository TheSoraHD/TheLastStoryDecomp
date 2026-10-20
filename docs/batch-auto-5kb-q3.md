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
