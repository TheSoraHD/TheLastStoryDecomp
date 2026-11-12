# Siguiente sesion (tras noescritas2-20261110)

> Sesion `noescritas2-20261110` cerrada el 2026-11-12 02:30 (inicio 2026-11-10 23:27; cortada por el entorno el
> 2026-11-11 hacia las 04:50 y reanudada el 12). Rama `noescritas2-20261110` empujada, **sin mergear**.

Estado: **15008 -> 15030 / 18109**, lost 0, sin bajadas de fuzzy, `main.dol: OK` (build limpio y clon limpio).
Funciones al 0 %: **1023 -> 972**. 42 DRAFT (0 en la ruta). Splits y game_units sin cambios. En `symbols.txt`,
ademas de renombrados: una etiqueta duplicada corregida, el byte 0x8087DD38 como `@N` local, `lbl_8074ECE0` partida
en "Rand" + `sValueTypes_unk` + `sFlagOps_unk`, y `sFlagScopes_unk`/`sLookDelays_unk` locales.
Notas: `docs/noescritas2-20261110.md`.

## Revisar y mergear

- Revisar las decisiones de `docs/noescritas2-20261110.md` y mergear la rama en `main`.
- Cambio de firma compartida: `Event::Manager::fn_8047EA34(int id, atn::Vector3 pos, int arg, float rot)`.

## Siguiente

- `game/GameEventScript` (16 al 0 %): fn_80399734 (5 KB, eventos de esferas; comparte con ApproachCharas_unk el
  bloque "examinar" de la camara y CanLook_unk) y el grupo de `boost::lexical_cast<std::string>(int)` (Boost
  1.34.1; necesita istream/iostream/stringstream de MSL en `include/std/`), mejor como lote propio.
- `game/MapPartsGimmickLoc`: dtor y vf0C/vf10 piden una cabecera comun de `SwingPart_unk`.
- Restos: `game/PopupTask` vf14 (9 KB), `RecycleBuffer_gmk/ItemSlotAnime` fn_80450B84/fn_804515A8,
  `atn/LayerOcclusionCPU` fn_800B5994.
