# game/main

- Estado: 11/14 funciones al 100%, `matching: false`; `main` 98,61%, arranque `fn_8047A4C8` 39,58%. No se modificaron splits ni `symbols.txt` y no hubo funciones nuevas al 100%.
- Prueba adicional: iterar `mData` con puntero dejó `main` en 90,95%; se revirtió.
- Validación limpia: `main.dol: OK`; `regress.py` lost: 0 frente a `build/report.before-clean.json`; `fuzzdrop.py` 0 bajadas; `poolcheck.py game/main` sin hallazgos.
- Decisiones a revisar: el orden de registros del bucle de callbacks; la construcción inline de `boost::function` en el arranque frente a `Vector_unk::push_back`; renombrar `GameSystem_unk` a `ResetShutdown` para alinear el tipo del puntero a miembro.
