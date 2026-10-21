# batch-auto-20kb-atn qA3: atn (cargador de recursos, teclado en pantalla de MP)

Cola: `atn/Loader`, `atn/SoftKeyboardU`.

## atn/Loader (commit 5858545)
- Resultado: 18/31 al 100% (+18 en el total), `matching: false`, `main.dol: OK`, regress lost: 0, fuzzdrop 0.
  - 2 funciones sin escribir: `fn_8046CBB0` (`m4044 = 0`) y `fn_8046CBBC` (vacia), ver pendientes.
- NONMATCHING (casi todo asignacion de registros):
  - `fn_8046D1EC` 99.89%: en el `fn_8046CBC0` inlineado puntero/longitud de la cadena en r27/r26 al reves.
  - `Loader::Loader` 99.72%: el puntero a "/test" va a r31 (base del pool) en el original, aqui a r30.
  - `Loader::~Loader` 98.05%: registros de los cuatro destructores de lista inlineados.
  - `vf10` 96.1%: registros y el original comprueba begin != end antes del primer bucle.
  - `vf14` 98.45%: r28/r29 cambiados (base del pool / fin de la lista de archivos), copia del iterador a r3.
  - `fn_8046E410` 95.1%: el original relee mCount para el erase; registros del bucle de borrado.
  - `Sort_unk<LoadCore*, ...>` x2 96.45%: el MSL `__sort` de siempre (mismo bloqueo que game/CharaTask y
    atn/Shader_unk).
  - `fn_8046ECDC` 97.7%: carga del tamano de fn_8006EF48 antes de la rama del data() inlineado; registros.
  - `fn_8046EF08` 98.2%, `fn_8046F014` 98.8%: insert inlineado de `std/list.h` (bloqueo conocido, ver
    `Loader::Get_unk`).
- **Decisiones sin el usuario**
  - **Nombre real de la clase**: `atn::LoadManager_unk` era `atn::Loader` (RTTI "atn::Loader" y vtable
    0x8078FB70 en este split; el ctor 0x8046D84C escribe esa vtable y el destructor pone a 0 `lbl_8087F518`).
    Renombrada en todo el repo (`include/atn/LoadManager.h`, `PfsArchive_unk.h`, LH_Motion, LH_Motion_cont,
    FileSystem_unk, LoadManager_unk, ResRef_unk, CompositeCache_unk) y en `symbols.txt`
    (`Q23atn15LoadManager_unk` -> `Q23atn6Loader`, 9 simbolos). La cabecera sigue llamandose `LoadManager.h`.
  - `atn::Loader` deriva ahora de `atn::Task` (vtable de 5: dtor, vf0C = `Task::fn_800D3FA4`, vf10, vf14,
    vf18 de Task) y tiene todos sus miembros 0x48-0x4048 (antes relleno `m04`/`m3EE0`): `std::string` en
    0x3EFC y 0x4010, ruta de 0x100 en 0x3F08 (`Path256_unk`, inventado), `ResHandle` (game/ResHandle.h) en
    0x401C, `LoaderD2A_unk*` (pantalla de carga, inventado) en 0x3EE0. `LoadCache_unk` gana ctor (`mCount = 0`),
    `end()` y `erase(first)`.
  - Las listas de `atn::Loader` pasan a `std::__list_owned_unk<T>` (nuevo en `include/std/list.h`: ctor y
    destructor de la lista MSL en tres niveles, como `CacheList_unk` de CompositeCache); `std::list` no cambia.
  - `include/std/string.h`: `operator[] const` (copia debil 0x8046D1CC). `include/atn/FileList.h`: quitada la
    virtual pura `vf0C` (la vtable 0x80777BC0 solo tiene el destructor y Loader construye un FileList en la
    pila), anadidos `GetName_unk` (copia debil 0x8046D19C), `fn_8006C3F0`, `fn_8006C300` (renombrados en
    symbols.txt como miembros de FileList).
  - Funciones que otros `.cpp` llaman como `extern "C"` se quedan asi, con el Loader como primer parametro:
    `fn_8046D7D8` (crea el Loader), `fn_8046DC5C`/`fn_8046DD20` (34 ficheros de db/), `fn_8046ECDC`,
    `fn_8046EED8`, `fn_8046EF08`, `fn_8046F014`, `fn_8046F2C4`, `fn_8046D1EC`. Declaraciones en
    `LoadManager.h`; quitada la declaracion local (conflictiva) de `fn_8046DD20` en `src/atn/LH_Core.cpp`.
  - symbols.txt (renombrados): miembros de Loader, `fn_8046CBC0__3atnFPCc` (static, scope:local),
    `fn_8046CC90__3atn...` (std::string, lo llama atn/FileList.s), copias debiles (getters 0x8046D184-0x8046D1C4,
    `GetName_unk`, `__vc__...CFUl`), `Sort_unk`/`Median3_unk`, `@stringBase0` (0x8075561C), los contadores del
    sort `@LOCAL@Sort_unk<...>@sOffset` (0x8087E050/54).
  - Split: anadido `.sbss 0x8087F518-0x8087F520` (`lbl_8087F518`, el singleton; estaba en un hueco).
  - `fn_8046F014`: las rutas .pkh van antes que las .pk en el pool -> variables locales con el indice primero.
  - `vf14` escribe a mano el cuerpo de `fn_8046EED8` (llamandolo, el 0.0f salia despues en .sdata2).
- Hallazgos MWCC
  - Una funcion `static` no inline que se autoinlinea por completo deja sus literales en `@stringBase0` (una
    `inline` los manda a `@STRING@`), pero el orden en el pool sigue el orden de uso en el cuerpo inlineado.
  - Llamar a una funcion inline del mismo fichero definida mas abajo (con `-ipa file`) retrasa sus constantes
    de `.sdata2` respecto a escribir el cuerpo en el sitio.
  - Funciones pequenas llamadas fuera de linea desde un ctor/dtor que si inlinea otras cosas = otro fichero
    (comprobado: definidas en el mismo fichero se inlinean).
- Pendientes
  - `fn_8046CBB0`/`fn_8046CBBC` (0x8046CBB0-0x8046CBC0) deberian pasar al final de `atn/LoadManager_unk.cpp`
    (no es de mi cola: no toco su split).
  - Cadenas del pool que no usa nadie: "wt" y "%08x %s ref=%d %s pack=%d\n" (funcion de volcado eliminada por el
    enlazador) y los errores de basic_string (el original los tiene en `@stringBase0`, aqui van a `@STRING@`).

## atn/SoftKeyboardU (MP::SoftKeyboard) (commit d12e982)
- Resultado: 27/46 al 100% (+27 en el total; todo el lote: 5961 -> 6006), `matching: false`, `main.dol: OK`,
  regress lost: 0, fuzzdrop 0.
- Contenido real del split: `MP::SoftKeyboard` (Task de la entrada de nombre del modo online, singleton
  `lbl_8087F860`) con un `atn::SoftKeyboardU` en 0xF8. Los nombres de tipo de `boost::bind` en .rodata dan la
  clase `MP::SoftKeyboard` y que los callbacks reciben `const Key&` (aqui `Key*`, como la cabecera de qA2).
- Sin emparejar (14): el ctor del original supera el presupuesto de inline y llama fuera de linea a
  `fn_805028D8` (caracter de la tecla), `Vector2_unk(float, float)`, `SoftKey_unk()`, `SoftKeyboardU()`,
  `fn_80503AB8` (SetKey), `fn_80503B04` (longitud maxima), los 4 `boost::bind` y los 4 ctor de `boost::function`
  (copias debiles 0x805028D8-0x80503B04). Aqui el ctor queda justo por debajo del presupuesto (una llamada inline
  mas lo saca todo de linea) y no se emiten. El original ademas construye las esquinas de la tecla en su sitio
  (ctor de Vector2 sobre la tecla, sin operator=): no reproducido.
- NONMATCHING: ctor (0%, todo inlineado), `fn_80504C00` 92% (indice base+desplazamiento), `vf14` 99.3% (registros),
  `fn_80505510` 95.9% (orden de carga, registros, reenvio del ptr_string_base local), `fn_80505C1C` 96.3% (un
  store programado una posicion despues).
- **Decisiones sin el usuario**
  - `include/atn/SoftKeyboardBase.h`: `SoftKey_unk` pasa de 4 floats a `Vector2_unk mMin, mMax` (fn_80503AB8 los
    copia con psq); `Contains_unk` de la base adaptado (sigue al 100%, `main.dol: OK`).
  - Cabecera nueva `include/atn/SoftKeyboardU.h` con `atn::SoftKeyboardU` (solo miembros inline: SetKey, longitud y
    los 7 setters de callbacks que devuelven el anterior). NO puede ir en SoftKeyboardBase.h: sus inline instancian
    miembros de boost::function y cambian el orden de las copias debiles de SoftKeyboardBase_wchar_t_0 (rompia
    `main.dol`).
  - Tablas inventadas: `sKeyChars_unk[3][4][11]` (QWERTY/AZERTY/QWERTZ, 0x8075A988), tres tablas de etiquetas
    (`sKeyLabels*_unk`, .data 0x80792AE0), `sSpecialText_unk` (ids de mensajes 0x1A5-0x1A8).
  - `atn::ptr_string_base<wchar_t, 0>` redeclarada localmente (como en MP/Text.cpp) con un ctor desde puntero.
  - symbols.txt: renombrados las 31 funciones emitidas (mas los nombres previstos de las no emitidas), datos
    (`sKeyChars_unk`, `sSpecialText_unk`, `@9316`, `@stringBase0`, `@wstringBase0`, `sKeyLabels_unk` reducido a
    0x1C8 + 7 constantes ptmf `@8749`-`@8755` NUEVAS en 0x80792CA8, RTTI de los bind_t, `@LOCAL@`/`@GUARD@` de
    `assign_to`, placeholders `_1`-`_6` locales en .sbss).
- Hallazgos MWCC
  - `x / 2.0f` y `x / 4.0f` dan `fmuls x, 0.5`/`fmadds x, 0.25` con `x` como primer operando; con `0.5f * x` o
    `x * 0.5f` el compilador pone la constante primero (fnmsubs/fmadds con los operandos cruzados).
  - `float v = a; if (c) v = b;` da `lfs; beq; lfs` como el original; el ternario da dos ramas con `b`.
  - `if (0.0f == x)` no cambia nada; copiar `x` a un local antes de comparar si (carga de la constante primero).
  - Las implicitas `operator=` de structs pequenos tambien salen fuera de linea (`__as__`) cuando la llamadora
    supera el presupuesto de inline.
- Pendientes
  - Encontrar la forma del ctor que supere el presupuesto (desbloquearia 14 funciones casi triviales).
