# Cola N (port-decomp-20261114): tercera pasada de combate

Unidades: `game/BattleTask`, `game/BattleCharaTask`, `game/AiTarget`, `chr/ControlBase` (solo el inline de vf2C).

## 1. Vistas de resultado de colision (pendiente de G) - hecho (d380e34)

- `game/BattleTask` (`CollResult_unk` local) y `game/BattleCharaTask` (`RayResult_unk`) pasan a
  `atn::ColliResult_unk` (`include/atn/ColliBase.h`); `*(unsigned int*)result.m34 == 0x1B` (agua).
- `RayObj_unk` (objeto que toca el rayo del cursor) era `atn::ColliBase` (`mFlags` 0x08, `m0C` dueno):
  `BattleTask::fn_801021B4(atn::ColliBase*, int, int)` y `BattleTask::m3E2E8` pasan a `atn::ColliBase*`
  (forward en `game/BattleTask.h`). `RayObj_unk` queda como `typedef atn::ColliBase RayObj_unk;` porque
  `game/GameUI.cpp` (`GetCursorTarget_unk`) lo usa: pendiente cambiarlo alli (unidad ajena).
  Renombrado en `symbols.txt`: `fn_801021B4__10BattleTaskFP10RayObj_unkii` -> `fn_801021B4__10BattleTaskFPQ23atn9ColliBaseii`.
- `HitResult_unk` (resultado de `fn_800F8C6C`: atacante y flags) unificado en `game/BattleTask.h` (struct sin
  inline: no renumera nada).
- `@N` re-emparejados por offset entre el `.o` anterior y el nuevo (`maprelabel` de G): 155 renombrados.
  Sin bajadas (156/212 y 69/88).

## 2. Constructor y destructor de BattleTask - 100 % (b5a2130, e654d79)

- `BattleTask::BattleTask(atn::Task*)` (0x800F1430) 100 %, `BattleTask::~BattleTask()` (0x800F1F94, antes
  `fn_800F1F94` en `symbols.txt`, renombrado a `__dt__10BattleTaskFv`) 100 %, copias debiles
  `__dt__12HomingBulletFv` y `__dt__Q210BattleTask9BetoModelFv` 100 %. BattleTask 156 -> 160/212.
- **Layout de `BattleTask` (para el port)** (`game/BattleTask.h` incluye ahora `atn/ResLoader_unk.h` y
  `atn/Texture_unk.h`; tamano y offsets sin cambios):
  - `mCB7C`, `mLineTex` (0x389C4): `atn::Texture`; `m389F4`, `mHitEff[3][7]`, `mEff3651C[5]`, `mEff36558[5]`,
    `mEff36594[3]`: `atn::EffFile`.
  - Los EfpFile que el constructor construye uno a uno (no `__construct_array`) son miembros sueltos:
    `mEfp363C0[4]` -> `mEfp363C0/363CC/363D8/363E4`; `mEfp365B8[4]` -> `mEfp365B8/365C4/365D0/365DC`;
    `mEfp38AF4[32]` -> `mEfp38AF4`, `mEfp38B00`, `mEfp38B0C[3]`, `mEfp38B30..38B60` (5), `mEfp38B6C[3]`,
    `mEfp38B90[3]`, `mEfp38BB4..38C68` (16). `m38C74[0x1C]` -> `int m38C74; float m38C78..m38C8C`.
  - `BattleMarkers_unk` (0x367B8) solo tiene `mItems[72]` (+ `fn_800F8538`); los efectos de las lineas
    (`BattleLineEff_unk {EffFile mBase, mHead}` x3: `mEnemyLine`, `mAppearLine`, `mOwnLine`), `mHideLines` y
    `mLineTex` son miembros de `BattleTask` (el destructor los destruye sin la comprobacion de nulo de un
    subobjeto). Fuera `GetEfp_unk`/`GetTex_unk`.
  - `mCBAC` + `mCBB0[8]` -> `BattleTimers_unk mTimers {mCount; BattleTimer_unk mItems[8];}` (el registro base del
    bucle del constructor solo cuadra con el contenedor). `GameTask::fn_80371664` (en `game/GameTask.cpp`):
    `mTimers.mCount = 0` (una linea en unidad ajena).
  - `m391E0[72][72]` -> `BattleHateRow_unk m391E0[72]` (`m[72]`, constructor con `memset`); usos `m391E0[i].m`.
  - Constructores inline nuevos: `BattleTargets_unk` (mSize 0), `BattleMarker_unk` (m70, mMode, mFlags),
    `BattleShot_unk` (mKind 2, resto 0), `BattleShots_unk` (mCount, mHead, mEnabled), `BattleTimer_unk`,
    `BattleTimers_unk`, `BattleSel_unk` (todo 0), `BattleHateRow_unk`, y `BattleCalcParam_unk`
    (`game/BattleCalc_unk.h`: m64..m78 en la lista, `m80 &= 0x0FFFFFFF; m7C = -1; fn_800E2A24(this);`).
    El de `BattleShot_unk` acerca `fn_800FAB80` (96,9 -> 98,5 %) y `fn_800F8C6C` (89,1 -> 89,9 %): sus copias
    locales ya llevaban el constructor en el original.
  - `extern "C" void* memset(void*, int, unsigned long);` declarado en `game/BattleTask.h`.
- `@N` re-emparejados: por offset (maprelabel) en las 59 unidades que incluyen la cabecera (~2600
  renombrados por paso, dos pasos; incluye `game/CharaTask`, `Event/Manager`, `game/GameUI`... solo
  `symbols.txt`), y BattleTask con `pairdata99.py` + un emparejado por valor (`valrelabel`: constante unica
  en los dos `.o`) para las de funciones por debajo del 99 %.

## 3. `chr::ControlBase::vf2C` - 100 %

- `virtual int vf2C() { return 1; }` inline en `chr/ControlBase.h` (firma sin cambios). La copia debil queda en
  BattleTask (primera en el orden de enlace; las demas unidades con vtables de controles la emiten y el
  enlazador las descarta).
- Renumera `@N` de 75 unidades (las que incluyen `chr/ControlBase.h`): maprelabel por offset (~2550) y los
  nombres con `@enum$N` (plantillas de boost::bind de `chr/ControlAppear`) re-emparejados por offset entre los
  `.o` (`enumrelabel`, 6 nombres). Hallazgo: un inline virtual sin constantes tambien consume numeros `@N`.

## 4. NONMATCHING de BattleTask / BattleCharaTask / AiTarget

- `BattleTask::fn_800F72F4(BattleCharaTask*, int)` (linea de objetivo): fuera el `DRAFT`. Arreglo de
  equivalencia: el color de cada vertice del bucle va en `verts[i + 1]` (el borrador lo escribia en `verts[i]`,
  dejando el ultimo sin color). 94,7 -> 98,2 % (con los miembros de las lineas en BattleTask). Resto: registros,
  los `Color` de `fn_800F8014` pasan a `operator=` desde r3 en el original, y la salida temprana de
  `IsPartyMember_unk` salta al reves (separar el `||` en dos `if` baja).
- **Valores distintos que objdiff no ve** (solo cuenta la etiqueta): script `valdiff` (carga `lf*` de la misma
  instruccion con valor distinto entre el `.o` original y el nuestro). Arreglos de equivalencia:
  - `BattleCharaTask::fn_800EBD00(int, int)`: `mMotion.fn_80097C08(0, 2, 1, 0, 1, 1.0f, 0.1f)` (era 0.0f) ->
    **100 %** (el comentario hablaba de offsets del pool).
  - `BattleCharaTask::fn_800E62AC`: tres `fn_800C31F4(sBattleSe_unk[4], 0, -1, 1.0f)` tenian los argumentos
    desordenados (`1.0f, 0, -1`). Sigue 98,7 % (registros).
  - `AiTarget` `fn_800197A4`: `2.0f * (2.0f * enemyDist) < e` (era 3.0f). Sigue 99,1 %.
  - BattleTask: los avisos de `valdiff` en vf10/vf14/fn_800F8C6C son cargas en otro orden (registros), no
    valores ausentes.
- `AIManager::fn_80018078(CharaTask*)` (AiTarget): **100 %** con `atn::VecAssign_unk(seg.mFrom, ...)` (cruce
  r3/r5, `asgwrap.py`). AiTarget 34 -> 35/44. Firma sin cambios.
- Tras `pdbatch.sh`, etiquetas de funciones por debajo del 99 % sin re-emparejar: `offrelabel` (misma
  seccion, mismo offset y mismos bytes entre el `.o` original y el nuestro, renombrado en dos fases).
- `BattleCharaTask::fn_800EE360()`: **100 %**. `BattleResult::AddItem_unk(int)` (`game/BattleResult.h`) compara
  con `ItemCount_unk()` (inline nuevo, `return mItemCount;`): la recarga de `mItemCount` para la direccion del
  hueco sale sola. Renumera `@N` de BattleResult, GameTask, GameEventScript y BattleCharaTask (maprelabel, 205).
  `GameEventScript::fn_803A6B9C` (cola ajena, mismo `AddItem_unk`) sube 92,3 -> 93,6 % sin tocarlo (su
  comentario sigue diciendo 92,3 %).
- Cadenas `""` sin referencias en el pool del original (tras "%.2f", "Timer:%d/%d", "Range: %d",
  "Range: Infinity", "HateAtt: Lv%d" de vf14): los formatos acaban en `\0` (`"%.2f\0"`), como los de
  `fn_800F39A4`. `fn_80100BF0` (99,4 %) y `fn_801061BC` (99,95 %) -> **100 %** (solo eran offsets del pool).
  BattleTask 161 -> 163/212.
- Intentado sin exito: `fn_800FDB20` con constructor inline en `BattleDamage_unk` (baja), `asgwrap` en
  `fn_800E932C` (sin mejora), `fn_800F72F4` separando el `||` de la salida temprana (baja).

## Resumen de cifras

- Funciones al 100 % (report.json): 15396 -> 15405 (mas `fn_800EBD00`, que report.json ya daba al 100 % por
  etiqueta). BattleTask 156 -> 163/212, BattleCharaTask 69 -> 71/88, AiTarget 34 -> 35/44. lost 0, sin bajadas.

## Cambios de firma o de layout (para el port)

- `BattleTask::fn_801021B4(atn::ColliBase*, int, int)` (antes `RayObj_unk*`); `BattleTask::m3E2E8` es
  `atn::ColliBase*`; `typedef atn::ColliBase RayObj_unk;` en `game/BattleTask.h`.
- `HitResult_unk` en `game/BattleTask.h`.
- Layout de BattleTask retipado (seccion 2): EffFile/Texture tipados, EfpFile sueltos, `mTimers`, lineas como
  miembros de BattleTask, `BattleHateRow_unk m391E0[72]`, `m38C74..m38C8C`.
- Constructores inline nuevos en `game/BattleTask.h` y `game/BattleCalc_unk.h` (`BattleCalcParam_unk`).
- `chr::ControlBase::vf2C()` inline (`return 1`).
- `BattleResult::ItemCount_unk() const` (inline nuevo en `game/BattleResult.h`).
- Simbolos renombrados: `__dt__10BattleTaskFv` (0x800F1F94, antes `fn_800F1F94`),
  `fn_801021B4__10BattleTaskFPQ23atn9ColliBaseii`.

## Pendientes

- `game/GameUI.cpp` (`GetCursorTarget_unk`) usa `RayObj_unk`: cambiarlo a `atn::ColliBase*` y quitar el typedef.
- BattleTask: al final de `@stringBase0` faltan 3 cadenas de `basic_string` ("basic_string::reserve
  length_error", "basic_string: out_of_range", "basic_string: length_error") y "Event::ActionProcessor"
  (`poolorder.py`): plantillas de `std::string` o RTTI que nuestro fuente no instancia.
- `BattleCharaTask::fn_800E9110` (98,7 %): `CharaStatus::m2CC` + `m2D0[4]` son un `FixedArray_unk<
  CharaStatusPair_unk, 4>` en el original (base 0xAA0 = &mStatus.m2CC). Toca `game/CharaStatus.h` (~200
  unidades): no hecho.
- `vf54__13GimmickCommonFv` (60 %): z se guarda antes que x/y; las redefiniciones de gmk (no const) cuadran con
  z primero, asi que no es el truco del getter const; una referencia const local no cambia nada.
- `GameEventScript::fn_803A6B9C` (cola ajena): su comentario dice 92,3 %, ahora 93,6 % por `ItemCount_unk`.
- Scripts de esta cola (en `/home/claude/scratch-N/bin`, no commiteados): `maprelabel.py` (de G), `changed.py`,
  `enumrelabel.py`, `valrelabel.py`, `offrelabel.py`, `valdiff.py`, `strdiff.py`.

## Hallazgos (una linea)

- Un inline virtual sin constantes (`vf2C() { return 1; }`) tambien renumera los `@N` y los `@enum$N` de los
  nombres de plantillas: re-emparejar por offset entre el `.o` viejo y el nuevo, tambien los `@enum$`.
- Miembros que el constructor construye con llamadas sueltas (sin `__construct_array`) no son arrays; un
  subobjeto con destructor no trivial se destruye tras una comprobacion de nulo de su direccion.
- MWCC pela la primera iteracion de los bucles de construccion de arrays de clases con constructor inline.
- objdiff no ve el valor de las constantes: `valdiff` encontro un 0.0f por 1.0f (funcion "al 99,95 % por el
  pool"), argumentos desordenados y un 3.0f por 2.0f.
- Una cadena `""` sin referencias tras un formato en el pool del original = el formato acaba en `\0`.
- Una recarga del contador entre la comprobacion y la direccion del hueco = la comprobacion pasa por un accesor
  inline (`ItemCount_unk()`).
