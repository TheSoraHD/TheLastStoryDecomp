#include "game/NewCampPartsOption.h"
#include "atn/Memory.h"
#include "atn/SoundRef.h"
#include "atn/D3Anime.h"
#include "atn/D3AnimeInstance.h"
#include "atn/Csv.h"
#include "game/SetConfig.h"
#include "std/string.h"
#include "std/string_members.h"

extern "C" int sprintf(char* s, const char* format, ...);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int fn_80686B24(const wchar_t* a, const wchar_t* b, int n); // inferred: wcsncmp
extern "C" void fn_801F6C80(atn::Task* task, const char* name, float value); // inferred: sets a value
extern "C" void fn_801F6D7C(atn::Task* task, const char* name, int index, float value);
extern "C" void fn_801F837C(atn::Task* task, const char* name, const wchar_t* text); // inferred: sets a text
extern "C" float fn_801F6C2C(atn::Task* layout); // inferred: last frame
extern "C" void fn_801FEE08(void* layout, unsigned int hash, const wchar_t* text); // inferred: sets a text
extern "C" void fn_801FECE0(void* layout, unsigned int hash, float value);
extern "C" void fn_801FEDBC(void* layout, unsigned int hash, int index, float value);
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
extern "C" const wchar_t* fn_80116FC0(int a, int id); // inferred: message text
extern "C" const wchar_t* fn_80116EB8(const NewCampOptionText_unk* ref); // inferred: message text of a reference
extern "C" atn::SoundRef fn_80117228(int id);        // inferred: plays a system sound effect
extern "C" void* fn_8008937C(void* menu, const char* name); // inferred: debug menu group
extern "C" void fn_80087E9C(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void* lbl_8087EF70;
extern "C" int fn_800A555C(void* pad, int port, int button); // inferred: button triggered
extern "C" int fn_800A55AC(void* pad, int port, int button); // inferred: button repeated
extern "C" int fn_800A58D0(void* pad, int port);             // inferred: classic controller connected
extern "C" void fn_8052DEEC(atn::Task* camp, int id); // inferred: changes the parts of the camp
extern "C" void* lbl_8087EEC8;                        // inferred: font
extern "C" void fn_8006F5E8(void* font, std::string* dst, const wchar_t* src);
extern "C" void PSMTXConcat(const float a[3][4], const float b[3][4], float ab[3][4]);

// inferred: d2anime task of the D3Anime objects and of fn_801F3FF8; only the members used here
struct NewCampOptionD2A_unk : public atn::Task {
    unsigned char m48[0x58 - 0x48];
    unsigned char mLayout[0x100 - 0x58]; // 0x58: inferred: layout
    float mFrame;                        // 0x100
    float mSpeed;                        // 0x104
};
// inferred: d2anime instance task of the D3AnimeInstance objects; only the members used here
struct NewCampOptionInst_unk : public atn::Task {
    bool IsEnd_unk(); // inferred: the frame reached the last one (name invented)
    unsigned char m48[0x50 - 0x48];
    float mFrame; // 0x50
    float mSpeed; // 0x54
};
extern "C" NewCampOptionD2A_unk* fn_801F3FF8(atn::Task* parent, const char* path, int loop); // inferred: d2anime task
inline bool NewCampOptionInst_unk::IsEnd_unk() {
    return mFrame >= fn_801F6C2C(this);
}
inline NewCampOptionD2A_unk* GetD2A_unk(atn::D3Anime* anime) {
    return (NewCampOptionD2A_unk*)anime->GetTask();
}
inline NewCampOptionInst_unk* GetInst_unk(atn::D3AnimeInstance* anime) {
    return (NewCampOptionInst_unk*)anime->GetTask();
}
extern "C" NewCampPartsPoint_unk fn_801F4E8C(atn::Task* layout, unsigned int hash);
extern "C" void fn_801F4728(atn::Task* task, const char* name, const NewCampPartsPoint_unk* pos);
extern "C" void fn_801F6E78(atn::Task* task, const char* name, const NewCampPartsPoint_unk* pos);
// inferred: rectangle of a pane of an instance (D2A_TaskInstanceBase<32, 8>::GetRect_unk)
extern "C" NewCampPartsPoint_unk fn_801F7068(atn::Task* task, const char* name);

// inferred: functions of NewCampPartsBase (0x8050F93C-0x80512024) still called by their automatic names
extern "C" void fn_805113E8(atn::D3Anime* anime, int a, int b, float c, float d);
extern "C" void fn_805114D4(atn::D3AnimeInstance* anime, int a, int b, float c, float d);
extern "C" void fn_805115D0(atn::Task* anime, int a, int b, float c, float d);

// inferred: camp task (the parent m48); only the members used here
struct NewCampOptionCamp_unk {
    unsigned char m0000[0x11F8];
    int m11F8;
};
// inferred: field (lbl_8087F430); only the member used here
class Field_unk {
public:
    void fn_803750E4(int id); // inferred: sets an event flag
};
extern "C" Field_unk* lbl_8087F430;

class MenuCommonTask {
public:
    static MenuCommonTask* sInstance;
    unsigned char m00[0x68];
    int m68; // inferred: answer of the dialog
};
extern "C" void fn_804A3C24(MenuCommonTask* menu, const wchar_t* text, int id);
extern "C" void fn_804A53D4(MenuCommonTask* menu, atn::Task* task, const char* name, const NewCampPartsPoint_unk* rect,
                            int end);
extern "C" void fn_804A5E40(MenuCommonTask* menu, int type, atn::Vector3 ofs, int id);
extern "C" void fn_804A62C8(MenuCommonTask* menu, int type, int noCancel); // inferred: opens a dialog
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector

static const NewCampPartsFile_unk sFiles[7] = {
    {"option_board01_0001", "option_logo.d2a", 0},
    {"option_board03_0002", "", 0},
    {"option_board02_0002", "", 1},
    {"option_board02_0003", "", 0},
    {"option_board02_0004", "", 1},
    {"option_board02_0001", "", 1},
    {"option_board02_0005", "", 1},
};
static const char* const sBoard = "option_board";
// inferred: the model moves with the pages (MoveOffset)
static const int sMoveFlags[7] = {0, 0, 1, 1, 1, 1, 1};
static const NewCampOptionText_unk sOnOff0[2] = {{0, 0x271D}, {0, 0x271C}};
static const NewCampOptionText_unk sChoices1[2] = {{0, 0x2720}, {0, 0x2722}};
static const NewCampOptionText_unk sChoices2[2] = {{0, 0x2723}, {0, 0x2724}};
static const NewCampOptionText_unk sChoices3[3] = {{0, 0x2725}, {0, 0x2726}, {0, 0x2727}};
static const NewCampOptionText_unk sChoices4[4] = {{0, 0x2743}, {0, 0x2744}, {0, 0x2745}, {0, 0x2746}};
static const NewCampOptionText_unk sChoices5[3] = {{0, 0x2743}, {0, 0x2745}, {0, 0x2748}};
static const NewCampOptionText_unk sChoices6[2] = {{0, 0x2749}, {0, 0x274A}};
static const NewCampOptionText_unk sChoices7[2] = {{0, 0x274D}, {0, 0x274E}};
static const NewCampOptionText_unk sNone[1] = {{0, 0x8954D9}};
static const NewCampOptionDef_unk sPage0[1] = {
    {{0, 0x2710}, {0, -1}, 1, sChoices1},
};
static const NewCampOptionDef_unk sPage1[2] = {
    {{0, 0x272A}, {0, 0xF4242}, 2, sChoices7},
    {{0, 0x2711}, {0, 0xF4241}, 2, sChoices2},
};
static const NewCampOptionDef_unk sPage2[6] = {
    {{0, 0x272C}, {0, 0xF4243}, 11, 0},
    {{0, 0x8954D1}, {0, 0x8954D4}, 11, 0},
    {{0, 0x8954D0}, {0, 0x8954D3}, 11, 0},
    {{0, 0x274F}, {0, 0xF4244}, 2, sOnOff0},
    {{0, 0x274B}, {0, 0xF4245}, 2, sChoices6},
    {{0, 0x274C}, {0, 0xF4246}, 2, sChoices6},
};
static const NewCampOptionDef_unk sPage4[8] = {
    {{0, 0x2729}, {0, 0xF424E}, 4, sChoices4},
    {{0, 0x2716}, {0, 0xF424F}, 2, sOnOff0},
    {{0, 0x271E}, {0, 0xF4251}, 2, sOnOff0},
    {{0, 0x271F}, {0, 0xF4250}, 2, sOnOff0},
    {{0, 0x271A}, {0, 0xF4252}, 2, sOnOff0},
    {{0, 0x271B}, {0, 0xF4253}, 2, sOnOff0},
    {{0, 0x2719}, {0, 0xF4254}, 3, sChoices5},
    {{0, 0x272B}, {0, 0xF4255}, 2, sOnOff0},
};
static const NewCampOptionDef_unk sPage5[5] = {
    {{0, 0x2713}, {0, 0xF424A}, 11, 0},
    {{0, 0x2714}, {0, 0xF424B}, 11, 0},
    {{0, 0x2715}, {0, 0xF424C}, 11, 0},
    {{0, 0x2712}, {0, 0xF424D}, 3, sChoices3},
    {{0, 0x2751}, {0, 0xF4256}, 1, sNone},
};
static const NewCampOptionDef_unk sPage3[3] = {
    {{0, 0x272D}, {0, 0xF4247}, 11, 0},
    {{0, 0x274B}, {0, 0xF4248}, 2, sChoices6},
    {{0, 0x274C}, {0, 0xF4249}, 2, sChoices6},
};
static const NewCampOptionDef_unk* const sPages[6] = {sPage0, sPage1, sPage2, sPage3, sPage4, sPage5};
static const NewCampOptionText_unk sSwapHelp[2] = {{0, 0xF4240}, {0, 0xF4257}};

inline NewCampPartsPoint_unk GetPanePos_unk(atn::Task* layout, const char* name) {
    return fn_801F4E8C(layout, fn_800DC6B4(name));
}
// inferred: set a text/value of the pane `name` (names invented)
inline void SetPaneText_unk(void* layout, const char* name, const wchar_t* text) {
    fn_801FEE08(layout, fn_800DC6B4(name), text);
}
inline void SetPaneFloat_unk(void* layout, const char* name, float value) {
    fn_801FECE0(layout, fn_800DC6B4(name), value);
}

inline void NewCampOptionPage_unk::Load_unk() {
    mAnime->Load(1);
    for (int j = 0; j < mCount; j++) {
        mItems[j].mAnime->Load();
    }
}

inline void NewCampOptionPage_unk::Show_unk(int show) {
    mAnime->fn_800D246C(show);
    for (int j = 0; j < mCount; j++) {
        mItems[j].mAnime->fn_800D246C(show);
    }
}

NewCampPartsOption* NewCampPartsOption::fn_80521E1C(atn::Task* parent) {
    if (parent != 0) {
        return new ((MemoryTag)1, "", "", 0) NewCampPartsOption(parent);
    }
    return 0;
}

// NONMATCHING: 99.81% - the original sets r4 = 1 before D3AnimeInstance::Load() (Fv) in the load loop
NewCampPartsOption::NewCampPartsOption(atn::Task* parent)
    : NewCampPartsBase(parent), mD4(0.0f), mD8(0), mDC(0), mE0(0), mE8(0), m180(0.0f), mMove(292.0f, 0.0f, 0.0f) {
    m98 = "option_board";
    m4C = 7;
    if (m4C > 0) {
        m50.Reset_unk(new ((MemoryTag)1, "", "", 0) NewCampPartsEntry_unk[m4C]);
        fn_8050FB38(m4C, m50.mData, sFiles, 0, 0);
        fn_8050FC88(m4C, m50.mData, 1);
    }
    m5C.vf0C("camera/ui/option_camera.xsca");
    (mPages[0].mAnime = atn::D3Anime::Create(this, "/data/d2anime/option_controller.d2a", 1))->fn_800D246C(1);
    mPages[0].mCount = 1;
    mPages[0].mItems.Reset_unk(new ((MemoryTag)1, "", "", 0) NewCampOptionItem_unk[mPages[0].mCount]);
    mPages[0].fn_805266BC(0, 4, this);
    (mPages[1].mAnime = atn::D3Anime::Create(this, "/data/d2anime/option_controller_b.d2a", 1))->fn_800D246C(1);
    mPages[1].mCount = 2;
    mPages[1].mItems.Reset_unk(new ((MemoryTag)1, "", "", 0) NewCampOptionItem_unk[mPages[1].mCount]);
    mPages[1].fn_805266BC(0, 7, this);
    mPages[1].fn_805266BC(1, 9, this);
    (mPages[2].mAnime = atn::D3Anime::Create(this, "/data/d2anime/option_controller_c.d2a", 1))->fn_800D246C(1);
    mPages[2].mCount = 6;
    mPages[2].mItems.Reset_unk(new ((MemoryTag)1, "", "", 0) NewCampOptionItem_unk[mPages[2].mCount]);
    mPages[2].fn_805266BC(0, 11, this);
    mPages[2].fn_805266BC(1, 11, this);
    mPages[2].fn_805266BC(2, 11, this);
    mPages[2].fn_805266BC(3, 3, this);
    mPages[2].fn_805266BC(4, 5, this);
    mPages[2].fn_805266BC(5, 5, this);
    (mPages[3].mAnime = atn::D3Anime::Create(this, "/data/d2anime/option_camera.d2a", 1))->fn_800D246C(1);
    mPages[3].mCount = 3;
    mPages[3].mItems.Reset_unk(new ((MemoryTag)1, "", "", 0) NewCampOptionItem_unk[mPages[3].mCount]);
    mPages[3].fn_805266BC(0, 11, this);
    mPages[3].fn_805266BC(1, 5, this);
    mPages[3].fn_805266BC(2, 5, this);
    (mPages[4].mAnime = atn::D3Anime::Create(this, "/data/d2anime/option_screen.d2a", 1))->fn_800D246C(1);
    mPages[4].mCount = 8;
    mPages[4].mItems.Reset_unk(new ((MemoryTag)1, "", "", 0) NewCampOptionItem_unk[mPages[4].mCount]);
    mPages[4].fn_805266BC(0, 6, this);
    mPages[4].fn_805266BC(1, 3, this);
    mPages[4].fn_805266BC(2, 3, this);
    mPages[4].fn_805266BC(3, 3, this);
    mPages[4].fn_805266BC(4, 3, this);
    mPages[4].fn_805266BC(5, 3, this);
    mPages[4].fn_805266BC(6, 6, this);
    mPages[4].fn_805266BC(7, 3, this);
    (mPages[5].mAnime = atn::D3Anime::Create(this, "/data/d2anime/option_sound.d2a", 1))->fn_800D246C(1);
    mPages[5].mCount = 5;
    mPages[5].mItems.Reset_unk(new ((MemoryTag)1, "", "", 0) NewCampOptionItem_unk[mPages[5].mCount]);
    mPages[5].fn_805266BC(0, 11, this);
    mPages[5].fn_805266BC(1, 11, this);
    mPages[5].fn_805266BC(2, 11, this);
    mPages[5].fn_805266BC(3, 2, this);
    mPages[5].fn_805266BC(4, 8, this);
    (m138 = atn::D3Anime::Create(this, "/data/d2anime/option_controller_cursor.d2a", 1))->fn_800D246C(1);
    (mController[0] = atn::D3Anime::Create(this, "/data/d2anime/option_controller0.d2a", 1))->fn_800D246C(1);
    (mController[1] = atn::D3Anime::Create(this, "/data/d2anime/option_controller2.d2a", 1))->fn_800D246C(1);
    (mDive[0] = atn::D3Anime::Create(this, "/data/d2anime/option_controller0_dive.d2a", 1))->fn_800D246C(1);
    (mDive[1] = atn::D3Anime::Create(this, "/data/d2anime/option_controller2_dive.d2a", 1))->fn_800D246C(1);
    mDiveAlpha[0] = 0.0f;
    mDiveAlpha[1] = 0.0f;
    m170 = 0;
    (m174 = fn_801F3FF8(this, "/data/d2anime/brightness_base.d2a", 0))->fn_800D246C(1);
    (m178 = fn_801F3FF8(this, "/data/d2anime/brightness_bar.d2a", 0))->fn_800D246C(1);
    (m17C = fn_801F3FF8(this, "/data/d2anime/status_select.d2a", 0))->fn_800D246C(1);
    mInfo.clear();
    m184.vf0C("database/camp/controller_info_xx.u16");
    for (int i = 0; i < 6; i++) {
        mPages[i].Load_unk();
    }
    m154 = 0;
    mSwap[0] = 0;
    mSwapAlpha[0] = 1.0f;
    mSwapStep[0] = 0.0f;
    mSwap[1] = 0;
    mSwapAlpha[1] = 1.0f;
    mSwapStep[1] = 0.0f;
}

NewCampPartsOption::~NewCampPartsOption() {}

int NewCampPartsOption::vf4C() {
    return 0;
}

void NewCampPartsOption::vf1C(NewCampScene_unk* scene) {
    fn_8050FD38(scene, m50.mData, sFiles, m4C, sBoard);
    m58 = -1.0f;
    for (int i = 0; i < 6; i++) {
        mPages[i].Show_unk(0);
    }
    m80 = 0;
    m84 = 0;
    m88 = 0;
    m8C = 0;
}

// inferred: copies the definition of an option and shows its name; auto-inlined in vf20 and removed by
// the linker (its string is in the pool before "changeCZ")
void NewCampOptionItem_unk::SetDef_unk(const NewCampOptionDef_unk* def) {
    mChoices = def->mChoices;
    mCount = def->mCount;
    fn_801F837C(GetInst_unk(mAnime), "name", fn_80116EB8(&def->mName));
}

// NONMATCHING: 96.10% - the original keeps the font and &name in registers across the loop (r22/r25)
void NewCampPartsOption::fn_80522938() {
    mInfo.clear();
    atn::CsvU csv;
    NewCampOptionInfo_unk info;
    csv.SetData((const wchar_t*)m184.fn_80470580(), (unsigned int)m184.fn_8047059C() / 2);
    do {
        if (fn_80686B24(csv.fn_8005B9CC(), L"", 1) != 0) {
            void* font = lbl_8087EEC8;
            std::string name;
            fn_8006F5E8(font, &name, csv.fn_8005B9CC());
            info.mName = name.c_str();
            info.mText[0] = csv.fn_8005B9CC();
            info.mText[1] = csv.fn_8005B9CC();
            info.mText[2] = csv.fn_8005B9CC();
            info.mText[3] = csv.fn_8005B9CC();
            mInfo.push_back(info);
        }
    } while (csv.fn_8005BBF8() != 0);
    fn_80522DD8();
}


// NONMATCHING: 88.18% - the original recomputes the destination address of each copy (no CSE with the previous copy)
void NewCampPartsOption::fn_80522DD8() {
    for (int i = 0; i < 2; i++) {
        if (GetD2A_unk(mController[i]) != 0) {
            SetPaneFloat_unk(GetD2A_unk(mController[i])->mLayout, "changeCZ", 0.0f);
        }
    }
    atn::static_string<32, wchar_t> tmp;
    if (mSwap[0] == 1) {
        tmp = mInfo[6].mText[0];
        mInfo[6].mText[0] = mInfo[7].mText[0];
        mInfo[7].mText[0] = tmp;
        tmp = mInfo[6].mText[1];
        mInfo[6].mText[1] = mInfo[7].mText[1];
        mInfo[7].mText[1] = tmp;
        tmp = mInfo[12].mText[0];
        mInfo[12].mText[0] = mInfo[13].mText[0];
        mInfo[13].mText[0] = tmp;
        tmp = mInfo[12].mText[1];
        mInfo[12].mText[1] = mInfo[13].mText[1];
        mInfo[13].mText[1] = tmp;
        for (int i = 0; i < 2; i++) {
            if (GetD2A_unk(mController[i]) != 0) {
                SetPaneFloat_unk(GetD2A_unk(mController[i])->mLayout, "changeCZ", 1.0f);
            }
        }
    }
    if (mSwap[1] == 1) {
        tmp = mInfo[8].mText[2];
        mInfo[8].mText[2] = mInfo[9].mText[2];
        mInfo[9].mText[2] = tmp;
        tmp = mInfo[8].mText[3];
        mInfo[8].mText[3] = mInfo[9].mText[3];
        mInfo[9].mText[3] = tmp;
    }
}

// inferred: shows the texts of the controller panes; auto-inlined in vf14 and removed by the linker
// (its string is in the pool before "reference")
void NewCampPartsOption::SetInfo_unk(atn::D3Anime* anime, int index) {
    for (unsigned int k = 0; k < mInfo.size(); k++) {
        SetPaneText_unk(GetD2A_unk(anime)->mLayout, mInfo[k].mName.mStr, mInfo[k].mText[index].mStr);
    }
    SetPaneFloat_unk(GetD2A_unk(anime)->mLayout, "commandOn", 1.0f);
}

// inferred: the d2anime of an option is moving between two choices (name invented)
inline bool IsMoving_unk(NewCampOptionItem_unk* item) {
    if (item->mAnime != 0 && item->mAnime->GetTask() != 0 && !item->IsBar_unk() &&
        !(atn::Abs_unk(GetInst_unk(item->mAnime)->mSpeed) < 1.1920929e-7f)) {
        return true;
    }
    return false;
}

// inferred: stops the choice animation at its end (name invented)
inline void NewCampOptionItem_unk::Update_unk() {
    atn::D3AnimeInstance* anime = mAnime;
    if (IsMoving_unk(this) && anime->GetTask() != 0) {
        NewCampOptionInst_unk* inst = GetInst_unk(anime);
        if (inst->mSpeed < 0.0f) {
            if (inst->mFrame <= 0.0f) {
                inst->mSpeed = 0.0f;
            }
        } else if (inst->IsEnd_unk()) {
            inst->mFrame = 0.0f;
            inst->mSpeed = 0.0f;
        }
    }
}

// NONMATCHING: 97.62% - register allocation of the page/item loop (r29-r31); tried page/item pointers and an item inline
void NewCampPartsOption::vf10() {
    vf20();
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < mPages[i].mCount; j++) {
            mPages[i].mItems[j].Update_unk();
        }
    }
    if (mD4 > 0.0f) {
        float v = mD4 - 0.2f;
        mD4 = 0.0f > v ? 0.0f : v;
    } else if (mD4 < 0.0f) {
        float v = 0.2f + mD4;
        mD4 = 0.0f < v ? 0.0f : v;
    }
    float arrow = m180;
    if (arrow > 0.0f) {
        float v = arrow - 0.2f;
        m180 = 0.0f > v ? 0.0f : v;
    }
    if (arrow < 0.0f) {
        float v = 0.2f + arrow;
        m180 = 0.0f < v ? 0.0f : v;
    }
}


// inferred: board of page `page` from the current one (name invented)
inline int GetBoard_unk(int page) {
    int board = page % 6 + 3;
    if (board >= 7) {
        board = 2;
    }
    return board;
}

// NONMATCHING: 87.15% - written from m2c, only the pool order was tuned
void NewCampPartsOption::vf14() {
    int show = 0;
    if (fn_8050FD20() && ((NewCampOptionCamp_unk*)m48)->m11F8 != 1) {
        show = 1;
    }
    vf3C(show);
    atn::Vector3 move = mMove * mD4;
    for (int i = 0; i < m4C; i++) {
        NewCampPartsEntry_unk* entry = &m50[i];
        vf60(entry, 0, 7, 0.0f);
        if (sMoveFlags[i] != 0 && GetModel_unk(entry->mModel) != 0) {
            atn::Matrix3x4 mtx = GetModel_unk(entry->mModel)->mMtx;
            atn::Vector3 pos = mtx.GetTrans_unk() + move;
            mtx.m[0][3] = pos.x;
            mtx.m[1][3] = pos.y;
            mtx.m[2][3] = pos.z;
            GetModel_unk(entry->mModel)->fn_8009EE30(mtx);
        }
        vf58(entry, "reference");
        vf54(entry, "reference");
        vf5C(entry);
    }
    for (int i = 0; i < 6; i++) {
        NewCampOptionPage_unk* page = &mPages[i];
        if (page->mAnime != 0) {
            int board = GetBoard_unk(i - mE8 + 6);
            fn_80511258(&m50[board], page->mAnime, atn::Vector3(0.0f, 0.0f, 0.0f), atn::Vector3(1.0f, 1.0f, 1.0f),
                        "reference");
            for (int j = 0; j < page->mCount; j++) {
                NewCampOptionItem_unk* item = &page->mItems[j];
                fn_805112A8(&m50[board], item->mAnime, atn::Vector3(0.0f, 0.0f, 0.0f),
                            atn::Vector3(1.0f, 1.0f, 1.0f), "reference");
                int cursor = 0;
                int current = 0;
                int list = 1;
                switch (mD8) {
                case 0:
                case 1:
                case 8:
                    list = 0;
                    break;
                }
                if (list && i == mE8) {
                    current = 1;
                }
                if (current && j == mE4) {
                    cursor = 1;
                }
                item->fn_80526358(cursor, page->mAnime);
            }
            if (i == 0) {
                fn_80511258(&m50[board], m138, atn::Vector3(0.0f, 0.0f, 0.0f), atn::Vector3(1.0f, 1.0f, 1.0f),
                            "reference");
            }
        }
    }
    int board = GetBoard_unk(6 - mE8);
    NewCampOptionItem_unk* type = &mPages[0].mItems[0];
    int classic = fn_800A58D0(lbl_8087EF70, 0) != 0;
    for (int i = 0; i < 2; i++) {
        fn_80511258(&m50[board], mController[i], atn::Vector3(0.0f, 0.0f, 0.0f), atn::Vector3(1.0f, 1.0f, 1.0f),
                    "reference");
        if (GetD2A_unk(mController[i]) != 0) {
            if (classic == i) {
                GetD2A_unk(mController[i])->mSpeed = 1.0f;
            } else {
                GetD2A_unk(mController[i])->mSpeed = -1.0f;
            }
            SetPaneFloat_unk(GetD2A_unk(mController[i])->mLayout, "czAlpha", mSwapAlpha[0]);
            SetPaneFloat_unk(GetD2A_unk(mController[i])->mLayout, "udAlpha", mSwapAlpha[1]);
        }
    }
    int attack = mPages[1].mItems[1].mValue;
    for (int i = 0; i < 2; i++) {
        if (GetD2A_unk(mController[i]) != 0) {
            SetInfo_unk(mController[i], i * 2 + attack);
        }
    }
    NewCampOptionInst_unk* inst = GetInst_unk(type->mAnime);
    if (inst != 0 && type->mChoices != 0) {
        if (IsMoving_unk(type) && inst->mSpeed > 0.0f) {
            if (classic - 1 >= 0) {
                fn_801F837C(inst, "conf_before", fn_80116EB8(&type->mChoices[classic - 1]));
            }
            fn_801F837C(inst, "conf_after", fn_80116EB8(&type->mChoices[classic]));
        } else {
            fn_801F837C(inst, "conf_before", fn_80116EB8(&type->mChoices[classic]));
            if (classic + 1 < type->mCount) {
                fn_801F837C(inst, "conf_after", fn_80116EB8(&type->mChoices[classic + 1]));
            }
        }
    }
    NewCampOptionD2A_unk* layout = GetD2A_unk(mPages[1].mAnime);
    int dive = GetBoard_unk(7 - mE8);
    NewCampOptionItem_unk* guard = &mPages[1].mItems[0];
    for (int i = 0; i < 2; i++) {
        fn_80511258(&m50[dive], mDive[i], atn::Vector3(0.0f, 0.0f, 0.0f), atn::Vector3(1.0f, 1.0f, 1.0f),
                    "reference");
        if (GetD2A_unk(mDive[i]) != 0) {
            if (layout != 0) {
                NewCampPartsPoint_unk pos = GetPanePos_unk(layout, "controller_point");
                fn_801F4728(GetD2A_unk(mDive[i]), "base_pos", &pos);
            }
            if (classic == i) {
                float v = 0.1f + mDiveAlpha[i];
                mDiveAlpha[i] = v < 1.0f ? v : 1.0f;
            } else {
                float v = mDiveAlpha[i] - 0.1f;
                mDiveAlpha[i] = v > 0.0f ? v : 0.0f;
            }
            fn_801FEDBC(GetD2A_unk(mDive[i])->mLayout, fn_800DC6B4("base_color"), 0, 255.0f * mDiveAlpha[i]);
            SetPaneFloat_unk(GetD2A_unk(mDive[i])->mLayout, "onGuard", guard->mValue == 0 ? 0.0f : 1.0f);
        }
    }
    SetConfig* config = SetConfig::sInstance;
    NewCampOptionD2A_unk* page1 = GetD2A_unk(mPages[1].mAnime);
    if (config->mAttackType == 1) {
        SetPaneFloat_unk(page1->mLayout, "aDive", 0.0f);
        for (int i = 0; i < 2; i++) {
            if (GetD2A_unk(mDive[i]) != 0) {
                SetPaneFloat_unk(GetD2A_unk(mDive[i])->mLayout, "aDive", 0.0f);
            }
        }
    } else {
        SetPaneFloat_unk(page1->mLayout, "aDive", 1.0f);
        for (int i = 0; i < 2; i++) {
            if (GetD2A_unk(mDive[i]) != 0) {
                SetPaneFloat_unk(GetD2A_unk(mDive[i])->mLayout, "aDive", 1.0f);
            }
        }
    }
    if (fn_800A58D0(lbl_8087EF70, 0) != 0) {
        SetPaneText_unk(page1->mLayout, "dive_desc", fn_80116FC0(0, 0xF42A6));
        SetPaneText_unk(page1->mLayout, "dive_desc2", fn_80116FC0(0, 0xF42AA));
    } else if (config->mInTacviewType == 1) {
        SetPaneText_unk(page1->mLayout, "dive_desc", fn_80116FC0(0, 0xF42A4));
        SetPaneText_unk(page1->mLayout, "dive_desc2", fn_80116FC0(0, 0xF42A8));
    } else {
        SetPaneText_unk(page1->mLayout, "dive_desc", fn_80116FC0(0, 0xF42A5));
        SetPaneText_unk(page1->mLayout, "dive_desc2", fn_80116FC0(0, 0xF42A9));
    }
    mPages[1].mItems[0].mShow = SetConfig::sInstance->mAttackType != 1;
    mPages[2].mItems[4].mEnabled = fn_800A58D0(lbl_8087EF70, 0);
    m138->mVisible = 0;
    if (show && GetModelRes_unk(m50[3].mModel) != 0 && m50[3].GetInstance_unk() != 0) {
        NewCampPartsPoint_unk pos;
        pos.x = 0.0f;
        pos.y = 0.0f;
        pos.m08 = 0.0f;
        pos.m0C = 0.0f;
        pos.m10 = 0.0f;
        char name[0x40];
        sprintf(name, "cursor_point");
        pos = GetPanePos_unk(GetD2A_unk(mPages[mE8].mAnime), name);
        fn_805118B4(0, 0, &m50[3], pos, mD8 != 0);
        switch (mD8) {
        case 0:
        case 7:
        case 8:
            if (m178 != 0) {
                sprintf(name, "null_0053(1)");
                NewCampPartsPoint_unk p = GetPanePos_unk(m178, name);
                pos.x = 151.0f + p.x;
                pos.y = p.y - 6.0f;
                pos.m08 = 2.1f;
                pos.m0C = 0.2f;
                pos.m10 = 0.0f;
                fn_804A53D4(MenuCommonTask::sInstance, m178, 0, &pos, 0);
            }
            break;
        case 1:
            if (m154 == 0) {
                m138->mVisible = 1;
                sprintf(name, "arrow_select%02d", mE4 + 1);
                atn::Task* task;
                if (fn_800A58D0(lbl_8087EF70, 0) != 0) {
                    task = GetD2A_unk(mController[1]);
                } else {
                    task = GetD2A_unk(mController[0]);
                }
                NewCampPartsPoint_unk p = GetPanePos_unk(task, name);
                fn_801F4728(GetD2A_unk(m138), "basePoint", &p);
            }
            break;
        default: {
            NewCampOptionItem_unk* item = &mPages[mE8].mItems[mE4];
            sprintf(name, "list_point%02d", mE4);
            NewCampPartsPoint_unk p = GetPanePos_unk(GetD2A_unk(mPages[mE8].mAnime), name);
            switch (item->mKind) {
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
                if (mD8 == 2) {
                    fn_801F6D7C(GetInst_unk(item->mAnime), "pos0012(1)", 0, -94.0f);
                    fn_801F6D7C(GetInst_unk(item->mAnime), "pos0006(1)", 0, 92.5f);
                    fn_801F6D7C(GetInst_unk(item->mAnime), "pos0001(1)", 0, -87.0f);
                    fn_801F6D7C(GetInst_unk(item->mAnime), "pos0001(1)", 2, 1.73f);
                }
            case 8:
            case 9:
                sprintf(name, "cursor_point");
                pos = fn_801F7068(GetInst_unk(item->mAnime), name);
                if (item->mEnabled == 0 || item->mShow == 0) {
                    fn_805119C8(GetInst_unk(item->mAnime), "dummy", &m50[3], pos, 0);
                } else {
                    fn_805119C8(GetInst_unk(item->mAnime), 0, &m50[3], pos, 0);
                }
                break;
            case 10:
            case 11:
                pos.x = 154.0f + p.x;
                pos.y = p.y - 6.0f;
                pos.m08 = 2.1f;
                pos.m0C = 0.2f;
                pos.m10 = 0.0f;
                if (item->mEnabled == 0 || item->mShow == 0) {
                    fn_805119C8(GetInst_unk(item->mAnime), "dummy", &m50[3], pos, 0);
                } else {
                    fn_805119C8(GetInst_unk(item->mAnime), 0, &m50[3], pos, 0);
                }
                break;
            }
            break;
        }
        }
    }
    NewCampOptionD2A_unk* select = (NewCampOptionD2A_unk*)m17C;
    if (select != 0 && !m96) {
        if (show && mD8 == 0) {
            select->mSpeed = 1.0f;
        } else {
            select->mSpeed = -1.0f;
        }
        float arrow = m180;
        if (arrow > 0.0f) {
            SetPaneFloat_unk(select->mLayout, "arrowR", arrow);
            SetPaneFloat_unk(select->mLayout, "arrowL", 0.0f);
        } else if (arrow < 0.0f) {
            SetPaneFloat_unk(select->mLayout, "arrowR", 0.0f);
            SetPaneFloat_unk(select->mLayout, "arrowL", -m180);
        } else {
            SetPaneFloat_unk(select->mLayout, "arrowR", 0.0f);
            SetPaneFloat_unk(select->mLayout, "arrowL", 0.0f);
        }
    }
    m178->mFlags |= 4;
    if (m174 != 0 && m178 != 0 && !m96) {
        NewCampPartsPoint_unk pos;
        pos.x = 0.0f;
        pos.y = 0.0f;
        pos.m08 = 0.0f;
        pos.m0C = 0.0f;
        pos.m10 = 0.0f;
        pos = GetPanePos_unk(m174, "null_0053(1)");
        fn_801F4728(m178, "base_pos", &pos);
        ((NewCampOptionD2A_unk*)m178)->mFrame = 0.0f;
        SetPaneFloat_unk(((NewCampOptionD2A_unk*)m178)->mLayout, "point", m170 + 5);
        if (mD8 == 7) {
            ((NewCampOptionD2A_unk*)m174)->mSpeed = 1.0f;
            m178->mFlags &= ~4;
        } else {
            ((NewCampOptionD2A_unk*)m174)->mSpeed = -1.0f;
        }
    }
    if (show) {
        switch (mD8) {
        case 0:
            fn_804A5E40(MenuCommonTask::sInstance, 4, lbl_807C7030, 0);
            fn_804A5E40(MenuCommonTask::sInstance, 3, lbl_807C7030, 0);
        default:
            fn_804A5E40(MenuCommonTask::sInstance, 5, lbl_807C7030, 0);
            break;
        case 7:
        case 8:
            break;
        }
    }
}

// inferred: the task of the anime is still loading (name invented)
inline int IsLoading_unk(atn::D3Anime* anime) {
    return anime != 0 && anime->mState == 1;
}

// NONMATCHING: 91.81% - not tuned
void NewCampPartsOption::vf20() {
    if (m96 != 0) {
        int ready = 1;
        for (int i = 0; i < m4C; i++) {
            if (IsLoading_unk(m50[i].mAnime)) {
                ready = 0;
                break;
            }
        }
        for (int i = 0; i < 6; i++) {
            int loading;
            if (IsLoading_unk(mPages[i].mAnime)) {
                loading = 1;
            } else {
                loading = 0;
                for (int j = 0; j < mPages[i].mCount; j++) {
                    if (mPages[i].mItems[j].mAnime->mState == 1) {
                        loading = 1;
                        break;
                    }
                }
            }
            if (loading) {
                ready = 0;
            }
        }
        if (m138->mState == 1) {
            ready = 0;
        }
        if (mController[0]->mState == 1) {
            ready = 0;
        }
        if (mDive[0]->mState == 1) {
            ready = 0;
        }
        if (mController[1]->mState == 1) {
            ready = 0;
        }
        if (mDive[1]->mState == 1) {
            ready = 0;
        }
        if (m184.fn_80473F50()) {
            ready = 0;
        }
        if ((m17C->mFlags & 2) != 2) {
            ready = 0;
        }
        if ((m174->mFlags & 2) != 2) {
            ready = 0;
        }
        if ((m178->mFlags & 2) != 2) {
            ready = 0;
        }
        if (ready) {
            fn_80522938();
            for (int i = 0; i < m4C; i++) {
                fn_805113E8(m50[i].mAnime, 1, 0, 0.0f, 0.0f);
            }
            for (int i = 0; i < 6; i++) {
                const NewCampOptionDef_unk* defs = sPages[i];
                fn_805113E8(mPages[i].mAnime, 1, 0, 0.0f, 0.0f);
                const NewCampOptionDef_unk* def = defs;
                for (int j = 0; j < mPages[i].mCount; j++, def++) {
                    NewCampOptionItem_unk* item = &mPages[i].mItems[j];
                    fn_805114D4(item->mAnime, 1, 0, 0.0f, 0.0f);
                    if (defs != 0) {
                        item->SetDef_unk(def);
                    }
                }
            }
            fn_805113E8(m138, 1, 1, 0.0f, 1.0f);
            for (int i = 0; i < 2; i++) {
                fn_805113E8(mController[i], 1, 0, 0.0f, 1.0f);
                fn_805113E8(mDive[i], 1, 1, 0.0f, 1.0f);
                mDiveAlpha[i] = 0.0f;
            }
            fn_805115D0(m17C, 1, 0, 0.0f, 1.0f);
            fn_805115D0(m174, 1, 0, 0.0f, 1.0f);
            fn_805115D0(m178, 1, 0, 0.0f, 0.0f);
            vf48();
            m96 = 0;
        }
    }
}

// NONMATCHING: 97.53% - register allocation of the first loop (same as NewCampPartsStatus/Hint::vf3C)
void NewCampPartsOption::vf3C(int show) {
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
    for (int i = 0; i < 6; i++) {
        int visible = show;
        if (i == mE8 && show == 0) {
            visible = 1;
        }
        if (mPages[i].mAnime != 0) {
            mPages[i].mAnime->mVisible = visible;
        }
        for (int j = 0; j < mPages[i].mCount; j++) {
            atn::D3AnimeInstance* anime = mPages[i].mItems[j].mAnime;
            if (anime != 0) {
                anime->mVisible = visible;
            }
        }
        if (i == 0) {
            mController[0]->mVisible = visible;
            mController[1]->mVisible = visible;
        }
        if (i == 1) {
            mDive[0]->mVisible = visible;
            mDive[1]->mVisible = visible;
        }
    }
}

void NewCampPartsOption::vf24() {
    m96 = 1;
    mE0 = 0;
    mE8 = 0;
    m80 = 0;
    m84 = 0;
    mD8 = 0;
    mDC = 0;
    m154 = 0;
    mSwap[0] = 0;
    mSwapAlpha[0] = 1.0f;
    mSwapStep[0] = 0.0f;
    mSwap[1] = 0;
    mSwapAlpha[1] = 1.0f;
    mSwapStep[1] = 0.0f;
    fn_80525498();
}

void NewCampPartsOption::vf28() {
    fn_805251CC();
    mD8 = 0;
    m80 = 0;
    mE0 = 0;
    m58 = -1.0f;
    m95 = 0;
    for (int i = 0; i < m4C; i++) {
        if (strcmp(sFiles[i].mAnime, "\0") == 0) { // inferred: "\0" (two NUL bytes at 0x35B of the pool)
            m50[i].mAnime = 0;
        }
    }
    if (m174 != 0) {
        ((NewCampOptionD2A_unk*)m174)->mFrame = 0.0f;
    }
    m96 = 0;
}

void NewCampPartsOption::vf30() {
    mE0 = 0;
    mE8 = 0;
    m80 = 0;
    m84 = 0;
    mD8 = 0;
    mDC = 0;
    m95 = 1;
    fn_80525498();
}

// NONMATCHING: 99.98% - stack slots of the SoundRef temporaries of mode 8 swapped (0xC/0x10)
void NewCampPartsOption::vf2C() {
    m58 = 1.0f;
    m95 = 0;
    switch (mD8) {
    case 0:
        fn_80525800();
        break;
    case 1:
        fn_80525F08();
        break;
    case 2:
        if (SetConfig::sInstance->mAttackType == 1) {
            mE4 = 1;
            fn_80525A08(1, 1);
        } else {
            fn_80525A08(2, 0);
        }
        break;
    case 3:
        fn_80525A08(6, 0);
        break;
    case 5:
        fn_80525A08(8, 0);
        break;
    case 6: {
        SoundSettings_unk sound = SetConfig::sInstance->mSound;
        NewCampOptionItem_unk* items = mPages[5].mItems.mData;
        sound.mBgmVolume = 0.1f * items[0].mValue;
        sound.mSeVolume = 0.1f * items[1].mValue;
        sound.mVoiceVolume = 0.1f * items[2].mValue;
        sound.mSoundType = items[3].mValue;
        SetConfig::sInstance->fn_8057F1B4(&sound);
        fn_80525A08(5, 0);
        if (mE4 == 4 && fn_800A555C(lbl_8087EF70, 0, 4) != 0) {
            fn_805256C4(7, 0);
            fn_80117228(1);
        }
        break;
    }
    case 4:
        fn_80525A08(3, 0);
        break;
    case 7: {
        void* pad = lbl_8087EF70;
        int old = m170;
        m80 = old + 5;
        m84 = 11;
        fn_80510E94(0, 3);
        m170 = m80 - 5;
        if (old != m170) {
            SetConfig::sInstance->fn_8057F198(m170);
        }
        if (fn_800A555C(pad, 0, 5) != 0) {
            fn_805256C4(6, 1);
            fn_80117228(2);
        }
        break;
    }
    case 8:
        if (MenuCommonTask::sInstance->m68 == 0) {
            SetConfig::sInstance->fn_8057EF40();
            fn_80525498();
            fn_80522938();
            fn_80117228(1);
            fn_805256C4(0, 0);
        } else {
            fn_80117228(2);
            fn_805256C4(0, 0);
        }
        break;
    }
    if (mD8 == 0 && m58 > 0.0f && fn_800A555C(lbl_8087EF70, 0, 6) != 0) {
        fn_805256C4(8, 0);
        fn_80117228(1);
    }
}

void NewCampPartsOption::fn_805251CC() {
    SetConfig* config = SetConfig::sInstance;
    config->mAttackType = mPages[1].mItems[1].mValue;
    config->mDiveType = mPages[1].mItems[0].mValue;
    config->mGatheringType = mSwap[0];
    config->mInTacviewType = mSwap[1];
    config->mRollbackSpeed = 0.16f * mPages[2].mItems[0].mValue;
    if (mPages[2].mItems[4].mValue != 0) {
        config->m50 |= 1;
    } else {
        config->m50 &= ~1;
    }
    if (mPages[2].mItems[5].mValue != 0) {
        config->m50 |= 2;
    } else {
        config->m50 &= ~2;
    }
    if (mPages[2].mItems[3].mValue != 0) {
        config->m50 |= 0x10;
    } else {
        config->m50 &= ~0x10;
    }
    config->mAttackTurnSpeed = 0.15f * mPages[2].mItems[2].mValue + 0.25f;
    config->mTurnSpeed = 0.15f * mPages[2].mItems[1].mValue + 0.25f;
    config->mCameraRotSpeed = 0.2f * mPages[3].mItems[0].mValue;
    if (mPages[3].mItems[1].mValue != 0) {
        config->m50 |= 4;
    } else {
        config->m50 &= ~4;
    }
    if (mPages[3].mItems[2].mValue != 0) {
        config->m50 |= 8;
    } else {
        config->m50 &= ~8;
    }
    config->mBattleDrawHateLine = mPages[4].mItems[0].mValue;
    config->mDrawEnemyHP = mPages[4].mItems[1].mValue;
    config->mDrawEnemyPopup = mPages[4].mItems[2].mValue;
    config->mDrawFriendPopup = mPages[4].mItems[3].mValue;
    config->mShowDialog = mPages[4].mItems[4].mValue;
    config->mShowTutorial = mPages[4].mItems[5].mValue;
    config->mEventSkipType = mPages[4].mItems[6].mValue;
    config->mEventCameraControl = mPages[4].mItems[7].mValue;
    config->fn_8057F198(m170);
    config->fn_8057F154(mPages[5].mItems[3].mValue);
    config->mSound.mBgmVolume = 0.1f * mPages[5].mItems[0].mValue;
    config->mSound.mSeVolume = 0.1f * mPages[5].mItems[1].mValue;
    config->mSound.mVoiceVolume = 0.1f * mPages[5].mItems[2].mValue;
}

// NONMATCHING: 98.35% - order in which the float constants are loaded into f1-f4
void NewCampPartsOption::fn_80525498() {
    SetConfig* config = SetConfig::sInstance;
    mPages[1].mItems[1].mValue = config->mAttackType;
    mPages[1].mItems[0].mValue = config->mDiveType;
    mSwap[0] = config->mGatheringType;
    mSwap[1] = config->mInTacviewType;
    mPages[2].mItems[0].mValue = 6.26f * config->mRollbackSpeed;
    mPages[2].mItems[3].mValue = (config->m50 >> 4) & 1;
    mPages[2].mItems[4].mValue = config->m50 & 1;
    mPages[2].mItems[5].mValue = (config->m50 >> 1) & 1;
    mPages[2].mItems[2].mValue = 10.0f * ((config->mAttackTurnSpeed - 0.25f) / 1.5f);
    mPages[2].mItems[1].mValue = 10.0f * ((config->mTurnSpeed - 0.25f) / 1.5f);
    mPages[3].mItems[0].mValue = 5.0f * config->mCameraRotSpeed;
    mPages[3].mItems[1].mValue = (config->m50 >> 2) & 1;
    mPages[3].mItems[2].mValue = (config->m50 >> 3) & 1;
    mPages[4].mItems[0].mValue = config->mBattleDrawHateLine;
    mPages[4].mItems[1].mValue = config->mDrawEnemyHP != 0;
    mPages[4].mItems[2].mValue = config->mDrawEnemyPopup != 0;
    mPages[4].mItems[3].mValue = config->mDrawFriendPopup != 0;
    mPages[4].mItems[4].mValue = config->mShowDialog != 0;
    mPages[4].mItems[5].mValue = config->mShowTutorial != 0;
    mPages[4].mItems[6].mValue = config->mEventSkipType;
    mPages[4].mItems[7].mValue = config->mEventCameraControl != 0;
    m170 = config->mA4;
    mPages[5].mItems[3].mValue = config->mSound.mSoundType;
    mPages[5].mItems[0].mValue = 10.0f * config->mSound.mBgmVolume;
    mPages[5].mItems[1].mValue = 10.0f * config->mSound.mSeVolume;
    mPages[5].mItems[2].mValue = 10.0f * config->mSound.mVoiceVolume;
}

void NewCampPartsOption::fn_805256C4(int mode, int last) {
    int list = 0;
    mDC = mD8;
    mD8 = mode;
    switch (mode) {
    case 0:
        fn_805251CC();
        m80 = mE0;
        m84 = 6;
        break;
    case 1:
        m84 = 2;
        list = 1;
        break;
    case 2:
        if (SetConfig::sInstance->mAttackType == 1) {
            m84 = 1;
            mE4 = 1;
        } else {
            m84 = 2;
            list = 1;
        }
        break;
    case 3:
        m84 = 6;
        list = 1;
        break;
    case 5:
        m84 = 8;
        list = 1;
        break;
    case 6:
        m84 = 5;
        list = 1;
        break;
    case 4:
        m84 = 3;
        list = 1;
        break;
    case 8:
        fn_804A62C8(MenuCommonTask::sInstance, 8, 1);
        break;
    }
    if (list) {
        if (last) {
            mE4 = m84 - 1;
        } else {
            mE4 = 0;
        }
    }
}

void NewCampPartsOption::fn_80525800() {
    void* pad = lbl_8087EF70;
    int old = mE8;
    m80 = old;
    m84 = 6;
    fn_80510E94(1, 10);
    mE8 = m80;
    if (mE8 != old) {
        if (fn_800A55AC(pad, 0, 0x17) != 0 || fn_800A55AC(pad, 0, 2) != 0) {
            mD4 = -1.0f;
            m180 = -1.0f;
        } else if (fn_800A55AC(pad, 0, 0x18) != 0 || fn_800A55AC(pad, 0, 3) != 0) {
            mD4 = 1.0f;
            m180 = 1.0f;
        }
    }
    if (fn_800A555C(pad, 0, 5) != 0) {
        fn_8052DEEC(m48, 0);
        fn_80117228(8);
        m58 = -1.0f;
    } else if (fn_800A555C(pad, 0, 4) != 0) {
        switch (mE8) {
        case 0:
            fn_805256C4(1, 0);
            break;
        case 1:
            fn_805256C4(2, 0);
            break;
        case 2:
            fn_805256C4(3, 0);
            break;
        case 3:
            fn_805256C4(4, 0);
            break;
        case 4:
            fn_805256C4(5, 0);
            break;
        case 5:
            fn_805256C4(6, 0);
            break;
        }
        fn_80117228(1);
    }
}


// inferred: shows the help line of an option (name invented)
inline void ShowHelp_unk(const NewCampOptionDef_unk* def) {
    MenuCommonTask* menu;
    if (def->mHelp.mId >= 0) {
        if (def->mHelp.mId == 0xF4241) {
            if (fn_800A58D0(lbl_8087EF70, 0) != 0) {
                menu = MenuCommonTask::sInstance;
                fn_804A3C24(menu, fn_80116FC0(0, 0xF4258), 1);
            } else {
                menu = MenuCommonTask::sInstance;
                fn_804A3C24(menu, fn_80116EB8(&def->mHelp), 1);
            }
        } else {
            menu = MenuCommonTask::sInstance;
            fn_804A3C24(menu, fn_80116EB8(&def->mHelp), 1);
        }
    }
}

// NONMATCHING: 99.19% - r30/r31 of the pad and the option swapped; the original evaluates MenuCommonTask::sInstance first (a member call?)
void NewCampPartsOption::fn_80525A08(int count, int ofs) {
    void* pad = lbl_8087EF70;
    m84 = count;
    m80 = mE4 - ofs;
    fn_80510D64(1, 3);
    mE4 = m80 + ofs;
    NewCampOptionItem_unk* item = &mPages[mE8].mItems[mE4];
    int old = item->mValue;
    m80 = old;
    m84 = item->mCount;
    if (item->IsBar_unk()) {
        fn_80510E94(0, item->mEnabled == 0 ? 11 : 3);
    } else {
        fn_80510E94(1, item->mEnabled == 0 ? 11 : 19);
    }
    if (item->mEnabled != 0) {
        item->mValue = m80;
    }
    if (old != item->mValue) {
        if (!item->IsBar_unk()) {
            if (fn_800A55AC(pad, 0, 0x17) != 0 || fn_800A55AC(pad, 0, 2) != 0) {
                if (item->mAnime != 0 && item->mAnime->GetTask() != 0) {
                    if (item->IsBar_unk()) {
                        int value = item->mValue;
                        if (item->mAnime != 0 && item->mAnime->GetTask() != 0 && !item->IsBar_unk()) {
                            fn_801F6C80(GetInst_unk(item->mAnime), "bar_idx", value);
                        }
                    } else {
                        NewCampOptionInst_unk* inst = GetInst_unk(item->mAnime);
                        inst->mFrame = fn_801F6C2C(inst);
                        GetInst_unk(item->mAnime)->mSpeed = -1.0f;
                    }
                }
            } else if (fn_800A55AC(pad, 0, 0x18) != 0 || fn_800A55AC(pad, 0, 3) != 0) {
                if (item->mAnime != 0 && item->mAnime->GetTask() != 0) {
                    if (item->IsBar_unk()) {
                        int value = item->mValue;
                        if (item->mAnime != 0 && item->mAnime->GetTask() != 0 && !item->IsBar_unk()) {
                            fn_801F6C80(GetInst_unk(item->mAnime), "bar_idx", value);
                        }
                    } else {
                        GetInst_unk(item->mAnime)->mFrame = 0.0f;
                        GetInst_unk(item->mAnime)->mSpeed = 1.0f;
                    }
                }
            }
        } else {
            int diff = item->mValue - old;
            if (item->mAnime != 0 && item->mAnime->GetTask() != 0) {
                if (item->IsBar_unk()) {
                    int value = item->mValue;
                    if (item->mAnime != 0 && item->mAnime->GetTask() != 0 && !item->IsBar_unk()) {
                        fn_801F6C80(GetInst_unk(item->mAnime), "bar_idx", value);
                    }
                } else if (diff < 0) {
                    GetInst_unk(item->mAnime)->mFrame = 0.0f;
                    GetInst_unk(item->mAnime)->mSpeed = 1.0f;
                } else if (diff > 0) {
                    NewCampOptionInst_unk* inst = GetInst_unk(item->mAnime);
                        inst->mFrame = fn_801F6C2C(inst);
                    GetInst_unk(item->mAnime)->mSpeed = -1.0f;
                }
            }
        }
        fn_805251CC();
        if (mE8 == 1 && mE4 == 1 && item->mValue == 1) {
            lbl_8087F430->fn_803750E4(0x155);
        }
    }
    if (fn_800A555C(pad, 0, 5) != 0) {
        fn_805256C4(0, 0);
        fn_80117228(2);
    }
    const NewCampOptionDef_unk* defs = sPages[mE8];
    if (defs != 0) {
        ShowHelp_unk(&defs[mE4]);
    }
}

// NONMATCHING: 99.93% - the original evaluates MenuCommonTask::sInstance before the text (member call?), r30 vs r29
void NewCampPartsOption::fn_80525F08() {
    int classic = fn_800A58D0(lbl_8087EF70, 0);
    int cursor = mE4;
    void* pad = lbl_8087EF70;
    float step = mSwapStep[cursor];
    if (step < 0.0f) {
        m154 = 1;
        mSwapAlpha[cursor] += mSwapStep[cursor];
        if (mSwapAlpha[mE4] <= 0.0f) {
            mSwapAlpha[mE4] = 0.0f;
            mSwapStep[mE4] = 0.2f;
            fn_80522938();
        }
    } else if (step > 0.0f) {
        m154 = 1;
        mSwapAlpha[cursor] += mSwapStep[cursor];
        if (mSwapAlpha[mE4] >= 1.0f) {
            mSwapAlpha[mE4] = 1.0f;
            mSwapStep[mE4] = 0.0f;
        }
    } else {
        m154 = 0;
        m80 = cursor;
        m84 = classic != 0 ? 1 : 2;
        fn_80510E94(0, 3);
        if (m80 >= m84) {
            m80 = m84 - 1;
        }
        mE4 = m80;
        int old = mSwap[mE4];
        m80 = old;
        m84 = 2;
        fn_80510D64(1, 3);
        mSwap[mE4] = m80;
        if (old != mSwap[mE4]) {
            mSwapStep[mE4] = -0.2f;
        } else if (fn_800A555C(pad, 0, 5) != 0) {
            fn_805256C4(0, 1);
            fn_80117228(2);
        }
    }
    MenuCommonTask* menu = MenuCommonTask::sInstance;
    fn_804A3C24(menu, fn_80116EB8(&sSwapHelp[mE4]), 1);
}

void NewCampPartsOption::vf40(void* group) {
    group = fn_8008937C(group, m98.data());
    fn_80087E9C(group, "MoveOffset", &mMove, 0, 0, -1000000.0f, 1000000.0f, 1.0f);
    fn_80087E9C(group, "OffsetPos", &mA4, 0, 0, -1000000.0f, 1000000.0f, 1.0f);
    fn_80087E9C(group, "OffsetScl", &mB0, 0, 0, -1000000.0f, 1000000.0f, 1.0f);
    fn_80087E9C(group, "LocatorScl", &mBC, 0, 0, -1000000.0f, 1000000.0f, 1.0f);
    fn_80087E9C(group, "BasePos", &mC8, 0, 0, -1000000.0f, 1000000.0f, 1.0f);
}

void NewCampPartsOption::vf48() {
    float off = 0.0f;
    for (int i = 0; i < m4C; i++) {
        if (m50[i].mAnime != 0 && GetD2A_unk(m50[i].mAnime) != 0) {
            SetPaneFloat_unk(GetD2A_unk(m50[i].mAnime)->mLayout, "frameOn", off);
        }
    }
    for (int i = 0; i < 6; i++) {
        if (mPages[i].mAnime != 0 && GetD2A_unk(mPages[i].mAnime) != 0) {
            SetPaneFloat_unk(GetD2A_unk(mPages[i].mAnime)->mLayout, "frameOn", off);
        }
    }
}

// NONMATCHING: 97.18% - register allocation and the test of page->GetTask() (`bne`+`b` in the original)
void NewCampOptionItem_unk::fn_80526358(int cursor, atn::D3Anime* page) {
    NewCampOptionInst_unk* inst = GetInst_unk(mAnime);
    if (inst != 0 && page != 0 && page->GetTask() != 0) {
        char name[0x20];
        sprintf(name, "list_point%02d", mIndex);
        NewCampPartsPoint_unk pos = GetPanePos_unk(GetD2A_unk(page), name);
        fn_801F6E78(inst, "base_pos", &pos);
        if (IsBar_unk()) {
            inst->mSpeed = 0.0f;
            if (cursor != 0) {
                inst->mFrame = 0.0f;
            } else {
                inst->mFrame = 1.0f;
            }
            fn_801F6C80(inst, "point", mValue);
        }
        int value = mValue;
        NewCampOptionInst_unk* text = GetInst_unk(mAnime);
        if (text != 0 && mChoices != 0) {
            if (IsMoving_unk(this) && text->mSpeed > 0.0f) {
                if (value - 1 >= 0) {
                    fn_801F837C(text, "conf_before", fn_80116EB8(&mChoices[value - 1]));
                }
                fn_801F837C(text, "conf_after", fn_80116EB8(&mChoices[value]));
            } else {
                fn_801F837C(text, "conf_before", fn_80116EB8(&mChoices[value]));
                if (value + 1 < mCount) {
                    fn_801F837C(text, "conf_after", fn_80116EB8(&mChoices[value + 1]));
                }
            }
        }
        if (inst != 0 && mChoices != 0) {
            fn_801F6C80(inst, "arrowR", 0.0f);
            fn_801F6C80(inst, "arrowL", 0.0f);
            if (cursor != 0 && mEnabled != 0) {
                fn_801F6C80(inst, "arrowL", 1.0f);
                fn_801F6C80(inst, "arrowR", 1.0f);
            }
            fn_801F6C80(inst, "show", mShow != 0 ? 1.0f : 0.0f);
        }
    }
}

void NewCampOptionPage_unk::fn_805266BC(int index, int kind, atn::Task* parent) {
    NewCampOptionItem_unk* item = &mItems[index];
    item->mKind = kind;
    item->mIndex = index;
    switch (kind) {
    case 1:
        (item->mAnime = atn::D3AnimeInstance::Create(parent, "/data/d2anime/option_select_type0.d2a", 0))->fn_800D246C(1);
        break;
    case 2:
        (item->mAnime = atn::D3AnimeInstance::Create(parent, "/data/d2anime/option_select_type1.d2a", 0))->fn_800D246C(1);
        break;
    case 3:
        (item->mAnime = atn::D3AnimeInstance::Create(parent, "/data/d2anime/option_select_type3.d2a", 0))->fn_800D246C(1);
        break;
    case 4:
        (item->mAnime = atn::D3AnimeInstance::Create(parent, "/data/d2anime/option_select_type4.d2a", 0))->fn_800D246C(1);
        break;
    case 5:
        (item->mAnime = atn::D3AnimeInstance::Create(parent, "/data/d2anime/option_select_type5.d2a", 0))->fn_800D246C(1);
        break;
    case 6:
        (item->mAnime = atn::D3AnimeInstance::Create(parent, "/data/d2anime/option_select_type6.d2a", 0))->fn_800D246C(1);
        break;
    case 7:
        (item->mAnime = atn::D3AnimeInstance::Create(parent, "/data/d2anime/option_select_type7.d2a", 0))->fn_800D246C(1);
        break;
    case 8:
        (item->mAnime = atn::D3AnimeInstance::Create(parent, "/data/d2anime/option_select_type8.d2a", 0))->fn_800D246C(1);
        break;
    case 9:
        (item->mAnime = atn::D3AnimeInstance::Create(parent, "/data/d2anime/option_select_type9.d2a", 0))->fn_800D246C(1);
        break;
    case 10:
        (item->mAnime = atn::D3AnimeInstance::Create(parent, "/data/d2anime/option_bar.d2a", 0))->fn_800D246C(1);
        break;
    case 11:
        (item->mAnime = atn::D3AnimeInstance::Create(parent, "/data/d2anime/option_bar2.d2a", 0))->fn_800D246C(1);
        break;
    }
}
