# AI/AI_archer_attack01

- Unidad: 517 funciones; 177 al 100%. `matching: false`.
- Se decompilaron stubs de AI, selección aleatoria, umbrales, setters de parámetros y accesores repetidos. Los cuerpos parciales llevan su porcentaje en `NONMATCHING`.
- El split aprobado en `splits-auto-A` se conserva (`0x8001BF44–0x8003E918`, con sus pools y datos); sin nuevos límites.
- `fn_8001C038` coincide al 100%; el condicional asigna explícitamente null o la cabeza del manager antes de comprobar el flag. Sin asm, pragma ni excepción nueva.
- `fn_8001BF64` (97,61%), stubs de umbral (93,89%), stubs aleatorios (91,85%), `fn_80026A40` (89,70%), guardas de float (88,97%), `fn_8002B434` (99,89%) y otros conservan `NONMATCHING`; ninguna se cuenta como match.
- `fn_8001E168` coincide al 100%; libera el buffer de cadena larga y, si se solicita, el objeto.
- `fn_8001E0B8` coincide al 100%; copia la cadena corta en línea o reserva y copia la larga, y conserva la palabra del allocator en el objeto extendido. El layout de 16 bytes se infiere del código original; sin asm ni pragma.
- Validación: build limpio (`main.dol: OK`), `regress.py` lost: 0 frente al reporte previo, `fuzzdrop.py` 0 bajadas, `poolcheck.py` 0 hallazgos.
- Decisiones a revisar: confirmar el layout inferido de la cadena extendida y su palabra de allocator, además de los supuestos previos de rango aleatorio y umbral de float; mantener `matching: false` hasta completar las 517 funciones.
