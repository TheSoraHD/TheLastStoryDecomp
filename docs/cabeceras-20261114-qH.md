# Cola H (cabeceras-20261114): segundas pasadas de las funciones que desbloquean las cabeceras

Rama `agent-H`, referencia `/home/claude/ref-H` (15433 / 18109 tras integrar A+B+C+D+F).

## Resultados

1. **`game/MslSort_unk.h`: seleccion con `MslMinElement_unk`** (MSL `min_element` como inline aparte, con su
   propio `result`; el bucle externo `Iter end(last); for (--end; ...)`). Re-emparejado en lote (pdbatch de las 6
   unidades que la incluyen + `lblmap` de Event/Node). **15433 -> 15436**:
   - `MslSort_unk<ExpEntry_unk*, ExpOrderLess_unk>` y su copia `R`: 99,74 -> **100 %**.
   - `MslSelectionSort_unk<ShopEntry5C_unk*>` (ShopBase): 99,40 -> **100 %**.
   - `MslSort_unk<ItemCandidate_unk*>` x4: 99,27 -> 99,93 (queda r7/r8 en el comparador).
   - CosEditItemList_unk: los 8 sorts 95,0/95,6 -> 95,8/96,3; las dos `MslSelectionSort_unk` 97,0/97,3 -> 98,7/98,8.
   - NewCampPartsItem BagLess 94,43 -> 94,89; Event/Node 78,4 -> 78,5.
   - **Bajada aceptada**: `MslSelectionSort_unk<NewCampItemEntry_unk, NewCampCategoryLess_unk>` 96,15 -> 95,47
     (fuera de linea; ver decisiones).

## Decisiones tomadas sin el usuario

- Aceptada la bajada de 0,7 % de `MslSelectionSort_unk<NewCampItemEntry_unk>` (0x8051AD54) a cambio de 3
  funciones al 100 % y 14 subidas: se probaron 16 variantes de la seleccion (`end(last - 1)` / `--end`, retorno
  temprano, `while`, cuatro formas de `min_element`); ninguna sube todas. `precommit.sh` da FAIL solo por esa
  bajada (main.dol OK, lost 0, labelcheck/rep100 igual).

## Para el port

- Sin cambios de tipos, firmas ni layouts. `include/game/MslSort_unk.h` tiene una plantilla inline nueva
  (`MslMinElement_unk`); `MslSelectionSort_unk` la usa.

## Hallazgos (una linea)

- Seleccion de MSL: `min_element` como inline aparte con su propio iterador `result` (no el `min` local) da los
  registros del original en las ordenaciones por puntero.
