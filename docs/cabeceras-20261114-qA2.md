# Sesion cabeceras-20261114, cola A2 (segunda pasada de `push_back`, copias debiles de Event)

Inicio: 2026-11-14 16:10 CET. Tope ~18:40.

## Forma del original (analisis)

Las tres formas que se ven en el original son la misma `push_back` de MSL con distintos niveles de inline:

- `push_back(x)`: `if (size < cap) { alloc().construct(data + size, x); ++size; } else slow(x);`
  (0x8054204C llama a `construct` 0x80542684 con `this + 0x18C` = `&mCap` y a `slow` 0x805428A8 con `(this, x)`).
- `slow(x)`: `if (expand_by(1)) { alloc().construct(data + size, x); ++size; } else realloc(1, x);`
  (en Event/Manager fn_80481B9C quedan fuera de linea `expand_by` 0x8049003C, `construct` 0x80482400 y
  `realloc` 0x80490098 con `(this, 1, x)`; en 0x805428A8 todo dentro).
- `expand_by(n)`: la comprobacion de longitud `n > max_size() - capacity()` y el `__recommend` muerto con
  el `n` del parametro; devuelve `false` (el asignador no crece en su sitio). Por eso el "1" del
  `__recommend` muerto se guarda en la pila antes de la comprobacion (luces de Event/Manager).
- `realloc(n, x)`: buffer, `__grow_cap(size + n - cap)`, reserva, `mStart = size`, `buf.__append_n(n, x)`
  (fn_8057E274 de EventParser_unk: `(buf, 1, x)`), `__swap_out`.
- La `resize` de `vector<Vector3>` (0x8048A584) tiene el mismo `expand_by` dentro (comprobacion + recommend muerto).
- Solo hay un `expand_by` fuera de linea en todo el DOL (0x8049003C).

## Variante probada (sin commitear): `docs/cabeceras-20261114-qA2.patch`

`push_back` = rapido (`__push_fast_unk`) o `__push_back_slow_unk(x)`; `slow` = `if (__expand_by_unk(1)) { __alloc_unk().construct(...); ++size; } else __insert_realloc_unk(1, x)`;
`__expand_by_unk(n)` con la comprobacion y el recommend muerto escritos en su cuerpo (no a traves de `__recommend_unk`:
asi el `1` se guarda antes de la comprobacion); `__insert_realloc_unk(n, x)` con `buf.__append_copies_unk(n, x)`
(miembro nuevo en los buffers, tambien en las especializaciones de int/float/punteros/string/wstring y en las 6
locales de D2ACommonDialog, MenuCommonTask, U16Table_unk, Xsca_unk, D2A_Manager y D2A_MessageTable_unk);
`__vec_allocator_unk<T>::construct` con el asignador en `&mCap` (`__alloc_unk()` en `__vec_storage_unk`).

Medido en build completo (variante U3, casi igual a la del parche) contra `ref-A2`: 15433 -> 15430 (las 3 perdidas
son etiquetas: `fn_800BD9C0` 99,99 y las dos de ControlAppear por `@enum$6929ControlAppear_cpp`, se arreglan con el
re-emparejado), `main.dol: OK`. Suben 37 funciones, entre ellas:
- Event/Manager `fn_80481B9C` 58,4 -> **96,0 %** (forma exacta: rapido en linea, `expand_by`, `construct` y `realloc`
  fuera; quedan registros y pila). Sus copias debiles salen del tamano del original: `__expand_by_unk` 0x5C
  (fn_8049003C), `__insert_realloc_unk` 0x7B8 (fn_80490098, 98,8 % renombrandola a mano: registros de los ceros del
  buffer r7/r8 y el bucle `__push_front`).
- Event/Section, Cut y Channel `fn_8053C788/8053B828/8053A510/8053B030` 69 -> 92 %; Sequence `fn_80540A08` 87 -> 92 %.
- Luces de Event/Manager `fn_80485028/80485964/804862C4` 95-96 -> 97,6-98,0 % (con la rapida por `new` en el parche):
  ya solo registros (r26/r27 y los ceros del buffer).
- push_back debiles de MapPartsBase, GameScene, GimmickStateCommand, Em204BigMulue, D2A_Manager: +1,5 a +2,5 %
  (quedan el orden de huecos de pila de `expand_by` frente a `__grow_cap` y los ceros del buffer).

Por que no se commitea (bajadas de fuzzy, regla de `fuzzdrop`):
- atn/RenderSystem_unk `RenderManager::RenderManager` 88,7 -> 46,3 %: el ctor pasa del presupuesto (R3) y llama a
  todo. Esta justo en el limite: sin la rama de `construct` de `slow` vuelve a 89,0-89,1 %; cualquier contenido en esa
  rama (construct, `new`, `__push_fast_unk`) lo tumba. Tampoco ayudan `AddLight_unk`, `Vector3 d(...)`, `set_buf`.
- Event/EventParser_unk `ParsePATH_unk` 72,7 -> 16,5 %, `ParseACID_unk` 64,4 -> 57,5 %, Sequence
  `DataList_unk<Actor>::Add_unk` 61,2 -> 52,6 %: `push_back<Path>`/`<Actor>` aceptan sus candidatas y ya no caben en
  la funcion. Es el bloqueo de qL (el original rechaza las candidatas de `push_back` de tipos con destructor).
- Pequenas: Em301LShade `fn_802D6004` 87,6 -> 86,9, NewCampPartsItem `fn_80517B3C` 99,4 -> 99,1, ShopBase
  `fn_805849E8` 99,77 -> 99,62, GameEventScript `fn_8039F08C` 99,97 -> 99,81, Manager `fn_80484088` 95,2 -> 94,9,
  ColorF push_back 95,6 -> 95,3 (sin mirar: pueden ser etiquetas).
- Probado sin efecto para la pila de `expand_by`/`__grow_cap`: definir `expand_by` despues de `__grow_cap_unk`,
  realloc por `__vec_set_buf_unk`, recommend en un ayudante con `const size_type&`. `__recommend_unk(n)` llamado
  desde `expand_by` empeora las luces (el 1 se guarda tarde). Despacho con etiqueta (`__vec_pod_tag_unk`) o con un
  selector estatico anade un nivel y empeora los tipos triviales; un `if` constante deja contar el codigo muerto.

## Funciones al 100 % / 0 %

Sin cambios (nada commiteado en codigo). Las 26 copias debiles de Event siguen sin escribir.

## Para el port

Ningun cambio.

## Decisiones tomadas sin el usuario

- No commitear `std/vector.h`: la forma nueva sube 37 funciones pero baja 4 de forma clara (presupuesto de inline).
  Decision para el usuario: aceptar esas bajadas (todas NONMATCHING) a cambio de la forma del original, o seguir
  con el presupuesto (ctor de RenderManager, `push_back` de tipos con destructor) antes de integrarla.

## Hallazgos (una linea)

- `push_back` de MSL: rapido / `slow(x)` = `expand_by(1)` (devuelve false; comprobacion + recommend muerto) o
  `realloc(1, x)` con `buf.__append_n(n, x)`; el `expand_by` fuera de linea solo existe en 0x8049003C.
- El codigo muerto tras un `if` constante (`__builtin_trivial_members`) cuenta para el presupuesto de inline (R3):
  una rama muerta de 1-3 sentencias basta para tumbar el ctor de RenderManager.
- La constante de `__recommend` guardada en la pila antes de la comprobacion de longitud = el parametro `n` de
  `expand_by` con la direccion tomada (max por referencia) en el mismo cuerpo.
