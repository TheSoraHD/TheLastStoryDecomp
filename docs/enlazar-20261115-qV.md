# enlazar-20261115, cola V: vistas locales de `D2A_TaskInstance32_unk` (regla 10)

Inicio 20:17 CET, cierre ~22:00.

## Resultado por unidad

Objetivo: quitar las vistas parciales de la instancia d2anime (`atn::D2A_TaskInstanceBase<32, 8>`) y los casts
`(D2A_TaskInstance32_unk*)` de los llamadores. **360 -> 22** lineas con cast (`grep -rc` en `src include`); quedan
`SelectMember` (4) e `ItemSlotAnime` (18), ver Pendientes. Ninguna funcion cambia de porcentaje: comprobado con
`objdiff-cli` por funcion (instantanea antes/despues de todas las unidades que incluyen cada cabecera tocada,
`@N` aparte) y con `precommit.sh` (lost 0, fuzzy drops 0, labelcheck/rep100 igual) en cada commit. 15623 / 18109
antes y despues; ninguna unidad nueva enlaza (cola de limpieza).

Por familia:
- `ShopLayout_unk` (`game/ShopBase.h`) -> `typedef D2A_TaskInstance32_unk` (`mRepeat` -> `mLoop`): 122 casts fuera en
  ShopBase/ShopBuySell/ShopColor/ShopIdentify/ShopUpGrade. Renombrados en `symbols.txt` (nuevo mangling):
  `fn_805846C0__8ShopBase...` y `fn_80598530__11ShopUpGrade...` (`PQ23atn26D2A_TaskInstanceBase<32,8>`).
  Re-emparejado (`pdbatch`) de ShopBase, ShopBuySell, ShopColor, ShopIdentify, ShopUpGrade, CosEditProtector,
  GameTask (la cabecera incluye ahora `atn/D2A_TaskInstanceC.h`).
- `MenuLayout_unk` (`game/MenuCommonTask.h`) -> typedef con declaracion adelantada de la plantilla (sin incluir la
  cabecera: no renumera `@N` en los 24 incluidores); MenuCommonTask y LibraryTask sin casts (`GetCursor_unk(..)->
  GetTask()` ya devuelve la instancia).
- Casts redundantes (el puntero ya era la instancia real, p. ej. `D3AnimeInstance::GetTask()`): CosEditBase,
  CosEditItemList_unk, CosEditPalette_unk, GameChoiceWindow, NewCampParts{Hint,Item,Option,Status,Top,MapSelect},
  TacView, GameUI (`(GameUIInst_unk*)`, 41), NewCampPartsBase y CosEditPalette (`(atn::D2A_TaskInstanceBase<32, 8>*)`).
- `CosEditBase::mE8/mEC`: no son instancias d2anime sino `&m54[11]`/`&m54[12]` de CosEditProtector ->
  `NewCampPartsMotion_unk*` (el cast a `CostumeEditFace_unk` era otra vista con el mismo offset 0x50).
- `ResultLayout_unk` (`game/BattleResult.h`) -> typedef (se va su `IsEnd_unk` fuera de clase: la clase real tiene uno
  igual). Las 7 bajadas de GameEventScript de la cola R eran etiquetas de `.sdata2` que `pd` no re-empareja por tener
  valores repetidos (0.0 y 1.0/0.5): renombradas a mano por direccion (`@47733 -> @47729` en 0x80885B10,
  `@50372/@50373 -> @50368/@50369` en 0x80885B30/34); GameTask igual (`@27962/@27966 -> @27958/@27962` en
  0x8088571C/2C, `@31571/@31572 -> @31567/@31568` en 0x808857E0/E4). Re-emparejado de BattleCharaTask, BattleResult,
  GameEventScript, GameTask; objdiff identico al de antes en las cuatro.
- `NewCampOptionInst_unk` (NewCampPartsOption, vista local) y `DropLayout_unk` (`gmk/DropItem.h`, `m50/m54` ->
  `mFrame/mSpeed`) -> typedef; `CampSaveLoad::mF4[8]`, los locales `task` (NewCampPartsHint/Item) y `bar`
  (CosEditProtector) con el tipo real.
- `CosEditLayout_unk` (vista local de CosEditProtector, `m4E`) fuera: `CosEditProtector::mB88` pasa a
  `atn::D2A_TaskInstanceBase<32, 8>*` (`mVisible = 0`).

Commits: `5acd528` (shops), `646bb4d` (menus), `640458e` (casts redundantes), `0efce90` (mE8/mEC), `e85a653`
(GameUI), `9d5d298` (ResultLayout), `3bb0098` (NewCampParts/CampSaveLoad/DropItem), y el de mB88 + estas notas.

## Unidades que enlazan (bytes)

Ninguna nueva (cola de limpieza de la regla 10).

## NONMATCHING con motivo

Ninguna nueva.

## Cambios de splits

Ninguno. `symbols.txt`: dos renombrados de funciones por el mangling nuevo (arriba), re-emparejados de `@N`
(`pd.sh`/`pdbatch.sh`) y los renombrados a mano de etiquetas de `.sdata2` de GameEventScript y GameTask (arriba).

## Para el port

- `include/game/BattleResult.h`: `struct ResultLayout_unk : atn::Task {m4D bool mRepeat, m50 mFrame, m54 mSpeed,
  IsEnd_unk()}` -> `typedef D2A_TaskInstance32_unk ResultLayout_unk` (`mRepeat` -> `mLoop`, `unsigned char`).
  Afecta a todo `game/BattleResult.cpp` (Create, vf10/vf14, fn_800F0*); sin cambios de firma.
- GameUI y TacView: solo se quitan casts redundantes (sin cambios de tipo, firma ni layout).
- `include/game/ShopBase.h`, `MenuCommonTask.h`, `CampSaveLoad.h`, `CosEditBase.h`, `CosEditProtector.h`,
  `gmk/DropItem.h`: no son unidades del port, pero los tipos de miembros cambian (ver arriba) por si el port los usa.

## Decisiones tomadas sin el usuario

- Typedef con declaracion adelantada de la plantilla (`namespace atn { template <int N, int M> class
  D2A_TaskInstanceBase; }`) en las cabeceras muy incluidas (MenuCommonTask.h, DropItem.h, CosEditProtector.h) en vez
  de incluir `atn/D2A_TaskInstanceC.h`: sus declaraciones con nombres de parametro renumerarian `@N` en todos los
  incluidores. En ShopBase.h y CampSaveLoad.h (pocos incluidores) se incluye la cabecera completa.
- Renombrados a mano por direccion de etiquetas de `.sdata2` que `pd` dejaba sin emparejar.
- `ItemSlotAnime` no se commitea: baja `fn_8044F96C` (NONMATCHING) 99,274 -> 99,255 %; parche en
  `docs/enlazar-20261115-qV-itemslot.patch`.

## Hallazgos (una linea cada uno)

- `pd.sh`/`pdbatch.sh` no re-emparejan etiquetas de `.sdata2` con valores repetidos en la unidad (0.0, 1.0): tras
  renumerar, alinear por offset los `.sdata2` de `build/SLSP01/src/X.o` y `build/SLSP01/obj/X.o` y renombrar a mano.
- Un typedef a la plantilla con solo una declaracion adelantada (`template <int N, int M> class X;`) no consume `@N`
  en los incluidores (incluir la cabecera completa si, por los nombres de parametro de sus declaraciones).
- `objdiff-cli diff -u <unidad> -o - --format json` sin simbolo da todos los simbolos con `match_percent`: sirve para
  comparar por funcion antes/despues (el `target_symbol` va desplazado en uno: emparejar por nombre).

## Pendientes

- `SelectMemberAnim_unk` (SelectMember): `mAnims[9]` mezcla `atn::D2A_Task` (0 y 8, de `fn_801F3FF8`) e instancias
  (1-7), y `vf14` recorre las 9 en un bucle: lo natural es `atn::Task* mAnims[9]` con dos accesores; pero
  SelectMember.cpp no puede incluir `atn/D2A_Task.h` (choca su `fn_801F4E8C` local, que devuelve
  `SelectMemberPoint_unk` con constructor vacio; `D2A_DrawRect` pone ceros y cambiaria el codigo) y tiene una vista
  parcial local de `atn::D2A_Task`.
- `ItemSlotTask_unk` (ItemSlotAnime, 18 casts): typedef en `gmk/ItemSlotAnime.h` probado (parche
  `docs/enlazar-20261115-qV-itemslot.patch`, objdiff identico en los otros 12 incluidores), pero baja
  `fn_8044F96C` 99,274 -> 99,255 % (cruce de registros del iterador) aunque se dejen los casts en esa funcion; hay
  que renombrar ademas `@STRING@ClearText_unk__Q23gmk13ItemSlotAnimeFP16ItemSlotTask_unkPCc`.
- `GameUIInst_unk` y `CostumeEditFace_unk` siguen como alias (typedef) de la instancia; podrian sustituirse por
  `D2A_TaskInstance32_unk`.
