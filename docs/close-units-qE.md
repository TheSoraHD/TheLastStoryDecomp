# Lote close-units, cola E (chr y game, 4-19 pendientes)

Inicio: 2026-10-22 05:34 CEST. Rama agent-E (sale de close-units con la cola A integrada, 6528 funciones al 100%).
La herramienta Projects no estaba disponible: reglas tomadas del prompt del lote y de docs/.

## Unidades

- `chr/ControlCannonLift` (13/17, sin cambio en el recuento): las 4 pendientes suben mucho pero no llegan.
  - `ControlCannonTarget` ctor 97.09% -> 99.63%: `GetFrontDir_unk` lee `control->mChara` en un local y el ctor
    escribe `m560` por un local `c = mChara`; llamada a `gmk::Cannonball::fn_803FB830` con su nombre real (incluye
    gmk/Cannonball.h, se quita la declaracion `extern "C"` local). Queda el `__VecCopy` r3/r4 (bloqueo documentado).
  - `ControlCannonLift::vf10` 97.78% -> 98.92%: el `new` va directo como argumento de `boost::bind`. Queda r28/r31.
  - `ControlCannonThrow::vf0C` 95.82% -> 99.90% y `ControlCannonTarget::vf0C` 98.44% -> 99.95%: cada prueba de la
    reina usa su propia variable (`queen`/`queen2`) y la rotacion del offset va escrita a mano (sin
    `GetQueenPoint_unk`), `found` como `int`. Quedan solo slots de pila de offset/punto/diferencia (~10 intentos cada una).
  - Hallazgo MWCC: reutilizar la misma variable local en dos `if (... (p = Find()) != 0 ...)` seguidos la mantiene
    viva entre ambos y cambia el reparto de registros salvados; con dos variables distintas sale como el original.
- `chr/ControlPushedDown` (10/14 -> 11/14): `ControlPushedAvoid` ctor 99.94% -> 100%. El vector frente del otro
  personaje va escrito a mano (sin `GetFrontDir_unk`), declarado entre `side` y `up` sin inicializar y rellenado
  componente a componente despues de `up` (el original guarda `up` antes que `front`).
  - `ControlPushedDown::vf0C` 98.71% -> 99.29% (`one` antes que `zero`, `motion` antes que `ret`, `mChara` directo
    en las llamadas de voz). Quedan f29/f30 y r4/r5 (10 intentos).
  - `ControlOttotto::vf0C` 99.06% -> 99.33% (`c = mChara` para m580/m584). Queda el contador en r5 y slots (8 intentos).
  - `ControlPushedAvoid::vf0C` 99.26% -> 99.28% (`m30 = scale + m30`). Registros/fneg sin cambios (8 intentos).
  - Hallazgo MWCC: un `Vector3 v;` declarado antes que otro inicializado y rellenado despues por componentes recibe
    el slot que le corresponde por declaracion, pero sus stores se emiten despues de los del otro.
- `game/Arms` (17/21 -> 19/21): `fn_8004439C` 99.84% -> 100% (grados a radianes con `atn::DegToRad_unk`, que
  multiplica `deg * k`; escrito `k * deg` salian f0/f2 cambiados). `fn_80044FBC` 99.93% -> 100% (`last` asignado en
  la condicion: `if ((last = chara->mFDC) != user)`). No enlaza:
  - `vf10` sigue 99.90% (6 intentos: Set_unk, asignacion, floats sueltos, inline, orden t/m de las matrices).
  - `fn_800457A4` (0%, factoria Sword/Gun) sin escribir: el borrador da 78.8%. El original toma los "" del objeto
    `lbl_80730F08` (8 bytes, aparte de `@stringBase0`), lo que apunta a otra TU, y pasa sus 5 parametros tal cual a
    los ctores (kind -> m04, id -> m08; r8 de mas: el ctor real tendria un quinto parametro sin usar). Pendiente (necesitaria partir el split de Arms en 0x800457A4).
  - Hallazgo MWCC: `x = k * x` y `x = x * k` (float) reparten distinto los registros volatiles en una secuencia de tres.
- `game/Em201ZanGurg` (11/15 -> 12/15): `fn_802640FC` 99.9% -> 100% escribiendo la diferencia de angulo en linea
  (con el inline `AngleDiffDeg_unk` salen f0/f3 cambiados; en `SelectAttack_unk`/vf10 el inline si casa).
  - `vf0C` 98.0% -> 99.89%: los tres colores como locales con nombre (`c`, `b`, `a`) y llamada directa a
    `fn_80094958`; arregla el r3/r4 del pool. Quedan los slots de a/c (6 ordenes probados).
  - ctor 99.77% (3 intentos mas: literal con `=`, puntero aparte, params aparte) y vf3C 99.71% (frsp de la z en el
    ToRotation inline, familia ya documentada; 3 intentos): sin cambios.
- `game/MapPartsLocatorSet` (9/13, sin cambios): `vf2C` 99.45% (3 intentos: puntero aparte, temporal con nombre,
  conversion implicita) y ctor 99.91% son registros del ctor inline de `std::string` desde `const char*`
  (r27/r28, r29/r30: misma familia que el ctor de Em201ZanGurg); `fn_8049B1F0` 97.3% (6 intentos: `mData[i]`,
  local `inst`, offset en bytes, indice sin signo, helper): MWCC reduce `i * 4` a una variable de induccion que el
  original no tiene. `vf24` 87.9% saltada (ya investigada, lectura doble de la caja).
- `game/MotionCommand_unk` (24/28): saltada, bloqueos documentados (parser sobre el presupuesto de inline solo con
  registros; `push_back` de `vector<string>` y pool de cadenas que empieza por "vector length error").
- `game/ShopIdentify` (16/20, sin nuevas): `fn_805897D4` 98.66% -> 98.97% (`mIndex--`; quedan registros volatiles del
  bucle, 7 intentos). `fn_80589DE0` 97.60% sin cambios (4 intentos): es la planificacion del argumento de tamano de
  `fn_8006EF48` (mismo bloqueo que MenuCommonTask y MP/D2AMpMenuSetRule). `fn_8058A0F0` (doble lectura del texto,
  SetPaneText) y `fn_8058A3AC` (insert de vector MSL sin escribir) saltadas: bloqueos documentados.
- `chr/ControlAppear` (17/24, sin cambios): `vf40` es el r3/r4 de `__VecCopy` (Assign_unk probado); `ControlAppearEnd/
  Skelton::vf10` son la planificacion del ptmf de bind (como ControlRideBeast::vf10; 3 intentos: local, inline,
  puntero a miembro en local). Las copias debiles invoke/manage del bind con el enum local no se pueden emparejar:
  nuestro .o las nombra `@enum$5814ControlAppear_cpp` y el original `@enum$3246...` (el numero depende del recuento
  de tipos de toda la TU). Saltada.
- `chr/ControlCliffhang` (3/8, sin cambios): `fn_801AC14C` 99.1% (10 intentos: rama vacia, `ret = ret`, constante,
  return, inline vacio, escrituras muertas...): el original evalua `right != 0` y salta sin escribir nada; con la
  rama vacia MWCC elimina la comparacion (96.98%) y con cualquier cuerpo deja un `li r4, -1` o reordena.
  `fn_801AB6B8` (r3/r4 de copia de Vector3) y `vf10` (prologo) son bloqueos documentados; `vf0C` y `fn_801AB9E4`
  (slots de pila repartidos por toda la funcion) no se tocan.
- `game/CharaFaceControl` (12/17 -> 13/17): `fn_8011EE58` 87.67% -> 100% con `(float)__fabs(total)` en lugar de
  `atn::Abs_unk(total)` (el original hace el `frsp` a f1, un temporal nuevo). `fn_8011EB18` 99.21% sin cambios
  (3 intentos; registros del operator= inline de Buffer_unk). `fn_8011D844`/`fn_8011E200` (registros/pila, ya
  investigadas) y `fn_8011EF14` (push_back de vector<int>) saltadas.
  - Hallazgo MWCC: `atn::Abs_unk(x) < k` reutiliza el registro del fabs para el redondeo; `(float)__fabs(x) < k`
    redondea a un registro nuevo.
- `game/CostumeEditChara` (6/11, sin cambios): `vf14` 98.9% (7 intentos mas: desplazamiento en bytes, `+0x680` a mano,
  puntero al array, local, inline, doble lectura): MWCC siempre pliega 0x680 en el `lwz` (add + lwz 0x680) y el
  original hace addi + lwzx. `vf10` 99.99% y las tres copias "z primero" (fn_801E9168/917C/91A0, sin helper en Vec.h
  y sin asm permitido) saltadas: ya investigadas.
- `game/GameOverTask` (2/7 -> 3/7): `fn_80377298` 99.45% -> 100%. Dos cambios: (1) **symbols.txt (decision sin el
  usuario)**: el objeto `lbl_8074DDF8` (.rodata, 0x9C) se parte en las 11 tablas locales de la fuente (`sMsgTitle`
  0x8074DDF8, `sOptTitle` 0x8074DE08, `sMsg` 0x8074DE10, `sOpt` 0x8074DE20, `sMsgEvent` 0x8074DE30, `sOptEvent`
  0x8074DE40, `sMsgRetry` 0x8074DE50, `sOptRetry` 0x8074DE60, `sMsgRetryTitle` 0x8074DE68, `sMsgArena` 0x8074DE78,
  `sOptArena` 0x8074DE88, todas scope:local) y `lbl_808857F8` (.sdata2) pasa a `sOptRetryTitle` (local); ningun .cpp
  los usaba. (2) `opts = sOptRetryTitle; push_back(opts[0]);` en dos sentencias arregla el r3/r4. Build limpio OK.
  - ctor 99.59% -> 99.71% (`dst != src` en CopyString_unk; quedan r30/r31 de origen/destino, 6 intentos). vf0C
    (48.9%), vf10 (89.7%) y vf14 (84.7%) saltadas (estructuras de escena sin reconstruir, demasiado grandes).
- `game/LibraryTask` (18/23, sin cambios): ctor 99.69% (6 intentos: literal en el bucle, `m58[i]` directo, puntero
  que avanza, asignacion separada): r30/r31 de ruta/desplazamiento. `vf14` 99.51% (5 intentos: orden l/r, lectura
  tardia, `book` local, hash en local, layouts en locales): r28/r29. El resto (push_back y pool de basic_string)
  son bloqueos documentados.
- `game/NewCampPartsHint` (14/19, sin cambios): `vf3C` 96.90% (5 intentos mas: `!show`, helper, `hidden` local,
  `++i`): r6/r7 de entrada/desplazamiento. vf14/vf30 (registros) y vf20/vf30/fn_80516004 (push_back) saltadas.
- `chr/ControlDive` (19/25, sin cambios): `ControlCoverSlip::vf0C` 98.9% (5 intentos: `ret` tardio, `ret = 0` en la
  rama, copia de m08, rot local): `ret` sigue en r31. Resto: registros/planificacion ya investigados (vf0C 85%,
  SwatTurn r4/r5 + frsp, SwapPos __VecCopy, BackStep r30/r31). Saltada tras eso.
- `chr/ControlJumpAt` (21/28): `ControlGrappleThrown::vf0C` escrita (era un borrador de 3 lineas): 9.04% -> 99.77%.
  Gira 0.02805 rad por frame y cae con gravedad -98.1/fps^2 durante 112 frames (barrido `fn_8004D388` como
  ControlTurnQuadruped). Queda el r3/r4 de `__VecCopy` al copiar la posicion (bloqueo documentado).
  - Claves: rotacion z/y/x en un inline con `SetIdentity` dentro y temporales `Mtx t; Mtx r;` (t primero);
    vector de movimiento construido con `Vector3(0, g, 0.125)` y `g` en local antes (fija el orden 0.125/-98.1/0.0
    del pool); `chara = mChara` local antes de la matriz.
  - Struct local `GrappleCollResult_unk` (copia de la de ControlTurnQuadruped) y `mFrameRate` (0x30) anadido al
    struct local `Unk_8087F0A8` del .cpp. Resto de funciones de la unidad: planificacion/registros documentados.
- `game/ChaserNpc` (8/14 -> 10/14): `fn_8018161C` 97.62% -> 100% (rotacion z/y/x en un inline `ChaserRotZYX_unk`
  con SetIdentity dentro y `Mtx t; Mtx r;`, y el frente leido como `chara->mRot.y` dentro del inline
  `GetFrontOf_unk`). `fn_8018128C` 99.93% -> 100% (`m14C4 = max(...); m14C4 *= 1.32f;` en dos sentencias).
  `fn_80181D94` 99.57% -> 99.63% (id del sitio en local; 3 intentos). ctor (push_back), fn_80180AE0 y fn_80181A64
  (registros/pila ya investigados) sin tocar.
  - Hallazgo MWCC: `__MtxRotate` de Vec.h declara `Mtx r` antes que `Mtx t`; varias unidades (ChaserNpc,
    ControlJumpAt, quiza ControlTurnQuadruped 85%) quieren `Mtx t; Mtx r;` y el SetIdentity dentro del mismo inline.
- `game/EnemyTrista` (12/18): saltada; sus pendientes son prologo (ctor), registros del bucle de eventos, pila de
  Side_unk (82%), copia de parametros de camara por FPR (61%, estructura sin reconstruir) y el r5 extra de vf3C:
  todos documentados en batch-auto-10kb-qG2.md.
- `game/EnvSeBase` (35/41, sin cambios): `EnvSeLine::vf14` 99.93% (6 intentos: punto medio en local, color en local,
  factor en local, operandos cambiados, Lerp): pa/pb siguen 4 bytes mas abajo. Resto ya investigado (vf1C 14.7%
  presupuesto de inline; las demas pila/registros).
- `game/GameBgm` (8/14): saltada (caso de inlining abierto del creador al 0% y registros/prologo documentados).
- `game/NewCampPartsGmk` (14/21): saltada; las 6 al 0% son las copias debiles de la copia del estado de escena
  (0x805134F4-0x805143C8), caso de inlining abierto documentado en el propio .cpp.
- `game/NewCampPartsStatus` (23/29, sin cambios): `fn_80528310/454/574` (base + 0x58 recalculado en cada llamada en
  el original) 2 intentos mas (`(char*)task + 0x58`, `&t->mLayout[0]`): MWCC sigue reutilizando la direccion.
  vf14/fn_80527938/vf3C son solo registros (ya investigados). Saltada.
- `game/SongManager` (sin cambios): `fn_805A38F0` 99.77% (4 intentos mas: limite con if/else, local, comparacion
  dentro del inline, puntero a counts[8]); el resto ya investigado (pool 1.0f/0.0f, planificacion del 1.0f).
  Nota: `fnstat.py` muestra fn_805A358C/3660/3734 al 99.81% compilando la unidad suelta, pero el report completo
  las da al 100% (igual que las referencias): no hay perdida.
- `game/SpecialEnemyBase` (14/22, sin cambios): `fn_805A3D68` 99.88% (3 intentos: literal en local antes/despues,
  `find(..., 0)`): el original reutiliza r29 (el `lis` del pool) para ".csv". Resto: pool de cadenas, datos de la
  tabla de nombres, aproximaciones grandes (66%/73%) y registros ya investigados.
- `game/ChaserGuard` (6/13, sin nuevas): `fn_8017F4A0` 99.39% -> 99.86% (frente leido dentro de `GetFrontOf_unk`
  como en ChaserNpc, `base = (2pi * n) / 8.0f`); quedan los slots del frente (8 intentos). `fn_8017FBEC` es el
  r4/r5 de __VecCopy; ctor (push_back), resto registros/pila ya investigados.
- `chr/ControlInteractionBase` (13/21, sin nuevas): los 4 ctores Constrict/BumpHead/StepOn/ThroughRope suben de
  ~93-95% a ~99.6% calculando la rotacion desde el miembro ya guardado (`mChara->mRot = m24.ToRotation_unk();` en
  Setup_unk). Queda solo la forma del ToRotation inline (frsp de z antes del fabs, reutilizado en atan2), que
  necesitaria otra variante en `include/math/Vec.h` (compartida; no tocada). Una copia local del inline cambia
  demasiado (88%).
- **CABECERA COMPARTIDA `include/math/Vec.h` (decision sin el usuario)**: anadidos `Vector3::RotZ_unk()` y
  `Vector3::ToRotationZ_unk()`, otra forma de `fn_80139F60`/`ToRotation_unk` que lee z en un local
  (`float fz = z; Abs_unk(fz) ... atan2(x, fz)`): el original redondea z una vez (frsp) y lo reutiliza para atan2.
  No cambia los inlines existentes (regress/fuzzdrop: 0). Con ella:
  - `chr/ControlInteractionBase` 13/21 -> 17/21: ctores de ControlConstrict, ControlBumpHead, ControlStepOn y
    ControlThroughRope al 100% (`mChara->mRot = m24.ToRotationZ_unk();` en Setup_unk).
  - `game/Em201ZanGurg::vf3C` 99.71% -> 100% (13/15).
  - `chr/ControlWallUp`: ctor de ControlWallSlash 99.61% -> 100%.
  - `chr/ControlDive`: ControlSwatTurn::vf0C 99.52% -> 99.84% (queda r4/r5).
  - Pendiente para la cola C: `game/CopyRobotTask::vf10` (99.8%, mismo sintoma "converts z again") deberia
    cerrarse con `ToRotationZ_unk()`; no tocado (no es de esta cola).
  - Hallazgo MWCC: leer `z` a un `float` local antes del `Abs_unk` hace que el `frsp` se emita una sola vez
    (antes del fabs) y que atan2 reciba el valor ya redondeado (`fmr f2, f3`).
  - Barrido automatico (script en el scratch: cambia cada `ToRotation_unk()` por `ToRotationZ_unk()` y se queda
    solo con los que suben sin bajar nada): ControlOttotto::vf0C 99.33% -> 99.56%, ChaserGuard::fn_8017FED0
    99.32% -> 99.46%. Sin efecto/peor en ControlDive, ControlWallUp, ControlCliffhang, ChaserNpc, SpecialEnemyBase.
