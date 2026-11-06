#include "game/NewCampPartsItem.h"
#include "atn/Memory.h"
#include "atn/D3Anime.h"
#include "atn/D3AnimeInstance.h"
#include "std/string_members.h"
#include "atn/SoundRef.h"
#include "game/MenuCommonTask.h"
#include "game/GameConfig_unk.h"

extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
extern "C" void* lbl_8087EF70;                                // inferred: pad
extern "C" int fn_800A555C(void* pad, int port, int button); // inferred: button triggered
extern "C" void fn_804A436C(int* cursor, int count, int wrap, int step, int arg); // inferred: moves a cursor
extern "C" atn::SoundRef fn_80117228(int id);                // inferred: plays a system sound effect
extern "C" void fn_8052DEEC(atn::Task* camp, int id);        // inferred: changes the parts of the camp
extern "C" void fn_804A4494(int* cursor, int* top, int count, int page, int a, int b); // inferred: moves a list cursor
extern "C" const wchar_t* fn_80116FC0(int a, int id);       // inferred: message text
extern "C" void fn_804A3C24(MenuCommonTask* menu, const wchar_t* text, int arg); // inferred: help text
extern "C" void fn_804A7DD4(MenuCommonTask* menu, int id);   // inferred: shows the item
extern "C" GameConfig_unk* lbl_8087F0A8;
// inferred: item data returned by fn_80211480 (only the members used here)
struct NewCampItemData_unk {
    unsigned char m00[0x0C];
    const wchar_t* mHelp; // 0x0C
};
extern "C" NewCampItemData_unk* fn_80211480(int id); // inferred: item data
// inferred: creates a d2anime task (atn/D2A_Task.h declares it with the D2A_Task type; not included: its
// D2A_Data.h gives a warning)
extern "C" atn::Task* fn_801F3FF8(atn::Task* parent, const char* path, int loop);

static const NewCampPartsFile_unk sFiles[12] = {
    {"shop_board01_0001", "item_logo.d2a", 0},
    {"item_board01_0001", "", 0},
    {"shop_board03_1001", "item_sort.d2a", 0},
    {"protector_in_board18_1001", "", 0},
    {"protector_in_board18_1002", "", 0},
    {"shop_board04_1001", "item_menu.d2a", 0},
    {"shop_board04_1002", "item_menu.d2a", 0},
    {"shop_board04_1003", "item_menu.d2a", 0},
    {"shop_board04_1004", "item_menu.d2a", 0},
    {"shop_board04_1005", "item_menu.d2a", 0},
    {"shop_board04_1006", "item_menu.d2a", 0},
    {"shop_board04_1007", "item_menu_all.d2a", 0},
};
static const char* const sBoard = "item_board";

NewCampPartsItem* NewCampPartsItem::fn_805169F8(atn::Task* parent) {
    if (parent != 0) {
        return new ((MemoryTag)1, "", "", 0) NewCampPartsItem(parent);
    }
    return 0;
}

NewCampPartsItem::NewCampPartsItem(atn::Task* parent)
    : NewCampPartsBase(parent), mE0(0), mEC(0), mBase(0), mNoItem(0), mScroll(0), mMove(0.0f, 0.0f, 0.0f) {
    m98 = "item_board";
    mE4 = 0;
    mE8 = 0;
    m4C = 12;
    if (m4C > 0) {
        m50.Reset_unk(new ((MemoryTag)1, "", "", 0) NewCampPartsEntry_unk[m4C]);
        fn_8050FB38(m4C, &m50[0], sFiles, 0, 0);
        fn_8050FC88(m4C, &m50[0], 1);
    }
    m5C.vf0C("camera/ui/item_camera.xsca");
    mList.clear();
    (mBase = atn::D3Anime::Create(this, "/data/d2anime/item_list_base.d2a", 1))->fn_800D246C(1);
    (mNoItem = atn::D3Anime::Create(this, "/data/d2anime/shop_text_noitem.d2a", 1))->fn_800D246C(1);
    for (int i = 0; i < 10; i++) {
        mItems[i] = atn::D3AnimeInstance::Create(this, "/data/d2anime/item_list.d2a", 1);
        mItems[i]->fn_800D246C(1);
    }
    (mScroll = atn::D3AnimeInstance::Create(this, "scroll_bar.d2a", 1))->fn_800D246C(1);
    (mSort = fn_801F3FF8(this, "/data/d2anime/camp_sort.d2a", 0))->fn_800D246C(1);
}

NewCampPartsItem::~NewCampPartsItem() {}

int NewCampPartsItem::vf4C() {
    return 0;
}

void NewCampPartsItem::vf1C(NewCampScene_unk* scene) {
    fn_8050FD38(scene, m50.mData, sFiles, m4C, sBoard);
    m80 = 0;
    m84 = 7;
    m88 = 0;
    m8C = 7;
    mE4 = 0;
    mE8 = 0;
    mBase->fn_800D246C(0);
    mNoItem->fn_800D246C(0);
    for (int i = 0; i < 10; i++) {
        mItems[i]->fn_800D246C(0);
    }
    mScroll->fn_800D246C(0);
    mSort->fn_800D246C(0);
}

void NewCampPartsItem::vf10() {
    vf20();
}

void NewCampPartsItem::vf24() {
    m96 = 1;
    mEC = 0;
    m80 = 0;
    mE0 = 0;
    mE4 = 0;
    mE8 = 0;
    fn_80517B3C();
    fn_8051B164();
}

void NewCampPartsItem::vf28() {
    m58 = -1.0f;
    mList.clear();
    m7C = 0;
    fn_8051B164();
    mEC = 0;
    mMove = lbl_807C7030;
    m96 = 0;
}

// NONMATCHING: offset and entry registers swapped (r6/r7) in the loop; tried the forms of
// NewCampPartsHint/Status/Option::vf3C
void NewCampPartsItem::vf3C(int show) {
    for (int i = 0; i < m4C; i++) {
        NewCampPartsEntry_unk* entry = &m50[i];
        if (entry->m3C != 0) {
            if (entry->mAnime != 0) {
                entry->mAnime->mVisible = show;
            }
            if (m50[i].mModel != 0) {
                m50[i].mModel->mHidden = show == 0;
            }
        }
    }
}

void NewCampPartsItem::vf2C() {
    switch (m7C) {
    case 0:
        fn_8051B43C();
        break;
    case 1:
        fn_8051B544();
        break;
    }
}

void NewCampPartsItem::vf30() {
    m80 = 0;
    mE0 = 0;
    mE4 = 0;
    mE8 = 0;
    fn_80517B3C();
    fn_8051B164();
}

// inferred: changes the state (the list is drawn again in state 0); name invented
inline void NewCampPartsItem::SetState_unk(int state) {
    m7C = state;
    if (m7C == 0) {
        fn_8051B164();
    }
}

void NewCampPartsItem::fn_8051B43C() {
    void* pad = lbl_8087EF70;
    int prev = mE0;
    fn_804A436C(&mE0, 7, 7, 1, 4);
    if (prev != mE0) {
        mE8 = 0;
        mE4 = 0;
        fn_80517B3C();
        fn_8051B164();
    }
    if (fn_800A555C(pad, 0, 4) != 0) {
        if (mList.size() != 0) {
            SetState_unk(1);
            fn_80117228(1);
        }
    } else if (fn_800A555C(pad, 0, 5) != 0) {
        fn_8052DEEC(m48, 0);
        fn_80117228(8);
    }
}

// inferred: help text and item view of an entry; name invented
static inline void ShowHelp_unk(NewCampItemEntry_unk* entry) {
    if (entry->m08 != 0) {
        NewCampItemData_unk* data = fn_80211480(entry->mId);
        if (data != 0) {
            fn_804A3C24(MenuCommonTask::sInstance, data->mHelp, 0);
        }
    } else {
        MenuCommonTask* menu = MenuCommonTask::sInstance;
        fn_804A3C24(menu, fn_80116FC0(1, 0x15B), 0);
    }
    if (lbl_8087F0A8->mShowReleadBar2 != 0) {
        fn_804A7DD4(MenuCommonTask::sInstance, entry->mId);
    }
}

void NewCampPartsItem::fn_8051B544() {
    void* pad = lbl_8087EF70;
    int prev = mE4;
    if (mList.size() != 0) {
        fn_804A4494(&mE4, &mE8, mList.size(), 10, 1, 3);
        if (prev != mE4) {
            fn_8051B164();
        }
    }
    if (fn_800A555C(pad, 0, 5) != 0) {
        SetState_unk(0);
        fn_80117228(2);
    } else if (fn_800A555C(pad, 0, 12) != 0) {
        fn_80117228(18);
        mEC = 0;
        NewCampItemEntry_unk entry = mList[mE4];
        fn_80517B3C();
        fn_8051B364(entry, &mE4, &mE8);
        fn_8051B164();
    } else if (fn_800A555C(pad, 0, 13) != 0) {
        fn_80117228(18);
        mEC = 1;
        NewCampItemEntry_unk entry = mList[mE4];
        fn_80517B3C();
        fn_8051B364(entry, &mE4, &mE8);
        fn_8051B164();
    }
    if (mE4 < (int)mList.size()) {
        ShowHelp_unk(&mList[mE4]);
    }
}
