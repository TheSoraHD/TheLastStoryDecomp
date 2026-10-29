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

### `chr/ControlMountle` (0x801C041C, 29 funciones)

- Subir a un saliente (`ControlMountle`, antes `Control801C041C_unk`, y `ControlMountleQuadruped` para los cuadrúpedos), subirse a un personaje que se cubre (`ControlGuardMountle`), trepar una pared (`ControlWallMountle`) y el salto hacia un saliente (`ControlJump`, cuyo `vf10` lanza `ControlWallMountle` o `CharaTask::fn_801591E8`). 23/29 al 100% (incluidas las copias débiles); 6 `NONMATCHING` entre 97,2% y 99,9%: asignación de registros, y el cruce r3/r4 en las copias de `Vector3` (orden origen/destino, como en q3 de `mwcc-blockers.md`).
- **Split:** se intentó añadir `.bss 0x807C7C78-0x807C7C80` (el `@LOCAL@` del `bind` con `ControlWallMountle`). dtk lo rechaza por dependencia cíclica: los `@LOCAL@` de `.bss` de esta zona no siguen el orden del `.text`, y `ControlPushedDown`, que va después, tiene `.bss` en 0x807C7C28. Se deshizo el cambio. El símbolo solo se renombra con el nombre de nuestro `.o`.
- **Truco nuevo:** el original declara `inline` accesores que nosotros tenemos fuera de línea en `CharaTask.cpp` (`Vector3::operator-()`, `Vector3::fn_8013C394`, `CharaTask::fn_8013C38C`/`fn_8013C460`, `Model::fn_80139F24`/`fn_8013C554`/`SetMotionSpeed`). `ControlWallMountle` los inlinea, y el constructor de `ControlGuardMountle` supera el presupuesto y los llama todos con `bl`. Definirlos `inline` en esta unidad (como ya hacían `ControlDamage` o `ControlCdnStone` con `operator-`) da las dos formas. Sin ellos, el constructor de `ControlGuardMountle` lo inlineaba todo y quedaba al 0%.
- **Decisiones sin el usuario:**
  - `CharaTask::vf74` recibe `const atn::Vector3&` (antes `int`): es la dirección que pasa al constructor de `ControlMountle`. Símbolo renombrado `vf74__9CharaTaskFRCQ23atn7Vector3`.
  - `CharaTask::fn_8013CB68` recibe la rotación como `const atn::Vector3&` (antes `void*`). El original pasa el temporal de `ToRotation_unk` directamente, sin copia; con puntero no se puede. Se cambiaron unas 40 llamadas (`&x` → `x`; `vf34` hace `*(const atn::Vector3*)target`) y se renombró el símbolo. Con esto `Em308ParmCrab::vfEC` llega al 100%. Sin pérdidas.
  - `CharaTask.h`: inline nuevos `SetControlId_unk`, `SetRot_unk` y `SetFlag4_unk` (copias débiles 0x801C0730/0x801C0738/0x801C1050). Se borra `include/chr/ControlGuardMountle.h`: lo sustituye `chr/ControlMountle.h`, que declara las cinco clases.
  - Los 12 floats de `.sdata` (0x8087DA10-0x8087DA3C) son `static float` de ajuste de `ControlJump::fn_801C2198`, con nombres inventados (`sDownHeight0`... `sUpTime1`, `scope:local`).
  - `poolcheck` avisa de π, 2π y -π: son de la copia débil de `WrapAngle_unk`, que nuestro `.o` emite y el original tiene en otra unidad.

### `Event/Path` (0x80112220, 56 funciones)

- Cámara de los eventos (`Event::EventCamera_unk`, 0x65C bytes, cuatro en `Event::Manager`), con su trayectoria `Event::Path` (nombre real por la vtable) y el objetivo `CameraTarget_unk` (actor, nodo, desplazamiento o trayectoria). 39/56 al 100%; 17 `NONMATCHING` (81,7–99,8%): asignación de registros y planificación de las interpolaciones (`fn_801144A8`, `fn_801146A4`, `fn_80114AB4`, `fn_801157DC`), el cruce r3/r4 de las copias de `Vector3` y el `push_back` de `fn_80112B64`.
- **Split:** `.data` ampliado de 0x80779F68 a 0x80779FA0 (avisado): las dos cadenas `"Memory allocation failure"` de `vector<PathCubic_unk>`/`vector<PathPoint_unk>::__allocate`, que eran un `auto_`. El siguiente split (`MenuLoadTask`) empieza en 0x80779FA0.
- **Trucos:** un `std::vector<float>` envuelto en una clase derivada (`PathLengths_unk`) da el nivel de destructor de más (como `WarLionPathList_unk`); `X / 8.0f` y `X * 0.125f` no dan el mismo orden de operandos; el constructor no inline de `CommonData` definido en `Path.cpp` (el enlazador lo quita si nadie lo llama) pone `""` en `@stringBase0` en vez de en `@STRING@`.
- **Decisiones sin el usuario:**
  - `CommonData` tiene ya `m04` (dueño, `DataOwner_unk*`), `m08` (`std::string`) y `m14`; se quitan de las siete clases derivadas de `MotionData.h`. Los bits de `DataOwner_unk::m98` se parten para nombrar `mPadCamera_unk`.
  - `Event::Path` es la misma clase que `PathData_unk` y que `WarLionPath_unk` (`Em403WarLion`); no se fusionan aquí. `PathSpline_unk` es el mismo objeto que la spline de `WarLionPath_unk`.
  - El nombre real de `Camera_unk` es `atn::Camera` (`SetPosition__Q23atn6Camera...` en el objetivo). No se cambia: toca 76 ficheros. Accesores inline nuevos en `atn/Camera_unk.h` (`GetPosition_unk`, `GetTarget_unk`, `SetTarget_unk`, `SetPosition`, `GetRot_unk`, `GetFovy_unk`).
  - Renombrados globales: `fn_805F9920` → `PSVECSquareMag` (104 ficheros, misma firma); los que llamaban `fn_800928B0` como `extern "C"` (`EquipItemData`, `SwingObject`, `GimmickStateCommand`, `ColliAttrManager`, `EffectManager`) usan ya `atn::Model::fn_800928B0`; `ControlMoveCursor` usa `GetPad_unk` y `Camera_unk::GetRot_unk`, y `FlamingArrow` `GetSoundMgr_unk`, que son los nombres que da `Path.o` a esas copias débiles.
  - `poolcheck` avisa de ±π/2: son de la copia débil de `ToRotation_unk`, que el original llama con `bl` sin emitirla en esta unidad (igual que `WrapAngle_unk` en `ControlMountle`). Arreglarlo pide cambiar `include/math/Vec.h` para todas las unidades.

### `game/PlayerInput_unk` (0x80121F00, 31 funciones)

- Acciones del juego leídas del mando (`Input_unk`, 0x50 bytes en +0x48C de los ajustes de `lbl_8087F0A8`): `fn_80122C70` traduce una acción (0..0x35) a un botón según `SetConfig` y el mando clásico (0x21: ninguno), y `fn_801231D0`/`fn_801237F0`/`fn_80123C7C`/`fn_801240B4` dicen si está pulsada, repetida, `fn_800A5584` o mantenida. También están el bloqueo de acciones (`m14`), la tabla de iconos de botones (`sButtonIcons`, 21 entradas con `__sinit`) y la dirección del stick. 27/31 al 100%; 4 `NONMATCHING` (95,7–99,5%: disposición de ramas y planificación).
- **Trucos:** el cuerpo de `fn_801240B4` está inlineado en `fn_8012256C`: hace falta una `inline` (`Held_unk`) que use la función fuerte; con la función fuerte directa, MWCC no la inlinea. `if (skip < 1 || skip > 2) return 0;` da el `subi/cmplwi/ble` con el `return 0` delante. La copia de `ButtonUv_unk` es `__Vec4Copy` y el planificador reordena sus `psq_st`. `double d = __fabs(x); return d;` da `fabs f0`/`frsp f1, f0`.
- **Decisiones sin el usuario:**
  - Clase `Input_unk` (nombre ya usado en `symbols.txt` por `fn_8012476C`) con cabecera nueva `include/game/Input_unk.h`. Sus métodos se renombran (`fn_XXXXXXXX__9Input_unkF...`) y los 17 ficheros que los llamaban como `extern "C"` (`CharaTask`, `ControlCharge`, `ControlChant`, `ControlWallUp`, `ControlDive`, `ControlCannonLift`, `ControlBreakOpen`, `Em201ZanGurgFinal`, `Em209SmallGolem`, `EnemyQuarkStand`, `D2AMpMenuMessageList`, `D2AMpMenuFriend`, `GameSoundControl`, `MarketTask`, `Path`, `Dialog`, `MenuCommonTask`) pasan a llamadas de miembro. `Dialog` y `MenuCommonTask` declaran una vista local de `Input_unk` porque devuelven su propio tipo de UV; las otras vistas locales que solo declaran `fn_8012476C` (`Trap`, `CharaStatus`, `ControlChaseCaught`, `ControlCaught`, `ControlCdnStone`, `ControlJumpAt`) siguen igual: su `Settings_unk` supone `sizeof(Input_unk) == 1`.
  - Las funciones sin clase (`fn_80121F00`, los cuatro botones `fn_80121F08`...`fn_8012216C`, los accesores de `SetConfig`) quedan `extern "C"` con su nombre automático, como en los demás ficheros que las usan.
  - `poolcheck` avisa de π y 2π: copia débil de `WrapAngle_unk`, como en `ControlMountle`.

### `game/GameConfig_unk` (0x80184A64, 15 funciones)

- Ajustes del juego con menú de depuración (`GameConfig_unk`, 0x1144 bytes, la instancia de `lbl_8087F0A8` que otras unidades ven como `Settings_unk`): `fn_80185AD4` la crea, el constructor pone los valores por defecto y `fn_80186520` registra el menú "Config" (los nombres de los miembros salen de sus etiquetas). Incluye `DialogConfig_unk` (+0x5BC: menú "Dialog", dos lados de cámara y cuatro `NewCampScene2_unk`). 11/15 al 100%; 4 `NONMATCHING` (93,1–98,0%: asignación de registros, y el `push_back` de `vector<Light_unk>` como en `Event/Path`).
- **Trucos:** el pool de cadenas sigue el orden de **compilación**, no el textual: las cadenas de `DifficultyParam_unk::Register_unk` y `TitleConfig_unk::Register_unk` van antes que las de `fn_80185AD4` porque en el original eran funciones fuertes definidas ahí (inlineadas en `fn_80186520` y quitadas por el enlazador); se escriben igual y quedan como "extra" en nuestro `.o`. El constructor vacío de `DialogSide_unk` es una función fuerte definida tras el de `DialogConfig_unk` (si es inline, su copia débil sale antes que las otras). Copiar un `float[11]` con un bucle da `lfs/stfs`; la copia de la estructura da `lwz/stw`. `MAX(-5, MIN(5, diff))` da la doble evaluación de `fn_80187CC0`.
- **Decisiones sin el usuario:**
  - `LightColor_unk` pasa a ser `typedef atn::Color4f_unk` (cabecera nueva `include/atn/Color4f_unk.h`, derivada de `D2A_DrawColor`: el original asigna un `Color4f_unk` con el `operator=` implícito de `D2A_DrawColor`). Se renombran los tres métodos de `Light_unk` que lo reciben (`RC14LightColor_unk` → `RCQ23atn11Color4f_unk`). `Light_unk` y `RenderSystem_unk` siguen igual.
  - `fn_800F84BC`/`fn_800F833C` pasan a `atn::Vec2_unk` (constructor y `operator=`; nombre inventado). No son de `atn::Vector2_unk`: `SoftKeyboardU` (enlazada) tiene sus propias copias con ese nombre en 0x805030FC, y renombrarlas así rompía el enlace.
  - Cabeceras nuevas `include/game/GameConfig_unk.h` y `include/game/NewCampScene2_unk.h`; las vistas parciales de esas clases en otros ficheros siguen como estaban.
  - `poolcheck` avisa de π/180: copia débil de `DegToRad_unk`, que el original tiene en otra unidad. El `@STRING@` de `vector<Light_unk>::__allocate` está en el `.data` de otra unidad (0x8077927C; este split no tiene `.data`); nuestro `.o` lo emite.

### `game/CosEditBase` (0x801CCE50, 37 funciones)

- Base de los menús de vestuario del campamento (`CosEditBase : NewCampPartsBase`, hija `CosEditProtector`): tres personajes (`CostumeEditChara`) del miembro elegido y sus vecinos, tres listas de 13 filas (equipo, color, partes), la cola de cargas de equipo (`mLoads`, 128 entradas) y el cambio de arma o de prenda en todos los personajes con el mismo modelo (`fn_801CEA98`, `fn_801CF3C0`). 30/37 al 100%; `NONMATCHING`: constructor 96,1%, `fn_801CD648` 94,0%, `fn_801CDA00` 95,0%, `fn_801CDE14` 98,8%, `fn_801CE148` 97,4%, `fn_801CEA98` 97,5% y `__sinit` 98,1% (motivos en el fuente: asignación de registros, y el `""` de los `new` que en el original es la primera cadena del pool).
- **Trucos:** las entradas de carga se escriben campo a campo en un local y se copian (`load.mType = ...; AddLoad_unk(load);`): con constructor el temporal desaparece, y con `= {1, 0, chara}` MWCC deja plantillas `{1,0,0}` en `.rodata`. `fn_801CDD9C` lleva `__declspec(noinline)` (el original la llama con `bl` y MWCC la inlineaba). El final de `vf10` es `bool idle = mA98 == 1 ? false : mA98 != 2; if (!idle) {...} else {...}`. Las tres llamadas `fn_801E9218` de `vf10` van en una inline (orden de pila invertido) y `mCur` se asigna con un setter inline por valor (`SetCur_unk`, deja la copia muerta en la pila). Los prefijos `"pc101_"`... llevan un NUL extra (`"pc101_\0"`).
- **Decisiones sin el usuario:**
  - `CostumeEditChara`: `fn_801E9168`, `fn_801E917C`, `fn_801E91A0` reciben `atn::Vector3` por valor (los llamadores copian el vector a la pila); nuevas `fn_801E9218(rot, scale)`, `fn_801E92D4(from, to)` y la inline `SetCur_unk`. Símbolos renombrados (`...FQ23atn7Vector3`).
  - `NewCampPartsBase::vf34(int state)` (antes sin argumento): las copias débiles guardan el argumento en `m7C`.
  - `Inventory`: bloques `mEquipShown[7]` (0xD380), `mEquipSetA[16]` (0xD540) y `mEquipSetB[16]` (0xD940). `CharaStatus`: `CharaEquip_unk::m008` (color) y `m430`.
  - `mLoads` es un array con contador (`mLoadCount`) y una inline `AddLoad_unk` en vez de `atn::FixedArray_unk`: el constructor del original no pone a cero el contador en la lista de inicialización (lo hace dos veces en el cuerpo).
  - `mAA0` sigue siendo `ResHandle`: el destructor coincide, pero el original lo pone a cero en el cuerpo del constructor (su clase no tiene constructor que lo haga). Nuestro `.o` emite además una copia débil de `~ResHandle` y de `~basic_string` que el original no tiene.
  - `pairdata` renombró `@stringBase0` y los floats de `.sdata2`; el `.bss` del original es una sola etiqueta (`lbl_807C7CB8`, 0x60 bytes) frente a nuestras ocho estáticas, sin partir.

### `game/CosEditPalette_unk` (0x801D54C0, 23 funciones)

- Segundo fichero de `CosEditProtector` (el primero, `game/CosEditProtector.cpp`, sigue sin fuente): las páginas de paletas de color, partes y juegos de vestuario del menú (estados 0, 6–10 y 14), con la rejilla de colores (8×8) y de partes (8×4), la lista de opciones de las partes y los juegos guardados. 17/23 al 100%; 6 `NONMATCHING` (91,0–99,9%: asignación de registros; en `fn_801D6C1C` un registro guardado de más).
- **Trucos:** el `MenuCommonTask::sInstance` se lee antes que el texto cuando el aviso es una inline miembro (`sInstance->ShowWarning_unk(id)`). El setter de UV por valor con el nombre como primer parámetro (`SetUV_unk(name, task, uv)`) da el orden original: copia, tarea y luego la cadena. `fn_801F4E8C(task, hash(name))` va en una inline `GetRect_unk(task, name)` (la tarea se evalúa antes que el hash). Un `int` de `CharaParam_unk` en medio de su relleno cambia las copias del bloque en `CharaStatus::fn_8012B3E8`: se lee con un accesor. `x < 0 ? 0 : x` sale sin saltos (`srawi/andc`) con `MAX(0, x)`. Las comprobaciones `if (!a || !a->GetTask()) return;` dan el `bne/b` del original y dos `if` separados no.
- **Decisiones sin el usuario:**
  - Cabecera nueva `include/game/CosEditProtector.h` (clase con sus virtuales de la vtable 0x807827A8 y los miembros de 0xACC a 0x1CA8 que usa este fichero). Símbolos de los 22 métodos y del `__sinit` renombrados; también `fn_801D3DF4` y `fn_801D4E74` (de `CosEditProtector.cpp`).
  - `include/db/OptionParts_unk.h`: los tipos de `db/OptionParts_unk.cpp` (`OptionParts_unk`, `OptionLevelList_unk`...) salen a una cabecera; esa unidad no cambia.
  - `MenuCommonTask::fn_804A4294` pasa a ser miembro (símbolo renombrado; solo lo usaban `MenuCommonTask.cpp` y este fichero), con la inline `ShowWarning_unk`.
  - `atn/D2A_TaskInstance.h`: las declaraciones `extern "C"` con nombre automático pasan a `atn/D2A_TaskInstanceC.h` (solo las usa `D2A_TaskInstanceBase_64_8.cpp`, que las define por valor). Este fichero declara `fn_801F6E78` y `fn_801F7590` por dirección, como los demás ficheros del juego, porque pasa dos veces el mismo local sin copiarlo.
  - `NewCampPartsBase::m80` pasa a `int` (se compara con signo); `CosEditBase::m20C`; `fn_801CE648`/`fn_801CE7E8`/`fn_801CE87C` devuelven `CharaEquip_unk*`, `OptionParts_unk*` y `unsigned int*`; `CharaEquip_unk::m00C`, `m430` (`OptionParts_unk*`) y `fn_805657CC` como miembro (símbolo renombrado); `CharaStatus::m231`.
  - Quedan un `int* data` local en `fn_801D6AB0` y un `int n = 0` en `fn_801D6C1C` (registros del original) y la copia de los ids de un juego como `int[8]` (`fn_801D5F04`).

### `Event/DialogHead` (0x802251F8, 25 funciones)

- Cara del personaje en la ventana de diálogo (`Event::DialogHead : CharaTask, atn::IRender`, 0x1A18 bytes): copia el modelo del personaje que habla, lo dibuja con su propia cámara en el viewport del lado izquierdo o derecho (`DialogConfig_unk`) y carga los ficheros de caras `/database/chara/face/ce_*.face` (`FaceData_unk`, 7×8×2 `scoped_ptr`) o, si no hay fichero, la animación `CH_FACE_<tipo><nn>`. 18/25 al 100% (contando los 5 thunks de `IRender`, que conservan su nombre `fn_`); 7 `NONMATCHING` 98,0–99,9% (copias de `Camera_unk` y de `Vector3` con otros registros, un `lwz m524; cmpwi` muerto del original en `vf10`, y la pila de los temporales de `std::string` en `fn_80227434`).
- **Trucos:** el tercer argumento del constructor base se lee antes que el tipo con un accesor (`chara->GetData_unk()`). Las cuatro comprobaciones de `m1840[...]` usan la conversión a `bool` de `boost::scoped_ptr` (`__ptmf_test` con el ptmf de `get`, dos constantes en `.data`). El `switch` con un caso negativo era una cadena de `if` (`type < 0 || type == 11`, luego 3 y 5): MWCC la convierte en árbol y el orden de los cuerpos sigue al del fuente. Asignar la cámara con un setter inline (`view->SetCamera_unk(m142C)`, `m1624.SetCamera_unk(camera)`) acerca el orden de registros de la copia. Los bucles de `fn_80094BD8` escritos en la función (no en una inline) dan los registros del original. `MAX(m1820, 0.01f)` reutiliza el valor (con `frsp`) si `GetSide_unk()->mPos.y` va a un local. Destructor con un nivel inline más en `m1A00`: `LipSyncData_unk` deriva de `LipSyncKeys_unk` (el `Event::Dialog` tiene la misma forma en `m184`).
- **Decisiones sin el usuario:**
  - Cabeceras nuevas `include/Event/DialogHead.h` e `include/atn/IRender.h` (con `RenderView_unk`, el sistema de render `lbl_8087EFB4` con la cámara en 0x104). El primer virtual de `IRender` se llama `GetRenderType_unk` y no `vf0C`: `DialogHead` lo redefine aparte de `Task::vf0C`, que tiene la misma firma. Las copias locales de `IRender` de otros ficheros (`LensFlare.h`, `PostFx.h`, `VideoTHP.cpp`...) siguen igual.
  - `atn::ModelBase` declara sus cinco virtuales de render (deriva de `IRender` según su RTTI); no cambia ningún otro fichero.
  - `CharaTask::fn_80148B38(float distSq)`: todos los llamadores pasan un float en f1 que la función no lee. Símbolo renombrado a `fn_80148B38__9CharaTaskFf`; se actualizan `CharaTask`, `CopyRobotTask`, `CostumeEditChara`, `EnemyQuarkStand`, `gmk/MotionObject` y `Em208Queen` (que lo llamaba por su símbolo con `extern "C"`). Sin bajadas; suben `EnemyQuarkStand` y `MotionObject`.
  - `FaceData_unk` (de `CharaFaceControl.h`) tiene el `LH_Binary` como miembro, no como base: el original la destruye sin llamada virtual. `fn_8011EEA0` llama al `vf0C` a través de `GetHandle_unk()`. `CharaFaceControl` no cambia.
  - `DialogConfig_unk::mScenes` pasa a `[2][2]` (`[oscuro][derecha]`, `fn_80226B28`); `GameConfig_unk` no cambia.
  - `ModelCameraParam_unk` y `fn_80095F10` pasan a `atn/Camera_unk.h`; `CharaTask.cpp` conserva su copia local porque usa su propia `Camera_unk`. `CreateCamera_unk`/`ModelCamera_unk` siguen como copia local (igual que en `CharaTask` y `AutomaticDoor`).
  - `CharaEquip_unk::GetModel_unk()` (el objeto en 0x24 es un `atn::Model`) y `CharaTask::GetData_unk()`.
  - `Event/Dialog.cpp` usa la clase real (se quitan `DialogHead_unk` y sus `extern "C"`) y `m184`/`m190` pasan a `LipSyncData_unk`/`LipSyncKeys_unk` (eran `DialogParams*_unk`); ninguna función baja.
