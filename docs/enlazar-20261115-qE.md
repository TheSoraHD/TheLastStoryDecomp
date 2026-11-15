# enlazar-20261115, cola E (personajes y gimmicks a una funcion)

Agente E, rama `agent-E`. Inicio 14:09.

## Resultado por unidad

| Unidad | Antes | Despues | Estado |
|---|---|---|---|
| chr/ControlOneMotion | 8/9 | 9/9 | **enlaza** (ctor de ControlOneMotionTransition, 99,26 -> 100 %) |
| chr/ControlDetect | 6/7 | 7/7 | **enlaza** (ControlDetectHateAtt::vf0C, 99,84 -> 100 %) |
| chr/ControlAppear | 23/24 | 23/24 | bloqueada por datos: `@enum$24281control_appear_cpp` (ver Hallazgos); vf40 sin cambios |
| chr/ControlShoot | 2/3 | 2/3 | ctor 99,90 %: ~18 variantes mas (y wrap greedy de `mChara->` con locales), sin 100 % |
| chr/ControlDraggingStart | 10/11 | 10/11 | DraggedStart::vf10 99,73 %: 6 variantes (cabecera y vf10), sin cambio |
| chr/ControlSitting | 2/3 | 2/3 | vf0C 97,9 %: 8 variantes, sin 100 % |
| gmk/ReinforcePoint | 7/8 | 7/8 | vf10 99,8 %: ~20 variantes; la busqueda de FindRoutePlace2_unk (array leido dentro del bucle, como Em208Queen) baja de 16 a 11 lineas, sin 100 % (no commiteado) |
| gmk/InteractionSpot | 9/10 | 9/10 | Start: 7 variantes, sin 100 % |
| gmk/FallingTower | 12/13 | 12/13 | fn_8041044C 99,7 %: 5 variantes, sin 100 % |
| gmk/CharaLight | 14/15 | 14/15 | vf6C 96,3 %: 4 variantes, sin 100 % |
| gmk/HealPoint | 14/15 | 14/15 | fn_8040ED00 93,8 % (4 lineas de planificacion): 4 variantes del ctor inline, sin cambio |
| gmk/ShakeCamera | 8/9 | 8/9 | vf10 96,7 %: 3 variantes del case 1, sin 100 % |
| gmk/LandMine, gmk/BulletPoint, gmk/OpenBoard, gmk/FlyObjManager, gmk/AutomaticDoor | - | - | no atacadas a fondo (diffs de pila/registros float ya muy probados) |

## Unidades que enlazan (bytes)

- chr/ControlOneMotion: 500 bytes de datos (`total_data`), commit 47296d3.
- chr/ControlDetect: 336 bytes de datos, commit 4601a80.
- Total: **836 bytes**, funciones 15551 -> 15553.

## NONMATCHING con motivo

- chr/ControlShoot ctor (99,90 %): el cero de mFB0/mFB4 sale en r6 (original r4). Probado ademas: `+=`,
  `mFB4 + m48`, local `add` antes del `mFB0 = 0` (da el r4 del cero pero adelanta la carga de m48),
  `data->m48`, punteros locales `c1/c2/c3` por sentencia, `(unsigned int)`, temporal para el fctiwz.
- chr/ControlDraggingStart DraggedStart::vf10: la copia del bind en el heap reutiliza el registro de
  `motion` (r29) en vez del de `next` (r30). Probado: ctor de ControlDragged con mChara/referencia/
  `Init_unk()`, `ControlBase*`, `boost::function` con nombre, `next` sin inicializar.
- chr/ControlAppear vf40 (98,8 %): r3/r4/r5 permutados (chara/temporal/entrada); probado orden de los
  locales, referencia a la entrada, temporal `ofs`, indice local, parentesis.
- gmk/InteractionSpot Start: probado puntero `s`, `mSpots + i`, `found` + `break`, sin local `table`,
  `spot` local, `id` local.
- gmk/FallingTower fn_8041044C: probado `sin * mAmp`, angulo local, `sinf`, tiempo local, cero local.
- gmk/CharaLight vf6C: probado `group = 0` inicial, ternario (peor: materializa el booleano), asignacion
  separada, puntero `state`.
- gmk/HealPoint fn_8040ED00: probado `const GimmickDesc_unk*`, referencia/float locales, m78 por
  componentes, temporal `rot`.
- gmk/ShakeCamera vf10: `return` en case 1 (27 lineas en vez de 28), `if (mWait < mWait)`.
- gmk/ReinforcePoint vf10: con FindRP2_unk quedan id/places (r3/r4) y las direcciones de `to`/`from`/global cruzadas; probado ademas VecAssign_unk en to/from, referencia `pos`, `id` local, ternario, if/else, `unsigned` id, `places + i`. Antes: `desc` local, `table = 0` y luego asignada, `id` local, `to`/`from` sin ctor.
- chr/ControlSitting vf0C: `GetMotion_unk(mChara)` inline (32 lineas en vez de 35; mismo cruce r30/r31),
  `self`, `--mCount` en la expresion, punteros locales en las copias.

## Cambios de splits

Ninguno.

## Para el port

Nada (ninguna unidad del port tocada).

## Decisiones tomadas sin el usuario

- chr/ControlOneMotion: el inline comun `ResetMove_unk` usa un local `c3` para la copia de m574 (los dos
  ctores que lo usan siguen al 100 %).
- chr/ControlDetect: `1.0f / fn_80097D7C(0)` en un local `inv` y un local `c = mChara` para la copia de
  mPos.

## Hallazgos (una linea cada uno)

- Cruce r4/r5 (o r3/r4) en la copia de un `Vector3` a un miembro de mChara: un local `Chara_unk* cN = mChara;` justo antes de **cada** sentencia que escribe (no uno compartido) da el orden del original (ControlOneMotion, ControlDetect).
- En ControlDetect hizo falta ademas sacar `1.0f / f()` a un local antes del puntero: el local solo, o el puntero solo, no bastan.
- chr/ControlAppear no puede enlazar: el nombre de tipo de boost::function en el `.dol` es `@enum$24281control_appear_cpp` (fichero original `control_appear.cpp`, contador 24281 frente a ~7500 en el nuestro); haria falta renombrar la unidad y reproducir el contador.

- `tools/decomp/asgwrap.py` no mejora ControlAppear vf40, ReinforcePoint vf10, ControlSitting vf0C, LandMine Start ni InteractionSpot Start.

## Pendientes

- chr/ControlAppear: renombrar a `chr/control_appear.cpp` no basta (el contador del enum anonimo es 24281); decidir si se acepta como no enlazable.
