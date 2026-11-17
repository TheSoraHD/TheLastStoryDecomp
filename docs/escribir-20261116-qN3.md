# escribir-20261116 cola N3: cerca de enlazar (tercera lista)

Inicio: 2026-11-17 05:19 CET (tope 07:49). Rama agent-N3.

Resumen: 6 funciones nuevas al 100 % (ArenaFloor vf0C, Leaf vf10, CharaLight vf6C de la cola; Ocean vf6C y
WaitObject vf6C fuera de ella) y 3 unidades que enlazan (gmk/ArenaFloor, gmk/Leaf, gmk/CharaLight):
funciones 15923 -> 15928, unidades 726 -> 729, lost 0, sin bajadas, labelcheck/rep100 igual.

## Tabla de la cola

| funcion | antes | despues |
|---|---|---|
| game/CostumeEditChara vf14 | 98,9 % | 98,9 % (sin cambio) |
| gmk/FlyObjManager FlyObj::fn_80427870 | 98,8 % | 98,8 % (sin cambio) |
| atn/TextureDraw_unk fn_800D5F68 | 98,1 % | 98,1 % (sin cambio) |
| gmk/InteractionSpot Start | 97,8 % | 97,8 % (sin cambio) |
| gmk/ArenaFloor vf0C | 97,6 % | 100 %, la unidad enlaza (e4f748c) |
| atn/RenderSetting_unk ctor | 97,1 % | 97,1 % (sin cambio) |
| atn/PostFxColorMatrix vf10 | 97,1 % | 97,1 % (sin cambio) |
| atn/PBoneBasic vf10 | 96,7 % | 96,7 % (sin cambio) |
| game/PickTask vf10 | 96,6 % | 96,6 % (sin cambio) |
| gmk/CharaLight vf6C | 96,3 % | 100 %, la unidad enlaza (06bb619) |
| atn/EffectSequencer_unk EfpData_unk::fn_802440F0 | 95,5 % | 95,5 % (99,36 sin commit, ver variantes) |
| MP/CryptMP vf0C | 95,2 % | 95,2 % (sin cambio; bloqueo .bss) |
| gmk/Leaf vf10 | 95,1 % | 100 %, la unidad enlaza (8b0dd0b) |

## Variantes probadas

- gmk/ArenaFloor vf0C: con `p = mData; p += n++` de qB (99,40), `perm.py` sobre los locales del caso 4
  (`obj`, `e`, `i` arriba del bloque: 99,64) y luego `p`, `model`, `type` arriba del cuerpo del bucle
  (`type = part->mType; switch (type)`): 100 %. `main.dol` FAILED hasta definir `~ArenaFloor()` (no inline) tras
  fn_8041DDE4: la copia debil del dtor iba al final y en el original esta a continuacion del cargador.
- gmk/Leaf vf10: cuerpo del bucle en una inline `UpdateLeaf_unk(self, i)` con la rotacion (identidad + tres
  `__MtxRotate`) en otra inline `SetRotZYX_unk`, `scale` como local con nombre y la traslacion por una inline con
  tres floats (argumentos evaluados z, y, x): 100 %. Sin la identidad dentro de la inline de rotacion: 99,29
  (r25/r26 cruzados).
- atn/EffectSequencer_unk fn_802440F0: perm.py de `elem`, `i`, `frames`, `length` (24 ordenes): 95,27-95,55.
- gmk/InteractionSpot Start: perm.py de `table`, `id`, `spot` locales: 97,24 en todos.
- gmk/CharaLight vf6C: perm.py de `name`, `group`, `current`, `sub`, `i` (120 ordenes): 96,28 en todos.
- atn/D3Anime vf14: perm.py de `m`, `scale`, `sphere`, `frustum` arriba: 99,80; `mMtx` sin referencia: 95,1.
- gmk/FlyObjManager fn_80427870: parche de qF (98,98); el fabs/frsp de fn_80139F60 sigue (no repetido).
- chr/ControlCannonLift Throw vf0C: `point` declarado antes de `offset` (89,7), con GetQueenPoint_unk (96,2).
- atn/EffectSequencer_unk fn_802440F0: el bucle entero en una inline `CalcLength_unk(EfpData_unk*)` (el truco del
  contenedor) sube a 99,36 (el elemento ya va en r31); quedan cruzados el contador (r29 en el original) y el
  desplazamiento (r30). Probado dentro: perm.py de 4 locales, referencia, `int i`, `mElems + i`, `int& total`,
  break, inline miembro, otra inline anidada para la longitud: 99,36 en todos. Sin commit.
- chr/ControlPushedDown vf0C (otra unidad de $3==2): `float frame` declarado arriba con `zero`/`one`/`motion`
  (perm.py) 99,29 -> 99,64; queda r4/r5 de `rot`/chara y FPR del clamp. Avoid vf0C: `Segment_unk` con
  asignaciones en el cuerpo: igual. Sin commit.
- gmk/OnOff vf0C (parche qF 99,95): `rot` antes de `mtx` en uno o los dos casos (99,88-99,91), rotacion y
  traslacion en una inline (96,8). fn_803F8324: `__Vec2Copy` + `z` (98,3; la z del original sale de 0x97c(r31)
  porque psq_l no admite el desplazamiento 0x974 y lfs si), `off` antes o con `=`: igual o peor.
- game/Em001Necromancer vf14: los tres textos en una inline (static: 99,52 = igual; no static: las cadenas
  pasan a `@STRING@`, 94,4).
- atn/PBoneBasic vf10: identidad + tres rotaciones en una inline con referencia (97,15), por valor (96,2),
  por ejes Vector3 (89,7). Los tres Vector3 de un componente del original (0x8/0x14/0x38: 0, 0, 1.0) son
  stores muertos de constantes; sin idea de su origen.
- atn/PostFxColorMatrix vf10: la copia de la columna con el ctor de 4 floats ordena las cargas como el original
  (97,21); `w(0.0f)` dentro del ctor de fila (97,21). Quedan los FPR del bucle del clamp. Sin commit.
- gmk/InteractionSpot Start: busqueda en inline libre, las tres busquedas en una inline (98,07), getter de la
  tabla: sin 100 %.
- gmk/FlyObjManager: `mRot = mVel; mRot.fn_80139F60()` (95,4), copia local (98,35), orden (92,3).
- game/CostumeEditChara vf14: el primer bucle en una inline que recibe `mChara` por referencia (98,90) o el array
  (96,5).
- gmk/SwingObject vf6C (fuera de la cola): con la inline del grupo y `param` declarado antes que `group` llega al
  100 % en objdiff, pero el `.sdata2` de la unidad sigue en otro orden (la unidad ya no enlazaba): no
  commiteado para no re-emparejar etiquetas por posicion sobre valores distintos.
- atn/RenderSetting_unk ctor: las etiquetas de `.sdata2` de symbols.txt estan mal emparejadas (0x80880FE4-0x80881010:
  `@1561 @1562 @1567 @1564 @1565 @1566 @1567 @1568 ... @1572` repetidas; las nuestras son @1565-@1572, @1576 en orden
  y con los mismos valores). Renombradas por direccion (pdfix) el ctor baja de 97,1 a 97,03 (las cargas que
  casaban por nombre falso dejan de casar) y queda el orden de las escrituras de 0x1C8-0x1E0 (antes de cargar las
  constantes en el original): no commiteado por la bajada de fuzzy. Pendiente: renombrar a la vez que se arregle.
- chr/ControlCannonLift vf10 (99,26): sin probar nada nuevo (orden de `new` frente a this/resultado).
- game/PickTask vf10 (96,56): `!(m560 != 6 && m560 != 7)` da los dos `cmpwi` del original (96,65); fila
  `int* row = m4C[i]` igual. Quedan los spills de wasSafe/near/detect. Sin commit.
- atn/TextureDraw_unk fn_800D5F68 (98,03): escrituras por `sCamera.` en vez de `c->` y orden de `c`/`cam`: igual
  (el original guarda `c` en r28 y una sexta GPR salvada).
- game/Em001Necromancer ctor (99,0): permutaciones de m9C / cadena de m18 / m14C0 / los dos `clear()`
  (`stmtperm.py`, 120 ordenes): 99,03 como mucho (los dos clear() al reves cargan en orden pero guardan al reves).
- MP/CryptMP vf0C: solo revisada (bloqueo del 0.0 double en .bss).
- atn/LipSync_unk fn_8007A530: perm.py de 5 locales: 99,51; `int i` igual; sin `key` 93,6; inline VowelOf_unk 99,14.

## Para el port

- Ninguna unidad del port tocada. Sin cambios de comportamiento (todo son reescrituras equivalentes).

## Decisiones tomadas sin el usuario

- `gmk::ArenaFloor::~ArenaFloor()` deja de ser inline en la cabecera y se define en el .cpp tras fn_8041DDE4
  (es donde lo pone el original). Renumera las `@N` de game/GimmickCommon (la incluye): re-emparejadas con pd.sh
  y pdfix.py (seis etiquetas repetidas `@10799/@10800` de 0x80885F64-0x80885F78 pasan a nombres unicos).
- Inlines `static` nuevas en los .cpp (nombres inventados): `SetTransXYZ_unk`, `SetRotZYX_unk`, `UpdateLeaf_unk`
  (Leaf), `MenuGroup_unk` (CharaLight, Ocean, WaitObject).
- gmk/Ocean y gmk/WaitObject vf6C (fuera de la cola, en ninguna cola del lote): mismo arreglo que CharaLight,
  commiteado (2 funciones mas al 100 %; ninguna de las dos unidades enlaza todavia).

## Hallazgos

- El orden de declaracion de los locales (todos arriba del bloque, asignados despues) decide los registros
  volatiles tambien dentro de un `switch`: perm.py sobre los locales de cada `case` (gmk/ArenaFloor vf0C). (escribir-20261116 qN3)
- Una funcion al 100 % con `main.dol` FAILED por el dtor virtual inline de la cabecera: el original lo define
  (no inline) justo tras el cargador; la copia debil de `SimpleArray<T>::~SimpleArray` que queda en su sitio la
  quita el enlazador (gmk/ArenaFloor). `doldiff.py` (scratch-N3/bin) da la primera direccion distinta. (escribir-20261116 qN3)
- Orden de pila invertido en un bucle con matrices: cuerpo en una inline y la rotacion (identidad incluida) en otra
  inline anidada; traslacion por una inline de tres floats (argumentos z, y, x) (gmk/Leaf vf10). (escribir-20261116 qN3)
- vf6C de los gimmicks (`lis` del pool antes del `mr r3, group`): el `if` que crea el grupo del menu en una inline
  `MenuGroup_unk(self, menu, name)` que devuelve el grupo (CharaLight, Ocean, WaitObject; SwingObject salvo `.sdata2`). (escribir-20261116 qN3)
- Una inline no `static` del .cpp emite sus literales como `@STRING@<inline>` y no en `@stringBase0`: las inlines
  auxiliares nuevas con cadenas, siempre `static` (Em001Necromancer). (escribir-20261116 qN3)
- `lfs fN, 0x97c(r31)` junto a `psq_l` por un registro en una copia de Vector3: es `__VecCopy` con el origen a mas
  de 0x7FF del objeto (psq_l solo tiene 12 bits de desplazamiento y MWCC pliega el de lfs). (escribir-20261116 qN3)

## Bloqueos

- EfpData_unk::fn_802440F0 99,36 (con el bucle en inline): contador/desplazamiento r29/r30 cruzados.
- InteractionSpot Start, CharaLight-like ya resuelto; CostumeEditChara vf14 (`addi+lwzx`) sigue.

## Cambios de splits/symbols

- splits.txt sin cambios. symbols.txt: solo renumerados `@N` (pd.sh/pdbatch.sh/pdfix.py) de gmk/ArenaFloor,
  game/GimmickCommon, gmk/Leaf, gmk/CharaLight, gmk/Ocean y gmk/WaitObject.
- Scripts: `scratch-N3/bin/side.py <unidad> <simbolo> [desde] [hasta]` (las dos columnas de objdiff),
  `doldiff.py` (primera direccion distinta del main.dol), `cands.sh` (cuenta diferencias semanticas con semdiff),
  `stmtperm.py <unidad> <simbolo> <fichero> <bloque_viejo> <bloque1> ...` (prueba todos los ordenes de varios
  bloques de sentencias), copias de perm.py/t.sh/semdiff.py con rutas de N3.
