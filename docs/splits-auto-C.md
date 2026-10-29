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

### Decisiones sin el usuario (grupo 1)
- Nombres nuevos `game/EnemyTasha.cpp` y `game/EnemyTask.cpp` (por clase/RTTI; EnemyTask: RTTI vive
  en ChaserGuard, pero su vtable/ctor/dtor estan aqui).
- GimmickCommonB0_unk (0x803EF608..) queda dentro de GimmickStateCommand (el nombre `_unk` de la clase ya existia).

### Hallazgos
- Los "creadores" `X* create(parent)` con `new(size, 0xC, pool, pool, 0)` van al principio del fichero
  de la clase creada, antes del ctor (EnvSeManager 0x8035DF48, FragileObject 0x803F1BB4).

## Grupo 2 (0x80363C28-0x803B33F8)

- **0x80363C28-0x80365390** -> `game/SystemErrorMessage_unk.cpp` (nombre inventado). Contenido: conversion
  char->wchar (0x80363C28), creador del singleton sbss 0x8087F420 (0x80363CE4, `new` con pool 0x8074D8C0:
  camp_info*.d2a/camp_cursor.d2a), y la ventana de mensajes de sistema que usa la tabla 0x8074CCD0 de
  mensajes de error de disco/memoria en 6 idiomas ("Please insert The Last Story disc."...). Constantes
  propias en .sdata2 0x808856C8..0x808856F8 (0, -1, 1 distintas de EquipItemData 0x808856C0 y de GameTask
  0x80885708). Datos: .rodata 0x8074B820-0x8074D968, .data 0x80789EB0-0x80789ED8, .sbss 0x8087F420,
  .sdata2 0x808856C8-0x80885700. Clase sin vtable/RTTI: no se pudo dar un nombre real.
- **0x8036554C-0x8036555C** -> `game/FriendManager_cont.cpp` (`fn_8036554C__13FriendManagerFv`, ya con
  nombre de FriendManager; FriendManager es unidad enlazada, no se toca). Revisar: fusionar al decompilar.
- **game/GameTask.cpp** ampliado (sin fuente): .text 0x8036555C-0x80376B68. 0x8036555C/0x8036562C (tablas
  de saltos 0x8078A070/0x8078A150) solo los llaman funciones de GameTask; 0x80365708 usa el pool de GameTask
  0x8074DC1C y el bss 0x807C8458 (inicializado por el `__sinit` 0x80376AD4). El tramo 0x8036CA60-0x80376AD4
  (GamePauseTask y funciones de campo/flags) comparte todas sus constantes .sdata2 0x80885708/0x8088570C y el
  pool 0x8074DC1C con GameTask; `__sinit` 0x80376AD4 cierra el fichero (.ctors 0x8072D354). Datos: .rodata
  0x8074D9A0-0x8074DDF8, .data 0x8078A070-0x8078A4B8, .bss 0x807C8458-0x807C84F0, .sdata 0x8087DCD0-0x8087DCE0,
  .sbss 0x8087F438, .sdata2 0x80885708-0x808857F8.
- **game/GameOverTask.cpp** ampliado: .text desde 0x80376B68 (creador del singleton sbss 0x8087F440, pool
  0x8074DEC0 compartido con 0x80376BE8); .rodata 0x8074DDF8-0x8074DF88, .sbss 0x8087F440.
- **0x80378474-0x8037C71C** -> `game/FinishCamera_unk.cpp` (nombre inventado por la cadena "FinishCamera" de
  su pool 0x8074DF98). Corte inicial: copia propia de 0.0f/1.0f (0x80885830..) frente a GameOverTask
  (0x808857F8..); corte final: GameBgm (unidad con fuente) empieza en 0x8037C71C.
- **0x8037D8B8-0x803935AC** -> `game/GameCamera_unk.cpp` (nombre inventado: pool 0x8074E210 con "GameCamera",
  modos Default/Hide/Shoot/...). Copias propias de constantes 0x808858E8..0x80885ACC; corte final: 0x803935AC
  usa la tabla 0x8074EF78 que usan las funciones del script de eventos.
- **std/bad_cast.cpp** (GameEventScript) ampliado: .text 0x803935AC-0x803B27AC (incluye 0x8039BDB4-0x803B2780
  y su `__sinit` 0x803B2780, .ctors 0x8072D358). Pruebas: pool 0x8074F8CC usado de 0x80393610 a 0x803AE1D4,
  tabla 0x8074ED24 y constantes 0x80885B10/14/30 usadas en todo el tramo; el creador 0x80393610 hace `new` +
  ctor 0x803936A4. La vtable `gmk::Cannon` que autoscan ve es solo vf64 de GimmickCommon (no es Cannon).
  Datos: .rodata 0x8074E440-0x8074FB78 (incluye nombres RTTI de basic_stringstream/iostream/istream), .data
  0x8078A780-0x8078B3A0, .sdata 0x8087DD38-0x8087DD60, .sbss 0x8087F450, .sdata2 0x80885AD0-0x80885C00.
- **auto_03_803B27AC_text.cpp** (fuente, GameLoadTask) ampliado con su `__sinit` 0x803B338C (.ctors
  0x8072D35C) y datos: .rodata 0x8074FB78-0x8074FC08 ("(New)", RTTI GameLoadTask/GameSaveLoadCommon, pool),
  .data 0x8078B3A0-0x8078B400 (vtable GameLoadTask), .bss 0x807C84F0-0x807C8600, .sdata 0x8087DD68,
  .sbss 0x8087F458, .sdata2 0x80885C00-0x80885C18.

### Decisiones sin el usuario (grupo 2)
- Nombres inventados: `game/SystemErrorMessage_unk.cpp`, `game/FinishCamera_unk.cpp`, `game/GameCamera_unk.cpp`.
- `game/FriendManager_cont.cpp` (resto de FriendManager, enlazado).
- En ese momento `std/bad_cast.cpp` era en realidad el fichero de GameEventScript; el split se corrigió después
  en `codex/std-bad-cast` (`docs/std-bad-cast.md`). `auto_03_803B27AC_text.cpp` es GameLoadTask
  (`game/GameLoadTask.cpp`); no se renombra porque esta en configure.py.
- No renombre el pool de `auto_03_803B27AC_text` a `@stringBase0`: su fuente lo referencia como `lbl_8074FBA4`.
- Las unidades nuevas dan "Missing configuration" en configure (no estan en game_units.json, que no toco).

### Hallazgos
- Los `__sinit` aislados como `auto_fn_*` llevan su entrada .ctors: hay que dar el rango .ctors a la unidad.
- Varios "creadores de singleton" (`if (!sInst) sInst = new(...) X(...)`) preceden al ctor y comparten pool.

## Grupo 3 (0x803B35FC-0x803CD9A8)

- **game/GameChoiceWindow.cpp** ampliado (sin fuente): .text 0x803B35FC-0x803B7944. 0x803B35FC..0x803B4338
  (ventanas de texto: ui_talk.d2a/ui_friend_mes.d2a, tabla SE 0x8074FCE8 usada por 0x803B4338 que llama
  GameChoiceWindow 0x803B6324; 0x803B3D2C lo llaman GameChoiceWindow y LogMessageTask) y 0x803B6970..0x803B78E8
  (usan las constantes .sdata2 0x80885C20/34/44/48/4C de GameChoiceWindow y el sbss 0x8087F468). Datos:
  .rodata 0x8074FC40-0x8074FF48 (pool 0x8074FD38), .sdata 0x8087DD70-0x8087DD98, .sbss 0x8087F468,
  .sdata2 hasta 0x80885C58.
- **0x803B7944-0x803B80F4** -> `game/GameSaveTask.cpp` (vtable/RTTI GameSaveTask, constantes propias
  0x80885C58..0x80885C70, pool 0x8074FF58). Incluye 0x803B7FC8/0x803B807C (mismos campos 0xd6c/0xd70/0xd80 que
  los metodos de GameSaveTask).
- **0x803B80F4-0x803B8C6C** -> `game/GameSaveLoadBackgroundCommon.cpp` (ctor y metodos
  `__28GameSaveLoadBackgroundCommon`, vtables GameSaveLoadBackgroundCommon/GameSaveLoadCommon; pool
  "savedata.dat"/"banner.bin"/"%s/save%02d.dat"; dobles 0x8074FF88/0x8074FFB0, .sdata2 0x80885C78-0x80885C98).
  Termina donde empieza GameSaveBackground (enlazada).
- `game/GameSaveBanner.cpp` (fuente): .sdata 0x8087DD98 (solo lo usa 0x803B9324).
- **0x803B9804-0x803BEBAC** -> `game/GameSaveData_unk.cpp` (nombre inventado; sin vtable; serializa el
  estado de juego: lo llaman GameTask, GameSaveTask, CampSaveLoad, GameLoadTask). Constantes propias
  .sdata2 0x80885CA0-0x80885CB8 (0, 1, -1 distintas de GameScene 0x80885CB8/BC); .data 0x8078B6C8-0x8078BBE0,
  .rodata 0x807500A8, .sdata 0x8087DDA0.
- **game/GameScene.cpp** ampliado (sin fuente): .text 0x803BEBAC (creador `new`+ctor 0x803BEC40, pool 0x807500DC)
  hasta 0x803CCF64 (incluye su `__sinit` 0x803CCF38, .ctors 0x8072D360). Todo el tramo 0x803C3664-0x803CCF38 usa
  el pool 0x807500DC y las constantes 0x80885CB8/0x80885CBC de GameScene. Datos: .rodata 0x807500B0-0x80750518,
  .data 0x8078BBE0-0x8078BCB8, .bss 0x807C8600-0x807C8628, .sdata 0x8087DDA8-0x8087DEE8, .sbss 0x8087F478,
  .sdata2 0x80885CB8-0x80885D48.
- **0x803CCF64-0x803CD9A8** -> `game/U16Table_unk.cpp` (nombre inventado). Tras el `__sinit` de GameScene, asi
  que es otro fichero; pool propio 0x80750518 ("vector length error") y "Memory allocation failure" 0x8078BCB8;
  funciones de tabla de u16 (lectura de fichero, liberar, busqueda acumulada) usadas por GameScene y otros.

### Decisiones sin el usuario (grupo 3)
- Nombres inventados: `game/GameSaveData_unk.cpp`, `game/U16Table_unk.cpp`. Nombres nuevos por clase:
  `game/GameSaveTask.cpp`, `game/GameSaveLoadBackgroundCommon.cpp`.
- 0x803B35FC-0x803B434C incluido en GameChoiceWindow sin constante duplicada que lo separe (no usa .sdata2);
  podria ser un fichero aparte de ventanas de texto. Revisar.
- Sin tocar: 0x803CE070-0x803CE4B0 (tras GameSoundControl, con fuente; parece GameBgm helpers + Unk_803CE3B8
  de GameSaveBackground) y 0x803CE688-0x803CE980 (entre GameTimer y GameUI): sin pruebas claras de a quien pertenecen.

## Grupo 4 (0x803CE854-0x803EBC44)

- **game/GameUI.cpp** (fuente) ampliado: .text 0x803CE854-0x803E836C. Todo el tramo 0x803CFC78-0x803E78B4 usa las
  copias de constantes de GameUI (0x80885D58/0x80885D60, ya en su split) y su pool 0x807506A0; el `__sinit`
  0x803E78B4 (.ctors 0x8072D364) cierra el fichero. Por delante: 0x803CE910 es el creador de GameUI (`new` con el
  pool 0x807506A0 + `__ct__6GameUI`) y 0x803CE854/0x803CE8A8 usan el sbss 0x8087F494 de GameUI.
  Datos: .rodata 0x80750558-0x80751D08 (tabla SE 0x80750618 que la fuente ya cita, dobles, pool), .data
  0x8078BE40-0x8078C690, .bss 0x807C8628-0x807C8728, .sdata 0x8087DEE8-0x8087DEF8, .sbss 0x8087F490-0x8087F498,
  .sdata2 0x80885D58-0x80885F48. Ninguna funcion al 100% perdida (regress lost: 0).
  - **symbols.txt**: `lbl_807506A0` -> `@stringBase0` (scope:local data:string_table), pool de cadenas de GameUI
    (empieza por "" y "GameUI"; 0x1668 bytes solo de cadenas).
- **0x803E836C-0x803EBC44** -> `game/GameVoice.cpp` (vtable/RTTI GameVoice, ctor 0x803E836C; constantes propias
  0x80885F48/0x80885F4C distintas de GameUI y de GimmickCommon). Datos: .rodata 0x80751D08-0x80751DB0 (doble,
  "GameVoice", pool "SoundWarning..."/"/database/lip/%s.lip"), .data 0x8078C690-0x8078C6C8, .sdata2 0x80885F48.

### Decisiones sin el usuario (grupo 4)
- 0x803CE688-0x803CE854 (entre GameTimer y GameUI, enlazada/fuente) se deja `auto_`: helpers de contador
  (llamados desde bad_cast/GameEventScript y GameSaveData_unk), sin constantes ni pool que lo asignen.
- Renombrado en symbols.txt: `lbl_807506A0` -> `@stringBase0`.

## Grupo 5 (gmk: 0x80408324-0x8040B560, 0x804225E0, 0x80423340, 0x8043465C-0x80435CE4, 0x8043F028-0x804405F8)

- **0x80408324-0x8040B560** -> `gmk/Cannon.cpp` (0x80408324-0x804093B4) + `gmk/DropItem.cpp` (0x804093B4-0x8040B560).
  Pruebas: vtables/RTTI `gmk::Cannon` y `gmk::DropItem` seguidas en .data; constante 0.0f propia (Cannon 0x80886294,
  DropItem 0x808862C0); cada uno con su creador (0x80408324 / 0x804093B4, `new` con su pool 0x80752954 / 0x80752A5C)
  y sus cadenas RTTI "gmk::Cannon" / "gmk::DropItem". DropItem: .bss 0x807C8888 (solo lo usa DropItem).
- `gmk/CharaLight.cpp` (fuente) ampliado: .text desde 0x804225E0 (`fn_804225E0`, ya escrito en su fuente con la nota
  "el split tendra que empezar ahi") hasta 0x8042335C (su `__sinit` 0x80423340 de los boost::function locales,
  .ctors 0x8072D368, .sbss 0x8087F4C0-0x8087F4E0 con el @GUARD); .rodata desde 0x807533F0 (dobles de 0x80422BF4).
  Resultado: `fn_804225E0` pasa a 100% (matched 4757 -> 4758).
- `gmk/RandomItem.cpp` (fuente) ampliado: .text hasta 0x80435CE4 (`fn_8043465C/804348C4/80435684__Q23gmk10RandomItem`
  ya nombrados como metodos de RandomItem; 0x80435908 lo llama 0x80435684; 0x80435CBC lee campos de RandomItem);
  .rodata desde 0x80754180 (cadenas SE + tabla 0x807541A0 usada por 0x804348C4), .sdata2 hasta 0x808868C8.
- **0x8043F028-0x804405F8** -> `game/Gun.cpp` (vtable 0x8078F228 con RTTI "Gun" 0x80886A1C, base Arms; constantes
  0x80886A10-0x80886A18; pool 0x80754498 "it090_00.model"). Incluye ColliObjectData_unk/ColliTreeData_unk
  (0x8043F5C4..0x8044058C) y el `__sinit` 0x804405CC (.ctors 0x8072D370), que cierra el fichero justo en el limite
  con el agente D. Datos: .rodata 0x80754498-0x807544C8, .data 0x8078F210-0x8078F258, .sdata2 0x80886A10-0x80886A20.

### Decisiones sin el usuario (grupo 5)
- `game/Gun.cpp` en `game/` (clase sin namespace, base Arms); ColliObjectData_unk/ColliTreeData_unk podrian ser otro
  fichero, pero no hay constante ni `__sinit` entre medias: quedan dentro de Gun. Revisar.
- La entrada sdata2 0x80886A1C ("Gun") queda en Gun; 0x80886A20+ es del agente D (HomeButtonMenu).

## Grupo 6 (tramos menores 0x803CE070-0x803CE4B0, 0x803CE688-0x803CE854)

- `game/GameSoundControl.cpp` (fuente) ampliado: .text hasta 0x803CE4B0 y .data desde 0x8078BCD8 (tabla de saltos de
  0x803CE278). 0x803CE074/0x803CE0B4 recorren la tabla m64/m68 de GameSoundControl (layout de su cabecera);
  0x803CE160/0x803CE278 los llama GameBgm junto a esos; Unk_803CE3B8 (lo usa GameSaveBackground) queda dentro por
  no haber prueba de corte. No se pierde ninguna funcion al 100%.
- **0x803CE688-0x803CE854** -> `game/GameTimer_cont.cpp`: helpers que escriben los campos 0x48/0x4C/0x50/0x54 de
  GameTimer (mState/mValue/mLimit/mCountDown) y 0x803CE768/0x803CE7E0 que los cargan/guardan; GameTimer esta enlazada.

### Decisiones sin el usuario (grupo 6)
- `game/GameTimer_cont.cpp` (resto de GameTimer, enlazada): fusionar al decompilar.
- Unk_803CE3B8 (0x803CE3B8-0x803CE4B0) incluido en GameSoundControl sin prueba fuerte (podria ser fichero propio).

## Grupo 7 (datos `auto_` de unidades con fuente del rango)

Objetos usados solo por una unidad y pegados a sus datos (comprobado con xref, tambien referencias desde otros datos):
- .rodata: Banana 0x80752EC0-0x80752EE8, BulletPoint desde 0x80753110, CharaLight hasta 0x80753450, InteractionSpot
  hasta 0x80753758, LandMine hasta 0x80753310, OpenBoard hasta 0x80753868, PassBoard 0x80753960-0x80753EC0,
  ShakeCamera 0x80753310-0x80753388, TodomeCountUp hasta 0x80753F98, WindyItem desde 0x80754230.
- .bss: SwingObject 0x807C8770-0x807C8878 (incluye el @LOCAL de su boost::function), Brazier 0x807C8878, HealPoint
  0x807C8898 (@LOCAL boost::function), Lemon 0x807C88A0, Banana 0x807C88E0, GimmickManager 0x807C8728.
- .sbss: SwingObject 0x8087F4A8 y HealPoint 0x8087F4B0 (@GUARD de sus boost::function), GimmickManager 0x8087F4A4.
- .sdata: AutomaticDoor 0x8087DF50, Bat 0x8087DF68 (@STRING Resize de 0x804038D0), RotaryRoom 0x8087DFA0, OpenBoard
  0x8087DFB8, LemonBucket 0x8087DFC8, RandomMerchant 0x8087DFD0, RandomItem 0x8087DFD8-0x8087DFF0.
- **symbols.txt**: pools de cadenas `lbl_` -> `@stringBase0` (scope:local data:string_table): 0x80752ED4 (Banana),
  0x80753410 (CharaLight), 0x807532E0 (LandMine), 0x80753830 (OpenBoard), 0x80753E98 (PassBoard), 0x8075332C
  (ShakeCamera), 0x80753F3C (TodomeCountUp). Todos son cadenas puras (sin relocalizaciones) que empiezan por "".

### Decisiones sin el usuario (grupo 7)
- Renombrados `@stringBase0` listados arriba.
- No asignados por compartidos: `@LOCAL@assign_to<PFP12MapModel_unk...>` (bss 0x807C88F0, lo usan CharaLight,
  Fireworks y RoomLight) y `__RTTI__PFP12MapModel_unkPv_v` (.data 0x8078DE50, entre Courtyard y Fireworks).
- 0x8087DFC0/C4 (.sdata) usados por PassBoard (0x8042D140) y CharaPathMove (0x8043E134): compartidos, sin asignar.

## Resumen final
- `.text` `auto_` en el rango 0x8033C3D8-0x804405F8: antes 0x932B4 bytes (autoscan), despues 0 (calculado sobre
  splits.txt; el directorio build/SLSP01/asm conserva .s `auto_` viejos que autoscan sigue leyendo).
- Build limpio: `main.dol: OK`, regress `lost: 0` (matched 4757 -> 4758 por `fn_804225E0` de CharaLight).
- El aviso "Alignment for std/runtime_error.cpp .rodata" ya existia antes y no es de este rango.
