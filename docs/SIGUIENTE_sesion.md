# Siguiente sesion (tras secuencial-20261119)

> Sesion `secuencial-20261119` (sin agentes) del 2026-11-19 02:12 a 14:12. Notas: `docs/secuencial-20261119.md`; para el
> port: `docs/secuencial-20261119-port.md` y `claude/port-secuencial-20261119.md` (proyecto).

Estado: **16075 / 18109**, lost 0, rep100 23, labelcheck 11 lineas, `main.dol: OK`. Codigo 62,40 %, datos 76,18 %,
742 unidades completas. DRAFT 0. Funciones al 0 %: 45 (20 sin iostream/auto_03).

## Decisiones pendientes del usuario

- Ninguna nueva. D1 (OSInit) aplicada; D2 (parche qK de push_back) revertida con la medida en `docs/secuencial-20261119.md`
  (neto -14; diff en `docs/secuencial-20261119-D2.patch`).

## Siguiente (port de PC)

- Regla 10, `extern "C"` con el objeto como primer argumento que quedan (no empezadas por tamano):
  `atn::AudioDevice` (35, clase en el .cpp con ~700 lineas de tipos locales), `atn::Font_unk` (12, ~40 llamadores),
  `atn::TextAnimation` (6; Event/Dialog, Event/Manager y ActionEffectPlay la ven como `DialogTextMgr_unk`),
  `atn::EffectManager` (5) y `atn::MindowsCommon` (3).
- GameTask: `fn_8036DBC0`, `fn_8036DDD4`, `fn_8036E624`, `fn_8036E6D4`, `fn_8036FE74` como miembros cambian registros y la
  pila de GameEventScript (ver `docs/mwcc-blockers.md`).
- Punteros guardados en `int` que quedan (bugs de 64 bits): `CharaTask::mF1C` (arrastra los constructores de
  ControlCaught/ControlMatHold/ControlThrownAway/ControlThrownAvoid), `Em303KrakenFoot::m1504` (`CharaTask::fn_8016C4D8(int, ...)`
  -> `ControlThrustForKraken`), `TacView::m54.mType` (`(int)chara`), `atn/DrawHelper` `m1C`, y el `owner` `unsigned int` de
  `atn::EffectManager`/`EffectSystem` (pide `uintptr_t` o `void*` en toda la familia, ~50 unidades).
- Vistas locales de globales que quedan (`/home/claude/scratch/gviews.py` en la sesion; ~70): `MpSession_unk` de
  lbl_8087F610 (20, MP), `RenderSetting_unk` de Dialog/CosEditBase/EnemyJill/Em302Kraken/NewCampPartsGmk (copias de structs
  enteros), tablas de texto de lbl_8087F1E4 que leen pares con cast (9), `TacView_unk` de ControlCharge (constructor propio
  del cursor), `CrossbowPlayer_unk` (0xAA4/0xAAC de CharaTask sin documentar), GameUI de MenuCommonTask (`m288C` como
  `float[4]`) y de Em308InvertCrab (pila de vf10 con `GameUIElem1C_unk`).
- Firmas: `tools/decomp/sigcheck.py` lista 77 funciones con declaraciones distintas; las que quedan cambian codigo al
  unificarlas (ver `docs/secuencial-20261119-sigcheck.txt`).

## Siguiente (decomp)

- A una funcion de enlazar (bloqueos de registros): EnvSeManager vf14, Em204MulueMp fn_802B55B0, ControlChaseCaught Follow
  vf0C, PostFxMotionBlur vf10, Em309GhoulSimple, Em209SmallGolem, Em303KrakenFootMp, CannonLift, ShaderManager, las LH_*.
- GameEventScript: familia `SetNext_unk` (21 comandos, r0/r4 cruzados; 10 variantes probadas en esta sesion sin exito).
- Patron `m97C.Set_unk` (~20 funciones de enemigos).

## Herramientas

- Re-emparejar `@N` tras cambios de tipos o cabeceras: `MAINREF=<build limpio de main> python3 tools/decomp/refmap.py <unidades>`
  (dos pasadas; las unidades sin `.o` en la referencia, a mano por direccion, como gmk/Trap). Despues restaurar `@STRING@` y las
  etiquetas vigiladas, `configure` y `labelcheck.py`.
- Cualquier inline nueva, parametro con nombre o declaracion adelantada en una cabecera muy incluida (`GameUI.h`,
  `RenderSetting_unk.h`, `ChaseManager.h`, `GameTask.h`) renumera los `@N` de todas sus unidades; una `static inline` del
  propio .cpp no (truco de Layer2DBlur/TownTpcLoader).
