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
    `lbl_80730F08` (8 bytes, aparte de `@stringBase0`), lo que apunta a otra TU, y pasa un quinto argumento (r8) a
    los ctores de 4 parametros. Pendiente (necesitaria partir el split de Arms en 0x800457A4).
  - Hallazgo MWCC: `x = k * x` y `x = x * k` (float) reparten distinto los registros volatiles en una secuencia de tres.
