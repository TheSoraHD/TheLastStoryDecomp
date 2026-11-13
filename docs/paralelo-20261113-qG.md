# Lote paralelo 2026-11-13, cola G (eventos 2)

Inicio: 2026-11-13 03:17 CET. Tope 05:47.

## Resultado por unidad (funciones al 0 % / sin escribir, antes -> despues)

- `Event/Puppeteer_CharaTask`: 19 -> 14 (81 -> 82 al 100 %).
  - `fn_800119B8` (numero de huecos de equipo, `return 8`) 100 %.
  - `Actor::fn_8000EE8C` (actualizacion: pesos de las motions, flag 0x200000 del personaje, equipo pendiente) 99,84 %.
  - `Actor::fn_80011E98` (motion facial) 94,3 %, `Actor::fn_80012054` (motion de un hueco del personaje) 97,1 %,
    `Actor::fn_80012294` (para una pista) 97,6 %: eran `extern "C"` con `self` en ActionActorPosition y Dialog;
    ahora miembros de `Event::Actor` (declarados sin nombres de parametro) y renombrados en `symbols.txt`.

## NONMATCHING

- `fn_8000EE8C` 99,84 %: registros del bucle que engancha el equipo (personaje r28 por r31). Con el bucle en un
  inline que recibe el personaje sale bien el personaje pero se cruzan los del bucle de pesos.
- `fn_80011E98` 94,3 %: el original salta al final si el tipo no es 7/1 (sin pasar por el personaje nulo) y
  planifica distinto los tres bool de `SetMotion`.
- `fn_80012054` 97,1 %: motion y modelo cruzados (r30/r31).
- `fn_80012294` 97,6 %: el original compara m20 con 7, 3 y 1 (un switch sigue el orden de sus casos).

## Decisiones tomadas sin el usuario

- `include/Event/Actor.h` (33 unidades): tres miembros nuevos `fn_80011E98`, `fn_80012054`, `fn_80012294`
  sin nombres de parametro (no renumeran). Llamadores actualizados: `Event/ActionActorPosition.cpp`
  (tambien `fn_80012540`, que ya era miembro y se llamaba con un `extern "C"`) y `Event/Dialog.cpp` (otra cola,
  solo el llamador).
- `src/Event/Puppeteer_CharaTask.cpp` incluye `Event/Action.h` y `chr/CharaEquip_unk.h`: renumera sus `@N`;
  re-emparejadas con `pd.sh` y `@14888`/`@14889` (0.0f/1.0f de .sdata2, que `pd.sh` no renombro) a mano.
- `Actor::m148` (IntVector_unk) guarda punteros `chr::CharaEquip_unk*`: se convierten en el uso (cambiar el tipo
  cambiaria el destructor debil).
- `IsState6_unk` (declarada en Actor.h, definida en ActionActorPosition) definida tambien en Puppeteer_CharaTask.

## Hallazgos

- Dos lecturas de `m20` sin fusionar en `fn_8000EE8C`: la de `== 6` pasa por el accesor `IsState6_unk()`.
- Un `switch` interior cuyo caso 0 cae al `return 0` final comun (rama nula compartida con default): escribirlo
  como inline con `return` por caso (`GetTrackMode_unk`).

## Pendientes

- `fn_80012054`/`fn_80011E98`: el 0.1f (`0x80880678`) va detras del 0.3f (`0x80880674`), que aun no usa ninguna
  funcion escrita (probablemente `fn_8000F1EC`): poolcheck dara el orden mal hasta escribirla.
