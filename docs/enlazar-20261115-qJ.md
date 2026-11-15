# enlazar-20261115, cola J (datos y medicion)

## Resultado por unidad

| Unidad | Antes | Despues | Notas |
|---|---|---|---|
| `key/src/revolution/WPAD/WPADHIDParser.c` | 33/33, no enlaza | 33/33, **enlaza** | el objeto de 0x10 en 0x8082DDF0 no existe: es relleno por la alineacion 32 del `.bss` de WUD |

## Unidades que enlazan (bytes)

- `WPADHIDParser`: +544 (`complete_data` 1005158 -> 1005702).
- `.sbss` absorbidos por unidades enlazadas: LinearAllocator +8, SequenceControl +8, EnemyManager +8, GameTimer +8 (`complete_data` 1005702 -> 1005734).
- `gmk/CharaObj` +8: los dos `""` de `new` de `gmk::CreateChara_unk` (0x8087DFC0) (`complete_data` 1005734 -> 1005742).

## NONMATCHING con motivo

- `game/GameSaveBanner fn_803B9638` 99,18 % (ultima funcion de la unidad; enlazaria 352 + 8 bytes): sin cambio con
  `2 * i` y cast `(unsigned short)` (forma de `NANDSetIconSpeed`), puntero fuente local antes del `memcpy`,
  indices con signo, `speed` local con `&=`/`|=`, `|` invertido, `struct NANDBanner*`/`void*`/`const unsigned
  short*` en `NANDInitBanner`, inline `InitBanner_unk` (con y sin `memset`) y todo el cuerpo en una inline con
  puntero. Siguen los registros r26/r27 (3 y desplazamiento), el incremento del desplazamiento de icono al final
  y los argumentos de `NANDInitBanner` calculados del ultimo al primero.

## Cambios de splits

- `smg2/src/RVL_SDK/wud/WUD.c` `.bss` con `align:32`. Prueba: WUD.c tiene `__WUDHandlerStack[0x1000] ATTRIBUTE_ALIGN(32)` (nuestro WUD.o compilado da `.bss` con alineacion 32) y `_wcb` esta en 0x8082DE00; `gap_08_8082DDF0_bss` no tiene referencias. WUD no enlaza, asi que se usa su objeto generado por dtk, que llevaba alineacion 8: con WPADHIDParser desde fuente (`.bss` 0x50) `_wcb` caia en 0x8082DDF0. Con `align:32` build limpio `main.dol: OK`.
- `.sbss` de los `sInstance`/globales que estaban en tramos `auto_10_*` (prueba: la clase dueña del estatico y el orden de `.text`; la unidad dueña esta entre la anterior y la siguiente con `.sbss` y no tenia `.sbss`):
  - `atn/LinearAllocator.cpp` 0x8087EF88-0x8087EF90: `lbl_8087EF88` (maximo del buffer temporal, `PostFxManager::fn_800A9794`) y `lbl_8087EF8C` (instancia de `PostFxManager`, `fn_800A8FF0`), definidos en el fuente.
  - `atn/SequenceControl.cpp` 0x8087EFC0-0x8087EFC8: `SequenceControl::sInstance`.
  - `game/EnemyManager.cpp` 0x8087F408-0x8087F410: `EnemyManager::sInstance`.
  - `game/GameTimer.cpp` 0x8087F488-0x8087F490: `GameTimer::sInstance`.
  - `gmk/CharaObj.cpp` `.sdata` 0x8087DFC0-0x8087DFC8: los dos `""` que pasan `gmk::CreateChara_unk` a `operator new`. Prueba: los usan CharaObj (inline en vf20) y CharaPathMove (fuera de linea, 0x8043E134), asi que son objetos debiles `@STRING@CreateChara_unk__3gmk...` (no literales locales); CharaObj es el primero en el orden de enlace con un uso (FloatingBox, entre OpenBoard y CharaObj, no lo usa). `CreateChara_unk` pasa de `static` a `inline` en CharaPathMove (mismo codigo y orden) y se define inline tambien en CharaObj.cpp; simbolos renombrados de `lbl_8087DFC0/C4`.
  - `game/GameSaveBanner.cpp` 0x8087F470-0x8087F478: `GameSaveBanner::sInstance` (ya definido en el fuente; la unidad no enlaza, fn_803B9638 al 99,18 %).

## Para el port

- Sin cambios.

## Decisiones tomadas sin el usuario

- `align:32` en el `.bss` de WUD (ver arriba) en lugar de anadir un array falso de 0x10 a WPADHIDParser.
- `strlen` (J.3) y `Csv()` (J.2) no integrados: el primero baja dos funciones enlazadas; el segundo da +8 bytes a cambio de re-emparejar 267 unidades. Quedan como parches en `docs/`.
- `gmk::CreateChara_unk` definida `inline` en CharaObj.cpp y en CharaPathMove.cpp (no en `GmkChara.h`): en la cabecera CharaPathMove la emite antes que sus otras debiles y sale fuera de orden.

## Hallazgos (una linea cada uno)

- Literales de una funcion `inline` (expandida o no) salen como objetos debiles `@STRING@<funcion>[@0]` en `.sdata` si miden <= 4: un `""` compartido por dos unidades sin ser `@stringBase0` es eso; los define la primera unidad del orden de enlace que usa la inline (CharaObj 0x8087DFC0).
- Una inline definida en el `.cpp` despues de su uso (declarada antes) sale fuera de linea en el sitio de la definicion, igual que una `static`, pero sus literales pasan a ser debiles (CharaPathMove `CreateChara_unk`).

- La nota de qD ("el enlazador no alinea por encima de 16 en `.bss`") era falsa: se enlazaba el objeto generado de WUD (alineacion 8), no el compilado.

## Tramos `auto_*` (cola J.2)

De los 49 tramos (924 B), los que reciben unidades enlazadas: LinearAllocator, SequenceControl, EnemyManager,
GameTimer (`.sbss`, +32) y CharaObj (`.sdata`, +8). Los demas, revisados con el orden de `.text` y las
referencias (`owners.py`/`between.py` en scratch-J):
- Rellenos de ceros sin simbolo entre dos enlazadas con la siguiente alineada a 8 (0x80734180 Sound3dListener|Sound3dEngine, 0x807773A0 antes de Arms): son datos de 8 bytes de una de las dos (sin referencias), no se pueden absorber sin inventar el objeto.
- 0x807A9AE0 (192 B, tras OSNet): cadenas de las versiones debiles de NWC24 (`NWC24iPrepareShutdown`, `/dev/net/kd/request`...) que el `OSNet.c` del SDK define con `DECL_WEAK`; el enlazador quita las funciones (ganan las de NWC24System.c) y deja las cadenas. Probado el `OSNet.c` completo de `libs/RVL_SDK` adaptado a las cabeceras de smg2: `__OSInitNet` inlinea las debiles (0x360 bytes en vez de 0xC0) y el orden del pool cambia; revertido (20 min).
- 0x8087D720 (`" \t"` de `Csv()`, tras AudioDevice): con el literal en `include/atn/Csv.h` sale como debil `@STRING@__ct__Q23atn3CsvFv` en AudioDevice y el `.dol` da OK (+8), pero el literal consume un `@N` en las 267 unidades que incluyen Csv.h: `pdbatch` de todas reescribe ~6200 lineas de `symbols.txt` y aun quedan labelcheck peores y un `ninja` roto. No integrado; parche `docs/enlazar-20261115-qJ-csv.patch`.
- `lbl_8087D710` (`" "`, tras 0x8087D708): lo usan ~60 fuentes con `extern` explicito (Arms el primero enlazado); seria la misma operacion con mas ficheros.
- El resto: el dueno (primer usuario de la debil o clase del estatico) no enlaza: stored_vtable de boost (ControlWall*, ControlAppear...), `@STRING@` de SimpleVector/D2A_Buffer, `lbl_8087F430/34` y `lbl_8087DCC8/CC` (GameTask), `CrowdSimulation::sInstance` (CrowdSimulation), `lbl_8087F498` (GameVoice), `lbl_8087EFE8` (SoundManager), `lbl_8087F8A8` (PopupTask), `lbl_8087EE68` (AiCommon_unk), `RenderManager::instance_` y vecinos (RenderSystem_unk), `GameSoundControl::sInstance`.

## Medicion de `strlen` intrinseca (cola J.3)

Prueba: `include/std/string.h` renombrada a `include/std/basic_string.h` (y los 147 `#include "std/string.h"`,
incluidos `libs/boost/msl_shim/string` y `stdexcept`), build completo frente al `report.json` de antes:

- Suben (fuzzy): `AI/AI_archer_attack01 fn_8001EA18` 90,75 -> 98,22; `game/TutorialTask fn_805BB654` 83,77 -> 85,02. Ninguna llega al 100 %.
- Bajan: `game/GimmickCommon fn_803EDB18` **100 -> 96,61** y `atn/ptr_string_base_char_0 VoiceEntry_unk::fn_8021FD7C` **100 -> 97,7** (las dos enlazadas: el `.dol` deja de dar OK); `AI_archer_attack01 fn_8001E1D0` 94,07 -> 90,61; `MaterialParser_unk fn_8007C46C` 97,31 -> 96,28 y `fn_8007B260` 99,84 -> 99,08.
- Balance: 15563 -> 15561 (lost 2). No se integra. Parche: `docs/enlazar-20261115-qJ-strlen.patch` (renombrado + includes).
- Lectura: el original trae `strlen` intrinseca en unas unidades (las que bajan) y no en otras (las que suben); no es una propiedad de la cabecera compartida sino de lo que incluya cada fichero. Lo correcto es por unidad: `#include "std/msl_common.h"` antes de `std/string.h` donde haga falta (como hizo qA en Sequence).

## Pendientes

- `GameSoundControl::sInstance` (0x8087F480) y `CrowdSimulation::sInstance` (0x8087F130), `lbl_8087F498` (GameVoice), `lbl_8087EFE8` (SoundManager): anadir su `.sbss` a esas unidades cuando se trabajen (no enlazan; sin ganancia ahora).
- `Csv()`/`CsvTab(const void*)`/`Csv(text, len)` con literales en `Csv.h` (0x8087D720/D710/D708): solo merece la pena en un lote que re-empareje todo de una vez (consume un `@N` en cada unidad que incluye Csv.h).
- `OSNet.c` con las funciones debiles de NWC24 (192 B): falta una version cuyas debiles no se inlineen en `__OSInitNet`.
