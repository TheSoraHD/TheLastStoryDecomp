# rso-names, cola B (motor atn)

Agente B, rama `agent-B`. Referencia: `/home/claude/ref/report2.json` (8180 funciones).

## atn::Keyboard (hecho)

- `include/atn/Keyboard.h` nueva: `atn::Keyboard` con `Order()` (inline, copia debil 0x802A49BC), `Press(Port, unsigned short) const` (inline, copia debil 0x802A4968), `CheckKeyState(const KeyState&, unsigned short) const` (0x8007708C, tramo `auto_`, sin fuente), `static Keyboard* instance_` (0x8087EEF0).
- `symbols.txt`: `fn_802A4968`, `fn_802A49BC`, `fn_8007708C`, `lbl_8087EEF0` renombrados a los nombres del RSO (`Order` e `instance_` no estaban en la tabla, pero el RSO los tiene para `atn::Keyboard` y la funcion de 8 bytes solo devuelve `lbl_8087EEF0`).
- Sustituye a `GhoulKeyboard_unk`, `GargoyleKeyboard_unk`, `JillKeyboard_unk`, `QuarkKeyboard_unk` (EnemyQuarkSecond y EnemyQuarkStand), `LibraryWatch_unk` (LibraryTask y SoftKeyboardBase) y `MindowsKeyboard_unk` (MindowsCommon: `m9C` = `mKeyState[4]`).
- Decisiones sin el usuario: `Port` con enumeradores inventados `PORT0_unk..PORT4_unk`; `KeyState` (0x1A bytes) deducido del asm de `CheckKeyState` (bit i de `mFlags` habilita `mKeys[i]`, u16 en 0xA). El nombre real `On` (otro metodo del RSO) no tiene direccion en el `.dol`.
- `SoftKeyboardBase fn_800C6128`: con `Press(Port, unsigned short)` el parametro `int` metia un `clrlwi`; el parametro real es `wchar_t` (el que se le pasa es `SoftKey_unk::m04`, `wchar_t`): renombrado `fn_800C6128__Fi` -> `fn_800C6128__Fw` y sigue al 100%.
- Sin cambios en cifras: 8180, lost 0, fuzzdrop 0, ordercheck igual que antes en las 8 unidades.
