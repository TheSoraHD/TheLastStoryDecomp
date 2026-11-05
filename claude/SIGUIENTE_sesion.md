# Siguiente sesión: propuestas de trabajo

## Estado actual (2026-11-05, rama `maparea-20261105` sin mergear)

- `maparea-20261105` (desde `main` ff386cd): **12698 / 18119** en `report.json` (lost 0, fuzzdrop 0, `main.dol: OK` en compilación limpia y en un
  clon limpio de la rama; labelcheck sin empeorar, rep100 idéntico, ordercheck/poolcheck/mnemcheck limpios).
  Notas en `docs/maparea-20261105.md`.
  - Tipos únicos en cabeceras: `MapArea` (`include/game/MapArea.h`, 0x314, `atn::Scene` como **miembro** en 0x4C),
    `MapPartsArea` (`include/game/MapPartsArea.h`, 0x70, `vector<MapArea*>` en 0x58), `MapTask` completa
    (`include/game/MapTask.h`, 0x11C alineada a 8), `MapManager` con `static_vector<MapTask*,64>` y la lista de áreas.
  - Fuera: `MapModelBase_unk`, `MapGroup_unk`, `MapPart_unk`, `MapObject_unk`, `MapPartsAreaInit_unk`, `MapSegment_unk`,
    `MapAreaBounds_unk` (era `MapPartsRain`), `GameMap_unk`, `MapPartsBox_unk` (= `atn::AABB_unk`) y ~30 vistas
    locales de MapTask (`MapInfo_unk`...) y de TownTask (`lbl_8087FA20`).
  - `MapPartsBase(atn::Task*, int type)`; nombres del RSO: `SetTask` (MapManager, MapTask, MapPartsArea,
    MapPartsLocatorSet, MapPartsGimmickLoc), `FindArea`, `instance_`, `GetMapNameFromId`, `RenderManager::GetScene`.

## Vistas que quedan (con motivo en `docs/maparea-20261105.md`, "Pendientes")

- Partes en gmk (PassPart_unk, OnOffPart_unk, RotaryPart_unk, ArenaPart_unk, LiftMapPart_unk): esperan a unificar
  `atn::ModelInstance` (propuesta 3).
- `extern "C"` accesores de MapTask en GimmickStateCommand (fn_803EFD9C/A4/AC, presupuesto de inline).

## Propuestas

1. **`atn::AABBTree` en una cabecera** (con `IRender` de atn/IRender.h, propuesta 5): desbloquea constructores y
   destructor de MapTask (0x8049D7E4, 0x8049D974, 0x8049DDD8), `SetTask` y `fn_8049D704`.
2. **MapPartsBase**: ParseScene (7880 bytes, estructura ya analizada en las notas) y vf1C; con ellas el pool
   `@stringBase0` queda en orden y fn_80494354 (94%) puede subir. ParseScene en sí no llegará al 100%
   (`AddLight_unk` inlinea el crecimiento de `vector`). `fn_800DC288` devuelve `double`, no `float`.
3. Enlazar `atn/Scene` (19/23): constructor 90%, `fn_800C2C20` 67% / `fn_800C2EE0` 72%, `fn_800C258C` 99,5%.
4. `atn::ModelInstance` definida en cuatro sitios (Lemon.h, NewCampPartsBase.h, MapLift.cpp, ModelInstance.cpp) e
   `InstModel_unk`: unificar (desbloquea las vistas de partes).
5. `LightSet_unk` de ModelDraw_unk frente a `Light_unk.h`.
6. Copias locales de `atn::IRender`; `atn::Frustum`/`atn::Camera`; RenderManager (`GetKeyLight` =
   `Event::GetViewLight_unk`?).
7. ut_TextWriterBase (objeto débil de `.sbss`), Gecko_ExceptionPPC (`extab`), DWC/NET (registros).

## Bloqueos conocidos (no repetir sin idea nueva)

- `push_back<boost::shared_ptr>` y el crecimiento de `vector` no trivial (también los push_back de MapPartsBase).
- Registros del contador de frames y del corte en fn_8048871C/88B5C; registros del gamma en fn_8048E9E0.
- Matriz de reflejo de `atn::Scene` (fn_800C2C20/fn_800C2EE0).
- MapManager fn_80491D50/DF8 (78%): el original carga la esfera antes de guardar r31.
- `pdbatch.sh` renombra `lbl_8087D6E8/EC`, `lbl_80756700` (`__files`), `lbl_8087EE78` (`sInstance`), a veces
  `0x8087DB00/04`, `__vt__` sobre cadenas (`lbl_8078CB00/08/10`, `lbl_80756EC8`): devolverlos a mano. Puede dejar
  `@N` inexistentes en el `.o` (GameUI 0x80885E40...): devolverlas a `lbl_` sin `scope:local`.
- Cambiar una cabecera muy incluida renumera las `@N` de muchas unidades: `pdbatch.sh` con la lista de
  `labelcheck`/`rep100` (repetir tras **cada** cambio de cabecera).
- `fnstat.py` frente a `report.json` no es comparable en unidades con entradas en `rep100` (CharaTask): comparar con
  un worktree de `main`.
- `ninja build/SLSP01/main.elf` no recompila las unidades no enlazadas: para `labelcheck`/`rep100` hace falta `ninja`.
- `configure.py` sin flags intenta descargar los compiladores: usar la línea de `docs/entorno-nube.md`.
- El volcado de símbolos del RSO USA está en `raw.githubusercontent.com/Aelbannan/last-story-decomp/main/docs/lastworld_tools_symbols.txt`.
