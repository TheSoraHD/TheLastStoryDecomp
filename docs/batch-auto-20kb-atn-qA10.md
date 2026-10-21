# batch-auto-20kb-atn qA10: atn (gestor de red Wi-Fi Connection)

Cola: `atn/NetManager`.

## atn/NetManager (commits 63c7c99, y el siguiente "Match atn/NetManager (71/73)")
- Resultado: 71/73 al 100% (+71 en el total), `matching: false`, `main.dol: OK`, regress lost: 0 (main e
  integracion), fuzzdrop 0.
- Contenido del fichero: `atn::net::Manager` (ctor/dtor/vf0C, inicializacion de DWC y sockets, update por
  estados, grupo abierto/cerrado, matchmaking con filtro "key = value", envio de paquetes, sincronizacion de
  la informacion de jugadores), los callbacks de DWC (envio/recepcion/timeout/ping/amigos/servidores/
  matchmaking), el hilo de trabajo (`NetThread_unk` en 0xBED0 y su funcion principal 0x8050CFC8) y el cliente
  de ranking del juego (`NetRanking_unk`, instancia lbl_8087F5A4, DWC_Rnk*).
- NONMATCHING:
  - `Manager::Manager` 97.92%: solo registros del bucle que construye los 8 `NetPlayerInfo_unk` (el puntero de
    fin va a r31 aqui y a r27 en el original). Probadas variantes del ctor del elemento (lista/cuerpo) y
    array plano / `FixedArray_unk`.
  - `fn_8050CFC8` (hilo) 98.47%: el comando 5 relee `lbl_8087F628` para `m260` (aqui reutiliza `mgr`), y los
    comandos 8/default reutilizan `mgr + 0x10000` del switch (r4) en vez de recalcularlo.
- **Decisiones sin el usuario**
  - Splits: sin cambios (el split ya cubria .text, pool, .data y .bss; no tiene .sdata/.sbss/.sdata2 propios:
    `lbl_8087F628`/`lbl_8087F62C` estan en `MP/NetGlobals_unk` y los objetos debiles en `atn/PacketPack`).
  - Nueva cabecera `include/atn/net/Manager.h`: `atn::net::Manager` (layout completo hasta 0xC21C),
    `NetCallbacks_unk` (0x98, 10 punteros copiados en bloque), `NetFilterKey_unk` (0x24: dos `std::string`),
    `NetPlayerInfo_unk` (0x34, mensaje 6), `NetErrorQueue_unk` (0xBE80, 8 errores; se descarta el mas viejo),
    `NetThread_unk` (0xBED0). Copias minimas de `NetStats_unk`/`MpFriendList_unk` (definidas en
    `src/atn/NetFriend_unk.cpp`, que no se toca) con dos inline nuevas (`AddSend_unk`/`AddRecv_unk`). Todos los
    nombres de miembros son inventados.
  - `vf10`/`vf14` de Manager son inline identicas (copias debiles 0x8050D3C4 aqui y 0x804C134C en
    D2AMpMenuLobby). Los jugadores son `atn::FixedArray_unk<NetPlayerInfo_unk, 8>` en 0x270.
  - Funciones `extern "C"` con su nombre `fn_` porque otros .cpp las llaman asi: `fn_805089E0` (cifrado AES de
    paquetes, PPDATAU), `fn_80509A88`/`fn_80509A90` (mutex, DwcAllocator_unk; reciben `Manager*`),
    `fn_8050B7EC` (envio, PPDATAU; llama al inline `Manager::Send_unk`), `fn_8050BA68` (aids conectados).
  - Funcion inventada que no esta en el DOL: `Manager::Stripped1_unk` (`strcpy(m10, "")`), eliminada por el
    enlazador; solo explica el `""` del pool tras `" and "` (0x8075B0C4), que reutiliza 0x8050CDD0. Los otros
    dos ceros del pool se explican con los formatos `"%s = %s\0"` y `"%s = %d\0"` (NUL explicito).
  - Literales de inline (`@STRING@`, debiles, en .data): `"lw_debug = 1"` (`__SetDebugFilter_unk`, este
    fichero), `"lw_debug"` y el `static int` 1 (`__SetDebugKey_unk`) y `L"<none>"` (ctor de
    `NetPlayerInfo_unk`) emitidos antes por `atn/PacketPack`. Estatico local de inline `__NetMsgStatic_unk`
    (guarda + objeto en .sbss de PacketPack, registro del destructor `@772` en la .bss de esta unidad); su
    destructor `~NetMsgStatic_unk` (0x804D8188) es de PacketPack. Plantilla inventada `NetMsg_unk<T>`
    (tamano + id + payload) para los mensajes 6 y 0x10.
  - `((void)0)` x16 (asserts de release vacios) en `fn_8050AA6C`/`fn_8050AA98`: sin ellos se autoinlinean en
    sus envoltorios `fn_8050AA8C`/`fn_8050AAD0` y el original los llama con `b`. Mismo recurso aceptado que en
    ColliAttrManager/dwc_match (revisar si se acepta aqui).
  - `symbols.txt`: las 68 funciones con nombre de clase renombradas a mano (pairnames se desalinea por las
    copias debiles); estaticas con `scope:local`. Datos: `lbl_8075B090` -> `@4021`, `lbl_8075B098` ->
    `@stringBase0` (0xE4), `lbl_80793108` -> `@STRING@__SetDebugFilter_unk__FPc`, `lbl_807C8FC8` -> `@772`
    (0xC), `lbl_807C8FD8`/`lbl_807C8FE0` -> `@LOCAL@...@sArgs`/`@sAids`. Fuera del split (solo nombres, en
    PacketPack/NetGlobals): `lbl_80791B00`/`lbl_80791B0C`/`lbl_8087E160` -> `@STRING@`/`@LOCAL@` de las inline,
    `lbl_8087F5F8`/`lbl_8087F5FC` -> guarda/objeto de `__NetMsgStatic_unk`, `fn_804D8188` ->
    `__dt__16NetMsgStatic_unkFv`, `fn_804DBC80` -> `__dt__Q33atn3net16NetFilterKey_unkFv`, `fn_804C134C` ->
    `vf14__Q33atn3net7ManagerFUi`. Los nombres `@4021`/`@772` dependen de la numeracion del compilador.
  - El nombre RTTI "atn::net::Manager" lo emite PacketPack (0x80759E18); aqui sale como local en .rodata y el
    enlazador tendra que descartarlo (pendiente al enlazar la unidad).
  - Nombre real propuesto: sin pruebas nuevas; el fichero tambien contiene el cliente de ranking y el hilo.
- Hallazgos MWCC
  - Indexar `this->arr[i]` con `int i` en un bucle con llamadas reduce el puntero (`p += 0x34`); con
    `arr[(unsigned int)i]` queda `this + i*0x34` recalculado y desplazamiento del miembro, como el original.
  - Una inline que devuelve `u8` de una funcion `int` necesita el cast explicito `(unsigned char)f()` para que
    el resultado vaya a r0 y se repita el `clrlwi` en la comparacion.
  - Leer un bitfield y reasignarlo (`int f = m.b; ... m.b = f;`) se pliega a `rlwimi r0,r0,0` salvo que la
    lectura este antes del `if` que guarda la asignacion (entonces sale `extrwi` + `rlwimi` con rotacion).
  - Funcion `static`, global o `extern "C"` de 2 sentencias: se autoinlinea igual en un envoltorio que la
    llama en cola; hacen falta ~16 `((void)0)` para que salga `b f`.
  - `""` no se fusiona con la cola de otra cadena (ni con un `"\0"` explicito): cada `""` nuevo ocupa su byte
    en el pool en orden de primer uso.
- Pendientes
  - Enlazar la unidad: faltan 2 funciones (ctor y hilo) y comprobar que el enlazador descarta el nombre RTTI
    local y las copias debiles (`vf14`, destructores de `std::string`/`NetFilterKey_unk`/`NetThread_unk`).
