# enlazar-20261115, cola T

Agente T, 2026-11-15 desde 18:59. Unidad: chr/ControlCdnStone (y, por la cabecera, chr/ControlStickByQuark).
Referencia: `/home/claude/ref-T/report.json` (15590 funciones al 100 %).

## Resultado por unidad

| Unidad | Antes | Despues | Notas |
|---|---|---|---|
| chr/ControlCdnStone | 57/57 | 57/57 | **enlaza** (`matching: true`, `main.dol: OK` tras build limpio) |
| chr/ControlStickByQuark | 134/159 | 136/159 (objdiff) | solo el cambio de tipo de la constante de `ControlThrownBreakSkelton::vf10` y etiquetas re-emparejadas |
| chr/ControlBase | 3/3 | 3/3 | enlazada; re-emparejada (sus referencias a `invoke`/`manage` cambian de nombre) |

`report.json`: 15590 -> 15590 (lost 0, sin bajadas de fuzzy).

## Unidades que enlazan (bytes)

- chr/ControlCdnStone: 3304 B de datos (`.rodata` 0x5D0, `.data` 0x690, `.bss` 0x38, `.sbss` 8, `.sdata2` 0x48).

## NONMATCHING con motivo

- Ninguna nueva. No hubo tiempo para las vf0C de ControlChaseCaught (ver Pendientes).

## Cambios de splits

- Ninguno. `symbols.txt`: solo renombrados.
  - Intercambiados los nombres de las dos copias debiles de `ControlCdnBetray::vf10` (invoke 0x801905AC/0x801906F8,
    manage 0x801905DC/0x80190728, `__RTTI__` 0x8077D688/0x8077D690, `@STRING@allocate` 0x8077DBF4/0x8077DC10,
    `@LOCAL@assign_to` 0x807C7B78/0x807C7B80, `@GUARD@assign_to` 0x8087F0BB/0x8087F0BC): la primera copia es la
    del bind con `value<int>` (su `__RTTI__` apunta a la cadena "...value<int>>>" en 0x80739260), no la de
    `value<CharaTask::CONTROL_MODE>`. Prueba: con los nombres anteriores, los `manage` del `.dol` apuntaban a la
    RTTI cuyo nombre es el del otro tipo.
  - ControlStickByQuark: `@28995`, `@28995` (duplicado), `@28997` en `.data` 0x8077DD50/5C/68 -> `@28968`/`@28969`/
    `@28970` (los de nuestro .o; `pd.sh` no los arreglaba por el nombre duplicado).
  - Re-emparejado (`pd.sh`) de ControlCdnStone, ControlBase y ControlStickByQuark.

## Para el port

- `CharaTask::m564`: `CONTROL_MODE` -> `int` (mismo tamano y offset 0x564). El enum `CharaTask::CONTROL_MODE` se
  queda, solo para la constante 2 de `ControlCdnBetray::vf10`. Afecta a quien lea/escriba m564 (asignaciones con
  `(CharaTask::CONTROL_MODE)1/2` siguen compilando: enum -> int).
- `ControlThrownBreakSkelton::vf10` (chr/ControlStickByQuark.cpp): bind de `fn_8015EFAC` con `1` (int) en vez de
  `(CharaTask::CONTROL_MODE)1`.

## Decisiones tomadas sin el usuario

- Aplicado el parche de P (`ControlCdnPiyo::Stripped1_unk`, funcion inventada que el enlazador quita, para el orden
  de `.sdata2`) y commiteado con `matching: true`.
- `CharaTask::m564` pasa a `int` y la constante 2 de `ControlCdnBetray::vf10` a `(CharaTask::CONTROL_MODE)2`; la
  constante 1 de `ControlThrownBreakSkelton::vf10` a `int` (fuera de la cola, una linea; necesario porque comparte
  copia debil con el bind de m564).

## Hallazgos (una linea cada uno)

- Copias debiles de plantillas con codigo identico (invoke/manage de dos `bind_t` que solo difieren en el tipo de
  un argumento int/enum): el nombre en `symbols.txt` puede estar cruzado; la verdad la da la cadena RTTI a la que
  apunta el `__RTTI__` que usa cada `manage` (CdnStone: el "orden raro" de cadenas era un tipo cruzado).
- `value<int>` frente a `value<Enum>` en un `boost::bind`: decide el tipo el argumento (`m564` int, `2` del enum);
  las cadenas RTTI de `.rodata` y los `__RTTI__` de `.data` salen en orden inverso a la instanciacion de la clase
  `bind_t` (un `sizeof(bind_t<...>)` anterior lo cambia); invoke/manage en `.text` en orden inverso de uso en la
  funcion.

## Pendientes

- ControlChaseCaught (ChaseCaught/Follow/Catch::vf0C): sin tocar (ver notas de P).
