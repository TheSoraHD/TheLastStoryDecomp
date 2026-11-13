# Lote paralelo-20261113, cola I (game/ItemManager_unk, game/AiTarget, game/GameTask)

## Resultado por unidad

- `game/ItemManager_unk`: funciones al 0 % 19 -> 1 (solo queda el constructor). Al 100 %: `fn_80444EFC` (dinero que suelta un enemigo),
  `fn_804491F4` (recompensas de un enemigo), `fn_80449448` (escala de recompensas por diferencia de nivel),
  `Inventory::AddItem` (0x804439FC). `fn_80445130` (sorteo de los codigos aleatorios 0x2711-0x271A) escrita,
  NONMATCHING 98,5 % (un `cmpwi` del resultado no usado de `fn_80213B78`). `fn_80445F44` (candidatos de cada
  codigo aleatorio) escrita, NONMATCHING 94,4 %; sus dos `std::sort` (`MslSort_unk` con
  `EquipCandidateLess_unk`/`WeaponCandidateLess_unk`) salen con el tamano exacto: medianas 0x80447740 y
  0x80448908 al 100 %, sorts 99,27 % (registros de la seleccion de `MslSort_unk.h`, cabecera compartida).
  `fn_80444C50` vuelve a 100 % tras el re-emparejado de datos. Escritas NONMATCHING: `fn_80444020` 88,7 %
  (constantes 0/-1/1/0xF0000 en r19-r23 en el original), `fn_80448B80` 99,85 % (operandos de cuatro `cmpw`
  con `atoi`), `Inventory::fn_80449ABC` 55 % (sin constructores, ver decisiones) y su `std::sort`
  (`MslSort_unk`/`MslMedian3_unk` con `ExpOrderLess_unk`, 0x8044A1D8/0x8044B44C/0x8044C6C0, 52-62 %: el
  original es mas grande, mas inlining).
- `game/AiTarget`: funciones al 0 % 11 -> 8. `fn_80016C10` escrita (NONMATCHING 98,76 %, registros:
  el original pone la escena en r30 y el indice*4 en r29, al reves que el nuestro). `AIManager::fn_80018D5C`
  (punto de ruta en un cono) 99,5 % y `fn_80018C80` 92,7 % escritas.

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

- `AddItem`: el original toma la direccion de los propios parametros (`fn_80445130(this, &id, &num, 0)`), no de
  copias locales; el cuerpo de `fn_80448F9C` va inlineado a mano (MWCC no lo inlinea solo).
- Bucles de `FixedArray_unk` con induccion de puntero: `list.mData[i]`, no `operator[](int)` (este da `mulli`).

- Clamps con la expresion repetida (recalculo tras el salto) = macros `MIN`/`MAX` (`a < b ? a : b`), no inline;
  el literal va a la izquierda (`1.0f > x ? 1.0f : x`) para que salga `fcmpo f_lit, f_x`.
- `fmuls` con los operandos al reves en `n * 3 * (8 + 8 * r)`: el factor aleatorio sale de una inline
  (`base + base * frac`).
- En `fn_80016C10` el original recarga `m98.mData` entre la prueba de `m2C` y el puntero al punto: la prueba
  lee `mData[i - 1]` directamente y el punto se toma con `operator[]`.

- `RouteSearch_unk` de `game/Em006DarkMaster.cpp` era `AIManager`: la unidad incluye `AiTarget.h` y llama a
  `AIManager::fn_80018D5C` (renombrado en `symbols.txt`; `main.dol: OK`). Declaracion nueva en `AiTarget.h`
  sin nombres de parametro. `fn_80018C80` se queda `extern "C"` con el `AIManager*` delante porque
  `AI/AiCommon_unk` la enlaza con nombre C (y le pasa registros sin inicializar; firma por revisar).

## Pendientes

- `fn_8001794C`/`fn_80017128` (AiTarget): necesitan `AiInput_unk`/`AiOutput_unk` (hoy locales en
  `AI/AI_archer_attack01.cpp`, con campos sueltos; el original usa arrays) en una cabecera comun, y
  `CharaRes_unk::m58-m84` se leen como float (la cabecera los tiene como pares de int).
- Constructor de `Inventory`: exige constructores en `Item_unk`/`ExpEntry_unk`/`Equip_unk` (renumera `@N`).

- `Inventory::AddItem` devuelve `int` (antes `void` en `Inventory.h`; los llamadores ignoran el valor).
- `DropSetting_unk` (ItemManager_unk) es una vista de `InitSetting_unk` de `db/InitSetting_unk.cpp`: falta una
  cabecera comun.
- Queda al 0 % en ItemManager_unk: el constructor de `Inventory` (0x80442FF4).
- `Inventory::mDB10` pasa a `int` (permite los objetos de estado en `fn_80445F44`).
- GameTask `fn_803731E8`: `FinishCamera_unk::fn_803786FC` recibe en realidad un entero (`leader != mChara ? 30
  : 0`), no un `CharaTask*`; corregir la firma exige tocar `game/FinishCamera_unk`.

- `fn_80449ABC`: con constructores (`LevelUpResult_unk() { memset }`, `ExpEntry_unk() : mChara(0), ... {}`) la
  copia sale miembro a miembro como en el original; exige re-emparejar las unidades que incluyen las cabeceras.

- `SetColorMatrix__Q23atn12RenderConfig...` (GameTask): exige la clase `atn::RenderConfig` (hoy
  `RenderSetting_unk`, cabecera de muchas unidades).
- `vf5C__13GimmickCommonFv` y `__dt__SimpleArray<i>` (AiTarget) salen solos al escribir `fn_80017128`.
- `fn_80018C80`/`fn_80018D5C`: `RouteSearch_unk` de `game/Em006DarkMaster.cpp` es `AIManager` (`lbl_8087EE68`);
  unificar exige tocar esa unidad.
