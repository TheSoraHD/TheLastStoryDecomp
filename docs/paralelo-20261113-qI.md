# Lote paralelo-20261113, cola I (game/ItemManager_unk, game/AiTarget, game/GameTask)

## Resultado por unidad

- `game/ItemManager_unk`: funciones al 0 % 19 -> 10. Al 100 %: `fn_80444EFC` (dinero que suelta un enemigo),
  `fn_804491F4` (recompensas de un enemigo), `fn_80449448` (escala de recompensas por diferencia de nivel).
  `fn_80444C50` vuelve a 100 % tras el re-emparejado de datos. Escritas NONMATCHING: `fn_80444020` 88,7 %
  (constantes 0/-1/1/0xF0000 en r19-r23 en el original), `fn_80448B80` 99,85 % (operandos de cuatro `cmpw`
  con `atoi`), `Inventory::fn_80449ABC` 55 % (sin constructores, ver decisiones) y su `std::sort`
  (`MslSort_unk`/`MslMedian3_unk` con `ExpOrderLess_unk`, 0x8044A1D8/0x8044B44C/0x8044C6C0, 52-62 %: el
  original es mas grande, mas inlining).
- `game/AiTarget`: funciones al 0 % 11 -> 10. `fn_80016C10` escrita (NONMATCHING 98,76 %, registros:
  el original pone la escena en r30 y el indice*4 en r29, al reves que el nuestro).

## Decisiones tomadas sin el usuario

- `AIManager::fn_80016A94`/`fn_80016C10`: el argumento era `boost::function<void()>` (nombre inventado en una
  sesion anterior), pero el original llama al invocador con el punto y prueba el resultado: pasa a
  `boost::function<int(gs::MovePoint*)>` (firma deducida; renombrados en `symbols.txt`). `AiTarget.h` gana la
  declaracion adelantada `gs::MovePoint` (19 unidades la incluyen; precommit sin bajadas).
  `game/CrowdSimulation.cpp` sigue llamando a `fn_80016C10` como `extern "C"` (pendiente, otra unidad).
- `ItemManager_unk`: la vista `InventoryField_unk` de `lbl_8087F430` se sustituye por `GameTask*` (regla 10).
  Las funciones nuevas siguen el convenio del fichero (`extern "C"` sin clase: los llamadores de otras unidades
  las declaran asi); `fn_80444020` y las demas que reciben el `Inventory*` deberian ser miembros (pendiente).
- `Inventory::ExpEntry_unk`: el bloque 0x04-0x80 es `LevelUpResult_unk` (movida de `CharaStatus.cpp` a
  `CharaStatus.h` y completada a 0x7C; `CharaStatus.o` identico) y `mExpEntries` tiene capacidad 8 (el
  original compara con 8; el relleno `m6410` era el octavo elemento). `game/BattleResult.cpp` cambia su
  `FixedArray_unk<..., 7>*` a 8 (misma salida). Nuevos `m600C`/`m6010` en `Inventory`, `m298`/`mD4`/`m284` en la
  vista `InventorySettings_unk`.
- Sin constructores en `ExpEntry_unk`/`LevelUpResult_unk`: comprobado que un constructor inline nuevo en
  `Inventory.h`/`CharaStatus.h` renumera `@N` en las unidades que las incluyen (GameUI, BattleResult);
  `fn_80449ABC` los sustituye por `memset` (equivalente, NONMATCHING).
- `lbl_80754730` (tabla de multiplicadores por progreso) definida en el fuente con su nombre automatico.

## Hallazgos

- Clamps con la expresion repetida (recalculo tras el salto) = macros `MIN`/`MAX` (`a < b ? a : b`), no inline;
  el literal va a la izquierda (`1.0f > x ? 1.0f : x`) para que salga `fcmpo f_lit, f_x`.
- `fmuls` con los operandos al reves en `n * 3 * (8 + 8 * r)`: el factor aleatorio sale de una inline
  (`base + base * frac`).
- En `fn_80016C10` el original recarga `m98.mData` entre la prueba de `m2C` y el puntero al punto: la prueba
  lee `mData[i - 1]` directamente y el punto se toma con `operator[]`.

## Pendientes

- `fn_80449ABC`: con constructores (`LevelUpResult_unk() { memset }`, `ExpEntry_unk() : mChara(0), ... {}`) la
  copia sale miembro a miembro como en el original; exige re-emparejar las unidades que incluyen las cabeceras.

- `SetColorMatrix__Q23atn12RenderConfig...` (GameTask): exige la clase `atn::RenderConfig` (hoy
  `RenderSetting_unk`, cabecera de muchas unidades).
- `vf5C__13GimmickCommonFv` y `__dt__SimpleArray<i>` (AiTarget) salen solos al escribir `fn_80017128`.
- `fn_80018C80`/`fn_80018D5C`: `RouteSearch_unk` de `game/Em006DarkMaster.cpp` es `AIManager` (`lbl_8087EE68`);
  unificar exige tocar esa unidad.
