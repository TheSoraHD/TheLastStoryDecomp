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
  - `fn_80106878` (efecto del movimiento de ataque "na000_ats..." de un personaje, `mEfp365E8[kind]`; 97,7 %:
    marco 0x10 mayor). Usa `PlayEfp_unk`, `fn_801070C8`/`fn_80107208` (se inlinean) y `HasCond_unk` (inline nuevo).
  - `fn_8010220C` (reticula de la camara: color, anillos con el stick y centro; 99,0 %, registros volatiles).
    Miembros nuevos `mCB34`, `mCB38`, `mCB40`, `mCB48`, `mCB50` (`atn::Vector2_unk`); `Arena_unk::m50C`.
  - `fn_800FCD24` (bala contra la esfera `index`; 97,3 %: huecos de las matrices de giro). El noveno argumento es
    el `BattleCalcParam_unk*` de salida (copia de m38A5C), el decimo float no se lee. Inline nuevo
    `HitCenterAt_unk` (como `HitCenter_unk` con indice); `BattleDamage_unk`/`fn_80108C10`/`fn_8010B250` suben
    delante.
  - `fn_800FC410` (golpe directo de un personaje contra la esfera `index`; 98,7 %: huecos de las matrices de giro,
    como fn_800FD658). Lee `mControl->vf2C()` (numero de ventanas de golpe), `m598`/`m59D`/`m59E`/`m59F`/`m5A0`
    del atacante y `GameConfig_unk::mForceRatio`.
  - `BattleTask::fn_8010B398` (estados que da el golpe y los que pierde el blanco; 98,8 %, registros): miembro
    nuevo declarado en `BattleTask.h` sin nombres de parametro (solo lo llama fn_800FDF14), renombrado en
    `symbols.txt`. Inlines nuevos `AttackArms_unk` (arma del ataque, como en fn_80101CDC) e `IsStatusBullet_unk`;
    `BattleItem_unk` gana `mConds[2]` (0xEC, condiciones de la flecha). El tercer argumento de
    `Arms::fn_80044E98`/`fn_80044FBC` es la posicion del golpe: se pasa con `(int)&pos` (la firma de `Arms.h`
    lo tipa `int`; pendiente corregirla en su cola).

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
- Includes nuevos en BattleTask.cpp: `game/HomeButtonMenu.h`, `atn/GraphicsDevice.h` (`lbl_8087EEE0` como
  `atn::GraphicsDevice_unk*`, como GameUI.cpp). `SetPaneValue_unk` inline local (como BattleResult.cpp).
- Vistas locales nuevas en BattleTask.cpp: `BattleWeapon_unk` (fn_80206C50), `BattleState_unk` (fn_8021AF50),
  `BattleLogDef_unk`; `BattleMsg_unk::GetPair_unk` (como CosEditItemList_unk).

- `fn_8010D320` sigue `extern "C"` con `BattleTask*` (GameUI.cpp la declara asi; no es de esta cola).
- `HateParamView_unk` (vista local de 0x38 bytes con `m34`) en BattleTask.cpp para el registro de
  `fn_80210220`: el tipo `HateParamEntry_unk` es local de `db/HateParam_unk.cpp` (otra cola). Pendiente: cabecera.

## Hallazgos

- `0x18F <= id && id <= 0x195` (constante a la izquierda) evita la fusion en `subi/cmplwi` (fn_8010B398; ya en
  trucos-mwcc.md, linea 53/294).

- `int n = 1; if (p) n = p->vf();` da el `li r4, 1` antes de la llamada; el ternario lo pone despues (fn_800FC410).

- Arrays separados (`a[5]`, `b[5]`) en vez de uno con `a[5 + i]` dan los punteros de bucle del original; con
  `(&arr[k])[i]` tambien se reduce el indice (fn_800F3170/fn_800F3490).

- `Vector3 v; v = a - b; v.Normalize_unk();` da la copia `psq` desde el temporal con el temporal debajo en la pila
  (`GetNormalize()` lo deja igual pero con r5/r6 cruzados) (fn_8010D320).
- `if (c && ok) { ... }` en vez de `return` temprano y `if (x == 0 && y >= K) {...}` sin `return` dan los saltos
  del original (fn_80102A40).

## Pendientes
