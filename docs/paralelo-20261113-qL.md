# paralelo-20261113, cola L (motor atn)

## atn/MotionBank_unk

- Funciones al 0 %: 1 -> 0. Escrita `fn_800A2664` (generador aleatorio de MP::CryptMP, `new` en el primer uso con
  `OSGetTick()` como semilla; NONMATCHING 83,4 %: el original construye el motor antes que la distribucion, igual
  que `CryptMP::vf0C`, y aqui MWCC evalua primero la distribucion).
- Las plantillas TR1 (`mersenne_twister`, `uniform_real`, `variate_generator`) salen de `src/MP/CryptMP.cpp` a
  `include/std/tr1_random.h` (CryptMP.o identico byte a byte).
- Probado sin exito: motor por `const&` en `variate_generator`; distribucion por defecto; `uniform_real` con
  `const RealType&` (los literales van a `.data` anonimo como en el original, pero carga min antes que max y cambia
  CryptMP); sin ctor de copia de usuario en `mersenne_twister` (orden correcto, 85,9 %, pero MWCC elide dos de las
  cuatro copias del motor); ctor de copia de usuario en `uniform_real`; array estatico `{0.0, 1.0}`.

## atn/LayerOcclusionCPU

- Funciones al 0 %: 3 -> 3, no empezadas: `fn_800B4390` es asm a mano (`mfcr`, `ps_cmpo0`, `mcrf`, `mtcrf`) y
  `fn_800B47AC`/`fn_800B5994` escriben `GQR6` con `mtspr` (paired-single cuantizado, no aprobado, regla 2).

## atn/Model

- Funciones al 0 %: 10 -> 8. Escrita `Model::fn_80097E80` (actualizacion de la animacion: avanza y muestrea las
  pistas en el buffer compartido `sNodePoses` (bloques de 0x100 nodos por pista, 0x300 para la mezcla), mezcla por
  peso, pista 3, `fn_800991D0` y la llamada a `m35C`; NONMATCHING 94,7 %: el original llama a `strlen(m3C8)`
  antes de elegir el puntero de la cadena del nombre de fichero y reutiliza esa prueba en el destructor; MWCC
  elige el puntero antes en todas las formas probadas).
- **`boost::function2<...>::dummy::nonnull` (0x80099F70) al 100 %**: la emite `if (m35C)` de fn_80097E80;
  renombrado en `symbols.txt`.
- `fn_800991D0` declarada `extern "C"` con el modelo como primer argumento (como `fn_8008E818`): declararla
  miembro exige tocar `atn/Model_unk.h` (muy incluida).
- `pd.sh atn/Model` vuelve a renombrar `lbl_8087D6E8/EC`: devueltos a mano.

## Decisiones tomadas sin el usuario

- Cabecera nueva `include/std/tr1_random.h` (solo la incluyen CryptMP y MotionBank_unk) movida desde MP/CryptMP
  (otra familia, autorizado por la cola).

## Hallazgos

- MWCC evalua la seleccion inline `is_long() ? ... : ...` de `c_str()` antes que un `strlen` de otro argumento
  de la misma llamada (`strncmp(s.c_str(), n, strlen(n))`), con o sin temporales; el original de fn_80097E80 no.

- Un literal `double` ligado a un parametro `const double&` de un ctor inline va a `.data` como objeto anonimo `@N`
  (no a `.rodata`): explica los `0.0`/`1.0` de `.data` de MotionBank_unk y CryptMP.

## Pendientes
