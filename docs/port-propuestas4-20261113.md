# port-propuestas4-20261113: arreglos del port de PC que valen para la decomp

Rama desde `main` (`c9b699c`, 15389 / 18109). Salen de la sincronización 1 del port con la decomp
(`TheSoraHD/TheLastStory-PortTest`, rama `sync-decomp-20261113`, notas en `docs/notas-sync1.md` de ese repo).
Cada commit: `ninja` con `main.dol: OK`, `regress.py` lost 0 y `fuzzdrop.py` sin bajadas contra el `report.json`
de `main` (build limpio). **15389 / 18109, sin cambios de cifras**. Sin cambios en `splits.txt`, `symbols.txt`,
`configure.py` ni flags.

## Cambios

1. `include/atn/EffectSystem_unk.h`: `EffectInst_unk::m00`, `EffectSlot_unk::m00` (anterior de la lista) y
   `EffectSystem_unk::m1C` (cabeza de la lista de slots usados) como punteros. `fn_8023793C` escribe punteros
   en ellos (`EffectNode_unk` de `atn/EffectRes_unk.cpp`). En el port de 64 bits, `m1C` como `int` hacía que
   se pisara `mParticleCount`.
2. `atn/D2A_Message.cpp`: `fn_8006EF48` declarada como la define `atn/Font_unk.cpp` (el tamaño antes que el
   texto). `vf18` sube de 92,51 a 92,72 %. **No** se ha cambiado en `atn/Loader.cpp` ni en `atn/NandManager.cpp`:
   con el orden de la definición, `NandManager::fn_800A4244` baja de 100 a 96,95 % y `fn_8046ECDC` de 99,92 a
   96,77 % (el orden de los argumentos cambia el orden en que se evalúan). El port los llama con una sobrecarga.
3. `gmk/SwingObject.cpp`: `fn_800DC6B4` devuelve `unsigned int`, como en `atn/MapModelCache_unk.h`.
4. `#include "macros.h"` en las unidades que usan `MIN`/`MAX` sin incluirlo (les llegaba por `types.h`):
   ItemSlotAnime, ControlAttractHateCharge, DropItem, Em202ForestSpider, Em306GurgBeast, Em401_01_WhiteLeoPard,
   QuadrupedTask.
5. `atn/WordWrap_unk.cpp` `fn_800E0000`: las posiciones como `size_type` (se comparan con `npos`). Mismo código.
6. `MapModelCache_unk` pasa de struct local de `atn/Motion_unk.cpp` a `include/atn/MapModelCache_unk.h` (regla
   10: un tipo por objeto), y `gmk/SwingObject.cpp` llega al modelo por el miembro (`mModel.GetModel_unk()`) en
   vez de `data + 0x10`. `poolcheck.py` sin avisos y `ordercheck.py` sin funciones fuera de orden en Motion_unk.

## Decisiones tomadas sin el usuario

- No se han llevado (quedan para un lote de integración):
  - las vistas `EffectMgr_unk { m00[0xB8]; int mB8; }` de ~95 unidades (`atn::EffectManager::mSystem.mLayer`):
    es deuda de la regla 10 de tamaño de lote;
  - la pareja de texturas de `atn/LH_Effect.cpp` en cabecera (`&pair->m30` en vez de `+ 0x30` en
    `EffectSystem_unk.cpp`), los structs de fichero compartidos con el port (`SceneData_unk.h`,
    `MotionData_unk.h`, oclusión) y la vista `SwingPartInst_unk` de los nodos;
  - los casts a `const char*` en `sprintf` (solo los necesita el `PTR32()` del port).
