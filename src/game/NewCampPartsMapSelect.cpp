#include "game/NewCampPartsMapSelect.h"
#include "game/GameTask.h"
#include "game/NewCampTask.h"
#include "atn/Memory.h"
#include "atn/D3Anime.h"
#include "atn/D3AnimeInstance.h"
#include "std/string_members.h"
#include "atn/D2A_Task.h"
#include "atn/SoundRef.h"
#include "atn/SoundManager.h"
#include "game/SelectMember.h"
#include "game/MenuCommonTask.h"
#include "game/GameScene.h"
#include "game/MapTask.h"
#include "game/TacView.h"
#include "game/TownTask.h"
#include "atn/Csv.h"
#include "atn/FadeCommon.h"
#include "game/PlayerManager.h"
#include "atn/RenderManager.h"
#include "atn/DrawHelper.h"
#include "macros.h"

extern "C" GameTask* lbl_8087F430; // inferred: the field

extern "C" void fn_800D2338(atn::Task* task); // inferred: deletes a task
extern "C" void fn_801FECE0(void* layout, unsigned int hash, float value);
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
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
    unsigned char m11F4[0x1350 - 0x11F4];
    int m1350; // inferred: all the routes are shown (debug)
};
// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk {
    unsigned char m00[0xB8];
    int mB8;
    int mBC;
};
struct Color4f_unk {
    Color4f_unk() {}
    Color4f_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r, g, b, a;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_8023A680(EffectMgr_unk* mgr, atn::EffFile* res, int arg2, int arg3, atn::Model* model,
                            const atn::Vector3* pos, const atn::Vector3* rot, int arg7, int arg8, int arg9,
                            int arg10, float scale);
extern "C" void fn_80232B7C(void* owner, int arg);
// inferred: plays an effect with the default color and scale (name invented)
static inline void PlayEfp_unk(atn::EfpFile* efp, const atn::Vector3& pos, const atn::Vector3& rot) {
    fn_8023A8B4(lbl_8087F3C0, efp, 0, 0, pos, rot, Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f), -1, -1, 1, 1.0f);
}
// inferred: takes a free mark of `entries` for the entry of `point` (names invented)
inline void UseMark_unk(NewCampPartsEntry_unk* entries, NewCampPartsEntry_unk*& mark, MapSelectPoint_unk* point) {
    for (int i = 0; i < 32; i++) {
        NewCampPartsEntry_unk* entry = &entries[i];
        if (!entry->m3C) {
            mark = entry;
            entry->mModel = point->m18->mModel;
            entry->mMtx = point->m18->mMtx;
            entry->m3C = 1;
            return;
        }
    }
}
// inferred: route effects shown between two areas (fn_80520B04, fn_80521A00): the area of the cursor and
// the chapters (GameTask::m10D0) in which the route is open; point 500 + index; names invented
struct MapSelectRouteEffect_unk {
    int mArea;
    int mMin;
    int mMax;
};
static const MapSelectRouteEffect_unk sRouteEffects[5] = {
    {0, 201000, 999999}, {1, 201000, 999999}, {0, 408000, 999999}, {2, 408000, 999999}, {1, 204000, 999999},
};
extern "C" void fn_804A62C8(MenuCommonTask* menu, int type, int noCancel); // inferred: opens a dialog
extern "C" atn::SoundRef fn_80117228(int id);                   // inferred: plays a system sound effect
extern "C" void fn_800CB5B4(atn::SoundRef* sound, int a, float b);
extern "C" void fn_8036F268(GameTask* field, int type, int id, int subId, int entry, int a, int b, int reason,
                            int c); // inferred: changes the map
extern "C" void* lbl_8087EF70;
extern "C" int fn_800A555C(void* pad, int port, int button); // inferred: button triggered
extern "C" void PSMTXScale(Mtx m, float x, float y, float z);
extern "C" void PSMTXConcat(const Mtx a, const Mtx b, Mtx ab);
// inferred: mtx = mtx * scale(s) (same helper as game/Arms.cpp; name invented)
inline void Scale_unk(atn::Matrix3x4& mtx, const atn::Vector3& s) {
    atn::Matrix3x4 t;
    atn::Matrix3x4 m;
    PSMTXScale(m.m, s.x, s.y, s.z);
    PSMTXConcat(mtx.m, m.m, t.m);
    mtx = t;
}
// inferred: scales the model of an entry keeping its position (name invented)
inline void SetEntryScale_unk(NewCampPartsEntry_unk* entry, float scale) {
    if (entry != 0 && GetModel_unk(entry->mModel) != 0) {
        atn::Matrix3x4 mtx(GetModel_unk(entry->mModel)->mMtx);
        atn::Vector3 pos = mtx.GetTrans_unk();
        mtx.m[0][3] = 0.0f;
        mtx.m[1][3] = 0.0f;
        mtx.m[2][3] = 0.0f;
        Scale_unk(mtx, atn::Vector3(scale, scale, scale));
        mtx.m[0][3] = pos.x;
        mtx.m[1][3] = pos.y;
        mtx.m[2][3] = pos.z;
        GetModel_unk(entry->mModel)->fn_8009EE30(mtx);
    }
}
// inferred: route data of the map table (fn_80216544); names invented
struct MapSelectRouteData_unk {
    unsigned char mMapType;
    unsigned char mMapId;
    unsigned char mMapSubId;
    int mEntry;
};
extern "C" int fn_80216988();                // inferred: number of routes of the table
extern "C" int fn_80216954(int index);       // inferred: id of a route
extern "C" MapSelectRouteData_unk* fn_80216544(int id, int chapter, int flag);
extern "C" const char* fn_80216700(int id, int chapter, int flag); // inferred: state of the route
extern "C" PlayerManager* lbl_8087F8A0;
// inferred: the camp maps of Lazulis (the map select opens there with route 201); name invented
static inline int IsCastleCamp_unk(int type, int id) {
    int ret;
    if (type == 2 && (id == 15 || id == 55)) {
        ret = 1;
    } else {
        ret = 0;
    }
    return ret;
}
extern "C" int fn_800DC12C(const char* s); // inferred: atoi
extern "C" int strcmp(const char* a, const char* b);
extern "C" unsigned long strlen(const char* s);
// inferred: areas of the points in map_param.csv (names invented)
static const char* const sAreas[3] = {"MAIN", "VOLNO", "GURG"};
static inline int FindArea_unk(const char* name) {
    for (int i = 0; i < 3; i++) {
        if (strcmp(sAreas[i], name) == 0) {
            return i;
        }
    }
    return -1;
}
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
      m46D4(0), m4800(sOffset), m4818(4.0f), m481C(20.0f),
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
    mRoutes.clear();
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

// NONMATCHING: 95% - the original keeps i * 0x94 and i * 0x40 + 0x20 as induction variables
int NewCampPartsMapSelect::vf4C() {
    if (mParam.fn_80473F50()) {
        return 1;
    }
    if (mD4 == 0) {
        fn_80520634();
        mD4 = 1;
        m4C = mPoints.mSize;
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
            if (mPoints.mData[i].m10 >= 0) {
                // inferred: the names end with an extra NUL in the original pool (unreferenced "" after each)
                if (mPoints.mData[i].m00 < 500) {
                    sprintf(m1990[i * 2], "map_board01_%04d\0", mPoints.mData[i].m14);
                    sprintf(m1990[i * 2 + 1], "map_board01.d2a\0");
                } else {
                    sprintf(m1990[i * 2], "map_be010_001_%04d\0", mPoints.mData[i].m14);
                    sprintf(m1990[i * 2 + 1], "\0");
                }
                m198C[i].m08 = 0;
                if (m198C[i].mAnime[0] != 0) {
                    NewCampPartsEntry_unk* entries = m50.mData;
                    entries[i].mAnime = atn::D3Anime::Create(this, m198C[i].mAnime, 1);
                    m50[i].mAnime->fn_800D246C(1);
                } else {
                    m50[i].mAnime = 0;
                }
                mPoints.mData[i].m18 = &m50[i];
                if (mPoints.mData[i].m04 > 0) {
                    m46D8.push_back(mPoints.mData[i].m04);
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
        MapSelectPoint_unk* point = mPoints.mData;
        for (; i < mPoints.mSize; i++, point++) {
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
        MapSelectPoint_unk* point = mPoints.mData;
        for (; i < mPoints.mSize; i++, point++) {
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
        MapSelectPoint_unk* point = mPoints.mData;
        for (; i < mPoints.mSize; i++, point++) {
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

extern "C" const wchar_t* fn_802168BC(int id);                // inferred: name of a map point
extern "C" void* lbl_8087EEC8;                                  // inferred: font used to measure the text
extern "C" float fn_8006EF48(void* font, float size, const wchar_t* text, float f2, int a, int b); // inferred: width
extern "C" void fn_801FEE08(void* layout, unsigned int hash, const wchar_t* text); // inferred: sets a text
extern "C" void fn_801FED24(void* layout, unsigned int hash, int index, float value); // inferred: sets a vector item
// inferred: data of a map (only the revisit level is read)
struct MapSelectMapData_unk {
    unsigned char m00[0xC8];
    int mLevel; // 0xC8
};
extern "C" MapSelectMapData_unk* fn_8021771C(int type, int id, int arg); // inferred: data of a map
extern "C" void fn_804A5E40(MenuCommonTask* menu, int type, atn::Vector3 ofs, int id);
static wchar_t sNoName_unk[] = L"???"; // inferred: name of a point without text (lbl_80793590)
inline void SetPaneItem_unk(void* layout, const char* name, int index, float value) {
    fn_801FED24(layout, fn_800DC6B4(name), index, value);
}

// inferred: frame of the map: blinks the marks, raises the boards of the open points of the area under the cursor,
// shows the name and the revisit level of the point under the cursor and places the cursor on the screen
// NONMATCHING: 94.9% - the loop over the points keeps the index (stfsu)
void NewCampPartsMapSelect::vf10() {
    m19BC += 1.0f;
    if (m19BC > 35.0f) {
        m19BC = 6.0f;
    }
    if (fn_80521010() != 0) {
        return;
    }
    vf20();
    int shown = 0;
    if (((NewCampTask*)m48)->m458 >= 30.0f && ((NewCampTask*)m48)->m11F0 == this) {
        shown = 1;
    }
    for (int i = 0; i < m4C; i++) {
        MapSelectPoint_unk* point = &mPoints.mData[i];
        if (FindRoute_unk(point->m00) == 0 || m1998 == 4 || m1998 == 5 || m19A8 != point->m10 || !shown) {
            point->m28 -= 0.075f;
        } else {
            point->m28 += 0.075f;
        }
        point->m28 = MIN(1.0f, MAX(0.0f, point->m28));
    }
    if (shown) {
        if (m1998 == 5) {
            m19B8 -= 0.1f;
        } else {
            m19B8 += 0.1f;
        }
    } else {
        m19B8 -= 0.1f;
    }
    m19B8 = MIN(1.0f, MAX(0.0f, m19B8));
    MapSelectPoint_unk* cur = FindPoint_unk(m19A4);
    if (cur != 0 && m1998 == 0) {
        atn::Task* name = mMapName;
        int level = 0;
        MapSelectRoute_unk* route = FindRoute_unk(cur->m00);
        int visits = lbl_8087F430->fn_80370174(cur->m04);
        if (route != 0) {
            bool revisit = false;
            if (visits >= 100) {
                revisit = true;
            }
            if (route->mId == 0xCB) {
                revisit = true;
            }
            if (revisit && cur->m08 != 0) {
                if (route->mId == 0xCB) {
                    int chapter = lbl_8087F430->m10D0;
                    int lv;
                    if (chapter < 400000) {
                        lv = 1;
                    } else if (chapter < 600000) {
                        lv = lbl_8087F430->fn_80370174(200) == 0 ? 1 : 2;
                    } else {
                        lv = 4;
                    }
                    level = lv > 5 ? 5 : lv;
                } else {
                    MapSelectMapData_unk* data = fn_8021771C(route->mMapType, route->mMapId, 1);
                    if (data != 0) {
                        level = data->mLevel;
                    }
                }
            }
        }
        SetPaneFloat_unk(((NewCampMapD2A_unk*)name)->mLayout, "battle", 0.0f);
        if (cur->m08 > 0) {
            SetPaneFloat_unk(((NewCampMapD2A_unk*)name)->mLayout, "battle", 1.0f);
        }
        if (name != 0) {
            int chapter = lbl_8087F430->m10D0;
            const wchar_t* text = fn_802168BC(cur->m00);
            if (text != 0) {
                if (cur->m00 == 0x193) {
                    if (chapter == 205000) {
                        text = fn_802168BC(0x276);
                    }
                } else if (cur->m00 == 500) {
                    if (201000 <= chapter && chapter <= 202000) {
                        text = fn_802168BC(0x1F8);
                    }
                }
                fn_801FEE08(((NewCampMapD2A_unk*)name)->mLayout, fn_800DC6B4("mapName01"), text);
                m1900 = fn_8006EF48(lbl_8087EEC8, 16.0f, text, 0.0f, 1, 1);
            } else {
                fn_801FEE08(((NewCampMapD2A_unk*)name)->mLayout, fn_800DC6B4("mapName01"), sNoName_unk);
                m1900 = 48.0f;
            }
            SetPaneFloat_unk(((NewCampMapD2A_unk*)name)->mLayout, "msgLength", m1900);
            if (cur->m08 != 0) {
                m1900 += 18.0f;
            }
            if (level >= 1) {
                SetPaneFloat_unk(((NewCampMapD2A_unk*)name)->mLayout, "revisitLevel", 1.0f);
            } else {
                SetPaneFloat_unk(((NewCampMapD2A_unk*)name)->mLayout, "revisitLevel", 0.0f);
            }
            SetPaneFloat_unk(((NewCampMapD2A_unk*)name)->mLayout, "lv1", 1.0f);
            SetPaneFloat_unk(((NewCampMapD2A_unk*)name)->mLayout, "lv2", 0.0f);
            SetPaneFloat_unk(((NewCampMapD2A_unk*)name)->mLayout, "lv3", 0.0f);
            SetPaneFloat_unk(((NewCampMapD2A_unk*)name)->mLayout, "lv4", 0.0f);
            SetPaneFloat_unk(((NewCampMapD2A_unk*)name)->mLayout, "lv5", 0.0f);
            if (level >= 5) {
                SetPaneFloat_unk(((NewCampMapD2A_unk*)name)->mLayout, "lv5", 1.0f);
            }
            if (level >= 4) {
                SetPaneFloat_unk(((NewCampMapD2A_unk*)name)->mLayout, "lv4", 1.0f);
            }
            if (level >= 3) {
                SetPaneFloat_unk(((NewCampMapD2A_unk*)name)->mLayout, "lv3", 1.0f);
            }
            if (level >= 2) {
                SetPaneFloat_unk(((NewCampMapD2A_unk*)name)->mLayout, "lv2", 1.0f);
            }
        }
        m18FC += 0.2f;
        m18F0 = GetModel_unk(cur->m18->mModel)->mMtx.GetTrans_unk();
        m18F0 += atn::Vector3(-10.0f * m18FC, 0.0f, 0.0f);
    } else {
        m18FC -= 0.2f;
    }
    m18FC = MIN(1.0f, MAX(0.0f, m18FC));
    if (m1998 == 0) {
        if (mDC == 1) {
            fn_804A5E40(MenuCommonTask::sInstance, 2, lbl_807C7030, 0);
        } else {
            fn_804A5E40(MenuCommonTask::sInstance, 1, lbl_807C7030, 0);
        }
        ((NewCampMapD2A_unk*)mUiCursor)->mSpeed = 1.0f;
        if (((NewCampMapD2A_unk*)mUiCursor)->mFrame >= 5.0f) {
            ((NewCampMapD2A_unk*)mUiCursor)->mFrame = 5.0f;
        }
    } else {
        ((NewCampMapD2A_unk*)mUiCursor)->mSpeed = -1.0f;
    }
    atn::Vector3 screen = atn::RenderManager::instance_->GetScreenPosition(m480C + m4800);
    SetPaneItem_unk(((NewCampMapD2A_unk*)mUiCursor)->mLayout, "base_pos", 0, screen.x);
    SetPaneItem_unk(((NewCampMapD2A_unk*)mUiCursor)->mLayout, "base_pos", 1, screen.y);
}

// inferred: ranges of the background boards (m1910): area of the cursor and chapters (GameTask::m10D0) in which
// each board is shown (lbl_8075BE80); names invented
static const MapSelectRouteEffect_unk sBackRanges_unk[6] = {
    {0, 400000, 999999}, {0, 0, 400000}, {0, 0, 999999}, {1, 203000, 999999}, {0, 501000, 999999}, {2, 408000, 999999},
};
extern "C" void fn_801FEEFC(void* layout, unsigned int hash, int value); // inferred: sets an item of a pane
extern "C" void fn_801F6C80(atn::D2AInstance_unk* task, const char* name, float value); // inferred: sets a pane value
inline void SetPaneInt_unk(void* layout, const char* name, int value) {
    fn_801FEEFC(layout, fn_800DC6B4(name), value);
}
// inferred: offset of the camera of the camp (inline setters of NewCampTask; names invented)
inline void SetCampPos_unk(NewCampTask* camp, atn::Vector3 pos) {
    camp->m12D4 = pos;
}
inline void SetCampTarget_unk(NewCampTask* camp, atn::Vector3 pos) {
    camp->m12E0 = pos;
}
// inferred: moves the model of an entry by the lerp of `ofs` to zero (the boards rise with m19B8)
inline void MoveEntry_unk(NewCampPartsEntry_unk* entry, const atn::Vector3& ofs, float t) {
    atn::Matrix3x4 mtx(GetModel_unk(entry->mModel)->mMtx);
    atn::Vector3 pos = atn::Lerp_unk(ofs, lbl_807C7030, t);
    atn::Vector3 trans = mtx.GetTrans_unk() + pos;
    mtx.m[0][3] = trans.x;
    mtx.m[1][3] = trans.y;
    mtx.m[2][3] = trans.z;
    GetModel_unk(entry->mModel)->fn_8009EE30(mtx);
}
// inferred: shows a mark of a point (battle/go/cursor), scaled as the d2anime of the board
inline void ShowMark_unk(NewCampPartsMapSelect* self, NewCampPartsEntry_unk* mark) {
    mark->mAnime->mVisible = 1;
    fn_801F6C80(((atn::D3AnimeInstance*)mark->mAnime)->GetTask(), "wRate", 1.18f);
    fn_801F6C80(((atn::D3AnimeInstance*)mark->mAnime)->GetTask(), "hRate", 1.28f);
}

// inferred: draw of the map: hides the marks, then either the boards of the points while the routes are being
// placed, or the boards of the open points of the area with their marks, the background boards, the name of the
// point under the cursor and the offset of the camera of the camp
// NONMATCHING: 92.4% - registers and scheduling of the matrix temporaries (SetEntryScale_unk, MoveEntry_unk) and
// of the camera offset
void NewCampPartsMapSelect::vf14() {
    if (m4C <= 0) {
        return;
    }
    for (int i = 0; i < 32; i++) {
        if (mBattles[i].mAnime != 0) {
            mBattles[i].mAnime->mVisible = 0;
        }
        if (mGoes[i].mAnime != 0) {
            mGoes[i].mAnime->mVisible = 0;
        }
        if (mCursors[i].mAnime != 0) {
            mCursors[i].mAnime->mVisible = 0;
        }
    }
    int moving = 0;
    mBattleBoard->FreezeTask();
    mLogo->FreezeTask();
    mBattle->mVisible = 0;
    mGo->mVisible = 0;
    if (((NewCampTask*)m48)->m458 >= 0.1f) {
        moving = 1;
    }
    if (fn_8050FD20() && ((NewCampTask*)m48)->m11F8 != 1) {
        mLogo->MeltTask();
    }
    if (fn_80521010()) {
        for (int i = 0; i < m4C; i++) {
            NewCampPartsEntry_unk* entry = &m50[i];
            if (entry->mAnime != 0) {
                entry->mAnime->mVisible = 1;
            }
            if (entry->mModel != 0) {
                entry->mModel->mHidden = 0;
                SetEntryScale_unk(entry, 0.0f);
                vf54(entry, "reference");
                vf5C(entry);
            }
        }
        for (int i = 0; i < m190C; i++) {
            NewCampPartsEntry_unk* entry = &m1910[i];
            if (entry->mAnime != 0) {
                entry->mAnime->mVisible = 1;
            }
            if (entry->mModel != 0) {
                entry->mModel->mHidden = 0;
                SetEntryScale_unk(entry, 0.0f);
                vf54(entry, "reference");
                vf5C(entry);
            }
        }
        return;
    }
    for (int i = 0; i < m4C; i++) {
        NewCampPartsEntry_unk* entry = &m50[i];
        MapSelectPoint_unk* point = &mPoints.mData[i];
        MapSelectRoute_unk* route = FindRoute_unk(point->m00);
        if (route == 0 || m19A8 != point->m10) {
            if (entry->mModel != 0) {
                entry->mModel->mHidden = 1;
            }
            if (entry->mAnime != 0) {
                entry->mAnime->mVisible = 0;
            }
            continue;
        }
        if (entry->mModel != 0) {
            entry->mModel->mHidden = 0;
        }
        if (entry->mAnime != 0) {
            entry->mAnime->mVisible = 1;
        }
        vf60(entry, 0, 7, 0.0f);
        MoveEntry_unk(entry, m1914[0].mPos, m19B8);
        SetEntryScale_unk(&m50[i], point->m28);
        vf54(entry, "reference");
        vf5C(entry);
        if (entry->mAnime != 0 && entry->mAnime->GetTask() != 0) {
            SetPaneInt_unk(((NewCampMapD2A_unk*)entry->mAnime->GetTask())->mLayout, "blend0002", 0);
            SetPaneFloat_unk(((NewCampMapD2A_unk*)entry->mAnime->GetTask())->mLayout, "icon", 0.0f);
            if (route != 0 && route->m08 == 1) {
                SetPaneInt_unk(((NewCampMapD2A_unk*)entry->mAnime->GetTask())->mLayout, "blend0002", -1);
            }
        }
        if (point->m1C != 0) {
            ShowMark_unk(this, point->m1C);
            vf54(point->m1C, "reference");
            vf5C(point->m1C);
            if (m19A4 == point->m00 && m1998 == 0) {
                if (mBattle != 0 && mBattle->GetTask() != 0) {
                    mBattle->mVisible = 1;
                    SetPaneFloat_unk(GetD2A_unk(mBattle)->mLayout, "wRate", 1.18f);
                    SetPaneFloat_unk(GetD2A_unk(mBattle)->mLayout, "hRate", 1.28f);
                    fn_80511258(point->m18, mBattle, atn::Vector3(0.0f, 0.0f, 0.0f), atn::Vector3(1.0f, 1.0f, 1.0f),
                                "reference");
                }
                if (point->m18->mAnime != 0 && point->m18->mAnime->GetTask() != 0) {
                    mBattleBoard->MeltTask();
                    atn::D2A_DrawRect p = fn_801F4E8C((atn::D2A_Task*)point->m18->mAnime->GetTask(), fn_800DC6B4("new_point"));
                    atn::Vector3 pos(p.x, p.y, 0.0f);
                    PSMTXMultVec(point->m18->mAnime->mMtx.m, &pos, &pos);
                    pos = atn::RenderManager::instance_->GetScreenPosition(pos);
                    SetPaneItem_unk(((NewCampMapD2A_unk*)mBattleBoard)->mLayout, "base_point", 0, pos.x);
                    SetPaneItem_unk(((NewCampMapD2A_unk*)mBattleBoard)->mLayout, "base_point", 1, pos.y);
                }
            }
        }
        if (point->m20 != 0) {
            ShowMark_unk(this, point->m20);
            vf54(point->m20, "reference");
            vf5C(point->m20);
            if (m19A4 == point->m00 && m1998 == 0 && mGo != 0 && mGo->GetTask() != 0) {
                mGo->mVisible = 1;
                SetPaneFloat_unk(GetD2A_unk(mGo)->mLayout, "wRate", 1.18f);
                SetPaneFloat_unk(GetD2A_unk(mGo)->mLayout, "hRate", 1.28f);
                fn_80511258(point->m18, mGo, atn::Vector3(0.0f, 0.0f, 0.0f), atn::Vector3(1.0f, 1.0f, 1.0f),
                            "reference");
            }
        }
        if (point->m24 != 0) {
            point->m24->mAnime->mVisible = 1;
            ((atn::D3AnimeInstance*)point->m24->mAnime)->GetTask()->mSpeed = 0.0f;
            ((atn::D3AnimeInstance*)point->m24->mAnime)->GetTask()->mFrame = 0.0f;
            fn_801F6C80(((atn::D3AnimeInstance*)point->m24->mAnime)->GetTask(),
                        "wRate", 1.18f);
            fn_801F6C80(((atn::D3AnimeInstance*)point->m24->mAnime)->GetTask(),
                        "hRate", 1.28f);
            vf54(point->m24, "reference");
            vf5C(point->m24);
        }
    }
    int chapter = lbl_8087F430->m10D0;
    for (int i = 0; i < m190C; i++) {
        NewCampPartsEntry_unk* entry = &m1910[i];
        if (entry->mModel == 0) {
            continue;
        }
        const MapSelectRouteEffect_unk* range = &sBackRanges_unk[i];
        if (range->mMin <= chapter && range->mMax > chapter && m19A8 == range->mArea) {
            entry->mModel->mHidden = 0;
        } else {
            entry->mModel->mHidden = 1;
        }
        vf60(entry, 0, 7, 0.0f);
        MoveEntry_unk(entry, m1914[i].mPos, m19B8);
        vf54(entry, "reference");
        vf5C(entry);
    }
    if (m18FC <= 0.0f) {
        mMapName->FreezeTask();
    } else {
        atn::Vector3 scale(m18FC, m18FC, m18FC);
        atn::Vector3 screen = atn::RenderManager::instance_->GetScreenPosition(m18F0);
        mMapName->MeltTask();
        SetPaneItem_unk(((NewCampMapD2A_unk*)mMapName)->mLayout, "basePos", 0, screen.x);
        SetPaneItem_unk(((NewCampMapD2A_unk*)mMapName)->mLayout, "basePos", 1, screen.y);
        SetPaneItem_unk(((NewCampMapD2A_unk*)mMapName)->mLayout, "basePos", 2, scale.x);
        SetPaneItem_unk(((NewCampMapD2A_unk*)mMapName)->mLayout, "basePos", 3, scale.y);
    }
    if (fn_8050FD20()) {
        if (moving) {
            atn::Vector3 ofs(0.0f, 0.0f, 0.0f);
            if (mDC == 0) {
                if (m19A8 == 0 && lbl_8087F430->m10D0 >= 400000) {
                    ofs += atn::Vector3(0.0f, 60.0f, 0.0f);
                    if (((NewCampTask*)m48)->m136C == 0) {
                        ofs += atn::Vector3(0.0f, 0.0f, 60.0f);
                    }
                }
            } else if (mDC == 1) {
                ofs = m480C;
                ofs += m4864;
            } else if (mDC == 2) {
                ofs = m480C;
                ofs += m4864;
                ofs += atn::Vector3(5.0f, 0.0f, 8.0f * -m19C0);
            }
            SetCampPos_unk((NewCampTask*)m48, ofs);
            SetCampTarget_unk((NewCampTask*)m48, ofs);
        } else {
            atn::Vector3 zero(0.0f, 0.0f, 0.0f);
            SetCampPos_unk((NewCampTask*)m48, zero);
            SetCampTarget_unk((NewCampTask*)m48, zero);
        }
    } else {
        atn::Vector3 zero(0.0f, 0.0f, 0.0f);
        SetCampPos_unk((NewCampTask*)m48, zero);
        SetCampTarget_unk((NewCampTask*)m48, zero);
    }
    if (m48A8 != 0) {
        atn::Vector3 center(0.5f * (m48B4 + m48AC), 0.5f * (m48B8 + m48B0), 0.0f);
        atn::Vector3 size(0.5f * (m48B4 - m48AC), 0.5f * (m48B8 - m48B0), 10.0f);
        center += m4800;
        lbl_8087EEB0->fn_800629F0(center, size, 0.0f, 0xFFFF0000, 0);
    }
}

// NONMATCHING: 89% - the route search of the camp is kept (dead) in the original; the anime of the thumbnail is
// reloaded after sprintf there
void NewCampPartsMapSelect::vf20() {
    if (m96) {
        for (int i = 0; i < m4C; i++) {
            fn_805113E8(m50[i].mAnime, 1, 1, 0.0f, 1.0f);
            if (m50[i].mAnime != 0 && m50[i].mAnime->GetTask() != 0) {
                SetTexture_unk(m50[i].mAnime, "thumbnail", mPoints.mData[i].mThumbnail);
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

void NewCampPartsMapSelect::vf24() {
    mD8 = 1;
    m19B4 = 0;
    fn_80521908(1);
    fn_80239DAC(lbl_8087F3C0, this, 1, 0);
    if (m1908 != 0) {
        fn_800D2338(m1908);
        m1908 = 0;
    }
    vf3C(0);
    m96 = 1;
    mD6 = 0;
    mD7 = 0;
    mDC = 0;
    m19A8 = 0;
    m19B0 = 0;
    m19C0 = 0.0f;
    for (int i = 0; i < m4C; i++) {
        mPoints.mData[i].m28 = 0.0f;
        SetEntryScale_unk(&m50[i], 0.0f);
    }
}

// inferred: answer of the dialog of fn_8051FB7C (state 2); name invented
inline void NewCampPartsMapSelect::Answer_unk() {
    if (MenuCommonTask::sInstance->m68 == 0) {
        fn_80117228(1);
        fn_8051FB7C(3);
    } else {
        fn_80117228(2);
        fn_8051FB7C(m199C);
    }
}

// inferred: clears the thumbnail of a point (white texture)
static inline void ClearTexture_unk(atn::D3Anime* anime, const char* pane, const char* name) {
    if (strncmp(name, "", 1) != 0) {
        char path[0x80];
        sprintf(path, "data/texture/sys_white.texture");
        ((atn::D2A_Task*)anime->GetTask())->fn_801F4D80(pane, (int)path);
    }
}

// inferred: resets the map (cursor, routes, marks, thumbnails and boards of the points)
// NONMATCHING: 96% - the anime of the thumbnail is reloaded after sprintf in the original (as vf20); loops over
// the points addressed from this
void NewCampPartsMapSelect::vf28() {
    m1998 = 4;
    m199C = 4;
    if (m1908 != 0) {
        fn_800D2338(m1908);
        m1908 = 0;
    }
    mD8 = 0;
    mRoutes.clear();
    memset(m45D4.m00, 0, sizeof(m45D4.m00));
    m46D4 = 0;
    mDC = 0;
    m19A8 = 0;
    m19B0 = 0;
    m19C0 = 0.0f;
    ((NewCampTask*)m48)->fn_8053032C(lbl_807C7030, lbl_807C7030);
    m19B4 = 0;
    fn_80521908(1);
    fn_80239DAC(lbl_8087F3C0, this, 1, 0);
    {
        NewCampPartsEntry_unk* entries = mBattles;
        for (int i = 0; i < 32; i++) {
            ResetEntry_unk(entries[i]);
        }
    }
    {
        unsigned int i = 0;
        MapSelectPoint_unk* point = mPoints.mData;
        for (; i < mPoints.mSize; i++, point++) {
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
        MapSelectPoint_unk* point = mPoints.mData;
        for (; i < mPoints.mSize; i++, point++) {
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
        MapSelectPoint_unk* point = mPoints.mData;
        for (; i < mPoints.mSize; i++, point++) {
            point->m24 = 0;
        }
    }
    m18FC = 0.0f;
    for (int i = 0; i < m4C; i++) {
        fn_805113E8(m50[i].mAnime, 1, 1, 0.0f, 1.0f);
        if (m50[i].mAnime != 0 && m50[i].mAnime->GetTask() != 0) {
            ClearTexture_unk(m50[i].mAnime, "thumbnail", mPoints.mData[i].mThumbnail);
        }
    }
    m96 = 0;
    for (int i = 0; i < m4C; i++) {
        mPoints.mData[i].m28 = 0.0f;
        SetEntryScale_unk(&m50[i], mPoints.mData[i].m28);
    }
}

// NONMATCHING: 95.8% - scheduling of the inlined SetCursorPoint_unk (same as vf30)
void NewCampPartsMapSelect::vf2C() {
    if (fn_80521010()) {
        return;
    }
    if (mD5) {
        fn_80520098();
        return;
    }
    switch (m1998) {
    case 4:
        if (m19B8 >= 1.0f) {
            fn_8051FB7C(0);
        }
        break;
    case 5:
        if (m19B8 <= 0.0f) {
            switch (m19A8) {
            case 0:
                if (m19B0 == 1) {
                    m19A4 = 501;
                } else {
                    m19A4 = 503;
                }
                break;
            case 1:
                m19A4 = 500;
                break;
            case 2:
                m19A4 = 502;
                break;
            }
            m19A8 = m19B0;
            SetCursorPoint_unk(FindPoint_unk(m19A4));
            fn_8051FB7C(4);
        }
        break;
    case 0:
        fn_8051FD20();
        break;
    case 1:
        if (m1908 == 0) {
            fn_8051FB7C(0);
        } else if (m1908->mState == 1) {
            if (m1908->mDecided) {
                fn_8051FB7C(2);
            } else {
                fn_8051FB7C(0);
            }
            if (m1908 != 0) {
                fn_800D2338(m1908);
                m1908 = 0;
            }
        }
        break;
    case 2:
        Answer_unk();
        break;
    case 3:
        m19C0 += 1.0f;
        if (15.0f <= m19C0 && m19C0 < 16.0f) {
            ((NewCampTask*)m48)->fn_80530CD0(15);
        } else if (30.0f <= m19C0 && lbl_8087F430 != 0) {
            lbl_8087F430->m5624->m78 = 0x12;
            lbl_8087F430->m5624->Capture();
            mD5 = 1;
        }
        break;
    }
    mD7 = 1;
    if (fn_800A555C(lbl_8087EF70, 0, 6)) {
        fn_80117228(0x21);
        mDC++;
        if (mDC > 1) {
            mDC = 0;
        }
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
            ((NewCampTask*)m48)->fn_80530174();
            ((NewCampTask*)m48)->fn_8052C9C4();
        } else {
            ((NewCampTask*)m48)->fn_8052DEEC(0);
            ((NewCampTask*)m48)->fn_8053032C(lbl_807C7030, lbl_807C7030);
            fn_80117228(8);
            mDC = 0;
            fn_8051FB7C(5);
        }
    } else if (fn_800A555C(pad, 0, 4)) {
        MapSelectRoute_unk* route = FindRoute_unk(m19A4);
        if (route != 0 && route->m08 == 2) {
            if (route->mId < 500) {
                if (lbl_8087F430->fn_8037587C(route->mMapType, route->mMapId, route->mMapSubId)) {
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

// NONMATCHING: 95% - volatile registers of the inlined point search (index/pointer/id)
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

void NewCampPartsMapSelect::fn_80520098() {
    if (lbl_8087F430 == 0) {
        return;
    }
    ((NewCampTask*)m48)->fn_80530D30(0);
    ((NewCampTask*)m48)->fn_8052DEEC(0);
    MapSelectRoute_unk* route = FindRoute_unk(m19A4);
    ((NewCampTask*)m48)->fn_80530174();
    ((NewCampTask*)m48)->fn_8052CA6C();
    lbl_8087F430->m563C = 0x12;
    if (lbl_8087F430->GetCurrentMap()->mType == 1 && route->mMapType == 1) {
        lbl_8087F430->fn_8036FB4C(route->mEntry);
        return;
    }
    if (lbl_8087F9F8 != 0 && lbl_8087F9F8->mA70 != 0) {
        lbl_8087F9F8->fn_805AA734();
        lbl_8087F430->fn_803743AC();
    }
    fn_8036F268(lbl_8087F430, route->mMapType, route->mMapId, route->mMapSubId, route->mEntry, 1, 0, 0x12, 1);
    if (lbl_8087F430->m10D8 != 0) {
        lbl_8087F430->m10D8->FreezeTask();
    }
}

int NewCampPartsMapSelect::fn_805201EC(int* point) {
    MapTask* map = lbl_8087F430->GetCurrentMap();
    int type = map->GetMapType();
    int id = map->GetMapId();
    int subId = map->GetMapSubId();
    *point = 205;
    FindRouteIndex_unk(205);
    if (IsCastleCamp_unk(type, id)) {
        if (FindRoute_unk(201) != 0) {
            *point = 201;
        }
    } else if (type == 2 && id == 9) {
        if (FindRoute_unk(203) != 0) {
            *point = 203;
        }
    } else if (type == 2 && id == 25) {
        if (FindRoute_unk(310) != 0) {
            *point = 310;
        }
    } else if (type == 2 && id == 67) {
        if (FindRoute_unk(314) != 0) {
            *point = 314;
        }
    } else if (type == 2 && id == 27) {
        if (FindRoute_unk(403) != 0) {
            *point = 403;
        }
    } else if (type == 2 && id == 3) {
        if (FindRoute_unk(208) != 0) {
            *point = 208;
        }
    } else if (type == 1 && lbl_8087FA20 != 0) {
        atn::Vector3 pos = lbl_8087F8A0->mHead->mPos;
        int x = 0;
        int z = 0;
        lbl_8087FA20->fn_805B8110(&pos, &x, &z);
        MapSelectRoute_unk* found = FindBlockRoute_unk(x, z);
        if (found != 0) {
            *point = found->mId;
        }
    } else {
        for (unsigned int i = 0; i < mRoutes.mSize; i++) {
            if (type == mRoutes.mData[i].mMapType && id == mRoutes.mData[i].mMapId) {
                *point = mRoutes.mData[i].mId;
                FindRoute_unk(*point);
                if (subId == mRoutes.mData[i].mMapSubId) {
                    break;
                }
            }
        }
    }
    return 1;
}

// NONMATCHING: 99% - only the offsets of ";" and "%s" in @stringBase0 (the strings of vf10/vf14, not written
// yet, go before them)
void NewCampPartsMapSelect::fn_80520634() {
    atn::Csv csv;
    MapSelectPoint_unk point;
    csv.SetData((const char*)mParam.fn_80470580(), mParam.fn_8047059C());
    do {
        point.Clear_unk();
        if (strncmp(csv.fn_8005B9CC(), ";", 1) == 0) {
            continue;
        }
        point.m00 = fn_800DC12C(csv.fn_8005B9CC());
        csv.fn_8005B9CC();
        csv.fn_8005B9CC();
        point.m08 = fn_800DC12C(csv.fn_8005B9CC());
        point.m0C = fn_800DC12C(csv.fn_8005B9CC());
        csv.fn_8005B9CC();
        point.m04 = fn_800DC12C(csv.fn_8005B9CC());
        point.m10 = FindArea_unk(csv.fn_8005B9CC());
        point.m14 = fn_800DC12C(csv.fn_8005B9CC());
        sprintf(point.mThumbnail, "%s", csv.fn_8005B9CC());
        std::string text;
        for (int i = 0; i < 5; i++) {
            text.clear();
            text = csv.fn_8005B9CC();
            if (text.size() >= 3) {
                point.m6C[i].mA = fn_800DC12C(text.c_str());
                point.m6C[i].mB = fn_800DC12C(text.c_str() + 2);
            }
        }
        mPoints.push_back(point);
    } while (csv.fn_8005BBF8());
    mParam.fn_80473F88();
}

// NONMATCHING: 97.8% - same code, other registers (the original gives r31-r25 to the variables of the route block and
// r24-r20 to chapter, flag, i, id and this); tried other declaration orders and an inline helper
void NewCampPartsMapSelect::fn_80520B04() {
    mRoutes.clear();
    int chapter = 0;
    int flag = 0;
    if (lbl_8087F430 != 0) {
        chapter = lbl_8087F430->m10D0;
        flag = lbl_8087F430->fn_80370174(0x63);
    }
    for (int i = 0; i < fn_80216988(); i++) {
        int id = fn_80216954(i);
        MapSelectRouteData_unk* data = fn_80216544(id, chapter, flag);
        const char* text = fn_80216700(id, chapter, flag);
        if (id > 0 && data != 0 && text != 0) {
            int state = *text;
            if (state != 0 && id < 500) {
                MapSelectPoint_unk* point = FindPoint_unk(id);
                if (state == 2 && lbl_8087F430 != 0) {
                    if (lbl_8087F430->fn_80375184() != 0 || lbl_8087F430->m54E4 == 3 ||
                        lbl_8087F430->fn_80370174(0x394) != 0 || lbl_8087F430->fn_80370174(0x62) != 0) {
                        state = 1;
                    }
                }
                int open = 1;
                int mapType = data->mMapType;
                int mapId = data->mMapId;
                int mapSubId = data->mMapSubId;
                int entry = data->mEntry;
                if (((NewCampMapCamp_unk*)m48)->m1350 == 0 && lbl_8087F430 != 0 && point != 0 && point->m04 > 0 &&
                    lbl_8087F430->fn_80370174(point->m04) == 0) {
                    open = 0;
                }
                if (point == 0 || point->m18 == 0) {
                    open = 0;
                }
                if (open) {
                    new (&mRoutes.mData[mRoutes.mSize])
                        MapSelectRoute_unk(point, id, state, mapType, mapId, mapSubId, entry);
                    mRoutes.mSize++;
                }
            }
        }
    }
    MapTask* map = lbl_8087F430->GetCurrentMap();
    int type = map->mType;
    int mapId = map->mId;
    int special;
    if (type == 2 && (mapId == 15 || mapId == 55)) {
        special = 1;
    } else {
        special = 0;
    }
    int start = 201;
    fn_805201EC(&start);
    MapSelectPoint_unk* cur = FindPoint_unk(start);
    MapSelectRoute_unk* route = mRoutes.begin();
    for (unsigned int i = 0; i < mRoutes.mSize; i++, route++) {
        int state = route->m08;
        MapSelectPoint_unk* point = route->m00;
        if ((state == 1 || state == 2) && lbl_8087F430 != 0) {
            if (special) {
                route->m08 = 1;
                if (point == 0 || point->m0C == 2) {
                    route->m08 = 0;
                }
            }
            if (cur != 0) {
                switch (cur->m0C) {
                case 0:
                case 3:
                    route->m08 = 1;
                    if (point == 0 || point->m0C == 2) {
                        route->m08 = 0;
                    }
                    break;
                case 1:
                case 4:
                    if (point == 0 || point->m0C == 2) {
                        route->m08 = 0;
                    }
                    break;
                case 2:
                    if (point == 0 || point->m0C != 2) {
                        route->m08 = 0;
                    }
                    break;
                }
            }
        }
    }
    for (int i = 0; i < 5; i++) {
        int id = 500 + i;
        MapSelectPoint_unk* point = FindPoint_unk(id);
        if (sRouteEffects[i].mMin <= chapter && sRouteEffects[i].mMax > chapter) {
            new (&mRoutes.mData[mRoutes.mSize]) MapSelectRoute_unk(point, id, 2, chapter, chapter, chapter, chapter);
            mRoutes.mSize++;
        }
    }
    for (MapSelectRoute_unk* it = mRoutes.begin(); it != mRoutes.end();) {
        if (it->m08 == 0) {
            mRoutes.erase(it);
        } else {
            it++;
        }
    }
}

int NewCampPartsMapSelect::fn_80521010() {
    int ret = 1;
    if (mD8 == 1) {
        fn_80520B04();
        memset(m45D4.m00, 0, sizeof(m45D4.m00));
        m46D4 = 0;
        MapSelectRoute_unk* route = mRoutes.begin();
        for (unsigned int i = 0; i < mRoutes.mSize; i++, route++) {
            m45D4.m00[m46D4] = route->mId;
            m46D4++;
        }
        for (unsigned int i = 0; i < mRoutes.mSize; i++) {
            MapSelectRoute_unk* route = &mRoutes.mData[i];
            MapSelectPoint_unk* point = FindPoint_unk(mRoutes.mData[i].mId);
            if (point != 0) {
                if (route->m08 == 2) {
                    if (point->m08 > 0) {
                        UseMark_unk(mBattles, point->m1C, point);
                    } else if (route->mId < 500) {
                        UseMark_unk(mGoes, point->m20, point);
                    }
                } else {
                    UseMark_unk(mCursors, point->m24, point);
                }
            }
        }
        m84 = m46D4;
        m19A4 = -1;
        mD8 = 2;
        ret = 0;
    } else if (mD8 == 2) {
        ret = 0;
    }
    return ret;
}

extern "C" float fn_800A56A8(void* pad, int port, int axis, int arg); // inferred: stick axis

// inferred: position of the model of a point (relative to m4800)
inline atn::Vector3 GetPointPos_unk(MapSelectPoint_unk* point, const atn::Vector3& origin) {
    return GetModel_unk(point->m18->mModel)->mMtx.GetTrans_unk() - origin;
}

// inferred: moves the cursor of the map with the stick: it snaps to the nearest point of an open route of the
// area within m481C (slower over the point already selected), then it is kept inside the area (the main
// map from chapter 400000 on is larger)
// NONMATCHING: 91.7% - the loop over the points keeps the index (lwzu), scheduling of the position temporaries
void NewCampPartsMapSelect::fn_805212BC() {
    atn::Vector3 move;
    move.x = fn_800A56A8(lbl_8087EF70, 0, 0, 0);
    move.y = fn_800A56A8(lbl_8087EF70, 0, 1, 0);
    move.z = 0.0f;
    if ((float)__fabs(move.x) < 0.3f) {
        move.x = 0.0f;
    }
    if ((float)__fabs(move.y) < 0.3f) {
        move.y = 0.0f;
    }
    MapSelectPoint_unk* cur = FindPoint_unk(m19A4);
    m19A4 = -1;
    float best = 1e16f;
    MapSelectPoint_unk* found = 0;
    for (int i = 0; i < m4C; i++) {
        MapSelectPoint_unk* point = &mPoints.mData[i];
        if (FindRoute_unk(point->m00) != 0 && m19A8 == point->m10) {
            atn::Vector3 pos = GetPointPos_unk(point, m4800);
            atn::Vector3 diff = m480C - pos;
            float dist = diff.x * diff.x + diff.y * diff.y;
            if (!(dist >= m481C * m481C) && dist < best) {
                best = dist;
                m19A4 = point->m00;
                found = point;
            }
        }
    }
    if (found != 0 && found != cur) {
        fn_80117228(3);
    }
    if (found != 0) {
        atn::Vector3 pos = GetPointPos_unk(found, m4800);
        atn::Vector3 dir;
        dir = pos - m480C;
        float len = dir.x * dir.x + dir.y * dir.y;
        if (len > 0.1f) {
            if (len > 3.0f * m4818) {
                len = 3.0f * m4818;
            }
            PSVECNormalize(&dir, &dir);
            dir *= len;
            if (cur == found) {
                dir *= m4820 * m4824;
            } else {
                dir *= m4820;
            }
            move += dir;
        }
    }
    m480C += move * m4818;
    if (m19A8 == 2) {
        m480C.x = MAX(-150.0f, MIN(0.0f, m480C.x));
        m480C.y = MAX(-60.0f, MIN(50.0f, m480C.y));
    } else {
        atn::Vector3 lo(0.0f, 0.0f, 0.0f);
        atn::Vector3 hi(0.0f, 0.0f, 0.0f);
        if (m19A8 == 0) {
            if (lbl_8087F430->m10D0 >= 400000) {
                lo += atn::Vector3(0.0f, 60.0f, 0.0f);
                lo += atn::Vector3(-20.0f, -30.0f, 0.0f);
                hi += atn::Vector3(0.0f, 60.0f, 0.0f);
                hi += atn::Vector3(50.0f, 5.0f, 0.0f);
            }
        } else {
            lo += atn::Vector3(0.0f, -15.0f, 0.0f);
            hi += atn::Vector3(0.0f, 0.0f, 0.0f);
        }
        m480C.x = MAX(m48AC + lo.x, MIN(m48B4 + hi.x, m480C.x));
        m480C.y = MAX(m48B0 + lo.y, MIN(m48B8 + hi.y, m480C.y));
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

// NONMATCHING: 98% - the addresses of rot and of the temporary vector take r29/r30 the other way round
void NewCampPartsMapSelect::fn_80521A00(int dir) {
    fn_80239DAC(lbl_8087F3C0, this, 1, 0);
    int chapter = lbl_8087F430->m10D0;
    for (int i = 0; i < 5; i++) {
        if (sRouteEffects[i].mMin <= chapter && sRouteEffects[i].mMax > chapter && m19A8 == sRouteEffects[i].mArea) {
            MapSelectPoint_unk* point = FindPoint_unk(500 + i);
            if (point != 0 && point->m18 != 0) {
                lbl_8087F3C0->mBC = 1;
                lbl_8087F3C0->mB8 = 6;
                fn_80232B7C(this, 1);
                atn::Vector3 rot = m4828[i];
                atn::Vector3 pos = GetModel_unk(point->m18->mModel)->mMtx.GetTrans_unk();
                if (i == 4) {
                    fn_8023A680(lbl_8087F3C0, &m47F4, -1, 0, 0, &pos, &rot, 0, 0, -1, 1, 2.5f);
                } else {
                    PlayEfp_unk(&m47DC, pos, rot);
                    rot = atn::Vector3(0.0f, 3.1415927f, 0.0f);
                    PlayEfp_unk(&m47E8, pos, rot);
                }
                lbl_8087F3C0->mB8 = 0;
                lbl_8087F3C0->mBC = 0;
            }
        }
    }
}

void NewCampPartsMapSelect::vf44() {
    for (int i = 0; i < m4C; i++) {
        if (GetModelData_unk(m50[i].mModel) != 0) {
            ((NewCampTask*)m48)->fn_80530D88(0, GetModelData_unk(m50[i].mModel));
        }
    }
}

// NONMATCHING: 99.8% - only the offset of its string in @stringBase0 (strings of vf10/vf14 not written yet)
void NewCampPartsMapSelect::vf48() {
    float frame = 0.0f;
    for (int i = 0; i < m4C; i++) {
        if (m50[i].mAnime != 0 && m50[i].mAnime->GetTask() != 0) {
            SetPaneFloat_unk(GetD2A_unk(m50[i].mAnime)->mLayout, "frameOn", frame);
        }
    }
}
