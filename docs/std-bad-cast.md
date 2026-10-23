# std/bad_cast

- Resultado: 2/219 funciones al 100% (`std::bad_cast::~bad_cast` en `0x80394068`; `std::bad_cast::what() const` en `0x803AD13C`). Unidad sin enlazar (`matching: false`).
- El split actual cubre `.text` `0x803935AC–0x803B27AC` y bloques grandes de datos que incluyen `GameEventScript`, `boost::bad_lexical_cast` y datos MSL. No se recortó porque los rangos de vtable, RTTI y cadenas están intercalados con otros datos.
- Decisión a revisar: separar `GameEventScript` y los datos ajenos para poder reducir `std/bad_cast`, o mantener la unidad combinada y decompilar el resto.
- Validación limpia: `main.dol: OK`; `regress.py` lost: 0; `fuzzdrop.py` drops: 0; `poolcheck.py std/bad_cast` sin avisos.
