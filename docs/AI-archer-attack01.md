# AI/AI_archer_attack01

- Unidad: 517 funciones; 90 al 100%, `fn_8001C038` en 78,33%, 426 sin fuente. `matching: false`.
- Coinciden helpers de ruta, flags/accesores, saltos de inicialización, seis retornos virtuales constantes, nueve retornos `-1` y 64 constructores que instalan la vtable. Sus nombres de constructor proceden de las vtables y sus llamadas desde fábricas; las declaraciones solo modelan esos constructores, no los layouts.
- El split aprobado en `splits-auto-A` se conserva (`0x8001BF44–0x8003E918`, con sus pools y datos); sin nuevos límites.
- Decisión para revisar: resolver el flujo de control de `fn_8001C038` antes de contarla como hecha. No se usó asm, pragma ni excepción nueva.
- Validación: build limpio, `main.dol: OK`; `regress.py` lost: 0, `fuzzdrop.py` 0 bajadas, `poolcheck.py` 0 hallazgos para esta unidad.
