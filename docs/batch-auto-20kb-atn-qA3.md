# batch-auto-20kb-atn qA3: atn (cargador de recursos, teclado en pantalla de MP)

Cola: `atn/Loader`, `atn/SoftKeyboardU`.

## atn/Loader (commit PENDIENTE)
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
