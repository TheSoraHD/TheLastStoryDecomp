# escribir-20261116, cola W3 (Event)

Inicio: 2026-11-16 23:58 CET (tope 02:28). Base `agent-W3` b4d7839 (15878 / 18109).

## Tabla de la cola

| Funcion | Antes | Despues |
|---|---|---|
| Sequence 0x80544650 grow de `vector<Actor>` (`__vec_event_unk<Actor>::__grow_by_unk`) | 0 % | equivalente, NONMATCHING 99,90 % (offset de "vector length error" en `@stringBase0`) |
| Sequence 0x80544748 grow de `vector<Path>` (`__vec_event_unk<Path>::__grow_by_unk`) | 0 % | equivalente, NONMATCHING 99,90 % (idem) |
| Sequence 0x80545B90 reservado `vector<Path>` (`__vec_event_unk<Path>::__reserve_unk`) | 0 % | equivalente, NONMATCHING 99,86 % (idem) |
| Sequence 0x80545C3C reservado `vector<Actor>` (`__vec_event_unk<Actor>::__reserve_unk`) | 0 % | equivalente, NONMATCHING 99,86 % (idem) |
| Sequence 0x80545290 `__dt__ __vec_split_unk<Path>` | 0 % | equivalente, NONMATCHING 99,59 % (registros) |
| Sequence 0x805455CC `__vec_event_unk<Path>::__push_front_unk` (antes `__swap_out_unk<Path>`) | 0 % | equivalente, NONMATCHING 95,06 % (registros de la copia de Path inline) |
| Sequence 0x80545084 `__vec_event_unk<Actor>::__push_front_unk` (antes `__swap_out_unk<Actor>`) | 84,15 % | **100 %** |
| Sequence 0x80542684 `__vec_allocator_unk<EffectEntry24_unk>::construct` (antes stub `Sequence::fn_80542684`) | DRAFT 0,7 % | equivalente, NONMATCHING 93,64 % (registros/orden) |
| EventParser_unk 0x8057DC38 `__vec_allocator_unk<Path>::construct` | 0 % | equivalente, NONMATCHING 97,61 % (registros) |
| Manager 0x8048B3B0 `size` / 0x8048E994 `clear` de `vector<Curve_unk>` | 0 % | sin cambios (bloqueo, ver abajo) |
| Puppeteer_CharaTask 0x80014250 `dtor_80014250` | 0 % | sin cambios (bloqueo, ver abajo) |
| ActionEffectPlay `ActionCheck::vf08` 0x8055E720 | DRAFT 93,95 % | equivalente (asm leido entero), NONMATCHING 94,05 % |

Al 100 % ademas (mismas unidades): `~__vec_split_unk<Actor>` 0x80544840 (99,70), `~__vec_split_unk<Path>`
0x80545290 (99,59), `__vec_storage_unk<EffectEntry24_unk>::clear` 0x8053E588 (98,94), mas
`__vec_event_unk<Actor>::__push_front_unk` 0x80545084 (tabla). Total 15878 -> 15882.

Otras subidas en las mismas unidades: `DataList_unk<Actor>::Add_unk` 88,87 -> 96,58 %, `Sequence::fn_80540A08`
92,63 -> 94,00 %, `fn_8054204C` 96,23 -> 97,78 %, `Add_unk<Actor>` -> 98,13 % (devuelve `&back()`), `ParseACID_unk` 89,73 -> 96,69 %, `ParsePATH_unk` 94,77 -> 98,61 %,
`vector<Path>::__push_back_slow_unk` 54,45 -> 90,40 %.

## Para el port
- `include/Event/EventVector_unk.h` (nuevo, solo lo incluyen Event/Sequence.cpp y Event/EventParser_unk.cpp):
  especializaciones explicitas de `vector<Actor>::__insert_realloc_unk`, `vector<Path>::__insert_realloc_unk`,
  `vector<EffectEntry24_unk>::push_back` y `vector<Path>::push_back` (mismo comportamiento que las genericas de
  `std/vector.h`; solo cambia la forma). Para el port: comportamiento identico, se puede ignorar.
- `Sequence::fn_80542684()` (stub vacio) eliminado de `Event/Sequence.h`: era la `construct` del allocator.
- `DataList_unk<Path>::Add_unk` nuevo (sin llamadores; el enlazador lo quita): para el port no hace falta.
- `DataList_unk<T>::Add_unk(const char*, int)`: `void` -> `T*` (devuelve la entrada nueva, `&back()`); los
  llamadores (Sequence.cpp, ParseXXID de EventParser_unk) no cambian.
- Tambien en `EventVector_unk.h`: destructores de `__vec_split_gbase_unk<Actor|Path>` y
  `__vec_storage_unk<EffectEntry24_unk>::clear` con otro orden de cargas (mismo comportamiento).
- `ActionCheck::vf08`: sin cambio de comportamiento (comparacion como int de la direccion del stick).

## Decisiones tomadas sin el usuario
- Funcion inventada sin llamadores `DataList_unk<Path>::Add_unk` (Sequence.cpp) para que Sequence.o emita las
  copias debiles de la realloc de `vector<Path>` que el original tiene en Sequence (0x80544748, 0x80545290,
  0x805455CC, 0x80545B90). Llama a `__push_back_slow_unk` directamente para que la `construct` y el slow path de
  `push_back<Path>` sigan en EventParser_unk (aun asi emite `__append_copies_unk<Path>`, que el original tiene en
  EventParser_unk 0x8057E274: no enlazaria en el orden exacto si Sequence pasara a enlazar).
- Cuatro asserts vacios `MSL_VEC_ASSERT_UNK(0)` en `__vec_event_unk<T>::__reserve_unk` para el umbral de R5 (con
  3 o menos se inlina en el slow path; con 4 o mas se llama, como en el original).
- Especializaciones por tipo (Actor, Path, EffectEntry24_unk) en una cabecera aparte en vez de cambiar
  `std/vector.h`: el cambio generico (ver Hallazgos) baja funciones en otras unidades (Manager, EventParser
  ParseXXID, Puppeteer ColorF push_back).
- `#include "std/string_members.h"` en Sequence.cpp: el pool original tiene los mensajes de `basic_string`
  (copias debiles que el enlazador quita); el pool pasa de `content` a `order`.

## Hallazgos
- La realloc de push_back de `vector<Actor>`/`<Path>` en el original llama por separado a: crecimiento (sin la
  comprobacion final de capacidad), reservado del buffer (con la comprobacion de longitud de la capacidad nueva y
  un primer argumento sin usar, el tamano nuevo), `__append_copies`, y el movimiento de los elementos viejos (bucle
  `first < last`); el swap queda inline. Es R5: la funcion de realloc rechaza a sus candidatas y se inlina en bruto,
  con sus llamadas pequenas (ctor del buffer, swaps) inlinadas (escribir-20261116 qW3).
- Llevar esa forma a `std/vector.h` generica (para tipos con destructor, despachando con
  `__vec_realloc_unk<T, __builtin_trivial_members(T) & 1>`) da lo mismo en Sequence pero cambia la paridad de
  niveles inline de las copias de string en `ParseMOID_unk` y familia (99,92 -> 99,69 %) y del ColorF push_back de
  Puppeteer: parche en `docs/escribir-20261116-qW3.patch` (escribir-20261116 qW3).
- Un `if` constante con `__builtin_trivial_members` dentro de una inline cuenta su rama muerta para el
  presupuesto R3 (`fn_8054204C` baja de 96 a 63 % con un bucle muerto en `__push_front_unk`): separar la rama en
  otra inline (escribir-20261116 qW3).
- El camino rapido de push_back de elementos con copia no trivial (EffectEntry24_unk, Path) lee `mSize` y `mCap`
  sin cachear y llama a `allocator::construct(p, x)` (con `if (p)`) (escribir-20261116 qW3).

- El destructor del buffer de `vector<Actor>`/`<Path>` lee la cuenta antes del puntero final y el `clear` de
  `vector<EffectEntry24_unk>` al reves (puntero y luego cuenta): especializacion por tipo en una cabecera
  incluida solo por las unidades que las usan para no tocar la forma generica (escribir-20261116 qW3).

## Bloqueos
- Manager `size`/`clear` de `vector<Curve_unk>` (0x8048B3B0/0x8048E994): quitando `__declspec(noinline)` a
  `fn_8048D3A4`/`fn_8048DE54` y poniendo N asserts vacios: N <= 5 se inlinan enteras en `fn_8048E904` y en
  `~Manager` (0 %); N >= 6 dejan de ser candidatas y `size`/`clear` se siguen inlinando. Umbral seco: en el
  original son candidatas que rompen el presupuesto de `fn_8048E904` pero no de `~Manager`. Sin idea nueva.
- `dtor_80014250` (Puppeteer): sigue siendo el destructor del buffer de vector de punteros compartido (`void*`) del
  original (lo usa la tabla de excepciones de locale.cpp); haria falta que todos los `vector<T*>` usen una sola
  implementacion `void*` en `std/vector.h`. No intentado.
- Pool de Sequence: el original tiene "basic_string: out_of_range" antes de "vector length error" (ninguna
  funcion del DOL lo referencia: viene de codigo quitado por el enlazador); las 4 funciones `grow`/`reserve` al
  99,9 % solo difieren en ese offset.

## Cambios de splits/symbols
- symbols.txt (renombrados): `fn_80544650`, `fn_80544748` -> `__grow_by_unk__Q23std3?__vec_event_unk<...>`;
  `fn_80545B90`, `fn_80545C3C` -> `__reserve_unk__...__vec_event_unk<...>`; `__swap_out_unk<Actor>`/`<Path>`
  (0x80545084/0x805455CC) -> `__push_front_unk__...__vec_event_unk<...>`; `fn_80542684__Q25Event8SequenceFv` y
  `fn_8057DC38` -> `construct__Q23std...__vec_allocator_unk<EffectEntry24_unk|Path>...`; `lbl_80794598`,
  `lbl_807945B4` -> `@STRING@__allocate__Q23std21vector<Q25Event4Path>FUi` / `...22vector<Q25Event5Actor>FUi`;
  re-emparejado de `@N` de Sequence/EventParser_unk (pd + lblmap). Sin cambios de splits.
