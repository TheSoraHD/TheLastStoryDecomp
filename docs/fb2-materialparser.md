# Propuesta F-b2: terminar `atn/MaterialParser_unk` (segunda parte)

Rama `fb2-materialparser`, sale de `main` 699b7aa. 2026-10-30. Trabajo secuencial, sin agentes.

## Resultado

- **8747 → 8750** funciones al 100% (+3), `main.dol: OK`; `regress` lost: 0, `fuzzdrop` sin bajadas, `poolcheck`/`ordercheck`/`mnemcheck` limpios contra el `report.json` del inicio.
- `atn/MaterialParser_unk`: **21/35** al 100% (antes 18). Nuevas: `fn_80081080`, `fn_800816B4` y el tokenizador `fn_8007A994`. La unidad sigue `matching: false`.
- `@stringBase0` idéntico al original (orden y contenido, salvo el relleno final de alineación).

| Función | Antes | Ahora | Qué falta |
|---|---|---|---|
| `fn_8007A994` (tokenizador) | 98,1 | **100** | — |
| `Material_unk::fn_80081080` | 99,8 | **100** | — (era el pool de cadenas) |
| `Material_unk::fn_800816B4` | 99,0 | **100** | — |
| `fn_80080C20` | 96,6 | 99,84 | r15/r24 entre el puntero del nombre y su longitud |
| `fn_8007B260` | 99,5 | 99,84 | r22–r28 rotados |
| `fn_80080048` | 90,0 | 99,38 | f1/f2 entre velocidad y desplazamiento en cada `fmadds` |
| `fn_8007B100` | 95,2 | 99,38 | `obj`/`minFilter` cruzados (r30/r31) |
| `Swap_unk` | 99,4 | 99,4 | registros del `String_unk::assign` inline |
| `fn_80080528` | 98,5 | 98,5 | r6/r7 (base/offset); 10 variantes sin efecto |
| `fn_80080B34` | 98,3 | 98,3 | registros del `assign` inline (mismo patrón que `Swap_unk`) |
| `SelectionSort_unk` | 98,2 | 98,2 | registros |
| `fn_8007C46C` (parser) | 97,0 | 97,31 | forma de la macro `FIND_NAME` (ver abajo) y registros |
| `fn_8007FE5C` | 91,8 | 96,38 | el original carga `mA30` tras insertar `mA28` (sin barrera de alias) |
| `fn_80080584` | 94,8 | 95,93 | planificación de las copias de campos (`m34`/`m00`) |
| `Sort_unk` | 95,9 | 95,9 | orden del índice del segundo pivote y registros |
| `fn_8007AB38` | 91,7 | 91,7 | el original sube cargas por encima de los stores float |
| `Material_unk::Material_unk` | 87,5 | 90,62 | planificación de las tres conversiones `ToGX` |

## Cambios

1. **Pool de cadenas.** El ayudante del menú de depuración de la etapa (`TexStageMenu_unk`) deja de ser `static`: MWCC lo emite en su sitio (sus literales `%s`, `ScaleX`… abren el pool, como en el original) y además lo inlinea en `fn_800816B4`; el enlazador quita la copia (como `TexStage_unk::operator=`). Arregla los offsets de `@stringBase0` de todas las funciones posteriores.
2. **`#include "std/string_members.h"`**: el pool original termina con los mensajes de `basic_string` (`reserve length_error`, `out_of_range`, `length_error`), igual que `GimmickManager` y `EquipItemData`. El parser sube 0,24 puntos.
3. **`SwapTable_unk`** (nuevo tipo en `include/atn/Material_unk.h`) para `Material_unk::m38`, con constructor `0,1,2,3`: el original lo escribe en la inicialización de miembros. `fn_8007B260` copia la tabla con `SetSwap_unk(swap, mat->m38)` (plantilla), y `ModelDraw_unk` recibe `SwapTable_unk` en sus `Swap*_unk` (sin cambios en su código).
4. Comparaciones de cadenas como `!(s == "")` / `s == "bake_tex"` en lugar de `s.compare("") != 0`: con esa anidación de inlines, los cuatro huecos de pila del `compare` inline salen en el orden del original (con `compare` directo salen al revés).
5. Locales que reproducen el orden de cargas del original: campos de `desc` leídos antes de las escrituras de campos de bits (`fn_8007FE5C`), velocidad de fotograma y periodo de la onda (`fn_80080048`), sesgo LOD (`fn_8007B100`), copia local del puntero de inicio (tokenizador).
6. `fn_80080048`: la onda en una inline `Wave_unk` (evita que MWCC funda `x * sin(...)` con la suma: el original tiene `fmuls` + `fadds`).
7. `fn_80080C20`: el `boost::function` se prueba con `getter != 0` (compara el puntero de vtable directamente, como el original); `empty()` materializa un booleano.
8. `fn_800816B4`: el título es `char[0x100]` (se trunca a 0x1F igualmente).

## Decisiones tomadas sin el usuario

1. Tipo nuevo `SwapTable_unk` en la cabecera compartida `Material_unk.h` (antes `GXColor`). Solo lo usan `MaterialParser_unk`, `Shader_unk` (escribe `.r/.g/.b/.a`, sin cambios) y `ModelDraw_unk` (parámetro de sus `Swap*_unk`). Sin bajadas.
2. Comentario en `include/std/string.h` sobre el uso de `operator==(const basic_string&, const T*)` inlineado aquí; el código de la cabecera no cambia.
3. Probado y descartado: otra forma de `FIND_NAME` con la salida `-1` dentro del bucle (parser 97,3 → 95,7%); cambiar el orden de los locales de `compare` en `string.h` (sin efecto); variantes de `String_unk::assign`/`operator=` (IR idéntico).

## Siguiente paso

- El parser sigue limitado por `FIND_NAME`: el original tiene la forma de una búsqueda con `return` (la inline `FindName_unk`), pero con ella el parser se pasa del presupuesto de inline. Habría que encontrar qué otra inline del parser no existía en el original.
- `fn_8007AB38` y `fn_8007FE5C` apuntan a lo mismo: el original no supone alias entre los campos de `ShaderDesc_unk` y los stores de la etapa/material. Una copia local del campo de bits `m14` (tipo con nombre) podría explicarlo en `fn_8007FE5C`.
