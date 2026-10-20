# splits-auto agente D (rango .text 0x804405F8-0x805BFAC0)

## Grupo 1: Event/Manager y NewCamp* (verificado: main.dol OK, lost: 0)

### 0x80479998-0x80490EB8 (auto_03_80479998, Event/Manager, auto_03_80488050, auto_03_8049003C, auto_fn_80490850, auto_03_80490884)
- `game/main.cpp` (nuevo, 0x80479998-0x8047B768): contiene `main` (0x80479A18), sus helpers (OSGetTime de arranque/fin de frame, sbss 0x8087F530-0x8087F540) y las funciones boost::function de ErrorManager/ResetShutdown. Datos propios contiguos: .rodata 0x80756168-0x80756310 (nombres de tipo boost `void (*)(void *)`, `bind_t<..ErrorManager..>`, `bind_t<..ResetShutdown..>`, tabla 0x8075626C), .data 0x80790060-0x807900A0 (functors), .bss 0x807C8A98-0x807C8AB0, .sdata 0x8087E078-0x8087E088, .sdata2 0x80886F70-0x80886F80 (0.6f, 1.0f, 0.0f propias: duplicadas con las de Event/Manager -> corte).
  - Corte main/Event::Manager en 0x8047B768: constantes 0/1 duplicadas (0x80886F74/78 frente a 0x80886F8C/90), y a partir de 0x8047B768 las funciones las llama codigo de Event (0x8048871C, 0x80488B5C).
- `Event/Manager.cpp` (ampliada, sin fuente): .text 0x8047B768-0x8048FF60 (todo el tramo auto anterior desde el corte + el tramo auto_03_80488050 posterior, hasta Event/PostSequence). Prueba: un solo pool de cadenas 0x80756380 (Event, DebugCamera, ... `event2/%s.event`, `/game_scene/%s.gs`) usado por todas las funciones del tramo; una sola copia de 0.0f/1.0f (0x80886F8C/90) en todo el rango; ctor del Manager (vtable) en 0x8047DF70; sInstance del Manager 0x8087F540.
  - Datos: .rodata 0x80756310-0x80756358 (GROUP_SYSTEM/GROUP_PC001, tabla, dobles int->float, nombre RTTI), .data 0x807900A0-0x807900F8 (jumptable de 0x80489D08 + vtable), .sdata 0x8087E088-0x8087E0B8, .sbss 0x8087F540-0x8087F550 (sInstance de Manager y, por contigüidad, los de SequenceRoot/PostSequence, que el ctor del Manager escribe), .sdata2 0x80886F80-0x80887048.
- `Event/Manager_cont.cpp` (nuevo, `_cont`): 0x8049003C-0x80490884, tras la unidad enlazada Event/PostSequence (matching: true, no se tocan sus limites). Usa el mismo pool 0x80756380 (+0x1C8: "vector length error"), y su `__sinit` 0x80490850 (ctors 0x8072D380) cierra el fichero. Datos: .rodata 0x80756380-0x80756690 (el pool entero), .data 0x80790168-0x807901D8 ("Memory allocation failure" x4), .bss 0x807C8AB0-0x807C8AC8, .sdata2 0x80887048-0x80887050.
- `game/MapInstance_unk.cpp` (nuevo, nombre INVENTADO): 0x80490884-0x80490EB8, 5 funciones de una instancia de modelo (atn::ModelInstance) que solo usa game/MapPartsLocatorSet; tiene sus propias copias de 0.0f/1.0f (0x80887054/58) -> fichero distinto del Event/Manager y de MapManager (cuyo 1.0f es 0x80887060). Datos: .sbss 0x8087F550 (contador), .sdata2 0x80887050-0x80887060.

### NewCamp* (0x805169F8-0x8053108C)
- Patron: cada fichero NewCampParts* empieza por su creador `fn_xxx` (0x64 bytes, `new` + ctor, usa el pool del fichero) y acaba en su `__sinit`; dtk los habia dejado fuera.
- `game/NewCampPartsItem.cpp` (ampliada, sin fuente): .text 0x805169F8-0x8051B920 (creador + `__sinit` 0x8051B900), .ctors 0x8072D3B0, .rodata 0x8075B988-0x8075BD28 (cadenas de la tabla, tabla 0x8075BAD0, "item_board", nombre RTTI, pool 0x8075BB80), .data hasta 0x80793508 ("Memory allocation failure", L"%s"), .bss 0x807C90F0, .sdata 0x8087E488-0x8087E498, .sdata2 0x808878B0-0x808878E8.
- `game/NewCampPartsMapSelect.cpp` (ampliada): .text 0x8051B920-0x80521E1C (creador + `__sinit` 0x80521DF8), .ctors 0x8072D3B4, .rodata 0x8075BD28-0x8075C260 (pool 0x8075BF40), .data hasta 0x80793598 (L"???"), .bss 0x807C9100, .sdata2 desde 0x808878E8.
- `game/NewCampPartsOption.cpp` (ampliada): .text 0x80521E1C-0x805268C8 (creador + tramo auto_03_80526358, que usa el pool 0x8075C6C4 y la jumptable 0x807935E0), .rodata 0x8075C260-0x8075CC30, .data desde 0x80793598 (3 jumptables), .sdata 0x8087E498, .sdata2 desde 0x808879E8.
- `game/NewCampTask.cpp` (ampliada, sin fuente): .text 0x8052A888-0x8053108C (creador, tramo auto_03_8052C108, `__sinit` 0x80530FB0 y su destructor de objeto global 0x80531014, que solo referencia el __sinit), .ctors 0x8072D3BC, .rodata hasta 0x8075D450 (pool 0x8075D31C), .data 0x807937E8-0x80793888 (2 jumptables + vtable + "Memory allocation failure"), .bss 0x807C9120-0x807C9140, .sdata 0x8087E4A4-0x8087E4B0, .sdata2 0x80887AB0-0x80887B30.

### Decisiones sin el usuario (grupo 1)
- Nombre inventado `game/main.cpp` (no hay RTTI; contiene el simbolo `main`).
- `Event/Manager_cont.cpp`: continuacion de Event/Manager tras la unidad enlazada Event/PostSequence. Hay que revisar: o PostSequence/SequenceRoot estaban en Event/Manager.cpp (el pool y las constantes son comunes), o las funciones de `_cont` son otro fichero; las pruebas (mismo pool) apuntan a que todo es un solo fichero.
- Nombre inventado `game/MapInstance_unk.cpp`.
- La .sbss de Event/Manager incluye sInstance de SequenceRoot/PostSequence (0x8087F544/48) porque un split es contiguo.
- La funcion 0x80531014 (tras el __sinit de NewCampTask) se ha dejado en NewCampTask: es el destructor registrado por su __sinit (__register_global_object), no un fichero aparte.

### Hallazgos
- Los creadores `fn_xxx` de 0x64 bytes justo antes de cada split NewCampParts* son la primera funcion del fichero (usan el pool del fichero).
- Un `__sinit` puede ir seguido de la funcion destructora que registra (copia generada).
