# list-get-unk (2026-10-30): `Loader::Get_unk<T>` y la familia `std::list`

Propuesta recomendada de `SIGUIENTE_sesion.md`. Resultado: 8759 → **8765** funciones al 100%,
31,44 → 31,47% de código, `main.dol: OK`, lost 0, sin bajadas de fuzzy. Unidades enlazadas: 375 (sin cambio).

## Ganadas

- `Loader::fn_8046EF08`, `Loader::fn_8046F014`, `Loader::~Loader`, `Loader::fn_8046F4D4`, `Loader::fn_8046F544`,
  `CompositeCache_unk::fn_800D8698`.

## Cambios en `include/std/list.h` (detalle en `docs/msl_cpp.md`, "Position of insert")

- Clase `const_iterator` con constructor de copia escrito; `begin()/end() const`.
- `insert(const_iterator pos, x)`; `push_back` construye la posición en sitio. `fn_8046F014` usa
  `insert(end(), x)` (el original tiene ahí el reparto contrario de registros).
- `erase(first, last)` con `const_iterator`; `clear()` borra con `begin()/end() const`.
- Holder del nodo liberado antes del enlace.

## Decisiones tomadas sin el usuario

- `__declspec(noinline)` en `fn_8046EF08` (fuerte, el original la llama con `bl` desde `fn_8046F2C4`;
  caso ya aceptado en la regla 2).
- `symbols.txt`: `fn_8046CBB0`/`fn_8046CBBC` renombrados a `fn_8046CBB0__Q23atn6LoaderFv` /
  `fn_8046CBBC__Q23atn6LoaderFv` (son métodos de `Loader`; sin ello `~Loader` quedaba al 99,97% por el nombre).
- `FileSystem_unk`: los bucles de `fn_8046F4D4`/`fn_8046F544` recorren una referencia `const` a la lista.
- Descartada (por decisión del usuario) la variante con constructor de copia en `iterator`: +6 pero
  `fn_800D8624` 100 → 91 y cinco bajadas.

## Pendiente

- `Get_unk<T>` 96,9 → 99,7% (11 copias, `T* core`), `Get_unk<LoadCore>` 99,0%: las 8 unidades `atn/LH_*`
  siguen sin enlazar. Residuo y formas probadas en `include/atn/LoadManager.h`.
- `Loader::fn_8046F460` 98,3%, `PacketPack::fn_8050DEA8` 99,5% (con `const_iterator` 99,7%).
