#include "game/ShopBase.h"
#include "game/GameUI.h"
#include "atn/Memory.h"
#include "atn/SoundRef.h"
#include "game/Inventory.h"
#include "game/MenuCommonTask.h"
#include "game/MarketTask.h"
#include "atn/Csv.h"
#include "math/Vec.h"
#include "std/new.h"

extern "C" int sprintf(char* s, const char* format, ...);
extern "C" ShopD2A_unk* fn_801F3FF8(atn::Task* parent, const char* path, int loop); // inferred: d2anime task
extern "C" ShopLayout_unk* fn_801F64D0(atn::Task* parent, const char* path);      // inferred: layout anime
// inferred: as in game/ShopIdentify.cpp (the inline keeps the path in a register across the loop)
inline ShopLayout_unk* CreateLayout_unk(atn::Task* parent, const char* path) {
    return fn_801F64D0(parent, path);
}
extern "C" atn::SoundRef fn_80117228(int id); // inferred: plays a system sound effect
extern "C" void* lbl_8087EF70;
extern "C" int fn_800A555C(void* pad, int port, int button); // inferred: button triggered
extern "C" void fn_804A436C(int* cursor, int count, int page, int step, int arg); // inferred: moves a cursor
extern "C" void fn_804A39EC(ShopD2A_unk* anime, int arg1, int repeat, float frame, float speed);
extern "C" void fn_804A3A68(ShopLayout_unk* layout, int arg1, int repeat, float frame, float speed);
struct ShopField_unk;
extern "C" ShopField_unk* lbl_8087F430;           // inferred: field
extern "C" void fn_8036E89C(ShopField_unk* field); // inferred: called when a shop closes
extern "C" void fn_804A4494(int* cursor, int* top, int count, int page, int a, int b); // inferred: moves a list cursor
extern "C" int fn_804444E8(Inventory* inv, int id);           // inferred: number of items
extern "C" void fn_8044441C(Inventory* inv, int id, int num); // inferred: removes items
extern "C" void fn_8044D490(Inventory* inv, int index, int num); // inferred: removes the item at `index`
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
// inferred: 2D point of a d2anime pane (0x14 bytes; as in game/ShopBuySell.cpp)
struct ShopPoint_unk {
    ShopPoint_unk() : x(0.0f), y(0.0f), w(0.0f), h(0.0f), z(0.0f) {}
    float x, y, w, h, z;
};
extern "C" ShopPoint_unk fn_801F4E8C(atn::Task* layout, unsigned int hash);
inline ShopPoint_unk GetPanePos_unk(atn::Task* layout, const char* name) {
    return fn_801F4E8C(layout, fn_800DC6B4(name));
}
extern "C" void fn_804A53D4(MenuCommonTask* menu, int id, const char* name, const ShopPoint_unk* rect, int end);
extern "C" void fn_804A55FC(MenuCommonTask* menu, atn::Task* id, const char* name, const ShopPoint_unk* rect,
                            int end);
extern "C" void fn_801FEE08(void* layout, unsigned int hash, const wchar_t* text); // inferred: sets a text
extern "C" void fn_801FECE0(void* layout, unsigned int hash, float value);
inline void SetPaneText_unk(void* layout, const char* name, const wchar_t* text) {
    fn_801FEE08(layout, fn_800DC6B4(name), text);
}
inline void SetPaneFloat_unk(void* layout, const char* name, float value) {
    fn_801FECE0(layout, fn_800DC6B4(name), value);
}
extern "C" void fn_801FED24(void* layout, unsigned int hash, int index, float value);
inline void SetPaneValue_unk(void* layout, const char* name, int index, float value) {
    fn_801FED24(layout, fn_800DC6B4(name), index, value);
}
extern "C" int fn_800DD3FC(wchar_t* buf, const wchar_t* format, ...); // inferred: swprintf
extern "C" const wchar_t lbl_808813D0[2]; // inferred: empty text
// inferred: message table (lbl_8087F1E4) seen as pairs; name invented
struct ShopMsgTable_unk {
    struct Entry {
        int m00;
        const wchar_t* mText;
    };
    const wchar_t* Get_unk(int id) {
        if (mEntries[id].mText != 0) {
            return mEntries[id].mText;
        }
        return lbl_808813D0;
    }
    Entry mEntries[1];
};
extern "C" ShopMsgTable_unk* lbl_8087F1E4;
extern "C" int fn_800A55AC(void* pad, int port, int button); // inferred: button repeated
extern "C" void fn_801F6C80(atn::Task* layout, const char* name, float value);    // inferred: sets a value
extern "C" void fn_801F837C(atn::Task* layout, const char* name, const wchar_t* text); // inferred: sets a text
extern "C" void fn_801F8598(atn::Task* layout, const char* name, int value, int arg); // inferred: sets a number
extern "C" void fn_801F7DF0(atn::Task* layout, const char* name, float r, float g, float b); // inferred: sets a colour
extern "C" void fn_801F6E78(atn::Task* task, const char* name, const ShopPoint_unk* pos);
extern "C" void fn_804A4930(atn::Task* task, int top, int count, int page); // inferred: scroll bar
// Only the members used here (the full class is in atn/D2A_Task.h, whose extern "C" declarations
// clash with the local ones of this file).

namespace atn {

class D2A_Task : public Task {
public:
    void SetParam2_unk(const char* name, const float* v);        // 0x801F48C8
    void SetNumber_unk(const char* name, int value, int digits); // 0x801F4CB4
};
} // namespace atn
// inferred: texture rectangle (as in game/ShopIdentify.cpp)
struct ShopUV_unk {
    ShopUV_unk() {}
    ShopUV_unk(const ShopUV_unk& o) {
        __Vec4Copy(&u0, &o.u0);
    }
    float u0, v0, u1, v1;
};
extern "C" void fn_801F7590(atn::Task* layout, const char* name, ShopUV_unk uv);
// inferred: item data (fn_80211480)
struct ShopItem_unk {
    unsigned char m00[0x04];
    int mCountId;          // 0x04
    const wchar_t* mName;  // 0x08
    const wchar_t* mText;  // 0x0C
    unsigned char m10[0xBC - 0x10];
    short mKind;           // 0xBC: inferred: 9 for a material sold at the market price
    unsigned char mBE[0xC8 - 0xBE];
    int mPrice;            // 0xC8
};
extern "C" ShopItem_unk* fn_80211480(int id);
// inferred: weapon data (fn_80206C50)
// inferred: skill slot of a weapon or an armour (0x14 bytes)
struct ShopSkill_unk {
    unsigned char m00[0x14];
};
// inferred: parameters shared by weapons and armours (the first four floats)
struct ShopParams_unk {
    float m00;
    float m04;
    float m08;
    float m0C;
};
struct ShopWeapon_unk {
    ShopParams_unk mParams; // 0x00
    unsigned char m10[0x88 - 0x10];
    const wchar_t* mHelp; // 0x88
    unsigned char m8C[0xB8 - 0x8C];
    int mKind; // 0xB8
    unsigned char mBC[0xC4 - 0xBC];
    ShopSkill_unk mSkills[2]; // 0xC4
    unsigned char mEC[0x124 - 0xEC];
    int mPrice; // 0x124
};
extern "C" int fn_80206B9C(int id); // inferred: nonzero for a weapon
// inferred: armour data (fn_8020EFEC)
struct ShopArmor_unk {
    ShopParams_unk mParams; // 0x00
    unsigned char m10[0x7C - 0x10];
    int m7C;
    unsigned char m80[0x84 - 0x80];
    const wchar_t* mHelp; // 0x84
    unsigned char m88[0xE4 - 0x88];
    ShopSkill_unk mSkills[2]; // 0xE4
};
extern "C" int fn_8020EF4C(ShopArmor_unk* armor); // inferred: the armour has a bonus part
extern "C" ShopParams_unk* fn_8020ED84(int slot, int item); // inferred: armour data
extern "C" int fn_8021C6A4(wchar_t* out, const ShopSkill_unk* skill); // inferred: name of a skill
extern "C" int fn_8020EF04(int id); // inferred: nonzero for an armour
extern "C" ShopArmor_unk* fn_8020EFEC(int id);
extern "C" void fn_804A3C24(MenuCommonTask* menu, const wchar_t* text, int id); // inferred: help text
extern "C" const wchar_t* fn_80116FC0(int a, int id); // inferred: message text
extern "C" unsigned long strlen(const char* s);
extern "C" ShopWeapon_unk* fn_80206C50(int id);
extern "C" void fn_8044D6AC(Inventory* party, int money); // inferred: pays money
extern "C" int fn_80211648(ShopItem_unk* item); // inferred: nonzero if the item can be sold
extern "C" int fn_8021150C(int code);           // inferred: index of an item code in the bag
extern "C" int fn_8011728C(int kind, ShopItem_unk* item); // inferred: the shop of kind `kind` deals the item
extern "C" int fn_8044D560(Inventory* party);           // inferred: number of items to identify
extern "C" int fn_8044D500(Inventory* party, int index); // inferred: item to identify
// inferred: comparison of the sorted list (empty object passed by value)
struct ShopEntryLess_unk {
    unsigned char m00;
};
// inferred: std::sort of the list (template code of this file, 0x80582598-0x80584750; not reconstructed)
extern "C" void fn_80582598(ShopEntry5C_unk* first, ShopEntry5C_unk* last, ShopEntryLess_unk comp);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" int atoi(const char* s);
extern "C" int fn_800DC12C(const char* s);    // inferred: string to int (item codes too)
extern "C" double fn_800DC288(const char* s); // inferred: string to float
extern "C" void* lbl_8087EEC8;                // inferred: font
extern "C" void fn_8006F2F0(void* font, wchar_t* dst, const char* src, int n); // inferred: to wide characters
// inferred: field (lbl_8087F430): chapter at 0x10D0
struct ShopField_unk {
    unsigned char m0000[0x10D0];
    int mChapter; // 0x10D0
};
extern "C" int fn_80370174(ShopField_unk* field, int index); // inferred: event flag
extern "C" int fn_80444564(Inventory* party, int id); // inferred: number of equipment items
extern "C" int fn_80444BE8(Inventory* party, ShopItem_unk* item);
extern "C" int fn_80444C48(Inventory* party);
extern "C" ShopUV_unk fn_80444C50(Inventory* party, int icon);
// inferred: debug menu items (atn/MindowsCommon.h: AddBool, AddInt)
extern "C" void fn_80087994(void* folder, const char* name, int* value, void* func, void* arg);
extern "C" void fn_800874C8(void* folder, const char* name, int* value, int min, int max, int step, void* func,
                            void* arg);

// NONMATCHING: 97.11% - registers of the arguments and the this+0x20000 base: the original recomputes it
// (addis) after the array loops, here it is kept from the loop end
ShopBase::ShopBase(atn::Task* parent, int kind, int a, int b)
    : atn::Task(parent), mParent(parent), mKind(kind), m50(a), m54(b), mTitle(0), mName(0), m6C(0), m78(0),
      m7C(-1), m84(0), m8C(0), mDC(0), mE0(0), mState(0), mEC(0), mF0(0), mF8(0), m32FC(0), m25B14(0),
      m25B18(0), m25B1C(0), m25B20(0), m25B24(0), m25B28(0), m25B3C(0), m25B40(1), m25B4C(0), m25B50(0),
      m25B54(0), m25B58(0), m25B5C(0), m25B60(0), m25B64(0) {
    m25B44.fn_80470364("/preload/shop.pk", 0, 0, 1);
    m25B00 = 6;
    mF8 = 0;
    m32FC = 0;
    m25B30.clear();
    if (m54 > 0 && lbl_8087F490 != 0 && lbl_8087F490->m262C.m0C == 0) {
        lbl_8087F490->fn_803E627C(b);
        m25B3C = 1;
    }
    (m68 = fn_801F64D0(this, "scroll_bar.d2a"))->fn_800D246C(1);
    (m70 = fn_801F3FF8(this, "/data/d2anime/shop_logo.d2a", 0))->fn_800D246C(1);
    (m74 = fn_801F3FF8(this, "/data/d2anime/camp_info.d2a", 0))->fn_800D246C(1);
    (m80 = fn_801F3FF8(this, "/data/d2anime/camp_info2.d2a", 0))->fn_800D246C(1);
    (m90 = fn_801F3FF8(this, "/data/d2anime/shop_material_content2.d2a", 0))->fn_800D246C(1);
    (m94 = fn_801F3FF8(this, "/data/d2anime/shop_material_content3.d2a", 0))->fn_800D246C(1);
    for (int i = 0; i < 2; i++) {
        (m98[i] = CreateLayout_unk(this, "/data/d2anime/shop_upgrade_skill_list.d2a"))->fn_800D246C(1);
    }
    (mA8 = fn_801F3FF8(this, "/data/d2anime/shop_material_list_base.d2a", 0))->fn_800D246C(1);
    for (int i = 0; i < 10; i++) {
        (mAC[i] = CreateLayout_unk(this, "/data/d2anime/shop_material_list.d2a"))->fn_800D246C(1);
    }
    (mD4 = fn_801F3FF8(this, "/data/d2anime/shop_material_content.d2a", 0))->fn_800D246C(1);
    (mD8 = fn_801F3FF8(this, "/data/d2anime/shop_material_noitem.d2a", 0))->fn_800D246C(1);
    (m25B2C = fn_801F3FF8(this, "/data/d2anime/shop_material_count.d2a", 0))->fn_800D246C(1);
    void* group = m25B48.Load("ShopParam");
    fn_80087994(group, "disableAppraise", &m25B50, 0, 0);
    fn_80087994(group, "disableChapter", &m25B54, 0, 0);
    fn_80087994(group, "disableGlobalFlag", &m25B58, 0, 0);
    fn_800874C8(group, "gameChapter", &m25B4C, 0, 999999, 1, 0, 0);
    if (mKind == 2 && a == 2) {
        mFile.vf0C("database/shop/color_shop.csv");
    } else {
        char path[0x100];
        if (mKind == 0 && m50 == 0) {
            sprintf(path, "database/shop/shop_00_00.csv");
        } else {
            sprintf(path, "database/shop/shop_%02d_%02d.csv", mKind + 1, m50);
        }
        mFile.vf0C(path);
    }
}

ShopBase::~ShopBase() {}

int ShopBase::fn_8057F880() {
    if (!fn_800D3FA4()) {
        return 1;
    }
    if (mFile.fn_80473F50()) {
        return 1;
    }
    if (lbl_8087F490 != 0 && lbl_8087F490->fn_803E644C()) {
        return 1;
    }
    return 0;
}

void ShopBase::fn_8057F8F8() {
    m25B44.fn_80470528();
    if (m54 > 0) {
        vf1C(0);
        if (lbl_8087F490 != 0 && m25B3C != 0) {
            lbl_8087F490->fn_803E64F0();
        }
    } else {
        vf1C(4);
        fn_80117228(30);
    }
    fn_804A39EC(m74, 1, 0, 0.0f, -3.0f);
    fn_804A39EC(m80, 1, 0, 0.0f, -3.0f);
    fn_804A39EC(m70, 1, 0, 0.0f, 1.0f);
    fn_804A3A68(m68, 1, 0, 0.0f, 1.0f);
    fn_804A39EC(m90, 1, 0, 0.0f, 1.0f);
    fn_804A39EC(m94, 1, 0, 0.0f, 1.0f);
    for (int i = 0; i < 2; i++) {
        fn_804A3A68(m98[i], 1, 0, 0.0f, 1.0f);
    }
    fn_804A39EC(mD4, 1, 0, 0.0f, 1.0f);
    fn_804A39EC(mD8, 1, 0, 0.0f, 1.0f);
    fn_804A39EC(m25B2C, 1, 0, 0.0f, 0.0f);
    fn_804A39EC(mA8, 1, 0, 0.0f, 0.0f);
    for (int i = 0; i < 10; i++) {
        fn_804A3A68(mAC[i], 1, 0, 0.0f, 1.0f);
        mAC[i]->mFlags |= 4;
    }
    fn_8058036C();
}

void ShopBase::fn_8057FB20() {
    mF4 = 2;
    if (lbl_8087F490 != 0) {
        mF4 = 0;
        mF0 = 0;
        if (lbl_8087F490 != 0 && m25B3C != 0) {
            lbl_8087F490->fn_803E65F8();
        }
    }
}

void ShopBase::vf28() {
    if (mEC == 0) {
        mEC = 1;
        if (lbl_8087F490 != 0 && m25B3C != 0) {
            lbl_8087F490->fn_803E656C();
        }
    } else if (mEC == 1) {
        if (lbl_8087F490 == 0 || lbl_8087F490->m262C.m0C == 2 || m25B3C == 0) {
            mEC = 2;
            vf1C(4);
            fn_80117228(30);
        }
    }
}

void ShopBase::vf2C() {
    if (mF4 == 0) {
        mF4 = 1;
        if (lbl_8087F490 != 0 && m25B3C != 0) {
            lbl_8087F490->fn_803E65F8();
        }
    } else if (mF4 == 1) {
        if (lbl_8087F490 == 0 || lbl_8087F490->m262C.m0C == 0 || m25B3C == 0) {
            mF4 = 2;
        }
    } else {
        mFile.fn_80473F88();
        if (lbl_8087F490 != 0 && m25B3C != 0) {
            lbl_8087F490->fn_803E63D0();
        }
        if (lbl_8087F430 != 0) {
            fn_8036E89C(lbl_8087F430);
        }
    }
}

void ShopBase::vf30() {
    void* pad = lbl_8087EF70;
    bool ok = m78 == 0 || m74->IsEnd_unk();
    if (ok && (fn_800A555C(pad, 0, 4) || fn_800A555C(pad, 0, 5))) {
        vf24(m7C);
        fn_8058181C(0, 0, 0);
        vf1C(mPrevState);
    }
}

void ShopBase::vf24(int message) {}

void ShopBase::vf34() {
    mScroll.mCount = 2;
    mScroll.mCursor = m8C;
    fn_804A436C(&mScroll.mCursor, 2, mScroll.mPage, 1, 3);
    m8C = mScroll.mCursor;
    void* pad = lbl_8087EF70;
    if (fn_800A555C(pad, 0, 4)) {
        if (m8C == 0) {
            vf20(m88);
        } else {
            fn_80117228(2);
            vf1C(mPrevState);
        }
        fn_80581964(0, 0, 0);
    } else if (fn_800A555C(pad, 0, 5)) {
        fn_80117228(2);
        vf1C(mPrevState);
        fn_80581964(0, 0, 0);
    }
}

void ShopBase::fn_8057FF38() {
    fn_804A4494(&mScroll.mCursor, &mScroll.mTop, mScroll.mCount, mScroll.mPage, 1, 3);
    mDC = mScroll.mCursor;
    mE0 = mScroll.mTop;
    void* pad = lbl_8087EF70;
    if (fn_800A555C(pad, 0, 4)) {
        if (m32FC == 0) {
            fn_80117228(11);
        } else {
            Inventory* inv = Inventory::Order();
            ShopEntry5C_unk* entry = &mEntries5C[mDC];
            int error;
            if (entry->m48 != 0 && fn_804444E8(inv, entry->m00) <= 0) {
                error = 10;
            } else {
                error = entry->m50 != 0 ? 0 : 3;
            }
            if (error == 0) {
                fn_80117228(1);
                vf1C(6);
            } else {
                fn_80117228(11);
                fn_8058181C(1, error, 0);
                vf1C(2);
            }
        }
    } else if (fn_800A555C(pad, 0, 5)) {
        fn_80117228(2);
        vf1C(4);
    }
}

void ShopBase::fn_805800FC() {
    fn_80580B50();
    void* pad = lbl_8087EF70;
    if (fn_800A555C(pad, 0, 4)) {
        ShopEntry5C_unk* entry = &mEntries5C[mDC];
        int price = entry->m04 * m25B24;
        if (price + Inventory::instance_->mMoney >= 999999) {
            fn_80581964(1, 5, 0);
            vf1C(3);
        } else {
            if (entry->m48 != 0) {
                fn_8044441C(Inventory::instance_, entry->m00, m25B24);
            } else {
                fn_8044D490(Inventory::instance_, entry->m4C, m25B24);
            }
            Inventory::instance_->AddMoney(price);
            fn_80581FD8();
            fn_80117228(25);
            vf1C(5);
        }
    } else if (fn_800A555C(pad, 0, 5)) {
        fn_80117228(2);
        vf1C(5);
    }
}

void ShopBase::fn_80580264() {
    fn_8058036C();
    if (m74 != 0) {
        if (m78 != 0) {
            m74->mSpeed = 3.0f;
        } else {
            m74->mSpeed = -3.0f;
        }
    }
    if (m80 != 0) {
        if (m84 != 0) {
            m80->mSpeed = 3.0f;
            if (m80->IsEnd_unk()) {
                char name[0x40];
                sprintf(name, "cursor_point%d", m8C + 1);
                ShopPoint_unk pos = GetPanePos_unk(m80, name);
                fn_804A53D4(MenuCommonTask::sInstance, 0, 0, &pos, 0);
            }
        } else {
            m80->mSpeed = -3.0f;
        }
    }
    fn_80581674();
}

void ShopBase::fn_8058036C() {
    m70->FreezeTask();
    if (mState != 0 && mState != 1) {
        m70->MeltTask();
    }
    m68->FreezeTask();
    if (m6C != 0) {
        m68->MeltTask();
        m6C = 0;
    }
    SetPaneText_unk(m70->mLayout, "title", lbl_8087F1E4->Get_unk(mTitle + 0x1D1));
    SetPaneText_unk(m70->mLayout, "shopName", lbl_8087F1E4->Get_unk(mName + 0x1D1));
    switch (vf3C()) {
    case 0:
        SetPaneFloat_unk(m70->mLayout, "iconType", 0.0f);
        break;
    case 1:
        SetPaneFloat_unk(m70->mLayout, "iconType", 1.0f);
        break;
    case 2:
        SetPaneFloat_unk(m70->mLayout, "iconType", 2.0f);
        break;
    case 3:
        SetPaneFloat_unk(m70->mLayout, "iconType", 3.0f);
        break;
    }
}

int ShopBase::vf3C() {
    return 0;
}

void ShopBase::fn_80580580() {
    fn_805807A4();
    if (m32FC != 0 && mDC < m32FC) {
        ShopEntry5C_unk* entry = &mEntries5C[mDC];
        fn_80581008(entry->m00);
        if (entry->m48 != 0) {
            fn_805811DC(entry->m00);
        }
        fn_80581570(entry->m00, entry->m48);
    }
    if (m32FC == 0) {
        mD8->MeltTask();
    } else {
        char name[0x40];
        int i = mDC - mE0;
        ShopD2A_unk* base = mA8;
        ShopLayout_unk* item = mAC[i];
        sprintf(name, "list_cursor%d", i + 1);
        ShopPoint_unk pos = GetPanePos_unk(base, name);
        fn_804A55FC(MenuCommonTask::sInstance, item, 0, &pos, 0);
    }
}

void ShopBase::fn_80580690() {
    fn_805807A4();
    if (m32FC != 0 && mDC < m32FC) {
        ShopEntry5C_unk* entry = &mEntries5C[mDC];
        fn_80581008(entry->m00);
        if (entry->m48 != 0) {
            fn_805811DC(entry->m00);
        }
        fn_80581570(entry->m00, entry->m48);
    }
    char name[0x40];
    int i = mDC - mE0;
    ShopD2A_unk* base = mA8;
    ShopLayout_unk* item = mAC[i];
    sprintf(name, "list_cursor%d", i + 1);
    ShopPoint_unk pos = GetPanePos_unk(base, name);
    fn_804A55FC(MenuCommonTask::sInstance, item, 0, &pos, 1);
    m25B18 = mDC - mE0;
    m25B14 = 1;
}

void ShopBase::fn_805807A4() {
    Inventory* inv = Inventory::instance_;
    mA8->MeltTask();
    mD4->MeltTask();
    ((atn::D2A_Task*)mD4)->SetNumber_unk("myMoney", Inventory::instance_->mMoney, 0);
    int num = m32FC;
    if (mDC < num) {
        ShopEntry5C_unk* entry = &mEntries5C[mDC];
        if (entry->m48 != 0) {
            ((atn::D2A_Task*)mD4)->SetNumber_unk("itemNum", fn_804444E8(inv, entry->m00), 0);
        } else {
            ((atn::D2A_Task*)mD4)->SetNumber_unk("itemNum", 1, 0);
        }
    }
    fn_80584750(mA8, mScroll.mTop, m32FC, 10);
    ShopPoint_unk point;
    for (int i = 0; i < 10; i++) {
        char name[0x40];
        sprintf(name, "list_point%d", i + 1);
        point = GetPanePos_unk(mA8, name);
        fn_801F6E78(mAC[i], "basePos", &point);
        int index = i + mScroll.mTop;
        if (index < m32FC) {
            ShopEntry5C_unk* entry = &mEntries5C[index];
            ShopItem_unk* item = fn_80211480(entry->m00);
            int icon;
            if (entry->m48 != 0) {
                fn_801F837C(mAC[i], "itemName", item->mName);
                fn_801F8598(mAC[i], "num", fn_804444E8(inv, entry->m00), 0);
                icon = fn_80444BE8(Inventory::instance_, item);
            } else {
                ShopWeapon_unk* weapon = fn_80206C50(entry->m00);
                if (weapon != 0) {
                    fn_801F837C(mAC[i], "itemName", lbl_8087F1E4->Get_unk(weapon->mKind + 0xEB));
                } else {
                    fn_801F837C(mAC[i], "itemName", L"WEAPON");
                }
                fn_801F837C(mAC[i], "num", L"1");
                icon = fn_80444C48(Inventory::instance_);
            }
            if (entry->m50 != 0) {
                fn_801F7DF0(mAC[i], "itemCol", 255.0f, 255.0f, 255.0f);
                fn_801F6C80(mAC[i], "nonePrice", 0.0f);
                fn_801F6C80(mAC[i], "showPrice", 1.0f);
            } else {
                fn_801F7DF0(mAC[i], "itemCol", 100.0f, 100.0f, 100.0f);
                fn_801F6C80(mAC[i], "nonePrice", 1.0f);
                fn_801F6C80(mAC[i], "showPrice", 0.0f);
            }
            fn_801F8598(mAC[i], "price", entry->m04, 0);
            ShopUV_unk uv = fn_80444C50(Inventory::instance_, icon);
            fn_801F7590(mAC[i], "iconUv", uv);
            mAC[i]->MeltTask();
        }
    }
}

void ShopBase::fn_80580B50() {
    void* pad = lbl_8087EF70;
    if (fn_800A55AC(pad, 0, 0) || fn_800A55AC(pad, 0, 26)) {
        int num = m25B24 + 1;
        m25B24 = num < m25B28 ? num : m25B28;
        m25B1C = 5;
        m25B20 = 1;
        fn_80117228(3);
    } else if (fn_800A55AC(pad, 0, 1) || fn_800A55AC(pad, 0, 25)) {
        int num = m25B24 - 1;
        m25B24 = num > 1 ? num : 1;
        m25B1C = 5;
        m25B20 = 2;
        fn_80117228(3);
    } else if (fn_800A55AC(pad, 0, 2) || fn_800A55AC(pad, 0, 23)) {
        int num = m25B24 + 10;
        m25B24 = num < m25B28 ? num : m25B28;
        m25B1C = 5;
        m25B20 = 1;
        fn_80117228(3);
    } else if (fn_800A55AC(pad, 0, 3) || fn_800A55AC(pad, 0, 24)) {
        int num = m25B24 - 10;
        m25B24 = num > 1 ? num : 1;
        m25B1C = 5;
        m25B20 = 2;
        fn_80117228(3);
    }
}

// inferred: hides a d2anime task (rewinds it if it was already hidden)
extern "C" void fn_80580D80(ShopD2A_unk* anime) {
    if (anime != 0) {
        if (anime->IsFlag_unk(4)) {
            anime->mFrame = 0.0f;
        }
        anime->FreezeTask();
    }
}

// inferred: same for a layout anime task
extern "C" void fn_80580DB0(atn::Task* anime) {
    ShopLayout_unk* layout = (ShopLayout_unk*)anime;
    if (layout != 0) {
        if (layout->IsFlag_unk(4)) {
            layout->mFrame = 0.0f;
        }
        layout->FreezeTask();
    }
}

void ShopBase::fn_80580DE0() {
    fn_80580D80(mD4);
    fn_80580D80(mD8);
    fn_80580D80(m90);
    fn_80580D80(m94);
    for (int i = 0; i < 2; i++) {
        fn_80580DB0(m98[i]);
    }
    mA8->FreezeTask();
    for (int i = 0; i < 10; i++) {
        fn_80580DB0(mAC[i]);
    }
}

// inferred: parameter rows of the skill window: message ids and the parameter shown in each row
static const struct {
    int mMessages[5];
    int mWeapon[3];
    int mArmor[3];
} sParamRows = {{0x191, 0x192, 0x195, 0x194, 0x193}, {0, 4, 2}, {1, 3, 2}};

void ShopBase::fn_80581008(int id) {
    if (MarketTask::sInstance->fn_804A24C4(id)) {
        m90->MeltTask();
        int num = fn_804444E8(Inventory::instance_, id);
        int average = MarketTask::sInstance->fn_804A2724(id);
        int price = MarketTask::sInstance->fn_804A251C(id);
        if (mKind == 4) {
            price = 1.2f * price;
        }
        int profit = 0;
        if (num > 0 && average > 0) {
            profit = price - average;
        }
        if (profit < 0) {
            profit -= profit * 2;
            char text[0x20];
            sprintf(text, "%d", profit);
            SetPaneFloat_unk(m90->mLayout, "priceUnit", strlen(text));
            SetPaneFloat_unk(m90->mLayout, "minus", 1.0f);
        } else {
            SetPaneFloat_unk(m90->mLayout, "minus", 0.0f);
        }
        ((atn::D2A_Task*)m90)->SetNumber_unk("average", average, 0);
        ((atn::D2A_Task*)m90)->SetNumber_unk("profit", profit, 0);
    }
}

// NONMATCHING: the original loads the address of sParamRows at the entry (kept in r31 for the whole
// function); every register after it is shifted
void ShopBase::fn_805811DC(int id) {
    if (fn_80211480(id) == 0) {
        return;
    }
    if (fn_8020EFEC(id) == 0 && fn_80206C50(id) == 0) {
        return;
    }
    m94->MeltTask();
    ShopPoint_unk pos;
    for (int i = 0; i < 2; i++) {
        char name[0x40];
        sprintf(name, "skill_point%d", i + 1);
        pos = GetPanePos_unk(m94, name);
        fn_801F6E78(m98[i], "basePos", &pos);
    }
    ShopArmor_unk* armor = fn_8020EFEC(id);
    ShopWeapon_unk* weapon = fn_80206C50(id);
    for (int i = 0; i < 2; i++) {
        const ShopSkill_unk* skill = 0;
        if (armor != 0) {
            skill = &armor->mSkills[i];
        } else if (weapon != 0) {
            skill = &weapon->mSkills[i];
        }
        wchar_t text[0x100];
        if (skill != 0 && fn_8021C6A4(text, skill)) {
            m98[i]->MeltTask();
            fn_801F837C(m98[i], "skillName", text);
        }
    }
    ShopParams_unk* params;
    ShopParams_unk* bonus = 0;
    int isWeapon;
    if (armor != 0) {
        params = &armor->mParams;
        if (fn_8020EF4C(armor)) {
            bonus = fn_8020ED84(1, armor->m7C);
        }
        isWeapon = 0;
    } else {
        params = &weapon->mParams;
        isWeapon = 1;
    }
    int values[5];
    memset(values, 0, sizeof(values));
    if (params != 0) {
        values[0] = params->m00;
        values[1] = params->m08;
        values[2] = 0;
        values[3] = params->m0C;
        values[4] = params->m04;
    }
    if (bonus != 0) {
        values[0] += (int)bonus->m00;
        values[1] += (int)bonus->m08;
        values[3] += (int)bonus->m0C;
        values[4] += (int)bonus->m04;
    }
    for (int i = 0; i < 3; i++) {
        int row = isWeapon ? sParamRows.mWeapon[i] : sParamRows.mArmor[i];
        char name[0x40];
        sprintf(name, "param%02d", i);
        SetPaneText_unk(m94->mLayout, name, fn_80116FC0(0, sParamRows.mMessages[row]));
        sprintf(name, "param%02d_before", i);
        ((atn::D2A_Task*)m94)->SetNumber_unk(name, values[row], 0);
    }
}

void ShopBase::fn_80581570(int id, int flag) {
    ShopItem_unk* item = fn_80211480(id);
    if (item != 0) {
        if (fn_80206B9C(item->mCountId)) {
            if (flag) {
                ShopWeapon_unk* weapon = fn_80206C50(item->mCountId);
                if (weapon != 0) {
                    fn_804A3C24(MenuCommonTask::sInstance, weapon->mHelp, 0);
                }
            } else {
                MenuCommonTask* menu = MenuCommonTask::sInstance;
                fn_804A3C24(menu, fn_80116FC0(1, 0x15B), 0);
            }
        } else if (fn_8020EF04(item->mCountId)) {
            ShopArmor_unk* armor = fn_8020EFEC(item->mCountId);
            if (armor != 0) {
                fn_804A3C24(MenuCommonTask::sInstance, armor->mHelp, 0);
            }
        } else {
            fn_804A3C24(MenuCommonTask::sInstance, item->mText, 0);
        }
    }
}

void ShopBase::fn_80581660(int index) {
    m25B14 = 1;
    m25B18 = index;
}

void ShopBase::fn_80581674() {
    m25B2C->FreezeTask();
    if (m25B14 != 0) {
        m25B2C->MeltTask();
        char name[0x40];
        sprintf(name, "list_cursor%d", m25B18 + 1);
        ShopPoint_unk pos = GetPanePos_unk(mA8, name);
        SetPaneValue_unk(m25B2C->mLayout, "basePos", 0, pos.x);
        SetPaneValue_unk(m25B2C->mLayout, "basePos", 1, pos.y);
        ((atn::D2A_Task*)m25B2C)->SetNumber_unk("num", m25B24, 0);
        if (m25B1C > 0) {
            m25B1C--;
            m25B2C->mFrame = m25B20 + 5;
        } else if (m25B2C->mFrame >= 5.0f) {
            m25B2C->mFrame = 5.0f;
        } else {
            m25B2C->mFrame = 1.0f + m25B2C->mFrame;
        }
    }
    m25B14 = 0;
}

void ShopBase::fn_8058181C(int show, int message, int arg) {
    static const int sMessages[18] = {0,     0x144, 0x145, 0x146, 0x147, 0x148, 0x149, 0x14A, 0x14B,
                                      0x14C, 0x14D, 0x14E, 0x14F, 0x150, 0x151, 0x152, 0x153, 0x154};
    ShopD2A_unk* window = m74;
    if (window != 0) {
        if (show) {
            m7C = message;
            m78 = 1;
            window->mSpeed = 3.0f;
            if (arg) {
                wchar_t text[0x100];
                fn_800DD3FC(text, lbl_8087F1E4->Get_unk(sMessages[message]), arg);
                SetPaneText_unk(m74->mLayout, "warning_msg", text);
            } else {
                const wchar_t* text = lbl_8087F1E4->Get_unk(sMessages[message]);
                if (text != 0) {
                    SetPaneText_unk(m74->mLayout, "warning_msg", text);
                }
            }
        } else {
            m7C = -1;
            m78 = 0;
            window->mSpeed = -3.0f;
        }
    }
}

void ShopBase::fn_80581964(int show, int message, int arg) {
    static const int sMessages[6] = {0x155, 0x156, 0x157, 0x158, 0x159, 0x15A};
    ShopD2A_unk* window = m80;
    if (window != 0) {
        if (show) {
            m8C = 0;
            if (arg) {
                m8C = 1;
            }
            m84 = 1;
            m88 = message;
            m80->mSpeed = 3.0f;
            const wchar_t* text = lbl_8087F1E4->Get_unk(sMessages[message]);
            if (text != 0) {
                SetPaneText_unk(m80->mLayout, "check_msg", text);
            }
        } else {
            m8C = 0;
            m88 = -1;
            m84 = 0;
            window->mSpeed = -3.0f;
        }
    }
}

// NONMATCHING: id/item registers of the second loop swapped (the first one is fixed by declaring item first)
// inferred: price of `num` items at the rate `rate` (inlined in vf38)
inline int GetPrice_unk(ShopItem_unk* item, int num, float rate) {
    if (item == 0) {
        return 0;
    }
    if (item->mKind == 9 && MarketTask::sInstance != 0) {
        return rate * (num * MarketTask::sInstance->fn_804A251C(item->mCountId));
    }
    return rate * (item->mPrice * num);
}

// NONMATCHING: the original stores the double returned by fn_800DC288 (stfs f1) and compares the rounded
// copy; here the rounded copy is stored
void ShopBase::vf38(const char* data, int size) {
    atn::Csv csv;
    ShopEntry64_unk entry;
    if (MarketTask::sInstance != 0) {
        MarketTask::sInstance->fn_804A26DC();
    }
    mF8 = 0;
    int chapter = 0;
    if (lbl_8087F430 != 0) {
        chapter = lbl_8087F430->mChapter;
    }
    if (m25B4C == 0) {
        m25B4C = chapter;
    }
    csv.SetData(data, size);
    do {
        const char* token = csv.fn_8005B9CC();
        if (strncmp(token, ";", 1) == 0) {
        } else if (strcmp(token, "shopTitle") == 0) {
            mTitle = atoi(csv.fn_8005B9CC());
        } else if (strcmp(token, "shopName") == 0) {
            mName = atoi(csv.fn_8005B9CC());
        } else if (*token == 0) {
            entry.Reset_unk();
            entry.m00 = fn_800DC12C(csv.fn_8005B9CC());
            ShopItem_unk* item = fn_80211480(entry.m00);
            if (item == 0 || !fn_8011728C(m25B00, item)) {
                continue;
            }
            entry.m48 = fn_800DC288(csv.fn_8005B9CC());
            if (entry.m48 <= 0.0f) {
                entry.m48 = 1.0f;
            }
            int from = fn_800DC12C(csv.fn_8005B9CC());
            int fromFlag = fn_800DC12C(csv.fn_8005B9CC());
            if (from != 0 && m25B4C < from && m25B54 == 0) {
                continue;
            }
            if (fromFlag != 0 && !fn_80370174(lbl_8087F430, fromFlag) && m25B58 == 0) {
                continue;
            }
            int to = fn_800DC12C(csv.fn_8005B9CC());
            int toFlag = fn_800DC12C(csv.fn_8005B9CC());
            if (to != 0 && m25B4C >= to && m25B54 == 0 && (toFlag == 0 || fn_80370174(lbl_8087F430, toFlag)) &&
                m25B58 == 0) {
                continue;
            }
            entry.m54 = fn_800DC12C(csv.fn_8005B9CC()) == 0;
            entry.m58 = fn_800DC12C(csv.fn_8005B9CC());
            if (entry.m58 <= 0) {
                entry.m58 = 1;
            }
            void* font = lbl_8087EEC8;
            fn_8006F2F0(font, entry.m08, csv.fn_8005B9CC(), 0x20);
            int price = fn_800DC12C(csv.fn_8005B9CC());
            if (price > 0) {
                entry.m04 = entry.m48 * (price * entry.m58);
            } else {
                entry.m04 = GetPrice_unk(item, entry.m58, entry.m48);
            }
            new (&mEntries64[mF8]) ShopEntry64_unk(entry);
            mF8++;
        }
    } while (csv.fn_8005BBF8());
}

void ShopBase::fn_80581FD8() {
    ShopEntry5C_unk entry;
    float b = mKind == 4 ? 2.0f : 1.0f;
    float c = mKind == 4 ? 1.2f : 1.0f;
    m32FC = 0;
    for (int i = 0; i < 0x600; i++) {
        Inventory::Item_unk* bag = Inventory::instance_->fn_80444804(i);
        if (bag != 0) {
            ShopItem_unk* item;
            int id = bag->mCode;
            item = fn_80211480(id);
            if (item != 0 && fn_8011728C(m25B00, item) && bag->mCount > 0) {
                entry.m00 = id;
                entry.m48 = 1;
                entry.m4C = -1;
                entry.m04 = fn_80584DD8(fn_80211480(entry.m00), 1, 1.0f, b, c);
                entry.m50 = fn_80211648(item);
                entry.m54 = (int)item;
                entry.m58 = fn_8021150C(entry.m00);
                new (&mEntries5C[m32FC]) ShopEntry5C_unk(entry);
                m32FC++;
            }
        }
    }
    int num = fn_8044D560(Inventory::instance_);
    for (int i = 0; i < num; i++) {
        int id = fn_8044D500(Inventory::instance_, i);
        ShopItem_unk* item = fn_80211480(id);
        if (item != 0 && fn_8011728C(m25B00, item)) {
            entry.m00 = id;
            entry.m48 = 0;
            entry.m4C = i;
            entry.m04 = fn_80584DD8(fn_80211480(entry.m00), 0, 1.0f, 1.0f, 1.0f);
            entry.m50 = 1;
            entry.m54 = (int)item;
            entry.m58 = fn_8021150C(entry.m00);
            new (&mEntries5C[m32FC]) ShopEntry5C_unk(entry);
            m32FC++;
        }
    }
    ShopEntryLess_unk less;
    less.m00 = 0;
    fn_80582598(&mEntries5C[0], &mEntries5C[m32FC], less);
    mScroll.mCount = m32FC;
    if (mScroll.mCursor >= mScroll.mCount) {
        int last = mScroll.mCount - 1;
        mScroll.mCursor = last < 0 ? 0 : last;
    }
    int top = mScroll.mCount - mScroll.mPage;
    if (mScroll.mTop >= top) {
        mScroll.mTop = top < 0 ? 0 : top;
    }
}

void ShopBase::fn_805846C0(ShopD2A_unk* anime, ShopLayout_unk* layout, int cursor, int top, int arg) {
    char name[0x40];
    sprintf(name, "list_cursor%d", cursor - top + 1);
    ShopPoint_unk pos = GetPanePos_unk(anime, name);
    fn_804A55FC(MenuCommonTask::sInstance, layout, 0, &pos, arg);
}

void ShopBase::fn_80584750(ShopD2A_unk* anime, int top, int count, int page) {
    m6C = 1;
    ShopPoint_unk pos = GetPanePos_unk(anime, "scroll_point");
    fn_801F6E78(m68, "basePos", &pos);
    fn_801F6C80(m68, "s_hightMax", 250.0f);
    fn_804A4930(m68, top, count, page);
}

void ShopBase::fn_805847EC(int arg) {
    fn_804A436C(&mScroll.mCursor, mScroll.mCount, mScroll.mPage, arg, 3);
}

void ShopBase::fn_80584808(int arg) {
    fn_804A4494(&mScroll.mCursor, &mScroll.mTop, mScroll.mCount, mScroll.mPage, arg, 3);
}

int ShopBase::fn_8058482C(ShopEntry64_unk* entry) {
    if (Inventory::instance_->mMoney < entry->m04) {
        return 1;
    }
    if (fn_80206B9C(entry->m00) || fn_8020EF04(entry->m00)) {
        if (fn_80444564(Inventory::instance_, entry->m00) >= 10) {
            return 2;
        }
    } else if (fn_804444E8(Inventory::instance_, entry->m00) >= 99) {
        return 2;
    }
    if (entry->m60 == 0) {
        return 5;
    }
    return 0;
}

int ShopBase::fn_805848E0(ShopEntry64_unk* entry) {
    int max;
    int equip = 0;
    Inventory* inv = Inventory::instance_;
    if (fn_80206B9C(entry->m00) || fn_8020EF04(entry->m00)) {
        equip = 1;
    }
    max = 99;
    if (equip) {
        max = 10;
    }
    int num = max - (equip ? fn_80444564(inv, entry->m00) : fn_804444E8(inv, entry->m00));
    if (entry->m58 > 1) {
        if (num % entry->m58 != 0) {
            num /= entry->m58;
            num++;
        } else {
            num /= entry->m58;
        }
    }
    int money = inv->mMoney / entry->m04;
    if ((num < money ? num : money) < 1) {
        return 1;
    }
    return num < money ? num : money;
}

// NONMATCHING: push_back growth residue (stack slots of the two growth temporaries, docs/msl_cpp.md)
void ShopBase::fn_805849E8(ShopEntry64_unk* entry, int num) {
    int price = entry->m04 * num;
    Inventory::instance_->mDB0C = 0;
    Inventory::instance_->AddItem(entry->m00, entry->m58 * num, entry->m54, 0, 0, 1, 1);
    Inventory::instance_->mDB0C = 1;
    if (entry->m5C != 0) {
        ShopBought_unk bought;
        bought.mId = entry->m00;
        bought.m04 = entry->m5C;
        m25B30.push_back(bought);
    }
    fn_8044D6AC(Inventory::instance_, price);
    if (MarketTask::sInstance->fn_804A24C4(entry->m00)) {
        MarketTask::sInstance->fn_804A2800(entry->m00, num);
    }
}

void ShopBase::fn_80584D78(ShopEntry5C_unk* entry, int num) {
    int price = entry->m04 * num;
    if (entry->m48 != 0) {
        fn_8044441C(Inventory::instance_, entry->m00, num);
    } else {
        fn_8044D490(Inventory::instance_, entry->m4C, num);
    }
    Inventory::instance_->AddMoney(price);
}

int ShopBase::fn_80584DD8(void* data, int num, float a, float b, float c) {
    ShopItem_unk* item = (ShopItem_unk*)data;
    if (item == 0) {
        return 0;
    }
    if (item->mKind == 9) {
        if (MarketTask::sInstance != 0) {
            return c * (a * MarketTask::sInstance->fn_804A251C(item->mCountId));
        }
        return c * (item->mPrice * a);
    }
    if (num == 0) {
        ShopWeapon_unk* weapon = fn_80206C50(item->mCountId);
        if (weapon != 0) {
            return weapon->mPrice * a;
        }
        return 10;
    }
    return b * (0.5f * item->mPrice * a);
}

int ShopBase::fn_80584F44(void* data, float rate) {
    ShopItem_unk* item = (ShopItem_unk*)data;
    if (item == 0) {
        return 0;
    }
    ShopWeapon_unk* weapon = fn_80206C50(item->mCountId);
    if (weapon != 0) {
        return weapon->mPrice * rate;
    }
    return 0;
}

void ShopBase::vf40() {
    if (mState == 5) {
        fn_80581FD8();
    }
}
