# escribir-20261116, cola G5 (atn/PacketPack, quinta pasada: al 100 %)

Inicio: 2026-11-17 08:08 CET (tope 10:38).

## Tabla de la cola

| funcion | antes | despues |
|---|---|---|
| fn_804D996C | 99,57 % | 100 % (contador `int i` frente a `size()` sin signo) |
| fn_804EF0F0 | 99,71 % | 99,87 % (`begin()[i]` en el return de PPCharaOfAid_unk: el `lwzx` del original) |
| fn_804F2994 | 97,66 % | 98,07 % (PPCharaOfId_unk en vez de GetHitAttacker_unk: los `lwzx`) |
| fn_804EAA84 | 99,91 % | 99,91 % (sin cambios: ver Bloqueos) |
| fn_804CF198 | 99,37 % | 99,37 % (sin cambios: ver Bloqueos) |

## Para el port

- Sin cambios de comportamiento: fn_804D996C cuenta con `int i` (antes `unsigned int`); PPCharaOfAid_unk devuelve
  `mPlayers.begin()[i].mRef.mChara` (mismo objeto); fn_804F2994 busca atacante y objetivo con PPCharaOfId_unk
  (mismo cuerpo que GetHitAttacker_unk; `PPMsgCharaRef_unk` y `PPId_unk` tienen la misma forma {tipo, indice}).

## Decisiones tomadas sin el usuario

## Hallazgos

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
