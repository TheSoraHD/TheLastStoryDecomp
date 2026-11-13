# Cola B2 (paralelo-20261113): segunda pasada de Camara

Unidades: `game/GameCamera_unk`, `game/FinishCamera_unk`.

## Resultado por unidad

### game/GameCamera_unk

- `GameCamera::fn_8038F52C` (0x8038F52C, 3656 bytes; eleccion del modo segun el estado m55C/control m560 del
  personaje, tabla de 0x8A casos): **100 %**. Salen con ella al 100 % sus copias weak `fn_8038B648` (mC48),
  `fn_80387540` (mControl), `fn_80382A00` (m58C), `fn_803830A0` (TownTask), `fn_803903C4` (ChaseManager),
  `fn_803903CC` (ChaseManager::mState != 0) y `fn_80390374` (inline en el .cpp, copia weak detras de ella).
  Funciones al 100 %: 28 -> 39 de 90 (tras re-emparejar etiquetas).
- `GameCamera::fn_80390A88` (0x80390A88, 3684 bytes; camara libre de la vista tactica, `m8E0`): escrita,
  NONMATCHING ~63 %: la logica coincide (comprobada bloque a bloque); difieren la disposicion de la pila (el
  original tiene 0x10 bytes mas de locales: Mtx en 0x168, move en 0x158, la copia del cursor de TacView repartida
  en 0x140/0x0C/0x1C) y la planificacion de las primeras sumas de vectores (el original redondea con `frsp`
  la z del ojo).
- `GameCamera::fn_80382A1C` (0x80382A1C, 1668 bytes; camara de la ciudad, parametros m6FC; por encima del
  presupuesto: todo fuera de linea): escrita, NONMATCHING 64,6 %: solo difieren las dos rotaciones en y (el original
  llama a `Matrix3x4::fn_8013A13C` como funcion que devuelve la matriz por valor; aqui `Matrix3x4::RotY_unk`,
  porque `Matrix3x4 m; m.fn_8013A13C()` anade una llamada al constructor vacio) y dos registros de la mezcla
  del pitch. Salen al 100 % sus copias weak `fn_8037F640` (cos) y `atn::Camera::SetUpVector`.
- `fn_8038AEF0`: el umbral era 1.0e-6f y el original lee 0.0001 (0x808859DC): corregido (quita el aviso
  1e-06 de poolcheck).
- `GameCamera::fn_8037E12C` (0x8037E12C, 1904 bytes; lee el csv de camaras del mapa): escrita, NONMATCHING 66 %:
  el original copia los ajustes del jugador de GameConfig_unk (0x244-0x333, un struct alli, miembros sueltos
  aqui) a una local y de vuelta para las dos distancias de fijado (0xF0 bytes mas de pila); el resto coincide.
  Con sus cadenas ("gun".."fixRotY") el pool queda completo: `fn_8037E9E0`, `fn_8037ECF0` y `fn_8039328C`
  pasan al 100 % (poolorder igual).
- `GameCamera::GameCamera` (0x8037D8B8, 944 bytes): **100 %**. Hace falta el constructor por defecto de
  `GameCameraParam_unk` (m34 = 0), `GameCameraTimer_unk` (mActive = 0), `GameCameraModeParam_unk` (mMode = 0) y
  `CameraShake_unk` (todo a 0): declarados sin definir en `GameCamera.h` y definidos `inline` en el .cpp.
  `mA0C[5]` pasa a cinco floats `mA0C..mA1C` (el original los inicializa como floats en la lista; solo esta
  unidad los usa). Con el ctor delante, el pool de `.sdata2` empieza en el orden del original.
- Funciones al 0 %: 16 -> 11. Al 100 %: 28 -> 45 de 90.

## Decisiones tomadas sin el usuario

- `fn_8038F52C` pasa a metodo `GameCamera::fn_8038F52C(CharaTask*)`: declaracion nueva en `GameCamera.h` (sin
  nombre de parametro, no renumera), nombre mangled en `symbols.txt` y `game/GameTask.cpp` (otra cola, ya
  cerrada) llama `mCamera.fn_8038F52C(...)` en lugar del `extern "C"` con `GameCamera*`. Quitar esa declaracion
  (dos parametros con nombre) renumero GameTask en -2: re-emparejada con `pd.sh` (98 -> 106 funciones al 100 %
  en fnstat; report sin perdidas) y `@27398` (0x808857A8, ambigua) renombrada a mano a `@27396`.
- `fn_800F8524` (lerp, copia weak de otro fichero) toma `const float&` en lugar de punteros: con un temporal
  (`-DegToRad_unk(7.0f)`) el original lo pone en la pila despues de las variables con nombre; actualizadas sus
  dos llamadas de fn_80387910 (borrador, sin cambio de codigo).
- `GameCamera.h` sin tocar salvo la declaracion. En `GameCamera_unk.cpp`: vistas locales `Settings_unk`,
  `Field_unk` y `CameraConfig_unk` (como en game/CharaTask.cpp, game/GameUI.cpp y Event/Path.cpp; el tipo real
  es GameConfig_unk/GameTask/SetConfig, pero los nombres mangled de sus copias weak usan esas clases).
- `fn_80390A88` tambien pasa a metodo (`fn_80390A88__10GameCameraFv`, declaracion sin parametros en
  `GameCamera.h`); GameTask la llama `mCamera.fn_80390A88()`. Sus llamadores de TacView (cola A2) no estan
  escritos en fuente: cuando se escriban, usar el metodo. `GameCamera.h`: `int m8E0` en lugar de relleno.
- En `GameCamera_unk.cpp`: `extern "C" fn_805AA94C(TacView*, ...)` (metodo de TacView sin declarar en
  `game/TacView.h`, otra familia) y vistas locales `TacViewTargetPos_unk` (posicion en 0x10 del objetivo del
  cursor) y `TacViewTask_unk` (`TacView::m15AC`, tarea con estado en 0x48): deuda de la regla 10, pendiente de
  llevar a `game/TacView.h`.
- `include/atn/Camera_unk.h` (otra familia, 252 unidades): declaracion `void SetUpVector(const Vector3&);` sin
  nombre de parametro (labelcheck igual); la definicion inline esta en `GameCamera_unk.cpp` (unico usuario).
- Bloque de ayudantes `extern "C" inline` (fn_80011220, fn_8037F664, fn_801C3910, ...) subido delante de
  fn_803830A8 para usarlo en fn_80382A1C; `fn_803830A0` tambien. `fn_805B81F0` declarada `extern "C"` como en
  game/TownTask.cpp.
- `GameCamera.h`: constructor por defecto `GameCameraParam_unk();` declarado sin definir (pone m34 a 0; la
  definicion inline esta en `GameCamera_unk.cpp`, no renumera: labelcheck igual) y `GameCameraParam_unk m430` en
  lugar del relleno (miembro de datos). Con el ctor declarado faltaria aun la herencia para el ctor de GameCamera.
- `GameCamera.h` (datos, no renumeran): `float m41C`, `int m8D0`, `float m958, m95C`, `float m974`, `int m984`,
  `float m988`, `atn::Vector2_unk mC88[3]` y `mCA0` en lugar de relleno; `mA0C[5]` -> `mA0C, mA10, mA14, mA18,
  mA1C`. Constructores por defecto declarados (sin parametros) en `GameCameraTimer_unk`, `GameCameraModeParam_unk`
  y `CameraShake_unk`. labelcheck igual.
- `.sdata` lbl_8087DD04..lbl_8087DD34 (extremos de las mezclas) quedan `extern "C"` como lbl_8087DCF0..DD00.
- Etiquetas ambiguas de `.sdata2` que `pd.sh` no re-empareja, renombradas a mano con su direccion:
  `@15867`->`@16522` (0x808858E8), `@15896`->`@16551` (0x808858F8), `@15877`->`@16532` (0x80885954),
  `@15893`->`@16548` (0x80885910); tras fn_80390A88 otra vez (+732): `@17254` (0x808858E8), `@17283`
  (0x808858F8), `@17280` (0x80885910), `@17264` (0x80885954). En GameTask `@27395`/`@27396` (0x808857A8/AC).
  Tras fn_80382A1C (+570): `@17824`, `@17853`, `@17850`, `@17834` en las mismas direcciones; tras fn_8037E12C
  (+187): `@18011`, `@18040`, `@18037`, `@18021`; tras el ctor: `@18264`, `@18268`, `@18274` (0x808858E8/F8,
  0x80885910) y `@18323` (0x80885954).
- poolcheck de GameCamera_unk avisa ahora de la cadena '?fff>L' (0.9 seguido de 0.2 en nuestro .sdata2: orden del
  pool por primer uso; la unidad no enlaza).

## Hallazgos

- `x / 4.0f` y `x / 8.0f` dan `fmuls x, 0.25` con x primero; `0.25f * x` o `x * 0.25f` dan el orden contrario.
- Un `unsigned char` comparado con `cmpwi` tras `lbz`: `(int)campo == 4`.
- MWCC ordena las locales de la pila por tamano (CollResult_unk 0x50 y Mtx arriba aunque se declaren despues).
- Un `Matrix3x4` temporal sin llamada al constructor sobre el que se llama `fn_8013A13C(angulo)`: es una funcion que
  devuelve la matriz por valor (`static Matrix3x4 fn_8013A13C(float)`), no un metodo sobre una local.
- `const atn::Vector3& head = c->fn_80178088();` mantiene la direccion del temporal en un registro salvado.
- Inicializacion de `x = y = z = 0.0f` encadenada: guarda z, y, x (orden del original en m98C y mC88).
- `cntlzw; srwi.; bne` en una condicion: inline que devuelve `x == 0` usada negada; con `!(x == 0)` directo sale
  `cmpwi` (no se consiguio en fn_80390A88).

## Pendientes

- `fn_80390A88`: pila (ver arriba).
- `~GameCamera` 99,97 %: el original trata la camara de 0x00 como miembro (no base); cambia la herencia de una
  clase incluida por 144 unidades.
- `fn_8037E12C`: agrupar 0x244-0x333 de `GameConfig_unk.h` en un struct (otra familia; cambia sus usos) para
  llegar al 100 %.
- `atn::Matrix3x4::fn_8013A13C` deberia ser estatica y devolver la matriz (cambia todos sus llamadores, otra
  familia): con eso fn_80382A1C llegaria casi al 100 %.
- TacView.h: declarar `fn_805AA94C` como metodo y los campos de `TacViewTarget_unk` (0x10) y `m15AC` (cola A2).
