# enemies-third, cola n1: Em001Necromancer y Em201ZanGurgFinal

Rama `agent-n1`. Referencia: `/home/claude/ref/n1_start.json` (7299 funciones).

## Resultado por unidad (report.json, antes -> despues / total)

| Unidad | Antes | Despues | Total |
|---|---|---|---|
| `game/Em001Necromancer` (fuente nueva) | 0 | 17 | 29 |
| `game/Em201ZanGurgFinal` (fuente nueva) | 0 | 56 | 78 |

### Em001Necromancer: NONMATCHING
- `vf0C` (0%): presupuesto de inline (mismo caso abierto que `Em301LShade::vf0C`). El original llama fuera de linea a todas sus candidatas (ctor/operator=/c_str de la ruta 0x80247674/0x802452B4/0x8024533C, los tres push_back, `FixedArray_unk::PushBack_unk` 0x80245310, los helpers de flags de Task 0x80244CA0-0x80244CBC). Medido: con una candidata extra de ~60 sentencias el cuerpo sale identico salvo nombres. No se deja la sonda en el codigo.
- Constructor (98.8%): registros del segundo `std::string` (caso conocido de Em201ZanGurg/Em101*).
- `vf14` (99.5%): r28/r29 del texto y del layout (mismo caso que `Em208Queen::vf14`).
- Quedan sin fuente (copias debiles que solo emite el vf0C fuera de presupuesto): 0x80244CA0, 0x80244CAC, 0x80244CBC, los dos push_back, 0x802452B4, 0x80245310, 0x8024533C, 0x80247674.

### Em201ZanGurgFinal: NONMATCHING
- `vf10`: presupuesto de inline (como `Em001Necromancer::vf0C`): el original llama fuera de linea a los helpers pequenos (fn_8026607C, fn_80265804, fn_802660EC/FC, fn_8026610C-fn_80266174, GetTrans_unk).
- Constructor: registros de los `std::string` (caso conocido).
- `vf3C` (12 lineas): r6/r7 de `msg->mData` y del bool de espada. `vf40` (35 lineas): huecos de pila de los temporales.
- `fn_80267134`: r5/r6 en la persecucion directa. `fn_802682C4`: orden de carga de los ids de movimiento.
- `fn_802692B4`, `fn_802696F8`, `fn_80269CBC`, `fn_8026D0F8`, `fn_8026F38C`, `fn_8026A9A0`: asignacion de registros/pila (10-15 variantes cada una).
- `fn_8026DB8C` (1600 lineas): escrita siguiendo la logica; el original inlinea la caida de `fn_8026D0F8(1)` junto al personaje golpeado y al inlinearla aqui se pasa el presupuesto.
- `vf98`/`vf9C`: solo difieren en el nombre del ptmf de `.data` (quedara al enlazar la unidad).

## Decisiones tomadas sin el usuario
- `symbols.txt`: renombrados los 20 `fn_` de Em001Necromancer a sus nombres reales; `lbl_8074320C` -> `@stringBase0` (`scope:local data:string_table`, como el resto de unidades) para que objdiff empareje el pool.
- `src/game/SpecialEnemyBase.cpp`: caso 1 de la factoria usa `new Em001Necromancer` (quitado el `extern "C" fn_80244228`).
- La ruta de "appear_effect" se modela con `atn::NandEmpty_unk` (0x100 bytes, destructor vacio 0x800A2D20 que ya existe en atn/NandManager.cpp con ese nombre); el ctor/operator= se definen en la unidad (no se toca NandManager).
- `fn_80244CA0/CAC/CBC` definidas como `extern "C" inline` en la unidad (otros ficheros las llaman por ese nombre: EnemyQuarkStand, GameUI, TownTask...).
- Em201ZanGurgFinal: renombrados 62 simbolos en `symbols.txt` (metodos y vfXX); `lbl_80744608` -> `@stringBase0` (0x2D0, `scope:local data:string_table`). Factoria caso 65 con `new Em201ZanGurgFinal` (quitado el extern `fn_80264DB4`). Helpers pequenos del original (fn_802657FC, fn_80265804, fn_8026607C, fn_802660EC/FC, fn_8026610C-fn_80266174, fn_80266FA4/FAC/FB4, fn_80267B20/B28/B30) como `extern "C" inline` en la unidad. `.sdata` `lbl_8087DC30/34` (0.0 y 50.0) definidos como floats globales con ese nombre (altura de la caida del escudo). Asserts vacios `ASSERT_UNK` (tecnica ya aceptada) en las funciones de estado que el original no inlinea en vf10/vf6C.

## Hallazgos generales
- `int id = m590;` antes de la llamada fija el orden `lwz r3 sym / mr r6 this` en las llamadas a fn_800F8C6C (Em201ZanGurgFinal::fn_8026A010).
- `__MtxRotate` con el angulo por referencia y la comparacion `0.0f != rad` reproduce la carga del angulo despues de la constante (fn_80268CB4).
- Colocar el resultado del barrido con un inline `SetPos_unk(this, result, sphere)` corrige r3/r4 de la copia de `result.m10`.
- `unsigned n <= 1` como bool da `subfic/orc/srwi/subf/srwi` (contador de vivos de Em001Necromancer::fn_80246244).
