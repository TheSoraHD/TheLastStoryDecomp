Inicio: 2026-11-10 12:28 (Europe/Madrid). Cierre minimo: 2026-11-11 00:28.

# noescritas-20261110

Rama desde `main` 4ef537b: 14958 / 18109, lost 0, `main.dol: OK`. Funciones al 0 % (report.json): 1148.

## Decisiones tomadas sin el usuario

- **Bloque 1: la inline de copia no va en `include/math/Vec.h` sino en `include/math/VecAssign_unk.h`** (incluida solo por
  las 33 unidades que la usan). Probado en `Vec.h`: una inline mas en esa cabecera renumera los `@N` de datos de ~300
  unidades (labelcheck peor en casi todas) y los `@enum$N` de los nombres mangled (ControlAppear perdia 2 funciones
  boost con `@enum$6816` en el nombre). Con la cabecera aparte: lost 0, sin bajadas, solo `lblmap` en HomingBullet y
  Cannonball.

- `MP/func_enter_setting`: `PacketPackMP::vf0C/vf10` inline en la clase (100 %); la unidad no enlaza: falta
  `atn::net::PacketPack::~PacketPack` (fn_804FC2F0, en `atn/PacketPack`).
- `game/PopupTask`: `PopupEntry_unk` con `atn::static_string<32, wchar_t>` (texto, 0x20) y dos `atn::Vector3` (0x60,
  0x6C); especializaciones wide locales en el `.cpp` (como `NewCampPartsOption.h`). `fn_8054A9A8` pasa a miembro:
  `GameTask.cpp` lo llama como `lbl_8087F8A8->fn_8054A9A8()` (vista de PopupTask en GameTask.cpp).
- `game/GameEventScript`: los comandos se declaran todos de una vez en la cabecera (46 + vf10/vf14) para no
  renumerar los `@N` de los que la incluyen (GameScene, GameTask, TownTpcLoader, CharaTask) en cada commit.
  `m12C` es un `std::vector<ScriptRecord14_unk>` (registro con el `CharaTask*` primero) que borra
  `fn_803AB37C(CharaTask*)`; `CharaTask::fn_80171DB0` lo llama a traves de `RouteTable_unk::m134` (es el
  `GameEventScript` de la escena), asi que `CharaTask.cpp` incluye `game/GameEventScript.h` (lblmap de 173
  etiquetas, sin perdidas).
- `gmk/InteractionSpot`: `MatchId_unk`/`NearSpot_unk` como `static inline` del `.cpp` (en la cabecera renumeraban
  los `@N` de `GimmickCommon`, que incluye todas las cabeceras de gimmicks).
- `atn/TextureComposite fn_800D6E48` sin escribir: usa `PostFxManager::mAlloc` (LinearAllocator), definido solo en
  `atn/LinearAllocator.cpp`; hace falta llevar `LinearAllocator`/`PostFxManager` a una cabecera.
- `GameChoiceWindow::fn_803B6E04(GameMessage*, int, float)` sustituye la firma del stub DRAFT (lblmap de 42 unidades).
- `MiniMapTask.h` declara `fn_804AAD68(CharaTask*, const atn::Color*)` y `fn_804AAFCC(CharaTask*)`; `ChaserGuard` los
  llama como miembros (`(MiniMapTask*)lbl_8087FA20->m258`).
- `GameEventScript.h`: `mE0` deja de ser relleno (`int mE0, mE4`); `m154[64]` = `ScriptPartsFlag_unk` (hash del
  nombre de la pieza de mapa + flags de dibujo, `fn_803A68B8`).
- `fn_803A1B20` y `fn_803A2E20` pasan a `ScriptCmd_unk*` no `const` (truco de `maraton-20261107` para la espera
  variable de `SetNext_unk`): cambio de nombre mangled en symbols.txt.
- jumptables `jumptable_8078AFAC/AFE0/B040` renombradas a su `@N` (truco existente, trucos-mwcc 435).

## Cambios de splits

Ninguno.

## Cambios de symbols.txt (salvo renombrados)

Ninguno: solo renombrados `fn_XXXXXXXX` -> nombre mangled de las funciones escritas y etiquetas `@N` (lblmap/pd).

## Resultados por unidad

## Hallazgos

- `5.0f + (a->m5B0 + b->m5B0)` con los `lfs` en el orden de `b` primero y `fadds fX, fY(a), fX(b)`: `float r; float c;
  r = b->m5B0; c = a->m5B0; 5.0f + (c + r)` (GameEventScript fn_803A2E20). Posible para RouteWalker_unk fn_80128930.
- Busqueda inline cuyo `id` se carga en el preheader del bucle (tras la carga del contador): pasar el puntero ya
  comprobado a la inline (`scene != 0 ? FindWarpIn_unk(scene, args->m28) : 0`); como parametro de la inline
  exterior se carga demasiado pronto y por referencia dentro del bucle (fn_803A2E20).
- Codigo en dos ramas con los temporales a la inversa en la pila (`Vector3` + `Mtx`): sacarlo a una `static
  inline` comun (`StepBack_unk`, fn_803A5118).
