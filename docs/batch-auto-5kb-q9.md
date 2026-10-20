# Lote batch-auto-5kb - cola q9

Cola (de menor a mayor): `game/SetConfig_cont`, `game/GameTimer_cont`, `MP/SaveLoad_cont`, `atn/LH_Motion_cont`,
`game/NpcManager_cont`, `atn/Bgm_cont`, `game/MarketTask_cont`. Todas `_cont`: se fusionan en su unidad enlazada.

## game/SetConfig_cont (commit 382ae8c)

- Resultado: 4/4 al 100%, fusionada en `game/SetConfig` (`matching: true`, `main.dol: OK`).
- NONMATCHING: ninguna.
- **Decisiones sin el usuario**
  - Splits: `game/SetConfig` .text 0x8057E83C-0x8057F0F8 -> 0x8057E83C-0x8057F280; borrada la entrada
    `game/SetConfig_cont` (contigua, sin datos). Confirmado que es el mismo fichero: usa los mismos campos
    (0x94 mSoundType, 0x98-0xA0 volumenes, 0xA4) y la misma logica que `Init_unk`.
  - `symbols.txt` (pairnames): `fn_8057F0F8/F154/F198/F1B4` -> metodos de `SetConfig`.
  - `include/game/SetConfig.h`: cuatro metodos nuevos y tipo inferido `SoundSettings_unk` (tipo de sonido + 3
    volumenes; `GameSaveData_unk` 0x803BD79C lo rellena y llama a 0x8057F1B4).
- Hallazgos MWCC: ninguno nuevo.

## game/GameTimer_cont (commit HASH)

- Resultado: 7/7 al 100%, fusionada en `game/GameTimer` (`matching: true`, `main.dol: OK`).
- NONMATCHING: ninguna.
- **Decisiones sin el usuario**
  - Splits: `game/GameTimer` .text 0x803CE4B0-0x803CE688 -> 0x803CE4B0-0x803CE854; borrada `game/GameTimer_cont`
    (contigua, sin datos).
  - `symbols.txt` (pairnames): `fn_803CE688..fn_803CE7E0` -> metodos de `GameTimer` (arrancar, pausar/reanudar,
    parar, sumar, restar, cargar/guardar en las variables de juego 0x385/0x386 via `lbl_8087F430`).
  - `include/game/GameTimer.h`: siete metodos nuevos. Externos locales `fn_803761EC` (getter del valor),
    `fn_80370174`/`fn_80370320` (leer/escribir variable de juego; firmas como en `gmk/AttractHatePoint`).
- Hallazgos MWCC: ninguno nuevo.
