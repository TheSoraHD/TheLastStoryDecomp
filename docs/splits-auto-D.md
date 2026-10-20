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

## Grupo 2: zona de red 0x80502708-0x805128C0 (verificado: main.dol OK, lost: 0)
- `MP/SaveLoad_cont.cpp` (nuevo, `_cont`): 0x80502708-0x805028D8, 4 funciones sobre el objeto MP::SaveLoad (campos 0x48/0x4C/0x50/0x64/0x36C del layout de src/MP/SaveLoad.cpp); SaveLoad esta enlazada (matching: true), asi que no se tocan sus limites. Sin datos propios.
- `atn/SoftKeyboardU.cpp` (ampliada, sin fuente): .text desde 0x805028D8 (helper que solo usa el ctor de SoftKeyboard, tabla de teclas 0x8075A988), .rodata 0x8075A988-0x8075B000 (tabla de teclas, nombres boost::bind_t, RTTI, pool con /data/d2anime/online_name_*.d2a), .data 0x80792AE0-0x80792DC0 (tabla de teclas en u16 -> .sdata2 L"1"..), .bss 0x807C8FA8-0x807C8FC8, .sdata 0x8087E440-0x8087E458, .sbss 0x8087F848-0x8087F868, .sdata2 desde 0x808876B8.
- `MP/Text.cpp` (nuevo): 0x80505DD0-0x80507790. Vtable/RTTI MP::Text (0x80792DC0), "/database/online/message_table_xx.u16", pool begin/PVP/end/RAID/title/point (0x8075B038), L"%s" x2; corte final: a partir de 0x80507790 empieza otro conjunto de constantes (.sdata2 0x80887798..) y otro .rodata (0x8075B058).
- `atn/Bezier_unk.cpp` (nuevo, nombre INVENTADO): 0x80507790-0x805089E0, matematica de curvas (1/3, 2/3, 4/3, evaluacion polinomica, recursion), con sus propios 0.0f/0.5f/1.0f (.sdata2 0x80887798-0x808877C0) y dobles (.rodata 0x8075B058-0x8075B090). Lo usa solo 0x805BF214.
- `atn/NetManager.cpp` (nuevo, nombre INVENTADO a partir de la clase atn::net::Manager, vtable 0x807930F0): 0x805089E0-0x8050D3E8. Pool 0x8075B098 ("laststorywii", estados INITIALIZE/REGISTER/JOIN..., clave), 12 jumptables .data 0x80792E08-0x807930F0, "lw_debug = 1", .bss 0x807C8FC8-0x807C8FF0.
- `atn/DwcAllocator_unk.cpp` (nuevo, nombre INVENTADO): 0x8050D3E8-0x8050D7EC, 6 funciones con su propio pool ("DwcAllocator", "DWCAlloc", 0x8075B180) y su sbss 0x8087F878.
- `atn/PPDATAU.cpp` (ampliada, sin fuente): .text hasta 0x8050E67C (PPDATA/PPDATAU/PPDATAR + helpers que usan su pool "in adduser"/"assert in sendmessage" 0x8075B1D4), .rodata hasta 0x8075B1F8 (incluye nombre RTTI "atn::net::PPDATA"), .data hasta 0x807931A0 (vtable/RTTI PPDATA + "Memory allocation failure"), .sdata 0x8087E468-0x8087E480.
- `atn/NetFriend_unk.cpp` (nuevo, nombre INVENTADO): 0x8050E67C-0x8050F93C, temporizadores OSGetTime y lista de amigos: tabla L"Friend%02d"/OFFLINE/ONLINE/MATCH... (.data 0x807931B4), "lw_mode/lw_group/..." (0x807931A0), "online/playerinfo7" + cadenas basic_string (pool 0x8075B238), su doble int->float 0x8075B1F8 y su 0.0f (0x808877C0).
- `game/NewCampPartsBase.cpp` (nuevo): 0x8050F93C-0x805128C0. Ctor/dtor de NewCampPartsBase (simbolos con nombre) y metodos de su vtable (0x807932B8), pool 0x8075B2B8 (reference/OffsetPos/...), dobles 0x8075B2A8/B0 y constantes .sdata2 0x808877C8-0x808877E0.
- Cortes por constantes duplicadas en .sdata2/.rodata (cada fichero su copia: 0.0f 0x8088779C / 0x808877C0 / 0x808877CC; dobles 0x8075B078 / 0x8075B090 / 0x8075B1F8 / 0x8075B2A8) y por pools de cadenas separados.

### Decisiones sin el usuario (grupo 2)
- Nombres inventados: atn/Bezier_unk.cpp, atn/NetManager.cpp (por atn::net::Manager), atn/DwcAllocator_unk.cpp, atn/NetFriend_unk.cpp.
- `MP/SaveLoad_cont.cpp` sin datos: revisar si es parte de SaveLoad (fusion cuando se decompile) o de otro fichero MP.
- El corte DwcAllocator/NetManager (0x8050D3E8) y NetFriend/PPDATAU (0x8050E67C) son los menos seguros (se basan en pools/sbss separados, no en constantes duplicadas); NetFriend podria ser parte de PPDATAU.
