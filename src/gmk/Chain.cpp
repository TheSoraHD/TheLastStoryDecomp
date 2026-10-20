#include "gmk/Chain.h"
#include "atn/Memory.h"
#include "game/CharaManager.h"
#include "game/EnemyManager.h"
#include "game/NpcManager.h"

extern "C" void fn_8008AD4C(Model_unk* model, const char* path, const char* texture); // inferred: loads a model
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
// inferred: camera (only the position)
struct CameraPos_unk {
    unsigned char m00[0x10C];
    atn::Vector3 mPos; // 0x10C
};
extern "C" CameraPos_unk* lbl_8087EFB4;
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
// inferred: game settings
struct Settings_unk {
    unsigned char m00[0x18];
    int m18;
};
extern "C" Settings_unk* lbl_8087F0A8;

// inferred: node names of the characters the chain hangs from (indexed by mDesc->m28)
static const char* sNodes[] = {"const_foot_l", "const_foot_r", "left_chainpoint", "right_chainpoint",
                               "leftfoot",     "rightfoot",    "leftshoulder",    "rightshoulder"};

static inline float SqLength_unk(const atn::Vector3& v) {
    return fn_805F9920(&v);
}

namespace gmk {

GimmickCommon* Chain::fn_8043B778(atn::Task* parent, GimmickDesc_unk* desc) {
    if (parent != 0) {
        new (12, "", "", 0) Chain(parent, desc);
    }
    return 0;
}

Chain::Chain(atn::Task* parent, GimmickDesc_unk* desc)
    : GimmickCommon(parent, desc->mId, desc->mArg), mDesc(desc), mModel(1), mTarget(0), m340(0.98f) {
    m54 = 0;
    m68 = 1;
    m6C = desc->mPos;
    m78 = atn::Vector3(0.0f, desc->mRotY, 0.0f);
    m90 = atn::Vector3(1.0f, 1.0f, 1.0f);
    fn_8008AD4C(mModel.GetModel_unk(), "it087_00.model", 0);
    mModel.GetModel_unk()->mAC = 5.0f;
}

Chain::~Chain() {}

int Chain::vf0C() {
    if (fn_803EC91C() == 0 && mModel.GetModel_unk()->fn_8008B140() == 0) {
        m54 = 1;
        return 1;
    }
    return 0;
}

// NONMATCHING: 99.95% - only the place of 90000.0f in .sdata2 (fn_8043B9E0, not decompiled yet,
// creates 1.4f before it).
void Chain::vf10() {
    if (SqLength_unk(lbl_8087EFB4->mPos - m6C) > 90000.0f) {
        return;
    }
    if (mTarget == 0) {
        if (mDesc->m20 == 0) {
            mTarget = (CharaTask*)lbl_8087F8A0->mHead;
        } else if (mDesc->m20 == 1) {
            mTarget = (CharaTask*)NpcManager::sInstance->Find_unk(mDesc->m24);
        } else if (mDesc->m20 == 2) {
            mTarget = (CharaTask*)EnemyManager::sInstance->Find_unk(mDesc->m24);
        } else if (mDesc->m20 == 3) {
            mTarget = (CharaTask*)lbl_8087F8A0->Find_unk(mDesc->m24);
        }
        if (mTarget == 0 || !mTarget->IsFlag_unk(2) || mTarget->IsFlag_unk(4)) {
            mTarget = 0;
            return;
        }
        mNode = mTarget->mMotion.fn_80092814(sNodes[mDesc->m28], 0);
        fn_8043B9E0();
    } else if (mTarget->IsFlag_unk(4)) {
        mTarget = 0;
        return;
    }
    fn_8043C508();
}

void Chain::vf14() {
    if (mTarget != 0 && lbl_8087F0A8->m18 != 0) {
        fn_803ED610(mModel.GetModel_unk());
    }
}

int Chain::vf40(GimmickMsg_unk* msg) {
    if (msg != 0) {
        m54 = msg->mType;
    }
    return 0;
}

void Chain::vf3C(void* lens) {
    mModel.GetModel_unk()->m1DC = (Lens_unk*)lens;
    mModel.GetModel_unk()->mFlags |= 0x10000;
}

} // namespace gmk
