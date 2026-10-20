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
