# escribir-20261116, cola G4 (atn/PacketPack, cuarta pasada: al 100 %)

Inicio: 2026-11-17 06:06 CET (tope 08:36).

## Tabla de la cola

| funcion | antes | despues |
|---|---|---|
| fn_804EE040 | 99,21 % | 99,21 % (sin cambios: ver Bloqueos) |
| `__sinit_\PacketPack_cpp` | 99,98 % | 100 % (pool con los mensajes de basic_string) |
| fn_804D8610 | 99,76 % | 100 % |
| fn_804DDE34 | 99,78 % | 100 % |
| fn_804E6518 | 99,84 % | 100 % |
| fn_804EA8B8 | 98,75 % | 100 % |
| fn_804EAA84 | 99,84 % | 99,91 % (queda el registro de `num`) |
| fn_804DD3F0 | 98,81 % | 99,07 % |
| fn_804EE4F0 | 99,63 % | 99,89 % (queda el orden de los argumentos de PSVECDotProduct) |
| fn_804F2450 | 99,00 % | 99,16 % (copias de `m13D8`/`m13E8` con `Assign_unk`) |
| fn_804DA690 | 96,89 % | 98,58 % (registros: m564 del primer fn_804DB408) |
| fn_804E82C4 | 95,52 % | 96,0 % (rango de `slot` con el literal a la izquierda) |
| fn_804D996C | 95,77 % | 99,57 % (llama a fn_804FB470, que el original inlinea) |
| fn_804EF0F0 | 99,07 % | 99,71 % (queda el `lwzx` del personaje encontrado) |

## Para el port

- Sin cambios de comportamiento: fn_804D8610, fn_804DD3F0, fn_804DDE34, fn_804E6518 y fn_804EA8B8 pasan
  `GetMyAid_unk()` directamente a fn_804DD158 (antes un local `unsigned int aid`); fn_804EAA84 copia `rot` con
  `atn::VecAssign_unk`; fn_804EE4F0 y fn_804F2450 copian `m13D8`/`m13E8` con `Assign_unk` (z primero; antes `VecAssign_unk` y `=`);
  fn_804DA690: las tres salidas en un solo `if` con `||`, rangos `0 <= rank && rank < 2`, y el `if (!last)` con los
  objetos 0x1E79.. primero (mismo comportamiento).
  fn_804D996C llama a `fn_804FB470(mgr, 158, 9)` (antes el cuerpo escrito a mano sin la prueba de m9750; mismo
  comportamiento: con m9750 = -1 la prueba no cambia nada).
  fn_804EF0F0 usa `IsServer_unk() == 1` (antes la comparacion de aids escrita a mano). Mismo comportamiento.

## Decisiones tomadas sin el usuario

- PacketPack incluye `std/string_members.h` (como GimmickManager/EquipItemData): el `@stringBase0` original tiene los
  tres mensajes de `basic_string` antes de "CustomData"; con ellos `poolorder` da igual y `__sinit` sale al 100 %.
  Las copias debiles de los miembros las quita el enlazador.

## Hallazgos

- PacketPack: la busqueda inlineada fn_804DD158 con aid/contador cruzados (r5/r4 en vez de r4/r5) era un local con
  nombre para el aid (`unsigned int aid = GetMyAid_unk(); fn_804DD158(mgr, aid)`): pasando la llamada como
  argumento el original sale al 100 % (fn_804D8610, fn_804DDE34, fn_804E6518, fn_804EA8B8); fn_804E8A80 si
  lleva el local (escribir-20261116 qG4)

- Copia de `Vector3` con origen y destino cruzados que ni `VecAssign_unk` ni `=` arreglan: mirar si el original
  carga z primero (`lfs f2, 8(src)` antes del `psq_l`): entonces es `dst.Assign_unk(src)` (PacketPack fn_804EE4F0,
  dos copias) (escribir-20261116 qG4)
- `(a == b) == 1` escrito en linea se pliega en `cmplw`; con la misma comparacion dentro de una inline `int`
  (`IsServer_unk() == 1`) MWCC la materializa (`subf; cntlzw; srwi; cmpwi 1`) como el original (PacketPack
  fn_804EF0F0) (escribir-20261116 qG4)

- `x >= 0 && x < 2` lo pliega MWCC en `cmplwi x, 1` (y `x >= 2 && x < 4` en `subi; cmplwi`); con el literal a la
  izquierda (`0 <= x && x < 2`) deja los dos `cmpwi` del original (PacketPack fn_804DA690) (escribir-20261116 qG4)
- `bne; b fin` donde el nuestro tiene `beq fin` (y al reves en el `if` anterior): el ultimo termino de un `||` sale
  invertido con `b`; juntar los `if (...) return;` seguidos en un solo `if` con `||` (fn_804DA690)
  (escribir-20261116 qG4)

- Una prueba de una constante sin plegar (`li r0, -162; cmplwi r0, 1`) delata una funcion inlineada con sus
  argumentos constantes: buscar la funcion con esa prueba (`(m9750 - 0xA1) <= 1` de fn_804FB470) y llamarla
  (PacketPack fn_804D996C 95,8 -> 99,6 %) (escribir-20261116 qG4)

## Bloqueos

- PacketPack fn_804EE040 (99,21 %): con `(unsigned char)type` en las cuatro llamadas toda la estructura sale como el
  original (el `clrlwi r4` del switch, el cero compartido con `data`/`type`) y solo queda una permutacion de registros
  (aid r31 -> r26; 98,92 %). Probados: `unsigned char`/`unsigned int` para `type`, un local `unsigned char kind` para
  el switch, parametros `unsigned char` en los manejadores (97,5 %), los 24 ordenes de declaracion de type/header/
  cursor/player (98,87-99,26 %, el mejor renumera `@N`), `if (player != 0) switch`, fn_804F4FC4 con `unsigned int`
  (escribir-20261116 qG4)
- PacketPack fn_804CF468 (99,91 %): el hueco compartido de los dos `fctiwz` no sale con una inline `PackValue_unk`,
  `(unsigned short)(int)` ni `id` declarado arriba (escribir-20261116 qG4)
- PacketPack fn_804EDCCC (99,86 %): r4/r0 del byte de user data y del modo: probados el local `unsigned int`, la
  expresion sin locales, un puntero `mgr`, inline bool (con `&&` y con `if`) (escribir-20261116 qG4)
- PacketPack fn_804EE4F0/fn_804F2450: orden de las dos direcciones de PSVECDotProduct (`PPDot_unk` por puntero,
  llamada directa, `dir(...)`, referencia `front`, comparacion al reves: igual o peor) (escribir-20261116 qG4)
- PacketPack fn_804F39D4/fn_804F3880 y PPFindTeam_unk (fn_804F2040, fn_804F2258): el manager en r5 y el
  desplazamiento en r3 del original; un local para el aid o el manager, y el getter fn_800F7F90: igual o peor
  (escribir-20261116 qG4)
- PacketPack fn_804EB26C: el original relee `mFlags` para el segundo `if (m28)` (el nuestro lo reutiliza); probado
  releer por el indice (escribir-20261116 qG4)
- PacketPack fn_804DC858/fn_804EDB0C: la lista de amigos (`+0x430`) un registro mas abajo que en el original; orden
  de declaraciones y contador `unsigned`: peor (escribir-20261116 qG4)
- PacketPack fn_804E4B44: `m08[i - 1]` sale reducido al puntero del bucle (el original `mulli`); con
  `(&m08[i])[-1]` igual (escribir-20261116 qG4)

## Cambios de splits/symbols

- Re-etiquetado `@N` de PacketPack (pd.sh + `/home/claude/scratch-G3/bin/fixlbl.py`). Splits sin cambios.

## Scripts

- `/home/claude/scratch-G4/bin/nl.py save|cmp NOMBRE` (tras `m.sh`): cuenta por funcion las diferencias que no son
  solo el nombre de una etiqueta (`@N`, `jumptable_`, `lbl_`): compara variantes sin pasar pd.sh.
- `/home/claude/scratch-G4/bin/m.sh <unidad> <patrones>`: compila y da el % de objdiff-cli (no el de report.json).
