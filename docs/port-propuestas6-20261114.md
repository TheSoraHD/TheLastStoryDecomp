# port-propuestas6-20261114: arreglos del port de PC (fase 4) que valen para la decomp

Rama desde `main` (`e992ba3`, 15424 / 18109). Salen de la fase 4 del port (`TheSoraHD/TheLastStory-PortTest`,
rama `fase4-20261114`, notas en `docs/port-fase4-notas.md` de ese repo). Comprobado con build completo:
`main.dol: OK`, `regress.py` lost 0 y `fuzzdrop.py` sin bajadas contra el `report.json` de `main`.

## Cambios

1. **`RenderManager::fn_800C06B0`** (`atn/RenderSystem_unk.cpp`, niebla): miembros (`m890`…`m8AC`,
   `mCamera.mNear/mFar`, `FogSettings`) en vez de offsets a mano. El fuzzy no cambia (98,10 %) si el color se
   copia como `int` (`*(int*)&m8A0 = *(const int*)&fog->mColor.r`, como el original); con copias `float` baja a
   93,49. En PC los offsets `0x8A4/0x8A8` caían sobre `m74C.mImage` (copia del framebuffer de los post-efectos):
   pantalla blanca después de NEW GAME.
2. **`AIString_unk`** (`AI/AI_archer_attack01.cpp`): las dos palabras de la vista de `std::string` pasan a
   `__msl_size_t` (el mismo tipo en la Wii), como el resto de vistas de `std::string`.
3. **`CharaTask.cpp`**: incluye `chr/ControlCannonLift.h` en vez de las vistas locales de
   `ControlCannonTarget`, `ControlCannonLift` y `ControlCannonThrow` (regla 10). Mismo código. En PC las vistas
   tenían el tamaño de la Wii y `new` reservaba menos de lo que escriben los constructores.
4. **`Event/Path.cpp`**: `Settings_unk::fn_80116BAC` devuelve `&GameConfig_unk::mInput` en vez de leer la vista
   en `0x48C`. Mismo código.

## Lo que no se ha subido (y por qué)

- `PTR32` en `ModelVtx_unk.h`/`ModelPose_unk.h`: la decomp no tiene `port_types.h`.
- `ActionOwner_unk` = `Event::Section`, `PathData_unk` = `Event::Path`, `MenuD2A_unk` = `D2A_Task`, las vistas
  de `AiCommon_unk`: cambian más código; sin probar el matching.
- Los campos de bits de `Event::Sequence::m98` en orden inverso y la conversión de los `.sc`: son solo de PC.
