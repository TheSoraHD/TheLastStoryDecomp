# port-decomp-20261114, cola O (tercera pasada de camaras y minimapa)

Agente O, rama `agent-O`. Unidades: `game/GameCamera_unk`, `game/FinishCamera_unk`, `game/MiniMapTask`.

## game/GameCamera_unk

- `GameCamera::fn_80385B48` (0x80385B48, 6648 bytes; modo apuntar m60C): **escrita** desde el asm (sin DRAFT),
  0 -> **98,4 %**. Firma `void GameCamera::fn_80385B48(const CharaTask*)` (sin cambios). Diferencias con
  fn_80384188: parametros m60C (m00 distancia, m04 fovy, m18 bajada del ojo), offset m874 entero (z incluido),
  el giro del offset usa `Vector3(0.5f * angle.x, chara->mRot.y, 0)` y el lateral `chara->mRot.y`, el origen es
  `chara->mPos` (referencia `hips`), sin giro con el stick mientras se lanza una magia (control 30 con
  `chr::ControlMagic::m1D`, o control 31), ojo bajado m60C.m18 con mFlag6 y parte m674, colision hacia atras en
  dos pasos (`back`), cabeza: `to = from + dir * (3 + len) * dot` y solo con m674 < 0 se mira la colision, y fovy
  maxima m60C.m04 con mFlag6 o 60 grados sin ella.
  **Esta justo por debajo del presupuesto de inlining**: con `SetTarget_unk(...)` en el primer bloque pasa por
  encima (31 %); con `mTarget = ...` y el objetivo mezclado en un local `v` queda por debajo y lo inlinea todo como
  el original. Resto: huecos de pila (los locales con nombre van 0xC mas arriba por `v`; en el original es un
  temporal), la recarga de `SetConfig::sInstance` antes de su comparacion con 0 (probados un local `config` y un
  inline `CameraFlag_unk`: no la dan, y el inline pasa el presupuesto) y el orden del primer blend de fovy.
- `GameCamera::fn_80384188` (~32 %): sin cambios. Medido: esta muy por encima del presupuesto (hay que quitar
  desde el bloque del offset hasta el final para que inlinee), mientras que fn_80385B48, casi igual, esta en el
  borde. Quitar el `rot` (fn_80139F60 de mFA4, que el original calcula y no usa), `eye` o `k`, o pasar a
  `mTarget =`/locales ox, oy como en fn_80385B48 no basta. Sigue sin saberse que la deja por debajo.

### Hallazgos de presupuesto (fn_80385B48)

- Un local declarado dentro de un bloque anidado (`case 30: { ControlBase* c = ...; }`) cuesta presupuesto y
  la saca por encima; el mismo local en el ambito de la funcion no.
- Usar los accesores que el original referencia primero aqui (`fn_80387540` = `chara->mControl.get()`, weak
  copy justo detras de fn_80385B48; `fn_80385B40` = mDist, detras de fn_80384188) la pasa por encima: el
  original tiene que ser mas barato en otra parte.
- `#include "chr/ControlChant.h"` (para `chr::ControlMagic`) renumera +996 las `@N` de la unidad; pd.sh no
  empareja 21 floats repetidos de `.sdata2` (0.0, 1.0, 3.0...): renombrados a mano (+996, valor comprobado).

### Cambios de layout/firma (para el port)

- Ninguno en GameCamera. `GameCamera_unk.cpp` incluye `chr/ControlChant.h`.

## game/MiniMapTask

- `atn::Camera::SetPosition(float, float, float)` y `SetTarget(float, float, float)` (0x804A9E54/0x804A9E64):
  - -> **100 %**. Declaradas en `atn/Camera_unk.h` **sin nombres de parametro** (0 `@N` en las demas unidades;
  labelcheck igual) y definidas `inline` en `MiniMapTask.cpp`, su unico usuario; `fn_804ABF84` las llama
  (antes `mPos.Set_unk`). fn_804ABF84 sin DRAFT (99,8 %, solo huecos de pila).
- `MiniMapTask::vf10(RenderManager*)` (0x804ABA2C): comprobada llamada a llamada contra el original y quitado el
  DRAFT; sigue al 0 %: aqui queda por debajo del presupuesto e inlinea todos los accesores, el original los llama
  todos. Medido con un candidato artificial: le faltan unas 36-40 sentencias. Un ctor de copia / operator= de
  `atn::Camera` escrito a mano (20 miembros) no basta; quitar los `__declspec(noinline)` de los accesores (y el
  de `DrawMaskVertex_unk::Set_unk`) tampoco, y hace desaparecer sus copias weak (deshecho).
- `fn_804ABF74` (ctor de `ComDrawTextureMask`): sigue sin salir (solo saldria si vf10 pasa el presupuesto).

### Cambios de layout/firma (para el port)

- `atn/Camera_unk.h`: `void SetPosition(float, float, float); void SetTarget(float, float, float);` (nuevas
  sobrecargas; el port necesita las definiciones de `MiniMapTask.cpp`).

### Decisiones tomadas sin el usuario

- Declarar las dos sobrecargas en `atn/Camera_unk.h` (cabecera compartida) sin nombres de parametro, con la
  definicion en la unidad: sin renumerar ninguna otra unidad.
