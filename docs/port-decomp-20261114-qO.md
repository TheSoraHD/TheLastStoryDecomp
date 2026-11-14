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

## game/FinishCamera_unk

- `FinishCamera_unk::fn_80378D34` (0x80378D34) 79,9 -> **100 %** (mnemcheck limpio). Firma sin cambios. Claves: el
  rayo va del objetivo al ojo (`from = *target`, antes al reves: no era equivalente); `move`, `center` y `zero`
  declarados fuera del bucle y asignados con `VecAssign_unk` (registros del original); la direccion con
  `(*eye - cur).GetNormalize()`; `PSVECMag(...) / 4.0f` y `(y + y') / 2.0f` (la division por potencia de dos da el
  orden de operandos del original); el barrido recibe un `Vector3(0, 0, 0)` temporal y el original rellena ademas
  un `zero` que no usa; mezcla final con un inline `(b - a) * t + a`.
- `fn_80379104` (99,7 %): probados `rot` fuera del bucle, `VecAssign_unk` en `to`/`b` y en la copia de AddCurve_unk,
  `const Mtx`: no cambian el cruce de registros.

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

- `MiniMapTask::vf0C` (0x804A8504) 1,75 -> **100 %** y `vf10()` (0x804A8644) 0,62 -> **100 %** (escritas desde el asm:
  espera de recursos y arranque del d2anime; marcadores de los personajes con `CharaTask::CanAct_unk` y matriz
  identidad del modelo con su escala maxima). En vf10 hace falta un puntero local a `lbl_807C7060`
  (`const Matrix3x4& mtx`) para que la direccion quede en un registro, y el indice del marcador en un local.
- Constructor (0x804A7F18) 60 -> **100 %**, destructor (0x804A83E4) 55,7 -> **100 %**, `fn_804A88C8` 70,8 -> **100 %**
  (el GXColor se escribe byte a byte en un `GXColor` local y se lee como palabra).
- `MiniMapTask::vf18` (0x804AB060): **escrita** 0,22 -> 98,5 % (copia de la camara del RenderManager, camara del
  minimapa, `m208.vf18`, se restaura). Resto: planificacion de las copias implicitas de `atn::Camera`, como su
  copia weak (98,1 %).
- `vf14` (5368 bytes) sigue sin escribir (no dio tiempo; estructura vista: desvanecido de los materiales
  m_house/m_line/m_road con m1608 segun el rectangulo LOWER_LAYER, OFF_AREA, modos de camara de
  `lbl_8087F0A8->m5A8`, dibujo con DrawHelper y camara m640 con SetPosition/SetTarget).

### Cambios de layout/firma (para el port)

- `game/MiniMapTask.h`: `m58[9]` pasa a **nueve miembros** `m58, m88, mB8, mE8, m118, m148, m178, m1A8, m1D8`
  (el ctor/dtor del original construye/destruye cada uno, sin bucle de array); `mE8` es la mascara de vf10.
  `m15BC` pasa de `atn::Task*` a `ResHandle` (el dtor lo libera; el ctor hace `Load("Minimap")`). Incluye
  `game/ResHandle.h`. `MiniMapTask.cpp` incluye `game/GimmickCommon.h` (`GimmickUvOwner_unk::fn_800D59B8`).
- `atn/Camera_unk.h`: `void SetPosition(float, float, float); void SetTarget(float, float, float);` (nuevas
  sobrecargas; el port necesita las definiciones de `MiniMapTask.cpp`).

### Decisiones tomadas sin el usuario

- Declarar las dos sobrecargas en `atn/Camera_unk.h` (cabecera compartida) sin nombres de parametro, con la
  definicion en la unidad: sin renumerar ninguna otra unidad.

## Hallazgos de una linea

- `x / 4.0f` y `x / 2.0f` dan `fmuls` con los operandos en el orden contrario a `x * 0.25f` (FinishCamera
  fn_80378D34).
- Un `Vector3` con nombre asignado desde un temporal (`v = a + b;`, declarado antes) deja el temporal en la pila y
  lo copia; con `VecAssign_unk(v, ...)` los registros de las direcciones salen como en el original.
- Destructor de `GameCamera` (99,97 %): el original llama al dtor de `atn::Camera` de offset 0 con -1 (miembro), no
  con 0 (base): GameCamera *contiene* una `atn::Camera` en 0 en vez de heredar de ella (cambio de layout grande,
  no hecho; apuntado para el port).
- Un dtor implicito sin cuerpo no reescribe las vtables al empezar (MiniMapTask).

## Resultado

- Funciones al 100 %: 15399 -> **15407** (+8: Camera::SetPosition/SetTarget, MiniMapTask ctor/dtor/vf0C/vf10/
  fn_804A88C8, FinishCamera fn_80378D34). Escritas: GameCamera fn_80385B48 (0 -> 98,4 %), MiniMapTask vf18
  (0,2 -> 98,5 %); MiniMapTask vf10(RenderManager*) comprobada (sigue al 0 %, presupuesto).
- lost 0, sin bajadas de fuzzy, labelcheck/rep100 iguales, PRECOMMIT OK en cada commit.
- Pendientes: GameCamera fn_80384188 (presupuesto), MiniMapTask vf14 (sin escribir) y vf10(RenderManager*)
  (presupuesto), GameCamera fn_8037F9AC / FinishCamera fn_803792F0 sin tocar (pila y registros).
- Etiquetas: tras cada `pd.sh` de estas unidades quedan 1-3 floats de `.sdata2` sin renombrar (0.0, 1.0, 0.5...):
  renombrados a mano comprobando el valor en el `.o` (ver lc.py en el scratch: labelcheck por unidad).
