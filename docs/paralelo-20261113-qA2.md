# Cola A2 (paralelo-20261113): segunda pasada de Batalla

Inicio: 2026-11-13 06:59 CET. Rama `agent-A2`. Unidades: `game/BattleTask`, `game/TacView`, `game/BattleCharaTask`.
Funciones al 0 % al empezar: BattleTask 29, TacView 6, BattleCharaTask 1.

## Resultado por unidad

- `game/BattleTask`:
  - `fn_8010D320` (direccion en pantalla hacia una posicion, para GameUI; `extern "C"` con `BattleTask*` como el
    llamador de GameUI.cpp; **100 %**, mnemcheck limpio). Miembros nuevos de datos `mCB58` y `mCB70`.
  - `BattleTask::fn_800FFE68` (objetivo de la camara delante del personaje con la esfera mas cercana; 97,1 %).
  - `BattleTask::fn_80102A40` (odio de un ataque segun la distancia; 99,4 %).

## Decisiones tomadas sin el usuario

- `fn_8010D320` sigue `extern "C"` con `BattleTask*` (GameUI.cpp la declara asi; no es de esta cola).
- `HateParamView_unk` (vista local de 0x38 bytes con `m34`) en BattleTask.cpp para el registro de
  `fn_80210220`: el tipo `HateParamEntry_unk` es local de `db/HateParam_unk.cpp` (otra cola). Pendiente: cabecera.

## Hallazgos

- `Vector3 v; v = a - b; v.Normalize_unk();` da la copia `psq` desde el temporal con el temporal debajo en la pila
  (`GetNormalize()` lo deja igual pero con r5/r6 cruzados) (fn_8010D320).
- `if (c && ok) { ... }` en vez de `return` temprano y `if (x == 0 && y >= K) {...}` sin `return` dan los saltos
  del original (fn_80102A40).

## Pendientes
