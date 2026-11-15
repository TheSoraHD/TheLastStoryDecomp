# enlazar-20261115, cola G (personajes a 2-4 funciones)

Agente G, rama `agent-G`. Inicio 15:18.

## Resultado por unidad

| Unidad | Antes | Despues | Estado |
|---|---|---|---|
| chr/ControlCdnStone | 55/57 | 56/57 | CdnBetray::vf1C al 100 % (`Chara_unk* c = mChara;` tras el primer test de estado); queda CdnSleep::vf10 (99,75, functor de boost::function en r29 en vez de r30) |
| chr/ControlStagger | 3/6 | 5/6 | Slip ctor (truco qE en la parada) y Stagger::vf0C (accesor `mMotion.fn_80139F2C(0)` para el id) al 100 %; queda Slip::vf0C (99,9: mChara en r3 en vez de r5) |
| chr/ControlGmkCoffinOpening | 15/18 | 17/18 | ctors de CoffinOpening y CoffinOpen al 100 % (truco qE); BreakArmor ctor: borrador 58 % -> 99,7 % con `dir.ToRotationZ_unk()` y `HasC48_unk()` |
| chr/ControlCliffhang | 5/8 | 6/8 | fn_801AB6B8 al 100 % (truco qE en el caso 1); vf0C 99,7 (huecos de pila), fn_801AB9E4 96,4 |
| chr/ControlInteractionBase | 17/21 | 18/21 | vf0C al 100 % (`Chara_unk* chara = mChara;` al principio + `c2` en la ultima copia); DuckUnderB ctor 91,9, DiveIntoWater ctor/vf0C 97,6/97,7 |
| chr/ControlPushedDown | 11/14 | 11/14 | Ottotto::vf0C 99,74 -> 99,86 (`IsPlayer()`, `m08.GetNormalize()`); PushedDown/PushedAvoid vf0C 99,3 |
| chr/ControlDamage | 38/41 | 38/41 | sin commit: ver Pendientes (fn_801AE41C `const`) |
| chr/ControlWallUp | 21/24 | 21/24 | sin commit: vf0C baja a 9 lineas con `Chara_unk* chara = mChara;` al principio (ver Pendientes) |
| chr/ControlTurnQuadruped, ControlRide, ControlCannonLift, ControlDropDown, ControlSitting, ControlDraggingStart, ControlChant, ControlMoveCursor | - | - | probadas sin 100 % (ver NONMATCHING) |

Funciones: 15563 -> 15570 (report.json del precommit), lost 0, sin bajadas de fuzzy, labelcheck/rep100 igual.

## Unidades que enlazan (bytes)

- Ninguna. CdnStone (3304 B), Stagger (344 B) y GmkCoffinOpening (1064 B) se quedan a una funcion.

## NONMATCHING con motivo

- ControlCdnSleep::vf10 / ControlDraggedStart::vf10 / ControlChaseHeld::vf10 (mismo residuo): copia en el heap
  del bind en r29 (el `motion` del ctor inline) en vez de r30 (`next`). Probado ademas de lo de qE: `new` dentro
  del bind (consume 21 `@N`), `Motion_unk&` en el ctor, ctor con un inline `PlayMotion_unk(motion, id)`.
- ControlSlip::vf0C (99,9): mChara en r3 (original r5, con r4 = 0 del argumento ya cargado). Probado: orden
  motion/frame, local `c`, referencia, `GetMotionFrame(0)` (de mChara->mMotion y de motion), inline
  `IsMotionEnd_unk(motion)` estatico, sin local `frame`.
- ControlDamageLastOne::vf0C y ControlKnockdown::vf0C: el bool del helper inline en r31/r28 (original r30/r27).
  Probado: `bool done`, referencia, local `chara`, modelo tras la llamada.
- ControlKnockdown ctor (99,95): ruta del motion (r26) y strlen (r29) cruzados en `std::string(path)`. Probado:
  sin local `path` (cruza la direccion del temporal), conversion implicita, `std/msl_common.h` delante.
- ControlTurnQuadruped ctor (99,9): el original guarda `f1` (double) directo con stfs y convierte aparte para
  comparar; probado `float a`, `mAngle < 0`, lista de inicializacion.
- ControlRideBeast ctor (99,7): mBeast/mRider r3/r6 cruzados; probado local beast/rider, reordenar, VecAssign_unk,
  `GetNodeMtx_unk("seatpoint")`.
- ControlCannonTarget ctor (99,6) y ControlOttotto::vf0C: `mChara->mPos +/- v * k`: original direccion del
  resultado en r3 y mChara en r4; probado VecAssign_unk, orden de operandos, locales, `-=`.
- ControlCannonTarget/Throw vf0C (99,9): huecos de pila de offset/point/diff; `GetQueenPoint_unk` (ya existia)
  empeora.
- ControlWallSlash::vf0C: chara/efecto/hit r27/r26/r28; probadas las permutaciones de las tres locales, sin hit,
  `int id` de fn_800F8548.
- ControlGmkBreakArmor ctor (99,7): mChara y las direcciones de rot/dir r3/r4/r5 en vez de r5/r3/r4.
- ControlMagic ctor (99,8): registros del inline `CanAct_unk` (cabecera compartida, no tocada).
- ControlDropDown ctor (98,0): el original guarda la z de la copia de m574 con un `frsp` (z en registro, no
  recargada); probadas 5 formas de la suma de -0.015.
- ControlMoveCursor fn_801C346C: bloqueo conocido del ctor vacio de Matrix3x4.

## Cambios de splits

- Ninguno (solo renombrados de `@N` por el re-emparejado de `pd.sh`).

## Para el port

- Nada (ninguna unidad del port tocada).

## Decisiones tomadas sin el usuario

- Commits de mejoras sin 100 % en dos casos con codigo mas real: borrador de BreakArmor (58 -> 99,7 %, fuera la
  matriz 4x4 escrita a mano en favor de `ToRotationZ_unk`) y Ottotto::vf0C (99,74 -> 99,86).
- `{ Chara_unk* c1 = mChara; c1->mPos = m14; }` en un bloque dentro del `case 1` de Cliffhang::fn_801AB6B8.

## Hallazgos (una linea cada uno)

- Los accesores inline reales cambian la asignacion de registros: `mMotion.fn_80139F2C(0)` en vez de
  `mTracks[0].m00` (Stagger::vf0C) e `IsPlayer()` en vez de `mType == 0` (Ottotto::vf0C) dieron el orden del original.
- `Chara_unk* chara = mChara;` como primera sentencia (y `motion = &chara->mMotion`) cuando el original usa el
  mismo mChara cargado al principio para el motion y una copia a mPos (InteractionBase::vf0C, WallUp::vf0C).
- Probar el truco qE (`Chara_unk* cN = mChara;` por sentencia) tambien en `m560 = id` antes de una copia de
  Vector3 (ctors de CoffinOpening/CoffinOpen), en un `int` (CdnBetray::vf1C, tras el primer test) y en un case
  dentro de un bloque `{ }`.
- Un helper inline nuevo con parametros con nombre en el .cpp desplaza los `@N` de la unidad: objdiff da 99,x a
  todas las funciones hasta el `pd.sh`; al iterar, filtrar las lineas con `@` de odiff2 o medir tras pd.
- `atan2` + matriz 4x4 a mano de un giro hacia un punto = `Vector3::ToRotationZ_unk()` de `math/Vec.h`
  (BreakArmor); `ToRotation_unk` deja un `frsp` en el argumento de atan2 que el original no tiene.
- `m08.GetNormalize() * k` en vez de copiar + `PSVECNormalize` da los huecos de pila del original (Ottotto).
- Hacer `const` el helper inline (`ControlKnockdown::fn_801AE41C`) hace que recargue mChara como el original
  (98,1 -> 98,7 %); requiere renombrar el simbolo a `...CFv` en symbols.txt (no commiteado).

## Pendientes

- ControlKnockdown::fn_801AE41C `const` (cabecera chr/ControlDamage.h + symbols.txt `CFv` + CharaTask.cpp lo
  llama): sube vf0C pero no llega al 100 %.
- ControlWallUp::vf0C con `Chara_unk* chara = mChara;` al principio: quedan el float de m40 (f3/f4), la carga de
  mType (r3/r4) y la llamada a `mC58.fn_8011BF3C` (r3/r4).
- `IsFree_unk` con `!mStatus.IsState_unk(0x20)` en CharaTask.h podria arreglar ControlMagic ctor (cabecera muy
  incluida: medir en lote).
- El residuo comun de boost::function (r29/r30) bloquea CdnStone (3,3 KB), DraggingStart y ChaseHeld.
