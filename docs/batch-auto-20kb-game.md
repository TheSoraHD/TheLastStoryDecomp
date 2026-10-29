# Lote `batch-auto-20kb-game` (propuesta E, primera mitad)

Unidades no `atn/` ni de enemigos, sin fuente, de 10–20 KB, en orden de dirección (`targets.py --max-unit 20480 --limit 1000`, > 10240). Trabajo secuencial. Rama creada desde `close-near-units` (propuesta K, sin mergear) para no perder sus dos unidades enlazadas.

## Resultado por unidad

### `Event/EventCommandDef_unk` (0x800081C0, 3 funciones)

- Fuente nuevo generado por script (`tools/decomp/eventcmd/`: `ecd_sim.py` simula el `__sinit` y saca las escrituras en tiempo de ejecución, `ecd_an.py` clasifica los objetos de datos, `ecd_gen.py` escribe el `.cpp`). 92 tablas `ParamDef_unk`, 38 `ParamEnum_unk`, 8 tablas de variantes y la tabla de 49 `CommandDef_unk` (0x80775208).
- `.rodata`, `.data`, `.sdata2` y sus reubicaciones son idénticos al original (comparados byte a byte y por destino de cada reubicación).
- `fn_80008210` 100%; `fn_800081C0` 94% (orden de `li r5, 0`); `__sinit` 99,96% (la última escritura va antes del `addi r11` del epílogo). Probado también con otros `-ipa`: igual.
- **Decisiones sin el usuario:**
  - `symbols.txt`: `fn_800082AC` → `__sinit_\EventCommandDef_unk_cpp` (`scope:local`).
  - `config.yml` (avisado en la sesión): `block_relocations` en `.data:0x807AC0D8`, un 0x8000A000 de la tabla `lbl_807ABF68` que dtk tomaba por un puntero al interior del `__sinit` y lo hacía global con sufijo.
  - `include/Event/Node.h`: `ParamValue_unk()` vacío para los atributos sin valor por defecto (282 de 811 no tienen escritura en el `__sinit`).
- Orden del fuente: cada tabla de enumerados va justo antes de la primera tabla de atributos cuyas cadenas nuevas aparecen después de ella en `.rodata` (las cadenas compartidas con un enumerado no cuentan).
