# Lote paralelo 2026-11-12, cola I (juego varios)

Inicio 17:49; el servidor se reinicio hacia las 18:33 (trabajo sin commitear recuperado de un `git stash` que quedo
a medias) y se retomo a las 20:16.

## Resultado por unidad (funciones al 0 % antes -> despues)

- `game/MapPartsGimmickLoc` 19 -> 15. Al 100 %: `~MapPartsGimmickLoc` (0x804970D4) y el destructor del registro
  `__dt__28MapPartsGimmickLocRecord_unkFv` (0x8049707C, copia weak). Escritas: `vf0C` (99,84 %), `vf2C` (99,95 %).
- `game/GameSaveData_unk` 6 -> 2. Escritas: `GameSaveFlags_unk::fn_803B9804` (97,87 %), `fn_803B9D78` (92,41 %),
  `GameSavePlayers_unk::fn_803BA620` (99,83 %), `GameSaveSettings_unk::fn_803BD79C` (36,91 %, ver abajo).
- `game/CampSaveLoad` 9 -> 8. Escrita: `fn_8011B570` (80,52 %, mismo problema que `fn_8011B3D8`).
- `game/ReplayBuffer` 7 -> 5. Escritas: `ReplayRecorder_unk::fn_8056B558` y `fn_8056CB20` (funcion libre sobre un
  `ReplayFrame_unk`; mismo codigo, 99,51 % cada una).
- `game/RouteWalker_unk` 7 -> 6. Escrita: `fn_801279E8` (98,65 %, distancia a la meta).
- Sin cambios: ItemManager_unk (19), AiTarget (11), GimmickManager (2), GameTask (6),
  Bullet (1), CharaTask (1).

## NONMATCHING

- MPGL `vf0C` 99,84 %: r27/r28/r29 del constructor inline de `std::string` (ruta, temporal, longitud).
- MPGL `vf2C` 99,95 %: solo el desplazamiento de "<all>" en `@stringBase0` (faltan las cadenas de fn_804985E4/vf10).
- `fn_803B9804` 97,87 %: asignacion de registros y la copia del siguiente nodo del arbol de eventos terminados.
- `fn_803B9D78` 92,41 %: el bucle de terminados va guardado con una prueba con signo; registros del ChaseManager.
- `fn_803BA620` 99,83 %: el original carga el id de la entrada antes que el del jugador en la busqueda.
- `fn_803BD79C` 36,91 %: el original copia el bloque de ajustes del jugador de `GameConfig_unk` (0x244-0x334) a un
  local, cambia `mAlwaysBirdview` y lo copia de vuelta miembro a miembro; no hay tipo para ese bloque y crearlo
  cambia todos los usuarios de `GameConfig_unk`. Se escribio la asignacion directa (equivalente).
- `RouteWalker_unk::fn_801279E8` 98,65 %: huecos de pila de los vectores de los modos 4, 5 y 7.
- `fn_8011B570` 80,52 %: las pruebas del pad se materializan como bool (`neg/or/srwi.`), igual que en `fn_8011B3D8`.
- `fn_8056B558`/`fn_8056CB20` 99,51 %: registros del prologo y un `frsp` del z antes de `atan2`.

## Decisiones tomadas sin el usuario

- `SwingPart_unk::m98` (`include/gmk/SwingPart_unk.h`, familia gmk) pasa a `boost::scoped_array<atn::Matrix3x4>`:
  el destructor de MapPartsGimmickLoc lo libera como miembro (tras `mEfp`, antes de `mLoader`).
  `src/gmk/SwingObject.cpp` usa `reset`/`get`; sin cambios de porcentaje en SwingObject.
- `~static_string()` vacio definido como plantilla inline en `MapPartsGimmickLoc.cpp` (como hace AreaEffConfig), no
  en la cabecera (la cabecera no puede: AreaEffConfig y PopupTask lo definen ellos).
- `GameSaveRoute_unk` con campos `unsigned int` y `GameSaveFlags_unk::mLeader` `unsigned int` (comparaciones sin
  signo del original); `GameSavePlayer_unk::mRouteMode/mRouteFlag` `unsigned short` (`lhz`).
- Ayudantes `static inline` en los `.cpp` (no miembros en cabecera, para no renumerar `@N`): `RestorePlayer_unk`
  (GameSaveData_unk), `GetPointPos_unk` (RouteWalker_unk). `fn_8056CB20` es una funcion libre (no miembro de `ReplayFrame_unk`) por la misma razon. Miembros nuevos en cabeceras: `CampSaveLoad::fn_8011B570`; `ReplayChara_unk::m80` pasa a
  `mRefPos[3]`/`mRefFlags`.
- Primer intento con miembros nuevos en `GameSaveData.h`/`ReplayBuffer.h`: renumeraba las `@N` de GameTask, Arms,
  Bullet, HomingBullet, PacketPack, GameLoadTask... y el reetiquetado automatico no las dejaba bien (HomingBullet):
  se quitaron los miembros y se devolvieron sus etiquetas a las de `HEAD`. El reetiquetado automatico de GameSaveData_unk
  habia puesto `sInstance__9SetConfig` en 0x80885CB0 (el 31.0f de fn_803BD79C, que no empareja): devuelto a
  `lbl_80885CB0` (rompia el enlace).
- Renombrados en `symbols.txt`: dtor y dtor del registro, `vf0C`/`vf10`/`vf2C`/`fn_804985E4` de MPGL,
  `fn_8011B570`, `fn_8056CB20`.

## Pendientes

- `CampSaveLoad::fn_8011B018` escrito pero retirado (borrador en scratch): necesita `atn/D2A_Task.h`, que choca con
  las declaraciones `extern "C"` locales de CampSaveLoad (`fn_801F3FF8`, `fn_801F4E8C` con `D2APoint_unk`).
- MPGL `fn_804985E4`: saca ~11 copias weak (vectores MSL, `SimpleArray::operator[]`/Resize, ctor del registro), pero
  varias funciones "al 100 %" del fichero son en realidad copias weak de otras clases con el tipo equivocado
  (`MapPartsGimmickLocObj_unk`, `fn_80499388` sobre un `atn::Csv`...): hay que rehacerlas antes.
- GameSaveInventory (0x803BB584/0x803BC2D4): copias de estructuras grandes con tipos sin unificar.
- GimmickManager vf0C sigue bloqueada (presupuesto de inline).

## Hallazgos

- Declarar un miembro nuevo (aunque sea `inline` sin cuerpo) en una cabecera renumera las `@N` de las unidades que
  la incluyen; un `static inline` libre en el `.cpp` no.

- Locales de un bloque en orden ascendente en la pila (inline): mover el bloque a un `static inline` libre (no
  miembro: sus literales irian a `@STRING@` propios en vez de `@stringBase0`).
- En bucles con contador, declarar el contador antes de una carga de global (`unsigned int i = 0; T* g = lbl;
  for (; i < n; i++)`) reordena los registros guardados (vf2C de MPGL).
