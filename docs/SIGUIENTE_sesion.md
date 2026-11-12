# Siguiente sesion (tras paralelo-20261112)

> Sesion `paralelo-20261112` (8 h, lote en paralelo con 4 agentes) cerrada el 2026-11-12 hacia las 23:45
> (inicio 14:47; reinicio del servidor 18:33-20:16). Rama `paralelo-20261112` **sin mergear**: pendiente de revision.

Estado: **15057 -> 15227 / 18109**, lost 0, sin bajadas de fuzzy, rep100 136 -> 135, `main.dol: OK` (build limpio y
clon limpio). Funciones al 0 %: **870 -> 546**. 42 DRAFT (0 en la ruta). Ninguna unidad nueva enlaza. Split nuevo
`atn/MotionBank_unk` (de Motion_unk en 0x800A0A68, aprobado al cierre); `symbols.txt` solo renombrados y `scope:local` de etiquetas. Notas:
`docs/paralelo-20261112.md` (resumen, decisiones e integraciones) y `docs/paralelo-20261112-q*.md` (una por cola).

## Revision

- Deuda de la regla 10 en `game/BattleTask`: nueve funciones nuevas `extern "C"` con `BattleTask*` primero (80
  llamadores de `fn_800F8548` en otras unidades). Pasarlas a miembros en un lote de integracion, junto con sus
  llamadores.
- Parche no aplicado `docs/paralelo-20261112-MslSort_unk-fix.patch` (`game/MslSort_unk.h`): sube los sorts de
  ShopBase y NewCampPartsItem al 100 %, pero cuatro sorts de CosEditItemList_unk bajan de 2,89 % a 0 %.
- Split `atn/MotionBank_unk` aplicado al cierre (ver "Cambios de splits" en las notas).
- Firma de `CharaTask::vfBC()`: el de LastCocoonImago (0x80459FB8) recibe un argumento.

## Siguiente

- Lote de integracion de cabeceras (re-emparejar `@N` de todas las unidades a la vez): `fn_8008B964` inline en
  `RenderManager.h` (117 unidades; sube los llamadores de la vibracion de GameCamera), `fn_8038E23C` en
  `GameCamera.h` (144), metodos de `GameUI.h` (35, hoy `extern "C"` desde GameTask), metodos libres de EffectSystem.
- `atn/EffectManager` (29 al 0 %): reconstruir `num_put`/`numpunct`/`basic_ostream` de MSL para `fn_8023BA10`.
- `game/CosEditItemList_unk` (37), `game/BattleTask` (51), `game/GameUI` (22), `Event/Sequence` (19),
  `Event/Puppeteer_CharaTask` (19), `game/ItemManager_unk` (19), `game/MapPartsGimmickLoc` (15), `LastCocoonEgg` (14),
  `LastCocoonImago` (12), `GameEventScript` (12), `AiTarget` (11).
- `atn/EffectSystem_unk` solo le falta `fn_8022DD1C` (7,5 KB) para tener todo escrito.
- Siguen bloqueadas: `atn/LH_Motion_cont`, `LastCocoonChrysalis` ctor, `GimmickManager` vf0C, `atn/BoneCalc_unk`.
