# std/allocator_char

Rama local `codex/std-allocator-char` (2026-10-23), desde `main`. Se trabajó la unidad permitida; no se tocaron splits ni `symbols.txt`, y no se añadieron excepciones a la regla 2.

- El archivo contiene el parser `TextParser_unk` y funciones MSL de iostream/stringbuf. Fuente: 15 de 35 funciones representadas; 11 al 100%, cuatro NONMATCHING (99.69%, 93.10%, 99.35% y 90%) y 20 aún sin implementar.
- Se añadieron siete entradas pequeñas de vtable en C++ normal; todas al 100%. El total del proyecto pasó de 7301 a 7308 según el estado anterior de `CLAUDE.md`.
- El intento de quitar un temporal de `fn_800E0F84` empeoró el resultado y se revirtió. No se cambió su estructura. `fn_800E114C` sigue diferiendo por asignación de registros de sus variables locales.
- En la continuación se reconstruyeron parcialmente `fn_800E1D88` (buffer bajo/alto; 99.35%, solo difiere un registro en el reload de `mBufferEnd`) y `fn_800E1D5C` (copia/incremento de referencia de un par shared_ptr; 90%, difiere asignación de registros). Se probaron formas directas de expresar los valores; no se usaron asm, pragmas ni excepciones nuevas. La unidad no puede marcarse como `matching` mientras queden estas diferencias, las otras funciones y datos sin reconstruir.
- Build limpio (712 pasos), `main.dol: OK`; 7308 funciones al 100%, 370 unidades enlazadas. `regress.py`: lost 0; `fuzzdrop.py`: 0 bajadas; `poolcheck.py std/allocator_char`: sin hallazgos. La referencia disponible (`report.before-clean.json`) tenía 7300 matches, uno menos que el estado documentado previo de 7301.

## Decisiones a revisar

1. Los siete símbolos añadidos conservan nombres automáticos y linkage C hasta conocer la clase/slot MSL exactos. Renombrarlos requerirá identificar sus vtables y revisar referencias.
2. El lote deja el resto del código iostream/stringbuf pendiente; no se amplió el split para mover funciones a otra unidad. Revisar si compensa investigar otra vez `fn_800E1D88`/`fn_800E1D5C` o separar el parser de las plantillas MSL mediante splits.
