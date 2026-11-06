#include "game/NewCampPartsMapSelect.h"
#include "atn/Memory.h"
#include "atn/D3Anime.h"
#include "atn/D3AnimeInstance.h"
#include "std/string_members.h"

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
extern "C" atn::Task* fn_801F3FF8(atn::Task* parent, const char* path, int loop); // inferred: d2anime task

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

NewCampPartsMapSelect* NewCampPartsMapSelect::fn_8051B920(atn::Task* parent) {
    if (parent != 0) {
        return new ((MemoryTag)1, "", "", 0) NewCampPartsMapSelect(parent);
    }
    return 0;
}

NewCampPartsMapSelect::NewCampPartsMapSelect(atn::Task* parent)
    : NewCampPartsBase(parent), mD4(0), mD5(0), mD6(0), mD7(0), mD8(0), mDC(1), m198C(0), m1990(0),
      m1998(4), m199C(4), m19A0(-1), m19A4(-1), m19A8(0), m19B4(0), m19B8(0.0f), m19BC(6.0f), m19C0(0.0f),
      m19CC(0), m3ED0(0), m3ED4(0), m46D4(0), m46D8(0), m4800(sOffset), m4818(4.0f), m481C(20.0f),
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
    m46D8 = 0;
    m1908 = 0;
}

NewCampPartsMapSelect::~NewCampPartsMapSelect() {
    if (m198C != 0) {
        delete[] m198C;
        delete[] m1990;
        m198C = 0;
        m1990 = 0;
    }
    if (m1908 != 0) {
        fn_800D2338(m1908);
        m1908 = 0;
    }
    m3ED0 = 0;
    m46D8 = 0;
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
