# close-one-func-2 (2026-10-30)

Objetivo: cerrar unidades sin enlazar a las que les falta una sola función (≥ 95%), de menor a mayor
tamaño (lista de `SIGUIENTE_sesion.md`). Trabajo secuencial, rama `close-one-func-2` desde `main` 29b705e.

Resultado: 8768 → 8770 funciones, 378 → 380 unidades enlazadas (`game/MapPartsEfp`, `game/Gun`),
`main.dol: OK`, lost 0, sin bajadas de fuzzy, `poolcheck.py` sin avisos nuevos.

## Por unidad

| Unidad | Resultado | Clave |
|---|---|---|
| `game/MapPartsEfp` | **enlazada** | `fn_8023A8B4` lleva un `float scale` final (1.0f en f1), como en los gimmicks: la declaración local sin él cambiaba los FPR de 0.0f/1.0f. Destructor fuera de línea tras el constructor (en la cabecera se emitía al final del `.o`). Resto de declaraciones de `fn_8023A8B4`: todas con 11 parámetros. |
| `game/Gun` | **enlazada** | Constantes de la traslación en locales declarados `x, y, z` y asignados `z, y, x`: da el orden de `.sdata2` (3.7, 0.5, 0.0) y los registros. Declarados y asignados en el mismo orden: 99,95% y pool al revés. |
| `game/IffChunkFile_unk` | 98,97 → 99,25% | Valor de `*m18` en un local. Quedan los registros de los intercambios in situ (valor/dirección). La forma en C con máscaras `(v<<24)\|((v>>8)&0xFF00)\|(v>>24)\|((v<<8)&0xFF0000)` da el mismo código que `__rlwimi` pero peor reparto aquí (99,0%). |
| `game/TownTpcLoader` | 99,54 → 99,59% | La llamada es `CharaTask::fn_8016E970` (no un `extern "C"`). Queda r29/r30 del último bucle. |
| `atn/CsvBase_char_124_13_34` | 99,48% (sin cambio) | r24/r26 de fila y cadena de niveles. |
| `db/MemberList_unk` | 98,39% (sin cambio) | r4/r5 de `count - 1` y su desplazamiento. |
| `chr/ControlSitting` | 95,61 → 97,88% | Lerp en inline `(b - a) * t + a` (como `ControlCliffhang`/`ControlOneMotion`): la aritmética casa. Quedan `this` r31/`motion` r30 y el hueco de `dir` en la pila. `atn::WrapAngle_unk` en lugar de la copia local. |
| `game/MapPartsRain` | 99,84% (sin cambio) | `box.mMin = a; box.mMax = b; mBox = box;` da la forma exacta (escritura por dirección propia, lectura por `box + 0xC`) pero las cuatro direcciones salen intercambiadas por pares (99,77%); no se dejó. |
| `game/Em209BigGolem` | 99,70% (sin cambio) | r3/r4 de la copia de la posición del nodo (familia `Em208Queen::vf20`). |
| `Event/EventLoader_unk` | 95,17 → 98,99% | `Get_unk(unsigned int)` da el orden contador-luego-puntero de los incrementos; `cut` declarado antes del bucle deja los contadores en r31..r28. Quedan los volátiles de los punteros (cut r9, group r10, track r11 en el original). |
| `atn/Texture_unk` | 99,69% (sin cambio) | Escribir el cuerpo de la liberación directamente en `fn_800D5B58` (un nivel de inline menos que vía `fn_800D58A4`) arregla el r31 del tamaño (99,83%), pero no el orden de carga ancho/alto; no se dejó (código duplicado). |
| `db/ArrowSet_unk` | 99,22% (sin tocar) | Residuo conocido del `push_back` en bucles (`docs/msl_cpp.md`). |
| `chr/ControlDraggingStart` | 99,62% (sin cambio) | r29/r30 de la copia del bind en el montón; mismo residuo que `ControlCdnSleep::vf10`. |
| `atn/LightUtil_unk` | 98,73% (sin cambio) | Un accesor de tamaño en la comprobación da la recarga de `mCount` del original, pero mueve los registros del bucle (95,6%). |

## Decisiones tomadas sin el usuario

- No se dejaron variantes que solo cambian el reparto sin acercarse (`MapPartsRain` 99,77%, `Texture_unk` con el cuerpo duplicado): anotadas en el `NONMATCHING`.
- Se commitearon las subidas parciales que corrigen tipos o llamadas (`CharaTask::fn_8016E970`, lerp, `Get_unk(unsigned)`) junto con las notas, en un commit aparte de los matches.

## Hallazgos (también en `docs/trucos-mwcc.md`)

- Floats en locales con nombre: el orden de **asignación** decide el pool `.sdata2` y el de **declaración** los registros (`Gun::vf14`).
- Al ver FPR de constantes intercambiados en una llamada, mirar si la firma real tiene un `float` más (la constante del argumento ocupa f1).
- `Get_unk(unsigned)` en un accesor con comprobación de rango: contador antes que el puntero en los incrementos del bucle.
