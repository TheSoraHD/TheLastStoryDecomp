# port-propuestas-20261112: cambios del port de PC que valen para la decompilacion

Sesion del 2026-11-12. Se llevan a la decompilacion los arreglos encontrados en el port de PC
(`TheSoraHD/TheLastStory-PortTest`, rama `fase1-20261112`; guia en `claude/ports-guia.md` del proyecto)
que generan el mismo codigo con MWCC. Cifras sin cambios: **15030 / 18109**, `main.dol: OK`, lost 0, sin
bajadas de fuzzy, labelcheck/rep100 iguales a la referencia (`main` f23abce).

## Commits

1. **Palabras de 32 bits como `unsigned int`**: `*(unsigned long*)0x800000F8` (reloj del bus; HeapInfo_unk,
   NetManager, LH_Handle, AudioDevice, PacketPack, NetFriend_unk, GameTask) y el bitmap de `SmallAllocator`.
   En MWCC `long` e `int` son de 32 bits; en 64 bits el bitmap de `unsigned long` pisaba bloques.
2. **Un tipo por objeto (regla 10)**, vistas encontradas por `port/tools/views.py` del port (compila con el
   layout de la Wii y con el de 64 bits y compara el DWARF):
   - `HomeButtonMenu`: `game/HomeButtonMenu.h` en main, SystemErrorMessage_unk, StrapTask y TutorialTask.
   - `atn::Light` y `SceneTrack_unk`: `atn/Light_unk.h` en LH_Scene.
   - `ModelData_unk`/`ModelPose_unk`/`ModelJoint_unk`: `atn/ModelPose_unk.h` en LH_Model (la cabecera
     recibe los nodos `m3C`/`m40` y `ModelJoint_unk::m70`).
3. **main y GameConfig_unk**: fuera la vista `MainConfig_unk` y los `extern "C"` con `self`; ahora miembros
   de `GameConfig_unk`. `fn_80187C44` recibe la ruta de `config.ini` (main la carga en r4 en 0x8047AA30; el
   cuerpo no la lee): renombrado `fn_80187C44__14GameConfig_unkFPCc`.
4. **SoundManager**: callbacks del menu de depuracion como `long` (`.o` identico byte a byte).

## Decisiones tomadas sin el usuario

- Cambiar `GameConfig_unk.h` renumera las etiquetas `@N` de sus 26 usuarios. `pdbatch.sh` no coloco
  todas: MapPartsBase por offset; DebugInfo_unk, GameEventScript y GameCamera_unk desplazadas en +1 y
  comprobadas contra los valores de `main.dol`; dos `lbl_` de un tramo `auto_` (`lbl_8087D6E8`/`EC`)
  que `pdbatch` habia renombrado con un nombre de otra unidad (enlace con simbolo duplicado), devueltos
  a su nombre. Hallazgo para `pdbatch.sh`: no debe renombrar dentro de tramos `auto_`.
- StrapTask: `@2444`/`@2445` a mano (pd.sh no los coloco; nombres `@N` repetidos entre unidades).

## Pendiente (no hecho en esta rama)

- Vistas de sonido: `atn::AudioDevice` (AudioDevice vs Sound3dEngine), `atn::SoundCore` (Bgm vs
  Sound3dEngine), `nw4r::snd::Sound3DManager` y `DvdSoundArchive`. Cada fichero repite a su manera
  declaraciones minimas de NW4R (`SoundCore.h`, `Sound3dEngine.cpp`, `AudioDevice.cpp`); primero habria
  que juntarlas en una cabecera.
- Datos de RenderSystem_unk sin definir: `.sdata` 0x8087D8F8-0x8087D918 (`{0, 255}` del clamp, -16777215.0f,
  1.0f). El primero vale 0 y esta en `.sdata`: seguramente un par `{0, 255}` de 8 bytes, lo que cambia
  tamanos en `symbols.txt` (regla 3).
- Punteros en enteros que cambian nombres de enlazado (miembros): `RenderManager::m52C`,
  `GameLoadBackground::fn_803B3530` (arg1 = buffer), `EffectManager::NotifyStop_unk`, DrawHelper `m1C`.
- `fn_80084EE8__Q23atn11DLAllocatorFPvUl` como `extern "C"` (SmallAllocator_8, DwcAllocator_unk): se
  queda; el original pasa el nombre del heap en r6 y el simbolo solo tiene dos parametros.
- Descartado: `#include "macros.h"` en las 7 unidades que usan MIN/MAX (en MWCC les llega por las
  cabeceras; el fallo era del build del port).
- Vistas que quedan en `views.txt` del port: ver la lista del port tras cada lote.
