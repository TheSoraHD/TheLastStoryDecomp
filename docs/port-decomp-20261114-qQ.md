# port-decomp-20261114, cola Q: unidades que pasan a enlazar

Agente Q, rama `agent-Q`. Lista: `near.txt` (unidades fuera de la ruta con una sola funcion por debajo del 100 %).

## Resultado por unidad

| Unidad | Funcion | Antes | Despues | Firma final |
|---|---|---|---|---|
| `db/RandomItemTable_unk` | dtor de la ranura 0x8021D58C | 96,30 % (16/17) | **100 %, enlaza** (17/17) | `RandomSlot_unk::~RandomSlot_unk()` (implicito) |
| `game/PlayerInput_unk` | `Input_unk::fn_801231D0` | 99,46 % (30/31) | **100 %, enlaza** (31/31) | `int Input_unk::fn_801231D0(int action)` (sin cambio) |
| `game/CopyRobotTask` | `CopyRobotTask::vf10` (0x801CC4F4) | 99,84 % (7/8) | **100 %, enlaza** (8/8) | `void CopyRobotTask::vf10()` (sin cambio); `vf3C(HitInfo_unk*)` fuera de la clase |

## Detalle

- `db/RandomItemTable_unk`: la ranura de cinco por tabla pasa de `typedef atn::FixedArray_unk<RandomItem_unk, 8>`
  a `struct RandomSlot_unk : public atn::FixedArray_unk<RandomItem_unk, 8> {}`. El destructor de la derivada
  inlinea el de `FixedArray_unk` (base en el offset 0) y da el segundo `beq` sobre el mismo `cr0`. MWCC emite
  ademas la copia debil `__dt__Q23atn34FixedArray_unk<14RandomItem_unk,8>Fv` sin referencias: **el enlazador
  la quita (deadstrip)**, `main.dol: OK`. Los destructores de `RandomTable_unk`/`WindTable_unk` pasan a ser
  implicitos (sin `~X() {}`): asi las copias debiles salen en el orden del original (ctor de WindTable antes del
  dtor de RandomTable). Renombrados en `symbols.txt`: `__ct__/__dt__Q23atn34FixedArray_unk<14RandomItem_unk,8>Fv`
  -> `__ct__/__dt__14RandomSlot_unkFv`, y tres `@N` de datos (pd.sh).
  - Probado y descartado: poner el almacenamiento de `atn::FixedArray_unk` en una base con destructor propio
    (`include/atn/FixedArray.h`): arregla esta pero baja 12 funciones de otras unidades (MarketTask,
    NewCampPartsStatus, TacView, PopupTask, LogMessageTask, ...). Revertido.

- `game/PlayerInput_unk` fn_801231D0: casos 39/40 con `if ((unsigned int)(skip - 1) > 1) return 0;` (el `return 0`
  queda delante de la llamada como en el original). Con eso el codigo es identico, pero las etiquetas `@N` de los
  floats salian +3: el caso 18 pasa a `result = a && b;` en sus dos ramas (-4) y el caso 13 a dos `if` anidados (+1).
  Despues `pd.sh` renombra 6 `@N` de datos. El aviso de `poolcheck` (pi, 2pi, -pi) es la copia debil de
  `atn::WrapAngle_unk` que emite nuestro .o: el enlazador la descarta con sus constantes y `main.dol: OK`.
  Se quita tambien un `NONMATCHING` viejo de `fn_801248DC` (ya al 100 %).
- `game/CopyRobotTask` vf10: `mRot = dir.ToRotationZ_unk();` (z redondeado una vez, `fmr f2, f7`) en vez de
  `ToRotation_unk()`. Para el orden del .dol, `vf3C(HitInfo_unk*) {}` pasa de inline en `game/CopyRobotTask.h` a
  definicion en el .cpp entre vf14 y fn_801CCD00 (la copia debil salia detras). Eso renumera `@N` en
  LastCocoonChrysalis, SpecialEnemyBase y em005_doppel_ganger (`pdbatch.sh`; em005 8 -> 9 al 100 %). Los avisos de
  `poolcheck` de estas unidades (0.7, 15, 2.44, 25, 20...) son tambien de copias debiles y no impiden enlazar.

## Sin cambio (intentos, revertidos)

- `atn/LightUtil_unk` fn_8007A154 (98,73 %): ~14 variantes (size() en la comprobacion da la recarga de mSize pero
  cambia los registros: 95,5 %; inline de erase, end(), if anidado, orden de dst/end, contador local).
- `game/SystemErrorMessage_unk` fn_803646C4 (99,86 %): ~10 variantes del bucle de DrawFontMessage_unk (y antes que
  i sube a 99,87 %, no se guarda).
- `db/MapEnvSeTable_unk`, `db/MemberList_unk`, `chr/ControlShoot` ctor, `atn/EfpHandle_unk` (16 bits con helpers),
  `game/GameSaveLoadCommon` (cuerpo del inline en el bucle, otro nivel de inline), `atn/LH_*` `Get_unk` (formas de
  `Find_unk` y del `core` inicial): sin cambio.
- `Event/EventCommandDef_unk` fn_800081C0 (94 %): solo el orden `li r5,0`/`addi r4`; 9 variantes mas.
- `game/GameSaveBanner` fn_803B9638 (99,18 %): inline `InitBanner_unk` (memset + NANDInitBanner) y locales
  title/comment: sin cambio.

## Cambios de firma o de layout (para el port)

- `game/CopyRobotTask.h`: `virtual void vf3C(HitInfo_unk*)` declarado (definido vacio en `CopyRobotTask.cpp`).

- `db/RandomItemTable_unk.cpp`: `RandomSlot_unk` es ahora una struct derivada de `atn::FixedArray_unk<RandomItem_unk, 8>`
  (mismo layout, 0xC4 bytes); `RandomTable_unk`/`WindTable_unk` sin destructor declarado.

## Hallazgos

- Una copia debil sin referencias que nuestro .o emite de mas (p. ej. el destructor interior inlineado) no impide
  enlazar: el enlazador la elimina (deadstrip). Lo que importa es el orden del resto de copias debiles.
- objdiff compara las etiquetas `@N` de `.sdata2` por nombre: con el mismo codigo una funcion puede quedarse en
  99,9x % si el fuente gasta otro numero de etiquetas internas. `if (a && b) r = 1;` gasta ~2 mas que
  `r = a && b;`, un `if` anidado 1 mas que `&&`; el efecto es aditivo. Despues `pd.sh` renombra los `@N` de datos.
- Los avisos de `poolcheck` por constantes de una copia debil (WrapAngle_unk, ToRotation...) no impiden enlazar: el
  enlazador descarta la copia repetida y sus constantes.
- "frsp de z otra vez" en un ToRotation inlineado: probar `ToRotationZ_unk()` (CopyRobotTask vf10).

## Segunda lista (`near2.txt`, dos funciones por debajo del 100 %)

Rama al dia con `port-decomp-20261114` (fb6791f) y referencia nueva en `$REF` (15421). Ninguna unidad nueva
enlaza; nada se commitea en codigo.

- `atn/LipSync_unk` fn_8007A7BC (99,70 %): 5 variantes del clamp (sin `cur`, limite en local, objetivo en local,
  delta reasignado, suma invertida); los FPR de `cur`/`m2C` siguen cruzados.
- `chr/ControlDropDown` ctor (98,04 %): con `mChara->m574.Assign_unk(v);` (copia con z primero) sale el `frsp` y el
  z guardado del original y sube a **99,37 %**; queda mChara en r4 por r6 y las direcciones de los temporales
  (r5/r6 por r4/r5). 8 variantes mas sin cambio (local para mChara, `chara->` en lugar de `mChara->`, Assign_unk de
  mStart, `v` asignado aparte). No se guarda: vf0C (98,2 %, huecos de pila) bloquea la unidad igualmente.
- `game/Em205Ogre` fn_802B98B0 (98,61 %): `asgwrap.py` llega a 99,02 % (4 asignaciones con VecAssign_unk), no al 100.
- `game/CostumeEditChara` vf10 (99,99 %): una de sus dos diferencias es de nombre: `lbl_8087F580` es
  `MenuCommonTask::sInstance` (vista local `CostumeEditCursor_unk` con `mOffset` 0x1EC / `mSize` 0x1F4 =
  `m1EC..m1F8` de `MenuCommonTask`). Pendiente (regla 10): usar `MenuCommonTask::sInstance`; requiere dar a
  `MenuCommonTask` los dos `atn::Vector2_unk` (cabecera compartida, ~20 unidades) o leer los floats sueltos. No
  tocado porque vf14 (98,90 %) sigue bloqueando la unidad.
- Sin probar mas (bloqueo anotado en `docs/mwcc-blockers.md` o en notas previas): TextureCacheManager8x32 (par
  contador/desplazamiento), MapIdTable_unk, PaletteTable_unk, PostFxDamage (comentarios con ~8-12 variantes),
  EnemySeaLizerd, Brazier, BoneCalc_unk, LH_Model, D3Anime, ArenaTask, Em001Necromancer, OnOff, Em312DarkServant,
  NewCampPartsTop (solo registros), CharaParam_unk (cambio de `std/string.h` que baja D2A_Message).

## Hallazgos (segunda lista)

- Copia de vuelta de un Vector3 temporal con `frsp` de z en el original: probar `Assign_unk` (z primero).
