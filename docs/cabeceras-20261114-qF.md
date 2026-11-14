# cabeceras-20261114, cola F: `GameCamera` con la `atn::Camera` como miembro

Rama `agent-F`. Inicio 15:18. **15426 -> 15427 / 18109** (contra `$REF`), lost 0, sin bajadas,
labelcheck/rep100 igual, `main.dol: OK`.

## Resultado

- `include/game/GameCamera.h`: `class GameCamera : public atn::Camera` -> `class GameCamera` con
  `atn::Camera mCamera;` como primer miembro (0x000). Layout y tamano iguales. No renumera `@N` (solo cambia un
  miembro de datos y la base; labelcheck/rep100 igual en el build completo).
- `GameCamera::~GameCamera` (0x8037DC68) 99,97 -> **100 %**: el dtor de la camara de 0 sale con -1, como el
  original (miembro, no base). Quitado el NONMATCHING.
- Ninguna otra funcion cambia de porcentaje (fuzzdrop 0, regress lost 0).
- Usuarios adaptados (153 unidades incluyen la cabecera; solo 8 dejaban de compilar):
  - `game/GameCamera_unk`: 226 lineas de los metodos de `GameCamera` (`mPos`, `mTarget`, `mFovy`, `mAspect`,
    `fn_8004B378()`, `SetPosition`, `SetTarget_unk`, `SetUpVector`, `GetPosition_unk`...) -> `mCamera.X`;
    `this` como `atn::Camera*` -> `&mCamera` (`fn_8037F688`, `fn_80383728`, `fn_80385B40`, `fn_80392A04`),
    `mA20 = *this` -> `mA20 = mCamera`.
  - `game/FinishCamera_unk`: `SetCamera_unk(*m368)` -> `SetCamera_unk(m368->mCamera)` (`FinishCamera_unk::fn_803792F0`).
  - `game/GameTask`, `game/BattleCharaTask`, `game/GameEventScript`, `gmk/RandomItem`, `gmk/WindyItem`,
    `game/LastCocoonImago`: `x.mPos` -> `x.mCamera.mPos` etc. sobre un `GameCamera` (`lbl_8087F430->mCamera`,
    `field->mCamera`, referencias locales `GameCamera& camera`); en GameEventScript tambien la copia a
    `EventCamera_unk::m444` y el `fn_80392D2C(&...mCamera.mCamera, ...)`.

## Para el port

- `GameCamera` ya no hereda de `atn::Camera`: la camara de 0x000 es el miembro `mCamera`
  (`GameTask::mCamera.mCamera`). Todo acceso a miembros/metodos de `atn::Camera` sobre un `GameCamera` pasa por
  `.mCamera`, y todo upcast `GameCamera*` -> `atn::Camera*` es `&cam->mCamera`. Funciones cuyo codigo cambio
  (sin cambiar firma):
  - GameCamera_unk: todos los metodos de `GameCamera` que tocaban la camara (ctor, dtor, fn_8037DD00,
    fn_8037E12C, fn_8037EF48, fn_8037F690/6D8/724, fn_8037F9AC, fn_80382A1C, los modos fn_8038xxxx,
    fn_8038AEF0, fn_8038E23C, fn_8038E540, fn_8038E9B0, fn_80390374, fn_803903E0, fn_80390A88, fn_803918EC,
    fn_803920C8, fn_80392510, fn_803928C0, fn_80392964, fn_80392C94/CE0, fn_8039328C...): `git diff` de la rama
    para la lista exacta (solo `X` -> `mCamera.X` y `this` -> `&mCamera`).
  - FinishCamera_unk: `fn_803792F0` llama a `RenderManager::SetCamera_unk(m368->mCamera)`.
  - BattleCharaTask: `camera->mCamera.mRot.y` (dos veces) en `BattleCharaTask::fn_800E4278`.
- Las vistas propias de `GameTask + 0x6C` como `atn::Camera` (LastCocoonImago `m006C`, EnemyQuarkStand `m06C`,
  BattleCharaTask `CameraSub_unk`) siguen igual (ahora coinciden con `mCamera.mCamera`).

## Decisiones tomadas sin el usuario

- Nombre del miembro: `mCamera` (inventado; asi `GameTask::mCamera.mCamera`).

## Pendientes

- `fn_80392D2C` (GameCamera_unk) es `extern "C"` con un `GameCamera*` y las unidades usuarias la declaran con
  `atn::Camera*` (GameEventScript, LastCocoonImago, EnemyQuarkStand): deberia ser un metodo de `GameCamera`
  (deuda de la regla 10, no tocado).
- `ordercheck` de `game/GameEventScript` ya daba `__dt__15GameEventScriptFv` fuera de orden antes de esta cola
  (comprobado con `git stash`): `precommit.sh` falla solo por eso.

## Hallazgos de una linea

- Un objeto en offset 0 destruido con `-1` en el dtor es un miembro, no la base: cambiar la herencia por un
  miembro no cambia el layout ni renumera etiquetas.
