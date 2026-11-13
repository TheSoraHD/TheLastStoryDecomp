# Cola N (lote paralelo-20261113): unidades cerca de enlazar

Unidades fuera de la ruta con una sola funcion por debajo del 100 % (lista en `/home/claude/scratch/n-candidates.txt`).

## Resultado por unidad

- `chr/ControlBase`: vf18 99,93 -> 100 %; **enlaza** (`matching: true`). Los tres bloques de rotacion van en
  una inline `RotZYX_unk(Matrix3x4*, const Vector3&)` con `Mtx t; Mtx r;` en cada bloque.
- `game/Arms`: vf10 99,90 -> 100 %; **enlaza**. Bloques de rotacion en una inline `SetRotYXZ_unk` dentro de
  `RotateYXZ_unk` (sin `Rotate_unk`), y `SetMtx_unk(model, mtx)` (como game/Gun.cpp) para la escala.
  Para enlazar: `m29C.get()->IsLoading()` en vf0C (la copia debil de `scoped_ptr::get` sale tras vf0C) e
  `#include "atn/Csv.h"` antes de `game/Arms.h` (vtable/RTTI de Arms antes que los de CsvTab en `.data`).
- `game/NewStaffRoll`: fn_80531394 99,86 -> 100 %; **enlaza**. `atn::CsvU csv((const wchar_t*)mLoader.fn_80470580(),
  (unsigned int)mLoader.fn_8047059C() / 2);` (argumentos en la llamada, sin locales).
- `MP/func_enter_setting` (206/206 al 100 %, no enlazaba): **enlaza**. Tres causas: (1) el destructor de
  `PacketPackMP` (copia debil en nuestro .o) va en atn/PacketPack: renombrados en `symbols.txt`
  `fn_804FC2F0` -> `__dt__Q22MP12PacketPackMPFv` y `fn_804D14B8` -> `__dt__Q33atn3net10PacketPackFv`
  (vtable de PacketPack); (2) faltaba la base `atn::SingletonNoStatic<PacketPackMP>` (lista de bases
  0x80792A44); (3) orden de RTTI/vtables: la clase va en `include/MP/PacketPackMP.h`, incluida antes de las
  `MP::func_*`, y `atn::net::FuncMessage` pasa a `include/atn/net/FuncMessage.h` (incluida por
  `MP/FuncMessage.h`, sin cambio para atn/PPDATAU). Resuelve el bloqueo de `docs/mwcc-blockers.md` (s12h-20261108).
- `game/TownTpcLoader`: vf0C 99,59 -> 100 %; **enlaza**. El ultimo bucle en una `static inline StartCharas_unk(FixedArray&)`.
- `gmk/LemonBucket`: vf10 99,77 -> 100 %; **enlaza**. Los bucles de fades/switches en `static inline UpdateFades_unk`/`UpdateSwitches_unk`.
- Sin cambio (intentos con inlines y ordenes, revertidos; 3-10 intentos cada una): `atn/D2A_Frame` vf18 (registros de float),
  `game/MapPartsRain` ctor, `chr/ControlDetect` vf0C (r3/r4 de la copia; asgwrap sin mejora),
  `game/CrossbowMenuTask` fn_801E9544 (con la inline el inventario sale bien pero se cruzan i/offset),
  `MP/D2AMpMenuSetRule` fn_804CECEC, `game/SelectMember` ctor (con la inline el ctor se inlinea en
  fn_8057162C), `atn/CsvBase_char_124_13_34` fn_802078C4, `db/MemberList_unk` fn_80217D9C,
  `atn/CompositeCache_unk` fn_800D8458, `chr/ControlAppear` vf40, `game/GameSaveBanner` fn_803B9638,
  `game/BattleResult` fn_800F0D14 (el bucle base_point en inline sube a 99,38 % pero no llega),
  `Event/EventLoader_unk` fn_80576258 (en inline se inlinea en fn_80576030), `gmk/FlyObjManager`
  fn_80427870 (`SetMtx_unk` arregla la escala pero baja vf40/vf44 por el orden del pool), `gmk/Augur` vf10,
  `game/Em201ZanGurg` ctor, `gmk/ReinforcePoint` vf10, `chr/ControlOneMotion` (ctor de Transition).
- No probadas por bloqueo conocido: los `Loader::Get_unk<T>` de los LH_* (cabecera muy incluida,
  `docs/mwcc-blockers.md`), Event/MotionData vf08, CopyRobotTask vf10.
- `game/GameLoadTask`: vf10 99,96 -> 100 %; **enlaza**. La rama "Debug Data" en una `static inline
  LoadDebugData_unk(GameLoadTask*)` (lee `self->m48` dentro) con los locales en orden `path, size, offset`.

## Hallazgos

- Orden de pila de bloques repetidos con locales `Mtx t; Mtx r;`: directos en una inline -> orden de
  declaracion; en una inline dentro de otra -> invertido entero. Probar un nivel mas de inline (ControlBase vf18, Arms vf10).
- Escala por columnas `s` debajo de los temporales: `SetMtx_unk` con `float sz/sy/sx = ColumnLength_unk(...)` (Arms vf10).
- `atn::CsvU csv(a(), b() / 2)` con los argumentos en la llamada da registros salvados mas altos que con locales (NewStaffRoll).
- Orden de vtables/RTTI en `.data`: incluir `atn/Csv.h` antes de la cabecera de la clase pone la vtable de la clase
  antes que las de CsvTab/CsvBase (Arms).
- **Orden de vtables/RTTI (y de sus cadenas de nombre) en `.data`/`.rodata` = inverso al orden de declaracion
  de las clases en la unidad**: la clase declarada antes sale despues. Se arregla con el orden de los
  `#include` o partiendo una cabecera (Arms, func_enter_setting).
- **Indice y desplazamiento de un bucle cruzados (r29/r30)**: sacar el bucle a una `static inline` que recibe el
  contenedor por referencia (TownTpcLoader, LemonBucket).
- Dos locales de un bloque en registros salvados cruzados: sacar el bloque a una `static inline` que recibe `this`
  y lee los miembros dentro (GameLoadTask vf10).
- Copia debil de `scoped_ptr::get` tras la primera funcion que llama a `get()` explicito (no a `if (p)`) (Arms vf0C).

## Decisiones tomadas sin el usuario

- `matching: true` solo en las unidades que dan `main.dol: OK`.
- Renombrados en `symbols.txt` de dos funciones de atn/PacketPack (fuera de alcance del lote) para que
  func_enter_setting enlace: solo nombres, ningun `.cpp` usaba los `fn_`.
- Cabeceras nuevas `include/MP/PacketPackMP.h` y `include/atn/net/FuncMessage.h` (esta sacada de
  `MP/FuncMessage.h`, que la incluye; solo la incluyen func_enter_setting y atn/PPDATAU).

## Pendientes

- Probar el truco del bucle en `static inline` en mas funciones con indice/desplazamiento cruzados
  (MapManager FindArea, TreasureBoxTable, LH_Handle ya probado sin exito).
- `gmk/FlyObjManager` fn_80427870: con `SetMtx_unk` quedan los temporales de pila de los estados 2/3 y la
  esfera; habria que resolver a la vez el orden de `.sdata2` de vf40/vf44.

## Resumen

- 7 unidades pasan a enlazar: chr/ControlBase, game/Arms, game/NewStaffRoll, MP/func_enter_setting,
  game/TownTpcLoader, gmk/LemonBucket, game/GameLoadTask (units 657 -> 664 en precommit).
- 6 funciones nuevas al 100 % (15313 -> 15319): ControlBase::vf18, Arms::vf10, NewStaffRoll::fn_80531394,
  TownTpcLoader::vf0C, LemonBucket::vf10, GameLoadTask::vf10. Ninguna funcion al 0 % en la cola (todas eran parciales).
