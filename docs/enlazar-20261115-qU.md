# enlazar-20261115, cola U (trucos de la sesion en sitios apuntados fuera de alcance)

Agente U, rama `agent-U`, 2026-11-15 19:52-20:40.

## Resultado por unidad

| Unidad | Funcion | Antes | Despues | Nota |
|---|---|---|---|---|
| Event/Manager | `fn_8047BFF4` | 99,55 % | **100 %** | 139 -> 140/185, commit `d0e6dca` |
| Event/Manager | `fn_8047B768` | 99,26 % | 99,26 % | 99,69 con macro (no commiteado, no es la ultima) |
| Event/Manager | `fn_80488E00` | 94,64 % | 94,64 % | sin cambios |
| game/Em206EggMonster | `fn_802C6878` | 95 % | 95 % | relectura arreglada con accesor `int`, queda pila (no commiteado) |
| game/QuadrupedTask | `vf20`, `vf24` | 91,44 / 99,96 % | igual | sin cambios |
| gmk/TodomeCountUp | `vf10` | - | - | ya enlaza (12/12): punto 3 de la cola sin objeto |
| chr/ControlCaught | - | - | - | ya enlaza (`matching: true`) |
| atn/PostFxColorMatrix, atn/PostFxMotionBlur, atn/PostFxRain, gmk/OpenBoard, gmk/OnOff, gmk/Brazier | ultima(s) funcion(es) | | igual | variantes abajo |

## Unidades que enlazan (bytes)

Ninguna.

## NONMATCHING con motivo

- `Event/Manager fn_8047B768` (99,26 %): next/section/channel rotados (original r31/r30/r29, aqui r29/r31/r30).
  Escribiendo `JumpTarget_unk` como macro (como `JUMP_LENGTH_UNK` de `fn_8047BBBC`) se arregla el cruce r5/r6
  de las tres copias de `FindCutByIndex_unk` (99,69 %), pero la rotacion sigue. Probado ademas: comprobaciones
  de indice escritas (qS), `next` antes/despues de `result`/`frame`, `nextIndex` local, cuenta de secciones
  local, `return` tempranos, `if` anidado al final, `int i` fuera del for, declaracion separada de
  section/channel, `value & ~0x8000` sin local, `const`/`unsigned` en `value`.
- `Event/Manager fn_80488E00` (94,64 %): el original relee `mActions.size()` en el test del bucle de acciones
  (con prueba previa `cmpwi r0, 0; ble`) y reutiliza ese registro en la comprobacion de indice; aqui la cuenta
  se saca a r23. Probado: comprobaciones escritas (95,04 %), solo la de la accion, cuenta local, `if (size > 0)`
  delante, `while`, `start` local.
- `game/Em206EggMonster fn_802C6878` (95 %): con un accesor `int IsFlag25_unk() const { return mFlag25; }`
  se arregla la relectura de 0x12A4 (con `bool` sale `cntlzw`/`srwi` doble); queda el slot de pila del
  temporal de `mRot` (el original guarda x en 0x14 antes de cargar `pos`). Probado: `VecAssign_unk`,
  `CharaTask* c`, referencia `p`, `Vector3(0.0f)`, helper `SetPosRot_unk(c, pos, rot)`. No commiteado (no sube
  a 100 y la unidad esta en 12/31).
- `game/QuadrupedTask vf24` (99,96 %, dt en f28/f29): locales de `GetMotionLength_unk`, dt tras las llamadas,
  `const`, `Div_unk` por valor, blend/walkSpeed/dashSpeed antes de dt o en otro orden, bloque para dt.
- `game/QuadrupedTask vf20` (91 %): `IsOnLedge_unk()` da `cntlzw` doble con `!`; no lo arregla.
- `atn/PostFxColorMatrix vf10` (97,12 %): `ClampF_unk` por valor (ternario 92,2; con `if` 92,2), referencia a la
  columna (95,8), `ScaleOf_unk` inline (95,2).
- `atn/PostFxMotionBlur vf10` (99,78 %): `Lerp_unk(a, b, k)` por valor con el ternario dentro (98,3, repite el
  ternario), con `t`/`k` locales (igual tras quitar ruido de `@N`), `sum2` antes del `if`, t antes de k, k/t
  fuera del `if`, `sum2 = sum2 + w`.
- `atn/PostFxRain vf10` (99,72 %): `fn_800B0A48() == 0`, w/h primero, puntero local del render manager.
- `gmk/OpenBoard vf10` (99,76 %): `Mix_unk(a, b, t)` por valor (98,8), `t` antes de `old`, `from` local,
  `cur` local, `Mix_unk(a, d, t)`, `Interp2_unk(float t)`, `Lerp_unk::SetCur_unk(float)`/`(const float&)`,
  delta antes del store.
- `gmk/OnOff fn_803F8324` (99,16 %): getter de `mBoxPos` por valor (inline libre, con y sin local), asignacion,
  `mtx = mMtx`, `Matrix3x4 mtx(mMtx)`. El original lee la z de `mBoxPos` como `0x97c(r31)`: la direccion se
  forma desde `this` ya en r31, aqui desde r3.
- `gmk/ShakeCamera vf10` (98,18 %): en RandRange_unk `(max - min) * t + min`, `t` local (98,11: el
  `fmadds` cambia de operandos, el resto igual), `d` local (93,3), referencias const + `t` (81,4), puntero
  del campo en local antes o despues de rate (88,4 antes; igual despues), min/max locales (88,3). En el
  original el puntero del campo (r5) se numera antes que el calculo de `rand() % 1001` y rate ocupa el
  registro de `min` (f31).
- `chr/ControlGmkCoffinOpening` ctor de ControlGmkBreakArmor (99,71 %, fuera de la cola, mirado por el
  patron qE): `Chara_unk* cN = mChara` antes de cada sentencia o de las dos, rot antes de dir,
  declaracion y asignacion separadas, `VecAssign_unk(rot, ...)` (94,4), copia `d` de dir, resultado de
  `ToRotationZ_unk` en local, dir como temporal, `atn::Vector3(dir)` (94,4), `c->mRot = rot`.
- `game/Em206EggMonster fn_802C6878` (segunda pasada, con el accesor `int`): rot/vel construidos antes,
  `SetRot_unk` de CharaTask.h, `CharaTask* c` antes de las copias o de la llamada final: igual (el x del
  temporal de mRot y el `mr r3, r30` del original van antes del `psq_l` de `pos`).
- `gmk/Brazier fn_80407D24` (99,83 %): radio local antes/despues de la copia, helper `SetSphereUp_unk`,
  mRadius antes de `y +=`, `y = y + r`, `sphere` antes de `res`, `mSpeed * mDir`, paso en local.

## Cambios de splits

Ninguno. `symbols.txt`: solo los renombrados `@N` de Event/Manager de `pd.sh` (y devuelta la linea de
`@stringBase0` de MiniMapTask, 0x8075765C).

## Para el port

Nada (Event/Manager solo cambia un bucle local de `fn_8047BFF4`).

## Decisiones tomadas sin el usuario

- `fn_8047BFF4`: la cuenta de canales se lee en el test del bucle (`j < (int)section->mChannels.size()`) en
  vez del local `channels`; se quedan los accesores `GetSection_unk`/`GetChannel_unk`/`GetAction_unk` (con las
  comprobaciones escritas de qS tambien da 100 %, no hacen falta).

## Hallazgos (una linea cada uno)

- Cuenta de un bucle en un local frente a leida en el test: con el local los contadores de los dos bucles
  exteriores se rotan (indice en volatil); leida en el test, como el original (Event `fn_8047BFF4`).
- Accesor de un bit que el original relee antes de limpiarlo: si el `bool` da `cntlzw`/`srwi` doble, probar el
  accesor con `int` (Em206EggMonster `fn_802C6878`, `mFlag25`).
- Una inline estatica con referencias de salida (`JumpTarget_unk(..., Cut*&, int&)`) cruza registros del
  `FindCutByIndex_unk` inlinado que la misma logica como macro no cruza (Event `fn_8047B768`).

- Event/DebugInfo_unk: el `""` que el original pone en `@stringBase0+0x14` (tras "vector length error")
  no lo referencia ninguna funcion anterior a `fn_80221FA4` por `@stringBase0+0x14`; esa funcion usa
  `String_unk::sEmpty_unk` (no un literal) cuando no hay evento. El original tiene ademas
  `Cut::GetSection_unk`, `Section::GetChannel_unk` y `Channel::GetAction_unk` fuera de linea (debiles) en
  esta unidad.

## Pendientes

- `Event/Manager fn_8047B768`: macro `JT_UNK` en vez de `JumpTarget_unk` (99,26 -> 99,69); falta la rotacion
  next/section/channel.
- `game/Em206EggMonster fn_802C6878`: accesor `int IsFlag25_unk()` en `CharaTask.h` cuando se ataque la pila.
