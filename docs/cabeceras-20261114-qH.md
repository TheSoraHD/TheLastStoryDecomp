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

2. **Accesores fuera de linea de CosEditItemList_unk** (cabeceras nuevas solo con cuerpos inline, para no
   renumerar): `Inventory::GetEquip_unk(int)` (declarada sin nombre de parametro en `game/Inventory.h`, cuerpo en
   `game/InventoryEquip_unk.h`; copia debil 0x801D80A0, antes `fn_801D80A0` al 0 %) y
   `NewCampTask::GetPairCount_unk() const` / `GetPair_unk(int)` (declaradas en `game/NewCampTask.h`, cuerpos en
   `game/NewCampPairs_unk.h`; copias 0x801DA2F0/0x801DA2F8, que el original lee con `this` = NewCampTask: antes
   se tomaban por `FixedArray_unk::size`/`operator[]`, 99,5/99,75 %). Renombrados en `symbols.txt` (3 lineas).
   NewCampPartsStatus llama al miembro en lugar del `extern "C" fn_801D80A0(Inventory*, int)` (regla 10).
   Resultado CosEditItemList_unk 55 -> 59 al 100 % (+ `fn_801DD26C` 96,6 -> **100**, `fn_801E07A4` 97,2 ->
   **100** con una sola entrada para los dos bucles y la llamada antes de los stores); total **15436 -> 15441**.

3. **`MslIter_unk::operator>=` como `!(a < b)`** (MSL; el original materializa el bool con xor/cntlzw) y, en los
   comparadores de CosEditItemList_unk, los nombres de los dos objetos en variables antes de `wcscmp`: sorts de
   CosEdit 95,8 -> 97,8 (proteccion) y 96,3 -> 98,0/98,2 (armas), medianas 97,9 -> 99,0/99,2, NewCampPartsItem
   BagLess 94,9 -> 96,7, Event/Node 78,5 -> 79,0. Queda la recarga de first/last tras `last - first` en la
   seleccion inline (probados `end(last - 1)`, `end = last`, `while`).

4. `lblmap.py game/Em305Vampire`: tres etiquetas de `.sdata2` renombradas (solo renombrados), `fn_802EBBA8`
   99,98 -> 100 en `objdiff-cli` (el report ya la daba al 100 %: no cambia el total, 15441).

5. **`NewCampCategoryLess_unk`** (NewCampPartsItem): `db->mKind` antes que `da->mKind` y los dos nombres en
   variables (b primero) antes de los `wcscmp`: comparador 98,1 -> 99,75, mediana 98,5 -> 99,8 y la seleccion
   fuera de linea 96,15 -> **96,57**, con lo que **desaparece la bajada** del punto 1. `PRECOMMIT OK`.
   Queda el reparto de registros de esos dos nombres (r29/r30 del original).

## Sin resolver (probado)

- `GameTask::fn_80371790` (99,15): marco 0x10 mayor y ranuras de las matrices de RotateYXZ_unk; no tocado.
- `Inventory::fn_80449ABC` (92,2): coloreo de registros y reduccion de induccion de los bucles; referencia en el
  segundo bucle baja (91,8).
- `GameTask::fn_8036F268` (99,77): registros del IsDungeonCleared_unk inline; retornos tempranos bajan vf0C y
  fn_803754F0.
- `GameTask::fn_80370320` (98,1): el original pasa `index` como segundo argumento a `GameUI::fn_803E43B8` y -1 a
  `fn_803E4434` (firma `(int, int)` en GameUI, unidad del port: no tocada) y relee mCount en Push_unk.
- `push_back` de vector (CosEdit 99,76, Em305Vampire vf0C, ShopUpGrade): ranuras de pila del `n` de
  `__grow_cap_unk` frente a los temporales de max(); el orden cambia segun la unidad (CosEdit al reves que
  ShopUpGrade). Probados max(n, x), `const size_type&` y alias por referencia: peor.
- `GameTask::fn_803729B4` (98,9): el original llama a `fn_802A4170`/`fn_802A4184` (las copias debiles
  `vector<EnemyTask*>` ctor y `operator[]` de Em203GreaterDaemonRedMp, hoy `extern "C"` con self alli: deuda de la
  regla 10, unidad fuera de la cola) y no llama al ctor vacio de `Matrix3x4`.
- `GameTask::fn_80370C2C` (99,06, 64 bytes): el original reusa r4 para mFrame; probados orden, local, `?:`, bool.
- `ShopUpGrade::fn_805969D4`: ademas el pool de cadenas anchas del original tiene un L"" sin referencia antes de
  L"+99" (offset 8, nuestro 6).

## Decisiones tomadas sin el usuario

- (Resuelto en el punto 5.) Aceptada en el primer commit la bajada de 0,7 % de
  `MslSelectionSort_unk<NewCampItemEntry_unk>` (0x8051AD54) a cambio de 3 funciones al 100 % y 14 subidas: se probaron 16 variantes de la seleccion (`end(last - 1)` / `--end`, retorno
  temprano, `while`, cuatro formas de `min_element`); ninguna sube todas. `precommit.sh` da FAIL solo por esa
  bajada (main.dol OK, lost 0, labelcheck/rep100 igual).

## Para el port

- `game/Inventory.h`: metodo nuevo `Equip_unk* GetEquip_unk(int)` (cuerpo en `game/InventoryEquip_unk.h`);
  `game/NewCampTask.h`: `GetPairCount_unk() const` y `GetPair_unk(int)` (cuerpos en `game/NewCampPairs_unk.h`).
  Sin cambios de layout. NewCampPartsStatus ya no declara `extern "C" fn_801D80A0`.
- Sin cambios de tipos, firmas ni layouts en lo demas. `include/game/MslSort_unk.h` tiene una plantilla inline nueva
  (`MslMinElement_unk`); `MslSelectionSort_unk` la usa.

## Hallazgos (una linea)

- Seleccion de MSL: `min_element` como inline aparte con su propio iterador `result` (no el `min` local) da los
  registros del original en las ordenaciones por puntero.
