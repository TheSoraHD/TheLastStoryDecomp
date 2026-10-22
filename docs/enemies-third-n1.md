# enemies-third, cola n1: Em001Necromancer y Em201ZanGurgFinal

Rama `agent-n1`. Referencia: `/home/claude/ref/n1_start.json` (7299 funciones).

## Resultado por unidad (report.json, antes -> despues / total)

| Unidad | Antes | Despues | Total |
|---|---|---|---|
| `game/Em001Necromancer` (fuente nueva) | 0 | 17 | 29 |

### Em001Necromancer: NONMATCHING
- `vf0C` (0%): presupuesto de inline (mismo caso abierto que `Em301LShade::vf0C`). El original llama fuera de linea a todas sus candidatas (ctor/operator=/c_str de la ruta 0x80247674/0x802452B4/0x8024533C, los tres push_back, `FixedArray_unk::PushBack_unk` 0x80245310, los helpers de flags de Task 0x80244CA0-0x80244CBC). Medido: con una candidata extra de ~60 sentencias el cuerpo sale identico salvo nombres. No se deja la sonda en el codigo.
- Constructor (98.8%): registros del segundo `std::string` (caso conocido de Em201ZanGurg/Em101*).
- `vf14` (99.5%): r28/r29 del texto y del layout (mismo caso que `Em208Queen::vf14`).
- Quedan sin fuente (copias debiles que solo emite el vf0C fuera de presupuesto): 0x80244CA0, 0x80244CAC, 0x80244CBC, los dos push_back, 0x802452B4, 0x80245310, 0x8024533C, 0x80247674.

## Decisiones tomadas sin el usuario
- `symbols.txt`: renombrados los 20 `fn_` de Em001Necromancer a sus nombres reales; `lbl_8074320C` -> `@stringBase0` (`scope:local data:string_table`, como el resto de unidades) para que objdiff empareje el pool.
- `src/game/SpecialEnemyBase.cpp`: caso 1 de la factoria usa `new Em001Necromancer` (quitado el `extern "C" fn_80244228`).
- La ruta de "appear_effect" se modela con `atn::NandEmpty_unk` (0x100 bytes, destructor vacio 0x800A2D20 que ya existe en atn/NandManager.cpp con ese nombre); el ctor/operator= se definen en la unidad (no se toca NandManager).
- `fn_80244CA0/CAC/CBC` definidas como `extern "C" inline` en la unidad (otros ficheros las llaman por ese nombre: EnemyQuarkStand, GameUI, TownTask...).

## Hallazgos generales
- `unsigned n <= 1` como bool da `subfic/orc/srwi/subf/srwi` (contador de vivos de Em001Necromancer::fn_80246244).
