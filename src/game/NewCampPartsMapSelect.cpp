#include "game/NewCampPartsMapSelect.h"
#include "atn/Memory.h"
#include "atn/D3Anime.h"
#include "atn/D3AnimeInstance.h"
#include "std/string_members.h"
#include "atn/D2A_Task.h"
#include "atn/SoundRef.h"
#include "atn/SoundManager.h"
#include "game/SelectMember.h"
#include "game/MenuCommonTask.h"
#include "game/GameField_unk.h"
#include "game/GameScene.h"
#include "game/MapTask.h"
#include "game/TacView.h"

extern "C" void fn_800D2338(atn::Task* task); // inferred: deletes a task
extern "C" void fn_801FECE0(void* layout, unsigned int hash, float value);
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
extern "C" void fn_80530D88(atn::Task* camp, int a, atn::ModelBase* model);
// inferred: d2anime task of a D3Anime (as NewCampHintD2A_unk in game/NewCampPartsHint.cpp)
struct NewCampMapD2A_unk : public atn::Task {
    unsigned char m48[0x58 - 0x48];
    unsigned char mLayout[0x100 - 0x58]; // 0x58: inferred: layout
    float mFrame;                        // 0x100
    float mSpeed;                        // 0x104
};
inline NewCampMapD2A_unk* GetD2A_unk(atn::D3Anime* anime) {
    return (NewCampMapD2A_unk*)anime->GetTask();
}
inline void SetPaneFloat_unk(void* layout, const char* name, float value) {
    fn_801FECE0(layout, fn_800DC6B4(name), value);
}
// inferred: debug menu (atn/MindowsCommon.h; arguments in the order of game/NewCampPartsBase.cpp)
extern "C" void* fn_8008937C(void* menu, const char* name);
extern "C" void fn_80087E9C(void* group, const char* name, void* value, int arg3, int arg4, float min, float max,
                            float step); // x, y, z
extern "C" void fn_80087BB4(void* group, const char* name, void* value, int arg3, int arg4, float min, float max,
                            float step); // x, y
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);
extern "C" void fn_80088724(void* group, const char* name, void* value, int arg3, int arg4, float min, float max,
                            float step); // degrees
extern "C" void fn_80088AF4(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step); // colour
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
// inferred: camp task (the parent m48); only the members used here
struct NewCampMapCamp_unk {
    unsigned char m0000[0x48];
    int mState; // 0x48: inferred: state of the camp (2: map select opened from the field)
    int mId; // 0x4C: inferred: current camp
    unsigned char m0050[0x11F0 - 0x50];
    NewCampPartsBase* mParts; // 0x11F0: inferred: current parts
};
extern "C" void* lbl_8087F3C0;                                          // inferred: effect manager
extern "C" void fn_80239DAC(void* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_804A62C8(MenuCommonTask* menu, int type, int noCancel); // inferred: opens a dialog
extern "C" atn::SoundRef fn_80117228(int id);                   // inferred: plays a system sound effect
extern "C" void fn_800CB5B4(atn::SoundRef* sound, int a, float b);
extern "C" void fn_8036F268(GameField_unk* field, int type, int id, int subId, int entry, int a, int b, int reason,
                            int c); // inferred: changes the map
extern "C" void fn_8036FB4C(GameField_unk* field, int entry); // inferred: moves to an entry of the current map
extern "C" void fn_803743AC(GameField_unk* field);
extern "C" void fn_80530D30(atn::Task* camp, int a);
extern "C" void fn_8052DEEC(atn::Task* camp, int id); // inferred: changes the parts of the camp
extern "C" void fn_80530174(atn::Task* camp);
extern "C" void fn_8052CA6C(atn::Task* camp);
extern "C" void fn_805AA734(TacView* view); // inferred: closes the tactical view
extern "C" void fn_8052C9C4(atn::Task* camp);
extern "C" void fn_8053032C(atn::Task* camp, atn::Vector3 pos, atn::Vector3 rot);
extern "C" int fn_8037587C(GameField_unk* field, int type, int id, int subId); // inferred: the map was visited
extern "C" void* lbl_8087EF70;
extern "C" int fn_800A555C(void* pad, int port, int button); // inferred: button triggered
extern "C" int sprintf(char* s, const char* format, ...);
extern "C" void fn_805115D0(atn::Task* task, int a, int b, float c, float d);
extern "C" void fn_805113E8(atn::D3Anime* anime, int a, int b, float c, float d);
extern "C" void fn_805114D4(atn::D3AnimeInstance* anime, int a, int b, float c, float d);

static const NewCampPartsFile_unk sFiles[6] = {
    {"map_back01_0001", "", 0}, {"map_back02_0001", "", 0}, {"map_back03_0001", "", 0},
    {"map_back04_0001", "", 0}, {"map_back05_0001", "", 0}, {"map_back06_0001", "", 0},
};
static const char* const sBoard = "map_board";
static const char* const sBattleB = "map_board01_select_battle_b.d2a";
static const char* const sGoB = "map_board01_select_go_b.d2a";
static const char* const sCursorB = "map_board01_select_cursor_b.d2a";
static const char* const sBattle = "map_board01_select_battle.d2a";
static const char* const sGo = "map_board01_select_go.d2a";
// inferred: offset of the board (constructed by __sinit 0x80521DF8)
static atn::Vector3 sOffset(-1820.0f, 0.0f, -1432.0f);

// inferred: sets the texture `name` on the pane `pane` of the task of `anime` (vf20)
static inline void SetTexture_unk(atn::D3Anime* anime, const char* pane, const char* name) {
    if (strncmp(name, "", 1) != 0) {
        char path[0x80];
        sprintf(path, "data/texture/%s.texture", name);
        ((atn::D2A_Task*)anime->GetTask())->fn_801F4D80(pane, (int)path);
    }
}

// inferred: clears the model of an entry (vf1C)
inline void ResetEntry_unk(NewCampPartsEntry_unk& entry) {
    entry.mModel = 0;
    entry.mMtx.SetIdentity();
    entry.m3C = 0;
}

NewCampPartsMapSelect* NewCampPartsMapSelect::fn_8051B920(atn::Task* parent) {
    if (parent != 0) {
        return new ((MemoryTag)1, "", "", 0) NewCampPartsMapSelect(parent);
    }
    return 0;
}

// NONMATCHING: 92% - the arrays of the points and routes are not peeled in the original (first element
// stored before the loop here), the float temporaries and the colours
NewCampPartsMapSelect::NewCampPartsMapSelect(atn::Task* parent)
    : NewCampPartsBase(parent), mD4(0), mD5(0), mD6(0), mD7(0), mD8(0), mDC(1), m198C(0), m1990(0),
      m1998(4), m199C(4), m19A0(-1), m19A4(-1), m19A8(0), m19B4(0), m19B8(0.0f), m19BC(6.0f), m19C0(0.0f),
      m19CC(0), m3ED0(0), m46D4(0), m4800(sOffset), m4818(4.0f), m481C(20.0f),
      m4820(0.2f), m4824(0.12f), m487C(0.0f), m4880(2500.0f), m4884(0.0f), m48A8(0), m48AC(-200.0f),
      m48B0(-150.0f), m48B4(170.0f), m48B8(180.0f) {
    m1910.Reset_unk(0);
    mC8 = atn::Vector3(-1820.0f, 0.0f, -1432.0f);
    mBC = atn::Vector3(1.0f, 1.0f, 1.0f);
    m4828[0] = atn::Vector3(0.0f, 100.0f, 0.0f);
    m4828[1] = atn::Vector3(180.0f, 60.0f, 0.0f);
    m4828[2] = atn::Vector3(0.0f, 80.0f, 0.0f);
    m4828[3] = atn::Vector3(180.0f, 100.0f, 0.0f);
    m4828[4] = atn::Vector3(0.0f, 180.0f, -40.0f);
    m4888[0] = 0.3372549f;
    m4888[1] = 0.41568628f;
    m4888[2] = 0.47058824f;
    m4888[3] = 1.0f;
    m4898[0] = 0.24705882f;
    m4898[1] = 0.1764706f;
    m4898[2] = 0.15686275f;
    m4898[3] = 1.0f;
    float* angle = &m4828[0].x;
    for (int i = 0; i < 15; i++) {
        angle[i] *= 0.017453292f;
    }
    m98 = "map_board";
    mParam.vf0C("database/camp/mapSelect/map_param.csv");
    m190C = 6;
    if (m190C > 0) {
        m1910.Reset_unk(new ((MemoryTag)1, "", "", 0) NewCampPartsEntry_unk[m190C]);
        fn_8050FB38(m190C, m1910.mData, sFiles, 1, 0);
    }
    (mUiCursor = fn_801F3FF8(this, "/data/d2anime/ui_cursor.d2a", 0))->fn_800D246C(1);
    for (int i = 0; i < 32; i++) {
        mBattles[i].m04 = 1;
        (mBattles[i].mAnime = (atn::D3Anime*)atn::D3AnimeInstance::Create(this, sBattleB, 1))->fn_800D246C(1);
        mGoes[i].m04 = 1;
        (mGoes[i].mAnime = (atn::D3Anime*)atn::D3AnimeInstance::Create(this, sGoB, 1))->fn_800D246C(1);
        mCursors[i].m04 = 1;
        (mCursors[i].mAnime = (atn::D3Anime*)atn::D3AnimeInstance::Create(this, sCursorB, 1))->fn_800D246C(1);
    }
    (mBattle = atn::D3Anime::Create(this, sBattle, 1))->fn_800D246C(1);
    (mGo = atn::D3Anime::Create(this, sGo, 1))->fn_800D246C(1);
    (mBattleBoard = fn_801F3FF8(this, "map_board01_battle.d2a", 0))->fn_800D246C(1);
    (mLogo = fn_801F3FF8(this, "map_logo.d2a", 0))->fn_800D246C(1);
    m18F0 = atn::Vector3(0.0f, 0.0f, 0.0f);
    (mMapName = fn_801F3FF8(this, "/data/d2anime/map_name.d2a", 0))->fn_800D246C(1);
    m1900 = 0.0f;
    m5C.vf0C("camera/ui/map_board_camera.xsca");
    m47DC.LoadRequest("menu_map00.efp");
    m47E8.LoadRequest("be010_004.efp");
    m47F4.LoadRequest("be010_009.eff");
    m4864 = atn::Vector3(0.0f, 0.0f, -200.0f);
    m4870 = atn::Vector3(-10.0f, 0.0f, -280.0f);
    m46D8.clear();
    m1908 = 0;
}

NewCampPartsMapSelect::~NewCampPartsMapSelect() {
    FreeFiles_unk();
    if (m1908 != 0) {
        fn_800D2338(m1908);
        m1908 = 0;
    }
    m3ED0 = 0;
    m46D8.clear();
}

void NewCampPartsMapSelect::vf40(void* menu) {
    void* group = fn_8008937C(menu, m98.c_str());
    fn_80087E9C(group, "ZoomOffset", &m4864, 0, 0, -1000000.0f, 1000000.0f, 1.0f);
    fn_80087E9C(group, "ZoomOffset2", &m4870, 0, 0, -1000000.0f, 1000000.0f, 1.0f);
    fn_80087994(group, "CursorDebug", &m48A8, 0, 0);
    fn_80087BB4(group, "CursorMin", &m48AC, 0, 0, -1000000.0f, 1000000.0f, 1.0f);
    fn_80087BB4(group, "CursorMax", &m48B4, 0, 0, -1000000.0f, 1000000.0f, 1.0f);
    fn_80087E9C(group, "BackOffset", &m487C, 0, 0, -1000000.0f, 1000000.0f, 1.0f);
    void* color = fn_8008937C(group, "MaterialColor");
    fn_80088AF4(color, "Type0", m4888, 0, 0, 0.0f, 1.0f, 1.0f);
    fn_80088AF4(color, "Type1", m4898, 0, 0, 0.0f, 1.0f, 1.0f);
    void* rot = fn_8008937C(group, "EffectRot");
    fn_80088724(rot, "Ruri to Volno", &m4828[0], 0, 0, -360.0f, 360.0f, 1.0f);
    fn_80088724(rot, "Volno to Ruri", &m4828[1], 0, 0, -360.0f, 360.0f, 1.0f);
    fn_80088724(rot, "Volno to Grug", &m4828[2], 0, 0, -360.0f, 360.0f, 1.0f);
    fn_80088724(rot, "Grug to Volno", &m4828[3], 0, 0, -360.0f, 360.0f, 1.0f);
    fn_80088724(rot, "Volno to Volno", &m4828[4], 0, 0, -360.0f, 360.0f, 1.0f);
    void* cursor = fn_8008937C(group, "Cursor");
    fn_8008771C(cursor, "MoveSpeed", &m4818, 0, 0, 0.0f, 10000.0f, 1.0f);
    fn_8008771C(cursor, "LockLength", &m481C, 0, 0, 0.0f, 10000.0f, 1.0f);
    fn_8008771C(cursor, "LockSpeed", &m4820, 0, 0, 0.0f, 10000.0f, 1.0f);
    fn_8008771C(cursor, "LockRate", &m4824, 0, 0, 0.0f, 10000.0f, 1.0f);
    fn_80087E9C(group, "OffsetPos", &mA4, 0, 0, -1000000.0f, 1000000.0f, 1.0f);
    fn_80087E9C(group, "OffsetScl", &mB0, 0, 0, -1000000.0f, 1000000.0f, 1.0f);
    fn_80087E9C(group, "LocatorScl", &mBC, 0, 0, -1000000.0f, 1000000.0f, 1.0f);
    fn_80087E9C(group, "BasePos", &mC8, 0, 0, -1000000.0f, 1000000.0f, 1.0f);
}

// NONMATCHING: 95% - the original keeps i * 0x94 and i * 0x40 + 0x20 as induction variables
// NONMATCHING: the route search of the camp is kept (dead) in the original; the anime of the thumbnail is
// reloaded after sprintf there
inline void NewCampPartsMapSelect::SetCursorPoint_unk(MapSelectPoint_unk* point) {
    if (point != 0) {
        m480C = point->m18->mMtx.GetTrans_unk();
        m480C -= m4800;
        m19A8 = point->m10;
        m19B0 = point->m10;
    } else {
        m480C = lbl_807C7030;
    }
}

int NewCampPartsMapSelect::vf4C() {
    if (mParam.fn_80473F50()) {
        return 1;
    }
    if (mD4 == 0) {
        fn_80520634();
        mD4 = 1;
        m4C = m19CC;
        if (m4C > 0) {
            m50.Reset_unk(new ((MemoryTag)1, "", "", 0) NewCampPartsEntry_unk[m4C]);
            m198C = new ((MemoryTag)1, "", "", 0) NewCampPartsFile_unk[m4C];
            m1990 = new ((MemoryTag)1, "", "", 0) char[m4C * 2][0x20];
            memset(m198C, 0, m4C * sizeof(NewCampPartsFile_unk));
            memset(m1990, 0, m4C * 0x40);
        }
        for (int i = 0; i < m4C; i++) {
            m198C[i].mModel = m1990[i * 2];
            m198C[i].mAnime = m1990[i * 2 + 1];
            if (mPoints[i].m10 >= 0) {
                if (mPoints[i].m00 < 500) {
                    sprintf(m1990[i * 2], "map_board01_%04d", mPoints[i].m14);
                    sprintf(m1990[i * 2 + 1], "map_board01.d2a");
                } else {
                    sprintf(m1990[i * 2], "map_be010_001_%04d", mPoints[i].m14);
                    sprintf(m1990[i * 2 + 1], "");
                }
                m198C[i].m08 = 0;
                if (m198C[i].mAnime[0] != 0) {
                    NewCampPartsEntry_unk* entries = m50.mData;
                    entries[i].mAnime = atn::D3Anime::Create(this, m198C[i].mAnime, 1);
                    m50[i].mAnime->fn_800D246C(1);
                } else {
                    m50[i].mAnime = 0;
                }
                mPoints[i].m18 = &m50[i];
                if (mPoints[i].m04 > 0) {
                    m46D8.push_back(mPoints[i].m04);
                }
            }
        }
        fn_805115D0(mUiCursor, 1, 0, 0.0f, -1.0f);
        return 1;
    }
    if (!fn_800D3FA4()) {
        return 1;
    }
    if (m47DC.IsLoading()) {
        return 1;
    }
    if (m47E8.IsLoading()) {
        return 1;
    }
    return m47F4.IsLoading() != 0;
}

// NONMATCHING: 97% - the point loops start from `this` in the original (pointer from mPoints here)
void NewCampPartsMapSelect::vf1C(NewCampScene_unk* scene) {
    fn_8050FD38(scene, m1910.mData, sFiles, m190C, sBoard);
    fn_8050FD38(scene, m50.mData, m198C, m4C, sBoard);
    m80 = 0;
    {
        NewCampPartsEntry_unk* entries = mBattles;
        for (int i = 0; i < 32; i++) {
            ResetEntry_unk(entries[i]);
        }
    }
    {
        unsigned int i = 0;
        MapSelectPoint_unk* point = mPoints;
        for (; i < m19CC; i++, point++) {
            point->m1C = 0;
        }
    }
    {
        NewCampPartsEntry_unk* entries = mGoes;
        for (int i = 0; i < 32; i++) {
            ResetEntry_unk(entries[i]);
        }
    }
    {
        unsigned int i = 0;
        MapSelectPoint_unk* point = mPoints;
        for (; i < m19CC; i++, point++) {
            point->m20 = 0;
        }
    }
    {
        NewCampPartsEntry_unk* entries = mCursors;
        for (int i = 0; i < 32; i++) {
            ResetEntry_unk(entries[i]);
        }
    }
    {
        unsigned int i = 0;
        MapSelectPoint_unk* point = mPoints;
        for (; i < m19CC; i++, point++) {
            point->m24 = 0;
        }
    }
    for (int i = 0; i < 32; i++) {
        fn_805114D4((atn::D3AnimeInstance*)mBattles[i].mAnime, 0, 0, 0.0f, 0.0f);
        fn_805114D4((atn::D3AnimeInstance*)mGoes[i].mAnime, 0, 0, 0.0f, 0.0f);
        fn_805114D4((atn::D3AnimeInstance*)mCursors[i].mAnime, 0, 0, 0.0f, 0.0f);
    }
    fn_805115D0(mLogo, 0, 0, 0.0f, 0.0f);
    fn_805115D0(mBattleBoard, 0, 1, 0.0f, 1.0f);
    fn_805113E8(mBattle, 0, 1, 0.0f, 1.0f);
    fn_805113E8(mGo, 0, 1, 0.0f, 1.0f);
    fn_805115D0(mMapName, 1, 0, 0.0f, 1.0f);
    FreeFiles_unk();
    vf3C(0);
}

void NewCampPartsMapSelect::vf20() {
    if (m96) {
        for (int i = 0; i < m4C; i++) {
            fn_805113E8(m50[i].mAnime, 1, 1, 0.0f, 1.0f);
            if (m50[i].mAnime != 0 && m50[i].mAnime->GetTask() != 0) {
                SetTexture_unk(m50[i].mAnime, "thumbnail", mPoints[i].mThumbnail);
            }
        }
        if (((NewCampMapCamp_unk*)m48)->mParts != this) {
            vf3C(0);
        }
        vf48();
        m96 = 0;
        m1998 = 4;
        m199C = 4;
        fn_805201EC(&m19A4);
        m19A8 = 0;
        m19B0 = 0;
        int id = ((NewCampMapCamp_unk*)m48)->mId;
        if (FindRouteIndex_unk(id) > 0) {
            FindRoute_unk(id);
        }
        m19A0 = m19A4;
        SetCursorPoint_unk(FindPoint_unk(m19A0));
        m19B4 = 0;
        fn_80521908(1);
    }
}

void NewCampPartsMapSelect::fn_8051FB7C(int state) {
    m199C = m1998;
    m1998 = state;
    switch (state) {
    case 4:
        fn_80521908(1);
        break;
    case 5:
        fn_80521908(0);
        fn_80239DAC(lbl_8087F3C0, this, 1, 0);
        break;
    case 0:
        fn_80521A00(m19A8);
        break;
    case 1: {
        if (m1908 != 0) {
            fn_800D2338(m1908);
            m1908 = 0;
        }
        MapSelectRoute_unk* route = FindRoute_unk(m19A4);
        if (route != 0) {
            m1908 = SelectMember::fn_8057162C(this, route->mId, 0);
        } else {
            fn_8051FB7C(0);
        }
        break;
    }
    case 2:
        fn_804A62C8(MenuCommonTask::sInstance, 0, 1);
        break;
    case 3: {
        mDC = 2;
        m19C0 = 0.0f;
        if (lbl_8087EFE8 != 0) {
            lbl_8087EFE8->m34C8 = 0;
        }
        atn::SoundRef sound = fn_80117228(7);
        fn_800CB5B4(&sound, 0x37, 0.0f);
        if (lbl_8087EFE8 != 0) {
            lbl_8087EFE8->m34C8 = 1;
        }
        break;
    }
    }
}

void NewCampPartsMapSelect::fn_8051FD20() {
    void* pad = lbl_8087EF70;
    fn_805212BC();
    if (fn_800A555C(pad, 0, 5)) {
        if (((NewCampMapCamp_unk*)m48)->mState == 2) {
            fn_80530174(m48);
            fn_8052C9C4(m48);
        } else {
            fn_8052DEEC(m48, 0);
            fn_8053032C(m48, lbl_807C7030, lbl_807C7030);
            fn_80117228(8);
            mDC = 0;
            fn_8051FB7C(5);
        }
    } else if (fn_800A555C(pad, 0, 4)) {
        MapSelectRoute_unk* route = FindRoute_unk(m19A4);
        if (route != 0 && route->m08 == 2) {
            if (route->mId < 500) {
                if (fn_8037587C(lbl_8087F430, route->mMapType, route->mMapId, route->mMapSubId)) {
                    fn_8051FB7C(1);
                } else {
                    fn_8051FB7C(2);
                }
            } else {
                switch (route->mId - 500) { // inferred: special routes 500-503
                case 0:
                    m19B0 = 1;
                    break;
                case 1:
                    m19B0 = 0;
                    break;
                case 2:
                    m19B0 = 2;
                    break;
                case 3:
                    m19B0 = 0;
                    break;
                }
                fn_8051FB7C(5);
                fn_80117228(9);
            }
            fn_80117228(1);
        } else {
            fn_80117228(11);
        }
    }
}

void NewCampPartsMapSelect::vf30() {
    int id = m19A0;
    m19A4 = id;
    SetCursorPoint_unk(FindPoint_unk(id));
    mD5 = 0;
    m1998 = 4;
    fn_8051FB7C(4);
    m80 = 0;
    m19C0 = 0.0f;
    mDC = 1;
}

// NONMATCHING: volatile registers of the inlined point search (index/pointer/id)
void NewCampPartsMapSelect::fn_80520098() {
    if (lbl_8087F430 == 0) {
        return;
    }
    fn_80530D30(m48, 0);
    fn_8052DEEC(m48, 0);
    MapSelectRoute_unk* route = FindRoute_unk(m19A4);
    fn_80530174(m48);
    fn_8052CA6C(m48);
    lbl_8087F430->m563C = 0x12;
    if (fn_80373148(lbl_8087F430)->mType == 1 && route->mMapType == 1) {
        fn_8036FB4C(lbl_8087F430, route->mEntry);
        return;
    }
    if (lbl_8087F9F8 != 0 && lbl_8087F9F8->mA70 != 0) {
        fn_805AA734(lbl_8087F9F8);
        fn_803743AC(lbl_8087F430);
    }
    fn_8036F268(lbl_8087F430, route->mMapType, route->mMapId, route->mMapSubId, route->mEntry, 1, 0, 0x12, 1);
    if (lbl_8087F430->GetScene_unk() != 0) {
        lbl_8087F430->GetScene_unk()->FreezeTask();
    }
}

// NONMATCHING: float registers of the negated offset
void NewCampPartsMapSelect::fn_80521908(int reset) {
    if (reset) {
        m19B8 = 0.0f;
    }
    int dir = 0;
    for (int i = 0; i < m190C; i++) {
        switch (m19A8) {
        case 0:
            dir = 1;
            break;
        case 1:
            dir = -1;
            break;
        case 2:
            dir = -1;
            break;
        }
        if (dir < 0) {
            m1914[i].mPos = atn::Vector3(-m487C, -m4880, -m4884);
        } else {
            m1914[i].mPos = *(atn::Vector3*)&m487C;
        }
        if (reset) {
            m1914[i].mFrame = 0.0f;
            m1914[i].m10 = 0;
        }
    }
}

void NewCampPartsMapSelect::vf44() {
    for (int i = 0; i < m4C; i++) {
        if (GetModelData_unk(m50[i].mModel) != 0) {
            fn_80530D88(m48, 0, GetModelData_unk(m50[i].mModel));
        }
    }
}

void NewCampPartsMapSelect::vf48() {
    float frame = 0.0f;
    for (int i = 0; i < m4C; i++) {
        if (m50[i].mAnime != 0 && m50[i].mAnime->GetTask() != 0) {
            SetPaneFloat_unk(GetD2A_unk(m50[i].mAnime)->mLayout, "frameOn", frame);
        }
    }
}
