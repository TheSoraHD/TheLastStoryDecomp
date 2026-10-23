# AI/AI_archer_attack01

- Unidad: 517 funciones; 185 al 100%. `matching: false`.
- Se decompilaron stubs de AI, selección aleatoria, umbrales, setters de parámetros y accesores repetidos. Los cuerpos parciales llevan su porcentaje en `NONMATCHING`.
- El split aprobado en `splits-auto-A` se conserva (`0x8001BF44–0x8003E918`, con sus pools y datos); sin nuevos límites.
- `fn_8001C038` coincide al 100%; el condicional asigna explícitamente null o la cabeza del manager antes de comprobar el flag. Sin asm, pragma ni excepción nueva.
- `fn_8001BF64` coincide al 100%; el índice unsigned y el puntero de efecto reutilizado conservan la comparación y los registros del bucle.
- Stubs de umbral (93,89%), stubs aleatorios (91,85%), `fn_80026A40` (89,70%), guardas de float (88,97%) y otros conservan `NONMATCHING`; ninguna se cuenta como match.
- `fn_8002B434` coincide al 100%; las cuatro salidas nulas se modelan como un agregado contiguo antes de llamar al helper.
- `fn_8003068C`, `fn_80033588` y `fn_80035274` coinciden al 100%; son stubs breves de configuración del estado y parámetros de ataque.
- `fn_8002B818` y `fn_80035C20` son el mismo stub de resultados y coinciden al 100%; el agregado local tiene cuatro palabras consecutivas, solo la primera inicializada desde `m80`.
- `fn_8003E688` coincide al 100%; inicializa el centinela de una lista global y lo registra con su destructor.
- `fn_80032FA8` y `fn_80035FD0` coinciden al 100%; son stubs de condición con salidas `0xF` y `0x5A`.
- `fn_8003CBE0` (89,40%) convierte el contador por un factor flotante; MWCC genera una secuencia de conversión y una constante distintas. `fn_8002B818` y `fn_80035C20` permanecen parciales al 88,31% por diferencias de asignación de registros; `fn_8002EB20` (89,57%) y `fn_80036AA8` (70,77%) también quedan parciales por el orden de salidas locales y la asignación de registros. Todas llevan `NONMATCHING` y no se cuentan como completadas.
- `fn_8001E168` coincide al 100%; libera el buffer de cadena larga y, si se solicita, el objeto.
- `fn_8001E0B8` coincide al 100%; copia la cadena corta en línea o reserva y copia la larga, y conserva la palabra del allocator en el objeto extendido. El layout de 16 bytes se infiere del código original; sin asm ni pragma.
- Validación: build limpio (`main.dol: OK`), `regress.py` lost: 0, `fuzzdrop.py` 0 bajadas, `poolcheck.py` 0 hallazgos.
- Decisiones a revisar: confirmar el layout inferido de la cadena extendida y su palabra de allocator, los agregados locales de salidas, el rol destructor/DSO del centinela global y los supuestos previos de rango aleatorio y umbral de float; valorar otra ronda para las conversiones y agregados parciales. Mantener `matching: false` hasta completar las 517 funciones.
