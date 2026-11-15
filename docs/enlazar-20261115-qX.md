# enlazar-20261115, cola X

Ultima pasada sobre las dos unidades a 1-2 funciones con mas datos (20:35 - 21:30).

## Resultado por unidad

- `msc/src/RVL_SDK/dwc/dwc_match`: 99/101 -> **101/101**; **enlaza** (build limpio `main.dol: OK`, poolcheck y
  poolorder limpios, mnemcheck limpio, precommit OK: 15624 -> 15626, lost 0, sin bajadas).
  - `DWCi_ProcessMatchCommand` (0x806BCD40) 99,995 -> 100 %: `u32 ip` declarada en el ambito de la funcion justo
    tras `u16 port` (antes `SOInAddr ip` en el bloque de TELL_ADDR); `remoteaddr.sin_addr.addr = ip`. Las pruebas
    anteriores tenian por separado `u32 ip` (en el bloque) e `ip` en el ambito de la funcion (como `SOInAddr`);
    solo la combinacion casa. `u32 ip` antes de `port` cruza ip/port (r26/r28).
  - `DWCi_CloseUnacknowledgedClients` (0x806C4430) 99,13 -> 100 %: `pending = DWCi_GetServerNode_unk();`, accesor
    `static inline` nuevo que devuelve `&DWCi_GetMatchCnt()->serverNode` (nombre inventado, `inferred:`). Con el
    accesor el `i` del segundo bucle pasa a r31 y `validPidLen` a r29, como el original. Un accesor que hace toda
    la busqueda (nodo o pendiente) arregla los no volatiles pero cruza dos volatiles (99,77 %); envolver solo
    `DWCi_GetNodeByProfileID` o `validPid[i]` no cambia nada.
- `game/SystemErrorMessage_unk`: 10/11 sin cambio. `ErrorManager::fn_803646C4` 99,86 % (ver NONMATCHING).

## Unidades que enlazan (bytes)

- `msc/src/RVL_SDK/dwc/dwc_match`: 13560 (`measures.total_data` del report).

## NONMATCHING con motivo

- SystemErrorMessage_unk `fn_803646C4` 99,86 %: contador `i` (r27 en el original) y su desplazamiento reducido
  `i * 4` (r29) cruzados, `y` en r28 en los dos. Probado ademas (unas 30 variantes, sin mejora; las de 6 lineas
  solo mueven el orden de los `li`): cuerpo de `DrawString_unk` escrito en el bucle (64 lineas), `int i` con
  comparacion sin signo (`4u`, `sizeof`), accesores inline de la linea / la fila / la tabla del idioma / `mError`
  / la fuente / `lbl_8087EEE0`, contador e indice separados (`line`), `y += 30` en el `for`, `y` inicializada
  antes de la cabecera o en la declaracion, `y` unsigned/short, el bucle en su propia inline (con `y` como
  parametro), el bucle escrito en la funcion con `i`/`y` en el ambito de la funcion (31), puntero a la linea o
  fila + indice como argumento de `DrawString_unk`, declaraciones del primer bucle (`int i`, fuera del `for`).

## Cambios de splits

Ninguno.

## Para el port

Nada de las unidades del port. `dwc_match.c` (libreria): `ip` de TELL_ADDR pasa de `SOInAddr` en el bloque a
`u32` en el ambito de `DWCi_ProcessMatchCommand`; accesor nuevo `DWCi_GetServerNode_unk()`.

## Decisiones tomadas sin el usuario

- Accesor inline inventado `DWCi_GetServerNode_unk` en `dwc_match.c` para `DWCi_CloseUnacknowledgedClients`
  (no esta en ninguna version publica de DWC que tengamos; en `libs/` solo existe esta copia).
- `docs/mwcc-blockers.md`: la seccion de `dwc_match` marcada como resuelta.

## Hallazgos (una linea cada uno)

- Una variable leida de memoria que hereda el registro del puntero base muerto se arregla a veces moviendola al ambito de la funcion con un tipo escalar (`u32` en vez de un struct de un campo) (dwc_match).
- Sustituir `&global->miembro` por un accesor `static inline` cambia el coloreado de registros no volatiles de toda la funcion (dwc_match `DWCi_CloseUnacknowledgedClients`).

## Pendientes

- SystemErrorMessage_unk `fn_803646C4` (8,6 KB de datos) sigue en 99,86 %; ideas no probadas: ninguna clara.
