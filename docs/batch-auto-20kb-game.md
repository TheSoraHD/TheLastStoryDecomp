# Lote `batch-auto-20kb-game` (propuesta E, primera mitad)

Unidades no `atn/` ni de enemigos, sin fuente, de 10–20 KB, en orden de dirección (`targets.py --max-unit 20480 --limit 1000`, > 10240). Trabajo secuencial. Rama creada desde `close-near-units` (propuesta K, sin mergear) para no perder sus dos unidades enlazadas.

## Resultado por unidad

### `Event/EventCommandDef_unk` (0x800081C0, 3 funciones)

- Fuente nuevo generado por script (`tools/decomp/eventcmd/`: `ecd_sim.py` simula el `__sinit` y saca las escrituras en tiempo de ejecución, `ecd_an.py` clasifica los objetos de datos, `ecd_gen.py` escribe el `.cpp`). 92 tablas `ParamDef_unk`, 38 `ParamEnum_unk`, 8 tablas de variantes y la tabla de 49 `CommandDef_unk` (0x80775208).
- `.rodata`, `.data`, `.sdata2` y sus reubicaciones son idénticos al original (comparados byte a byte y por destino de cada reubicación).
- `fn_80008210` 100%; `fn_800081C0` 94% (orden de `li r5, 0`); `__sinit` 99,96% (la última escritura va antes del `addi r11` del epílogo). Probado también con otros `-ipa`: igual.
- **Decisiones sin el usuario:**
  - `symbols.txt`: `fn_800082AC` → `__sinit_\EventCommandDef_unk_cpp` (`scope:local`).
  - `config.yml` (avisado en la sesión): `block_relocations` en `.data:0x807AC0D8`, un 0x8000A000 de la tabla `lbl_807ABF68` que dtk tomaba por un puntero al interior del `__sinit` y lo hacía global con sufijo.
  - `include/Event/Node.h`: `ParamValue_unk()` vacío para los atributos sin valor por defecto (282 de 811 no tienen escritura en el `__sinit`).
- Orden del fuente: cada tabla de enumerados va justo antes de la primera tabla de atributos cuyas cadenas nuevas aparecen después de ella en `.rodata` (las cadenas compartidas con un enumerado no cuentan).

### `game/Bullet` (0x8010EE78, 27 funciones)

- 14/27 al 100%. `fn_80110858` (6 KB) sin escribir; el resto `NONMATCHING` con motivo.
- **Decisiones sin el usuario:**
  - `Bullet.h` reescrito: las estructuras de repetición (`ReplayShot_unk`, `ReplayEvent_unk`), `HomingField_unk` y `ShotEvent_unk` viven ahí; los `fn_` del objeto son miembros de `Bullet` (antes `extern "C"`). `vf1C` devuelve `int`; `vf20` recibe la dirección no `const`.
  - Llamadores ajustados: `HomingBullet` (usa `mA0.m08` como velocidad y `__MtxCopy`), `BulletPoint`, `Em204Mulue`, `Em204MulueMp`, `Em204BigMulue`, `EnemyQuarkStand`.
  - `include/math/Vec.h`: declaraciones `Matrix3x4::Translate_unk(const Vector3&)` y `Vector3::Transform_unk(const Matrix3x4&) const` (copias débiles 0x8010F67C / 0x8010F6FC, definidas inline en `Bullet.cpp`).
  - `gmk/Lemon.h`: `CollResult_unk::m00` pasa a `int`.
- Herramienta nueva `tools/decomp/pairdata.py`: renombra los `lbl_` de datos del split con los nombres de nuestro `.o` emparejando reubicaciones de las mismas instrucciones (ejecutar con `--apply` al cerrar cada unidad). Los objetos débiles de plantilla (`@STRING@`, `@LOCAL@`, `@GUARD@`) mantienen el ámbito por defecto: con `scope:local`, dtk los globaliza con sufijo de dirección cuando otra unidad los usa.

### Reordenación del lote

`DamageCalc_unk` (`HitInfo_unk` mezcla el registro de habilidad y el mensaje de golpe de `vf3C`) y `Event/Path` (necesita reconstruir una clase de cámara de 0x65C bytes) se pasan al final; primero las unidades `chr/Control*`.

### `chr/ControlCdnStone` (0x8018EEC4, 57 funciones) y `chr/ControlBase` (nueva, 3)

- Estados alterados (piedra, parálisis, traición, sueño, mareo "piyo", confusión "psycho") y sus controles de recuperación. 51/57 al 100%; los 6 restantes `NONMATCHING` (5 `vf10` con la carga temprana de `mChara` antes del prólogo o registros del functor; `Betray::vf1C` r5/r6).
- **Split (avisado y aprobado en la sesión):** las tres últimas funciones (`ControlBase::vf14/vf18/vf10`) son otro fichero, `chr/ControlBase.cpp` (van tras los destructores implícitos, tienen sus propios 0.0/1.0 en `.sdata2` y su vtable `__vt__Q23chr11ControlBase`). `ControlCdnStone` termina en `.text` 0x80192578, `.data` 0x8077DC80, `.sdata2` 0x80881FB0, y su `.bss` se amplía a 0x807C7B98 (el `@LOCAL@` de la plantilla mf0 estaba en un hueco). `ControlBase`: `vf14` 100%, `vf18` 98,2% (orden de las ranuras de pila de las tres rotaciones), `vf10` 96,3%.
- **Decisiones sin el usuario:**
  - `ControlCdnStone` pasa de `ControlStickByQuark.h` a `ControlCdn.h` (este lo incluye); layouts de `Stone`, `Paralyzed`, `Sleep`, `Piyo` y `Psycho`, y clases nuevas `ControlCdnRestore*`.
  - `CharaTask.h`: `enum CONTROL_MODE {}` dentro de la clase y `m564` de ese tipo (el nombre sale del tipo del `bind` en el `.dol`), con casts en `CharaTask.cpp`, `gmk/MeetingSpot.cpp` y `gmk/InteractionSpot.cpp`; `CharaInfo_unk::m04` es `const wchar_t*`; inline nuevo `SetSide_unk(int)`.
  - El texto vacío de `Betray` es `lbl_808813D0` (compartido, como en `MP/Text.cpp`), no un `L""` propio.

### `chr/ControlCharge` (0x801A2CEC, 38 funciones)

- Carga de un ataque (`ControlCharge`, hija de `ControlMoveCursor`) y los ataques que la siguen: `ControlChargeAttackBase` (vf0C puro) con `ControlChargeAttack`, `ControlHighJumpAttack` y `ControlQuarkSeirenAttack`; además `ControlZanTackle`. 26/38 al 100%; el resto `NONMATCHING` (asignación de registros, sobre todo el cruce r3/r4 de los primeros stores de los constructores derivados, y el orden z/xy de dos getters de `Vector3`).
- **Decisiones sin el usuario:**
  - `CharaTask::fn_8016D74C` y el constructor de `ControlCharge` reciben el objetivo como `const atn::Vector3*` (los llamadores pasaban `(int)&pos`); `symbols.txt` renombrado.
  - `CharaTask::vfE0` recibe la dirección (`const atn::Vector3&`, la pasa `ControlQuarkSeirenAttack`); overrides de `Em203GreaterDaemon`, `Em203GreaterDaemonRed` y `EnemyQuarkStand` y sus símbolos renombrados.
  - `HitInfo_unk::mC8` nuevo (último miembro).
  - `ControlCharge.h` reescrito con las seis clases; `ControlHighJumpAttack::m2C` es `float`.

### `chr/ControlChaseCaught` (0x801A77BC, 39 funciones)

- Ocho controles del modo persecución: `ControlChaseCaught`/`Held`/`Release` (atrapado por un guardia), `ControlChaseCatch`/`Hold`/`Released` (atrapa a otro), `ControlChaseAssist` y `ControlChaseFollow` (sigue al líder). 29/39 al 100%; el resto `NONMATCHING` (registros; la matriz de rotación de `Catch`/`Hold` tiene el mismo problema de huecos de pila que `ControlBase::vf18`).
- **Decisiones sin el usuario:**
  - `CharaTask::fn_801598B4` y `fn_80159BA4` y los constructores de `ControlChaseCaught`/`ControlChaseCatch` reciben `CharaTask*` en vez de `int` (los llamadores `ChaserNpc` y `ChaseManager` hacían `(int)ptr`); símbolos renombrados.
  - El orden de las clases en `ControlChaseCaught.h` sigue el de las vtables (Caught, Held, Release, Catch, Hold, Released, Assist, Follow): decide el orden de los destructores implícitos.

### `chr/ControlDamage` (0x801AC434, 41 funciones)

- Reacciones a golpes: `ControlDamage`, `ControlGuard`, `ControlAvoid`, `ControlKnockdown`, `ControlGetUp`, `ControlRevival`, `ControlRevivalSkelton`, `ControlFallDead`, `ControlGuarded` y `ControlDamageLastOne`. 30/41 al 100%; los 11 restantes `NONMATCHING` (3 `vf10` con la carga temprana de `mChara`; 2 `fn_` "se puede cancelar" con cargas planificadas en el prólogo; en `Knockdown::vf0C` y `DamageLastOne::vf0C` el original no fusiona la segunda lectura de `mChara` de la inline expandida; registros y huecos de pila sueltos).
- **Decisiones sin el usuario:**
  - El constructor `__ct__17EnemyReaction_unk...` era `chr::ControlDamageLastOne(CharaTask*, const atn::Vector3*, int)`: renombrado y `game/EnemyTask.cpp`/`EnemyTasha.cpp` usan la clase real (incluyen `chr/ControlDamage.h`).
  - `ControlDamage.h` reescrito con layouts y las clases en el orden de los destructores implícitos.
  - `FuncPair_unk` (cabecera nueva `chr/FuncPair_unk.h`, nombre inventado): par de `boost::function` que `ControlRevival::vf10` convierte en una sola acción. Sus copias débiles 0x8019C8A8-0x8019D634 (dentro de `chr/ControlStickByQuark`) renombradas en `symbols.txt` con los nombres de nuestro `.o`. Recibe los argumentos por valor: con referencias MWCC inlinea el constructor y el original lo llama.
  - `poolcheck` avisa de la cadena `FuncPair_unk` (nombre RTTI del gestor débil del functor; en el original vive en `ControlStickByQuark`). En `.data` el original deja 0x14 bytes de ceros tras los ptmf (dos RTTI débiles eliminados) y nosotros emitimos tres: no afecta a objdiff, sí al enlace cuando la unidad se active.

### `chr/ControlChant` (0x801BC5B8, 21 funciones)

- Canto de una magia (`ControlChant`, hija de `ControlMoveCursor`), lanzamiento (`ControlMagic`) y final (`ControlMagicEnd`, hija de `ControlDropDown`). 15/21 al 100%; `NONMATCHING`: los dos `vf0C` (el de `ControlMagic`, 6,4 KB, al 90%: el original tiene 0x30 bytes más de pila), `ControlChant::vf10` y `ControlMagic::vf18` (cargas en el prólogo), `vf4C` (orden z/xy) y el constructor de `ControlMagic` (registros de `CanAct_unk`).
- **Split (avisado en la sesión):** `.rodata` de la unidad ampliado de 0x8073B590-0x8073B5D0 a 0x8073B3B4-0x8073B5D8. El SE del canto, el double de conversión, los nombres de hueso, 2π y los nombres RTTI de sus dos `bind_t` estaban en tramos `auto_`, y `""`/`"%s"` en 0x8073B5D0. Empieza en 0x8073B3B4 y no en 0x8073B3B8 porque `ControlLookAround` (enlazada) termina ahí: con 0x8073B3B8, objdiff falla por un símbolo de hueco fuera de sección. Queda un aviso de alineación de dtk, como en otras unidades.
- **Decisiones sin el usuario:**
  - `CharaTask::fn_801603CC(int, float)`: antes era `(float, bool)`. El nombre RTTI de su `bind` (`mf2<void, CharaTask, int, float>`) da el orden real. Llamadas de `CharaTask`, `Em001Necromancer` y `Em101ElderZesyaSecond` y símbolo renombrados. El constructor de `ControlMagic` recibe el `int` y lo convierte a `bool`.
  - `ControlBase::vf50` devuelve `int` (`ControlMagic::vf50` devuelve 0/1; la base es `li r3,0`); también en `ControlGmkCoffinOpening.h`.
  - `fn_801C02F4` es el constructor de `ControlMagicEnd`: `Em001Necromancer`, `Em003LesserDaemon`, `Em101ElderZesya` y `Em101ElderZesyaSecond` usan `new chr::ControlMagicEnd(this)` en vez del `extern "C"` (regla 10). Sin pérdidas.
  - Herramienta de sesión: los nombres `@NNNN` cambian al editar el fuente, así que `pairdata` se ejecuta al final de cada unidad (o se devuelven a `lbl_` y se repite).
