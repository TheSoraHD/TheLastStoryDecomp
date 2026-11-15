# enlazar-20261115 - cola W

Cola W (unidades a 2-3 funciones con mas datos): atn/D2A_Manager, game/GameConfig_unk, chr/ControlDamage,
db/PaletteTable_unk, atn/THPPlayer_unk, chr/ControlInteractionBase, game/Em309GhoulSimple, game/EnemyQuarkFirst,
game/NewCampPartsGmk. Agente W, 20:17 -> 22:45.

## Resultado por unidad

| Unidad | Antes | Despues | Nota |
|---|---|---|---|
| db/PaletteTable_unk | 12/14 | 13/14 | `fn_8020BB0C` 100 %; `fn_8020B2BC` 98,03 -> 99,96 (solo registros) |
| game/GameConfig_unk | 12/15 | 13/15 | `DialogConfig_unk::fn_801856B8` 100 % |
| game/EnemyQuarkFirst | 12/15 | 13/15 | `vf104` 100 % (`world` local + 2 `VecAssign_unk`) |
| chr/ControlDamage | 38/41 | 38/41 | `ControlDamageLastOne::vf0C` 99,4 -> 99,64 sin commit (ver Pendientes) |
| atn/THPPlayer_unk | 18/22 | 18/22 | probadas sin cambio |
| chr/ControlInteractionBase | 18/21 | 18/21 | probadas sin cambio |
| game/Em309GhoulSimple | 19/22 | 19/22 | `vf100` 99,84 -> 99,96 sin commit |
| atn/D2A_Manager, game/NewCampPartsGmk | - | - | no atacadas (cabeceras compartidas de vector/Scene) |

## Unidades que enlazan (bytes)

Ninguna: en todas las de la cola queda al menos una funcion de registros o de cabecera compartida.

## NONMATCHING con motivo

- `db/PaletteTable_unk fn_8020B2BC` 99,96 %: solo el registro del nombre UTF-16 leido en los bloques 2 y 4
  (original r30/r29, aqui r27). Probado: local en el uso, en el bloque, en la funcion, reutilizar `field`.
- Resto: comentarios existentes sin cambio.

## Cambios de splits

Ninguno. `symbols.txt` solo con los renombrados de `@N` del re-emparejado (`pd.sh`).

## Para el port

Nada (ninguna unidad del port tocada).

## Decisiones tomadas sin el usuario

- PaletteTable: `FindColor_unk`/`FindEffColor_unk` (inlines estaticas) se quitan: el bucle escrito en el sitio da
  los registros del original. El array sin usar de `fn_8020BB0C` es `Alpha_unk alphas[4]` (4 bytes, ctor 1.0f).
- GameConfig_unk: los menus Left/Right pasan a una `static inline AddSideMenu_unk(folder, DialogSide_unk&)`.
- EnemyQuarkFirst: `#include "math/VecAssign_unk.h"` y dos `VecAssign_unk`.

## Hallazgos (una linea cada uno)

- Un array local sin usar de N objetos con ctor inline ocupa N*N*sizeof en la pila (MWCC): el patron de huecos
  de las stores (indices 0,5,10,11 para N=4) da el tamano del elemento (PaletteTable `fn_8020BB0C`: 4 bytes).
- Reutilizar la misma variable para el campo de control y el valor (`name = csv.fn_8005B9CC()` dos veces) cambia
  el orden de colores igual que declararla antes del puntero de datos (PaletteTable).
- Una busqueda en tabla escrita como inline estatica `Find_unk(name)` da otros registros que el bucle en el
  sitio; el original tenia el bucle en el sitio (PaletteTable 98,0 -> 99,96).
- Seis llamadas de menu sobre un struct repetidas para dos miembros (Left/Right) = una inline con
  `(folder, Side&)`: da al folder el registro del original (GameConfig `fn_801856B8`).
- Bucle con `names[i]` y `&mScenes[i][0]` dentro (indices) en vez de punteros incrementados fuera: el orden
  de creacion de las variables de induccion sigue al del original (GameConfig).
- Global pasada como primer argumento junto a una llamada en otro argumento: `world = lbl_...;` antes de la
  llamada la carga antes (en registro salvado) como el original (EnemyQuarkFirst `vf104`).
- `VecAssign_unk(dst, a - b)` arregla tambien copias a locales (no solo a miembros) con r3/r4 cruzados.

## Pendientes

- ControlDamageLastOne::vf0C 99,64 con `CharaTask* c1 = mChara; c1->m6B8 = m08; CharaTask* c2 = ...` (caso 11)
  y `atn::VecAssign_unk(side, ...)`; queda el bool del helper inline (r30/r31 cruzado con el modelo).
- Em309GhoulSimple::vf100 99,96 con `atn::Vector3 back = GhoulNeg_unk(front); m1540 = back;`: quedan los
  huecos de pila de `d` (estado 9-11) y `back` cruzados.
- THPSimpleSetBuffer: el original comparte el registro del 0 entre isValid y validSample; probado como C,
  `long` en u32/s32 (SDK), BOOL en validSample.
- PaletteTable `fn_8020B2BC`: con la llamada directa `sPalettes[index].SetName_unk(csv.fn_8005B9CC())` en el
  bloque 2 quedan nombre/destino cruzados (r28/r30); con local `text` el nombre cae en r25 (el de `index`, que
  en el original sigue vivo tras la llamada). Variantes del cuerpo de `SetName_unk` (src/dst): sin cambio.
- ControlKnockdown ctor: `std::string(path, strlen(path))` peor (98,7).
- ControlInteractionBase DiveIntoWater ctor/vf0C: `GetBodyPos_unk` por valor, `c->mPos`, `VecAssign_unk`,
  local `pos`: sin mejora (el original da r31 a `ret`/`this` y el registro mas bajo a la direccion de `pos`).
- EnemyQuarkFirst: tras `pd` hizo falta renombrar a mano `@10681` -> `@10699` (0x808851B0, como en qL2).
- THPSimpleDecode 99,81: 120 permutaciones de las declaraciones, `readBuffer` local y AudioDecode inline:
  sin cambio.
