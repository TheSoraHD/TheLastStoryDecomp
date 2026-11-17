# Cola N5b (lote escribir-20261116): gimmicks, segunda pasada

Inicio: 2026-11-17 08:55 (tope 10:55). Rama agent-N5b.

| Funcion | Antes | Despues |
|---|---|---|
| gmk/SwingObject vf10 | 95,8 % | 98,36 % (e418d23) |
| gmk/SwingObject SwingPart_unk ctor | 99,5 % | 99,46 % (sin cambio) |
| gmk/SwingObject fn_803FFF40 | 96,4 % | 99,90 % (e418d23) |
| gmk/SwingObject fn_804003E0 | 97,2 % | **100 %** (e418d23) |
| gmk/PassBoard vf0C | 98,7 % | sin cambio (no trabajada: bloqueo del cursor) |
| gmk/RandomMerchant vf10 / vf20 | 98,6 / 99,3 % | sin cambio (no trabajadas: bloqueo del cursor) |
| gmk/MotionObject vf10 / vf24 | 97,8 / 99,9 % | sin cambio |
| gmk/LandMine Start | 98,3 % | sin cambio (no trabajada) |
| gmk/ChickenManager vf0C (extra) | 93,9 % | 96,15 % (b30a05d) |
| gmk/RotaryRoom fn_80424AD4 (extra) | 99,16 % | **100 %** (b3c2971) |
| gmk/RotaryRoom fn_80424FB4 (extra) | 99,15 % | **100 %** (66be691) |
| gmk/PigeonManager vf0C (extra) | 93,92 % | 96,15 % (d35674b) |
| gmk/RotaryRoom vf0C (extra) | 96,14 % | 96,23 % (66be691) |
| gmk/RotaryRoom vf10 (extra) | 95,65 % | 95,85 % (59cd3d6) |
| gmk/InteractionSpot Start (extra) | 97,81 % | 98,07 % (3a59809) |
| gmk/WaitObject vf20 (extra) | 99,95 % | 99,98 % (b3c2971) |
| gmk/BulletPoint vf10 | 98,5 % | sin cambio |
| gmk/ShakeCamera vf10 | 98,2 % | 99,34 % (b30a05d) |

Resumen: 3 funciones nuevas al 100 % (SwingPart_unk fn_804003E0, RotaryRoom fn_80424AD4 y fn_80424FB4);
report.json 15953 -> 15956, lost 0, sin bajadas, labelcheck/rep100 igual. Ninguna unidad nueva enlaza
(SwingObject 17/20: faltan vf10 98,36, SwingPart ctor 99,46 y fn_803FFF40 99,90; RotaryRoom 15/18: faltan
vf0C 96,2, vf10 95,9 y fn_80425498 99,7).

## Variantes probadas

- gmk/SwingObject UpdateSwing_unk (los FPR que bloqueaban vf10, fn_803FFF40 y fn_804003E0): tres arreglos.
  (1) `core->mMaxWind * ToRad_unk(2.0f)` en vez de `* 0.034906585f`: el literal suelto se carga antes que
  el miembro (MWCC lo pone como primer operando); plegado desde la inline, el miembro va primero como en
  el original (mismo valor en `.sdata2`). (2) La suma del viento escrita en cada uso
  (`range / mMaxWind * (mWindPower + mWind)` y `if (mWindPower + mWind > mMaxWind)`) en vez del local
  `wind`: con el local MWCC reasocia `range / max * wind` como `(range * wind) / max` en fn_804003E0 y
  conmuta los operandos en vf10. (3) Los tres senos por `SinF_unk(float)` (inline que devuelve
  `(float)sin(x)`): el argumento del seno siguiente se calcula antes que el producto, como en el original;
  el seno del primer angulo ya no necesita local. Probados antes sin efecto: `s * ToRad`, `0.0349 * max`,
  locales `max`/`ratio`, `range * wind / max` (peor), sin2 en local, `ToRad_unk(1.0f) *`.
- SwingPart_unk fn_804003E0 (-> 100): el bucle de las matrices de nodos en una inline `CalcNodes_unk(part)`
  compartida con fn_803FFF40 (con `local` antes que `count` y `mtx` antes que `t`: un nivel de inline
  invierte la pila). perm.py de nodes/data/count/local/i fuera de la inline: 99,82 en los 120 ordenes.
- SwingPart_unk fn_803FFF40 (96,4 -> 99,90): CalcNodes_unk y el efecto en `PlayPartEfp_unk(mgr, efp, owner)`
  (el gestor es argumento de la inline y se carga antes, como en el original). Queda m98 de m9C/mA0/mA4 en
  r26/r27 (r28/r29 en el original): sin base local, tres locales, base arriba, base como
  `atn::Matrix3x4*`, inline `PartNodeMtx_unk(this, i)` / `(this, node)`, AllocNodes_unk entera (96,7,
  saca cadenas `@STRING@`), m80 antes de mA4, `m98.reset(new ...)` directo: igual o peor.
- SwingObject vf10 (98,36): queda la direccion del modelo (r31+0xF4) calculada antes del salto del LOD.
  Probados: `(atn::Model*)&mModel` en la llamada, `atn::Model* model` local (95,5), LOD en
  `DecLod_unk(model)` (95,5), el lod repetido sin local (98,48 pero peor forma), `Hide_unk(this)`.
- SwingPart_unk ctor (99,46): m94..m134 en el cuerpo (88,6), `m98()`, `mLoader()` explicito, m134 en el
  cuerpo: igual. Es el color de vt/begin/end del bucle del array de SwingParam_unk.
- gmk/ShakeCamera vf10 (98,18 -> 99,34): `SqrtF_unk(float)` para las dos raices del volumen (la rate en
  f31) y el tono con `Rand01_unk` escrito a mano (como ColliAttrManager: con la inline el 0.5f se carga
  antes que el campo). Queda r5/r7 del puntero a lbl_8087F430 (y r5/r6 del resto de rand() % 1001).
  Probados: `SetShake_unk(m96C % 2, ...)` como MotionCommand_unk, x/y en la llamada, frame local, perm de
  rate/x/y/field (24), RandRange_unk con el factor o el rango local o escrito: igual o peor.
- gmk/ChickenManager vf0C (93,92 -> 96,15): `Chicken_unk* c = &mChickens[i]` para mCenter/mRadius (el
  original no recarga mChickens entre las dos copias). Mismo arreglo valdria para PigeonManager vf0C.
- gmk/ChickenManager FindNear_unk (pila de fn_8040C058/fn_8040C610/vf0C): `atn::Vector3 t = ...; mTmp = t;`
  quita el `frsp` de `t = mTmp = ...` (97,56/97,97 pero vf0C 95,89; sin commit). Modelo de pila que
  predice las tres variantes medidas (de arriba abajo): locales con nombre de la funcion en orden de
  declaracion, temporales, y despues cada inline con su lista (con nombre + temporales + inlines
  anidadas) invertida. El original (next, n, d, op, temp de next, diff, t, pos) no sale con FindNear por
  valor, con `AvoidOthers_unk` (bloque comun de fn_8040C058/fn_8040C610) ni con las dos cosas.
- gmk/MotionObject vf24 (99,94): `const void*`/`void*`/`int` para data/size, CsvTab(const void*) con los
  separadores dentro, size local y data en la llamada: igual o peor (size toma el registro de `field`, r22;
  en el original el de `fade`, r25).
- gmk/Chain fn_8043B9E0: lo que falta es el push_back de std/vector.h (cabecera compartida), no tocado.
- gmk/FlyObjManager fn_80427870: el fabs/frsp esta en fn_80139F60 de Vec.h (fuera de la cola).
- gmk/RotaryRoom fn_80424AD4 (99,16 -> 100): la pila de las tres rotaciones de RotateZYX_unk (Vec.h) sale
  al reves (z abajo): una copia local `RotateZYX_unk(m, rot)` que llama a `RotateAxes_unk(p, rot)` (un
  nivel mas de inline para las tres RotateAxis_unk) la da vuelta. Registros: `atn::Model* model; int type =
  part->mType; switch (type)` al principio del cuerpo del bucle (perm.py: model antes que type) y en el
  case 4 `set; inst; i` arriba con `e` puntero (`n++` al final). Mismo estilo que gmk/ArenaFloor.
- gmk/RotaryRoom fn_80424FB4 (99,15 -> 100): case 0 por `PlaceModel_unk(model, m)` ->
  `SetModelScale_unk(model, m)` -> `MaxScale_unk(m)` -> `ColLen_unk` (las de ArenaFloor): el puntero al
  modelo como parametro de una inline toma el ultimo registro salvado (r22) y las columnas quedan en el
  orden de pila del original. Sin SetModelScale_unk (un nivel menos) la pila de las columnas sale
  invertida (99,75); con el modelo como local arriba 99,55; con MaxScale_unk escrito en el case 99,30.
- gmk/RotaryRoom vf0C (96,14 -> 96,23): MaxScale_unk en el case 0. PlaceModel_unk ahi: 96,14.
- gmk/WaitObject vf20 (99,95 -> 99,98): `WaitTrigger_unk::m08` pasa a `float m08[5]` (las cinco
  lecturas del TRIGGER se direccionan desde el trigger, +8) y `atn::VecAssign_unk(sphere.mCenter, pos)`
  (registros r25/r26 del original). Queda el orden de los dos `addi` de &pos/&sphere al principio; probados
  `colli->mSphere` por VecAssign o por campos, `Vector3(pos)` (98,1), `pos = pos + m6C` (96,9), pos.x +=...,
  sphere declarado antes (99,66) o justo tras pos, referencia al centro: igual o peor.
- gmk/WaitObject vf60 (93,33): el original relee m54 en el bucle. Probados: indice directo, puntero, inline
  `HasTrigger_unk(self, type)` (89,7), `if (m54 != 0) {bucle}` (84,6), if anidados, `m54 == t.mState`
  (93,75), puntero que avanza: igual o peor.
- gmk/OpenBoard vf10: `LerpF_unk(from, to, t)` / `Lerp2_unk(from, d, t)` en Interp_unk: 98,8 (peor).
- gmk/DropItem vf14: el codigo muerto del original (comparaciones de m16C..m178 con 0 y 1 sin usar el
  resultado y tres stores a la pila) no se ha escrito; el `@wstringBase0` del original no es `.sdata`.
- gmk/PigeonManager vf0C (93,92 -> 96,15): el mismo puntero `Pigeon_unk* c` que ChickenManager. `t =
  mPigeons[i].mPos; mTmp = t;` en FindNear_unk: fn_804052EC 97,28 -> 97,56 pero vf0C 96,15 -> 95,89 (sin
  commit, como en Chicken).
- gmk/RotaryRoom vf10 (95,65 -> 95,85): `if (mFrames > mFrame)` (el original carga mFrames primero). Queda la
  relectura de `mTree->mFlags` antes de cada escritura (parece un campo de bits de atn::ColliBase, cabecera
  ajena) y el orden del contador del bucle. `int frames` local: igual.
- gmk/RotaryRoom fn_80425498 (99,74): registros de `areas` y de los cursores del primer bucle; perm.py de
  areas/j/a/k/seg (120): 99,74 como mucho; inline `ShowArea_unk(areas)` 99,35; `area = colli->m08`, campos
  `mSize/mData`, sin locales: igual o peor.

- gmk/InteractionSpot Start (97,81 -> 98,07): las tres busquedas en `SpotTable_unk::FindSpots_unk(spots, id)`
  (el 98,07 de qN3, ahora con commit). La tabla del original va en r7 (ultimo) y el puntero encontrado en
  el registro del id; probados FindSpots estatica con la tabla como parametro (en los dos ordenes),
  local `spot` resultado (96,1), puntero al elemento (93,0), `int id` local antes/despues: igual o peor.

- gmk/BulletPoint vf10 (98,55): el `li r6, 0; mr r5, r6` del original (el 0 copiado de otro registro, bloqueo
  conocido) no sale con asignacion encadenada `disabled = ok = false`, `ok = disabled = false` ni con
  `bool disabled = ok;` en CanTarget_unk: MWCC pliega la constante (sin commit).
- gmk/RotaryRoom fn_80425498: en el original `areas` del primer bucle toma r4, el mismo registro que el
  `area` del principio (que no vive a traves de la llamada); `area = colli->m08` no lo reproduce.

- gmk/MotionObject vf10 (97,79): el original carga los argumentos de fn_80097C08 en el orden del struct
  (mMotion, mBlend, mLoop, mReverse). Inline `PlayMotion_unk(model, motion, blend, loop, reverse)`: 97,81
  (orden invertido); con los parametros al reves (reverse, loop, blend, motion) 97,95 y solo mBlend fuera de
  sitio, pero es un orden poco natural y quedan los registros de this (r24/r25). Sin commit. Mismo patron
  en WaitObject vf40 (99,28).

## Para el port

- gmk/WaitObject: `WaitTrigger_unk::m08` `unsigned char[0x14]` -> `float[5]` (vf20 escribe `trigger.m08[i]`;
  las vistas `(WaitArea_unk*)t->m08` no cambian). Mismo comportamiento.
- gmk/SwingObject: `UpdateSwing_unk` con `SinF_unk`, la suma del viento escrita en cada uso y el limite de
  mAngle0 `mMaxWind * ToRad_unk(2.0f)` (mismo valor que 0.034906585f); inlines nuevas `CalcNodes_unk`,
  `PlayPartEfp_unk`. Mismo comportamiento.
- gmk/RotaryRoom: `RotateZYX_unk`/`RotateAxes_unk` locales (mismo calculo que atn::Matrix3x4::RotateZYX_unk),
  `ColLen_unk`/`MaxScale_unk`/`SetModelScale_unk`/`PlaceModel_unk` (como gmk/ArenaFloor). Mismo
  comportamiento.
- gmk/ShakeCamera (`SqrtF_unk`), gmk/ChickenManager y gmk/PigeonManager (puntero al elemento): mismo
  comportamiento. Literales: ninguno corregido. Ninguna unidad del port tocada.

## Decisiones tomadas sin el usuario

- Inlines nuevas con nombre inventado (todas con `// inferred:`): SinF_unk, CalcNodes_unk, PlayPartEfp_unk
  (SwingObject); SqrtF_unk (ShakeCamera); RotateAxes_unk, RotateZYX_unk, ColLen_unk, MaxScale_unk,
  SetModelScale_unk, PlaceModel_unk (RotaryRoom). Locales declarados arriba en RotaryRoom fn_80424AD4
  (`model`, `type`, y en el case 4 `inst`, `i`), como hace gmk/ArenaFloor.
- `WaitTrigger_unk::m08` como `float[5]` (cabecera de la familia gmk).

## Hallazgos

- Un literal float suelto en `x * K` se carga antes que el miembro (MWCC lo pone de primer operando); si el
  original carga el miembro antes, la constante sale de una inline plegada (`x * ToRad_unk(2.0f)`).
  (escribir-20261116 qN5b)
- Un local `float wind = a + b` usado en `r / max * wind` deja que MWCC reasocie a `(r * wind) / max` o
  conmute operandos; escribir la suma en cada uso (CSE) da el orden del original. (escribir-20261116 qN5b)
- `(float)sin(x)` a traves de una inline `SinF_unk(float)` (y `SqrtF_unk`) cambia la planificacion: el
  argumento de la llamada siguiente se calcula antes y los FPR salen como en el original.
  (escribir-20261116 qN5b)
- Pila, modelo que predice todas las variantes medidas (de arriba abajo): locales con nombre de la funcion
  en orden de declaracion, sus temporales, y despues la lista de cada inline invertida (la lista de una
  inline es igual: con nombre, temporales, inlines anidadas). Un bloque en espejo del original se arregla
  con un nivel mas de inline alrededor de las llamadas (RotaryRoom RotateAxes_unk) y un bloque en espejo
  propio con un nivel menos. (escribir-20261116 qN5b)
- Un puntero que el original guarda en el ultimo registro salvado (despues de las direcciones de pila
  izadas del bucle) es el parametro de una inline (`PlaceModel_unk(model, m)`), no un local.
  (escribir-20261116 qN5b)
- `((float*)s.m08)[i]` se direcciona desde `&m08`; si el original lo hace desde `&s` (+8) el miembro es un
  array `float m08[5]`. (escribir-20261116 qN5b)
- Copias `a[i].x = v; a[i].y = w;` que el original hace sin releer el array: `T* c = &a[i]`.
  (escribir-20261116 qN5b)

## Bloqueos

- SwingPart_unk ctor (99,46): color de vt/begin/end del bucle del array de SwingParam_unk tras el ctor
  inline de LH_Binary. SwingPart_unk fn_803FFF40 (99,90): m98 de m9C/mA0/mA4 en r26/r27 (r28/r29).
  SwingObject vf10 (98,36): la direccion del modelo izada antes del LOD.
- WaitObject vf60: relectura de m54 en el bucle. MotionObject vf24: registro del tamano del CSV.
- ChickenManager/PigeonManager FindNear_unk: pila (pos, t, diff) del original no reproducida.

## Cambios de splits/symbols

- splits.txt sin cambios. symbols.txt: solo `@N` re-emparejados con pd.sh (gmk/SwingObject, ShakeCamera,
  ChickenManager, PigeonManager, RotaryRoom, WaitObject).
- Scripts (`/home/claude/scratch-N5b/bin/`): `vs.py`/`vrr.py`/`vwo.py`... (variante de una funcion: base,
  viejo, nuevo), `vu.py <unidad> <base> <patron> [viejo nuevo]...` (variante, % de las funciones que casan
  con el patron), `vh.py` (variante de una cabecera), `side.py` (copia de agentes/bin con la ruta de la
  copia), `sd2.py` (sd2n5.py con la ruta de la copia).
