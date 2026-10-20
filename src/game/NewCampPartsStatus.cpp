#include "game/NewCampPartsStatus.h"
#include "atn/Memory.h"
#include "atn/SoundRef.h"
#include "atn/D3Anime.h"
#include "atn/D3AnimeInstance.h"
#include "game/CharaStatus.h"

extern "C" int sprintf(char* s, const char* format, ...);
extern "C" int strcmp(const char* a, const char* b);
extern "C" void fn_801F6E78(atn::Task* task, const char* name, const void* pos);
extern "C" void fn_801F6C80(atn::Task* task, const char* name, float value); // inferred: sets a value
extern "C" void fn_801F8598(atn::Task* task, const char* name, int value, int arg); // inferred: sets a number
extern "C" void fn_801FEE08(void* layout, unsigned int hash, const wchar_t* text); // inferred: sets a text
extern "C" void fn_801FECE0(void* layout, unsigned int hash, float value);
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
extern "C" const wchar_t* fn_80116FC0(int a, int id); // inferred: message text
extern "C" atn::SoundRef fn_80117228(int id);        // inferred: plays a system sound effect
extern "C" void fn_800D2338(atn::Task* task);        // inferred: deletes the task
extern "C" void* fn_8008937C(void* menu, const char* name); // inferred: debug menu group
extern "C" void fn_80087E9C(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void* lbl_8087EF70;
extern "C" int fn_800A555C(void* pad, int port, int button); // inferred: button triggered
extern "C" int fn_800A55AC(void* pad, int port, int button); // inferred: button repeated
extern "C" int fn_800A58D0(void* pad, int port);
extern "C" void fn_8052DEEC(atn::Task* camp, int id); // inferred: changes the parts of the camp
extern "C" unsigned int fn_80372574(GameUICounter_unk* list); // inferred: size of a fixed list

// inferred: d2anime task of the D3Anime objects and of "status_select"; only the members used here
struct NewCampStatusD2A_unk : public atn::Task {
    unsigned char m48[0x58 - 0x48];
    unsigned char mLayout[0x100 - 0x58]; // 0x58: inferred: layout
    float mFrame;                        // 0x100
    float mSpeed;                        // 0x104
};
// inferred: pane setters of a page task (names invented)
inline void SetTaskFloat_unk(atn::D2ATask_unk* task, const char* name, float value);
inline void SetTaskText_unk(atn::D2ATask_unk* task, const char* name, const wchar_t* text);
extern "C" NewCampStatusD2A_unk* fn_801F3FF8(atn::Task* parent, const char* path, int loop); // inferred: d2anime task
inline NewCampStatusD2A_unk* GetD2A_unk(atn::D3Anime* anime) {
    return (NewCampStatusD2A_unk*)anime->GetTask();
}
// inferred: functions on the d2anime task of a page (names unknown)
extern "C" void fn_803DD160(atn::D2ATask_unk* task, int arg);
extern "C" void fn_803D70B4(atn::D2ATask_unk* task, const char* name, const wchar_t* text); // inferred: sets a text
extern "C" void fn_803D6EF8(atn::D2ATask_unk* task, const char* name, float value);         // inferred: sets a value

// inferred: 2D point of a d2anime pane (0x14 bytes)
struct NewCampStatusPoint_unk {
    NewCampStatusPoint_unk() {}
    float x;
    float y;
    float m08;
    float m0C;
    float m10;
};
extern "C" NewCampStatusPoint_unk fn_801F4E8C(atn::Task* layout, unsigned int hash);

// inferred: model of an entry (atn::ModelInstance, src/atn/ModelInstance.cpp); only the members used here
namespace atn {
class ModelInstance {
public:
    void fn_8009EE30(const Matrix3x4& mtx); // inferred: sets the matrix
    unsigned char* m00;
    unsigned char* m04; // inferred: resource; its data starts at 0x10
    unsigned char m08[0x30 - 0x08];
    Matrix3x4 mMtx; // 0x30
    unsigned char m60[0x88 - 0x60];
    unsigned int mHidden : 1; // 0x88
};
} // namespace atn
struct NewCampPartsModel_unk : public atn::ModelInstance {};
// inferred: the null-checked cast of the model (name invented)
inline atn::ModelInstance* GetModel_unk(NewCampPartsModel_unk* model) {
    return model != 0 ? static_cast<atn::ModelInstance*>(model) : 0;
}
// inferred: data of the resource of the model (name invented)
inline unsigned char* GetModelData_unk(NewCampPartsModel_unk* model) {
    return model != 0 ? model->m04 + 0x10 : 0;
}

// inferred: functions of NewCampPartsBase (0x8050F93C-0x80512024) still called by their automatic names
extern "C" int fn_8050FD20(NewCampPartsBase* self); // inferred: these parts are the current ones
extern "C" void fn_8050FB38(NewCampPartsBase* self, int count, NewCampPartsEntry_unk* entries,
                            const NewCampPartsFile_unk* file, int a, int b);
extern "C" void fn_8050FC88(NewCampPartsBase* self, int count, NewCampPartsEntry_unk* entries, int a);
extern "C" void fn_8050FD38(NewCampPartsBase* self, NewCampScene_unk* scene, NewCampPartsEntry_unk* entries,
                            const NewCampPartsFile_unk* file, int count, const char* name);
extern "C" void fn_80510D64(NewCampPartsBase* self, int a, int b); // inferred: moves the cursor (m80/m84)
extern "C" void fn_80510E94(NewCampPartsBase* self, int a, int b);
extern "C" void fn_80511258(NewCampPartsBase* self, NewCampPartsEntry_unk* entries, atn::D3Anime* anime,
                            const atn::Vector3& pos, const atn::Vector3& scale, const char* name);
extern "C" void fn_805112A8(NewCampPartsBase* self, NewCampPartsEntry_unk* entries, atn::D3AnimeInstance* anime,
                            const atn::Vector3& pos, const atn::Vector3& scale, const char* name);
extern "C" void fn_805113E8(atn::D3Anime* anime, int a, int b, float c, float d);
extern "C" void fn_805114D4(atn::D3AnimeInstance* anime, int a, int b, float c, float d);
extern "C" void fn_805115D0(atn::Task* anime, int a, int b, float c, float d);
extern "C" void fn_805118B4(NewCampPartsBase* self, atn::Task* task, const char* name, NewCampPartsEntry_unk* entry,
                            const NewCampStatusPoint_unk& pos, int b); // inferred: cursor of a model
extern "C" void fn_80511D9C(atn::D2AInstance_unk* task, CharaStatus* status, int chara, int arg);

// inferred: camp task (the parent m48); only the members used here
struct NewCampStatusMember_unk {
    int mChara;
    int m04;
};
struct NewCampStatusCamp_unk {
    unsigned char m0000[0x6C];
    unsigned char m6C[0x464 - 0x6C];
    int m464; // inferred: mode of the camp
    unsigned char m0468[0x11F0 - 0x468];
    NewCampPartsBase* m11F0; // inferred: current parts
    unsigned char m11F4[0x11F8 - 0x11F4];
    int m11F8;
    unsigned char m11FC[0x1224 - 0x11FC];
    int mMemberCount;                    // 0x1224
    NewCampStatusMember_unk mMembers[1]; // 0x1228
};
// inferred: tutorial task made by fn_805B9F50; only the member used here
struct NewCampStatusTutorial_unk : public atn::Task {
    unsigned char m48[0x88 - 0x48];
    int m88; // inferred: state
};
extern "C" NewCampStatusTutorial_unk* fn_805B9F50(atn::Task* parent, int id, int arg); // inferred: starts a tutorial
extern "C" void fn_805BA354(NewCampStatusTutorial_unk* task, void* arg);
// inferred: tutorial entry (fn_8021F09C)
struct NewCampStatusHelp_unk {
    int mFlag;
    int mSeen;
};
extern "C" NewCampStatusHelp_unk* fn_8021F09C(int id);

// inferred: field (lbl_8087F430)
struct NewCampStatusField_unk;
extern "C" NewCampStatusField_unk* lbl_8087F430;
extern "C" void fn_803766E4(NewCampStatusField_unk* field);
extern "C" void fn_80376324(NewCampStatusField_unk* field, int id);

// inferred: characters (fn_8000D9E8 returns the manager lbl_8087F8A0)
class CharaTask {
public:
    CharaStatus* GetStatus_unk(); // weak copy 0x800F52F0, called out of line here
};
extern "C" void* fn_8000D9E8();
extern "C" CharaTask* fn_8054A33C(void* manager, int id); // inferred: character of a member
// inferred: party data (fn_801D80B4 returns lbl_8087F4F0); one 0x43C-byte block per member
struct NewCampStatusPartyMember_unk {
    unsigned char m000[0x43C];
};
struct NewCampStatusParty_unk {
    // inferred: status of a member (weak copy 0x805282A8)
    CharaStatus* GetStatus_unk(int i) { return (CharaStatus*)&mMembers[i]; }
    unsigned char m0000[0x64EC];
    NewCampStatusPartyMember_unk mMembers[1]; // 0x64EC
};
extern "C" NewCampStatusParty_unk* fn_801D80B4();
extern "C" void fn_801D80A0(NewCampStatusParty_unk* party, int chara);
// inferred: character name data (fn_8020924C)
struct NewCampStatusName_unk {
    unsigned char m00[8];
    const wchar_t* mName; // 0x08
    unsigned char m0C[0x14 - 0x0C];
    int m14;
};
extern "C" int fn_80219544(int chara);
extern "C" NewCampStatusName_unk* fn_8020924C(int id);
extern "C" int fn_8020A81C(int id);
CharaParam_unk fn_80134800(CharaStatus* status, int id, int kindA, int indexA, int itemA, int kindB, int indexB,
                           int itemB);
// inferred: message reference (weak copy of the getter 0x805282B8)
extern "C" const wchar_t lbl_808813D0[2]; // inferred: empty text
struct NewCampStatusText_unk {
    const wchar_t* Get_unk() { return mText != 0 ? mText : lbl_808813D0; }
    int m00;
    const wchar_t* mText;
};
// inferred: skill data (fn_8021AF50)
struct NewCampStatusSkillInfo_unk {
    unsigned char m00[8];
    NewCampStatusText_unk mName; // 0x08
    NewCampStatusText_unk mHelp; // 0x10
};
extern "C" NewCampStatusSkillInfo_unk* fn_8021AF50(int id);

class MenuCommonTask {
public:
    static MenuCommonTask* sInstance;
};
extern "C" void fn_804A3C24(MenuCommonTask* menu, const wchar_t* text, int id);
extern "C" void fn_804A5E40(MenuCommonTask* menu, int type, atn::Vector3 ofs, int id);
extern "C" void fn_804A4AEC(atn::D2ATask_unk* task, NewCampStatusName_unk* item, int cols, int rows);

extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector

static const NewCampPartsFile_unk sFiles[8] = {
    {"option_board01_0001", "status_logo.d2a", 0},
    {"option_board03_0002", "", 0},
    {"protector_in_board18_8001", "", 0},
    {"status_board01_0004", "", 1},
    {"status_board01_0003", "", 0},
    {"status_board01_0002", "", 1},
    {"status_board01_0001", "", 1},
    {"status_board01_0005", "", 1},
};
static const char* const sBoard = "status_board";
// inferred: the model moves with the page (MoveOffset)
static const int sMoveFlags[8] = {0, 0, 0, 1, 1, 1, 1, 1};
static const char* const sCursorNames[10] = {
    "lw_list_text_message_0001", "lw_list_text_message_0002", "lw_list_text_message_0003",
    "lw_list_text_message_0004", "lw_list_text_message_0005", "lw_list_text_message_0006",
    "lw_list_text_message_0007", "lw_list_text_message_0008", "lw_list_text_message_0009",
    "lw_list_text_message_0010",
};
// inferred: offset of the pages in mode 6 of the camp
static atn::Vector3 sOffset(500.0f, 0.0f, 0.0f);

// inferred: (b - a) * t + a (name invented)
inline atn::Vector3 LerpFrom_unk(const atn::Vector3& a, const atn::Vector3& b, float t) {
    return (b - a) * t + a;
}

inline NewCampStatusPoint_unk GetPanePos_unk(atn::Task* layout, const char* name) {
    return fn_801F4E8C(layout, fn_800DC6B4(name));
}
// inferred: set a text/value of the pane `name` (names invented)
inline void SetPaneText_unk(void* layout, const char* name, const wchar_t* text) {
    fn_801FEE08(layout, fn_800DC6B4(name), text);
}
inline void SetPaneFloat_unk(void* layout, const char* name, float value) {
    fn_801FECE0(layout, fn_800DC6B4(name), value);
}
inline void SetTaskFloat_unk(atn::D2ATask_unk* task, const char* name, float value) {
    unsigned int hash = fn_800DC6B4(name);
    fn_801FECE0(((NewCampStatusD2A_unk*)task)->mLayout, hash, value);
}
inline void SetTaskText_unk(atn::D2ATask_unk* task, const char* name, const wchar_t* text) {
    unsigned int hash = fn_800DC6B4(name);
    fn_801FEE08(((NewCampStatusD2A_unk*)task)->mLayout, hash, text);
}

NewCampPartsStatus* NewCampPartsStatus::fn_805268C8(atn::Task* parent) {
    if (parent != 0) {
        return new (1, "", "", 0) NewCampPartsStatus(parent);
    }
    return 0;
}

NewCampPartsStatus::NewCampPartsStatus(atn::Task* parent)
    : NewCampPartsBase(parent), mArrow(0.0f), mPage(0), mScroll(0.0f), mSkill(0), mOffset(0.0f, 0.0f, 0.0f),
      mMove(292.0f, 0.0f, 0.0f) {
    m98 = "status_board";
    m7C = 0;
    m4C = 8;
    if (m4C > 0) {
        m50.Reset_unk(new (1, "", "", 0) NewCampPartsEntry_unk[m4C]);
        fn_8050FB38(this, m4C, m50.mData, sFiles, 0, 0);
        fn_8050FC88(this, m4C, m50.mData, 1);
    }
    m5C.vf0C("camera/ui/status_camera.xsca");
    for (int i = 0; i < 7; i++) {
        mPages[i].mBase = atn::D3Anime::Create(this, "/data/d2anime/status_base.d2a", 1);
        mPages[i].mBase->fn_800D246C(1);
        mPages[i].mParam = atn::D3AnimeInstance::Create(this, "/data/d2anime/camp_status.d2a", 1);
        mPages[i].mParam->fn_800D246C(1);
    }
    (mSelect = fn_801F3FF8(this, "/data/d2anime/status_select.d2a", 0))->fn_800D246C(1);
    mTutorial = 0;
    mWait = 0;
}

NewCampPartsStatus::~NewCampPartsStatus() {}

int NewCampPartsStatus::vf4C() {
    return 0;
}

void NewCampPartsStatus::vf1C(NewCampScene_unk* scene) {
    fn_8050FD38(this, scene, m50.mData, sFiles, m4C, sBoard);
    m58 = -1.0f;
    for (int i = 0; i < 7; i++) {
        mPages[i].mBase->fn_800D246C(0);
        mPages[i].mParam->fn_800D246C(0);
    }
    vf3C(0);
}

void NewCampPartsStatus::vf10() {
    vf20();
    if (mScroll > 0.0f) {
        float v = mScroll - 0.2f;
        mScroll = 0.0f > v ? 0.0f : v;
    } else if (mScroll < 0.0f) {
        float v = 0.2f + mScroll;
        mScroll = 0.0f < v ? 0.0f : v;
    }
    if (mArrow > 0.0f) {
        float v = mArrow - 0.1f;
        mArrow = 0.0f > v ? 0.0f : v;
    } else if (mArrow < 0.0f) {
        float v = 0.1f + mArrow;
        mArrow = 0.0f < v ? 0.0f : v;
    }
}

// NONMATCHING: 99.70% - register allocation only (the show flag and the entry offset swap r30/r31, the
// source vector address and its copy swap r3/r4)
void NewCampPartsStatus::vf14() {
    int show = 0;
    if (fn_8050FD20(this) && ((NewCampStatusCamp_unk*)m48)->m11F8 != 1) {
        show = 1;
    }
    vf3C(show);
    const atn::Vector3* src = &lbl_807C7030;
    if (((NewCampStatusCamp_unk*)m48)->m464 == 6) {
        src = &sOffset;
    }
    atn::Vector3 target = *src;
    mOffset = LerpFrom_unk(mOffset, target, 0.2f);
    atn::Vector3 move = mMove * mScroll;
    for (int i = 0; i < m4C; i++) {
        NewCampPartsEntry_unk* entry = &m50[i];
        vf60(entry, 0, 7, 0.0f);
        if (GetModel_unk(entry->mModel) != 0) {
            atn::Matrix3x4 mtx = GetModel_unk(entry->mModel)->mMtx;
            atn::Vector3 pos = mtx.GetTrans_unk() + mOffset;
            if (sMoveFlags[i] != 0) {
                pos += move;
            }
            mtx.m[0][3] = pos.x;
            mtx.m[1][3] = pos.y;
            mtx.m[2][3] = pos.z;
            GetModel_unk(entry->mModel)->fn_8009EE30(mtx);
        }
        vf58(entry, "reference");
        vf54(entry, "reference");
        vf5C(entry);
    }
    if (m7C == 1) {
        int skill = mSkill;
        if (skill >= 0) {
            NewCampStatusSet_unk& set = mSets[mPage];
            if (skill < (int)set.mSkills.mSize && set.mSkills.mData[skill].mText != 0) {
                fn_804A3C24(MenuCommonTask::sInstance, set.mSkills.mData[skill].mText, 0);
            }
        }
    }
    NewCampPartsEntry_unk* cursor = &m50[4];
    if (show && GetModelData_unk(cursor->mModel) != 0 && GetModel_unk(cursor->mModel) != 0) {
        int page = mPage;
        NewCampStatusPage_unk* layout = &mPages[page];
        NewCampStatusSet_unk* set = &mSets[page];
        if (layout->mBase->GetTask() != 0) {
            char name[0x40];
            sprintf(name, "null_cursor");
            NewCampStatusPoint_unk pos = GetPanePos_unk(layout->mBase->GetTask(), name);
            fn_805118B4(this, 0, 0, cursor, pos, m7C != 0);
            if (m7C != 0) {
                int skill = mSkill;
                if (skill >= 0 && skill < (int)set->mSkills.mSize && set->mSkills.mData[skill].mTension != 0) {
                    skill = 9;
                }
                sprintf(name, "camp_cursor%d", skill + 1);
                NewCampStatusPoint_unk pos2 = GetPanePos_unk(layout->mBase->GetTask(), name);
                fn_805118B4(this, layout->mBase->GetTask(), sCursorNames[skill], cursor, pos2, 0);
            }
        }
    }
    if (mSelect != 0 && !m96) {
        if (show && (unsigned int)mSets.mSize >= 2 && m7C == 0) {
            mSelect->mSpeed = 1.0f;
        } else {
            mSelect->mSpeed = -1.0f;
        }
        float arrow = mArrow;
        if (arrow > 0.0f) {
            SetPaneFloat_unk(mSelect->mLayout, "arrowR", arrow);
            SetPaneFloat_unk(mSelect->mLayout, "arrowL", 0.0f);
        } else if (arrow < 0.0f) {
            SetPaneFloat_unk(mSelect->mLayout, "arrowR", 0.0f);
            SetPaneFloat_unk(mSelect->mLayout, "arrowL", -mArrow);
        } else {
            SetPaneFloat_unk(mSelect->mLayout, "arrowR", 0.0f);
            SetPaneFloat_unk(mSelect->mLayout, "arrowL", 0.0f);
        }
    }
    fn_80528BF0();
}

void NewCampPartsStatus::vf20() {
    if (!m96) {
        return;
    }
    int ready = 1;
    for (int i = 0; i < m4C; i++) {
        if (m50[i].mAnime != 0 && m50[i].mAnime->mState == 1) {
            ready = 0;
            break;
        }
    }
    for (int i = 0; i < 7; i++) {
        if (mPages[i].mBase->mState == 1) {
            ready = 0;
        }
        if (mPages[i].mParam->mState == 1) {
            ready = 0;
        }
    }
    if (!mSelect->IsFlag_unk(2)) {
        ready = 0;
    }
    if (!ready) {
        return;
    }
    for (int i = 0; i < m4C; i++) {
        fn_805113E8(m50[i].mAnime, 1, 0, 0.0f, 1.0f);
    }
    for (int i = 0; i < 7; i++) {
        fn_805113E8(mPages[i].mBase, 1, 0, 1000.0f, 0.0f);
        fn_805114D4(mPages[i].mParam, 1, 0, 0.0f, 0.0f);
    }
    fn_805115D0(mSelect, 1, 0, 0.0f, 1.0f);
    if (((NewCampStatusCamp_unk*)m48)->m11F0 != this) {
        vf3C(0);
    }
    vf48();
    fn_80527938();
    m96 = 0;
}

void NewCampPartsStatus::vf24() {
    vf3C(0);
    mPage = 0;
    mSkill = 0;
    m7C = 0;
    mOffset = lbl_807C7030;
    mSets.mSize = 0;
    NewCampStatusSet_unk set;
    for (int i = 0; i < ((NewCampStatusCamp_unk*)m48)->mMemberCount; i++) {
        set.mIndex = i;
        set.mChara = ((NewCampStatusCamp_unk*)m48)->mMembers[set.mIndex].mChara;
        mSets.push_back(set);
    }
    if (mTutorial != 0) {
        fn_800D2338(mTutorial);
        mTutorial = 0;
    }
    mWait = 0;
    m96 = 1;
}

void NewCampPartsStatus::vf28() {
    m7C = 0;
    m58 = -1.0f;
    m95 = 0;
    for (int i = 0; i < m4C; i++) {
        if (strcmp(sFiles[i].mAnime, "\0") == 0) {
            m50[i].mAnime = 0;
        }
    }
    if (mTutorial != 0) {
        fn_800D2338(mTutorial);
        mTutorial = 0;
    }
    mWait = 0;
    m96 = 0;
}

void NewCampPartsStatus::vf2C() {
    m95 = 0;
    m58 = 1.0f;
    switch (m7C) {
    case 0:
        fn_805289A8();
        if (fn_8050FD20(this) && ((NewCampStatusCamp_unk*)m48)->m11F8 != 1 && mPage >= 0 &&
            (unsigned int)mPage < (unsigned int)mSets.mSize && mSets[mPage].mSkills.mSize != 0) {
            fn_804A5E40(MenuCommonTask::sInstance, 0xF, lbl_807C7030, 0);
        }
        break;
    case 1:
        fn_80528A90();
        break;
    case 2:
        if (mTutorial == 0) {
            if (--mWait <= 0) {
                fn_805288BC(1);
            }
        } else if (mTutorial->IsFlag_unk(2) && mTutorial->m88 == 0) {
            fn_805BA354(mTutorial, ((NewCampStatusCamp_unk*)m48)->m6C);
        } else if (mTutorial->m88 == 4) {
            fn_800D2338(mTutorial);
            mTutorial = 0;
            mWait = 15;
        }
        break;
    }
}

void NewCampPartsStatus::vf30() {
    m95 = 1;
    mPage = 0;
    mSkill = 0;
    m7C = 0;
    fn_80527938();
}

// NONMATCHING: 99.40% - register allocation only (the page tasks, the character task and the status
// get other saved registers)
void NewCampPartsStatus::fn_80527938() {
    CharaTask* leaderTask = fn_8054A33C(fn_8000D9E8(), 0);
    CharaStatus* leader;
    if (leaderTask != 0) {
        leader = leaderTask->GetStatus_unk();
    } else {
        leader = fn_801D80B4()->GetStatus_unk(0);
    }
    for (unsigned int i = 0; i < fn_80372574((GameUICounter_unk*)&mSets); i++) {
        NewCampStatusSet_unk& set = mSets[i];
        ((GameUICounter_unk*)&set.mSkills)->fn_803CF734();
        atn::D2ATask_unk* base = mPages[i].mBase->GetTask();
        atn::D2AInstance_unk* param = mPages[i].mParam->GetTask();
        if (base == 0 || param == 0) {
            continue;
        }
        CharaTask* task = fn_8054A33C(fn_8000D9E8(), set.mChara);
        CharaStatus* status;
        if (task != 0) {
            status = task->GetStatus_unk();
        } else {
            status = fn_801D80B4()->GetStatus_unk(set.mChara);
        }
        NewCampStatusName_unk* info = fn_8020924C(fn_80219544(set.mChara));
        fn_801D80A0(fn_801D80B4(), set.mChara);
        fn_803DD160(base, fn_8020A81C(info->m14));
        fn_803D70B4(base, "name", info->mName);
        fn_803D70B4(base, "nameSub", info->mName);
        fn_804A4AEC(base, info, 4, 2);
        fn_80511D9C(param, status, set.mChara, task == 0);
        CharaParam_unk p = fn_80134800(status, set.mChara, 0, 0, 0, 0, 0, 0);
        fn_801F8598(param, "atk_param", (int)p.m00, 0);
        fn_801F8598(param, "def_param", (int)p.m08, 0);
        fn_801F8598(param, "matk_param", (int)p.m04, 0);
        fn_801F8598(param, "mdef_param", (int)p.m0C, 0);
        fn_801F8598(param, "agi_param", p.m94, 0);
        fn_801F6C80(param, "atk_updw_disp", 0.0f);
        fn_801F6C80(param, "def_updw_disp", 0.0f);
        fn_801F6C80(param, "matk_updw_disp", 0.0f);
        fn_801F6C80(param, "mdef_updw_disp", 0.0f);
        fn_801F6C80(param, "agi_updw_disp", 0.0f);
        char name[0x20];
        for (int j = 0; j < 9; j++) {
            sprintf(name, "skillOn%02d", j);
            fn_803D6EF8(base, name, 0.0f);
        }
        int count = 0;
        int tension = 0;
        int tensionHelp = -1;
        status->fn_8012CE4C(set.mChara);
        switch (set.mChara) {
        case 0:
            if (leader->fn_801346C8(9)) {
                fn_80528454(base, &count, &set);
            }
            fn_80528310(base, status, 0xA, &count, &set, 0x93);
            if (status->fn_801346C8(0x3F)) {
                fn_80528310(base, status, 0x3F, &count, &set, 0x14E);
            }
            fn_80528310(base, status, 0xD, &count, &set, 0x93);
            fn_80528310(base, status, 0x9, &count, &set, 0x152);
            fn_80528310(base, status, 0xE, &count, &set, 0x9D);
            fn_80528310(base, status, 0xF, &count, &set, 0xA4);
            fn_80528310(base, status, 0x10, &count, &set, 0xE1);
            fn_80528310(base, status, 0x2, &count, &set, 0xBE);
            tension = 0x1C;
            tensionHelp = 0xA7;
            break;
        case 1:
            if (status->fn_801346C8(0x32)) {
                fn_80528310(base, status, 0x32, &count, &set, -1);
            } else if (status->fn_801346C8(0x31)) {
                fn_80528310(base, status, 0x31, &count, &set, -1);
            }
            if (status->fn_801346C8(0x1A)) {
                fn_80528310(base, status, 0x1A, &count, &set, -1);
            } else if (status->fn_801346C8(0x19)) {
                fn_80528310(base, status, 0x19, &count, &set, -1);
            }
            tension = 0x3C;
            tensionHelp = 0xC5;
            break;
        case 2:
            if (leader->fn_801346C8(9)) {
                fn_80528454(base, &count, &set);
            }
            if (status->fn_801346C8(0x22)) {
                fn_80528310(base, status, 0x22, &count, &set, -1);
            } else {
                fn_80528310(base, status, 0x36, &count, &set, -1);
            }
            fn_80528310(base, status, 0x3, &count, &set, -1);
            tension = 0x24;
            tensionHelp = 0xC0;
            break;
        case 3:
            if (leader->fn_801346C8(9)) {
                fn_80528574(base, &count, &set);
            }
            fn_80528310(base, status, 0x36, &count, &set, -1);
            fn_80528310(base, status, 0x3, &count, &set, -1);
            tension = 0x35;
            tensionHelp = 0xC3;
            break;
        case 4:
            if (status->fn_801346C8(0x13)) {
                fn_80528310(base, status, 0x13, &count, &set, -1);
            } else if (status->fn_801346C8(0x12)) {
                fn_80528310(base, status, 0x12, &count, &set, -1);
            }
            fn_80528310(base, status, 0x3B, &count, &set, -1);
            tension = 0x29;
            tensionHelp = 0xC1;
            break;
        case 5:
            if (status->fn_801346C8(0x1A)) {
                fn_80528310(base, status, 0x1A, &count, &set, -1);
            } else if (status->fn_801346C8(0x19)) {
                fn_80528310(base, status, 0x19, &count, &set, -1);
            }
            if (status->fn_801346C8(0x39)) {
                fn_80528310(base, status, 0x39, &count, &set, -1);
            } else if (status->fn_801346C8(0x38)) {
                fn_80528310(base, status, 0x38, &count, &set, -1);
            }
            tension = 0x1;
            tensionHelp = 0xC2;
            break;
        case 6:
            if (status->fn_801346C8(0x17)) {
                fn_80528310(base, status, 0x17, &count, &set, -1);
            } else if (status->fn_801346C8(0x16)) {
                fn_80528310(base, status, 0x16, &count, &set, -1);
            }
            fn_80528310(base, status, 0x2C, &count, &set, -1);
            fn_80528310(base, status, 0x3, &count, &set, -1);
            tension = 0x27;
            tensionHelp = 0xC4;
            break;
        }
        if (status->fn_801346C8(tension)) {
            NewCampStatusSkillInfo_unk* skill = fn_8021AF50(tension);
            fn_803D70B4(base, "skill_tension", skill->mName.Get_unk());
            fn_803D6EF8(base, "tensionSkillOn", 1.0f);
            NewCampStatusSkill_unk entry;
            entry.mIndex = count;
            entry.mText = skill->mHelp.Get_unk();
            entry.mTension = 1;
            entry.mHelp = tensionHelp;
            set.mSkills.push_back(entry);
        } else {
            fn_803D70B4(base, "skill_tension", L"-");
            fn_803D6EF8(base, "tensionSkillOn", 0.0f);
        }
    }
}

// NONMATCHING: 96.47% - the original computes base + 0x58 at each call; here it is computed once (and the
// saved registers follow); tried member/free setters and evaluation orders
void NewCampPartsStatus::fn_80528310(atn::D2ATask_unk* base, CharaStatus* status, int skill, int* count,
                                     NewCampStatusSet_unk* set, int help) {
    if (status->fn_801346C8(skill)) {
        char name[0x20];
        sprintf(name, "skillOn%02d", *count);
        SetTaskFloat_unk(base, name, 1.0f);
        sprintf(name, "skill%02d", *count);
        NewCampStatusSkillInfo_unk* info = fn_8021AF50(skill);
        SetTaskText_unk(base, name, info->mName.Get_unk());
        NewCampStatusSkill_unk entry;
        entry.mIndex = *count;
        entry.mText = info->mHelp.Get_unk();
        entry.mTension = 0;
        entry.mHelp = help;
        set->mSkills.push_back(entry);
        (*count)++;
    }
}

// NONMATCHING: 93.11% - same layout address difference as fn_80528310
void NewCampPartsStatus::fn_80528454(atn::D2ATask_unk* base, int* count, NewCampStatusSet_unk* set) {
    char name[0x20];
    sprintf(name, "skillOn%02d", *count);
    SetTaskFloat_unk(base, name, 1.0f);
    sprintf(name, "skill%02d", *count);
    SetTaskText_unk(base, name, fn_80116FC0(1, 0x135));
    NewCampStatusSkill_unk entry;
    entry.mIndex = *count;
    entry.mText = fn_80116FC0(1, 0x13D);
    entry.mTension = 0;
    entry.mHelp = -1;
    set->mSkills.push_back(entry);
    (*count)++;
}

// NONMATCHING: 93.11% - same layout address difference as fn_80528310
void NewCampPartsStatus::fn_80528574(atn::D2ATask_unk* base, int* count, NewCampStatusSet_unk* set) {
    char name[0x20];
    sprintf(name, "skillOn%02d", *count);
    SetTaskFloat_unk(base, name, 1.0f);
    sprintf(name, "skill%02d", *count);
    SetTaskText_unk(base, name, fn_80116FC0(1, 0x136));
    NewCampStatusSkill_unk entry;
    entry.mIndex = *count;
    entry.mText = fn_80116FC0(1, 0x13E);
    entry.mTension = 0;
    entry.mHelp = -1;
    set->mSkills.push_back(entry);
    (*count)++;
}

void NewCampPartsStatus::fn_80528694() {
    void* pad = lbl_8087EF70;
    int page = mPage;
    m80 = page;
    m84 = mSets.mSize;
    fn_80510E94(this, 1, 0x11);
    mPage = m80;
    if (mPage != page) {
        if (fn_800A55AC(pad, 0, 0x17) || fn_800A55AC(pad, 0, 2)) {
            mScroll = -1.0f;
            mArrow = -1.0f;
        } else if (fn_800A55AC(pad, 0, 0x18) || fn_800A55AC(pad, 0, 3)) {
            mScroll = 1.0f;
            mArrow = 1.0f;
        }
    } else if (fn_800A58D0(pad, 0)) {
        if (fn_800A555C(pad, 0, 0xB)) {
            mPage = (mPage + 1) % m84;
            if (mPage != page) {
                fn_80117228(0x11);
                mScroll = 1.0f;
                mArrow = 1.0f;
            }
        } else if (fn_800A555C(pad, 0, 0xA)) {
            mPage = (m84 + mPage - 1) % m84;
            if (mPage != page) {
                fn_80117228(0x11);
                mScroll = -1.0f;
                mArrow = -1.0f;
            }
        }
    } else if (fn_800A555C(pad, 0, 6)) {
        mPage = (mPage + 1) % m84;
        if (mPage != page) {
            fn_80117228(0x11);
            mScroll = 1.0f;
            mArrow = 1.0f;
        }
    }
}

void NewCampPartsStatus::fn_805288BC(int state) {
    int prev = m7C;
    mPrevState = prev;
    m7C = state;
    switch (state) {
    case 1:
        if (prev == 0) {
            mSkill = 0;
        }
        break;
    case 2: {
        NewCampStatusHelp_unk* help = fn_8021F09C(mSets[mPage].mSkills.mData[mSkill].mHelp);
        if (help != 0) {
            if (help->mSeen != 0) {
                if (lbl_8087F430 != 0) {
                    fn_803766E4(lbl_8087F430);
                    fn_80376324(lbl_8087F430, help->mFlag);
                }
                fn_805288BC(1);
            } else {
                mTutorial = fn_805B9F50(this, help->mFlag, 0);
            }
        } else {
            fn_805288BC(1);
        }
        break;
    }
    }
}

void NewCampPartsStatus::fn_805289A8() {
    fn_80528694();
    void* pad = lbl_8087EF70;
    if (fn_800A555C(pad, 0, 5)) {
        fn_8052DEEC(m48, 0);
        fn_80117228(8);
    } else if (fn_800A555C(pad, 0, 4)) {
        if (mSets[mPage].mSkills.mSize == 0) {
            fn_80117228(0xB);
        } else {
            fn_805288BC(1);
            fn_80117228(1);
        }
    }
}

void NewCampPartsStatus::fn_80528A90() {
    void* pad = lbl_8087EF70;
    m80 = mSkill;
    m84 = mSets[mPage].mSkills.mSize;
    fn_80510D64(this, 1, 3);
    mSkill = m80;
    NewCampStatusHelp_unk* help = fn_8021F09C(mSets[mPage].mSkills.mData[mSkill].mHelp);
    if (help != 0) {
        fn_804A5E40(MenuCommonTask::sInstance, 8, lbl_807C7030, 0);
    }
    if (fn_800A555C(pad, 0, 4)) {
        if (help != 0) {
            fn_805288BC(2);
            fn_80117228(1);
        } else {
            fn_80117228(0xB);
        }
    }
    if (fn_800A555C(pad, 0, 5)) {
        fn_805288BC(0);
        fn_80117228(2);
    }
}

void NewCampPartsStatus::fn_80528BF0() {
    int page = mPage;
    int count = mSets.mSize;
    int prev = page - 1;
    if (prev < 0) {
        prev = count - 1;
    }
    int next = page + 1;
    if (next >= count) {
        next = 0;
    }
    for (int i = 0; i < (int)mSets.mSize; i++) {
        int slot;
        if (i == mPage) {
            slot = 4;
        } else if (i == next) {
            slot = 3;
        } else if (i == prev) {
            slot = 5;
        } else {
            mPages[i].mBase->mVisible = 0;
            mPages[i].mParam->mVisible = 0;
            continue;
        }
        NewCampPartsEntry_unk* entry = &m50[slot];
        NewCampStatusPoint_unk pos;
        pos.x = 0.0f;
        pos.y = 0.0f;
        pos.m08 = 0.0f;
        pos.m0C = 0.0f;
        pos.m10 = 0.0f;
        if (mPages[i].mBase->GetTask() != 0) {
            pos = GetPanePos_unk(mPages[i].mBase->GetTask(), "status_point");
        }
        if (mPages[i].mParam->GetTask() != 0) {
            fn_801F6E78(mPages[i].mParam->GetTask(), "basePos", &pos);
        }
        fn_80511258(this, entry, mPages[i].mBase, atn::Vector3(0.0f, 0.0f, 0.0f), atn::Vector3(1.0f, 1.0f, 1.0f),
                    "reference");
        fn_805112A8(this, entry, mPages[i].mParam, atn::Vector3(0.0f, 0.0f, 0.0f), atn::Vector3(1.0f, 1.0f, 1.0f),
                    "reference");
    }
}

// NONMATCHING: 98.36% - register allocation of the first loop (same difference as
// NewCampPartsHint::vf3C)
void NewCampPartsStatus::vf3C(int show) {
    for (int i = 0; i < m4C; i++) {
        if (m50[i].m3C) {
            if (m50[i].mAnime != 0) {
                m50[i].mAnime->mVisible = show;
            }
            if (m50[i].mModel != 0) {
                m50[i].mModel->mHidden = show == 0;
            }
        }
    }
    for (unsigned int i = 0; i < 7; i++) {
        int visible = show;
        if (i == mPage && !show) {
            visible = 1;
        }
        if (i >= mSets.mSize) {
            visible = 0;
        }
        mPages[i].mBase->mVisible = visible;
        mPages[i].mParam->mVisible = visible;
    }
}

void NewCampPartsStatus::vf40(void* group) {
    group = fn_8008937C(group, m98.data());
    fn_80087E9C(group, "MoveOffset", &mMove, 0, 0, -1000000.0f, 1000000.0f, 1.0f);
    fn_80087E9C(group, "OffsetPos", &mA4, 0, 0, -1000000.0f, 1000000.0f, 1.0f);
    fn_80087E9C(group, "OffsetScl", &mB0, 0, 0, -1000000.0f, 1000000.0f, 1.0f);
    fn_80087E9C(group, "LocatorScl", &mBC, 0, 0, -1000000.0f, 1000000.0f, 1.0f);
    fn_80087E9C(group, "BasePos", &mC8, 0, 0, -1000000.0f, 1000000.0f, 1.0f);
}

void NewCampPartsStatus::vf48() {
    float frame = 0.0f;
    for (int i = 0; i < m4C; i++) {
        if (m50[i].mAnime != 0 && m50[i].mAnime->GetTask() != 0) {
            SetPaneFloat_unk(GetD2A_unk(m50[i].mAnime)->mLayout, "frameOn", frame);
        }
    }
    for (int i = 0; i < 7; i++) {
        if (mPages[i].mBase != 0 && mPages[i].mBase->GetTask() != 0) {
            SetPaneFloat_unk(GetD2A_unk(mPages[i].mBase)->mLayout, "frameOn", frame);
        }
    }
}
