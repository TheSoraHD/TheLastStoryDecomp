# enlazar-20261115, cola F (red y SDK con pocas funciones)

Agente F, 2026-11-15 14:19 - ~16:30. Referencia: `/home/claude/ref-F` (15552 / 18109).
Resultado: **15552 -> 15555 / 18109**, lost 0, sin bajadas de fuzzy, `main.dol: OK`. Ninguna unidad pasa a enlazar.

## Resultado por unidad

| Unidad | Antes | Despues | Datos sin enlazar |
|---|---|---|---|
| `msc/src/RVL_SDK/dwc/dwc_match` | 97/101 | **98/101** (`DWCi_MatchProcess`) | 13560 |
| `smg2/src/RVL_SDK/wud/WUD` | 77/85 | **79/85** (`WUDStoredLinkKeyCallback`, `DeleteAllHandler`) | 9536 |
| `dwc/src/base/dwc_transport` | 23/24 | 23/24 | 1256 |
| `dwc/src/base/dwc_account` | 40/41 | 40/41 | 304 |
| `dwc/src/base/dwc_lanmatch` | 20/23 | 20/23 | 200 |
| `dwc/src/ranking/dwc_ranking` | 8/9 | 8/9 | 64 |

Commits: `966569a` (DWCi_MatchProcess), `44f0e42` (WUD).

## Unidades que enlazan (bytes)

Ninguna. Lo que queda en las seis unidades son asignaciones de registros ya investigadas (salvo WUD, que tiene diferencias de codigo).

## NONMATCHING con motivo

- dwc_match `DWCi_ProcessMatchCommand` (99,995 %): `ip` de `DWC_MATCH_COMMAND_TELL_ADDR` en r28 (nosotros r30, el de `data`). Probado ademas: `u32 ip`, declararla antes/despues de `remoteaddr`, cargar `data[1]` antes, `ip` en el ambito de la funcion (una o las dos `SOInAddr ip` de la funcion), primera/ultima declaracion. Sin efecto.
- dwc_match `DWCi_CancelMatchingProcess` (99,65 %): base de `.data` (r31) y `hasConnections` (r30) cruzados. Probado: orden `result`/`hasConnections`.
- dwc_match `DWCi_CloseUnacknowledgedClients` (99,13 %): el `i` del segundo bucle en r31 (nosotros r25) y los contadores un registro abajo. Probado: `i` declarado primero, `j`/`k` separados para el segundo y el tercer bucle, `validPid[validPidLen++]`/`senddata[senddataLen++]` en el primer bucle (peor).
- WUD `SyncHandler` (81,7 -> 83,1 %): arbol de comparaciones ya igual; queda el `WUDiDelaySearch` inline: el original materializa cada `return` con ramas (`li r0, 14; b`), nosotros `li r4, 29` adelantado. Probado: if/else, ternario, condicion invertida, tipo de retorno `int`/`s8`/enum. Tambien Wii/1.1 y Wii/1.3 (peor en otras funciones).
- WUD `WUDiDeleteDevice` (72,9 %): el original recorre `stdDevs`/`smpDevs` con desplazamiento `i * 0x60` y `add r3, p, off` en cada vuelta. Probado: indexado `p->stdDevs[i].status`, `&_wcb.stdDevs[i]` (peor).
- WUD `fn_80670C70` (71 %): con `case 6:` (solo o con `default`) sigue siendo cadena lineal.
- WUD `WUDiGetRegisteredDevice` 96,7 -> 98,5 % (comentario actualizado), `WUDiVirginStandardSync` 72 %, `remove_patch_cb` 98,2 % (permutacion r5/r6/r7 en el `WUDiWritePatch` inline): sin cambios.
- dwc_account `DWC_ReportFriendData`: MWCC compara los casos en el orden del fuente (probados los seis ordenes de 3 casos y `switch (DWC_GetFriendDataType(...))`); el original compara 2,3,1 y coloca los cuerpos 3,2,1.
- dwc_transport, dwc_ranking, dwc_lanmatch: ya investigadas (`docs/nw4r-20261105.md`, comentarios NONMATCHING); no he repetido.

## Cambios de splits

Ninguno.

## Para el port

Nada de las unidades del port. Fuera de ellas: `qr2_implementation_s` de `libs/msc/src/RVL_SDK/gamespy/qr2/qr2.h` gana `cc_callback` y `hr_callback` tras `pa_callback` (como `libs/gamespy/include/qr2/qr2.h`), `userstatechangerequested` pasa de 0xB4 a 0xBC; solo lo incluye `dwc_match`.

## Decisiones tomadas sin el usuario

- `qr2.h` de msc con el layout de TLS (en vez de cambiar las rutas de include de `dwc_match`, que es tocar flags).
- `DWCi_MatchProcess`: `s_nnProcessing = 1` tambien antes de `ServerBrowserThink` y `s_needNNFree`/`s_nnProcessing` antes de `DWCi_SendStateChanged` (lo que hace el codigo del original; el nombre `s_nnProcessing` del 0xC puede no ser exacto).
- WUD: `pInfo = NULL` dentro de la rama estandar de `WUDiGetNewDevInfo` (inline compartida; sube 3 funciones, no baja ninguna); casos vacios con `return` en `DeleteAllHandler` (7) y `SyncHandler` (6, 13, 26); ruta de RPHealth en un local en `WUDiOpenWiiFitFile`.

## Hallazgos (una linea cada uno)

- Un `case` vacio con `break` (igual que `default`) desaparece del arbol de comparaciones de MWCC; con `return` se queda: si el arbol del original tiene un caso que salta al final, escribirlo con `return` (WUD DeleteAllHandler, SyncHandler).
- MWCC (Wii/1.0-1.3) compara los casos de un `switch` pequeno (cadena lineal) en el orden del fuente.
- Un `const char* path = "..."` local al principio de una inline adelanta la carga de la cadena por encima del primer `if` (WUD `WUDiOpenWiiFitFile`).
- El GameSpy de TLS tiene `cc_callback` y `hr_callback` en `qr2_implementation_s` (`userstatechangerequested` en 0xBC); el de msc no.
- `precommit.sh` lanza `ninja` en paralelo: con varios agentes falla al azar (`.d` no encontrado); `ninja -j1` antes lo evita.
- `Event/ActionActorBehavior 4 missing 3` en labelcheck aparece en esta copia sin tocar esa unidad (ya estaba tras el merge de la cola A, o la referencia es anterior).

## Pendientes

- dwc_match: tres funciones de registros (arriba); con ellas enlazaria (13,5 KB). Comprobar antes los datos del fuente (regla 10).
- WUD: seis funciones (71-98 %); `fn_80670C70` y `WUDiVirginStandardSync` parecen de otra version del compilador o de flags.
