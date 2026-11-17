# escribir-20261116 cola N2: cerca de enlazar (segunda lista)

Inicio: 2026-11-17 04:07 (tope 06:37).

## Tabla de la cola

| Unidad | Funcion | Antes | Despues |
|---|---|---|---|
| RVL_SDK/os/OS | OSInit | 99,97 (ya enlazada) | sin cambio: reubicacion a `__check_pad3` de dtk (cierre99 qF) |
| chr/ControlStagger | ControlSlip::vf0C | 99,92 | |
| dwc/src/base/dwc_transport | DWCi_TransportProcess | 99,81 | |
| Event/MotionData | MovieData::vf08 | 99,81 | |
| gmk/OpenBoard | vf10 | 99,76 | |
| Event/ActionActorBehavior | Sequence::fn_8054FBE0 | 99,77 | |
| atn/PostFxRain | vf10 | 99,72 | |
| gmk/FallingTower | fn_8041044C | 99,73 | |
| game/Em309GhoulSimple | vf108 | 99,61 | |
| game/SelectMember | ctor | 99,64 | |
| db/WeaponParam_unk | boost token_iterator `operator++` | 99,23 | |
| db/MapEnvSeTable_unk | fn_80214F80 | 99,21 | **100 %, la unidad enlaza** |

## Para el port

## Decisiones tomadas sin el usuario
- db/MapEnvSeTable_unk: destructores de `MapEnvSe_unk` y `LoadingEnvSe_unk` definidos `inline` fuera de la clase tras el cargador (como `WeaponParamEntry_unk`): el original emite los dos ctores debiles antes que los dtores.

## Hallazgos
- Registros volatiles = orden de declaracion de los locales: con todos los locales declarados arriba (estilo C), MWCC les da registros crecientes en ese orden; los temporales del compilador (induccion de bucle, `j*8`) van antes. Para un cruce de registros en un bucle, permutar el orden de las declaraciones de arriba (db/MapEnvSeTable_unk fn_80214F80: `se, entry, count, size, i` -> 100 %, tras 3 lotes de variantes de bucle). (escribir-20261116 qN2)
- En una inline los locales de la inline cuentan como temporales (van antes que los del llamador) y entre ellos mandan en orden de declaracion (atn/PostFxRain vf10 con el cuerpo en inline). (escribir-20261116 qN2)
- Pruebas sueltas (try.sh): los registros salvados van r31, r30... en orden de declaracion de los locales, los parametros (y `this`) despues, y las constantes CSE (`lis 0x100; subi`) antes que los locales; un valor salvado de una fase posterior reutiliza el ultimo registro asignado (f29 tras f31/f30/f29), no el primero libre. Los locales de pila van de la direccion mas alta a la mas baja en orden de declaracion, sin compartir huecos entre bloques de un switch. (escribir-20261116 qN2)
- `__string_iterator_unk` con `operator==`/`!=` como plantillas libres de dos tipos por `base()` y `!(a == b)` (forma de libc++ `__wrap_iter`, del mismo autor que MSL C++): quita la recarga de `next` en `char_separator::operator()` (WeaponParam 99,23 -> 99,32). No commiteado: solo sube fuzzy. (escribir-20261116 qN2)
- Orden de los ctores/dtores debiles emitidos (new[] de varias clases): definir el dtor `inline` fuera de la clase, despues de la funcion que lo usa, lo emite despues de los ctores (db/MapEnvSeTable_unk). Una unidad con todo al 100 % que da `main.dol: FAILED`: mirar el orden de los debiles con `nm -n` frente a symbols.txt. (escribir-20261116 qN2)

## Bloqueos
- Orden de declaracion de los locales (todos arriba, 120 permutaciones): sin efecto en dwc_transport DWCi_TransportProcess (C, las 120 iguales), MovieData::vf08, SelectMember ctor (registros salvados), ControlSlip::vf0C (mChara r3/r5, tambien con `Chara_unk* chara` local), PostFxRain vf10 (fullW/fullH/w/h/image: maximo el actual 99,72); FallingTower fn_8041044C (heights/i/a/b/blend: peor, 99,69). (escribir-20261116 qN2)

## Cambios de splits/symbols

## Mediciones
- atn/PostFxRain vf10 (99,72): el cuerpo en dos niveles de inline (`Draw_unk(fullW, fullH)` -> `Draw2_unk(...)`) da los GPR salvados y la pila del original; quedan los volatiles del bucle de escala (exp/first/again r3-r5, original r6-r8, temporales r3-r5): 98,71. Con un nivel, la pila sale invertida (99,36). Orden de declaracion de again/first/exp y un `float* row`: maximo 99,57 (row r3, first r4, again r5, exp r6 ya como el original; faltan la referencia de Max_unk en r3 y el puntero del elemento en r5). Max en inline aparte (98,64), bucle en vf10 (vuelven los GPR). Sin commit: dos niveles de inline no es codigo natural.
- db/WeaponParam_unk `token_iterator` `operator++` (99,23): `operator==` de `__string_iterator_unk` por `a.base() == b.base()` (include/std/string.h) quita la recarga de `next` tras `InputIterator start(next)` (99,32; BattleCharaTask `__pp` 98,19 -> 98,29). Queda start/end en r7/r6 (original r6/r7) en `char_separator::operator()` (boost, no se toca) y en las copias de BattleCharaTask. Probado: miembro/amigo, por valor (94,9), constructor de copia explicito (94,9), `!(a == b)`, `base()`/`operator->` en `replace`, orden de los locales de `find` (24 ordenes, maximo el actual). Sin commit (solo sube fuzzy).
- gmk/OpenBoard vf10 (99,76): orden de declaracion de old/t/from/to en `Interp_unk` (24 ordenes): maximo el actual.
- Event/MotionData vf08: fabrica inline `CreateMovieTask_unk(parent, name)` (como SelectMember::fn_8057162C): los `""` del `new` pasan a la `.sdata` de la inline (97,58): el original tiene el `new` en vf08.
- gmk/FallingTower fn_8041044C: `Height_unk(const float&)` 99,41; `Vector3 pos` local 99,56; matriz local 88,9.
- game/SelectMember ctor: anim local, `int i` fuera del for, list dentro del for (99,40), puntero `anims` (95,3), literal en la llamada (94,8: el addi no se saca del bucle).
