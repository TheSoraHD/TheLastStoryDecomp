# SoundManager (avance inicial)

Rama local `codex/soundmanager-decomp`, 2026-10-24. Se registró `atn/SoundManager.cpp` aprovechando el split existente, sin tocar `splits.txt`; la revisión identificó constructor y destructor en `symbols.txt`. De las 76 funciones del split, 25 están al 100%: métodos `SoundRef`, wrappers de `SoundCore`, cuatro saltos directos y dos helpers de estado/liberación. Ocho funciones tienen fuente pero siguen `NONMATCHING`; 43 aún no tienen implementación. La unidad permanece `matching: false`.

Build limpio: `main.dol: OK`. `regress.py`: lost 0 (7526 funciones frente a la referencia de 7501); `fuzzdrop.py`: 0 bajadas; `poolcheck.py atn/SoundManager`: 0 hallazgos.

## Revisión del split y símbolos

El split se conserva: sus límites contiguos (`Sound3dEngine` termina en `0x800CB360`, `SoundManager` termina en `0x800D1C60`, y `Task` empieza allí) coinciden con el orden de emisión y no cortan ninguna función. El área de datos incluye la vtable/RTTI de `atn::SoundManager` (`0x80779698/0x807796A8`), el singleton `lbl_8087EFE8`, los flags y el estado estático que usan sus métodos.

Se renombraron dos símbolos con evidencia directa: `fn_800CCBF8` es `__ct__Q23atn12SoundManagerFv` (tamaño asignado 0x46E8 y llamada desde el inicializador del singleton); `fn_800CDD8C` es `__dt__Q23atn12SoundManagerFv` (la vtable apunta a él y destruye los miembros). `fn_800CDD40`/`fn_800CDD4C` se implementaron sin nombres semánticos: el primero pone a cero un byte; el segundo libera el puntero si el indicador es positivo y lo devuelve. Siguen anónimos los métodos no identificados; tres rutinas iniciales de 0x68C son parecidas, pero su semántica no es fiable para nombrarlas.

## Decisiones a revisar

- La unidad contiene el rango completo `0x800CB360–0x800D1C60`; el constructor/destructor confirman que los métodos del manager están dentro. Los helpers iniciales pertenecen a `SoundRef`/`SoundCore`. Mantener el split mientras no aparezca una frontera de fuente independiente.
- `atn::SoundManager` es el nombre documentado por RTTI/vtable (`0x80779698`), pero la unidad también incluye numerosas funciones anónimas. No inventarles pertenencia hasta revisar sus referencias.
- No se añadieron excepciones a la regla 2.
