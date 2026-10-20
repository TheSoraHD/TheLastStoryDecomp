# Lote batch-auto-5kb - cola q6

Cola (de menor a mayor): `db/TreasureList_unk`, `db/MarketPrice_unk`, `db/StrategyAiTable_unk`, `db/PlayerPriority_unk`,
`db/LevelEquipTable_unk`, `db/VoiceLengthTable_unk`, `db/TutorialTable_unk`, `db/MemberList_unk`,
`db/ReverbPresetParam_unk`, `db/SkillGrowth_unk`, `db/SkillParam_unk`, `db/HairBackControl_unk`, `db/PlayerParam_unk`,
`db/TreasureBoxTable_unk`, `db/ParseUtil_unk`, `db/MapEnvSeTable_unk`, `db/MapIdTable_unk`, `db/RandomItemTable_unk`.

Mismo patron que la cola q5 (`docs/batch-auto-5kb-q5.md`): cargador de `/boot/*.csv` con `atn::Csv` + accesores,
funciones `extern "C" fn_<addr>`.

## db/TreasureList_unk (commit HASH)
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
