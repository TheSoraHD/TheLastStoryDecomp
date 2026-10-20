#include "game/CopyRobotTask.h"
#include "std/string.h"
#include "macros.h"

extern "C" CharaInfo_unk* fn_8020924C(int id);
extern "C" void fn_8006AD24(std::string& path, const std::string& ext); // inferred: adds an extension
extern "C" void fn_804776F4(atn::LH_MotionSet* set, Model_unk* model);    // inferred: applies the motions

// inferred: party data (lbl_8087F4F0); pointers to the member data at 0x64A0
struct PartyData_unk {
    unsigned char m0000[0x64A0];
    void* mMembers[10];
};
extern "C" PartyData_unk* lbl_8087F4F0;
extern "C" int fn_80219544(void* member); // inferred: character id of a party member

struct Settings_unk {
    unsigned char m00[0x34];
    int mDrawDebug; // 0x34
};
extern "C" Settings_unk* lbl_8087F0A8;
extern "C" void* lbl_8087EEB0;
extern "C" void fn_80063484(void* draw, int segments, unsigned int color, float x, float y, float z, float rot,
                            float a, float range, float angle); // inferred: fan

struct EffectMgr_unk {
    unsigned char m00[0xB8];
    int mB8;
};
struct Color4f_unk {
    float r, g, b, a;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, Model_unk* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_8023A02C(EffectMgr_unk* mgr, void* owner, int arg2, int arg3);
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_80232B7C(void* owner, int arg);

extern "C" double acos(double x);
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" void PSMTXMultVec(const Mtx m, const atn::Vector3* src, atn::Vector3* dst);
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" void* memcpy(void* dst, const void* src, unsigned long n);
// inferred: camera (see game/CharaTask.cpp)
struct CopyCamera_unk {
    unsigned char m000[0x10C];
    atn::Vector3 mEye; // 0x10C
};
extern "C" CopyCamera_unk* lbl_8087EFB4;
// inferred: resource of the model (Model_unk::m16C); names invented
struct CopyModelRes_unk {
    unsigned char m00[0x44];
    int mNumNodes; // 0x44
};

static atn::Vector3 sFront_unk(0.0f, 0.0f, 1.0f);
static atn::Vector3 sOffset_unk(0.0f, 0.0f, 26.81f);

// inferred: character data id used for the copy robot (name invented)
inline void* GetCharaId_unk(EnemyInfo_unk* info) {
    int id = (int)info->m20;
    if ((unsigned int)(id - 0x31416) <= 9) {
        id = fn_80219544(lbl_8087F4F0->mMembers[id % 10]);
    } else {
        switch (id) {
        case 0x313EC:
            id = 1;
            break;
        case 0x313ED:
            id = 0x18769;
            break;
        case 0x313FA:
            id = 0x18895;
            break;
        case 0x313FB:
            id = 0x18831;
            break;
        case 0x31C92:
            id = 0x18831;
            break;
        case 0x31C93:
            id = 0x18895;
            break;
        case 0x31C94:
            id = 0x188F9;
            break;
        }
    }
    return (void*)id;
}

// NONMATCHING: 96.9% - info->m20 is loaded after the parameter copies instead of being interleaved with the
// LR save in the prologue (tried int/pointer/member/static helpers, local index, return/assign forms)
CopyRobotTask::CopyRobotTask(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, GetCharaId_unk(info)), m14B0(0), m14B4(0), m14B8(0) {
    m54C |= 0x2000;
    m14BC.fn_8023780C("/effect/ef_bu016.efp");
    m14C8.fn_8023780C("/effect/ef_bu017.efp");
    CharaInfo_unk* chara = fn_8020924C((int)info->m20);
    std::string path("/database/chara/motion/");
    path += chara->m18;
    fn_8006AD24(path, "mchr");
    m14D4.vf0C(path.c_str());
}

CopyRobotTask::~CopyRobotTask() {}

int CopyRobotTask::vf0C() {
    int ready = 1;
    if (fn_8013655C() != 0 || m14D4.fn_80473F50() != 0 || m14BC.fn_80237874() != 0 || m14C8.fn_80237874() != 0) {
        ready = 0;
    }
    if (ready != 0) {
        fn_804776F4(&m14D4, &mMotion);
        return 1;
    }
    return 0;
}

inline float LengthSq_unk(const atn::Vector3& v) {
    return fn_805F9920(&v);
}

inline float Length3_unk(float x, float y, float z) {
    atn::Vector3 v;
    v.x = x;
    v.y = y;
    v.z = z;
    return PSVECMag(&v);
}

// inferred: copies the model matrix and computes the largest scale axis (as CharaTask::fn_80144710);
// name invented
inline void SetModelMtx_unk(Model_unk* model, const Mtx mtx) {
    __MtxCopy(model->mMtx, mtx);
    float sz = Length3_unk(mtx[0][2], mtx[1][2], mtx[2][2]);
    float sy = Length3_unk(mtx[0][1], mtx[1][1], mtx[2][1]);
    float sx = Length3_unk(mtx[0][0], mtx[1][0], mtx[2][0]);
    atn::Vector3 s(sx, sy, sz);
    model->m54 = MAX(s.x, MAX(s.y, s.z));
}

// NONMATCHING: 99.7% - stack slots of the case 0/1 locals (d/dir, rot/rot2, front/v) are swapped, float
// registers of the scale vectors, and the inlined ToRotation_unk converts z again (frsp) instead of reusing
// the converted value (fmr), as in CharaTask::fn_80144710.
// inferred: copies the pose and the motion tracks of another character (mode 0: whole pose of m14B4,
// mode 1: mirrored motion of m14B8, modes 2-3: motion and position of m14B8)
void CopyRobotTask::vf10() {
    if (m624.mSize != 0) {
        m624.clear();
    }
    if (mC58.mC0 == 0) {
        fn_80145334();
        return;
    }
    fn_8014C540();
    switch (m14B0) {
    case 0: {
        if (m14B4 == 0) {
            break;
        }
        if (m14B4->m674 != m674) {
            fn_8014DEE4(m14B4->m674, 0, 0);
        }
        m54C |= 0x2000;
        CharaTask* src = m14B4;
        SetModelMtx_unk(&mMotion, src->mMotion.mMtx);
        mPos = m14B4->mPos;
        mRot = m14B4->mRot;
        mMotion.fn_80097C08(0, m14B4->mMotion.mTracks[0].m00, 1, 0, 1, 0.0f, 0.1f);
        memcpy(mMotion.m220, m14B4->mMotion.m220,
               (*(CopyModelRes_unk**)((char*)&mMotion + 0x16C))->mNumNodes * sizeof(Model_unk::Node_unk));
        fn_8000D430(&mMotion, boost::function<void()>());
        LengthSq_unk(lbl_8087EFB4->mEye - mPos);
        fn_80148B38();
        vf20();
        mColli.mFlags &= ~1;
        vf54();
        break;
    }
    case 1: {
        if (m14B8 == 0) {
            break;
        }
        if (m674 == 1) {
            fn_8014DEE4(0, 0, 0);
        }
        m54C |= 0x2000;
        CharaTask* src = m14B8;
        mMotion.m34C = src->mMotion.m34C;
        int n = mMotion.m34C;
        Model_unk* model = &src->mMotion;
        for (int i = 0; i < n; i++) {
            mMotion.mTracks[i].m20 = model->mTracks[i].m20;
            mMotion.mTracks[i].m0C = model->mTracks[i].m0C;
            mMotion.mTracks[i].m08 = model->mTracks[i].m08;
            mMotion.fn_80097C08(i, model->mTracks[i].m00, model->mTracks[i].m18, 0, 1, 1.0f, 0.1f);
        }
        src = m14B8;
        mPos = src->mPos;
        mPos.z = 2.0f * sOffset_unk.z - mPos.z;
        atn::Vector3 dir(0.0f, 0.0f, 1.0f);
        Mtx rot;
        PSMTXRotRad(rot, 'y', src->mRot.y);
        PSMTXMultVec(rot, &dir, &dir);
        float angle = acos(-1.0f * PSVECDotProduct(&dir, &sFront_unk));
        if (dir.x < 0.0f) {
            angle *= -1.0f;
        }
        Mtx rot2;
        PSMTXRotRad(rot2, 'y', angle);
        atn::Vector3 front(0.0f, 0.0f, 1.0f);
        atn::Vector3 v = front;
        PSMTXMultVec(rot2, &v, &v);
        dir = v;
        mRot = dir.ToRotation_unk();
        fn_80145334();
        mColli.mFlags &= ~1;
        break;
    }
    case 2:
    case 3: {
        if (m14B8 == 0) {
            break;
        }
        if (m674 == 1) {
            fn_8014DEE4(0, 0, 0);
        }
        m54C |= 0x2000;
        CharaTask* src = m14B8;
        mMotion.m34C = src->mMotion.m34C;
        int n = mMotion.m34C;
        Model_unk* model = &src->mMotion;
        for (int i = 0; i < n; i++) {
            mMotion.mTracks[i].m20 = model->mTracks[i].m20;
            mMotion.mTracks[i].m0C = model->mTracks[i].m0C;
            mMotion.mTracks[i].m08 = model->mTracks[i].m08;
            mMotion.fn_80097C08(i, model->mTracks[i].m00, model->mTracks[i].m18, 0, 1, 0.0f, 0.1f);
        }
        mPos = m14B8->mPos;
        mRot = m14B8->mRot;
        fn_80145334();
        mColli.mFlags &= ~1;
        break;
    }
    }
}

void CopyRobotTask::vf14() {
    fn_80149A30();
    if (lbl_8087F0A8->mDrawDebug != 0) {
        fn_80063484(lbl_8087EEB0, 20, 0xFFFF0000, mPos.x, 3.0f + mPos.y, mPos.z, mRot.y, 0.0f, 12.0f, 1.5707964f);
    }
}

void CopyRobotTask::fn_801CCD00(int state) {
    if (state != m14B0) {
        m14B0 = state;
        lbl_8087F3C0->mB8 = 2;
        fn_80239DAC(lbl_8087F3C0, this, 100, 1);
        if ((unsigned int)(m14B0 - 1) <= 1) {
            fn_80232B7C(this, 100);
            Color4f_unk color;
            atn::Vector3 rot;
            atn::Vector3 pos;
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
            fn_8023A8B4(lbl_8087F3C0, &m14BC, &mMotion, 0, pos, rot, color, -1, -1, 1, 1.0f);
            if (m14B0 == 1) {
                fn_8023A02C(lbl_8087F3C0, this, 100, 0x10);
                fn_8023A02C(lbl_8087F3C0, this, 100, 8);
            }
        }
        lbl_8087F3C0->mB8 = 0;
    }
}
