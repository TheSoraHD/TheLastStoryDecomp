# cabeceras-20261114, cola C: `game/CharaStatus.h` y vecinas

Rama `agent-C`. Inicio 14:00, fin ~15:05. **15424 -> 15426 / 18109** (contra `$REF`), lost 0, sin bajadas,
labelcheck/rep100 igual, `main.dol: OK` en cada commit (`precommit.sh` OK).

## Resultado por punto

1. **`CharaStatus::m2CC` + `m2D0[4]` -> `FixedArray_unk<CharaStatusPair_unk, 4> m2CC`** (`game/CharaStatus.h`,
   el `FixedArray_unk` global de esa cabecera). Instanciar la plantilla renumera los `@N` solo de las unidades
   que la usan (12): re-emparejadas con `pdbatch.sh` + `lblmap.py` (EnemyQuarkFirst, EnemyQuarkSecond, GameUI).
   - `GameTask::fn_80367A40` 98,4 -> **100 %** (con el punto 2), `fn_8036F268` 98,7 -> 99,8 %, `fn_80370320`
     96,8 -> 98,1 %: la copia de `CharaStatus` copia el `FixedArray` en bloque, como el original.
   - `BattleCharaTask::fn_800E9110` sigue en 98,8 %: el original calcula `&mStatus.m2CC` (0xAA0) en r4 antes de
     leer `m145C`; `operator[]`, `mData[i]`, `begin() + i` y una referencia local lo pliegan en el offset.
   - `CharaStatus` ctor sigue en 98,1 % (solo registros, igual que antes).
   - `CharaC58_unk::fn_8011C4AC` bajaba a 93,8 %: la primera lectura por `m2CC.mData[3]` (offset directo) la
     devuelve al 100 %. `fn_8012D180`: el bucle de borrado por `m2CC.mData[i]` (97,5 %, como antes).
2. **Struct anonimo pendiente** (`docs/paralelo-20261113-qI2.md`): `CharaStatusData_unk::m54..m7C` es ahora un
   struct con nombre `CharaStatusTail_unk` (`m00`, `m04`, `m08[0x20]`): se copia en pares de 8 bytes como el
   original y, al tener nombre, no consume `@N`. Usuarios: `m1A8.m54` -> `m1A8.m54.m00`, `m1A8.m58` -> `m1A8.m54.m04`.
3. **`MotionCommand_unk::m10` como `bool`** (`game/CharaTask.h`; no renumera): `Actor::fn_8000FAE8` 93,0 -> 93,2 %
   (ya no sale la mascara de 8 bits). `SetBool_unk` de Puppeteer_CharaTask toma `bool&`.
4. **Constructores de `LevelUpResult_unk` y `Inventory::ExpEntry_unk`**: declarados sin cuerpo en
   `CharaStatus.h`/`Inventory.h` (comprobado: una declaracion sin parametros no renumera; el cuerpo inline si) y
   definidos inline en `include/game/InventoryExp_unk.h`, que solo incluyen GameTask e ItemManager_unk
   (re-emparejadas en lote). `MslMedian3_unk<ExpEntry_unk>` **100 %** (antes 51,6), los dos `MslSort_unk<ExpEntry_unk>`
   61,5 -> 99,7 % (registros), `Inventory::fn_80449ABC` 55 -> 92 % (fuzzy del report; objdiff 92,2: registros),
   `GameTask::fn_80371790` 77 -> 97,2 % (copia de la lista por `party->begin()[i]`; queda el marco 0x20 mayor del
   original y la copia de `mDF30`).
5. Extra (deuda de la regla 10): `include/db/PlayerParam_unk.h` con `PlayerParam_unk` (constructor declarado,
   cuerpo inline en `db/PlayerParam_unk.cpp`); fuera las vistas `EquipSet_unk` (CharaStatus), `LevelSkills_unk`
   (GameTask) y `GameUISkillTable_unk` (GameUI). Codigo y simbolos de los cuatro `.o` identicos.

## NONMATCHING con motivo

- `BattleCharaTask::fn_800E9110` 98,8 %: base 0xAA0 en registro (ver punto 1) y huecos de pila de temporales.
- `GameTask::fn_80371790` 97,2 %: marco de pila 0x20 mayor en el original, registros de la copia de `mDF30`,
  setters de `atn::Model` fuera de linea.
- `Inventory::fn_80449ABC` 92 %: registros r26-r30 rotados y lugar de las lecturas de experiencia.
- `MslSort_unk<ExpEntry_unk>` (x2) 99,7 %: registros.

## Decisiones tomadas sin el usuario

- Los puntos 1, 2 y 3 se compilaron y comprobaron juntos (un solo build de ~270 unidades y un `precommit` OK
  sobre el estado conjunto) y se repartieron despues en tres commits; los intermedios no se pasaron por
  `precommit` uno a uno.
- Cuerpos inline nuevos en `game/InventoryExp_unk.h` (cabecera nueva, solo dos includers) en lugar de en
  `CharaStatus.h`/`Inventory.h`, para no renumerar ~270 unidades. Lo mismo para el ctor de `PlayerParam_unk`.
- `symbols.txt`: solo renombrados de `@N` (pdbatch/lblmap) en las 14 unidades re-emparejadas; no cambian
  `lbl_8087D6E8/EC`, `lbl_8087EE78`, `lbl_80885CB0` ni las cadenas 0x80783CA8 / 0x8078CB00/08/10.
- Avisos de `poolcheck` en CharaStatus (15.0), CharaTask, Em201ZanGurgFinal, EnemyQuarkFirst, ItemManager_unk (15.0),
  GameUI: ningun literal float cambiado en esta cola (valores ya avisados antes, sin comprobar uno a uno).

## Para el port

- `CharaStatus` (`game/CharaStatus.h`): `int m2CC; CharaStatusPair_unk m2D0[4];` -> `FixedArray_unk<CharaStatusPair_unk, 4>
  m2CC;` (mismos offsets 0x2CC/0x2D0, mismo tamano). `mStatus.m2D0[i]` -> `mStatus.m2CC[i]`, la cuenta
  `mStatus.m2CC` -> `mStatus.m2CC.mCount`. Unidades del port tocadas: **BattleCharaTask** (fn_800E9110 y los
  usos de `m2D0[m145C]`, `m2D0[3]`, `m2D0[1]`), **GameUI** (dos usos de `m2D0[0]`/`m2D0[3]`), **TacView**
  (`m2CC - 1` y cuatro `m2D0[...]`). Tambien AiCommon_unk, CharaTask (`fn_8013655C`: `PushBack_unk(pair)`),
  CharaC58_unk, ControlUseSkill, Em201ZanGurgFinal, EnemyQuarkFirst/Second, em005_doppel_ganger, CharaStatus.
- `CharaStatusData_unk` (`game/CharaStatus.h`): `unsigned int m54; int m58; unsigned char m5C[0x20];` ->
  `CharaStatusTail_unk m54;` (`m54.m00`, `m54.m04`, `m54.m08[0x20]`; mismos offsets).
- `MotionCommand_unk::m10` (`game/CharaTask.h`): `unsigned char` -> `bool` (mismo offset 0x10).
  Unidad del port **Event/Puppeteer_CharaTask**: `SetBool_unk(bool&, unsigned int)` (antes `unsigned char&`).
- `LevelUpResult_unk()` e `Inventory::ExpEntry_unk()` declarados (cuerpos en `game/InventoryExp_unk.h`); el port
  debe incluir esa cabecera donde construya esas estructuras (GameTask, ItemManager_unk).
- `PlayerParam_unk` en `include/db/PlayerParam_unk.h` (`mPlayer`, `mLevel`, `bool mSkills[0x80]`, `mSlots[6]`);
  **GameUI**: `GameUISkillTable_unk` desaparece (`table->mLearnt[i]` -> `table->mSkills[i]`); `fn_80219160`
  devuelve `PlayerParam_unk*` en CharaStatus, GameTask y GameUI.

## Pendientes

- `LevelUpResult_unk` y `CharaStatusData_unk` tienen la misma forma (0x7C bytes, mismos offsets y la misma copia):
  probablemente el mismo tipo; unificarlos cambia el nombre mangled de `CharaStatus::fn_8013354C`. Con eso,
  `InventoryFinish_unk` podria derivar de `ExpEntry_unk` y quitar los `*(ExpEntry_unk*)&mDF30` de GameTask y GameUI.
- `BattleCharaTask::fn_800E9110`: buscar la inline que deja `&mStatus.m2CC` en registro.

## Hallazgos (una linea)

- Un `struct` con nombre como miembro de un POD se copia en bloque (pares de 8 bytes) dentro de la copia
  miembro a miembro del contenedor; un struct anonimo hace lo mismo pero consume un `@N`.
- Declarar un constructor sin parametros (sin cuerpo) en una clase de cabecera no renumera los `@N`; el cuerpo
  inline si: poner los cuerpos en una cabecera aparte que solo incluyan las unidades que construyen.
- Instanciar una plantilla nueva como miembro de una cabecera compartida solo renumera las unidades que usan ese
  miembro (12 de ~270 con `CharaStatus::m2CC`).
- Copia implicita de una clase con `FixedArray_unk` miembros: cada `FixedArray` se copia como un bloque (pares de
  8 bytes, o bucle `lwzu/stwu` si es grande); con la cuenta y el array como miembros sueltos no sale igual.
