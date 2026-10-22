#include "game/Em302KrakenMp.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "game/CharaManager.h"
#include "game/CharaStatus.h"
#include "game/Em303KrakenFootMp.h"
#include "math/Vec.h"
#include "std/string.h"
#include "std/string_members.h"

extern "C" unsigned long strlen(const char* s);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" int fn_800DC12C(const char* s);   // inferred: string to int
// inferred: std::string concatenation (weak copy 0x8006AFF8)
extern "C" std::string fn_8006AFF8(const std::string& a, const std::string& b);
// inferred: effect/bullet manager (lbl_8087F048, as in chr/ControlUseSkill.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
extern "C" void fn_800EE360(CharaTask* chara); // inferred: BattleCharaTask (enemies only)
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" void fn_800CB5C8(atn::SoundRef* sound, int frames, int arg); // inferred: stops the sound
// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk;
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, Model_unk* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
// inferred: multiplayer session (lbl_8087F610, as in game/Em305VampireMp.cpp)
struct MpSession_unk;
extern "C" MpSession_unk* lbl_8087F610;
extern "C" void fn_804DA48C(MpSession_unk* session, int arg);
// inferred: field object (lbl_8087F430): camera shake at 0x96C (names invented, as in game/Em301LShade.cpp)
struct Field_unk {
    unsigned char m000[0x96C];
    int m96C;
    int m970;
    float m974;
    float m978;
};
extern "C" Field_unk* lbl_8087F430;

// inferred: commands of "act_command" ({name, id}, terminated by a negative id; names invented)
struct KrakenMpCommandName_unk {
    char mName[0x40];
    int mId;
};
static const KrakenMpCommandName_unk sCommands_unk[] = {
    {"swing", 8}, {"pound", 9}, {"wait", 10}, {"", -1},
};

// inferred: id of the command `name`, -1 if unknown
static int fn_802DDD1C(const char* name) {
    for (int i = 0; sCommands_unk[i].mId >= 0; i++) {
        if (strcmp(name, sCommands_unk[i].mName) == 0) {
            return sCommands_unk[i].mId;
        }
    }
    return -1;
}

// inferred: node position (name invented, as in game/Em209BigGolem.cpp)
inline atn::Vector3 GetNodePos_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

// inferred: plays an effect on the model (same as in game/Em305Vampire.cpp; names invented)
inline void PlayEffect_unk(atn::EfpHandle_unk* efp, Model_unk* model) {
    atn::Vector3 pos;
    atn::Vector3 rot;
    Color4f_unk color;
    rot.x = 0.0f;
    rot.y = 0.0f;
    rot.z = 0.0f;
    pos.x = 0.0f;
    pos.y = 0.0f;
    pos.z = 0.0f;
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    color.a = 1.0f;
    fn_8023A8B4(lbl_8087F3C0, efp, model, 0, pos, rot, color, -1, -1, 1, 1.0f);
}

// NONMATCHING: draft - registers of the second std::string constructor (as in game/Em201ZanGurg.cpp)
Em302KrakenMp::Em302KrakenMp(atn::Task* parent, EnemyInfo_unk* info)
    : SpecialEnemyBase(parent, info), m14D4(0), m14DC(2), m14E0(2), m14E4(0), m14E8(0), m14EC(0), m14F0(-70.0f),
      m14F4(0), m14F8(0), m14FC(0), m1500(0x450), m151C(0), m1538(0), m153C(0), m1540(0), m1544(0), m1548(1),
      m154C(0), m1550(0), m1554(0), m1558(0), m155C(0x712), m15A0(0.0f, 0.0f, 0.0f), m15AC(0), m15B0(-1),
      m15B4(1420.0f), m15B8(20.0f), m15BC(-1720.0f), m15D8(0), m15DC(0), m15E0(0), m15E4(0), m1608(0),
      m1648(0), m164C(0), m1650(0), m165C(0) {
    mFlag22 = 1;
    mStatus.mParamB.m9C |= 0x210;
    std::string dir("database/chara/special/");
    std::string name("db_em302_00_00.csv");
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
    m1630.vf0C(path.c_str());
    m1510.fn_8023780C("/effect/ef_bs030c.efp");
    m1520.fn_8023780C("/effect/ef_bu022.efp");
    m152C.fn_8023780C("/effect/ef_bu023.efp");
    m1564.fn_8023780C("/effect/ef_bs028.efp");
    m1570.fn_8023780C("/effect/ef_bs029a.efp");
    m157C.fn_8023780C("/effect/ef_bs029c.efp");
    m15CC.fn_8023780C("/effect/ef_bd005.efp");
    m15E8.clear();
    m15F4.clear();
    m1624.clear();
    m1618.clear();
    m160C.clear();
    m1638.clear();
    m1644 = 0;
}

Em302KrakenMp::~Em302KrakenMp() {}

void Em302KrakenMp::vf14() {
    if (m151C != 0) {
        return;
    }
    fn_80149A30();
}

// inferred: back to state 0 (names invented)
inline void Em302KrakenMp::Reset_unk() {
    m14EC = 0;
    m14E8 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 0;
    fn_8016E970(3);
}

void Em302KrakenMp::vfFC() {
    Reset_unk();
    fn_800CB5C8(&m1560, 0, 0);
    fn_80239DAC(lbl_8087F3C0, this, 100, 1);
    fn_80239DAC(lbl_8087F3C0, this, 101, 1);
}

void Em302KrakenMp::vf100(int state) {
    if (mC58.mC4 == 0) {
        return;
    }
    switch (state) {
    case 7:
        fn_802E1290();
        break;
    case 8:
        m14EC = 0;
        m14E8 = 0;
        m590 = fn_800F8548(lbl_8087F048);
        m58C = 8;
        fn_8016E970(3);
        fn_800C344C("SE_EM302_013", &m614, 0, -1, 1.0f);
        break;
    case 9:
        fn_802E1414();
        break;
    case 10:
        fn_802E1624();
        break;
    case 2:
        fn_802E177C();
        break;
    default:
        Reset_unk();
        fn_800CB5C8(&m1560, 0, 0);
        fn_80239DAC(lbl_8087F3C0, this, 100, 1);
        fn_80239DAC(lbl_8087F3C0, this, 101, 1);
        break;
    }
}

void Em302KrakenMp::vf108() {}

void Em302KrakenMp::vf40(HitInfo_unk* hit) {
    if ((mStatus.m0C & 0x20) == 0x20 && m1650 == 0) {
        fn_802E177C();
    }
}

void Em302KrakenMp::vf54() {
    if (m624.mSize != 0) {
        MtxPtr head = mMotion.GetNodeMtx_unk("head");
        m624.mData[0].m04.mCenter = GetNodePos_unk(head) + m5A4;
    }
}

void Em302KrakenMp::vf20() {
    m620 = m5B0 - 3.0f;
    MtxPtr head = mMotion.GetNodeMtx_unk("head");
    m614 = GetNodePos_unk(head) + m5A4;
    atn::Vector3 a = m614;
    atn::Vector3 b = a;
    b.y += m5B4;
    float r = m620;
    mColli.m3C = a;
    mColli.m48 = b;
    mColli.m54 = r;
}

void Em302KrakenMp::vfEC() {
    m1654--;
    if (mMotion.mTracks[0].m08 <= 0.0f || m1654 < 0) {
        mFlag25 = 1;
        mColli.mFlags &= ~1;
        fn_800EE360(this);
        mFlag15 = 1;
        fn_801765D8();
    }
}

void Em302KrakenMp::fn_802E1A0C() {
    fn_80087994(m1658.Load("Em302KrakenMp"), "DebugDisp", &m165C, 0, 0);
}

void Em302KrakenMp::vf110(SpecialState_unk* state) {
    state->m00 = m58C;
    state->m04 = m14BC;
    state->m08 = m151C;
    *((signed char*)state + 0xC) = m14DC;
    *((signed char*)state + 0xD) = m1540;
    *((signed char*)state + 0xE) = m154C;
    *((signed char*)state + 0xF) = m1554;
    *(short*)&state->m0A = m15E4;
}

void Em302KrakenMp::vf114(const SpecialState_unk* state) {
    if (state->m00 == 10 && m58C != 10) {
        fn_80232B7C(0, 0);
        PlayEffect_unk(&m1570, (Model_unk*)&mMotion);
        Field_unk* field = lbl_8087F430;
        field->m96C = field->m96C % 2;
        field->m970 = 140;
        field->m974 = 1.0f;
        field->m978 = 1.0f;
    }
    if (state->m00 == 2 && m1650 == 0) {
        vf44();
        fn_802E177C();
        return;
    }
    if (state->m00 == 6 && m58C != 6 && lbl_8087F610 != 0) {
        fn_804DA48C(lbl_8087F610, 0);
    }
    if (state->m00 == 7 && m58C != 7 && m14FC >= 150 && lbl_8087F610 != 0) {
        fn_804DA48C(lbl_8087F610, 1);
    }
    m58C = state->m00;
    m14BC = state->m04;
    m151C = state->m08;
    m14DC = *((const signed char*)state + 0xC);
    m1540 = *((const signed char*)state + 0xD);
    m154C = *((const signed char*)state + 0xE);
    m1554 = *((const signed char*)state + 0xF);
    m15E4 = *(const short*)&state->m0A;
}

// NONMATCHING: ~99.5% - stack slots of the two temporaries are swapped (7 variants: early return,
// if/else, named offsets, result local, helper inline)
atn::Vector3 Em302KrakenMp::vfAC() {
    if (m624.mSize == 0) {
        return mPos + atn::Vector3(0.0f, 15.0f, 0.0f);
    }
    return m624.mData[0].m04.mCenter + atn::Vector3(0.0f, 15.0f, 0.0f);
}
