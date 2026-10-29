# enemies-third, cola n3a: game/Em203GreaterDaemon

## Resultado

| Unidad | Antes | Después | Total | Enlazada |
|---|---|---|---|---|
| `game/Em203GreaterDaemon` (0x80287E7C) | 0 (sin fuente) | 36 | 66 | no (`matching: false`) |

Proyecto: 7299 → 7335 funciones al 100%, `main.dol: OK`, `regress.py` lost: 0, `fuzzdrop.py` sin bajadas, `poolcheck.py` sin avisos. El orden de `.sdata2` y `@stringBase0` coincide con el original. Lo único que queda son las entradas finales del pool, que no tienen referencias (pertenecían a funciones eliminadas por el enlazador).

## NONMATCHING (27, con el motivo en el fuente)

- **Casi al 100%, con bloqueos conocidos:**
  - ctor, 99,86%: intercambio de registros en el segundo `std::string`.
  - vf40, 99,90%, y fn_8028FC28, 99,80%: cruce r6/r7 en la copia psq de un vector temporal.
  - fn_8028E800, fn_8028EDC4 y fn_8028EF20, 98–99,6%: orden de los stores r4/r5 de la mirada de cámara (como en Em101LittleZesyaSecond).
  - fn_8028BFE0, 99,2%: orden f0/f3 tras la longitud de la animación y huecos de pila.
- **fn_8028CB38, 0%:** el original supera el presupuesto de inline (R3), así que llama fuera de línea a fn_802913B0, fn_800109E0 y fn_8028CFB0. El nuestro los inlinea (0xE4C frente a 0x478). No encontré una forma natural de pasarse del presupuesto sin hinchar la función.
- **fn_8028AC70, 80%:** spline débil (Catmull-Rom). Es la asignación de registros float: el original guarda f28.
- **Resto (74–97%):** huecos de pila y asignación de registros en funciones grandes con matrices y vectores temporales. Son vf10, vf3C, fn_8028AFD8, fn_8028B47C, fn_8028CFE0, fn_8028D5C8, fn_8028DF34, fn_8028E1A0, fn_8028E488, fn_8028F1D0, fn_8028F560, fn_80290088, fn_80290674, fn_80290AB4, fn_8029101C, fn_802915F4, fn_802921F8 y fn_80292DB4.

## Decisiones tomadas sin el usuario

- **`include/game/CharaTask.h` (compartido):**
  - `virtual void vf58()` pasa a `virtual atn::Vector3 vf58()`, porque el override del demonio devuelve un vector. Ninguna otra unidad lo sobrescribía.
  - `HitMsg_unk::m4C` pasa de `unsigned char[4]` a `int`.
  - Nada baja (`regress` lost: 0).
- **`src/game/SpecialEnemyBase.cpp`:** el caso 17 usa `new (3, "", "", 0) Em203GreaterDaemon(parent, info)` en lugar del `extern "C"` fn_80287E7C.
- **Cabecera nueva `include/game/Em203GreaterDaemon.h`:** contiene la clase y los tipos comunes para la cola Red/RedMp: `DaemonPlaceList_unk`, `DaemonResPair_unk`, `DaemonLine_unk`, `DaemonZeroMtx_unk` y las copias débiles `fn_80288DD8`/`fn_80288E30`.
- **Relleno de autoinline:** `#define ASSERT_UNK(x) ((void)0)` (idioma aprobado) en 13 funciones que el original no inlinea en sus llamantes.
- **Instanciación explícita** `template void std::vector<RoutePlace_unk*>::push_back(...)` tras vf0C, para colocar "vector length error" en `@stringBase0`.
- **Valor muerto `float unused_unk = t * 0.8f;`** en fn_8028AA38, que pone 0.8f en su sitio de `.sdata2`. La función sigue al 100%. Probablemente sea un término de depuración eliminado. Hay que revisarlo si se prefiere otra forma.

## Splits y símbolos

- No cambian los splits.
- `symbols.txt` solo tiene renombrados:
  - Todas las fn_ de la unidad pasan a nombres mangled (ctor/dtor/vfXX/fn_XXX__18Em203GreaterDaemon…, `push_back` de `Vector_unk<Item14_unk>` en 0x8028A1F8).
  - `lbl_80745314` pasa a `@stringBase0` (local, string_table).
  - `lbl_80785910` pasa a `@STRING@vfC8__18Em203GreaterDaemonFv` (cadena "neck" del inline vfC8).
  - `lbl_8087DC38`/`lbl_8087DC3C` pasan a locales (static floats -15/-70).

## Hallazgos generales

- Un vector temporal en expresión (`GetTrans_unk() + atn::Vector3(0,5,0)`) en vez de una variable local con nombre cambia los huecos de pila al orden del original (vf58).
- Un helper `static inline` con parámetro `float` por valor (`Front_unk`, rotación de (0,0,1) en y) reproduce la carga tardía de `mRot.y`. Con `const float&` no lo hace (vfD8, fn_8028B75C).
- `int index = fn(...) - 1; table->mPoints[index]` en lugar del índice en línea invierte `add rX, base, off` a la forma del original.
- Hacer la comparación con una constante local antes de una llamada (`float limit = -0.5f;`) la carga en un registro no volátil antes de la llamada.
- Un accesor `int` sobre un campo `unsigned char` convierte `cmplwi` en `cmpwi`.
