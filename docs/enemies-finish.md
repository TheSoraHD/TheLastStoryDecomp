# enemies-finish (propuesta G): terminar los enemigos empezados en close-units

Rama `enemies-finish` desde `main` 64df7a6 (2026-10-22, sesion en la nube, 09:00-21:00 aprox.). Permisos del usuario: ajustar splits de estas unidades, renombrar en `symbols.txt` y push de la rama (no de `main`). Excepciones a la regla 2: solo las aprobadas (ningun `asm` ni `#pragma` nuevo). El usuario aprobo durante el lote renombrar `src/game/Em005DoppelGanger.cpp` a su nombre real.

## Resultado

Build limpio `main.dol: OK`; `regress.py` **lost: 0** y `fuzzdrop.py` **0 bajadas** contra main 64df7a6, contra cada referencia de integracion y contra el `report.json` final de cada cola. Sin `asm`/`#pragma` nuevos, sin cambios en `configure.py`, `config.yml`, `libs_ref.json` ni flags; avisos de dtk iguales que en main; los 53 `scope:local` nuevos estan dentro de unidades con fuente.

| | main 64df7a6 | enemies-finish |
|---|---|---|
| Funciones al 100% | 6806 | **7299** (+493) |
| Codigo | 22,57% | 26,02% |
| Fuzzy | 35,45% | 42,56% |
| Datos | 17,29% | 17,46% |
| Unidades enlazadas | - | ninguna nueva |

Ninguna unidad llega a `"matching": true`: a todas les queda alguna funcion NONMATCHING (familias conocidas: `__VecCopy` r3/r4, `push_back` en bucles, segundo `std::string` de los ctores, slots de pila) o alguna funcion grande sin escribir.

### Por unidad (funciones al 100% en report.json, antes -> despues / total, fuzzy de la unidad)

| Unidad | Antes | Despues | Total | Fuzzy |
|---|---|---|---|---|
| `game/Em003LesserDaemon` | 7 | 14 | 22 | 99.8% |
| `game/em005_doppel_ganger` (antes `Em005DoppelGanger`) | 5 | 9 | 17 | 98.4% |
| `game/Em101ElderZesya` | 8 | 12 | 20 | 99.2% |
| `game/Em101ElderZesyaSecond` | 3 | 12 | 15 | 98.9% |
| `game/Em101LittleZesya` | 13 | 19 | 25 | 99.1% |
| `game/Em101LittleZesyaSecond` | 3 | 9 | 15 | 99.4% |
| `game/Em202Spider` | 4 | 15 | 25 | 99.3% |
| `game/Em202SpiderBase` | 3 | 15 | 23 | 99.3% |
| `game/Em205Ogre` | 14 | 19 | 21 | 99.8% |
| `game/Em208Queen` (fuente nueva) | 0 | 22 | 29 | 99.6% |
| `game/Em209SmallGolem` | 7 | 16 | 22 | 98.7% |
| `game/Em301LShade` (fuente nueva) | 0 | 17 | 31 | 83.2% |
| `game/Em302Kraken` (fuente nueva) | 0 | 38 | 49 | 82.2% |
| `game/Em302KrakenMp` (fuente nueva) | 0 | 33 | 39 | 98.3% |
| `game/Em303KrakenFoot` | 13 | 20 | 26 | 99.7% |
| `game/Em303KrakenFootMp` | 15 | 18 | 21 | 99.9% |
| `game/Em305Vampire` | 15 | 26 | 30 | 98.6% |
| `game/Em306GurgBeast` | 9 | 14 | 22 | 59.4% |
| `game/Em308InvertCrab` (fuente nueva) | 0 | 16 | 29 | 93.7% |
| `game/Em308ParmCrab` | 3 | 10 | 24 | 92.9% |
| `game/Em309Ghoul` | 8 | 14 | 33 | 21.9% |
| `game/Em309GhoulSimple` | 9 | 12 | 22 | 41.5% |
| `game/Em310Gargoyle` | 2 | 5 | 21 | 29.3% |
| `game/Em312DarkServant` | 12 | 19 | 21 | 99.8% |
| `game/Em314Ivy` | 4 | 11 | 18 | 45.7% |
| `game/Em315Coffin` | 7 | 18 | 21 | 99.5% |
| `game/Em316LaserSphere` (fuente nueva) | 0 | 14 | 40 | 40.7% |
| `game/Em316LastLaser` | 6 | 8 | 19 | 27.4% |
| `game/Em401_01_WhiteLeoPard` | 1 | 14 | 25 | 48.4% |
| `game/Em403WarLion` (fuente nueva) | 0 | 41 | 67 | 82.8% |
| `game/EnemyJill` (fuente nueva) | 0 | 30 | 39 | 96.4% |
| `game/EnemyQuarkFirst` | 4 | 11 | 15 | 60.1% |
| `game/EnemyQuarkSecond` (fuente nueva) | 0 | 42 | 70 | 45.2% |
| `game/EnemyQuarkStand` (fuente nueva) | 0 | 55 | 80 | 97.7% |
| `game/EnemyTasha` (fuente nueva) | 0 | 20 | 25 | 99.7% |

Fuzzy bajo (< 60%) = quedan funciones grandes sin escribir (vf10/estados de varios KB).

## Alcance y colas (4 agentes a la vez; notas en `docs/enemies-finish-q{1..12}.md`)

Las 24 unidades con fuente nueva a medias de close-units E/F y, "de Em208Queen en adelante", las 11 unidades de enemigos sin fuente con >= 20 funciones a partir de 0x802C7BE4 **por direccion** (no se guardo el orden exacto de la cola F): Em208Queen, Em301LShade, Em302Kraken, Em302KrakenMp, Em308InvertCrab, Em316LaserSphere, Em403WarLion, EnemyJill, EnemyQuarkSecond, EnemyQuarkStand, EnemyTasha. Quedan fuera los enemigos sin fuente anteriores a Em208Queen que la cola F no toco (Em001Necromancer, Em201ZanGurgFinal, Em202ForestSpider/MothersSpider/_01_BSpider, Em203*, Em204*, Em206*).

- Ganancias que dio cada cola al terminar (sobre su referencia de inicio): q1 Em003-Em101* (+26), q2 Em202*-Em209SmallGolem (+49), q3 Em301-Em302* (+83), q4 Em303-Em308 (+33), q5 Em309-Em315 (+37), q6 Em316*-Em401_01 (+29), q7 Em403WarLion/EnemyJill (+71), q8 EnemyQuarkFirst/Second (+49), q9 EnemyQuarkStand/EnemyTasha (+75).
- Segundas pasadas con los huecos libres: q10 (unidades de q1, +10), q11 (unidades de q2, +10), q12 (unidades de q4, +16).
- Tanda inicial con colas no adyacentes (q1, q3, q5, q7) para reducir choques; cada cola nueva desde la rama de integracion actualizada.

## Integracion
- Conflictos recurrentes en la factoria de `game/SpecialEnemyBase.cpp` (`fn_805A5374`): cada cola sustituye los `extern "C"` de los ctores de sus enemigos por `new (3, "", "", 0) EmXXX(parent, info)`; se resuelve conservando todos los `#include` y quitando los `extern` sustituidos. La factoria sigue al 73,3%.
- q4 renombro `Em303KrakenFoot::fn_802E55B0` y q3 (Em302Kraken) la llamaba como `extern "C"`: ahora la llama como miembro.
- q8 y q9 definian `IntVector_unk` en sus cabeceras: movido a `include/game/IntVector_unk.h`.
- Nombres de las copias debiles de Em316LaserSphere (0x803161F8-0x80316A68): se conservan los de q6 (cola propietaria; q8 y q9 las habian llamado `MtxBlock_unk`/`Mtx34_unk`), sin bajadas en ningun lado.
- `symbols.txt` con `symresolve.py`; `@STRING@reserve__...Item14_unk...` en 0x8087D9E0/E4 con la forma de 4 bytes de sus vecinos.

## Comprobaciones nuevas (objdiff no las hace)
- **Valores de `.sdata2`** (`/home/claude/work/sdata2vals.py` en la sesion; idea: valores de nuestro pool que no existen en el pool original de la unidad): q10 encontro `Em101ElderZesya::vf50` con 1.5f donde el original tiene 10.0f (objdiff lo daba al 100%). Al cierre no queda ningun valor erroneo; solo constantes de mas: 5.0f en Em209SmallGolem (ctor inline de `CharaTask::Item14_unk`), +-pi/2 en Em312DarkServant y +-pi, 2pi en Em315Coffin (probablemente un helper inline de angulos que el original no emite en esas unidades).
- **Cadenas**: q9 corrigio "SE_DG040_109", "SE_DG040_107" y "head" en EnemyQuarkStand; al cierre, `Em201ZanGurg` (de main) tenia "AttackFrequcy" donde el original dice "AttackFrequicy": corregido. Queda el RTTI `@unnamed@EnemyTrista_cpp@` (el original usa otro nombre de fichero; de main, fuera del lote).
- **report.json frente a `objdiff-cli diff`**: varias colas vieron funciones al 100% en el report que el diff directo da al 99.5-99.9% (solo cambian etiquetas/posiciones de constantes del pool); no se contaron como ganadas en las notas y quedan marcadas NONMATCHING en el codigo, pero el contador del report si las incluye.

- **Herramienta nueva `tools/decomp/poolcheck.py`** (las dos comprobaciones anteriores para cualquier unidad). Fuera del lote da 25 unidades con avisos, a revisar: floats que el original no emite en esa unidad (helpers de angulos con +-pi/2, pi, 2pi, FLT_EPSILON en CharaLook_unk, FlamingArrow, Cannonball, ControlMoveCursor; +-1e16 en MapParts*; 0.98 en gmk/Chain; 0.7071 en ControlPushedDown; DegToRad en CharaTask), cadenas de tipos de `boost::function` con nombres inventados (`void (*)(MapModel_unk *, void *)` en CharaLight/Fireworks/RoomLight, el enum de ControlAppear, `GameLoadTask` en SequenceUnitParam1), "character not found! %s" en CharaTask, y cadenas de depuracion de librerias (NWC24, DLMalloc, GameSpy) que en el original no estan (probablemente compiladas fuera).

## Decisiones a revisar (tomadas sin el usuario)
- **Cabeceras compartidas:** `CharaTask::vfB0` -> `int vfB0(atn::Vector3*)` (q2/q3/q5 coincidieron), `vf1C` -> `int vf1C(atn::Matrix3x4*, ModelNodeDesc_unk*)` (q2, con `CharaTask::fn_80148CC4` ajustada), `vfE0` devuelve `float` (q9), `CharaTask::fn_8016C7D8` recibe `CharaTask* holder` (q9); `atn/EffectModel_unk.h` ctor con `flags = 9` por defecto y `atn/Model_unk.h` con una segunda callback de pose `m35C` (q6); `HitInfo_unk::m3C` expuesto (q9); declaraciones de `chr::ControlMatHoldPre` y `chr::ControlThrustForQuarkStand` en `chr/ControlStickByQuark.h`. Todo sin bajadas.
- **Pendiente (no tocado):** `CharaTask::vf60` deberia devolver `float` (los cangrejos devuelven 8.0f); el ctor inline de `CharaTask::Item14_unk` emite un 5.0f que el original de Em209SmallGolem no tiene (5 funciones de la unidad subirian al 100%, pero Em205Ogre si lo necesita).
- **splits.txt:** `game/Em005DoppelGanger.cpp` -> `game/em005_doppel_ganger.cpp` (aprobado); `.rodata` de Em209SmallGolem termina en 0x80747080 y la de Em301LShade empieza ahi (tres doubles que solo usa Em301) y se amplia hasta 0x807473F8; `.rodata` de Em305Vampire 0x80747F58-0x80748238; `.bss` nuevo 0x807C8398-0x807C83A8 en Em202SpiderBase (color que construye su `__sinit` y que tambien leen Em202ForestSpider, Em202_01_BSpider y Em308ParmCrab).
- **Trucos de orden y autoinline:** `((void)0)` en Em301LShade, Em316 (4), Em101LittleZesya (3-12 en diez estados), Em308InvertCrab/ParmCrab (4); funciones inline sin emitir solo para colocar constantes (`WarLionStripped1_unk`, `Stripped1_unk` de EnemyJill); `frame / 15.0f` sin uso en Em302Kraken/KrakenMp; copia sin uso en `MovePos_unk` de SpiderBase (el original deja el store); objeto `static Color4f_unk sUnused_unk` en Em310Gargoyle para su `__sinit` vacio.
- **`EnemyQuarkSecond::fn_8033A7A0` declarada `inline`:** asi vf10 supera el presupuesto y sale al 100%, pero fn_8033A7A0 no se emite (0%): impide enlazar la unidad tal cual. Revisar.
- **Prototipos incoherentes entre ficheros** (`extern "C"`): `fn_800F8C6C` (`float, int` en EnemyJill, `int, float` en Em403WarLion; devuelve `int` en EnemyQuarkStand y `void` en Em201ZanGurg); `fn_8020A81C` con `const char*` en Em005/Em209SmallGolem y con `int` en CharaTask.
- **Renombrados en `symbols.txt`:** metodos de todas las clases nuevas, pools a `@stringBase0` (con `scope:local`), simbolos de `boost::bind` (RTTI, `@LOCAL@`, `@GUARD@`); copias debiles de otras unidades: `fn_802BABC0` -> `__ct__9ResHandleFv` (en Em206CocoonMp), 0x803165E0 -> `__ct__Q23atn9Matrix3x4Fv`, `fn_80197528` -> ctor de `chr::ControlMatHoldPre`, `fn_80323C34` -> `TransMtx_unk__Ffff`, 0x80288390/0x8000D760 (IntVector_unk).
- **Clases y vistas locales con nombres inventados** (marcadas `// inferred:`): `Field_unk` y su layout de camara, `QuarkGame_unk`/`TashaGame_unk` (la misma struct de `lbl_8087EFA8`, ya en tres fuentes), `WarLionPath_unk`/`Cubic_unk`/`Len_unk`, `GolemMaterial_unk`, etc. Unificar cuando haya cabeceras reales.
- **Em005:** la clase `ControlCall` (namespace anonimo) esta escrita (dtor al 100%); el fichero ya tiene el nombre real.

## Pendientes
- Escribir lo que falta: EnemyQuarkSecond (15 estados, ~27 KB), EnemyQuarkFirst vf108, Em306GurgBeast vf104 y fn_802F09D4, huidas de los cangrejos (fn_802F693C, fn_802FC2E0), helpers de vector/spline de Em403WarLion (fn_8032AB90..fn_8032C1EC), funciones grandes de Em309Ghoul/GhoulSimple, Em310Gargoyle, Em314Ivy, Em316*, Em401_01.
- Bloqueo comun: copia de los parametros de camara en registros float (EnemyTrista::fn_8035D294, EnemyQuarkStand fn_8034B8CC/fn_8034ECE8, Em101LittleZesyaSecond fn_80261DC0, EnemyJill).
- Unificar las clases de los Quark (First/Second/Stand) y la struct de `lbl_8087EFA8`.
