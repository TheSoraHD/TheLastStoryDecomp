#include "MP/D2AMpMenu_unk.h"
#include "math/Vec.h"
#include "atn/Memory.h"
#include "atn/SoundRef.h"
#include "atn/Texture_unk.h"
#include "Event/Behavior.h"
#include "game/CharaTask.h"
#include "Event/DialogHead.h"
#include "atn/LipSync_unk.h"
#include "game/GimmickCommon.h"
#include "std/string.h"
#include "atn/DrawHelper.h"
#include "std/string_members.h"

// Everything below is inferred from Event::Dialog (0x802280B4..0x80229DF0); names are invented.

typedef MP::MpD2A_unk DialogD2A_unk;

namespace Event {
class Dialog;
}

extern "C" CharaTask* fn_80012934(Event::Actor_unk* actor); // inferred: character of an actor
extern "C" void fn_80012054(Event::Actor_unk* actor, int a, int b, int c, float f1, float f2);
extern "C" int fn_8006AA20(const char* path); // inferred: nonzero if the file exists

// inferred: render manager (see game/MenuCommonTask.cpp)
struct DialogRender_unk {
    unsigned char m00[0x3C];
    int mWidth;  // 0x3C
    int mHeight; // 0x40
};
extern "C" DialogRender_unk* lbl_8087EEE0;

// inferred: text renderer (lbl_8087F008)
struct DialogTextMgr_unk {
    unsigned char m00[0xA4];
    int mA4;
};
extern "C" DialogTextMgr_unk* lbl_8087F008;
extern "C" void fn_800DAA3C(DialogTextMgr_unk* mgr, int arg);
extern "C" int fn_800DBCC8(DialogTextMgr_unk* mgr);
extern "C" void fn_800DBC1C(DialogTextMgr_unk* mgr);
extern "C" void fn_800D9560(DialogTextMgr_unk* mgr);

// inferred: parameters of a text (fn_800DAB68)
struct DialogColor_unk {
    DialogColor_unk(unsigned int color) {
        r = (1.0f / 255.0f) * (float)((color >> 16) & 0xFF);
        g = (1.0f / 255.0f) * (float)((color >> 8) & 0xFF);
        b = (1.0f / 255.0f) * (float)(color & 0xFF);
        a = (1.0f / 255.0f) * (float)(color >> 24);
    }
    float r, g, b, a;
};
struct DialogColor4_unk {
    float r, g, b, a;
};
// inferred: two floats copied with one paired single
struct DialogVec2_unk {
    DialogVec2_unk() {}
    DialogVec2_unk(const DialogVec2_unk& o) { __Vec2Copy(&x, &o.x); }
    DialogVec2_unk& operator=(const DialogVec2_unk& o) {
        __Vec2Copy(&x, &o.x);
        return *this;
    }
    float x, y;
};
struct DialogText_unk {
    DialogText_unk() {}
    int mSlot;
    float m04;
    std::basic_string<wchar_t> mText;
    DialogVec2_unk mPos;
    int mAlign;
    int mKind;
    int m24;
    DialogColor4_unk mColor;
    float mSize;
    float m3C;
    int m40;
};
extern "C" void fn_800DAB68(DialogTextMgr_unk* mgr, DialogText_unk* text);
extern "C" const wchar_t lbl_8087DBD8[2]; // inferred: one space (another unit's literal)

// inferred: field (lbl_8087F430)
// inferred: camera (CameraSub_unk at lbl_8087F430 + 0x6C)
struct CameraSub_unk {
    void fn_80389838(int arg);
    unsigned char m000[0x7FC];
    int m7FC;
};
struct DialogField_unk {
    unsigned char m00[0x4C];
    int m4C; // inferred: map
    int m50;
    unsigned char m54[0x6C - 0x54];
    CameraSub_unk m6C;
};
extern "C" DialogField_unk* lbl_8087F430;
extern "C" int fn_80370174(DialogField_unk* field, int arg);
extern "C" void fn_80370320(DialogField_unk* field, int a, int b, int c);

// inferred: post effects (lbl_8087EF8C)
struct DialogPostFx_unk {
    unsigned char m000[0x778];
    int m778;
};
extern "C" DialogPostFx_unk* lbl_8087EF8C;

// inferred: fog parameters (lbl_8087EFA8 + 0xD4)
struct DialogFog_unk {
    DialogFog_unk()
        : mEnable(1), m04(0), m08(0), m0C(1), m10(200.0f), m14(500.0f), m18(30.0f), m1C(10.0f), m20(0x140), m24(0xE0),
          m28(1.5f), m2C(1.5f) {}
    int mEnable;
    int m04;
    int m08;
    int m0C;
    float m10;
    float m14;
    float m18;
    float m1C;
    int m20;
    int m24;
    float m28;
    float m2C;
};
// inferred: four floats copied with paired singles (color of the bloom)
struct DialogVec4_unk {
    DialogVec4_unk() {}
    DialogVec4_unk(const DialogVec4_unk& o) { __Vec4Copy(&x, &o.x); }
    DialogVec4_unk& operator=(const DialogVec4_unk& o) {
        __Vec4Copy(&x, &o.x);
        return *this;
    }
    float x, y, z, w;
};
// inferred: bloom parameters (lbl_8087EFA8 + 0x324)
struct DialogBloom_unk {
    int mEnable;
    DialogVec4_unk m04[4];
    int m44;
    int m48;
    float m4C;
};
struct DialogEnv_unk {
    unsigned char m000[0xD4];
    DialogFog_unk mFog;
    unsigned char m104[0x324 - 0x104];
    DialogBloom_unk mBloom;
};
extern "C" DialogEnv_unk* lbl_8087EFA8;
// inferred: fog and bloom targets of the dialog (another unit's .sdata)
extern "C" float lbl_8087DBB8;
extern "C" float lbl_8087DBBC;
extern "C" float lbl_8087DBC0;
extern "C" float lbl_8087DBC4;
extern "C" float lbl_8087DBC8;
extern "C" float lbl_8087DBCC;
extern "C" float lbl_8087DBD0;
extern "C" float lbl_8087DBD4;

// inferred: game manager (lbl_8087F540)
struct DialogScene_unk {
    unsigned char m000[0x194];
    int m194;
};
struct DialogGame_unk {
    unsigned char m0000[0x70];
    DialogScene_unk* m70;
    unsigned char m0074[0x2390 - 0x74];
    int m2390;
    unsigned char m2394[0x2448 - 0x2394];
    int m2448;
};
extern "C" DialogGame_unk* lbl_8087F540;
extern "C" void fn_80489C98(DialogGame_unk* game);

extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume);
extern "C" void* lbl_8087F498; // inferred: voice manager
extern "C" void fn_803EBAC8(void* mgr, int a, int b);

// inferred: game settings (lbl_8087F0A8): help/button display at 0x48C
struct DialogUv_unk {
    DialogUv_unk() {}
    DialogUv_unk(const DialogUv_unk& o) { __Vec4Copy(&u0, &o.u0); }
    float u0;
    float v0;
    float u1;
    float v1;
};
struct DialogSettings_unk {
    unsigned char m000[0x48C];
    unsigned char mPad[1];
};
extern "C" DialogSettings_unk* lbl_8087F0A8;
// inferred: pad input of the settings (game/Input_unk.h; declared here with the uv type of this file)
struct Input_unk {
    DialogUv_unk fn_80124C6C(int action);
    void fn_80122330(int reset);
};

extern "C" void fn_800D5908(atn::Texture_unk* texture, const char* path); // inferred: loads a texture

// inferred: weak @STRING@ pool of LipSyncData_unk::Resize_unk (atn/LipSync_unk; symbols.txt names them
// @STRING@Resize_unk__15LipSyncData_unkFUii and ...@0): to link, use the same inline/template
extern "C" const char lbl_8087D764[];
extern "C" const char lbl_8087D760[];

namespace Event {

// inferred: key of a speech balloon: character id / 10000, character type and kind
union DialogKey_unk {
    struct {
        unsigned short mId;
        unsigned char mType;
        unsigned char mKind;
    } s;
    unsigned int w;
};

// inferred: m184 and m190 are lip sync data (atn/LipSync_unk.h); m184 is passed to DialogHead::fn_80227F78

// inferred: event data (the actors and the command lists)
struct DialogCmd_unk;
template <class T>
struct DialogList_unk {
    bool IsValid_unk(int i) { return 0 <= i && i < mCount; }
    T* Get_unk(int i) { return IsValid_unk(i) ? mData[i].mPtr : 0; }
    struct Entry {
        T* mPtr;
        int m04;
    };
    Entry* mData;
    int mCount;
};
struct DialogCmd_unk {
    unsigned char m00[0x2C];
    ActionParam_unk* m2C;
    int m30;
    ActionParam_unk* GetParam_unk(int n) { return m30 > n ? &m2C[n] : 0; }
};
struct DialogGroup_unk {
    unsigned char m00[0xC];
    int mC; // inferred: type (0x29: dialog)
    unsigned char m10[0x14 - 0x10];
    DialogList_unk<DialogCmd_unk> m14;
};
struct DialogTrack_unk {
    unsigned char m00[0xC];
    int mC; // inferred: type (4: actor track)
    unsigned char m10[0x18 - 0x10];
    DialogList_unk<DialogGroup_unk> m18;
};
struct DialogScene2_unk {
    unsigned char m00[0x3C];
    DialogList_unk<DialogTrack_unk> m3C;
};
struct DialogEvent_unk {
    unsigned char m000[0xB8];
    DialogList_unk<DialogScene2_unk> mB8;
    unsigned char m0C0[0x164 - 0xC0];
    Actor_unk* m164;
    int m168;
};

// inferred: position of a balloon slot (fractions of the screen)
struct DialogPos_unk {
    float x;
    float y;
    int mKind;
};
static const DialogPos_unk sPos[4] = {{0.07f, 0.645f, 1}, {0.9f, 0.645f, 2}, {0.6f, 0.4f, 1}, {0.3f, 0.2f, 1}};
static const char* const sSound = "SE_UICOM_027";

// inferred from __vt__Q25Event6Dialog (0x807839E0): balloons of the event conversations
class Dialog : public atn::Task {
public:
    struct Head_unk {
        Head_unk() : mKey(0) {}
        unsigned int mKey;
        DialogHead* mHead;
    };
    struct Slot_unk {
        const DialogPos_unk* mPos;
        DialogVec2_unk mXY;
        DialogHead* mHead;
    };

    Dialog(atn::Task* parent);
    static Dialog* fn_802288D0(atn::Task* parent); // inferred: creates the task
    virtual ~Dialog();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();

    void fn_80228AF4(int slot, Actor_unk* actor, int kind, int arg);
    void fn_80228AF8(int slot, Actor_unk* actor, int kind, int arg); // inferred: shows a balloon
    void fn_80228CA0(int slot, const wchar_t* text, int align, int size, int arg, float f1, float f2,
                     unsigned int color, int mode, int offset); // inferred: adds a text
    void fn_80228FD8(); // inferred: starts the conversation
    int fn_802291BC();
    void fn_80229240(); // inferred: ends the conversation
    void fn_802292AC(int hide);
    void fn_80229414(); // inferred: input
    DialogHead* fn_8022960C(Actor_unk* actor);
    void fn_802297AC(); // inferred: update

    void ResetPos_unk() {
        float w = lbl_8087EEE0->mWidth;
        float h = lbl_8087EEE0->mHeight;
        for (int i = 0; i < 4; i++) {
            mSlots[i].mXY.x = w * sPos[i].x;
            mSlots[i].mXY.y = h * sPos[i].y;
        }
    }
    static unsigned int MakeKey_unk(Actor_unk* actor, int kind) {
        DialogKey_unk key;
        key.w = 0;
        CharaTask* chara = fn_80012934(actor);
        if (chara != 0) {
            key.s.mType = chara->fn_80179AB4();
            key.s.mId = chara->fn_80179A34() / 10000;
            key.s.mKind = kind;
        }
        return key.w;
    }
    Head_unk* FindHead_unk(Actor_unk* actor, int kind);

    int m48; // inferred: number of balloons
    Head_unk mHeads[16];
    Slot_unk mSlots[4];
    DialogD2A_unk* m10C; // inferred: ui_c_event_button.d2a
    int m110;            // inferred: state
    int m114;
    int m118; // inferred: saved camera mode
    int m11C; // inferred: last slot with a text
    atn::Texture_unk m120;
    int m150; // inferred: fade counter
    DialogFog_unk mFog;
    LipSyncData_unk m184;
    LipSyncKeys_unk m190; // inferred: default keys
    unsigned char m198;
    unsigned char m199;
    int m19C;
};

// inferred: name of the balloon file of a character (defined after the creator)
static std::basic_string<char> GetPath_unk(Actor_unk* actor, int kind);

inline Dialog::Head_unk* Dialog::FindHead_unk(Actor_unk* actor, int kind) {
    int i;
    unsigned int key = MakeKey_unk(actor, kind);
    for (i = 0; i < 16; i++) {
        if (key == mHeads[i].mKey) {
            return &mHeads[i];
        }
    }
    GetPath_unk(actor, kind);
    return 0;
}

// NONMATCHING: 95.03% - register allocation of the event loops (this in r21 in the original), the
// element constructor passed to __construct_new_array (0x8007A748 in the original) and the .sdata2 order
Dialog::Dialog(atn::Task* parent)
    : atn::Task(parent), m110(0), m114(-1), m118(0), m11C(-1), m150(-1), m198(0), m199(0) {
    for (int i = 0; i < 4; i++) {
        mSlots[i].mPos = &sPos[i];
        mSlots[i].mHead = 0;
    }
    for (int i = 0; i < 16; i++) {
        mHeads[i].mHead = 0;
    }
    ResetPos_unk();
    m198 = 0;
    m48 = 0;
    DialogEvent_unk* event = (DialogEvent_unk*)m20;
    for (int i = 0; i < event->mB8.mCount; i++) {
        DialogScene2_unk* scene = event->mB8.Get_unk(i);
        for (int j = 0; j < scene->m3C.mCount; j++) {
            DialogTrack_unk* track = scene->m3C.Get_unk(j);
            if (track->mC != 4) {
                continue;
            }
            for (int k = 0; k < track->m18.mCount; k++) {
                DialogGroup_unk* group = track->m18.Get_unk(k);
                if (group->mC != 0x29) {
                    continue;
                }
                for (int l = 0; l < group->m14.mCount; l++) {
                    DialogCmd_unk* cmd = group->m14.Get_unk(l);
                    Actor_unk* actor = ((EventManager_unk*)event)->FindActor_unk(cmd->GetParam_unk(0)->m04.i);
                    if (actor == 0) {
                        continue;
                    }
                    int kind = cmd->GetParam_unk(1)->m04.i;
                    int arg = cmd->GetParam_unk(3)->m04.i;
                    Head_unk* head = FindHead_unk(actor, kind);
                    if (head == 0) {
                        if (m48 >= 16) {
                            break;
                        }
                        mHeads[m48].mKey = MakeKey_unk(actor, kind);
                        std::basic_string<char> path = GetPath_unk(actor, kind);
                        if (fn_8006AA20(path.c_str()) != 0) {
                            mHeads[m48].mHead = DialogHead::fn_802251F8(this, fn_80012934(actor));
                            mHeads[m48].mHead->fn_800D246C(1);
                        }
                        head = &mHeads[m48++];
                    }
                    if (head->mHead != 0) {
                        head->mHead->fn_80227D2C(kind, arg);
                    }
                }
            }
        }
    }
    m10C = fn_801F3FF8(this, "/data/d2anime/ui_c_event_button.d2a", 0);
    m10C->fn_800D246C(1);
    fn_800D5908(&m120, "/data/texture/ui_c_event_under_filter.texture");
    m120.m2D = 0;
    m120.m2E = 0;
    delete[] m190.mKeys;
    m190.mCount = 8;
    m190.mKeys = new ((MemoryTag)4, lbl_8087D764, lbl_8087D760, 0) LipSyncKey_unk[8];
    for (unsigned int i = 0; i < m190.mCount; i++) {
        m190.mKeys[i].mVowel = 5;
        m190.mKeys[i].mTime = 1.0f;
    }
    unsigned int n = m190.mCount;
    delete[] m184.mKeys;
    m184.mCount = n;
    if (n != 0) {
        m184.mKeys = new ((MemoryTag)0, lbl_8087D764, lbl_8087D760, 0) LipSyncKey_unk[n];
    } else {
        m184.mKeys = 0;
    }
    for (unsigned int i = 0; i < m184.mCount; i++) {
        m184.mKeys[i] = m190.mKeys[i];
    }
}

Dialog* Dialog::fn_802288D0(atn::Task* parent) {
    return new ((MemoryTag)1, "", "", 0) Dialog(parent);
}

// NONMATCHING: 99.57% - registers inside the inlined basic_string(const char*) constructors
static std::basic_string<char> GetPath_unk(Actor_unk* actor, int kind) {
    CharaTask* chara = fn_80012934(actor);
    if (chara == 0) {
        return std::basic_string<char>("");
    }
    if (chara->GetModel_unk()->m16C == 0) {
        return std::basic_string<char>("");
    }
    std::basic_string<char> name(chara->GetModel_unk()->fn_8008B130());
    return name;
}

void Dialog::fn_80228AF4(int slot, Actor_unk* actor, int kind, int arg) {
    fn_80228AF8(slot, actor, kind, arg);
}

void Dialog::fn_80228AF8(int slot, Actor_unk* actor, int kind, int arg) {
    Head_unk* head = FindHead_unk(actor, kind);
    if (head != 0) {
        if (mSlots[slot].mHead != 0) {
            mSlots[slot].mHead->mFlags |= 4;
        }
        DialogHead* h = head->mHead;
        if (h != 0) {
            mSlots[slot].mHead = h;
            h->fn_80227434(kind, arg, slot == 1);
            int flag = 0;
            DialogField_unk* field = lbl_8087F430;
            if (field != 0) {
                int map = field->m4C;
                if ((map == 14 && field->m50 == 6) || (map == 0x1D && field->m50 == 6)) {
                    flag = 1;
                }
            }
            h->fn_80226B28(flag, slot == 1);
            h->mFlags &= ~4;
            h->fn_800D246C(0);
        }
    }
}

// NONMATCHING: 99.78% - only the .sdata2 pool differs: the original has 0.0f right after 1.0f (first
// used before this function; not found), so every later constant is one slot off
void Dialog::fn_80228CA0(int slot, const wchar_t* text, int align, int size, int arg, float f1, float f2,
                         unsigned int color, int mode, int offset) {
    float fontSize;
    if (size == 0) {
        fontSize = 16.0f;
    } else if (size == 1) {
        fontSize = 20.0f;
    } else {
        fontSize = 28.0f;
    }
    if (mode == 0) {
        mSlots[slot].mXY.x = lbl_8087EEE0->mWidth * mSlots[slot].mPos->x;
        mSlots[slot].mXY.y += 26.0f;
        if (offset == 0) {
            mSlots[slot].mXY.y += 2.0f;
        }
        if (offset == 1) {
            mSlots[slot].mXY.y += -2.0f;
        }
        if (offset == 2) {
            mSlots[slot].mXY.y += -3.0f;
        }
    } else if (mode == 2) {
        mSlots[slot].mXY.x += fn_8006EF48(lbl_8087EEC8, lbl_8087DBD8, fontSize, 0.0f, 1, 1);
    }
    if (m11C != slot) {
        fn_800DAA3C(lbl_8087F008, 0);
    }
    m11C = slot;
    DialogText_unk param;
    param.mSlot = slot;
    param.m04 = f1;
    param.mText = text;
    param.mPos = mSlots[slot].mXY;
    param.mAlign = align;
    param.mKind = mSlots[slot].mPos->mKind;
    switch (param.mKind) {
    case 1:
        param.mAlign = 1;
        break;
    case 2:
        param.mAlign = 2;
        break;
    }
    param.m24 = 1;
    DialogColor_unk c(color);
    param.mColor.r = c.r;
    param.mColor.g = c.g;
    param.mColor.b = c.b;
    param.mColor.a = c.a;
    param.mSize = fontSize;
    param.m3C = f2;
    param.m40 = offset;
    fn_800DAB68(lbl_8087F008, &param);
    mSlots[slot].mXY.x += fn_8006EF48(lbl_8087EEC8, text, fontSize, 0.0f, 1, 1);
}

// inferred: actor taking part in the conversation
static inline bool IsTalking_unk(Actor_unk* actor) {
    return actor->m20 == 1 || actor->m20 == 7;
}

// NONMATCHING: 99.78% - the original increments the index before the actor offset
void Dialog::fn_80228FD8() {
    Actor_unk* actor;
    DialogEvent_unk* event = (DialogEvent_unk*)m20;
    for (int i = 0; i < event->m168; i++) {
        actor = &event->m164[i];
        if (IsTalking_unk(actor)) {
            if (actor->fn_800128FC()->m55C == 6) {
                actor->fn_800128FC()->fn_8016EB48(0, 0, 0);
            }
            if (actor->fn_800128FC()->mFlag6) {
                actor->fn_800128FC()->fn_80155DAC(0);
            }
            if (actor->fn_800128FC()->m1208 != 0) {
                actor->fn_800128FC()->fn_801750FC();
            }
            fn_80012054(actor, 0, 1, 0x14, 1.0f, 0.0f);
        }
    }
    if (lbl_8087F430 != 0) {
        m118 = fn_80370174(lbl_8087F430, 3);
        fn_80370320(lbl_8087F430, 3, 1, 0);
        CameraSub_unk* camera = &lbl_8087F430->m6C;
        if (camera->m7FC != 0) {
            camera->fn_80389838(0);
        }
    }
    fn_800DAA3C(lbl_8087F008, 0);
    lbl_8087EF8C->m778 = 0x11;
    m150 = 0;
    mFog = lbl_8087EFA8->mFog;
    m198 = 0;
    m199 = 1;
}

int Dialog::fn_802291BC() {
    if (lbl_8087F008->mA4 == 0) {
        return 0;
    }
    if (m10C->IsFlag_unk(4) && lbl_8087F008->mA4 <= -1) {
        m10C->mSpeed = 1.0f;
        m10C->fn_800D246C(0);
        m10C->mFlags &= ~4;
    }
    return 1;
}

void Dialog::fn_80229240() {
    lbl_8087EF8C->m778 = 14;
    if (lbl_8087F430 != 0) {
        fn_80370320(lbl_8087F430, 3, m118, 0);
    }
    m150 = -1;
    fn_802292AC(1);
    m199 = 0;
}

// NONMATCHING: 89.60% - scheduling of the inlined ResetPos_unk (tried the loop, explicit statements,
// unsigned index and the operand order)
void Dialog::fn_802292AC(int hide) {
    if (hide != 0) {
        for (int i = 0; i < 4; i++) {
            DialogHead* head = mSlots[i].mHead;
            if (head != 0) {
                head->mFlags |= 4;
                head->fn_800D246C(1);
            }
        }
    }
    ResetPos_unk();
    m198 = 0;
    m10C->mSpeed = -1.0f;
    m10C->mFlags &= ~4;
    m10C->fn_800D246C(0);
    fn_800DAA3C(lbl_8087F008, 10);
    lbl_8087F540->m2390 = 0;
    m110 = 0;
}

void Dialog::fn_80229414() {
    if (fn_800A555C(lbl_8087EF70, 0, 4) != 0 || lbl_8087F540->m2448 != 0) {
        if (m110 == 1 && lbl_8087F008->mA4 <= -1) {
            if (fn_800DBCC8(lbl_8087F008) != 0) {
                fn_80489C98(lbl_8087F540);
                fn_800DBC1C(lbl_8087F008);
                m110 = 3;
            } else {
                fn_80489C98(lbl_8087F540);
                fn_800DBC1C(lbl_8087F008);
                m110 = 2;
                m114 = 2;
                m198 = 1;
            }
        }
        if (m110 == 3) {
            fn_800C31F4(sSound, 0, -1, 1.0f);
            fn_802292AC(0);
            for (int i = 0; i < 4; i++) {
                if (mSlots[i].mHead != 0) {
                    mSlots[i].mHead->fn_80227F78(&m184);
                }
            }
            if (lbl_8087F498 != 0) {
                fn_803EBAC8(lbl_8087F498, 15, 1);
            }
        }
    }
    if (m110 == 1 && lbl_8087F008->mA4 <= -1 && fn_800DBCC8(lbl_8087F008) != 0) {
        fn_80489C98(lbl_8087F540);
        fn_800DBC1C(lbl_8087F008);
        m110 = 2;
        m114 = 2;
        m198 = 1;
    }
    if (m110 == 0 && lbl_8087F540->m70 != 0 && lbl_8087F540->m70->m194 == 10) {
        m110 = 1;
    }
    if (m110 == 2) {
        if (m114 == 0) {
            m110 = 3;
        }
        if (m114 > 0) {
            m114--;
        }
    }
}

// NONMATCHING: 99.32% - the original keeps the strength-reduced pointer in r30 and the character in
// r31 (tried the declaration orders and a pointer loop)
DialogHead* Dialog::fn_8022960C(Actor_unk* actor) {
    CharaTask* chara;
    if (actor == 0 || fn_80012934(actor) == 0) {
        return 0;
    }
    for (int i = 0; i < m48; i++) {
        DialogHead* head = mHeads[i].mHead;
        if (head != 0 && !head->IsFlag_unk(4)) {
            chara = head->m1428;
            if (chara == fn_80012934(actor)) {
                return mHeads[i].mHead;
            }
        }
    }
    return 0;
}

int Dialog::vf0C() {
    for (int i = 0; i < m48; i++) {
        DialogHead* head = mHeads[i].mHead;
        if (head != 0) {
            if (head->IsFlag_unk(2)) {
                head->fn_800D246C(0);
                mHeads[i].mHead->mFlags |= 4;
            } else {
                return 0;
            }
        }
    }
    if (!m10C->IsFlag_unk(2)) {
        return 0;
    }
    if (((GimmickUvOwner_unk*)&m120)->fn_800D59B8() != 0) {
        return 0;
    }
    m10C->mFlags |= 4;
    m10C->mRepeat = 0;
    return 1;
}

#define DIALOG_MIN_UNK(a, b) ((a) < (b) ? (a) : (b))

// NONMATCHING: 89.06% - scheduling/registers of the fog and bloom copies and the .sdata2 order
void Dialog::fn_802297AC() {
    if (!m10C->IsFlag_unk(4) && m10C->mSpeed < 0.0f && m10C->mFrame <= 1.0f) {
        m10C->mFlags |= 4;
    }
    if (lbl_8087F0A8 != 0) {
        DialogUv_unk uv = ((Input_unk*)lbl_8087F0A8->mPad)->fn_80124C6C(0);
        DialogUv_unk uv0 = uv;
        m10C->SetParam2_unk("btnUv", &uv0.u0);
        DialogUv_unk uv1 = uv;
        m10C->SetParam2_unk("btnUvMask", &uv1.u0);
    }
    fn_80229414();
    fn_800D9560(lbl_8087F008);
    if (lbl_8087F0A8 != 0) {
        ((Input_unk*)lbl_8087F0A8->mPad)->fn_80122330(0);
    }
    if (m150 >= 0 && ++m150 <= 30) {
        float t = DIALOG_MIN_UNK(1.0f, m150 / 30.0f);
        DialogFog_unk fog;
        fog.mEnable = 1;
        fog.m04 = mFog.m04;
        fog.m08 = mFog.m08;
        fog.m0C = mFog.m0C;
        fog.m10 = mFog.m10 + t * (lbl_8087DBB8 - mFog.m10);
        fog.m14 = mFog.m14 + t * (lbl_8087DBBC - mFog.m14);
        fog.m18 = mFog.m18 + t * (lbl_8087DBC0 - mFog.m18);
        fog.m1C = mFog.m1C + t * (lbl_8087DBC4 - mFog.m1C);
        fog.m20 = mFog.m20;
        fog.m24 = mFog.m24;
        fog.m28 = mFog.m28 + t * (lbl_8087DBC8 - mFog.m28);
        fog.m2C = mFog.m28 + t * (lbl_8087DBCC - mFog.m28);
        lbl_8087EFA8->mFog = fog;
        DialogBloom_unk bloom = lbl_8087EFA8->mBloom;
        bloom.mEnable = 1;
        bloom.m48 = 1;
        bloom.m4C = lbl_8087DBD0 + t * (lbl_8087DBD4 - lbl_8087DBD0);
        lbl_8087EFA8->mBloom = bloom;
    }
}

// inferred: float color channel to a byte
static inline unsigned int ToByte_unk(float x) {
    if (x >= 1.0f) {
        return 0xFF;
    }
    if (x <= 0.0f) {
        return 0;
    }
    return (unsigned int)(255.0f * x + 0.5f);
}

// NONMATCHING: 99.64% - registers of the r/g/b bytes (r28/r30 swapped) and the .sdata2 order
void Dialog::vf14() {
    if (m150 >= 0) {
        float w = lbl_8087EEE0->mWidth;
        float h = lbl_8087EEE0->mHeight;
        float t = DIALOG_MIN_UNK(1.0f, m150 / 30.0f);
        DialogColor4_unk c;
        c.r = 0.0f;
        c.g = 0.0f;
        c.b = 0.0f;
        c.a = 0.75f * t;
        unsigned int b;
        unsigned int g;
        unsigned int r;
        r = ToByte_unk(c.r);
        g = ToByte_unk(c.g);
        b = ToByte_unk(c.b);
        unsigned int a = ToByte_unk(c.a);
        lbl_8087EEB0->fn_8005E7F4((a << 24) | (r << 16) | (g << 8) | b, 0, &m120, 0.0f, h - 256.0f, 100.0f, w, 256.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    }
}

Dialog::~Dialog() {}

void Dialog::vf10() {}

} // namespace Event
