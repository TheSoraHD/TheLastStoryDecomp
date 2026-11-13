# Cola B2 (paralelo-20261113): segunda pasada de Camara

Unidades: `game/GameCamera_unk`, `game/FinishCamera_unk`.

## Resultado por unidad

### game/GameCamera_unk

- `GameCamera::fn_8038F52C` (0x8038F52C, 3656 bytes; eleccion del modo segun el estado m55C/control m560 del
  personaje, tabla de 0x8A casos): **100 %**. Salen con ella al 100 % sus copias weak `fn_8038B648` (mC48),
  `fn_80387540` (mControl), `fn_80382A00` (m58C), `fn_803830A0` (TownTask), `fn_803903C4` (ChaseManager),
  `fn_803903CC` (ChaseManager::mState != 0) y `fn_80390374` (inline en el .cpp, copia weak detras de ella).
  Funciones al 100 %: 28 -> 39 de 90 (tras re-emparejar etiquetas).

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
- `.sdata` lbl_8087DD04..lbl_8087DD34 (extremos de las mezclas) quedan `extern "C"` como lbl_8087DCF0..DD00.
- Etiquetas ambiguas de `.sdata2` que `pd.sh` no re-empareja, renombradas a mano con su direccion:
  `@15867`->`@16522` (0x808858E8), `@15896`->`@16551` (0x808858F8), `@15877`->`@16532` (0x80885954),
  `@15893`->`@16548` (0x80885910).

## Hallazgos

- `x / 4.0f` y `x / 8.0f` dan `fmuls x, 0.25` con x primero; `0.25f * x` o `x * 0.25f` dan el orden contrario.
- Un `unsigned char` comparado con `cmpwi` tras `lbz`: `(int)campo == 4`.

## Pendientes

- `fn_80390A88` (3684 bytes): la llaman GameTask y TacView (cola A2) como `extern "C"` con `GameCamera*`.
