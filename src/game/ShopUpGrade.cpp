#include "game/ShopUpGrade.h"
#include "game/GameTask.h"
#include "game/Inventory.h"
#include "game/MenuCommonTask.h"
#include "game/CharaTask.h"
#include "chr/CharaEquip_unk.h"
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
extern "C" void* lbl_8087F048; // inferred: effect manager of the characters
extern "C" void fn_8010C00C(void* mgr, CharaTask* chara);
extern "C" bool fn_802124B4(ItemParamEntry_unk* item, int level); // inferred: the item can be upgraded
extern "C" int fn_80211940(ItemParamEntry_unk* item, int level);   // inferred: makes the upgraded item
extern "C" int fn_80444564(Inventory* party, int id); // inferred: number of equipment items
extern "C" void fn_8044441C(Inventory* party, int id, int num); // inferred: removes items
extern "C" void fn_8044D6AC(Inventory* party, int money);       // inferred: pays money
// inferred: party members available at a progress (as game/ShopBuySell.cpp)
struct ShopMembers_unk {
    unsigned char m00[0x0C];
    unsigned char mIn[8];
};
extern "C" ShopMembers_unk* fn_80217D9C(int progress);

// inferred: predicates of the std::sort of the list (by kind and by name, for weapons and protectors)
struct ShopUpGradeLessA_unk {
    unsigned char m00;
};
struct ShopUpGradeLessB_unk {
    unsigned char m00;
};
struct ShopUpGradeLessC_unk {
    unsigned char m00;
};
struct ShopUpGradeLessD_unk {
    unsigned char m00;
};
// inferred: the std::sort instantiations of this unit (their template internals are not reconstructed)
extern "C" void fn_805933E8(ShopUpGradeEntry_unk** first, ShopUpGradeEntry_unk** last, ShopUpGradeLessA_unk* comp);
extern "C" void fn_80591858(ShopUpGradeEntry_unk** first, ShopUpGradeEntry_unk** last, ShopUpGradeLessB_unk* comp);
extern "C" void fn_8058FF18(ShopUpGradeEntry_unk** first, ShopUpGradeEntry_unk** last, ShopUpGradeLessC_unk* comp);
extern "C" void fn_8058E0C8(ShopUpGradeEntry_unk** first, ShopUpGradeEntry_unk** last, ShopUpGradeLessD_unk* comp);

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

// inferred: std::sort(list.begin(), list.end(), Pred()) (the arguments are evaluated from the last one)
template <class Pred>
inline void SortList_unk(std::vector<ShopUpGradeEntry_unk>& list, Pred comp,
                         void (*sort)(ShopUpGradeEntry_unk**, ShopUpGradeEntry_unk**, Pred*)) {
    ShopUpGradeEntry_unk* last = list.end();
    ShopUpGradeEntry_unk* first = list.begin();
    sort(&first, &last, &comp);
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
            SortList_unk(mItems, ShopUpGradeLessA_unk(), fn_805933E8);
        } else {
            SortList_unk(mItems, ShopUpGradeLessB_unk(), fn_80591858);
        }
    } else if (mTab == 1) {
        if (mSortOrder == 0) {
            SortList_unk(mItems, ShopUpGradeLessC_unk(), fn_8058FF18);
        } else {
            SortList_unk(mItems, ShopUpGradeLessD_unk(), fn_8058E0C8);
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

// NONMATCHING: 86.64% - the inlined vector growth moves the old elements with a word loop instead of the unrolled copy of the original
void ShopUpGrade::fn_805969D4(int code, int member, int slot, int have) {
    ShopUpGradeEntry_unk entry;
    entry.mCode = 0;
    entry.mPrice = 0;
    memset(entry.m08, 0, sizeof(entry.m08));
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
