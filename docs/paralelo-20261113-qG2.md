# Lote paralelo 2026-11-13, cola G2 (eventos 2, segunda pasada)

Inicio: 2026-11-13 07:19 CET. Tope 09:49.

Total: funciones al 0 % (sin escribir) de la cola 18 -> 6 (Puppeteer_CharaTask 14 -> 6, ActionActorPosition 4 -> 0);
las 6 que quedan son copias debiles de libreria o de otras cabeceras (`Model::CalcMatrixStack`, destructor de
`SimpleVector<PoseKey_unk>`, `atn::Task::vf18`, `basic_string::reserve/do_replace`, `dtor_80014250`).
Nuevas al 100 %: `fn_80010B68`, `fn_800119C0`, `PoseKey_unk::PoseKey_unk`, `fn_80012AC4`, `fn_8001011C`,
`Puppeteer<atn::Model>::vf10` (Puppeteer_CharaTask 82 -> 88/111). `report.json` 15314 -> 15320, lost 0, sin
bajadas. Ninguna unidad pasa a enlazar. 11 commits.

## Resultado por unidad (funciones al 0 % / sin escribir, antes -> despues)

- `Event/Puppeteer_CharaTask`: 14 -> 6 (82 -> 88 al 100 %).
  - `Actor::fn_80010B68` (rotacion del actor; para modelo y dummy, del eje z de su matriz) 100 %: metodo
    `const` (renombrado en `symbols.txt` a `fn_80010B68__Q25Event5ActorCFv`), punteros locales por caso,
    `GetRow_unk(2).ToRotationZ_unk()` y `operator-` inline (como en gmk/Cannonball).
  - `Actor::fn_80010490` (pone la rotacion; modelo y dummy conservan la traslacion) 97,8 % y
    `Actor::fn_80011420` (rotacion en y) 99,2 %: el mismo cuerpo en el inline `Actor::SetRotation_unk`
    (el original lo inlinea en las dos).
  - `Actor::fn_800119C0` (motion de un gesto, Event::BehaviorGesture) 100 %: el getter del modelo como inline
    `Actor::GetModel_unk` (mismo codigo que fn_8001296C, con `?:` en el caso 2) y la pista en una local con
    `case 1: t = track;` (si no, el caso 1 pierde su comparacion).
  - `Actor::fn_80013558` (copia los colores de material guardados y los vuelve a poner) 97,7 %.
  - `Actor::fn_8000F1EC` (inicio del evento: guarda en m18/m24 el estado del personaje y lo toma) 96,3 %; con el
    cast a `SimpleVector<Event::PoseKey_unk>` sale tambien `PoseKey_unk::PoseKey_unk` (0x8000FAE4) al 100 %.
  - `Actor::fn_8000FAE8` (fin del evento: devuelve al personaje el estado guardado en m18/m24) 92,9 %.
  - `Puppeteer<atn::Model>::vf10` (stub con DRAFT) -> 100 %: pone el modelo sobre la matriz m54 y calcula su pila
    de matrices en los estados 10-12; el estado 11/12 como bool en un inline (`IsPlayState_unk`, `bool r = true;
    if (...) r = false;`), el concat por `GetTarget_unk()`. `SetModelMtx_unk` y sus inlines movidos antes.
  - `Puppeteer<CharaTask>::vf10` (parcial, 20 %) -> 93,0 %, escrita entera: pone al personaje sobre la matriz
    m54 (con su posicion salvo mB8, su rotacion salvo mB9) por m58 en m88 (`CharaTask::mF1C`), llama a
    `fn_80145334` y suelta la matriz. Rotacion y,x,z conservando la traslacion en el inline `SetRotYXZ_unk`
    (matriz por un puntero, como `Matrix3x4::RotateZYX_unk`).
  - `Actor::fn_80012D34` 99,81 -> 99,98 % (sin DRAFT): la diferencia de la cadena de `GetTrans` en una local; solo
    queda el desplazamiento de "eye_R" en `@stringBase0` (el pool original 0x8072EE0C tiene tambien los nombres
    de parametros de las acciones).
  - `Actor::fn_80012AC4` (matriz de un nodo del actor) 99,6 -> 100 %: metodo `const` (renombrado a
    `fn_80012AC4__Q25Event5ActorCFUi`; llamador Event/ActionEffectPlay sin cambios en el fuente).
  - `Actor::fn_8001011C` (mueve el actor) 99,2 -> 100 %: columnas de `SetModelMtx_unk` por `MtxCol_unk`,
    evaluadas z, y, x, y el modelo por `Puppeteer::GetTarget_unk()` en vez de `m48`.

- `Event/ActionActorPosition`: 4 -> 0 (34 al 100 %, sin cambios).
  - `ActionActorNode::vf08` (mueve y gira un nodo del modelo con fundidos; acumula los offsets por nodo en
    m118/m124) 96,1 %.
  - `ActionActorPose::vf0C` (aplica una pose: las claves de nodo de la pose sustituyen o se anaden a una copia
    de las ediciones guardadas por el actor) 97,9 %.
  - `ActionActorHeading::vf08` (gira la cabeza: hacia los angulos de los parametros 5-7 desde el cuello o hacia
    el nodo del actor objetivo desplazado por 2-4; transicion por la curva, parada en el fundido de salida)
    97,1 %.
  - `ActionActorEyes::fn_80554088` (ojos fuera de la ventana de vagabundeo: hacia los angulos de los
    parametros 6-8 o hacia el nodo del actor objetivo desplazado por 2-4, con transicion por la curva) 93,7 %.

## NONMATCHING

- `Puppeteer<CharaTask>::vf10` 93,0 %: registros salvados (r27-r31 aqui), el bloque de m54 colocado tras la
  llamada y las copias de las traslaciones por temporales de pila.
- `fn_80010490` 97,8 %: el original guarda this/rot en r28/r29 y la matriz y el modelo del caso 3 en r31/r30
  (aqui comparten registro); matriz del caso 3 arriba del marco.
- `fn_80011420` 99,2 %: huecos de las matrices de rotacion (0x30 mas arriba aqui).
- `fn_80013558` 97,7 %: el `push_back` inline de `std::vector<ColorF_unk>` (std/vector.h, otra familia: el
  original mueve los elementos con un bucle contado) y huecos de pila de los dos colores (invertidos).

- `fn_8000F1EC` 96,3 %: registros (this en r29 en el original), huesos fisicos desde la direccion de su
  contador, y el original relee la palabra de flags del personaje para borrar mFlag3/mFlag5.
- `fn_8000FAE8` 92,9 %: el original relee m18 en cada bit guardado (aqui queda en registro), guarda r28-r31 uno a
  uno (un registro mas aqui), recorre los huesos fisicos desde la direccion de su contador
  (`atn::Model::m384/m388` como un array) y copia los offsets de la camara con un temporal de 0x1C bytes
  (`GameCamera::m93C/m948/m954`); "z" en otro sitio del pool de cadenas.
- `ActionActorEyes::fn_80554088` 93,7 %: marco 0x10 menor aqui (huecos de los vectores), registros volatiles de
  las dos busquedas de actores y orden de las cargas de `arg->m18` y del gestor de eventos.

- `ActionActorNode::vf08` 96,1 %: planificacion de las conversiones de los tiempos de fundido y de los grados, y
  registros de las busquedas de claves.
- `ActionActorPose::vf0C` 97,9 %: registros de las dos busquedas y el bucle de copia de las claves guardadas.
- `ActionActorHeading::vf08` 97,1 %: marco 0x10 mayor aqui (huecos de los vectores y del offset convertido) y
  registros del indice y del id en las dos busquedas de actores (r4/r5).

## Decisiones tomadas sin el usuario

- `include/Event/Actor.h` (33 unidades): `fn_80010B68` y `fn_80012AC4` pasan a `const` y miembro nuevo
  `inline void SetRotation_unk(const atn::Vector3&)` e `inline atn::Model* GetModel_unk()` sin nombre de
  parametro (no renumeran). Llamadores de
  `fn_80010B68` (Event/Path, Event/ActionActorPosition) sin cambios en el fuente; el nombre en `symbols.txt`.
- `@15509`/`@15510` (0.0f/1.0f de `.sdata2` de Puppeteer_CharaTask) renombrados a mano en cada re-emparejado
  mientras ninguna funcion escrita los usaba primero (pd.sh no los renombraba): ahora `@17892`/`@17893`; con
  `Puppeteer<CharaTask>::vf10` escrita `pd.sh` ya los renombra solo.
- `include/Event/Actor.h`: `inline void SetRotation_unk` y `inline atn::Model* GetModel_unk` son miembros nuevos
  (declarados sin nombre de parametro, definidos en Event/Puppeteer_CharaTask.cpp).

- `Event/Actor.h`: bits de `m18` (MSB primero) detallados: `m18_0` estado guardado, `m18_8` (8 bits) modo de
  control, `m18_16..22` bits guardados del personaje; `ActorAnim_unk::m0C` int. Las dos posiciones guardadas
  (`m24 + 0x10/0x1C`) quedan como bytes (`m10[0x18]`) leidas con `SavedVec_unk` (reinterpretacion): como
  miembros `atn::Vector3` el constructor implicito del actor renumera las `@N` de Event/Sequence y
  Event/EventParser_unk (cola F). Cambiarlo junto con `ActorAnim_unk::m00` (pendiente).
- `Event/Actor.h`: `ActorKey10_unk` pasa de `{Vector3 m00; float m0C}` a `{int m00; Vector3 m04}` (indice de
  nodo y offset, ActionActorNode::vf08); nadie mas usaba sus campos (precommit sin renumerar).
- `ActionActorPose::vf0C` copia `actor->m24.m00` con `Resize_unk` y un bucle (el original no pasa por
  `clear()` de `Assign_unk`; `SimpleVector` no tiene constructor de copia en `atn/Array_unk.h`, otra familia).
- `fn_8000FAE8` pasa `m24.m00` a `atn::Model::fn_80094420` como `SimpleVector<Event::PoseKey_unk>` con un cast
  (mismo layout que `ActorKey2C_unk`; pendiente del cambio de `ActorAnim_unk::m00`).
- `src/Event/Puppeteer_CharaTask.cpp` incluye `atn/PBone.h` (renumera sus `@N`, re-emparejada).

## Hallazgos

- Primera carga de `m20` antes de guardar los FPR en el prologo: metodo `const`; con `const`, `if (m1C != 0)`
  se relee: usar un puntero local por caso.

- Un `case` vacio de un `switch` que el original compara: asignar una local en ese caso (`case 1: t = track;`
  con la misma asignacion en `default`) mantiene la comparacion.
- `fn_80013558`: con `fn_8000DD04` (12 asserts vacios) el presupuesto de inline rechaza todo; con
  `GetRes_unk()` lo inlinea todo como el original.

- Las `@STRING@Resize_unk__...` de `.sdata` son cadenas vacias (los argumentos de nombre de `operator new[]`):
  solo cuenta el nombre de la etiqueta (el tipo del `SimpleVector`).

- Leer el objetivo de un `Puppeteer` por `GetTarget_unk()` en vez de `m48` cambia los registros de lo que sigue
  (`fn_8001011C` 99,6 -> 100 %); crea una instancia nueva de la plantilla (renumera las `@N`: `pd.sh`).

- Orden de `@stringBase0`: las cadenas de una inline se ponen en el pool detras de las de la funcion que la
  llama (en `fn_80557F84`, con la deteccion "pc101".."pc107" en una inline, "earring_L".."bust_R" pasan delante
  como en el original, pero la funcion baja de 99,1 a 94 %: no se aplica; pista para quien la retome).
- Escribir las funciones de Eyes/Pose/Heading/Node pone "eye_R", "reference" y "neck" en su sitio:
  `ActionActorSetting::fn_80557E00` da 100 % (su comentario de NONMATCHING por el pool sigue: queda el orden
  "pc10x" frente a "earring_L".."bust_R" de `fn_80557F84`).

## Pendientes

- `ActionActorSetting::fn_80557F84` 99,1 %: orden de las cadenas "pc101".."pc107" en el pool (ver hallazgos).
- `ActorAnim_unk::m00` como `SimpleVector<Event::PoseKey_unk>` y `m10/m1C` como `atn::Vector3` (lote con
  `pdbatch.sh` de todas las unidades que incluyen `Event/Actor.h`): quitar los casts de `fn_8000F1EC`,
  `fn_8000FAE8` y `SavedVec_unk`/`SavedVecRef_unk`.
