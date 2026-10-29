# rso-names, cola B (motor atn)

Agente B, rama `agent-B`. Referencia: `/home/claude/ref/report2.json` (8180 funciones).

## atn::Keyboard (hecho)

- `include/atn/Keyboard.h` nueva: `atn::Keyboard` con `Order()` (inline, copia debil 0x802A49BC), `Press(Port, unsigned short) const` (inline, copia debil 0x802A4968), `CheckKeyState(const KeyState&, unsigned short) const` (0x8007708C, tramo `auto_`, sin fuente), `static Keyboard* instance_` (0x8087EEF0).
- `symbols.txt`: `fn_802A4968`, `fn_802A49BC`, `fn_8007708C`, `lbl_8087EEF0` renombrados a los nombres del RSO (`Order` e `instance_` no estaban en la tabla, pero el RSO los tiene para `atn::Keyboard` y la funcion de 8 bytes solo devuelve `lbl_8087EEF0`).
- Sustituye a `GhoulKeyboard_unk`, `GargoyleKeyboard_unk`, `JillKeyboard_unk`, `QuarkKeyboard_unk` (EnemyQuarkSecond y EnemyQuarkStand), `LibraryWatch_unk` (LibraryTask y SoftKeyboardBase) y `MindowsKeyboard_unk` (MindowsCommon: `m9C` = `mKeyState[4]`).
- Decisiones sin el usuario: `Port` con enumeradores inventados `PORT0_unk..PORT4_unk`; `KeyState` (0x1A bytes) deducido del asm de `CheckKeyState` (bit i de `mFlags` habilita `mKeys[i]`, u16 en 0xA). El nombre real `On` (otro metodo del RSO) no tiene direccion en el `.dol`.
- `SoftKeyboardBase fn_800C6128`: con `Press(Port, unsigned short)` el parametro `int` metia un `clrlwi`; el parametro real es `wchar_t` (el que se le pasa es `SoftKey_unk::m04`, `wchar_t`): renombrado `fn_800C6128__Fi` -> `fn_800C6128__Fw` y sigue al 100%.
- Sin cambios en cifras: 8180, lost 0, fuzzdrop 0, ordercheck igual que antes en las 8 unidades.

## atn::Model (hecho)

- `Model_unk` era `atn::Model` (destructor 0x800971D4 con `__vt__Q23atn5Model`): renombrada la clase en `include/atn/Model_unk.h` (el fichero conserva el nombre para no tocar ~180 `#include`) y en todos sus usos (178 ficheros); `symbols.txt`: 64 nombres mangled `9Model_unk` -> `Q23atn5Model`. `chr::Motion_unk` sigue como alias.
- `db/OptionParts_unk.cpp` tenia su propia vista `struct Model_unk` (solo `fn_8008B130`): ahora `atn::Model` en ese mismo bloque (no se incluye la cabecera para no arrastrar conflictos de `Color4f_unk`); pendiente de unificar.
- `MP/D2AMpMenuCockpit.cpp` declaraba `extern "C" fn_80092814__9Model_unkFPCci` (regla 10): ahora `MpChara_unk::mModel` es `atn::Model` y se llama al metodo.
- Metodos: `GetMotionFrame(int) const` (0x80139550), `SetMotionSpeed(int, float)` (0x80139F3C), `SetMotionFrame(int, float)` (0x80357F38; era `extern "C"` con `self` en EnemyTasha, ahora miembro definido en el mismo sitio) y, fuera de la cola, `GetSpeed_unk` -> `GetMotionSpeed(int) const` (copia debil 0x80224628, ya nombrada en `symbols.txt`).
- `CalcMatrixStack`: tipo real del callback `boost::function<void(Matrix3x4*, const NodeChunk&, const ModelNodeCallbackContext&)>` (typedef `atn::ModelNodeCallback_unk`). Nueva `atn::ModelBase` (vtable `__vt__Q23atn9ModelBase` en el `.dol`) con `virtual ~ModelBase()` y `CalcMatrixStack(const MotionBuffer*, callback)` (0x800902C0); `atn::Model : ModelBase` con `CalcMatrixStack(callback)` (0x8000D430, llama a la de la base con `m220`) y `using ModelBase::CalcMatrixStack`. Los demas miembros siguen en `Model` (no se sabe cuales son de la base). Antes eran `extern "C"` con `boost::function<void()>`. Sin bajadas.

## atn::Task::FreezeTask (hecho)

- `void FreezeTask() { mFlags |= 4; }` inline en `include/atn/Task.h` (copia debil 0x80244CAC). Quitados el `extern "C" inline` de Em001Necromancer y el `extern "C"` de EnemyQuarkStand. Em001Necromancer ya no emitia la copia antes (sigue `-`, igual que sus vecinas `fn_80244CA0`/`fn_80244CBC`); EnemyQuarkStand ahora emite la copia debil; cifras y ordercheck iguales.

## atn::MemoryManager::Instance / Free (hecho)

- `MemoryManager_unk` = `atn::MemoryManager` (constructor 0x80082970 y destructor 0x80082BE0 renombrados tambien en `symbols.txt`, y `@GUARD@`/`@LOCAL@` de `sInstance`). Nueva `include/atn/MemoryManager.h` con el layout de `atn/SmallAllocator_8.cpp`, `static MemoryManager* Instance()` (0x800827E0) y `void Free(void*)` (0x80083AD4); la plantilla `atn::SmallAllocator<N>` (y `SmallPool_unk`/`SmallPoolConfig_unk`) pasa a `include/atn/SmallAllocator.h`; `OSMutex_unk` (0x18 bytes) y `MEMAllocator_unk` a `MemoryManager.h`.
- 24 ficheros: quitadas las vistas locales (`HeapInfo_unk`: `mHeap0/1/2` = `mMem1/mMem2/mOverflow`; `HomeButton_unk`: `mAllocator` = `mSdk`; `DwcAllocator_unk`; `AudioDevice` repetia `OSMutex_unk`), los `extern "C" fn_800827E0/fn_80083AD4` y las declaraciones adelantadas; `fn_80083AD4(fn_800827E0(), p)` -> `atn::MemoryManager::Instance()->Free(p)`.
- Exigido por el renombrado (fuera de la cola): `atn/LinearAllocator.cpp` (enlazada) tenia su propia `class IAllocator` con `vf0C(size, align, heap, name)`, incompatible con `include/IAllocator.h` (`vf0C(size, align)`). Como todas las llamadas de `SmallAllocator_8` (via `NamedAllocator_unk`) y `DwcAllocator_unk` pasan heap y nombre, la firma real es la de 4 parametros: `include/IAllocator.h`, `DLAllocator` y `SmallAllocator<N>` pasan a `vf0C(unsigned long, unsigned long, int, const char*)` (renombrados `vf0C__...FUlUl` -> `FUlUliPCc` en `symbols.txt`) y LinearAllocator usa la cabecera comun. Mismo codigo (los parametros nuevos no se usan).
- No hecho: `AllocFromMem2(UlUl MemoryTag PCcPCcii)` del RSO es casi seguro `fn_800839EC` (size, align, heap, name, file, line, mayFail), pero no estaba en la cola y necesita el tipo `MemoryTag`; los demas `extern "C"` con `self` de la clase (`fn_800832AC`, `fn_800838B8`, `fn_800839EC`, `fn_80082CAC`, `fn_800838AC`) siguen igual.
- Sin bajadas; ordercheck igual en SmallAllocator_8, DLAllocator, LinearAllocator, HomeButton_unk, AudioDevice, HeapInfo_unk, DwcAllocator_unk.

## atn::FileFreeBuffer (hecho)

- `void atn::FileFreeBuffer(char* buf, bool own)` (0x8006BB6C) declarada en `include/atn/FileList.h` (su split) y definida en `atn/FileList.cpp` en el mismo sitio; sigue al 100% con `bool` (`cmpwi r4,0`). 13 ficheros: quitados los `extern "C" fn_8006BB6C` (y el `void fn_8006BB6C(void*, int)` C++ de `auto_03_803B27AC_text.cpp`, que daba otro nombre mangled); llamadas `atn::FileFreeBuffer((char*)p, false)`.

## atn::EfpFile (hecho)

- `atn::EfpHandle_unk` = `atn::EfpFile`: renombrada en `include/atn/EfpHandle_unk.h` (el fichero conserva el nombre) y en 172 ficheros; `fn_8023781C` -> `Destroy()` (0x8023781C). `symbols.txt`: 11 nombres, incluidos los de plantillas (`scoped_ptr<...>`, `Vector_unk<...>`) con el prefijo de longitud recalculado (herramienta propia `mrename.py` en el scratchpad del agente: sustituye un token mangled y corrige los prefijos de las plantillas que lo contienen). `game_units.json`: clase de `atn/EfpHandle_unk.cpp`.
- Candidatos sin aplicar (no estaban en la cola): `fn_8023780C(const char*)` = `LoadRequest`, `fn_80237874()` = `IsLoading` (RSO), por el comportamiento ya anotado.

## atn::SimpleVector (hecho: clase y `operator[]`)

- `atn::Vector_unk<T>` = `atn::SimpleVector<T>` (`include/atn/Array_unk.h`, 26 ficheros, 58 nombres de `symbols.txt` con prefijos recalculados). `operator[](unsigned int)` -> `operator[](int)` (firma del RSO `__vc__...Fi`); los 4 `__vc__` de MotionCommand_unk (0x801207BC, 0x80120D24, 0x80120E38, 0x80120F4C) siguen al 100% con el nombre nuevo.
- Sin aplicar (no en la cola, firmas distintas): en el RSO `reserve`/`resize` son `(unsigned int, MemoryTag, const char*)` (nuestros `reserve(Ui, int)` y `Resize_unk(Ui, int, int)`: el tercer argumento "que nunca se lee" es el `const char*`); `__vc__` de `SimpleVector<AudioDevice::LoadBankQueue>` es `const` (`CFi`); `SimpleVector<atn::EffFile>` existe en el RSO (posible nombre real de `ResLoader_unk`, sin comprobar).

## atn::static_string<32, char, std::char_traits<char>> (hecho)

- `MotionCmdElemD_unk` = `atn::static_string<32>`: nueva plantilla `include/atn/static_string.h` (+ `static_string_fwd.h` con los argumentos por defecto `char`, `std::char_traits<C>`); borrada `include/game/MotionCmdElemD_unk.h`; quitada la vista local de `atn/LH_MotionCommandData.cpp`; constructor y destructor siguen como inline fuera de clase en `game/AreaEffConfig.cpp` (ahora como miembros de plantilla). 16 nombres de `symbols.txt`; `__vc__Q23atn64SimpleVector<Q23atn42static_string<32,c,Q23std14char_traits<c>>>Fi` (0x80120F4C) queda igual que en el RSO.
- Hallazgo: incluir la definicion completa en `game/GimmickCommon.h` (antes solo `struct MotionCmdElemD_unk;`) cambiaba la asignacion de registros de `GimmickCommon::fn_803EDB18` (unidad enlazada, `main.dol: FAILED`; las declaraciones `extern "C" strlen/memcpy` y la plantilla quedaban antes en la unidad). Con solo la declaracion adelantada (`static_string_fwd.h`) vuelve al 100%. Ojo: con `main.dol: FAILED` el `report.json` no se regenera y `regress.py`/`fuzzdrop.py` comparan el viejo.
- Pendiente (fuera de la cola): `include/gmk/AutomaticDoor.h`, `MotionObject.h`, `RandomMerchant.h`, `SeTable.h` tienen copias propias de la misma cadena fija (probablemente otros `static_string<N>`).

## atn::MotionCommandData::Command (hecho)

- `MotionCmdData_unk` = `atn::MotionCommandData` y `MotionCmdEntry_unk` = `atn::MotionCommandData::Command` (en `include/game/MotionCmdData_unk.h`, que conserva el nombre). Decision sin el usuario: la clase contenedora se deduce del RSO, cuyos `SimpleVector` de esa familia son exactamente `<MotionCommandData::Command>`, `<EffFile>`, `<EfpFile>` y `<static_string<32>>`, los cuatro miembros de la estructura. `atn/LH_MotionCommandData.cpp` tenia vistas locales de `MotionCmdData_unk`, `ResLoader_unk` (4 bytes en vez de 0xC) y del elemento (`MotionCmdElemA_unk`): ahora incluye la cabecera comun. `symbols.txt`: 7 nombres; `__vc__Q23atn47SimpleVector<Q33atn17MotionCommandData7Command>Fi` (0x801207BC) igual que en el RSO. Sin cambios en cifras.
- Candidato (no aplicado): `ResLoader_unk` = `atn::EffFile` (mismo patron que `EfpFile`: recurso + `LH_Effect` en 0x04).

## atn::CsvBase::SetData (hecho)

- `CsvBase<...>::vf08(const T*, int)` -> `virtual void SetData(const T*, unsigned int)` (firma del RSO) en `include/atn/Csv.h`, y los overrides de `CsvU`/`CsvTabU`. 7 nombres en `symbols.txt` (0x800420F4, 0x8004476C del RSO y, por coherencia, los de `<w,44>`, `<c,124>`, `<w,9>`, `CsvU`, `CsvTabU`); llamadas `csv.vf08(...)` -> `csv.SetData(...)` en ~60 ficheros. Mismo codigo.
- Sin aplicar: el RSO tambien da `GetNextCell` (= `fn_8005B9CC`), `GetCurrentCell` (= `GetField_unk`), `NextLine` (= `fn_8005BBF8`), `SetTrimStr(const char*)` (= `fn_8005BCE8`) y los constructores `Csv(const char*, unsigned int)`/`CsvTab(...)`: fuera de la cola.

## std::__red_black_tree::balance_insert (no hecho)

- `fn_8003E120` (AI_archer_attack01, al 100%) es `std::__red_black_tree<std::__tree_node_base<std::allocator<void*>>*>::balance_insert(node*, node*)`, un miembro estatico de una plantilla de MSL `<tree>`. No es un renombrado: hace falta declarar `__tree_node_base`/`__red_black_tree` (la cabecera `tree` de MSL, ver `docs/mwcc-blockers.md`, "Arbol del mapa de scripts"), reescribir el cuerpo sobre `__tree_node_base` y, como en esta unidad nadie la llama (el `map::insert` `fn_8003DD50` no esta hecho), solo se emitiria con una instanciacion explicita, que el original no tenia. Ademas la llaman con sus propios tipos de nodo `game/TownTask.cpp`, `atn/ptr_string_base_char_0.cpp` y `atn/AudioDevice.cpp`. Se deja para cuando se haga el `std::map` de scripts.

## Resumen

- Hechos: atn::Keyboard (Press, CheckKeyState, y Order/instance_), atn::Model (clase; GetMotionFrame, SetMotionSpeed, SetMotionFrame, CalcMatrixStack; ademas GetMotionSpeed) y atn::ModelBase::CalcMatrixStack, atn::Task::FreezeTask, atn::MemoryManager (clase; Instance, Free), atn::FileFreeBuffer, atn::EfpFile (clase; Destroy, __dt__), atn::SimpleVector (clase; `__vc__` con `int`), atn::static_string<32>, atn::MotionCommandData::Command, atn::CsvBase::SetData.
- No hecho: `balance_insert` (arriba).
- Bajadas aceptadas: ninguna. En todos los grupos: 8180/18215, lost 0, fuzzdrop 0, `main.dol: OK`, ordercheck igual que antes en las unidades tocadas.
- Cambios fuera de la cola exigidos por los renombrados: `IAllocator::vf0C` a 4 parametros (LinearAllocator), `fn_800C6128` con `wchar_t` (SoftKeyboardBase), vistas locales unificadas (OptionParts_unk, D2AMpMenuCockpit, HeapInfo_unk, HomeButton_unk, DwcAllocator_unk, AudioDevice, LH_MotionCommandData).

## Hallazgos (para `docs/trucos-mwcc.md`)

- Incluir antes en la unidad la definicion de una plantilla con inlines (`static_string` con `strlen`/`memcpy`) puede cambiar la asignacion de registros de una funcion no relacionada (`GimmickCommon::fn_803EDB18`); una declaracion adelantada lo evita.
- Con `main.dol: FAILED` el `report.json` no se regenera: `regress.py`/`fuzzdrop.py` dan "0" comparando el viejo. Mirar siempre la linea de `main.dol` (o la hora de `report.json`) antes de fiarse.
- Renombrar una clase que aparece como argumento de plantilla obliga a recalcular el prefijo de longitud de la plantilla en `symbols.txt` (`Q25boost33scoped_ptr<Q23atn13EfpHandle_unk>` -> `26scoped_ptr<Q23atn7EfpFile>`).
