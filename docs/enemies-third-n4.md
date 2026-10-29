# Lote enemies-third, cola n4 (Em204Mulue, Em204BigMulue, Em204MulueMp)

Rama agent-n4. Referencia: /home/claude/ref/n4_start.json (7305 al 100%). Final: 7377 (+72), lost: 0, sin bajadas
de fuzzy, poolcheck limpio, `main.dol: OK`. Ninguna unidad enlazable (todas con NONMATCHING).

## Unidades

### game/Em204Mulue (0/39 -> 29/39)
- Fuente nueva completa en orden del .dol.
- NONMATCHING: ctor 99.80 (registros del segundo std::string), vf10 99.81 (orden de dos cargas del caso 9 y pila
  de la matriz de "catchpoint"), fn_802A7970 98.41 y vf54 98.93, fn_802AA1B8 99.91, vfEC 99.26 (familia
  `__VecCopy`), fn_802A8BBC 99.58 (constantes del bucle en f20-f31, un registro desplazado), fn_802A91D0 98.82 y
  fn_802A94C8 99.95 (bitfield del registro de daño), fn_802A9ADC 99.59 (orden de stores del disparo).

### game/Em204BigMulue (0/38 -> 20/38)
- Fuente nueva completa. vf30/vf90 al 100% al poner las instanciaciones explícitas de los dos `push_back` detrás de
  fn_802AC8E8 (su "vector length error" va detrás de las claves del lector en el pool).
- Corregidas constantes del borrador: fn_802B0D90 (rayo 300, altura 20, ancho de línea 7, centro `a + b - mPos`),
  vf108 (20 en vez de 24), vf40 (`ratio * ratio`, `== 0x2000000`); el ctor no inicializa m1AC0.
- NONMATCHING: fn_802AC8E8 0% (el original llama fuera de línea al ctor de atn::Csv, los ctores/dtores de los
  elementos 0x802ACC94/0x802AD358 y los push_back; el nuestro lo inlinea todo: probadas instanciaciones explícitas y
  ctores/dtores explícitos), push_back 75.7/76.3 (familia conocida), vf0C 99.37, vf10 99.16, vf50 96.82, vf20 99.52,
  vf100 97.60, vf104 99.79, fn_802B0430 99.89, fn_802B07F0 98.98, fn_802B0D90 99.48 (pila), fn_802B12D8 98.29
  (`mfcr` en la comparación final, 6 variantes), fn_802B17AC 99.41, fn_802B1B50 98.77 (r24-r31), dtor 99.15.

### game/Em204MulueMp (0/31 -> 23/31)
- Fuente nueva completa, incluida la tabla de colores de materiales `lbl_807860A0` (6 entradas x 3 modos) y su
  `__sinit` (al 100%).
- NONMATCHING: ctor 99.81 (std::string), vf104 99.90 (pila de los casos 10 y default), fn_802B4B08 98.41, vf54 98.93,
  fn_802B596C 99.93, fn_802B5DA4 99.91, fn_802B6104 99.83 (familia `__VecCopy`), fn_802B55B0 98.72 (r23-r31).

## Decisiones tomadas sin el usuario
- Fábrica de `game/SpecialEnemyBase.cpp`: casos 20/21/22 con `new (3, "", "", 0) Em204Mulue/Em204BigMulue/
  Em204MulueMp(parent, info)` (quitados los extern de los tres ctores); incluye `game/Em204Mulue.h`.
- `MulueMpState_unk`: Em204MulueMp::vf110/vf114 usan su propia vista de `SpecialState_unk` (shorts en 8/0xA/0xC).
- `BigMulueChara_unk`/`BigMulueState_unk` (registro 0x1C en 0x1470 del jugador, como `PlayerState_unk` de CharaTask).
- `fn_802A7910`/`fn_802A7964` (ayudantes de ángulo) también como `extern "C" inline` en Em204MulueMp.cpp (copias
  débiles, igual que Em403WarLion).

## Cambios en ficheros compartidos
- `config/SLSP01/symbols.txt`: solo renombrados en los rangos de las tres unidades (`@stringBase0` para los pools
  0x80745FE4/0x807462A8/0x807465E0, `__sinit_\Em204MulueMp_cpp` local). Sin cambios de splits.
- `include/game/Em204Mulue.h` nuevo (las tres clases y sus tipos auxiliares).
- `src/game/SpecialEnemyBase.cpp`: fábrica (ver arriba). Posible conflicto con otras colas que toquen la misma fábrica.

## Hallazgos generales (una línea cada uno)
- f30/f31 cruzados entre distancia y yaw: escribir el ángulo en una sola expresión
  `fn(d.GetNormalized_unk().ToRotation_unk().y - GetRot_unk()->y)` (Mulue fn_802AC0E0, MulueMp fn_802B6F80).
- Bloque de efecto escrito en línea con locales `color/rot/pos` (orden inverso) da la pila del inline sin hacer de la
  función un candidato de inline para su llamadora (fn_802AB5E8 deja de inlinearse en vf10).
- `EffectMgr_unk* mgr = lbl_8087F3C0;` / `Effect_unk* mgr = lbl_8087F048;` antes de la llamada: el global se carga
  antes que los argumentos (como Em101ElderZesya).
- Interpolación de color: `inline Lerp_unk(a, b, t) { return (b - a) * t + a; }` da el 100% donde la expresión directa no.
- Orden de los `case` en el fuente = orden de direcciones de los destinos de la tabla de saltos.
- Instanciaciones explícitas de plantillas detrás de la función que las usa: sus literales van detrás en el pool.
- `float y = mRot.y; PSMTXRotRad(m, 'y', y);` dentro de un inline corrige el registro de `this`.
- `if (!(fabs(x) < eps))` da el `blt` del original donde `>=` da `cror`.
- `GimmickMsg_unk msg; msg.mType = 4;` (ctor por defecto y luego el tipo) reproduce el orden de stores.
- `if (frame >= len) {...} else ...` con `float frame = m08;` local en vez de un inline `bool`: evita `mfcr` en
  funciones grandes (vf10, vfEC).
