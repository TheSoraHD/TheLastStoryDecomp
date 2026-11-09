#include "game/LastCocoonQuadruped.h"
#include "atn/RenderManager.h"
#include "chr/ControlBase.h"
#include "game/CharaManager.h"
#include "game/PlayerTask.h"
#include "game/BattleTask.h"
#include "game/GameTask.h"

struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
// inferred: field (lbl_8087F430): camera shake and event flags (as in gmk/ShakeCamera.cpp)
struct QuadField_unk {
    void Shake_unk(int frame, float x, float y) {
        m96C = m96C % 2;
        m970 = frame;
        m974 = x;
        m978 = y;
    }
    unsigned char m00[0x96C];
    int m96C;
    int m970;
    float m974;
    float m978;
    unsigned char m97C[0x10D8 - 0x97C];
    RouteTable_unk* m10D8;
};
extern "C" QuadField_unk* lbl_8087F430;
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager

// inferred: nodes of the hit spheres (vf50, vf54)
static const char* const sSphereNodes_unk[3] = {"head", "waist", "spine"};

// inferred: hit capsules of the legs and the neck: two nodes and a radius (vf20; names invented)
struct QuadCapsule_unk {
    const char* mFrom;
    const char* mTo;
    float mRadius;
};
static const QuadCapsule_unk sQuadCapsules_unk[5] = {
    {"neck", "ef_eye", 8.0f},
    {"rightleg", "rightfoot", 8.0f},
    {"leftleg", "leftfoot", 8.0f},
    {"rightmiddle1", "rightforearm", 8.0f},
    {"lefthand", "leftforearm", 8.0f},
};

// inferred: angle used by fn_80460B48 (50 degrees; initialized in the __sinit; name invented)
static float sAngle_unk = atn::DegToRad_unk(50.0f);

// inferred: game state (as in game/CharaTask.cpp); names invented
struct QuadGameState_unk {
    unsigned char m000[0x3A4];
    float m3A4; // inferred: time of a frame
};
extern "C" QuadGameState_unk* lbl_8087EFA8;
extern "C" float fn_800A08D4(atn::LoadHandle* motion); // inferred: length of the motion data
extern "C" float PSVECSquareMag(const atn::Vector3* v);

// inferred: as in game/CharaTask.cpp (names invented)
inline float GetMotionLength_unk(atn::Model* model, int id) {
    atn::LoadHandle* motion = model->fn_80097D40(id);
    float length = motion != 0 ? fn_800A08D4(motion) : 30.0f;
    if (length == 0.0f) {
        length = 1.0f;
    }
    return length;
}
inline float Clamp01_unk(float x) {
    if (x < 0.0f) {
        x = 0.0f;
    } else if (x > 1.0f) {
        x = 1.0f;
    }
    return x;
}
inline float LengthSq_unk(const atn::Vector3& v) {
    return PSVECSquareMag(&v);
}

// inferred: damage record shown by fn_80108C10 (as StatusDamage_unk of game/CharaStatus.cpp)
struct QuadDamage_unk {
    QuadDamage_unk(int damage) : m08(0), m0C(0), m10(0), m14(-1) {
        m1C = 0;
        m18 = -1;
        m04 = damage;
        m00 = 1;
    }
    int m00;
    int m04;
    int m08;
    int m0C;
    int m10;
    int m14;
    int m18;
    unsigned int m1C : 4;
    unsigned int m1C_rest : 28;
};
extern "C" void fn_80108C10(Effect_unk* effect, QuadDamage_unk* damage, const atn::Vector3& pos, CharaTask* from,
                            CharaTask* to, int b, int c);

extern "C" void fn_800FAB80(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* bullet, int id, const atn::Vector3* pos,
                            const atn::Vector3* rot, int arg6, int arg7, int arg8, int arg9, float arg10,
                            float arg11); // inferred: fires a bullet (as in game/BattleCharaTask.cpp)

struct HitResult_unk;
extern "C" int fn_800F8C6C(Effect_unk* mgr, HitResult_unk* results, int count, CharaTask* chara, HitInfo_unk* data,
                           int id, int time, int hits, float angle); // inferred: deals a hit, returns the hits
// inferred: as in game/Em206EggMonster.cpp (name invented)
inline float WrapDeg_unk(float deg) {
    float d = fmod(deg, 360.0);
    if (d > 180.0f) {
        d -= 360.0f;
    }
    if (d < -180.0f) {
        d += 360.0f;
    }
    return d;
}

// inferred: places the quadruped walks around (ids 601-610; names invented)
struct QuadPlace_unk {
    int mId;
    int m04;
    int m08;
};
static const QuadPlace_unk sQuadPlaces[10] = {
    {601, 1, 0}, {606, 0, 1}, {602, 1, 0}, {607, 0, 1}, {603, 1, 0},
    {608, 0, 1}, {604, 1, 0}, {609, 0, 1}, {605, 1, 0}, {610, 0, 1},
};

// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk {
    unsigned char m00[0xB8];
    int mB8;
};
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, void* model, int arg3, const atn::Vector3& pos,
                            const atn::Vector3& rot, const Color4f_unk& color, int arg7, int arg8, int arg9,
                            float scale);
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" void fn_80232B7C(void* owner, int arg);
// inferred: plays an effect on the model with the default color and scale (as in game/LastCocoonEgg.cpp)
static inline void PlayEfp_unk(atn::EfpFile* efp, void* model) {
    atn::Vector3 pos;
    atn::Vector3 rot;
    rot.Set_unk(0.0f, 0.0f, 0.0f);
    pos.Set_unk(0.0f, 0.0f, 0.0f);
    Color4f_unk color;
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    color.a = 1.0f;
    fn_8023A8B4(lbl_8087F3C0, efp, model, 0, pos, rot, color, -1, -1, 1, 1.0f);
}

extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void* lbl_8087EE98; // inferred: collision manager
// inferred: result of the ray test (as in game/Em309Ghoul.cpp)
struct QuadCollResult_unk {
    QuadCollResult_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    unsigned char m00[0x04];
    atn::Vector3 m04; // inferred: hit position
    atn::Vector3 m10; // inferred: position after the sweep
    unsigned char m1C[0x34 - 0x1C];
    void* m34;
    void* m38;
    unsigned int m3C;
    int m40;
    atn::Vector3 m44;
};
extern "C" int fn_8004ED34(void* colli, QuadCollResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7); // inferred: ray test

inline void LastCocoonQuadruped::SetMode_unk(int mode) {
    m14BC = 0;
    m14C0 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    if (lbl_8087F430 != 0) {
        ((GameTask*)lbl_8087F430)->fn_80370AE4(0x8C, 0);
    }
    m58C = mode;
    fn_8016E970(3);
}

void LastCocoonQuadruped::vf14() {
    fn_80149A30();
}

void LastCocoonQuadruped::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (m14C4 != 7 && m58C != 8) {
        if (m14C4 == 6 || m14C4 == 1 || (m58C >= 17 && m58C <= 19)) {
            msg->m10 *= 0.0f;
        }
        fn_80151448(msg);
        vf40(hit);
    }
}

atn::Vector3 LastCocoonQuadruped::vf64(int center) {
    atn::Vector3 result = mPos;
    result = m624.mData[0].m04.mCenter;
    return result;
}

// inferred: sets the segment and the radius of a capsule (name invented)
inline void SetCapsule_unk(atn::ColliCapsule& c, const atn::Vector3& a, const atn::Vector3& b, float r) {
    c.m3C = a;
    c.m48 = b;
    c.m54 = r;
}

// inferred: hit spheres on the head, waist and spine
void LastCocoonQuadruped::vf50() {
    Item14_unk item;
    item.m04.mRadius = 1.5f * m5B0;
    for (int i = 0; i < 3; i++) {
        item.m04.mCenter = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk(sSphereNodes_unk[i]))->GetTrans_unk();
        m624.push_back(item);
    }
    mFlagB29 = 0;
}

void LastCocoonQuadruped::vf54() {
    if (m624.mSize != 0) {
        for (int i = 0; i < 3; i++) {
            MtxPtr mtx = mMotion.GetNodeMtx_unk(sSphereNodes_unk[i]);
            m624.mData[i].m04.mCenter = ((atn::Matrix3x4*)mtx)->GetTrans_unk();
            if (i == 1) {
                m624.mData[i].m04.mRadius = 10.0f;
            } else if (i == 2) {
                m624.mData[i].m04.mCenter = (m624.mData[0].m04.mCenter + m624.mData[1].m04.mCenter) * 0.5f;
                m624.mData[i].m04.mRadius = 17.0f;
            }
        }
    }
}

void LastCocoonQuadruped::vf20() {
    m620 = 1.5f * m5B0;
    m614 = mPos + m5A4;
    m614.y += m620;
    float radius = m5B0;
    atn::Vector3 a = m614;
    atn::Vector3 b = a;
    b.y += MAX(m5B4 - 2.0f * radius, 1.0f);
    SetCapsule_unk(mColli, a, b, radius);
    for (int i = 0; i < 5; i++) {
        float r = sQuadCapsules_unk[i].mRadius;
        a = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk(sQuadCapsules_unk[i].mFrom))->GetTrans_unk();
        b = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk(sQuadCapsules_unk[i].mTo))->GetTrans_unk();
        SetCapsule_unk(mCapsules[i], a, b, r);
    }
    a = mPos;
    b = a;
    b.y += 100.0f;
    SetCapsule_unk(m17A8, a, b, 50.0f);
}

void LastCocoonQuadruped::fn_804617A4() {
    if (m58C >= 11 && m58C <= 12) {
        mC58.mC8 = (int)mC58.mC4;
        m1454 = 0;
        m14B8 = m1554;
        mC58.mC4 = m1554;
    } else if (m58C == 13) {
        mC58.mC8 = (int)mC58.mC4;
        m1454 = 0;
        m14B8 = m1574;
        mC58.mC4 = m1574;
    } else if (m58C == 17) {
        mC58.mC8 = (int)mC58.mC4;
        m1454 = 0;
        m14B8 = m15C8;
        mC58.mC4 = m15C8;
    } else {
        m1454 = 1;
        m14B8 = mC58.mC4;
    }
}

// NONMATCHING: 99.5% - the frame time (dt) is in f28 here, f29 in the original. Tried: local
// declaration orders, a copy of dt after the motion lengths, separate weights.
// inferred: walk motions blended by speed (simplified CharaTask::vf24)
void LastCocoonQuadruped::vf24() {
    if (mFlag6) {
        return;
    }
    if (m55C == 6 && mControl && mControl->vf3C() == 0) {
        return;
    }
    if (m55C == 9 || mFlag2) {
        return;
    }
    if (m55C == 2 && mC58.mMode >= 20 && mC58.mMode <= 22 && !mC58.mDone_unk) {
        return;
    }
    float dt = lbl_8087EFA8->m3A4;
    float walk = GetMotionLength_unk(&mMotion, m484[1]);
    float run = GetMotionLength_unk(&mMotion, m484[3]);
    float hi;
    float rate = m570 / dt;
    float lo = m500;
    hi = m508;
    float blend;
    float walkSpeed;
    float runSpeed;
    if (rate < lo) {
        blend = rate / lo;
        walkSpeed = 1.0f;
        runSpeed = 0.0f;
    } else if (rate < hi) {
        blend = 1.0f;
        runSpeed = 0.0f;
        walkSpeed = blend + (rate - lo) / (hi - lo);
    } else {
        blend = 2.0f;
        walkSpeed = 0.0f;
        runSpeed = 0.1f * (rate / hi - 1.0f) + 1.0f;
    }
    if (LengthSq_unk(atn::RenderManager::instance_->mCamera.mPos - mPos) > 62500.0f) {
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        if (blend < 0.5f) {
            mMotion.fn_80097C08(0, m484[0], 1, 0, 1, 0.0f, 0.1f);
            mMotion.mTracks[0].m0C = 1.0f;
        } else if (blend < 1.5f) {
            mMotion.fn_80097C08(0, m484[1], 1, 0, 1, 0.0f, 0.1f);
            mMotion.mTracks[0].m0C = walkSpeed;
        } else {
            mMotion.fn_80097C08(0, m484[3], 1, 0, 1, 0.0f, 0.1f);
            mMotion.mTracks[0].m0C = runSpeed;
        }
    } else {
        mMotion.m34C = 3;
        float w = Clamp01_unk(1.0f - blend);
        mMotion.fn_80097C08(0, m484[0], 1, 0, 1, 0.0f, 0.1f);
        mMotion.mTracks[0].m20 = w;
        mMotion.mTracks[0].m0C = 1.0f;
        w = Clamp01_unk(1.0f - (float)__fabs(1.0f - blend));
        mMotion.fn_80097C08(1, m484[1], 1, 0, 1, 0.0f, 0.1f);
        mMotion.mTracks[1].m0C = walkSpeed;
        mMotion.mTracks[1].m20 = w;
        w = Clamp01_unk(blend - 1.0f);
        mMotion.fn_80097C08(2, m484[3], 1, 0, 1, 0.0f, 0.1f);
        mMotion.mTracks[2].m0C = runSpeed;
        mMotion.mTracks[2].m20 = w;
    }
}

void LastCocoonQuadruped::fn_804635B4() {
    if (m14C4 == 4 && (m55C < 6 || m55C > 7)) {
        fn_80170A20(m1554, -1.0f, 0);
    }
    fn_80461838();
}

void LastCocoonQuadruped::fn_804638A4() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        fn_80466974();
    } else {
        if (mMotion.mTracks[0].m08 < 20.0f) {
            CharaTask* target = m14B8;
            atn::Vector3 d0 = target->mPos - mPos;
            atn::Vector3 rot = d0.GetNormalize().ToRotation_unk();
            float d = WrapDeg_unk(57.295776f * (rot.y - mRot.y));
            if ((float)__fabs(d) > atn::DegToRad_unk(2.0f)) {
                d = d / (float)__fabs(d) * atn::DegToRad_unk(2.0f);
            }
            mRot.y += d;
        }
        float f = mMotion.mTracks[0].m08;
        if (f >= 19.0f && f <= 26.0f) {
            fn_80144710();
            fn_800F8C6C(lbl_8087F048, 0, 0, this, fn_80219E6C(0x5B4), m590, 30, -1, 0.0f);
        }
    }
}

void LastCocoonQuadruped::fn_80463C58() {
    if (mMotion.mTracks[0].m08 >= 5.0f) {
        SetMode_unk(9);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x140, 0, 0, 1, 1.0f, 0.1f);
        QuadCollResult_unk res;
        mPos = m14D4;
        atn::Vector3 offset(0.0f, -1000.0f, 0.0f);
        atn::Vector3 to = offset + m14D4;
        if (fn_8004ED34(lbl_8087EE98, &res, &m14D4, &to, 0x20000000, &mColli, 0) != 0) {
            mPos = res.m10;
        }
        mRot = m14E0;
        fn_80239DAC(lbl_8087F3C0, this, 0x65, 1);
    }
}

void LastCocoonQuadruped::fn_80463DCC() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        if (m14C4 == 1 || m14C4 == 7) {
            fn_80466974();
        } else {
            fn_80466AE4();
        }
    }
}

// NONMATCHING: 99.8% - the bitfield word of the damage record is loaded into r7 (r3 in the original),
// as in Em204Mulue::fn_802A91D0
void LastCocoonQuadruped::fn_80465294() {
    if (m1574 == 0 || m1574->m560 != 0x6E) {
        SetMode_unk(14);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x148, 0, 0, 1, 0.0f, 0.1f);
        if (m1574 != 0) {
            m1574->fn_8015B238();
            m1574 = 0;
        }
        fn_80239DAC(lbl_8087F3C0, this, 1000, 0);
    } else if (m14C0 % 30 == 0) {
        float drain = 50.0f;
        QuadDamage_unk damage((int)drain);
        m1574->mStatus.fn_8012DF7C(damage.m04, 0, 0);
        fn_80108C10(lbl_8087F048, &damage, m1574->mPos + atn::Vector3(0.0f, 15.0f, 0.0f), this, m1574, 0, 0);
        damage.m04 /= -1;
        atn::Vector3 pos = fn_80148B0C(0)->m04.mCenter;
        pos += atn::Vector3(0.0f, 15.0f, 0.0f);
        mStatus.fn_8012DF7C(damage.m04, 0, 0);
        fn_80108C10(lbl_8087F048, &damage, pos, this, this, 0, 0);
    }
}

void LastCocoonQuadruped::fn_804654F0() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        fn_80466974();
    }
}

void LastCocoonQuadruped::fn_80466370() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        fn_80466974();
    } else {
        float f = mMotion.mTracks[0].m08;
        if (f <= 20.0f) {
            mPos += m15BC;
        } else if (20.0f < f && f < 24.0f && m14BC == 0) {
            HitInfo_unk* skill = fn_80219E6C(0x5B5);
            Effect_unk* mgr = lbl_8087F048;
            fn_800FAB80(mgr, this, skill, m590, &mPos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
            m14BC = 1;
        }
        atn::Vector3 dir = m15BC;
        dir.y = 0.0f;
        dir.Normalize_unk();
        mRot = dir.ToRotation_unk();
    }
}

void LastCocoonQuadruped::fn_804666D8() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        fn_80466974();
    } else {
        float f = mMotion.mTracks[0].m08;
        if (f > 80.0f) {
            if (m1800 != 0) {
                ((BattleTask*)lbl_8087F048)->fn_80107A68(&mMotion);
                m1800 = 0;
            }
            mMotion.mTracks[0].m0C = 1.0f;
        } else if (f > 60.0f) {
            if (m1800 == 0) {
                ((BattleTask*)lbl_8087F048)->fn_801079C0(&mMotion, &mMotion);
                m1800 = 1;
            }
            mMotion.mTracks[0].m0C = 0.075f;
        } else if (!(f <= 10.0f) && 10.0f < f && f <= 55.0f) {
            mPos += m15BC;
        }
    }
}

void LastCocoonQuadruped::fn_80466868() {
    if ((float)__fabs(mMotion.mTracks[0].m08 - 3.0f) < 1.1920929e-7f) {
        lbl_8087F430->Shake_unk(60, 0.3f, 0.3f);
    }
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        SetMode_unk(0);
        if (m1800 != 0) {
            ((BattleTask*)lbl_8087F048)->fn_80107A68(&mMotion);
            m1800 = 0;
        }
    }
}

void LastCocoonQuadruped::fn_80466974() {
    SetMode_unk(0);
    if (m1800 != 0) {
        ((BattleTask*)lbl_8087F048)->fn_80107A68(&mMotion);
        m1800 = 0;
    }
}

// NONMATCHING: 91.7% - "SE_EM206_001" is at @stringBase0+0xC9 (needs the earlier strings of the ctor)
void LastCocoonQuadruped::fn_80466A04() {
    SetMode_unk(6);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x14A, 0, 0, 1, 0.0f, 0.1f);
    m17A8.mFlags |= 1;
    fn_800C344C("SE_EM206_001", &mPos, 0, -1, 1.0f);
}

void LastCocoonQuadruped::fn_80466AE4() {
    SetMode_unk(7);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 2.0f;
    mMotion.fn_80097C08(0, 0x156, 0, 0, 1, 0.0f, 0.1f);
}

void LastCocoonQuadruped::fn_80466B8C() {
    SetMode_unk(8);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x13F, 0, 0, 1, 0.0f, 0.1f);
    fn_80232B7C(this, 0x65);
    PlayEfp_unk(&m1790, &mMotion);
}

// inferred: RouteTable_unk::FindPlace_unk on the field table with the array read inside the loop
// (as in game/Em208Queen.cpp)
inline RoutePlace_unk* FindQuadPlace_unk(int id) {
    RouteTable_unk* table = lbl_8087F430->m10D8;
    for (unsigned int i = 0; i < table->mNumPlaces; i++) {
        RoutePlace_unk* places = table->mPlaces;
        if (id == places[i].mId) {
            return &places[i];
        }
    }
    return 0;
}

void LastCocoonQuadruped::fn_804678F4() {
    SetMode_unk(12);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x145, 0, 0, 1, 0.0f, 0.1f);
    m155C = m1554->mPos;
    m1568 = m155C - mPos;
    m1568.y = 0.0f;
    m1568 *= 0.033333335f;
    mRot = m1568.GetNormalize().ToRotation_unk();
}

// NONMATCHING: 86% - the store of m15AC is scheduled after the step (one more saved register) and the
// subtractions are in another order. Tried: m15B0/m15AC order, pos instead of m15B0, a named
// difference, the scalar on the left.
void LastCocoonQuadruped::fn_80467C18(CharaTask* target, const atn::Vector3& pos) {
    SetMode_unk(15);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x142, 0, 0, 1, 0.0f, 0.1f);
    m15AC = target;
    m15B0 = pos;
    m15BC = (m15B0 - mPos) * 0.2f;
    m15BC.y = 4.6666665f;
    atn::Vector3 dir = m15BC;
    dir.y = 0.0f;
    dir.Normalize_unk();
    mRot = dir.ToRotation_unk();
}

// NONMATCHING: 99.8% - the two compares with the table are emitted as cmpw arg, entry (entry, arg in
// the original). Tried: both operand orders, != with continue, a reference to the entry, locals.
atn::Vector3 LastCocoonQuadruped::fn_804687B0(int* id, int kind8, int kind4) {
    atn::Vector3 result = lbl_807C7030;
    atn::Vector3 center = lbl_807C7030;
    center.x = 0.0f;
    center.y = 0.0f;
    center.z = 0.0f;
    int count = 0;
    for (PlayerTask* p = lbl_8087F8A0->mHead; p != 0; p = p->mNext) {
        if (p->IsActive_unk() || p->mType == 0) {
            count++;
            center += p->mPos;
        }
    }
    float inv = 1.0f / count;
    center *= inv;
    float maxDist = 0.0f;
    int found = 0;
    for (int i = 0; i < 10; i++) {
        const QuadPlace_unk& e = sQuadPlaces[i];
        if (e.m08 == kind8 && e.m04 == kind4) {
            RoutePlace_unk* place = FindQuadPlace_unk(e.mId);
            if (place != 0) {
                float dist = LengthSq_unk(place->mPos - center);
                if (dist > maxDist) {
                    maxDist = dist;
                    result = place->mPos;
                    found = place->mId;
                }
            }
        }
    }
    if (id != 0) {
        *id = found;
    }
    return result;
}

atn::Vector3 LastCocoonQuadruped::fn_80468A08(int id, int* next) {
    atn::Vector3 pos = lbl_807C7030;
    int nextId = 0;
    for (int i = 0; i < 10; i++) {
        if (id == sQuadPlaces[i].mId) {
            if (i == 9) {
                nextId = 601;
            } else {
                nextId = sQuadPlaces[i + 1].mId;
            }
        }
    }
    RoutePlace_unk* place = FindQuadPlace_unk(nextId);
    if (place != 0) {
        pos = place->mPos;
    }
    if (next != 0) {
        *next = nextId;
    }
    return pos;
}

int LastCocoonQuadruped::vfE4() {
    int ret = 0;
    if (m58C == 16 && m14BC == 1) {
        ret = 1;
    }
    return ret;
}

float LastCocoonQuadruped::vf90() {
    return 3.0f;
}

LastCocoonQuadruped::~LastCocoonQuadruped() {}
