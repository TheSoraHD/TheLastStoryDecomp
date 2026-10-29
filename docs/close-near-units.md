# Lote `close-near-units` (propuesta K, 2026-10-29)

Cerrar unidades casi enlazables: `MP/Popup`, `db/EventIdTable_unk`, `gmk/SparkCube` y los enemigos de la tabla de `SIGUIENTE_sesion.md`. Trabajo secuencial, sin agentes.

## Resultado

- Funciones al 100%: 8180 → 8180. Enlazados: 370 → **372** (`gmk/SparkCube`, `gmk/Banana`). `main.dol: OK`, `regress.py` lost: 0, `fuzzdrop.py` sin bajadas, `poolcheck.py` sin avisos en lo tocado.

| Unidad | Antes | Después | Nota |
|---|---|---|---|
| `gmk/SparkCube` | 19/19, `matching: false` | enlazada | La estática local `sListeners` de `fn_8014F8F4` enlaza sin más, como en `MP/CharaListeners_unk` (su registro de destructor ya estaba en el `.bss` del split). La nota antigua decía "no se prueba". |
| `gmk/Banana` | 12/12, `matching: false` | enlazada | El destructor era `virtual ~Banana() {}` en la cabecera y salía al final del `.o`; ahora `Banana::~Banana() {}` va en el `.cpp` justo tras la factoría (0x80416764). `vf40` movida a su sitio con `reorder.py`. |
| `MP/Popup` | `vf10` 97,58% | `vf10` 99,81% | Ver abajo. Solo falta el test de la bandera de la tarea de campo. |
| `db/EventIdTable_unk` | 7/7, sin enlazar | sin cambios | Ver "Bloqueos". |
| `std/bad_cast`, `std/bad_cast_what` | 1/1, sin enlazar | sin cambios | `multiply-defined: std::bad_cast::__vt` (la vtable es de `GameEventScript`): son copias débiles partidas en unidades propias; no enlazan sueltas. |
| `std/runtime_error` | — | sin cambios | `ordercheck`: 0 emparejadas, 7 que faltan. No se mira. |
| Enemigos (Ogre, DarkServant, KrakenFootMp, Coffin, Tasha, Vampire) | — | sin cambios | Lo que les falta son bloqueos documentados: `__VecCopy` r3/r4, numeración de registros/FPR y orden del pool por inline débiles. Ver "Bloqueos". |

## `MP::Popup::vf10` (0x80501588), 97,58% → 99,81%

- Índice del array de tareas: `m48.mData[(unsigned int)i]`. Con `int` MWCC funde `this + i*4` en una sola inducción (desplaza todos los registros en uno); con el índice sin signo guarda `this` y el desplazamiento por separado, como el original. El contador sigue siendo `int` (el original compara con `cmpwi`); `unsigned int i` en el bucle da `cmplwi`.
- `atn::Vector3 screen` dentro del segundo bucle y la actualización del hueco (`front`, `mtx`, `PSMTXRotRad`, producto escalar) en un inline `SetSlot_unk(p, g, dist, dir, chara)`: los locales del inline van por debajo de los del llamador y salen los huecos de pila del original (front 0x8, screen 0x14, dir 0x20).
- Floats de color: `float add, b, gr, r;` y luego las asignaciones en el orden del pool (30, 20, 100, 200). Los FPR salen en orden de declaración (f30, f29, f28, f27); reordenar las declaraciones con inicializador cambia también el orden del pool, así que se separa declaración y asignación.
- Queda: el original hace `beq cuerpo; b epílogo` en el test `IsFlag_unk(2)` de `lbl_8087F430`, aquí un solo `bne`. Probado: guardas separadas, el cuerpo dentro de un `if`, la guarda como inline `bool` (tres formas), `a != 0 && flag` con el cuerpo dentro, y la guarda más el cuerpo como inline miembro (la pila crece y baja al 76%).

## Bloqueos (para no repetir)

- `db/EventIdTable_unk`: el `.rodata` del original **no** tiene las cadenas RTTI de `atn::Csv`/`CsvBase<char,44,13,34>` y **sí** tiene tres de `basic_string` (`"basic_string::reserve length_error"`, `"basic_string: out_of_range"`, `"basic_string: length_error"`) tras `CAPTURE/CAMERA/TV/ADD`. El fuente original usaba algo de `std::string` (inline débiles cuyas cadenas quedan en el pool) y no emitía las vtables de `Csv` aquí. Con `matching: true` enlaza pero el `.data` global se mueve 0x60. Las demás unidades de `db/` que construyen `atn::Csv` sí tienen esas cadenas y enlazan, así que no basta con tocar `atn/Csv.h`.
- `Em315Coffin::vfA0` (8 bytes, 97,5%) y otras al 99,9x%: es el orden del pool de `.sdata2`. `atn::Vector3::ToRotation_unk` es inline en `include/math/Vec.h` y sale como copia débil en el `.o`, que añade pi, 2pi y -pi al pool y cambia el orden. El original lo llama fuera de línea (0x80011034).
- Varias unidades de enemigos tienen hoy menos funciones al 100% que en la tabla de `SIGUIENTE_sesion.md` (Tasha 12/25, Vampire 20/30, Coffin y DarkServant 15/21). Las que faltan son de pool o de registros, no regresiones de este lote (el `report.json` de referencia es el de `main`).

## Decisiones tomadas sin el usuario

- `include/gmk/Banana.h`: `virtual ~Banana();` (antes inline vacío) y su definición en `src/gmk/Banana.cpp`, como ya estaba `SparkCube`. `game/GimmickCommon.cpp`, que incluye la cabecera, sigue al 100%.
- Ningún cambio en `splits.txt`, `symbols.txt`, `configure.py` ni en los flags.
