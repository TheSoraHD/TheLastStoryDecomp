# Siguiente sesión: propuestas de trabajo

# Siguiente sesión: propuestas de trabajo

## Siguiente sesión: `claude/prompt-maraton.md` (proyecto)

- Más de 10 h, secuencial. Prioridad 1: las unidades sin ninguna función (≈1213 funciones: `GameEventScript_cont`,
  `BattleTask`, `GameTask`, `ShopUpGrade`, `DebugInfo_unk`, `CosEditItemList_unk`, `TacView`, `AiTarget`,
  `GameCamera_unk`…); prioridad 2: funciones de ≤ 0x100 B de unidades empezadas (`atn/PacketPack` 255).

## Estado actual (2026-11-07, `main` 44a077a: `hermanas-20261106` mergeada)

- `hermanas-20261106` (desde `main` 1bd7fca, 13619): report **13737 / 18117** (lost 0; `main.dol: OK` en build
  limpio y en clon limpio; 652 unidades enlazadas; sin cambios de splits). Notas, decisiones y pendientes:
  `docs/hermanas-20261106.md`. Mergeada en `main` (44a077a).
  - Unidades nuevas: `ShopBase` 33/48, `CosEditProtector` 18/25, `NewCampPartsMapSelect` 11/26, `NewCampTask` 7/64,
    `LastCocoonQuadruped` 24/54, `LastCocoonImago` 14/36, `LastCocoonEgg` 11/42.
  - Siguiente paso con más rendimiento: escribir los ctor de `LastCocoonQuadruped`/`Imago` (aunque queden
    `NONMATCHING`, como Chrysalis) para fijar `@stringBase0`: desbloquea varias funciones con sonidos y nodos.

## Estado anterior (2026-11-06, `main` f92ee8d)

- Prompt de la siguiente sesión: `claude/prompt-hermanas.md` (proyecto): unidades hermanas, patrones repetidos,
  menús MP; secuencial y con ahorro de tokens. Herramientas nuevas: `tools/decomp/mkref.sh` (referencia) y
  `tools/decomp/precommit.sh [--pd] <unidades>` (todas las comprobaciones, `PRECOMMIT OK/FAIL`); `try.sh` ya
  encuentra los compiladores en la nube.

- `unidades-20261106b` (mergeada; desde c6fdafc, 13524): report **13619 / 18117** (lost 0, fuzzdrop 0,
  `main.dol: OK` en build limpio y en clon limpio, 652 unidades enlazadas). Notas y decisiones:
  `docs/unidades-20261106b.md`.
  - KPAD 2010: `KPAD.c` 33/33 (no enlaza: solo el orden de tres entradas de `.sdata2`), `KPADMpls_unk.c` 10/16 y
    `KPADMplsMath_unk.c` (unidad nueva partiendo KPADMpls) 4/6. Los `WBC*` débiles de KEY ocupan el sitio de los
    stubs `fn_8065CED0..CF20`.
  - Menús MP: `D2AMpMenuSetMessage` 20/26 (las 26 escritas), `D2AMpMenu` 20/41, `D2AMpMenuLobby` 15/39 (nueva;
    split FriendRoom/Lobby corregido), FriendRoom 10/13, Friend 16/21, MessageList 9/16, Result 13/20. Clases en
    `include/MP/D2AMpMenu*.h`; `Decide_unk` (cursor + decidir/cancelar) en `include/MP/D2AMpMenu_unk.h`.
  - `game/NewCampPartsItem.cpp` nueva: 15/41 (como `NewCampPartsHint`); `vf20` 99,23% y `vf3C` 96,9% por
    registros; siguen `fn_80517B3C` (lectura de la lista; fija el `@wstringBase0` de `.data`), `fn_8051B164` y
    el `std::sort` de la lista.
- `main` (fb7f7d0): 13524. Sin enlazar: `bd.c`, `WPADHIDParser.c` (32/33), `WUD.c` (77/85), `HBMBase.cpp` (33/34),
  `db_assert.cpp` (4/5), `CXUncompression.c` (3/4), `dwc_match` (96/101).
- Fuentes externas: Swiftshine/key (CC0), mscharged-decomp (CC0), wii-ipl (CC0), pokemon-rumble-decomp (MIT).
  `robojumper/sdk_2009-12-11` (sin licencia) y `mkwcat/nsmbw-project` (MPL 2.0): sin usar, decide el usuario.

## Propuestas (por rendimiento esperado; detalle en `claude/prompt-hermanas.md`)

1. Unidades nuevas con una hermana ya hecha: `NewCampPartsMapSelect` (base `NewCampPartsBase`), `LastCocoonEgg/
   Imago/Quadruped` (hermana `LastCocoonChrysalis`), `CosEditProtector` (`CosEditBase`), `ShopBase`/`ShopUpGrade`
   (`ShopBuySell`, `ShopColor`, `ShopIdentify`). Fábrica, ctor (fija los pools), dtor y virtuales pequeños primero.
2. Patrones repetidos: agrupar los `NONMATCHING` por motivo y buscar un arreglo común (como `Decide_unk`).
3. Menús MP: unificar las vistas de `lbl_8087F628`/`lbl_8087F610` en una cabecera; luego `D2AMpMenu::fn_804AF520`
   (5456 B) y Lobby `fn_804BC628` (5000 B), que desbloquean los pools de unas 45 funciones.
4. Solo con idea nueva: búsqueda por aid (Lobby/FriendRoom; pista: `clrlwi` del aid en `fn_804C23E4`), registros
   texto/layout de `SetPaneText_unk`, KPAD `.sdata2`, `KPADMpls_unk` `fn_80659DF0`, `dwc_match`, TeamAI.

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
  `MP/D2ACommonDialog` (labelcheck lo detecta). Tras añadir una función con constantes `.sdata2`, `pd.sh` de la
  unidad antes del commit (rep100 lo detecta).
- No partir los símbolos `@wstringBase0`: dtk falla con "Non-terminated UTF-16 string".
- Tras borrar `build/SLSP01/obj` hay que borrar también `build/SLSP01/config.json`.
- `configure.py` sin flags intenta descargar los compiladores: usar la línea de `docs/entorno-nube.md`.
- `odiff2.py` no recompila: compilar la unidad (o `fnstat.py`) antes.
- Al renombrar un `fn_` en symbols.txt, renombrarlo también en **todos** los fuentes, o el enlace falla.
