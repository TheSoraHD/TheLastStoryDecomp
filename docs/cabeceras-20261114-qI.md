# cabeceras-20261114, cola I: pendientes de cabeceras de esta sesion

Agente I, rama `agent-I`. Referencia `$REF` = `/home/claude/ref-I` (15434 / 18109). Inicio 17:03.

## Resultado por punto

1. `fn_80392D2C` -> `GameCamera::fn_80392D2C(const atn::Camera*, float)` (declarada sin nombres de parametro en
   `include/game/GameCamera.h`, definida como metodo en `game/GameCamera_unk.cpp`). Llamadores:
   `GameEventScript` (`lbl_8087F430->mCamera.fn_80392D2C(...)`), `EnemyQuarkStand` y `LastCocoonImago`
   (`((GameTask*)lbl_8087F430)->mCamera.fn_80392D2C(...)`, como los otros usos de `lbl_8087F430` en ese fuente).
   Quitar los `extern "C"` (con nombres de parametro) renumera `@N` en los tres llamadores: re-emparejados con
   `pdbatch.sh` y luego `lblmap.py` en GameEventScript y LastCocoonImago (rep100 igual). `symbols.txt`:
   `fn_80392D2C` -> `fn_80392D2C__10GameCameraFPCQ23atn6Cameraf`. La funcion sigue en 98,8 %. `ordercheck` de
   GameEventScript ya fallaba antes (`__dt__15GameEventScriptFv`, nota de la cola F).
2. `CostumeEditChara::vf10`: `MenuCommonTask::sInstance->m1EC` / `m1F4` en lugar de la vista
   `CostumeEditCursor_unk* lbl_8087F580` (borrada). Como el original copia los dos pares con `psq_l`/`psq_st`,
   `MenuCommonTask::m1EC`/`m1F4` pasan a ser `atn::Vector2_unk` (antes cuatro `float` m1EC/m1F0/m1F4/m1F8; mismos
   offsets). vf10 sigue en 99,99 % (el diff de siempre), el ctor de MenuCommonTask en 99,6 % (igual). PRECOMMIT OK.
3. `LevelUpResult_unk` / `CharaStatusData_unk`: **no se unifican** (revertido). Probado: (a) un solo tipo
   `CharaStatusData_unk` con constructor declarado (memset, cuerpo en `InventoryExp_unk.h`; CharaStatus.cpp lo
   incluye y el ctor de CharaStatusBase deja de hacer el memset de m1A8): `CharaStatusBase` sigue al 100 %, pero
   `Em203GreaterDaemon::fn_8028D5C8` 78,3 -> 41,1 % (copia `CharaStatusBase status(m1630->mStatus)`: con un
   constructor declarado MWCC copia el miembro campo a campo); (b) el tipo sin constructor y el memset en el ctor
   de `ExpEntry_unk`: `MslMedian3_unk<ExpEntry_unk>` 100 -> roto, los dos `MslSort_unk<ExpEntry_unk>` y
   `Inventory::fn_80449ABC` caen (ItemManager_unk 46 -> 41, GameTask 109 -> 108): el original copia el resultado
   de `ExpEntry_unk` campo a campo (lwz/lfs por miembro), que es lo que da un tipo con constructor; con `int m24[8]`
   en lugar de `unsigned char m24[0x20]` igual. Conclusion: la copia de `CharaStatus::m1A8` (bloque, pares de 8
   bytes en GameTask) y la de `ExpEntry_unk::mResult` (campo a campo) piden un tipo sin constructor y otro con
   constructor: son tipos distintos (o el original tenia dos structs con la misma forma). El parche probado esta
   en `/home/claude/scratch-I/punto3_unificar.diff` (fuera del repo). Sin unificar, `InventoryFinish_unk` no puede
   derivar de `ExpEntry_unk` (ademas, `ExpEntry_unk` esta anidada en `Inventory`, que contiene `mDF30`: haria falta
   anidar tambien `InventoryFinish_unk`); se quedan los casts `*(ExpEntry_unk*)&mDF30`.
4. `BattleCharaTask::fn_800E9110` (98,8 %): sin cambio. 11 variantes, todas pliegan 0xAA0 + 4 en la carga
   (`lwz 0xaa4(r3)`): puntero local al par, puntero/referencia local al `FixedArray_unk`, `CharaStatus*` local,
   indice en local, `begin()[i]`, `operator[]` explicito, helpers `static inline` que reciben el array por
   referencia/puntero o el puntero a los datos, helper que devuelve puntero al par, helper que recibe `this`.
   Pendiente: el original calcula `&mStatus.m2CC` (addi r4, r3, 0xaa0) antes de leer m145C, como un argumento de
   una inline evaluado primero; ninguna forma probada lo reproduce. Ademas quedan diferencias de huecos de pila.

## Decisiones tomadas sin el usuario

- Punto 2: en lugar de una vista nueva, `MenuCommonTask::m1EC/m1F4` como `atn::Vector2_unk` (el original los
  copia por pares con psq).
- Punto 3: no unificar (bajadas), ver arriba.

## Para el port

- `fn_80392D2C(GameCamera*, const atn::Camera*, float)` (`extern "C"`, GameCamera_unk) ->
  `GameCamera::fn_80392D2C(const atn::Camera*, float)`. Unidad del port: GameCamera_unk (la definicion).
  Llamadores fuera del port: GameEventScript, EnemyQuarkStand, LastCocoonImago.

- `MenuCommonTask` (`game/MenuCommonTask.h`): `float m1EC, m1F0, m1F4, m1F8;` -> `atn::Vector2_unk m1EC, m1F4;`
  (mismos offsets; `m1F0` -> `m1EC.y`, `m1F8` -> `m1F4.y`). No es unidad del port; tampoco CostumeEditChara.

## Pendientes

- `fn_800E9110` (ver punto 4).
- `LevelUpResult_unk` y `CharaStatusData_unk` siguen separados; si alguien vuelve a intentarlo, mirar primero la
  copia de `Em203GreaterDaemon::fn_8028D5C8` y de `MslMedian3_unk<ExpEntry_unk>`.

## Hallazgos de una linea

- Un struct con constructor declarado se copia campo a campo (lwz/stw, lfs/stfs por miembro) en la copia
  implicita de quien lo contiene; sin constructor, en bloque (pares de 8 bytes): dos structs de la misma forma con
  copias distintas no son el mismo tipo (LevelUpResult_unk / CharaStatusData_unk).
- Quitar un `extern "C"` con nombres de parametro de un .cpp renumera los `@N` de esa unidad: `pdbatch.sh` y luego
  `lblmap.py` si rep100 empeora.
