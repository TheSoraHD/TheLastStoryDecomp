# AI/AI_archer_attack01

- Unidad: 517 funciones; 17 al 100%, 1 en 78,33%, resto sin fuente. `matching: false`.
- Coinciden helpers de ruta, flags/accesores, saltos de inicialización y seis retornos virtuales constantes. Sin renombrados en `symbols.txt`.
- El split aprobado en `splits-auto-A` se conserva (`0x8001BF44–0x8003E918`, con sus pools y datos); sin nuevos límites.
- Decisiones para revisar: mantener juntas las ~65 clases `AI::Script`; resolver el flujo de control de `fn_8001C038` antes de contarla como hecha. No se usó asm ni pragma.
- Validación: build limpio, `main.dol: OK`; `regress.py` lost: 0, `fuzzdrop.py` 0 bajadas, `poolcheck.py` 0 hallazgos para esta unidad.
