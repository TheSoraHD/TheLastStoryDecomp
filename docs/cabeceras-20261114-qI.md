# cabeceras-20261114, cola I: pendientes de cabeceras de esta sesion

Agente I, rama `agent-I`. Referencia `$REF` = `/home/claude/ref-I` (15434 / 18109). Inicio 17:03.

## Resultado por punto

1. `fn_80392D2C` -> `GameCamera::fn_80392D2C(const atn::Camera*, float)` (declarada sin nombres de parametro en
   `include/game/GameCamera.h`, definida como metodo en `game/GameCamera_unk.cpp`). Llamadores:
   `GameEventScript` (`lbl_8087F430->mCamera.fn_80392D2C(...)`), `EnemyQuarkStand` y `LastCocoonImago`
   (`((GameTask*)lbl_8087F430)->mCamera.fn_80392D2C(...)`, como los otros usos de `lbl_8087F430` en ese fuente).
   Quitar los `extern "C"` (con nombres de parametro) renumera `@N` en los tres llamadores: re-emparejados con
   `pdbatch.sh`. `symbols.txt`: `fn_80392D2C` -> `fn_80392D2C__10GameCameraFPCQ23atn6Cameraf`.

## Decisiones tomadas sin el usuario

## Para el port

- `fn_80392D2C(GameCamera*, const atn::Camera*, float)` (`extern "C"`, GameCamera_unk) ->
  `GameCamera::fn_80392D2C(const atn::Camera*, float)`. Unidad del port: GameCamera_unk (la definicion).
  Llamadores fuera del port: GameEventScript, EnemyQuarkStand, LastCocoonImago.

## Pendientes

## Hallazgos de una linea
