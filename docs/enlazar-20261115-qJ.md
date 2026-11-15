# enlazar-20261115, cola J (datos y medicion)

## Resultado por unidad

| Unidad | Antes | Despues | Notas |
|---|---|---|---|
| `key/src/revolution/WPAD/WPADHIDParser.c` | 33/33, no enlaza | 33/33, **enlaza** | el objeto de 0x10 en 0x8082DDF0 no existe: es relleno por la alineacion 32 del `.bss` de WUD |

## Unidades que enlazan (bytes)

- `WPADHIDParser`: +544 (`complete_data` 1005158 -> 1005702).
- `.sbss` absorbidos por unidades enlazadas: LinearAllocator +8, SequenceControl +8, EnemyManager +8, GameTimer +8 (`complete_data` 1005702 -> 1005734).

## NONMATCHING con motivo

## Cambios de splits

- `smg2/src/RVL_SDK/wud/WUD.c` `.bss` con `align:32`. Prueba: WUD.c tiene `__WUDHandlerStack[0x1000] ATTRIBUTE_ALIGN(32)` (nuestro WUD.o compilado da `.bss` con alineacion 32) y `_wcb` esta en 0x8082DE00; `gap_08_8082DDF0_bss` no tiene referencias. WUD no enlaza, asi que se usa su objeto generado por dtk, que llevaba alineacion 8: con WPADHIDParser desde fuente (`.bss` 0x50) `_wcb` caia en 0x8082DDF0. Con `align:32` build limpio `main.dol: OK`.
- `.sbss` de los `sInstance`/globales que estaban en tramos `auto_10_*` (prueba: la clase dueña del estatico y el orden de `.text`; la unidad dueña esta entre la anterior y la siguiente con `.sbss` y no tenia `.sbss`):
  - `atn/LinearAllocator.cpp` 0x8087EF88-0x8087EF90: `lbl_8087EF88` (maximo del buffer temporal, `PostFxManager::fn_800A9794`) y `lbl_8087EF8C` (instancia de `PostFxManager`, `fn_800A8FF0`), definidos en el fuente.
  - `atn/SequenceControl.cpp` 0x8087EFC0-0x8087EFC8: `SequenceControl::sInstance`.
  - `game/EnemyManager.cpp` 0x8087F408-0x8087F410: `EnemyManager::sInstance`.
  - `game/GameTimer.cpp` 0x8087F488-0x8087F490: `GameTimer::sInstance`.
  - `game/GameSaveBanner.cpp` 0x8087F470-0x8087F478: `GameSaveBanner::sInstance` (ya definido en el fuente; la unidad no enlaza, fn_803B9638 al 99,18 %).

## Para el port

- Sin cambios.

## Decisiones tomadas sin el usuario

- `align:32` en el `.bss` de WUD (ver arriba) en lugar de anadir un array falso de 0x10 a WPADHIDParser.

## Hallazgos (una linea cada uno)

- La nota de qD ("el enlazador no alinea por encima de 16 en `.bss`") era falsa: se enlazaba el objeto generado de WUD (alineacion 8), no el compilado.

## Medicion de `strlen` intrinseca (cola J.3)

Prueba: `include/std/string.h` renombrada a `include/std/basic_string.h` (y los 147 `#include "std/string.h"`,
incluidos `libs/boost/msl_shim/string` y `stdexcept`), build completo frente al `report.json` de antes:

- Suben (fuzzy): `AI/AI_archer_attack01 fn_8001EA18` 90,75 -> 98,22; `game/TutorialTask fn_805BB654` 83,77 -> 85,02. Ninguna llega al 100 %.
- Bajan: `game/GimmickCommon fn_803EDB18` **100 -> 96,61** y `atn/ptr_string_base_char_0 VoiceEntry_unk::fn_8021FD7C` **100 -> 97,7** (las dos enlazadas: el `.dol` deja de dar OK); `AI_archer_attack01 fn_8001E1D0` 94,07 -> 90,61; `MaterialParser_unk fn_8007C46C` 97,31 -> 96,28 y `fn_8007B260` 99,84 -> 99,08.
- Balance: 15563 -> 15561 (lost 2). No se integra. Parche: `docs/enlazar-20261115-qJ-strlen.patch` (renombrado + includes).
- Lectura: el original trae `strlen` intrinseca en unas unidades (las que bajan) y no en otras (las que suben); no es una propiedad de la cabecera compartida sino de lo que incluya cada fichero. Lo correcto es por unidad: `#include "std/msl_common.h"` antes de `std/string.h` donde haga falta (como hizo qA en Sequence).

## Pendientes
