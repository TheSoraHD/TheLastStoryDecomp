#include "game/Em316LaserSphere.h"
#include "atn/Csv.h"
#include "game/CharaManager.h"
#include "game/HitInfo_unk.h"
#include "game/PlayerTask.h"
#include "math/Vec.h"
#include "chr/ControlBase.h"
#include "std/string.h"
#include "std/string_members.h"
#include <boost/tokenizer.hpp>

extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
// inferred: std::string concatenation (weak copy 0x8006AFF8)
extern "C" std::string fn_8006AFF8(const std::string& a, const std::string& b);
extern "C" int atoi(const char* s);
extern "C" int rand();
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk;
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
// inferred: effect/bullet manager (lbl_8087F048, as in chr/ControlUseSkill.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
// inferred: debug menu entries (as in game/CharaTask.cpp)
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);
extern "C" void fn_80087E9C(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void fn_8008AD4C(Model_unk* model, const char* path, const char* node); // inferred: loads a model
extern "C" void fn_80094D88(Model_unk* model, int blend, int a, int b, int c, int d);
extern "C" int fn_800DC12C(const char* s); // inferred: string to int
// inferred: control of a character caught by the tackle (only m4C, set to end it); name invented
struct LaserCatch_unk {
    unsigned char m00[0x4C];
    int m4C;
};
// inferred: accessors of the caught character and its control (weak copies 0x80318F90, 0x80319998)
extern "C" inline chr::ControlBase* fn_80318F90(CharaTask* chara) {
    return chara->mControl.get();
}
extern "C" inline void fn_80319998(LaserCatch_unk* control) {
    control->m4C = 1;
}
// inferred: game state (lbl_8087EFA8, see game/CharaTask.cpp): time of a frame at 0x3A4
struct GameState_unk {
    unsigned char m000[0x3A4];
    float m3A4;
};
extern "C" GameState_unk* lbl_8087EFA8;
extern "C" void fn_800EB7A0(CharaTask* chara); // inferred: BattleCharaTask (death effect?)
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);

// inferred: sound effects of the sphere (laser, laser end, ?, death); name invented
static const char* const sSounds_unk[] = {"SE_EM601_006", "SE_EM601_002", "SE_EM601_006", "SE_EM601_004"};

// inferred: alpha range of the trail models (.sdata 0x8087DC48/0x8087DC4C); names invented
static float sOne_unk = 1.0f;
static float sZero_unk = 0.0f;

// inferred: shared by all the spheres: set once the targets have been shared this frame (lbl_8087F3F4)
// and the last angle (lbl_8087F3F0); names invented
static float sAngle_unk;
static int sShared_unk;

// inferred: translation of a matrix (name invented)
inline atn::Vector3 GetTrans_unk(const Mtx m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

// inferred: linear interpolation with the offset added last (atn::Lerp_unk adds `a` first); name invented
inline atn::Vector3 LaserLerp_unk(const atn::Vector3& a, const atn::Vector3& b, float t) {
    return (b - a) * t + a;
}


// inferred: the three components are zero (written with branches, not as a bool inline); name invented
inline int IsZero3_unk(const atn::Vector3& v) {
    int zero = 0;
    if (v.x == 0.0f && v.y == 0.0f && v.z == 0.0f) {
        zero = 1;
    }
    return zero;
}

// inferred: color as four floats (as in game/Em315Coffin.cpp)
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, Model_unk* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume);

// inferred: plays `efp` on `model` with default placement (as CoffinEffect_unk of game/Em315Coffin.cpp)
inline void LaserEffect_unk(atn::EfpHandle_unk* efp, Model_unk* model) {
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

inline void Em316LaserSphere::SetState_unk(int state) {
    m14BC = 0;
    m14C0 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = state;
    fn_8016E970(3);
}

// inferred: the tests of fn_80316F74 (bits 3 and 9 of the status flags); name invented
extern "C" inline int fn_80316F74(CharaStatus* status) {
    return (status->m0C & 8) == 8 || (status->m0C & 0x200) == 0x200;
}

// NONMATCHING: the tokenizer loop (same open case as LastCocoonChrysalis, docs/inlining.md)
Em316LaserSphere::Em316LaserSphere(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14B8(0), m14BC(0), m14C0(0), m14C4(0), m14C8(0), m14CC(1), m14D0(-1),
      m14D4(0), m14D8(0.0f, 0.0f, 0.0f), m14E4(0.0f, 0.0f, 0.0f), m14F0(0.25f), m14F4(0.0f, 0.0f, 0.0f),
      m1500(25.0f), m1520(0), m1524(0), m153C(0), m1540(0.0f, 0.0f, 0.0f), m155C(0.0f, 0.0f, 0.0f),
      m1568(0.0f, 0.0f, 0.0f), m1574(60), m1578(0), m15B4(0), m1C84(60), m1C88(15), m1C90(0), m1C94(0),
      m1C98(4), m1C9C(8) {
    std::string dir("database/chara/special/");
    std::string name("db_em601_00_00.csv");
    std::string params(info->m2C);
    typedef boost::tokenizer<boost::char_separator<char> > Tokenizer;
    Tokenizer tok(params, boost::char_separator<char>(";"));
    for (Tokenizer::iterator it = tok.begin(); it != tok.end(); ++it) {
        if (strncmp(it->c_str(), "db_name=", 8) == 0) {
            name = it->c_str() + 8;
        } else if (strncmp(it->c_str(), "target=", 7) == 0) {
            m14D0 = fn_800DC12C(it->c_str() + 7);
        }
    }
    std::string path = fn_8006AFF8(dir, name);
    m14B0.vf0C(path.c_str());
    m1550.fn_8023780C("ef_uc066.efp");
    m1584.fn_80237654("ef_sk043c_01.eff");
    m1590.fn_8023780C("ef_sk043a.efp");
    m159C.fn_80237654("ef_sk043g_01.eff");
    m15A8.fn_80237654("ef_sk043g_02.eff");
    for (int i = 0; i < 3; i++) {
        fn_8008AD4C((Model_unk*)&m1648[i], "em316_00_bs00.model", 0);
    }
    mFlag21 = 1;
}

// NONMATCHING: 99.49% - in the last loop the original steps `this` itself (0x164C(r30)) for the model
// flags and keeps the counter in r27; ~12 variants (pointer, index, increments, locals)
int Em316LaserSphere::vf0C() {
    EffectModel_unk* models = m1648;
    for (int i = 0; i < 3; i++) {
        if (models[i].GetModel_unk()->fn_8008B140() != 0) {
            return 0;
        }
    }
    if (fn_8013655C() == 0 && m14B0.fn_80473F50() == 0 && m1550.fn_80237874() == 0 &&
        m1584.fn_802376D0() == 0 && m1590.fn_80237874() == 0 && m159C.fn_802376D0() == 0 &&
        m15A8.fn_802376D0() == 0) {
        void* group = m1C8C.Load("Bit");
        fn_80087994(group, "DebugDisp", &m1C90, 0, 0);
        fn_800874C8(group, "LaserWait", &m1C84, 1, 10000, 1, 0, 0);
        fn_800874C8(group, "MoveWait", &m1C88, 1, 10000, 1, 0, 0);
        fn_80087994(group, "DebugLog", &m1C94, 0, 0);
        fn_8008771C(group, "AppearMoveRate", &m14F0, 0, 0, 0.01f, 0.99f, 0.1f);
        fn_800874C8(group, "ActionMode", &m58C, 1, 100, 1, 0, 0);
        fn_80087E9C(group, "Pos", &mPos, 0, 0, 1.0f, 100.0f, 1.0f);
        if (m14B0.fn_80470580() != 0) {
            atn::Csv csv((const char*)m14B0.fn_80470580(), m14B0.fn_8047059C());
            do {
                const char* key = csv.fn_8005B9CC();
                if (strcmp(key, "LaserID") == 0) {
                    m157C = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "TackleID") == 0) {
                    m1580 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "MoveWait") == 0) {
                    m1C88 = atoi(csv.fn_8005B9CC());
                } else if (strcmp(key, "LaserWait") == 0) {
                    m1C84 = atoi(csv.fn_8005B9CC());
                }
            } while (csv.fn_8005BBF8());
        }
        mStatus.m18 |= 0x1C0;
        mStatus.m18 |= 0x10000;
        mStatus.m18 |= 0xC21D;
        mStatus.m18 |= 0x2880000;
        if (m14D0 <= 0) {
            m14D4 = lbl_8087F8A0->mHead;
        } else {
            m14D4 = lbl_8087F8A0->Find_unk(m14D0);
        }
        m54C |= 0x200;
        for (int i = 0; i < 3; i++) {
            Model_unk* model = models[i].GetModel_unk();
            fn_80094D88(model, 3, 0, 0, 0, 0);
            float t = (float)(i + 1) / 4.0f;
            float a = sOne_unk + t * (sZero_unk - sOne_unk);
            Model_unk::Float4_unk color;
            color.m00 = 1.0f;
            color.m04 = 1.0f;
            color.m08 = 1.0f;
            color.m0C = a;
            model->m40 = color;
            models[i].GetModel_unk()->mFlags |= 0x10;
        }
        return 1;
    }
    return 0;
}

void Em316LaserSphere::vf14() {
    sShared_unk = 0;
    fn_80149A30();
    for (int i = 0; i < 3; i++) {
        atn::Vector3 d = GetTrans_unk(mMotion.mMtx) - GetTrans_unk(m1648[i].GetModel_unk()->mMtx);
        if (fn_805F9920(&d) > 25.0f) {
            m1648[i].GetModel_unk()->fn_8008CD60();
        }
    }
}

void Em316LaserSphere::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    msg->m10 *= 0.0f;
    fn_80151448(msg);
    vf40(hit);
}

// NONMATCHING: 99.33% - stack slots of the inlined fn_80318648 (pos/translation swapped) and the offset of
// "spine1" in the pool; ~12 variants (locals, chained copies, accessors)
void Em316LaserSphere::vf40(HitInfo_unk* hit) {
    if ((mStatus.m0C & 0x20) == 0x20) {
        fn_8031A484();
        return;
    }
    HitInfo_unk* data = ((HitMsg_unk*)hit)->mData;
    if (data != 0 && (data->m90 & 0x800000) == 0x800000) {
        SetState_unk(12);
        atn::Vector3 pos = fn_80318648(m14B8);
        m1510 = m1504 = mPos;
        if (m1510.y > pos.y) {
            m1510.y = pos.y;
        }
        m151C = 0.0f;
        return;
    }
    fn_80319C10();
}

// NONMATCHING: 99.57% - only the offset of "spine1" in @stringBase0 (strings of vf10 not written yet)
void Em316LaserSphere::vf80(const atn::Vector3& pos, const atn::Vector3& offset) {
    SetState_unk(6);
    m14D8 = pos + offset;
    m14E4 = pos + offset;
    mPos = m14E4;
    if (!IsZero3_unk(offset)) {
        atn::Vector3 rot(offset);
        rot.fn_80139F60();
        mRot = rot;
    }
    LaserEffect_unk(&m1550, &mMotion);
    fn_800C31F4(sSounds_unk[0], 0, -1, 1.0f);
}

atn::Vector3 Em316LaserSphere::fn_80318648(CharaTask* chara) {
    Model_unk* model = &chara->mMotion;
    MtxPtr m = model->GetNodeMtx_unk("spine1");
    atn::Vector3 pos(0.0f, 0.0f, 0.0f);
    if (m != 0) {
        atn::Vector3 t(m[0][3], m[1][3], m[2][3]);
        pos = t;
    } else {
        pos = chara->mPos;
        pos.y += 10.0f;
    }
    return pos;
}

void Em316LaserSphere::fn_80317EA0() {
    CharaTask* leader = lbl_8087F8A0->mHead;
    if (leader->mFlag28) {
        mC58.mC8 = (int)mC58.mC4;
        m1454 = 0;
        m14B8 = leader;
        mC58.mC4 = leader;
    } else {
        m1454 = 1;
        m14B8 = mC58.mC4;
    }
    if (m14B8 == 0) {
        m14B8 = m14D4;
    }
}


CharaTask* Em316LaserSphere::fn_80318A30() {
    for (PlayerTask* p = lbl_8087F8A0->mHead; p != 0; p = p->mNext) {
        if (fn_80318AA4(p) != 0) {
            fn_8031A670(p);
            return p;
        }
    }
    return 0;
}

void Em316LaserSphere::fn_80318F9C() {
    if (m14C0 > 90) {
        fn_80319C10();
        int wait = m1C84;
        m1574 = wait + rand() % wait;
        if (fn_80316F74(&mStatus) || mStatus.IsState_unk(0x800000)) {
            m1574 *= 2;
        }
    }
}

extern "C" void* lbl_8087EE98; // inferred: collision world
// inferred: collision result (as in game/Em003LesserDaemon.cpp)
struct CollResult_unk {
    CollResult_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    unsigned char m00[0x04];
    atn::Vector3 m04; // inferred: hit position
    atn::Vector3 m10; // inferred: position after the sweep
    unsigned char m1C[0x34 - 0x1C];
    int m34;
    int m38;
    int m3C;
    int m40;
    unsigned char m44[0x50 - 0x44];
};
extern "C" int fn_8004D388(void* world, CollResult_unk* result, CharaTask::Sphere_unk* sphere, atn::Vector3* move,
                           unsigned int mask, void* ignore, int arg7, float radius);

void Em316LaserSphere::vfEC() {
    if (m14C0 >= 30) {
        fn_80239DAC(lbl_8087F3C0, this, 1001, 0);
        mFlag25 = 1;
        mColli.mFlags &= ~1;
        mFlag15 = 1;
        fn_801765D8();
    } else {
        CollResult_unk result;
        Sphere_unk sphere = fn_80176548(mPos);
        int hit = fn_8004D388(lbl_8087EE98, &result, &sphere, &m1540, 0x80000080, &mColli, 0, sphere.mRadius);
        m1540.y += m154C;
        if (hit != 0) {
            m1540 *= -0.3f;
            if (mPos.y < result.m10.y) {
                mPos.y = result.m10.y;
                if (__fabs(m1540.y) < 0.5f) {
                    m1540 = lbl_807C7030;
                }
            }
        }
    }
    mPos += m1540;
}

void Em316LaserSphere::fn_803191FC() {
    if (m14C0 >= 20) {
        mPos = m14E4;
        m1578 = 0;
        int wait = m1C84;
        m1574 = wait + rand() % wait;
        if (fn_80316F74(&mStatus) || mStatus.IsState_unk(0x800000)) {
            m1574 *= 2;
        }
        fn_80319C10();
    } else {
        mPos = LaserLerp_unk(mPos, m14E4, m14F0);
    }
}

void Em316LaserSphere::fn_80319384() {
    float rate = 0.34f;
    if (fn_80316F74(&mStatus) || mStatus.IsState_unk(0x800000)) {
        rate *= 0.1f;
    }
    if (mStatus.IsState_unk(0x1000000)) {
        rate *= 0.01f;
    }
    m151C = rate * lbl_8087EFA8->m3A4 + m151C;
    float t = m151C < 1.0f ? m151C : 1.0f;
    mPos = LaserLerp_unk(m1504, m1510, t);
    if (IsMoveDone_unk()) {
        if (m58C == 12) {
            SetState_unk(11);
            fn_80239DAC(lbl_8087F3C0, this, 1000, 1);
        } else {
            fn_80319C10();
        }
        m1520 = 0;
    }
}

void Em316LaserSphere::fn_80319578() {
    if (m14C0 > 5) {
        fn_80319C90();
    }
}

void Em316LaserSphere::fn_803199A4() {
    if (__fabs(m1534 - m1538) <= 0.01f) {
        fn_80319C10();
        return;
    }
    float rate = 0.35f;
    if (fn_80316F74(&mStatus) || mStatus.IsState_unk(0x800000)) {
        rate *= 0.1f;
    }
    if (mStatus.IsState_unk(0x1000000)) {
        rate *= 0.01f;
    }
    m1534 = rate * (m1538 - m1534) + m1534;
    atn::Vector3 v(m1528);
    Mtx rot;
    PSMTXRotRad(rot, 'y', m1534);
    PSMTXMultVec(rot, &v, &v);
    mPos = fn_80318648(m14B8) + v * 25.0f;
}

void Em316LaserSphere::fn_80319C10() {
    SetState_unk(0);
    fn_80239DAC(lbl_8087F3C0, this, 1000, 1);
    fn_80239DAC(lbl_8087F3C0, this, 1001, 1);
}

// inferred: settings (lbl_8087F0A8, as in game/CopyRobotTask.cpp): frames per second at 0x30
struct LaserSettings_unk {
    unsigned char m00[0x30];
    int mFrameRate;
};
extern "C" LaserSettings_unk* lbl_8087F0A8;

void Em316LaserSphere::fn_8031A484() {
    SetState_unk(2);
    fn_80239DAC(lbl_8087F3C0, this, 1000, 1);
    fn_80239DAC(lbl_8087F3C0, this, 1001, 1);
    fn_800C344C(sSounds_unk[3], &mPos, 0, -1, 1.0f);
    if (!IsZero3_unk(m6B8)) {
        atn::Vector3 d(m6B8);
        PSVECNormalize(&d, &d);
        m1540 = d * 2.0f;
        m1540.y *= 0.5f;
        if (m1540.y > 0.0f) {
            m1540.y = 0.0f;
        }
    }
    int fps = lbl_8087F0A8->mFrameRate;
    m154C = 1.5f * (-98.1f / (float)(fps * fps) * lbl_8087EFA8->m3A4);
    CharaTask* caught = m15B4;
    if (caught != 0 && caught->m55C == 6 && caught->m560 == 0x8C) {
        fn_80319998((LaserCatch_unk*)fn_80318F90(caught));
        m15B4 = 0;
    }
    fn_800EB7A0(this);
}

int Em316LaserSphere::vfE4() {
    return m58C == 8;
}
