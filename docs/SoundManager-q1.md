# SoundManager (avance inicial)

Rama local `codex/soundmanager-decomp`, 2026-10-24. Se registró `atn/SoundManager.cpp` aprovechando el split existente, sin tocar `splits.txt` ni `symbols.txt`. De las 76 funciones del split, 23 están al 100%: métodos `SoundRef`, wrappers de `SoundCore`, y cuatro saltos directos. Ocho funciones tienen fuente pero siguen `NONMATCHING`; 45 aún no tienen implementación. La unidad permanece `matching: false`.

Build limpio: `main.dol: OK`. `regress.py`: lost 0 (7524 funciones frente a la referencia de 7501); `fuzzdrop.py`: 0 bajadas; `poolcheck.py atn/SoundManager`: 0 hallazgos.

## Decisiones a revisar

- La unidad contiene el rango completo `0x800CB360–0x800D1C60`; los primeros símbolos pertenecen a `SoundRef`, mientras que los siguientes todavía no se han identificado. Confirmar la pertenencia por referencias; mantener el split existente mientras se reconstruyen.
- `atn::SoundManager` es el nombre documentado por RTTI/vtable (`0x80779698`), pero la unidad también incluye numerosas funciones anónimas. No inventarles pertenencia hasta revisar sus referencias.
- No se añadieron excepciones a la regla 2.
