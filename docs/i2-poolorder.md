# I2: orden de `@stringBase0` en todo el juego (2026-10-30)

Rama `i2-poolorder` desde `main` 56f156c. Trabajo secuencial, sin agentes.

## Resultado

- **8750 → 8752** funciones al 100% (comprobadas con `objdiff-cli`, no solo `report.json`), código 31,36 → 31,37%, enlazados 371 (sin cambios). `main.dol: OK` en cada commit; `regress` lost: 0 y `fuzzdrop` sin bajadas contra el `report.json` del inicio; `poolcheck`/`mnemcheck` limpios en lo tocado; `ordercheck` sin funciones nuevas fuera de orden; avisos de dtk idénticos a los del build inicial.
- **Pools idénticos al original: 279 → 303 unidades** (de 338 con `@stringBase0` en los dos lados). La cifra de partida es la de la primera versión de la herramienta, que quitaba también los `""` finales; con la versión final la diferencia es de unas pocas unidades.
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
| `game/SpecialEnemyBase` | `"%s\0"` y `out_of_range` detrás de `fn_805A3D68` | NUL e instanciación explícita del ctor de subcadena detrás de `fn_805A3D68` | **`fn_805A5220` al 100%**; queda el `""` de la fábrica (ver decisiones) |
| `game/LibraryTask` | ctor de subcadena `wchar_t` detrás de `fn_8046AE7C` | instanciación explícita | pool idéntico |
| `game/MotionCommand_unk` | `push_back` de `vector<string>` es copia débil detrás de `fn_8011FFF8` (0x801207D4) y su mensaje abre el pool | instanciación explícita detrás de `fn_8011FFF8` | pool idéntico, `fn_8012133C` +0,8 |
| `atn/AudioDevice`, `atn/LH_Effect`, `atn/TextAnimation`, `game/CopyRobotTask`, `game/EnemyJill`, `game/MenuCommonTask`, `game/ShopBuySell`, `game/NewCampParts{Gmk,Hint,Status,Top}`, `game/TownTask`, `Event/Dialog`, `game/LogMessageTask` | el pool acaba con los mensajes de `basic_string` | `#include "std/string_members.h"` | pool idéntico (ninguna función cambia) |
| `Event/DialogHead` | además, `out_of_range` detrás de `fn_80227A94` | `string_members.h` e instanciación explícita | pool idéntico |
| `game/Arms`, `game/EnvSeManager` | `"%s.efp\0"`, `"%s\0"` | NUL y `string_members.h` | pool idéntico |
| `atn/ptr_string_base_char_0` | `"%s\0"`; el mensaje de `__allocate` se llamaba `lbl_807835D0` | NUL y renombrado a `@STRING@__allocate__15Map_unk<Ui,PCc>Fv` en `symbols.txt` (solo renombrado, como los de `AudioMap_unk`) | **`InsertNode` al 100%** |

## Lo que queda (35 unidades) y por qué

- **Presupuesto de inline (bloqueo conocido):** el original llama a `push_back` fuera de línea y su `"vector length error"` va detrás de la función; el nuestro lo inlinea y el literal sale en medio. `Em301LShade` (3 `only` bloqueadas por `vf0C` al 0%), `EnvSeBase` (`EnvSeLine::vf1C` 14,7%), `Em001Necromancer`, `Em204BigMulue`, `gmk/CharaPathMove`, `GimmickStateCommand`, `gmk/WindyItem`, `D2A_Manager` (`fn_801F1658` 0%), `SequenceHolder`. Una instanciación explícita no basta mientras el llamador lo inlinee (probado en `Em301LShade`).
- **Unidades a medias:** faltan funciones con literales (`EnemyQuarkFirst` `vf108`, `EnemyQuarkSecond`, `Em316*`, `Em003`, `Em203*`, `em005`, `ControlLadder`, `CampSaveLoad`, `PopupTask`, `Loader`, `MindowsCommon`, `FileList`, `FileSystem_unk`, `EffectManager`, `MP/Text`, `GimmickStateCommand`, `ShopIdentify`, `MapManager`, `gmk/Chain`, `CrowdSimulation`, `GameSaveBanner`).
- `GameSaveLoadBackgroundCommon`: el original tiene `"%s/save%02d.dat"`, `"%s/save_sys.dat"` y tres `""` sin ninguna referencia en el código (literales de alguna inline que no se emitió).
- `db/EventIdTable_unk`: el original acaba con los mensajes de `basic_string` pero el nuestro no usa ningún miembro fuera de línea; `string_members.h` no los añade.
- `CharaTask`: `"righthand"` va a `@STRING@` porque `fn_8013A4E8` es `inline` (ver su NONMATCHING) y `"DamageSphereList"` no lo usa ningún código; las 6 `only` siguen pendientes de eso.

## Decisiones tomadas sin el usuario

- Instanciaciones explícitas de miembros de `basic_string`/`vector` detrás de su primer uso (precedente: `Em203`, `Em204`, truco de `docs/trucos-mwcc.md`). Las copias débiles que el original no tiene en la unidad las quita el enlazador (se quedan las de 0x80069BF4 en `atn/FileUtil_unk` para `char` y 0x800E0908 para `wchar_t`, que van antes en el enlazado). `ordercheck.py` no ve estas líneas: las da como `missing`.
- Ayudante inventado `NewCharaEquip_unk` (no `static`) en `CosEditBase`, como el de F-b2.
- Renombrado de `lbl_807835D0` en `symbols.txt` (solo nombre; regla 3 lo permite).
- Probado y **descartado** en `SpecialEnemyBase`: un `static const char sEmpty_unk[8]` para el `""` de la fábrica sube `fn_805A5374` de 74 a 99,99%, pero el objeto original mide 4 bytes en `.rodata` (uno de 4 bytes iría a `.sbss2`) y la tabla, sus cadenas y ese `""` forman un segundo bloque de `.rodata`: parece **otro fichero fuente**. Revertido.

## Propuestas para el usuario (tocan `splits.txt`/`symbols.txt`, regla 3)

1. **Partir `game/SpecialEnemyBase`**: `fn_805A52F4` (nombre → índice) y la fábrica `fn_805A5374` usan otro bloque de `.rodata` (cadenas de `sEnemyNames`, la tabla 0x807636B8 y el `""` de 4 bytes 0x807637DC), separado del pool `@stringBase0` 0x807632A0 del resto. Encaja con un segundo fichero `.text` 0x805A52F4–0x805A6208, `.rodata` 0x80763330–0x807637E0 (más su parte de `.data`/`.sdata2`). Con el `""` en su propio pool, la fábrica pasa a ~99,99%.
2. **Pools escondidos en `lbl_`**: en 17 unidades sin `matching` el pool original está entero en `.rodata` pero dtk lo partió en objetos `lbl_` con `.4byte` (p. ej. `AI/AI_archer_attack01` 65 cadenas y 24 funciones < 100%, `atn/RenderSystem_unk` 22, `game/CharaStatus` 17, `Event/Path` 17, `atn/D2A_Texture` 16, `game/MapPartsBase` 15, `atn/WordWrap_unk` 14, `atn/Font_unk` 11). Unirlos en un `@stringBase0 scope:local data:string_table` exige borrar/fusionar símbolos, no solo renombrar. Lista con `poolorder.py --all` (`no-pool` con pool nuestro).

## Hallazgos (una línea en `docs/trucos-mwcc.md`)

- MWCC instancia los miembros de plantilla definidos fuera de la clase al final del fichero, en un orden que depende de qué miembros referencia el código del usuario (ctor de subcadena solo: `out_of_range` primero; con `string(const char*)` en el fichero, `reserve` primero). El original los emite justo detrás de su primer uso; una instanciación explícita en ese sitio reproduce copia y literales.
- `sprintf` con `"%s"`, `"%s.efp"`, `"Param%d"`, `"%s [%d]"` y NUL extra: patrón frecuente; `poolorder.py` lo delata como un `""` que falta justo detrás del formato.
