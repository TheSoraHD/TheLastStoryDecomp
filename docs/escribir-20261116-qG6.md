# escribir-20261116, cola G6 (atn/PacketPack, sexta pasada, corta)

Inicio: 2026-11-17 10:26 CET (tope 11:35). Fin: ~10:55.

Resumen: ninguna funcion nueva al 100 % (report 15974 / 18109 igual, lost 0, sin bajadas de fuzzy, labelcheck/rep100
igual, PRECOMMIT OK en los dos commits). Se quitan los 13 `// NONMATCHING` obsoletos (punto 3) y se encuentra la
forma de la relectura del tamano de `mPlayers` (punto 1) en parte de los bucles: la condicion del bucle y la busqueda
inline del cuerpo tienen que partir de expresiones base distintas (global frente a local, o referencia local al
vector frente a `mgr`); con la misma base MWCC reutiliza la carga de la condicion.

## Tabla de la cola

| funcion | antes | despues |
|---|---|---|
| fn_804DCA54 | 99,31 % | 99,52 % (condicion con `lbl_8087F610`, cuerpo con `mgr2`: relee el tamano; quedan r6/r7 cruzados) |
| fn_804DD574 | 90,47 % | 94,20 % (idem, condiciones de los bucles `mgr2`) |
| fn_804E5BC4 | 99,17 % | 99,60 % (idem) |
| fn_804DE2F8 | 98,34 % | 98,96 % (idem) |
| fn_804DE884 | 99,81 % | 99,99 % (referencia local `players` en la condicion; queda `lwz 0(r29)` del mFlagC25) |
| fn_804DDA04 | 98,09 % | 98,54 % (referencia local `players`) |
| fn_804EB934 | 96,87 % | 96,95 % (referencia local `players`) |
| fn_804F1564 | 99,24 % | 99,24 % (sin cambios: ver Bloqueos) |
| fn_804F3BAC | 97,86 % | 97,86 % (sin cambios: ver Bloqueos) |
| fn_804CF468 | 99,91 % | 99,91 % (sin cambios: ver Bloqueos) |
| 13 funciones al 100 % (fn_804DBD08, fn_804DE560, fn_804DF11C, fn_804DF404, fn_804E7628, fn_804EB010, fn_804EB2D8, fn_804EB4AC, fn_804EB814, fn_804F4E40, fn_804FB220, fn_804FC010, fn_804FC0D4) | 100 % con `// NONMATCHING` | 100 %, comentario quitado |

## Para el port

- Sin cambios de comportamiento: los bucles con `mgr2 = lbl_8087F610` (fn_804DCA54, fn_804DD574, fn_804DE2F8,
  fn_804E5BC4 y uno de fn_804DDA04) leen el tamano en la condicion por `((PPManager2_unk*)lbl_8087F610)` (el mismo
  objeto que `mgr2`).
- Sin cambios de comportamiento: fn_804DDA04, fn_804DE884 y fn_804EB934 leen el tamano de un bucle por una referencia
  local `PPPlayerArray_unk& players = mgr->mPlayers`.

## Decisiones tomadas sin el usuario

- Se commitean las subidas de fuzzy del punto 1 aunque ninguna llegue al 100 %: son la forma que pedia la cola y no
  bajan nada (fn_804DE884 queda a una instruccion).

## Hallazgos

- Tamano de un vector que el original relee dentro del cuerpo de un bucle `for (i < v.size()) { p = Slot(mgr, i) }`
  (la busqueda inline vuelve a cargar `size`) y el nuestro reutiliza la carga de la condicion: la condicion y la
  busqueda parten de expresiones base distintas (`lbl_X->v.size()` en la condicion y el local `mgr2` en el cuerpo, o
  una referencia local `T& v = mgr->v;` en la condicion); el cambio de tipo de la carga no influye (PacketPack
  fn_804DE884 99,81 -> 99,99 %, fn_804DD574 90,5 -> 94,2 %) (escribir-20261116 qG6)
- Dos `fctiwz` que el original hace en el mismo hueco de pila: con `if/else` y la asignacion en cada rama sale el
  hueco compartido (marco 0x20), pero la tienda se duplica; con la ternaria cada conversion tiene su hueco
  (PacketPack fn_804CF468) (escribir-20261116 qG6)

## Bloqueos

- PacketPack fn_804CF468 (99,91 %): el hueco compartido de los dos `fctiwz`; probados `if/else` por rama (98,87 %,
  sale el hueco pero no el `clrlwi` comun), local `unsigned short`/`int` asignado en `if/else` o ternaria, `(int)`,
  ternaria con `0.0f` e inlines `PPPackF_unk(task, f)` (tres formas): igual o peor (escribir-20261116 qG6)
- PacketPack fn_804DE884 (99,99 %): el original relee `player->mRef.mChara` (r29) para mFlagC25 tras
  IsTeamPlayer_unk; probados `player->`, `players[i]`, `mgr->mPlayers[i]`, una inline `PPBattleZ(player)` y
  IsTeamPlayer sobre `player`/`players[i]`: igual o peor (escribir-20261116 qG6)
- PacketPack fn_804F3BAC y fn_804F1564: el original relee `lbl_8087F610` en el cuerpo y en la condicion; probados
  getter/global/local `mgr` cruzados entre condicion y cuerpo y un local dentro del cuerpo: igual o peor (fn_804F1564
  99,30 % con condicion getter y cuerpo `mgr`, pero cruza r6/r7 y r28/r31) (escribir-20261116 qG6)
- La referencia local `players` en el resto de bucles con `mgr` (fn_804DF11C, fn_804DF4B8, fn_804E4EB0, fn_804E5BC4 y
  dos de fn_804DE884) baja o no cambia (escribir-20261116 qG6)
- Hacer fn_804D5F0C metodo, `const PPManager2_unk*`, un puntero local al vector dentro de la inline o una inline de
  `size()` en la condicion: igual o peor en fn_804DCA54 (escribir-20261116 qG6)

## Cambios de splits/symbols

- Solo re-emparejado de etiquetas `@N` de PacketPack (pd.sh + fixlbl.py).

## Scripts

- `/home/claude/scratch-G6/bin/refloop.py list|apply N...`: lista los bucles `for (i < X->mPlayers.size())` con
  PPSlot_unk/fn_804D5F0C y les pone una referencia local `players` en la condicion. Resto: copias de los de G5 con
  rutas de G6.

## Pendiente

- fn_804DE884 (una instruccion), fn_804DCA54 (r6/r7), fn_804F1564/fn_804F3BAC (relectura del global).
