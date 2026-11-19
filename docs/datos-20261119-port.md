# Para el port: datos-20261119

Cambios de firma, tipo, layout o comportamiento de la sesion datos-20261119 (2026-11-19 13:50 - 2026-11-20 00:10). Resumen y despues el detalle por cola.

## Resumen

- `include/math/Vec.h`: `Matrix3x4::fn_8013A13C(angle)` (0x8013A13C) **devuelve la matriz por valor** (antes era un metodo que la modificaba en el sitio); el metodo en el sitio se llama ahora `SetRotationY_unk` (siempre inline). 48 unidades tocadas; 14 llamadores fuera de linea usan `Matrix3x4 m = Matrix3x4::fn_8013A13C(a)` (cola B2).
- `atn/Color.h`: `ColorToU8_unk` con un local antes del `return` (sin cambio de comportamiento) (cola G6).
- `SetColor255_unk(atn::Color)` por valor en `D2A_TaskInstanceBase` y `atn::D2A_Task`; fuera `D2A_RGB_unk` (cola G1). GameUI: `lbl_807C8628` son 16 `atn::Color` sueltos (`sColorJobSoldier_unk`...).
- Comportamiento corregido: NewCampPartsOption::fn_80522DD8 intercambiaba los textos equivocados (mInfo[12]/[13] = textos 2 y 3; mInfo[8]/[9] = 0 y 1) (D); GameEventScript fn_803A330C (faltaba un `else`: con valor 0 se llamaba tambien a `fn_8014C0B4`) y `0.5f` -> `1.0f` en fn_803A4A7C, fn_8039B7B0, fn_8039B2BC (G5b); ControlThrownAway/Avoid ponen x y z a 0 tras `RotZ_unk` y ControlThrown pone `dir.y = 0` tras normalizar (G2).
- `ArenaTask::sInstance` (0x8087EE78) miembro estatico de la clase (antes `static` de fichero mas `extern` sueltos en BattleTask/GameScene/GameEventScript/GameTask) (C3).
- atn/Util_unk: parsers como plantillas `StrToInt_unk<T>`/`StrToFloat_unk<T>` con envoltorios; CRC-32 `fn_800DC500(const char*, unsigned long)` (A2).
- NewCampPartsStatus usa `atn::D2A_Task::SetLayoutText_unk`/`SetLayoutValue_unk` en vez de `extern "C"` (D2). CharaTask llama como miembros a `Arms::fn_80045694`, `atn::Model::fn_8009757C`, `Event::Manager::fn_80481654` (G3b). `fn_80050A1C(const Vector3*, const Segment_unk*, Vector3*)` (B). PacketPack: lista de 0x99A4 de `MP::Manager2` = `std::list` real de MSL (G8). `RandomMerchantChara` declarada despues de `gmk::RandomMerchant` (F).
- Parche sin aplicar con arreglos de logica para el port: `docs/datos-20261119-qC4-vf40.patch` (Em201ZanGurgFinal::vf40: bit 0x40000000 invertido, `fn_80266FA4` recibe el jugador cabeza, `m163C = 1` solo en la rama `m14C0 != 0`) y `docs/datos-20261119-qG2b.patch` (ControlThrust::vf0C: con `hint` falso se salta tambien el bloque del boton 4).

## Detalle por cola

- Nada (ningun cambio de firma, tipo ni layout). (datos-20261119 qA)
- atn/Util_unk: fn_800DC500 pasa de `(const void* data, unsigned long len)` a `(const char* data, unsigned long len)` (nadie la declara fuera de Util_unk.cpp). Los parsers fn_800DC12C/1DC/288/3CC siguen con la misma firma; su cuerpo esta en las plantillas `StrToInt_unk<T>`/`StrToFloat_unk<T>` del mismo .cpp. (datos-20261119 qA2)
- chr/ControlInteractionBase: `fn_80050A1C(const Vector3*, Vector3, Vector3*, Vector3)` -> la firma real `float fn_80050A1C(const Vector3* point, const Segment_unk* seg, Vector3* out)` con `Segment_unk` local (mismo struct que game/CharaTask.cpp, atn/Geometry_unk.cpp, GameScene, ControlPushedDown...: pendiente una cabecera comun). (datos-20261119 qB)
- `include/math/Vec.h`: `void atn::Matrix3x4::fn_8013A13C(float)` (rotacion en el sitio) -> `void SetRotationY_unk(float)`; `static Matrix3x4 RotY_unk(float)` -> `static Matrix3x4 fn_8013A13C(float)` (por valor). En el port: `m.fn_8013A13C(a)` -> `m.SetRotationY_unk(a)` y `Matrix3x4::RotY_unk(a)` -> `Matrix3x4::fn_8013A13C(a)`; las 14 llamadas fuera de linea usan `Matrix3x4 m = Matrix3x4::fn_8013A13C(a)` (los `Mtx` de CharaTask fn_80148CC4, ControlMoveCursor, Em306GurgBeast vf108 y Em203GreaterDaemonRedMp fn_802A3D80 pasan a `atn::Matrix3x4`). (datos-20261119 qB2)
- Sin cambios de firma, tipo ni layout. (datos-20261119 qC)
- `ArenaTask::sInstance`: antes `static ArenaTask* sInstance;` de fichero en ArenaTask.cpp y `extern "C" ArenaTask* lbl_8087EE78` en BattleTask, GameScene, GameEventScript y GameTask; ahora miembro estatico de la clase (`include/game/ArenaTask.h`) y los cuatro llamadores usan `ArenaTask::sInstance`. (datos-20261119 qC3)
- Em403WarLion::fn_803258D4 y Em401_01_WhiteLeoPard::fn_80321900 escriben la rotacion z/y/x en el sitio con `atn::Matrix3x4::RotateAxis_unk` (como QuadrupedTask::fn_805688CC); `MakeRotationZYX_unk` se quita de Em401_01_WhiteLeoPard.cpp (sin otro uso) y se queda en Em403WarLion.cpp (la usa fn_80325BA0). Mismo comportamiento. (datos-20261119 qC3)
- Em201ZanGurgFinal::vf40 (solo en el parche, no commiteado): la condicion del empujon estaba invertida (`!(msg->m0C & 0x40000000)` -> `(msg->m0C & 0x40000000) == 0x40000000`); `fn_80266FA4(this)` -> `fn_80266FA4(player)` (las piezas son del jugador cabeza, no del jefe); `m163C = 1` solo en la rama `m14C0 != 0`. (datos-20261119 qC4)
- Em201ZanGurgFinal::vf3C: `*(int*)((char*)attacker->m648 + 8)` -> `attacker->m648->m08` (CharaPart_unk). (datos-20261119 qC4)
- Sin cambios de firma ni de layout. (datos-20261119 qC4)
- NewCampPartsOption::fn_80522DD8: los indices de los intercambios estaban mal (comportamiento): el bloque de mSwap[0] cambia mInfo[6]/[7] textos 0 y 1 y mInfo[12]/[13] textos **2 y 3** (antes 0 y 1); el de mSwap[1] cambia mInfo[8]/[9] textos **0 y 1** (antes 2 y 3). Direcciones 0xEE0/0xFF0/0xF20/0x1030 y 0xA20/0xB30/0xA60/0xB70. (datos-20261119 qD)
- `atn::static_string<32, wchar_t>` en NewCampPartsOption.cpp: destructor inline vacio y copia con la fuente en un local (como en game/PopupTask.h), solo en esa unidad. (datos-20261119 qD)
- Sin cambios de firma, tipo ni layout. QuadrupedTask::fn_805688CC escribe la rotacion z/y/x en el sitio (antes `MakeRotationZYX_unk`, que se ha quitado de QuadrupedTask.cpp; mismo comportamiento). (datos-20261119 qD2)
- NewCampPartsStatus: `fn_803D70B4`/`fn_803D6EF8` (extern "C" con la tarea) -> `atn::D2A_Task::SetLayoutText_unk`/ `SetLayoutValue_unk` (los nombres reales de symbols.txt; copias debiles de GameUI); el tipo de `base` sigue siendo `atn::D2ATask_unk*` con un cast en la llamada. (datos-20261119 qD2)
- TutorialTask fn_805BC1C0: `yaw` guardado negado (`-(to - from)...y`, mismo valor de `angle`). (datos-20261119 qD2)
- `atn/SoftKeyboardU.cpp`: `sKeyChars_unk[3][4][11]` -> tres arrays `sKeyChars_unk`, `sKeyCharsFr_unk`, `sKeyCharsDe_unk` de `[4][11]` (mismo contenido y orden en `.rodata`); `fn_805028D8` los elige por idioma. (datos-20261119 qE)
- `Event/ActionEffectPlay.cpp`: `ActionColorMatrix::vf0C` copia el parametro con `SetColorMatrix_unk(mgr, param)` (static inline del .cpp); `ActionCloudMap::vf0C` guarda `m1F0C` a traves de un `Manager* mgr` local. Sin cambios de firma ni de layout. (datos-20261119 qE)
- `fn_8021DECC` en gmk/RandomMerchant: `int` -> `bool` (firma real de db/RandomItemTable_unk). (datos-20261119 qF)
- `RandomMerchantChara` declarada despues de `gmk::RandomMerchant` en `gmk/RandomMerchant.h` (con declaracion adelantada); fuera `RandomMerchant::PlayMotion3_unk`. (datos-20261119 qF)
- `lbl_8087DFD0/D4` (extern) -> literales `""` del `new` de `NewChara_unk` (inline no estatica). (datos-20261119 qF)
- gmk/FloatingBox: fuera las vistas `ModelBounds_unk` (`*(ModelBounds_unk**)((char*)&mModel + 0x16C)` -> `mModel.GetRes_unk()`) y `CharaListNode_unk` (`PlayerTask* chara`, `chara->mNext`); `Box_unk` sin ctor por valor. (datos-20261119 qF)
- `atn::D2A_TaskInstanceBase<N, M>::SetColor255_unk(const char*, D2A_RGB_unk)` -> `SetColor255_unk(const char*, atn::Color)` (`D2A_RGB_unk` borrado); `atn::D2A_Task::SetColor255_unk(const char*, const float*)` -> `(const char*, atn::Color)`. Los llamadores copian el color entero (16 bytes) por valor. (datos-20261119 qG1)
- `lbl_807C8628` (`atn::Color[16]`) -> 16 objetos `atn::Color` sueltos `sColorJobSoldier_unk` ... `sColorAttrBack2_unk` (nombres de las entradas del menu de depuracion de fn_803CFC78; 0x807C8628 + 0x10*k). (datos-20261119 qG1)
- `.data` 0x8078BE68 (`lbl_8078BE68`, 0x65C) -> `sButtonGlyphs_unk` (0x1E0), `sButtonHelp_unk` (0x1F8), `sButtonHelp2_unk` (0x284), estaticas que ya estaban en el fuente. (datos-20261119 qG1)
- Helpers borrados de GameUI.cpp: `SetInstColor_unk`, `SetWindowColor_unk`, `SetJobColor_unk`, `SetAttrColor_unk` (llamadas directas a `SetColor255_unk`). (datos-20261119 qG1)
- Sin cambios de firma ni de tipos commiteados (`math/VecAssign_unk.h` incluido en GameUI.cpp). (datos-20261119 qG1b)
- Sin cambios de firma, tipo ni layout. (datos-20261119 qG2)
- Sin cambios de firma, tipo ni layout. `ThrustHint_unk` y `Slow_unk` (structs locales del .cpp) cambian el orden de las asignaciones del ctor, no el layout. (datos-20261119 qG2b)
- Sin cambios de firma/tipo/layout. (datos-20261119 qG3)
- `PartParam_unk` (vista de `ArmsCond_unk` en CharaTask.cpp) eliminada: `fn_80045694` es `Arms::fn_80045694(int, int, int, int)` y devuelve `ArmsCond_unk*` (`m04` = valor). `chr::CharaPart_unk` sigue siendo una vista de `Arms` (CharaTask::m648): se castea a `Arms*` en `HasPartParam_unk`. (datos-20261119 qG3b)
- `fn_8009757C(atn::Model*)` -> `atn::Model::fn_8009757C()`; `fn_80481654(void*)` -> `Event::Manager::fn_80481654()` (CharaTask.cpp incluye `Event/Manager.h`). (datos-20261119 qG3b)
- `fn_8003E530(void* tree, const std::string* key, ...)` (antes `const AIString_unk* key`); el llamador fn_8001E1D0 hace el cast. (datos-20261119 qG4)
- Nuevas `static inline` en el .cpp: `CommandR_unk` (como `Command_unk` con los locales declarados al reves) y `StrLess_unk` (operator< de la clave). (datos-20261119 qG4)
- fn_8001C284: el macro `REGISTER_AI_SCRIPT` usa locales por bloque en vez de arrays de 64. (datos-20261119 qG4)
- `lbl_8074ED24`: `int[]` -> `const int[]` (include/game/EventScriptCmd_unk.h); `SetNext_unk` con `const int&`. (datos-20261119 qG5)
- 11 manejadores de GameEventScript `ScriptCmd_unk*` -> `const ScriptCmd_unk*` (fn_8039E834, fn_803A1B20, fn_803A2600, fn_803A2E20, fn_803A3654, fn_803A36D8, fn_803A39F8, fn_803A48A8, fn_803A49F4, fn_803AB1B0, fn_803AB26C); la tabla de punteros a miembro ya no lleva casts. `fn_803957F0` recorre `mScene->m80` por referencia (mismo comportamiento). (datos-20261119 qG5)
- fn_803A4A7C: el frente del golpe, los colores de los efectos y su escala son 1.0f (eran 0.5f, valores mal). (datos-20261119 qG5b)
- fn_8039B7B0/fn_8039B2BC: alfa del marcador `1 - clamp(x, 0, 1)` (era con 0.5f). (datos-20261119 qG5b)
- `fn_803A450C(ScriptResult_unk&, ScriptCmd_unk*)` -> `(ScriptResult_unk&, const ScriptCmd_unk*)`. (datos-20261119 qG5b)
- `fn_803A330C`: con valor 0 ya no llama tambien a `fn_8014C0B4` (else, como el original). (datos-20261119 qG5b)
- `atn/Color.h`: `ColorToU8_unk` mismo resultado (condicional en un local); sin cambio de firma. (datos-20261119 qG6)
- `game/GameTask.cpp`: `InClearedDungeon_unk(GameTask*)` estatica (igual que fn_803754F0) en fn_80375D0C; `FindRoutePlace_unk(GameScene*, int)` estatica (mismo resultado que `GameScene::FindPlace_unk`) en fn_8036FE74 y fn_80374710. (datos-20261119 qG6)
- EnemyQuarkStand: fuera `extern "C" fn_8000DCF4(CharaManager<PlayerTask>*)`; `fn_8000D9E8()` devuelve `PlayerManager*` y el lider es `fn_8000D9E8()->GetHead_unk()` (`game/PlayerManager.h`). (datos-20261119 qG7)
- EnemyQuarkStand fn_80351AB8: fuera `QuarkItemSlot_unk`/`QuarkSlots_unk`; el equipo de Quark es `Inventory::instance_->GetEquip_unk(2)->mProtectors` (`Inventory::EquipSlot_unk`, `mId`; armas en `[4]` = `mWeapons`). (datos-20261119 qG7)
- Sin cambios de firma, tipo ni layout. (datos-20261119 qG7b)
- `PPListImp_unk` (lista de MP::Manager2 en 0x99A4) deriva de `std::__list_owned_imp_unk<int>`; mismo layout (cuenta, centinela {prev, next}); `Clear_unk()` = borrar todos los nodos. Antes: struct propio con `mCount`/`mHead`. (datos-20261119 qG8)
- `atn::net::NetRanking_unk` con destructor inline vacio (sin cambio de comportamiento). (datos-20261119 qG8)
- `MP::Manager2::~Manager2`: la espera del hilo guarda `&mThread` en un local (mismo comportamiento). (datos-20261119 qG8)
- Sin cambios de firma, tipo ni layout. (datos-20261119 qH)
- Event/MotionData: nueva `static inline MovieTask* Create_unk(atn::Task*, const std::string&)` en el .cpp; sin cambios de firma ni de layout. (datos-20261119 qR)
- chr/ControlStagger: solo orden de locales. (datos-20261119 qR)
- Sin cambios de firma, tipo ni layout. Em003LesserDaemon: el sitio de las asignaciones a cero (mismo efecto), inlines estaticas nuevas en el .cpp y `void InitDebug_unk();` en `include/game/Em003LesserDaemon.h` (funcion inventada sin llamadores; el port puede ignorarla o usarla para el menu de depuracion). (datos-20261119 qR3)
