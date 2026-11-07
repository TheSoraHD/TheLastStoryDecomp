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
