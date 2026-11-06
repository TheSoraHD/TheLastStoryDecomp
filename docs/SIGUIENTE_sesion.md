# Siguiente sesión: propuestas de trabajo

## Estado actual (2026-11-06, cierre de `unidades-20261106b`, sin mergear)

- Rama `unidades-20261106b` (desde `main` c6fdafc, 13524): report **13617 / 18117** (lost 0, fuzzdrop 0,
  `main.dol: OK` en build limpio y en clon limpio, 652 unidades enlazadas). Notas y decisiones:
  `docs/unidades-20261106b.md`.
  - KPAD 2010: `KPAD.c` 33/33 (no enlaza: solo el orden de tres entradas de `.sdata2`), `KPADMpls_unk.c` 10/16 y
    `KPADMplsMath_unk.c` (unidad nueva partiendo KPADMpls) 4/6. Los `WBC*` débiles de KEY ocupan el sitio de los
    stubs `fn_8065CED0..CF20`.
  - Menús MP: `D2AMpMenuSetMessage` 20/26 (las 26 escritas), `D2AMpMenu` 20/41, `D2AMpMenuLobby` 15/39 (nueva;
    split FriendRoom/Lobby corregido), FriendRoom 10/13, Friend 16/21, MessageList 9/16, Result 13/20. Clases en
    `include/MP/D2AMpMenu*.h`; `Decide_unk` (cursor + decidir/cancelar) en `include/MP/D2AMpMenu_unk.h`.
  - `game/NewCampPartsItem.cpp` nueva: 13/41 (como `NewCampPartsHint`); siguen `vf14` (2132 B, primera con
    cadenas: desbloquea los pools), `vf20`, `fn_80517B3C` (lectura de la lista) y el `std::sort` de la lista.
- `main` (fb7f7d0): 13524. Sin enlazar: `bd.c`, `WPADHIDParser.c` (32/33), `WUD.c` (77/85), `HBMBase.cpp` (33/34),
  `db_assert.cpp` (4/5), `CXUncompression.c` (3/4), `dwc_match` (96/101).
- Fuentes externas: Swiftshine/key (CC0), mscharged-decomp (CC0), wii-ipl (CC0), pokemon-rumble-decomp (MIT).
  `robojumper/sdk_2009-12-11` (sin licencia) y `mkwcat/nsmbw-project` (MPL 2.0): sin usar, decide el usuario.

## Propuestas

1. Menús MP: unificar las vistas de `lbl_8087F628` (`MpData_unk`, `MpMsgData_unk`, `MpLobbyData_unk`,
   `atn::net::Manager`) y de `lbl_8087F610` en una cabecera; es requisito de `D2AMpMenu::fn_804AF520` (5456 B) y
   esa función desbloquea el pool `@stringBase0` del resto de D2AMpMenu. En Lobby, `fn_804BC628` (5000 B) hace lo
   mismo para su pool (y usa el `@wstringBase0` de `.data` 0x80790D74).
2. Bloqueo de la búsqueda por aid (`docs/mwcc-blockers.md`): Lobby `vf0C`, `fn_804C2C14`, `fn_804BE610`,
   `fn_804BEDB8`, `fn_804C23E4`, FriendRoom `vf0C`. Pista: en `fn_804C23E4` el aid entra con `clrlwi` (parámetro de
   8 bits de la búsqueda real).
3. Registros texto/layout en `SetPaneText_unk` (SetMessage, Result, Cockpit, SetRule, MessageList `fn_804C38C0`):
   probar formas nuevas (no repetir las anotadas en cada `NONMATCHING`).
4. KPAD: `matching: true` solo depende del orden `.sdata2` (1/200 y 1/100 antes de `{1,1,1}`); `KPADMpls_unk.c`
   `fn_80659DF0` (2752 B).
5. `dwc_match` (96/101); NewCampPartsItem (resto) y unidades del juego sin ninguna función al 100% (NewCampTask, TacView, ShopBase,
   LastCocoon*, AiTarget, ReplayBuffer, Event/DebugInfo_unk). TeamAI_unk solo con idea nueva.

## Bloqueos conocidos (no repetir sin idea nueva)

- `16.0f`/`21.0f` de `fn_8006EF48` cargados antes de la rama de `data()` (D2AMpMenu `vf0C`, SetRule `vf0C`,
  FriendRoom `vf0C`, MessageList `fn_804C50C0`): igual con todos los compiladores.
- `push_back<boost::shared_ptr>` y el crecimiento de `vector` no trivial.
- MapManager fn_80491D50/DF8; matriz de reflejo de `atn::Scene`; MapTask::fn_804A0F80 (presupuesto de inline).
- `IpcReplyHandler`: `switch` en árbol en el original.
- `pdbatch.sh` renombra `lbl_8087D6E8/EC`, `lbl_80756700`, `lbl_8087EE78`, a veces `0x8087DB00/04`, `__vt__` sobre
  cadenas: devolverlos a mano. `.sdata2` ambiguas: `tools/decomp/sdata2pair.py <unidad>`. dtk vuelve a poner
  `jumptable_` sobre un `lbl_`: renombrar la tabla al `@N` del compilador.
- Tras cambiar una cabecera compartida de los menús MP, `pdbatch.sh` de todas las unidades `MP/D2AMpMenu*` y
  `MP/D2ACommonDialog` (labelcheck lo detecta).
- No partir los símbolos `@wstringBase0`: dtk falla con "Non-terminated UTF-16 string".
- Tras borrar `build/SLSP01/obj` hay que borrar también `build/SLSP01/config.json`.
- `configure.py` sin flags intenta descargar los compiladores: usar la línea de `docs/entorno-nube.md`.
- `odiff2.py` no recompila: compilar la unidad (o `fnstat.py`) antes. `tools/decomp/try.sh` busca los compiladores
  en `../compilers`: en la nube están en `/home/claude/tls-build/build/compilers` (ajustar la ruta).
- Al renombrar un `fn_` en symbols.txt, renombrarlo también en **todos** los fuentes, o el enlace falla.
