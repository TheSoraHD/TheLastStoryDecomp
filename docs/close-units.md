# close-units (propuesta D): cerrar y enlazar unidades casi terminadas

Rama `close-units` desde `main` 6111ca3 (2026-10-22, sesion en la nube). Permisos del usuario: ajustar splits de las unidades del lote, renombrar en `symbols.txt` y push de la rama. Excepciones a la regla 2: solo las aprobadas (ningun `asm` ni `#pragma` nuevo).

## Resultado

Build limpio `main.dol: OK`; `regress.py` **lost: 0** y `fuzzdrop.py` **0 bajadas** contra main 6111ca3, contra cada referencia de integracion y contra el `report.json` final de cada cola.

| | main 6111ca3 | close-units |
|---|---|---|
| Funciones al 100% | 6476 | **6806** (+330) |
| Codigo | 21,33% | 22,57% |
| Fuzzy | 34,00% | 35,45% |
| Datos | 17,05% | 17,29% |
| Ficheros enlazados | 342 | **370** (+28) |

### Unidades nuevas enlazadas (28)
- Juego (5): `atn/ParseTree`, `atn/LayerPostFx`, `chr/ControlAttractHateCharge`, `chr/ControlCaught`, `game/HomeButtonMenu`.
- Librerias (23): MSL `strtold`; SDK `AXSPB` + `AXVPB` (tienen que ir juntas), `esp`, `GXAttr`, `mem_allocator`, `mem_heapCommon`, `OSFatal`, `OSReboot`, `__ppc_eabi_init`; HBM `lyt_arcResourceAccessor`, `ut_ResFont`, `ut_TagProcessorBase`; NW4R `snd_InstancePool`, `snd_SoundHandle`; smg2 `bte_hcisu`, `bte_logmsg`, `hcisu_h2`, `GXFifo`, `GXPixel`, `OSNet`; DWC `dwc_main`, `dwci_ranking`.

### Al 100% pero sin enlazar
- `std/char_traits_char`: su `.rodata` empieza en 0x807350A4 y MWCC lo alinea a 8 (`align:4` no basta); solo enlazara fusionada con `atn/WordWrap_unk` (falta iostream de MSL).
- `db/EventIdTable_unk` (7/7): emite copias debiles de `Buffer_unk`/`CsvBase` y `vf08`, y vtables/RTTI de `atn::Csv` en `.data`, que el original no tiene en esta unidad; guarda y `sInstance` salen como comunes. Arreglarlo requiere tocar `atn/Csv.h`.
- `gmk/SparkCube` (19/19): define el static local `sListeners` de `fn_8014F8F4` (como Item/DropItem/Banana).

## Colas (4 agentes a la vez; notas en `docs/close-units-q{A..F}.md`)
Unidades con fuente y `"matching": false` (395 del juego + 33 de librerias) ordenadas por funciones pendientes y repartidas por familia; 20 saltadas por bloqueo documentado (PostFx*, Layer2DBlur, MapPartsRain, CryptMP, Gecko_ExceptionPPC, dwc_match, fragmentos de otra TU, inline recursivo, CharaTask...).
- **A** `atn`/`std` (+52): enlaza ParseTree y LayerPostFx; FileList 2 -> 20/27; accesores `LH_*`, `CsvBase_char_124_13_34`, `NetFriend_unk`, `CharaLightData_unk`.
- **B** `gmk`/`db` (+43): Bat, Leaf, SparkCube, LemonBucket, Pigeon/ChickenManager, RandomItem, Banana::vf40...; 25 funciones nuevas escritas (WaitObject, Pigeon_unk, Chicken_unk, Chain, WindyItem) casi todas > 90%.
- **C** `chr`/`game` <= 3 pendientes (+27): enlaza ControlAttractHateCharge, ControlCaught, HomeButtonMenu; GameSoundControl escrita.
- **D** `Event`/`MP`/`AI`/librerias (+10 en el juego y librerias): las 23 librerias enlazadas; `MP::Popup::vf10` escrita (97,6%, unica que falta para enlazar Popup).
- **E** `chr`/`game` 4-19 pendientes (+~58): PopupTask, ControlInteractionBase, ControlWallUp, ControlLadder; fuente nueva de Em314Ivy, Em316LastLaser, Em005DoppelGanger, Em101ElderZesyaSecond, Em101LittleZesyaSecond, EnemyQuarkFirst.
- **F** `chr`/`game` >= 20 pendientes (+150): fuente nueva de 18 enemigos (Em003, Em101ElderZesya/LittleZesya, Em202Spider/SpiderBase, Em205Ogre, Em209SmallGolem, Em303KrakenFoot/Mp, Em305Vampire, Em306GurgBeast, Em308ParmCrab, Em309Ghoul/GhoulSimple, Em310Gargoyle, Em312DarkServant, Em315Coffin, Em401_01_WhiteLeoPard).

## Integracion
- A, C, D, B sin conflictos. E contra C (5 ficheros): eleccion por funcion compilando las dos versiones (version de E en ControlWallUp, GameOverTask, GameSoundControl y ChaserNpc salvo `fn_80181D94` de C; en Em201ZanGurg `vf0C` de C y `vf3C` de E). F contra E: `symbols.txt` en los limites 0x8025A03C (`__sinit` de Em101ElderZesya / ctor-dtor de Em101ElderZesyaSecond) y 0x8031348C (`vf30` de Em314Ivy / Em315Coffin), y `extern "C"` de la factoria de `SpecialEnemyBase` que ninguna rama usa ya.

## Decisiones a revisar (tomadas sin el usuario)
- **Fuera de lo autorizado (regla 3):** `config.yml` dos `block_relocations` para la constante 0x80808081 de `ScreenReport` (OSFatal); `libs_ref.json` `-func_align 16` en `snd_InstancePool`, `snd_SoundHandle` y `bte_hcisu`. Ambas necesarias para enlazar esas librerias.
- `symbols.txt`: `align:32` en `__AXCompressorDefaultTable`, `__responses`, `__BTUInterruptHandlerStack`, `Scb` y `lbl_8081C280` (este ultimo, bss de uusb, supuesto por la direccion); `align:16` en `PSMTXIdentity`; `strdup` -> `nan`; las dos copias de stricmp: 0x8068B104 `stricmp`, 0x8068B1FC `strcasecmp` (GameSpy declara `strcasecmp` en vez del `#define`); `sEncServerUrl`/`sEncSeed` estaban intercambiados; nombres reales de 3 simbolos debiles de `boost::bind` de ControlCaught; muchos renombrados `fn_`/`lbl_` a nombres de las clases nuevas (lista en las notas de cada cola).
- `splits.txt`: recortes en GXAttr `.data`, mem_allocator `.sdata2`, OSNet `.data`; ampliaciones de `.text` en GXFifo y GXPixel; `.rodata` de Em303KrakenFootMp hasta 0x80747F58.
- Funcion inventada `fn_Stripped_unk` en ParseTree solo para el literal "tree::insert length error" del pool (como Event/Label).
- Constante de `OpenBoard::vf28` corregida de 1.0f a 0.1f (valor real del `.sdata2`).
- Fuente nueva con relleno para miembros desconocidos en 24 enemigos (E y F); factoria de `SpecialEnemyBase` con `new (3, "", "", 0) Clase(...)` para las clases ya escritas.
- Cabeceras compartidas: `atn/Csv.h` (ctor de `CsvPipe`), `atn/LH_Material.h`, `atn/LH_Xsca.h`, `atn/LayerPostFx.h` (`__Vec2Copy`), `math/Vec.h` (`Vector3::RotZ_unk`, `ToRotationZ_unk`), `chr/ControlLadder.h`, `Em305Vampire.h` (`m15BC` ResHandle, `m14E8` RoutePlaceList_unk), `Em209SmallGolem.h` (`m14B0` LH_Binary, `m1550` ResHandle), `MP/Popup.h` (`mTarget` es `GimmickCommon*`). Ninguna funcion baja.

## Hallazgos de MWCC (una linea cada uno; detalle en las notas de cola)
- Una lectura `volatile` del global dentro del cuerpo del bucle reproduce la recarga del original; un local puntero tomado del global al principio del cuerpo, a veces tambien.
- El orden de registros salvados puede depender de pasar `p + k` frente a `p += k` antes de la llamada.
- Un indice `int` usado como `a[(unsigned)i]` reordena los incrementos del bucle; reutilizar una variable ya viva como contador cambia su registro.
- Un envoltorio inline `float Atan2_unk(float, float)` cambia el orden de carga de los argumentos de `atan2`.
- Una referencia local a un miembro matriz dentro de un inline manda su direccion a un registro salvado; `x = x + d` con local evita el `fmadds`.
- `fcmpo K,f` con `cror lt`: recargar el campo en un local nuevo y escribir `if (K <= f)`.
- El inline que lanza efectos tiene los locales en orden color/rot/pos en unas unidades y pos/rot/color en otras; los operadores de `Vec.h` (`v * k + w`) dan el orden de pila correcto y los ctores escritos a mano no.
