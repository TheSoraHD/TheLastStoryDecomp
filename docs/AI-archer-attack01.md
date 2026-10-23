# AI/AI_archer_attack01

- Unidad: 517 funciones; 92 al 100%, `fn_8001BF64` al 97,61%, 424 sin fuente. `matching: false`.
- Coinciden helpers de ruta, flags/accesores, saltos de inicialización, seis retornos virtuales constantes, nueve retornos `-1` y 64 constructores que instalan la vtable. Sus nombres de constructor proceden de las vtables y sus llamadas desde fábricas; las declaraciones solo modelan esos constructores, no los layouts.
- El split aprobado en `splits-auto-A` se conserva (`0x8001BF44–0x8003E918`, con sus pools y datos); sin nuevos límites.
- `fn_8001C038` coincide al 100%; el condicional asigna explícitamente null o la cabeza del manager antes de comprobar el flag. Sin asm, pragma ni excepción nueva.
- `fn_8001BF64` cuenta efectos activos aplicables al personaje; queda parcial solo por los registros del puntero e índice del bucle.
- `fn_8001E168` coincide al 100%; libera el buffer de cadena larga y, si se solicita, el objeto.
- Validación del último cambio: build incremental, `regress.py` lost: 0, `fuzzdrop.py` 0 bajadas, `poolcheck.py` 0 hallazgos. El cambio anterior terminó con build limpio y `main.dol: OK`.
