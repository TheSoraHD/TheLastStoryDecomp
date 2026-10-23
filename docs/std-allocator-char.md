# std/allocator_char

Rama local `codex/std-allocator-char` (2026-10-23). Trabajo en MSL iostream/stringbuf, sin asm, pragmas ni nuevas excepciones a la regla 2. Se mantuvieron los splits; se renombraron símbolos identificados con autorización.

- `StringBuf_unk` usa el layout observado en los accesos del original; quedaron reconstruidos `underflow`, `pbackfail`, `overflow`, `xsgetn`, `sbumpc`, `xsputn`, `seekoff` y `seekpos`. El `vtable` se modela con virtuales C++ normales.
- Matches exactos nuevos incluyen `sbumpc`, ambos constructores de posición y el overload fill de `basic_string::replace`. Renombrados MSL: `underflow`, `pbackfail`, `overflow`, `basic_string::replace` y `__sinit_\\allocator_char_cpp`.
- Estado actual: 16/35 funciones al 100%; `underflow` 99,35%, `pbackfail` 77,66%, `overflow` 73,95%, `seekoff` 69,21%, `seekpos` 86,21%, `xsgetn` 96,51%, `xsputn` 99,47%, y `basic_string::replace` 97,68%. 19 funciones siguen sin match o sin implementar. `game_units.json` sigue con `matching: false`; no es enlazable.
- Build limpio: `main.dol: OK`; 7313 funciones al 100%, 370 ficheros enlazados. `regress.py`: lost 0; `fuzzdrop.py`: 0 bajadas; `poolcheck.py std/allocator_char`: sin hallazgos.

## Decisiones a revisar

1. Los métodos de buffer y seek siguen siendo reconstrucciones parciales; sus diferencias principales son selección de registros/temporales y aún no justifican marcar la unidad como matching.
2. Se conservó `StringBuf_unk` y el overload `replace(pos,n1,n2,char)` con nombres descriptivos inferidos; revisar si se identifica la clase/template MSL original antes de renombrar más.
3. No se ampliaron ni cortaron splits. La unidad permanece no enlazable hasta obtener 100% en todas sus funciones y validar el enlace.
