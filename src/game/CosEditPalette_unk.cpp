#include "game/CosEditProtector.h"
#include "game/CostumeEditChara.h"
#include "game/CharaTask.h"
#include "game/CharaStatus.h"
#include "game/Inventory.h"
#include "game/MenuCommonTask.h"
#include "atn/D3Anime.h"
#include "atn/D3AnimeInstance.h"
#include "atn/D2A_Task.h"
#include "atn/D2A_TaskInstance.h"
#include "atn/SoundRef.h"
#include "atn/SoundManager.h"
#include "db/EquipParam_unk.h"
#include "db/OptionParts_unk.h"
#include "macros.h"

// Second file of CosEditProtector (game/CosEditProtector.cpp has the rest): the palette and part
// pages of the costume menu.

extern "C" int sprintf(char* s, const char* format, ...);
extern "C" int fn_800DD3FC(wchar_t* s, const wchar_t* format, ...); // inferred: wide sprintf
extern "C" unsigned int fn_800DC6B4(const char* s);               // inferred: string hash
extern "C" void* lbl_8087EF70;                                     // inferred: pad
extern "C" int fn_800A555C(void* pad, int port, int button);      // inferred: button triggered
extern "C" int fn_800A55AC(void* pad, int port, int button);      // inferred: button repeated
extern "C" atn::SoundRef fn_80117228(int id);                      // inferred: plays a system sound effect
extern "C" int fn_80117214(int index);                             // inferred: palette slot of a colour index
extern "C" const wchar_t* fn_80116FC0(int a, int id);              // inferred: system text
extern "C" const wchar_t lbl_808813D0[2];                          // inferred: empty text
extern "C" void fn_804A62C8(MenuCommonTask* menu, int type, int noCancel); // inferred: opens a dialog
extern "C" void fn_804A4494(int* cursor, int* top, int count, int page, int a, int b); // inferred: moves a list cursor
extern "C" void fn_80510FB8(NewCampPartsBase* self, int a, int b); // inferred: moves the cursor (m80/m88)
extern "C" void fn_80510E94(NewCampPartsBase* self, int a, int b);
extern "C" void fn_80510D64(NewCampPartsBase* self, int a, int b);
extern "C" void fn_805112A8(NewCampPartsBase* self, NewCampPartsEntry_unk* entry, atn::D3AnimeInstance* anime,
                            const atn::Vector3& pos, const atn::Vector3& scale, const char* name);
extern "C" void* lbl_8087F430; // inferred: field
extern "C" int fn_80370174(void* field, int id);
extern "C" void fn_80370320(void* field, int id, int a, int b);
extern "C" int fn_80219558(int id); // inferred: party member of a character id
extern "C" int fn_8020EF80(EquipParamEntry_unk* entry); // inferred: item id of an equipment entry
extern "C" int fn_80444A50(Inventory* inv, int kind);   // inferred: item of a kind
extern "C" int fn_804444E8(Inventory* inv, int id);     // inferred: number of items
extern "C" int fn_80444828(Inventory* inv, int palette); // inferred: item of a palette colour
extern "C" int fn_8044493C(Inventory* inv, int part);    // inferred: item of a part
extern "C" int fn_80444B64(Inventory* inv, int id);      // inferred: icon of an item

// Members of atn::D2A_TaskInstanceBase<32, 8> with automatic names (atn/D2A_TaskInstanceC.h). The
// rectangle and the UV are passed by address here: the same local is passed twice without a copy.
typedef atn::D2A_TaskInstanceBase<32, 8> CosEditLayout_unk;
extern "C" void fn_801F6C80(CosEditLayout_unk* inst, const char* name, float value);
extern "C" void fn_801F6E78(CosEditLayout_unk* inst, const char* name, const atn::D2A_DrawRect& r);
extern "C" void fn_801F7590(CosEditLayout_unk* inst, const char* name, const float* uv);
extern "C" void fn_801F7DF0(CosEditLayout_unk* inst, const char* name, float r, float g, float b);
extern "C" void fn_801F837C(CosEditLayout_unk* inst, const char* name, const wchar_t* text);

// inferred: texture rectangle (u0, v0, u1, v1), passed by value (as MenuUV_unk in game/MenuCommonTask.cpp)
struct CosEditUV_unk {
    CosEditUV_unk() {}
    CosEditUV_unk(float u0_, float v0_, float u1_, float v1_) : u0(u0_), v0(v0_), u1(u1_), v1(v1_) {}
    CosEditUV_unk(const CosEditUV_unk& o) { __Vec4Copy(&u0, &o.u0); }
    CosEditUV_unk& operator=(const CosEditUV_unk& o) {
        __Vec4Copy(&u0, &o.u0);
        return *this;
    }
    float u0, v0, u1, v1;
};
extern "C" CosEditUV_unk fn_80444C50(Inventory* inv, int icon); // inferred: texture rectangle of an icon

// inferred: setters taking the rectangle by value (as SetUV_unk in game/MenuCommonTask.cpp); the name
// comes first (the copy and the task are evaluated before it)
inline void SetUV_unk(const char* name, atn::D2A_TaskInstanceBase<32, 8>* inst, CosEditUV_unk uv) {
    fn_801F7590(inst, name, &uv.u0);
}
inline void SetUV_unk(const char* name, atn::D2A_Task* task, CosEditUV_unk uv) {
    task->SetParam2_unk(name, &uv.u0);
}

// inferred: rectangle of a pane by name (as in include/MP/D2AMpMenu_unk.h)
inline atn::D2A_DrawRect GetRect_unk(atn::D2A_Task* task, const char* name) {
    return fn_801F4E8C(task, fn_800DC6B4(name));
}

struct Palette_unk;
extern "C" Palette_unk* fn_8020BD14(int index); // inferred: part palette (db/PaletteTable_unk.cpp)

// inferred: item data (only the icon is read)
struct CosEditItemInfo_unk {
    unsigned char m00[0x2C];
    int mIcon; // 0x2C: inferred: 1-based, 4 per row
};
extern "C" CosEditItemInfo_unk* fn_802091E8(int id);

// inferred: camp message table (lbl_8087F1E4, as in MP/D2AMpMenuSetRule.cpp)
struct CosEditMsgTable_unk {
    const wchar_t* Get_unk(int id) { return mText[id] != 0 ? mText[id] : lbl_808813D0; }
    const wchar_t* mText[1];
};
extern "C" CosEditMsgTable_unk* lbl_8087F1E4;

// inferred: not used by this file (only its __sinit is left)
static atn::Vector3 sHidePos(0.0f, -600.0f, 0.0f);

// inferred: number of dye items of a kind
inline int GetDyeCount_unk(int kind) {
    Inventory* inv = Inventory::instance_;
    return fn_804444E8(inv, fn_80444A50(inv, kind));
}

// inferred: shows the equipment of the four slots with a colour, a palette id or a costume set
// (inlined in fn_801D5AB0 and fn_801D6188)
inline void CosEditProtector::SetEquip_unk(int member, CharaTask* chara, int color, short id, int set, int saved) {
    for (int i = 0; i < 4; i++) {
        m210 = color;
        m21C = id;
        m214 = set;
        m218 = saved;
        m20C = -1;
        fn_801CEA50(member, chara, i, 1, Inventory::instance_->GetPlayerStatusInfo(member)->fn_801347C8(i), 0x10);
    }
}

void CosEditProtector::fn_801D54C0() {
    void* pad = lbl_8087EF70;
    if (fn_800A555C(pad, 0, 4)) {
        CostumeEditChara* sel = fn_801CF334();
        Inventory::Equip_unk* equip = &Inventory::instance_->mEquip[sel->mFace];
        switch (m1BA0) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            if (fn_8020EF4C(fn_8020ED84(0, equip->mProtectors[0].mId))) {
                MenuCommonTask::sInstance->ShowWarning_unk(0x105);
                fn_80117228(0xB);
                return;
            }
            m1BB8 = m1BA0;
            vf34(8);
            break;
        case 8:
            vf34(9);
            break;
        case 9:
            if (fn_8020EF4C(fn_8020ED84(0, equip->mProtectors[0].mId))) {
                MenuCommonTask::sInstance->ShowWarning_unk(0x105);
                fn_80117228(0xB);
                return;
            }
            if (!fn_80370174(lbl_8087F430, 0x147)) {
                fn_80370320(lbl_8087F430, 0x147, 1, 0);
            }
            if (GetDyeCount_unk(0) > 0 || GetDyeCount_unk(1) > 0) {
                vf34(0xE);
            } else {
                fn_80117228(0xB);
                return;
            }
            break;
        case 10:
            if (fn_8020EF4C(fn_8020ED84(0, equip->mProtectors[0].mId))) {
                MenuCommonTask::sInstance->ShowWarning_unk(0x105);
                fn_80117228(0xB);
                return;
            }
            fn_801D6AB0();
            if (mPartLists[0].mSize == 0) {
                MenuCommonTask::sInstance->ShowWarning_unk(0x106);
                fn_80117228(0xB);
                return;
            }
            vf34(10);
            break;
        case 11:
            if (fn_8020EF4C(fn_8020ED84(0, equip->mProtectors[0].mId))) {
                MenuCommonTask::sInstance->ShowWarning_unk(0x105);
                fn_80117228(0xB);
                return;
            }
            vf34(6);
            break;
        case 12:
            if (fn_8020EF4C(fn_8020ED84(0, equip->mProtectors[0].mId))) {
                MenuCommonTask::sInstance->ShowWarning_unk(0x105);
                fn_80117228(0xB);
                return;
            }
            vf34(7);
            break;
        }
        fn_80117228(0xA);
        fn_80117228(1);
    } else if (fn_800A555C(pad, 0, 5)) {
        if (m1E0 == 0) {
            fn_80117228(2);
            fn_80117228(0xA);
            vf34(0);
        } else {
            fn_80117228(2);
            vf34(1);
            if (lbl_8087EFE8) {
                lbl_8087EFE8->fn_800CFBA0(1, 0, 0x20, 30);
                lbl_8087EFE8->fn_800CFBA0(2, 0, 0x20, 15);
            }
        }
    } else {
        int cursor = m80;
        fn_80510FB8(this, 1, 3);
        if (cursor != m80) {
            m1BA0 = m80;
        }
    }
}

void CosEditProtector::fn_801D59A8() {
    void* pad = lbl_8087EF70;
    if (fn_800A555C(pad, 0, 4)) {
        fn_80117228(1);
        vf34(8);
    } else if (fn_800A555C(pad, 0, 5)) {
        fn_80117228(2);
        fn_80117228(0xA);
        vf34(4);
    } else {
        int cursor = m80;
        fn_80510E94(this, 1, 3);
        if (cursor != m80) {
            m1BB8 = m80;
        }
    }
}

void CosEditProtector::fn_801D5AB0() {
    if (m1B90) {
        m1B90 = 0;
        if (MenuCommonTask::sInstance->m68 == 0) {
            int member = fn_801CF334()->mFace;
            CharaTask* chara = fn_801CF334()->mChara;
            int set;
            if (m1BB0 == 1) {
                set = mSets[m1BA8].mId;
            } else {
                set = m1BA4;
            }
            SetEquip_unk(member, chara, -1, -1, set, m1BB0 == 1);
            m1B94 = 1;
            fn_80117228(0xF);
        } else {
            fn_80117228(2);
        }
    } else if (m1B94) {
        MenuCommonTask::sInstance->fn_804A4294(lbl_8087F1E4->Get_unk(0x377));
        m1B94 = 0;
    } else {
        void* pad = lbl_8087EF70;
        if (fn_800A555C(pad, 0, 4)) {
            CostumeEditChara* sel = fn_801CF334();
            if (fn_8020EF4C(fn_8020ED84(0, Inventory::instance_->mEquip[sel->mFace].mProtectors[0].mId))) {
                MenuCommonTask::sInstance->ShowWarning_unk(0x105);
                fn_80117228(0xB);
            } else if (m1BB0 == 1) {
                if (m1BA8 < mSetCount) {
                    fn_804A62C8(MenuCommonTask::sInstance, 4, 1);
                    m1B90 = 1;
                    fn_80117228(1);
                } else {
                    fn_80117228(0xB);
                }
            } else if (Inventory::instance_->mEquipSetA[m1BA4].mProtectors[0].mId < 0) {
                MenuCommonTask::sInstance->ShowWarning_unk(0x107);
                fn_80117228(0xB);
            } else {
                fn_804A62C8(MenuCommonTask::sInstance, 4, 1);
                m1B90 = 1;
                fn_80117228(1);
            }
        } else if (fn_800A555C(pad, 0, 5)) {
            fn_80117228(2);
            fn_80117228(0xA);
            vf34(4);
        } else if (fn_800A555C(pad, 0, 0xC)) {
            m1BB0 = 0;
            fn_80117228(0x12);
            m80 = m1BA4;
            fn_801D6F84();
            m1BA4 = m80;
            fn_801D3DF4();
        } else if (fn_800A555C(pad, 0, 0xD)) {
            m1BB0 = 1;
            fn_80117228(0x12);
            m80 = m1BA8;
            fn_801D6F84();
            m1BA8 = m80;
            fn_801D3DF4();
        } else {
            int cursor = m80;
            int top = m88;
            fn_80510FB8(this, 1, 3);
            if (cursor != m80) {
                if (m1BB0 == 1) {
                    m1BA8 = m80;
                } else {
                    m1BA4 = m80;
                }
            }
            if (top != m88) {
                fn_801D6F84();
            }
        }
    }
}

void CosEditProtector::fn_801D5F04() {
    if (m1B90) {
        m1B90 = 0;
        if (MenuCommonTask::sInstance->m68 == 0) {
            int member = fn_80219558(fn_801CF334()->mChara->m50);
            // inferred: the protector ids are copied as ints (the original copies word by word)
            int* src = (int*)&Inventory::instance_->mEquipShown[member];
            int* dst = (int*)&Inventory::instance_->mEquipSetA[m1BAC];
            for (int i = 0; i < 8; i++) {
                dst[i] = src[i];
            }
            MenuCommonTask::sInstance->fn_804A4294(lbl_8087F1E4->Get_unk(0x231));
            fn_801D6F84();
            fn_80117228(0xC);
        } else {
            fn_80117228(2);
        }
    } else {
        void* pad = lbl_8087EF70;
        if (fn_800A555C(pad, 0, 4)) {
            CostumeEditChara* sel = fn_801CF334();
            if (fn_8020EF4C(fn_8020ED84(0, Inventory::instance_->mEquip[sel->mFace].mProtectors[0].mId))) {
                MenuCommonTask::sInstance->ShowWarning_unk(0x105);
                fn_80117228(0xB);
            } else {
                fn_804A62C8(MenuCommonTask::sInstance, 3, 1);
                m1B90 = 1;
                fn_80117228(1);
            }
        } else if (fn_800A555C(pad, 0, 5)) {
            fn_80117228(2);
            fn_80117228(0xA);
            vf34(4);
        } else {
            int cursor = m80;
            int top = m88;
            fn_80510FB8(this, 1, 3);
            if (cursor != m80) {
                m1BAC = m80;
            }
            if (top != m88) {
                fn_801D6F84();
            }
        }
    }
}

// NONMATCHING: 99.5% - register coloring (pad, palette id and loop index)
void CosEditProtector::fn_801D6188() {
    void* pad = lbl_8087EF70;
    if (fn_800A555C(pad, 0, 4)) {
        CosEditCell_unk* cell = GetCell_unk();
        if (cell) {
            if (cell->mEnabled) {
                int member = fn_801CF334()->mFace;
                CharaTask* chara = fn_801CF334()->mChara;
                int id = cell->mId;
                int slot = fn_80117214(m1BB8);
                int* ids = (int*)&Inventory::instance_->mEquipShown[member];
                if (slot < 0 || id != ids[slot]) {
                    SetEquip_unk(member, chara, slot, id, -1, 0);
                }
                fn_80117228(0xF);
            } else {
                fn_80117228(0xB);
            }
        }
    } else if (fn_800A555C(pad, 0, 5)) {
        vf34(4);
        fn_80117228(2);
    } else {
        fn_801D7DE8(&m1F8, &m1FC, 8, 8, 0x40);
    }
}

// NONMATCHING: 99.9% - the pad gets r30 (shared with the cell) instead of r29
void CosEditProtector::fn_801D6390() {
    void* pad = lbl_8087EF70;
    if (fn_800A555C(pad, 0, 4)) {
        CosEditCell_unk* cell = GetCell_unk();
        if (cell) {
            if (cell->mEnabled) {
                int member = fn_801CF334()->mFace;
                CharaTask* chara = fn_801CF334()->mChara;
                if (cell->mId == 0x1F) {
                    if (chara) {
                        chara->mStatus.m231 = 0;
                    }
                    Inventory::instance_->GetPlayerStatusInfo(member)->m231 = 0;
                } else {
                    unsigned char mark = cell->mId + 1;
                    if (chara) {
                        chara->mStatus.m231 = mark;
                    }
                    Inventory::instance_->GetPlayerStatusInfo(member)->m231 = mark;
                }
                fn_80117228(0x10);
            } else {
                fn_80117228(0xB);
            }
        }
    } else if (fn_800A555C(pad, 0, 5)) {
        vf34(4);
        fn_80117228(2);
    } else {
        fn_801D7DE8(&m200, &m204, 8, 4, 0x20);
    }
}

// inferred: toggles the option parts of an option (inlined in fn_801D6560)
inline void CosEditProtector::ToggleOption_unk(int slot, int index) {
    unsigned int* bits;
    OptionParts_unk* parts = fn_801CE7E8(slot);
    bits = fn_801CE87C(slot);
    chr::CharaEquip_unk* equip = fn_801CF334()->mChara->m680[slot];
    OptionLevelList_unk* list = fn_8020DB18(equip->m414->GetLetter_unk(), slot);
    if (bits && list) {
        OptionLevel_unk* level = &list->mLevels[index];
        for (unsigned int i = 0; i < 3; i++) {
            if (level->mHashes[i]) {
                int bit = fn_8020D608(parts, level->mHashes[i]);
                if (bit >= 0) {
                    unsigned int mask = 1 << bit;
                    if ((mask & *bits) == mask) {
                        *bits &= ~mask;
                    } else {
                        *bits |= mask;
                    }
                    equip->fn_805657CC(equip->m008, bits);
                }
            }
        }
    }
}

// NONMATCHING: 99.1% - register coloring of the inlined ToggleOption_unk
void CosEditProtector::fn_801D6560() {
    void* pad = lbl_8087EF70;
    if (fn_800A555C(pad, 0, 4)) {
        if (mPartLists[0].mSize) {
            CosEditPart_unk& part = mPartLists[0].mData[m80];
            ToggleOption_unk(part.mSlot, part.mIndex);
            fn_80117228(0xF);
            fn_801D6C1C(m1BB4);
        }
    } else if (fn_800A555C(pad, 0, 5)) {
        fn_80117228(2);
        fn_80117228(0xA);
        vf34(4);
    } else {
        int cursor = m80;
        fn_804A4494(&m80, &m88, m84, m8C, 1, 3);
        if (cursor != m80) {
            m1BB4 = m80;
            fn_801D6C1C(m1BB4);
        }
    }
}

// NONMATCHING: 99.3% - register coloring (pad, member and the inventory of GetDyeCount_unk)
void CosEditProtector::fn_801D6754() {
    void* pad = lbl_8087EF70;
    if (fn_800A555C(pad, 0, 4)) {
        CharaTask* chara;
        int member = fn_801CF334()->mFace;
        chara = fn_801CF334()->mChara;
        int ok = 0;
        switch (m1BC0) {
        case 0:
            if (GetDyeCount_unk(0) > 0) {
                ok = 1;
            }
            break;
        case 1:
            if (GetDyeCount_unk(1) > 0) {
                ok = 1;
            }
            break;
        case 2:
            if (GetDyeCount_unk(0) > 0 && GetDyeCount_unk(1) > 0) {
                ok = 1;
            }
            break;
        case 3:
            ok = 1;
            break;
        }
        if (ok) {
            fn_80117228(0xF);
            fn_801D694C(member, chara, m1BC0);
        } else {
            fn_80117228(0xB);
        }
    } else if (fn_800A555C(pad, 0, 5)) {
        fn_80117228(2);
        vf34(4);
    } else {
        int cursor = m80;
        fn_80510D64(this, 1, 3);
        if (cursor != m80) {
            m1BC0 = m80;
        }
    }
}

void CosEditProtector::fn_801D694C(int member, CharaTask* chara, int mode) {
    int a = 0;
    int b = 0;
    if (mode == 0) {
        a = 1;
        b = 0;
    } else if (mode == 1) {
        a = 0;
        b = 1;
    } else if (mode == 2) {
        a = 1;
        b = 1;
    } else if (mode == 3) {
        a = 0;
        b = 0;
    }
    CharaStatus* status = Inventory::instance_->GetPlayerStatusInfo(member);
    if (a != status->fn_801347C8(0)) {
        fn_801CEA50(member, chara, 0, 0, a, 0x10);
        fn_801CEA50(member, chara, 2, 0, a, 0x10);
    }
    if (b != status->fn_801347C8(1)) {
        fn_801CEA50(member, chara, 1, 0, b, 0x10);
        fn_801CEA50(member, chara, 3, 0, b, 0x10);
    }
}

int CosEditProtector::fn_801D6A88() {
    if ((m7C >= 4 && m7C <= 10) || m7C == 0xE) {
        return 1;
    }
    return 0;
}

// NONMATCHING: 98.5% - register coloring of the list data pointer and the level pointer
void CosEditProtector::fn_801D6AB0() {
    int slots[4] = {0, 2, 3, 1};
    CosEditPart_unk* data = mPartLists[0].mData; // inferred: kept in a register in the original
    mPartLists[0].mSize = 0;
    mPartLists[1].mSize = 0;
    for (int i = 0; i < 4; i++) {
        int slot = slots[i];
        chr::CharaEquip_unk* equip = fn_801CE648(slot);
        if (equip->m00C == 0) {
            OptionParts_unk* parts = fn_801CE7E8(slot);
            OptionLevelList_unk* list = fn_8020DB18(equip->m414->GetLetter_unk(), slot);
            for (unsigned int j = 0; j < list->mCount; j++) {
                OptionLevel_unk* level = &list->mLevels[j];
                if (level->mFree) {
                    int found = 0;
                    for (unsigned int k = 0; k < 3; k++) {
                        if (level->mHashes[k] && parts->mParts[fn_8020D608(parts, level->mHashes[k])].mLevel) {
                            found = 1;
                        }
                    }
                    if (found) {
                        CosEditPart_unk part;
                        part.mSlot = slot;
                        part.mIndex = j;
                        if (mPartLists[0].size() < 128) {
                            new (&data[mPartLists[0].mSize]) CosEditPart_unk(part);
                            mPartLists[0].mSize++;
                        }
                    }
                }
            }
        }
    }
}

// NONMATCHING: 91.0% - one more saved register: the address of the UV argument copy is hoisted
// out of the loop and `list` does not share its register with the item offset
void CosEditProtector::fn_801D6C1C(int cursor) {
    atn::FixedArray_unk<CosEditPart_unk, 128>* list = &mPartLists[0];
    int n = 0; // inferred: list shown (only the first one is used; kept in a register in the original)
    m84 = list->mSize;
    m8C = 13;
    if (m80 >= m84) {
        cursor = MAX(0, m84 - 1);
        m80 = cursor;
    }
    if (m84 <= m8C) {
        m88 = 0;
    }
    if (m84 >= m8C && m84 - 1 <= m8C + m88 - 1) {
        m88 = m84 - m8C;
    }
    if (mLists[2].mBase) {
        atn::D2ATask_unk* task = mLists[2].mBase->GetTask();
        if (cursor >= 0 && cursor < m84) {
            int slot = list->mData[cursor].mSlot;
            if (slot == 0 || slot == 2) {
                fn_801D4E74(fn_801CF334()->mFace, task, 2);
            } else {
                fn_801D4E74(fn_801CF334()->mFace, task, 3);
            }
        } else {
            fn_801D4E74(fn_801CF334()->mFace, task, -1);
        }
    }
    int member = fn_801CF334()->mFace;
    for (int i = 0; i < m8C; i++) {
        atn::D2A_TaskInstanceBase<32, 8>* item = (atn::D2A_TaskInstanceBase<32, 8>*)mLists[2].mItems[i]->GetTask();
        int index = m88 + i;
        if (index >= 0 && index < (int)mPartLists[n].mSize) {
            CosEditPart_unk& part = mPartLists[n].mData[index];
            int slot = part.mSlot;
            chr::CharaEquip_unk* equip = fn_801CE648(slot);
            unsigned int* bits = fn_801CE87C(slot);
            OptionParts_unk* parts = fn_801CE7E8(slot);
            OptionLevelList_unk* levels = fn_8020DB18(equip->m414->GetLetter_unk(), slot);
            OptionLevel_unk* level = &levels->mLevels[part.mIndex];
            fn_801F837C(item, "item01", level->mName.mStr);
            int set = 1;
            if (bits) {
                for (unsigned int k = 0; k < 3; k++) {
                    if (level->mHashes[k]) {
                        unsigned int mask = 1 << fn_8020D608(parts, level->mHashes[k]);
                        if ((mask & *bits) != mask) {
                            set = 0;
                        }
                    }
                }
            }
            if (set) {
                item->mFrame = 1.0f;
                fn_801F6C80(item, "check", 1.0f);
            } else {
                item->mFrame = 3.0f;
                fn_801F6C80(item, "check", 0.0f);
            }
            if (slot == 2) {
                slot = 0;
            } else if (slot == 3) {
                slot = 1;
            }
            int icon = fn_80444B64(Inventory::instance_, fn_8020EF80((EquipParamEntry_unk*)fn_801CFAA8(member, slot)));
            CosEditUV_unk uv;
            uv = fn_80444C50(Inventory::instance_, icon);
            SetUV_unk("iconUv01", item, uv);
        } else {
            item->mFrame = 0.0f;
        }
    }
}

void CosEditProtector::fn_801D6F84() {
    if (m1BB0 == 0) {
        m84 = 10;
        m8C = 10;
        m80 = MAX(0, MIN(m80, m84 - 1));
    } else {
        m84 = mSetCount;
        m8C = mSetCount;
        m80 = MAX(0, MIN(m80, m84 - 1));
    }
    fn_801CF334();
    for (int i = 0; i < 10; i++) {
        atn::D2A_TaskInstanceBase<32, 8>* item = (atn::D2A_TaskInstanceBase<32, 8>*)mLists[1].mItems[i]->GetTask();
        if (i < m84) {
            wchar_t text[64];
            fn_800DD3FC(text, L"%02d", i + 1);
            fn_801F837C(item, "setName", text);
            item->mFrame = 1.0f;
        } else {
            item->mFrame = 0.0f;
        }
    }
}

void CosEditProtector::fn_801D70EC(atn::D3Anime* base) {
    if (!base || !base->GetTask()) {
        return;
    }
    atn::D2A_Task* task = (atn::D2A_Task*)base->GetTask();
    atn::D2A_DrawRect pos;
    task->ApplyFrame();
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            CosEditCell_unk& cell = mColorCells[y][x];
            cell.mEnabled = 0;
            OptionPalette_unk* palette = fn_8020BCEC(cell.mId);
            if (cell.mIcon && cell.mFrame) {
                char name[0x40];
                sprintf(name, "slt_palette%d%d", x, y);
                pos = fn_801F4E8C(task, fn_800DC6B4(name));
                fn_801F6E78((atn::D2A_TaskInstanceBase<32, 8>*)cell.mIcon->GetTask(), "base_pos", pos);
                fn_801F6E78((atn::D2A_TaskInstanceBase<32, 8>*)cell.mFrame->GetTask(), "base_pos", pos);
                if (palette) {
                    fn_801F7DF0((atn::D2A_TaskInstanceBase<32, 8>*)cell.mIcon->GetTask(), "paletteColor",
                                255.0f * (0.25f * palette->mColor.r), 255.0f * (0.25f * palette->mColor.g),
                                255.0f * (0.25f * palette->mColor.b));
                    int item = fn_80444828(Inventory::instance_, cell.mId);
                    if (fn_804444E8(Inventory::instance_, item) > 0 || item == 0) {
                        cell.mEnabled = 1;
                    }
                } else {
                    cell.mEnabled = 1;
                }
                fn_801F6C80((atn::D2A_TaskInstanceBase<32, 8>*)cell.mIcon->GetTask(), "unlock", cell.mEnabled);
                fn_801F6C80((atn::D2A_TaskInstanceBase<32, 8>*)cell.mFrame->GetTask(), "unlock", cell.mEnabled);
            }
        }
    }
}

void CosEditProtector::fn_801D7354(atn::D3Anime* base) {
    if (!base || !base->GetTask()) {
        return;
    }
    atn::D2A_Task* task = (atn::D2A_Task*)base->GetTask();
    atn::D2A_DrawRect pos;
    task->ApplyFrame();
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 4; x++) {
            CosEditCell_unk& cell = mPartCells[y][x];
            cell.mEnabled = 0;
            Palette_unk* part = fn_8020BD14(cell.mId);
            if (cell.mIcon && cell.mFrame) {
                char name[0x40];
                sprintf(name, "slt_palette%d%d", x, y);
                pos = fn_801F4E8C(task, fn_800DC6B4(name));
                fn_801F6E78((atn::D2A_TaskInstanceBase<32, 8>*)cell.mIcon->GetTask(), "base_pos", pos);
                fn_801F6E78((atn::D2A_TaskInstanceBase<32, 8>*)cell.mFrame->GetTask(), "base_pos", pos);
                if (part) {
                    if (fn_804444E8(Inventory::instance_, fn_8044493C(Inventory::instance_, cell.mId)) > 0 ||
                        cell.mId == 0x1F) {
                        cell.mEnabled = 1;
                    }
                } else {
                    cell.mEnabled = 1;
                }
                fn_801F6C80((atn::D2A_TaskInstanceBase<32, 8>*)cell.mIcon->GetTask(), "unlock", cell.mEnabled);
                fn_801F6C80((atn::D2A_TaskInstanceBase<32, 8>*)cell.mFrame->GetTask(), "unlock", cell.mEnabled);
            }
        }
    }
}

void CosEditProtector::fn_801D7560(int member, CharaStatus* status, NewCampPartsEntry_unk* entry, atn::D3Anime* base) {
    if (!entry || !base || !base->GetTask()) {
        return;
    }
    fn_801D7AC0((int*)&Inventory::instance_->mEquipShown[member], mPalettes, entry, base, -1);
    mPalettes[8]->mVisible = 0;
    char name[0x40];
    sprintf(name, "palette%02d", 8);
    atn::D2A_Task* task = (atn::D2A_Task*)base->GetTask();
    int mark = status->m231 - 1;
    atn::D3AnimeInstance* inst = mark >= 0 ? mColorMarks[mark] : 0;
    if (inst && inst->GetTask()) {
        inst->mVisible = 1;
        fn_805112A8(this, entry, inst, atn::Vector3(0.0f, 0.0f, 0.0f), atn::Vector3(1.0f, 1.0f, 1.0f), "reference");
        fn_801F6E78((atn::D2A_TaskInstanceBase<32, 8>*)inst->GetTask(), "base_pos",
                    fn_801F4E8C(task, fn_800DC6B4(name)));
    }
    if (m1378->GetTask()) {
        m1378->mVisible = 1;
        fn_805112A8(this, entry, m1378, atn::Vector3(0.0f, 0.0f, 0.0f), atn::Vector3(1.0f, 1.0f, 1.0f), "reference");
        fn_801F6E78((atn::D2A_TaskInstanceBase<32, 8>*)m1378->GetTask(), "base_pos",
                    fn_801F4E8C(task, fn_800DC6B4(name)));
        if (mark > 0) {
            fn_801F6C80((atn::D2A_TaskInstanceBase<32, 8>*)m1378->GetTask(), "unlock", 1.0f);
        } else {
            fn_801F6C80((atn::D2A_TaskInstanceBase<32, 8>*)m1378->GetTask(), "unlock", 0.0f);
        }
    }
}

void CosEditProtector::fn_801D77A0(int* ids, atn::D3AnimeInstance** items, NewCampPartsEntry_unk* entry,
                                   atn::D3AnimeInstance* base, int item) {
    fn_801D77A4(ids, items, entry, base, item);
}

void CosEditProtector::fn_801D77A4(int* ids, atn::D3AnimeInstance** items, NewCampPartsEntry_unk* entry,
                                   atn::D3AnimeInstance* base, int item) {
    if (!items) {
        return;
    }
    CosEditUV_unk uv(1.0f, 1.0f, 1.0f, 1.0f);
    if (item >= 0) {
        CosEditItemInfo_unk* info = fn_802091E8(item);
        int icon = info ? info->mIcon : 0;
        if (icon > 0) {
            int index = icon - 1;
            int x = index % 4;
            int y = index / 4;
            uv = CosEditUV_unk(0.25f * x, 0.5f * y, 0.25f * (x + 1), 0.5f * (y + 1));
        }
    }
    SetUV_unk("uv0001", (atn::D2A_TaskInstanceBase<32, 8>*)base->GetTask(), uv);
    SetUV_unk("mask_uv0001", (atn::D2A_TaskInstanceBase<32, 8>*)base->GetTask(), uv);
    for (int i = 0; i < 8; i++) {
        atn::D3AnimeInstance* mark = items[i];
        mark->mVisible = 1;
        fn_805112A8(this, entry, mark, atn::Vector3(0.0f, 0.0f, 0.0f), atn::Vector3(1.0f, 1.0f, 1.0f), "reference");
        char name[0x40];
        sprintf(name, "palette%02d", i);
        fn_801F6E78((atn::D2A_TaskInstanceBase<32, 8>*)mark->GetTask(), "base_pos",
                    ((atn::D2A_TaskInstanceBase<32, 8>*)base->GetTask())->GetRect_unk(name));
        OptionPalette_unk* palette = fn_8020BCEC(ids[fn_80117214(i)]);
        if (palette) {
            fn_801F6C80((atn::D2A_TaskInstanceBase<32, 8>*)mark->GetTask(), "unlock", 1.0f);
            fn_801F7DF0((atn::D2A_TaskInstanceBase<32, 8>*)mark->GetTask(), "paletteColor",
                        255.0f * (0.25f * palette->mColor.r), 255.0f * (0.25f * palette->mColor.g),
                        255.0f * (0.25f * palette->mColor.b));
        } else {
            fn_801F6C80((atn::D2A_TaskInstanceBase<32, 8>*)mark->GetTask(), "unlock", 0.0f);
        }
    }
    items[8]->mVisible = 0;
}

void CosEditProtector::fn_801D7AC0(int* ids, atn::D3AnimeInstance** items, NewCampPartsEntry_unk* entry,
                                   atn::D3Anime* base, int item) {
    if (!items) {
        return;
    }
    CosEditUV_unk uv(1.0f, 1.0f, 1.0f, 1.0f);
    if (item >= 0) {
        CosEditItemInfo_unk* info = fn_802091E8(item);
        int icon = info ? info->mIcon : 0;
        if (icon > 0) {
            int index = icon - 1;
            int x = index % 4;
            int y = index / 4;
            uv = CosEditUV_unk(0.25f * x, 0.5f * y, 0.25f * (x + 1), 0.5f * (y + 1));
        }
    }
    SetUV_unk("uv0001", (atn::D2A_Task*)base->GetTask(), uv);
    SetUV_unk("mask_uv0001", (atn::D2A_Task*)base->GetTask(), uv);
    for (int i = 0; i < 8; i++) {
        atn::D3AnimeInstance* mark = items[i];
        mark->mVisible = 1;
        fn_805112A8(this, entry, mark, atn::Vector3(0.0f, 0.0f, 0.0f), atn::Vector3(1.0f, 1.0f, 1.0f), "reference");
        char name[0x40];
        sprintf(name, "palette%02d", i);
        fn_801F6E78((atn::D2A_TaskInstanceBase<32, 8>*)mark->GetTask(), "base_pos",
                    GetRect_unk((atn::D2A_Task*)base->GetTask(), name));
        OptionPalette_unk* palette = fn_8020BCEC(ids[fn_80117214(i)]);
        if (palette) {
            fn_801F6C80((atn::D2A_TaskInstanceBase<32, 8>*)mark->GetTask(), "unlock", 1.0f);
            fn_801F7DF0((atn::D2A_TaskInstanceBase<32, 8>*)mark->GetTask(), "paletteColor",
                        255.0f * (0.25f * palette->mColor.r), 255.0f * (0.25f * palette->mColor.g),
                        255.0f * (0.25f * palette->mColor.b));
        } else {
            fn_801F6C80((atn::D2A_TaskInstanceBase<32, 8>*)mark->GetTask(), "unlock", 0.0f);
        }
    }
    items[8]->mVisible = 0;
}

void CosEditProtector::fn_801D7DE8(int* x, int* y, int cols, int rows, int count) {
    void* pad = lbl_8087EF70;
    if (fn_800A55AC(pad, 0, 0x1A) || fn_800A55AC(pad, 0, 0)) {
        if (--*y < 0) {
            *y = rows - 1;
        }
        if (count <= *x + *y * cols) {
            *y = *y - 1;
        }
        fn_80117228(3);
    } else if (fn_800A55AC(pad, 0, 0x19) || fn_800A55AC(pad, 0, 1)) {
        (*y)++;
        if (count <= *x + *y * cols) {
            (*y)++;
        }
        if (*y >= rows) {
            *y = 0;
        }
        fn_80117228(3);
    } else if (fn_800A55AC(pad, 0, 0x18) || fn_800A55AC(pad, 0, 3)) {
        if (++*x >= cols) {
            *x = 0;
        }
        if (count <= *x + *y * cols) {
            *x = 0;
        }
        fn_80117228(3);
    } else if (fn_800A55AC(pad, 0, 0x17) || fn_800A55AC(pad, 0, 2)) {
        if (--*x < 0) {
            *x = cols - 1;
            if (count <= *x + *y * cols) {
                *x = count % cols - 1;
            }
        }
        fn_80117228(3);
    }
}

CosEditCell_unk* CosEditProtector::fn_801D8040() {
    return &mColorCells[m1F8][m1FC];
}

CosEditCell_unk* CosEditProtector::fn_801D8060() {
    return &mPartCells[m200][m204];
}
