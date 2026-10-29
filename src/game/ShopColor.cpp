#include "game/ShopColor.h"
#include "atn/Memory.h"
#include "atn/SoundRef.h"
#include "atn/Csv.h"
#include "math/Vec.h"

extern "C" int sprintf(char* s, const char* format, ...);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" int atoi(const char* s);
extern "C" int fn_800DD3FC(wchar_t* s, const wchar_t* format, ...); // inferred: swprintf
extern "C" int fn_800DC12C(const char* s);                          // inferred: string to int
extern "C" unsigned int fn_800DC6B4(const char* s);                 // inferred: string hash
extern "C" ShopD2A_unk* fn_801F3FF8(atn::Task* parent, const char* path, int loop); // inferred: d2anime task
extern "C" ShopLayout_unk* fn_801F64D0(atn::Task* parent, const char* path);      // inferred: layout anime
extern "C" void fn_801F6C80(atn::Task* layout, const char* name, float value);    // inferred: sets a value
extern "C" void fn_801F837C(atn::Task* layout, const char* name, const wchar_t* text); // inferred: sets a text
extern "C" void fn_801F8598(atn::Task* layout, const char* name, int value, int arg); // inferred: sets a number
extern "C" void fn_801F7DF0(atn::Task* layout, const char* name, float r, float g, float b); // inferred: sets a colour
extern "C" void fn_801FEE08(void* layout, unsigned int hash, const wchar_t* text); // inferred: sets a text
extern "C" void fn_801FECE0(void* layout, unsigned int hash, float value);
extern "C" atn::SoundRef fn_80117228(int id); // inferred: plays a system sound effect
extern "C" void* lbl_8087EF70;
extern "C" int fn_800A555C(void* pad, int port, int button); // inferred: button triggered

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
inline void SetPaneText_unk(void* layout, const char* name, const wchar_t* text) {
    fn_801FEE08(layout, fn_800DC6B4(name), text);
}
inline void SetPaneFloat_unk(void* layout, const char* name, float value) {
    fn_801FECE0(layout, fn_800DC6B4(name), value);
}

// inferred: item data (fn_80211480)
struct ShopItem_unk {
    unsigned char m00[0x04];
    int mCountId;          // 0x04: inferred: id counted by fn_804444E8
    const wchar_t* mName;  // 0x08
    const wchar_t* mText;  // 0x0C
};
extern "C" ShopItem_unk* fn_80211480(int id);
// inferred: party data (lbl_8087F4F0)
extern "C" void* lbl_8087F4F0;
extern "C" int fn_804444E8(void* party, int id); // inferred: number of items
extern "C" void fn_8044441C(void* party, int id, int num); // inferred: removes items
extern "C" void fn_804439FC(void* party, int a, int b, int c, int d, int e, int f, int g); // inferred: adds an item
extern "C" int fn_80444BE8(void* party, ShopItem_unk* item);
extern "C" ShopUV_unk fn_80444C50(void* party, int icon);
// inferred: message table (lbl_8087F1E4)
extern "C" const wchar_t lbl_808813D0[2]; // inferred: empty text
struct ShopMsg_unk {
    const wchar_t* Get_unk(int id) {
        return mText[id] != 0 ? mText[id] : lbl_808813D0;
    }
    const wchar_t* mText[1];
};
extern "C" ShopMsg_unk* lbl_8087F1E4;
class MenuCommonTask {
public:
    static MenuCommonTask* sInstance;
};
extern "C" void fn_804A5824(MenuCommonTask* menu, atn::Task* anime, const char* name, int end);
extern "C" void fn_804A55FC(MenuCommonTask* menu, atn::Task* id, const char* name, const ShopPoint_unk* rect,
                            int end);
extern "C" void fn_804A3C24(MenuCommonTask* menu, const wchar_t* text, int id);
extern "C" void fn_804A39EC(ShopD2A_unk* anime, int arg1, int repeat, float frame, float speed);
extern "C" void fn_804A3A68(ShopLayout_unk* layout, int arg1, int repeat, float frame, float speed);

inline ShopLayout_unk* CreateLayout_unk(atn::Task* parent, const char* path) {
    return fn_801F64D0(parent, path);
}


ShopColor* ShopColor::sInstance;

ShopColor* ShopColor::fn_80586DB0(atn::Task* parent, int a, int b) {
    if (sInstance == 0) {
        sInstance = new (1, "", "", 0) ShopColor(parent, a, b);
    }
    return sInstance;
}

ShopColor::ShopColor(atn::Task* parent, int a, int b)
    : ShopBase(parent, 2, a, b), m25BAC(0), mTopCursor(0), mCursor(0), mScrollTop(0) {
    (mTop = fn_801F3FF8(this, "/data/d2anime/shop_color_top_base.d2a", 0))->fn_800D246C(1);
    (mContent = fn_801F3FF8(this, "/data/d2anime/shop_color_content.d2a", 0))->fn_800D246C(1);
    (mInfo = fn_801F3FF8(this, "/data/d2anime/camp_info.d2a", 0))->fn_800D246C(1);
    (mListBase = fn_801F3FF8(this, "/data/d2anime/shop_material_list_base.d2a", 0))->fn_800D246C(1);
    for (int i = 0; i < 10; i++) {
        (mList[i] = CreateLayout_unk(this, "/data/d2anime/shop_color_list.d2a"))->fn_800D246C(1);
    }
    for (int i = 0; i < 3; i++) {
        (mNeed[i] = CreateLayout_unk(this, "/data/d2anime/shop_color_list.d2a"))->fn_800D246C(1);
    }
}

ShopColor::~ShopColor() {
    if (sInstance != 0) {
        sInstance = 0;
    }
}

int ShopColor::vf0C() {
    if (fn_800D3FA4() && !fn_8057F880(this)) {
        fn_8057F8F8(this);
        vf38((const char*)mFile.fn_80470580(), mFile.fn_8047059C());
        fn_804A39EC(mTop, 1, 0, 0.0f, 1.0f);
        mTop->mFlags |= 4;
        mContent->fn_800D246C(0);
        mContent->mRepeat = 0;
        mContent->mFlags |= 4;
        mInfo->fn_800D246C(0);
        mInfo->mRepeat = 0;
        mInfo->mSpeed = -1.0f;
        fn_804A39EC(mListBase, 1, 0, 0.0f, 0.0f);
        for (int i = 0; i < 10; i++) {
            fn_804A3A68(mList[i], 1, 0, 0.0f, 1.0f);
            mList[i]->mFlags |= 4;
        }
        for (int i = 0; i < 3; i++) {
            mNeed[i]->fn_800D246C(0);
            mNeed[i]->mRepeat = 0;
            mNeed[i]->mFlags |= 4;
        }
        m25BAC = 1;
        return 1;
    }
    return 0;
}

void ShopColor::vf10() {
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
        fn_80587D10();
        break;
    case 7:
        fn_80587E3C();
        break;
    case 8: {
        void* pad = lbl_8087EF70;
        if (fn_800A555C(pad, 0, 4) || fn_800A555C(pad, 0, 5)) {
            fn_80117228(2);
            vf1C(7);
        }
        break;
    }
    }
    if (mState == 8) {
        mInfo->mSpeed = 1.0f;
    } else {
        mInfo->mSpeed = -1.0f;
    }
    fn_80580264(this);
}

void ShopColor::vf14() {
    fn_80580DE0(this);
    mTop->mFlags |= 4;
    fn_80580D80(mContent);
    mListBase->mFlags |= 4;
    for (int i = 0; i < 10; i++) {
        fn_80580DB0(mList[i]);
    }
    for (int i = 0; i < 3; i++) {
        fn_80580DB0(mNeed[i]);
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
        fn_805880F0();
        if (mCursor < (int)mColors.size()) {
            fn_805883D8(&mColors[mCursor]);
        }
        if (mColors.size() != 0) {
            char name[0x40];
            sprintf(name, "list_cursor%d", mCursor - mScrollTop + 1);
            ShopPoint_unk pos = GetPanePos_unk(mListBase, name);
            fn_804A55FC(MenuCommonTask::sInstance, mList[mCursor - mScrollTop], 0, &pos, 0);
        }
        break;
    case 8:
        fn_805880F0();
        if (mCursor < (int)mColors.size()) {
            fn_805883D8(&mColors[mCursor]);
        }
        break;
    case 2:
        if (mPrevState == 4) {
            mTop->mFlags &= ~4;
        } else {
            fn_805880F0();
            if (mCursor < (int)mColors.size()) {
                fn_805883D8(&mColors[mCursor]);
            }
        }
        break;
    }
}

// NONMATCHING: 99.38% - inlined std::vector::push_back: the original loads the size before the capacity
// (known issue of std/vector.h) and keeps the max() temporaries in other stack slots; the pointers of the
// inlined __push_front_unk are swapped
void ShopColor::vf38(const char* data, int size) {
    mColors.clear();
    atn::Csv csv;
    csv.SetData(data, size);
    do {
        const char* token = csv.fn_8005B9CC();
        if (strncmp(token, ";", 1) == 0) {
        } else if (strcmp(token, "shopTitle") == 0) {
            mTitle = atoi(csv.fn_8005B9CC());
        } else if (strcmp(token, "shopName") == 0) {
            mName = atoi(csv.fn_8005B9CC());
        } else {
            ShopColorEntry_unk entry;
            entry.mId = fn_800DC12C(token);
            if (fn_80211480(entry.mId) != 0) {
                for (int i = 0; i < 3; i++) {
                    entry.mMat[i] = fn_800DC12C(csv.fn_8005B9CC());
                    entry.mNum[i] = fn_800DC12C(csv.fn_8005B9CC());
                }
                if (fn_804444E8(lbl_8087F4F0, entry.mId) > 0) {
                    entry.mOwned = 1;
                } else {
                    entry.mOwned = 0;
                }
                entry.mShow = 0;
                if (entry.mOwned != 0) {
                    entry.mShow = 1;
                }
                for (int i = 0; i < 3; i++) {
                    if (fn_804444E8(lbl_8087F4F0, entry.mMat[i]) > 0) {
                        entry.mShow = 1;
                        break;
                    }
                }
                mColors.push_back(entry);
            }
        }
    } while (csv.fn_8005BBF8());
}

void ShopColor::vf1C(int state) {
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
        mScroll.mCount = 2;
        mScroll.mTop = 0;
        mScroll.mPage = 2;
        break;
    case 7:
        vf38((const char*)mFile.fn_80470580(), mFile.fn_8047059C());
        mScroll.mCursor = mCursor;
        mScroll.mTop = mScrollTop;
        mScroll.mCount = mColors.size();
        mScroll.mPage = 10;
        break;
    }
}

void ShopColor::vf20(int result) {
    if (result == 5) {
        fn_80584D78(this, &mEntries5C[mDC], m25B24);
        fn_80581FD8(this);
        fn_80117228(25);
        vf1C(5);
    } else {
        fn_80117228(2);
        vf1C(7);
    }
}

void ShopColor::fn_80587D10() {
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
            fn_80117228(2);
            vf1C(1);
            break;
        }
    } else if (fn_800A555C(pad, 0, 5)) {
        fn_80117228(2);
        vf1C(1);
    }
}

void ShopColor::fn_80587E3C() {
    fn_80584808(this, 1);
    mCursor = mScroll.mCursor;
    mScrollTop = mScroll.mTop;
    void* pad = lbl_8087EF70;
    if (fn_800A555C(pad, 0, 4)) {
        ShopColorEntry_unk* entry = &mColors[mCursor];
        int error = fn_80588004(entry);
        if (error == 0) {
            ShopItem_unk* item = fn_80211480(entry->mId);
            if (item != 0) {
                wchar_t text[0x40];
                fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0x353), item->mName);
                SetPaneText_unk(mInfo->mLayout, "warning_msg", text);
            }
            fn_80117228(25);
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

int ShopColor::fn_80588004(ShopColorEntry_unk* entry) {
    if (entry->mOwned != 0) {
        return 2;
    }
    for (int i = 0; i < 3; i++) {
        ShopItem_unk* item = fn_80211480(entry->mMat[i]);
        if (item != 0 && entry->mNum[i] > fn_804444E8(lbl_8087F4F0, item->mCountId)) {
            return 14;
        }
    }
    fn_804439FC(lbl_8087F4F0, entry->mId, 1, 0, 0, 0, 1, 1);
    for (int i = 0; i < 3; i++) {
        if (entry->mMat[i] != 0) {
            fn_8044441C(lbl_8087F4F0, entry->mMat[i], entry->mNum[i]);
        }
    }
    return 0;
}

// NONMATCHING: 97.90% - register allocation only (the party, the pool and the address of uv are colored
// in another order)
void ShopColor::fn_805880F0() {
    void* party = lbl_8087F4F0;
    mListBase->mFlags &= ~4;
    SetPaneFloat_unk(mListBase->mLayout, "ofsOn", 1.0f);
    fn_80584750(this, mListBase, mScroll.mTop, mColors.size(), 10);
    ShopPoint_unk pos;
    mContent->mFlags &= ~4;
    for (int i = 0; i < 10; i++) {
        char name[0x40];
        sprintf(name, "list_point%d", i + 1);
        pos = GetPanePos_unk(mListBase, name);
        fn_801F6E78(mList[i], "basePos", &pos);
        int index = i + mScroll.mTop;
        if (index < (int)mColors.size()) {
            ShopItem_unk* item = fn_80211480(mColors[index].mId);
            if (item != 0) {
                fn_801F837C(mList[i], "itemName", item->mName);
                int num = fn_804444E8(party, item->mCountId);
                fn_801F6C80(mList[i], "showPrice", 0.0f);
                fn_801F6C80(mList[i], "showNum2", 0.0f);
                fn_801F8598(mList[i], "needNum", num, 0);
                fn_801F6C80(mList[i], "faceOn", 0.0f);
                ShopUV_unk uv = fn_80444C50(lbl_8087F4F0, fn_80444BE8(lbl_8087F4F0, item));
                fn_801F7590(mList[i], "iconUv", uv);
                if (num == 0) {
                    fn_801F6C80(mList[i], "colorOn", 0.0f);
                    fn_801F7DF0(mList[i], "itemCol", 255.0f, 255.0f, 255.0f);
                    fn_801F6C80(mList[i], "subColorR", 180.0f);
                    fn_801F6C80(mList[i], "subColorR", 180.0f);
                    fn_801F6C80(mList[i], "subColorR", 180.0f);
                } else {
                    fn_801F6C80(mList[i], "colorOn", 0.0f);
                    fn_801F7DF0(mList[i], "itemCol", 128.0f, 128.0f, 128.0f);
                    fn_801F6C80(mList[i], "subColorR", 100.0f);
                    fn_801F6C80(mList[i], "subColorR", 100.0f);
                    fn_801F6C80(mList[i], "subColorR", 100.0f);
                }
                mList[i]->mFlags &= ~4;
            }
        }
    }
}

void ShopColor::fn_805883D8(ShopColorEntry_unk* entry) {
    void* party = lbl_8087F4F0;
    ShopItem_unk* item = fn_80211480(entry->mId);
    if (item != 0) {
        fn_804A3C24(MenuCommonTask::sInstance, item->mText, 0);
    }
    ShopPoint_unk pos;
    for (int i = 0; i < 3; i++) {
        char name[0x40];
        sprintf(name, "NeedItem%d", i + 1);
        pos = GetPanePos_unk(mContent, name);
        fn_801F6E78(mNeed[i], "basePos", &pos);
        if (entry->mMat[i] != 0) {
            mNeed[i]->mFlags &= ~4;
            ShopItem_unk* mat = fn_80211480(entry->mMat[i]);
            if (mat != 0) {
                fn_801F6C80(mNeed[i], "showNum2", 1.0f);
                fn_801F8598(mNeed[i], "needNum", entry->mNum[i], 0);
                fn_801F8598(mNeed[i], "num", fn_804444E8(party, mat->mCountId), 0);
                fn_801F6C80(mNeed[i], "faceOn", 0.0f);
                fn_801F837C(mNeed[i], "itemName", mat->mName);
                if (entry->mNum[i] <= fn_804444E8(party, mat->mCountId)) {
                    fn_801F6C80(mNeed[i], "colorOn", 0.0f);
                    fn_801F7DF0(mNeed[i], "itemCol", 255.0f, 255.0f, 255.0f);
                    fn_801F6C80(mNeed[i], "subColorR", 180.0f);
                    fn_801F6C80(mNeed[i], "subColorR", 180.0f);
                    fn_801F6C80(mNeed[i], "subColorR", 180.0f);
                } else {
                    fn_801F6C80(mNeed[i], "colorOn", 0.0f);
                    fn_801F7DF0(mNeed[i], "itemCol", 128.0f, 128.0f, 128.0f);
                    fn_801F6C80(mNeed[i], "subColorR", 100.0f);
                    fn_801F6C80(mNeed[i], "subColorR", 100.0f);
                    fn_801F6C80(mNeed[i], "subColorR", 100.0f);
                }
            }
        }
    }
}

int ShopColor::vf3C() {
    return 1;
}
