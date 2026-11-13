# Lote paralelo 2026-11-13, cola G (eventos 2)

Inicio: 2026-11-13 03:17 CET. Tope 05:47.

## Resultado por unidad (funciones al 0 % / sin escribir, antes -> despues)

- `Event/Puppeteer_CharaTask`: 19 -> 14 (81 -> 82 al 100 %).
  - `fn_800119B8` (numero de huecos de equipo, `return 8`) 100 %.
  - `Actor::fn_8000EE8C` (actualizacion: pesos de las motions, flag 0x200000 del personaje, equipo pendiente) 99,84 %.
  - `Actor::fn_80011E98` (motion facial) 94,3 %, `Actor::fn_80012054` (motion de un hueco del personaje) 97,1 %,
    `Actor::fn_80012294` (para una pista) 97,6 %: eran `extern "C"` con `self` en ActionActorPosition y Dialog;
    ahora miembros de `Event::Actor` (declarados sin nombres de parametro) y renombrados en `symbols.txt`.

- `Event/ActionActorPosition`: 13 -> 7 (32 al 100 %, sin cambios).
  - `ActionActorSetting::fn_80557E00` (nodo de pelo del tipo de pelo, casos que caen) 99,4 %,
    `ActionActorSetting::fn_80557F84` (activa los huesos fisicos de pelo/pendientes/faldon/pecho segun el modelo
    "pc101".."pc107") 99,1 %, `ActionActorSetting::vf0C` 99,96 %, `ActionActorSetting::vf08` 99,4 %.
  - `ActionActorNode::vf0C` 98,6 %, `ActionActorHeading::vf0C` 98,7 %.
    Los dos `fn_` pasan a miembros de `ActionActorSetting` (declarados sin nombres en
    `Event/ActionProcessors_unk.h`, 4 unidades) y renombrados en `symbols.txt`.

## NONMATCHING

- `fn_8000EE8C` 99,84 %: registros del bucle que engancha el equipo (personaje r28 por r31). Con el bucle en un
  inline que recibe el personaje sale bien el personaje pero se cruzan los del bucle de pesos.
- `fn_80011E98` 94,3 %: el original salta al final si el tipo no es 7/1 (sin pasar por el personaje nulo) y
  planifica distinto los tres bool de `SetMotion`.
- `fn_80012054` 97,1 %: motion y modelo cruzados (r30/r31).
- `fn_80012294` 97,6 %: el original compara m20 con 7, 3 y 1 (un switch sigue el orden de sus casos).

- `fn_80557E00`/`fn_80557F84`: desplazamientos de las cadenas en `@stringBase0` ("eye_R", "reference" y "neck"
  van antes en el original: las usan funciones de Eyes/Pose/Heading/Node sin escribir).
- `fn_80557F84`: el original recorre los huesos desde la direccion del contador (chara + 0x434, como un
  `FixedArray_unk` de contador y array: `atn::Model::m384/m388` serian un solo miembro; cabecera de otra cola).
- `ActionActorSetting::vf0C` 99,96 %: la fase 3 guarda el estado del nodo en r30 (registro de la seccion), el
  nuestro en r28.
- `ActionActorSetting::vf08` 99,4 %: registros float (la constante de las conversiones en f6; clamps del tipo 9).

- `ActionActorNode::vf0C` 98,6 %: el original relee el tamano de la lista de actores en cada busqueda de la
  fase 3 (la primera, sin usar el resultado).
- `ActionActorHeading::vf0C` 98,7 %: huecos de pila de las posiciones de nodos ("neck"/"reference") en orden
  inverso.

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

- `src/Event/ActionActorPosition.cpp` incluye `game/CharaTask.h` y `atn/PBone.h` (re-emparejada con `pd.sh`).

## Hallazgos

- `name.compare("pc101") == 0` en vez de `name == "pc101"` da los huecos de pila del original (un nivel de
  inline menos); `std::string(name, 0, 5)` en vez de `name.substr(0, 5)` (substr no se inlinea aqui).
- Argumentos de una llamada evaluados en orden (param 1 en r4 antes que param 2): pasarlos por locales.
- Cadena `if (p->m04.i == 3) ... else if (p->m04.i == 6)` con `ActionParam_unk* p = GetParam_unk(0)`: deja el
  valor en r0 y el tamano de la lista en r4, como el original (con `int kind` local va a r3).

- Dos lecturas de `m20` sin fusionar en `fn_8000EE8C`: la de `== 6` pasa por el accesor `IsState6_unk()`.
- Un `switch` interior cuyo caso 0 cae al `return 0` final comun (rama nula compartida con default): escribirlo
  como inline con `return` por caso (`GetTrackMode_unk`).

## Pendientes

- `fn_80012054`/`fn_80011E98`: el 0.1f (`0x80880678`) va detras del 0.3f (`0x80880674`), que aun no usa ninguna
  funcion escrita (probablemente `fn_8000F1EC`): poolcheck dara el orden mal hasta escribirla.
