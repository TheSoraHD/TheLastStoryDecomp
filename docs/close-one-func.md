# close-one-func (2026-10-30)

Objetivo: cerrar unidades sin enlazar a las que les falta una sola función por debajo del 95%, de menor
a mayor tamaño. Trabajo secuencial, rama `close-one-func` desde `main` 442aebd.

Resultado: 8765 → 8768 funciones según `report.json` (+6 según `fnstat.py`: tres ya contaban por fallar
solo un nombre), 375 → 378 unidades enlazadas, `main.dol: OK`, lost 0, sin bajadas de fuzzy.

## Por unidad

| Unidad | Resultado | Clave |
|---|---|---|
| `gmk/TodomeCountUp` | **enlazada** | `vf10`: `if (m54 == 0) Start(); if (m54 == 0) return;` (forma de `FloatingBox`/`OpenBoard`/`Trap::vf10` con el cuerpo vacío) mantiene el marco, `this` en r31 y la llamada con `bctrl`. Destructor fuera de línea tras el constructor (inline en la cabecera se emitía al final del `.o`). |
| `gmk/CharaObj` | **enlazada** | `vf10`: igual, pero con la comprobación sobre un global (`lbl_8087F0A8 == 0`): marco sin r31. `GmkChara` declarada tras `CharaObj` (declaración adelantada en `CharaObj.h`, `#include` en el `.cpp`) para que su vtable vaya primero en `.data`. |
| `atn/TaskBase` | **enlazada** | `static const unsigned long long sTaskTag`: la carga de la etiqueta se planifica por encima de los stores del contador de ids. Sigue en `.bss` (init dinámica). |
| `game/LastCocoonChrysalis` | `vf10` al 100%, ctor 0% | `m14CC` es `CopyRobotTask` (`fn_801CCD00` miembro); ctor: `mFlag22 = 1` (0x12A4, no `m14A8`), `it->c_str()` (`operator->`), `fn_8006AFF8` compartida en vez de una `static` propia. El ctor original está por encima del presupuesto de inline (R3): con una candidata artificial de 36+ sentencias (33 no basta) queda solo en registros (89%). No se encontró la forma natural. |
| `game/GameSaveBanner` | 88,2 → 99,18% | índices `(unsigned int)i` en los tres arrays del bucle (`this` + desplazamiento); `mFlags` antes de `size`. Quedan el orden de argumentos de `NANDInitBanner` y r26/r27 del bucle. |
| `gmk/HealPoint` | 93,75% (sin cambio) | Planificación del ctor inline (carga de `mRotY` antes de la posición; z antes que xy en la copia). 8 formas nuevas en el comentario. |
| `gmk/ShakeCamera` | 92,33 → 96,74% | Escritura del campo de vibración por una inline `Shake_unk(frame, x, y)`: los miembros se leen antes de los stores. Quedan la comparación de `case 1` y f30/f31. |
| `atn/EfpHandle_unk` | 74,74% (sin cambio) | Mismo bloqueo que los intercambiadores de `game/HColliData_unk` (bucles desenrollados; `__sthbrx`/`__stwbrx` empeoran a 66–67%). |
| `game/StrapTask` | ctor y `vf10` al 100%, `vf14` 85,9% | `lbl_8087F4E8` = `HomeButtonMenu::sInstance`. `vf14`: FPR y constantes recargadas, sin tocar. |
| `Event/Section` | 41,2% (sin cambio) | Residuo del `push_back` de `std::vector` (crecimiento), no abordado. |

## Decisiones tomadas sin el usuario

- `StrapTask.cpp` declara una vista parcial de `HomeButtonMenu` (solo `m88`/`m90`), igual que
  `game/GameSystem_unk.cpp` y `game/main.cpp`. Una cabecera única exige unificar antes `atn::IRender`
  (8 copias locales con firmas distintas); queda pendiente.
- Las condiciones de las comprobaciones vacías de `TodomeCountUp::vf10` (`m54 == 0`) y `CharaObj::vf10`
  (`lbl_8087F0A8 == 0`) son supuestas: el código que las usaba no está en el `.dol`.
- `LastCocoonChrysalis.h` incluye `game/CopyRobotTask.h` y desaparece `LastCocoonPart_unk`.
