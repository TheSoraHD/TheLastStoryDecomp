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

## Decisiones tomadas sin el usuario

- Cabecera nueva `include/std/tr1_random.h` (solo la incluyen CryptMP y MotionBank_unk) movida desde MP/CryptMP
  (otra familia, autorizado por la cola).

## Hallazgos

- Un literal `double` ligado a un parametro `const double&` de un ctor inline va a `.data` como objeto anonimo `@N`
  (no a `.rodata`): explica los `0.0`/`1.0` de `.data` de MotionBank_unk y CryptMP.

## Pendientes
