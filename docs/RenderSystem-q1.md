# RenderSystem — avance local

Rama `codex/rendersystem-decomp`, 2026-10-24. Se incorporó `atn/RenderSystem_unk.cpp` para el rango ya asignado por el split (`0x800BC468–0x800C16C0`); no se modificaron `splits.txt` ni `symbols.txt`, y `matching` sigue en `false`.

Hay 8/37 funciones al 100%: `fn_800BDB40`, `fn_800BFAC4`, `fn_800C0508`, `fn_800C0A8C`, `fn_800C0A98`, `fn_800C169C`, `fn_800C16B4` y `fn_800C16BC`. Se escribieron además implementaciones parciales para `fn_800BC468` (80,87%), `fn_800C0590` (57,35%), `fn_800C08B0` (73,90%), `fn_800C0A50` (72,67%), `fn_800C1660` (73,33%) y `fn_800BDB58` (61,75%). Las otras 23 funciones siguen pendientes.

Validación del estado de este lote: build limpio, `main.dol: OK`; `regress.py` lost: 0; `fuzzdrop.py` 0 bajadas; `poolcheck.py atn/RenderSystem_unk` 0 hallazgos.

## Decisiones para revisar

- Confirmar el nombre/layout inferido de `RenderSystem_unk` y los campos usados por las funciones de cola de render (`0xF0–0xF8`, `0x50`, `0xA0`, `0x2FC`, `0x52C`, `0x550`, `0x774–0x79C`, `0x890`, `0x910–0x914`).
- `fn_800BC468` construye una textura de gradiente con un bucle tiled; falta igualar el orden de instrucciones del compilador.
- La unidad queda deliberadamente como `matching: false` hasta completar las 23 funciones y llevar las restantes implementaciones parciales al 100% cuando sea posible.

No se usó asm inline ni pragmas. No se subió la rama.
