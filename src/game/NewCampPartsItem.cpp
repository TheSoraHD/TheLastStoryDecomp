#include "game/NewCampPartsItem.h"
#include "atn/Memory.h"
#include "atn/D3Anime.h"
#include "atn/D3AnimeInstance.h"
#include "std/string_members.h"

extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
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
