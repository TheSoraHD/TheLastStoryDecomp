# Cola D2 (segunda pasada de efectos, tanda corta 11:12-12:10) — sin integrar

El entorno en la nube cayo hacia las 11:20 y el agente no llego a commitear. Su trabajo, sin commit y con
`PRECOMMIT FAIL`, esta en `docs/paralelo-20261113-qD2.patch` (solo `include/std/locale_facets_unk.h` y
`src/atn/EffectManager.cpp`; sin los cambios de `symbols.txt`, que habia que rehacer).

- `num_put::do_put(long double)` (0x8023F5E0) 0 -> 90,2 % (NONMATCHING): inline `__put_float_unk` con "nan"/"inf",
  enumeradores `__fixed_unk`/`__scientific_unk`, `ctype<char>::toupper(char*, const char*)`,
  `extern "C" __signbitd/__fpclassifyd`. Los tres ayudantes (0x80241CB8, 0x802411B8, 0x802407CC) solo declarados.
- `ostreambuf_iterator::operator=` con un `bool failed` intermedio: sube todas las `do_put` de EffectManager
  (bool 88 -> 90,5 %, enteros ~79 -> ~86,7 %, `const void*` 82,7 -> 85,7 %).
- Bloqueo del precommit: GameEventScript tambien emite la vtable de `num_put<char>`; con la definicion nueva instancia
  alli `do_put(long double)` y sus literales, y 12 funciones de GameEventScript al 100 % bajan a 99,8-99,97 %.
  Si el original no tiene esa copia en GameEventScript, mover la definicion al `.cpp` de EffectManager; si la tiene,
  re-emparejar GameEventScript con `pdbatch.sh`.
