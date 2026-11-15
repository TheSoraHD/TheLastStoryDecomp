# enlazar-20261115, cola L (segunda pasada)

## Resultado por unidad

- `Event/EventCommandDef_unk`: 2/3 -> 3/3, **enlaza** (31220 bytes de datos; build limpio `main.dol: OK`,
  poolcheck/poolorder limpios). `fn_800081C0` 94 -> 100 %: no era el C++ de la funcion sino que era la
  primera del fichero. MWCC planifica distinto la primera funcion de una unidad: tras cualquier funcion que
  opere con sus argumentos (`return a + 1`, `-a`, `a * b`, `&tabla[i]`; no basta una vacia, una que lee un
  global ni una inline/estatica sin emitir) el `li r5, 0` va detras del `addi` como en el original. En el
  original habia una funcion delante que el enlazador quito; repuesta como `fn_Stripped_unk(int i)`
  (`return &sCommands_unk[i];`, cuerpo inventado). Su parametro con nombre renumera las `@N`: `pd.sh`.

## Unidades que enlazan (bytes)

- `Event/EventCommandDef_unk`: 31220 (total_data del report).

## NONMATCHING con motivo

## Cambios de splits

Ninguno. `symbols.txt`: solo renombrados (`@N` re-emparejadas, `sCommands_unk__5Event`, `sCommandCount_unk__5Event`).

## Para el port

## Decisiones tomadas sin el usuario

- `fn_Stripped_unk` en EventCommandDef_unk: funcion inventada (no esta en el DOL) para reproducir la
  planificacion de la primera funcion, como los `Stripped*_unk` de otras unidades.

## Hallazgos (una linea cada uno)

- La primera funcion de un fichero se planifica distinto (orden de `li`/`addi` independientes): si el original
  difiere solo ahi, habia una funcion quitada por el enlazador delante (EventCommandDef_unk fn_800081C0).

## Pendientes
