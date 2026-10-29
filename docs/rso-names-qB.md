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
