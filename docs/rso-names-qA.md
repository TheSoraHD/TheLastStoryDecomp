# rso-names-qA: nombres reales del RSO, familia std/MSL (agente A, rama `agent-A`)

Cola: filas `std`/MSL de la tabla "Pendientes" de `docs/rso-names.md`. Referencia: `/home/claude/ref/report2.json` (8180 / 18215).

## Resultado

8182 / 18215 al 100% (+2: `__dt__ basic_ostream<char>` y `__dt__ basic_stringbuf<char>`), `main.dol: OK`, lost 0, `poolcheck` sin avisos, fuzzdrop 1 aceptada (`0x800e14a4` 57,35 → 35,05 en `report.json`). Commits: `25164b0` (do_replace, operator+), `3586b8f` (get_deleter), `855212a` (streams).

## Resultado por nombre

| Dirección | Nombre del RSO | Estado |
|---|---|---|
| 0x80013F78 (+ wchar_t 0x8006F72C) | `std::basic_string::do_replace` | hecho: `__replace_unk` → `do_replace` en `string.h`, `string_members.h` y usos |
| 0x8006AF38 | `std::operator+<c,...>(const char*, const string&)` | hecho: plantilla de espacio de nombres en `string.h` (amiga de la clase); `FileList` ya no la escribe a mano y sigue al 100% |
| 0x8006D008 | `std::operator+<c,...>(const string&, const char*)` | hecho: misma plantilla (antes amiga en línea no plantilla); 100% |
| 0x8001A34C | `shared_ptr_deleter::get_deleter(const type_info&) const` | hecho (`stdexcept.h`, `runtime_error.cpp`), 100% |
| 0x800E0B18 | `__dt__ basic_ostream<char>` | hecho: generado por el compilador (base virtual `basic_ios`); **sube de 99,97 a 100%** |
| 0x800E0B90 | `__dt__ basic_stringbuf<char>` | hecho, **sube de 99,69 a 100%** |
| 0x800E0C20 | `__dt__ basic_streambuf<char>` | hecho, 100% |
| 0x800E0C8C | `__dt__Q23std6localeFv` (no estaba en la tabla; es `dtor_800E0C8C`) | hecho: copia débil de `~locale` que emite el compilador tras `~basic_streambuf`, 100% |
| 0x800E14A4 / 0x800E1630 / 0x800E1A48 | `basic_ostream::sentry::sentry`, `~sentry`, `basic_ostream::flush` | hecho como miembros reales; fuzzy 57,25→35,00 (ctor), 29,76→38,21 (dtor), 16,52→36,12 (flush). **Bajada aceptada en el ctor** (ver abajo) |
| 0x800E1D4C, 1D54, 24CC–2670 | virtuales de `basic_streambuf<char>` (sync, overflow, imbue, setbuf, seekoff, seekpos, showmanyc, xsgetn, underflow, uflow, pbackfail, xsputn) | hecho; los que estaban al 100% siguen al 100%; xsgetn 96,51 y xsputn 99,47 igual |
| 0x800E1D5C | `ios_base::getloc() const` | hecho, 100% |
| 0x800E1D88 / 1DE4 / 1E64 / 2064 / 22E8 | `basic_stringbuf<char>::underflow/pbackfail/overflow/seekoff/seekpos` (`StringBuf_unk` eliminada) | hecho; 99,35 / 77,66 / 73,95 iguales, seekoff 69,21→70,84, seekpos 86,21→89,62 |
| 0x800E2A10 | `@8@60@__dt__ basic_ostream<char>` | solo `symbols.txt`: el thunk lo emite el compilador junto con la vtable, que no se emite sin un constructor en la unidad (sigue «-») |
| 0x800E0A64 | `ios_base::setstate(iostate)` | `symbols.txt` + declaración en `ios_base.h`; el cuerpo (copia débil de `atn/WordWrap_unk`) no está decompilado |
| 0x8068B39C | `ios_base::throw_failure(const char*)` | `symbols.txt` + miembro estático declarado en `ios_base.h` |
| 0x80690D60 | `Metrowerks::bufferedbuf<char>::showmanyc` | hecho: `auto_03_80690D60_text.cpp` (enlazada) con la clase real derivada de `basic_streambuf` e instanciación explícita |
| 0x803940E8 / 0x80394190 / 0x80394208 | `__dt__` de `basic_iostream`/`basic_istream`/`basic_stringstream` | **no hecho**: `GameEventScript_cont.cpp` no tiene fuente, pero `auto_03_803B27AC_text.cpp` los llama por su nombre `fn_` desde los thunks `fn_803B27D4`/`fn_803B27F0` (100%): renombrarlos los haría perder (lost 2). Hay que hacerlo cuando esos thunks salgan del compilador (clases `basic_istream`/`basic_iostream`/`basic_stringstream` reales en la unidad de `GameEventScript`) |
| 0x803B27DC | `@76@60@__dt__ basic_stringstream` | **no hecho**: hay fuente (`auto_03_803B27AC_text.cpp`, `fn_803B27DC` al 67%); es un thunk del compilador que no se puede escribir en C++; se renombra al tener la clase real |

## Cabeceras nuevas (`include/std/`)

- `ios_base.h`: `fpos<int>` (`streampos`), `streamoff`, `streamsize`, `ios_base` (enums `iostate`, `openmode`, `seekdir`, `fmtflags` con valores deducidos, `getloc`, `good`, `fail`, `flags`, `clear` en línea, `setstate` y `throw_failure` declaradas, `Init`, layout hasta 0x34) y `basic_ios` (`rdbuf`, `tie`, fin en 0x3C). `ios_init.h` ahora la incluye.
- `locale.h`: `std::locale` con `tr1::__shared_ptr_base_unk<imp>` (dos niveles de prueba de dirección en el destructor, como en `~ios_base` 0x8068B50C) y constructor de copia que incrementa el contador.
- `streambuf.h`, `sstream.h` (`basic_stringbuf`), `ostream.h` (`basic_ostream : virtual basic_ios`, `sentry`).

## Decisiones tomadas sin el usuario

- Bajada aceptada: `basic_ostream::sentry::sentry` (0x800E14A4, NONMATCHING) 57,25 → 35,00. El código anterior era un `extern "C"` con offsets crudos que simulaba la profundidad de inline a mano; con la clase real MWCC inlinea un nivel más (`flush` dentro del `~sentry` de la sentinela del `tie`). Probé 8 combinaciones (bloque del `tie` escrito o vía `flush()`, `setstate` en línea o no, `failbit` con `setstate` o `clear`); la elegida es la mejor de las tres funciones en conjunto (las otras dos suben).
- Las copias débiles se escriben como **instanciaciones explícitas de miembros** (`template int std::basic_streambuf<char>::sync();`) en el orden del `.dol`: MWCC emite cada una débil en ese punto. Las definiciones genéricas de las plantillas van arriba del fichero.
- `setstate` queda declarada sin cuerpo (no en línea): en línea, MWCC la inlineaba también donde el original la llama.
- Extras en `allocator_char.o` (no en el original de este split): `__dt__ basic_ios<char>` (está en el split `std/char_traits_char`, mismo fichero real), `__dt__ __shared_ptr_base_unk<locale::imp>` (copia débil, probablemente eliminada por el enlazador como duplicada), además de los ya existentes `__dt__ basic_string` y `do_replace`. La unidad no se enlaza (`matching: false`); para enlazarla habrá que fusionar `atn/WordWrap_unk` + `std/char_traits_char` + `std/allocator_char` (aviso: toca `splits.txt`).
- `ordercheck.py` ignora los símbolos débiles, así que ahora da 27 «missing» en `std/allocator_char`; comprobado a mano con `nm`: 0 fuera de orden, faltan solo el constructor de copia de `basic_string` (0x800E19AC, ya faltaba) y el thunk 0x800E2A10.

## Hallazgos (MWCC)

- Una instanciación explícita de un miembro de plantilla (`template R C<char>::f(...);`) emite solo ese miembro, débil, en ese punto del fichero, sin vtable.
- La vptr se coloca donde se declara la primera función virtual entre los miembros (`struct B { int x; virtual ~B(); int y; }` → vptr en 0x04).
- Destructor de una clase con base virtual: guarda `this+off - vbase` en el hueco de la base virtual (al final de su objeto) y llama al destructor de la base no virtual con `r4 = 0`; el thunk `@<off>@<slot>@__dt__...` sale con la vtable.
