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
- Orden de los ctores/dtores debiles emitidos (new[] de varias clases): definir el dtor `inline` fuera de la clase, despues de la funcion que lo usa, lo emite despues de los ctores (db/MapEnvSeTable_unk). Una unidad con todo al 100 % que da `main.dol: FAILED`: mirar el orden de los debiles con `nm -n` frente a symbols.txt. (escribir-20261116 qN2)

## Bloqueos

## Cambios de splits/symbols

## Mediciones
