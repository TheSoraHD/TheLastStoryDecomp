# enlazar-20261115, cola S (segunda pasada de eventos)

Agente S, rama `agent-S`. Primera pasada: `docs/enlazar-20261115-qA.md`.

## Resultado por unidad

- `Event/EventLoader_unk`: 4/5 -> **5/5, enlaza**. `Sequence::fn_80576258` 99,2 -> 100 %: las comprobaciones
  de indice escritas en el bucle (`cut->IsSectionIndex_unk(j) ? cut->mSections[j].get() : 0`, igual para
  canal y accion) en vez de los accesores `GetSection_unk`/`GetChannel_unk`/`GetAction_unk`; con los accesores
  los volatiles de cut/section/channel salen rotados (r11/r9/r10 frente a r9/r10/r11). La funcion quitada de
  ".xml" (`fn_Stripped_unk`) deja de ser `static`: siendo estatica y sin uso el compilador la quitaba con su
  literal y `poolorder` daba `.xml` ausente; el enlazador la quita (como en EventCommandDef_unk).
- `Event/ActionActorBehavior`: 12/14 -> **13/14**. `Sequence::fn_80550254` 99,4 -> 100 %: las mismas
  comprobaciones escritas (el processor pasa a r24), `int index;` declarado antes de `Cut* current` y el
  resultado de `current->fn_8053B028()` en un local `next` antes de comparar (`cmpw r3, r14`). Queda
  `fn_8054FBE0` 99,77 %.
- `Event/Label`: 1/2 sin cambio (`__sinit` 93,8 %).
- `Event/MotionData`: 25/26 sin cambio (`MovieData::vf08` 99,8 %).
- `Event/Section`: 5/6 sin cambio (`fn_8053C788`, push_back de `shared_ptr`).

## Unidades que enlazan (bytes)

- `Event/EventLoader_unk`: 120 bytes de datos (`total_data`); build limpio `main.dol: OK`, poolcheck/poolorder limpios.

## NONMATCHING con motivo

- `Sequence::fn_8054FBE0` (ActionActorBehavior) 99,77 %: el `clear()` de la lista del segundo `LeaveCut_unk`
  (rama else) da nodo/fin en r30/r29 (original r29/r30, como el primero). Probado ademas de lo de qA: las
  comprobaciones de `CutAt_unk` escritas, copia manual en el else con `mgr` local / sin local / referencia a la
  lista / `erase(begin, end)`, `Manager* manager` declarado arriba, `Sequence* self`, contador unsigned del
  bucle de actores (98,4), `m198` duplicado en las dos ramas del else (88,7), `LeaveCut_unk` static / no
  inline, iteradores con nombre, `erase` en las dos llamadas o solo en la primera (99,0). Casi: copia manual en
  el else con `erase(l.begin(), mgr->mB8.end())` o `erase(begin(), end())` (fin no const) arregla ese `clear`,
  pero entonces el bucle de actores de la primera rama rematerializa el nulo (`li r4, 0` en vez de
  `mr r4, r6`, 99,2 %) con cualquier forma del bucle (while, `++i`, orden de los stores, `Action* none`,
  helper inline, `actor` declarado fuera).
- `__sinit_\Label_cpp` 93,8 % (y Channel/Cut/Sequence): ver Hallazgos. Probado ademas de lo de qA: `mHash`
  `const`, tabla `const`, `ParamValue_unk(0)` explicito, la funcion quitada de delante con mas cuerpo o
  leyendo la tabla (detras de ella). En ficheros sueltos (try.sh): POD + casts `(int)""` (pasa a estatico),
  hash como clase con ctor / agregado anidado, `PV` con ctor de copia / union, puntero global a la tabla,
  `PV(int v = 0)` con el default omitido: siempre hash y luego default.
- `MovieData::vf08` 99,8 %: `MovieKind_unk` con ternario (96,5), con `kind` sin `pos` (98,5), `if/return`
  (96,5), `size_type pos` (igual), `inline` (94,4); `kind` local en el ctor (91,0); `pos` local para "RTMV"
  (38); `npos != find` (igual); tambien: MovieKind escrito en el ctor (ternario 96,5; con if el ctor deja de
  inlinearse, 38), `MovieKind_unk` definida tras el ctor, orden de declaracion de `parent`/`task`, la cadena de
  `fn_8006B174` en un local (93,1), la creacion en una inline (97,5).
- `Section::fn_8053C788`: con el parche de qA, la condicion de `__expand_by_unk` negada en
  `__push_back_slow_unk` baja a 92 %. Quedan unas 60 instrucciones distintas (huecos de pila de los dos
  grupos, `m` en r25, orden de los stores del buffer, registros del bucle del destructor, lectura de
  `buf.mData` por puntero): no atacado mas. Tras la reserva del buffer el original lee `buf.mSize` antes
  de guardar `buf.mData` y calcula `mSize * 8` antes que `mData + mStart * 8` (aqui al reves:
  `begin() + mSize` de `__append_n_unk`).

## Cambios de splits

Ninguno.

## Para el port

Nada (sin cambios de tipos, firmas ni layouts; solo cuerpos de funciones de EventLoader_unk y
ActionActorBehavior, que no son del port).

## Decisiones tomadas sin el usuario

- `fn_Stripped_unk` de EventLoader_unk no estatica (funcion inventada que el enlazador quita), para que el
  literal ".xml" quede en el pool.

## Hallazgos

- Un accesor inline con comprobacion de indice (`GetX_unk(i) { if (IsXIndex_unk(i)) return v[i].get(); return 0; }`)
  cambia el coloreado de los punteros que devuelve frente a la misma comprobacion escrita como ternario en el
  sitio (`IsXIndex_unk(i) ? v[i].get() : 0`): EventLoader_unk fn_80576258, ActionActorBehavior fn_80550254.
- Una funcion `static` sin uso no se compila y su literal no llega a `@stringBase0`; para reponer una funcion
  quitada por el enlazador, que no sea `static`.
- `a != b` con `b` el resultado de una llamada: el resultado en un local (`int next = f(); if (next != b)`)
  da `cmpw r3, rB` donde la llamada en la condicion da `cmpw rB, r3` (fn_80550254).

## Pendientes

- `Event/Manager fn_80488E00` (94,6 %, "un registro salvado mas en el original") usa `GetSection_unk`/
  `GetChannel_unk`/`GetAction_unk` en sus bucles: probar el truco de las comprobaciones escritas (fuera de
  esta cola).

- `__sinit` de las tablas `ParamDef_unk` (Label, Channel, Cut, Sequence): el original guarda el default
  del ultimo atributo antes que su hash (y este tras restaurar r31); estas cuatro son las unicas funciones del
  DOL con un store intercalado en el epilogo.
- push_back de `shared_ptr` (Section/Cut/Channel/Sequence) en `std/vector.h`.
