# SoundManager (avance inicial)

Rama local `codex/soundmanager-decomp`, 2026-10-24. Se registró `atn/SoundManager.cpp` aprovechando el split existente, sin tocar `splits.txt`; la revisión identificó constructor y destructor en `symbols.txt`. De las 76 funciones del split, 27 están al 100%: métodos `SoundRef`, wrappers de `SoundCore`, cuatro saltos directos, dos helpers de estado/liberación y dos adaptadores de callback. Nueve funciones tienen fuente pero siguen `NONMATCHING`; 40 aún no tienen implementación. La unidad permanece `matching: false`.

Build limpio: `main.dol: OK`. `regress.py`: lost 0 (7528 funciones frente a la referencia de 7501); `fuzzdrop.py`: 0 bajadas; `poolcheck.py atn/SoundManager`: 0 hallazgos.

## Revisión del split y símbolos

El split se conserva: sus límites contiguos (`Sound3dEngine` termina en `0x800CB360`, `SoundManager` termina en `0x800D1C60`, y `Task` empieza allí) coinciden con el orden de emisión y no cortan ninguna función. El área de datos incluye la vtable/RTTI de `atn::SoundManager` (`0x80779698/0x807796A8`), el singleton `lbl_8087EFE8`, los flags y el estado estático que usan sus métodos.

Se renombraron dos símbolos con evidencia directa: `fn_800CCBF8` es `__ct__Q23atn12SoundManagerFv` (tamaño asignado 0x46E8 y llamada desde el inicializador del singleton); `fn_800CDD8C` es `__dt__Q23atn12SoundManagerFv` (la vtable apunta a él y destruye los miembros). `fn_800CDD40`/`fn_800CDD4C` se implementaron sin nombres semánticos: el primero pone a cero un byte; el segundo libera el puntero si el indicador es positivo y lo devuelve. Siguen anónimos los métodos no identificados; tres rutinas iniciales de 0x68C son parecidas, pero su semántica no es fiable para nombrarlas.

## Referencias cruzadas

- `fn_800D1430`, llamada por el constructor, arma controles de depuración con las etiquetas `PlayerParam`, `CategoryParam` y `ScopeParam`. Registra `fn_800CB800`, `fn_800CBE8C` y `fn_800CC518` para tablas de 8, 32 y 16 entradas, respectivamente. Se dejan anónimas: el rol está probado, pero el nombre de cada callback requiere entender la notificación Boost/UI.
- `fn_800CDF84` y `fn_800CE368` guardan y restauran esas tablas; sus llamadores incluyen `MP::Manager2`, `GameTask` y `NewTitleTask`. No se les asigna todavía un nombre de método.
- `fn_800D0240` escribe un valor por índice en una tabla de 8 entradas. `SetConfig` lo llama para los índices 0, 1 y 2 con los tres valores de `SoundSettings`, lo que confirma el rol de ajuste de volumen.
- `fn_800D089C` cuenta entradas de voz activas por categoría y puede excluir las que tienen el flag de reproducción activo. `SongManager`, `TutorialTask`, `GameTask` y `Event/Manager` consultan ese conteo. La implementación actual queda `NONMATCHING` al 85,17%.
- `fn_800D0AB0` crea un `SoundObject` usado por BGM, SE y voz; `fn_800D0C1C` comprueba/añade el sonido a la tabla de seguimiento y `fn_800D19FC` ajusta su volumen. `fn_800CF7DC` se usa para pausar/reanudar sonidos desde menú, cámara y gestión de errores.
- No bastan las referencias actuales para dar nombre preciso a `fn_800CF45C`, `fn_800CF680`, `fn_800CFA28` o `fn_800CFBA0`.
- `fn_800D0180` y `fn_800D07E8` son adaptadores de llamada indirecta de los callbacks Boost; se implementaron como llamadas de función ordinarias y ambas coinciden al 100%. Se mantienen anónimos porque son adaptadores reutilizables, no métodos con nombre propio.
- `fn_800D089C` tiene referencias suficientes para confirmar el conteo por categoría/estado, pero se conserva `NONMATCHING` al 85,17%; el siguiente intento debe enfocarse en igualar el rango firmado del índice y el registro de iteración.

## Decisiones a revisar

- La unidad contiene el rango completo `0x800CB360–0x800D1C60`; el constructor/destructor confirman que los métodos del manager están dentro. Los helpers iniciales pertenecen a `SoundRef`/`SoundCore`. Mantener el split mientras no aparezca una frontera de fuente independiente.
- `atn::SoundManager` es el nombre documentado por RTTI/vtable (`0x80779698`), pero la unidad también incluye numerosas funciones anónimas. No inventarles pertenencia hasta revisar sus referencias.
- No se añadieron excepciones a la regla 2.
