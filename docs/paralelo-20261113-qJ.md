# paralelo-20261113, cola J (juego varios 2)

## Resultado por unidad

- `game/RouteWalker_unk` 6 -> 2 al 0 %. Al 100 %: `fn_80128CF8` (meta a la vista), `fn_80129670` (segmento entre
  dos puntos). Escritas: `fn_80128090` (97,05 %, replanifica el modo actual), `fn_80129144` (98,97 %, va directo a
  la meta). Quedan `fn_801255C8` (3 KB) y `fn_80126214` (6 KB).
- `game/CampSaveLoad` 8 -> 3 al 0 %. Al 100 %: `vf10` (0x801185E0). Escritas: `vf0C` (92,83 %), `fn_8011B018`
  (99,33 %), `fn_801191A8` (98,58 %, estado 1), `fn_8011999C` (92,34 %, estado 5). Sube `fn_8011B8D4` 95,79 -> 99,93 % (pool de cadenas).

## NONMATCHING

- `RouteWalker_unk::fn_80129144` 98,97 %: solo registros (tabla en r5 y `m44` en r4 en el original). Probado: local
  para `m44` antes y despues de la tabla, tabla pedida en el caso 3, getter inline del sitio, orden de los casos,
  bandera de `m48` tras el switch.
- `RouteWalker_unk::fn_80128090` 97,05 %: solo registros de los planificadores inlineados (el destino en r26).

- `CampSaveLoad::vf0C` 92,83 %: un registro salvado de mas; el original toma `""` de `@stringBase0 + 0x3D` con la
  misma base (pool aun incompleto: faltan vf14 y los ayudantes de estado).
- `CampSaveLoad::fn_8011B018` 99,33 %: offsets del pool y registros r23/r24 (tamano/puntero) de los dos `+=`
  cruzados; probado `append` y `replace(size(), 0, s)`.
- `CampSaveLoad::fn_801191A8` 98,58 % y `fn_8011999C` 92,34 %: registros del layout de `m60` en los textos de
  aviso (el original lo toma antes del hash del nombre), indice/puntero del ultimo bucle cruzados, pool. Los avisos
  repetidos no son una inline (los temporales de `SoundRef` estan arriba en la pila, en el orden del fuente).

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
- `fn_801F4D80(name, (int)path.c_str())` como en NewCampPartsMapSelect (la firma de `atn/D2A_Task.h` tiene `int`).
- `CampSaveLoad::GetSlot_unk(int)` inline nueva en `game/CampSaveLoad.h` (el original carga el indice antes de
  `GetData_unk()`).
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

## Pendientes

- RouteWalker_unk `fn_801255C8`, `fn_80126214`.
