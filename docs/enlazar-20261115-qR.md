# enlazar-20261115, cola R: enlazar `atn/D2A_TaskInstanceBase_64_8`

Inicio 18:37 CET.

## Resultado por unidad

- `atn/D2A_TaskInstanceBase_64_8`: 51/51 -> 51/51 al 100 % y **enlaza** (`matching: true`, `main.dol: OK` en build
  limpio, `poolcheck`/`poolorder`/`mnemcheck` limpios, `ordercheck` 0 fuera de orden).
  Los dos problemas que dejaba la cola M:
  1. Copias de `Text_unk::Text_unk()`/`~Text_unk()` justo detras de `Create` (en el original, al final de cada
     grupo): ahora son **especializaciones explicitas** de cada instanciacion (`template <> D2A_TaskInstanceBase<64, 8>
     ::Text_unk::Text_unk() {}`), declaradas antes de las instanciaciones explicitas y definidas tras el ultimo
     miembro de cada grupo. Una especializacion es una funcion normal: se emite donde se define, no tras su primer
     usuario. `Text_unk` declara su ctor/dtor (sin definicion generica). El codigo de `SetText_unk` no cambia (los
     sigue inlineando: `-inline auto` inlinea tambien funciones no inline del fichero).
  2. `SetFloat_unk<8, 2>` (0x230) en el `.dol`: una instanciacion explicita marca la funcion como **force active**
     en `.comment` (flag 0x08; comprobado con un volcado de `.comment`), y el enlazador no la quita aunque nadie la
     llame. Ahora es una especializacion explicita de `<8, 2>` (sin definicion generica): funcion normal sin el flag,
     el enlazador la quita y su `L"%f\0"` queda en el pool de cadenas anchas en su sitio.

Commit: ver `git log` (`Link atn/D2A_TaskInstanceBase_64_8`).

## Unidades que enlazan (bytes)

- `atn/D2A_TaskInstanceBase_64_8`: 528 bytes de datos (`measures.total_data` en `report.json`), 17664 de codigo
  (0x801F4FF4-0x801F94F4). Build limpio (`rm -rf build/SLSP01/obj build/SLSP01/src build/SLSP01/config.json`,
  configure, `ninja -j1`): `main.dol: OK`; precommit: 15587 funciones (= referencia), lost 0, fuzzy drops 0,
  labelcheck/rep100 igual.

## NONMATCHING con motivo

Ninguna nueva.

## Cambios de splits

Ninguno. `symbols.txt`: solo re-emparejado de `@N` de la unidad (`pd.sh`).

## Para el port

- `include/atn/D2A_TaskInstance.h`: `D2A_TaskInstanceBase<N, M>::Text_unk` declara `Text_unk(); ~Text_unk();`
  (antes implicitos). Sus definiciones son especializaciones de `<64, 8>`, `<32, 8>` y `<8, 2>` en
  `atn/D2A_TaskInstanceBase_64_8.cpp`; otra instanciacion necesitaria su propia definicion (o una generica).
- `SetFloat_unk` solo existe para `<8, 2>` (especializacion); antes era un miembro generico.

## Decisiones tomadas sin el usuario

- Especializaciones explicitas (ctor/dtor de `Text_unk` por instanciacion y `SetFloat_unk<8, 2>`) en vez de
  instanciaciones explicitas: es C++ normal (sin pragma ni asm) y la unica forma encontrada de reproducir el orden
  y la eliminacion del original con este compilador. Ver Hallazgos para lo que no funciona.

## Hallazgos (una linea cada uno)

- Instanciacion explicita (`template class X<N>;` o miembro a miembro) = flag force active (0x08) en `.comment` para
  cada funcion instanciada: el enlazador no la quita aunque nadie la use. Las implicitas y las funciones normales no
  lo llevan y si se quitan.
- Una especializacion explicita (`template <> void X<1>::f() {...}`) es una funcion normal: se emite en su sitio del
  fuente, sin force active (el enlazador la quita si nadie la usa) y `-inline auto` la sigue inlineando.
- Instanciacion explicita miembro a miembro: cada linea genera en el acto la funcion y sus dependencias implicitas
  (copias debiles) justo detras; `template class` encola primero todos los miembros (orden de declaracion) y luego
  las dependencias.
- Una funcion (plantilla o no) usada antes de estar definida se queda pendiente y se emite al final del fichero, no
  donde se define.
- Instanciaciones implicitas (desde codigo no plantilla): primero las referenciadas directamente, agrupadas por
  instancia de clase en orden inverso al de referencia y por orden de declaracion; luego, en otra pasada, lo
  descubierto al generarlas (vtables, clases anidadas), todas las instancias juntas.
- Una llamada cualificada a una virtual (`p->X::vf0C()`) es una referencia directa y no se inlinea.

## Pendientes

- Unificar las vistas de la instancia en los llamadores (365 casts `(D2A_TaskInstance32_unk*)` en 26 ficheros:
  `D2AInstance_unk` de D3AnimeInstance, `CostumeEditFace_unk`, `SelectMemberAnim_unk`, `ShopLayout_unk`,
  `MenuLayout_unk`, `ResultLayout_unk`, `GameLayout_unk`...). Regla 10.
