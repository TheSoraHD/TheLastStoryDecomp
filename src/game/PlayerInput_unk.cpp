#include "game/Input_unk.h"
#include "game/SetConfig.h"
#include "game/CharaManager.h"
#include "game/CharaTask.h"
#include "atn/Camera_unk.h"
#include "string.h"

extern "C" void* lbl_8087EF70; // inferred: pad
extern "C" int fn_800A555C(void* pad, int port, int button); // inferred: button triggered
extern "C" int fn_800A5584(void* pad, int port, int button);
extern "C" int fn_800A55AC(void* pad, int port, int button); // inferred: button repeated
extern "C" int fn_800A55D4(void* pad, int port, int button); // inferred: button held
extern "C" float fn_800A56A8(void* pad, int port, int axis, int arg); // inferred: stick axis
extern "C" int fn_800A5870(void* pad, int port, int axis, float threshold); // inferred: remote swung
extern "C" int fn_800A58D0(void* pad, int port); // inferred: classic controller connected
extern "C" float PSVECMag(const atn::Vector3* v);

// inferred: field (lbl_8087F430), only the members read here
struct Field_unk {
    unsigned char m0000[0x54E4];
    int m54E4; // inferred: nonzero while a menu of the field is open
};
extern "C" Field_unk* lbl_8087F430;
extern "C" int fn_80370174(Field_unk* field, int id); // inferred: field flag
// inferred: tactics view (lbl_8087F9F8, as in game/CharaTask.cpp)
struct TacView_unk {
    unsigned char m000[0xA70];
    int mA70; // inferred: nonzero while the view is active
};
extern "C" TacView_unk* lbl_8087F9F8;
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager

// inferred: settings (lbl_8087F0A8), only the members read here
struct SettingsOpt_unk {
    unsigned char m00[0x34];
    int m34;
    int m38;
};
struct Settings_unk {
    unsigned char m000[0x244];
    SettingsOpt_unk m244;
};
extern "C" Settings_unk* lbl_8087F0A8;

// inferred: part of SetConfig from m24 (mAttackType at 0x04, mDiveType at 0x08, mGatheringType at 0x10)
struct ControlConfig_unk {
    int m00;
    int m04;
    int m08;
    int m0C;
    int m10;
};
// inferred: part of SetConfig from mShowDialog (mEventSkipType at 0x08)
struct DialogConfig_unk {
    int m00;
    int m04;
    int m08;
};

extern "C" int fn_801237E0(SetConfig* config);
extern "C" DialogConfig_unk* fn_801237E8(SetConfig* config);

// inferred: camera of the game (lbl_8087EFB4); weak copies of another file
extern "C" void* lbl_8087EFB4;
extern "C" inline void* fn_8008B964() {
    return lbl_8087EFB4;
}
extern "C" inline Camera_unk* fn_800F7258(void* camera) {
    return (Camera_unk*)((char*)camera + 0x104);
}
// inferred: weak copy of another file
extern "C" inline Settings_unk* fn_800F52F8() {
    return lbl_8087F0A8;
}
// inferred: weak copies 0x801156BC and 0x801156C4 of Event/Path.cpp
inline void* GetPad_unk() {
    return lbl_8087EF70;
}
inline SetConfig* GetSetConfig_unk() {
    return SetConfig::sInstance;
}
// inferred: the leader is controlled by the game (field and tactics view idle)
inline int IsLocked_unk() {
    if (lbl_8087F430 && !lbl_8087F430->m54E4 && lbl_8087F9F8 && !lbl_8087F9F8->mA70) {
        CharaTask* leader = lbl_8087F8A0 ? (CharaTask*)lbl_8087F8A0->mHead : 0;
        if (leader) {
            return leader->fn_8017C974() == 1;
        }
    }
    return 0;
}

// inferred: field (weak copy, also in the enemy files)
extern "C" Field_unk* fn_80121F00() {
    return lbl_8087F430;
}
// inferred: button triggered (0x21: no button); `lock` ignores it while the leader is locked
extern "C" int fn_80121F08(int button, int lock) {
    if (button == 0x21 || (lock && IsLocked_unk())) {
        return 0;
    }
    return fn_800A555C(GetPad_unk(), 0, button) != 0;
}
// inferred: button repeated
extern "C" int fn_80121FD4(int button, int lock) {
    if (button == 0x21 || (lock && IsLocked_unk())) {
        return 0;
    }
    return fn_800A55AC(GetPad_unk(), 0, button) != 0;
}
extern "C" int fn_801220A0(int button, int lock) {
    if (button == 0x21 || (lock && IsLocked_unk())) {
        return 0;
    }
    return fn_800A5584(GetPad_unk(), 0, button) != 0;
}
// inferred: button held
extern "C" int fn_8012216C(int button, int lock) {
    if (button == 0x21 || (lock && IsLocked_unk())) {
        return 0;
    }
    return fn_800A55D4(GetPad_unk(), 0, button) != 0;
}
// inferred: button of the tactics view
extern "C" int fn_80122238() {
    if (fn_800A58D0(GetPad_unk(), 0)) {
        return 0xF;
    }
    return GetSetConfig_unk()->mInTacviewType == 0;
}
extern "C" ControlConfig_unk* fn_80122280(SetConfig* config) {
    return (ControlConfig_unk*)&config->m24;
}
// inferred: field flag 0xD0 not set
extern "C" int fn_80122288() {
    int result = 1;
    if (fn_80121F00() && fn_80370174(fn_80121F00(), 0xD0)) {
        result = 0;
    }
    return result;
}

Input_unk::Input_unk() : m00(0), m04(0), m08(0), m0C(0.0f), m10(0), m4C(0) {
    memset(m14, 0, sizeof(m14));
}

// inferred: part of the field input (weak copies)
struct FieldInput_unk;
struct FieldCursor_unk {
    float m00;
    float m04;
    float m08;
    float m0C;
};
extern "C" inline FieldInput_unk* fn_80122550(Field_unk* field) {
    return (FieldInput_unk*)((char*)field + 0x6C);
}
extern "C" inline FieldCursor_unk* fn_80122558(FieldInput_unk* input) {
    return (FieldCursor_unk*)((char*)input + 0x51C);
}
extern "C" inline float fn_80122560(float x) {
    double d = __fabs(x);
    return d;
}

// NONMATCHING: 99.2% - the original branches to the yaw update on `ble` and jumps over it with `b`
void Input_unk::fn_80122330(int reset) {
    if (reset) {
        m00 = 0;
        fn_80124BE4();
        fn_8012256C();
        return;
    }
    if (fn_801240B4(0x11) && fn_80122288()) {
        m00++;
    } else {
        m00 = 0;
    }
    if (fn_8012216C(fn_80122C70(8), 1) || fn_801220A0(fn_80122C70(8), 1)) {
        m04++;
    } else {
        m04 = 0;
    }
    if (fn_801240B4(3)) {
        m08++;
    } else {
        m08 = 0;
    }
    if (fn_801240B4(7)) {
        m4C++;
    } else {
        m4C = 0;
    }
    float yaw = 0.0f;
    if (fn_80121F00()) {
        FieldInput_unk* input = fn_80122550(fn_80121F00());
        yaw = fn_80122558(input)->m0C / 5.0f;
        yaw *= 0.12566371f;
        yaw *= 40.0f / fn_80122558(input)->m00;
    }
    if (fn_801248DC(0) < 0.4f) {
        m0C = yaw + fn_800F7258(fn_8008B964())->GetRot_unk()->y;
        m10--;
    } else {
        yaw += fn_800F7258(fn_8008B964())->GetRot_unk()->y;
        if (!(m10 > 0 && fn_80122560(atn::WrapAngle_unk(yaw - m0C)) > 0.7853982f)) {
            m0C = yaw;
            m10--;
        }
    }
    fn_8012256C();
}

// inferred: body of fn_801240B4, also inlined in fn_8012256C
inline int Input_unk::Held_unk(int action) {
    switch (action) {
    case 4:
        return fn_8012216C(fn_80122C70(action), 1);
    case 7:
    case 36:
        if (fn_800A58D0(GetPad_unk(), 0)) {
            int held = fn_8012216C(0xB, 1);
            if (!held) {
                held = fn_8012216C(fn_80122C70(action), 1);
            }
            return held;
        }
        return fn_8012216C(fn_80122C70(action), 1);
    case 18:
        if (fn_80122280(GetSetConfig_unk())->m10 == 0) {
            return fn_8012216C(9, 1) && m00 >= 10;
        }
        return fn_8012216C(6, 1) && m00 >= 10;
    case 35:
        return fn_8012216C(4, 1) && m08 >= 8;
    case 39:
    case 40: {
        int skip = fn_801237E8(GetSetConfig_unk())->m08;
        if (skip < 1 || skip > 2) {
            return 0;
        }
        return fn_8012216C(fn_80122C70(action), 1);
    }
    default:
        return fn_8012216C(fn_80122C70(action), 1);
    }
}

void Input_unk::fn_8012256C() {
    for (int i = 0; i < 0x36; i++) {
        if (m14[i]) {
            m14[i] = 0;
            if (Held_unk(i)) {
                m14[i] = 1;
            } else {
                m14[i] = 0;
            }
        }
    }
}

// NONMATCHING: 99.3% - case 44: the original keeps `li r3, 0` / `beq` / `li r3, 7`; MWCC makes it branchless
int Input_unk::fn_80122C70(int action) {
    if (m14[action]) {
        return 0x21;
    }
    Settings_unk* settings = lbl_8087F0A8;
    switch (action) {
    case 0:
        return 4;
    case 1:
        return 5;
    case 2:
        if (GetSetConfig_unk()->mAttackType == 0) {
            return 4;
        }
        return fn_80122238();
    case 3:
        return 4;
    case 4:
        return GetSetConfig_unk()->mAttackType == 0 ? 0x21 : 4;
    case 5:
        return 4;
    case 6:
        return 4;
    case 7:
        return fn_800A58D0(GetPad_unk(), 0) ? 0x20 : 5;
    case 8:
        return GetSetConfig_unk()->mGatheringType == 0 ? 6 : 9;
    case 9:
        switch (GetSetConfig_unk()->mWindKey) {
        case 0:
            return 4;
        case 1:
            return 4;
        case 2:
            return 6;
        }
        break;
    case 10:
        return settings->m244.m34 ? 0x21 : 0xE;
    case 11:
        return 0xF;
    case 12:
        return 0xF;
    case 14:
        return 0;
    case 15:
        return 1;
    case 16:
        return fn_800A58D0(GetPad_unk(), 0) ? 0xE : 0xF;
    case 17:
        return GetSetConfig_unk()->mGatheringType == 0 ? 9 : 6;
    case 18:
        return GetSetConfig_unk()->mGatheringType == 0 ? 9 : 6;
    case 19:
        if (GetSetConfig_unk()->mAttackType == 0) {
            return 4;
        }
        return fn_80122238();
    case 20:
        return fn_80122C70(2);
    case 21:
        if (GetSetConfig_unk()->mGatheringType == 0) {
            return fn_80122288() ? 9 : 0x21;
        }
        return fn_80122288() ? 6 : 0x21;
    case 22:
        if (GetSetConfig_unk()->mAttackType == 0) {
            return 4;
        }
        return fn_80122238();
    case 23:
        return 0;
    case 24:
        return 1;
    case 25:
        return 1;
    case 26:
        return 2;
    case 27:
        return 3;
    case 28: {
        int button = 0;
        if (GetSetConfig_unk()->mAttackType != 0) {
            button = 3;
        }
        return button;
    }
    case 29: {
        int button = 1;
        if (GetSetConfig_unk()->mAttackType != 0) {
            button = 2;
        }
        return button;
    }
    case 30:
        return GetSetConfig_unk()->mGatheringType == 0 ? 9 : 6;
    case 32:
        return settings->m244.m38 ? 4 : 0x21;
    case 31:
        return settings->m244.m38 ? 5 : 4;
    case 35:
        return 4;
    case 36:
        return fn_80122C70(7);
    case 37:
        return 0xC;
    case 38:
        return 0xD;
    case 39:
    case 41:
        return 4;
    case 40:
    case 42:
        return fn_800A58D0(GetPad_unk(), 0) ? 0xF : 0xE;
    case 43:
    case 45:
        return fn_800A58D0(GetPad_unk(), 0) ? 0xD : 0xE;
    case 44:
        if (fn_800A58D0(GetPad_unk(), 0)) {
            return 7;
        }
        return 0;
    case 46:
        return fn_800A58D0(GetPad_unk(), 0) ? 0xF : 0xE;
    case 48:
        return fn_800A58D0(GetPad_unk(), 0) ? 0xF : 0xE;
    case 49:
        if (fn_800A58D0(GetPad_unk(), 0)) {
            return 0;
        }
        return GetSetConfig_unk()->mInTacviewType != 0;
    case 50:
        return fn_800A58D0(GetPad_unk(), 0) ? 6 : 0xA;
    case 51:
        return fn_800A58D0(GetPad_unk(), 0) ? 9 : 0x1F;
    case 52:
        return fn_800A58D0(GetPad_unk(), 0) ? 7 : 0xF;
    case 53:
        return fn_800A58D0(GetPad_unk(), 0) ? 8 : 0xE;
    }
    return 0x21;
}

extern "C" SettingsOpt_unk* fn_801231B0(Settings_unk* settings) {
    return &settings->m244;
}
extern "C" int fn_801231B8(SetConfig* config) {
    return config->mAttackType == 0;
}
extern "C" int fn_801231C8(SetConfig* config) {
    return config->mWindKey;
}

// NONMATCHING: 99.5% - actions 39/40: the original places the `return 0` before the call
int Input_unk::fn_801231D0(int action) {
    SettingsOpt_unk* opt = fn_801231B0(fn_800F52F8());
    switch (action) {
    case 4:
        return fn_80121F08(fn_80122C70(action), 1);
    case 7:
    case 36: {
        int result;
        if (fn_800A58D0(GetPad_unk(), 0)) {
            result = 0;
            if (fn_80121F08(0xB, 1) || fn_80121F08(fn_80122C70(action), 1)) {
                result = 1;
            }
        } else {
            result = fn_80121F08(fn_80122C70(action), 1);
        }
        return result;
    }
    case 18: {
        int result;
        if (fn_80122280(GetSetConfig_unk())->m10 == 0) {
            result = 0;
            if (fn_8012216C(9, 1) && m00 == 10) {
                result = 1;
            }
        } else {
            result = 0;
            if (fn_8012216C(6, 1) && m00 == 10) {
                result = 1;
            }
        }
        return result;
    }
    case 22:
        switch (fn_80122280(GetSetConfig_unk())->m08) {
        case 0:
            if (fn_80121F08(fn_80122238(), 1) && fn_801248DC(0) > 0.4f) {
                return 1;
            }
            if (fn_80121F08(fn_80122C70(action), 1) && fn_801248DC(0) > 0.4f) {
                return 1;
            }
            break;
        case 1:
            if (fn_80121F08(fn_80122238(), 1) && fn_801248DC(0) > 0.4f) {
                return 1;
            }
            if (fn_801248DC(0) > 0.4f) {
                int result = 0;
                if ((fn_801240B4(7) && fn_80121F08(fn_80122C70(action), 1)) ||
                    (fn_8012216C(fn_80122C70(action), 1) && fn_801231D0(7))) {
                    result = 1;
                }
                return result;
            }
            break;
        }
        break;
    case 11:
        if (atn::Abs_unk(fn_800A56A8(GetPad_unk(), 0, 4, 0)) > 2.0f && fn_801237E0(GetSetConfig_unk())) {
            return 1;
        }
        return fn_80121F08(fn_80122C70(action), 1);
    case 12:
        if (fn_80121F08(fn_80122C70(1), 1)) {
            return 1;
        }
        break;
    case 13:
        if (!fn_80121F08(fn_80122C70(0), 1) && fn_80121FD4(fn_80122C70(0), 1)) {
            return 1;
        }
        break;
    case 32:
        if (opt->m38) {
            if (!fn_801231B8(GetSetConfig_unk()) && !m14[0x16] && fn_8012216C(fn_80122238(), 1)) {
                return 1;
            }
            return fn_80121F08(fn_80122C70(0x20), 1);
        } else {
            float value = -fn_800A56A8(GetPad_unk(), 0, 6, 0);
            value += fn_800A56A8(GetPad_unk(), 0, 5, 0);
            return value > 0.6f;
        }
    case 33:
        if (opt->m38) {
            int result;
            if (fn_801231B8(GetSetConfig_unk())) {
                result = !fn_8012216C(4, 1);
            } else {
                result = 0;
                if (!fn_8012216C(fn_80122238(), 1) && !fn_8012216C(4, 1)) {
                    result = 1;
                }
            }
            return result;
        } else {
            return fn_800A56A8(GetPad_unk(), 0, 6, 0) > 2.0f;
        }
    case 35: {
        int result = 0;
        if (fn_8012216C(4, 1) && m08 == 8) {
            result = 1;
        }
        return result;
    }
    case 8: {
        int result;
        if (fn_801231C8(GetSetConfig_unk()) == 2) {
            result = 0;
            if (fn_801220A0(fn_80122C70(action), 1) && m04 < 7) {
                result = 1;
            }
        } else {
            result = fn_80121F08(fn_80122C70(action), 1);
        }
        return result;
    }
    case 47: {
        int result = 0;
        if (fn_80121F08(6, 1) || fn_80121F08(9, 1)) {
            result = 1;
        }
        return result;
    }
    case 16: {
        int button = fn_80122C70(action);
        return fn_800A555C(GetPad_unk(), 0, button) != 0;
    }
    case 39:
    case 40: {
        int skip = fn_801237E8(GetSetConfig_unk())->m08;
        if (skip < 1 || skip > 2) {
            break;
        }
        return fn_80121F08(fn_80122C70(action), 1);
    }
    case 43:
    case 45:
        return fn_80121F08(fn_80122C70(action), 0);
    default:
        return fn_80121F08(fn_80122C70(action), 1);
    }
    return 0;
}

extern "C" int fn_801237E0(SetConfig* config) {
    return config->mSwingReplay;
}
extern "C" DialogConfig_unk* fn_801237E8(SetConfig* config) {
    return (DialogConfig_unk*)&config->mShowDialog;
}

int Input_unk::fn_801237F0(int action) {
    switch (action) {
    case 4:
        return fn_80121FD4(fn_80122C70(action), 1);
    case 18:
        if (fn_80122280(GetSetConfig_unk())->m10 == 0) {
            return fn_8012216C(9, 1) && m00 == 10;
        }
        return fn_8012216C(6, 1) && m00 == 10;
    case 35:
        return fn_8012216C(4, 1) && m08 == 8;
    case 39:
    case 40: {
        int skip = fn_801237E8(GetSetConfig_unk())->m08;
        if (skip < 1 || skip > 2) {
            return 0;
        }
        return fn_80121FD4(fn_80122C70(action), 1);
    }
    default:
        return fn_80121FD4(fn_80122C70(action), 1);
    }
}

int Input_unk::fn_80123C7C(int action) {
    switch (action) {
    case 4:
        return fn_801220A0(fn_80122C70(action), 1);
    case 18:
        if (fn_80122280(GetSetConfig_unk())->m10 == 0) {
            return fn_801220A0(9, 1);
        }
        return fn_801220A0(6, 1);
    case 35:
        return fn_801220A0(4, 1);
    case 39:
    case 40: {
        int skip = fn_801237E8(GetSetConfig_unk())->m08;
        if (skip < 1 || skip > 2) {
            return skip == 0;
        }
        return fn_801220A0(fn_80122C70(action), 1);
    }
    default:
        return fn_801220A0(fn_80122C70(action), 1);
    }
}

int Input_unk::fn_801240B4(int action) {
    return Held_unk(action);
}

int Input_unk::fn_8012476C(int action) {
    if (action == 0x22) {
        if (GetSetConfig_unk()->mShakeType == 0) {
            if (IsLocked_unk()) {
                return 0;
            }
            return fn_800A5870(GetPad_unk(), 0, 4, 2.0f) != 0;
        } else {
            if (IsLocked_unk()) {
                return 0;
            }
            return fn_800A5870(GetPad_unk(), 0, 0, 2.0f) != 0;
        }
    }
    return 0;
}

// NONMATCHING: 95.7% - scheduling of the stores of the stick vector and of the result
float Input_unk::fn_801248DC(atn::Vector3* dir) {
    atn::Vector3 v;
    v.x = -fn_800A56A8(GetPad_unk(), 0, 0, 0);
    v.y = 0.0f;
    v.z = -fn_800A56A8(GetPad_unk(), 0, 1, 0);
    float strength = PSVECMag(&v);
    if (dir) {
        v.RotZ_unk();
        v.y = m0C - v.y;
        *dir = v;
    }
    return strength;
}

int Input_unk::fn_80124B60() {
    return IsLocked_unk();
}

void Input_unk::fn_80124BE4() {
    m0C = fn_800F7258(fn_8008B964())->GetRot_unk()->y;
    m10 = 0;
}

void Input_unk::fn_80124BFC(int button, int lock) {
    for (int i = 0; i < 0x36; i++) {
        unsigned char old = m14[i];
        m14[i] = 0;
        if (button == fn_80122C70(i)) {
            m14[i] = lock;
        } else {
            m14[i] = old;
        }
    }
}

// inferred: icons of the pad buttons (Wii remote, nunchuk and classic controller)
struct ButtonIcon_unk {
    wchar_t mName[4];
    ButtonUv_unk mUv;
};
static ButtonIcon_unk sButtonIcons[21] = {
    {L"A", ButtonUv_unk(0.0f, 0.0f, 0.25f, 0.25f)},
    {L"B", ButtonUv_unk(0.25f, 0.0f, 0.5f, 0.25f)},
    {L"C", ButtonUv_unk(0.0f, 0.25f, 0.25f, 0.5f)},
    {L"Z", ButtonUv_unk(0.75f, 0.0f, 1.0f, 0.25f)},
    {L"1", ButtonUv_unk(0.0f, 0.0f, 0.0f, 0.0f)},
    {L"2", ButtonUv_unk(0.0f, 0.0f, 0.0f, 0.0f)},
    {L"+", ButtonUv_unk(0.5f, 0.25f, 0.75f, 0.5f)},
    {L"-", ButtonUv_unk(0.5f, 0.0f, 0.75f, 0.25f)},
    {L"", ButtonUv_unk(0.0f, 0.0f, 0.0f, 0.0f)},
    {L"", ButtonUv_unk(0.0f, 0.0f, 0.0f, 0.0f)},
    {L"", ButtonUv_unk(0.0f, 0.0f, 0.0f, 0.0f)},
    {L"", ButtonUv_unk(0.0f, 0.0f, 0.0f, 0.0f)},
    {L"a", ButtonUv_unk(0.0f, 0.5f, 0.25f, 0.75f)},
    {L"b", ButtonUv_unk(0.25f, 0.25f, 0.5f, 0.5f)},
    {L"x", ButtonUv_unk(0.5f, 0.75f, 0.75f, 1.0f)},
    {L"y", ButtonUv_unk(0.5f, 0.5f, 0.75f, 0.75f)},
    {L"L", ButtonUv_unk(0.0f, 0.75f, 0.25f, 1.0f)},
    {L"R", ButtonUv_unk(0.25f, 0.75f, 0.5f, 1.0f)},
    {L"ZL", ButtonUv_unk(0.75f, 0.5f, 1.0f, 0.75f)},
    {L"ZR", ButtonUv_unk(0.25f, 0.5f, 0.5f, 0.75f)},
    {L"", ButtonUv_unk(0.0f, 0.0f, 0.0f, 0.0f)},
};

ButtonUv_unk Input_unk::fn_80124C6C(int action) {
    switch (action) {
    case 0:
        return !fn_800A58D0(GetPad_unk(), 0) ? sButtonIcons[0].mUv : sButtonIcons[12].mUv;
    case 1:
        return !fn_800A58D0(GetPad_unk(), 0) ? sButtonIcons[1].mUv : sButtonIcons[13].mUv;
    case 2:
        return !fn_800A58D0(GetPad_unk(), 0) ? sButtonIcons[0].mUv : sButtonIcons[12].mUv;
    case 4:
        if (GetSetConfig_unk()->mAttackType == 0) {
            return sButtonIcons[20].mUv;
        }
        return !fn_800A58D0(GetPad_unk(), 0) ? sButtonIcons[0].mUv : sButtonIcons[12].mUv;
    case 16:
        return !fn_800A58D0(GetPad_unk(), 0) ? sButtonIcons[7].mUv : sButtonIcons[14].mUv;
    case 7:
        return !fn_800A58D0(GetPad_unk(), 0) ? sButtonIcons[1].mUv : sButtonIcons[17].mUv;
    case 8:
        if (GetSetConfig_unk()->mGatheringType == 0) {
            return !fn_800A58D0(GetPad_unk(), 0) ? sButtonIcons[2].mUv : sButtonIcons[16].mUv;
        }
        return !fn_800A58D0(GetPad_unk(), 0) ? sButtonIcons[3].mUv : sButtonIcons[18].mUv;
    case 39:
    case 41:
        return !fn_800A58D0(GetPad_unk(), 0) ? sButtonIcons[0].mUv : sButtonIcons[12].mUv;
    case 40:
    case 42:
        return !fn_800A58D0(GetPad_unk(), 0) ? sButtonIcons[6].mUv : sButtonIcons[15].mUv;
    case 43:
        return sButtonIcons[6].mUv;
    case 48:
        return !fn_800A58D0(GetPad_unk(), 0) ? sButtonIcons[6].mUv : sButtonIcons[15].mUv;
    case 50:
        return !fn_800A58D0(GetPad_unk(), 0) ? sButtonIcons[2].mUv : sButtonIcons[16].mUv;
    case 51:
        return !fn_800A58D0(GetPad_unk(), 0) ? sButtonIcons[3].mUv : sButtonIcons[18].mUv;
    default:
        return sButtonIcons[20].mUv;
    }
}
