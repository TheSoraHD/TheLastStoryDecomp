#include "chr/ControlUseSkill.h"

// inferred: effect/bullet manager (lbl_8087F048, as in game/CharaTask.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" void* lbl_8087F430; // inferred: field manager (as in game/CharaTask.cpp)
extern "C" int fn_80375184(void* field);
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" int fn_800F8548(Effect_unk* mgr);
// inferred: fire the skill (bullet, area or simple effect); argument names unknown
extern "C" void fn_800F8574(Effect_unk* mgr, CharaTask* chara, HitInfo_unk* data, const atn::Vector3* pos,
                            const atn::Vector3* dir, int a, int b, int c, float f1, float f2);
extern "C" void fn_800F8C6C(Effect_unk* mgr, int a, int b, CharaTask* chara, HitInfo_unk* data, int id, int time,
                            int c, float f);
extern "C" void fn_800FB4B0(Effect_unk* mgr, CharaTask* chara, HitInfo_unk* data, int id, const atn::Vector3* pos,
                            int a, int time, int b, int c);
extern "C" void fn_801092C8(Effect_unk* effect, int msg, CharaTask* chara, int value, int arg);
// inferred: parameters of fn_8003EA3C (defined in game/CharaTask.cpp)
struct HitParam_unk;
extern "C" HitParam_unk lbl_807C6B90; // inferred: default parameters
// inferred: result of fn_8003EA3C (as in game/Em002Skelton.cpp); names invented
struct DamageResult_unk {
    DamageResult_unk() : m00(0), m04(0), m08(0), m0C(0), m10(0), m14(-1) {
        mFlags31_28 = 0;
        m18 = -1;
    }
    int m00;
    int m04;
    int m08;
    int m0C;
    int m10;
    int m14;
    int m18;
    unsigned int mFlags31_28 : 4;
    unsigned int mFlags27_0 : 28;
};
extern "C" void fn_8003EA3C(DamageResult_unk* result, HitInfo_unk* data, CharaTask* attacker, CharaTask* owner,
                            unsigned int flags, const HitParam_unk* param, int arg, int arg2); // inferred: computes the damage

namespace chr {

ControlUseSkill::ControlUseSkill(Chara_unk* chara, void* data, CharaTask* target, int motion, const char* name, float f)
    : ControlBase(chara), mData((HitInfo_unk*)data), mMotion(motion), mFrame(f), mNode(0) {
    chara->m560 = 0x25;
    Motion_unk* m = &mChara->mMotion;
    if (mMotion < 0) {
        mMotion = 0x6A;
    }
    if (mFrame < 0.0f) {
        mFrame = 24.0f;
    }
    if (name != 0) {
        mNode = m->GetNodeMtx_unk(name);
    }
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->fn_80097C08(0, mMotion, 0, 0, 1, 0.0f, 0.1f);
    m->mTracks[0].m0C = 1.0f;
    if (data != 0 && (int)((HitInfo_unk*)data)->m02 == 4) {
        mTarget = mChara;
    } else {
        mTarget = target;
    }
    if (mChara->IsOnLedge_unk()) {
        mChara->fn_801539E0();
    }
}

// NONMATCHING: 99.9% - copying the node translation into pos swaps r3/r4 (source and destination addresses)
int ControlUseSkill::vf0C() {
    Motion_unk* motion = &mChara->mMotion;
    int result = 0;
    atn::Vector3 rot;
    atn::Vector3 dir(mTarget->mPos.x - mChara->mPos.x, mTarget->mPos.y - mChara->mPos.y,
                     mTarget->mPos.z - mChara->mPos.z);
    if (!(atn::Abs_unk(fn_805F9920(&dir)) < 1.1920929e-7f)) {
        rot = dir.GetNormalize().ToRotation_unk();
    } else {
        rot = mChara->mRot;
    }
    float frame = motion->mTracks[0].m08;
    if (atn::Abs_unk(frame - mFrame) < 1.1920929e-7f) {
        HitInfo_unk* data = mData;
        if (data == 0) {
            goto end;
        }
        signed char kind = data->m02;
        if (kind == 2 || kind == 6 || kind == 1) {
            atn::Vector3 pos = mChara->mPos + atn::Vector3(0.0f, 10.0f, 0.0f);
            if (mNode != 0) {
                pos = ((atn::Matrix3x4*)mNode)->GetTrans_unk();
            }
            atn::Vector3 d = dir;
            if (!(atn::Abs_unk(fn_805F9920(&d)) < 1.1920929e-7f)) {
                PSVECNormalize(&d, &d);
            }
            fn_800F8574(lbl_8087F048, mChara, data, &pos, &d, 0, 0, 0, 0.0f, 1.0f);
        } else if (data->m01 == 0) {
            fn_800F8C6C(lbl_8087F048, 0, 0, mChara, data, fn_800F8548(lbl_8087F048), 30, -1, 0.0f);
        } else if ((data->mAC & 0x100000) == 0x100000) {
            DamageResult_unk res;
            fn_8003EA3C(&res, data, mChara, mChara, 0, &lbl_807C6B90, 0, 0);
        } else {
            Effect_unk* mgr = lbl_8087F048;
            fn_800FB4B0(mgr, mChara, data, fn_800F8548(mgr), &mChara->mPos, 0, 30, -1, 1);
        }
        if (lbl_8087F430 != 0 && fn_80375184(lbl_8087F430) && data->m04 == mChara->mStatus.m2D0[3].m[0]) {
            if (mChara->mType == 2) {
                fn_801092C8(lbl_8087F048, 0xC, mChara, data->m04, 0);
            } else {
                fn_801092C8(lbl_8087F048, 0xD, mChara, data->m04, 0);
            }
        }
        if ((data->mAC & 0x40000) == 0x40000) {
            mChara->mStatus.m228 = 0.0f;
        } else if (mChara->mType == 0) {
            mChara->mStatus.m08 = 0.0f;
        }
    } else if (frame >= motion->fn_80097D7C(0)) {
        if (mChara->mC58.mDC != 0 && (mChara->mType == 2 || mChara->mType == 3)) {
            mChara->mC58.mDone_unk = 1;
        }
        result = 1;
    }
end:
    mChara->vf34(&rot, 0, 0.0f, 1.0f);
    return result;
}

} // namespace chr
