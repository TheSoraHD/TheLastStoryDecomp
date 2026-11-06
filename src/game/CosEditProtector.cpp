#include "game/CosEditProtector.h"
#include "game/CostumeEditChara.h"
#include "game/CharaTask.h"
#include "game/Inventory.h"
#include "game/MenuCommonTask.h"
#include "game/CharaStatus.h"
#include "db/EquipParam_unk.h"
#include "math/Vec.h"
#include "atn/D3Anime.h"
#include "atn/D3AnimeInstance.h"
#include "atn/D2A_Task.h"
#include "atn/SoundRef.h"
#include "std/string_members.h"

// First file of CosEditProtector (game/CosEditPalette_unk.cpp has the palette and part pages).
extern "C" int sprintf(char* s, const char* format, ...);
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
struct Field_unk;
extern "C" Field_unk* lbl_8087F430; // inferred: field
extern "C" int fn_80370174(Field_unk* field, int id); // inferred: event flag
extern "C" void fn_80370320(Field_unk* field, int id, int a, int b); // inferred: sets an event flag
extern "C" void fn_8052DEEC(atn::Task* camp, int id); // inferred: changes the parts of the camp
extern "C" int fn_80117214(int index); // inferred: palette slot of a colour index
extern "C" int fn_8020BE40(int id);    // inferred: cell of a palette colour
extern "C" int fn_8020BF20(int mark);  // inferred: cell of a colour mark
// inferred: item data (fn_80211480)
struct CosEditItemData_unk {
    unsigned char m00[0x08];
    const wchar_t* mName; // 0x08
};
extern "C" CosEditItemData_unk* fn_80211480(int id);
extern "C" int fn_80206BE4(void* item);                   // inferred: item id
extern "C" int fn_8020EF80(EquipParamEntry_unk* entry);    // inferred: item id of an equipment entry
extern "C" int fn_80444B64(Inventory* inv, int id);       // inferred: icon of an item
extern "C" int fn_80444BE8(Inventory* inv, CosEditItemData_unk* item);
extern "C" int fn_80444C48(Inventory* inv);
extern "C" void fn_801FEE08(void* layout, unsigned int hash, const wchar_t* text); // inferred: sets a text
inline void SetPaneText_unk(void* layout, const char* name, const wchar_t* text) {
    fn_801FEE08(layout, fn_800DC6B4(name), text);
}
// inferred: wide literals of this file kept in .sdata (L"", L"???", four U+2015 bars), as in
// game/ShopIdentify.cpp
extern "C" const wchar_t lbl_8087DA58[12];
// inferred: texture rectangle (as ShopUV_unk in game/ShopIdentify.cpp)
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
extern "C" CosEditUV_unk fn_80444C50(Inventory* inv, int icon);
inline void SetUV_unk(atn::D2A_Task* task, const char* name, CosEditUV_unk uv) {
    task->SetParam2_unk(name, &uv.u0);
}
extern "C" const char* fn_8020BD78(int index); // inferred: name of a colour
extern "C" int fn_8020BDE0(int index);         // inferred: palette id of a colour cell
extern "C" int fn_8020BE10(int index);         // inferred: part id of a part cell
extern "C" void fn_800D2338(atn::Task* task);  // inferred: deletes a task
extern "C" atn::Vector3 lbl_807C7030;          // inferred: zero vector
extern "C" void* lbl_8087EF70;                                // inferred: pad
extern "C" int fn_800A555C(void* pad, int port, int button); // inferred: button triggered
extern "C" int fn_800A58D0(void* pad, int port);              // inferred: second stick available
extern "C" atn::SoundRef fn_80117228(int id);                 // inferred: plays a system sound effect
extern "C" void fn_801FECE0(void* layout, unsigned int hash, float value);
extern "C" void fn_801F6C80(atn::Task* layout, const char* name, float value); // inferred: sets a value
inline void SetPaneFloat_unk(void* layout, const char* name, float value) {
    fn_801FECE0(layout, fn_800DC6B4(name), value);
}
// inferred: d2anime task seen through its layout
struct CosEditTaskLayout_unk : public atn::Task {
    unsigned char m48[0x58 - 0x48];
    unsigned char mLayout[4]; // 0x58
};
// inferred: sets a value of the layout of a task (the layout address is taken after the hash)
inline void SetTaskFloat_unk(void* task, const char* name, float value) {
    fn_801FECE0(((CosEditTaskLayout_unk*)task)->mLayout, fn_800DC6B4(name), value);
}
extern "C" atn::Task* fn_801F64D0(atn::Task* parent, const char* path); // inferred: layout anime
// inferred: d2anime task seen through its frame and speed (as ShopD2A_unk in game/ShopBase.h)
struct CosEditD2A_unk : public atn::Task {
    unsigned char m48[0x100 - 0x48];
    float mFrame; // 0x100
    float mSpeed; // 0x104
};
// inferred: layout anime task (as ShopLayout_unk in game/ShopBase.h)
struct CosEditLayout_unk : public atn::Task {
    unsigned char m48[0x4E - 0x48];
    unsigned char m4E;
};
// inferred: field (lbl_8087F430): chapter at 0x10D0
struct Field_unk {
    void fn_803750E4(int id); // inferred: (as in gmk/RandomMerchant.cpp)
    unsigned char m0000[0x10D0];
    int mChapter; // 0x10D0
};
// inferred: functions of NewCampPartsBase (0x8050F93C-0x80512024) still called by their automatic names
extern "C" void fn_805113E8(atn::D3Anime* anime, int a, int b, float c, float d);
extern "C" void fn_805114D4(atn::D3AnimeInstance* anime, int a, int b, float c, float d);
extern "C" void fn_805115D0(atn::Task* task, int a, int b, float c, float d);

static const NewCampPartsFile_unk sFiles[11] = {
    {"protector_in_board12_0001", "equip_logo.d2a", 0},
    {"protector_in_board10_0002", "", 0},
    {"protector_in_board13_0001", "", 0},
    {"protector_in_board10_0003", "", 0},
    {"protector_in_board16_0001", "", 0},
    {"protector_in_board18_0001", "", 0},
    {"protector_in_board05_0001", "", 0},
    {"protector_in_board14_0001", "equip_logo2.d2a", 0},
    {"protector_in_board15_0001", "", 0},
    {"protector_in_board17_0001", "", 0},
    {"protector_in_board18_0002", "", 0},
};
static const char* const sBoard = "protector_in_board";
static const NewCampPartsMotionFile_unk sMotions[13] = {
    {"protector_in_board12", ""},
    {"protector_in_board10", ""},
    {"protector_in_board13", ""},
    {"protector_in_board10", ""},
    {"protector_in_board16", ""},
    {"protector_in_board18", ""},
    {"protector_in_board05", "protector_in_board05_0001.motion"},
    {"protector_in_board14", ""},
    {"protector_in_board15", ""},
    {"protector_in_board17", ""},
    {"protector_in_board18", ""},
    {"protector_in_board10", "protector_in_board10_0011.motion"},
    {"protector_in_board13", "protector_in_board13_0001.motion"},
};

// inferred: creates a hidden d3anime instance into `anime` (inlined; the destination address is taken first)
inline void CreateInstance_unk(atn::Task* parent, atn::D3AnimeInstance*& anime, const char* path) {
    (anime = atn::D3AnimeInstance::Create(parent, path, 1))->fn_800D246C(1);
}
inline void CreateAnime_unk(atn::Task* parent, atn::D3Anime*& anime, const char* path) {
    (anime = atn::D3Anime::Create(parent, path, 1))->fn_800D246C(1);
}
static const char* const sListTexts[5] = {
    "lw_list_text_message_0001", "lw_list_text_message_0002", "lw_list_text_message_0003",
    "lw_list_text_message_0004", "lw_list_text_message_0005",
};
static const char* const sPartTexts[13] = {
    "lw_list_text_body",     "lw_list_text_bodysub",   "lw_list_text_armor", "lw_list_text_accessories",
    "lw_list_text_pants",    "lw_list_text_boots",     "lw_list_text_legarmor",
    "lw_list_text_legaccessories", "lw_list_text_effect", "lw_list_text_toumei", "lw_list_text_patuhazusi",
    "lw_list_text_load",     "lw_list_text_save",
};
// inferred: start frame and flags of the boards (one per entry of sFiles, 0x14 bytes)
struct CosEditBoardAnime_unk {
    float mFrame;
    float m04;
    float m08;
    int m0C;
    int m10;
};
static const CosEditBoardAnime_unk sBoardAnimes[13] = {
    {0.0f, 0.0f, 0.0f, 1, 1}, {4.0f, 4.0f, 4.0f, 1, 1}, {1.0f, 0.0f, 0.0f, 0, 1}, {0.0f, 0.0f, 0.0f, 1, 1},
    {0.0f, 0.0f, 0.0f, 1, 1}, {0.0f, 0.0f, 0.0f, 1, 1}, {0.0f, 0.0f, 0.0f, 1, 0}, {0.0f, 0.0f, 0.0f, 1, 1},
    {4.0f, 4.0f, 4.0f, 1, 1}, {1.0f, 0.0f, 0.0f, 0, 1}, {0.0f, 0.0f, 0.0f, 1, 1}, {0.0f, 0.0f, 0.0f, 0, 0},
    {0.0f, 0.0f, 0.0f, 0, 0},
};
// inferred: offset used by vf64 (constructed by __sinit 0x801D54A0)
static atn::Vector3 sOffset(0.0f, -600.0f, 0.0f);

CosEditProtector* CosEditProtector::fn_801CFD68(atn::Task* parent) {
    if (parent != 0) {
        return new ((MemoryTag)1, "", "", 0) CosEditProtector(parent);
    }
    return 0;
}

// NONMATCHING: 97% - register allocation of the creation loops (this in r29 in the original)
CosEditProtector::CosEditProtector(atn::Task* parent)
    : CosEditBase(parent), mB7C(0), mB80(0), mB88(0), m1B84(0.0f, 0.0f, 0.0f), m1B90(0), m1B94(0), m1B98(0), m1B9C(0), m1BA0(0), m1BA4(0), m1BA8(0), m1BAC(0), m1BB0(0),
      m1BB4(0), m1BB8(0), m1BBC(0), m1BC0(0), m1BC8(-1), m1BCC(0), mSetCount(0) {
    m64 = atn::Vector3(-10.0f, 0.0f, 20.0f);
    m98 = "equip_board";
    m4C = 11;
    if (m4C > 0) {
        m50.Reset_unk(new ((MemoryTag)1, "", "", 0) NewCampPartsEntry_unk[11]);
        m54.Reset_unk(new ((MemoryTag)1, "", "", 0) NewCampPartsMotion_unk[13]);
        fn_8050FB38(11, m50.mData, sFiles, 1, 0);
        fn_8050FC04(13, m54.mData, sMotions);
    }
    (mSelect = atn::D3Anime::Create(this, "data/d2anime/equip_select.d2a", 1))->fn_800D246C(1);
    (mListParts = atn::D3Anime::Create(this, "data/d2anime/protector_list_parts.d2a", 1))->fn_800D246C(1);
    (mListBase = atn::D3Anime::Create(this, "data/d2anime/protector_list_base.d2a", 1))->fn_800D246C(1);
    (mColorSelect = atn::D3Anime::Create(this, "data/d2anime/equip_color_select.d2a", 1))->fn_800D246C(1);
    (mColorPalette = atn::D3Anime::Create(this, "data/d2anime/equip_color_palette.d2a", 1))->fn_800D246C(1);
    (mEffectPalette = atn::D3Anime::Create(this, "data/d2anime/equip_effect_palette.d2a", 1))->fn_800D246C(1);
    (mEquipStatus = atn::D3Anime::Create(this, "data/d2anime/equip_status.d2a", 1))->fn_800D246C(1);
    (mCampStatus = atn::D3Anime::Create(this, "data/d2anime/camp_status.d2a", 1))->fn_800D246C(1);
    (mSort = (atn::D2ATask_unk*)fn_801F3FF8(this, "/data/d2anime/camp_sort.d2a", 0))->fn_800D246C(1);
    (mSortBase = atn::D3Anime::Create(this, "/data/d2anime/camp_sort.d2a", 1))->fn_800D246C(1);
    (mCharaSelect = (atn::D2ATask_unk*)fn_801F3FF8(this, "/data/d2anime/camp_chara_select.d2a", 0))
        ->fn_800D246C(1);
    (mCharaSelect2 = (atn::D2ATask_unk*)fn_801F3FF8(this, "/data/d2anime/camp_chara_select2.d2a", 0))
        ->fn_800D246C(1);
    (mIconCursor = atn::D3Anime::Create(this, "data/d2anime/equip_color_icon_cursor.d2a", 1))->fn_800D246C(1);
    for (int i = 0; i < 9; i++) {
        {
            atn::D3AnimeInstance** anime = &mPalettes[i];
            (*anime = atn::D3AnimeInstance::Create(this, "data/d2anime/equip_color_icon.d2a", 1))->fn_800D246C(1);
        }
    }
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 8; j++) {
            {
            atn::D3AnimeInstance** anime = &mBB8[i][j];
            (*anime = atn::D3AnimeInstance::Create(this, "data/d2anime/equip_color_icon.d2a", 1))->fn_800D246C(1);
        }
        }
    }
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int index = i + j * 8;
            if (index < 64) {
                {
            atn::D3AnimeInstance** anime = &mColorCells[i][j].mIcon;
            (*anime = atn::D3AnimeInstance::Create(this, "data/d2anime/equip_color_icon.d2a", 1))->fn_800D246C(1);
        }
                {
                atn::D3AnimeInstance** anime = &mColorCells[i][j].mFrame;
                (*anime = atn::D3AnimeInstance::Create(this, "data/d2anime/equip_color_icon_none.d2a", 1))->fn_800D246C(1);
            }
                mColorCells[i][j].mId = fn_8020BDE0(index);
            }
        }
    }
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 4; j++) {
            int index = i + j * 8;
            if (index == 31) {
                {
                atn::D3AnimeInstance** anime = &mPartCells[i][j].mIcon;
                (*anime = atn::D3AnimeInstance::Create(this, "data/d2anime/equip_color_icon_def.d2a", 1))->fn_800D246C(1);
            }
                {
                atn::D3AnimeInstance** anime = &mPartCells[i][j].mFrame;
                (*anime = atn::D3AnimeInstance::Create(this, "data/d2anime/equip_color_icon_none.d2a", 1))->fn_800D246C(1);
            }
                mPartCells[i][j].mId = 31;
                {
            atn::D3AnimeInstance** anime = &mColorMarks[index];
            (*anime = atn::D3AnimeInstance::Create(this, "data/d2anime/equip_color_icon_def.d2a", 1))->fn_800D246C(1);
        }
            } else if (index < 32) {
                char name[0x100];
                sprintf(name, "data/d2anime/equip_color_%s.d2a", fn_8020BD78(index));
                {
            atn::D3AnimeInstance** anime = &mPartCells[i][j].mIcon;
            (*anime = atn::D3AnimeInstance::Create(this, name, 1))->fn_800D246C(1);
        }
                {
                atn::D3AnimeInstance** anime = &mPartCells[i][j].mFrame;
                (*anime = atn::D3AnimeInstance::Create(this, "data/d2anime/equip_color_icon_none.d2a", 1))->fn_800D246C(1);
            }
                mPartCells[i][j].mId = fn_8020BE10(index);
                {
            atn::D3AnimeInstance** anime = &mColorMarks[index];
            (*anime = atn::D3AnimeInstance::Create(this, name, 1))->fn_800D246C(1);
        }
            }
            (m1378 = atn::D3AnimeInstance::Create(this, "data/d2anime/equip_color_icon_none.d2a", 1))
                ->fn_800D246C(1);
        }
    }
    for (int i = 0; i < 5; i++) {
        {
            atn::D3Anime** anime = &mStatusUp[i];
            (*anime = atn::D3Anime::Create(this, "data/d2anime/weapon_status_up_loop.d2a", 1))->fn_800D246C(1);
        }
        {
            atn::D3Anime** anime = &mStatusDown[i];
            (*anime = atn::D3Anime::Create(this, "data/d2anime/weapon_status_down_loop.d2a", 1))->fn_800D246C(1);
        }
    }
    (mBest = atn::D3Anime::Create(this, "/data/d2anime/equip_best.d2a", 1))->fn_800D246C(1);
    mB28 = 0.0f;
    (mBestTask = (atn::D2ATask_unk*)fn_801F3FF8(this, "/data/d2anime/equip_best.d2a", 0))->fn_800D246C(1);
    (mNaked = (atn::D2ATask_unk*)fn_801F3FF8(this, "/data/d2anime/equip_naked.d2a", 0))->fn_800D246C(1);
    m46C = atn::Vector3(10.0f, 15.0f, -30.0f);
    m7C = 0;
    m5C.vf0C("camera/ui/weapon_board_camera.xsca");
}

CosEditProtector::~CosEditProtector() {}

int CosEditProtector::vf4C() {
    for (int i = 0; i < 13; i++) {
        if (m54[i].mMotion.fn_800A0448()) {
            return 1;
        }
    }
    return 0;
}

int CosEditProtector::fn_801D0BAC() {
    if (mLoadCount != 0 || mA88 > 0 || fn_801CE4CC()) {
        return 1;
    }
    return m7C == 13;
}

void CosEditProtector::vf1C(NewCampScene_unk* scene) {
    fn_8050FD38(scene, m50.mData, sFiles, m4C, sBoard);
    for (int i = 0; i < m4C; i++) {
        NewCampPartsEntry_unk* entries = &m50[0];
        if (entries[i].mAnime != 0) {
            entries[i].mAnime->fn_800D246C(0);
            fn_805113E8(entries[i].mAnime, 1, 0, sBoardAnimes[i].mFrame, 0.0f);
        }
    }
    mSelect->fn_800D246C(0);
    mListParts->fn_800D246C(0);
    mListBase->fn_800D246C(0);
    mColorSelect->fn_800D246C(0);
    mColorPalette->fn_800D246C(0);
    mEffectPalette->fn_800D246C(0);
    mEquipStatus->fn_800D246C(0);
    mCampStatus->fn_800D246C(0);
    mSort->fn_800D246C(0);
    mSortBase->fn_800D246C(0);
    mCharaSelect->fn_800D246C(0);
    mCharaSelect2->fn_800D246C(0);
    for (int i = 0; i < 3; i++) {
        if (mLists[i].mBase != 0) {
            mLists[i].mBase->fn_800D246C(0);
        }
        if (mLists[i].mScrollBar != 0) {
            mLists[i].mScrollBar->fn_800D246C(0);
        }
        for (int j = 0; j < 13; j++) {
            mLists[i].mItems[j]->fn_800D246C(0);
        }
    }
    mIconCursor->fn_800D246C(0);
    for (int i = 0; i < 9; i++) {
        mPalettes[i]->fn_800D246C(0);
    }
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 8; j++) {
            mBB8[i][j]->fn_800D246C(0);
        }
    }
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (mColorCells[i][j].mIcon != 0) {
                mColorCells[i][j].mIcon->fn_800D246C(0);
            }
            if (mColorCells[i][j].mFrame != 0) {
                mColorCells[i][j].mFrame->fn_800D246C(0);
            }
        }
    }
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 4; j++) {
            if (mPartCells[i][j].mIcon != 0) {
                mPartCells[i][j].mIcon->fn_800D246C(0);
            }
            if (mPartCells[i][j].mFrame != 0) {
                mPartCells[i][j].mFrame->fn_800D246C(0);
            }
        }
    }
    for (int i = 0; i < 32; i++) {
        if (mColorMarks[i] != 0) {
            mColorMarks[i]->fn_800D246C(0);
        }
    }
    if (m1378 != 0) {
        m1378->fn_800D246C(0);
    }
    for (int i = 0; i < 5; i++) {
        mStatusUp[i]->fn_800D246C(0);
        mStatusUp[i]->mVisible = 0;
        mStatusDown[i]->fn_800D246C(0);
        mStatusDown[i]->mVisible = 0;
    }
    fn_805113E8(mSelect, 1, 0, 4.0f, 0.0f);
    fn_805113E8(mListParts, 1, 0, 4.0f, 0.0f);
    fn_805113E8(mListBase, 1, 0, 0.0f, 0.0f);
    fn_805113E8(mColorSelect, 1, 0, 0.0f, 0.0f);
    fn_805113E8(mColorPalette, 1, 0, 0.0f, 0.0f);
    fn_805113E8(mEffectPalette, 1, 0, 0.0f, 0.0f);
    fn_805113E8(mEquipStatus, 1, 0, 0.0f, 0.0f);
    fn_805113E8(mCampStatus, 1, 0, 0.0f, 0.0f);
    fn_805115D0(mSort, 1, 0, 0.0f, 1.0f);
    fn_805113E8(mSortBase, 1, 0, 0.0f, 1.0f);
    fn_805115D0(mCharaSelect, 1, 0, 0.0f, 1.0f);
    fn_805115D0(mCharaSelect2, 1, 0, 0.0f, 1.0f);
    for (int i = 0; i < 3; i++) {
        if (mLists[i].mBase != 0) {
            fn_805113E8(mLists[i].mBase, 1, 0, 0.0f, 0.0f);
        }
        if (mLists[i].mScrollBar != 0) {
            fn_805114D4(mLists[i].mScrollBar, 1, 0, 100.0f, 0.0f);
        }
        for (int j = 0; j < 13; j++) {
            fn_805114D4(mLists[i].mItems[j], 1, 0, 0.0f, 0.0f);
        }
    }
    fn_805113E8(mIconCursor, 1, 1, 0.0f, 1.0f);
    for (int i = 0; i < 9; i++) {
        fn_805114D4(mPalettes[i], 1, 0, 0.0f, 0.0f);
    }
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 8; j++) {
            fn_805114D4(mBB8[i][j], 1, 0, 0.0f, 0.0f);
        }
    }
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            fn_805114D4(mColorCells[i][j].mIcon, 1, 0, 0.0f, 0.0f);
            fn_805114D4(mColorCells[i][j].mFrame, 1, 0, 0.0f, 0.0f);
        }
    }
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 4; j++) {
            fn_805114D4(mPartCells[i][j].mIcon, 1, 0, 0.0f, 0.0f);
            fn_805114D4(mPartCells[i][j].mFrame, 1, 0, 0.0f, 0.0f);
        }
    }
    for (int i = 0; i < 32; i++) {
        fn_805114D4(mColorMarks[i], 1, 0, 0.0f, 0.0f);
    }
    fn_805114D4(m1378, 1, 0, 0.0f, 0.0f);
    fn_805113E8(mBest, 1, 0, 0.0f, -1.0f);
    fn_805115D0(mBestTask, 1, 0, 0.0f, -1.0f);
    fn_805115D0(mNaked, 1, 0, 0.0f, -1.0f);
    mE8 = (atn::Task*)&m54[11];
    mEC = (atn::Task*)&m54[12];
    vf34(0);
    vf74(0, 0);
}

void CosEditProtector::vf24() {
    m7C = 0;
    vf74(0, 0);
    m1B90 = 0;
    m1B94 = 0;
    m1B98 = 0;
    m1B9C = 0;
    m1BA0 = 0;
    m1BA4 = 0;
    m1BA8 = 0;
    m1BAC = 0;
    m1BB4 = 0;
    m1BB8 = 0;
    m1BBC = 0;
    m1BC0 = 0;
    mB28 = 0.0f;
    m1F8 = 0;
    m1FC = 0;
    m200 = 0;
    m204 = 0;
    mB7C = 0;
    mB80 = 0;
    mSetCount = 0;
    for (int i = 0; i < 7; i++) {
        switch (i) {
        case 1:
            if (lbl_8087F430->mChapter <= 109000 && fn_80370174(lbl_8087F430, 0x764) < 1) {
                continue;
            }
            break;
        case 5:
        case 6:
            if (lbl_8087F430->mChapter < 103000) {
                continue;
            }
            break;
        }
        CosEditSet_unk set;
        set.mId = i;
        set.m04 = i;
        new (&mSets[mSetCount]) CosEditSet_unk(set);
        mSetCount++;
    }
    if (mBestTask != 0) {
        ((CosEditD2A_unk*)mBestTask)->mFrame = 0.0f;
        ((CosEditD2A_unk*)mBestTask)->mSpeed = -3.0f;
    }
    if (mB88 != 0) {
        fn_800D2338(mB88);
        mB88 = 0;
    }
    (mB88 = fn_801F64D0(this, "data/d2anime/equip_face.d2a"))->fn_800D246C(1);
    ((CosEditLayout_unk*)mB88)->m4E = 0;
    m1C54.clear();
}

// NONMATCHING: 93% - the copy of the zero vector takes the destination before the source (blocker of
// docs/trucos-mwcc.md: Vector3 operator= register crossing)
void CosEditProtector::vf28() {
    atn::Vector3* pos = &m1B84;
    *pos = lbl_807C7030;
    mB28 = 0.0f;
    for (int i = 0; i < m4C; i++) {
        m54[i].mFrame = 0.0f;
    }
    fn_801CE518();
    if (mB88 != 0) {
        fn_800D2338(mB88);
        mB88 = 0;
    }
}

void CosEditProtector::vf30() {
    m100 = 0;
    if (m1E0 == 0) {
        m104 = (CosEditMember_unk*)((char*)m48 + 0x1228);
    } else {
        m104 = (CosEditMember_unk*)((char*)m48 + 0x126C);
    }
    fn_801CE518();
    mE0 = 0.0f;
    mE4 = 0.0f;
    fn_801CE148(m104);
    if (m1E0 == 0) {
        vf34(0);
    } else {
        vf34(4);
    }
    mF8 = 0.0f;
}

void CosEditProtector::vf48() {
    float value = 0.0f;
    for (int i = 0; i < m4C; i++) {
        NewCampPartsEntry_unk* entry = &m50[i];
        if (entry->m04 == 1) {
            atn::D3AnimeInstance* anime = (atn::D3AnimeInstance*)entry->mAnime;
            if (anime != 0 && anime->GetTask() != 0) {
                fn_801F6C80((atn::Task*)((atn::D3AnimeInstance*)m50[i].mAnime)->GetTask(), "frameOn", value);
            }
        } else {
            atn::D3Anime* anime = entry->mAnime;
            if (anime != 0 && anime->GetTask() != 0) {
                SetTaskFloat_unk(m50[i].mAnime->GetTask(), "frameOn", value);
            }
        }
    }
    if (mSelect != 0 && mSelect->GetTask() != 0) {
        SetTaskFloat_unk(mSelect->GetTask(), "frameOn", value);
    }
    if (mListParts != 0 && mListParts->GetTask() != 0) {
        SetTaskFloat_unk(mListParts->GetTask(), "frameOn", value);
    }
    if (mListBase != 0 && mListBase->GetTask() != 0) {
        SetTaskFloat_unk(mListBase->GetTask(), "frameOn", value);
    }
    if (mColorSelect != 0 && mColorSelect->GetTask() != 0) {
        SetTaskFloat_unk(mColorSelect->GetTask(), "frameOn", value);
    }
    if (mColorPalette != 0 && mColorPalette->GetTask() != 0) {
        SetTaskFloat_unk(mColorPalette->GetTask(), "frameOn", value);
    }
    if (mEffectPalette != 0 && mEffectPalette->GetTask() != 0) {
        SetTaskFloat_unk(mEffectPalette->GetTask(), "frameOn", value);
    }
    if (mEquipStatus != 0 && mEquipStatus->GetTask() != 0) {
        SetTaskFloat_unk(mEquipStatus->GetTask(), "frameOn", value);
    }
    for (int i = 0; i < 3; i++) {
        if (mLists[i].mBase != 0 && mLists[i].mBase->GetTask() != 0) {
        SetTaskFloat_unk(mLists[i].mBase->GetTask(), "frameOn", value);
    }
    }
    if (mBest != 0 && mBest->GetTask() != 0) {
        SetTaskFloat_unk(mBest->GetTask(), "frameOn", value);
    }
    if (mNaked != 0) {
        SetTaskFloat_unk(mNaked, "frameOn", value);
    }
}

void CosEditProtector::fn_801D463C(atn::D3Anime** out, CosEditList_unk* lists, int state, int a, int b) {
    switch (state) {
    case 5:
        lists[1].m40 = b;
        lists[1].m3C = a;
        mB8C = a;
        break;
    case 6:
        lists[2].m3C = a;
        lists[2].m40 = b;
        break;
    case 7:
        *out = mColorPalette;
        mColorPalette->mVisible = 1;
        mB8C = a;
        break;
    case 8:
        *out = mEffectPalette;
        mEffectPalette->mVisible = 1;
        mB8C = a;
        break;
    case 4:
        *out = mColorSelect;
        mColorSelect->mVisible = 1;
        mB8C = a;
        break;
    default:
        *out = mColorSelect;
        mColorSelect->mVisible = 1;
        break;
    }
}

void CosEditProtector::vf2C() {
    if (mLoadCount == 0 && mA88 <= 0 && !fn_801CE4CC()) {
        void* pad = lbl_8087EF70;
        if (m1F0 == 0 && mF4 == 0 && mFC == 0 && mF0 == 0) {
            int dir = 0;
            if (m7C == 0 || m7C == 4) {
                if (fn_800A555C(pad, 0, 2) || fn_800A555C(pad, 0, 23) || fn_800A555C(pad, 0, 10)) {
                    dir = -1;
                } else if (fn_800A555C(pad, 0, 3) || fn_800A555C(pad, 0, 24) || fn_800A555C(pad, 0, 11)) {
                    dir = 1;
                } else if (fn_800A555C(pad, 0, 6)) {
                    dir = 1;
                }
            } else if (m7C != 13) {
                if (fn_800A58D0(pad, 0)) {
                    if (fn_800A555C(pad, 0, 10)) {
                        dir = -1;
                    } else if (fn_800A555C(pad, 0, 11)) {
                        dir = 1;
                    }
                } else if (fn_800A555C(pad, 0, 6)) {
                    dir = 1;
                }
            }
            if (dir != 0) {
                CosEditMember_unk* member = fn_801CDD9C(dir);
                if (member != 0 && member->mId != mChara[0]->mFace) {
                    fn_801CDE14(member, dir);
                    fn_80117228(10);
                }
            }
        }
        if (mF4 == 0) {
            switch (m7C) {
            case 0:
                fn_801D80BC();
                break;
            case 2:
                fn_801D8560();
                break;
            case 3:
                fn_801D8CC4();
                break;
            case 4:
                fn_801D54C0();
                break;
            case 5:
                fn_801D59A8();
                break;
            case 6:
                fn_801D5AB0();
                break;
            case 7:
                fn_801D5F04();
                break;
            case 10:
                fn_801D6560();
                break;
            case 8:
                fn_801D6188();
                break;
            case 9:
                fn_801D6390();
                break;
            case 11:
                fn_801D924C();
                break;
            case 12:
                fn_801D9388();
                break;
            case 13:
                fn_801D94D0();
                break;
            case 14:
                fn_801D6754();
                break;
            }
        }
    }
}

// NONMATCHING: case 8 stores mFC before computing the cell (scheduling of the % / / pair)
void CosEditProtector::vf34(int state) {
    int prev = m7C;
    m7C = state;
    switch (state) {
    case 1:
        fn_801CE5B0(0, 0);
        mFC = 1;
        fn_8052DEEC(m48, 0);
        fn_80117228(8);
        m58 = -1.0f;
        break;
    case 0:
        fn_801CE5B0(0, 1);
        mFC = 0;
        m80 = m1B98;
        m84 = 5;
        m88 = 0;
        m8C = 5;
        break;
    case 2:
        fn_801CE5B0(2, 1);
        mFC = 0;
        fn_801DD26C(fn_801CF334()->mFace);
        fn_801E6EF0(fn_801CF334()->mFace, m1E4, &m80, &m88, 1);
        fn_801E6714();
        break;
    case 3:
        fn_801CE5B0(3, 1);
        mFC = 0;
        fn_801E07A4(fn_801CF334()->mFace, m1E4);
        fn_801E6FC8(fn_801CF334()->mFace, m1E4, &m80, &m88, 1);
        fn_801E6B10();
        break;
    case 4:
        if (!fn_80370174(lbl_8087F430, 0xE8)) {
            fn_80370320(lbl_8087F430, 0xE8, 1, 0);
        }
        fn_801CE5B0(4, (prev == 0 || prev == 1) ? 0 : 1);
        mFC = 0;
        m80 = m1BA0;
        m84 = 13;
        m88 = 0;
        m8C = 13;
        break;
    case 5:
        fn_801CE5B0(7, 1);
        mFC = 0;
        m80 = m1BB8;
        m84 = 8;
        m88 = 0;
        m8C = 8;
        fn_801D70EC(mColorPalette);
        break;
    case 6:
    case 7:
        fn_801CE5B0(5, 1);
        mFC = 0;
        m1B90 = 0;
        m1B94 = 0;
        m88 = 0;
        if (m7C == 6) {
            m80 = m1BB0 == 1 ? m1BA8 : m1BA4;
        } else {
            m80 = m1BAC;
        }
        m1BB0 = 0;
        fn_801D6F84();
        break;
    case 10:
        if (lbl_8087F430 != 0) {
            lbl_8087F430->fn_803750E4(0xE7);
        }
        fn_801CE5B0(6, 1);
        mFC = 0;
        m1BB4 = 0;
        m88 = 0;
        fn_801D6AB0();
        fn_801D6C1C(m1BB4);
        m80 = m1BB4;
        break;
    case 8: {
        fn_801CE5B0(7, 1);
        int member = fn_801CF334()->mFace;
        int* ids = (int*)&Inventory::instance_->mEquipShown[member];
        int cell = fn_8020BE40(ids[fn_80117214(m1BB8)]);
        mFC = 1;
        m1F8 = cell % 8;
        m1FC = cell / 8;
        m88 = 0;
        fn_801D70EC(mColorPalette);
        break;
    }
    case 9: {
        fn_801CE5B0(8, 1);
        mFC = 1;
        m88 = 0;
        CharaStatus* status;
        if (fn_801CF334()->mChara != 0) {
            status = fn_801CF334()->mChara->GetStatus_unk();
        } else {
            status = Inventory::instance_->GetPlayerStatusInfo(fn_801CF334()->mFace);
        }
        int cell = fn_8020BF20(status->m231 - 1);
        m200 = cell % 8;
        m204 = cell / 8;
        fn_801D7354(mEffectPalette);
        break;
    }
    case 11:
        m80 = m1BBC;
        m84 = 2;
        m88 = 0;
        mFC = 1;
        break;
    case 12:
    case 13:
        m1BC8 = -1;
        m1BCC = 0;
        break;
    case 14:
        m1BC0 = 0;
        m80 = 0;
        m84 = 4;
        m88 = 0;
        mFC = 1;
        break;
    }
}

// NONMATCHING: registers of member/item swapped; the original takes the address of the bars literal once
void CosEditProtector::fn_801D4E74(int member, atn::D2ATask_unk* task, int mode) {
    if (task != 0) {
        CosEditUV_unk uv;
        std::basic_string<wchar_t> name;
        if ((unsigned int)mode <= 1) {
            void* item = fn_801CFA5C(member, mode);
            if (item != 0) {
                CosEditItemData_unk* data = fn_80211480(fn_80206BE4(item));
                uv = fn_80444C50(Inventory::instance_, fn_80444BE8(Inventory::instance_, data));
                if (data != 0) {
                    name = data->mName;
                }
            } else {
                fn_80444C48(Inventory::instance_);
                uv = CosEditUV_unk(0.0f, 0.0f, 0.0f, 0.0f);
                name = lbl_8087DA58 + 5;
            }
        } else if ((unsigned int)(mode - 2) <= 1) {
            int slot = mode - 2;
            EquipParamEntry_unk* entry = (EquipParamEntry_unk*)fn_801CFAA8(member, slot);
            if (slot == 1 && fn_8020EF4C((EquipParamEntry_unk*)fn_801CFAA8(member, 0))) {
                entry = (EquipParamEntry_unk*)fn_801CFAA8(member, 0);
            }
            int id = fn_8020EF80(entry);
            CosEditItemData_unk* data = fn_80211480(id);
            uv = fn_80444C50(Inventory::instance_, fn_80444B64(Inventory::instance_, id));
            if (data != 0) {
                name = data->mName;
            }
        } else if (mode < 0) {
            uv = CosEditUV_unk(0.0f, 0.0f, 0.0f, 0.0f);
            name = lbl_8087DA58 + 5;
        } else {
            int id = fn_8020EF80((EquipParamEntry_unk*)fn_801CFAA8(member, 2));
            CosEditItemData_unk* data = fn_80211480(id);
            uv = fn_80444C50(Inventory::instance_, fn_80444B64(Inventory::instance_, id));
            if (data != 0) {
                name = data->mName;
            }
        }
        SetPaneText_unk(((CosEditTaskLayout_unk*)task)->mLayout, "item01", name.data());
        SetUV_unk((atn::D2A_Task*)task, "iconUv01", uv);
    }
}

void CosEditProtector::vf6C() {
    switch (m7C) {
    case 2:
        fn_801DD26C(fn_801CF334()->mFace);
        fn_801E6EF0(fn_801CF334()->mFace, m1E4, &m80, &m88, 1);
        fn_801E6714();
        break;
    case 3:
        fn_801E07A4(fn_801CF334()->mFace, m1E4);
        fn_801E6FC8(fn_801CF334()->mFace, m1E4, &m80, &m88, 1);
        fn_801E6B10();
        break;
    case 5:
        fn_801D70EC(mColorPalette);
        break;
    case 6:
    case 7:
        fn_801D6F84();
        break;
    case 10:
        fn_801D6AB0();
        fn_801D6C1C(m1BB4);
        m1BB4 = m80;
        break;
    case 8:
        fn_801D70EC(mColorPalette);
        break;
    case 9:
        fn_801D7354(mEffectPalette);
        break;
    }
}

void CosEditProtector::vf74(int a, int b) {
    m1EC = m1E8;
    m1E8 = b;
}

int CosEditProtector::fn_801D5420(int a, int b) {
    if (lbl_8087F430 != 0 && fn_80370174(lbl_8087F430, 0x395) && a == 0 && b == 0 &&
        fn_80370174(lbl_8087F430, 0x396)) {
        return 0;
    }
    return 1;
}
