# enlazar-20261115, cola J (datos y medicion)

## Resultado por unidad

| Unidad | Antes | Despues | Notas |
|---|---|---|---|
| `key/src/revolution/WPAD/WPADHIDParser.c` | 33/33, no enlaza | 33/33, **enlaza** | el objeto de 0x10 en 0x8082DDF0 no existe: es relleno por la alineacion 32 del `.bss` de WUD |

## Unidades que enlazan (bytes)

- `WPADHIDParser`: +544 (`complete_data` 1005158 -> 1005702).

## NONMATCHING con motivo

## Cambios de splits

- `smg2/src/RVL_SDK/wud/WUD.c` `.bss` con `align:32`. Prueba: WUD.c tiene `__WUDHandlerStack[0x1000] ATTRIBUTE_ALIGN(32)` (nuestro WUD.o compilado da `.bss` con alineacion 32) y `_wcb` esta en 0x8082DE00; `gap_08_8082DDF0_bss` no tiene referencias. WUD no enlaza, asi que se usa su objeto generado por dtk, que llevaba alineacion 8: con WPADHIDParser desde fuente (`.bss` 0x50) `_wcb` caia en 0x8082DDF0. Con `align:32` build limpio `main.dol: OK`.

## Para el port

- Sin cambios.

## Decisiones tomadas sin el usuario

- `align:32` en el `.bss` de WUD (ver arriba) en lugar de anadir un array falso de 0x10 a WPADHIDParser.

## Hallazgos (una linea cada uno)

- La nota de qD ("el enlazador no alinea por encima de 16 en `.bss`") era falsa: se enlazaba el objeto generado de WUD (alineacion 8), no el compilado.

## Pendientes
