# Cola A2 (paralelo-20261113): segunda pasada de Batalla

Inicio: 2026-11-13 06:59 CET. Rama `agent-A2`. Unidades: `game/BattleTask`, `game/TacView`, `game/BattleCharaTask`.
Funciones al 0 % al empezar: BattleTask 29, TacView 6, BattleCharaTask 1.

## Resultado por unidad

- `game/BattleTask`:
  - `fn_8010D320` (direccion en pantalla hacia una posicion, para GameUI; `extern "C"` con `BattleTask*` como el
    llamador de GameUI.cpp; **100 %**, mnemcheck limpio). Miembros nuevos de datos `mCB58` y `mCB70`.
  - `BattleTask::fn_800FFE68` (objetivo de la camara delante del personaje con la esfera mas cercana; 97,1 %).
  - `BattleTask::fn_80102A40` (odio de un ataque segun la distancia; 99,4 %).
  - `BattleChara_unk::fn_801083D8` (valor de odio hacia una entrada; 93,7 %, registros).
  - `BattleTask::fn_80100BF0` (menu de depuracion "Battle"; 99,4 %: solo los desplazamientos de las cadenas en
    `@stringBase0`, faltan las de funciones sin escribir). `BattleTask::fn_80100BC0` pasa a
    `static void fn_80100BC0(void*, void*)` (callback del menu: recibe `this` como argumento), 100 %.
    `m38A58` es un `ResHandle` (`Load("Battle")`; el destructor lo libera, 0x800F2370). Miembros de datos
    nuevos: `m367B4`, `m38A08` (int), `m38A10`, `m38A14`, `m38A20`, `m38A24`, `m38A30`, `m38A34`, `m38A3C`-`m38A54`,
    `m3E390`, `m3E3A0`-`m3E3A8` (parametros del menu).
  - `fn_801092C8` (mensaje del registro de batalla segun `lbl_80735AB0`; 87,9 %: dos casos comparten la cola).

## Decisiones tomadas sin el usuario

- `game/ResHandle.h` incluido en `BattleTask.h` (las 56 unidades que incluyen BattleTask.h ya lo incluian).
  `ResHandle` es en realidad la carpeta de Mindows (`fn_8008937C` = AddFolder); no se renombra (otra cola, 355
  unidades).
- `fn_80100BC0` cambia de `void ()` a `static void (void*, void*)`: el original pasa su direccion como funcion C al
  menu, y un puntero a miembro no se puede convertir. Renombrado en `symbols.txt` (no tenia llamadores C++).
- Declaraciones locales `extern "C"` de `fn_800874C8`/`fn_8008771C`/`fn_80087994` en BattleTask.cpp con
  `void*`: `atn/MindowsCommon.h` choca con `game/ResHandle.h` en el tipo de `lbl_8087EF10`.
- `fn_801092C8` sigue `extern "C"` con `BattleTask*` (la llaman ~10 unidades de otras colas). `lbl_80735AB0`
  (`.rodata` de este fichero) como `extern` como ya estaba `lbl_80735DD0`: la unidad no enlaza todavia.
- Vistas locales nuevas en BattleTask.cpp: `BattleWeapon_unk` (fn_80206C50), `BattleState_unk` (fn_8021AF50),
  `BattleLogDef_unk`; `BattleMsg_unk::GetPair_unk` (como CosEditItemList_unk).

- `fn_8010D320` sigue `extern "C"` con `BattleTask*` (GameUI.cpp la declara asi; no es de esta cola).
- `HateParamView_unk` (vista local de 0x38 bytes con `m34`) en BattleTask.cpp para el registro de
  `fn_80210220`: el tipo `HateParamEntry_unk` es local de `db/HateParam_unk.cpp` (otra cola). Pendiente: cabecera.

## Hallazgos

- `Vector3 v; v = a - b; v.Normalize_unk();` da la copia `psq` desde el temporal con el temporal debajo en la pila
  (`GetNormalize()` lo deja igual pero con r5/r6 cruzados) (fn_8010D320).
- `if (c && ok) { ... }` en vez de `return` temprano y `if (x == 0 && y >= K) {...}` sin `return` dan los saltos
  del original (fn_80102A40).

## Pendientes
