# escribir-20261116, cola G4 (atn/PacketPack, cuarta pasada: al 100 %)

Inicio: 2026-11-17 06:06 CET (tope 08:36).

## Tabla de la cola

| funcion | antes | despues |
|---|---|---|
| fn_804EE040 | 99,21 % | (en curso) |
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
| fn_804EF0F0 | 99,07 % | 99,71 % (queda el `lwzx` del personaje encontrado) |

## Para el port

- Sin cambios de comportamiento: fn_804D8610, fn_804DD3F0, fn_804DDE34, fn_804E6518 y fn_804EA8B8 pasan
  `GetMyAid_unk()` directamente a fn_804DD158 (antes un local `unsigned int aid`); fn_804EAA84 copia `rot` con
  `atn::VecAssign_unk`; fn_804EE4F0 y fn_804F2450 copian `m13D8`/`m13E8` con `Assign_unk` (z primero; antes `VecAssign_unk` y `=`);
  fn_804DA690: las tres salidas en un solo `if` con `||`, rangos `0 <= rank && rank < 2`, y el `if (!last)` con los
  objetos 0x1E79.. primero (mismo comportamiento).
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

## Bloqueos

## Cambios de splits/symbols

- Re-etiquetado `@N` de PacketPack (pd.sh + `/home/claude/scratch-G3/bin/fixlbl.py`). Splits sin cambios.

## Scripts

- `/home/claude/scratch-G4/bin/nl.py save|cmp NOMBRE` (tras `m.sh`): cuenta por funcion las diferencias que no son
  solo el nombre de una etiqueta (`@N`, `jumptable_`, `lbl_`): compara variantes sin pasar pd.sh.
- `/home/claude/scratch-G4/bin/m.sh <unidad> <patrones>`: compila y da el % de objdiff-cli (no el de report.json).
