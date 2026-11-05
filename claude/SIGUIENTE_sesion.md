# Siguiente sesión: propuestas de trabajo

## Estado actual (2026-11-05, rama `maptask-20261105` mergeada en `main`)

- `maptask-20261105` (desde `main` 7a5c73f): **12710 / 18119** en `report.json` (lost 0, fuzzdrop 0, `main.dol: OK`;
  labelcheck sin empeorar, rep100 identico, ordercheck/poolcheck/mnemcheck limpios; build limpio). Notas en
  `docs/maptask-20261105.md`.
  - MapTask 20/27 al 100%; escritas todas las demas (vf0C 97,4, fn_8049E480 98,6, fn_8049ED90 95,5, CreateParts 99,8,
    fn_804A06B4 99,9, fn_804A0BCC 99,8, fn_804A0F80 5% por presupuesto de inline).
  - `ResRef_unk` unificada en `include/atn/ResRef_unk.h` (4 bytes); accesores de MapTask con nombres del RSO en
    GimmickStateCommand/Cannonball; `GameCamera` (GameTask + 0x6C); `DofParam_unk`; `atn/Lerp_unk.h`.
  - MapPartsArea: vf1C 99,90% (menu de depuracion), ParseScene escrita (34%, pool de cadenas en orden);
    `Texture_unk::fn_800D5908` miembro; `atn/SceneAddLight_unk.h`; `CharaLightData_unk::mPos`.

## Propuestas

1. MapPartsArea: fn_80494354 (94,2%) y vf14 (95,8%) con el pool ya en orden; vf1C 99,90 (indice de los bucles
   internos sumado desplazamiento+base); ParseScene por encima del presupuesto de inline.
   Decidir si se aprueba `explicit_zero_data` para `lbl_8087E0D8` de MapTask (0 en `.sdata`).
2. `atn::PostFxManager` y las clases PostFx* en cabeceras (definiciones duplicadas en atn/LinearAllocator.cpp y en
   cada PostFx*.cpp; vistas en Event/Dialog, atn/PostFx.h, LayerShadow, LayerPostFx, Layer2DBlur, game/main):
   desbloquea la ultima instruccion de MapTask::fn_8049ED90.
3. Mindows: unificar las ~100 declaraciones `void*` de `fn_800874C8`/`fn_8008771C`/`fn_8008937C`... con
   `atn/MindowsCommon.h`; `ResHandle` es una carpeta del menu de depuracion (romper eso hoy rompe ~95 unidades).
4. `atn::ModelInstance` en cuatro sitios (Lemon.h, NewCampPartsBase.h, MapLift.cpp, ModelInstance.cpp) e
   `InstModel_unk`: unificar (desbloquea las vistas de partes en gmk).
5. Enlazar `atn/Scene` (19/23); `LightSet_unk` frente a `Light_unk.h`; copias locales de `atn::IRender`.
6. `EventConfig_unk` (Event/Manager) m88..mB4 es un `DofParam_unk`; `RenderSetting_unk` es `atn::RenderConfig` (RSO).

## Bloqueos conocidos (no repetir sin idea nueva)

- `push_back<boost::shared_ptr>` y el crecimiento de `vector` no trivial (también los push_back de MapPartsBase).
- Registros del contador de frames y del corte en fn_8048871C/88B5C; registros del gamma en fn_8048E9E0.
- Matriz de reflejo de `atn::Scene` (fn_800C2C20/fn_800C2EE0).
- MapManager fn_80491D50/DF8 (78%): el original carga la esfera antes de guardar r31.
- Otros pendientes de antes: ut_TextWriterBase (objeto débil de `.sbss`), Gecko_ExceptionPPC (`extab`), DWC/NET
  (registros); RenderManager `GetKeyLight` = `Event::GetViewLight_unk`?
- Cadena y longitud del `std::string(const char*)` intercambiadas cuando el `const char*` sale de una llamada
  (MapTask fn_804A06B4/0BCC, constructor de MapPartsLocatorSet).
- MapTask::fn_804A0F80: el original inlinea ~25 lerps de colores/vectores y accesores; el nuestro queda por encima
  del presupuesto (R3 de docs/inlining.md) y llama a todo.
- `pdbatch.sh` renombra `lbl_8087D6E8/EC` (`@STRING@Resize_unk...`), `lbl_80756700` (`__files`), `lbl_8087EE78`
  (`sInstance`), a veces `0x8087DB00/04`, `__vt__` sobre cadenas: devolverlos a mano. Las `.sdata2` ambiguas (pairdata
  las salta) se pueden emparejar por valor y desplazamiento (`tools/decomp/sdata2pair.py <unidad>`).
- Cambiar una cabecera muy incluida renumera las `@N` de muchas unidades: `pdbatch.sh` con la lista de
  `labelcheck`; anadir funciones inline a `math/Vec.h` o `atn/Color.h` renumera casi todas (por eso `atn/Lerp_unk.h`).
- `fnstat.py` frente a `report.json` no es comparable en unidades con entradas en `rep100` (CharaTask).
- `ninja build/SLSP01/main.elf` no recompila las unidades no enlazadas: para `labelcheck`/`rep100` hace falta `ninja`.
- `configure.py` sin flags intenta descargar los compiladores: usar la línea de `docs/entorno-nube.md`.
- El volcado de símbolos del RSO USA está en `raw.githubusercontent.com/Aelbannan/last-story-decomp/main/docs/lastworld_tools_symbols.txt`.
