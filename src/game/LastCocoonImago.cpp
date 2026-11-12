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
    unsigned char m0000[0x54F4];
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
