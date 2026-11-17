# escribir-20261116, cola G5 (atn/PacketPack, quinta pasada: al 100 %)

Inicio: 2026-11-17 08:08 CET (tope 10:38).

## Tabla de la cola

| funcion | antes | despues |
|---|---|---|
| fn_804D996C | 99,57 % | 100 % (contador `int i` frente a `size()` sin signo) |
| fn_804EF0F0 | 99,71 % | 99,87 % (`begin()[i]` en el return de PPCharaOfAid_unk: el `lwzx` del original) |
| fn_804F2994 | 97,66 % | 98,07 % (GetHitAttacker_unk con `begin()[i]`: los `lwzx`) |
| fn_804EEB34 | 99,25 % | 100 % (GetHitAttacker_unk sin local para el slot y con `begin()[i]`) |
| fn_804EED7C | 99,07 % | 99,75 % (idem) |
| fn_804EF9B4 | 99,40 % | 100 % (idem, y sin el local `mgr`) |
| fn_804EFC94 | 99,15 % | 100 % (idem) |
| fn_804D8910 | 98,97 % | 99,41 % (contador `int i`) |
| fn_804D9C14 | 94,17 % | 95,19 % (contador `int i`) |
| fn_804DDA04 | 95,52 % | 98,07 % (contadores `unsigned int` sin `(int)`) |
| fn_804DF4B8 | 97,36 % | 97,89 % (contadores `unsigned int` sin `(int)`) |
| fn_804EB934 | 96,79 % | 96,83 % (contador del tercer bucle con `(int)`) |
| fn_804DD1A8 | 99,35 % | 100 % (fn_804DD158 sin local para el slot) |
| fn_804DD3F0 | 99,07 % | 100 % (fn_804DD158 sin local para el slot) |
| fn_804F47FC | 98,80 % | 99,17 % (idem) |
| fn_804DA690 | 98,57 % | 98,78 % (idem) |
| fn_804F60E0 | 99,60 % | 100 % (local `v` declarado antes del ultimo bucle) |
| fn_804FC7E0 | 96,86 % | 98,90 % (dos `if` del equipo sin `else`; `point` declarado arriba) |
| fn_804E8BF4 | 98,87 % | 99,76 % (`category` asignada despues de `value`) |
| fn_804EFF74 | 99,23 % | 99,35 % (push_back de las entradas por una inline sobre el CharaStatus; getter fn_8013A194) |
| fn_804CF640 | 96,92 % | 98,41 % (getter debil fn_8013A194 del BattleTask en las llamadas) |
| fn_804EAA84 | 99,91 % | 99,91 % (sin cambios: ver Bloqueos) |
| fn_804CF198 | 99,37 % | 99,37 % (sin cambios: ver Bloqueos) |

## Para el port

- Sin cambios de comportamiento: fn_804D996C cuenta con `int i` (antes `unsigned int`); PPCharaOfAid_unk devuelve
  `mPlayers.begin()[i].mRef.mChara` (mismo objeto), y GetHitAttacker_unk tambien (sin el local del slot);
  fn_804EF9B4 sin el local `mgr`; fn_804EFF74 usa la copia
  `GetHitChara_unk` (la forma anterior, que lee el personaje por el slot).
- Sin cambios de comportamiento: tipo del contador y del limite (`int`/`unsigned int`, con o sin `(int)size()`) en
  los bucles de fn_804D8910, fn_804D9C14, fn_804DDA04, fn_804DF4B8 y fn_804EB934.
- Sin cambios de comportamiento: fn_804DD158 devuelve `&mgr->mPlayers[i]` (antes un local); `PPFindSlot_unk`
  (misma busqueda) en fn_804D90E0, fn_804EB010 y fn_804F3EEC.
- fn_804FC7E0: las dos pruebas del equipo (2: `mId >= 50`, 1: `mId < 50`) son dos `if` seguidos (antes `else if`);
  mismo comportamiento. fn_804E8BF4: `category` se asigna despues de `value` (mismo comportamiento).
- fn_804EFF74: el push_back de las entradas de estado pasa por la inline nueva `PPAddEntry_unk(CharaStatus*, const
  CharaStatusEntry_unk&)` (mismo comportamiento).
- fn_804CF640 y fn_804EFF74 llaman a los metodos del BattleTask a traves de la inline `fn_8013A194()` (getter debil
  de lbl_8087F048, como en Em203GreaterDaemon); mismo comportamiento.
- Sin cambios de comportamiento: fn_804F60E0 limita el puesto del ultimo bucle a traves de un local `v`.

## Decisiones tomadas sin el usuario

- GetHitAttacker_unk lee el personaje con `begin()[i]` (sube fn_804EEB34, fn_804EED7C, fn_804EF9B4, fn_804EFC94,
  fn_804F2994); fn_804EFF74 necesita la forma anterior: copia `GetHitChara_unk` (`// inferred:`) solo para el.

- fn_804DD158 (100 % en las dos formas) se escribe sin el local `player` (`return &mgr->mPlayers[i];`): asi salen al
  100 % fn_804DD1A8 y fn_804DD3F0 (y suben fn_804F47FC y fn_804DA690). fn_804D90E0, fn_804EB010 (100 %) y fn_804F3EEC
  necesitan la forma con el local: inline nueva `PPFindSlot_unk` (mismo cuerpo con el local, `// inferred:`) en sus
  cuatro llamadas.

## Hallazgos

- El original carga un global en un registro salvado antes de evaluar los argumentos de la llamada a un metodo
  (`lwz r29, global` antes de las llamadas virtuales de los argumentos; `mr r3, r29` despues): el objeto viene de un
  getter debil inline (`fn_8013A194()->f(a, b->vf90())`) y no del global (PacketPack fn_804CF640 96,9 -> 98,4 %)
  (escribir-20261116 qG5)

- `push_back` de un miembro FixedArray cuya cuenta el original direcciona desde la base del objeto que lo contiene
  (`addi rX, chara, 0x7D4; lwz 0x304(rX)` en vez de `addi rX, chara, 0xAD8; lwz 0(rX)`): la llamada estaba dentro de una
  inline que recibe el objeto contenedor (`PPAddEntry_unk(status, e)`) (PacketPack fn_804EFF74) (escribir-20261116 qG5)

- Fila y valor cruzados (r5/r6) en un bucle `p->m = MIN(MAX(p->m, 1), N)`: un local para el valor declarado antes del
  bucle (`unsigned long v; for (...) { v = p->m; p->m = MIN(MAX(v, 1), N); }`) da el orden del original; declarado
  dentro del bucle cruza el valor con el contador (PacketPack fn_804F60E0) (escribir-20261116 qG5)

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

- PacketPack: el original relee el tamano de `mPlayers` (o `lbl_8087F610`) dentro del cuerpo de los bucles que llaman a
  fn_804D5F0C/PPSlot_unk (fn_804DCA54, fn_804DE2F8, fn_804DF310, fn_804E5BC4...) y el nuestro reutiliza la carga de la
  condicion; probado el bucle de la cuenta por equipos en una inline con `int* counts`: igual (escribir-20261116 qG5)
- PacketPack fn_804F3258 (99,57 %): datos/cola en r7/r6; probados sin local de cola, getter fn_800F7F90, cola declarada
  antes, `id` local, una inline (cola, datos) y (datos, cola): igual o peor (escribir-20261116 qG5)
- PacketPack fn_804E6DD0 (99,85 %): `own`/`head` en r14/r15 cruzados; probados `head` declarado antes, `own`
  asignado despues y PPFindSlot_unk: igual (escribir-20261116 qG5)

- PacketPack fn_804EAA84 (99,91 %): `num` en el registro de `sel` (r27) y no en el de `index` (r23); probados `int num`,
  `num` declarado dentro del `if` y al principio de la funcion: igual o peor (escribir-20261116 qG5)
- PacketPack fn_804CF198 (99,37 %): `d` en f3 (registro de `frame`) y no en f1; probados `__fabs` sin `(float)`,
  local para el limite y para el valor absoluto, `speed * 4.0f`, `d` double, sin `d`, `+=`/`-=`, referencia al slot,
  `k = 255.0f`: igual o peor (escribir-20261116 qG5)
- PacketPack fn_804EFF74/fn_804F2994: el original relee `lbl_8087F610` antes de la segunda busqueda (atacante y
  objetivo); un local `mgr` para la segunda y una inline que lee el global no cambian nada (escribir-20261116 qG5)

## Cambios de splits/symbols
