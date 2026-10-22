#include "game/Em308ParmCrab.h"
#include "game/HitInfo_unk.h"
#include "atn/Csv.h"
#include "std/string.h"
#include "std/string_members.h"

extern "C" unsigned long strlen(const char* s);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
// inferred: std::string concatenation (weak copy 0x8006AFF8)
extern "C" std::string fn_8006AFF8(const std::string& a, const std::string& b);

// inferred: effect/bullet manager (lbl_8087F048); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk;
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_800CB5C8(atn::SoundRef* sound, int frames, int arg); // inferred: stops the sound
extern "C" void fn_80097CCC(Model_unk* model, int track, float value); // inferred: stops a track
extern "C" void fn_800EB7A0(CharaTask* chara); // inferred: BattleCharaTask (death effect?)

// NONMATCHING: 99.83% - r26/r27 of the second std::string constructor (known blocker, as in Em308InvertCrab)
// inferred: same pattern as Em308InvertCrab's constructor (data file "database/chara/special/" + db_name)
Em308ParmCrab::Em308ParmCrab(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14B0(0), m14B4(0), m14E8(60), m14EC(15), m14F0(70.0f), m14F8(180.0f),
      m152C(30.0f), m1534(179.0f), m1538(0), m1540(0), m1544(0.0f), m1548(0.0f), m1574(300), m157C(45),
      m1580(45), m1584(25.0f), m1588(50.0f), m158C(90.0f), m1590(0), m1594(0.5f), m15B8(0), m15BC(0),
      m15C8(0), m15CC(0), m15D0(90), m15D4(90), m15DC(0), m15E0(1.0f), m15E4(1.0f), m15E8(1.0f),
      m15EC(1.0f), m15F0(1.0f), m1618(0) {
    std::string dir("database/chara/special/");
    std::string name("db_em308_00_00.csv");
    atn::Csv csv(info->m2C, strlen(info->m2C));
    do {
        const char* s = csv.fn_8005B9CC();
        if (s == 0 || strcmp(s, "") == 0) {
            break;
        }
        if (strncmp(s, "db_name=", 8) == 0) {
            name = s + 8;
        }
    } while (csv.fn_8005BBF8());
    std::string path = fn_8006AFF8(dir, name);
    m160C.vf0C(path.c_str());
    m568 = 0.5f;
    m1530 = fn_80219E6C(0x3F8);
    m14F4 = fn_80219E6C(0x3F7);
    m1578 = fn_80219E6C(0x400);
    m15F4.fn_8023780C("/effect/ef_uc001.efp");
    m1520.fn_8023780C("/effect/ef_sk002d.efp");
    m14DC.fn_8023780C("/effect/ef_uc007.efp");
    m1600.fn_8023780C("/effect/ef_sc003_01b.efp");
}

Em308ParmCrab::~Em308ParmCrab() {}

float Em308ParmCrab::vf90() {
    return 3.5f;
}

void Em308ParmCrab::fn_802FC188() {
    fn_80239DAC(lbl_8087F3C0, this, 0, 0);
    fn_800CB5C8(&m151C, 0, 0);
    fn_80239DAC(lbl_8087F3C0, this, 200, 0);
    vf48();
    fn_80178A6C();
    fn_8015495C(0, 1, 1, 1);
    fn_8016DA4C();
    m14B0 = 0;
    m15CC = 0;
    m15D4 = m14F4 != 0 ? m14F4->m68 : 90;
    m590 = fn_800F8548(lbl_8087F048);
    fn_800CB5C8(&m151C, 15, 0);
    fn_80239DAC(lbl_8087F3C0, this, 200, 0);
    m58C = 2;
    fn_8016E970(9);
    fn_80097CCC(&mMotion, 0, 0.0f);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x2E, 0, 0, 1, 0.0f, 0.1f);
    fn_800EB7A0(this);
}
