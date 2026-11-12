#include "game/LastCocoonImago.h"
#include "chr/ControlBase.h"
#include "game/GameMessageList.h"
#include "game/GimmickManager.h"
#include "game/GameTask.h"
#include <boost/tokenizer.hpp>
#include "std/string_members.h"
#include "std/string.h"

// inferred: effect/bullet manager (lbl_8087F048); only the member used here (name invented)
struct Effect_unk {
    unsigned char m00000[0x3E2E8];
    void* m3E2E8; // inferred: sphere of the current target (compared with m18FC)
};
extern "C" Effect_unk* lbl_8087F048;
// inferred: field (lbl_8087F430); only the member used here
struct Field_unk {
    unsigned char m0000[0x10D8];
    RouteTable_unk* m10D8; // inferred: route table of the map
    unsigned char m10DC[0x54F4 - 0x10DC];
    unsigned char m54F4[4]; // inferred: name table (fn_803B3B38)
};
extern "C" Field_unk* lbl_8087F430;
extern "C" wchar_t* wcscpy(wchar_t* dst, const wchar_t* src);
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
extern "C" atn::Vector3 lbl_807C7030;        // inferred: zero vector
extern "C" int fn_80051B14(const atn::ColliSphereShape_unk* s, const atn::ColliCapsuleShape_unk* c); // inferred: hit test

extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" std::string fn_8006AFF8(const std::string& a, const std::string& b); // inferred: a + b

// NONMATCHING: the original is over the inlining budget (docs/inlining.md R3) and calls every inline
// (LH_Binary, Vector3, LH_Xsca and ResHandle constructors, the strings, the tokenizer); this one stays under it
// and inlines them (same problem as the constructor of LastCocoonChrysalis, docs/close-one-func.md)
LastCocoonImago::LastCocoonImago(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14B8(0), m14BC(0), m14C0(0), m14C4(0), m14D0(5), m18C8(1), m18CC(1),
      m18D0(0), m18EC(0), m18F0(0), m1960(0), m1978(0), m197C(0), m1984(0), m1988(40.0f) {
    mFlag22 = 1;
    std::string dir("database/chara/special/");
    std::string name("db_last_cocoon.csv");
    std::string params(info->m2C);
    typedef boost::tokenizer<boost::char_separator<char> > Tokenizer;
    Tokenizer tok(params, boost::char_separator<char>(";"));
    for (Tokenizer::iterator it = tok.begin(); it != tok.end(); ++it) {
        if (strncmp(it->c_str(), "db_name=", 8) == 0) {
            name = it->c_str() + 8;
        }
    }
    std::string path = fn_8006AFF8(dir, name);
    m14B0.vf0C(path.c_str());
    m14F0.LoadRequest("ef_sk023a.efp");
    m18F8 = 0;
    m1948 = lbl_807C7030;
    m1954 = 30.0f;
    m1958 = 0;
    m195C = 0;
    m1964.vf0C("camera/battle/cm000_em206_00.xsca");
    m196C.LoadRequest("ef_uc030.efp");
    mFlagC31 = 1;
}

extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
// inferred: RouteTable_unk::FindPlace_unk with the array read inside the loop (as in
// game/LastCocoonQuadruped.cpp)
inline RoutePlace_unk* FindImagoPlace_unk(RouteTable_unk* table, int id) {
    for (unsigned int i = 0; i < table->mNumPlaces; i++) {
        RoutePlace_unk* places = table->mPlaces;
        if (id == places[i].mId) {
            return &places[i];
        }
    }
    return 0;
}

int LastCocoonImago::vf0C() {
    if (fn_8013655C() == 0 && m14F0.IsLoading() == 0 && m14B0.fn_80473F50() == 0 && m1964.fn_80473F50() == 0 &&
        m196C.IsLoading() == 0) {
        mStatus.m18 |= 0x1C0;
        mStatus.m18 |= 0x10000;
        mStatus.m18 |= 0xC219;
        mStatus.m18 |= 0x3800000;
        fn_80087994(m1980.Load("LastCocoonImago"), "DebugDisp", &m1984, 0, 0);
        if (m14B0.fn_80470580() != 0 && lbl_8087F430 != 0 && lbl_8087F430->m10D8 != 0) {
            m18F4 = FindImagoPlace_unk(lbl_8087F430->m10D8, 2);
        }
        m56C = 0.02f;
        m5B0 = 25.0f;
        m5B4 = 120.0f;
        for (int i = 0; i < 11; i++) {
            mCapsules[i].mFlags |= 3;
            mCapsules[i].m0C = (int)this;
            mCapsules[i].mFlags &= ~0x80000000;
        }
        m18FC.mFlags |= 3;
        m18FC.m0C = (int)this;
        vf20();
        ResetAttacks_unk();
        return 1;
    }
    return 0;
}

// inferred: effect manager (class unknown), global instance at 0x8087F3C0 (as in game/LastCocoonEgg.cpp)
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
extern "C" void fn_80232B7C(void* owner, int arg);
extern "C" double sqrt(double x);
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

void LastCocoonImago::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (m58C == 12 && msg->mData->m04 != 0x5BC) {
        return;
    }
    // inferred: HitMsg_unk+0x40 is the index of the part hit (as in game/Em205Ogre.cpp)
    if (*(int*)&msg->m3C[4] == 3) {
        if (m18F8->m54 == 3 || m18F8->m54 == 4) {
            return;
        }
        if (++m1958 > 3) {
            GimmickMsg_unk gm;
            atn::Vector3 d = mPos - m1948;
            if ((float)sqrt(d.x * d.x + d.z * d.z) < 30.0f) {
                gm.mType = 3;
                m14BC = 0;
                m14C0 = 0;
                m590 = fn_800F8548(lbl_8087F048);
                m58C = 14;
                fn_8016E970(3);
                mMotion.m34C = 1;
                mMotion.mTracks[0].m20 = 1.0f;
                mMotion.mTracks[0].m0C = 1.0f;
                mMotion.fn_80097C08(0, 0x2F, 0, 0, 1, 0.0f, 0.1f);
                m574 = lbl_807C7030;
            } else {
                gm.mType = 4;
                m195C = 1;
            }
            m18F8->vf40(&gm);
            m18FC.mFlags &= ~1;
        } else {
            GimmickMsg_unk gm;
            gm.mType = m1958 % 2 == 0 ? 2 : 6;
            m18F8->vf40(&gm);
            if (((GameTask*)lbl_8087F430)->fn_80370A78(0xD3) == 0) {
                ((GameTask*)lbl_8087F430)->fn_80370AE4(0xD3, 1);
            }
        }
        return;
    }
    msg->m10 *= 0.0f;
    if (m58C == 14 || m58C == 12) {
        msg->m0C |= 0x80;
    } else if (!IsDown_unk() && !msg->mData->m90.Check(0x80000)) {
        msg->m0C |= 8;
    }
    if (*(int*)&msg->m3C[4] == 1) {
        msg->m0C |= 0x80000000;
        msg->m0C &= ~8;
        fn_80232B7C(0, 0);
        PlayEfp_unk(&m196C, &mMotion);
    }
    fn_80151448(msg);
    vf40(hit);
}

atn::Vector3 LastCocoonImago::vf64(int center) {
    atn::Vector3 result = mPos;
    if (center == 0) {
        result = mPos;
    } else if (m18D0 != 0 && m58C != 12) {
        result = m624.mData[m18C8].m04.mCenter;
    }
    return result;
}

// inferred: the three attacks of the imago: nodes, two values, a radius and the wait before the first one
// (names invented)
struct ImagoAttack_unk {
    const char* mNode0;
    const char* mNode1;
    const char* mNode2;
    int m0C;
    int m10;
    float mRadius;
    int mWait;
};
static const ImagoAttack_unk sImagoAttacks_unk[3] = {
    {"reference", "", "", -1, -1, 25.0f, 0},
    {"head", "", "em206_body_l", 100, 200, 12.0f, 1000},
    {"spine1", "", "", -1, -1, 30.0f, 1000},
};

// inferred: hit capsules of the body: two nodes and a radius (vf20; names invented)
struct ImagoCapsule_unk {
    const char* mFrom;
    const char* mTo;
    float mRadius;
};
static const ImagoCapsule_unk sImagoCapsules_unk[11] = {
    {"neck", "ef_eye", 8.0f},
    {"spine", "neck", 20.0f},
    {"rightleg", "rightfoot", 10.0f},
    {"rightupleg", "rightleg", 10.0f},
    {"leftleg", "leftfoot", 10.0f},
    {"leftupleg", "leftleg", 10.0f},
    {"righthand", "rightmiddle3", 10.0f},
    {"rightarm", "righthand", 10.0f},
    {"lefthand", "leftmiddle1", 10.0f},
    {"leftarm", "lefthand", 10.0f},
    {"tail1", "tail3", 15.0f},
};

inline void LastCocoonImago::ResetAttacks_unk() {
    for (int i = 0; i < 3; i++) {
        m18D4[i].mWait = sImagoAttacks_unk[i].mWait;
        m18D4[i].mCount = 0;
    }
}

inline void LastCocoonImago::SetMode_unk(int mode) {
    m14BC = 0;
    m14C0 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    vf48();
    fn_80178A6C();
    fn_8015495C(0, 1, 1, 1);
    fn_8016DA4C();
    m58C = mode;
    fn_8016E970(3);
}

// inferred: name of the character, unless the current target is its sphere
int LastCocoonImago::vfB8(wchar_t* name) {
    if (lbl_8087F048 != 0 && lbl_8087F048->m3E2E8 != &m18FC && m5C != 0 && m5C->mC4 > 0 && lbl_8087F430 != 0) {
        GameMessage* entry = ((GameMessageList*)lbl_8087F430->m54F4)->fn_803B3B38(m5C->mC4);
        if (entry != 0) {
            wcscpy(name, entry->mText);
            return 1;
        }
    }
    return 0;
}

// NONMATCHING: 98.5% - stack slots of the temporaries of the two branches (and the load order of the nodes)
void LastCocoonImago::vf54() {
    if (m624.mSize != 0) {
        for (int i = 0; i < 3; i++) {
            if (i == 0) {
                atn::Vector3 dir;
                dir.x = 0.0f;
                dir.y = 0.0f;
                dir.z = 1.0f;
                atn::Matrix3x4 rot;
                rot.fn_8013A13C(mRot.y);
                PSMTXMultVec(rot.m, &dir, &dir);
                m624.mData[i].m04.mCenter = mPos - dir * 3.0f;
                m624.mData[i].m04.mCenter.y += 0.5f * sImagoAttacks_unk[i].mRadius;
            } else {
                MtxPtr a = mMotion.GetNodeMtx_unk(sImagoAttacks_unk[i].mNode0);
                MtxPtr b = mMotion.GetNodeMtx_unk(sImagoAttacks_unk[i].mNode1);
                if (a != 0) {
                    if (b != 0) {
                        atn::Vector3 pb = ((atn::Matrix3x4*)b)->GetTrans_unk();
                        atn::Vector3 pa = ((atn::Matrix3x4*)a)->GetTrans_unk();
                        m624.mData[i].m04.mCenter = pa + (pb - pa) * 0.6f;
                    } else {
                        m624.mData[i].m04.mCenter = ((atn::Matrix3x4*)a)->GetTrans_unk();
                    }
                }
            }
        }
        m624.mData[3].m04.mCenter = m1948;
    }
}

void LastCocoonImago::vf24() {
    if (m55C == 6 && mControl && !mControl->vf3C()) {
        return;
    }
    if (m197C) {
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x3B, 1, 0, 1, 0.0f, 0.025f);
    } else if (m14C8 != 0) {
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x14, 1, 0, 1, 0.0f, 0.025f);
    } else {
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 2, 1, 0, 1, 0.0f, 0.025f);
    }
}

// NONMATCHING: 98.8% - stack slots of the two points and the register of m614.z
// inferred: places the collision capsule, the hit capsules of the body and the sphere
void LastCocoonImago::vf20() {
    m614 = mPos + m5A4;
    m620 = m5B0;
    m614.y += m620;
    atn::Vector3 a = m614;
    atn::Vector3 b = a;
    b.y += m5B4 - 2.0f * m620;
    mColli.m3C = a;
    mColli.m48 = b;
    mColli.m54 = m620;
    for (int i = 0; i < 11; i++) {
        float r = sImagoCapsules_unk[i].mRadius;
        a = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk(sImagoCapsules_unk[i].mFrom))->GetTrans_unk();
        b = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk(sImagoCapsules_unk[i].mTo))->GetTrans_unk();
        mCapsules[i].m3C = a;
        mCapsules[i].m48 = b;
        mCapsules[i].m54 = r;
    }
    m18FC.mSphere = atn::ColliSphereShape_unk(m1948, m1954);
    m148C = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("head"))->GetTrans_unk();
    m1498 = 13.0f;
}

// NONMATCHING: 98.2% - stack slots of the rotation and r30/r31 of dir and of the copy
// inferred: placed at pos facing dir, in mode 6 (motion 2)
void LastCocoonImago::vf80(const atn::Vector3& pos, const atn::Vector3& dir) {
    m14BC = 0;
    m14C0 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 6;
    fn_8016E970(3);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 2, 0, 1, 1, 1.0f, 0.025f);
    mPos = pos;
    mMotion.mTracks[0].m08 = 0.0f;
    atn::Vector3 rot = dir;
    rot.fn_80139F60();
    mRot = rot;
    fn_80144710();
    mMotion.CalcMatrixStack(atn::ModelNodeCallback_unk());
}

void LastCocoonImago::vf38(int enable) {
    for (int i = 0; i < 11; i++) {
        if (enable) {
            mCapsules[i].mFlags |= 1;
        } else {
            mCapsules[i].mFlags &= ~1;
        }
    }
}

void LastCocoonImago::fn_8045D224() {
    if (m18F8->m54 == 5 && m18F8->vf38() != 0) {
        // inferred: the model is read again before each access (a local per access keeps the constants
        // out of the saved registers)
        atn::Model* model = m18F8->vf38();
        model->mTracks[0].m0C = 1.0f;
        model = m18F8->vf38();
        if (model->mTracks[0].m08 < 201.0f) {
            model = m18F8->vf38();
            model->mTracks[0].m08 = 201.0f;
        }
    }
    if (m14C0 == 120) {
        LastCocoonImago::vf38(0);
        if (((GameTask*)lbl_8087F430)->fn_80370A78(0xCD) == 0) {
            ((GameTask*)lbl_8087F430)->fn_80370AE4(0xCD, 1);
        }
    } else {
        mMotion.fn_80097D7C(0);
    }
}

void LastCocoonImago::fn_8045E548() {
    if (m14BC == 0) {
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            m14EC = 210;
            m14BC = 1;
            mMotion.fn_80097C08(0, 0x14B, 1, 1, 1, 0.0f, 0.025f);
        }
    } else if (m14BC == 1) {
        if (--m14EC < 0) {
            ResetAttacks_unk();
            m14BC = 2;
            mMotion.fn_80097C08(0, 0x14C, 0, 1, 1, 0.0f, 0.025f);
        }
    } else if (m14BC == 2) {
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            atn::Vector3 dir;
            dir.x = 0.0f;
            dir.y = 0.0f;
            dir.z = 1.0f;
            atn::Matrix3x4 rot;
            rot.fn_8013A13C(mRot.y);
            PSMTXMultVec(rot.m, &dir, &dir);
            dir *= 15.0f;
            atn::Vector3 pos = dir;
            m14BC = 0;
            m14C0 = 0;
            m590 = fn_800F8548(lbl_8087F048);
            m58C = 10;
            fn_8016E970(3);
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x14D, 0, 1, 1, 0.0f, 0.025f);
            m14D4 = pos;
            return;
        }
    }
    fn_8045ABB4(m14E0, 0.0f, m568);
}

void LastCocoonImago::fn_8045EB60() {
    int fromHang = m58C == 14;
    SetMode_unk(2);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.fn_80097C08(0, fromHang ? 0x31 : 0x37, 0, 0, 1, 0.0f, 0.025f);
    mMotion.mTracks[0].m0C = 0.5f;
    if (fromHang) {
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.mTracks[0].m08 = 200.0f;
        GimmickMsg_unk msg;
        msg.mType = 5;
        m18F8->vf40(&msg);
    }
    m574 = lbl_807C7030;
}

void LastCocoonImago::fn_8045ECC4() {
    SetMode_unk(12);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x31, 0, 0, 1, 0.0f, 0.1f);
    m574 = lbl_807C7030;
    GimmickMsg_unk msg;
    msg.mType = 5;
    m18F8->vf40(&msg);
}

// inferred: the sphere of a ColliSphere read as one shape (name invented)
inline const atn::ColliSphereShape_unk& GetShape_unk(const atn::ColliSphere& c) {
    return *(const atn::ColliSphereShape_unk*)&c.mSphere;
}
// inferred: the segment and radius of a capsule (m3C-m57) read as one shape (name invented)
inline const atn::ColliCapsuleShape_unk& GetShape_unk(const atn::ColliCapsule& c) {
    return *(const atn::ColliCapsuleShape_unk*)&c.m3C;
}

void LastCocoonImago::fn_8045F4CC() {
    for (GimmickCommon* g = GimmickManager::instance_->mHead; g != 0; g = g->m5C) {
        if (g->m50 == 1 && g->vf5C() != 0) {
            atn::ColliSphere* colli = (atn::ColliSphere*)g->vf50();
            if (colli != 0) {
                atn::ColliSphereShape_unk s = GetShape_unk(*colli);
                int hit = 0;
                atn::ColliCapsuleShape_unk c = GetShape_unk(mColli);
                if (fn_80051B14(&s, &c)) {
                    hit = 1;
                }
                c = GetShape_unk(mCapsules[6]);
                if (fn_80051B14(&s, &c)) {
                    hit = 1;
                }
                c = GetShape_unk(mCapsules[8]);
                if (fn_80051B14(&s, &c)) {
                    hit = 1;
                }
                if (hit) {
                    GimmickMsg_unk msg;
                    msg.mType = 4;
                    g->vf40(&msg);
                }
            }
        }
    }
}

// inferred: frames 80-105 of the motion of state 11
int LastCocoonImago::vfE4() {
    if (m58C == 11) {
        float frame = mMotion.mTracks[0].m08;
        if (80.0f <= frame && frame <= 105.0f) {
            return 1;
        }
    }
    return 0;
}

// NONMATCHING: 98% - offsets in the string pool (lbl_80754DE0, the strings of the unwritten functions are missing)
// inferred: the four points of the body (right shoulder, spine, left shoulder, hips)
void LastCocoonImago::vfCC(atn::Vector3* out, const atn::Vector3& dir) {
    out[0] = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("rightshoulder"))->GetTrans_unk();
    out[1] = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("spine1"))->GetTrans_unk();
    out[2] = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("leftshoulder"))->GetTrans_unk();
    out[3] = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("hips"))->GetTrans_unk();
}

// NONMATCHING: 99.4% - offset of "head" in the string pool (see vfCC)
atn::Vector3 LastCocoonImago::vfD8(CharaTask* target) {
    atn::Vector3 pos = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("head"))->GetTrans_unk();
    pos.y += 10.0f;
    return pos;
}

float LastCocoonImago::vfDC() {
    return 0.7f;
}

float LastCocoonImago::vfD4() {
    return 2.2f;
}

float LastCocoonImago::vf90() {
    return 5.0f;
}

LastCocoonImago::~LastCocoonImago() {}
