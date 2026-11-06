#include "game/NewCampPartsItem.h"
#include "atn/Memory.h"
#include "atn/D3Anime.h"
#include "atn/D3AnimeInstance.h"
#include "std/string_members.h"
#include "atn/SoundRef.h"
#include "atn/Lerp_unk.h"
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
extern "C" NewCampItemD2A_unk* fn_801F3FF8(atn::Task* parent, const char* path, int loop);
extern "C" int sprintf(char* s, const char* format, ...);
extern "C" void fn_801F6E78(atn::Task* task, const char* name, const void* pos);
extern "C" void fn_801F6C80(atn::Task* task, const char* name, float value); // inferred: sets a value
extern "C" void fn_804A4930(atn::Task* task, int top, int count, int page); // inferred: scroll bar
extern "C" NewCampPartsPoint_unk fn_801F4E8C(atn::Task* layout, unsigned int hash);
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
extern "C" void fn_801FEE08(void* layout, unsigned int hash, const wchar_t* text); // inferred: sets a text
extern "C" void fn_801FECE0(void* layout, unsigned int hash, float value);
extern "C" void fn_801F4728(atn::Task* task, const char* name, const NewCampPartsPoint_unk* rect); // inferred: sets a rect
extern "C" float fn_804A4264(MenuCommonTask* menu); // inferred: position of the help window

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
// inferred: offset of the list in the camp state 8 (constructed by __sinit 0x8051B900)
static atn::Vector3 sOffset(0.0f, 340.0f, 0.0f);

// inferred: camp task (the parent m48); only the members used here
struct NewCampItemCamp_unk {
    unsigned char m0000[0x464];
    int m464;                  // inferred: state of the camp
    unsigned char m0468[0x11F8 - 0x468];
    int m11F8;
};

inline NewCampItemD2A_unk* GetD2A_unk(atn::D3Anime* anime) {
    return (NewCampItemD2A_unk*)anime->GetTask();
}
inline NewCampPartsPoint_unk GetPanePos_unk(atn::Task* layout, const char* name) {
    return fn_801F4E8C(layout, fn_800DC6B4(name));
}
inline void SetPaneText_unk(void* layout, const char* name, const wchar_t* text) {
    fn_801FEE08(layout, fn_800DC6B4(name), text);
}
// inferred: sets the text of a pane of a task (the layout is taken inside)
inline void SetTaskText_unk(NewCampItemD2A_unk* task, const char* name, const wchar_t* text) {
    void* layout = task->mLayout;
    fn_801FEE08(layout, fn_800DC6B4(name), text);
}
inline void SetPaneFloat_unk(void* layout, const char* name, float value) {
    fn_801FECE0(layout, fn_800DC6B4(name), value);
}

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

void NewCampPartsItem::vf14() {
    int show = 0;
    if (fn_8050FD20() && ((NewCampItemCamp_unk*)m48)->m11F8 != 1) {
        show = 1;
    }
    vf3C(show);
    NewCampItemCamp_unk* camp = (NewCampItemCamp_unk*)m48;
    atn::Vector3 target = camp->m464 == 8 ? sOffset : lbl_807C7030;
    mMove = atn::Lerp_unk<atn::Vector3>(mMove, target, 0.2f);
    for (int i = 0; i < m4C; i++) {
        NewCampPartsEntry_unk* entry = &m50[i];
        vf60(entry, 0, 7, 0.0f);
        if (GetModel_unk(entry->mModel) != 0) {
            atn::Matrix3x4 mtx = GetModel_unk(entry->mModel)->mMtx;
            atn::Vector3 pos = mtx.GetTrans_unk() + mMove;
            mtx.m[0][3] = pos.x;
            mtx.m[1][3] = pos.y;
            mtx.m[2][3] = pos.z;
            GetModel_unk(entry->mModel)->fn_8009EE30(mtx);
        }
        vf58(entry, "reference");
        vf54(entry, "reference");
        vf5C(entry);
    }
    char name[0x40];
    NewCampPartsPoint_unk pos;
    pos.x = 0.0f;
    pos.y = 0.0f;
    pos.m08 = 0.0f;
    pos.m0C = 0.0f;
    pos.m10 = 0.0f;
    NewCampPartsEntry_unk* cat = &m50[mE0 + 5];
    for (int i = 0; i < 7; i++) {
        int index = i + 5;
        NewCampPartsEntry_unk* entries = m50.mData;
        if (entries[index].mAnime->GetTask() != 0) {
            if (mE0 == i) {
                GetD2A_unk(entries[index].mAnime)->mFrame = 11.0f;
                if (show) {
                    int mode = m7C != 0;
                    sprintf(name, "cursor_point");
                    pos = GetPanePos_unk(cat->mAnime->GetTask(), name);
                    fn_805118B4(cat->mAnime->GetTask(), 0, cat, pos, mode);
                }
            } else {
                GetD2A_unk(entries[index].mAnime)->mFrame = 10.0f;
            }
        }
    }
    fn_80511258(&m50[1], mBase, atn::Vector3(0.0f, 0.0f, 0.0f), atn::Vector3(1.0f, 1.0f, 1.0f), "reference");
    fn_80511258(&m50[1], mNoItem, atn::Vector3(0.0f, 0.0f, 0.0f), atn::Vector3(1.0f, 1.0f, 1.0f), "reference");
    fn_805112A8(&m50[1], mScroll, atn::Vector3(0.0f, 0.0f, 0.0f), atn::Vector3(1.0f, 1.0f, 1.0f), "reference");
    if (mBase->GetTask() != 0) {
        for (int i = 0; i < 10; i++) {
            if (mItems[i]->GetTask() != 0) {
                sprintf(name, "item_list%d", i + 1);
                pos = GetPanePos_unk(mBase->GetTask(), name);
                fn_801F6E78(mItems[i]->GetTask(), "base_point", &pos);
                fn_805112A8(&m50[1], mItems[i], atn::Vector3(0.0f, 0.0f, 0.0f), atn::Vector3(1.0f, 1.0f, 1.0f),
                            "reference");
            }
        }
        if (m7C == 1) {
            int index = mE4 - mE8;
            sprintf(name, "item_cursor%d", index + 1);
            pos = GetPanePos_unk(mBase->GetTask(), name);
            fn_805119C8(mItems[index]->GetTask(), 0, &m50[1], pos, 0);
        }
        if (mScroll->GetTask() != 0) {
            sprintf(name, "scroll_point");
            pos = GetPanePos_unk(mBase->GetTask(), name);
            fn_801F6E78(mScroll->GetTask(), "basePos", &pos);
            fn_801F6C80(mScroll->GetTask(), "s_hightMax", 245.0f);
            fn_804A4930(mScroll->GetTask(), mE8, mList.size(), 10);
        }
    }
    if (mSort != 0) {
        mSort->FreezeTask();
        if (m7C == 1 && fn_804A4264(MenuCommonTask::sInstance) >= -5.0f) {
            mSort->MeltTask();
            pos.x = 515.0f;
            pos.y = 360.0f;
            pos.m0C = 1.0f;
            pos.m08 = 1.0f;
            fn_801F4728(mSort, "basePos", &pos);
            SetTaskText_unk(mSort, "sortMainName", fn_80116FC0(0, 0x196));
            SetTaskText_unk(mSort, "sortSubName", fn_80116FC0(0, 0x197));
            if (mEC == 0) {
                SetPaneFloat_unk(mSort->mLayout, "sortID", 1.0f);
                SetPaneFloat_unk(mSort->mLayout, "sortSub", 0.0f);
            } else if (mEC == 1) {
                SetPaneFloat_unk(mSort->mLayout, "sortID", 0.0f);
                SetPaneFloat_unk(mSort->mLayout, "sortSub", 1.0f);
            }
        }
    }
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

void NewCampPartsItem::fn_8051B364(const NewCampItemEntry_unk& entry, int* cursor, int* top) {
    int offset = *cursor - *top;
    for (int i = 0; i < (int)mList.size(); i++) {
        NewCampItemEntry_unk& e = mList[i];
        if (e.mId == entry.mId && e.m08 == entry.m08 && e.m0C == entry.m0C) {
            *cursor = i;
            int t = i - offset;
            *top = t > 0 ? t : 0;
            if (mList.size() < 10) {
                *top = 0;
            }
        }
    }
    int count = mList.size();
    if (*cursor >= count) {
        int c = count - 1;
        *cursor = c < 0 ? 0 : c;
    }
    if (count >= 10 && count - 1 <= *top + 9) {
        *top = count - 10;
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
