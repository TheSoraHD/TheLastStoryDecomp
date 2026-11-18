# Siguiente sesion (tras enlazar2-20261117)

> Sesion `enlazar2-20261117` (hasta 4 agentes, 31 colas) del 2026-11-17 15:28 al 2026-11-18 ~01:45 (cierre adelantado por
> el usuario). Notas: `docs/enlazar2-20261117.md` y `docs/enlazar2-20261117-q*.md`. Para el port:
> `claude/port-enlazar2-20261117.md` (proyecto).

Estado: **16074 / 18109**, lost 0, rep100 23, labelcheck 11 lineas, `main.dol: OK`. Codigo 62,39 %, datos 76,18 %,
742 unidades completas. DRAFT 0. Funciones al 0 %: 45 (20 sin iostream/auto_03). PacketPack 57 funciones < 100 %.

## Decisiones pendientes del usuario

- Parche de push_back `docs/enlazar2-20261117-qK.patch`: arregla el cruce tamano/capacidad del push_back inline (~73 sitios)
  pero rompe las copias debiles fuera de linea; medida orientativa +8 al 100 %, ~12 bajadas y Event/Sequence fn_8054204C
  97,8 -> 10,3. Bloquea ArrowSet, ShopColor, FileList, Np131, Em305VampireMp, FriendRoom, AreaEffConfig, ChaserNpc...
- `block_relocations` de OSInit en `config.yml` (`docs/enlazar2-20261117-qI-config.patch`), como las de OSFatal.
- "Para revisar" de `docs/enlazar2-20261117.md`: `int& cur = m84;` (EnvSeManager), `float zero` (Em208Queen),
  `FindPlace_unk(id = ...)` (Em310Gargoyle), `GXWGFifo` directo (EffectDesc_unk), `GetSaveFileName_unk` (GameSaveLoadCommon),
  alias `self` en `std::string::operator=`, dos funciones `const` sin 100 % (C2).

## Siguiente

- A una funcion de enlazar (bloqueos de registros, ver notas de R/N2): EnvSeManager vf14, Em204MulueMp fn_802B55B0,
  ControlChaseCaught Follow vf0C 99,97, PostFxMotionBlur vf10 99,9, Em309GhoulSimple, Em209SmallGolem, Em303KrakenFootMp,
  CannonLift, ShaderManager (push_back), las LH_* (`Loader::Get_unk<T>`).
- Patron `m97C.Set_unk` (~20 funciones de enemigos, cola T): el original no sustituye el `this` de la inline.
- GameEventScript: familia `SetNext_unk` (21 comandos, r0/r4 cruzados), y fn_803A2378/2210/7D94/98EC sin probar.
- Truco `const` (P3/C2): seguir con candidatas que releen campos tras una llamada; parches de C2 (ModelDraw_unk, ReplayBuffer).
- PacketPack: pendientes de P2/P3 (fn_804F8464, fn_804EAA84, fn_804EE4F0, destructores con `erase(begin(), end)`).
- Regla 10 / port: ~60 vistas locales de `GameTask` (`Field_unk` y similares) con el accesor debil `fn_8013C504__9Field_unkFv`
  declarado en 21 unidades (pasarlas todas a la vez a `GameTask::fn_8013C504()`); TownTask (`m553C` dentro de `GameTask::m550C`);
  `atn::AudioDevice` sin cabecera (propuesta 5c del port); `EffectMgr_unk::mB8` (28 unidades de gmk), los 47 `Settings_unk`,
  `ActionOwner_unk` = `Event::Section`, `PathData_unk` = `Event::Path`, `MenuD2A_unk` = `D2A_Task`, structs de fichero
  compartidos (`SceneData_unk.h`, `MotionData_unk.h`, `ModelOcclusion_unk.h`).
- Re-emparejar tras merges en conflicto: `tools/decomp/remap.py <copia_agente> <unidades>` (por posicion con el .o del agente),
  `lcfix.py`, `lblremap.py`.
