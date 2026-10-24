# GraphicsDevice

Rama local `codex/graphicsdevice-decomp` (2026-10-24). La fuente `atn/GraphicsDevice.cpp` se registra con `matching: false`; el split existente ya cubre `0x80070E14–0x8007708C` y sus datos.

## Resultado

- Nueve funciones al 100% en objdiff: `fn_80071D60`, `fn_80071D68`, `fn_80071D74`, `fn_80071D80`, `fn_800760BC`, `fn_800760D0`, `fn_80076174`, `fn_800763FC` y `fn_80076F88`.
- `fn_80071088` compara nueve registros de `0x30` bytes y nueve registros de seis palabras en `+0x1B0`; queda al **65,57%**, `NONMATCHING`.
- `fn_800763C0` administra el binding del GX texture cache; queda al **72,00%**, `NONMATCHING` por la normalización del puntero nulo en los registros.
- No se cambiaron `splits.txt`, `symbols.txt`, flags ni excepciones de la regla 2.
- Build limpio: `main.dol: OK`. `regress.py`: `lost: 0`; `fuzzdrop.py`: 0 bajadas; `poolcheck.py atn/GraphicsDevice`: 0 hallazgos.
- La mayoría de las otras funciones de la unidad siguen pendientes; no se atribuye progreso de match a la unidad.

## Decisiones a revisar

- La disposición comparada por `fn_80071088` está inferida por los offsets del ensamblador; confirmar el nombre/tipo real de esos registros.
- El nombre de fichero `GraphicsDevice` está apoyado por la cadena `atn_graphics_device.cpp`; no hay prueba suficiente aquí de que exista una clase C++ `atn::GraphicsDevice`.
- Las diferencias de bucle y asignación de registros hacen que la implementación actual no sea equivalente a nivel de instrucciones. Mantener `NONMATCHING` y continuar con las funciones siguientes en otro lote.
