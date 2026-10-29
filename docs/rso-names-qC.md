# rso-names, cola C: clases del juego

Rama `agent-C` (2026-10-29). Nombres de la tabla "Pendientes" de `docs/rso-names.md` aplicados en el fuente y en `symbols.txt` (solo renombrados). Comprobado tras cada grupo: `main.dol: OK`, `regress.py` lost: 0, `fuzzdrop.py` sin bajadas contra `/home/claude/ref/report2.json`, `ordercheck.py` sin cambios en las unidades tocadas.

## Resultado por nombre

| Nombre real | Dirección | Estado | Notas |
|---|---|---|---|
| `CharaTask::SetFlag(int)` | `0x803396EC` | hecho | Inline miembro de `CharaTask` (`m54C \|= flag`). Fuera los `extern "C" fn_803396EC` de `EnemyQuarkSecond.cpp` (definición inline) y `EnemyQuarkStand.cpp` (declaración). La copia débil sigue saliendo en `EnemyQuarkSecond`. |
| `CharaTask::GetTacViewPosition() const` | `0x800EF2F0` | hecho | Sustituye `vfC8` en `CharaTask` y en las cuatro sobrescrituras (`Em203GreaterDaemon`, `Em203GreaterDaemonRed`, `Em302Kraken`, `Em302KrakenMp`), con sus símbolos y los `@STRING@` asociados. |
| `GimmickCommon::Start()` | `0x803EDF5C` | hecho | `vf28` -> `Start` en `GimmickCommon` y sus 48 derivadas de `gmk::` (cabeceras, fuentes y llamadas), `GimmickManager::fn_803EE374` y 49 símbolos (`vf28__13GimmickCommonFv` y `vf28__Q23gmk...Fv`). No se tocan los `vf28` de clases ajenas (`Em315Coffin`, `chr::Control*`, `AI::Script`, `MapParts*`, `NewCampParts*`, `atn::*`). |
| `db::SkillBase::Condition::Check(int) const` | `0x80151168` | hecho | Cabecera nueva `include/db/SkillBase.h` (`db::SkillBase::Condition` con un solo `int mFlags`, inferido). `HitInfo_unk::m90` pasa a ser `db::SkillBase::Condition`; los usos como entero (`CharaTask.cpp`, `CharaStatus.cpp`) leen `.mFlags`. Fuera la función libre `HasFlags_unk`. |
| `Inventory::GetPlayerStatusInfo(int)` | `0x805282A8` | hecho | `NewCampStatusParty_unk` es `Inventory`: cabecera nueva `include/game/Inventory.h` (solo lo usado: `m0000[0x64EC]` y los bloques de 0x43C por jugador). `fn_801D80B4` devuelve `Inventory*`. |
| `CharaFaceControl::MotionPart` | `0x8011D320` (`__dt__`) | hecho | `FaceKey_unk` -> `CharaFaceControl::MotionPart` (anidada). Para poder anidarla, `Chara1188Entry_unk` también pasa a ser anidada, con el nombre `CharaFaceControl::MotionSet` (inferido: ver decisiones). Renombrados sus `__ct__`/`__dt__`, `fn_8011E200`, `fn_8011EB18` y los `@STRING@Resize_unk` de `Buffer_unk<MotionPart>`. |

## Decisiones tomadas sin el usuario

- `GetTacViewPosition() const`: el getter de nodo de `atn::Model` (`Model_unk::GetNodeMtx_unk`) no es `const` y `atn::Model` es de otra cola, así que las cuatro sobrescrituras usan `const_cast<...>(this)->mMotion` con un comentario `// inferred:`. Cuando se haga `const` el getter, quitar el `const_cast`.
- `Chara1188Entry_unk` -> `CharaFaceControl::MotionSet`: hacía falta anidarla (es miembro por valor de `CharaFaceControl` y contiene `Buffer_unk<MotionPart>`). El RSO solo lista los tipos anidados `MotionPart` y `MotionSet`, y esta es la que guarda el array de `MotionPart` (el RSO tiene `SimpleArray<MotionPart>`), así que se le da ese nombre con `// inferred:`.
- `0x8011D320`: `pairnames.py` empareja ese destructor con `__dt__Buffer_unk<MotionPart>` (los dos miden 0x74). Se mantiene la asignación que ya teníamos (antes `__dt__11FaceKey_unkFv` al 100%), que es la que da el RSO (`__dt__Q216CharaFaceControl10MotionPartFv`).
- `db::SkillBase::Condition` solo declara el primer `int`; `HitInfo_unk::m94` se queda fuera (no hay pruebas de que sea parte de `Condition`). No se ha decidido si `HitInfo_unk` es el propio `db::SkillBase`.
- `Inventory`: el bloque por jugador se deja como `Inventory::PlayerStatus_unk` (0x43C bytes, el tamaño de `CharaStatus`) y `GetPlayerStatusInfo` devuelve `CharaStatus*` como hasta ahora. `fn_801D80B4` (lee `lbl_8087F4F0`) y `fn_801D80A0` siguen como `extern "C"`: no hay nombre seguro.

## Hallazgos (no aplicados, fuera de la cola)

- `lbl_8087F4F0` (`.sbss`, 8 bytes) es casi seguro `Inventory::instance_` (`instance___9Inventory` en el RSO). Lo usan como `void*` `MP/D2AMpMenuResult`, `gmk/RandomMerchant`, `gmk/TreasureBox`, `game/Em201ZanGurgFinal` y otros. Candidatos por firma: `fn_804439FC(inv, id, num, 0, 0, 0, 1, 1)` = `Inventory::AddItem(int, int, int, int, int*, int, int)` (`AddItem__9InventoryFiiiiPiii`); `fn_8044D6AC(inv, price)` = `AddMoney(int)` o `SubItem`; sin dirección comprobada.
- `CharaFaceControl` en el RSO: `Initialize(atn::Model*)` (= `fn_8011D424`), `SetMotion(int, bool, bool, bool, float, float)` (= `fn_8011E81C`, hoy con `unsigned char`), `SetMotionParts(const atn::SimpleArray<MotionPart>&, bool, bool, float, float)` (= `fn_8011EB18`), `ClearMotion(bool)` (= `fn_8011EDE0`), `StartBlink(float x5)` (= `fn_8011EE58`), `SetNodeVariationList(const atn::SimpleVector<atn::ModelNodeVariation>&)`, `Assign(const CharaFaceControl&)`. Las firmas cuadran, pero dependen de `atn::SimpleArray`/`SimpleVector` y `atn::Model` (otra cola): `atn::Buffer_unk` parece ser `atn::SimpleArray`.
- Siguen vistas parciales de `GimmickCommon` con huecos `vfXX` (incluido `vf28`): `QuarkGimmick_unk` (`EnemyQuarkSecond.cpp`), `KrakenGimmick_unk` (`Em302Kraken.cpp`), `GimmickObj_unk` (`SpecialEnemyBase.cpp`). Habría que cambiarlas por `GimmickCommon` (regla 10); no se han tocado.
- Varios `(data->m90.mFlags & K) == K` de las unidades de enemigos (EnemyQuarkFirst, EnemyTasha, Em204BigMulue, Em316LaserSphere, Em403WarLion) son probablemente `Condition::Check(K)` inlineado; se dejan con `.mFlags` para no arriesgar el código generado. Probar `Check` cuando se trabaje cada unidad.
- `poolcheck.py game/CharaTask` avisa de `0.0174533` y `'character not found! %s'`: ya pasaba antes (comprobado compilando el fuente anterior).
- `NewCampPartsStatus.cpp` declara su propia vista de `class CharaTask` (regla 10); no se ha tocado.

## Bajadas aceptadas

Ninguna (fuzzdrop 0, lost 0; 8180 funciones al 100%, igual que la referencia).

## Commits

- `2f5c13b` Use RSO names CharaTask::SetFlag(int) (0x803396EC) and CharaTask::GetTacViewPosition() const (0x800EF2F0)
- `b5bedbf` Use RSO name GimmickCommon::Start() (0x803EDF5C)
- `d1aa1bb` Use RSO name db::SkillBase::Condition::Check(int) const (0x80151168)
- `57e2584` Use RSO name Inventory::GetPlayerStatusInfo(int) (0x805282A8)
- `d9de194` Use RSO name CharaFaceControl::MotionPart (__dt__ 0x8011D320)
