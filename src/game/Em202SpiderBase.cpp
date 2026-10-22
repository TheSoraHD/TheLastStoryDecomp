#include "game/Em202SpiderBase.h"
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
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector

// NONMATCHING: 99.81% - r26/r27 of the second std::string constructor (as in Em201ZanGurg)
Em202SpiderBase::Em202SpiderBase(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14B0(0), m14B4(0), m14E8(60), m14EC(15), m14F0(70.0f), m14F8(180.0f),
      m152C(30.0f), m1534(3.1415927f), m153C(0), m1540(0.0f), m1544(0.0f), m1554(0), m157C(0), m1580(0),
      m1584(0), m158C(0), m1590(90), m1594(90), m159C(0), m15A0(1.0f), m15D8(0) {
    std::string dir("database/chara/special/");
    std::string name("db_em202_base_00.csv");
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
    m15BC.vf0C(path.c_str());
    m568 = 0.5f;
    m1530 = fn_80219E6C(0x3F8);
    m14F4 = fn_80219E6C(0x3F7);
    m15A4.fn_8023780C("/effect/ef_uc001.efp");
    m1520.fn_8023780C("/effect/ef_sk002d.efp");
    m14DC.fn_8023780C("/effect/ef_uc007.efp");
    m15B0.fn_8023780C("/effect/ef_sc003_01b.efp");
}

Em202SpiderBase::~Em202SpiderBase() {}

void Em202SpiderBase::fn_80287788() {
    fn_80239DAC(lbl_8087F3C0, this, 0, 0);
    fn_800CB5C8(&m151C, 0, 0);
    fn_80239DAC(lbl_8087F3C0, this, 200, 0);
    m14B0 = 0;
    m158C = 0;
    m1594 = m14F4 != 0 ? m14F4->m68 : 90;
    m590 = fn_800F8548(lbl_8087F048);
    fn_800CB5C8(&m151C, 15, 0);
    fn_80239DAC(lbl_8087F3C0, this, 200, 0);
    vf48();
    fn_80178A6C();
    fn_8015495C(0, 1, 1, 1);
    fn_8016DA4C();
    m58C = 2;
    fn_80097CCC(&mMotion, 0, 0.0f);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x2E, 0, 0, 1, 0.0f, 0.1f);
    m6B8 = lbl_807C7030;
    fn_800EB7A0(this);
}

void Em202SpiderBase::vf7C(int arg) {
    m14B0 = 0;
    m158C = 0;
    m1594 = m14F4 != 0 ? m14F4->m68 : 90;
    m590 = fn_800F8548(lbl_8087F048);
    fn_800CB5C8(&m151C, 15, 0);
    fn_80239DAC(lbl_8087F3C0, this, 200, 0);
    m58C = 14;
    fn_8016E970(6);
    m560 = 0x16;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x1DD, 0, 0, 1, 0.0f, 0.05f);
}
