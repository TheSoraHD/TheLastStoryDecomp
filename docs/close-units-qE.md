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
