# Lote batch-auto-10kb (2026-10-20/21): resumen y decisiones a revisar

Lote de decompilacion sobre las 42 unidades partidas sin fuente de 5-10 KB (`targets.py --max-unit 10240`, quedandose con las > 5120), con una fase 0 de cortes de splits pendientes de `batch-auto-5kb`. 9 colas (4 `atn`, 1 `db`, 4 `game`/`AI`/`gmk`/`MP`), repartidas por familia y tramos de direcciones contiguos, con 4 agentes a la vez; cada copia nueva se creo desde la rama de integracion ya actualizada. Notas completas de cada cola (resultado por unidad, NONMATCHING con motivo, decisiones y hallazgos MWCC) en `docs/batch-auto-10kb-q{A1,A2,A3,A4,D,G1,G2,G3,G4}.md`.

Permisos del usuario para este lote: ampliar o cortar splits de esas unidades (y los de la fase 0), renombrar en `symbols.txt` y hacer push de la rama (no de `main`). Excepciones a la regla 2: solo las ya aprobadas.

## Resultado

- Build limpio (`rm -rf build/SLSP01`) con `main.dol: OK`, tras fusionar tambien el `main` nuevo (e4a1847, rama `mwcc-blockers`).
- `regress.py` **lost: 0** contra `main` antiguo (6ace171), contra `main` nuevo (e4a1847), contra la rama antes de fusionar main y contra el `report.json` final de cada una de las 9 colas.
- Funciones al 100%: `main` 6ace171 **5375** -> fase 0 **5379** -> 9 colas integradas **5847** (+468) -> con `main` e4a1847 (5487) **5961** (+474 sobre ese main). Codigo 16,69% -> 19,46%; fuzzy 27,45% -> 30,94%; datos 15,89% -> 16,87%; ficheros enlazados 335 -> 341.
- De las 42 unidades (+4 nuevas al partir): todas con fuente; **473/723** funciones al 100%; enlazadas (`matching: true`) `atn/Frustum_unk`, `atn/SoftKeyboardBase_wchar_t_0`, `atn/DLMalloc_unk` (y `atn/ResRef_unk` de la fase 0).
- Bajadas de fuzzy (todas por el cambio de `vector::push_back` de `main`, rama `mwcc-blockers`, `vector_a7`: el camino rapido carga el tamano primero):
  - Ya bajaban en el propio `main` nuevo (10, entre 0,01 y 0,04 puntos): `ChaserNpc` ctor, `db/ArrowSet_unk fn_80207310`, `D2AMpMenuFriend fn_804B6F08/fn_804B7F88`, `D2AMpMenuFriendRoom fn_804BB2CC`, `NewCampPartsHint vf20/vf30/fn_80516004`, `Np131LivingArmor vf10`, `ShopColor vf38`.
  - Funciones de este lote (4, entre 0,01 y 0,04 puntos): `ShaderManager_unk fn_800C5644` 95,53 -> 95,49, `FaceData_unk::fn_8011EF14` 99,32 -> 99,30, `Em305VampireMp::vf0C` 93,98 -> 93,97, `Xsca_unk::fn_805BD4B8` 99,09 -> 99,07.
  - `fn_80201E10` (0%) desaparece a proposito (q A3: era la rama muerta del ctor de `D2A_Name_unk`); el total de funciones pasa de 18216 a 18215.

## Fase 0 (cortes de `docs/batch-auto-5kb.md`)

Cada corte con build + `regress.py` (lost: 0, sin bajadas de fuzzy):
1. **`atn/ResRef_unk`** (nueva, enlazada 5/5): `.text` 0x80470364-0x80470580 y `.rodata` 0x80755BF0-0x80755BF8 (su `@stringBase0` es `"%sh"`), sacados de `atn/FileSystem_unk`. Las clases pfs pasan a `include/atn/PfsArchive_unk.h`. `PfsArchive_unk::fn_8046FD2C` (FileSystem) 35,8% -> 100%. (commit df068d9)
2. **`atn/CompositeIntensityTintMulti`**: `.text` desde 0x800D87A4 (copias debiles de sus inline; antes hueco `auto_`). Siguen sin emitirse en nuestro `.o` porque `vf14` (0%) no supera el presupuesto de inline. (4a9cc78)
3. **`db/SpText_unk`** `.text` hasta 0x8021CCC4 (dtor debil 0x8021CC50 sacado de `db/StrategyAiTable_unk`, que sigue enlazada) y **`db/MapLink_unk`** hasta 0x80216AFC (dtors 0x80216A2C/0x80216A94 sacados de `db/MapIdTable_unk`). Fuente provisional con solo los dtors, que la cola qD sustituyo por las plantillas reales. (6410687)
4. **`atn/EffectManager`** fusionada con `std/char_traits_char_8023C080` y `atn/EffectManager_cont`: `.text` 0x80239798-0x8024373C, `.ctors` 0x8072D31C, `.rodata` 0x8074300C-0x80743140 (incluye el pool 0x8074308C, ahora `@stringBase0`), `.data` 0x80783BB8-0x80783CC8, `.sdata` 0x8087DC10-0x8087DC20 ("inf"/"nan"), `.sbss` 0x8087F3C4-0x8087F3D0. Thunks renombrados `@72@vf10__Q23atn13EffectManagerFP10Camera_unk` / `@72@__dt__Q23atn13EffectManagerFv` (los emite nuestro `.o`); borrado `src/atn/EffectManager_cont.cpp`. Queda 28/60 (las plantillas de iostream de MSL siguen sin fuente). (94af1ca)

No tocado (fuera de la fase 0): `Event/Manager` + `PostSequence` + `Manager_cont`.

## Arreglos de integracion

- `include/game/HitInfo_unk.h` (qG2 vs qG1): `m40` float en ambas; queda `m40` + `m44` float (qG1).
- `symbols.txt` .sdata 0x8087D918-0x8087D920 (qA1 vs qA2): se conservan los nombres de ambos (`@STRING@ResizeTextures_unk...` y `sSortOffset`).
- `include/std/vector.h` (qA2 vs qA3): se conservan los dos helpers (`__insert_one_unk`, `__check_append_unk`).
- `include/atn/Array_unk.h` (qG1 vs qG3): se conservan `Alloc0_unk`/`operator=` y `Resize_unk(n, heap, name)`.
- `symbols.txt` 0x80363D38-0x80365390 (qG3 vs qG4): qG3 llamo a la clase `SystemErrorMessage_unk`; qG4 encontro el nombre real **`ErrorManager`** en los nombres de tipo de `boost::bind`. Se dejan los metodos de qG3 con clase `ErrorManager` (fuente `game/SystemErrorMessage_unk.cpp`, nombre de fichero sin cambiar).
- Fusion de `main` (e4a1847): sin conflictos de texto; `atn/Xsca_unk` no compilaba por la nueva firma `__push_fast_unk(n, x)` de `std/vector.h`: adaptada su especializacion (commit aparte).

## Decisiones a revisar

**Reglas y patrones fuera de la guia:**
1. `template class atn::SoftKeyboardBase<wchar_t, 0>;` (qA2) contra la norma de "instanciaciones explicitas por miembro": es la unica forma que encontro de reproducir el orden de emision; la unidad enlaza (`main.dol: OK`, 34/34).
2. `atn/DLMalloc_unk.cpp` (qA3) es dlmalloc 2.8.3 sin modificar (de libffi 3.0.13, dominio publico) con `ONLY_MSPACES 1`, `HAVE_MMAP 0`, `USE_LOCKS 0`. Conserva ramas de preprocesador de MSVC/x86 que no se compilan (`#pragma warning`, `__asm__("bsrl ...")`); no son asm/pragma del juego, pero se pueden recortar si se prefiere.
3. Codigo que no esta en el DOL (solo para el orden de pools/copias debiles; lo elimina el enlazador): `Em201ZanGurg::fn_DebugMenu_unk` (qG2), dos funciones de `TeamAIManager` y su ctor fuera de linea (qA4), `fn_SoftKeyboard_dead_unk`, `LoadShaders_unk`/`CreateLODs_unk` (qA2), tablas sin usar `sRanks`/`sKinds` en OptionParts (qD) y una tabla de estados en TeamAIManager.
4. Literales dudosos `"%s %s\0"`, `"%s\0"`, `"\0"` en GimmickStateCommand/GameOverTask (qG3) para reproducir ceros extra del pool.
5. **`OSInitFastCast` aprobado por el usuario (2026-10-21) y aplicado:** `include/sdk/OSFastCast.h` (copia del `static inline` con asm del SDK, porque las unidades del juego no tienen la ruta de include del SDK; no en `include/revolution/`, que taparia la cabecera del SDK a las librerias) y la llamada tras `OSInit()` en `game/main` `fn_8047A4C8`. Esa funcion sigue al ~40% por el presupuesto de inline (el original inlinea todo salvo `push_back`; aqui sale todo fuera de linea, tambien `OSInitFastCast`), y su fuzzy pasa de 40,28 a 40,13 por la llamada extra: se recuperara cuando la funcion entre en presupuesto. **Sin aprobar:** `atn/LH_Binary` `fn_80471FD0` y `SceneTrack_unk::fn_8047202C` (paired-single/GQR con `psq_l` cuantizado), sin decompilar.

**Splits cambiados en las colas** (ademas de la fase 0):
- `atn/Shader_unk` cortada en 0x800C5644: unidad nueva **`atn/ShaderManager_unk`** (`.text` 0x800C5644-0x800C6128, `.rodata` 0x807338C8-0x80733FF8, `.data` 0x80779368-0x807794A8, `.bss` 0x807C75A0-0x807C75B0, `.sbss` 0x8087EFE0-0x8087EFE8, `.sdata2` 0x80881150-0x80881158): dos pools y dos `0.0f` (qA2).
- `game/Gun` cortada en 0x8043F5C4: unidad nueva **`game/HColliData_unk`** (`.text` hasta 0x804405F8, `.ctors` 0x8072D370, `.rodata` 0x807544B8-0x807544C8) (qG4).
- `game/HomingBullet` cortada en 0x80442B50: unidad nueva **`game/IffChunkFile_unk`** (0x80442B50-0x80442F10) (qG4). Nombres inventados respetando el orden alfabetico.
- Ampliaciones sobre huecos `auto_`: `.sdata` 0x8087D918-0x8087D920 a `atn/CharaLightData_unk` (pools `""` de un `new[]` inline), `.sbss` 0x8087F018-20 a `atn/WPad_unk`, `.sbss` 0x8087F138-40 a `atn/D2A_Manager`, `.sdata` de `game/AreaEffConfig` hasta 0x8087D708, `.sdata` de `game/CharaFaceControl` desde 0x8087D9A0, `.sbss` de `game/TeamAIManager` desde 0x8087FA00.
- No hecho (pendiente): fusion `atn/WordWrap_unk` + `std/char_traits_char` + `std/allocator_char` (0x800DF22C-0x800E2A24, hay pruebas de un solo fichero pero el resto es iostream de MSL sin fuente); corte `Xsca_unk`/`XsfParser_unk` probablemente en 0x805BEACC; `Em305Vampire` `.rodata` (su pool 0x80747F70 queda fuera); `.data` 0x80787548-0x80787568 de Em305VampireMp.

**Firmas y cabeceras compartidas** (todas con `regress.py` sin perdidas):
- `CharaTask::fn_8015783C` gana un `float` (8 llamadores pasan f1); `CharaTask::vfA8` pasa a `int vfA8(int)` (qG2). `SpecialEnemyBase::vf110/114/118` con argumentos (qG2). `Arms::vf18`/`Sword::vf18` a `(const Vector3&, const Vector3&, int, int)` (qG4; Sword sigue enlazada).
- `HitInfo_unk` (`m40`, `m44`), `CharaTask.h`, `CharaStatus.h`, `Model_unk.h` (callback `boost::function` en `m370`), `LH_MotionCommandData.h` (firma del parser), `Array_unk.h`, `Csv.h` (`GetField_unk`, ctor `CsvTab(const void*, int)`), `std/vector.h` (`empty`, `insert`, `resize`, `__append_unk`, `__check_append_unk`, `__reserve_checked_unk`), `std/string.h` (`empty`, `operator+=(T)`, y `operator=` devuelve el resultado de `__replace_unk`), `D2A_Data.h`, `gmk/CharaLight.h`, `gmk/ItemSlotAnime.h`, `gmk/Lemon.h`, `gmk/SeTable.h`, `GimmickCommon.h`, `math/Vec.h` (`Matrix44_unk::operator=`).
- Cabeceras nuevas: `atn/PfsArchive_unk.h`, `atn/Camera_unk.h`, `atn/SoftKeyboardBase.h`, `atn/Shader_unk.h`, `atn/WPad_unk.h`, `db/WeaponParam_unk.h`, `db/EquipParam_unk.h`, `game/AreaEffConfig.h`, `game/CharaFaceControl.h`, `game/MotionCmdData_unk.h`, `Event/PoseKey_unk.h`, `game/Em201ZanGurg.h`, `game/Em305Vampire.h`, `game/Em305VampireMp.h`, `game/EnemyTrista.h`, `game/Gun.h`, `game/Bullet.h`, `game/HomingBullet.h`, `game/ChunkFile_unk.h`, `game/MapPartsLocatorSet.h`, `game/HeapInfo_unk.h`, `game/MapInstance_unk.h`, `std/vector_string.h`.

**`symbols.txt` mas alla de renombrar:** borrado `fn_80201E10` y ctor de `D2A_Name_unk` 0x28 -> 0x34 (qA3); simbolos nuevos `sCondValues2..6` en `.bss` de SpText (qD); tamanos de pools/floats ajustados; nombres en unidades ajenas en asm (`Em305Vampire` ctor/dtor/metodos, `Bullet` ctor/dtor, `XsfCurveSet_unk`, `RenderCallback_unk`, `GammaParams_unk` ctor en `RenderSetting_unk`, `@STRING@reserve__...Vector_unk<i>` usados tambien por CharaTask).

**Nombres reales encontrados/propuestos:** `ErrorManager` (aplicado) y `ResetShutdown` (= `GameSystem_unk`, no aplicado) por los tipos de `boost::bind`; `CharaFaceControl` (= `Chara1188_unk`, aplicado con typedef); `enemy_trista.cpp` (RTTI `@unnamed@enemy_trista_cpp@` de `ControlShingan`; renombrar la unidad haria casar esos simbolos); `atn/CharaLightData_unk` es la escena `NewCampScene2_unk`; db: `db/WeaponParam.cpp`, `cosedit/OptionParts.cpp`, `db/ItemParam.cpp`, `db/MapLink.cpp`, `db/SpText.cpp`; `atn_camera.cpp`.

## Pendientes

- `std/vector.h`: el `push_back` generico de las copias debiles (bucle contado `--last; new (--p) T(*last);` y `mSize = buf.mSize` sin poner a 0 antes) y su punto de instanciacion (junto a su llamadora) afectan a pools de MotionCommand_unk y GimmickStateCommand (ver qG1/qG3).
- Presupuesto de inline: Load de `AreaEffConfig` (0%), `GimmickStateCommand::fn_803EF780` (0%, 25 copias debiles), `D2A_Manager::fn_801F1658` (0%), `CompositeIntensityTintMulti::vf14` (0%): el original llama fuera de linea a todas sus inline; falta coste en las inline reconstruidas.
- Sin escribir: `EffectDesc_unk fn_80235BF8` (0xC74), `HomingBullet::vf1C` (0xB78), `MP/Text` `fn_8050655C/fn_80506C10/fn_80506D94`, `__sinit` de `HColliData_unk`, iostream de `std/allocator_char`.
- No enlazables aunque lleguen al 100%: PostFx* (emiten copias debiles de `~IPostFx`/`~IRender` que el original tiene en `atn/LinearAllocator`), `gmk/DropItem` y `gmk/Item` (definen el static local `sListeners` de `fn_8014F8F4`).
- `BulletParam_unk` (`db/ParseUtil_unk.h`) incompleto (sub-structs que revela `fn_80211940`); `ptr_string_base` comun en `include/atn/`; clase de `CompositeIntensityTintMulti` a cabecera; `TeamAI_unk` comun para `game/TeamAI_unk`; `src/auto_fn_801F1EA0_text.cpp` huerfano.

## Resultado por unidad

Funciones al 100% / total del `report.json` final (con `main` fusionado).

**qA1** (atn)

| Unidad | 100% / total | matching |
|---|---|---|
| `atn/Camera_unk` | 2/4 | false |
| `atn/Frustum_unk` | 7/7 | **true** |
| `atn/PostFxGamma` | 3/6 | false |
| `atn/PostFxGodRay` | 2/6 | false |
| `atn/CharaLightData_unk` | 14/23 | false |

**qA2** (atn)

| Unidad | 100% / total | matching |
|---|---|---|
| `atn/Shader_unk` | 9/12 | false |
| `atn/SoftKeyboardBase_wchar_t_0` | 34/34 | **true** |
| `atn/WPad_unk` | 16/18 | false |
| `atn/WordWrap_unk` | 4/8 | false |
| `std/allocator_char` | 4/35 | false |
| `atn/ShaderManager_unk` | 2/4 | false |

**qA3** (atn)

| Unidad | 100% / total | matching |
|---|---|---|
| `atn/D2A_Manager` | 11/21 | false |
| `atn/D2Anime_unk` | 4/7 | false |
| `atn/DLMalloc_unk` | 12/12 | **true** |
| `atn/EffectDesc_unk` | 8/14 | false |

**qA4** (atn+game)

| Unidad | 100% / total | matching |
|---|---|---|
| `atn/LH_Binary` | 13/19 | false |
| `atn/THPPlayer_unk` | 18/22 | false |
| `atn/Xsca_unk` | 4/6 | false |
| `game/SongManager` | 7/13 | false |
| `game/TeamAIManager` | 7/17 | false |

**qD** (db)

| Unidad | 100% / total | matching |
|---|---|---|
| `db/WeaponParam_unk` | 15/16 | false |
| `db/OptionParts_unk` | 12/16 | false |
| `db/ItemParam_unk` | 13/17 | false |
| `db/MapLink_unk` | 14/17 | false |
| `db/SpText_unk` | 5/10 | false |

**qG1** (AI/game)

| Unidad | 100% / total | matching |
|---|---|---|
| `AI/AiCommon_unk` | 27/31 | false |
| `game/AreaEffConfig` | 10/35 | false |
| `game/CharaC58_unk` | 31/33 | false |
| `game/CharaFaceControl` | 12/17 | false |
| `game/MotionCommand_unk` | 24/28 | false |

**qG2** (enemigos)

| Unidad | 100% / total | matching |
|---|---|---|
| `game/Em201ZanGurg` | 11/15 | false |
| `game/Em305VampireMp` | 12/14 | false |
| `game/EnemyTrista` | 12/18 | false |
| `game/SpecialEnemyBase` | 16/22 | false |

**qG3** (game/gmk)

| Unidad | 100% / total | matching |
|---|---|---|
| `game/SystemErrorMessage_unk` | 9/11 | false |
| `game/GameOverTask` | 2/7 | false |
| `game/GimmickStateCommand` | 9/39 | false |
| `gmk/DropItem` | 10/13 | false |

**qG4** (game/MP)

| Unidad | 100% / total | matching |
|---|---|---|
| `game/Gun` | 5/6 | false |
| `game/HomingBullet` | 7/10 | false |
| `game/main` | 11/14 | false |
| `game/MapPartsLocatorSet` | 9/13 | false |
| `MP/Text` | 8/16 | false |
| `game/HColliData_unk` | 4/12 | false |
| `game/IffChunkFile_unk` | 4/5 | false |

