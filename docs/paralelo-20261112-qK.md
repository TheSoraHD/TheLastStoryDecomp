# Lote paralelo 2026-11-12, cola K (red del lobby y restos)

Inicio: 2026-11-12 18:25 CET; corte del servidor 18:33-20:16; reanudado 20:16, tope ~22:35.

## Resultado por unidad (funciones al 0 % / sin escribir, antes -> despues)

- `MP/D2AMpMenuLobby`: 8 -> 1 (queda `fn_804BC628`, 5 KB). Escritas, todas `NONMATCHING`:
  - `vf10` (update, switch de estados) 98,6 %: registros (this r30/r31) y los temporales de `ShowRules_unk`.
    Las reglas del anfitrion son una `static inline ShowRules_unk(lobby)` comun con `fn_804C0094` (el codigo
    es identico en las dos); como miembro inline sus literales irian a `.data` (peor). Contador estatico local
    del estado 7 (`lbl_8087F5C8`).
  - `fn_804C0094` (lado del invitado de las reglas del anfitrion) 98,6 %: registros del id del mensaje (r4/r5)
    y de los temporales layout/modo (r26/r27).
  - `fn_804BFB54` (coloca los huecos de jugador en los "profile_null0N") 99,9 %: solo el offset de
    "profile_null0%d" en `@stringBase0` (faltan las funciones anteriores). Inline miembro
    `Player_unk::SetPos_unk` definido `inline` en el `.cpp` (sus literales van a `.data` como
    `@STRING@SetPos_unk...@N`, en orden inverso: casa con `lbl_80790D10..D68`).
  - `fn_804BE610` (update del lobby) 99,9 %: registros volatiles de la busqueda del miembro (como vf0C).
  - `fn_804BEDB8` (update del estado 6) 99,6 %: el original recarga `lbl_8087F610` tras mirar el tiempo;
    busqueda del miembro.
  - `fn_804C18A8` (refresca los huecos desde la sesion) 95,4 %: asignacion de registros del bucle; el rango
    del personaje sale como un `cmplwi` (el original hace dos `cmpwi`); `(mode == 1)` de "drawFlg1" se
    calcula tras el hash.
  - `fn_804C23E4` (sorteo del mapa) 95,8 %: el relleno de `mE14` sale desenrollado entero (el original 8x2);
    busqueda del miembro.
- `AI/AI_archer_attack01`, `game/GameEventScript`: no tocadas (sin tiempo).

Funciones al 100 % nuevas: ninguna. Ninguna unidad pasa a enlazar.

## Decisiones tomadas sin el usuario

- **Regresion aparente aceptada**: `fn_804C0F88` pasa de 100 % en `report.json` a 99,98 % y `fn_804C2D5C`
  baja 98,31 -> 98,29. No es una perdida real: `objdiff-cli` ya la daba al 99,6 % (regla 1). El `L""` del pool
  ancho (`@wstringBase0`, `lbl_80790D74`) estaba en 0x18 por casualidad; con L"ON"/L"OFF"/L"%02d" de
  `fn_804C0094` pasa a 0x30. El orden real (ON, OFF, %02d, "", "$B+", "$Ba", ...) llega al escribir
  `fn_804BC628`, que usa esas cadenas antes que vf14. Por eso el ultimo `precommit` da FAIL solo en
  regress/fuzzdrop por estas dos; main.dol OK, labelcheck/rep100 iguales.
- Declaraciones nuevas en `include/MP/D2AMpMenuLobby.h` (fn_804BE610, fn_804BEDB8, fn_804BFB54,
  fn_804C0094, fn_804C18A8, fn_804C23E4 y `Player_unk::SetPos_unk`): renumeran `@N` en las unidades que la
  incluyen; `lblmap.py` pasado a atn/PacketPack, MP/D2AMpMenuFriendRoom, MP/D2AMpMenuMessageList y
  MP/D2AMpMenu (D2AMpMenu tenia 12 etiquetas sin casar en la referencia: ahora 0).
- `include/game/CharaTask.h`: `CharaInfo_unk::m2C` (indice del patron de cara) anadido al final (solo dato).
- La vista local `MpLobbySession_unk` del lobby crece (m564, m5A4, m5B0, `MpLevelTable_unk m610`, m999C);
  sigue siendo una vista por fichero (deuda previa de la regla 10, igual que antes).
- Bitfields de `MpLobbyPlayerInfo_unk` y `MpLobbyMember_unk` partidos en mas campos (mismos bits).

## Hallazgos (una linea)

- Una inline con parametro `int` convierte a float despues del hash del nombre (`xoris` tras la llamada);
  con parametro `float` la conversion va antes: `SetPaneInt_unk`/`SetPaneValue2Int_unk` en el lobby.
- Alfa 0/255 de una condicion: `(unsigned char)(c ? 0xFF : 0)` como `int` da `srawi 31; clrlwi 24; xoris`.
- Literales de una inline miembro definida `inline` fuera de la clase, en el `.cpp`: van a `.data` en
  objetos `@STRING@<func>@N` en orden inverso de uso; definirla en la cabecera renumera `@N` en todas las
  unidades que la incluyen, aunque no la usen.
