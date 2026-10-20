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

## Event/BehaviorGesture (commit f9c5123)

- Resultado: 2/2 al 100%, `matching: true`. Splits ya correctos (vtable/RTTI/nombres y 1.0f/0.0f propios).
- **Decisiones sin el usuario**
  - `symbols.txt`: `fn_8010E3F0` -> `vf08__Q25Event15BehaviorGestureFPQ25Event13ActionArg_unk`, `fn_8010E604` -> `vf0C__...ActionArg_unki` (por la vtable); `fn_800119C0` -> `fn_800119C0__Q25Event9Actor_unkFiiiiPviiff` (metodo de `Event::Actor_unk`, r3 = actor; firma inferida de los registros, el 5o argumento se usa como puntero).
  - `include/Event/Behavior.h`: declarado `Actor_unk::fn_800119C0`.
- Hallazgos MWCC: objdiff compara `.sdata2` por posicion, no por valor: con dos floats pasados como argumento (1.0f, 0.0f) en orden cambiado da 100% igualmente; mirar `cmp.py -v` (muestra el valor del literal).

## AI/AiUtil_unk (commit 4798db2)

- Resultado: 4/4 al 100%, `matching: true`.
- **Decisiones sin el usuario**
  - `splits.txt`: anadida `.sbss 0x8087EE60-0x8087EE68` (`lbl_8087EE60`/`lbl_8087EE64`, antes en `auto_10_8087EE60_sbss`): los escriben solo los setters `fn_800142C4`/`fn_800142CC` de esta unidad y los lee `AI/AiCommon_unk`; se definen aqui como globales `void*` (tipo desconocido). `lbl_8087EE68` (lo usan AiCommon/AI_archer/Em006) se queda en `auto_`.
  - `symbols.txt`: `lbl_808806B0` (60.0f) tamano 0x8 -> 0x4.
  - Tipo local `AiSettings_unk` para `lbl_8087F0A8` (solo `mA4`). `CharaStatus::m14` se lee con cast a `unsigned` (el original usa `cmplwi`); no se cambia la cabecera.
- Hallazgos MWCC: un clamp que reevalua la llamada en cada rama (`f()` llamada hasta 3 veces) sale de macros `MIN(MAX(e, 0), 1)` sin locales.

## game/HomeButtonMenu (commit 303f02c)

- Resultado: 4/5 al 100%, `matching: false`.
- NONMATCHING: `fn_80440994__14HomeButtonMenuFv` 83.4% - planificacion del calculo de la relacion de pantalla (el original copia el tamano 64 con `fmr` y calcula `sy` tras `k * sx`; mismo problema que `StrapTask::vf14`). Con un helper inline que recibe el tamano, la funcion pasa a inlinearse en `fn_8044073C`.
- Pendiente para `matching: true`: el .o emite la RTTI debil de `atn::IRender` (cadena "atn::IRender" en .rodata antes del pool), que el original no tiene aqui; comprobar al enlazar si el enlazador la descarta.
- **Decisiones sin el usuario**
  - `symbols.txt`: `fn_804405F8` -> `fn_804405F8__14HomeButtonMenuFPv` (crea el singleton), `fn_804406C4` -> `__dt__14HomeButtonMenuFv`, `fn_8044073C` -> `fn_8044073C__14HomeButtonMenuFv`, `fn_80440990` -> `vf10__14HomeButtonMenuFPv`, `fn_80440994` -> `fn_80440994__14HomeButtonMenuFv`, `lbl_8087F4E8` -> `sInstance__14HomeButtonMenu`, `lbl_807544E0` -> `@stringBase0` (tamano 0x20 -> 0x1E, `scope:local data:string_table`).
  - Clase inferida `HomeButtonMenu : atn::IRender` (0xD4 bytes; `IRender` declarada localmente como en otras unidades). El constructor no es `inline` (su literal va en `@stringBase0`) y se define despues de `fn_804405F8` para que el pool empiece por `""`.
  - `game/HomeButton_unk` (siguiente unidad, 0x80440B24-0x80441590) contiene las funciones HBM que usa esta clase (`fn_80440F64`, `fn_804410C0`, `fn_80441170`, `fn_804411DC`, `fn_804413D4`); sigue como unidad aparte (1.0f propio).

## Event/BehaviorMoveTo (commit HASH_MT)

- Resultado: 1/2 al 100%, `matching: false`.
- NONMATCHING: `vf0C__Q25Event14BehaviorMoveToFPQ25Event13ActionArg_unki` 97.8% - en `case 6` el desplazamiento y el puntero de la busqueda inline del actor intercambian r4/r5 (mismo problema que `BehaviorFollow::vf0C`; probadas 7 variantes).
- **Decisiones sin el usuario**
  - `symbols.txt`: `fn_8010E60C` -> `vf0C__Q25Event14BehaviorMoveToFPQ25Event13ActionArg_unki`, `fn_8010E7B8` -> `vf08__Q25Event14BehaviorMoveToFPQ25Event13ActionArg_unk` (por la vtable; mismo orden de fuente que BehaviorFollow).
