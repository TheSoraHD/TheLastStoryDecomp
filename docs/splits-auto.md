# splits-auto: reparto de los tramos `auto_` del juego/motor (2026-10-20)

Rama `splits-auto` (desde `main` 6e7fd82 + `tools/decomp/autoscan.py` de `splits-prep`). Solo se han tocado `config/SLSP01/splits.txt`, renombrados en `config/SLSP01/symbols.txt` y documentación: ningún `.cpp`, `game_units.json`, `libs_ref.json`, `configure.py`, flags ni `config.yml`.

Cuatro agentes en paralelo, cada uno con un rango contiguo de `.text` y su copia del repo; integrados con `git merge --no-ff`. Notas completas (prueba de cada corte, decisiones por grupo, hallazgos):

| Agente | Rango `.text` | Notas |
|---|---|---|
| A | 0x80004000–0x800F7250 (motor `atn::`, `AI::Script`) | `docs/splits-auto-A.md` |
| B | 0x800F7250–0x8033C3D8 (`chr::Control*`, `db/`, efectos, D2A, `Em*`) | `docs/splits-auto-B.md` |
| C | 0x8033C3D8–0x804405F8 (`EnemyQuark*`, `Game*`, `gmk`) | `docs/splits-auto-C.md` |
| D | 0x804405F8–0x805BFAC0 (`MP`, `Event`, cargador `atn`, red) | `docs/splits-auto-D.md` |

## Resultado (build limpio de la rama integrada)

- `build/SLSP01/main.dol: OK`. `regress.py` contra el `report.json` de `main`: **lost: 0**; 4757 → **4758** funciones al 100% (`fn_804225E0` de `gmk/CharaLight` sube sola al entrar su dato en el split).
- `.text` en tramos `auto_` del juego/motor (0x80004000–0x805BFAC0, `autoscan.py --min 0`): **0x263718 → 0xF14 bytes** (4 tramos). Quedan:
  - 0x8007708C–0x80077160 (`fn_8007708C`, entre `atn/GraphicsDevice` y `atn/LensFlare`, sin datos que lo sitúen);
  - 0x800F13BC–0x800F1430 (entre `game/BattleResult` y `game/BattleTask`, sin datos);
  - 0x801E9218–0x801E9398 (dos funciones entre `CostumeEditChara` y `CrossbowMenuTask`, sin pruebas);
  - 0x803B27AC–0x803B33F8: no es un tramo suelto, es la unidad ya existente `auto_03_803B27AC_text.cpp` (contiene `GameLoadTask`; se dejó el nombre heredado).
- `splits.txt`: 629 → 806 unidades (179 nuevas: 118 `_unk`, 13 `_cont`; se quitan los bloques explícitos `auto_fn_801F1EA0_text.cpp` y `auto_fn_8030DA1C_text.cpp`, absorbidos por `atn/D2A_Manager` y `game/Em310Gargoyle`). Ficheros en el `report.json`: 1922 → 1347.
- Datos que siguen en unidades `auto_` (todo el `.dol`, incluido el SDK, según `report.json`): `.rodata` 0x357CC → 0xD15C, `.data` 0x21FB8 → 0x1037C, `.sdata2` 0x3078 → 0x928, `.sdata` 0x1564 → 0x758, `.sbss` 0x12F0 → 0xA14, `.bss` 0x8EFB8 → 0x8BE30 (las tres `.bss` grandes siguen sin dueño).
- Avisos de alineación de dtk en el build: 67 → 20 (`.rodata`/`.sbss`/`.sdata2` de unidades en asm; inocuos). Aparecen 179 avisos "Missing configuration" (uno por unidad nueva, inocuos).
- `targets.py --max-unit 100000000` ve ahora 303 unidades partidas sin fuente.

## Decisiones a revisar (tomadas sin el usuario)

Resumen; el detalle con las pruebas está en las notas de cada agente.

1. **Nombres inventados (`_unk`), 118 unidades.** Sin clase con vtable ni nombre de fichero en el binario; nombre según lo que hace el código (p. ej. `Event/EventCommandDef_unk`, `game/BattleCalc_unk`, `game/CosEditItemList_unk`, 39 `db/*_unk` uno por `/boot/*.csv`, `game/ItemManager_unk`, `game/SystemErrorMessage_unk`). Algunas sin `_unk` también son deducidas: `atn/GraphicsDevice` (cadena `atn_graphics_device.cpp`), `atn/LH_Core`, `atn/LH_Handle`, `atn/NetManager`, `game/MapPartsFactory` (orden alfabético o RTTI) y `game/main` (contiene `main`).
2. **Unidades `_cont` (13):** continuación del fichero real de una unidad ya **enlazada** (no se podía ampliar sin romper el enlace); fusionar al decompilarla: `atn/Sound3dListener_cont`, `atn/Bgm_cont`, `atn/EffectManager_cont`, `atn/LH_Motion_cont`, `game/FriendManager_cont`, `game/GameTimer_cont`, `Event/Manager_cont`, `Event/Sequence_cont`, `MP/SaveLoad_cont`, `game/MapPartsOcean_cont`, `game/MarketTask_cont`, `game/NpcManager_cont`, `game/SetConfig_cont` (las dos últimas sin datos: apoyo débil).
3. **Unidades grandes sin cortes internos por falta de pruebas** (mejor grande que cortes falsos):
   - `AI/AI_archer_attack01` (0x8001BF44–0x8003E918): los ~65 `AI::Script` en una unidad (RTTI y vtables contiguas, un double de 0x8072FF60 usado 252 veces en todo el tramo).
   - `game/BattleTask` (0x800F1430–0x8010D98C, ~120 KB): los `gmk::Cannon`/`Bullet`/`ControlCdnRestorePsycho` del tramo son solo heredados/débiles.
   - `atn/PacketPack`: absorbe el tramo de 0x28DC4 bytes (`MP::Manager2`/`ManagerMP`/`SpecialEnemyBase`, 342 funciones); el fichero real será algo como `MP/ManagerMP.cpp`.
   - `game/CosEditItemList_unk` (0xFF58), `game/Gun` (incluye `ColliObjectData`/`ColliTreeData`), `game/GameSoundControl` (incluye `Unk_803CE3B8`), `game/GameChoiceWindow` (0x803B35FC–0x803B434C).
4. **Unidad existente reducida:** `Event/Node` (inferida, sin fuente) se parte en `Event/Node`, `Channel`, `Cut`, `Label`, `Section` (cuatro `__sinit`, cinco pools, cadenas `"id"` duplicadas). Las instrucciones solo permitían ampliar.
5. **`atn/CsvBase_char_124_13_34` contradice el plan:** las pruebas dan 6 ficheros cargadores distintos; la unidad queda solo como el cargador de `battle_eff_se_table` (0x802078C4–0x80207CC0), no 23 funciones más.
6. **Nombres heredados que no cuadran (sin renombrar la unidad):** `std/bad_cast.cpp` contiene `GameEventScript`; `auto_03_803B27AC_text.cpp` contiene `GameLoadTask`; `std/char_traits_char`/`std/allocator_char` parecen la cola (plantillas débiles de iostream + `__sinit`) de otro fichero (¿`WordWrap_unk`?); `std/char_traits_char_8023C080` se amplió con la plantilla std y los thunks de `EffectManager` (su fichero real parece 0x80239798–0x80243758).
7. **`ComDrawTexture`** era el resto del fichero de `DrawHelper`: se amplió en vez de crear `DrawHelper_cont`.
8. **Cortes débiles** (primer punto coherente tras la última función que usa las constantes de un fichero): Geometry_unk/GraphicsDevice, inicio de `atn/Model`, BoneCalc_unk, Se_unk, SoundManager/Task, Xsca_unk/XsfParser_unk (0x805BE8FC), ReplayBuffer/GameSystem_unk (0x80570A14), DwcAllocator_unk, NetFriend_unk, 0x8025FE30 en `Em101LittleZesya` (podría abrir `Em101LittleZesyaSecond`); funciones sueltas sin datos pegadas a la unidad anterior (0x80531E80 → NewStaffRoll, 0x804BBB60 → D2AMpMenuLobby, accesores `LH_*`).
9. **`symbols.txt`:** solo renombrados de pools a `@stringBase0` (`scope:local data:string_table`): FileList (`lbl_8073148C`), `lbl_8073EA48`, GameUI (0x807506A0), Banana, CharaLight, LandMine, OpenBoard, PassBoard, ShakeCamera, TodomeCountUp, LH_Effect (`lbl_80755CB8`), LH_Material (`lbl_80755D68`), LH_MotionSet (`lbl_80756030`).
10. **Datos ajustados por dtk:** `.rodata` de `game/MotionCommand_unk` empieza en el relleno de 4 bytes de 0x807370B4 (la unidad anterior está enlazada); `.sbss` de `ControlCdnStone` y de la unidad std absorben guardas compartidas vecinas. No se ha usado `align:`.
11. **Datos no asignados a propósito:** tabla de errores de `Sound3dListener_cont` (0x8073401C–0x80734130), `.bss` 0x807C78C8 de ShopUpGrade, `.rodata` 0x807570C0 de MarketTask_cont (dtk: ciclo de orden de enlace), `@LOCAL` de `boost::function` en 0x807C88F0 (CharaLight/Fireworks/RoomLight), `__RTTI__PFP12MapModel_unkPv_v`, `.sdata` 0x8087DFC0/C4, floats `.sdata` 0x8087E480 de NewCampPartsHint (una nota anterior los deja como externos), datos de ActionCutAdjust y pools de nw4r.

## Hallazgos generales

- dtk exige que el orden de las secciones de datos entre unidades siga el orden de `.text`: dar a una unidad datos situados antes de los de una unidad anterior en `.text` da "Cyclic dependency encountered while resolving link order". Por eso un `_cont` no puede llevar datos que estén antes de los de su unidad enlazada.
- Una guarda o puntero compartido que queda solo en un hueco de ≤ 8 bytes entre unidades sale como símbolo indefinido al enlazar: absorberlo en la unidad vecina.
- `configure.py` no borra los `.s` de `build/SLSP01/asm` de unidades que ya no existen; `autoscan.py`/`xref.py` los siguen leyendo. Borrar `build/SLSP01/asm` (o hacer build limpio) antes de inventariar.
- Las cadenas `"Memory allocation failure"` están deduplicadas por el enlazador: no sirven para decidir a qué fichero pertenece el código que las usa.
- Las 179 unidades nuevas sin entrada en `game_units.json` dan un aviso "Missing configuration" cada una al compilar (inocuo: se enlazan desde el asm); los avisos de alineación de dtk bajan de 67 a 20.
- Método de corte más fiable: constante (o cadena) con una copia por fichero, y pool de cadenas al final de `.rodata` de cada fichero. El agente B probó su detector sobre tramos ya partidos en ficheros conocidos sin cortes falsos.
