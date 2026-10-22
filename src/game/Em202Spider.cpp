#include "game/Em202Spider.h"
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

// NONMATCHING: 96.39% - written for the order of the strings; registers r28-r31 of the std::string
// constructors swapped (as in Em101LittleZesya)
Em202Spider::Em202Spider(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14D8(0), m14DC(55.0f), m14E4(0), m14E8(0), m14EC(0), m14F0(0), m14FC(0),
      m1500(0), m1504(0), m1508(0), m150C(0), m1510(0), m1514(0), m1518(0), m151C(0), m1520(0), m1524(0),
      m1528(0), m152C(0), m1534(60), m1538(0), m153C(0.0f), m1544(0), m1548(0), m154C(1) {
    mFlag22 = 1;
    mStatus.mParamB.m9C |= 0x10;
    m55C = 3;
    m14C0.clear();
    m14CC.clear();
    m14F4 = 0;
    m14F8 = 0;
    m151C = 0;
    m14B8 = 0;
    m14BC = 0;
    std::string dir("database/chara/special/");
    std::string name("db_em202_00_00.csv");
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
    m14B0.vf0C(path.c_str());
    m1550.fn_8023780C("/effect/ef_uc019.efp");
    m155C.fn_80237654("/effect/ef_sk002b.eff");
    m1568.fn_8023780C("/effect/ef_uc001.efp");
    m1574.fn_8023780C("/effect/ef_sk002d.efp");
    m1580.fn_8023780C("/effect/ef_uc007.efp");
    m158C.fn_8023780C("/effect/ef_sc003_01b.efp");
}

Em202Spider::~Em202Spider() {}

void Em202Spider::vf7C(int arg) {
    m14D8 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 11;
    fn_8016E970(6);
    m560 = 0x16;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x1DD, 0, 0, 1, 0.0f, 0.05f);
}

void Em202Spider::fn_80281ED8() {
    if (m14B8 > 0) {
        m14D8 = 0;
        m590 = fn_800F8548(lbl_8087F048);
        m58C = 7;
        m1504 = m14B8;
        fn_8017039C(m14B8, 2);
        m1520 = 0;
        m151C = 1;
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 2.0f;
        mMotion.fn_80097C08(0, 0x14, 0, 0, 1, 0.0f, 0.1f);
    }
}

void Em202Spider::fn_802829DC() {
    if (m58C == 5 && m1530 == 1) {
        fn_80239DAC(lbl_8087F3C0, this, 0, 0);
    }
    fn_800CB5C8(&m15A8, 15, 0);
    fn_80239DAC(lbl_8087F3C0, this, 200, 0);
    vf48();
    fn_80178A6C();
    fn_8015495C(0, 1, 1, 1);
    fn_8016DA4C();
    m58C = 10;
    fn_80097CCC(&mMotion, 0, 0.0f);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x2E, 0, 0, 1, 0.0f, 0.1f);
    fn_800EB7A0(this);
}
