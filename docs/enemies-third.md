# enemies-third (propuesta H): segundas pasadas de enemies-finish y enemigos sin fuente anteriores a Em208Queen

Rama `enemies-third` desde `main` d51a317 (2026-10-22/29, sesión en la nube), con `main` 5125aea fusionado al cierre. Permisos del usuario: ajustar splits de estas unidades, renombrar en `symbols.txt` y push de la rama (no de `main`). Ningún `asm` ni `#pragma` nuevo. **Cerrado antes de tiempo** (límite semanal de uso y petición del usuario de terminar al cambiar `main`): quedan colas sin hacer (abajo).

## Resultado

Build limpio `main.dol: OK`; `regress.py` **lost: 0** contra `main` 5125aea, contra `main` d51a317 y contra el `report.json` final de cada cola; `fuzzdrop.py` 1 bajada aceptada (Em310Gargoyle `fn_8030D0D8` 99,79 -> 99,71, ya NONMATCHING). Avisos de dtk iguales que en `main` (16). Los 18 `scope:local` nuevos caen dentro de unidades con fuente. `poolcheck.py`: un aviso nuevo (Em310Gargoyle, abajo).

| | main 5125aea | enemies-third |
|---|---|---|
| Funciones al 100% | 7564 | **7825** (+261) |
| Código | 26,21% | 27,92% |
| Fuzzy | 43,46% | 47,48% |
| Datos | 17,46% | 17,66% |
| Unidades enlazadas | 370 | 370 (ninguna nueva) |

Ninguna unidad llega a `"matching": true`.

### Por unidad (funciones al 100% en report.json, antes -> después / total, fuzzy)

| Unidad | Cola | Antes | Después | Total | Fuzzy |
|---|---|---|---|---|---|
| `game/Em001Necromancer` (fuente nueva) | n1 | 0 | 17 | 29 | 79,0% |
| `game/Em201ZanGurgFinal` (fuente nueva) | n1 | 0 | 56 | 78 | 94,4% |
| `game/Em202_01_BSpider` (fuente nueva) | n2 | 0 | 14 | 19 | 99,2% |
| `game/Em203GreaterDaemon` (fuente nueva) | n3a | 0 | 36 | 66 | 91,9% |
| `game/Em203GreaterDaemonRed` (fuente nueva) | n3b | 0 | 27 | 53 | 94,8% |
| `game/Em204Mulue` (fuente nueva) | n4 | 0 | 29 | 39 | 99,8% |
| `game/Em204BigMulue` (fuente nueva) | n4 | 0 | 20 | 38 | 92,5% |
| `game/Em204MulueMp` (fuente nueva) | n4 | 0 | 23 | 31 | 99,8% |
| `game/Em301LShade` | s1 | 17 | 19 | 31 | 90,4% |
| `game/Em302Kraken` | s1 | 38 | 41 | 49 | 97,6% |
| `game/Em302KrakenMp` | s1 | 33 | 34 | 39 | 98,3% |
| `game/Em306GurgBeast` | s2 | 14 | 17 | 22 | 99,2% |
| `game/Em308InvertCrab` | s2 | 16 | 17 | 29 | 99,4% |
| `game/Em308ParmCrab` | s2 | 10 | 11 | 24 | 98,9% |
| `game/Em309Ghoul` | s3a | 14 | 20 | 33 | 98,9% |
| `game/Em309GhoulSimple` | s3a | 12 | 17 | 22 | 99,8% |
| `game/Em310Gargoyle` | s3a | 5 | 14 (9 en `objdiff-cli`) | 21 | 99,1% |
| `game/Em401_01_WhiteLeoPard` | s4 | 14 | 16 | 25 | 99,1% |
| `game/Em316LastLaser` | s4 | 8 | 8 (vf10 escrita, 95,3%) | 19 | sube |
| `game/Em403WarLion` | s5 | 41 | 47 | 67 | 98,9% |
| `game/EnemyJill` | s5 | 30 | 30 | 39 | 96,4% |

Notas por cola (resultado por función, NONMATCHING con motivo, hallazgos): `docs/enemies-third-{n1,n2,n3a,n3b,n4,s1,s2,s3a,s4,s5}.md`.

## No hecho (cierre anticipado)
- Sin fuente: `Em202ForestSpider`, `Em202MothersSpider` `Em203GreaterDaemonRedMp`, `Em206CocoonMp`, `Em206EggMonster`. Lo avanzado de ForestSpider (cabecera), RedMp (cabecera y fuente a medias) y CocoonMp (análisis del asm) está en `wip/enemies-third/` (no se compila; ver su README).
- Segundas pasadas no lanzadas: Em312DarkServant/Em314Ivy/Em315Coffin, Em316LaserSphere (+ resto de Em316LastLaser), EnemyQuarkFirst/Second, EnemyQuarkStand/EnemyTasha y la struct de `lbl_8087EFA8`.

## Decisiones a revisar (tomadas sin el usuario)
- **poolcheck nuevo en Em310Gargoyle:** 7 floats de más en `.sdata2` (255, ±3, 5, ±pi, 2pi) de copias inline que el original no emite en la unidad; ningún valor erróneo. Quitarlas rompe el inline de `fn_8030D0D8` en `vf10`.
- **Bajada aceptada:** Em310Gargoyle `fn_8030D0D8` 99,79 -> 99,71 (sigue autoinlineada en `vf10` como el original).
- **Cabeceras compartidas** (sin bajadas): `CharaTask::vf58` devuelve `atn::Vector3`, `vf70` devuelve `float`, `HitMsg_unk::m4C` `int` (n3a/n3b); `CharaStatus.h`: `RouteTable_unk::m98` `unsigned int`, `RoutePoint_unk::m10` `int`; `include/std/vector.h` ampliado (s5: resize, `vector(size_type)`, especializaciones de `float`, `capacity()`, `__allocate` público; n3b y s5: `__shrink_unk<float>`); `chr/ControlStickByQuark.h` declara `ControlThrustForGhoul`. Cabeceras nuevas `Em203GreaterDaemon.h`, `Em203GreaterDaemonRed.h`, `Em204Mulue.h`.
- **`CharaTask::vf60` -> `float` (pendiente de enemies-finish):** no cambiado; subiría 2 funciones (las dos `vf60` de los cangrejos). La base 0x800EF38C (`game/BattleCharaTask`, sin fuente) también devuelve float.
- **splits.txt:** `.data` de Em403WarLion termina en 0x80788D20 (antes 0x80788D00). Nada más.
- **symbols.txt:** renombrados de todas las unidades del lote; pools a `@stringBase0` (`scope:local`); tablas `@LOCAL@...@sOrder_unk`/`sRange_unk` en Em302Kraken; doubles de `.rodata` de BSpider a los `@NNNN` locales de nuestro .o (objdiff solo los empareja así); `lbl_80788D00` -> `@STRING@__allocate__...`; `fn_800D8814` -> `__ct__Q23atn11Color4f_unkFffff` (actualizado su llamador en `gmk/FlamingArrow.cpp`, `vf10` 97,34 -> 97,47).
- **`__dt__18NecroPlaceList_unkFv` (0x80244674, nombre de n1):** según n5 es el dtor de un `vector<EnemyTask*>` que también usan Em203GreaterDaemonRedMp, Em206CocoonMp y GameTask; el nombre probablemente está mal.
- **Factoría `SpecialEnemyBase.cpp`:** casos 1, 13, 17, 18, 20-22, 65 y Em309Ghoul con `new` real; sigue en DIFF (73,25%).
- **Asserts vacíos `((void)0)` en cantidad:** ~80 en `Em403WarLion::fn_8032AC1C`, ~50 en el accesor de Em309/Em310, hasta 30 por función en Em201ZanGurgFinal, y en 13 funciones de Em203GreaterDaemon. Es la técnica aceptada, pero el volumen es alto.
- **Código para colocar constantes/cadenas:** funciones inventadas que el enlazador quita (`fn_DebugMenu_unk` en Em403WarLion y WhiteLeoPard; `Stripped*_unk` en Em203GreaterDaemonRed); valor muerto `float unused_unk = t * 0.8f` en Em203GreaterDaemon `fn_8028AA38`; instanciaciones explícitas de `push_back` detrás de su función (n3a, n4, n3b).
- **Prototipos:** `fn_800F8C6C` sigue incoherente (EnemyJill mantiene `float, int`: con `int, float` bajan dos funciones); `fn_801077A8` con tres argumentos (s1); helpers pequeños como `extern "C" inline` por unidad (n1, n4).
- **Error del original reproducido:** Em309Ghoul `fn_80304FC0` escala las balas laterales desde `d0`.
- **Literales corregidos que objdiff daba por buenos:** Ghoul `vf0C` `m56C = 0.08f` (no 35.0f), Gargoyle `vf3C`/`vf40` (900.0f y 0.05f intercambiados), GhoulSimple `vf20` nodo `"spine1"`.
