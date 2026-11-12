# port-propuestas2-20261112

Segundo lote de cambios del port de PC que valen para la decomp. Sale del build de Windows x86-64
del port (rama `windows-20261112` de TheLastStory-PortTest, seccion 14 de `docs/guia-port.md` de ese
repo). Base: `main` e15010e (merge de `port-propuestas-20261112`).

## Cambio

`include/std/msl_size.h`: `typedef unsigned long __msl_size_t;`, el `size_t` de MSL. Se usa donde
el tipo tiene que ser exactamente `size_t` o tan ancho como un puntero:

- el tamano de `operator new` (`std/msl_common.h`; las versiones con `MemoryTag` de `atn/Memory.h`;
  las cuatro definiciones de `atn/SmallAllocator_8.cpp`);
- las palabras de `std::basic_string` (`Long`, `Raw`, `__zero`), en `std/string.h`.

Un typedef se mangla como su tipo (`__nw__FUl`), asi que MWCC genera lo mismo. En Windows de 64 bits
`size_t` es `unsigned long long` y `unsigned long` mide 32 bits: `operator new(unsigned long)` no
compila alli, y la `std::string` dejaba de ser tan ancha como sus punteros. Con el typedef, el port
cambia una linea en vez de esos siete sitios.

No se cambia el `Flags` de `std::string` (campo de bits 1 + 31 de la Wii), ni las ~190
redeclaraciones `extern "C"` de `memset`/`memcpy`/`strncmp`... con `unsigned long`: el port las
renombra en `pc.h`, como las `wcs*`.

## Comprobacion

Build limpio de `main` como referencia, luego el cambio: `main.dol: OK`, 15057 / 18109, lost 0, sin
bajadas de fuzzy, labelcheck y rep100 iguales. `precommit.sh atn/SmallAllocator_8` solo marca el
fuera de orden que ya tenia `main`.
