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

## Decisiones tomadas sin el usuario

## Para el port

- `fn_80392D2C(GameCamera*, const atn::Camera*, float)` (`extern "C"`, GameCamera_unk) ->
  `GameCamera::fn_80392D2C(const atn::Camera*, float)`. Unidad del port: GameCamera_unk (la definicion).
  Llamadores fuera del port: GameEventScript, EnemyQuarkStand, LastCocoonImago.

- `MenuCommonTask` (`game/MenuCommonTask.h`): `float m1EC, m1F0, m1F4, m1F8;` -> `atn::Vector2_unk m1EC, m1F4;`
  (mismos offsets; `m1F0` -> `m1EC.y`, `m1F8` -> `m1F4.y`). No es unidad del port; tampoco CostumeEditChara.

## Pendientes

## Hallazgos de una linea
