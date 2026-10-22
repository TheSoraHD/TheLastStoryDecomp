# Lote batch-auto-20kb-atn (2026-10-21/22): resumen y decisiones a revisar

Propuesta B de `SIGUIENTE_sesion.md`: las 20 unidades `atn/` partidas sin fuente de 10-20 KB (`targets.py --max-unit 20480 --limit 1000`, solo `atn/` y > 10240), 638 funciones objetivo. 10 colas por tramos de direcciones contiguos, de menor a mayor, con 4 agentes a la vez; cada copia nueva se creo desde la rama de integracion ya actualizada (qA5 tras qA1 para reutilizar las cabeceras de colision, qA10 tras qA3). Notas completas de cada cola (resultado por unidad, NONMATCHING con motivo, decisiones y hallazgos MWCC) en `docs/batch-auto-20kb-atn-qA{1..10}.md`.

Permisos del usuario para este lote: ampliar o cortar splits de esas unidades, renombrar en `symbols.txt` y hacer push de la rama (no de `main`). Excepciones a la regla 2: solo las ya aprobadas (ninguna cola uso asm ni pragma).

| Cola | Unidades (tramo) |
|---|---|
| qA1 | `atn_colli`, `ColliCalc_unk` (+ nueva `Csv_unk`) (0x80056DB8-0x8005C248) |
| qA2 | `LayerShadow`, `RenderSetting_unk` (0x800B6BEC-0x800BC468) |
| qA3 | `Loader`, `SoftKeyboardU` |
| qA4 | `ModelDraw_unk`, `Motion_unk`, `NandManager` (0x8009B93C-0x800A5508) |
| qA5 | `ColliBaseFunc_unk`, `AudioDevice` (0x80046220-0x8004F45C) |
| qA6 | `Sound3dListener_cont` (fusionada), `Task` (+ nueva `TaskBase`), `TextAnimation` |
| qA7 | `D2A_Common`, `D2A_TaskInstanceBase_64_8` |
| qA8 | `Font_unk`, `MindowsCommon` |
| qA9 | `SequenceUnitParam1_const_int` |
| qA10 | `NetManager` |

## Resultado

- Build limpio (`rm -rf build/SLSP01`) con `main.dol: OK`.
- `regress.py` **lost: 0** contra `main` b015150, contra la referencia de integracion de cada cola y contra el `report.json` final de cada una de las 10 colas.
- **0 bajadas de fuzzy** (umbral 0,005) contra `main` y contra cada cola (`tools/decomp/fuzzdrop.py`, nuevo).
- Funciones al 100%: `main` b015150 **5961 -> 6475 (+514)**. Codigo 19,46% -> 21,31%; fuzzy 30,94% -> 33,99%; datos 16,87% -> 17,05%; ficheros enlazados 341 -> 342 (+`atn/D2A_Common`; `atn/Sound3dListener` sigue enlazada con su `_cont` dentro). Unidades totales 1323 -> 1318 (fusiones de `_cont`/`auto_`).
- De las 20 unidades (+2 nuevas al partir: `atn/Csv_unk`, `atn/TaskBase`): todas con fuente; **520/680** funciones de esas unidades al 100% (0,81 funciones al 100% por funcion objetivo).

| Unidad | 100% | Unidad | 100% |
|---|---|---|---|
| atn_colli | 44/51 | AudioDevice | 42/56 |
| ColliCalc_unk | 12/15 | Sound3dListener (+_cont) | 37/37 enlazada |
| Csv_unk (nueva) | 10/15 | Task | 11/25 |
| LayerShadow | 2/7 | TaskBase (nueva) | 3/4 |
| RenderSetting_unk | 14/15 | TextAnimation | 14/29 |
| Loader | 18/31 | D2A_Common | 13/13 enlazada |
| SoftKeyboardU | 27/46 | D2A_TaskInstanceBase_64_8 | 47/51 |
| ModelDraw_unk | 1/14 | Font_unk | 12/23 |
| Motion_unk | 11/30 | MindowsCommon | 50/57 |
| NandManager | 32/34 | SequenceUnitParam1_const_int | 34/37 |
| ColliBaseFunc_unk | 15/17 | NetManager | 71/73 |

Casi enlazables (solo registros): `NetManager` 71/73, `NandManager` 32/34, `RenderSetting_unk` 14/15, `SequenceUnitParam1_const_int` 34/37 (push_back), `ColliBaseFunc_unk` 15/17, `TaskBase` 3/4.

## Arreglos de integracion

- `include/atn/Task.h` (qA6 vs qA7): qA7 tipo `m24` como `Task*` con relleno; qA6 reconstruyo el layout entero (arbol de tareas, `TaskBase` con `mTag`/`mId` de 64 bits). Se queda el de qA6 (mantiene `Task* m24`). Sin cambios de codigo.
- Resto de merges sin conflictos de texto; `game_units.json` y `symbols.txt` fusionaron solos.
- Aviso preexistente (tambien en `main`): `Alignment for std/runtime_error.cpp .rodata expected 8`.

## Decisiones a revisar

**Reglas y patrones fuera de la guia:**
1. **Pendiente de aprobacion:** `atn/ModelDraw_unk fn_8009BF3C` (35,9%) escala colores con paired-single a mano (GQR6, `psq_l`/`psq_st` cuantizados), como `LH_Binary`: no se puede igualar sin asm (qA4).
2. Codigo que no esta en el DOL (solo para el orden de pools; lo elimina el enlazador): `NandManager::Stripped1_unk` (cadenas "atn_nand.cpp" / "[NAND] Couldn't open a file.", qA4), `atn::net::Manager::Stripped1_unk` (byte `""` del pool, qA10), instanciacion de `SetFloat_unk` para su `L"%f"` (qA7), ctor de `Mindows_unk` fuera de la clase (copia sin llamadores, qA8).
3. `((void)0)` (asserts de release, excepcion aceptada) en cantidades grandes: **16 en cada una de** `NetManager fn_8050AA6C`/`fn_8050AA98` (qA10, pide confirmacion), 10 en `Motion_unk fn_800A05AC` (qA4), 6 en el ctor de `Font_unk` (qA8), 4 en `AudioDevice fn_80047B54` (qA5).
4. Macros para el orden de literales/constantes: `TmpPath_unk`, `SET_MAT_COLOR_UNK` (qA4); `TASK_U64` en `Task.h` (los 64 bits de `TaskBase` se guardan como dos `unsigned long`: un `long long` alineaba la clase a 8 y movia miembros de `CharaTask`/`ControlCannonLift`) (qA6).
5. Literales con `"\0"` al final para reproducir `""` sin fusionar del pool de `AudioDevice` (qA5).
6. `include/atn/D2A_Data.h` (qA7): `D2A_Track_unk` tiene un array de longitud cero seguido de un miembro (`mBuf[0]` + `mValue` en 0x0C); provoca el warning de MWCC "no members allowed after a flexible array" en las unidades que lo incluyen (no cambia el codigo). Conviene limpiarlo (union o accesor).
7. `AudioDevice::m04` `volatile` (el original lo relee tras cada comprobacion) (qA5).

**Splits cambiados:**
- `atn/atn_colli` termina en 0x80059660 (`fn_80059660` va tras los dtors debiles y la llama ColliCalc); `atn/ColliCalc_unk` 0x80059660-0x8005B3CC; **nueva `atn/Csv_unk`** 0x8005B3CC-0x8005C248 (solo `.text`: instancias de `atn::CsvBase` + `fn_8005C220`, que quiza sea de otro fichero; nombre real probable `atn_csv.cpp`) (qA1).
- **`atn/Sound3dListener` fusionada con `_cont`**: `.text` 0x800C7DC4-0x800CAD9C, `.rodata` 0x8073401C-0x80734180 (incluye la tabla de errores que estaba sin asignar; empieza en 0x8073401C porque 0x80734020 rompia objdiff con un simbolo de hueco, y acaba en 0x80734180 porque los 8 ceros finales desplazaban el DOL), `.data` hasta 0x807795C0, `.sdata2` 0x80881160-0x80881180. Sigue enlazada (qA6).
- **`atn/Task` partida**: `Task` hasta 0x800D5618 y **nueva `atn/TaskBase`** 0x800D5618-0x800D5738 (+ `.ctors`, rodata, vtable, bss). Prueba: `Task.cpp` conserva las llamadas a `IsValid` sin usar, que MWCC borraria si la definicion estuviera en el mismo fichero (comprobado) (qA6).
- `atn/D2A_Common`: `.rodata` 0x8073CD68-0x8073CDA8 (su `@stringBase0`) y `.sdata` 0x8087DAB0-0x8087DAD0 (pools `@STRING@` de `Reserve`), antes `auto_` (qA7).
- `.sbss` sacados de huecos `auto_`: 0x8087F518-20 a `Loader` (qA3), 0x8087EF68-70 a `NandManager` (qA4), 0x8087EE98-A0 a `ColliBaseFunc_unk` y 0x8087EE80-98 a `AudioDevice` (qA5), 0x8087EF10-18 a `MindowsCommon` (qA8).
- No hecho (pendiente): `fn_8046CBB0`/`fn_8046CBBC` son de `atn/LoadManager_unk` (qA3); `.sbss` 0x8087EEC8 de `Font_unk` (qA8); el double 0x807347E0 de `TextAnimation` esta dentro de `atn/CompositeIntensityTintMulti` (qA6); `Motion_unk` 0x800A0A68-0x800A1F84 (`std::map` de MSL con clave de 64 bits) y 0x800A2664 (`boost::random` de CryptMP) probablemente son de otros ficheros (qA4).

**Nombres reales y renombrados amplios:**
- **`atn::LoadManager_unk` es `atn::Loader`** (RTTI y vtable 0x8078FB70): clase renombrada en todo el repo y 9 simbolos en `symbols.txt` (cabecera sigue en `include/atn/LoadManager.h`); deriva de `atn::Task` (qA3).
- 31 metodos de `SoundCore` renombrados y llamadores actualizados (Bgm, Se_unk, Sound3dEngine, SongManager, GameBgm); cabecera nueva `include/atn/SoundCore.h` (qA6).
- Nombres NW4R reales en `symbols.txt`: ctor/dtor de `DvdSoundArchive`, `SoundArchivePlayer`, `SoundHeap` (qA5); ctor de `MP::Manager2` 0x804D0328 (qA9).
- Fichero real de `NandManager`: `atn_nand.cpp` (cadena del pool). `SequenceUnitParam1_const_int` es el arranque por secuencias (BootTask/Title/...); nombre propuesto tipo `SequenceBoot`, sin renombrar (qA9).
- Siguen `extern "C"` por sus muchos llamadores: `fn_8046DC5C`/`fn_8046DD20` de Loader (34 ficheros db/), 14 funciones de `D2A_TaskInstanceBase<32,8>` (~200 llamadas en 14 ficheros), los `Add*` de Mindows y funciones de `Font_unk` (~100 ficheros), cinco de `NetManager` (las llaman PPDATAU/DwcAllocator_unk), `fn_803B27F8` (tres `game/`).

**`symbols.txt` mas alla de renombrar:** `lbl_807C7270` (0x40) partido en cinco registros de dtor de 0xC y `lbl_8087EF38` 8 -> 1 byte (qA4); `lbl_80792AE0` reducido a 0x1C8 y 7 constantes ptmf nuevas `@8749..@8755` (qA3); `@LOCAL@`/`@GUARD@` de `fn_80059784` (qA1); `0x803B29A8` pasa a dtor de `GameLoadTask::OptionTmp_unk`, redefinido en `auto_03_803B27AC_text.cpp` (sigue al 100%) (qA9); nombres en unidades ajenas en asm (PacketPack, NetGlobals) (qA10).

**Cabeceras compartidas y firmas** (todas con regress sin perdidas):
- Reescritas/ampliadas: `atn/ColliBase.h` (orden real, firmas de vf0C-vf1C, `ColliResult_unk` con ctor; subclases sin dtor declarado; triangulo como struct plano porque con `Vector3` el ctor inline generaba un bucle que rompia `gmk/WaterLevel`), `atn/Task.h` (layout completo; `m20` sigue `void*` por `gmk/OnOff`), `atn/LoadManager.h`, `atn/D2A_Common.h` (layout 0x12C; `src/atn/D2A_Reference.cpp` adaptado a los nombres nuevos, sigue al 100%), `atn/D2A_Data.h`, `atn/SoftKeyboardBase.h` (`SoftKey_unk` con dos `Vector2_unk`), `gmk/MapLift.h` (`MotionPose_unk::m10` como `Quaternion_unk`), `game/MotionCmdData_unk.h`, `FileList.h` (sin la virtual pura `vf0C`), `std/list.h` (`__list_owned_unk`), `std/string.h` (`operator[] const`, `operator+(const basic_string&, const T*)`).
- Llamadores ajustados (mismo codigo): `gmk/FloatingBox`, `gmk/WaterLevel`, `gmk/OnOff`, `game/EnemyFieldMagician` (qA1); `LH_Core.cpp` (qA3); `GameOverTask`, `StrapTask`, `NewCampPartsTop`, `main`, `auto_03_803B27AC_text` (qA9). `fn_8004D388`/`fn_8004D314` reciben `const Vector3*` + radio; `gmk/Brazier` y `game/CharaTask` las siguen declarando con una esfera (mismo codigo) (qA5).
- Duplicado a unificar: `GammaParams_unk` en `include/atn/RenderSetting_unk.h` y local en `atn/PostFxGamma.cpp` (qA2); `CollResult_unk` locales de gmk/game frente a `atn::ColliResult_unk` (qA1); `LightSet_unk` redeclarado en `ModelDraw_unk` (qA4); `IRenderLayer`/`Layer3D` redeclarados en `LayerShadow.cpp` (qA2).
- Cabeceras nuevas: `atn/ColliData_unk.h`, `atn/RenderSetting_unk.h`, `atn/SoftKeyboardU.h` (aparte de `SoftKeyboardBase.h`: dentro cambiaba el orden de copias debiles de la unidad base), `atn/NandManager.h`, `atn/SoundCore.h`, `atn/D2A_TaskInstance.h` (plantilla `D2A_TaskInstanceBase<N,M>`), `atn/MindowsCommon.h`, `atn/Font_unk.h`, `atn/SequenceHolder.h`, `atn/SequenceBoot_unk.h`, `atn/net/Manager.h`.

**Otros:** `configure.py`: quitada la linea `Object(NonMatching, "atn/atn_colli.cpp")` que duplicaba la entrada de `game_units.json` y hacia fallar `configure` (qA1).

## Bloqueos recurrentes (no repetir sin idea nueva)
- Presupuesto de inline: el original inlinea un nivel mas de recursion en los recorridos de arbol (`Task fn_800D4558/498C/4DB8/51EC` 42,7%) y en `Destroy` de los mapas MSL (`AudioDevice` dtor 29%, `Destroy` 9%; igual que TownTask y `ptr_string_base_char_0`); el ctor de `SoftKeyboardU` queda justo por debajo del presupuesto y no saca de linea los bind/`boost::function` (14 funciones); `LayerShadow::vf10` lo supera.
- `D2A_TaskInstanceBase::vf10` (x3) y `Update<32,8>`: el original relee `mTask` tras actualizar el frame (~10 variantes probadas).
- Copias de `Camera_unk` con otro reparto de registros (LayerShadow; probablemente en todo el juego).
- `push_back` de `std/vector.h` (residuo conocido): `Regist_unk<T>` x3 en SequenceUnitParam1.

## Herramienta nueva
- `tools/decomp/fuzzdrop.py <now/report.json> <ref/report.json> [umbral]`: lista las funciones cuyo fuzzy baja (o desaparecen) respecto a una referencia; complementa a `regress.py`, que solo mira las que estaban al 100%. Usada en todas las integraciones del lote.
