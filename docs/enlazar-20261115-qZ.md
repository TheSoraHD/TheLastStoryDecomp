# enlazar-20261115, cola Z

Cola Z (corta): chr/ControlDamage (2,3 KB de datos, 38/41). Agente Z, 21:57 -> 22:15.

## Resultado por unidad

| Unidad | Antes | Despues | Nota |
|---|---|---|---|
| chr/ControlDamage | 38/41 | 38/41 | sin commit de codigo: parche `docs/enlazar-20261115-qZ.patch` (incluye el de qY) |

Con el parche: ctor de ControlKnockdown 99,95 (igual), `ControlKnockdown::vf0C` 98,1 -> 99,70,
`ControlDamageLastOne::vf0C` 99,35 -> 99,64 (el de qY), el resto al 100 %.

## Unidades que enlazan (bytes)

Ninguna.

## NONMATCHING con motivo

- `ControlKnockdown::ControlKnockdown` (99,95): ruta del motion (r29, original r26) y strlen (r26, original r29)
  cruzados en `std::string(path)`. Probado ademas de qG/qW: `handle` declarado en el `if`, `path` declarado
  al principio del ctor o antes del `if`, `const char* const`, sin local `path` (cruza path/temporal r25/r26,
  99,94), helper inline `FileName_unk(path)` (95,3, cambia la pila), `std::string path(...)` con nombre (98,0),
  `name` asignado en un bloque (91,8), `strlen` directo en el ctor de `std/string.h` (sin cambio).
- `ControlKnockdown::vf0C` (99,70 con el parche): quedan solo registros: el bool del `fn_801AE41C` inline r28
  (original r27) y el modelo del `GetNodeMtx_unk` inline r27 (original r26); mChara r5/r4 y la base de `sSe`
  r4/r3 en los dos sonidos; r3/r4 cruzados en la copia `pos = res.m10`. Probado sin cambio: `CharaTask* c`
  local, `atn::SoundRef` con nombre, `const char* se` local o cast en los sonidos; `VecAssign_unk`,
  `pos(res.m10)`, `pos; pos = ...`, `*&res.m10` en la copia; sin `chara` local / modelo primero (99,54);
  variantes del helper (`return a && b` 99,66, `if (m14 > 15) return true` y sin `frame` bajan el helper).
- `ControlDamageLastOne::vf0C` (99,64, parche de qY): modelo r30 / bool r31 (original r31/r30). Probado ademas:
  `CharaTask* c` local fuera o dentro del helper (97,6), cast en el helper (igual).

## Cambios de splits

Ninguno. El parche renombra `fn_801AE41C__Q23chr16ControlKnockdownFv` -> `...CFv` en symbols.txt
(helper `const`; lo llama `CharaTask.cpp`, que se recompila con la cabecera).

## Para el port

Nada (ControlDamage no es unidad del port). El parche hace `const` `ControlKnockdown::fn_801AE41C`
(`include/chr/ControlDamage.h`).

## Decisiones tomadas sin el usuario

- No se commitea codigo: ninguna funcion llega al 100 % y la unidad no enlaza; todo queda en el parche.

## Hallazgos (una linea cada uno)

- Accesor inline con `?:` (`GetLens_unk`) llamado tres veces: para que MWCC recargue el miembro en cada
  llamada como el original, cada llamada tiene que partir de una base distinta: `mChara->` en la primera,
  `CharaTask* c1 = mChara; c1->` en la segunda y `((CharaTask*)mChara)->` en la tercera (dos locales copia
  de `mChara` se funden y el CSE junta sus cargas).
- Un local nuevo (`path` arriba, en el `if`, etc.) no cambia la asignacion de registros: el orden de
  declaracion no numera los vregs.

## Pendientes

- Aplicar `docs/enlazar-20261115-qZ.patch` (99,70 + 99,64) cuando el ctor de ControlKnockdown salga; luego
  `pd.sh chr/ControlDamage` (el `#include "math/VecAssign_unk.h"` de qY renumera `@N`) y revisar labelcheck.
- Los tres restos son de asignacion de registros (bool de helper inline frente al modelo, r3/r4 de
  argumentos); probablemente necesitan otra forma del helper inline que no se ha encontrado.
