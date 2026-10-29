# enemies-third, cola n3b

Unidades: `game/Em203GreaterDaemonRed` (0x80293738, 53 funciones) y `game/Em203GreaterDaemonRedMp` (0x8029CB00, 63 funciones).

## Resultado

| Unidad | Antes | Después | Enlazada |
|---|---|---|---|
| `game/Em203GreaterDaemonRed` | 0/53 | 27/53 | no |
| `game/Em203GreaterDaemonRedMp` | 0/63 | 0/63 (sin empezar en el árbol) | no |

Al cerrar: `main.dol: OK`, `regress.py` lost: 0, `fuzzdrop.py` 0 bajadas, `poolcheck.py game/Em203GreaterDaemonRed` sin avisos.

RedMp: el lote se cerró antes de escribir el fuente. El análisis del asm y un primer borrador de la cabecera (layout completo de 0x14D4 a 0x16CC, obtenido del ctor, vf0C, el destructor y las funciones de estado) quedan en `/home/claude/scratch/n3b/redmp_wip/` (fuera del repo). Resumen para retomarla:
- Deriva de `SpecialEnemyBase`; vtable 0x80785B38, virtual nueva en el slot 0x11C (cápsula, 0x8029E0B4); `vf108` y `fn_802A49C8` (¿está volando?) son inline al final del fichero; destructor inline (0x802A4A04).
- Ayudantes repetidos: ResetState (`m14D8`, `m14DC`, `m590`, `m155C`, `m15FC`), PlayMotion (como en Red), ToIdle (estado 6 + `fn_80239DAC(-2)`), StartState13 (motion 0x153), muerte (mismo bloque en `vf40` y `vf114`).
- `fn_802A4968`/`fn_802A49BC` son las `extern "C" inline` de `game/EnemyJill.cpp` (teclado de depuración); `fn_802A36B0`/`fn_802A4094` son las de `gmk/Cannonball.cpp`.
- Las cadenas de 0x28F a 0x2E6 del pool (`em203_GreaterDaemon`, `DebugDisp`, `AttackedMax`, `GunPerAttack`, `Ground*Time`) son de un menú de depuración que el enlazador quitó, entre `fn_802A36B8` y el final: hará falta una función stub, como `Stripped*_unk` en Red.
- Caso 19 de `SpecialEnemyBase.cpp` sigue con el `extern "C" fn_8029CB00`.

## NONMATCHING de Red (26)

Todos llevan `// NONMATCHING: X% - motivo` en el fuente. Por tipo:
- ctor 99,85%: el intercambio de registros conocido del segundo `std::string` temporal (igual que en Em203GreaterDaemon).
- `fn_80298850` y `fn_80298E98` (~98,8%): la carga de `mRot.y` se programa antes de guardar la posición del nodo.
- `fn_8029A788` y `fn_80296DB8` (99,8%): cruce de registros en la copia psq a `m1530` (bloqueo conocido).
- Funciones de cámara (`fn_802992F8`, `fn_802995D4`, `fn_802997B8`, ~98,8–99,2%): orden r4/r5 de los stores del look de cámara (conocido).
- Huecos de pila de los temporales de `ToRotation_unk`: `fn_8029A928`, `fn_8029AC1C`, `fn_8029AF6C`, `fn_8029B24C`, `fn_80296B8C`, `fn_80299A7C`, `vfF8` (77%).
- Funciones grandes con `push_back` inline o mucha asignación de registros: `vf10`, `fn_80296268`, `fn_8029795C`, `fn_8029B704`, `fn_8029BE7C`, `fn_8029A0A8`, `fn_80297114`, `fn_80297474`, `fn_80298B10`, `vfEC`, `fn_8029C564`.

## Decisiones tomadas sin el usuario (revisar)

- `include/game/CharaTask.h`: `virtual void vf70()` pasa a `virtual float vf70()`. La base (0x800E8FA4) devuelve un valor en f1 y Red lo redefine como `return 100.0f`. No baja nada.
- `include/game/CharaStatus.h`: `RoutePoint_unk::m10[...]` pasa a `int m10`. Es el id de punto que compara Red en 0x80296DB8. No baja nada.
- `include/std/vector.h` (**conflicto previsto con la cola s5, que también amplió este fichero**): añadida la especialización `__vec_storage_unk<float>::__shrink_unk`, sin bucle de destructores y con el tamaño restado por referencia. Hacía falta para que los destructores de las listas de floats (`StunHPRate`/`RecoverHPRate`) salgan como en el original.
- `src/game/SpecialEnemyBase.cpp`: el caso 18 usa `new (3, "", "", 0) Em203GreaterDaemonRed(parent, info)`, se quita el `extern "C" fn_80293738` y se incluye la cabecera.
- Funciones stub `Stripped1_unk`/`Stripped2_unk` en Red: las quitó el enlazador, pero sus constantes (4.0f, 2.0f y 100.0f) siguen en `.sdata2`, entre las de `vf64`/`fn_80296268` y las de `vfE4`/`vfE0`. Así el pool sale en el mismo orden (precedente: los Stripped de CharaTask).
- Relleno `ASSERT_UNK` (`((void)0)` ×6) en `fn_802953C8`, `fn_8029B59C`, `fn_8029A788`, `fn_8029A674` y `fn_8029C564`, para que se llamen fuera de línea como en el original. Es el idioma aprobado.
- Instanciación explícita `template void std::vector<RoutePlace_unk*>::push_back` tras `vf0C`. Coloca "vector length error" en el pool justo detrás de las claves del CSV.

## symbols.txt (solo renombrados)

- Ctor, dtor, `vfXX` y todos los `fn_` miembros de Red con su nombre decorado (`fn_XXXXXXXX__21Em203GreaterDaemonRedF...`).
- `fn_800DB3EC` → `push_back__Q23std9vector<f>FRCf`.
- `lbl_80745818` → `@stringBase0` (local, string_table).
- `lbl_80785AE0` → `@STRING@vfC8__21Em203GreaterDaemonRedFv` (local).

## Hallazgos

- Con `-ipa file`, una virtual inline definida fuera de la clase no mueve sus constantes de `.sdata2`. Para ordenar el pool hay que usar stubs de funciones quitadas por el enlazador.
- Comprobar el fin de motion con un `bool` inline da `mfcr`. Con un local `float end = frame; if (end >= fn_80097D7C(0))` sale la rama directa del original.
