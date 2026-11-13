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

- Funciones al 0 %: 10 -> 3. Escrita `Model::fn_80097E80` (actualizacion de la animacion: avanza y muestrea las
  pistas en el buffer compartido `sNodePoses` (bloques de 0x100 nodos por pista, 0x300 para la mezcla), mezcla por
  peso, pista 3, `fn_800991D0` y la llamada a `m35C`; NONMATCHING 94,7 %: el original llama a `strlen(m3C8)`
  antes de elegir el puntero de la cadena del nombre de fichero y reutiliza esa prueba en el destructor; MWCC
  elige el puntero antes en todas las formas probadas).
- **`boost::function2<...>::dummy::nonnull` (0x80099F70) al 100 %**: la emite `if (m35C)` de fn_80097E80;
  renombrado en `symbols.txt`.
- `fn_800991D0` declarada `extern "C"` con el modelo como primer argumento (como `fn_8008E818`): declararla
  miembro exige tocar `atn/Model_unk.h` (muy incluida).
- Escritas `fn_80090C58` y `fn_800910D4` (skinning de 3 y 4 juntas en memoria principal, `**out` avanza; mismo
  esquema que `fn_800909B0`: `ScaleMtx_unk`/`AddMtx_unk`); NONMATCHING 77 % y 70 %: orden de los temporales y
  de las sumas (el original carga la tercera matriz entre los stores de la suma con la segunda). `SkinWeight3_unk`
  sube delante de ellas y se define `SkinWeight4_unk` (0x24 bytes).
- Escrita `fn_8008E818` (dibujo de las mallas: LOD o modelo de sombra, materiales de la pasada, esfera inline
  `fn_80094F98`, `DrawEntry_unk` y `fn_8009B93C`; parpadeo del color de `m15C` en la pasada 10; NONMATCHING
  93,9 %: el original lee m50 y el color m40 a traves del bloque de matrices, `m20C_7` con signo y orden de los
  argumentos en pila del constructor). `DrawEntry_unk` pasa de `src/atn/ModelDraw_unk.cpp` a
  `include/atn/DrawEntry_unk.h` (ModelDraw_unk.o identico; `fn_8009B93C` declarada sin nombre de parametro).
- Los limites 0.0/1.0 del parpadeo son temporales `const float&` (`ClampRef_unk`): asi quedan en `.sdata`
  (0x8087D7A0/A4) como en el original. Con el `.sdata` completo la etiqueta 0x8087D7CC deja de cuadrar por
  casualidad con `fn_80096218`: su segunda lista pasa a `SimpleVector<unsigned long>` (instanciacion propia,
  cadenas de `reserve` en 0x8087D7CC; con el tipo puntero el valor se carga tarde). `fn_80096218` sigue al 100 %.
  `@19379` (255.0f) renombrada a `@19235` a mano (pd no la reasigna: el orden de `.sdata2` de Model ya difiere).
- Escrita `fn_8008EEDC` (caja de los joints visibles: recorre hijos y hermanos, esfera de la caja del joint
  contra el frustum, caja escalada `fn_80070B60`, transformada `fn_8008F5F8` y unida a `out`; NONMATCHING
  80,4 %). `fn_8008F5F8` pasa a `inline` (el original la expande aqui y deja su copia debil justo detras; sigue
  al 100 %, ahora `W`, por eso `ordercheck` la cuenta como "missing"). Con asignaciones de `AABB_unk` en
  fn_8008EEDC el `operator=` debil sale detras de ella: se usan copias por miembro. `ModelJoint_unk`
  (`atn/ModelPose_unk.h`) gana los miembros de datos `m2C` y la caja `m58`/`m64` (antes relleno).
- Escrita `Model::fn_8008CD60` (cola del modelo en las pasadas de render segun `mFlags`, escena de la esfera,
  dibujo de depuracion de joints y esfera con `DrawHelper`; NONMATCHING 95,8 %: el original guarda las pruebas
  de flags como bool en registros y recarga `mFlags`; probado con inline que devuelven bool, igual o peor).
- Cada cambio de numeracion deja el 255.0f de `.sdata2` (0x80880C14) sin renombrar por pd: ahora `@20749`.
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

- `ModelBase::vf14` (0x8008FA24) recibe 4 argumentos (el cuarto es la caja de salida de fn_8008EEDC), la
  cabecera declara 3: cambiar la firma de la virtual toca `atn/Model_unk.h` y sus derivadas; no hecho.

- `m20C_7` de `ModelBase` es un bit con signo en `fn_8008E818` (srawi): la cabecera lo declara `unsigned`
  (cabecera compartida, no tocado).
- Orden de `.sdata2` de atn/Model: el 0.0f va primero en el original y tercero aqui.
