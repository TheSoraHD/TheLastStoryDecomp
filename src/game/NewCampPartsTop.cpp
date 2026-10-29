#include "game/NewCampPartsTop.h"
#include "game/Inventory.h"
#include "atn/Memory.h"
#include "atn/SoundRef.h"
#include "atn/SoundManager.h"
#include "atn/D3AnimeInstance.h"
#include "atn/FadeCommon.h"
#include "MP/SaveLoad.h"
#include "string.h"

extern "C" int sprintf(char* s, const char* format, ...);
extern "C" int fn_800DD3FC(wchar_t* s, const wchar_t* format, ...); // inferred: swprintf
extern "C" void fn_801F6E78(atn::Task* task, const char* name, const void* pos);
extern "C" void fn_801F6C80(atn::Task* layout, const char* name, float value);   // inferred: sets a value
extern "C" void fn_801F837C(atn::Task* layout, const char* name, const wchar_t* text); // inferred: sets a text
extern "C" void fn_801FEE08(void* layout, unsigned int hash, const wchar_t* text); // inferred: sets a text
extern "C" void fn_801FECE0(void* layout, unsigned int hash, float value);
extern "C" void fn_801FED24(void* layout, unsigned int hash, int index, float value);
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
extern "C" const wchar_t* fn_80116FC0(int a, int id); // inferred: message text
extern "C" atn::SoundRef fn_80117228(int id);        // inferred: plays a system sound effect
extern "C" void fn_800D2338(atn::Task* task);        // inferred: deletes the task

// Only the members used here (the full class is in atn/D2A_Task.h, whose extern "C" declarations
// clash with the local ones of this file).
namespace atn {
class D2A_Task : public Task {
public:
    void SetNumber_unk(const char* name, int value, int digits); // 0x801F4CB4
    unsigned char m48[0x58 - 0x48];
    unsigned char mLayout[0x104 - 0x58]; // 0x58: inferred: layout
    float mSpeed;                        // 0x104
};
} // namespace atn
extern "C" atn::D2A_Task* fn_801F3FF8(atn::Task* parent, const char* path, int loop); // inferred: d2anime task
// inferred: 2D point of a d2anime pane (0x14 bytes)
struct NewCampTopPoint_unk {
    NewCampTopPoint_unk() {}
    float x;
    float y;
    float m08;
    float m0C;
    float m10;
};
extern "C" NewCampTopPoint_unk fn_801F4E8C(atn::Task* layout, unsigned int hash);
// inferred: text parameter of a d2anime message (fn_801FEC74)
namespace atn {
struct D2A_TextParam_unk;
}
extern "C" atn::D2A_TextParam_unk* fn_801FEC74(void* layout, const char* name);
extern "C" std::basic_string<wchar_t> fn_800A4450(atn::D2A_TextParam_unk* text, int index, int table); // inferred
extern "C" void* lbl_8087EEC8; // inferred: font
extern "C" float fn_8006EF48(void* font, const wchar_t* text, float size, float f2, int a, int b); // inferred: width

// inferred: model of an entry (atn::ModelInstance, src/atn/ModelInstance.cpp); only the member used here
namespace atn {
class ModelInstance {
public:
    void fn_8009EE30(const Matrix3x4& mtx); // inferred: sets the matrix
};
} // namespace atn
struct NewCampPartsModel_unk : public atn::ModelInstance {};
// inferred: the null-checked cast of the model (name invented)
inline atn::ModelInstance* GetModel_unk(NewCampPartsModel_unk* model) {
    return model != 0 ? static_cast<atn::ModelInstance*>(model) : 0;
}

// inferred: functions of NewCampPartsBase (0x8050F93C-0x80512024) still called by their automatic names
extern "C" void fn_8050FB38(NewCampPartsBase* self, int count, NewCampPartsEntry_unk* entries,
                            const NewCampPartsFile_unk* file, int a, int b);
extern "C" void fn_8050FC88(NewCampPartsBase* self, int count, NewCampPartsEntry_unk* entries, int a);
extern "C" void fn_8050FD38(NewCampPartsBase* self, NewCampScene_unk* scene, NewCampPartsEntry_unk* entries,
                            const NewCampPartsFile_unk* file, int count, const char* name);
extern "C" void fn_80510D64(NewCampPartsBase* self, int a, int b);
extern "C" void fn_805112A8(NewCampPartsBase* self, NewCampPartsEntry_unk* entries, atn::D3AnimeInstance* anime,
                            const atn::Vector3& pos, const atn::Vector3& scale, const char* name);
extern "C" void fn_805113E8(atn::D3Anime* anime, int a, int b, float c, float d);
extern "C" void fn_805114D4(atn::D3AnimeInstance* anime, int a, int b, float c, float d);
extern "C" void fn_805115D0(atn::Task* anime, int a, int b, float c, float d);

// inferred: d2anime instance task; only the members used here
struct NewCampTopLayout_unk : public atn::Task {
    unsigned char m48[0x50 - 0x48];
    float mFrame; // 0x50
};

// inferred: save data object made by fn_803B27F8 (game/CampSaveLoad.h); only the members used here
class GameSaveLoadBackgroundCommon {
public:
    int fn_803B8144(); // inferred: error code of the save/load manager
    unsigned char m000[0x154];
    unsigned char m154; // inferred: active flag
};
struct CampSaveData_unk : public atn::Task {
    unsigned char m048[0xC84 - 0x48];
    int mC84;
    unsigned char mC88[0xD60 - 0xC88];
    int mD60;
    int mD64;
    unsigned char mD68[0x14F60 - 0xD68];
    GameSaveLoadBackgroundCommon* m14F60;
};
// inferred: name and flag given to fn_803B27F8 (as in game/CampSaveLoad.cpp)
struct CampSaveArg_unk {
    char mName[0x40];
    int mFlag; // 0x40
};
inline void CopyString_unk(char* dst, const char* src) {
    if (src != dst) {
        memcpy(dst, src, strlen(src) + 1);
    }
}
extern "C" CampSaveData_unk* fn_803B27F8(atn::Task* parent, const CampSaveArg_unk* arg);
extern "C" int fn_803B87A8(CampSaveData_unk* data);
extern "C" void fn_803B3270(CampSaveData_unk* data, int slot);
extern "C" void fn_803B2FBC(CampSaveData_unk* data, int slot);
extern "C" void fn_803B8758(CampSaveData_unk* data, int arg);
class MenuLoadTask : public atn::Task {
public:
    static MenuLoadTask* fn_80117914(atn::Task* parent, CampSaveData_unk* data);
    static MenuLoadTask* sInstance;
    unsigned char m48[0x13C - 0x48];
    int m13C; // inferred: state
};

// inferred: field (lbl_8087F430); only the members used here
struct NewCampTopField_unk {
    unsigned char m0000[0x20];
    atn::Task* m20;
    unsigned char m0024[0x10D8 - 0x24];
    atn::Task* m10D8;
    unsigned char m10DC[0x5744 - 0x10DC];
    unsigned int m5744; // inferred: play time in frames
};
extern "C" NewCampTopField_unk* lbl_8087F430;
extern "C" int fn_803754F0(NewCampTopField_unk* field);
extern "C" int fn_80370174(NewCampTopField_unk* field, int id); // inferred: event flag
extern "C" void fn_80370320(NewCampTopField_unk* field, int id, int value, int arg);
extern "C" void fn_80370F8C(NewCampTopField_unk* field, int a, MP::SaveData_unk* data, int b);
extern "C" void fn_803743AC(NewCampTopField_unk* field);
extern "C" void fn_803CCC70(atn::Task* task, int arg);
extern "C" MP::SaveData_unk* lbl_8087F460;
extern "C" int fn_803BE854(MP::SaveData_unk* data);
// inferred: game UI (lbl_8087F490)
struct NewCampTopUI_unk {
    unsigned char m0000[0x263C];
    void* m263C;
    void* m2640;
};
extern "C" NewCampTopUI_unk* lbl_8087F490;
extern "C" void fn_803B57B0(void* p, int arg);
extern "C" void fn_803B6C88(void* p, int arg);
extern "C" atn::Task* lbl_8087F8A0;
class EnemyManager {
public:
    static atn::Task* sInstance;
};
class NpcManager {
public:
    static atn::Task* sInstance;
};
struct NewCampTopMap_unk {
    unsigned char m000[0xA70];
    int mA70;
};
extern "C" NewCampTopMap_unk* lbl_8087F9F8;
extern "C" void fn_805AA734(NewCampTopMap_unk* p);
class GameBgm {
public:
    int fn_8037D4C0(int id, int fade, int b, int c, float volume); // inferred: requests a BGM
    static GameBgm* sInstance;
};
#include "atn/SequenceBoot_unk.h"
class MenuCommonTask {
public:
    static MenuCommonTask* sInstance;
    unsigned char m00[0x54];
    int m54;
    unsigned char m58[0x68 - 0x58];
    int m68;
};
extern "C" void fn_804A4300(MenuCommonTask* menu, const wchar_t* text);
extern "C" void fn_804A62C8(MenuCommonTask* menu, int a, int b);
extern "C" void fn_804A5824(MenuCommonTask* menu, atn::D2A_Task* anime, const char* name, int end);
extern "C" void fn_804A436C(int* cursor, int count, int wrap, int step, int arg); // inferred: moves a cursor
extern "C" const wchar_t lbl_808813D0[2]; // inferred: empty text
// inferred: message table (lbl_8087F1E4)
struct NewCampTopMsg_unk {
    const wchar_t* Get_unk(int id) {
        return mText[id] != 0 ? mText[id] : lbl_808813D0;
    }
    const wchar_t* mText[1];
};
extern "C" NewCampTopMsg_unk* lbl_8087F1E4;
extern "C" void* lbl_8087EF70;
extern "C" int fn_800A555C(void* pad, int port, int button); // inferred: button triggered
extern "C" void fn_8052CA6C(atn::Task* camp);
extern "C" void fn_8052C9C4(atn::Task* camp);
extern "C" void fn_8052DEEC(atn::Task* camp, int id);
extern "C" int lbl_8087DCCC;
extern "C" int lbl_8087DCC8;
extern "C" int lbl_8087F434;

static const NewCampPartsFile_unk sFile = {"menu_board01_0001", "main_menu.d2a", 0};
static const char* const sBoard = "board01_01_01";
static const NewCampTopInfo_unk sInfo[8] = {
    {0, 0x272F, 0x2739}, {6, 0x2733, 0x273D}, {2, 0x2730, 0x273A}, {3, 0x2731, 0x273B},
    {1, 0x272E, 0x2738}, {4, 0x2734, 0x273E}, {5, 0x2732, 0x273C}, {7, 0x2735, 0x273F},
};
static const int sActions[3] = {0, 1, 2};
static const int sActionsB[3] = {3, 1, 2};

inline NewCampTopPoint_unk GetPanePos_unk(atn::Task* layout, const char* name) {
    return fn_801F4E8C(layout, fn_800DC6B4(name));
}
// inferred: set a text/value of the pane `name` (names invented)
inline void SetPaneText_unk(void* layout, const char* name, const wchar_t* text) {
    fn_801FEE08(layout, fn_800DC6B4(name), text);
}
inline void SetPaneFloat_unk(void* layout, const char* name, float value) {
    fn_801FECE0(layout, fn_800DC6B4(name), value);
}

// inferred: starts a fade (name invented)
inline void StartFade_unk(int wait) {
    atn::FadeColor* fade = atn::FadeColor::Create(lbl_8087F430->m20, 0, 0, 0);
    fade->mDeleteAtEnd = 1;
    fade->mFrame = 0;
    fade->mDelay = 0;
    fade->mDuration = 45;
    fade->mWait = wait;
    fade->mColor0 = 0;
    fade->mColor1 = 0xFF000000;
    fade->mZ = -100000.0f;
    fade->mActive = 1;
}

NewCampPartsTop* NewCampPartsTop::fn_80529118(atn::Task* parent) {
    if (parent != 0) {
        return new ((MemoryTag)1, "", "", 0) NewCampPartsTop(parent);
    }
    return 0;
}

// NONMATCHING: 98.88% - register allocation only (string/size of the inlined std::string assignment and
// the entry pointer of the loop)
NewCampPartsTop::NewCampPartsTop(atn::Task* parent)
    : NewCampPartsBase(parent), mTimer(0), mSave(0), mCanLoad(1), m1D0(0) {
    m98 = "top_board";
    m7C = 0;
    m4C = 1;
    mCursor = 1;
    if (m4C > 0) {
        m50.Reset_unk(new ((MemoryTag)1, "", "", 0) NewCampPartsEntry_unk[m4C]);
        fn_8050FB38(this, m4C, m50.mData, &sFile, 0, 0);
        fn_8050FC88(this, m4C, m50.mData, 0);
    }
    for (int i = 0; i < 8; i++) {
        NewCampTopEntry_unk* entry = &mEntries[i];
        (entry->mList = atn::D3AnimeInstance::Create(this, "/data/d2anime/main_menu_list.d2b", 1))->fn_800D246C(1);
        (entry->mShine = atn::D3AnimeInstance::Create(this, "/data/d2anime/main_menu_list_shine.d2b", 1))
            ->fn_800D246C(1);
        entry->mId = i;
        entry->mInfo = sInfo[i];
    }
    (mInfoAnime = fn_801F3FF8(this, "/data/d2anime/camp_info3.d2b", 0))->fn_800D246C(1);
}

NewCampPartsTop::~NewCampPartsTop() {}

void NewCampPartsTop::vf1C(NewCampScene_unk* scene) {
    fn_8050FD38(this, scene, m50.mData, &sFile, m4C, sBoard);
    vf3C(0);
    m80 = 0;
    m84 = 8;
    m88 = 0;
    m8C = 8;
    for (int i = 0; i < m4C; i++) {
        fn_805113E8(m50[i].mAnime, 1, 0, 0.0f, 0.0f);
    }
    for (int i = 0; i < 8; i++) {
        fn_805114D4(mEntries[i].mList, 0, 0, 0.0f, 0.0f);
        fn_805114D4(mEntries[i].mShine, 0, 1, 0.0f, 1.0f);
    }
    fn_805115D0(mInfoAnime, 0, 0, 0.0f, -3.0f);
}

int NewCampPartsTop::fn_805294E4() {
    return m7C != 0;
}

void NewCampPartsTop::vf10() {
    if (m7C == 1) {
        mInfoAnime->mSpeed = 3.0f;
    } else {
        mInfoAnime->mSpeed = -3.0f;
    }
}

// NONMATCHING: 99.72% - register allocation only (0.0f and the int conversion constant in f31/f30 swapped,
// text/layout of the inlined SetPaneText_unk swapped)
void NewCampPartsTop::vf14() {
    if (m4C <= 0) {
        return;
    }
    for (int i = 0; i < m4C; i++) {
        GetModel_unk(m50[i].mModel)->fn_8009EE30(m50[i].mMtx);
        vf58(&m50[i], "reference");
        vf54(&m50[i], "reference");
        vf5C(&m50[i]);
    }
    for (unsigned int i = 0; i < mList.size(); i++) {
        NewCampTopEntry_unk* entry = mList.mData[i];
        fn_805112A8(this, m50.mData, entry->mList, atn::Vector3(0.0f, 0.0f, 0.0f), atn::Vector3(1.0f, 1.0f, 1.0f),
                    "reference");
        fn_805112A8(this, m50.mData, entry->mShine, atn::Vector3(0.0f, 0.0f, 0.0f), atn::Vector3(1.0f, 1.0f, 1.0f),
                    "reference");
        char name[0x40];
        if (entry->mId == 7) {
            sprintf(name, "ListPoint07");
        } else {
            sprintf(name, "ListPoint%02d", i);
        }
        NewCampTopPoint_unk pos = GetPanePos_unk(m50[0].mAnime->GetTask(), name);
        fn_801F6E78(entry->mList->GetTask(), "basePoint", &pos);
        fn_801F6C80(entry->mList->GetTask(), "index", entry->mInfo.mIndex);
        fn_801F837C(entry->mList->GetTask(), "title", fn_80116FC0(0, entry->mInfo.mTitle));
        fn_801F837C(entry->mList->GetTask(), "titleSub", fn_80116FC0(0, entry->mInfo.mTitle));
        fn_801F837C(entry->mList->GetTask(), "titleLang", fn_80116FC0(0, entry->mInfo.mTitleLang));
        fn_801F6E78(entry->mShine->GetTask(), "basePoint", &pos);
        fn_801F837C(entry->mShine->GetTask(), "title", fn_80116FC0(0, entry->mInfo.mTitle));
        fn_801F6C80(entry->mShine->GetTask(), "index", entry->mInfo.mIndex);
        if (i == m80) {
            ((NewCampTopLayout_unk*)entry->mList->GetTask())->mFrame = 0.0f;
            entry->mShine->mVisible = 0;
        } else {
            ((NewCampTopLayout_unk*)entry->mList->GetTask())->mFrame = 1.0f;
            entry->mShine->mVisible = 0;
        }
    }
    if (m50[0].mAnime != 0 && m50[0].mAnime->GetTask() != 0) {
        atn::D2A_Task* task = (atn::D2A_Task*)m50[0].mAnime->GetTask();
        wchar_t time[0x100];
        unsigned int sec = lbl_8087F430->m5744 / 30;
        unsigned int min = sec / 60;
        unsigned int hour = min / 60;
        fn_800DD3FC(time, L"%d:%02d:%02d", hour, min % 60, sec % 60);
        fn_801FEE08(task->mLayout, fn_800DC6B4("playTime(1)"), time);
        task->SetNumber_unk("price(1)", Inventory::instance_->mMoney, 0);
    }
    if (fn_803754F0(lbl_8087F430)) {
        SetPaneText_unk(mInfoAnime->mLayout, "select00_text", fn_80116FC0(0, 0x2767));
        SetPaneFloat_unk(mInfoAnime->mLayout, "enableLoad", mCanLoad != 0 ? 1.0f : 0.5f);
        SetPaneFloat_unk(mInfoAnime->mLayout, "enableCheckPoint", 1.0f);
    } else {
        SetPaneText_unk(mInfoAnime->mLayout, "select00_text", fn_80116FC0(0, 0x2764));
        if (fn_80370174(lbl_8087F430, 0x394)) {
            SetPaneFloat_unk(mInfoAnime->mLayout, "enableLoad", 0.5f);
            SetPaneFloat_unk(mInfoAnime->mLayout, "enableCheckPoint", 0.5f);
        } else {
            SetPaneFloat_unk(mInfoAnime->mLayout, "enableLoad", mCanLoad != 0 ? 1.0f : 0.5f);
            if (lbl_8087DCCC != 0) {
                SetPaneFloat_unk(mInfoAnime->mLayout, "enableCheckPoint", 1.0f);
            } else {
                SetPaneFloat_unk(mInfoAnime->mLayout, "enableCheckPoint", 0.5f);
            }
        }
    }
    if (m7C == 1) {
        char name[0x40];
        sprintf(name, "lw_list_text_select%02d", mCursor);
        fn_804A5824(MenuCommonTask::sInstance, mInfoAnime, name, 0);
    }
}

void NewCampPartsTop::vf20() {}

void NewCampPartsTop::vf24() {
    vf30();
    m7C = 0;
    m80 = 0;
    mCursor = 1;
    mTimer = 0;
    mCanLoad = 1;
    if (mSave != 0) {
        fn_800D2338(mSave);
        mSave = 0;
    }
    CampSaveArg_unk arg;
    // inferred: a 2-byte literal (00 00), not merged with ""
    CopyString_unk(arg.mName, "\0");
    arg.mFlag = 0;
    mSave = fn_803B27F8(this, &arg);
    mSave->mD64 = 0;
}

void NewCampPartsTop::vf28() {
    m7C = 0;
    m80 = 0;
    mCursor = 1;
    mTimer = 0;
    if (mSave != 0) {
        fn_800D2338(mSave);
        mSave = 0;
    }
}

// NONMATCHING: 99.51% - the original passes a third argument (0) to MenuLoadTask::fn_80117914, whose known
// declaration (game/CampSaveLoad.cpp) has two; the size argument of fn_8006EF48 is loaded before the
// data() branch in the original
void NewCampPartsTop::vf2C() {
    if (mSave == 0 || !mSave->IsFlag_unk(2)) {
        return;
    }
    const int* actions = fn_803754F0(lbl_8087F430) ? sActionsB : sActions;
    if (m7C == 4) {
        if (++mTimer > 60) {
            switch (actions[mCursor]) {
            case 0:
            case 1:
                fn_8052CA6C(m48);
                fn_80370F8C(lbl_8087F430, 0, lbl_8087F460, 0);
                if (lbl_8087F430->m10D8 != 0) {
                    lbl_8087F430->m10D8->mFlags |= 4;
                }
                break;
            case 2:
                fn_800D2338((atn::Task*)lbl_8087F430);
                sSequenceBoot_unk->SetNext_unk("Title");
                break;
            case 3:
                fn_8052CA6C(m48);
                fn_80370320(lbl_8087F430, 0x1B, 1, 0);
                if (lbl_8087F430->m10D8 != 0) {
                    fn_803CCC70(lbl_8087F430->m10D8, 999999);
                }
                fn_803B57B0(lbl_8087F490->m263C, 1);
                fn_803B6C88(lbl_8087F490->m2640, 1);
                lbl_8087F8A0->fn_800D246C(1);
                EnemyManager::sInstance->fn_800D246C(1);
                NpcManager::sInstance->fn_800D246C(1);
                if (lbl_8087F9F8 != 0 && lbl_8087F9F8->mA70 != 0) {
                    fn_805AA734(lbl_8087F9F8);
                    fn_803743AC(lbl_8087F430);
                }
                if (GameBgm::sInstance != 0) {
                    GameBgm::sInstance->fn_8037D4C0(0, 90, 1, 0, 1.0f);
                }
                break;
            }
        }
    } else if (m7C == 5) {
        int fade = 0;
        int done = 0;
        if (actions[mCursor] == 0) {
            if (m1D0 != 0) {
                if (fn_803B87A8(mSave) == 0) {
                    if (mSave->mC84 == 7) {
                        m1D0 = 0;
                        done = 1;
                    } else {
                        m1D0 = 0;
                        fn_803B3270(mSave, 0);
                    }
                }
            } else if (mSave != 0 && mSave->m14F60->m154 == 0) {
                if (mSave->m14F60->fn_803B8144()) {
                    done = 1;
                } else {
                    fn_803B2FBC(mSave, 0);
                    if (fn_803BE854(lbl_8087F460)) {
                        if (lbl_8087F460 != 0) {
                            delete lbl_8087F460;
                            lbl_8087F460 = 0;
                        }
                        done = 1;
                    } else {
                        fade = 1;
                    }
                }
            }
        } else if (MenuLoadTask::sInstance != 0) {
            int state = MenuLoadTask::sInstance->m13C;
            if (state == 7) {
                fn_800D2338(MenuLoadTask::sInstance);
                m7C = 1;
            } else if (state == 8) {
                fn_800D2338(MenuLoadTask::sInstance);
                fn_80117228(1);
                fade = 1;
            }
        }
        if (fade) {
            m7C = 4;
            StartFade_unk(30);
        } else if (done) {
            lbl_8087DCCC = 0;
            lbl_8087F434 = 1;
            CampSaveData_unk* save = mSave;
            if (save != 0) {
                save->mD60 = 0;
                save->mC84 = 0;
            }
            fn_804A4300(MenuCommonTask::sInstance, lbl_8087F1E4->Get_unk(0x237));
            m7C = 6;
            fn_80117228(11);
        }
    } else if (m7C == 2) {
        if (MenuCommonTask::sInstance->m68 == 0) {
            if (actions[mCursor] == 0) {
                if (mSave != 0) {
                    if (mSave->mD60 == 0) {
                        fn_803B8758(mSave, 0);
                        m1D0 = 1;
                    } else {
                        fn_803B3270(mSave, 0);
                    }
                    m7C = 5;
                    fn_80117228(1);
                } else {
                    m7C = 1;
                    fn_80117228(2);
                }
            } else {
                m7C = 4;
                fn_80117228(1);
                StartFade_unk(20);
            }
        } else {
            m7C = 1;
            fn_80117228(2);
        }
    } else if (m7C == 1) {
        fn_804A436C(&mCursor, 3, 3, 1, 3);
        void* pad = lbl_8087EF70;
        if (fn_800A555C(pad, 0, 4)) {
            switch (actions[mCursor]) {
            case 0:
                if (fn_80370174(lbl_8087F430, 0x394)) {
                    fn_80117228(11);
                    return;
                }
                if (lbl_8087DCCC != 0) {
                    fn_804A62C8(MenuCommonTask::sInstance, 2, 0);
                    m7C = 2;
                } else {
                    fn_80117228(11);
                    return;
                }
                break;
            case 1:
                if (mCanLoad == 0 || fn_80370174(lbl_8087F430, 0x394)) {
                    fn_80117228(11);
                    return;
                }
                m7C = 5;
                if (MenuLoadTask::sInstance == 0) {
                    MenuLoadTask::fn_80117914(this, mSave);
                }
                break;
            case 2:
                fn_804A62C8(MenuCommonTask::sInstance, 1, 0);
                m7C = 2;
                break;
            case 3:
                fn_804A62C8(MenuCommonTask::sInstance, 10, 0);
                m7C = 2;
                break;
            }
            fn_80117228(1);
        } else if (fn_800A555C(pad, 0, 5)) {
            m7C = 0;
            fn_80117228(2);
        }
    } else if (m7C == 6) {
        if (MenuCommonTask::sInstance->m54 == 0) {
            m7C = 1;
            fn_80117228(1);
        }
    } else if (m7C == 0) {
        fn_80510D64(this, 1, 3);
        atn::D2A_Task* task = (atn::D2A_Task*)m50[0].mAnime->GetTask();
        float width;
        {
            std::basic_string<wchar_t> text = fn_800A4450(fn_801FEC74(task->mLayout, "string0004(1)(1)"), 0, 0);
            width = fn_8006EF48(lbl_8087EEC8, text.data(), 16.0f, 0.0f, 1, 1);
        }
        fn_801FED24(task->mLayout, fn_800DC6B4("TimePos"), 0, 18.0f + width);
        void* pad = lbl_8087EF70;
        if (fn_800A555C(pad, 0, 4)) {
            switch (mList[m80]->mId) {
            case 4:
                fn_8052DEEC(m48, 4);
                break;
            case 0:
                fn_8052DEEC(m48, 5);
                if (lbl_8087EFE8 != 0) {
                    lbl_8087EFE8->fn_800CFBA0(1, 1, 0x20, 30);
                    lbl_8087EFE8->fn_800CFBA0(2, 1, 0x20, 15);
                }
                break;
            case 1:
                fn_8052DEEC(m48, 7);
                if (lbl_8087EFE8 != 0) {
                    lbl_8087EFE8->fn_800CFBA0(1, 1, 0x20, 30);
                    lbl_8087EFE8->fn_800CFBA0(2, 1, 0x20, 15);
                }
                break;
            case 2:
                fn_8052DEEC(m48, 2);
                break;
            case 3:
                fn_8052DEEC(m48, 1);
                break;
            case 6:
                fn_8052DEEC(m48, 6);
                break;
            case 5:
                fn_8052DEEC(m48, 8);
                break;
            case 7:
                m7C = 1;
                mCursor = 2;
                mCanLoad = lbl_8087DCC8;
                if (mCanLoad != 0) {
                    mCursor = 1;
                }
                break;
            }
            fn_80117228(6);
        } else if (fn_800A555C(pad, 0, 5)) {
            fn_8052C9C4(m48);
        }
    }
}

void NewCampPartsTop::vf30() {
    mList.mSize = 0;
    for (int i = 0; i < 8; i++) {
        mEntries[i].mList->mVisible = 0;
        mEntries[i].mShine->mVisible = 0;
    }
    for (int i = 0; i < 8; i++) {
        mEntries[i].mList->mVisible = 1;
        mList.push_back(&mEntries[i]);
    }
    m84 = mList.size();
    m8C = mList.size();
    vf48();
}
