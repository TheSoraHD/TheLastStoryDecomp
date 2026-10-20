# splits-auto, agente B (rango .text 0x800F7250 - 0x8033C3D8)

Herramientas propias usadas (scratchpad, no en el repo): vista por funcion de constantes duplicadas
(`cc.py`), propietario de cada objeto de datos = unidad que contiene todas las funciones que lo usan
(directa o via vtable/tablas) (`gen.py`), y aplicacion de bloques a `splits.txt`.

## Grupo 1 (prioridad 1 + 0x800F7250)

| Tramo | Unidad | Prueba |
|---|---|---|
| 0x800F7250-0x8010D98C | `game/BattleTask.cpp` ampliada (antes acababa en 0x800F7250) | Las 191 funciones usan las mismas copias de `.sdata2` que BattleTask (0.0f `lbl_80881478`, 1.0f `lbl_80881494`, 1.5707964, 1e16...): ninguna constante duplicada cambia de copia entre 0x800F1430 y 0x8010D98C; la `.sdata2` propia sigue sin hueco hasta 0x808815F8 (BehaviorFollow). `.rodata` 0x807355C8-0x80736040 (tablas de SE, double int->float `lbl_80735EB0` usada por ambas partes). |
| 0x80116EB8-0x8011770C | `game/MenuCommon_unk.cpp` (nuevo, nombre inventado) + `.ctors` 0x8072D2D0 | `__sinit` 0x801175EC al final; 1.0f propia (`lbl_80881720`) distinta de la de Event/Path; funciones de sonido de menu (`fn_80117228` = sonido de sistema SE_UICOM_*), tabla de stats (atk_/def_/agi_...). |
| 0x8011BD14-0x8011D24C | `game/CharaC58_unk.cpp` (nuevo, nombre inventado por el struct `CharaC58_unk`) | 0.0f/1.0f/3.0f propios (0x80881798...) distintos de CampSaveLoad; termina donde -1.0f y 1.0f cambian de copia (0x808817BC->0x808817C0, 0x8088179C->0x808817C4). Contiene ctor/metodos de `CharaC58_unk` y `CharaD74_unk`. |
| 0x8011D24C-0x8011F6B0 | `game/CharaFaceControl.cpp` (nuevo) | Cambio de copia de constantes en 0x8011D24C; cadena RTTI de boost::function `bind_t<..., CharaFaceControl, atn::MotionBuffer*, int>` (0x80736EF0) usada aqui. Clase principal sin vtable: `Chara1188_unk` en el repo = probablemente `CharaFaceControl`. |
| 0x80204958-0x80207C00 | `atn/CsvBase_char_124_13_34.cpp` ampliada hacia atras (+23 funciones) | Ya decia el plan: el fichero real empieza en 0x80204958 (tras Event/MotionData). Mismo `.rodata` contiguo hasta la vtable/RTTI de CsvPipe y CsvBase<char,'|'>; pool `""` 0x8073EA48 al final. Cargadores de CSV de /boot (ai_table, weapon_param, weapon_effect, arrow_set, battle_eff_se_table). |
| 0x80263CF0-0x80264DB4 | `game/Em201ZanGurg.cpp` ampliada | Mismas copias (0.1, 1.0, 0.0 en 0x808835F8...) que Em201ZanGurg; la `.sdata2` 0x80883638-0x80883670 solo la usan estas funciones. Corte con ZanGurgFinal en 0x80264DB4: todas las constantes cambian de copia. |
| 0x80267F88-0x8026FC0C | `game/Em201ZanGurgFinal.cpp` ampliada | Mismas copias (0x80883670..) que ZanGurgFinal; funciones finales 0x8026FBB8/0x8026FBD4 en la vtable de Em201ZanGurgFinal. Datos: tabla de cadenas op_* (0x807444D0-), `lbl_80744608`, jumptables 0x80784D28-0x80784DD8. |
| 0x8031B09C (`__sinit`, 0xC) | `game/Em316LaserSphere.cpp` ampliada + `.ctors` 0x8072D350 | `__sinit` usa `lbl_80884DE8` (sdata2 de LaserSphere): ultima funcion de LaserSphere. |
| 0x8031CA30-0x8031EB3C | `game/Em316LastLaser.cpp` ampliada (incluye `LaserTrace`) | Mismas copias (0.0f 0x80884DF0, 1.0f 0x80884E04...) que LastLaser; WhiteLeoPard empieza con copias nuevas (0x80884E48...). |

Datos: cada unidad recibe los objetos `auto_` que solo usan sus funciones, pegados a su rango.

### Decisiones sin el usuario
- Nombres inventados: `game/MenuCommon_unk.cpp`, `game/CharaC58_unk.cpp`. `game/CharaFaceControl.cpp` sale de la cadena RTTI de boost (`CharaFaceControl`), sin vtable propia.
- `game/BattleTask.cpp` (sin fuente) crece de 0x5E20 a 0x1D55C bytes: no hay ninguna prueba de corte (constantes compartidas en todo el tramo, ninguna `__sinit`). Las vtables de `gmk::Cannon`, `chr::ControlCdnRestorePsycho` y `Bullet` solo tienen funciones debiles/heredadas en el tramo (vf58/vf44 de GimmickCommon, vf2C de ControlBase, `fn_80101330`), no son ficheros de esas clases. Revisar si se quiere partir por otra via.
- `symbols.txt`: `lbl_8073EA48` -> `@stringBase0` (pool `""` de CsvBase_char_124_13_34).

## Hallazgos
- dtk acepta los bloques `.ctors` de 4 bytes de las `__sinit` aisladas junto a la unidad anterior sin `align:`.

## Grupo 2 (chr::Control*)

| Tramo | Unidad | Prueba |
|---|---|---|
| 0x8018EEC4-0x80192908 | `chr/ControlCdnStone.cpp` (nuevo) | Constantes nuevas (0.0f 0x80881F70, 1.0f 0x80881F7C...) frente a ControlAttackBase; todas las clases `chr::ControlCdn*` (Stone, RestoreStone, Paralyzed, RestoreParalyzed, Betray, Sleep, RestoreSleep, Piyo, RestorePiyo, Psycho, RestorePsycho) con sus RTTI contiguos en `.rodata` 0x807395B4-0x807396D4 en orden inverso (Stone ultima = primera del codigo). Incluye al final los debiles `vf14/vf18/vf10__Q23chr11ControlBaseFv` (0x80192578-0x80192908, con su propia copia 0.0f/1.0f 0x80881FB0) y la vtable de `chr::ControlBase` (0x8077DC90): primer fichero que los usa. ControlStickByQuark empieza en 0x80192908 (nuevas copias 0.5f/1.0f/0.0f 0x80881FB8...). |
| 0x80192908-0x801963F4 | `chr/ControlStickByQuark.cpp` ampliada hacia atras | Mismas copias de constantes que el resto de ControlStickByQuark (0x80881FBC=1.0, 0x80881FCC=0.0...) hasta 0x801A0BA8; clases StickByQuark, Thrust, ThrustForCrab/Kraken/QuarkStand/Ghoul. |
| 0x801AAAE4-0x801AAB64 | `chr/ControlChaseCaught.cpp` ampliada | Dos destructores debiles en las vtables de ControlChaseAssist/ControlChaseFollow (clases de ControlChaseCaught). |
| 0x801C041C-0x801C3414 | `chr/ControlMountle.cpp` (nuevo) | Constantes nuevas frente a ControlChant (0x80882698...); clases Mountle, MountleQuadruped, GuardMountle, WallMountle, Jump. |
| 0x801C3414-0x801C3DCC | `chr/ControlMoveCursor.cpp` (nuevo) | 0.0f/1.0f cambian de copia en 0x801C3414 (0x80882748) y otra vez en ControlOneMotion (0x801C3DCC); vtable `chr::ControlMoveCursor` 0x80781B90 justo antes de la de ControlOneMotion. |

### Decisiones sin el usuario
- `chr::ControlBase` (vtable y vf10/14/18 debiles) queda dentro de `chr/ControlCdnStone.cpp` (no es un fichero aparte: solo funciones debiles con su copia de constantes; a revisar).
- `.bss` 0x807C7C78 (usado solo por ControlMountle) se queda en `auto_`: asignarlo creaba un ciclo de orden de enlace (ControlMountle -> ControlMoveCursor -> ControlOneMotion -> ControlPushedDown -> auto bss), porque el `.bss` de ControlPushedDown esta antes. Igual con `.bss` 0x807C7C30/0x807C7C38 de ControlChaseCaught (detras del bss de ControlPushedDown).
- `.sbss` de ControlCdnStone extendido hasta 0x8087F0C0 para incluir el guard compartido `@GUARD@assign_to<...mf0<v,9CharaTask>...>` (0x8087F0BE, 1 byte): dejarlo solo en un trozo `auto_` de 2 bytes da `undefined` al enlazar.

## Hallazgos (grupo 2)
- Si un objeto solo usado por la unidad X queda detras de datos de otra unidad Y y X va antes que Y en .text, dtk falla por ciclo de orden de enlace: hay que dejar ese objeto en `auto_`.
- Un guard `@GUARD@` (1 byte) compartido no puede quedarse solo en un hueco `auto_` minimo entre dos unidades: el enlace da `undefined`; meterlo en la unidad vecina.
