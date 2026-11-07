# msl-iostream-20261107 — MSL C++ (lo último de código en tramos `auto_`)

Rama `msl-iostream-20261107` (desde `nw4rmath-20261107` 0703f89). **Trabajo a medias, sin enlazar.**
Fuente: `libs/PowerPC_EABI_Support/src/MSL_C++/iostream.cpp` (escrito desde el asm: no hay fuente pública de
MSL C++ con licencia válida; solo cabeceras parciales de smart pointers en KooShnoo/pokemon-rumble, MIT/CC0, sin
usar).

## Estado

- **43/170 funciones al 100%** en objdiff dentro de la unidad nueva (14082+ en `report.json`, +29 sobre la rama
  nw4rmath). Unidad `matching: false` (no enlaza todavía).
- `regress.py` contra `/home/claude/ref`: **lost 3**, las tres de las antiguas unidades pequeñas absorbidas:
  `codecvt<wchar_t>::do_unshift` (0x8068D7AC, no se emite aún: falta definir los demás virtuales de
  `codecvt<wchar_t>`), `ctype<char>::do_toupper(char*, const char*)` (0x80692A10, 86%: el original recarga
  `upper_` en cada vuelta; con `this` const MWCC lo saca del bucle) y el `dispose` del deleter de `__locale_imp`
  (0x8069512C, falta `__locale_imp`). Por eso esto va en su propia rama y **no** en `nw4rmath-20261107`.

## Cambios de configuración (rama WIP)

- `splits.txt`: unidad nueva `PowerPC_EABI_Support/src/MSL_C++/iostream.cpp` = un solo objeto (el `__sinit`
  0x80695270 va al final: es una sola TU, como `iostream.cpp` de MSL): `extab` 0x80006720-0x80007B94,
  `extabindex` 0x80007C60-0x800080F8, `.text` 0x8068B39C-0x8069533C, `.ctors` 0x8072D400, `.rodata`
  0x807661C0-0x80766768, `.data` 0x807BBB08-0x807BBF58, `.bss` 0x808327C0-0x80832C50, `.sdata`
  0x8087EC18-0x8087ED20, `.sbss` 0x80880378-0x808803D8. Absorbe las 11 unidades `auto_03_806*`/`8068*` del
  juego (borradas de `splits.txt`, de `configure.py` (lib `game_nodefer`) y de `src/`).
  Pendiente de comprobar: `release()` (0x806952C4, detrás del `__sinit`) seguramente es de otra TU.
- `libs_ref.json` (categoría `mw`): **Wii/1.1**, flags de `NMWException.cp` sin `-inline auto`, con
  `-Cpp_exceptions on`, `-O4,p`, **`-opt nostrength`** (sin ella `~ios_base` reduce el índice del bucle de
  callbacks), `-inline on`, `-ipa file`, `-lang=c++`. Wii/1.0/1.3 dan lo mismo; GC/3.0a5.2 peor.
- `symbols.txt`: ~70 renombrados a nombres reales (`init__Q23std8ios_baseFPv`, `__global__Q23std6localeFv`,
  destructores/constructores de `console_inputbuf`/`console_outputbuf`/`bufferedbuf`, facets...). **Sin comillas**
  (dtk las mete en el nombre).

## Hallazgos (para seguir)

- Es la `iostream.cpp` de MSL con todo instanciado: `ios_base` (failure, init, ~ios_base), `ios_base::Init` y su
  gemela ancha (`wios_base_Init_unk`, nombre ya usado por `include/std/ios_init.h`), `console_inputbuf`,
  `console_outputbuf`, `bufferedbuf` (char y wchar_t), `basic_streambuf<wchar_t>`, istream/ostream wide, facets
  `ctype<char/wchar_t>`, `codecvt<char/wchar_t>`, `locale::__global`/clásico, `__locale_imp::replace`
  (0x806920C0), y al final `__sinit` (guardas de los `locale::id`).
- Las copias `char` de plantillas que también define el juego (basic_streambuf<char>...) no aparecen: el enlazador
  se queda la primera copia débil (la del juego). Se pueden definir genéricas sin problema.
- `shared_ptr`: dos niveles en los destructores (`addic.; beq; beq`): `shared_ptr<T>` deriva de una base con los
  datos y el destructor, y su constructor plantilla **reenvía** a la base pasando el deleter por referencia
  (`D&`); `locale` guarda la base directamente (un nivel). Con esto salen `throw_failure` (solo nombres de
  datos), `~failure`, `~ios_base`.
- `ios_base::Init::Init` del original no inlinea nada (todo llamado: `operator new(size_t, void*)`, `setf`, `tie`,
  `pubsetbuf`, `scoped_lock`): está por encima del presupuesto de IPA (`docs/inlining.md` R3). El nuestro aún lo
  inlinea todo: faltan los constructores grandes reales para que pase del presupuesto.
- Orden de emisión con `-ipa file`: cada función seguida de las inline/plantillas que usa y no se inlinearon.
- `init`: queda `extrwi` frente a `srwi` al guardar `sb == 0` en el byte de estado (99.1%).
- Layouts: `ios_base` 0x34 (+tie 0x34, fill 0x38 en basic_ios); istream 0x4C (puntero a base virtual 0, gcount 4,
  vptr 8, basic_ios en 0xC), ostream 0x48 (base en 8); `bufferedbuf` 0x4D, `console_outputbuf` 0x54 (FILE* 0x50),
  `console_inputbuf` 0x3C (FILE* 0x24); `ctype<char>` 0x18 (tabla 8, tolower 0xC, toupper 0x10, del 0x14);
  `codecvt` 8; `locale` {imp, cntrl}; `__locale_imp` 0x18 (vector de facets 0/4/8, nombre basic_string en 0xC).
- `.bss` (base 0x808327C0): cin 0, cout 0x50, clog 0x98, cerr 0xE0, wcin 0x128, wcout 0x178, wclog 0x1C0, wcerr
  0x208, registros de destructores 0x250..; estáticos locales de Init: bufs en 0x80832A70/AAC/B00 (char) y
  B54/B90/BE4 (wchar_t).
- Herramientas de la sesión (scratch, no en el repo): `chk.py` (match por función vía objdiff-cli), `ren.py`
  (renombrar en symbols.txt), `split.sh` (re-split dtk tras renombrar), volcado `all.s` de las 170 funciones.

## Pendientes

1. Las ~127 funciones restantes: bufferedbuf (setbuf/sync/flush_put/empty_get/underflow/overflow/seekoff/seekpos/
   imbue/fill_get), console_inputbuf (imbue/setbuf/sync/underflow/uflow/pbackfail), codecvt<wchar_t> (do_out/
   do_in/do_length), ctype<wchar_t> (constructor con tablas en .sdata, do_is/scan/toupper/tolower/narrow),
   `locale::__global`, `__locale_imp::replace`, num_put/num2dec (0x80693884/0x80693AB8), vector de facets
   (0x80694790/0x80694C38), `__sinit`.
2. Recuperar las 3 perdidas antes de mergear (lost 0).
3. Init por encima del presupuesto de IPA; `init` (`srwi`).
4. Dividir `release()` a su TU si hace falta, datos (`pd.sh`) y `matching: true` al final.

# Sesión 2 (2026-11-07, tarde)

## Resultado (en curso)

- **Cierre: 95/170** al 100% (14135 en `report.json`), lost 1, build limpio `main.dol: OK`, poolcheck/mnemcheck sin
  avisos en las tres unidades. Sin enlazar (`matching: false`).
- Punto de control 1: **92/170** al 100% (de 43), `regress.py` lost 1 (`do_toupper` 98% por un `nop` de alineación),
  `main.dol: OK`. Las tres unidades siguen con `matching: false`.
- Hechos: codecvt (todos), `bufferedbuf` (setbuf, sync, flush_put, empty_get, fill_get, underflow, overflow,
  pbackfail, seekoff, seekpos, showmanyc; imbue al 98%), `console_inputbuf` (setbuf, showmanyc, underflow, uflow;
  sync/pbackfail/imbue/`__get_char_unk` al 97-99,6%), `console_outputbuf::sync`/`seek`, `basic_streambuf<wchar_t>`
  (xsgetn, uflow, xsputn), `release`, deleter/hold de `__locale_imp`, dtor de `vector<facet*>`.

## Cambios de splits

- La unidad se parte en **tres TU**: `iostream.cpp` (.text 0x8068B39C-0x80691E50), `locale.cpp` (0x80691E50-0x806952C4,
  con el `.ctors`/`__sinit`) y `shared_ptr_unk.cpp` (`release()`, 0x806952C4-0x8069533C). Pruebas: dos pools de
  cadenas (`@stringBase0` 0x8076645C tras los nombres RTTI de iostream; segundo pool 0x80766688 con "vector length
  error", "C", "*", tras los nombres RTTI de locale/facets); `release()` va detrás del `__sinit`; el original llama a
  `locale::__global` y a los constructores de los facets desde iostream (no los inlinea aunque sean candidatos únicos).
  Límites de datos por referencias: `.sdata` 0x8087EC80 (tablas de `ctype<wchar_t>`), `.sbss` 0x80880390
  (`locale::id::__id_count_`), `.data` 0x807BBDE0 (vtable del deleter de `__locale_imp`), `.bss` 0x80832C38 (cadenas de
  registro de `classic_`/`global_`), extab/extabindex por las entradas de las funciones.
- `libs_ref.json`: dos entradas nuevas con los mismos flags que `iostream.cpp`.
- Fuente: las clases comunes pasan a `libs/PowerPC_EABI_Support/src/MSL_C++/msl_iostream_unk.h` (nombre inventado).

## Decisiones tomadas sin el usuario

**Aprobadas por el usuario el 2026-11-07** (particion en tres TU y sus nombres, `-enum min`, `do_toupper/do_tolower`
de rango sin `const`, temporal de `use_facet`).

- `-enum min` para las tres unidades (ver Hallazgos).
- Campos y nombres inventados de los buffers (`buf_size_`, `ext_beg_/ext_next_/ext_end_`, `__init_areas_unk`,
  `__get_char_unk`, `__unget_unk`, `__min_size_unk` = 16, `__min_len_unk` = 1...), todos con `// inferred`.
- `_FILE` local con `buffer_ptr`/`buffer_len` en 0x24/0x28 copiados de `MSL_C/MSL_Common/file_struc.h` (no se
  incluye: su `types.h` declara `operator new` sin `throw()` y choca) y la macro `getc` de MSL C.

- `ctype<char>::do_toupper/do_tolower(char*, const char*)` **sin `const`**: es lo único (de ~15 variantes: const_cast,
  mutable, tipos de la tabla, bases, referencias, flags) que hace que MWCC recargue la tabla en cada vuelta como el
  original. Queda al 98% solo por un `nop` de alineación del bucle (depende de la posición en el objeto).
- `-opt nostrength`: se mantiene. Sin él, `~__locale_imp` y `Init` mejoran pero `~ios_base` baja a 67% (y no hay forma
  de fuente que lo evite, 8 variantes). Con la TU partida, `locale.cpp` podría llevar otros flags: pendiente probar.
- `Metrowerks::recursive_mutex` (nombre inferido): el lock de las tablas de locale tiene su propio destructor
  (0x80691EE4, distinto de `scoped_lock<mutex>` 0x8068BA80).
- `~__locale_imp() throw()` y `__global() throw()`: tienen manejador `__unexpected`; con `throw()` el destructor deja de
  inlinearse en `dispose` como en el original.

## Hallazgos

- `scoped_lock` con destructor no vacío (`if (locked_) m_.unlock();`, `unlock` vacío): así el lock vive en la pila y
  tiene acción de limpieza en extab (como el original), y `Init` deja de inlinearlo.
- Un `delete p` de un tipo incompleto en el punto de instanciación (constructor de `locale` dentro de la clase) no
  llama al destructor: el constructor que crea el `shared_ptr<__locale_imp>` va fuera de la clase, tras `__locale_imp`.
- MWCC emite las virtuales inline de un facet (do_out/do_unshift/do_in) tras la primera función que las llama por la
  vtable (`flush_put`), aunque la vtable esté en otra TU.
- La alineación de bucles (`nop`) depende del offset en el objeto: funciones al 98% por un `nop` se arreglan solas
  cuando todo lo anterior tiene el tamaño correcto.
- `-enum min` en las tres unidades MSL C++: los `codecvt_base::result` se comparan como byte (`clrlwi r0,r3,24`).
  Sin bajadas en el resto.
- `seekdir` vale 1/2/4 (beg/cur/end) y `seek()` del dispositivo recibe el `whence` de C (SEEK_SET/CUR/END = 0/1/2):
  `seekoff` hace la traducción con un `switch`.
- `empty_get` reconvierte con `out()` (no `in()`) los caracteres no leídos para saber cuántos bytes retroceder.
- `use_facet`: cada copia inlineada empieza limpiando un byte de un objeto vacío en la pila (puesto como
  `default_delete<Facet>` temporal, `// inferred`); y relee `loc.imp_` en cada acceso (sin local).
- Registros: varias funciones al 99,x% solo por la asignación de registros (versiones `wchar_t` del
  `__init_areas_unk`, `__get_char_unk`, `sync`/`pbackfail` de `console_inputbuf`); no se insistió (regla 8).

## Pendientes (sesion 2)

1. `ctype<char>::do_toupper/do_tolower` rango (lost 1): 98% solo por el `nop` de alineacion; se arreglara cuando el
   objeto tenga los tamanos correctos antes de la funcion (o cuando enlace). No mergear hasta lost 0.
2. `ios_base::Init`/`wios_base_Init_unk`: el original no inlinea nada (presupuesto de IPA superado); el nuestro lo
   inlinea todo menos el constructor de `console_inputbuf` (no es `inline` y es grande). Probado: marcarlo `inline`
   hace que se inlinee tambien. Falta que los candidatos (constructores de los streams y de `console_outputbuf`)
   cuesten lo del original.
3. `ctype<wchar_t>`: tablas de rangos `__range_map_unk<T>` (vector de {lo, hi, mul, add}, busqueda binaria por `hi`).
   Las variantes de rango estan al 88-92%: el original vuelve a calcular `end()` tras la busqueda. Faltan el
   constructor (0x80692C3C, inserta rangos con `insert(lo, hi, vlo, vhi)` = 0x806945A4 (mascara) / 0x806943B8
   (wchar_t), que a su vez usan 0x80694790/0x80694C38) y los destructores (0x80692EFC/0x80692F78/0x80692FF4); las
   virtuales de un caracter (0x806940A8, 0x80694188, 0x80694254) estan escritas inline y saldran con la vtable.
4. `__locale_imp::~__locale_imp` (94%): el original reduce el bucle (offset + contador); con `-opt nostrength` no.
   `~ios_base` necesita `nostrength`. Ahora que estan en TU distintas, probar `locale.cpp` sin `nostrength`.
5. `locale::__global` (80%): el literal "C" esta en el pool de la TU (no en un `@STRING@` de una inline): revisar si
   `classic()` es una funcion no inline (quizas eliminada por el enlazador) o si el codigo esta en `__global`.
6. `__locale_imp::replace` (0x614), `basic_ostream<wchar_t>::flush` (0x8068E3FC) y su `sentry` (0x8069194C, dtor
   0x80691AC0) con try/catch, constructores de streams (0x8068E554, 0x8068E6E8, 0x8068E878), `console_outputbuf`
   (0x8068C4B8/0x8068C72C), 0x8068E270, 0x80693884/0x806939C8/0x80693AB8 (`__num2dec`/basic_string),
   `logic_error`/`length_error` (0x80694320, 0x80694328, 0x8069519C), `__sinit` (guardas de los `locale::id`).
7. Registros: varias funciones al 99,x% (ver comentarios `NONMATCHING`).
8. Orden: `ordercheck.py` no se ha pasado (las unidades no enlazan aun); escribir en orden del `.dol` al final.

Siguiente sesion: prompt `claude/prompt-msl-iostream-3.md` del proyecto.

# Sesion 3 (2026-11-07, tarde)

## Resultado

- **Cierre: 106/170** (14146 en `report.json`), lost 0, build limpio `main.dol: OK`, poolcheck/mnemcheck sin avisos.
  **`shared_ptr_unk.cpp` enlazada** (`matching: true`, nuestro `.o` en el enlace, `main.dol: OK`).
  `iostream.cpp` y `locale.cpp` siguen con `matching: false`.
- `__locale_imp::replace` escrito (91%): con el, el pool de cadenas de `locale.cpp` queda en el orden del original
  ("vector length error" primero) y `__insert_one_unk` sube a 93%.
- Punto de control 1: **106/170** (de 95), `regress.py` contra `main` **lost 0** (la unica bajada de fuzzy es
  `pad_03_8070DC0C`, del tramo de `nw4rmath`), `main.dol: OK`. Sin enlazar (`matching: false`).
- Nuevas al 100%: `ctype<char>::do_toupper/do_tolower` de rango (lost recuperado), destructores de `ctype<wchar_t>`
  y de sus dos `vector` de rangos, constructor de `ctype<wchar_t>`, `logic_error::what`, `~logic_error`,
  `~length_error`, `__sinit_\locale_cpp`. `~__locale_imp` 94 -> 99,98%.

## Cambios de splits/flags

- `libs_ref.json` (las tres unidades): **`-func_align 4` detras de `-O4,p`** (el `,p` de `-O4,p` vuelve a poner la
  alineacion de funciones a 16 aunque `-func_align 4` vaya antes; con 16 MWCC mete `nop` de alineacion de bucles y
  todas las funciones quedaban en multiplos de 16). Arregla los `nop` de `do_toupper/do_tolower`.
- `libs_ref.json`: **`-RTTI on`** en las tres (las vtables del original apuntan a `__RTTI__...` y los nombres de tipo
  estan en `.rodata` de cada TU, no en el pool de cadenas). Ningun porcentaje cambia.
- `libs_ref.json`: **`locale.cpp` sin `-opt nostrength`** (pendiente 4): `~__locale_imp` sube; nada baja.
  `iostream.cpp` lo sigue necesitando (`~ios_base`).
- `symbols.txt` (renombrados): constructor/destructor de `ctype<wchar_t>`, `vector` de rangos, `insert` de rangos,
  `__insert_one_unk`, `logic_error`, `length_error`, `__sinit_\locale_cpp`, ids y guardas de los facets
  (`id__Q23std8ctype<c>` 0x808803C0 ... `@GUARD@id__...` 0x808803D0...), `__id_count___Q33std6locale2id`,
  `__dt__...scoped_lock<recursive_mutex>` (0x80691EE4).

## Decisiones tomadas sin el usuario

- Los tres cambios de flags de arriba (`-func_align 4` tras `-O4,p`, `-RTTI on`, `locale.cpp` sin `nostrength`).
- `ctype<wchar_t>`: los miembros son `vector<__range_entry_unk<T> >` y `__range_map_unk<T>` solo tiene funciones
  estaticas (`find`, `insert`): el EH del constructor original llama directamente a los destructores de `vector`.
- `ctype_base` con `enum __mask_unk` y un `operator|` inline (inferido): las mascaras compuestas del constructor van a
  temporales de pila (resultado de una llamada) y las simples a constantes de `.sdata`, como en el original.
- `numeric_limits<wchar_t>` minimo (`min`/`max` como llamadas: sus resultados van a la pila).
- `vector`: el constructor por defecto pone los campos a 0 (las bases sin constructor); `clear()` con un
  `allocator<T>` temporal (el `stb 0` del original); `insert` -> `__insert_one_unk(pos, x, tag)` con un tag vacio por
  valor; `__recommend_unk` (comprobacion de longitud + crecimiento, resultado descartado) y `__grow_cap_unk`
  fuera de la clase para que "vector length error" vaya al pool del fichero.
- `classic()` deja de ser `inline` (el literal "C" va al pool de la TU, no a un `@STRING@`).
- `locale::id` con constructor vacio: asi las cuatro ids tienen guarda y `__sinit` como el original.

## Hallazgos

- `-O4,p` despues de `-func_align 4` deja las funciones alineadas a 16 (ver arriba): mirar el orden de los flags
  si un `.o` tiene huecos entre funciones o `nop` de alineacion de bucles que el original no tiene.
- Las funciones de mas en nuestro `.o` (destructores de las bases de `vector`) solo tienen entrada en extabindex:
  el enlazador las elimina si nadie las referencia.
- El pool de cadenas de `locale.cpp` sigue el orden del fuente: "vector length error" (de `replace`), cadenas de
  `basic_string` (out_of_range), "C" (`classic`), "*".
- 0x80693884/0x806939C8/0x80693AB8 son de `num_put` para `double` (`__num2dec`, comparacion, formateo a
  `basic_string`): las llama el `num_put` inlineado en `atn/EffectManager` (0x80241700).
- `Init`: el original llama hasta a `operator new(size_t, void*)`, `tie`, `setf` y `pubsetbuf` (presupuesto R3
  superado); el nuestro lo inlinea todo. Falta saber que candidato cuesta mas en el original.

## Pendientes

1. `ctype<wchar_t>` de rango (88-92%) y las de un caracter (80%), `__range_map_unk::insert` (76%): el original
   recarga `size_`/`data_` tras la busqueda binaria (CSE roto); ~15 formas probadas (const/no const, mutable,
   iteradores struct, busqueda por referencia, `end()` por accesor de referencia, otros compiladores Wii/GC).
2. `__insert_one_unk` (90,5%): huecos de pila de `n` y de los temporales de `max`, y registros del pool (se arreglara
   en parte con el orden del pool).
3. `__locale_imp::replace` (91%): el original tiene ocho temporales de un byte (`stb 0`) en el `resize` inlineado
   (al empezar `__append_unk`, en la comprobacion de longitud, tras el `new`, al principio y al final de
   `__push_front_unk`, antes de la ultima comprobacion de `__grow_cap_unk` y en el recorte); origen desconocido
   (quizas un `allocator` o un tag por valor de la implementacion de MSL). `__global` (85%: el `shared_ptr`
   temporal de `classic_` va en memoria). Funciones de `num_put` (3, sin escribir).
5. `locale.cpp` no enlaza aun: faltan `num_put` (3), las de rango y `replace`/`__global` al 100%; despues
   `ordercheck.py`, `pd.sh` y datos (`classic_`/`global_`/mutex: renombrar a `@GUARD@`/`@LOCAL@` como los ids).
4. iostream: `Init`/`wios_base_Init_unk`, constructores de streams y de `console_outputbuf`, `sentry`, `flush`,
   0x8068E270, y las pequenas que el original emite fuera de linea (`__nw__FUlPv`, `setf`, `tie`, `pubsetbuf`...).

# Sesion 4 (2026-11-07, rama `msl-iostream-20261107b`)

## Resultado

- Inicio: 106/170, `report.json` 14146, `main.dol: OK` (build limpio de `main` be459fe).
- Punto de control 1: **111/170** (`report.json` 14151), lost 0, sin bajadas de fuzzy, `main.dol: OK`.
  Nuevas: `__decimal_unk::__decimal_unk(double)` (0x80693884), `__decimal_unk::__round_dir_unk` (0x806939C8),
  `basic_ostream<wchar_t>::sentry::~sentry` (0x80691AC0), `ios_base::Init::~Init` (0x8068BC30),
  `wios_base_Init_unk::~wios_base_Init_unk` (0x8068C240).
  Escritas sin llegar: `__decimal_unk::__round_unk` (0x80693AB8, 97,9%), `basic_ostream<wchar_t>::flush`
  (0x8068E3FC, 99,9%), `sentry::sentry` (0x8069194C, 97,2%).

## Cambios de splits/flags

- `symbols.txt` (renombrados): `fn_80693884/806939C8/80693AB8` -> miembros de `std::__decimal_unk`; `fn_8068E3FC`,
  `fn_8069194C`, `dtor_80691AC0` -> `flush`/`sentry` de `basic_ostream<wchar_t>`; `lbl_80779AB8` ->
  `@STRING@clear__Q23std8ios_baseFQ33std8ios_base7iostate` (literal de `ios_base::clear`, objeto debil que el
  enlazador deja en `.data` de `atn/WordWrap_unk`); datos de la inicializacion de streams (`init_cnt_` x2, guarda,
  objeto y bandera del mutex, guardas y `@LOCAL@` de `fin`/`fout`/`ferr` x2).
- `symbols.txt` (no es solo renombrado): `lbl_808327C0` (0x2B0) partido en los ocho almacenes de streams
  (`__cin_storage__3std` ... `__wcerr_storage__3std`, `scope:local`) y `lbl_80832A10` (0x60, el resto).
- Sin cambios de flags ni de `splits.txt`.

## Decisiones tomadas sin el usuario

- Rama `msl-iostream-20261107b`: `msl-iostream-20261107` ya existe en el remoto (sesion 3, mergeada) y es el mismo dia.
- Referencia (`mkref.sh` -> `/home/claude/ref`) sacada del build limpio del propio arbol recien creado desde
  `origin/main` (identico a `main`), sin `git worktree` aparte: ahorra un build de ~3 min.
- `num_put` de `double`: struct `__decimal_unk` (string de digitos 0-9, exponente, clase) con constructor desde
  `double` y dos miembros (`__round_dir_unk`, `__round_unk`); nombres inventados. `decimal`/`decform`/
  `__num2dec`/`__fpclassifyd` declarados en `locale.cpp` (las cabeceras de MSL C traen `types.h`, cuyo
  `operator new` de colocacion choca con el de `msl_iostream_unk.h`).
- `basic_string` (cabecera MSL): constructor por defecto/copia/subcadena, `size`, `data`, `begin`/`end`,
  `operator[]`, `insert(pos, s, n)`, `insert(pos1, str, pos2, n)`, `push_back`, `operator+=`, `resize`,
  `find_first_not_of`, `replace(pos, n1, n2, c)` (fuera de linea). `end()` y `find_first_not_of` leen datos y
  tamano en una sola rama (`!is_long()`), como el original.
- `ios_base::throw_failure` con `__declspec(noinline)` (permitido por la regla 2: funcion fuerte que el original
  llama con `bl` y MWCC inlinearia en `sentry`/`flush`).
- `ios_base`: `clear`/`setstate` inline en la clase (su literal va a `@STRING@clear...`), `good`, `fail`,
  `rdstate`, `exceptions`, `flags`, `__setstate_nothrow_unk` (inventado: los `catch` ponen el bit sin comprobar
  excepciones). `basic_ios::rdbuf()`/`tie()`, `basic_streambuf::pubsync`.
- `basic_istream`: `gcount_` declarado antes del destructor virtual (el original tiene el puntero de vtable en
  0x08 y `gcount_` en 0x04).
- `__ios_mutex_ready_unk` definido despues de la inline del mutex (orden de `.sbss` del original).

## Hallazgos

- Un constructor se reconoce porque devuelve `this` (`mr r3, rN` en todas las salidas); una funcion que devuelve
  un objeto por valor no lo hace.
- `push_back(c)` escrito como `operator+=` -> `push_back` -> `insert(pos, &c, 1)`: el caracter queda por debajo de
  las etiquetas de iterador en la pila (cada nivel de inline invierte el orden de los huecos).
- `ios_base::Init`: con un candidato artificial de >= 75 sentencias (coste tras su propio inline: un envoltorio de
  1 linea sobre 80 sentencias tambien vale) al final, nuestro `Init` deja de inlinear y la secuencia de llamadas
  es exactamente la del original. El constructor de `console_inputbuf` no es candidato (no es `inline` y es
  grande); con el `inline`, faltan solo 20-40 sentencias. `-inline on,noauto` o `-ipa function` no dan el
  original (inlinean `tie`/`setf`/`pubsetbuf`). Los constructores de streams fuera de linea (sonda) son casi
  identicos a los del original: la diferencia de coste no esta en ellos.
- MWCC coloca el puntero de vtable donde se declara la primera funcion virtual: miembros declarados antes quedan
  delante (`basic_istream::gcount_`).

## Pendientes

1. `Init`/`wios_base_Init_unk` (ver Hallazgos): desbloquea ~19 funciones (constructores de streams y de
   `console_outputbuf`, destructores de `wostream`/`wios`/`wistream`, 0x8068E270, `__nw__FUlPv`, `setf`, `tie`,
   `pubsetbuf`...).
2. `sentry::sentry` (97,2%) y `flush` (99,9%): el `and.` de `clear()` tiene los operandos al reves
   (`state & exceptions` con `exceptions` cargado primero) y el `setstate` inlineado recarga el estado; ~12
   variantes probadas.
3. `__round_unk` (97,9%): registro del resultado de `__round_dir_unk` inlineado (r3 frente a r4).
