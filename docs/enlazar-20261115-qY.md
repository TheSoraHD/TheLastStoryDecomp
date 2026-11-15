# enlazar-20261115 cola Y

Cola Y (corta): lo que la cola W dejo a un paso. Agente Y, 21:29 -> 21:55.

## Resultado por unidad

| Unidad | Antes | Despues | Nota |
|---|---|---|---|
| db/PaletteTable_unk | 13/14 | 14/14 | `fn_8020B2BC` 99,96 -> 100 %; **enlaza** |
| chr/ControlDamage | 38/41 | 38/41 | `ControlDamageLastOne::vf0C` 99,64 (variante de W) sin commit: parche `docs/enlazar-20261115-qY.patch` |

## Unidades que enlazan (bytes)

- `db/PaletteTable_unk`: `"matching": true`, build limpio `main.dol: OK`, `poolcheck`/`poolorder` limpios,
  `total_data` 1664 bytes (`complete_data` 1664). Funciones 15627 -> 15628.

## NONMATCHING con motivo

- `ControlDamageLastOne::vf0C` 99,64 % (con el parche): el puntero `model` de fuera va a r30 y el bool del
  `fn_801B0260` inline a r31 (original r31/r30). Quedan ademas el ctor (99,95) y `ControlKnockdown::vf0C` (98,1),
  asi que la unidad no enlazaria aunque llegue.

## Cambios de splits

Ninguno. `symbols.txt`: solo `@855` -> `@851` (0x80882E2C) del re-emparejado de PaletteTable.

## Para el port

Nada.

## Decisiones tomadas sin el usuario

- PaletteTable: `Palette_unk::SetName_unk` se sustituye por una inline estatica `CopyName_unk(wchar_t* dst,
  const wchar_t* src)` llamada con el campo del CSV directo: `CopyName_unk(sPalettes[index].mName, csv.fn_8005B9CC())`.
- ControlDamage: no se commitea la variante de W (99,4 -> 99,64): solo sube el fuzzy de una funcion que no es la
  ultima, y el `#include "math/VecAssign_unk.h"` renumera los `@N` (labelcheck peor, 5 faltan).

## Hallazgos (una linea cada uno)

- Copia a un buffer con el valor de una llamada: una inline libre `Copy(dst, src)` con la llamada como argumento
  da otro registro al valor que el metodo `obj.Set(src)` o un local (PaletteTable: r25 -> r30/r29 como el original).
- `ControlDamageLastOne::vf0C`: el cruce model/bool no cambia con orden de declaracion, bool/int, `== true`,
  referencia, accesor `GetModel_unk`, ni la forma del helper (resultado unico, switch); escrito en el sitio, MWCC
  deja de recargar `mChara` (el original lo recarga: hace falta la llamada inline).

## Pendientes

- `ControlDamageLastOne::vf0C`: aplicar `docs/enlazar-20261115-qY.patch` (99,64) solo cuando el ctor y
  `ControlKnockdown::vf0C` esten cerca; re-emparejar los `@N` (`pd.sh`) y revisar labelcheck.
