# Cola N (lote paralelo-20261113): unidades cerca de enlazar

Unidades fuera de la ruta con una sola funcion por debajo del 100 % (lista en `/home/claude/scratch/n-candidates.txt`).

## Resultado por unidad

- `chr/ControlBase`: vf18 99,93 -> 100 %; **enlaza** (`matching: true`). Los tres bloques de rotacion van en
  una inline `RotZYX_unk(Matrix3x4*, const Vector3&)` con `Mtx t; Mtx r;` en cada bloque.
- `game/Arms`: vf10 99,90 -> 100 %; **enlaza**. Bloques de rotacion en una inline `SetRotYXZ_unk` dentro de
  `RotateYXZ_unk` (sin `Rotate_unk`), y `SetMtx_unk(model, mtx)` (como game/Gun.cpp) para la escala.
  Para enlazar: `m29C.get()->IsLoading()` en vf0C (la copia debil de `scoped_ptr::get` sale tras vf0C) e
  `#include "atn/Csv.h"` antes de `game/Arms.h` (vtable/RTTI de Arms antes que los de CsvTab en `.data`).
- `game/NewStaffRoll`: fn_80531394 99,86 -> 100 %; **enlaza**. `atn::CsvU csv((const wchar_t*)mLoader.fn_80470580(),
  (unsigned int)mLoader.fn_8047059C() / 2);` (argumentos en la llamada, sin locales).
- `game/GameLoadTask` vf10 (99,96): sin cambio, ~6 intentos (buffer/puntero borrado en r29/r30 cruzados).

## Hallazgos

- Orden de pila de bloques repetidos con locales `Mtx t; Mtx r;`: directos en una inline -> orden de
  declaracion; en una inline dentro de otra -> invertido entero. Probar un nivel mas de inline (ControlBase vf18, Arms vf10).
- Escala por columnas `s` debajo de los temporales: `SetMtx_unk` con `float sz/sy/sx = ColumnLength_unk(...)` (Arms vf10).
- `atn::CsvU csv(a(), b() / 2)` con los argumentos en la llamada da registros salvados mas altos que con locales (NewStaffRoll).
- Orden de vtables/RTTI en `.data`: incluir `atn/Csv.h` antes de la cabecera de la clase pone la vtable de la clase
  antes que las de CsvTab/CsvBase (Arms).
- Copia debil de `scoped_ptr::get` tras la primera funcion que llama a `get()` explicito (no a `if (p)`) (Arms vf0C).

## Decisiones tomadas sin el usuario

- Ninguna fuera de lo normal: solo `matching: true` en las unidades que dan `main.dol: OK`.

## Pendientes
