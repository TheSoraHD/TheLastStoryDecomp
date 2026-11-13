# paralelo-20261113, cola J (juego varios 2)

## Resultado por unidad

- `game/RouteWalker_unk` 6 -> 2 al 0 %. Al 100 %: `fn_80128CF8` (meta a la vista), `fn_80129670` (segmento entre
  dos puntos). Escritas: `fn_80128090` (97,05 %, replanifica el modo actual), `fn_80129144` (98,97 %, va directo a
  la meta). Quedan `fn_801255C8` (3 KB) y `fn_80126214` (6 KB).
- `game/CampSaveLoad` 8 -> 0 al 0 % (todas escritas). Al 100 %: `vf10` (0x801185E0). Escritas: `vf0C` (92,83 %), `fn_8011B018`
  (99,36 %), `fn_801191A8` (99,98 %, estado 1, solo pool), `fn_8011999C` (97,77 %, estado 5), `fn_80119F0C`
  (99,43 %, barra de informacion), `vf14` (98,83 %, registro de la historia), `fn_8011A4D0` (99,18 %, ventana de
  confirmacion). El pool `@stringBase0` ya coincide salvo las 3 cadenas de `basic_string` del final. Sube `fn_8011B8D4` 95,79 -> 99,93 % (pool de cadenas).
- `game/ReplayBuffer` 5 -> 2 al 0 %. Escritas: `fn_8056C3D8` (98,77 %, graba un fotograma), `fn_8056F5B4`
  (98,65 %, graba el fotograma en vivo `mLive`), `fn_8056FC24` (96,63 %, restaura `mLive`: personajes, modelos y
  balas). Quedan `fn_8056DACC` (3,6 KB) y `fn_8056E918` (2,5 KB). GameTask 98 -> 106 en fnstat tras `pdbatch` (renumerado por la
  llamada a miembro).

## NONMATCHING

- `RouteWalker_unk::fn_80129144` 98,97 %: solo registros (tabla en r5 y `m44` en r4 en el original). Probado: local
  para `m44` antes y despues de la tabla, tabla pedida en el caso 3, getter inline del sitio, orden de los casos,
  bandera de `m48` tras el switch.
- `RouteWalker_unk::fn_80128090` 97,05 %: solo registros de los planificadores inlineados (el destino en r26).

- `CampSaveLoad::vf0C` 92,83 %: un registro salvado de mas; el original toma `""` de `@stringBase0 + 0x3D` con la
  misma base (pool aun incompleto: faltan vf14 y los ayudantes de estado).
- `CampSaveLoad::fn_8011B018` 99,33 %: offsets del pool y registros r23/r24 (tamano/puntero) de los dos `+=`
  cruzados; probado `append` y `replace(size(), 0, s)`.
- `CampSaveLoad::fn_8011999C` 97,77 %: un registro salvado de mas en los paneles de `m64` y texto/layout del
  segundo aviso cruzados. Los avisos repetidos no son una inline (los temporales de `SoundRef` estan arriba en la
  pila, en el orden del fuente).
- `CampSaveLoad::fn_80119F0C` 99,43 %: pool y registros r27/r28 del texto de "ampm".
- `CampSaveLoad::fn_8011A4D0` 99,18 %: registros de `L""` y del layout en los dos primeros textos, huecos de pila
  de los dos iconos de boton (0x30/0x20 en el original).
- `CampSaveLoad::vf14` 98,83 %: registros del bucle de entradas y de los dos colores (`atn::Color::ToU32` hace los
  OR en otro orden; no se toca la cabecera compartida) y pool.

- `ReplayBuffer::fn_8056C3D8` 98,77 % y `fn_8056F5B4` 98,65 %: registros (puntero del fotograma; palabra de flags y
  resultado de `IsSkipped_unk` en el bucle de enemigos, como `ReplayRecorder_unk::fn_8056B3D4`).

- `ReplayBuffer::fn_8056FC24` 96,63 %: marco 0x40 mas pequeno (huecos de las matrices) y registros.

## Decisiones tomadas sin el usuario

- `__declspec(noinline)` en la definicion de `RouteWalker_unk::fn_801286F0` (excepcion aceptada de la regla 2): el
  original la llama con `bl` desde `fn_80128090`, que inlinea los otros planificadores (`fn_80128818`...). Sigue al
  100 %.
- Cuatro miembros nuevos de `RouteWalker_unk` en `game/CharaStatus.h` (269 unidades), sin nombres de parametro:
  `fn_80128090`, `fn_80128CF8`, `fn_80129144`, `fn_80129670(int, int)`. Precommit sin bajadas.
- `RouteWalker_unk.cpp` incluye `game/GameConfig_unk.h` (`lbl_8087F0A8->m060`) y `math/VecAssign_unk.h`: los `@N`
  de la unidad cambian (renombrados en `symbols.txt` con `pd.sh`).

- CampSaveLoad pasa a `atn::D2A_Task` (incluye `atn/D2A_Task.h`): fuera la vista `D2ATask_unk` y las declaraciones
  locales que chocaban (`fn_801F3FF8`, `fn_801F4E8C`, `fn_801F4728`); `D2APoint_unk` -> `atn::D2A_DrawRect` en
  `fn_8011AD38` (sin cambio de %). `m118..m124` -> `CampArrow_unk mArrows[2]` (el original las recorre en bucle),
  `m178` -> `TutorialPopup*`. Solo esta unidad incluye `game/CampSaveLoad.h`.
- `GameSaveSlot_unk*` por cast desde `SaveSlot_unk*` (vista local de CampSaveLoad del mismo slot de 0xB8 bytes):
  deuda de la regla 10, pendiente unificar.
- CampSaveLoad usa `GameConfig_unk` (incluye `game/GameConfig_unk.h`) en vez de la vista local `Settings_unk`.
- Vista local `CampSaveLoadMsg_unk` de la tabla de mensajes `lbl_8087F1E4` (como CosEditItemList_unk): la
  cabecera `MP/MpCampMsgTable_unk.h` incluye `MP/D2AMpMenu_unk.h`, que redefine `atn::D2A_Task`. Deuda de la
  regla 10 del lado MP.
- `fn_801F4D80(name, (int)path.c_str())` como en NewCampPartsMapSelect (la firma de `atn/D2A_Task.h` tiene `int`).
- `CampSaveLoad::GetSlot_unk(int)` inline nueva en `game/CampSaveLoad.h` (el original carga el indice antes de
  `GetData_unk()`).
- `ReplayBuffer::fn_8056C3D8()` y `fn_8056F5B4(int)` como miembros en `game/ReplayBuffer.h` (5 unidades; sin
  nombres de parametro, no renumeran). GameTask llamaba a `fn_8056C3D8` como `extern "C"` con `self`: ahora
  `m5590->fn_8056C3D8()` (se quita la declaracion local, que tenia un parametro con nombre: GameTask renumera -1,
  re-emparejada con `pdbatch.sh` y dos etiquetas de `.sdata2` corregidas a mano, 0x808857A8/0x808857AC).
- `ReplayFrame_unk`: `mLeaderMtx` (0x17D8) en la cabecera; las 32 balas (0x1558) siguen como bytes en la cabecera
  y la struct `ReplayBullet_unk` esta en el `.cpp`: un array de una struct con `atn::Vector3` en la cabecera renumera
  los `@N` de Arms, Bullet y HomingBullet (+5). Vista local `ReplayFrameTime_unk` de `lbl_8087EFA8` (como en
  GimmickStateCommand/AiTarget).
- Renombrados en `symbols.txt`: `vf0C/vf10/vf14__12CampSaveLoadFv`, `fn_8011B018__12CampSaveLoadFiP12SaveSlot_unk`,
  `fn_801191A8/fn_8011999C/fn_80119F0C/fn_8011A4D0__12CampSaveLoadFv`.

## Hallazgos

- Producto vectorial en una inline `Cross_unk(a, b)` que devuelve por valor y comprobacion de linea
  `LineHit_unk(const Vector3&, const Vector3&)` con temporales (`from + side`) dan los huecos de pila del original
  (RouteWalker_unk::fn_80128CF8).
- `RoutePoint_unk* GetPoint_unk(table, i)` inline (`&mPoints[i - 1]`) arregla el reparto de registros frente a
  indexar `mPoints[i - 1]` directamente (fn_80129670).

- Dos actualizaciones identicas seguidas (flechas de CampSaveLoad::vf10) con las constantes cargadas una vez: era
  un bucle `for (i < 2)` que MWCC desenrolla, no dos llamadas a la inline.

- Dos formas de poner texto/valor en un panel d2anime: por tarea (`fn_801FEE08(&task->mData.m10, hash, t)`, el
  puntero de la tarea queda en un registro) y por layout (`SetLayoutText_unk(&task->mData.m10, name, t)`: la
  direccion del layout se calcula antes del hash). CampSaveLoad usa las dos (fn_80119F0C 85 -> 99 %,
  fn_801191A8 98,6 -> 99,98 %).
- Una conversion int -> float muerta (`lis 0x4330` + `xoris` + dos `stw`, sin `lfd`) es un local `float` sin usar
  (`float hour = save->m10;`, CampSaveLoad::fn_80119F0C).

- Un miembro array de una struct con `atn::Vector3` dentro de una struct con constructor inline en una cabecera
  renumera los `@N` de las unidades que la incluyen (ReplayFrame_unk: +5); un `atn::Matrix3x4` suelto o una
  declaracion de metodo sin nombres de parametro no.

## Pendientes

- RouteWalker_unk `fn_801255C8`, `fn_80126214`.
