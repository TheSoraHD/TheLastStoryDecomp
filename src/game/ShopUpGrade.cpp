#include "game/ShopUpGrade.h"
#include "game/GameTask.h"
#include "game/Inventory.h"
#include "game/MenuCommonTask.h"
#include "game/CharaTask.h"
#include "chr/CharaEquip_unk.h"
#include "game/PlayerManager.h"
#include "game/PlayerTask.h"
#include "atn/Memory.h"
#include "atn/SoundRef.h"
#include "atn/FixedArray.h"
#include "math/Vec.h"
#include "std/string.h"

extern "C" int sprintf(char* s, const char* format, ...);
extern "C" wchar_t* wcsstr(const wchar_t* s, const wchar_t* sub);
extern "C" ShopD2A_unk* fn_801F3FF8(atn::Task* parent, const char* path, int loop); // inferred: d2anime task
extern "C" ShopLayout_unk* fn_801F64D0(atn::Task* parent, const char* path);      // inferred: layout anime
extern "C" atn::SoundRef fn_80117228(int id); // inferred: plays a system sound effect
extern "C" void fn_804A5824(MenuCommonTask* menu, atn::Task* anime, const char* name, int end);
extern "C" void fn_804A39EC(ShopD2A_unk* anime, int arg1, int repeat, float frame, float speed);
extern "C" void fn_804A3A68(ShopLayout_unk* layout, int arg1, int repeat, float frame, float speed);
extern "C" GameTask* lbl_8087F430; // inferred: field
extern "C" int fn_804444E8(Inventory* party, int id); // inferred: number of items
extern "C" void fn_80212714(ItemParamEntry_unk* item); // inferred: frees an upgraded item
extern "C" int fn_80206BE4(WeaponParamEntry_unk* weapon); // inferred: item code of a weapon
extern "C" int fn_8020EF80(EquipParamEntry_unk* equip); // inferred: item code of a protector
extern "C" int fn_8044453C(Inventory* inv, int index);  // inferred: number of items of an entry of the item table
extern "C" int fn_8044D560(Inventory* party);           // inferred: number of items to identify
extern "C" int fn_8044D500(Inventory* party, int index); // inferred: item to identify
extern "C" void* lbl_8087EF70; // inferred: pad
extern "C" int fn_800A555C(void* pad, int port, int button); // inferred: button triggered
extern "C" void* lbl_8087F518; // inferred: loader
extern "C" void fn_8046ECDC(void* loader, int show); // inferred: shows the loading screen
extern "C" void fn_8046EED8(void* loader);           // inferred: hides the loading screen
extern "C" PlayerManager* lbl_8087F8A0; // inferred: the party
extern "C" void* fn_8044D034(Inventory* party, int index); // inferred: equipment data
extern "C" void fn_8044D104(Inventory* party, int slot, int item, int upgrade, int chara); // inferred: sets a weapon
extern "C" int fn_80219544(int member); // inferred: character id of a party member
extern "C" void* lbl_8087F048; // inferred: effect manager of the characters
extern "C" void fn_8010C00C(void* mgr, CharaTask* chara);
extern "C" bool fn_802124B4(ItemParamEntry_unk* item, int level); // inferred: the item can be upgraded
extern "C" int fn_80211940(ItemParamEntry_unk* item, int level);   // inferred: makes the upgraded item
extern "C" int fn_80444564(Inventory* party, int id); // inferred: number of equipment items
extern "C" void fn_8044441C(Inventory* party, int id, int num); // inferred: removes items
extern "C" void fn_8044D6AC(Inventory* party, int money);       // inferred: pays money
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
extern "C" void fn_801FEE08(void* layout, unsigned int hash, const wchar_t* text); // inferred: sets a text
extern "C" void fn_801FECE0(void* layout, unsigned int hash, float value);
extern "C" void fn_801FED24(void* layout, unsigned int hash, int index, float value);
// inferred: 2D point of a d2anime pane (0x14 bytes)
struct ShopPoint_unk {
    ShopPoint_unk() : x(0.0f), y(0.0f), w(0.0f), h(0.0f), z(0.0f) {}
    float x, y, w, h, z;
};
extern "C" ShopPoint_unk fn_801F4E8C(atn::Task* layout, unsigned int hash);
extern "C" void fn_801F4728(atn::Task* task, const char* name, const ShopPoint_unk* pos); // inferred: SetParam0
extern "C" void fn_804A55FC(MenuCommonTask* menu, atn::Task* id, const char* name, const ShopPoint_unk* rect,
                            int end);
inline ShopPoint_unk GetPanePos_unk(atn::Task* layout, const char* name) {
    return fn_801F4E8C(layout, fn_800DC6B4(name));
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
extern "C" void fn_801F6E78(atn::Task* task, const char* name, const ShopPoint_unk* pos);
extern "C" void fn_801F6C80(atn::Task* layout, const char* name, float value);    // inferred: sets a value
extern "C" void fn_801F837C(atn::Task* layout, const char* name, const wchar_t* text); // inferred: sets a text
extern "C" void fn_801F8598(atn::Task* layout, const char* name, int value, int arg); // inferred: sets a number
extern "C" void fn_801F7DF0(atn::Task* layout, const char* name, float r, float g, float b); // inferred: sets a colour
extern "C" void fn_804A4CE4(atn::Task* task, int face, int a, int b); // inferred: sets the face of a member
// inferred: texture rectangle (as MenuUV_unk in game/MenuCommonTask.cpp)
struct ShopUV_unk {
    ShopUV_unk() {}
    ShopUV_unk(const ShopUV_unk& o) {
        __Vec4Copy(&u0, &o.u0);
    }
    float u0, v0, u1, v1;
};
extern "C" void fn_801F7590(atn::Task* layout, const char* name, ShopUV_unk uv); // inferred: sets a rectangle
extern "C" int fn_80444BE8(Inventory* party, ItemParamEntry_unk* item); // inferred: icon of an item
extern "C" int fn_80444C48(Inventory* party);                          // inferred: icon of an unknown item
extern "C" ShopUV_unk fn_80444C50(Inventory* party, int icon);
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
// Only the members used here (the full class is in atn/D2A_Task.h, whose extern "C" declarations
// clash with the local ones of this file, as in game/ShopIdentify.cpp).
namespace atn {
class D2A_Task : public Task {
public:
    void SetColor_unk(const char* name, float r, float g, float b); // 0x801F4AA0
    void SetNumber_unk(const char* name, int value, int digits);     // 0x801F4CB4
};
} // namespace atn
// inferred: the first parameters of a weapon or protector row (same prefix in both tables)
struct ShopParamHead_unk {
    float m00;
    float m04;
    float m08;
    float m0C;
};
// inferred: message ids of the five parameters, then the parameters shown for weapons and protectors
struct ShopUpGradeParams_unk {
    int mMsg[5];
    int mWeapon[3];
    int mArmor[3];
};
// inferred: colours of a raised, lowered and unchanged value (as in game/CosEditItemList_unk.cpp)
struct ShopColor_unk {
    float r, g, b, a;
};
extern "C" ShopColor_unk lbl_807C7898;
extern "C" ShopColor_unk lbl_807C78A8;
extern "C" ShopColor_unk lbl_807C78C8;
extern "C" const wchar_t* fn_80211770(int id, int slot); // inferred: name of an upgraded item
extern "C" bool fn_8021C6A4(wchar_t* out, const void* skill); // inferred: name of a skill
extern "C" void fn_80212C34(EquipParamEntry_unk* equip, EquipParamEntry_unk* base, int level);
extern "C" void fn_80212840(WeaponParamEntry_unk* weapon, WeaponParamEntry_unk* base, int level);
extern "C" void fn_804A3C24(MenuCommonTask* menu, const wchar_t* text, int id);
extern "C" void fn_804A7DD4(MenuCommonTask* menu, int item);
// inferred: debug settings (lbl_8087F0A8)
struct ShopSettings_unk {
    unsigned char m000[0x488];
    int m488;
};
extern "C" ShopSettings_unk* lbl_8087F0A8;
// inferred: party members available at a progress (as game/ShopBuySell.cpp)
struct ShopMembers_unk {
    unsigned char m00[0x0C];
    unsigned char mIn[8];
};
extern "C" ShopMembers_unk* fn_80217D9C(int progress);


// inferred: message ids of the parameters and the parameters shown for weapons and protectors
static const ShopUpGradeParams_unk sParams = {
    {0x191, 0x192, 0x195, 0x194, 0x193},
    {0, 4, 2},
    {1, 3, 2},
};

// inferred: entries of the top menus of the four shops
static const int sMenuItems0[4] = {0, 1, 3, 4};
static const int sMenuItems1[5] = {0, 1, 2, 3, 4};
static const int sMenuItems2[3] = {1, 3, 4};
static const ShopUpGradeMenu_unk sMenus[4] = {
    {sMenuItems0, 4},
    {sMenuItems1, 5},
    {sMenuItems2, 3},
    {sMenuItems2, 3},
};

ShopUpGrade* ShopUpGrade::sInstance;

inline ShopLayout_unk* CreateLayout_unk(atn::Task* parent, const char* path) {
    return fn_801F64D0(parent, path);
}

inline WeaponParamEntry_unk::~WeaponParamEntry_unk() {}

// NONMATCHING: 98.72% - register allocation: the original keeps the offset of the member in r31
void ShopUpGrade::fn_8058AABC() {
    for (unsigned int i = 0; i < 0x600; i++) {
        Inventory::Item_unk* item = Inventory::instance_->fn_80444804(i);
        if (item != 0 && item->mCode >= 1000000 && item->mCount <= 0) {
            int worn = 0;
            for (unsigned int j = 0; j < 7; j++) {
                Inventory::Equip_unk& equip = Inventory::instance_->mEquip[j];
                if (fn_8020EF04(item->mCode)) {
                    EquipParamEntry_unk* param = fn_8020EFEC(item->mCode);
                    if (param != 0 &&
                        (equip.mProtectors[0].mId == param->mId || equip.mProtectors[1].mId == param->mId)) {
                        worn = 1;
                        break;
                    }
                } else if (fn_80206B9C(item->mCode)) {
                    WeaponParamEntry_unk* param = fn_80206C50(item->mCode);
                    if (param != 0 && (equip.mWeapons[0].mId == param->mId || equip.mWeapons[1].mId == param->mId)) {
                        worn = 1;
                        break;
                    }
                }
            }
            if (!worn) {
                fn_80212714(fn_80211480(item->mCode));
                item->mCode = -1;
            }
        }
    }
}

ShopUpGrade* ShopUpGrade::fn_8058ABF4(atn::Task* parent, int a, int b, int mode) {
    if (sInstance == 0) {
        sInstance = new ((MemoryTag)1, "", "", 0) ShopUpGrade(parent, a, b, mode);
    }
    return sInstance;
}

// NONMATCHING: 94.66% - inside the inlined WeaponParamEntry_unk/EquipParamEntry_unk constructors the original keeps this + 0x20000 in a saved register across memset
ShopUpGrade::ShopUpGrade(atn::Task* parent, int a, int b, int mode)
    : ShopBase(parent, 2, a, b), mMode(mode), m26070(0), mTopCursor(0), m26078(0), m2607C(0), m26080(-1),
      mSortOrder(0), mTab(0), mReloadNum(0), m26108(-1), m2610C(-1), m26110(-1) {
    if (mMode == 1 && !lbl_8087F430->fn_80370174(0xD7)) {
        mMode = 0;
    }
    memset(mCursors, 0, sizeof(mCursors));
    memset(mTops, 0, sizeof(mTops));
    switch (mMode) {
    case 0:
        (mTop = fn_801F3FF8(this, "/data/d2anime/shop_upgrade_top_base.d2a", 0))->fn_800D246C(1);
        break;
    case 1:
        (mTop = fn_801F3FF8(this, "/data/d2anime/shop_rotta_top_base.d2a", 0))->fn_800D246C(1);
        break;
    case 2:
    case 3:
        (mTop = fn_801F3FF8(this, "/data/d2anime/shop_upgrade_extra_top_base.d2a", 0))->fn_800D246C(1);
        break;
    }
    (mContent = fn_801F3FF8(this, "/data/d2anime/shop_upgrade_content.d2a", 0))->fn_800D246C(1);
    (mNoItem = fn_801F3FF8(this, "/data/d2anime/shop_material_noitem.d2a", 0))->fn_800D246C(1);
    (mListBase = fn_801F3FF8(this, "/data/d2anime/shop_material_list_base.d2a", 0))->fn_800D246C(1);
    for (int i = 0; i < 10; i++) {
        (mList[i] = CreateLayout_unk(this, "/data/d2anime/shop_color_list.d2a"))->fn_800D246C(1);
    }
    (mColor = CreateLayout_unk(this, "/data/d2anime/shop_color_list.d2a"))->fn_800D246C(1);
    (mColorNoItem = CreateLayout_unk(this, "/data/d2anime/shop_color_list_noitem.d2a"))->fn_800D246C(1);
    for (int i = 0; i < 2; i++) {
        (mSkill[i] = CreateLayout_unk(this, "/data/d2anime/shop_upgrade_skill_list.d2a"))->fn_800D246C(1);
        (mSkillBlue[i] = CreateLayout_unk(this, "/data/d2anime/shop_upgrade_skill_blue.d2a"))->fn_800D246C(1);
    }
    for (int i = 0; i < 3; i++) {
        (mColors[i] = CreateLayout_unk(this, "/data/d2anime/shop_color_list.d2a"))->fn_800D246C(1);
    }
    for (int i = 0; i < 3; i++) {
        (mStatusUp[i] = CreateLayout_unk(this, "data/d2anime/weapon_status_up_loop.d2a"))->fn_800D246C(1);
        (mStatusDown[i] = CreateLayout_unk(this, "data/d2anime/weapon_status_down_loop.d2a"))->fn_800D246C(1);
    }
    mEffect[0] = 0;
    (mEffect[1] = fn_801F3FF8(this, "data/d2anime/ui_upgrade00.d2a", 0))->fn_800D246C(1);
    (mCursorAnime = fn_801F3FF8(this, "data/d2anime/camp_cursor_upgrade.d2a", 0))->fn_800D246C(1);
    (mSort = fn_801F3FF8(this, "/data/d2anime/camp_sort.d2a", 0))->fn_800D246C(1);
}

ShopUpGrade::~ShopUpGrade() {
    if (sInstance != 0) {
        sInstance = 0;
    }
}

int ShopUpGrade::vf0C() {
    if (fn_800D3FA4() && !fn_8057F880()) {
        fn_8057F8F8();
        vf38((const char*)mFile.fn_80470580(), mFile.fn_8047059C());
        fn_8058DB10();
        fn_804A39EC(mTop, 1, 0, 0.0f, 1.0f);
        mTop->mFlags |= 4;
        mContent->fn_800D246C(0);
        mContent->mRepeat = 0;
        mContent->mFlags |= 4;
        mNoItem->fn_800D246C(0);
        mNoItem->mRepeat = 0;
        mNoItem->mFlags |= 4;
        fn_804A39EC(mListBase, 1, 0, 0.0f, 0.0f);
        for (int i = 0; i < 10; i++) {
            fn_804A3A68(mList[i], 1, 0, 0.0f, 1.0f);
            mList[i]->mFlags |= 4;
        }
        mColor->fn_800D246C(0);
        mColor->mRepeat = 0;
        mColor->mFlags |= 4;
        mColorNoItem->fn_800D246C(0);
        mColorNoItem->mRepeat = 0;
        mColorNoItem->mFlags |= 4;
        for (int i = 0; i < 2; i++) {
            mSkill[i]->fn_800D246C(0);
            mSkill[i]->mRepeat = 0;
            mSkill[i]->mFlags |= 4;
            mSkillBlue[i]->fn_800D246C(0);
            mSkillBlue[i]->mRepeat = 1;
            mSkillBlue[i]->mFlags |= 4;
        }
        for (int i = 0; i < 3; i++) {
            mColors[i]->fn_800D246C(0);
            mColors[i]->mRepeat = 0;
            mColors[i]->mFlags |= 4;
        }
        for (int i = 0; i < 3; i++) {
            mStatusUp[i]->fn_800D246C(0);
            mStatusUp[i]->mFlags |= 4;
            mStatusDown[i]->fn_800D246C(0);
            mStatusDown[i]->mFlags |= 4;
        }
        for (int i = 0; i < 2; i++) {
            if (mEffect[i] != 0) {
                mEffect[i]->fn_800D246C(0);
                mEffect[i]->mRepeat = 0;
                mEffect[i]->mFrame = mEffect[i]->mLastFrame;
                mEffect[i]->mFlags |= 4;
            }
        }
        mCursorAnime->fn_800D246C(0);
        mCursorAnime->mRepeat = 0;
        mCursorAnime->mFlags |= 4;
        mSort->fn_800D246C(0);
        mSort->mRepeat = 0;
        mSort->mFlags |= 4;
        m26070 = 1;
        return 1;
    }
    return 0;
}

void ShopUpGrade::vf10() {
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
        fn_8058C74C();
        break;
    case 7:
        fn_8058CA44();
        break;
    case 8:
        fn_8058CF60();
        break;
    case 5:
        fn_8057FF38();
        break;
    case 6:
        fn_805800FC();
        break;
    }
    fn_80580264();
}

void ShopUpGrade::vf14() {
    fn_80580DE0();
    mTop->mFlags |= 4;
    fn_80580D80(mContent);
    mNoItem->mFlags |= 4;
    mListBase->mFlags |= 4;
    for (int i = 0; i < 10; i++) {
        fn_80580DB0(mList[i]);
    }
    fn_80580DB0(mColor);
    fn_80580DB0(mColorNoItem);
    for (int i = 0; i < 2; i++) {
        fn_80580DB0(mSkill[i]);
        fn_80580DB0(mSkillBlue[i]);
    }
    for (int i = 0; i < 3; i++) {
        fn_80580DB0(mColors[i]);
    }
    for (int i = 0; i < 3; i++) {
        mStatusUp[i]->mFlags |= 4;
        mStatusDown[i]->mFlags |= 4;
    }
    for (int i = 0; i < 2; i++) {
        if (mEffect[i] != 0) {
            mEffect[i]->mFlags |= 4;
        }
    }
    fn_80580D80(mCursorAnime);
    mSort->mFlags |= 4;
    switch (mState) {
    case 2:
    case 3:
        fn_8058D6D8();
        break;
    case 4: {
        mTop->mFlags &= ~4;
        char name[0x40];
        sprintf(name, "lw_list_text_select%02d", mTopCursor);
        fn_804A5824(MenuCommonTask::sInstance, mTop, name, 0);
        break;
    }
    case 7:
        fn_8058D16C();
        break;
    case 8:
        fn_8058D458();
        break;
    case 5:
        fn_80580580();
        break;
    case 6:
        fn_80580690();
        break;
    }
}

void ShopUpGrade::vf1C(int state) {
    m25B40 = 1;
    mPrevState = mState;
    mState = state;
    switch (state) {
    case 1:
        m25B40 = 0;
        fn_8057FB20();
        break;
    case 4:
        mScroll.mCursor = mTopCursor;
        mScroll.mCount = sMenus[mMode].mCount;
        mScroll.mTop = 0;
        mScroll.mPage = sMenus[mMode].mCount;
        break;
    case 7: {
        fn_8058DB10();
        mScroll.mCount = mItems.size();
        mScroll.mPage = 10;
        fn_80597098(m26108, m2610C, mTab);
        if (mCursors[mTab] >= mScroll.mCount) {
            int last = mScroll.mCount - 1;
            mCursors[mTab] = last < 0 ? 0 : last;
        }
        if (mScroll.mCount >= mScroll.mPage && mScroll.mCount - 1 <= mTops[mTab] + mScroll.mPage - 1) {
            mTops[mTab] = mScroll.mCount - mScroll.mPage;
        }
        mScroll.mCursor = mCursors[mTab];
        mScroll.mTop = mTops[mTab];
        m26108 = -1;
        m2610C = -1;
        break;
    }
    case 5:
        mScroll.mCursor = mDC;
        mScroll.mTop = mE0;
        mScroll.mPage = 10;
        fn_80581FD8();
        if (mPrevState == 4) {
            mScroll.mCursor = 0;
            mScroll.mTop = 0;
            mDC = 0;
            mE0 = 0;
        }
        if (m32FC == 0) {
            fn_80117228(11);
            fn_8058181C(1, 10, 0);
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
        if (m26080 >= 0 && mEffect[m26080] != 0) {
            mEffect[m26080]->mFrame = 0.0f;
        }
        mCursorAnime->mFrame = 0.0f;
        break;
    case 3:
        m25B40 = 0;
        break;
    }
}

// inferred: whether `code` is in the list ended by a negative value (inline, name invented)
inline bool Contains_unk(const int* list, int code) {
    for (; *list >= 0; list++) {
        if (code == *list) {
            return true;
        }
    }
    return false;
}

// NONMATCHING: 98.67% - register allocation (the original keeps the entry in r31 and this in r27)
void ShopUpGrade::vf20(int result) {
    if (result == 0 || result == 1) {
        ShopUpGradeEntry_unk* entry = &mItems[mCursors[mTab]];
        ItemParamEntry_unk* item = fn_80211480(entry->mCode);
        if (item != 0) {
            int code = item->m134;
            if (entry->mMember < 0) {
                code = fn_8058D994(entry, 1);
            } else {
                if (code == 0) {
                    fn_8058AABC();
                    code = fn_80211940(item, 1);
                }
                if (fn_8020EF04(code)) {
                    EquipParamEntry_unk* param = fn_8020EFEC(code);
                    int id = param != 0 ? param->mId : 0;
                    EquipParamEntry_unk* pair = 0;
                    if (param->mKind == 0) {
                        pair = fn_8020ED84(2, param->mId);
                    } else if (param->mKind == 1) {
                        pair = fn_8020ED84(3, param->mId);
                    }
                    {
                        int member = entry->mMember;
                        int chara = fn_80219544(member);
                        int index = param->mKind + member * 4;
                        PlayerTask* player = lbl_8087F8A0->fn_8054A33C(member);
                        if (player != 0) {
                            unsigned int* data = (unsigned int*)fn_8044D034(Inventory::instance_, index);
                            int options = Inventory::instance_->mPlayers[member].fn_801347C8(param->mKind);
                            chr::CharaEquip_unk* equip = new ((MemoryTag)6, "", "", 0)
                                chr::CharaEquip_unk(param->mKind, id, index, player, data, 0, options, 1, 1);
                            ShopUpGradeReload_unk reload;
                            reload.mKind = 0;
                            reload.mEquip = equip;
                            reload.mChara = player;
                            PushReload_unk(reload);
                        }
                        Inventory::instance_->fn_8044D060(param->mKind, id, index, chara);
                    }
                    {
                        int member = entry->mMember;
                        int chara = fn_80219544(member);
                        int index = pair->mKind + member * 4;
                        PlayerTask* player = lbl_8087F8A0->fn_8054A33C(member);
                        if (player != 0) {
                            unsigned int* data = (unsigned int*)fn_8044D034(Inventory::instance_, index);
                            int options = Inventory::instance_->mPlayers[member].fn_801347C8(pair->mKind);
                            chr::CharaEquip_unk* equip = new ((MemoryTag)6, "", "", 0)
                                chr::CharaEquip_unk(pair->mKind, id, index, player, data, 0, options, 1, 1);
                            ShopUpGradeReload_unk reload;
                            reload.mKind = 0;
                            reload.mEquip = equip;
                            reload.mChara = player;
                            PushReload_unk(reload);
                        }
                        Inventory::instance_->fn_8044D060(pair->mKind, id, index, chara);
                    }
                    if (param->mKind == 0 && fn_8020EF4C(param)) {
                        EquipParamEntry_unk* a = fn_8020ED84(1, param->mId);
                        EquipParamEntry_unk* b = fn_8020ED84(3, param->mId);
                        {
                            int member = entry->mMember;
                            int chara = fn_80219544(member);
                            int index = a->mKind + member * 4;
                            PlayerTask* player = lbl_8087F8A0->fn_8054A33C(member);
                            if (player != 0) {
                                unsigned int* data = (unsigned int*)fn_8044D034(Inventory::instance_, index);
                                int options = Inventory::instance_->mPlayers[member].fn_801347C8(a->mKind);
                                chr::CharaEquip_unk* equip = new ((MemoryTag)6, "", "", 0)
                                    chr::CharaEquip_unk(a->mKind, id, index, player, data, 0, options, 1, 1);
                                ShopUpGradeReload_unk reload;
                                reload.mKind = 0;
                                reload.mEquip = equip;
                                reload.mChara = player;
                                PushReload_unk(reload);
                            }
                            Inventory::instance_->fn_8044D060(a->mKind, id, index, chara);
                        }
                    {
                        int member = entry->mMember;
                        int chara = fn_80219544(member);
                        int index = b->mKind + member * 4;
                        PlayerTask* player = lbl_8087F8A0->fn_8054A33C(member);
                        if (player != 0) {
                            unsigned int* data = (unsigned int*)fn_8044D034(Inventory::instance_, index);
                            int options = Inventory::instance_->mPlayers[member].fn_801347C8(b->mKind);
                            chr::CharaEquip_unk* equip = new ((MemoryTag)6, "", "", 0)
                                chr::CharaEquip_unk(b->mKind, id, index, player, data, 0, options, 1, 1);
                            ShopUpGradeReload_unk reload;
                            reload.mKind = 0;
                            reload.mEquip = equip;
                            reload.mChara = player;
                            PushReload_unk(reload);
                        }
                        Inventory::instance_->fn_8044D060(b->mKind, id, index, chara);
                    }
                    }
                    fn_8058D994(entry, 0);
                } else if (fn_80206B9C(code)) {
                    WeaponParamEntry_unk* weapon = fn_80206C50(code);
                    int member = entry->mMember;
                    int slot = entry->mSlot;
                    int chara = fn_80219544(member);
                    PlayerTask* player = lbl_8087F8A0->fn_8054A33C(member);
                    fn_8044D104(Inventory::instance_, slot, weapon->mId, -1, chara);
                    if (player != 0) {
                        int part = player->m674;
                        player->fn_8014EEC4(1);
                        player->fn_8014F5D4(slot, weapon->mKind, weapon->mId);
                        if (part >= 0) {
                            player->fn_8014DEE4(part, 0, 1);
                        }
                        ShopUpGradeReload_unk reload;
                        reload.mKind = 1;
                        reload.mEquip = 0;
                        reload.mChara = player;
                        PushReload_unk(reload);
                    }
                    fn_8058D994(entry, 0);
                }
            }
            m26110 = entry->mCode;
            m26108 = code;
            m2610C = entry->mMember;
        }
        if (lbl_8087F430->fn_80370174(0x11C)) {
            lbl_8087F430->fn_80370320(0x11C, 0, 0);
        }
        m26080 = -1;
        if (result == 1) {
            fn_80117228(0x1B);
        } else {
            fn_80117228(0x1C);
            // inferred: weapons whose upgrade plays the second effect
            int codes[7] = {0x1882F, 0x18830, 0x18831, 0x18832, 0x18833, 0x18834, -1};
            bool found = Contains_unk(codes, m26108);
            if (found) {
                m26080 = 1;
                fn_80117228(0x1D);
            } else {
                m26080 = 0;
            }
        }
        vf1C(8);
    } else if (result == 2) {
        if (lbl_8087F430->fn_80370174(0x11C)) {
            lbl_8087F430->fn_80370320(0x11C, 0, 0);
        }
        fn_80117228(1);
        vf1C(1);
    } else if (result == 5) {
        fn_80584D78(&mEntries5C[mDC], m25B24);
        fn_80581FD8();
        fn_80117228(0x19);
        vf1C(5);
    } else {
        fn_80117228(2);
        vf1C(4);
    }
}

void ShopUpGrade::fn_8058C74C() {
    fn_805847EC(1);
    mTopCursor = mScroll.mCursor;
    m26078 = sMenus[mMode].mItems[mTopCursor];
    void* pad = lbl_8087EF70;
    if (fn_800A555C(pad, 0, 4)) {
        switch (m26078) {
        case 0:
        case 1:
        case 2:
            if (m26078 == 0) {
                mTab = 0;
            } else if (m26078 == 2) {
                mTab = 2;
            } else {
                mTab = 1;
            }
            fn_8058DB10();
            if (mItems.size() == 0) {
                fn_80117228(11);
                if (m26078 == 2) {
                    fn_8058181C(1, 13, 0);
                } else {
                    fn_8058181C(1, 12, 0);
                }
                vf1C(2);
            } else {
                fn_80117228(1);
                vf1C(7);
            }
            break;
        case 3:
            fn_80117228(1);
            vf1C(5);
            break;
        case 4:
            if (lbl_8087F430->fn_80370174(0x11C)) {
                fn_80117228(2);
                fn_80581964(1, 2, 0);
                vf1C(3);
            } else {
                fn_80117228(2);
                vf1C(1);
            }
            break;
        }
    } else if (fn_800A555C(pad, 0, 5)) {
        if (lbl_8087F430->fn_80370174(0x11C)) {
            fn_80117228(2);
            fn_80581964(1, 2, 1);
            vf1C(3);
        } else {
            fn_80117228(2);
            vf1C(1);
        }
    }
}

void ShopUpGrade::fn_8058CA44() {
    if (mItems.size() == 0) {
        vf1C(4);
        fn_80117228(11);
        fn_8058181C(1, 13, 0);
        vf1C(2);
        return;
    }
    fn_80584808(1);
    if (mScroll.mCursor != mCursors[mTab]) {
        mSkillBlue[0]->mFrame = 0.0f;
        mSkillBlue[1]->mFrame = 0.0f;
    }
    mCursors[mTab] = mScroll.mCursor;
    mTops[mTab] = mScroll.mTop;
    void* pad = lbl_8087EF70;
    if (fn_800A555C(pad, 0, 4)) {
        int error = fn_8058D7F8(&mItems[mCursors[mTab]]);
        if (error == 0) {
            fn_80117228(1);
            if (mTab == 2) {
                fn_80581964(1, 1, 0);
            } else {
                fn_80581964(1, 0, 0);
            }
            vf1C(3);
        } else {
            fn_80117228(11);
            fn_8058181C(1, error, 0);
            vf1C(2);
        }
    } else if (fn_800A555C(pad, 0, 5)) {
        fn_80117228(2);
        vf1C(4);
    } else if (fn_800A555C(pad, 0, 12)) {
        if (mSortOrder == 1) {
            mSortOrder = 0;
            ShopUpGradeEntry_unk entry = mItems[mCursors[mTab]];
            fn_8058DB10();
            fn_80597164(entry, mScroll.mCursor, mScroll.mTop);
            mCursors[mTab] = mScroll.mCursor;
            mTops[mTab] = mScroll.mTop;
        }
        fn_80117228(0x12);
    } else if (fn_800A555C(pad, 0, 13)) {
        if (mSortOrder == 0) {
            mSortOrder = 1;
            ShopUpGradeEntry_unk entry = mItems[mCursors[mTab]];
            fn_8058DB10();
            fn_80597164(entry, mScroll.mCursor, mScroll.mTop);
            mCursors[mTab] = mScroll.mCursor;
            mTops[mTab] = mScroll.mTop;
        }
        fn_80117228(0x12);
    }
}

void ShopUpGrade::fn_8058CF60() {
    if (mReloadNum != 0) {
        fn_8046ECDC(lbl_8087F518, 0);
        int loading = 0;
        for (unsigned int i = 0; i < mReloadNum; i++) {
            if (mReload[i].mKind == 0) {
                if (mReload[i].mEquip->fn_8056405C()) {
                    loading = 1;
                    break;
                }
            } else if (mReload[i].mKind == 1) {
                if (mReload[i].mChara->fn_8014FAD0()) {
                    loading = 1;
                    break;
                }
            }
        }
        if (!loading) {
            for (unsigned int i = 0; i < mReloadNum; i++) {
                if (mReload[i].mKind == 0) {
                    mReload[i].mEquip->fn_80566010();
                    mReload[i].mChara->fn_80150178(*(int*)mReload[i].mEquip, mReload[i].mEquip);
                    mReload[i].mEquip->fn_805659B4();
                    if (lbl_8087F048 != 0) {
                        mReload[i].mChara->mStatus.fn_8013407C();
                        fn_8010C00C(lbl_8087F048, mReload[i].mChara);
                    }
                    mReload[i].mChara->mStatus.fn_8012B988();
                    if (mReload[i].mChara->m674 < 0) {
                        mReload[i].mChara->mStatus.fn_8012D8B8();
                    }
                }
                if (mReload[i].mChara != 0) {
                    mReload[i].mChara->fn_80148B38(0.0f);
                }
            }
            mReloadNum = 0;
            fn_8046EED8(lbl_8087F518);
        }
    } else if (m26080 < 0) {
        vf1C(7);
        if (mTab == 2) {
            ItemParamEntry_unk* item = fn_80211480(m26110);
            if (item != 0) {
                fn_8058181C(1, 15, (int)item->mName);
                vf1C(2);
            }
        }
    }
}

// NONMATCHING: 99.88% - registers of the first inlined SetPaneText_unk (text and layout swapped)
void ShopUpGrade::fn_8058D16C() {
    fn_80597268(1);
    if (mCursors[mTab] < (int)mItems.size()) {
        fn_80597554(&mItems[mCursors[mTab]]);
    }
    if (mItems.size() != 0) {
        char name[0x40];
        int line = mCursors[mTab] - mTops[mTab];
        sprintf(name, "list_cursor%d", line + 1);
        ShopPoint_unk pos = GetPanePos_unk(mListBase, name);
        fn_804A55FC(MenuCommonTask::sInstance, mList[line], 0, &pos, 0);
    }
    mSort->mFlags &= ~4;
    ShopPoint_unk pos;
    pos.x = 515.0f;
    pos.y = 360.0f;
    pos.h = 1.0f;
    pos.w = 1.0f;
    fn_801F4728(mSort, "basePos", &pos);
    if (mTab == 1) {
        SetPaneText_unk(mSort->mLayout, "sortMainName", fn_80116FC0(0, 0x192));
        SetPaneText_unk(mSort->mLayout, "sortSubName", fn_80116FC0(0, 0x196));
    } else {
        SetPaneText_unk(mSort->mLayout, "sortMainName", fn_80116FC0(0, 0x191));
        SetPaneText_unk(mSort->mLayout, "sortSubName", fn_80116FC0(0, 0x196));
    }
    if (mSortOrder == 0) {
        SetPaneFloat_unk(mSort->mLayout, "sortID", 1.0f);
        SetPaneFloat_unk(mSort->mLayout, "sortSub", 0.0f);
    } else if (mSortOrder == 1) {
        SetPaneFloat_unk(mSort->mLayout, "sortID", 0.0f);
        SetPaneFloat_unk(mSort->mLayout, "sortSub", 1.0f);
    }
}

void ShopUpGrade::fn_8058D458() {
    int arg = 0;
    if (m26080 >= 0) {
        ShopD2A_unk* effect = mEffect[m26080];
        if (effect == 0 || effect->IsEnd_unk()) {
            if (mCursorAnime->mFrame <= 0.0f) {
                fn_8058DB10();
                mScroll.mCount = mItems.size();
                mScroll.mPage = 10;
                fn_80597098(m26108, m2610C, mTab);
                mScroll.mCursor = mCursors[mTab];
                mScroll.mTop = mTops[mTab];
            }
            char name[0x40];
            sprintf(name, "list_cursor%d", mCursors[mTab] - mTops[mTab] + 1);
            ShopPoint_unk pos = GetPanePos_unk(mListBase, name);
            mCursorAnime->mFlags &= ~4;
            SetPaneValue_unk(mCursorAnime->mLayout, "cursor_pos", 0, pos.x);
            SetPaneValue_unk(mCursorAnime->mLayout, "cursor_pos", 1, pos.y);
            SetPaneValue_unk(mCursorAnime->mLayout, "cursor_pos", 4, pos.z);
            SetPaneFloat_unk(mCursorAnime->mLayout, "wRate", pos.w);
            SetPaneFloat_unk(mCursorAnime->mLayout, "hRate", pos.h);
            if (mCursorAnime->IsEnd_unk()) {
                m26080 = -1;
            }
            arg = 1;
        } else if (effect != 0) {
            effect->mFlags &= ~4;
        }
    }
    fn_80597268(arg);
}

void ShopUpGrade::fn_8058D6D8() {
    if (mPrevState == 4) {
        mTop->mFlags &= ~4;
    } else if (mPrevState == 5) {
        fn_805807A4();
    } else if (mPrevState == 6) {
        fn_805807A4();
        if (m32FC != 0 && mDC < m32FC) {
            ShopEntry5C_unk* entry = &mEntries5C[mDC];
            fn_80581008(entry->m00);
            fn_805811DC(entry->m00);
            fn_80581570(entry->m00, entry->m48);
        }
        fn_805846C0(mA8, mAC[mDC - mE0], mDC, mE0, 1);
    } else {
        fn_80597268(1);
        if (mCursors[mTab] < (int)mItems.size()) {
            fn_80597554(&mItems[mCursors[mTab]]);
        }
    }
}

int ShopUpGrade::fn_8058D7F8(ShopUpGradeEntry_unk* entry) {
    ItemParamEntry_unk* item = fn_80211480(entry->mCode);
    ItemParamEntry_unk* next = fn_80211480(item->m134);
    if (entry->mHave == 0) {
        return 8;
    }
    if (entry->mCanUpgrade == 0) {
        return 7;
    }
    if (entry->mPrice > Inventory::instance_->mMoney) {
        return 1;
    }
    if (next == 0 && lbl_8087F430 != 0 && lbl_8087F430->fn_80370174(0x11B) > 0) {
        if (fn_804444E8(Inventory::instance_, 0x143) < 1) {
            return 14;
        }
        if (!fn_802124B4(item, 1)) {
            return 2;
        }
        if (fn_80444564(Inventory::instance_, item->mId + 1000000) >= 10) {
            return 2;
        }
    } else {
        for (int i = 0; i < 3; i++) {
            ItemParamEntry_unk* need = fn_80211480(item->m11C[i]);
            if (need != 0 && item->m11C[i] != 0 &&
                fn_804444E8(Inventory::instance_, need->mId) < (signed char)item->m12C[i]) {
                return 14;
            }
        }
        if (fn_80444564(Inventory::instance_, next->mId) >= 10) {
            return 2;
        }
    }
    return 0;
}

// NONMATCHING: 99.63% - registers: the next item is kept in r31 and the count of the needed item is loaded first
int ShopUpGrade::fn_8058D994(ShopUpGradeEntry_unk* entry, int give) {
    int code = 0;
    ItemParamEntry_unk* item = fn_80211480(entry->mCode);
    if (item != 0) {
        code = item->m134;
        ItemParamEntry_unk* next = fn_80211480(code);
        if (next == 0) {
            fn_8044441C(Inventory::instance_, 0x143, 1);
            fn_8044D6AC(Inventory::instance_, entry->mPrice);
            if (give) {
                fn_8058AABC();
                int upgraded = fn_80211940(item, 1);
                if (upgraded >= 0) {
                    Inventory::instance_->AddItem(upgraded, 1, 1, 0, 0, 1, 1);
                    code = upgraded;
                }
            }
        } else {
            for (int i = 0; i < 3; i++) {
                ItemParamEntry_unk* need = fn_80211480(item->m11C[i]);
                if (need != 0 && item->m11C[i] != 0) {
                    fn_8044441C(Inventory::instance_, need->mId, (signed char)item->m12C[i]);
                }
            }
            fn_8044D6AC(Inventory::instance_, entry->mPrice);
            if (give) {
                Inventory::instance_->AddItem(next->mId, 1, 1, 0, 0, 1, 1);
            } else {
                Inventory::instance_->fn_804438E0(next->mId);
            }
        }
        if (give) {
            fn_8044441C(Inventory::instance_, item->mId, 1);
        }
    }
    return code;
}

// inferred: whether the protector is dealt by this shop (inline, name invented)
inline int ShopUpGrade::IsTarget_unk(EquipParamEntry_unk* param) {
    int ok = 0;
    if (param != 0) {
        switch (mMode) {
        case 0:
        case 1:
            if (param->mA8 - 8 > 2u) {
                ok = 1;
            }
            break;
        case 2:
            if (param->mA8 - 9 <= 1u) {
                ok = 1;
            }
            break;
        case 3:
            if (param->mA8 == 8) {
                ok = 1;
            }
            break;
        }
    }
    return ok;
}

// inferred: iterator of the list given to std::sort; its members are not inlined in the big sort
// functions (0x8058ECD4-0x8058EEEC, 0x8058FE8C-0x8058FF08)
class ShopUpGradeIter_unk {
public:
    ShopUpGradeIter_unk(ShopUpGradeEntry_unk* const& p) : mPtr(p) {}
    ShopUpGradeIter_unk& operator=(const ShopUpGradeIter_unk& o) {
        mPtr = o.mPtr;
        return *this;
    }
    ShopUpGradeEntry_unk& operator*() const { return *mPtr; }
    ShopUpGradeIter_unk& operator++() {
        mPtr++;
        return *this;
    }
    ShopUpGradeIter_unk& operator--() {
        mPtr--;
        return *this;
    }
    ShopUpGradeEntry_unk* operator+(int n) const { return mPtr + n; }
    ShopUpGradeEntry_unk* operator-(int n) const { return mPtr + -n; }
    int operator-(const ShopUpGradeIter_unk& o) const { return mPtr - o.mPtr; }
    bool operator==(const ShopUpGradeIter_unk& o) const { return mPtr == o.mPtr; }
    bool operator!=(const ShopUpGradeIter_unk& o) const { return mPtr != o.mPtr; }
    bool operator<(const ShopUpGradeIter_unk& o) const { return mPtr < o.mPtr; }
    bool operator>=(const ShopUpGradeIter_unk& o) const { return mPtr >= o.mPtr; }

    ShopUpGradeEntry_unk* mPtr;
};

extern "C" int wcscmp(const wchar_t* a, const wchar_t* b);
extern "C" unsigned long wcslen(const wchar_t* s);
extern "C" wchar_t* wcschr(const wchar_t* s, wchar_t c);
extern "C" int fn_800DD3FC(wchar_t* out, const wchar_t* format, ...); // inferred: swprintf

// NONMATCHING: 99.81% (0x8058E5F8, 0x80591D88) - L"%s" is a weak @STRING@ here (inline member) and in the
// file pool in the original
// inferred: order of the list (std::sort predicates): the items with a row first; the same item by member
// (the bag last); then by the main parameter (higher first, by name when equal: A weapons, C protectors)
// or by name (without the "+n" of the upgrades, shorter first: B weapons, D protectors)
struct ShopUpGradeLessA_unk {
    bool operator()(const ShopUpGradeEntry_unk& a, const ShopUpGradeEntry_unk& b) const {
        WeaponParamEntry_unk* pa = fn_80206C50(a.mCode);
        WeaponParamEntry_unk* pb = fn_80206C50(b.mCode);
        if (pa != 0 && pb == 0) {
            return true;
        }
        if (pa == 0 && pb != 0) {
            return false;
        }
        if (a.mCode == b.mCode) {
            if (a.mMember >= 0 && b.mMember >= 0) {
                return a.mMember < b.mMember;
            }
            if (a.mMember >= 0) {
                return true;
            }
            return false;
        }
        if ((float)__fabs(pa->m00 - pb->m00) < 1.1920929e-07f) {
            return wcscmp(fn_80211480(a.mCode)->mName, fn_80211480(b.mCode)->mName) < 0;
        }
        return pa->m00 > pb->m00;
    }
};
struct ShopUpGradeLessB_unk {
    bool operator()(const ShopUpGradeEntry_unk& a, const ShopUpGradeEntry_unk& b) const {
        WeaponParamEntry_unk* pa = fn_80206C50(a.mCode);
        WeaponParamEntry_unk* pb = fn_80206C50(b.mCode);
        if (pa != 0 && pb == 0) {
            return true;
        }
        if (pa == 0 && pb != 0) {
            return false;
        }
        if (a.mCode == b.mCode) {
            if (a.mMember >= 0 && b.mMember >= 0) {
                return a.mMember < b.mMember;
            }
            if (a.mMember >= 0) {
                return true;
            }
            return false;
        }
        ItemParamEntry_unk* ia = fn_80211480(a.mCode);
        ItemParamEntry_unk* ib = fn_80211480(b.mCode);
        wchar_t na[0x40];
        fn_800DD3FC(na, L"%s", ia->mName);
        wchar_t* p = wcschr(na, L'+');
        if (p != 0) {
            *p = 0;
        }
        wchar_t nb[0x40];
        fn_800DD3FC(nb, L"%s", ib->mName);
        p = wcschr(nb, L'+');
        if (p != 0) {
            *p = 0;
        }
        if (wcscmp(na, nb) == 0) {
            int la = wcslen(ia->mName);
            int lb = wcslen(ib->mName);
            if (la != lb) {
                return la < lb;
            }
        }
        return wcscmp(ia->mName, ib->mName) < 0;
    }
};
struct ShopUpGradeLessC_unk {
    bool operator()(const ShopUpGradeEntry_unk& a, const ShopUpGradeEntry_unk& b) const {
        EquipParamEntry_unk* pa = fn_8020EFEC(a.mCode);
        EquipParamEntry_unk* pb = fn_8020EFEC(b.mCode);
        if (pa != 0 && pb == 0) {
            return true;
        }
        if (pa == 0 && pb != 0) {
            return false;
        }
        if (a.mCode == b.mCode) {
            if (a.mMember >= 0 && b.mMember >= 0) {
                return a.mMember < b.mMember;
            }
            if (a.mMember >= 0) {
                return true;
            }
            return false;
        }
        if ((float)__fabs(pa->m08 - pb->m08) < 1.1920929e-07f) {
            return wcscmp(fn_80211480(a.mCode)->mName, fn_80211480(b.mCode)->mName) < 0;
        }
        return pa->m08 > pb->m08;
    }
};
struct ShopUpGradeLessD_unk {
    bool operator()(const ShopUpGradeEntry_unk& a, const ShopUpGradeEntry_unk& b) const {
        EquipParamEntry_unk* pa = fn_8020EFEC(a.mCode);
        EquipParamEntry_unk* pb = fn_8020EFEC(b.mCode);
        if (pa != 0 && pb == 0) {
            return true;
        }
        if (pa == 0 && pb != 0) {
            return false;
        }
        if (a.mCode == b.mCode) {
            if (a.mMember >= 0 && b.mMember >= 0) {
                return a.mMember < b.mMember;
            }
            if (a.mMember >= 0) {
                return true;
            }
            return false;
        }
        ItemParamEntry_unk* ia = fn_80211480(a.mCode);
        ItemParamEntry_unk* ib = fn_80211480(b.mCode);
        wchar_t na[0x40];
        fn_800DD3FC(na, L"%s", ia->mName);
        wchar_t* p = wcschr(na, L'+');
        if (p != 0) {
            *p = 0;
        }
        wchar_t nb[0x40];
        fn_800DD3FC(nb, L"%s", ib->mName);
        p = wcschr(nb, L'+');
        if (p != 0) {
            *p = 0;
        }
        if (wcscmp(na, nb) == 0) {
            int la = wcslen(ia->mName);
            int lb = wcslen(ib->mName);
            if (la != lb) {
                return la < lb;
            }
        }
        return wcscmp(ia->mName, ib->mName) < 0;
    }
};

// inferred: std::swap of two lines (0x8058ED28)
inline void Swap_unk(ShopUpGradeEntry_unk& a, ShopUpGradeEntry_unk& b) {
    ShopUpGradeEntry_unk tmp = a;
    a = b;
    b = tmp;
}

// inferred: the reference type of the predicate (the recursion of std::sort uses its own copy of the
// function, with its own counter: 0x8058E0C8 / 0x8058E7A4)
template <class T>
struct ShopAddRef_unk {
    typedef T& type;
};
template <class T>
struct ShopAddRef_unk<T&> {
    typedef T& type;
};

// inferred: std::min_element (inlined in the selection sort)
template <class It, class Cmp>
inline It MinElement_unk(It first, It last, Cmp& comp) {
    if (first == last) {
        return first;
    }
    It result = first;
    while (++first != last) {
        if (comp(*first, *result)) {
            result = first;
        }
    }
    return result;
}

// inferred: selection sort of the short ranges (0x8058FAD0)
// NONMATCHING: 93.40% - the original keeps last - 1 and last on the stack
template <class It, class Cmp>
void SortSelect_unk(It first, It last, Cmp& comp) {
    if (first == last) {
        return;
    }
    It end = last - 1;
    for (; first != end; ++first) {
        It min = MinElement_unk(first, last, comp);
        if (min != first) {
            Swap_unk(*min, *first);
        }
    }
}

// inferred: puts the median of the three elements in *c (0x8058EF08)
// NONMATCHING: 99.1-99.9% - the wide literal L"%s" of the inlined predicates is a weak @STRING@ here and
// part of the file pool (@wstringBase0) in the original
template <class It, class Cmp>
void SortMedian_unk(It a, It b, It c, Cmp& comp) {
    bool x = !comp(*c, *a);
    bool y = !comp(*b, *c);
    if (x && y) {
        return;
    }
    if (!x && !y) {
        Swap_unk(*a, *b);
        return;
    }
    if (comp(*b, *a)) {
        Swap_unk(*a, *b);
    }
    if (x) {
        Swap_unk(*b, *c);
    } else {
        Swap_unk(*a, *c);
    }
}

// inferred: std::sort (quicksort on a median of three with a rotating offset, selection sort below 21)
// NONMATCHING: 96-98% (0x8058E0C8, 0x8058E7A4, 0x80591858, 0x80591F34, 0x805933E8, 0x805949B0) - scheduling of
// the two pivot offsets; with LessC_unk the predicate and the selection sort are inlined here while the
// original keeps them out of line (0x8058FF18 and 0x80590578 differ in size)
template <class It, class Cmp>
void Sort_unk(It first, It last, Cmp comp) {
    static int sOffset = -4;
    for (;;) {
        int n = last - first;
        if (n <= 1) {
            return;
        }
        if (n <= 20) {
            SortSelect_unk(first, last, comp);
            return;
        }
        It i = first + (n / 4 + sOffset % 5);
        if (++sOffset >= 5) {
            sOffset = -4;
        }
        It j = first + (3 * n / 4 + sOffset % 5);
        if (++sOffset >= 5) {
            sOffset = -4;
        }
        It pivot = last - 1;
        SortMedian_unk(i, j, pivot, comp);
        i = first;
        j = pivot;
        while (comp(*i, *pivot)) {
            ++i;
        }
        while (--j != i && !comp(*j, *pivot)) {
        }
        if (i < j) {
            Swap_unk(*i, *j);
            ++i;
            for (;;) {
                while (comp(*i, *pivot)) {
                    ++i;
                }
                while (!comp(*--j, *pivot)) {
                }
                if (i >= j) {
                    break;
                }
                Swap_unk(*i, *j);
                ++i;
            }
        }
        if (i == first) {
            Swap_unk(*i, *pivot);
            ++i;
            j = last;
            if (!comp(*first, *--j)) {
                while (i != last && !comp(*first, *i)) {
                    ++i;
                }
                if (i < j) {
                    Swap_unk(*i, *j);
                }
            }
            if (i < j) {
                for (;;) {
                    while (!comp(*first, *i)) {
                        ++i;
                    }
                    while (comp(*first, *--j)) {
                    }
                    if (i >= j) {
                        break;
                    }
                    Swap_unk(*i, *j);
                    ++i;
                }
            }
            first = i;
        } else if (i - first < last - i) {
            Sort_unk<It, typename ShopAddRef_unk<Cmp>::type>(first, i, comp);
            first = i;
        } else {
            Sort_unk<It, typename ShopAddRef_unk<Cmp>::type>(i, last, comp);
            last = i;
        }
    }
}

// inferred: std::sort(list.begin(), list.end(), Pred())
template <class Pred>
inline void SortList_unk(std::vector<ShopUpGradeEntry_unk>& list, Pred comp) {
    Sort_unk(ShopUpGradeIter_unk(list.begin()), ShopUpGradeIter_unk(list.end()), comp);
}

// NONMATCHING: 94.41% - the original reads the members of the local list through other address forms (lwzx) and repeats the null check of the protector
void ShopUpGrade::fn_8058DB10() {
    Inventory* party = Inventory::instance_;
    mItems.clear();
    atn::FixedArray_unk<int, 8> members;
    members.push_back(0);
    ShopMembers_unk* in = fn_80217D9C(lbl_8087F430->m10D0);
    if (in != 0) {
        for (unsigned int i = 1; i < 7; i++) {
            if (in->mIn[i] != 0) {
                members.push_back(i);
            }
        }
    }
    if (mTab == 0 || mTab == 2) {
        int kind = mTab == 2;
        for (int i = 0; i < members.size(); i++) {
            Inventory::Equip_unk* equip = &Inventory::instance_->mEquip[members[i]];
            for (int j = 0; j < 2; j++) {
                WeaponParamEntry_unk* param = fn_80206B14(0, equip->mWeapons[j].mId);
                if (members[i] == 0 && j == 1) {
                    param = fn_80206B14(1, equip->mWeapons[j].mId);
                }
                if (param != 0 && param->mF0 == kind) {
                    fn_805969D4(fn_80206BE4(param), members[i], j, 1);
                }
            }
        }
        for (int i = 0; i < 0x600; i++) {
            ItemParamEntry_unk* item = fn_802114E0(i);
            if (item != 0) {
                int code = item->mId;
                if (fn_80206B9C(code) && fn_8044453C(party, i) > 0) {
                    WeaponParamEntry_unk* param = fn_80206C50(code);
                    if (param != 0 && param->mF0 == kind) {
                        fn_805969D4(code, -1, -1, 1);
                    }
                }
            }
        }
    } else if (mTab == 1) {
        for (unsigned int i = 0; i < members.size(); i++) {
            Inventory::Equip_unk* equip = &Inventory::instance_->mEquip[members[i]];
            for (int j = 0; j < 2; j++) {
                EquipParamEntry_unk* param = fn_8020ED84(j, equip->mProtectors[j].mId);
                if (param != 0 && IsTarget_unk(param)) {
                    fn_805969D4(fn_8020EF80(param), members[i], j, 1);
                }
            }
        }
        for (int i = 0; i < 0x600; i++) {
            ItemParamEntry_unk* item = fn_802114E0(i);
            if (item != 0) {
                int code = item->mId;
                if (fn_8020EF04(code) && IsTarget_unk(fn_8020EFEC(code)) && fn_8044453C(party, i) > 0) {
                    fn_805969D4(code, -1, -1, 1);
                }
            }
        }
    }
    if (mTab == 0 || mTab == 2) {
        if (mSortOrder == 0) {
            SortList_unk(mItems, ShopUpGradeLessA_unk());
        } else {
            SortList_unk(mItems, ShopUpGradeLessB_unk());
        }
    } else if (mTab == 1) {
        if (mSortOrder == 0) {
            SortList_unk(mItems, ShopUpGradeLessC_unk());
        } else {
            SortList_unk(mItems, ShopUpGradeLessD_unk());
        }
    }
    if (mTab == 0) {
        int num = fn_8044D560(party);
        for (int i = 0; i < num; i++) {
            fn_805969D4(fn_8044D500(party, i), -1, -1, 0);
        }
    }
    fn_8058AABC();
}

// NONMATCHING: 90% - push_back: the original stores the entry by the POD path of MSL (size first), here the
// placement copy of a class with a constructor
void ShopUpGrade::fn_805969D4(int code, int member, int slot, int have) {
    ShopUpGradeEntry_unk entry;
    ItemParamEntry_unk* item = fn_80211480(code);
    if (item == 0) {
        return;
    }
    if (fn_80211480(item->m134) == 0) {
        int can = 0;
        if (lbl_8087F430 != 0 && lbl_8087F430->fn_80370174(0x11B) > 0) {
            can = 1;
        }
        if (fn_80206B9C(item->mId) && fn_80206C50(item->mId) != 0 && fn_80206C50(item->mId)->mKind != 0) {
            can = 0;
        }
        if (wcsstr(item->mName, L"+99") != 0) {
            can = 0;
        }
        if (can && have) {
            entry.mPrice = item->mC8 / 500 * 100 + 500;
            entry.mCanUpgrade = 1;
        } else {
            entry.mPrice = 0;
            entry.mCanUpgrade = 0;
        }
    } else if (!have) {
        entry.mPrice = 0;
        entry.mCanUpgrade = 0;
    } else {
        int debug = 0;
        if (lbl_8087F430 != 0 && lbl_8087F430->fn_80370174(0x11B) > 0) {
            debug = 1;
        }
        if (!debug && item->m11C[0] == 0x143) {
            entry.mPrice = 0;
            entry.mCanUpgrade = 0;
        } else {
            entry.mPrice = item->m130;
            if (lbl_8087F430 != 0) {
                entry.mPrice = entry.mPrice * (100 - lbl_8087F430->fn_80370174(0xDB)) / 100;
            }
            entry.mCanUpgrade = 1;
        }
    }
    if (lbl_8087F430->fn_80370174(0x11C)) {
        entry.mPrice = 0;
    }
    entry.mCode = code;
    entry.mMember = member;
    entry.mSlot = slot;
    entry.mHave = have;
    mItems.push_back(entry);
}

void ShopUpGrade::fn_80597098(int code, int member, int tab) {
    if (code < 0) {
        return;
    }
    int cursor = mCursors[tab];
    int top = mTops[tab];
    for (unsigned int i = 0; i < mItems.size(); i++) {
        ShopUpGradeEntry_unk& e = mItems[i];
        if (e.mCode == code && e.mMember == member) {
            cursor = i;
            break;
        }
    }
    if (cursor < top) {
        top = cursor;
    } else if (cursor >= top + 10) {
        top = cursor - 9;
    }
    if (cursor >= mScroll.mCount) {
        int last = mScroll.mCount - 1;
        cursor = last < 0 ? 0 : last;
    }
    if (cursor >= 10 && mScroll.mCount - 1 <= top + 9) {
        top = mScroll.mCount - 10;
    }
    mCursors[tab] = cursor;
    mTops[tab] = top;
}

void ShopUpGrade::fn_80597164(const ShopUpGradeEntry_unk& entry, int& cursor, int& top) {
    int offset = cursor - top;
    for (int i = 0; i < (int)mItems.size(); i++) {
        ShopUpGradeEntry_unk& e = mItems[i];
        if (e.mCode == entry.mCode &&
            ((e.mMember < 0 && entry.mMember < 0) || (e.mMember == entry.mMember && e.mSlot == entry.mSlot))) {
            cursor = i;
            int t = i - offset;
            top = t > 0 ? t : 0;
            if (mItems.size() < 10) {
                top = 0;
            }
            break;
        }
    }
    int size = mItems.size();
    if (cursor >= size) {
        int last = size - 1;
        cursor = last < 0 ? 0 : last;
    }
    if (size >= 10 && size - 1 <= top + 9) {
        top = size - 10;
    }
}

void ShopUpGrade::fn_80597268(int show) {
    Inventory* party = Inventory::instance_;
    mListBase->mFlags &= ~4;
    SetPaneFloat_unk(mListBase->mLayout, "ofsOn", 1.0f);
    mContent->mFlags &= ~4;
    ((atn::D2A_Task*)mContent)->SetNumber_unk("myMoney", Inventory::instance_->mMoney, 0);
    fn_80584750(mListBase, mScroll.mTop, mItems.size(), 10);
    ShopPoint_unk pos;
    for (int i = 0; i < 10; i++) {
        char name[0x40];
        sprintf(name, "list_point%d", i + 1);
        pos = GetPanePos_unk(mListBase, name);
        fn_801F6E78(mList[i], "basePos", &pos);
        int index = i + mScroll.mTop;
        if (index < (int)mItems.size()) {
            ShopUpGradeEntry_unk* entry = &mItems[index];
            ItemParamEntry_unk* item = fn_80211480(entry->mCode);
            int num = fn_804444E8(party, item->mId);
            if (show) {
                if (entry->mMember < 0) {
                    fn_801F6C80(mList[i], "faceOn", 0.0f);
                } else {
                    fn_804A4CE4(mList[i], entry->mMember, 4, 2);
                    fn_801F6C80(mList[i], "faceOn", 1.0f);
                    num = -1;
                }
                if (entry->mCanUpgrade != 0) {
                    fn_801F7DF0(mList[i], "itemCol", 255.0f, 255.0f, 255.0f);
                    fn_801F6C80(mList[i], "subColorR", 180.0f);
                    fn_801F6C80(mList[i], "subColorR", 180.0f);
                    fn_801F6C80(mList[i], "subColorR", 180.0f);
                } else {
                    fn_801F7DF0(mList[i], "itemCol", 128.0f, 128.0f, 128.0f);
                    fn_801F6C80(mList[i], "subColorR", 100.0f);
                    fn_801F6C80(mList[i], "subColorR", 100.0f);
                    fn_801F6C80(mList[i], "subColorR", 100.0f);
                }
                fn_80598530(mList[i], item, -1, num, -1, entry->mHave);
            } else {
                mList[i]->mFlags &= ~4;
            }
        }
    }
}

// NONMATCHING: 91.43% - the copy of the item row (mItem = *item) is member-wise here; the original copies
// 0x10-0x70 of the row as a block (layout of BulletParam_unk in db/ParseUtil_unk.h), plus registers
void ShopUpGrade::fn_80597554(ShopUpGradeEntry_unk* entry) {
    Inventory* party = Inventory::instance_;
    ((atn::D2A_Task*)mContent)->SetNumber_unk("upgradeMoney", entry->mPrice, 0);
    ShopPoint_unk pos;
    pos = GetPanePos_unk(mContent, "AfterItem");
    fn_801F6E78(mColor, "basePos", &pos);
    fn_801F6C80(mColor, "faceOn", 0.0f);
    fn_801F6E78(mColorNoItem, "basePos", &pos);
    char name[0x40];
    for (int i = 0; i < 2; i++) {
        sprintf(name, "skill_point%d", i + 1);
        pos = GetPanePos_unk(mContent, name);
        fn_801F6E78(mSkill[i], "basePos", &pos);
        fn_801F6E78(mSkillBlue[i], "basePos", &pos);
    }
    for (int i = 0; i < 3; i++) {
        sprintf(name, "NeedItem%d", i + 1);
        pos = GetPanePos_unk(mContent, name);
        fn_801F6E78(mColors[i], "basePos", &pos);
        fn_801F6C80(mColors[i], "faceOn", 0.0f);
    }
    ItemParamEntry_unk* item = fn_80211480(entry->mCode);
    ItemParamEntry_unk* next = fn_80211480(item->m134);
    int stone = 0;
    EquipParamEntry_unk* equip = 0;
    EquipParamEntry_unk* equip2 = 0;
    WeaponParamEntry_unk* weapon = 0;
    if (next != 0) {
        if (entry->mCanUpgrade != 0) {
            equip = fn_8020EFEC(next->mId);
            weapon = fn_80206C50(next->mId);
            if (equip != 0 && fn_8020EF4C(equip)) {
                equip2 = fn_8020ED84(1, equip->mId);
            }
        }
    } else if (entry->mCanUpgrade != 0 && lbl_8087F430 != 0 && lbl_8087F430->fn_80370174(0x11B) > 0) {
        mItem = *item;
        next = &mItem;
        if (fn_8020EF04(next->mId)) {
            equip = fn_8020EFEC(next->mId);
        }
        if (fn_80206B9C(next->mId)) {
            weapon = fn_80206C50(next->mId);
        }
        if (equip != 0) {
            fn_80212C34(&mEquipA, equip, 1);
            equip = &mEquipA;
        }
        if (weapon != 0) {
            fn_80212840(&mWeapon, weapon, 1);
            weapon = &mWeapon;
        }
        mItem.mId += 1000000;
        mItem.mName = fn_80211770(mItem.mId, -1);
        if (equip != 0 && fn_8020EF4C(equip)) {
            equip2 = fn_8020ED84(1, equip->mId);
        }
        stone = 1;
    }
    if (entry->mCanUpgrade != 0) {
        int have = fn_804444E8(party, next->mId) < 0 ? 0 : fn_804444E8(party, next->mId);
        fn_80598530(mColor, next, -1, have, -1, 1);
        fn_801F7DF0(mColor, "itemCol", 255.0f, 255.0f, 255.0f);
        if (stone) {
            ItemParamEntry_unk* need = fn_80211480(0x143);
            if (need != 0) {
                int num = fn_804444E8(party, need->mId);
                fn_80598530(mColors[0], need, -1, 1, num, 1);
                if (num >= 1) {
                    fn_801F7DF0(mColors[0], "itemCol", 255.0f, 255.0f, 255.0f);
                } else {
                    fn_801F7DF0(mColors[0], "itemCol", 128.0f, 128.0f, 128.0f);
                }
            }
        } else {
            for (int i = 0; i < 3; i++) {
                ItemParamEntry_unk* need = fn_80211480(item->m11C[i]);
                if (need != 0 && item->m11C[i] != 0) {
                    int num = fn_804444E8(party, need->mId);
                    fn_80598530(mColors[i], need, -1, (signed char)item->m12C[i], num, 1);
                    if ((signed char)item->m12C[i] <= num) {
                        fn_801F7DF0(mColors[i], "itemCol", 255.0f, 255.0f, 255.0f);
                    } else {
                        fn_801F7DF0(mColors[i], "itemCol", 128.0f, 128.0f, 128.0f);
                    }
                }
            }
        }
    } else {
        mColorNoItem->mFlags &= ~4;
    }
    for (int i = 0; i < 2; i++) {
        const void* skill = 0;
        if (equip != 0) {
            skill = &equip->mE4[i];
        } else if (weapon != 0) {
            skill = &weapon->mC4[i];
        }
        mSkillBlue[i]->mFlags &= ~4;
        wchar_t text[0x100];
        if (entry->mCanUpgrade != 0 && entry->mHave != 0 && skill != 0 && fn_8021C6A4(text, skill)) {
            mSkill[i]->mFlags &= ~4;
            fn_801F837C(mSkill[i], "skillName", text);
        } else {
            mSkillBlue[i]->mFrame = 0.0f;
        }
    }
    int isWeapon = 0;
    ShopParamHead_unk* base = 0;
    ShopParamHead_unk* upgraded = 0;
    ShopParamHead_unk* base2 = 0;
    ShopParamHead_unk* upgraded2 = 0;
    if (fn_80206B9C(item->mId)) {
        isWeapon = 1;
        WeaponParamEntry_unk* param = fn_80206C50(item->mId);
        base = (ShopParamHead_unk*)param;
        upgraded = (ShopParamHead_unk*)weapon;
        if (param != 0) {
            if (entry->mHave != 0) {
                fn_804A3C24(MenuCommonTask::sInstance, param->mText, 0);
            } else {
                MenuCommonTask* menu = MenuCommonTask::sInstance;
                fn_804A3C24(menu, fn_80116FC0(1, 0x15B), 0);
            }
        }
    } else if (fn_8020EF04(item->mId)) {
        isWeapon = 0;
        EquipParamEntry_unk* param = fn_8020EFEC(item->mId);
        base = (ShopParamHead_unk*)param;
        upgraded = (ShopParamHead_unk*)equip;
        if (fn_8020EF4C(param)) {
            base2 = (ShopParamHead_unk*)fn_8020ED84(1, param->mId);
            upgraded2 = (ShopParamHead_unk*)equip2;
        }
        if (param != 0) {
            if (entry->mHave != 0) {
                fn_804A3C24(MenuCommonTask::sInstance, param->mText, 0);
            } else {
                MenuCommonTask* menu = MenuCommonTask::sInstance;
                fn_804A3C24(menu, fn_80116FC0(1, 0x15B), 0);
            }
        }
    }
    int before[5];
    memset(before, 0, sizeof(before));
    int after[5];
    memset(after, 0, sizeof(after));
    if (base != 0) {
        before[0] = (int)base->m00;
        before[1] = (int)base->m08;
        before[2] = 0;
        before[3] = (int)base->m0C;
        before[4] = (int)base->m04;
    }
    if (base2 != 0) {
        before[0] += (int)base2->m00;
        before[1] += (int)base2->m08;
        before[3] += (int)base2->m0C;
        before[4] += (int)base2->m04;
    }
    int diff[5];
    memset(diff, 0, sizeof(diff));
    if (base != 0 && upgraded != 0) {
        diff[0] = (int)upgraded->m00 - (int)base->m00;
        diff[1] = (int)upgraded->m08 - (int)base->m08;
        diff[2] = 0;
        diff[3] = (int)upgraded->m0C - (int)base->m0C;
        diff[4] = (int)upgraded->m04 - (int)base->m04;
        if (base2 != 0 && upgraded2 != 0) {
            diff[0] += (int)upgraded2->m00 - (int)base2->m00;
            diff[1] += (int)upgraded2->m08 - (int)base2->m08;
            diff[3] += (int)upgraded2->m0C - (int)base2->m0C;
            diff[4] += (int)upgraded2->m04 - (int)base2->m04;
        }
        for (int k = 0; k < 5; k++) {
            after[k] = before[k] + diff[k];
        }
    }
    for (int i = 0; i < 3; i++) {
        int k = isWeapon ? sParams.mWeapon[i] : sParams.mArmor[i];
        ShopLayout_unk* arrow = 0;
        sprintf(name, "param%02d", i);
        SetPaneText_unk(mContent->mLayout, name, fn_80116FC0(0, sParams.mMsg[k]));
        if (entry->mHave != 0) {
            sprintf(name, "param%02d_color", i);
            if (diff[k] > 0) {
                arrow = mStatusUp[i];
                ((atn::D2A_Task*)mContent)->SetColor_unk(name, lbl_807C7898.r, lbl_807C7898.g, lbl_807C7898.b);
            } else if (diff[k] == 0) {
                ((atn::D2A_Task*)mContent)->SetColor_unk(name, lbl_807C78C8.r, lbl_807C78C8.g, lbl_807C78C8.b);
            } else {
                arrow = mStatusDown[i];
                ((atn::D2A_Task*)mContent)->SetColor_unk(name, lbl_807C78A8.r, lbl_807C78A8.g, lbl_807C78A8.b);
            }
            sprintf(name, "param%02d_before", i);
            ((atn::D2A_Task*)mContent)->SetNumber_unk(name, before[k], 0);
            sprintf(name, "param%02d_after", i);
            if (upgraded != 0) {
                ((atn::D2A_Task*)mContent)->SetNumber_unk(name, after[k], 0);
            } else {
                SetPaneText_unk(mContent->mLayout, name, L"");
            }
        } else {
            sprintf(name, "param%02d_color", i);
            ((atn::D2A_Task*)mContent)->SetColor_unk(name, lbl_807C78C8.r, lbl_807C78C8.g, lbl_807C78C8.b);
            sprintf(name, "param%02d_before", i);
            SetPaneText_unk(mContent->mLayout, name, L"???");
            sprintf(name, "param%02d_after", i);
            SetPaneText_unk(mContent->mLayout, name, L"???");
        }
        if (arrow != 0) {
            char posName[0x40];
            sprintf(posName, "param%02d_updw_pos", i);
            ShopPoint_unk arrowPos = GetPanePos_unk(mContent, posName);
            arrow->mFlags &= ~4;
            fn_801F6E78(arrow, "basePos", &arrowPos);
        }
    }
    if (lbl_8087F0A8->m488 != 0) {
        fn_804A7DD4(MenuCommonTask::sInstance, entry->mCode);
    }
}

// NONMATCHING: 95.96% - the original reads the message text twice (two address forms, as in
// ShopIdentify::fn_8058A0F0) and the wide literal offsets differ (L"%s" of the sort code is not written)
void ShopUpGrade::fn_80598530(ShopLayout_unk* layout, ItemParamEntry_unk* item, int price, int need, int num,
                              int have) {
    layout->mFlags &= ~4;
    int icon = 0;
    if (item != 0) {
        if (have == 0) {
            WeaponParamEntry_unk* weapon = fn_80206C50(item->mId);
            if (weapon != 0) {
                fn_801F837C(layout, "itemName", lbl_8087F1E4->GetPair_unk(weapon->mType + 0xEB));
            } else {
                fn_801F837C(layout, "itemName", L"--");
            }
            fn_801F837C(layout, "needNum", L"-");
            fn_801F6C80(layout, "showNum2", 0.0f);
            fn_801F837C(layout, "num", L"");
            icon = fn_80444C48(Inventory::instance_);
        } else {
            fn_801F837C(layout, "itemName", item->mName);
            if (need < 0) {
                fn_801F837C(layout, "needNum", L"-");
            } else {
                fn_801F8598(layout, "needNum", need, 0);
            }
            if (num < 0) {
                fn_801F6C80(layout, "showNum2", 0.0f);
                fn_801F837C(layout, "num", L"");
            } else {
                fn_801F6C80(layout, "showNum2", 1.0f);
                fn_801F8598(layout, "num", num, 0);
            }
            icon = fn_80444BE8(Inventory::instance_, item);
        }
    }
    if (price < 0) {
        fn_801F6C80(layout, "showPrice", 0.0f);
        fn_801F837C(layout, "price", L"");
    } else {
        fn_801F6C80(layout, "showPrice", 1.0f);
        fn_801F8598(layout, "price", price, 0);
    }
    ShopUV_unk uv = fn_80444C50(Inventory::instance_, icon);
    fn_801F7590(layout, "iconUv", uv);
}

void ShopUpGrade::vf40() {
    if (mState == 7) {
        fn_8058DB10();
        mScroll.mCount = mItems.size();
        if (mScroll.mCursor >= mScroll.mCount) {
            int last = mScroll.mCount - 1;
            mScroll.mCursor = last < 0 ? 0 : last;
        }
        if (mScroll.mCursor >= mScroll.mPage && mScroll.mCount - 1 <= mScroll.mTop + mScroll.mPage - 1) {
            mScroll.mTop = mScroll.mCount - mScroll.mPage;
        }
    } else if (mState == 5) {
        fn_80581FD8();
    }
}

int ShopUpGrade::vf3C() {
    return 2;
}
