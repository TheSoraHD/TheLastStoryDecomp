# GraphicsDevice

Rama local `codex/graphicsdevice-decomp` (2026-10-24). La fuente `atn/GraphicsDevice.cpp` se registra con `matching: false`; el split existente ya cubre `0x80070E14–0x8007708C` y sus datos.

## Resultado

- Se reconstruyó una implementación semántica inicial de `fn_80071088`: compara nueve registros de `0x30` bytes y nueve registros de seis palabras en `+0x1B0`. Objdiff: **65,57%**, permanece `NONMATCHING`.
- No se cambiaron `splits.txt`, `symbols.txt`, flags ni excepciones de la regla 2.
- Build limpio: `main.dol: OK`. `regress.py`: `lost: 0`; `fuzzdrop.py`: 0 bajadas; `poolcheck.py atn/GraphicsDevice`: 0 hallazgos.
- Las otras funciones de la unidad siguen pendientes; no se atribuye progreso de match a la unidad.

## Decisiones a revisar

- La disposición comparada por `fn_80071088` está inferida por los offsets del ensamblador; confirmar el nombre/tipo real de esos registros.
- El nombre de fichero `GraphicsDevice` está apoyado por la cadena `atn_graphics_device.cpp`; no hay prueba suficiente aquí de que exista una clase C++ `atn::GraphicsDevice`.
- Las diferencias de bucle y asignación de registros hacen que la implementación actual no sea equivalente a nivel de instrucciones. Mantener `NONMATCHING` y continuar con las funciones siguientes en otro lote.
