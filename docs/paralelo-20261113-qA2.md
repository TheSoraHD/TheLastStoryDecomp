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
  - `fn_8010C00C` (estados de las condiciones del equipo; 97,5 %, registros de los bucles).
  - `fn_800F3170` (libera los efectos de batalla) y `BattleTask::fn_800F3490` (cargando): **100 %** las dos,
    mnemcheck limpio. Tipado de 0x363C0..0x3663C: `mEfp363C0[4]`, `mHitEff[3][7]` (EffFile), `mEfp364EC`,
    `mSplashEfp[3]`, `mEff3651C[5]`, `mEff36558[5]`, `mEff36594[3]` (EffFile), `mEfp365B8[4]`, `mEfp365E8[7]`;
    `m389F4` es un EffFile; `mCB7C` un `atn::Texture`; `m3E394` el flag de cargados. Las EffFile y la Texture son
    almacenamiento crudo (`unsigned char[0xC]`) como en `BattleMarkers_unk`, con `AsEffFile_unk`/`AsTexture_unk`
    en el .cpp. `fn_801018E4` sube de 93,9 a 98,5 % con el tipado.
  - `fn_800F2C9C` (peticiones de carga de los efectos de batalla): **100 %**, mnemcheck limpio. Los cinco
    formatos de `sprintf` llevan `\0` extra (los `""` del pool). Los nombres de `.sdata2` (`lbl_808813DC`...
    `lbl_80881474`) y las tablas `lbl_80735A94`/`lbl_80735A48` de `.rodata` quedan `extern` como las demas del
    fichero (la unidad no enlaza aun).

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
- `game/SkillEffectManager.h` incluido en BattleTask.cpp (quita el `extern "C" void* lbl_8087F9E8` local);
  `chr/CharaEquip_unk.h` y `db/EquipParam_unk.h` tambien (fn_8010C00C: `m414` se lee como `EquipParamEntry_unk`
  y sus condiciones `mE4` como `ArmsCond_unk`, con cast).
- `fn_800D58A4` (liberar `atn::Texture`) como `extern "C"` (como GameUI.cpp) y `fn_800D59B8` llamado a traves de
  `GimmickUvOwner_unk` (su nombre en `symbols.txt`, como gmk/AutomaticDoor.cpp): las dos son de `atn::Texture`
  (atn/Texture_unk.cpp, otra cola). Pendiente: pasarlas a miembros de `atn::Texture`.
- `fn_800F3170` sigue `extern "C"` con `BattleTask*` (GameTask.cpp la declara asi).
- Vistas locales nuevas en BattleTask.cpp: `BattleWeapon_unk` (fn_80206C50), `BattleState_unk` (fn_8021AF50),
  `BattleLogDef_unk`; `BattleMsg_unk::GetPair_unk` (como CosEditItemList_unk).

- `fn_8010D320` sigue `extern "C"` con `BattleTask*` (GameUI.cpp la declara asi; no es de esta cola).
- `HateParamView_unk` (vista local de 0x38 bytes con `m34`) en BattleTask.cpp para el registro de
  `fn_80210220`: el tipo `HateParamEntry_unk` es local de `db/HateParam_unk.cpp` (otra cola). Pendiente: cabecera.

## Hallazgos

- Arrays separados (`a[5]`, `b[5]`) en vez de uno con `a[5 + i]` dan los punteros de bucle del original; con
  `(&arr[k])[i]` tambien se reduce el indice (fn_800F3170/fn_800F3490).

- `Vector3 v; v = a - b; v.Normalize_unk();` da la copia `psq` desde el temporal con el temporal debajo en la pila
  (`GetNormalize()` lo deja igual pero con r5/r6 cruzados) (fn_8010D320).
- `if (c && ok) { ... }` en vez de `return` temprano y `if (x == 0 && y >= K) {...}` sin `return` dan los saltos
  del original (fn_80102A40).

## Pendientes
