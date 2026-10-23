# std/allocator_char

Rama local `codex/std-allocator-char` (2026-10-23). Trabajo en MSL iostream/stringbuf, sin asm, pragmas ni nuevas excepciones a la regla 2. Se mantuvieron los splits; se renombraron símbolos identificados con autorización.

- `StringBuf_unk` usa el layout observado en el original; hay fuente para `underflow`, `pbackfail`, `overflow`, `xsgetn`, `sbumpc`, `xsputn`, `seekoff` y `seekpos`. El `vtable` se modela con virtuales C++.
- Matches exactos incluyen `sbumpc`, destructores `0x800E0C20`/`0x800E0C8C`, ambos constructores de posición y el overload fill de `basic_string::replace`.
- Estado: 18/35 funciones al 100%; 17 siguen parciales o no emitidas. `0x800E0B18` 99,97%, `0x800E0B90` 99,69%, sentry ctor `0x800E14A4` 57,25%, cleanup `0x800E1630` 29,76%, flush `0x800E1A48` 16,52% y thunk `0x800E2A10` 0%. Otros NONMATCHING: `fn_800E0F84` 99,69%, `fn_800E114C` 93,10%, `underflow` 99,35%, `pbackfail` 77,66%, `overflow` 73,95%, `seekoff` 69,21%, `seekpos` 86,21%, `xsgetn` 96,51%, `xsputn` 99,47% y `basic_string::replace` 97,68%.
- `game_units.json` sigue con `matching: false`; la unidad no es enlazable. Build limpio: `main.dol: OK`; 7315 funciones al 100%, 370 ficheros enlazados. `regress.py`: lost 0; `fuzzdrop.py`: 0 bajadas; `poolcheck.py std/allocator_char`: sin hallazgos.

## Decisiones a revisar

1. `sentry` y `flush` usan offsets/layout inferidos; confirmar el flujo de excepciones y el tratamiento del stream tied.
2. La thunk `0x800E2A10` se inlinea en el destructor y no reproduce el tail-call; resolverla sin asm/pragma puede requerir cambiar el split.
3. `0x800E19AC` tiene una especialización de fuente, pero el compilador no la emite en el objeto actual; investigar cómo se materializa el weak MSL constructor sin romper el copy constructor de `wchar_t`.
4. La unidad permanece no enlazable hasta tener 100% en todas sus funciones y datos y validar el enlace.
