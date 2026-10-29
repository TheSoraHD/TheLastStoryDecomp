# std::bad_cast y GameEventScript

- Resultado: `std::bad_cast::~bad_cast` (`0x80394068`) y `std::bad_cast::what() const` (`0x803AD13C`) siguen al 100%, cada una en su split de una función. Ambos están sin enlazar (`matching: false`).
- El `.text` restante está repartido en `game/GameEventScript.cpp` y dos continuaciones, con cortes exactos a los límites de las funciones MSL. `.ctors`, pools y datos intercalados se mantienen en las unidades GameEventScript que los contienen; no se inventaron cortes de datos.
- Decisión pendiente: revisar si conviene separar más tarde los datos MSL (`__vt__Q23std8bad_cast`/RTTI en `.data`) del bloque compartido de GameEventScript.
- Validación limpia: `main.dol: OK`; `regress.py` lost: 0 (7301 vs. 7300); `fuzzdrop.py` drops: 0; `poolcheck.py std/bad_cast std/bad_cast_what` sin hallazgos.
