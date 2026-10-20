# Lote batch-auto-5kb - cola q7

Cola (de menor a mayor): `game/FriendManager_cont`, `game/BattleCalc_unk`, `game/TargetPos_unk`,
`Event/BehaviorGesture`, `AI/AiUtil_unk`, `game/HomeButtonMenu`, `Event/BehaviorMoveTo`, `game/EnemyTask`,
`game/GameSaveTask`, `game/MenuCommon_unk`, `chr/ControlMoveCursor`, `game/U16Table_unk`,
`game/GameSaveLoadBackgroundCommon`, `game/BattleEfp_unk`, `gmk/Cannon`.

## game/FriendManager_cont (commit 2391d39)

Fusionada en `game/FriendManager`.

- Resultado: 1/1 al 100% (`fn_8036554C__13FriendManagerFv`); `game/FriendManager` sigue `matching: true`, `main.dol: OK`.
- **Decisiones sin el usuario**
  - `splits.txt`: `game/FriendManager.cpp` .text 0x80365464-0x8036554C -> 0x80365464-0x8036555C (absorbe el `_cont`, contiguo y sin datos); borrada la entrada `game/FriendManager_cont.cpp`.
  - La funcion no usa `this`: devuelve `lbl_8087F8A0->mHead->mNext` (segundo jugador de `CharaManager<PlayerTask>`, el primer "amigo" tras el lider); firma existente en `include/game/FriendManager.h` sin cambios.

## game/BattleCalc_unk (commit bfef61e)

- Resultado: 1/1 al 100% (`fn_800E2A24`), `matching: true`.
- **Decisiones sin el usuario**
  - `symbols.txt`: `lbl_808812D0` (1.0f) tamano 0x8 -> 0x4 (con 8 bytes objdiff lo trata como double y da 96%; el relleno lo pone la alineacion).
  - Tipo inferido `BattleCalcParam_unk` (0x98 bytes, local al .cpp): bloque de parametros que BattleTask reinicia en +0x38A5C antes de cada calculo; nombres de campos desconocidos.
  - La funcion se deja `extern "C"` (nombre de symbols.txt sin clase).

## game/TargetPos_unk (commit 50562cb)

- Resultado: 2/2 al 100%, `matching: true`.
- **Decisiones sin el usuario**
  - `symbols.txt`: `fn_8003E918` -> `fn_8003E918__8AiTargetFv`, `fn_8003E950` -> `fn_8003E950__8AiTargetCFv` (las llama `AIManager` sobre sus `AiTarget` en +0x424/+0x458 justo tras poner `__vt__8AiTarget`); `lbl_808807C0` (0.0f) tamano 0x8 -> 0x4 (objdiff lo leia como double).
  - Cabecera nueva `include/game/AiTarget.h` (clase `AiTarget`, 0x34 bytes, layout inferido de estas dos funciones; `AiTargetObj_unk` para el objetivo de tipo 2).
  - Nombre real propuesto del fichero: probablemente parte de `game/AiTarget.cpp` o un `AiTarget.cpp` propio (el split inferido esta lejos de AiTarget en .text, asi que es otro fichero; nombre sin pruebas).

## Event/BehaviorGesture (commit HASH_BG)

- Resultado: 2/2 al 100%, `matching: true`. Splits ya correctos (vtable/RTTI/nombres y 1.0f/0.0f propios).
- **Decisiones sin el usuario**
  - `symbols.txt`: `fn_8010E3F0` -> `vf08__Q25Event15BehaviorGestureFPQ25Event13ActionArg_unk`, `fn_8010E604` -> `vf0C__...ActionArg_unki` (por la vtable); `fn_800119C0` -> `fn_800119C0__Q25Event9Actor_unkFiiiiPviiff` (metodo de `Event::Actor_unk`, r3 = actor; firma inferida de los registros, el 5o argumento se usa como puntero).
  - `include/Event/Behavior.h`: declarado `Actor_unk::fn_800119C0`.
- Hallazgos MWCC: objdiff compara `.sdata2` por posicion, no por valor: con dos floats pasados como argumento (1.0f, 0.0f) en orden cambiado da 100% igualmente; mirar `cmp.py -v` (muestra el valor del literal).
