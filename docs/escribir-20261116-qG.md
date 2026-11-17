# escribir-20261116, cola G (atn/PacketPack)

Inicio: 2026-11-16 23:58 CET (tope 2026-11-17 02:28).

## Tabla de la cola

| funcion | antes | despues |
|---|---|---|
| fn_804EE040 | 99,49 % | 99,49 % (sin recuperar, ver Bloqueos) |
| fn_804D00DC | DRAFT 5,6 % | **100 %** |
| __dt__Q22MP8Manager2Fv | DRAFT 33 % | DRAFT 74,6 % (cuerpo completo; falta destruir miembros) |
| fn_804E7AC0 | DRAFT 0 % | equivalente NONMATCHING 0 % (comprobada contra el asm entero) |
| fn_804D8910 | 0 % | equivalente NONMATCHING 98,91 % |
| fn_804D90E0 | 0 % | equivalente NONMATCHING 94,97 % |
| fn_804D9C14 | 0 % | equivalente NONMATCHING 94,17 % |
| fn_804DCA54 | 0 % | equivalente NONMATCHING 98,46 % |
| fn_804E82C4 | 0 % | equivalente NONMATCHING 95,09 % |
| fn_804E8BF4 | 0 % | equivalente NONMATCHING 97,42 % |
| fn_804E9360 | 0 % | equivalente NONMATCHING 86,98 % |
| fn_804EF0F0 | 0 % | equivalente NONMATCHING 98,47 % |
| fn_804F3EEC | 0 % | equivalente NONMATCHING 75,22 % |
| fn_804E5BC4 | 0 % | equivalente NONMATCHING 97,31 % |
| vector<PPPlayer_unk> (fn_804D6688, fn_804FC8CC, fn_804FC9C0, fn_804FCA34, fn_804FE2DC, fn_804FF3D0) | 0 % | 0 % (pendiente) |
| fn_804DE884 | 0 % | equivalente NONMATCHING 96,27 % **en el parche** `docs/escribir-20261116-qG.patch` (no commiteado, ver Bloqueos) |
| fn_804E4EB0, fn_804EB934, fn_804E9C64, fn_804EFF74 | 0 % | 0 % (pendiente) |
| fn_804CF640 | DRAFT (stub vacio) | equivalente NONMATCHING 90,7 % |
| vf10 de Manager2 | DRAFT 95 % | DRAFT (pendiente: 14,6 KB sin comprobar) |

PacketPack al 100 % (fnstat): 270 -> 271 / 380 (fn_804D00DC). Sin escribir 23 -> 13 (14 con fn_804DE884 en el parche); DRAFT 5 -> 2.

## Para el port

- `GameSaveInventory_unk::fn_803BD0FC` y `GameSaveParty_unk::fn_803BAE54` devuelven `bool` (antes `int`; include/game/GameSaveData.h y game/GameSaveData_unk.cpp, mismo codigo).
- `MP::Manager2::~Manager2`: ahora con su cuerpo real (sonido, espera y parada del hilo de red, borrado de ManagerMP, globales, flags de HomeButtonMenu/WPad/ErrorManager); cambio de comportamiento. `PPTable_unk` con destructor (delete[] de mFC).
- fn_804D00DC: deja de devolver 0, prueba el teclado de depuracion (`Keyboard::Press` con el puerto -2, como GameTask).
- fn_804E82C4 devuelve `int` (antes declarado `void`). `D2AMpMenuCockpit::fn_804B671C(int, int)` declarado en la vista.
- Nuevos en PacketPack: `PPMsgEquip_unk` (movido antes, tambien payload de 0x1009), `PPMsgWeapon_unk`, `PPMsgCpuTable_unk`, `PPMessageRef_unk`, `PPMsgVoice_unk`, `PPMsgGimmick_unk`, `PPMsgGimmickAnswer_unk` (pack 1), tabla `lbl_80791018[36]` (voces VO_AR*), inlines `SendEquip_unk`, `SendWeapon_unk`, `PPSendGimmick_unk`, `PPCharaOfAid_unk`, `PPTeamSize_unk`, `PPSlot_unk`; `GetNumHosts_unk`, `GetHostAid_unk`, `FindByChara_unk`, `FindCharaIndex_unk`, `GetMyPlayer_unk`, `PPMessages_unk` y el include de MP/MpMessage_unk.h movidos antes (mismo codigo).
- Literales: tabla de categorias por etapa de fn_804E82C4 `{0,2,3,4,6,5,1,8,7,9,14,17,17,17,16,16,16,11,11,12,18,18,0,13,19,0}`; flotante 2.0f de los mensajes de caida de las balas de canon (fn_804E9360).

- fn_804CF640 deja de ser un stub vacio (cambio de comportamiento): aplica el estado empaquetado del personaje; firma `(PackedChara_unk*, CharaTask*)` (antes `(void*, void*)`, sin `noinline`); los tres llamadores pasan el buffer con un cast. Inlines nuevas `PPSetTargets_unk`, `PPSetTarget_unk`, `PPCharaOfId_unk` (definidas despues de MP::Manager2 y auto-inlineadas).

## Decisiones tomadas sin el usuario

- `if (m9770 == 0) m9770 = 0;` en ~Manager2: es lo que hace el original (store condicional); se deja literal.
- fn_804F3EEC llama a fn_804DB1C4 en vez de inlinearla: inlineada (93,67 %) su `L""` pasa a un objeto propio de `.sdata` y desplaza `@wstringBase0`, y bajan fn_804FA7C8/88C/8E8/FAF48.
- `PPPlayerFlags_unk` local puesta a 0 por palabra (`*(unsigned int*)&flags = 0`) en fn_804E9360 para limpiar la slot con m10 = 2.

## Hallazgos

- Con `-ipa file` una `static inline` declarada arriba y definida mas abajo (despues de los tipos que necesita) se inlinea igual en una funcion que va antes en el `.dol`: sirve para escribir en su sitio funciones tempranas que tocan clases definidas despues (PacketPack fn_804CF640) (escribir-20261116 qG)
- `if (bit) { if (!state) on(); } else if (state) off();` frente a `if (bit && !state) on(); else if (!bit && state) off();`: la segunda deja el `cmpwi bit, 0` repetido del original (PacketPack fn_804CF640) (escribir-20261116 qG)

- `bool` devuelto por las funciones llamadas: `ok = f(); if (ok) ok &= g();` da `and; neg; or; srwi` sin normalizar el primero solo si `f`/`g` devuelven `bool` (con `int` sale `neg/or/srwi` tras cada uno) (escribir-20261116 qG)
- El `L""` de una funcion inline sale como `@STRING@<inline>` en `.sdata` (objeto propio de 2 bytes), como el `@wstringBase0` sda21 que usan todas las funciones de PacketPack en el original (escribir-20261116 qG)

## Bloqueos

- PacketPack fn_804EE040: pasar el tipo `(unsigned char)` a fn_804F1564/2040/2450 (o parametros `unsigned char`) deja el `clrlwi r4` del original pero rota la asignacion de registros salvados (98,87 %); probados `unsigned char`/`unsigned int`/`char` locales y declarados en el bucle (escribir-20261116 qG)
- PacketPack: prueba `my == server` materializada (cntlzw) en fn_804E9360/fn_804EF0F0; probados `IsServer_unk()`, `== 1`, fn_804EB1AC (escribir-20261116 qG)
- PacketPack fn_804E9C64: inlinea `push_back` de `std::vector<PPCharaEntry_unk>` en `mCharas` (vista `PPCharaArray_unk`): necesita el vector real (escribir-20261116 qG)

- PacketPack fn_804DE884 (parche): el ctor de `MP::Chara<EnemyTask>` inlineado hace emitir la vtable y con ella copias debiles de virtuales inline de CharaTask.h (`vfE0`, `vfA0`, `vf70`) cuyos floats (1.5708, -1.5708, 2.44346, 25) entran en la `.sdata2` de PacketPack (poolcheck nuevo); el original no los tiene: esas virtuales no serian inline en CharaTask.h. El parche aplica sobre 0f9e820 (incluye el ctor inline y `Create_unk`, `PPSlot_unk` movida, `fn_804C566C` declarada); tras aplicarlo, pd.sh y pdfix de 0x807596F0, 0x807598B0.. (35 VO_) y los `.bss` 0x807C8AE8/AF4/B00 (escribir-20261116 qG)

## Cambios de splits/symbols

- `@stringBase0` de MiniMapTask: el `data:string_table` se colo en ba48f1e (precommit re-lanza configure); quitado de nuevo.
- Re-etiquetado `@N` (pd.sh) y a mano la base de `.rodata` 0x807596F0 y los `.bss` 0x807C8AE8/AF4/B00 (`/home/claude/scratch-G/bin/pdfix.py 0xADDR=@N`, por direccion; `lcmiss.py <unidad>` lista los que faltan).
- `lbl_80759CC8` -> `@N` local (tabla local de fn_804E82C4); `lbl_807598B0..80759AD0` (35 cadenas VO_) -> `@N` locales de la tabla `lbl_80791018`.

## Scripts (para /home/claude/agentes/bin/)

- `/home/claude/scratch-G/bin/pct.py <unidad> <simbolos...>`: porcentaje de objdiff-cli por simbolo.
- `/home/claude/scratch-G/bin/pdfix.py 0xADDR=@N ...`: renombra por direccion en symbols.txt.
- `/home/claude/scratch-G/bin/lcmiss.py <unidad>`: etiquetas `@N` del split que faltan en el .o y los `@N` de .bss/.rodata del .o.
