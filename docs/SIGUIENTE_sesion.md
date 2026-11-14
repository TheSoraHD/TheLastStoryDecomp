# Siguiente sesion (tras port-decomp-20261114)

> Sesion `port-decomp-20261114` (10 h, hasta 4 agentes, 20 colas A-T, centrada en el port de PC) cerrada el
> 2026-11-14 hacia las 12:30 (inicio 02:23). Mergeada en `main`.

Estado: **15389 -> 15424 / 18109**, lost 0, sin bajadas, rep100 139, labelcheck 49, `main.dol: OK` (build limpio y clon
limpio). Funciones al 0 %: **253 -> 201**. 34 DRAFT (0 en la ruta). Enlazan 143 unidades. Notas: `docs/port-decomp-20261114.md`
y `docs/port-decomp-20261114-q*.md`. Para el port: `claude/port-decomp-20261114.md` (proyecto).

## Siguiente

- **Sincronizacion del port** con esta rama: sustituir las versiones funcionales y los stubs por las de la decomp, quitar
  los `#if TARGET_PC` de la fase 0 y corregir las diferencias de logica encontradas (`fn_8009DE60`, LoadModel,
  `Sequence::fn_80550254`, `fn_8000FAE8`, BattleTask vf10/vf14, `fn_80378D34`...).
- Lotes de cabeceras compartidas (re-emparejar en lote):
  - `push_back` de `std/vector.h` (bloquea 26 copias debiles de Event/*; `fn_800F8C6C`, `MeleeMedian3_unk`): decidir
    copia trivial frente a `__builtin_is_pod` (qL, qT).
  - `HitInfo_unk::m50/m54` como `float` (`game/HitInfo_unk.h`, ~200 unidades), `m0C` como `const wchar_t*`.
  - `CharaStatus::m2CC/m2D0` como `FixedArray` (~200 unidades; `BattleCharaTask::fn_800E9110`).
  - `fn_801F6E78` con el rectangulo por `const atn::D2A_DrawRect&` (`GameUI::fn_803E0E10` sube a 93,5 %).
  - `GameUI::m30C` como `std::vector` (`fn_803E2110`); `std/string.h` (`operator+` friend, accesores `is_long`).
  - Inlines `Model::CalcMatrixStack` en `atn/Model_unk.h` y `Task::vf18` en `Task.h` (367 unidades).
  - `GameCamera` con la `atn::Camera` como miembro (el dtor la destruye con -1).
- `.rodata` de BattleTask en el fuente (`fn_800FDF14`, `lbl_80735DD0`).
- Presupuesto de inline: `GameCamera::fn_80384188` (~32 %), `MiniMapTask::vf10(RenderManager*)`, ctor de LastCocoonImago.
- Deuda de la regla 10: `fn_800F8C6C` (`extern "C"` con `BattleTask*`, ~40 llamadores), vistas locales `MeleeTarget_unk`,
  `LevelSkills_unk`, `Effect_unk` en LastCocoonImago, `AiInput_unk`/`AiOutput_unk` de AI_archer_attack01,
  `OccObj_unk`/`OccRenderNode_unk` de LayerOcclusionCPU, copias locales de `IRender` (VideoTHP, Rain, LinearAllocator,
  TextureComposite, CompositeIntensityTintMulti).
- Siguen bloqueadas: `atn/LH_Motion_cont` (`Get_unk<Load*>`, registros), `atn/BoneCalc_unk` (`cntlzw`),
  `atn/LayerOcclusionCPU fn_800B47AC` (GQR6 sin aprobar), `LastCocoonChrysalis` ctor, `GimmickManager` vf0C.
