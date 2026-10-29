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
