# enlazar-20261115, cola H (enemigos a 1-4 funciones)

Agente H, rama `agent-H`, 2026-11-15 15:18 -> 17:05. Rama: 15563 -> 15564 funciones (PRECOMMIT OK).

## Resultado por unidad

- `game/Em002_02SkeltonKing`: ctor 93,75 -> 100 % (7/9 -> 8/9). `const EnemyInfo_unk*` como en EnemyJill: la
  carga de `info->m20` sube por encima de los guardados de registros (renombrado del simbolo en `symbols.txt`;
  el llamador `SpecialEnemyFactory_unk` sigue enlazando, `main.dol: OK`). Queda `fn_80249588` 98,9 %
  (recarga de `m55C` y huecos de pila de `GetFront_unk`, ya documentado). Commit.
- Bloqueo comun de los ctores con dos `std::string` + `atn::Csv` (qB): **resuelto en `std/string.h`** (solo
  parche, sin commit, porque la cola J toca la cabecera): `basic_string(const T*)` mide con
  `Traits::length(s)` (nuevo `static unsigned long char_traits<T>::length(const T* s) { return strlen(s); }`),
  un nivel de inline mas. Para no renumerar los `@N`/`@enum$N` (la definicion nueva consume uno: perdia 2
  funciones de `chr/ControlAppear`, `@enum$7508` -> `7509`) se quita el nombre del parametro de la declaracion
  `void reserve(size_type res_arg)` -> `reserve(size_type)`. Parche: `docs/enlazar-20261115-qH.patch`
  (incluye `"matching": true` de Em201ZanGurg).
  - Medido en todo el proyecto (build completo contra `$REF`): **15563 -> 15591** (sin el +1 de SkeltonKing),
    lost 0, 28 funciones suben a 100 %: los ctores de Em201ZanGurg, Em201ZanGurgFinal, Em202SpiderBase,
    Em204Mulue, Em204MulueMp, Em308InvertCrab, Em308ParmCrab, Em101ElderZesya, Em101ElderZesyaSecond,
    Em101LittleZesya, Em101LittleZesyaSecond, Em203GreaterDaemon, Em203GreaterDaemonRed,
    Em203GreaterDaemonRedMp, Em005DoppelGanger, y Event/Dialog `GetPath_unk`, DialogHead `fn_80227A94`,
    `atn::D2A_Task` ctor, FileList `fn_8006B594`, Loader ctor y `fn_8046D1EC`, MaterialParser `fn_80080C20`,
    ptr_string_base_char_0 `fn_8021FC00`, CharaTask `fn_80138528`, EnemyJill `fn_8032E3DC`,
    MapPartsLocatorSet ctor y `vf2C`, SpecialEnemyBase `fn_805A3D68`.
  - **5 bajadas de fuzzy** (todas en funciones ya NONMATCHING): atn/Model `fn_8008BD10` 97,26 -> 97,23,
    CampSaveLoad `vf0C` 93,64 -> 93,57, CharaTask `fn_801371AC` 99,67 -> 99,66, MapTask `fn_804A06B4`
    99,91 -> 99,80 y `fn_804A0BCC` 99,80 -> 99,56 (en MapTask las dos cadenas del bucle, tipo 0 desde una
    llamada y tipo 4 desde `this+0x5C`, tienen en el original puntero/longitud en registros opuestos; con el
    parche las dos salen al reves).
  - Renumeracion de `@N`: aun con la compensacion, 126 unidades con `labelcheck` peor y 156 entradas nuevas de
    `rep100` (sin compensar: 355 y 296). Re-emparejado en lote con `pdbatch.sh` de esas 126 unidades y vuelta
    a como estaban de los 21 renombrados que no son `@N` (nombres globales `@STRING@...`/`__vt__...` en
    `auto_09_8087D6DC_sdata` y otras: daban `multiply-defined` al enlazar): `main.dol: OK`, lost 0,
    15592 funciones, quedan 26 `LC+` (una etiqueta cada una) y 22 `REP+` (12 de GameEventScript, 6 de
    CosEditProtector). Parche completo (cabecera + `symbols.txt`): `docs/enlazar-20261115-qH-repar.patch`.
  - Con el parche `game/Em201ZanGurg` tiene 15/15 al 100 % y **enlaza** (build limpio, `main.dol: OK`,
    736 bytes de datos). Sin el parche no se puede enlazar.
- `game/Em201ZanGurg` (sin parche): ctor 99,77 % sin cambio con ~20 variantes en el fuente (literal en local,
  `const char* s` fuera del bucle, bloque para el Csv, `for(;;)`, `assign`, `int`/`unsigned long` para la
  longitud, `std::strlen`, texto en local) y 6 en la cabecera (tag local, palabras a mano, `s + strlen`,
  `&s[n]`, helper `__init_unk`, `allocator` por defecto): solo `Traits::length` lo arregla.
- Con el parche siguen sin 100 %: Em302KrakenMp ctor 98,23, Em001Necromancer ctor 99,03 (registros del bloque
  de banderas y de los dos `clear()`; 7 ordenes y `__shrink_unk`), Em305VampireMp ctor 96,91.
- Sin cambio (variantes probadas): Em002_01BlackSkelton `vf0C` 94,4 (26 posiciones de las sentencias del
  bloque, una cada vez), Em303KrakenFootMp `vf0C` 99,6 (10 formas de las dos banderas; ninguna da los
  registros: el original or-ea el segundo en un registro nuevo), Em209SmallGolem `vf80` 99,7 (10 formas:
  `VecAssign_unk` en cada copia, temporal con nombre, `pos + offset`, referencia), Em205Ogre `fn_802B98B0`
  98,61 -> 99,02 con `asgwrap` [0,3,5,6] (no se deja: no llega a 100, quedan FPR), EnemySeaLizerd
  `fn_80355390` 99,96 (RotZ_unk, z en local, referencia), EnemyFieldMagician `vf3C` 98,86 (asgwrap sin
  mejora; `const` del msg, self). Em312DarkServant y el `CanAct_unk` de Em309GhoulSimple: bloqueos ya
  documentados, no intentados mas.

## Unidades que enlazan (bytes)

- Ninguna en la rama. Con el parche de `std/string.h`: `game/Em201ZanGurg` (736 bytes de datos).

## NONMATCHING con motivo

- Comentarios de las funciones sin cambio; el de Em002_02SkeltonKing ctor se quita.

## Cambios de splits

- Ninguno. `symbols.txt`: renombrado `__ct__19Em002_02SkeltonKingFPQ23atn4TaskP13EnemyInfo_unk` ->
  `...PC13EnemyInfo_unk`.

## Para el port

- `Em002_02SkeltonKing(atn::Task*, EnemyInfo_unk*)` -> `(atn::Task*, const EnemyInfo_unk*)`.
- Parche (sin commit): `char_traits<T>::length` nuevo en `std/string.h`.

## Decisiones tomadas sin el usuario

- El parche de `std/string.h` no se commitea (la cola J toca la cabecera y hay 5 bajadas de fuzzy); queda en
  `docs/enlazar-20261115-qH.patch` con las mediciones de arriba.

## Hallazgos

- `const` en el puntero de un parametro que solo se lee al principio deja que MWCC suba su `lwz` por encima de
  los `stw` de los registros guardados (el puntero a no-const puede apuntar a la pila): EnemyJill/SkeltonKing.
- La copia inline de `std::string(const char*)` mide con `char_traits::length` (un nivel de inline mas que
  `strlen`): cambia la asignacion literal/objeto del segundo string seguido de un `atn::Csv` (28 funciones).
- Una inline nueva con parametro con nombre en una cabecera comun consume un `@N`; se compensa quitando el
  nombre del parametro de una declaracion de la misma cabecera (`reserve(size_type)`).
- `pdbatch.sh` tras una cabecera comun puede dar nombres globales (`@STRING@<func>`, `__vt__...`) a etiquetas de
  splits `auto_`/enlazados que chocan con las copias debiles al enlazar: dejar solo los renombrados `@N`.
- `precommit.sh` corta `ninja` a los 590 s: tras tocar una cabecera comun, `ninja -j1` antes a mano (si no,
  "interrupted by user" y un `report.json` viejo).
- `build.ninja` no depende de `game_units.json`: tras volver `matching` a false hay que repetir `configure.py`
  o el `.dol` se enlaza con el fuente viejo (`main.dol: FAILED`).

## Pendientes

- Decidir el parche de `std/string.h` (+28 funciones, Em201ZanGurg enlaza, 5 bajadas en NONMATCHING) junto
  con lo que haga la cola J en la misma cabecera.
- MapTask `fn_804A06B4`/`fn_804A0BCC` con el parche: las dos cadenas del bucle piden asignaciones opuestas.
