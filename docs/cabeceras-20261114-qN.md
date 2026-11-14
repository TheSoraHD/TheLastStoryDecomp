# cabeceras-20261114, cola N: unidades cerca de enlazar

Agente N, rama `agent-N`. Lista generada de `build/SLSP01/report.json` (unidades fuera de `ROUTE` con 1-2 funciones
por debajo del 100 %, el resto al 100 %): 147 unidades. Referencia `$REF` = `/home/claude/ref-N` (15433).

## Resultado por unidad

| Unidad | Funcion | Antes | Despues | Cambio |
|---|---|---|---|---|
| `RVL_SDK/.../hbm/HBMBase` (libreria) | `homebutton::HomeButton::draw` | 98,80 % (33/34) | **100 %, enlaza** (34/34) | `static const GXColor fogClr` |

## Detalle

- `HBMBase` draw: el color de la niebla es `static const GXColor fogClr = {0, 0, 0, 255};` (va a `.rodata` en el
  mismo sitio del pool, 0x330); como local no static la carga se programaba tras los `fmr`. `"matching": true` en
  `libs_ref.json` y `main.dol: OK`. Renumera 7 `@N` de datos de la propia unidad (pd.sh, solo renombrados).

## Sin cambio (intentos, revertidos)

- `chr/ControlDetect` HateAtt vf0C (99,84 %): asgwrap (VecAssign_unk y --template), `-=`, temporal con nombre,
  inverso en local, `chara` local, velocidad aparte, `Assign_unk`, referencia a mPos: r3/r4 del __VecCopy siguen
  cruzados.
- `atn/D2A_Frame` vf18 (99,85 %): copia de oc antes del test, ToU32, ctor de 4 floats, operator=, alpha en local.
- `gmk/Brazier` fn_80407D24 (99,83 %, mRadius en f3): radio antes del `+=`, local, `y = y + r`, Assign_unk,
  `m6C = m6C + ...`, velocidad aparte, orden de res/sphere. La unidad tiene ademas vf40 (r27/r28) y el orden
  de 0.5f/2.5f en `.sdata2` de vf10/vf14.
- `game/Em201ZanGurg` ctor (99,77 %): una sola declaracion, copy-init, `const`, literal en puntero: sin cambio.
  Es el bloqueo del segundo `std::string` comun a ~10 ctores de enemigos (Em003, Em101*, Em203, Em301,
  Em308, Em201ZanGurgFinal...); `Em316LastLaser` tiene el mismo par y casa, asi que depende del contexto
  (despues viene `atn::Csv` en vez del tokenizer), no de la forma del ctor de `std/string.h`.
- `chr/ControlTurnQuadruped` ctor (99,92 %): el original guarda el double sin convertir (`stfs f1`) y hace
  `frsp` aparte para la comparacion; probado `mAngle < 0`, `(double)` / `0.0`, cast, local float, inline
  `IsNeg_unk(float)` (da `mfcr`), ternario, comparacion antes, `mAngle(angle)` en la lista (queda en f3 y
  cambia el orden), puntero al miembro.
- `game/CrossbowMenuTask` fn_801E9544 (99,82 %): Inventory directo, clave en local, referencia, indice
  unsigned (99,46), aritmetica de punteros, declaraciones de `inventory`/`i` en varios sitios.
- `chr/ControlDetect`, `gmk/OnOff` fn_803F8324 (copia de mBoxPos/mMtx: `=`, operator=, Assign_unk, ctor de
  Matrix3x4, orden), `MP/D2AMpMenuSetRule` fn_804CECEC (SetTaskText_unk, texto antes de los arrays, layout
  local, ternario a mano), `db/ChaseParam_unk` (char*, sin `len`), `game/GameSaveLoadCommon` (cuerpo del
  inline escrito en el bucle en 3 formas: cambia la pila y sigue el `bne`), `game/Em205Ogre` vf108 (asgwrap:
  99,688 -> 99,71, no se guarda): sin cambio.
- `dwc/src/base/dwc_account` DWC_ReportFriendData (96,46 %): la prueba de "buddy y flag 8" del caso
  GS_PROFILE_ID es un inline que devuelve BOOL (`if (... && ...) return TRUE; return FALSE;`, da el
  `li r0,1 / li r0,0 / cmpwi` del original): **99,79 %**, pero entonces la funcion se inlinea en
  DWC_ReportUserData (que baja del 100 %), asi que no se guarda. Queda el orden de comparaciones del switch
  (original 2,3,1 con cuerpos 3,2,1; MWCC compara y coloca siempre en el orden del fuente, probadas las 24
  permutaciones con default; `case 2: case 3:` compartido da una comprobacion de rango) y los buffers del
  nombre de usuario intercambiados.
- `game/CostumeEditChara` vf14 (98,90 %): accesores libres (puntero, referencia, sub-objeto), `*(p + i)`,
  `(&a[0])[i]`, indice int: el `add + lwz 0x680` no pasa a `addi + lwzx`.
- `atn/EffectSequencer_unk` fn_802440F0 (95,55 %): elem dentro del bucle, `mElems + i`, prueba por indice,
  indice int. `gmk/FallingTower` fn_8041044C: orden de mAmp/sin, sin en local, forma del lerp.
- Sin probar mas por notas previas exhaustivas: OS (OSInit, solo relocs de la 0x80004000), dwc_transport,
  dwc_ranking, LH_* `Get_unk` (plantilla de `atn/LoadManager.h`, cabecera compartida), Texture_unk,
  MapIdTable_unk, SelectMember, MotionData, Event/Label, HealPoint, Em002_02SkeltonKing ctor (carga de
  `info->m20` antes del prologo), LastCocoonChrysalis/GimmickManager (presupuesto de inline).

## Decisiones tomadas sin el usuario

- `HBMBase.cpp` pasa a `"matching": true` en `libs_ref.json` (enlaza, `main.dol: OK`).

## Para el port

- Nada.

## Pendientes (cabeceras compartidas, fuera de esta cola)

- `game/CostumeEditChara` vf10: `lbl_8087F580` = `MenuCommonTask::sInstance` (nota de la cola Q).
- `db/ArrowSet_unk`, `Event/Section`: `push_back` de `std/vector.h`.

## Hallazgos

- Un agregado local inicializado desde el pool que se programa tarde: probar `static const` (HomeButton::draw).
- Un `li r0,1 / li r0,0 / cmpwi r0,0` antes de un `if` es un inline que devuelve BOOL con `return TRUE/FALSE`
  (DWC_ReportFriendData).
- MWCC emite la cadena de comparaciones de un switch pequeno y los cuerpos en el orden del fuente; dos
  etiquetas consecutivas con el mismo cuerpo se convierten en una comprobacion de rango.
