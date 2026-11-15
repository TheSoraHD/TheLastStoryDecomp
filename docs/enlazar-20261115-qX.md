# enlazar-20261115, cola X

Ultima pasada sobre las dos unidades a 1-2 funciones con mas datos (20:35 - 21:45). Las dos enlazan.

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
- `game/SystemErrorMessage_unk`: 10/11 -> **11/11**; **enlaza** (build limpio `main.dol: OK`, poolcheck y poolorder
  limpios, precommit OK: 15626 -> 15627, lost 0, sin bajadas).
  - `ErrorManager::fn_803646C4` (0x803646C4) 99,86 -> 100 %. El bucle va en la funcion, no en una inline: `gfx`
    sale de una inline `BeginFontDraw_unk()` (fondo negro + `fn_8007192C`/`fn_80075DEC`, devuelve `gfx`),
    `for (unsigned int i = 0; i < 4; i++) DrawLine_unk(this, sMessages[...][mError][i], i * 30 + 50);` (la `y` sale
    de `i`, como el `1.1f * size * i + 80.0f` del primer bucle) y `DrawLine_unk` es una inline que solo llama a
    `DrawString_unk` (con `DrawString_unk` a un nivel de inline menos, sus locales `p`/`x` y los huecos de pila
    de `OSGetFontTexture` salen al reves).
  - Para enlazar: `ErrorManager* lbl_8087F420;` definido en el fuente (su `.sbss`; antes solo `extern` de
    `ErrorManager.h`: el enlace fallaba con `undefined`), y `DrawString_unk` lee la fuente con `&*m->mFont` en vez
    de `m->mFont.get()`: la copia weak de `scoped_ptr<Uc>::get` (la referencia el ptmf `@7345` del
    `operator unspecified_bool_type` de MWCC) salia detras de `fn_803646C4` y el original la tiene detras de
    `fn_80364BE8` (0x80364DAC); el `.dol` daba 374 bytes distintos.

## Unidades que enlazan (bytes)

- `msc/src/RVL_SDK/dwc/dwc_match`: 13560 (`measures.total_data` del report).
- `game/SystemErrorMessage_unk`: 8624.
- Total: 22184 bytes.

## NONMATCHING con motivo

Ninguno en la cola. Lo probado sin exito en `fn_803646C4` antes de dar con la forma (unas 60 variantes), por si
sirve en otra parte: accesores inline (linea, fila, tabla del idioma, `mError`, fuente, `lbl_8087EEE0`), contador e
indice separados, ordenes de declaracion (en una inline: la declarada despues va mas alta y el desplazamiento
reducido siempre el mas bajo), bucle en su propia inline, puntero a la linea, `y` unsigned/short.

## Cambios de splits

Ninguno.

## Para el port

Nada de las unidades del port. `SystemErrorMessage_unk` (no es del port): `DrawFontMessage_unk` sustituida por
`BeginFontDraw_unk` + bucle en `fn_803646C4` + `DrawLine_unk`; `lbl_8087F420` definido en el `.cpp`.
`dwc_match.c` (libreria): `ip` de TELL_ADDR pasa de `SOInAddr` en el bloque a
`u32` en el ambito de `DWCi_ProcessMatchCommand`; accesor nuevo `DWCi_GetServerNode_unk()`.

## Decisiones tomadas sin el usuario

- Accesor inline inventado `DWCi_GetServerNode_unk` en `dwc_match.c` para `DWCi_CloseUnacknowledgedClients`
  (no esta en ninguna version publica de DWC que tengamos; en `libs/` solo existe esta copia).
- `docs/mwcc-blockers.md`: la seccion de `dwc_match` marcada como resuelta.

## Hallazgos (una linea cada uno)

- Una variable leida de memoria que hereda el registro del puntero base muerto se arregla a veces moviendola al ambito de la funcion con un tipo escalar (`u32` en vez de un struct de un campo) (dwc_match).
- Con `-ipa file`, en las variables de una inline la declarada despues recibe el registro mas alto y el desplazamiento reducido de un indice el mas bajo; en la funcion, al reves (la declarada antes, mas alta) y el desplazamiento por encima: si el original tiene el desplazamiento entre las variables, el bucle esta en la funcion y la variable de encima viene de una inline (SystemErrorMessage_unk).
- Una inline a un nivel de anidamiento distinto invierte el orden de registros y de pila de sus locales: una inline envoltorio que solo llama a otra lo arregla (`DrawLine_unk` -> `DrawString_unk`).
- `boost::scoped_ptr` con MWCC: `if (p)` usa un ptmf a `get` (dato `@N` = {0, -1, get, 0}) y `p.get()` en una funcion hace salir la copia weak de `get` detras de ella; usar `&*p` donde el original no la referencia (SystemErrorMessage_unk).
- Sustituir `&global->miembro` por un accesor `static inline` cambia el coloreado de registros no volatiles de toda la funcion (dwc_match `DWCi_CloseUnacknowledgedClients`).

## Pendientes

Ninguno de la cola.
