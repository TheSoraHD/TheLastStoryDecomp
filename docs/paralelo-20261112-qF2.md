# Lote paralelo 2026-11-12, cola F2 (Efectos, segunda pasada)

Inicio: 2026-11-12 21:40 CET. Fin: 22:30 CET. Rama `agent-F2`. Unidades: `atn/EffectSystem_unk`, `atn/EffectManager`.

## Resultado por unidad

- `atn/EffectSystem_unk` (commit 1): 0 % 7 -> 4; escritas NONMATCHING fn_8022C3F0 (liberacion de instancias y
  slots, anillo de fundidos, 99,3 %), fn_8022C9C8 (matriz de una instancia segun el modo del emisor, 98,3 %),
  fn_8022FAC4 (creacion de un efecto, 99,8 %).
- `atn/EffectSystem_unk` (commit 2): 0 % 4 -> 2; escritas NONMATCHING fn_80230780 (preparacion de un slot nuevo,
  99,9 %) y fn_80233D74 (valores iniciales aleatorios de un slot, 86,5 %).
- `atn/EffectSystem_unk` (commit 3): 0 % 2 -> 1; escrita NONMATCHING fn_8023193C (paso de un slot: frame de la
  textura, desplazamiento, posicion, tamano, direccion, velocidad con rebote en el suelo, escala y color,
  98,4 %). Queda al 0 % solo fn_8022DD1C (dibujado de un slot, 7592 bytes, 18 floats guardados).

- **Total EffectSystem_unk**: funciones al 0 % 7 -> 1; al 100 % sin cambios (43/58 en `report.json`); escritas
  NONMATCHING nuevas 6. No enlaza todavia (falta fn_8022DD1C).
- `atn/EffectManager`: sin cambios (29 al 0 %). fn_8023BA10 es el `operator<<` de un `ostream` (sentry, flush,
  `num_put::put`) instanciado en la unidad: hay que reconstruir `num_put`/`numpunct`/`basic_ostream` de MSL en
  `libs/` (no estan en `msl_iostream_unk.h`); no se empezo.

## Pendientes

- fn_8022DD1C (dibujado de un slot): ~1900 instrucciones, 26 PSMTXConcat, 20 PSMTXRotRad, una matriz 4x4
  (PSMTX44MultVec) y atan2 (parece `Vector3::ToRotation_unk`/`Matrix44` de `math/Vec.h`); empieza mirando
  param->m25C bit 1 (distancia a la camara > m260 + m264: no se dibuja) y los flags 0x20/0x100 de la instancia.
- EffectManager: MSL de `ostringstream` (ver arriba).

## NONMATCHING

- `fn_8022C3F0` 99,3 %: en la condicion del bucle del anillo el original carga m1AC (r3) antes del maximo de mB4.
- `fn_8022C9C8` 98,3 %: modo 1: el original guarda punteros a la posicion y angulos de la camara (r25/r27) y el
  RenderManager en r4; huecos de pila de los temporales (marco 0x10 menor).
- `fn_8022FAC4` 99,8 %: huecos de pila de los temporales de las rotaciones y y z intercambiados (el original
  pone los de y arriba); probado: rotaciones encadenadas, bloques sin inline, orden de declaracion.

- `fn_80230780` 99,9 %: rama else: el original carga el puntero de entradas despues de param->m40 (y r5/r7 del
  indice del emisor). Probado: local para las entradas, aritmetica de punteros.
- `fn_80233D74` 86,5 %: registros del rango y su negacion cruzados (f29/f30), el 0.0 de la comparacion se carga
  antes; el original guarda un cuarto float (f28) para la base 0.0 de m12C. Probado: rango por referencia,
  inline (lo, hi), base como variable, varias formas de la expresion.

- `fn_8023193C` 98,4 %: los resultados de las curvas multiplicados por la escala pasan por `frsp` sin store en el
  original (m98.z, mB8, mCC; con `fn_8023329C` devolviendo double bajan otras 6 funciones, descartado);
  orden de carga de los argumentos de fn_8022D790; emitter*4 se calcula una vez.

## Decisiones tomadas sin el usuario

- `include/atn/EffectSystem_unk.h` (67 unidades): solo **miembros de datos** (sin funciones nuevas, no renumera
  `@N`): `EffectAnimData_unk` m08/mEntries(0x0C)/mParticleDefCount(0x10)/m14 y m20..mParticleOn(0x2C) al
  final (la estructura crece de 0x20 a 0x30; solo se usa por puntero); `EffectParam_unk::m40` (indice de la
  entrada de datos) y `m64` (escala del movimiento); `EffectSlot_unk::m1AC/m1B0/m1B4` pasan a `unsigned int`
  (divwu y conversion sin signo en el original).
- Mas miembros de datos en `EffectSystem_unk.h` (commit 2): `EffectAnimData_unk::mResources` (0x34; crece a
  0x38), rangos aleatorios de `EffectParam_unk` (0xD4.., 0x110.., 0x14C.., 0x188.., 0x1D4..0x1E4, 0x1F4,
  0x228..0x230), `m234` y `m23C`; `EffectSlot_unk` m98 (Vector3), mA8/mAC, mBC (color), mDC/mE0, m12C, m174/
  m180/m184. Los arrays de relleno se acortan; ningun cambio de tamano en EffectParam_unk ni EffectSlot_unk.
- Commit 3: `EffectAnimData_unk::mLocked` (0x30, puntero por emisor de curvas no aplicadas); velocidades y
  amortiguaciones de `EffectParam_unk` (0xF8..0x100, 0x134..0x148, 0x170..0x184, 0x1B4..0x1D0, 0x1E8..0x1F0,
  0x210..0x224) y rebote 0x240..0x258. `fn_8022D790` gana un sexto parametro sin usar (el original pasa
  slot->m24 en r8; su codigo no cambia).
- fn_8023193C reproduce un fallo del original: la direccion en el suelo solo se normaliza si sus tres
  componentes son no nulas (y = 0 siempre, asi que nunca).
- `fn_80237B8C` se declara `void*` (lista generica, como fn_80237C88) y el llamador de slots hace el cast.
- `lbl_8087DBEC` definida en la unidad como `sSerial_unk = 1` (numero de serie de los efectos).
- Inlines nuevos en el `.cpp` (no en cabeceras): `FreeSlot_unk`, `EffectMaxT_unk`, `EffectRotate_unk`,
  `EffectPosRotMtx_unk` (trans(pos) * rot_y * rot_x * rot_z, con `Matrix3x4* p = &m` como en RotateZYX_unk),
  `EffectColLen_unk`, `SetParticleOwner_unk`.

## Hallazgos

- Patron `cntlzw; srwi.` = `if (Differ(a, b))` con un inline bool que niega otro inline bool de igualdad.
- Una condicion `a == 0 || (a[i] & f) != f` dentro de un inline bool se materializa (li 1/li 0); escrita en el
  `if` sale como el original.

- `max<unsigned>(x - 1, 0)` sale como `neg; or; srawi 31; and` (x != 0 ? x : 0).
- Escribir en `Matrix3x4` a traves de `Matrix3x4* p = &m` deja la matriz en un registro (mr r4, r25) como el
  original; con `m` directa sale `addi r4, r1, X`.
