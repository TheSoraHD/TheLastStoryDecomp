# Lote batch-auto-5kb - cola q8

Cola (de menor a mayor): `game/MapPartsOcean_cont`, `MP/NetGlobals_unk`, `Event/Label`, `game/HeapInfo_unk`,
`MP/CharaListeners_unk`, `game/MapPartsFactory`, `game/PlayerTask`, `Event/EventLoader_unk`,
`game/MapInstance_unk`, `game/PlayerManager`, `Event/Manager_cont`, `Event/Section`, `game/HomeButton_unk`,
`Event/Channel`, `game/GameSystem_unk`, `game/PathData_unk`.

## Arreglo previo de enlace (commit b0c20bf)

La copia recibida (merge de q1/q4/q5/q7) no enlazaba: `undefined fn_800698DC` (desde `atn/LH_Core`) y
`undefined fn_80473F18` (desde `game/BattleEfp_unk`), porque otra cola habia renombrado esos simbolos a
`fn_800698DC__17DebugMessages_unkFPCc` y `fn_80473F18__Q23atn10LoadHandleFv`.
- **Cambio fuera de mis unidades:** `src/atn/LH_Core.cpp` declara un `DebugMessages_unk` minimo y llama a
  `lbl_8087EEB8->fn_800698DC(mPath)`; `src/game/BattleEfp_unk.cpp` llama a `mLoader.fn_80473F18()`
  (miembro de `atn::LoadHandle`). Mismo codigo; ambas siguen al 100%, `main.dol: OK`, lost 0.

## game/MapPartsOcean_cont (commit HASH1)

Fusionada en `game/MapPartsOcean`.

- Resultado: 1/1 al 100% (`__sinit_\MapPartsOcean_cpp`); `game/MapPartsOcean` sigue `matching: true`, `main.dol: OK`.
- **Decisiones sin el usuario**
  - `splits.txt`: `game/MapPartsOcean.cpp` .text 0x8049C8DC-0x8049CDA0 -> 0x8049C8DC-0x8049CDBC y gana
    `.ctors` 0x8072D384-0x8072D388 y `.sbss` 0x8087F560-0x8087F578 (los del `_cont`); borrada la entrada
    `game/MapPartsOcean_cont.cpp`.
  - El `__sinit` solo guarda la direccion de tres estaticos de 4 bytes en otros tres: se reproduce con
    `static int sStatic0..2_unk` y `static StaticRef_unk sStaticRef0..2_unk(&sStaticN_unk)` (tipo inventado
    con constructor; un puntero inicializado con `&x` sale estatico en `.sdata`, sin `__sinit`). El mismo
    `__sinit` exacto esta en `gmk/CharaLight` (0x80423340) y `atn/SequenceHolder` (0x800C41B4): viene de
    alguna cabecera comun (sin identificar).
  - `symbols.txt`: `fn_8049CDA0` -> `__sinit_\MapPartsOcean_cpp`; `lbl_8087F560..574` -> `sStatic0_unk`,
    `sStatic1_unk`, `sStatic2_unk`, `sStaticRef0_unk`, `sStaticRef1_unk`, `sStaticRef2_unk` (`scope:local`).
- Hallazgos MWCC: un objeto estatico sin usar cuyo constructor inline solo guarda un puntero no se elimina y
  genera `__sinit` (`li` x3 + `stw` x3); estaticos POD sin usar si se eliminan.
