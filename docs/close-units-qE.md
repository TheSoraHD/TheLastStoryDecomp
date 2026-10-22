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
