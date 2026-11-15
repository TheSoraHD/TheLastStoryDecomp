# Sesion enlazar-20261115, cola A (eventos)

Inicio: 2026-11-15 13:22 CET (reinicio del servidor a las 13:43). Tope ~15:50.

## Resultado por unidad

- `Event/EventCommandDef_unk` (31 KB de datos): 2/3, sin cambio. Datos ya definidos en el fuente (tablas de
  parametros y de comandos, `poolcheck` limpio, `ordercheck` 0). Falta `fn_800081C0` al 94 %: solo el orden de
  `li r5, 0` frente al `addi` de la direccion de la tabla (planificacion). 13 variantes mas (ver NONMATCHING).
- `Event/Section`: 5/6, sin cambio. `fn_8053C788` 91,9 % -> 98,95 % con el parche de `std/vector.h` (no
  commiteado, ver abajo); solo con el temporal en `push_back` (sin tocar vector.h) 93,9 %.
- `Event/Label`: 1/2, sin cambio (`__sinit` 93,8 %, mismo bloqueo que Channel y Cut).
- `Event/MotionData`: 25/26, sin cambio (`MovieData::vf08` 99,8 %, r28/r29).
- `Event/EventLoader_unk`: 4/5, sin cambio (`fn_80576258` 99,2 %, volatiles r9-r12).
- `Event/ActionActorBehavior`: 11/14 -> **12/14**: `Sequence::fn_80550A78` al 100 % (el id del
  `FindActor_unk` en una local: `int id = section->m10; m1BC = FindActor_unk(id);`). Quedan `fn_8054FBE0`
  99,36 % y `fn_80550254` 99,39 %.
- `Event/Channel`: no atacada aparte (sus tres funciones son los bloqueos de push_back, `__sinit` y registros).

## Unidades que enlazan (bytes)

Ninguna.

## NONMATCHING con motivo

- `fn_800081C0` (EventCommandDef_unk) 94 %: `li r5, 0` antes del `addi` de la tabla. Probado ademas: puntero
  que avanza + indice, puntero local a la tabla (const), indice unsigned, `continue`, referencia/temporal para
  mId, getter inline, `&tabla[0] + i`, variable resultado + break, while(true), `0L`.
- `__sinit_\Label_cpp` 93,8 % (y Channel 93,6, Cut 97,4, Sequence 98,1 con lo mismo): el original guarda el
  default del ultimo atributo (`stw r31`) antes del hash, restaura r31 y guarda el hash despues. Probado: tabla
  `const` (sin efecto, tambien referenciada desde una funcion), ctor de `ParamValue_unk` con asignacion en el
  cuerpo (+ const), agregado sin ctor (los ceros pasan a estaticos, 73 %).
- `MovieData::vf08` 99,8 %: probadas 9 formas (m04 directo, task = 0 + if, if/else sobre m18, indice unsigned,
  referencia al char, tamano local, ctor de MovieTask con `kind` local / puntero local / `pos`): todas peores.
- `Sequence::fn_80576258` (EventLoader_unk) 99,2 %: `switch` (86 %), alias local de `cut`, contadores internos
  unsigned (96-97 %), `continue`: sin mejora.
- `Sequence::fn_8054FBE0` 99,36 %: el original recarga `lbl_8087F540` tras el `strlen` (lee m1F1C por la
  recarga y m1F5C por `manager`); el nuestro reutiliza r28. `strlen` declarado a mano en el .cpp: sin efecto.
- `Sequence::fn_80550254` 99,39 %: registros guardados (processor r24 en el original). Probado: `int id`,
  processor con if/else, declarado aparte, declaraciones arriba de section/channel/action.

## push_back de `std/vector.h` (parche `docs/enlazar-20261115-qA.patch`, no commiteado)

Forma que acerca el `push_back` de `shared_ptr` (Section/Cut/Channel/Sequence `fn_80540A08`): `max_size()` en
una local `m` compartida por la comprobacion de longitud y por la division (el original la hace en tiempo de
ejecucion, `mulhwu` sobre r25), `third = m / 3` en una local (da `slwi` y no `clrrwi`), recommend escrito en
`expand_by` con `cap +=`, lo mismo en `__grow_cap_unk`, y el bucle de `__push_front_resize_unk` como
`while (first < last)` (el original no lo convierte en bucle contado). Ademas el `push_back` de Section con
el `shared_ptr` temporal (se destruye antes de leer `mChannels[size - 1]`).
- Suben (solo compilando las unidades): Section `fn_8053C788` 91,9 -> 98,95; Cut `fn_8053B030` 91,5 -> 96,3,
  `fn_8053B828` 92,0 -> 97,0; Channel `fn_8053A510` 92,0 -> 97,0; Sequence `fn_80540A08` 92,2 -> 97,0;
  Manager `resize<Vector3>` 90,6 -> 93,9.
- Bajan: Sequence `fn_8054204C` 96,2 -> 92,9 (push_back en bucle: el original elimina entero el recommend
  muerto y aqui quedan sus stores), Manager `fn_80484088`/`fn_80485028` -0,05; y renumeracion de `@N` en las
  unidades que incluyen vector.h (Manager: 8 funciones al 100 % a 99,x, arreglable con re-emparejado).
- Lo que falta en Section: los huecos de pila de expand_by (n 0x8, temporales 0x10/0xc) y de `__grow_cap`
  (0x1c, 0x14/0x18) salen intercambiados, y la `m` va a r31 en vez de r25. Un nivel de inline mas para el
  recommend (`__vec_recommend_unk(m, cap, n)`) invierte los dos temporales pero no el grupo; con `n` por valor
  baja a 97 %.

## Cambios de splits

Ninguno.

## Para el port

Nada (no hay cambios de tipos, firmas ni layouts).

## Decisiones tomadas sin el usuario

- No commitear el parche de `std/vector.h`: no llega al 100 % en ninguna funcion y baja `fn_8054204C`.

## Hallazgos

- Un argumento leido de un miembro (`FindActor_unk(section->m10)`) en una local `int id` antes de la llamada
  inline invierte los volatiles del bucle de busqueda (r4/r5; Sequence::fn_80550A78).
- `max_size() / 3` en tiempo de ejecucion (mulhwu sobre un registro con 0x1FFFFFFF): `max_size()` en una local
  que despues se sobrescribe o se usa en la comprobacion de longitud; `m / 3` en otra local para el `slwi`.

## Pendientes

- Huecos de pila de expand_by/`__grow_cap` en el push_back de `shared_ptr` (Section/Cut/Channel/Sequence) con
  el parche de arriba; y que el recommend muerto se siga eliminando en bucles (`fn_8054204C`).
- `__sinit` de las tablas `ParamDef_unk` (Label, Channel, Cut, Sequence).
