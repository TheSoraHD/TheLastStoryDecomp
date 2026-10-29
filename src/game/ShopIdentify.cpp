#include "game/ShopIdentify.h"
#include "game/Inventory.h"
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
extern "C" void fn_801FED24(void* layout, unsigned int hash, int index, float value);
extern "C" atn::SoundRef fn_80117228(int id); // inferred: plays a system sound effect
extern "C" const wchar_t* fn_80116FC0(int a, int id); // inferred: message text
extern "C" const wchar_t* fn_801170C8(int a, int id, const wchar_t* arg); // inferred: formatted message text
extern "C" void* lbl_8087EF70;
extern "C" int fn_800A555C(void* pad, int port, int button); // inferred: button triggered

// Only the members used here (the full class is in atn/D2A_Task.h, whose extern "C" declarations
// clash with the local ones of this file).
namespace atn {
class D2A_Task : public Task {
public:
    void SetParam2_unk(const char* name, const float* v);        // 0x801F48C8
    void SetNumber_unk(const char* name, int value, int digits); // 0x801F4CB4
};
struct D2A_TextParam_unk;
} // namespace atn
extern "C" atn::D2A_TextParam_unk* fn_801FEC74(void* layout, const char* name);
extern "C" std::basic_string<wchar_t> fn_800A4450(atn::D2A_TextParam_unk* text, int index, int table); // inferred
extern "C" void* lbl_8087EEC8; // inferred: font
extern "C" float fn_8006EF48(void* font, const wchar_t* text, float size, float f2, int a, int b); // inferred: width

// inferred: 2D point of a d2anime pane (0x14 bytes)
struct ShopPoint_unk {
    ShopPoint_unk() : x(0.0f), y(0.0f), w(0.0f), h(0.0f), z(0.0f) {}
    float x, y, w, h, z;
};
extern "C" ShopPoint_unk fn_801F4E8C(atn::Task* layout, unsigned int hash);
extern "C" void fn_801F6E78(atn::Task* task, const char* name, const ShopPoint_unk* pos);
inline ShopPoint_unk GetPanePos_unk(atn::Task* layout, const char* name) {
    return fn_801F4E8C(layout, fn_800DC6B4(name));
}
// inferred: texture rectangle (as MenuUV_unk in game/MenuCommonTask.cpp)
struct ShopUV_unk {
    ShopUV_unk() {}
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
inline void SetPaneValue_unk(void* layout, const char* name, int index, float value) {
    fn_801FED24(layout, fn_800DC6B4(name), index, value);
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
};
extern "C" ShopItem_unk* fn_80211480(int id);
extern "C" int fn_80211648(ShopItem_unk* item); // inferred: nonzero if the item can be sold
// inferred: weapon data (fn_80206C50)
struct ShopWeapon_unk {
    unsigned char m00[0xB8];
    int mKind; // 0xB8
};
extern "C" ShopWeapon_unk* fn_80206C50(int id);
extern "C" int fn_804444E8(Inventory* party, int id); // inferred: number of items
extern "C" void fn_8044441C(Inventory* party, int id, int num); // inferred: removes items
extern "C" int fn_80444564(Inventory* party, int id);
extern "C" int fn_80444BE8(Inventory* party, ShopItem_unk* item);
extern "C" int fn_80444C48(Inventory* party);
extern "C" ShopUV_unk fn_80444C50(Inventory* party, int icon);
extern "C" int fn_8044D560(Inventory* party);           // inferred: number of items to identify
extern "C" int fn_8044D500(Inventory* party, int index); // inferred: item to identify
extern "C" void fn_8044D56C(Inventory* party, int index);
extern "C" void fn_8044D678(Inventory* party, int money);
extern "C" void fn_8044D6AC(Inventory* party, int money);
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
// inferred: wide literals of this file kept in .sdata (L"", L"1", L"WEAPON")
extern "C" const wchar_t lbl_8087E690[2];
// inferred: debug settings (lbl_8087F0A8)
struct ShopSettings_unk {
    unsigned char m000[0x488];
    int m488;
};
extern "C" ShopSettings_unk* lbl_8087F0A8;
class MenuCommonTask {
public:
    static MenuCommonTask* sInstance;
};
extern "C" void fn_804A5824(MenuCommonTask* menu, atn::Task* anime, const char* name, int end);
extern "C" void fn_804A55FC(MenuCommonTask* menu, atn::Task* id, const char* name, const ShopPoint_unk* rect,
                            int end);
extern "C" void fn_804A3C24(MenuCommonTask* menu, const wchar_t* text, int id);
extern "C" void fn_804A7DD4(MenuCommonTask* menu, int item);
extern "C" void fn_804A39EC(ShopD2A_unk* anime, int arg1, int repeat, float frame, float speed);
extern "C" void fn_804A3A68(ShopLayout_unk* layout, int arg1, int repeat, float frame, float speed);
// inferred: weak copy of an MSL vector insertion helper (0x8058A3AC, called with the position, the value and
// a tag); not reconstructed
extern "C" void fn_8058A3AC(std::vector<ShopIdentifyEntry_unk>* v, ShopIdentifyEntry_unk* pos,
                            const ShopIdentifyEntry_unk* x, const char* tag);

// inferred: inlined vector::insert (name invented)
inline void Insert_unk(std::vector<ShopIdentifyEntry_unk>& v, ShopIdentifyEntry_unk* pos,
                       const ShopIdentifyEntry_unk& x) {
    char tag = 0;
    fn_8058A3AC(&v, pos, &x, &tag);
}

ShopIdentify* ShopIdentify::sInstance;

ShopIdentify* ShopIdentify::fn_80588684(atn::Task* parent, int a, int b) {
    if (sInstance == 0) {
        sInstance = new (1, "", "", 0) ShopIdentify(parent, a, b);
    }
    return sInstance;
}

ShopIdentify::ShopIdentify(atn::Task* parent, int a, int b)
    : ShopBase(parent, 2, a, b), m25C18(0), mTopCursor(0), mCursor(0), mScrollTop(0) {
    (mTop = fn_801F3FF8(this, "/data/d2anime/shop_identify_top_base.d2a", 0))->fn_800D246C(1);
    (mContent = fn_801F3FF8(this, "/data/d2anime/shop_identify_content.d2a", 0))->fn_800D246C(1);
    (mListBase = fn_801F3FF8(this, "/data/d2anime/shop_material_list_base.d2a", 0))->fn_800D246C(1);
    for (int i = 0; i < 10; i++) {
        (mList[i] = CreateLayout_unk(this, "/data/d2anime/shop_material_list.d2a"))->fn_800D246C(1);
    }
    (mResult = fn_801F3FF8(this, "/data/d2anime/shop_identify.d2a", 0))->fn_800D246C(1);
}

ShopIdentify::~ShopIdentify() {
    if (sInstance != 0) {
        sInstance = 0;
    }
}

inline void ShopIdentify::ReadItems_unk() {
    mItems.clear();
    Inventory* party = Inventory::instance_;
    int num = fn_8044D560(party);
    for (int i = 0; i < num; i++) {
        ShopIdentifyEntry_unk entry;
        entry.mIndex = i;
        entry.mItem = fn_8044D500(party, i);
        entry.mDone = 0;
        entry.mPrice = fn_80584F44(this, fn_80211480(entry.mItem), 1.0f);
        Insert_unk(mItems, mItems.begin(), entry);
    }
}

int ShopIdentify::vf0C() {
    if (fn_800D3FA4() && !fn_8057F880(this)) {
        fn_8057F8F8(this);
        vf38((const char*)mFile.fn_80470580(), mFile.fn_8047059C());
        ReadItems_unk();
        fn_804A39EC(mTop, 1, 0, 0.0f, 1.0f);
        mTop->mFlags |= 4;
        mContent->fn_800D246C(0);
        mContent->mRepeat = 0;
        mContent->mFlags |= 4;
        fn_804A39EC(mListBase, 1, 0, 0.0f, 0.0f);
        for (int i = 0; i < 10; i++) {
            fn_804A3A68(mList[i], 1, 0, 0.0f, 1.0f);
            mList[i]->mFlags |= 4;
        }
        mResult->fn_800D246C(0);
        mResult->mRepeat = 0;
        mResult->mFlags |= 4;
        m25C18 = 1;
        return 1;
    }
    return 0;
}

void ShopIdentify::vf10() {
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
        fn_8058944C();
        break;
    case 7:
        fn_805895B4();
        break;
    case 8:
        fn_805897D4();
        break;
    case 5:
        fn_8057FF38(this);
        break;
    case 6:
        fn_805800FC(this);
        break;
    }
    fn_80580264(this);
}

void ShopIdentify::vf14() {
    fn_80580DE0(this);
    mTop->mFlags |= 4;
    fn_80580D80(mContent);
    mListBase->mFlags |= 4;
    for (int i = 0; i < 10; i++) {
        fn_80580DB0(mList[i]);
    }
    mResult->mFlags |= 4;
    switch (mState) {
    case 4: {
        mTop->mFlags &= ~4;
        char name[0x40];
        sprintf(name, "lw_list_text_select%02d", mTopCursor);
        fn_804A5824(MenuCommonTask::sInstance, mTop, name, 0);
        break;
    }
    case 7: {
        fn_80589AF4();
        if (mCursor < (int)mItems.size()) {
            fn_80589DE0(&mItems[mCursor], 1);
        }
        char name[0x40];
        sprintf(name, "list_cursor%d", mCursor - mScrollTop + 1);
        ShopPoint_unk pos = GetPanePos_unk(mListBase, name);
        fn_804A55FC(MenuCommonTask::sInstance, mList[mCursor - mScrollTop], 0, &pos, 0);
        break;
    }
    case 8:
        fn_80589AF4();
        if (mCursor < (int)mItems.size()) {
            ShopIdentifyEntry_unk* entry = &mItems[mCursor];
            fn_80589DE0(entry, 0);
            fn_8058A0F0(entry);
        }
        break;
    case 2:
    case 3:
        fn_80589958();
        break;
    case 5:
        fn_80580580(this);
        break;
    case 6:
        fn_80580690(this);
        break;
    }
}

void ShopIdentify::vf1C(int state) {
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
        mScroll.mCount = 3;
        mScroll.mTop = 0;
        mScroll.mPage = 3;
        break;
    case 7:
        if (mPrevState == 4) {
            ReadItems_unk();
            mCursor = 0;
            mScrollTop = 0;
            mScroll.mTop = 0;
        }
        if (mItems.size() == 0) {
            fn_80117228(11);
            fn_8058181C(this, 1, 11, 0);
            mState = 4;
            vf1C(2);
        } else {
            mScroll.mCursor = mCursor;
            mScroll.mTop = mScrollTop;
            mScroll.mCount = mItems.size();
            mScroll.mPage = 10;
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
    case 6:
        m25B40 = 0;
        m25B24 = 1;
        if (mEntries5C[mDC].m48 != 0) {
            m25B28 = fn_804444E8(Inventory::instance_, mEntries5C[mDC].m00);
        } else {
            m25B28 = 1;
        }
        m25B2C->mFrame = 0.0f;
        break;
    case 8:
        m25B40 = 0;
        mResult->mFrame = 0.0f;
        break;
    case 3:
        m25B40 = 0;
        break;
    }
}

void ShopIdentify::vf20(int result) {
    ShopIdentifyEntry_unk* entry = &mItems[mCursor];
    if (result == 3) {
        fn_80117228(28);
        vf1C(8);
        fn_8044D6AC(Inventory::instance_, entry->mPrice);
    } else if (result == 5) {
        fn_80584D78(this, &mEntries5C[mDC], m25B24);
        fn_80581FD8(this);
        fn_80117228(25);
        vf1C(5);
    } else if (result == 4) {
        int state = fn_805892CC(&mCursor, 1);
        fn_80117228(25);
        vf1C(state);
    } else {
        fn_80117228(1);
        vf1C(7);
    }
}

void ShopIdentify::vf24(int message) {
    if (message == 17) {
        mPrevState = fn_805892CC(&mCursor, 0);
        fn_80117228(25);
    }
}

int ShopIdentify::fn_805892CC(int* cursor, int remove) {
    ShopIdentifyEntry_unk* entry = &mItems[*cursor];
    int money = fn_80584DD8(this, fn_80211480(entry->mItem), 1, 1.0f, 1.0f, 1.0f);
    if (remove) {
        fn_8044441C(Inventory::instance_, entry->mItem, 1);
    }
    fn_8044D678(Inventory::instance_, money);
    ShopIdentifyEntry_unk* pos = &mItems[*cursor];
    memmove(pos, pos + 1, (mItems.end() - pos - 1) * sizeof(ShopIdentifyEntry_unk));
    if (--mItems.mSize != 0) {
        if (mItems.mSize <= 10) {
            *cursor = *cursor < 9 ? *cursor : 9;
            mScrollTop = 0;
            if ((int)mItems.mSize <= *cursor) {
                *cursor = mItems.mSize - 1;
            }
        } else {
            if ((int)mItems.mSize < *cursor + 10) {
                *cursor = mItems.mSize - 10;
            }
            if ((int)mItems.mSize <= *cursor) {
                *cursor = mItems.mSize - 1;
            }
        }
        return 7;
    }
    return 4;
}

void ShopIdentify::fn_8058944C() {
    fn_805847EC(this, 1);
    mTopCursor = mScroll.mCursor;
    void* pad = lbl_8087EF70;
    if (fn_800A555C(pad, 0, 4)) {
        switch (mTopCursor) {
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

void ShopIdentify::fn_805895B4() {
    fn_80584808(this, 1);
    mCursor = mScroll.mCursor;
    mScrollTop = mScroll.mTop;
    void* pad = lbl_8087EF70;
    if (fn_800A555C(pad, 0, 4)) {
        Inventory* party = Inventory::instance_;
        ShopIdentifyEntry_unk* entry = &mItems[mCursor];
        int error;
        if (entry->mDone != 0) {
            ShopItem_unk* item = fn_80211480(entry->mItem);
            if (item != 0 && fn_80211648(item) == 0) {
                error = 3;
            } else if (fn_804444E8(Inventory::instance_, entry->mItem) <= 0) {
                error = 16;
            } else {
                error = 6;
            }
        } else {
            error = party->mMoney < entry->mPrice;
        }
        if (error == 0) {
            fn_80117228(1);
            fn_80581964(this, 1, 3, 0);
            vf1C(3);
        } else if (error == 6) {
            fn_80117228(1);
            fn_80581964(this, 1, 4, 0);
            vf1C(3);
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

// NONMATCHING: 98.97% - register allocation only (volatile registers of the index loop)
void ShopIdentify::fn_805897D4() {
    if (mResult->IsEnd_unk()) {
        void* pad = lbl_8087EF70;
        if (fn_800A555C(pad, 0, 4) || fn_800A555C(pad, 0, 5)) {
            mItems[mCursor].mDone = 1;
            int num = fn_80444564(Inventory::instance_, mItems[mCursor].mItem);
            ShopIdentifyEntry_unk* items = mItems.mData;
            int cursor = mCursor;
            for (unsigned int i = 0; i < mItems.size(); i++) {
                if (mItems[i].mIndex > items[cursor].mIndex) {
                    mItems[i].mIndex--;
                }
            }
            fn_8044D56C(Inventory::instance_, items[cursor].mIndex);
            fn_80117228(1);
            if (num >= 10) {
                fn_8058181C(this, 1, 17, 0);
                vf1C(2);
            } else {
                vf1C(7);
            }
        }
    }
}

void ShopIdentify::fn_80589958() {
    if (mPrevState == 4) {
        mTop->mFlags &= ~4;
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
    } else if (mPrevState == 7 || mPrevState == 8) {
        fn_80589AF4();
        if (mCursor < (int)mItems.size()) {
            fn_80589DE0(&mItems[mCursor], 1);
        }
        char name[0x40];
        sprintf(name, "list_cursor%d", mCursor - mScrollTop + 1);
        ShopPoint_unk pos = GetPanePos_unk(mListBase, name);
        fn_804A55FC(MenuCommonTask::sInstance, mList[mCursor - mScrollTop], 0, &pos, 1);
    }
}

void ShopIdentify::fn_80589AF4() {
    mListBase->mFlags &= ~4;
    fn_80584750(this, mListBase, mScroll.mTop, mItems.size(), 10);
    ShopPoint_unk pos;
    for (int i = 0; i < 10; i++) {
        char name[0x40];
        sprintf(name, "list_point%d", i + 1);
        pos = GetPanePos_unk(mListBase, name);
        fn_801F6E78(mList[i], "basePos", &pos);
        fn_801F7DF0(mList[i], "itemCol", 255.0f, 255.0f, 255.0f);
        int index = i + mScroll.mTop;
        if (index < (int)mItems.size()) {
            ShopIdentifyEntry_unk* entry = &mItems[index];
            int icon;
            ShopItem_unk* item = fn_80211480(entry->mItem);
            if (entry->mDone != 0) {
                fn_801F837C(mList[i], "itemName", item->mName);
                fn_801F6C80(mList[i], "showPrice", 0.0f);
                fn_801F6C80(mList[i], "nonePrice", 1.0f);
                fn_801F837C(mList[i], "price", lbl_8087E690);
                fn_801F837C(mList[i], "num", lbl_8087E690 + 1);
                icon = fn_80444BE8(Inventory::instance_, item);
                if (fn_80211648(item) == 0 || fn_804444E8(Inventory::instance_, entry->mItem) <= 0) {
                    fn_801F7DF0(mList[i], "itemCol", 128.0f, 128.0f, 128.0f);
                }
            } else {
                ShopWeapon_unk* weapon = fn_80206C50(entry->mItem);
                if (weapon != 0) {
                    fn_801F837C(mList[i], "itemName", lbl_8087F1E4->GetPair_unk(weapon->mKind + 0xEB));
                } else {
                    fn_801F837C(mList[i], "itemName", lbl_8087E690 + 3);
                }
                fn_801F6C80(mList[i], "showPrice", 1.0f);
                fn_801F6C80(mList[i], "nonePrice", 0.0f);
                fn_801F8598(mList[i], "price", entry->mPrice, 0);
                fn_801F837C(mList[i], "num", lbl_8087E690 + 1);
                icon = fn_80444C48(Inventory::instance_);
            }
            ShopUV_unk uv = fn_80444C50(Inventory::instance_, icon);
            fn_801F7590(mList[i], "iconUv", uv);
            mList[i]->mFlags &= ~4;
        }
    }
}

// NONMATCHING: 97.60% - the 14.0f size argument of fn_8006EF48 and the pool address are scheduled earlier in
// the original; one register of the help branch differs
void ShopIdentify::fn_80589DE0(ShopIdentifyEntry_unk* entry, int help) {
    mContent->mFlags &= ~4;
    ((atn::D2A_Task*)mContent)->SetNumber_unk("myMoney", Inventory::instance_->mMoney, 0);
    ShopItem_unk* item = fn_80211480(entry->mItem);
    if (entry->mDone == 0 || (item != 0 && fn_80211648(item) == 0)) {
        SetPaneFloat_unk(mContent->mLayout, "showSell", 0.0f);
    } else {
        SetPaneFloat_unk(mContent->mLayout, "showSell", 1.0f);
        ((atn::D2A_Task*)mContent)->SetNumber_unk("price", fn_80584DD8(this, item, 1, 1.0f, 1.0f, 1.0f), 0);
    }
    float width;
    {
        std::basic_string<wchar_t> text = fn_800A4450(fn_801FEC74(mContent->mLayout, "string0012(1)"), 0, 0);
        width = fn_8006EF48(lbl_8087EEC8, text.data(), 14.0f, 0.0f, 1, 1);
    }
    SetPaneValue_unk(mContent->mLayout, "pos0011", 0, 175.0f - width - 16.0f);
    {
        std::basic_string<wchar_t> text = fn_800A4450(fn_801FEC74(mContent->mLayout, "string0010(1)"), 0, 0);
        width = fn_8006EF48(lbl_8087EEC8, text.data(), 14.0f, 0.0f, 1, 1);
    }
    SetPaneValue_unk(mContent->mLayout, "pos0009", 0, 175.0f - width - 16.0f);
    if (help) {
        if (entry->mDone != 0) {
            ShopItem_unk* info = fn_80211480(entry->mItem);
            if (info != 0) {
                fn_804A3C24(MenuCommonTask::sInstance, info->mText, 0);
            }
        } else {
            MenuCommonTask* menu = MenuCommonTask::sInstance;
            fn_804A3C24(menu, fn_80116FC0(1, 0x15B), 0);
        }
        if (lbl_8087F0A8->m488 != 0) {
            fn_804A7DD4(MenuCommonTask::sInstance, entry->mItem);
        }
    }
}

// NONMATCHING: 96.33% - the original reads the message text twice (two address forms), the stack slots of
// the temporaries differ and the registers of the inlined SetPaneText_unk are swapped
void ShopIdentify::fn_8058A0F0(ShopIdentifyEntry_unk* entry) {
    mResult->mFlags &= ~4;
    ShopItem_unk* item = fn_80211480(entry->mItem);
    ShopWeapon_unk* weapon = fn_80206C50(entry->mItem);
    std::basic_string<wchar_t> name;
    if (weapon != 0) {
        name = lbl_8087F1E4->GetPair_unk(weapon->mKind + 0xEB);
    }
    SetPaneText_unk(mResult->mLayout, "before", fn_801170C8(0, 0x2760, name.data()));
    SetPaneText_unk(mResult->mLayout, "after", item->mName);
    ShopUV_unk uv = fn_80444C50(Inventory::instance_, fn_80444BE8(Inventory::instance_, item));
    SetUV_unk((atn::D2A_Task*)mResult, "iconUv01", uv);
    float width;
    {
        std::basic_string<wchar_t> text = fn_800A4450(fn_801FEC74(mResult->mLayout, "string0012(1)(1)"), 0, 0);
        width = fn_8006EF48(lbl_8087EEC8, text.data(), 14.0f, 0.0f, 1, 1);
    }
    SetPaneValue_unk(mResult->mLayout, "pos0011", 0, 155.0f - width - 16.0f);
}

void ShopIdentify::vf40() {
    if (mState == 5) {
        fn_80581FD8(this);
    }
}

int ShopIdentify::vf3C() {
    return 3;
}
