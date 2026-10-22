#include "game/Em005DoppelGanger.h"
#include "atn/Csv.h"
#include "std/string.h"
#include "std/string_members.h"

extern "C" unsigned long strlen(const char* s);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
// inferred: std::string concatenation (weak copy 0x8006AFF8)
extern "C" std::string fn_8006AFF8(const std::string& a, const std::string& b);
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
struct Field_unk;
extern "C" Field_unk* lbl_8087F430;
extern "C" int fn_80370A78(Field_unk* field, int index);
extern "C" void fn_80370AE4(Field_unk* field, int index, int value);
// inferred: effect/bullet manager (lbl_8087F048, as in chr/ControlUseSkill.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
extern "C" void fn_800EB7A0(CharaTask* chara); // inferred: BattleCharaTask (death effect?)
// inferred: word at 0x1C of the running control (class unknown)
struct Control1C_unk {
    unsigned char m00[0x1C];
    int m1C;
};
// inferred: 0x1C-byte record at 0x1470 of the battle characters (as PlayerState_unk in
// game/CharaTask.cpp; names invented)
struct DoppelState_unk {
    DoppelState_unk(short id) : m00(id), m02(0), m04(0), m08(150), m0C(0.0f, 0.0f, 0.0f), m18(0) {}
    short m00;
    short m02;
    int m04;
    int m08;
    atn::Vector3 m0C;
    int m18;
};
// inferred: the doppelganger as a battle character with that record (names invented)
struct DoppelChara_unk : public CharaTask {
    void SetState_unk(const DoppelState_unk& state) { m1470 = state; }

    unsigned char m1424[0x1470 - 0x1424];
    DoppelState_unk m1470;
};

// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk {
    unsigned char m00[0xEC];
    unsigned int mEC; // inferred: flags (bit 3 set while a doppelganger exists)
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" int lbl_8087F3D8; // inferred: cleared by vf14 (file static)

// inferred: offsets of the hit sphere (static constructors at 0x80254530)
static atn::Vector3 sSphereOffset_unk(0.0f, 0.0f, 1.0f);
static atn::Vector3 sOffsetB_unk(0.0f, 0.0f, 26.81f);
static atn::Vector3 sOffsetC_unk(-130.0f, 0.0f, 26.81f);

// inferred: through an inline taking `this`; with the test written in the destructor body the vtable
// store of the original is dropped (name invented)
inline void ClearDoppel_unk(Em005DoppelGanger* self) {
    if (lbl_8087F3C0 != 0) {
        lbl_8087F3C0->mEC &= ~8;
    }
}

// NONMATCHING: 99.6% - r26/r27 of the second std::string and r25/r26 of the strlen of the parameters (same
// as Em101ElderZesya / Em201ZanGurg)
Em005DoppelGanger::Em005DoppelGanger(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14B0(0), m14B4(0), m14B8(0), m14BC(0), m14C0(240), m14C4(0), m14F8(0),
      m14FC(0), m1500(0), m1504(0), m1510(0), m1514(0), m1518(0), m151C(1), m1520(0), m1524(4), m1528(0),
      m152C(0), m1540(0), m1544(0), m1548(0), m154C(0), m1554(0) {
    mFlag22 = 1;
    mFlagB15 = 1;
    std::string dir("database/chara/special/");
    std::string name("db_em005_00_00.csv");
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
    m1508.vf0C(path.c_str());
    m14E0.fn_8023780C("/effect/ef_bs024a.efp");
    m14C8.fn_8023780C("/effect/ef_cm007.efp");
    m14EC.fn_8023780C("/effect/ef_bs038a.efp");
    m14D4.fn_8023780C("/effect/ef_cm007.efp");
}

Em005DoppelGanger::~Em005DoppelGanger() {
    ClearDoppel_unk(this);
}

void Em005DoppelGanger::vf54() {
    CharaTask::vf54();
    if (m624.mSize != 0) {
        if (m14BC == 0) {
            m624.mData[0].m04.mRadius = 5.0f;
            m624.mData[0].m04.mCenter += sSphereOffset_unk * -3.0f;
        } else {
            m624.mData[0].m04.mRadius = 5.0f;
        }
    }
}

// NONMATCHING: 98.98% - the original keeps an unreachable `b` after the `return` of the id test (5
// variants: switch, else if, goto, empty branch, double return)
void Em005DoppelGanger::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (m14BC == 0) {
        if (msg->mAttacker != m1504) {
            return;
        }
        if (m58C == 11 || m58C == 10 || m58C == 6) {
            if (fn_805F9920(&msg->m10) > 0.0001f) {
                PSVECNormalize(&msg->m10, &msg->m10);
            }
            msg->m10 *= 8.0f;
            if (fn_80370A78(lbl_8087F430, 0xE9) == 0) {
                fn_80370AE4(lbl_8087F430, 0xE9, 1);
            }
        } else {
            HitInfo_unk* data = msg->mData;
            if (data != 0) {
                int id = data->m04;
                if (id == 200) {
                    m14B4 = 0;
                    m14B8 = 0;
                    m590 = fn_800F8548(lbl_8087F048);
                    m58C = 1;
                    fn_8015DEC0((int)&msg->m10);
                    ((Control1C_unk*)mControl.get())->m1C = 1;
                    msg->mAttacker->vf7C(1);
                    msg->m90 = 0;
                    msg->m84 = 2;
                    return;
                } else if (id != 208 && id != 10000) {
                    return;
                }
            }
        }
    } else if (m14BC == 2 && m58C == 8) {
        return;
    }
    fn_80151448(msg);
    vf40(hit);
}

// NONMATCHING: 99.58% - r5/r6 of the copy of the record at 0x1470 (Vector3 copy from a temporary of an
// inline: __VecCopy family, documented)
void Em005DoppelGanger::vf40(HitInfo_unk* hit) {
    if (m14BC == 0 && m55C == 6 && m560 - 20U <= 3) {
        m58C = 10;
        m14B4 = 0;
        m14B8 = 0;
        m590 = fn_800F8548(lbl_8087F048);
    }
    if (mStatus.IsState_unk(0x20)) {
        if (m14BC == 0) {
            mStatus.m04 = 1.0f;
            mStatus.fn_8013322C(0x20);
        } else {
            vf44();
            m58C = 2;
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x14A, 0, 0, 1, 0.0f, 0.1f);
            fn_800EB7A0(this);
        }
    } else if (m14BC == 2) {
        int kind = m560;
        if (kind - 20U <= 3 && ++m1520 >= m1524 && m1470 != 14) {
            ((DoppelChara_unk*)this)->SetState_unk(DoppelState_unk(14));
            if (kind != 23 && PSVECMag(&m6B8) > 0.01f) {
                PSVECNormalize(&m6B8, &m6B8);
                m6B8 *= 5.0f;
                mFlag5 = 0;
                fn_8015E4B0(m6B8, -1);
            }
        }
    }
}

void Em005DoppelGanger::vf14() {
    lbl_8087F3D8 = 0;
    fn_80149A30();
}

void Em005DoppelGanger::vf44() {
    if (m14BC == 0) {
        mStatus.m04 = 1.0f;
        mStatus.fn_8013322C(0x20);
    } else {
        BattleCharaTask::vf44();
    }
}
