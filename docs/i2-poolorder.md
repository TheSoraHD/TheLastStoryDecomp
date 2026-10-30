# I2: orden de `@stringBase0` en todo el juego (2026-10-30)

Rama `i2-poolorder` desde `main` 56f156c. Trabajo secuencial, sin agentes.

## Resultado

- **8750 → 8752** funciones al 100% (comprobadas con `objdiff-cli`, no solo `report.json`), código 31,36 → 31,37%, enlazados 371 (sin cambios). `main.dol: OK` en cada commit; `regress` lost: 0 y `fuzzdrop` sin bajadas contra el `report.json` del inicio; `poolcheck`/`mnemcheck` limpios en lo tocado; `ordercheck` sin funciones nuevas fuera de orden; avisos de dtk idénticos a los del build inicial.
- **Pools idénticos al original: 279 → 317 unidades** (303 antes de las dos operaciones aprobadas; ver abajo). La cifra de partida es la de la primera versión de la herramienta, que quitaba también los `""` finales; con la versión final la diferencia es de unas pocas unidades.
- Nuevas al 100%: `SpecialEnemyBase::fn_805A5220` y `Map_unk<Ui,PCc>::InsertNode` (`atn/ptr_string_base_char_0`).
- Suben sin llegar al 100% (fuzzy de `report.json`): `CosEditBase` ctor 97,2 → 98,9 y `fn_801CEA98` 97,8 → 99,5; `MotionCommand_unk` `push_back` 69,1 → 73,4 y `fn_8012133C` 93,9 → 94,7; `TextAnimation` `reserve<w>` 0 → 97,1.

## Herramienta

`tools/decomp/poolorder.py [--all] [--funcs [--mixed]] [unidad...]`: compara la lista ordenada de cadenas de `@stringBase0` del `.o` original (`build/SLSP01/obj`) con la nuestra (`build/SLSP01/src`). Da `same` / `order @i` (mismas cadenas en otro orden) / `content @i` (cadenas distintas, con faltan/sobran) / `no-pool`. Ignora el relleno final de alineación del original y conserva los `""` finales nuestros. `--funcs` lista, por unidad con diferencias, las funciones < 100% cuyas únicas diferencias en `objdiff-cli` son desplazamientos de `@stringBase0` (`only`, se arreglan solo con el pool) y cuenta las que los tienen junto con otras diferencias (`mixed`). Todo el juego tarda ~3 s sin `--funcs` y ~4 min con él.

Pasada inicial: 11 unidades `order`, 48 `content`; 15 funciones `only` en 7 unidades (CharaTask 6, Em301LShade 3, GimmickStateCommand 2, EnemyQuarkSecond, SpecialEnemyBase, EnvSeBase y GameSaveLoadBackgroundCommon 1).

## Por unidad (commit propio cada una)

| Unidad | Causa | Arreglo | Efecto |
|---|---|---|---|
| `gmk/HealPoint` | literal erróneo: `"SKILL"` en vez de `"PARAM"` (objdiff no compara literales; `poolcheck` no lo vio porque `"SKILL"` existe en otro sitio del DOL) | `"PARAM"` | pool idéntico |
| `game/CosEditBase` | el `""` del `new` abre el pool original; faltaban `out_of_range`/`length_error` | ayudante no `static` `NewCharaEquip_unk` delante del ctor (se emite en su sitio y se inlinea en `fn_801CEA98`), `std/string_members.h` | pool idéntico, ctor y `fn_801CEA98` +1,7 |
| `game/CharaTask` | `"%s [%d]\0"`; erratas `"charactor"` del original | NUL y `"charactor not found! %s"` | quedan `righthand` (inline `fn_8013A4E8`) y `DamageSphereList` (no referenciado) |
| `gmk/SwingObject` | `"Param%d\0"` | NUL | pool idéntico |
| `atn/FileUtil_unk` | las copias débiles de `basic_string(str, pos, n)` y `find` van justo detrás de su primer uso en el original (0x80069BF4, 0x8006A004) | instanciación explícita detrás de `fn_800698DC` y `fn_80069E50` | pool y orden del `.text` idénticos |
| `game/SpecialEnemyBase` | `"%s\0"` y `out_of_range` detrás de `fn_805A3D68` | NUL e instanciación explícita del ctor de subcadena detrás de `fn_805A3D68` | **`fn_805A5220` al 100%**; el `""` de la fábrica se resolvió con el split |
| `game/LibraryTask` | ctor de subcadena `wchar_t` detrás de `fn_8046AE7C` | instanciación explícita | pool idéntico |
| `game/MotionCommand_unk` | `push_back` de `vector<string>` es copia débil detrás de `fn_8011FFF8` (0x801207D4) y su mensaje abre el pool | instanciación explícita detrás de `fn_8011FFF8` | pool idéntico, `fn_8012133C` +0,8 |
| `atn/AudioDevice`, `atn/LH_Effect`, `atn/TextAnimation`, `game/CopyRobotTask`, `game/EnemyJill`, `game/MenuCommonTask`, `game/ShopBuySell`, `game/NewCampParts{Gmk,Hint,Status,Top}`, `game/TownTask`, `Event/Dialog`, `game/LogMessageTask` | el pool acaba con los mensajes de `basic_string` | `#include "std/string_members.h"` | pool idéntico (ninguna función cambia) |
| `Event/DialogHead` | además, `out_of_range` detrás de `fn_80227A94` | `string_members.h` e instanciación explícita | pool idéntico |
| `game/Arms`, `game/EnvSeManager` | `"%s.efp\0"`, `"%s\0"` | NUL y `string_members.h` | pool idéntico |
| `atn/ptr_string_base_char_0` | `"%s\0"`; el mensaje de `__allocate` se llamaba `lbl_807835D0` | NUL y renombrado a `@STRING@__allocate__15Map_unk<Ui,PCc>Fv` en `symbols.txt` (solo renombrado, como los de `AudioMap_unk`) | **`InsertNode` al 100%** |

## Lo que queda (34 unidades) y por qué

- **Presupuesto de inline (bloqueo conocido):** el original llama a `push_back` fuera de línea y su `"vector length error"` va detrás de la función; el nuestro lo inlinea y el literal sale en medio. `Em301LShade` (3 `only` bloqueadas por `vf0C` al 0%), `EnvSeBase` (`EnvSeLine::vf1C` 14,7%), `Em001Necromancer`, `Em204BigMulue`, `gmk/CharaPathMove`, `GimmickStateCommand`, `gmk/WindyItem`, `D2A_Manager` (`fn_801F1658` 0%), `SequenceHolder`. Una instanciación explícita no basta mientras el llamador lo inlinee (probado en `Em301LShade`).
- **Unidades a medias:** faltan funciones con literales (`EnemyQuarkFirst` `vf108`, `EnemyQuarkSecond`, `Em316*`, `Em003`, `Em203*`, `em005`, `ControlLadder`, `CampSaveLoad`, `PopupTask`, `Loader`, `MindowsCommon`, `FileList`, `FileSystem_unk`, `EffectManager`, `MP/Text`, `GimmickStateCommand`, `ShopIdentify`, `MapManager`, `gmk/Chain`, `CrowdSimulation`, `GameSaveBanner`).
- `GameSaveLoadBackgroundCommon`: el original tiene `"%s/save%02d.dat"`, `"%s/save_sys.dat"` y tres `""` sin ninguna referencia en el código (literales de alguna inline que no se emitió).
- `db/EventIdTable_unk`: el original acaba con los mensajes de `basic_string` pero el nuestro no usa ningún miembro fuera de línea; `string_members.h` no los añade.
- `CharaTask`: `"righthand"` va a `@STRING@` porque `fn_8013A4E8` es `inline` (ver su NONMATCHING) y `"DamageSphereList"` no lo usa ningún código; las 6 `only` siguen pendientes de eso.

## Decisiones tomadas sin el usuario

- Instanciaciones explícitas de miembros de `basic_string`/`vector` detrás de su primer uso (precedente: `Em203`, `Em204`, truco de `docs/trucos-mwcc.md`). Las copias débiles que el original no tiene en la unidad las quita el enlazador (se quedan las de 0x80069BF4 en `atn/FileUtil_unk` para `char` y 0x800E0908 para `wchar_t`, que van antes en el enlazado). `ordercheck.py` no ve estas líneas: las da como `missing`.
- Ayudante inventado `NewCharaEquip_unk` (no `static`) en `CosEditBase`, como el de F-b2.
- Renombrado de `lbl_807835D0` en `symbols.txt` (solo nombre; regla 3 lo permite).
- Probado y **descartado** en `SpecialEnemyBase`: un `static const char sEmpty_unk[8]` para el `""` de la fábrica sube `fn_805A5374` de 74 a 99,99%, pero el objeto original mide 4 bytes en `.rodata` (uno de 4 bytes iría a `.sbss2`) y la tabla, sus cadenas y ese `""` forman un segundo bloque de `.rodata`: parece **otro fichero fuente**. Revertido; resuelto después con el split (abajo).

## Cambios de `splits.txt`/`symbols.txt` (aprobados por el usuario el 2026-10-30)

1. **Split de `game/SpecialEnemyBase`**: `fn_805A52F4` (nombre → índice), la tabla `sEnemyNames` y la fábrica `fn_805A5374` pasan a `game/SpecialEnemyFactory_unk.cpp` (`.text` 0x805A52F4–0x805A6208, `.rodata` 0x80763330–0x807637E0, `.data` 0x80797488–0x807975B0, `.sdata2` 0x80888298–0x808882A0). Motivo: tienen su propio bloque de `.rodata` (cadenas de la tabla, tabla y un pool con solo el `""` del `new`, 0x807637DC, renombrado `@stringBase0`), su tabla de saltos y su `""` de `.sdata2`, sin cruces con el resto; una prueba con `try.sh` reproduce esa disposición. La fábrica pasa de 74,2 a **99,97%** (faltan los tamaños de 6 clases de enemigo, 4 bytes cortos en nuestras cabeceras, y 2 constructores llamados como `extern "C"`); `SpecialEnemyBase` sube de 85,5 a 92,4% de fuzzy de unidad. Los dos pools, idénticos.
2. **12 pools escondidos en un único `lbl_`** renombrados a `@stringBase0 scope:local data:string_table` (`Event/EventCommandDef_unk`, `Event/Path`, `atn/CompositeIntensityTintMulti`, `atn/Font_unk`, `atn/NetFriend_unk`, `atn/PBoneBasic`, `atn/RenderSystem_unk`, `game/CharaStatus`, `game/CrossbowMenuTask`, `game/GimmickManager`, `game/main`, `gmk/ArenaFloor`). Ningún otro fichero los usa. objdiff no compara cadenas, así que **no cambia ningún porcentaje**: sirve para que `poolorder`/`poolcheck` puedan comparar esas unidades. Con ello `Event/Path`, `atn/NetFriend_unk` y `atn/PBoneBasic` recibieron `std/string_members.h` y los 12 quedan idénticos.
- Pendiente: `AI/AI_archer_attack01` (pool 0x807307A0) no se renombró porque su fuente lo usa como `extern unsigned char lbl_807307A0[]` con desplazamientos a mano (incumple la regla 10): primero hay que pasar a literales. Otras 4 unidades (`D2A_Texture`, `WordWrap_unk`, `MapPartsBase`, `LH_Motion_cont`) eran falsos positivos.

## Hallazgos (una línea en `docs/trucos-mwcc.md`)

- MWCC instancia los miembros de plantilla definidos fuera de la clase al final del fichero, en un orden que depende de qué miembros referencia el código del usuario (ctor de subcadena solo: `out_of_range` primero; con `string(const char*)` en el fichero, `reserve` primero). El original los emite justo detrás de su primer uso; una instanciación explícita en ese sitio reproduce copia y literales.
- `sprintf` con `"%s"`, `"%s.efp"`, `"Param%d"`, `"%s [%d]"` y NUL extra: patrón frecuente; `poolorder.py` lo delata como un `""` que falta justo detrás del formato.
