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

## Cambios de splits

Ninguno.

## Cambios de symbols.txt (salvo renombrados)

Ninguno: solo renombrados `fn_XXXXXXXX` -> nombre mangled de las funciones escritas y etiquetas `@N` (lblmap/pd).

## Resultados por unidad

## Hallazgos
