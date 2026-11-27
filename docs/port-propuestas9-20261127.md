# port-propuestas9-20261127: arreglos del port de PC llevados a la decomp (regla 10)

Rama `port-propuestas9-20261127` desde `main` (`02f7f9c8`, accesores-20261126), 2026-11-27, secuencial y sin agentes,
dentro de la sesión de sincronización 7 del port (`TheSoraHD/TheLastStory-PortTest`, rama `sync-decomp7-20261127`).
Objetivo: lo que el port de PC tuvo que arreglar con `#if TARGET_PC` porque la decomp leía un objeto con una vista de
offsets de la Wii, con un tipo distinto en cada fichero o con una aridad que no era la real. Es la regla 10 de
`CLAUDE.md`: un objeto, un solo tipo.

## Resultado

- **16418 / 18113** funciones (igual), código 66,59 %, unidades 757; fuzzy total 97,81190 -> 97,81192 (TacView).
- `main.dol: OK` tras cada commit; `regress.py` lost 0 y `fuzzdrop.py` 0 contra el `report.json` de `02f7f9c8`;
  `ordercheck.py` 0 fuera de orden y `mnemcheck.py` limpio en las 36 unidades tocadas; `poolcheck.py` con los mismos
  avisos que antes en lo tocado; `labelcheck` igual que la referencia (tras `pdbatch.sh`); rep100 6.
- **Sin bajadas.** Las que salieron al probar se quitaron (abajo, «Fuera»).
- `symbols.txt`: solo renombrados (los métodos y parámetros que cambian de clase), seis tipos `data:` nuevos y el
  re-emparejado de `@N` de `pd.sh`/`pdbatch.sh`.

## Qué entró

Cada cambio se probó por unidad con `fnstat.py` antes y después (mismo % en todas las funciones, salvo la subida de
TacView), y con el build completo de la Wii.

| Commit | Qué |
|---|---|
| `585e275f` | TacView `fn_805ABE74`: `fn_80219344(chara, type)` con sus dos argumentos, como su definición; r4 ya lleva `type` (97,08 -> 97,11 %) |
| `cd1a2107` | Aridades que el original pasa en registros sin moverlos: `fn_80239794`/`fn_80234A8C` (los cuatro tamaños de main), `fn_800A5B30`/`fn_800DF16C` (sensibilidad), `fn_80089620` (dueño del menú de depuración) |
| `c34f992b` | `atn::ColliBase` como objeto tocado por un rayo (fuera las dos vistas `CollObj_unk` de ControlStickByQuark y Em403WarLion; FlamingArrow tipado); `AIManager::m90` (fuera `Global8087EE68_unk`); `EquipParamEntry_unk::mA8` (fuera `CosEditEquip2_unk`) |
| `2af0ffa3` | `SkillEffect`: el círculo mágico de `CharaTask::mC58.m110` y los efectos de `CharaTask::m120C` (fuera `CharaD68_unk` y `CharaAttach_unk`); `fn_8013C514/53C/544` son métodos de `SkillEffect` (copias fuera de línea en CharaTask.cpp) y `fn_8017639C/801763E0` toman `SkillEffect*` |
| `a533dfb2` | `SkillEffect` como objetivo de TacView, de las líneas de puntería de GameUI y de `AiTarget` tipo 2 (fuera `TacViewTarget_unk` y `GameUIAimTarget_unk`); `TacView::fn_805AE034(SkillEffect*)` |
| `043c42d4` | Em308InvertCrab pide el tutorial con el `GameUIElem1C_unk` real (fuera `CrabTutorial_unk`/`CrabMessage_unk`; vf10 99,80 -> 99,88 con `pd.sh`); `NewStaffRoll::mFade` es `atn::FadeColor*` (fuera `StaffRollFade_unk`); las tablas de las pistas d2anime por `atn::D2A_Track_unk::mTables` en D2AMpMenu, GameChoiceWindow, MenuCommonTask y NewTitleTask (antes `(char*)pista + 4`) |
| `d3773d29` | `ColliTreeBounds_unk` como rejilla de las formas (fuera `ColliGrid_unk`; `vf1C` de ColliBase y de las cuatro formas); `ReplayShot_unk` deriva de `BulletShot_unk`; MenuCommonTask lee el evento como su `D2A_Common` y los colores del menú de `GameUI` (fuera `MenuAnimeEvent_unk` y `MenuColors_unk`) |
| `cc8b9673` | Pad_unk lee las banderas del gestor de Wii Remote del `WPad_unk` real con máscaras, como las escribe WPad_unk.cpp (fuera las vistas locales `WPad_unk`/`WPadFlags_unk`) |
| `39457724` | De la sync 6: `Sequence::m1CC` es el `unsigned long` que escribe `fn_8006BA8C` (EventParser ya no pasa un `int*`); Em001Necromancer lee `info->m04`; la tira de un efecto guarda su escena como `atn::Scene* mScene` (antes un `int` y un cast) |
| `12c80ad2` | `symbols.txt`: `data:4byte` en `lbl_80750658` y `lbl_80787F68`, `data:wstring` en `lbl_8078C638`, `lbl_8087E0E0`, `lbl_8087E690`, `lbl_808813D0` (tablas que el port copia del `main.dol`); fuera los restos `.orig`/`.rej` que dejó accesores-20261126 |
| `3834c25b`, `1fc7ca5b` | Vistas de parámetros de objeto, arma y armadura a la clase real (`ItemParamEntry_unk`, `WeaponParamEntry_unk`, `EquipParamEntry_unk`): MarketTask, ShopBase/ShopColor/ShopIdentify/ShopBuySell, TreasureBoxTable_unk, D2AMpMenuResult, CosEditBase, NewCampPartsItem, CosEditItemList_unk e ItemRare_unk; los miembros pasan al del mismo offset de la Wii (`mCountId` -> `mId`, `mKind` -> `mBC` o `mType`, `mPrice` -> `mC8`...) y las declaraciones locales de los getters, a las de las cabeceras de `db/`. `ItemParamEntry_unk::mC2` (rareza) es `signed char`: ItemRare_unk y GameUI la leen con `extsb` |
| `1e492122` | `pdbatch.sh` en las unidades cuyas `@N` se movieron con los `#include` nuevos; Em201ZanGurgFinal vuelve a su vista (20 etiquetas de su pool no volvían a emparejar) |

## Segunda parte: `port-propuestas9b-20261127`

Rama desde `main` con la primera parte mergeada (`25f3916a`), mismas comprobaciones contra su `report.json`:

| Commit | Qué |
|---|---|
| `190bd2c9` | `gmk::DropItem::mData` es `ItemParamEntry_unk*` (fuera `DropItemData_unk` de `gmk/DropItem.h`; `mCode` -> `mId`) |
| `79ad9798` | Las prioridades de equipo de CosEditItemList_unk comparan `EquipParamEntry_unk`/`WeaponParamEntry_unk` (estadísticas `m00`-`m0C`, elementos `mE4`/`mC4`); fuera `CosEditGear_unk` y `CosEditArmor_unk` |

Probadas y fuera por las etiquetas: `GameUIItem_unk` (GameUI), `ItemSlotData_unk` (ItemSlotAnime) y `BattleItem_unk`
(BattleTask) dan el mismo código, pero el `#include "db/ItemParam_unk.h"` renumera cadenas de `.rodata` que
`pdbatch.sh` no vuelve a emparejar (labelcheck 6, 14 y 10 etiquetas más); se quedan como vistas.

## Tercera parte: `port-propuestas9c-20261127`

Rama desde `main` con 9b mergeada (`a33776be`), mismas comprobaciones (`main.dol: OK`, lost 0, sin bajadas de fuzzy,
labelcheck y poolcheck iguales; las etiquetas renumeradas por cabeceras nuevas, con `pd.sh`/`pdbatch.sh`). Bloques
`#if TARGET_PC` del port que leian por offsets de la Wii, pasados a miembros de la clase real:

| Commit | Que |
|---|---|
| `c2454a93` | `chr::ControlMagic` en vez de tres vistas `ControlMagic_unk` (Em003LesserDaemon, Em101ElderZesya, Em101ElderZesyaSecond); miembros reales en PacketPack (Manager2::vf10 95,19 -> 95,74), LH_Model, RenderSystem_unk, Em203GreaterDaemon, Em302Kraken, Em309Ghoul, EnemyQuarkSecond, PlayerInput_unk, SpecialEnemyBase y em005_doppel_ganger |
| `968573f8` | PacketPack: la camara del campo por `GameTask::mCamera` |
| `72c344ca` | RenderSystem_unk: `m52C`, `m74C.mImage`, `m890`, `mCamera.mNear/mFar`, `mResolveCount`/`mResolve`; `RenderManager::m52C` pasa a `void*` (tabla de remapeo de fn_800BC618); fn_800BDBC8 98,37 -> 98,38 |
| `fd65ab49` | `GameConfig_unk::mInput` en vez de `ajustes + 0x48C` en CharaTask, Em209SmallGolem, MarketTask y ControlBreakOpen |
| `fbba5538` | `GraphicsDevice_unk::mWidth/mHeight` en RenderSystem_unk; filas de `GameScene::mA0` por indice en CrowdSimulation, RouteWalker_unk y ChaserNpc |

Fuera: los `PORT_OFF` de rellenos de struct (menus MP: son tamanos, no lecturas), `CrowdSimulation` `mChara + 0x528`
(el walker guarda `void*`; tiparlo pide `CharaTask.h` en la unidad) y CosEditProtector `m48 + 0x1228/0x126C` (bajaba).
Fallaban o bajaban y se quedan: D2A_Task, D2A_Texture, EffectManager, ModelInstance, TextureComposite, EnemyJill,
NewCampPartsStatus, MapLift.

## Cuarta parte: `port-propuestas9d-20261127`

Rama desde `main` con 9c (`d1551ca7`), mismas comprobaciones (`main.dol: OK`, lost 0, sin bajadas, labelcheck y
poolcheck iguales). Con los datos del disco el port ya llega al primer combate; esto sale de `port/link/views.txt`:

| Commit | Que |
|---|---|
| `9ff9ec2f` | ChaseManager lee los ajustes de la persecucion de `GameConfig_unk::m4DC` (`mReleaseSwingCount`, `mCapturedTime`, `m50`-`m58` como vector con una inline local, `m5C`) en vez de la vista `ChaseSettings_unk` con offsets de la Wii |
| `5976b143` | Un tipo por nombre: los structs locales de evento de BattleTask y CharaTask pasan a `HitEffectEvent_unk` y `HitReactEvent_unk` (`HitEvent_unk` es el de `game/HitEvent_unk.h`); `__ct__17HitReactEvent_unkFv` renombrado en `symbols.txt` |

Probadas y fuera: un accesor `inline` dentro de `GameConfig4DC_unk` (renumera las etiquetas de las ~140 unidades que
incluyen `GameConfig_unk.h`; se usa una inline local de ChaseManager); renombrar el `EffectRes_unk` local de
atn/EffectRes_unk.cpp (su `mRes` es el `EffectRes_unk*` de `atn/EffectManager.h`: dos vistas del mismo objeto con
layouts distintos, queda pendiente de entender cual es el real).

## Quinta parte: `port-propuestas9e-20261127`

Las vistas de objeto que la propuesta 9b dejo fuera por las etiquetas, ahora con el re-emparejado adecuado.
Mismas comprobaciones (`main.dol: OK`, lost 0, sin bajadas, labelcheck y poolcheck iguales):

| Commit | Que |
|---|---|
| `05c02cd4` | GameUI: `ItemParamEntry_unk` en vez de `GameUIItem_unk` (`m64`, `mB4`, `mB8`, `mBC`, `mC2` son los de `BulletParam_unk`/`ItemParamEntry_unk`) |
| `bfdeac94` | BattleTask: `ItemParamEntry_unk` y `WeaponParamEntry_unk` en vez de `BattleItem_unk`/`BattleWeapon_unk`; el `mKind` de la vista es `mType`; las condiciones de la flecha son `ItemParamEntry_unk::mEC` leidas como `ArmsCond_unk` (mismo layout de cinco enteros) |
| `d42c6156` | ItemSlotAnime: `ItemParamEntry_unk` en vez de `ItemSlotData_unk` |

Como se re-emparejan las etiquetas: `pd.sh` y despues, para los floats de `.sdata2` con el mismo nombre en varias
direcciones (valores repetidos), `lblseq.py <unidad> --apply` (alinea por secuencia de valores; ahora admite
`4byte` con reubicacion en vez de cifra), y para las cadenas de `.rodata`, `datafix.py` (mismo offset y
tamano) comprobado con `lblval.py` (0 distintas, 0 sin pareja). Despues, `configure.py` con las rutas de `pd.sh`.

Fuera: Em201ZanGurgFinal `ZanWeapon_unk` (el escudo es `WeaponParamEntry_unk`, pero el mismo objeto llega por
`CharaPart_unk::m274`, que es `chr::CharaPartData_unk`, otra vista: hay que unificar esas cabeceras primero).

## Sexta parte: `port-propuestas9f-20261127`

Mismo metodo que la 9e (`pd.sh`, `lblseq.py --apply`, `lblval.py`), mismas comprobaciones:

| Commit | Que |
|---|---|
| `0187b235` | CosEditProtector: `ItemParamEntry_unk` en vez de `CosEditItemData_unk`; el re-emparejado por secuencia arregla ademas un float de `.sdata2` emparejado con otro valor (`lblval`: 2 distintas y 1 sin pareja antes, 1 distinta despues, la de siempre de `@wstringBase0`) |
| `30506d60` | DropItem: `WeaponParamEntry_unk` y `EquipParamEntry_unk` (`mKind`, `mId`) en vez de `DropWeapon_unk`; `fn_80206B9C` (`bool`) y `fn_8020EF04` declaradas solo en las cabeceras de `db/` |

Queda: ShopBase `ShopWeapon_unk`/`ShopArmor_unk` (el codigo trata los cuatro primeros floats de armas, armaduras y
bonus como un mismo `ShopParams_unk*`, y las habilidades como `ShopSkill_unk`: con las clases reales seguirian siendo
casts).

## Septima parte: `port-propuestas9g-20261127`

Firmas reales de llamadas de cola, desde `sigcheck.py` del port (definicion `void`, otras unidades la declaran con
resultado y lo usan): en la Wii un envoltorio `void` que acaba en un `b` devuelve el r3 de la funcion llamada; en PC
el llamador lee un valor sin definir.

| Commit | Que |
|---|---|
| `5c01fce3` | Se_unk `fn_800C310C`/`fn_800C3118`/`fn_800C31E4` devuelven lo que devuelve el motor de sonido (NewTitleTask, Sequence, PacketPack, GameScene, los Em202 y EnvSeManager lo comprueban) y el grupo es `const char*` (en AudioDevice ya lo era; PacketPack lo declaraba `int`); Pad_unk `fn_800A5B38` devuelve el nivel de bateria de `fn_800DF1D0` (Loader lo comprueba). Mismo codigo (`b`), mnemcheck limpio |

## Octava parte: `port-propuestas9h-20261127`

| Commit | Que |
|---|---|
| `f96d129f` | CharaTask `fn_80178BFC`: `fn_800CB5B4(&m1414, m1410, 10)` con el volumen, como lo define SoundManager (`SoundCore::fn_800CA0AC(float, int)`). El original no carga f1 porque `m1410` ya esta ahi; declarada sin el float, en Win64 el llamador ponia los frames en la casilla del volumen. Mismo codigo; el parametro sin nombre en la declaracion (con nombre consume un `@N` y renumera 130 etiquetas) |

## Fuera, y por qué

- **Campos de bits leídos también con máscaras** (`BattleCharaTask::m14A8`, `NewCampPartsModel::mFlags`, las banderas
  de golpe de `BattleCalc_unk`): el original usa las dos formas en unidades ya al 100 % (`mFlagC26 = 1` en el
  constructor y `m14A8 |= 0x4000000` en Em203, Em312...). Son un solo tipo (una unión de palabra y campos), así que no
  rompen la regla 10; el problema del port es solo el orden de los bits en PC y se queda en su `#if`. No he probado a
  pasar todos los lectores a una forma: `rlwimi` y `oris` no dan el mismo código.
- **`SoundObject` sobre `SoundCore`**: es una vista del mismo objeto, pero `atn/SoundRef.h` lo incluyen cientos de
  unidades y `SoundCore.h` trae declaraciones con parámetros con nombre: renumeraría `@N` en todo el juego. Pendiente
  para un lote de cabeceras.
- **`Model::fn_80096218` con `SimpleVector<unsigned int>`**: el original instancia `SimpleVector<unsigned long>` en
  esa unidad (sus cadenas `@STRING@reserve__Q23atn16SimpleVector<Ul>FUii` están en `.sdata`, 0x8087D7CC). La decomp
  tiene razón; el port se queda con su `#if`.
- **ItemManager_unk con `atn::Vector4_unk`** (rectángulo de icono devuelto por memoria): baja cinco funciones del 100 %.
- **DLAllocator** (`mallinfo` con `size_t`): en PC son 8 bytes; en la Wii `unsigned long` no es lo mismo que el `int`
  del original. Se queda en el port.
- **Vistas que quedan**: `GameUIItem_unk`, `BattleItem_unk` (sus condiciones son otra vista de `ItemParamFive_unk`),
  `ItemSlotData_unk` (lee una palabra entera en 0x00), `ShopBase` `ShopWeapon_unk`/`ShopArmor_unk` (subestructuras
  propias), `DropWeapon_unk` (una vista para armas y armaduras), `CosEditProtector` `CosEditItemData_unk` (baja con el
  `#include`), Em201ZanGurgFinal `ZanWeapon_unk` (etiquetas), las vistas de equipo de CosEditItemList_unk
  (`CosEditGear_unk`/`CosEditArmor_unk`) y el enlace `CharaListNode_unk` de Em202ForestSpider/EnemyQuarkSecond (el
  `mNext` real está en `PlayerTask` y en `EnemyTask` por separado; llevarlo a `BattleCharaTask` es un cambio de cabecera).

## Herramientas (en el proyecto del port, no en la decomp)

- Conversión de una vista a la clase real: los miembros de la vista se marcan, clang (rama Wii, `wii_layout.py`)
  señala cada acceso, y se renombran al miembro de la clase real del mismo offset de la Wii. Clang no siempre llega a
  los cuerpos de los functores locales: la comprobación que vale es `fnstat.py` igual antes y después (un nombre que
  existe en la clase real con otro offset cambia el código, como `mKind` de `BulletParam_unk` frente al `mBC` de la
  vista).
- Un `#include` nuevo de una cabecera de `db/` renumera `@N` de la unidad: `pd.sh`/`pdbatch.sh` lo arregla casi
  siempre; `labelcheck.py` contra la referencia dice dónde falta.
