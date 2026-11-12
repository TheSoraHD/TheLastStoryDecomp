# Lote paralelo 2026-11-12, cola D2 (segunda pasada: interfaz)

Agente D2, rama `agent-D2`. Inicio: 2026-11-12 20:43 CET (tope ~2,5 h).
Unidades: `game/GameUI`, `game/MiniMapTask`, `game/FinishCamera_unk`, `game/PopupTask`; despues `game/GameCamera_unk`.

## Resultado por unidad

### game/GameUI (27 al 0 %)

- `fn_803DF648` (ventanas de los personajes del HUD, 932 bytes): escrita, 99,15 %.
- Ordenacion de esas ventanas (MSL `__sort`, como game/GameTask.cpp): `GameUISort_unk<..., GameUIWindowLess_unk>`
  0x803DF9EC 99,94 %, la copia por referencia 0x803E0250 99,94 %, `GameUIMedian3_unk` 0x803E0AB4 **100 %**.
  Renombradas en `symbols.txt` (tambien los contadores `@LOCAL@...@sOffset` de `.sdata` 0x8087DEF0/4).
- `fn_803DE89C` (marcador de un efecto de habilidad, 1416 bytes): escrita, ~96 % (r5/r6 cruzados al principio;
  el resto solo difiere en `@N` y en los offsets de `@stringBase0`, porque el pool de la unidad no esta en orden).

### game/MiniMapTask (10 al 0 %)

- Cabecera: la zona 0x834-0x1538 son `MiniMapMarker_unk mMarkers[64]` (0x2C) y
  `FixedArray_unk<MiniMapCharaMark_unk, 64> mCharaMarks` (0x1334); el constructor (borrador) se ajusta.
- `fn_804AABA8` (anade un marcador; parametros **por valor**: `Vector3`, `Color`, `float`) **100 %**,
  `fn_804AAD54` (quita un marcador) **100 %**, `fn_804AAFCC` (quita la marca de un personaje) **100 %**,
  `fn_804AAD68` (marca un personaje) 99,58 %.
- `fn_804AA490` (lee el csv del minimapa: "LOCATION", "RADAR", "POINT", "ARROW", "OFF_AREA", "LOWER_LAYER")
  99,67 %; la cabecera gana los campos que lee (m5EC-m5F8, `atn::Color m600/m610/m620`, m630-m63C,
  `FixedArray_unk<MiniMapArea_unk, 8> m1538` (dos `Vector2_unk`: la copia con dos `psq_l/psq_st`), m15F4-m1608).
- `fn_804A9EB4` (coloca el marco y dibuja los marcadores activos) 82 %: `m5D8` pasa a `atn::D2A_Task*`.
- `game/ChaseManager` llamaba a fn_804AABA8/fn_804AAD54 como `extern "C"` con un `void*`: ahora incluye
  `game/MiniMapTask.h` y llama a los metodos (fn_80183618 sigue en 99,60 %).

### game/GameCamera_unk (27 al 0 %)

- `fn_80383F7C` (giro de la camara con el stick alrededor de un punto; funcion libre, no metodo): 98,4 %.

### game/FinishCamera_unk, game/PopupTask

- Sin cambios: funciones unicas de 13216 y 9328 bytes.

## NONMATCHING nuevas

- `GameUI::fn_803DF648` 99,15 %: el original relee `lbl_8087F610` y su numero de jugadores dentro del bucle (aqui
  reutiliza la carga de la condicion) y cruza r29/r30.
- `GameUISort_unk` (las dos copias) 99,94 %: el `min_element` inline cruza r28/r29 (como game/CharaTask.cpp).
- `GameUI::fn_803DE89C` ~96 %: registros del campo y del lider (r5/r6).
- `MiniMapTask::fn_804AAD68` 99,58 %: registros de la llamada inline a fn_804AABA8.
- `fn_80383F7C` 98,4 %: huecos de pila del vector girado y registros float del clamp.

## Decisiones tomadas sin el usuario

- `CharaInfo_unk` (`game/CharaTask.h`) gana `int m2C` (miembro de datos al final: no renumera `@N`).
- `GameUI.h`: `m10B4` pasa de `atn::Task*` a `GameUIInst_unk*` (cambio de tipo de un miembro, sin declaraciones).
- `MiniMapTask.h`: incluye `game/CharaStatus.h` (para `FixedArray_unk`) y declara `fn_804AABA8`/`fn_804AAD54`;
  re-emparejados con `pdbatch` MiniMapTask, ChaseManager, ChaserGuard y GameEventScript (los que la incluyen).
- `lbl_8087F610` en GameUI.cpp pasa de `void*` a `GameUISession_unk*` (tipo local de la unidad, ampliado con los
  jugadores 0xD5C).
- `GameCamera_unk.cpp`: macros `GAMECAMERA_MAX_UNK`/`GAMECAMERA_MIN_UNK` (el original evalua dos veces el
  segundo operando del clamp, como una macro).

- `pdbatch` dejo `@N` sin re-emparejar en GameCamera_unk (5), GameEventScript (3) y MiniMapTask (3) por
  pares ambiguos de `pairdata` (la funcion nueva al 98 % aporta referencias cruzadas): renombrados a mano en
  `symbols.txt` por valor (unico candidato con el mismo valor). `labelcheck/rep100: igual`.
- `poolcheck`: GameCamera_unk avisa de 0.2 (fn_80383F7C usa el literal; el original lee `lbl_808859E0`, float
  con nombre del pool de la unidad).

## Pendientes

- `GameUI`: casi todas las funciones al 0 % restantes (fn_803CFC78, fn_803D022C, fn_803D0B20, fn_803D164C,
  fn_803D2194, fn_803D83A8, fn_803DAB4C...) son metodos de GameUI sin declarar (GameTask las llama como
  `extern "C"` con un `GameUI*`): declararlas en `GameUI.h` renumera los `@N` de 35 unidades de varias colas.
  Hacerlo en un lote de integracion.
- `GameUI::fn_803E2110`: `push_back` de `std::vector` en m30C (`GameUIList_unk` es una vista a mano del vector);
  pasar m30C a `std::vector` toca funciones ya al 100 % (destructor, fn_803E0D84).
- `GameUI fn_803E78B4`: `__sinit` de la unidad (tabla de colores de `.bss` y la tabla de `.data` 0x8078BE40).
- `TownTask::m258` es el `MiniMapTask*` (ChaseManager lo convierte); cambiar el tipo en `TownTask.h` (otra familia).
