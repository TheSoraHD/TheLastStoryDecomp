#include "game/ShopBuySell.h"
#include "atn/Memory.h"
#include "atn/SoundRef.h"
#include "math/Vec.h"
#include "std/string.h"

extern "C" int sprintf(char* s, const char* format, ...);
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
extern "C" ShopD2A_unk* fn_801F3FF8(atn::Task* parent, const char* path, int loop); // inferred: d2anime task
extern "C" ShopLayout_unk* fn_801F64D0(atn::Task* parent, const char* path);      // inferred: layout anime
extern "C" void fn_801F6C80(atn::Task* layout, const char* name, float value);    // inferred: sets a value
extern "C" void fn_801F837C(atn::Task* layout, const char* name, const wchar_t* text); // inferred: sets a text
extern "C" void fn_801F8598(atn::Task* layout, const char* name, int value, int arg); // inferred: sets a number
extern "C" void fn_801F7DF0(atn::Task* layout, const char* name, float r, float g, float b); // inferred: sets a colour
extern "C" void fn_801FEE08(void* layout, unsigned int hash, const wchar_t* text); // inferred: sets a text
extern "C" void fn_801FECE0(void* layout, unsigned int hash, float value);
extern "C" atn::SoundRef fn_80117228(int id); // inferred: plays a system sound effect
extern "C" const wchar_t* fn_80116FC0(int a, int id); // inferred: message text
extern "C" void* lbl_8087EF70;
extern "C" int fn_800A555C(void* pad, int port, int button); // inferred: button triggered
extern "C" int fn_8067CE80(int x); // inferred: abs

// Only the members used here (the full class is in atn/D2A_Task.h, whose extern "C" declarations
// clash with the local ones of this file).
namespace atn {
class D2A_Task : public Task {
public:
    void SetParam2_unk(const char* name, const float* v);        // 0x801F48C8
    void SetNumber_unk(const char* name, int value, int digits); // 0x801F4CB4
};
} // namespace atn

// inferred: 2D point of a d2anime pane (0x14 bytes)
struct ShopPoint_unk {
    ShopPoint_unk() : x(0.0f), y(0.0f), w(0.0f), h(0.0f), z(0.0f) {}
    float x, y, w, h, z;
};
extern "C" ShopPoint_unk fn_801F4E8C(atn::Task* layout, unsigned int hash);
extern "C" void fn_801F6E78(atn::Task* task, const char* name, const ShopPoint_unk* pos);
extern "C" void fn_801F4728(atn::Task* task, const char* name, const ShopPoint_unk* pos); // inferred: SetParam0
inline ShopPoint_unk GetPanePos_unk(atn::Task* layout, const char* name) {
    return fn_801F4E8C(layout, fn_800DC6B4(name));
}
// inferred: texture rectangle (as MenuUV_unk in game/MenuCommonTask.cpp)
struct ShopUV_unk {
    ShopUV_unk() {}
    ShopUV_unk(float u0_, float v0_, float u1_, float v1_) : u0(u0_), v0(v0_), u1(u1_), v1(v1_) {}
    ShopUV_unk(const ShopUV_unk& o) {
        __Vec4Copy(&u0, &o.u0);
    }
    float u0, v0, u1, v1;
};
// inferred: sets a texture rectangle (the rectangle is passed by value)
extern "C" void fn_801F7590(atn::Task* layout, const char* name, ShopUV_unk uv);
inline void SetUV_unk(atn::D2A_Task* anime, const char* name, ShopUV_unk uv) {
    anime->SetParam2_unk(name, &uv.u0);
}
inline void SetPaneText_unk(void* layout, const char* name, const wchar_t* text) {
    fn_801FEE08(layout, fn_800DC6B4(name), text);
}
inline void SetPaneFloat_unk(void* layout, const char* name, float value) {
    fn_801FECE0(layout, fn_800DC6B4(name), value);
}
inline ShopLayout_unk* CreateLayout_unk(atn::Task* parent, const char* path) {
    return fn_801F64D0(parent, path);
}

// inferred: item data (fn_80211480)
struct ShopItem_unk {
    unsigned char m00[0x04];
    int mCountId;          // 0x04: inferred: id counted by fn_804444E8
    const wchar_t* mName;  // 0x08
    const wchar_t* mText;  // 0x0C
    unsigned char m10[0xBC - 0x10];
    short mKind;           // 0xBC
};
extern "C" ShopItem_unk* fn_80211480(int id);
// inferred: weapon data (fn_80206C50); names invented
struct ShopWeapon_unk {
    float mPower;   // 0x00
    float mMagic;   // 0x04
    unsigned char m08[0x78 - 0x08];
    int mHand;      // 0x78: inferred: 1 for the second hand
    unsigned char m7C[0xB8 - 0x7C];
    int mKind;      // 0xB8
};
extern "C" int fn_80206B9C(int id); // inferred: nonzero for a weapon
extern "C" ShopWeapon_unk* fn_80206C50(int id);
extern "C" ShopWeapon_unk* fn_80206B14(int id);
extern "C" int fn_80206D18(ShopWeapon_unk* weapon, int member, int hand); // inferred: can be equipped
// inferred: armour data (fn_8020EFEC); names invented
struct ShopArmor_unk {
    unsigned char m00[0x08];
    float mDefense; // 0x08
    float mResist;  // 0x0C
    unsigned char m10[0x78 - 0x10];
    int mPart;      // 0x78
};
extern "C" int fn_8020EF04(int id); // inferred: nonzero for an armour
extern "C" ShopArmor_unk* fn_8020EFEC(int id);
extern "C" ShopArmor_unk* fn_8020ED84(int part, int id);
extern "C" int fn_8020EF4C(ShopArmor_unk* armor);
// inferred: character table entry (fn_8020924C)
struct ShopChara_unk {
    unsigned char m00[0x08];
    const wchar_t* mName; // 0x08
    unsigned char m0C[0x2C - 0x0C];
    int mIcon;            // 0x2C: inferred: face icon (1-based, 4 per row)
};
extern "C" int fn_80219544(int member); // inferred: character id
extern "C" ShopChara_unk* fn_8020924C(int id);
// inferred: equipment of a member (0x40 bytes)
struct ShopSlot_unk {
    int mId;
    int m04;
};
struct ShopEquip_unk {
    ShopSlot_unk mArmor[4];  // 0x00
    ShopSlot_unk mWeapon[2]; // 0x20
    unsigned char m30[0x40 - 0x30];
};
// inferred: party data (lbl_8087F4F0)
struct ShopParty_unk {
    unsigned char m0000[0x6000];
    int mMoney; // 0x6000
    unsigned char m6004[0x8290 - 0x6004];
    ShopEquip_unk mEquip[7]; // 0x8290
};
extern "C" ShopParty_unk* lbl_8087F4F0;
extern "C" int fn_80444564(ShopParty_unk* party, int id);
extern "C" int fn_804444E8(ShopParty_unk* party, int id); // inferred: number of items
extern "C" int fn_80444BE8(ShopParty_unk* party, ShopItem_unk* item);
extern "C" int fn_80444C48(ShopParty_unk* party);
extern "C" ShopUV_unk fn_80444C50(ShopParty_unk* party, int icon);
// inferred: field (lbl_8087F430)
struct ShopField_unk {
    unsigned char m0000[0x10D0];
    int m10D0; // inferred: story progress
};
extern "C" ShopField_unk* lbl_8087F430;
// inferred: members of the party (fn_80217D9C); flags at 0x0C
struct ShopMembers_unk {
    unsigned char m00[0x0C];
    unsigned char mIn[8];
};
extern "C" ShopMembers_unk* fn_80217D9C(int progress);
// inferred: message table (lbl_8087F1E4)
extern "C" const wchar_t lbl_808813D0[2]; // inferred: empty text
struct ShopMsgPair_unk {
    const wchar_t* m00;
    const wchar_t* mText;
};
struct ShopMsg_unk {
    const wchar_t* GetPair_unk(int id) {
        return mPairs[id].mText != 0 ? mPairs[id].mText : lbl_808813D0;
    }
    ShopMsgPair_unk mPairs[1];
};
extern "C" ShopMsg_unk* lbl_8087F1E4;
class MenuCommonTask {
public:
    static MenuCommonTask* sInstance;
};
extern "C" void fn_804A5824(MenuCommonTask* menu, atn::Task* anime, const char* name, int end);
extern "C" void fn_804A39EC(ShopD2A_unk* anime, int arg1, int repeat, float frame, float speed);

// inferred: message ids of the parameters (weapon: 0, 4, 2; armour: 1, 3, 2)
static const int sParamMsg[5] = {0x13, 0x16, 0x15, 0x14, 0x12};
static const int sWeaponParams[3] = {0, 4, 2};
static const int sArmorParams[3] = {1, 3, 2};

ShopBuySell* ShopBuySell::sInstance;

ShopBuySell* ShopBuySell::fn_80584FE4(atn::Task* parent, int kind, int a, int b) {
    if (sInstance == 0) {
        sInstance = new (1, "", "", 0) ShopBuySell(parent, kind, a, b);
    }
    return sInstance;
}

ShopBuySell::ShopBuySell(atn::Task* parent, int kind, int a, int b)
    : ShopBase(parent, kind, a, b), m25C88(0), mTopCursor(0), mCursor(0), mScrollTop(0) {
    if (kind == 4) {
        (mTop = fn_801F3FF8(this, "/data/d2anime/shop_sell_top_base.d2a", 0))->fn_800D246C(1);
    } else {
        (mTop = fn_801F3FF8(this, "/data/d2anime/shop_material_top_base.d2a", 0))->fn_800D246C(1);
    }
    (mStatusBase = fn_801F3FF8(this, "/data/d2anime/shop_material_status_base.d2a", 0))->fn_800D246C(1);
    for (int i = 0; i < 7; i++) {
        mStatus[i] = fn_801F3FF8(this, "/data/d2anime/shop_material_status_list.d2a", 0);
        mStatus[i]->fn_800D246C(1);
    }
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 3; j++) {
            (mUp[i][j] = CreateLayout_unk(this, "data/d2anime/shop_status_up_loop.d2a"))->fn_800D246C(1);
            (mDown[i][j] = CreateLayout_unk(this, "data/d2anime/shop_status_down_loop.d2a"))->fn_800D246C(1);
            (mEven[i][j] = CreateLayout_unk(this, "data/d2anime/shop_status_even_loop.d2a"))->fn_800D246C(1);
        }
    }
    if (kind == 4) {
        switch (a) {
        case 1:
            m25B00 = 1;
            break;
        case 2:
            m25B00 = 2;
            break;
        case 3:
            m25B00 = 3;
            break;
        case 4:
            m25B00 = 0;
            break;
        case 5:
            m25B00 = 4;
            break;
        case 0x62:
            m25B00 = 0x62;
            break;
        case 0x63:
            m25B00 = 0x63;
            break;
        default:
            m25B00 = 6;
            break;
        }
    }
}

ShopBuySell::~ShopBuySell() {
    if (sInstance != 0) {
        sInstance = 0;
    }
}

int ShopBuySell::vf0C() {
    if (fn_800D3FA4() && !fn_8057F880(this)) {
        fn_8057F8F8(this);
        vf38((const char*)mFile.fn_80470580(), mFile.fn_8047059C());
        fn_804A39EC(mTop, 1, 0, 0.0f, 1.0f);
        mTop->mFlags |= 4;
        mStatusBase->fn_800D246C(0);
        mStatusBase->mRepeat = 0;
        mStatusBase->mFlags |= 4;
        for (int i = 0; i < 7; i++) {
            mStatus[i]->fn_800D246C(0);
            mStatus[i]->mRepeat = 0;
            mStatus[i]->mFlags |= 4;
        }
        for (int i = 0; i < 7; i++) {
            for (int j = 0; j < 3; j++) {
                mUp[i][j]->fn_800D246C(0);
                mUp[i][j]->mRepeat = 1;
                mUp[i][j]->mSpeed = 0.0f;
                mUp[i][j]->mFrame = 10.0f;
                mUp[i][j]->mFlags |= 4;
                mDown[i][j]->fn_800D246C(0);
                mDown[i][j]->mRepeat = 1;
                mDown[i][j]->mSpeed = 0.0f;
                mDown[i][j]->mFrame = 10.0f;
                mDown[i][j]->mFlags |= 4;
                mEven[i][j]->fn_800D246C(0);
                mEven[i][j]->mRepeat = 1;
                mEven[i][j]->mSpeed = 0.0f;
                mEven[i][j]->mFrame = 10.0f;
                mEven[i][j]->mFlags |= 4;
            }
        }
        m25C88 = 1;
        return 1;
    }
    return 0;
}

void ShopBuySell::vf10() {
    switch (mState) {
    case 0:
        vf28();
        break;
    case 1:
        vf2C();
        break;
    case 2:
        vf30();
        break;
    case 3:
        vf34();
        break;
    case 4:
        fn_80585D08();
        break;
    case 7:
        fn_80585E80();
        break;
    case 5:
        fn_8057FF38(this);
        break;
    case 8:
        fn_80585FD4();
        break;
    case 6:
        fn_805800FC(this);
        break;
    }
    fn_80580264(this);
}

void ShopBuySell::vf14() {
    fn_80580DE0(this);
    mTop->mFlags |= 4;
    fn_80580D80(mStatusBase);
    for (int i = 0; i < 7; i++) {
        fn_80580D80(mStatus[i]);
    }
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 3; j++) {
            mUp[i][j]->mFlags |= 4;
            mDown[i][j]->mFlags |= 4;
            mEven[i][j]->mFlags |= 4;
        }
    }
    switch (mState) {
    case 4: {
        mTop->mFlags &= ~4;
        char name[0x40];
        sprintf(name, "lw_list_text_select%02d", mTopCursor);
        fn_804A5824(MenuCommonTask::sInstance, mTop, name, 0);
        break;
    }
    case 7:
        fn_8058621C();
        if (mF8 != 0 && mCursor < mF8) {
            ShopEntry64_unk* entry = &mEntries64[mCursor];
            fn_80586620(entry->m00);
            fn_80581008(this, entry->m00);
            fn_80581570(this, entry->m00, entry->m54);
        }
        if (mF8 == 0) {
            mD8->mFlags &= ~4;
        } else {
            fn_805846C0(this, mA8, mAC[mCursor - mScrollTop], mCursor, mScrollTop, 0);
        }
        break;
    case 5:
        fn_80580580(this);
        break;
    case 2:
    case 3:
        fn_805860F0();
        break;
    case 8:
        fn_8058621C();
        if (mF8 != 0 && mCursor < mF8) {
            ShopEntry64_unk* entry = &mEntries64[mCursor];
            fn_80586620(entry->m00);
            fn_80581570(this, entry->m00, entry->m54);
        }
        fn_805846C0(this, mA8, mAC[mCursor - mScrollTop], mCursor, mScrollTop, 1);
        fn_80581660(this, mCursor - mScrollTop);
        break;
    case 6:
        fn_80580690(this);
        break;
    }
}

void ShopBuySell::vf1C(int state) {
    m25B40 = 1;
    mPrevState = mState;
    mState = state;
    switch (state) {
    case 1:
        m25B40 = 0;
        fn_8057FB20(this);
        break;
    case 4:
        mScroll.mCursor = mTopCursor;
        mScroll.mTop = 0;
        if (mKind == 4) {
            mScroll.mCount = 2;
            mScroll.mPage = 2;
        } else {
            mScroll.mCount = 3;
            mScroll.mPage = 3;
        }
        break;
    case 7:
        mScroll.mCursor = mCursor;
        mScroll.mTop = mScrollTop;
        mScroll.mPage = 10;
        vf38((const char*)mFile.fn_80470580(), mFile.fn_8047059C());
        mScroll.mCount = mF8;
        if (mScroll.mCursor >= mF8) {
            int last = mF8 - 1;
            mScroll.mCursor = last < 0 ? 0 : last;
        }
        if (mPrevState == 4) {
            mScroll.mCursor = 0;
            mScroll.mTop = 0;
            mCursor = 0;
            mScrollTop = 0;
        }
        if (mF8 == 0) {
            fn_80117228(11);
            fn_8058181C(this, 1, 9, 0);
            mState = 4;
            vf1C(2);
        }
        break;
    case 5:
        mScroll.mCursor = mDC;
        mScroll.mTop = mE0;
        mScroll.mPage = 10;
        fn_80581FD8(this);
        if (mPrevState == 4) {
            mScroll.mCursor = 0;
            mScroll.mTop = 0;
            mDC = 0;
            mE0 = 0;
        }
        if (m32FC == 0) {
            fn_80117228(11);
            fn_8058181C(this, 1, 10, 0);
            mState = 4;
            vf1C(2);
        }
        break;
    case 8:
        m25B40 = 0;
        m25B24 = 1;
        m25B28 = fn_805848E0(this, &mEntries64[mCursor]);
        m25B2C->mFrame = 0.0f;
        break;
    case 6:
        m25B40 = 0;
        m25B24 = 1;
        if (mEntries5C[mDC].m48 != 0) {
            m25B28 = fn_804444E8(lbl_8087F4F0, mEntries5C[mDC].m00);
        } else {
            m25B28 = 1;
        }
        m25B2C->mFrame = 0.0f;
        break;
    }
}

void ShopBuySell::vf20(int result) {
    if (result == 5) {
        fn_80584D78(this, &mEntries5C[mDC], m25B24);
        fn_80581FD8(this);
        fn_80117228(25);
        vf1C(5);
    } else {
        vf1C(4);
    }
}

void ShopBuySell::fn_80585D08() {
    fn_805847EC(this, 1);
    mTopCursor = mScroll.mCursor;
    void* pad = lbl_8087EF70;
    if (fn_800A555C(pad, 0, 4)) {
        int select = mTopCursor;
        if (mKind == 4) {
            select++;
        }
        switch (select) {
        case 0:
            fn_80117228(1);
            vf1C(7);
            break;
        case 1:
            fn_80117228(1);
            vf1C(5);
            break;
        case 2:
            fn_80117228(2);
            vf1C(1);
            break;
        }
    } else if (fn_800A555C(pad, 0, 5)) {
        fn_80117228(2);
        vf1C(1);
    }
}

void ShopBuySell::fn_80585E80() {
    fn_80584808(this, 1);
    mCursor = mScroll.mCursor;
    mScrollTop = mScroll.mTop;
    void* pad = lbl_8087EF70;
    if (fn_800A555C(pad, 0, 4)) {
        int error = fn_8058482C(this, &mEntries64[mCursor]);
        if (error == 0) {
            fn_80117228(1);
            vf1C(8);
        } else {
            fn_80117228(11);
            fn_8058181C(this, 1, error, 0);
            vf1C(2);
        }
    } else if (fn_800A555C(pad, 0, 5)) {
        fn_80117228(2);
        vf1C(4);
    }
}

void ShopBuySell::fn_80585FD4() {
    fn_80580B50(this);
    void* pad = lbl_8087EF70;
    if (fn_800A555C(pad, 0, 4)) {
        fn_805849E8(this, &mEntries64[mCursor], m25B24);
        vf38((const char*)mFile.fn_80470580(), mFile.fn_8047059C());
        fn_80117228(25);
        vf1C(7);
    } else if (fn_800A555C(pad, 0, 5)) {
        fn_80117228(2);
        vf1C(7);
    }
}

void ShopBuySell::fn_805860F0() {
    if (mPrevState == 4) {
        mTop->mFlags &= ~4;
    } else if (mPrevState == 7) {
        fn_8058621C();
        if (mF8 != 0 && mCursor < mF8) {
            ShopEntry64_unk* entry = &mEntries64[mCursor];
            fn_80586620(entry->m00);
            fn_80581008(this, entry->m00);
        }
    } else if (mPrevState == 5) {
        fn_805807A4(this);
    } else if (mPrevState == 6) {
        fn_805807A4(this);
        if (m32FC != 0 && mDC < m32FC) {
            ShopEntry5C_unk* entry = &mEntries5C[mDC];
            fn_80581008(this, entry->m00);
            fn_805811DC(this, entry->m00);
            fn_80581570(this, entry->m00, entry->m48);
        }
        fn_805846C0(this, mA8, mAC[mDC - mE0], mDC, mE0, 1);
    }
}

// NONMATCHING: 98.82% - register allocation (the entry and icon locals one register higher in the
// original) and the load order of the fn_80584750 arguments
void ShopBuySell::fn_8058621C() {
    ShopParty_unk* party = lbl_8087F4F0;
    mA8->mFlags &= ~4;
    mD4->mFlags &= ~4;
    ((atn::D2A_Task*)mD4)->SetNumber_unk("myMoney", lbl_8087F4F0->mMoney, 0);
    if (mCursor < mF8) {
        ((atn::D2A_Task*)mD4)->SetNumber_unk("itemNum", fn_80444564(party, mEntries64[mCursor].m00), 0);
    }
    fn_80584750(this, mA8, mScroll.mTop, mF8, 10);
    ShopPoint_unk pos;
    for (int i = 0; i < 10; i++) {
        char name[0x40];
        sprintf(name, "list_point%d", i + 1);
        pos = GetPanePos_unk(mA8, name);
        fn_801F6E78(mAC[i], "basePos", &pos);
        int index = i + mScroll.mTop;
        if (index < mF8) {
            ShopEntry64_unk* entry = &mEntries64[index];
            ShopItem_unk* item = fn_80211480(entry->m00);
            int icon;
            if (entry->m54 != 0) {
                std::basic_string<wchar_t> text(item->mName);
                if (entry->m08[0] != 0) {
                    text = entry->m08;
                }
                fn_801F837C(mAC[i], "itemName", text.data());
                fn_801F8598(mAC[i], "num", fn_80444564(party, entry->m00), 0);
                icon = fn_80444BE8(lbl_8087F4F0, item);
            } else {
                ShopWeapon_unk* weapon = fn_80206C50(entry->m00);
                if (weapon != 0) {
                    fn_801F837C(mAC[i], "itemName", lbl_8087F1E4->GetPair_unk(weapon->mKind + 0xEB));
                } else {
                    fn_801F837C(mAC[i], "itemName", L"WEAPON");
                }
                fn_801F837C(mAC[i], "num", L"--");
                icon = fn_80444C48(lbl_8087F4F0);
            }
            if (entry->m60 != 0) {
                fn_801F7DF0(mAC[i], "itemCol", 255.0f, 255.0f, 255.0f);
            } else {
                fn_801F7DF0(mAC[i], "itemCol", 100.0f, 100.0f, 100.0f);
            }
            fn_801F6C80(mAC[i], "nonePrice", 0.0f);
            fn_801F6C80(mAC[i], "showPrice", 1.0f);
            fn_801F8598(mAC[i], "price", entry->m04, 0);
            ShopUV_unk uv = fn_80444C50(lbl_8087F4F0, icon);
            fn_801F7590(mAC[i], "iconUv", uv);
            mAC[i]->mFlags &= ~4;
        }
    }
}

// NONMATCHING: 99.68% - scheduling only (li of the slot counter after the flag load in the original)
void ShopBuySell::fn_80586620(int id) {
    ShopItem_unk* item = fn_80211480(id);
    if (item != 0 && (item->mKind == 2 || item->mKind == 3 || item->mKind == 6)) {
        mStatusBase->mFlags &= ~4;
        int slot = 1;
        fn_805866F4(id, 0, 0);
        ShopMembers_unk* members = fn_80217D9C(lbl_8087F430->m10D0);
        if (members != 0) {
            for (unsigned int i = 1; i < 7; i++) {
                if (members->mIn[i] != 0) {
                    fn_805866F4(id, i, slot++);
                }
            }
        }
    }
}

// NONMATCHING: 93.58% - the layout of SetPaneText_unk is kept in a saved register in the original, the
// icon rectangle is computed in another order (0.5f first) and several temporaries take other registers
void ShopBuySell::fn_805866F4(int id, int member, int slot) {
    ShopPoint_unk pos;
    ShopChara_unk* chara = fn_8020924C(fn_80219544(member));
    ShopEquip_unk* equip = &lbl_8087F4F0->mEquip[member];
    ShopD2A_unk* base = mStatusBase;
    ShopD2A_unk* anime = mStatus[slot];
    anime->mFlags &= ~4;
    char name[0x40];
    sprintf(name, "list_point%d", slot + 1);
    pos = GetPanePos_unk(base, name);
    fn_801F4728(anime, "basePos", &pos);
    SetPaneText_unk(anime->mLayout, "name", chara->mName);
    if (chara->mIcon > 0) {
        int index = chara->mIcon - 1;
        int x = index % 4;
        int y = index / 4;
        ShopUV_unk uv(0.25f * x, 0.5f * y, 0.25f * (x + 1), 0.5f * (y + 1));
        SetUV_unk((atn::D2A_Task*)anime, "uv0001", uv);
        SetUV_unk((atn::D2A_Task*)anime, "mask_uv0001", uv);
    }
    int diff[3];
    void* now = 0;
    void* next = 0;
    int show = 0;
    memset(diff, 0, sizeof(diff));
    if (fn_80206B9C(id)) {
        ShopWeapon_unk* weapon = fn_80206C50(id);
        if (weapon != 0 && fn_80206D18(weapon, member, weapon->mHand == 1)) {
            SetPaneFloat_unk(anime->mLayout, "cantEquip", 0.0f);
            now = fn_80206B14(equip->mWeapon[weapon->mHand == 1].mId);
            next = weapon;
            show = 1;
        } else {
            SetPaneFloat_unk(anime->mLayout, "cantEquip", 1.0f);
        }
        if (now != 0 && next != 0) {
            diff[0] = (int)((ShopWeapon_unk*)next)->mPower - (int)((ShopWeapon_unk*)now)->mPower;
            diff[2] = 0;
            diff[1] = (int)((ShopWeapon_unk*)next)->mMagic - (int)((ShopWeapon_unk*)now)->mMagic;
        }
        for (int i = 0; i < 3; i++) {
            sprintf(name, "param%02d", i);
            SetPaneText_unk(base->mLayout, name, fn_80116FC0(0, sParamMsg[sWeaponParams[i]]));
        }
    } else if (fn_8020EF04(id)) {
        SetPaneFloat_unk(anime->mLayout, "cantEquip", 0.0f);
        ShopArmor_unk* armor = fn_8020EFEC(id);
        float dx = 0.0f;
        float dy = dx;
        if (armor != 0) {
            int part = armor->mPart;
            next = armor;
            show = 1;
            int cur = equip->mArmor[part].mId;
            now = fn_8020ED84(part, cur);
            if (fn_8020EF4C((ShopArmor_unk*)now) && cur >= 1000) {
                ShopArmor_unk* a = fn_8020ED84(0, cur);
                ShopArmor_unk* b = fn_8020ED84(0, cur % 1000);
                if (a != 0 && b != 0) {
                    dx = a->mDefense - b->mDefense;
                    dy = a->mResist - b->mResist;
                }
                if (part == 0) {
                    dx *= -0.5f;
                    dy *= -0.5f;
                } else {
                    dx *= 0.5f;
                    dy *= 0.5f;
                }
            }
        }
        if (now != 0 && next != 0) {
            diff[0] = (int)((ShopArmor_unk*)next)->mDefense - (int)(((ShopArmor_unk*)now)->mDefense + dx);
            diff[2] = 0;
            diff[1] = (int)((ShopArmor_unk*)next)->mResist - (int)(((ShopArmor_unk*)now)->mResist + dy);
        }
        for (int i = 0; i < 3; i++) {
            sprintf(name, "param%02d", i);
            SetPaneText_unk(base->mLayout, name, fn_80116FC0(0, sParamMsg[sArmorParams[i]]));
        }
    } else {
        SetPaneFloat_unk(anime->mLayout, "cantEquip", 0.0f);
    }
    if (show) {
        for (int i = 0; i < 3; i++) {
            ShopLayout_unk* layout;
            if (diff[i] > 0) {
                layout = mUp[slot][i];
            } else if (diff[i] < 0) {
                layout = mDown[slot][i];
            } else {
                layout = mEven[slot][i];
            }
            if (layout != 0) {
                int value = fn_8067CE80(diff[i]);
                sprintf(name, "param%02d_updw", i);
                ShopPoint_unk point = GetPanePos_unk(anime, name);
                layout->mFlags &= ~4;
                fn_801F6E78(layout, "basePos", &point);
                fn_801F8598(layout, "paramDif", value, 0);
            }
        }
    }
}

int ShopBuySell::vf3C() {
    return 0;
}
