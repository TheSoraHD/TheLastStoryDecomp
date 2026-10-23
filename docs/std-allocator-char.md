# std/allocator_char

Rama local `codex/std-allocator-char` (2026-10-23), desde `main`. Se trabajó la unidad permitida; no se tocaron splits ni `symbols.txt`, y no se añadieron excepciones a la regla 2.

- El archivo contiene el parser `TextParser_unk` y funciones MSL de iostream/stringbuf. Fuente: 13 de 35 funciones representadas; 11 al 100%, dos NONMATCHING (99.69% y 93.10%) y 22 aún sin implementar.
- Se añadieron siete entradas pequeñas de vtable en C++ normal; todas al 100%. El total del proyecto pasó de 7301 a 7308 según el estado anterior de `CLAUDE.md`.
- El intento de quitar un temporal de `fn_800E0F84` empeoró el resultado y se revirtió. No se cambió su estructura. `fn_800E114C` sigue diferiendo por asignación de registros de sus variables locales.
- Build recompilado (709 pasos), `main.dol: OK`. `regress.py`: lost 0; `fuzzdrop.py`: 0 bajadas; `poolcheck.py std/allocator_char`: sin hallazgos. La referencia disponible (`report.before-clean.json`) tenía 7300 matches, uno menos que el estado documentado previo de 7301.

## Decisiones a revisar

1. Los siete símbolos añadidos conservan nombres automáticos y linkage C hasta conocer la clase/slot MSL exactos. Renombrarlos requerirá identificar sus vtables y revisar referencias.
2. El lote deja el resto del código iostream/stringbuf pendiente; no se amplió el split para mover funciones a otra unidad.
