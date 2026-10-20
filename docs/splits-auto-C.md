# splits-auto, agente C (rango .text 0x8033C3D8 - 0x804405F8)

## Grupo 1 (EnemyQuark*, EnemyTasha/EnemyTask, EnvSeManager, GimmickStateCommand)

- **0x8033C3D8-0x80352EC8** (auto) repartido entre `game/EnemyQuarkSecond.cpp` y `game/EnemyQuarkStand.cpp`:
  - `EnemyQuarkSecond`: .text 0x803388A4-0x80345384. Corte en 0x80345384: 0x80345384 es el ctor de
    EnemyQuarkStand (guarda `__vt__15EnemyQuarkStand`); las funciones anteriores usan las copias
    0.0f/1.0f... en 0x80885238.., las posteriores en 0x80885378.. (constantes duplicadas). Los usuarios
    de cada `.sdata2` caen todos dentro de su fichero (comprobado objeto a objeto).
  - Datos de Second: .rodata 0x8074A490-0x8074A8D8 (dobles + pool 0x418), .data 0x80789330-0x807894B8
    (tablas de saltos), .sdata 0x8087DC58-0x8087DC80, .sdata2 0x80885238-0x80885378.
  - `EnemyQuarkStand`: .text 0x80345384-0x803545E0 (~0xF25C). Datos: .rodata 0x8074A8D8-0x8074AF38
    (dobles, nombres boost::bind de RTTI, pool 0x4E0), .data 0x807894B8-0x807896B8, .bss
    0x807C8448-0x807C8458, .sdata 0x8087DC80-0x8087DCB0, .sbss 0x8087F410-0x8087F418,
    .sdata2 0x80885378-0x808854D8.
- **0x803559F8-0x8035BC84** (auto) -> `game/EnemyTasha.cpp` (0x803559F8-0x8035B534) +
  `game/EnemyTask.cpp` (0x8035B534-0x8035BC84). Prueba: copia propia de 0x4330000080000000 (Tasha
  0x8074B090, Task 0x8074B388) y de 0.0f (0x8088552C vs 0x808855F8 en sdata2); vtables `__vt__10EnemyTasha`
  y `__vt__9EnemyTask` seguidas en .data; nombre de RTTI "EnemyTasha" en .rodata de Tasha.
  Datos Task: .rodata 0x8074B388-0x8074B398, .data 0x80789A08-0x80789B00, .sdata2 0x808855F8-0x80885600.
- `game/EnemyTrista.cpp` ampliado: .text hasta 0x8035DF48 (fn_8035DF08 es el dtor de la clase local
  `@unnamed@enemy_trista_cpp@::ControlShingan`, en la vtable 0x80789B10); .rodata desde 0x8074B398
  (cadenas SE, tabla, rutas de efecto, nombre ControlShingan, pool 0xE0); .data desde 0x80789B00.
- `game/EnvSeManager.cpp` ampliado: .text 0x8035DF48 (getter/creador de la instancia, sbss 0x8087F418,
  usa el pool 0x8074B538) hasta 0x80360C5C (inicio de EnvSeBase); .rodata hasta 0x8074B630 (pool),
  .sdata 0x8087DCB0-0x8087DCC8, .sbss 0x8087F418-0x8087F420, .sdata2 0x80885670-0x80885690.
  Las constantes 0x80885670/74 las usan tanto 0x8035DFB8 (ctor) como 0x8035E858.. : mismo fichero.
- `game/GimmickStateCommand.cpp` ampliado: .text 0x803EF590-0x803F1BB4; la tabla de nombres de comandos
  (0x80752040, "EFF_PLAY".."EXPLODE") la usan 0x803EF590 y 0x803F11F8 (ctor del fichero ya conocido)
  y el pool 0x80752084 lo usan 0x803EF780 y 0x803F11F8 -> mismo fichero. .rodata 0x80751FF0-0x80752138,
  .data hasta 0x8078CB98, .sdata 0x8087DF18-0x8087DF38, .sdata2 desde 0x80885F90.
- `gmk/FragileObject.cpp` ampliado: .text desde 0x803F1BB4 (creador `new`+ctor de FragileObject con su
  pool 0x8075215C); .rodata 0x80752138-0x807522E0, .bss 0x807C8730-0x807C8740, .sdata 0x8087DF38-0x8087DF40.

### Decisiones sin el usuario
- Nombres nuevos `game/EnemyTasha.cpp` y `game/EnemyTask.cpp` (por clase/RTTI; EnemyTask: RTTI vive
  en ChaserGuard, pero su vtable/ctor/dtor estan aqui).
- GimmickCommonB0_unk (0x803EF608..) queda dentro de GimmickStateCommand (el nombre `_unk` de la clase ya existia).

### Hallazgos
- Los "creadores" `X* create(parent)` con `new(size, 0xC, pool, pool, 0)` van al principio del fichero
  de la clase creada, antes del ctor (EnvSeManager 0x8035DF48, FragileObject 0x803F1BB4).
