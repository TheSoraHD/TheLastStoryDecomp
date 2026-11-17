# escribir-20261116, cola G5 (atn/PacketPack, quinta pasada: al 100 %)

Inicio: 2026-11-17 08:08 CET (tope 10:38).

## Tabla de la cola

| funcion | antes | despues |
|---|---|---|
| fn_804D996C | 99,57 % | 100 % (contador `int i` frente a `size()` sin signo) |
| fn_804EF0F0 | 99,71 % | 99,87 % (`begin()[i]` en el return de PPCharaOfAid_unk: el `lwzx` del original) |
| fn_804F2994 | 97,66 % | 98,07 % (PPCharaOfId_unk en vez de GetHitAttacker_unk: los `lwzx`) |
| fn_804D8910 | 98,97 % | 99,41 % (contador `int i`) |
| fn_804D9C14 | 94,17 % | 95,19 % (contador `int i`) |
| fn_804DDA04 | 95,52 % | 98,07 % (contadores `unsigned int` sin `(int)`) |
| fn_804DF4B8 | 97,36 % | 97,89 % (contadores `unsigned int` sin `(int)`) |
| fn_804EB934 | 96,79 % | 96,83 % (contador del tercer bucle con `(int)`) |
| fn_804DD1A8 | 99,35 % | 100 % (fn_804DD158 sin local para el slot) |
| fn_804DD3F0 | 99,07 % | 100 % (fn_804DD158 sin local para el slot) |
| fn_804F47FC | 98,80 % | 99,17 % (idem) |
| fn_804DA690 | 98,57 % | 98,78 % (idem) |
| fn_804EAA84 | 99,91 % | 99,91 % (sin cambios: ver Bloqueos) |
| fn_804CF198 | 99,37 % | 99,37 % (sin cambios: ver Bloqueos) |

## Para el port

- Sin cambios de comportamiento: fn_804D996C cuenta con `int i` (antes `unsigned int`); PPCharaOfAid_unk devuelve
  `mPlayers.begin()[i].mRef.mChara` (mismo objeto); fn_804F2994 busca atacante y objetivo con PPCharaOfId_unk
  (mismo cuerpo que GetHitAttacker_unk; `PPMsgCharaRef_unk` y `PPId_unk` tienen la misma forma {tipo, indice}).
- Sin cambios de comportamiento: tipo del contador y del limite (`int`/`unsigned int`, con o sin `(int)size()`) en
  los bucles de fn_804D8910, fn_804D9C14, fn_804DDA04, fn_804DF4B8 y fn_804EB934.
- Sin cambios de comportamiento: fn_804DD158 devuelve `&mgr->mPlayers[i]` (antes un local); `PPFindSlot_unk`
  (misma busqueda) en fn_804D90E0, fn_804EB010 y fn_804F3EEC; `PPOwnPlayer_unk` definida antes (sin cambios).

## Decisiones tomadas sin el usuario

- fn_804DD158 (100 % en las dos formas) se escribe sin el local `player` (`return &mgr->mPlayers[i];`): asi salen al
  100 % fn_804DD1A8 y fn_804DD3F0 (y suben fn_804F47FC y fn_804DA690). fn_804D90E0, fn_804EB010 (100 %) y fn_804F3EEC
  necesitan la forma con el local: inline nueva `PPFindSlot_unk` (mismo cuerpo con el local, `// inferred:`) en sus
  cuatro llamadas.

## Hallazgos

- Busqueda inlineada cuyo resultado sale un registro mas abajo (o cruzado con el desplazamiento) que en el original:
  probar la busqueda sin local para el elemento (`return &v[i];`, indexando otra vez en la prueba) frente a
  `T* p = &v[i]; ... return p;`; el original puede tener las dos formas en inlines distintas (PacketPack
  fn_804DD158 frente a PPFindSlot_unk) (escribir-20261116 qG5)

- Bucle `for` con el contador en un registro y el desplazamiento en otro cruzados (r4/r3) y `cmplwi` del tamano:
  `for (int i = 0; i < v.size(); i++)` (int frente a `size()` sin signo); con `unsigned int i` y `(int)size()` cruza
  los registros y con `int i` y `(int)size()` sale `cmpwi` (PacketPack fn_804D996C) (escribir-20261116 qG5)
- Busqueda inlineada que lee el resultado con `lwzx base, desplazamiento` en vez de `lwz 0(elemento)`: el return
  indexa otra vez el vector por `begin()[i]` (PacketPack PPCharaOfAid_unk) (escribir-20261116 qG5)

## Bloqueos

- PacketPack fn_804EAA84 (99,91 %): `num` en el registro de `sel` (r27) y no en el de `index` (r23); probados `int num`,
  `num` declarado dentro del `if` y al principio de la funcion: igual o peor (escribir-20261116 qG5)
- PacketPack fn_804CF198 (99,37 %): `d` en f3 (registro de `frame`) y no en f1; probados `__fabs` sin `(float)`,
  local para el limite y para el valor absoluto, `speed * 4.0f`, `d` double, sin `d`, `+=`/`-=`, referencia al slot,
  `k = 255.0f`: igual o peor (escribir-20261116 qG5)
- PacketPack fn_804EFF74/fn_804F2994: el original relee `lbl_8087F610` antes de la segunda busqueda (atacante y
  objetivo); un local `mgr` para la segunda y una inline que lee el global no cambian nada (escribir-20261116 qG5)

## Cambios de splits/symbols
