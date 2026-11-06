Inicio: 2026-11-06 11:36 CET (cierre minimo 2026-11-06 17:36)

# unidades-20261106b: librerias 2010 y unidades que faltan (sesion larga autonoma, tercera)

Rama `unidades-20261106b` desde `main` c6fdafc (13524 / 18117, 652 enlazados). Prompt: `claude/prompt-unidades-restantes.md`.
Referencia guardada antes de tocar nada en `/home/claude/ref/`: `report.json`, `labelcheck` de las 1051 unidades
con `.o` y split (164 lineas) y `rep100` (145 lineas, total 144).

## Resumen

| Que | Antes | Despues |
|---|---|---|
| Funciones al 100% (report) | 13524 | 13600 |
| Ficheros enlazados | 652 / 1304 | 652 / 1304 |

### Unidades y funciones nuevas

| Unidad | Funciones | Notas |
|---|---|---|
| `msc/src/RVL_SDK/kpad/KPADMplsMath_unk.c` (nueva, matching false) | 4/6 | rutinas de vectores/rotacion de Motion Plus (`fn_8065AED0`, `fn_8065AF20` movidas de KPADMpls; `fn_8065B630` (producto de rotaciones) y `fn_8065B7F0` (interpolacion) nuevas; `fn_8065AFB0` 90%, `fn_8065B320` 53%) |
| `msc/src/RVL_SDK/kpad/KPADMpls_unk.c` (matching false) | 4 -> 10/16 | area de trabajo de Motion Plus por canal (0xDB0) reconstruida; `fn_80658DF0`, `fn_80659070`, `fn_80659220`, `fn_80659B20/C10/D00` (por eje), mas `fn_8065AA80` 99,8%, `fn_8065A8B0` 98%, `fn_806594D0` 96%; falta `fn_80659DF0` (2752 B) |
| `MP/D2AMpMenuSetMessage.cpp` (nueva, matching false) | 0 -> 20/26 | las 26 escritas; NONMATCHING por registros: `fn_804C8D80` 98,4%, `vf10` 99,75%, `fn_804CB2F0` 99,93%, `fn_804CC618` 99,4%, `fn_804CC7A4` 99,85%, `fn_804CCB54` 99,87% |
| `MP/D2AMpMenu.cpp` (nueva, matching false) | 0 -> 18/41 | fabrica, ctor de `RANKING_DATA`, dtors, `vf1C`, `vf20`, `fn_804B4150`, `D2AMpMenuRankMark` (dtor, `vf0C`, `fn_804B4678/4700/4770`) y los 6 virtuales de las listas de ranking; ctor 99,2%, `fn_804B0A78` 99,95%, `fn_804B12F8` 99,97%; `fn_804B49C0` y `__sinit` solo por desplazamientos del pool de cadenas (faltan funciones) |
| `MP/D2AMpMenuLobby.cpp` (nueva, matching false) | 0 -> 15/39 | fabrica, ctor, ctor/dtor de `Player_unk`, dtor, `vf1C`, `vf20`, `fn_804BD9B8` (entrada en el estado `mD8C`), `fn_804BE330` (cursor y teclas), `fn_804C0548` (cambio de cara), `fn_804C0EA4`, `fn_804C2260`, `fn_804C28A0`, `fn_804C2F98`, `fn_804C2F9C`; `vf0C` 99,35% y `fn_804C2C14` 99,15% (bloqueo de la busqueda por aid, `docs/mwcc-blockers.md`) |
| `MP/D2AMpMenuFriendRoom.cpp` (matching false) | 8 -> 9/13 | `fn_804BBB60` (estaba en el split de Lobby) |
| `MP/D2AMpMenuMessageList.cpp` (matching false) | 7 -> 9/16 | `MpMessageTable_unk::fn_804C54FC` (inline en cabecera) y `__sinit` (`const sCustomData`); `fn_804C50C0` 92 -> 93,8% |
| `MP/D2AMpMenuResult.cpp` (matching false) | 12 -> 13/20 | `__sinit` (`const sCustomData`) |
| `msc/src/RVL_SDK/kpad/KPAD.c` (matching false) | 22 -> 33/33 | `KPADInsideStatus` 2010 completo (puntero 0x17C, ticks y Motion Plus 0x660-0x688); `reset_kpad(chan)`, `KPADInitEx`, `KPADiConnectCallback`, `KPADiRead`, `KPADiSamplingCallback`, `read_kpad_ext`, `read_kpad_acc`, callbacks WBC y `fn_80658300` (Motion Plus) desde el asm. Falta `fn_80657230` (61%) |

## Decisiones tomadas sin el usuario

- KPAD: campos nuevos de `KPADInsideStatus` con nombres inventados (`mplsBuf` 0x17C, `tickHist[3]`, `prevTick`,
  `curTick`, `cntHist[2]`, `unk_0x676`..`unk_0x67d`, `mplsCallback` 0x680, `unk_0x684`); `KPADStatus.mpls`
  (`KPADMPStatus`/`KPADMPDir` de 0x3C, nombres del SDK posterior) + `unk_0xEC[4]`. Estado WBC 2010 con nombres
  inventados (`kp_wbc_state`, `kp_wbc_err`, `kp_wbc_unk_d2/d5/d6/b0/b8/c0`, `kp_wbc_unk_tbl`); `kp_wbc_enabled` y
  `kp_wbc_state` son `s8`. Prototipos de `KPADMpls_unk.c` (`fn_8065AA50`, `fn_8065AA80`, `fn_8065AEB0`) declarados
  en `KPAD.c` con los tipos que usa `KPADiRead` (el fuente de Mpls aun los declara con `s32`).
- KPAD: `f64 kp_wbc_unk_bss[3]` global (0x18 bytes de `.bss` tras `kp_wbc_average` sin referencias en el `.dol`;
  global para que MWCC lo emita). Estaticos WBC con inicializador `= 0` y en el orden del `.sbss` original (MWCC
  pone primero, en orden de declaracion, los inicializados y despues, al reves, los que no lo estan).
- KPADMpls: estructuras inventadas `KPADMplsChan_unk` (0xDB0 por canal tras una cabecera de 8 bytes: `m000` y la
  callback), `KPADMplsCalib_unk` (= `WPADMplsCalibration` de KEY), `KPADMplsRaw_unk` (bytes 0x36-0x3D del estado WPAD
  de 2010), tablas `kp_mpls_unk_tbl_a/b`; `__declspec(noinline)` en `fn_80658F50` (el original la llama con `bl`
  desde `fn_806594D0`).
- `WPAD.c` (KEY): los seis stubs `fn_8065CED0`..`fn_8065CF20` eran los `WBC*` debiles del SDK (mismo orden y valores
  de retorno): quitados los `fn_` y los `WBC*` debiles de KEY ocupan su sitio (simbolos renombrados). KPAD los llama.

- `MpMessageTable_unk` (tabla de mensajes de `lbl_8087F610 + 0x2A50`) pasa a `include/MP/MpMessage_unk.h` con
  `fn_804C54FC` **inline**: SetMessage la expande en su sitio (con su llamada recursiva una vez) y MessageList emite
  la copia fuera de linea tras su primer uso. Con el rango `category < 0 || 21 < category` (constante a la izquierda)
  sale al 100% en MessageList. La clase `D2AMpMenuMessageList` pasa a `include/MP/D2AMpMenuMessageList.h` (SetMessage
  usa `sPage`).
- SetMessage: tablas de `.rodata` de la unidad: `sDefaultIds0/1` (`MpMsgIds_unk`, struct de 10 `int` copiado por
  valor a la pila: la copia en dos tandas de registros del original) y `sCharaIds[22]` (ids de personaje de los
  nombres; `GetCharaName_unk` estatica). `MpMsgData_unk`/`MpMsgSession_unk`/`MpMsgSettings_unk` son vistas parciales
  de `lbl_8087F628`/`lbl_8087F610`/`lbl_8087F0A8` como las de MessageList (no hay cabecera comun aun).

- Cabeceras nuevas con clases que estaban en su `.cpp`: `include/MP/D2AMpMenu.h` (`D2AMpMenu`, sus listas de
  ranking y `D2AMpMenuRankMark`), `D2AMpMenuFriend.h`, `D2AMpMenuFriendRoom.h`, `D2AMpMenuMessageList.h`,
  `D2AMpMenuSetMessage.h`, `MpMessage_unk.h`. Cockpit, SetRule, Result y FriendRoom usan `MP::D2AMpMenu::sInstance` y
  `D2AMpMenuRankMark` en vez de `lbl_8087F59C`/`lbl_8087F5A0` (simbolos renombrados; re-emparejados con pdbatch).
- `D2AMpMenuRankMark::D2AMpMenuRankMark` es una funcion normal detras del ctor del menu: MWCC la auto-inlinea y el
  enlazador quita la copia (`ordercheck` la da como extra). `RANKING_DATA::Clear` es inline de la clase (su `L""` va
  a `@STRING@Clear...`, como en el original).
- `MpMessage_unk` gana `mVoice` (0x00), `m04` (bits de las listas), `m08` (byte alto: personaje) y `m10` (hablante).
- SetMessage: la vista de `lbl_8087F610` (`MpMsgSession_unk`, en `D2AMpMenuSetMessage.h`) incluye `m4FC`/`m540` para
  que D2AMpMenu use el mismo tipo; `lbl_8087F860` (teclado) y `lbl_8087F1E4` siguen como vistas parciales.
- Jumptable de `D2AMpMenu::fn_804B0A78` renombrada `@3209` (antes `jumptable_8079098C`) para que objdiff la empareje.

- `include/MP/D2AMpMenuLobby.h` (nueva): clase `D2AMpMenuLobby` (0xF98) con `Player_unk mPlayers[6]` (0x22C,
  `mTasks[6]` y `Clear`), `mDD4[16]`/`mE14[16]`/`mE54` (lista de entradas) y `mTasks` en 0xE80. Vistas locales en
  `D2AMpMenuLobby.cpp`: `MpLobbySession_unk` (`lbl_8087F610`: `m508` tiempo, `m55C`, jugadores `m5E4`/`m5E8` con
  `FindMember_unk`), `MpLobbyMember_unk`, `MpLobbyPlayerInfo_unk` (`mB0`, cara en los bits 6-9 de 0xD0) y
  `MpLobbyData_unk` (`lbl_8087F628`: bit 12 de 0x1F8, `m25C`, `mC215`); `lbl_8087F0A8` como `GameConfig_unk`
  (`mInput`), como en `game/MapTask.cpp`. `fn_804A3C24` sigue como `extern "C"` con el menu de primer parametro,
  igual que en SetMessage, SetRule y ShopIdentify (deuda anterior: es de `MenuCommonTask`).
- SetRule: `GetMaxLives_unk` y `ReadTime_unk` declaradas en `include/MP/D2AMpMenuSetRule.h` (definidas `inline` en el
  `.cpp`) para que Lobby incluya la cabecera.

## Decisiones a revisar

- `lbl_8087F628` sigue con varias vistas parciales (`MpData_unk` de `D2AMpMenuFriend.h`, `MpMsgData_unk`,
  `atn::net::Manager` en `include/atn/net/Manager.h`, cada una con su `MpFriendList_unk`): no se pueden incluir juntas.
  Unificarlas (una sola clase `atn::net::Manager`) es requisito para seguir con `D2AMpMenu::fn_804AF520`.

## Cambios de splits

- `msc/src/RVL_SDK/kpad/KPADMpls_unk.c` se parte en dos: `KPADMpls_unk.c` `.text` 0x80658DB0-0x8065AED0, `.data`
  0x807B8A28-0x807B8A70, `.sbss` 0x808801E8-F0, `.sdata2` 0x80888938-0x808889C0; y `KPADMplsMath_unk.c` (nueva)
  `.text` 0x8065AED0-0x8065B8E0, `.data` 0x807B8A70-0x807B8A98 (rotacion identidad), `.sdata2` 0x808889C0-0x808889D0.
  Prueba: las funciones desde 0x8065AED0 usan otro pool (`@1502` 0.5f, `@1503` 0.0f, `@1508` 1.0f repetidos tras el
  0.0f/1.0f de KPADMpls) y numeros `@N` bajos (fichero pequeno). Build limpio `main.dol: OK`.

- `MP/D2AMpMenuFriendRoom.cpp` / `MP/D2AMpMenuLobby.cpp`: el limite de `.text` pasa a 0x804BBBA0 (`fn_804BBB60` es
  de FriendRoom: usa sus tareas en 0x88 y llama a la fabrica de Lobby, que empieza en 0x804BBBA0) y el de `.sdata2` a
  0x808873F8 (el `0.0f` de Lobby). Build limpio `main.dol: OK`.

## Renombrados en symbols.txt

- KPAD/KPADMpls/KPADMplsMath: `fn_`/`lbl_` de las funciones y datos escritos (nombres del SDK donde existen,
  `fn_XXXXXXXX` con su firma en C si no); los seis `fn_8065CED0..CF20` de WPAD pasan a `WBCSetupCalibration` ...
  `WBCGetTGCWeight` (los debiles de KEY).
- MP: todas las funciones escritas de `D2AMpMenuSetMessage` (`vf10`, `vf14`, `fn_804C9C88__...Fii`, ...),
  `D2AMpMenu` (`fn_804AE820__...`, `__ct__/__dt__` del menu, de `RANKING_DATA` y de `D2AMpMenuRankMark`, `Get`/
  `GetCount` de las tres listas, `__sinit_\D2AMpMenu_cpp`), datos `sInstance__Q22MP9D2AMpMenu` (0x8087F59C),
  `sInstance__Q22MP17D2AMpMenuRankMark` (0x8087F5A0), `sTablePath__Q22MP17D2AMpMenuRankMark` (0x8087E0F0),
  `sCustomData` (0x8087F598 y 0x8087F5E8, locales), `__sinit_\D2AMpMenuSetMessage_cpp`; `@3209` (jumptable).
- Lobby: `sInstance__Q22MP14D2AMpMenuLobby`, funciones escritas (`fn_804BBBA0__...`, `vf0C/10/14`, `fn_804BC628__Q22MP14D2AMpMenuLobbyFi`,
  `fn_804C2C14__...`, `fn_804C2994__...Fi`, `fn_804C0A40__...Fv`, `fn_804C1634__...Fv`, `fn_804BF550__...Fv`, ...).
- Re-emparejados `@N` (pd/pdbatch) de Lobby, MessageList, FriendRoom y CommonDialog tras los cambios de cabeceras; , MessageList, D2AMpMenu, Friend, FriendRoom, Cockpit, SetRule y
  Result tras los cambios de cabeceras.

## Cambios en libs/

- `libs/msc` (CC0, mscharged): `KPAD.c`, `KPAD.h`, `KPADPriv.h` actualizados a la version 2010 del juego;
  `KPADMpls_unk.c` reescrito y `KPADMplsMath_unk.c` nuevo (desde el asm). `libs/key` (CC0): `WPAD.c` sin los stubs
  `fn_8065CED0..CF20`. Ninguna fuente externa nueva copiada.

## Hallazgos

- Ver `docs/trucos-mwcc.md` (lineas marcadas `unidades-20261106b`).

## Pendientes

- `MP/D2AMpMenuLobby.cpp` (15/39): las que quedan usan el pool de cadenas (`fn_804BC628`, 5000 B, la primera: sin
  ella no cuadran los desplazamientos de `@stringBase0`), el `@wstringBase0` de `.data` 0x80790D74 (`fn_804C0F88`:
  `L"ON"`, `L"OFF"`, `L"%02d"`...) o la busqueda por aid bloqueada (`fn_804BE610` y `fn_804BEDB8` la llevan
  inlineada, `fn_804C23E4`). Los `char[]` de `.data` 0x80790D10-0x80790D68 (`"pos0001(1)(1)"`, `"basePos"`...) son
  de alguna funcion de dibujo (por ver).

- `MP/D2AMpMenu.cpp`: 23 funciones sin escribir; la siguiente es `fn_804AF520` (cambio de estado, 5456 B, 23 casos;
  usa SoftKeyboard, `atn::net::Manager`, D2AMpMenuFriend/FriendRoom/Lobby/MessageList y las cadenas 0x2ec-0x3bc del
  pool: sin ella las funciones posteriores no pueden cuadrar sus desplazamientos de `@stringBase0`). Bloqueo comun:
  el `16.0f`/`21.0f` de `fn_8006EF48` que el original carga antes de la rama de `data()` (vf0C, AF520, SetRule vf0C,
  FriendRoom; en `fn_804C50C0` de MessageList igual con los floats de `fn_80061AE4`): probados temporal con nombre,
  `c_str()`, variable `size`, inline con la cadena o el puntero, compiladores Wii 1.0-1.7/GC 3.0a5.2 (todos iguales).

- `MP/D2AMpMenuSetMessage.cpp`: faltan `fn_804CA250` (2200 B) y las de 0x804CAAE8 en adelante (15 funciones);
  `vf10` 99,75% (caso 5: texto en r31 y layout en r29; el original r29/r30) y `fn_804C8D80` 98,4%.

- KPAD (33/33, datos ya en el orden de 2010): probado `matching: true`: el enlazador quita las constantes sin uso
  (200.0f de `KPADSetBtnRepeat`, estaticas de `get_kobj`) y `.data/.bss/.sdata/.sbss` cuadran; **solo** falla el
  orden de tres entradas de `.sdata2` (0x80888920): el original tiene `1/200` y `1/100` antes del `{1,1,1}` de
  `KPADiSamplingCallback`, como si una funcion anterior quitada por el enlazador (entre `KPADInitEx` y
  `KPADiSamplingCallback`) las usara primero. MWCC ordena el pool por primer uso al generar codigo (probado con un
  fichero suelto: una inline no cambia el orden). No he inventado esa funcion: queda `matching: false`.
- `KPADMpls_unk.c` (6/40): libreria Motion Plus de 2010 entera (~10 KB, area de trabajo de 0xDB0 por mando); sin
  fuente de referencia: aplazada. `fn_8065B7F0` es un lerp de `KPADMPDir` + `fn_8065AFB0`; los tres por eje usan
  0x108/0x120/0x138/0x154 + 8 por eje y 0xD80/0xDA4 (+8/+4).
