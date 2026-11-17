# escribir-20261116, cola G2 (atn/PacketPack, segunda pasada)

Inicio: 2026-11-17 02:00 CET (tope 04:30).

## Tabla de la cola

| funcion | antes | despues |
|---|---|---|
| resize de `std::vector<PPPlayer_unk>` (0x804D6688) | sin escribir | equivalente NONMATCHING 4,4 % (plantilla real) |
| `__vec_pp_unk::__grow_by_unk` (0x804FC8CC) | sin escribir | **100 %** |
| `__vec_split_unk<PPPlayer_unk>` dtor (0x804FC9C0) | sin escribir | **100 %** |
| `__vec_pp_unk::__append_n_unk` (0x804FCA34) | sin escribir | equivalente NONMATCHING 94,8 % |
| `__vec_pp_unk::__push_front_unk` (0x804FE2DC) | sin escribir | equivalente NONMATCHING 97,96 % |
| `__vec_pp_unk::__reserve_unk` (0x804FF3D0) | sin escribir | **100 %** |
| fn_804E9C64 | sin escribir | equivalente NONMATCHING 72,06 % |
| __dt__Q22MP8Manager2Fv | DRAFT 74,6 % | equivalente NONMATCHING 97,72 % |
| __ct__Q22MP8Manager2FPQ23atn4Task | 89,53 % | 91,75 % |
| fn_804D6058 (clear de la lista) | 87,69 % | 90,83 % |
| fn_804DE884 (parche CharaTask.h) | 0 % | sin tocar (pendiente) |
| fn_804E4EB0, fn_804EFF74, fn_804EB934 | sin escribir | (ver abajo) |
| vf10 de Manager2 | DRAFT 95 % | DRAFT (sin comprobar) |

## Para el port

- `MP::Manager2::mPlayers`/`mCharas`: ahora `std::vector<PPPlayer_unk>` / `std::vector<PPCharaEntry_unk>` (antes vistas `{mData, mNum}` + `m5EC`/`m5F8` sueltos). Bucles con `(int)v.size()` y `v[i]`; `PPFindTeam_unk(&players, aid)` libre (antes metodo de la vista). Cambia el layout (mismo tamano: 12 bytes cada uno).
- `vector<PPPlayer_unk>::resize` especializado en PacketPack (los slots nuevos son copias de uno construido por defecto); helpers `std::__vec_pp_unk<T>` (crecimiento, reserva, slots nuevos, traslado). fn_804D6688 deja de ser un `extern "C"` sin cuerpo: cambio de comportamiento para el port (antes no se definia).
- fn_804E9C64 escrito (antes sin cuerpo): reparte los enemigos entre los hosts y rellena `mCharas` (cambio de comportamiento).
- `~Manager2` destruye sus miembros: `m510` es `ResHandle` (antes `void*`, carga con `ResHandle::Load`), `m610` es `PPLevelData_unk` (tabla `PPLevelTable_unk : MpLevelTable_unk` con `delete[]`; antes `unsigned char[0x8C]`; llamadas con `&mgr->m610` / `&mgr->m610.mTable`), `m99A4` lista con destructor (`PPListBase_unk/PPListImp_unk/PPList_unk`), `m2B88`/`m2B8C` pasan a `atn::FixedArray_unk<PPPlayer_unk, 8> m2B88` (`push_back`, `clear`, `size`).
- fn_804F2450: sin cambios de comportamiento.

## Decisiones tomadas sin el usuario

- Bajada de registros aceptada: fn_804F2450 98,36 -> 98,21 % (cambio de r28/r29 entre `mgr` y el jugador de la busqueda inline al pasar `mPlayers` a `std::vector`). Probadas: busqueda como metodo de una vista derivada (98,17), con campos (98,32, pero bajan fn_804F2258 y fn_804EE4F0), `begin()[i]`, puntero/referencia, recarga con `fn_800F7F90()`, `mgr` antes. Se commitea porque el cambio desbloquea las 6 copias del vector (3 al 100 %) y fn_804E9C64. Si no se acepta: revertir dfccb07..HEAD no sirve (todo depende del vector); habria que buscar otra forma para fn_804F2450.
- `memset(&entry.m04[12], 0, 0xB0 - 0x34)` en fn_804E9C64 (el original limpia solo 0x34-0xAF de la entrada).
- `__vec_pp_unk` y la especializacion de `resize` van en PacketPack.cpp (solo los usa esta unidad), como `include/Event/EventVector_unk.h` de W3.

## Hallazgos

- Explicit specialization de un miembro (`template <> inline void vector<X>::resize(size_type)`) en el .cpp: compila y sale como la copia debil `resize__Q23std...`; si su cuerpo pasa del presupuesto de inline (R3) se llaman todos sus candidatos, incluidos `max_size`/`capacity` (escribir-20261116 qG2)
- Copias de una entrada con constructor declarado: el operador= implicito copia miembro a miembro, y un struct anidado con constructor declarado se copia en bloque (bucle de palabras) tanto en la copia como en la asignacion: no sirve para separar las dos (PacketPack fn_804E9C64 frente a fn_804E7434) (escribir-20261116 qG2)
- Destructor de un miembro con N comprobaciones de `this` (`addic.; beq` repetidos): N niveles de destructor (clases base con `~X() {}` vacio), no niveles de funciones inline (PacketPack m99A4, m610) (escribir-20261116 qG2)
- `v[i]` con `int i` sobre `std::vector` (operator[] de `size_type`) puede cambiar la asignacion de registros frente a `v.begin()[i]` (PacketPack fn_804E9360) (escribir-20261116 qG2)
- Rango `0xAAE60 <= x && x < 0xAAE7A` deja las dos comparaciones con signo del original (con `x >= a && x < b` sale `subis/cmplwi`) (PacketPack fn_804E9C64) (escribir-20261116 qG2)

## Bloqueos

- PacketPack resize de `vector<PPPlayer_unk>` (0x804D6688): el original inlinea el slot nuevo (constructor + copia) y la reasignacion (solo llama a crecimiento, reserva, slots nuevos, traslado y dtor del buffer); el nuestro pasa del presupuesto y lo llama todo. Una inline aparte para la reasignacion la inlinea entera (con crecimiento/reserva/traslado dentro) y deja una funcion debil de mas; sin ella el bucle en sitio tambien sale fuera (escribir-20261116 qG2)
- PacketPack fn_804E9C64: la copia de la entrada en el push_back es en bloque para 0x34-0xAF en el original y miembro a miembro en fn_804E7434 (100 %); un struct anidado da 95,6 % en fn_804E9C64 pero rompe fn_804E7434 (escribir-20261116 qG2)

## Cambios de splits/symbols

- Renombrados: fn_804D6688 -> `resize__Q23std22vector<12PPPlayer_unk>FUi`; fn_804FC8CC/FC9C0/FCA34/FE2DC/FF3D0 -> `__grow_by_unk__Q23std28__vec_pp_unk<12PPPlayer_unk>...`, `__dt__Q23std31__vec_split_unk<12PPPlayer_unk>Fv`, `__append_n_unk...`, `__push_front_unk...`, `__reserve_unk...`; `lbl_80791764` -> `@STRING@__allocate__Q23std22vector<12PPPlayer_unk>FUi`.
- Re-etiquetado `@N` (pd.sh) y por direccion (pdfix): 0x807596F0, las 35 VO_ 0x807598B0.., los `.bss` 0x807C8AE8/AF4/B00 y las tablas de saltos 0x807911B4 / 0x8079120C (antes `jumptable_*`).

## Scripts

- `/home/claude/scratch-G2/bin/fixlbl.py` (desde la copia): tras `pd.sh atn/PacketPack`, re-etiqueta por direccion los objetos que pd deja sin emparejar (tabla 0x807596F0, 35 VO_, 3 `.bss`, tablas de saltos de fn_804E7628 y fn_804E9C64).
- `/home/claude/scratch-G2/bin/ours.py <unidad> <simbolo> [left|right]`: instrucciones de un lado del diff de objdiff.
