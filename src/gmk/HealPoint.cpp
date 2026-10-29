#include "gmk/HealPoint.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "game/CharaManager.h"
#include "game/PlayerTask.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>

extern "C" int strcmp(const char* a, const char* b);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int atoi(const char* s);
extern "C" char lbl_8087D710[2]; // inferred: field separators of the tab CSV
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" void fn_8008AD4C(atn::Model* model, const char* path, const char* texture); // inferred: loads a model
extern "C" void fn_80092F1C(atn::Model* model, const char* path, const char* node, float distance); // inferred: adds a LOD

extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager

// inferred: skill effect manager (class unknown); fn_8059B66C casts a skill and returns its effect,
// the callback is called when the effect ends; fn_805991E0 stops an effect
extern "C" void* lbl_8087F9E8;
extern "C" SkillEffect* fn_8059B66C(void* mgr, HitInfo_unk* skill, CharaTask* user, CharaTask* target,
                                    const atn::Vector3* pos, const atn::Vector3* rot,
                                    boost::function<void(SkillEffect*)> callback, float rate);
extern "C" void fn_805991E0(SkillEffect* effect);

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
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_80232B7C(void* owner, int arg);

namespace gmk {

inline void HealPoint::StopEffect_unk() {
    mWait = 0;
    if (mEffect != 0) {
        fn_805991E0(mEffect);
    }
}

GimmickCommon* HealPoint::fn_8040EC30(atn::Task* parent, int id, int arg) {
    if (parent != 0) {
        return new (12, "", "", 0) HealPoint(parent, id, arg);
    }
    return 0;
}

// NONMATCHING: 93.8% - scheduling of the inline constructor (the original loads desc->mRotY and the
// position between the stores of mWait and m54). Tried: rotation temporary/local first, member
// helpers, mWait/m54 stores before/after the copies, m78 before m6C.
GimmickCommon* HealPoint::fn_8040ED00(atn::Task* parent, GimmickDesc_unk* desc) {
    if (parent != 0) {
        return new (12, "", "", 0) HealPoint(parent, desc);
    }
    return 0;
}

HealPoint::~HealPoint() {}

int HealPoint::vf0C() {
    if (fn_803EC91C() == 0) {
        fn_803EDB18(&mModel);
        return 1;
    }
    return 0;
}

void HealPoint::Start() {
    vf2C();
    if (vf38() != 0) {
        fn_803ED0D4(vf38(), 1);
    }
    mColli.SetMtx_unk(*(atn::Matrix3x4*)mModel.mMtx);
    mColli.mFlags |= 1;
    mWait = 0;
    GimmickMsg_unk msg(2);
    if (m58 == 1) {
        msg.mType = 1;
    }
    vf40(&msg);
}

void HealPoint::vf10() {
    if (m54 == 0) {
        Start();
    }
    if (mWait > 0) {
        mWait--;
    }
    if (m54 == 2 && mEffect == 0 && mWait <= 0 && vf60() != 0 && mEffect == 0) {
        StartEffect_unk();
    }
    if (m54 == 1 && mEffect != 0) {
        StopEffect_unk();
    }
    if (vf38() != 0) {
        fn_803ED0D4(vf38(), 1);
    }
    mColli.SetMtx_unk(*(atn::Matrix3x4*)mModel.mMtx);
}

void HealPoint::vf14() {
    if (vf38() != 0) {
        fn_803ED774(vf38());
    }
}

int HealPoint::vf1C() {
    if (mModel.fn_8008B140() != 0) {
        return 1;
    }
    if (mColli.fn_800580BC() != 0) {
        return 1;
    }
    return mEfp.IsLoading() != 0;
}

void HealPoint::vf20() {
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    char name[0x100];
    csv.fn_8005BCE8(lbl_8087D710);
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "MODEL") == 0) {
            fn_8008AD4C(&mModel, csv.fn_8005B9CC(), 0);
            fn_803EC7A0(&mModel, &csv);
        } else if (strcmp(field, "MODEL_LOD") == 0) {
            strcpy(name, csv.fn_8005B9CC());
            fn_80092F1C(&mModel, name, 0, fn_800DC288(csv.fn_8005B9CC()));
        } else if (strcmp(field, "COLLI_OBJECT") == 0) {
            mColli.fn_80058078(csv.fn_8005B9CC());
        } else if (strcmp(field, "EFP") == 0) {
            mEfp.LoadRequest(csv.fn_8005B9CC());
        } else if (strcmp(field, "SKILL") == 0) {
            mSkill = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        }
    } while (csv.fn_8005BBF8() != 0);
}

int HealPoint::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    switch (msg->mType) {
    case 1:
        fn_80239DAC(lbl_8087F3C0, this, 0, 1);
        StopEffect_unk();
        break;
    case 2: {
        lbl_8087F3C0->mB8 = 1;
        fn_80239DAC(lbl_8087F3C0, this, 0, 1);
        fn_80232B7C(this, 0);
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
        fn_8023A8B4(lbl_8087F3C0, &mEfp, &mModel, 0, pos, rot, color, -1, -1, 1, 1.0f);
        lbl_8087F3C0->mB8 = 0;
        if (vf60() != 0 && mEffect == 0) {
            StartEffect_unk();
        }
        break;
    }
    }
    return m54 = msg->mType;
}

int HealPoint::vf60() {
    int ret = 0;
    if (m54 == 2 && !IsFlag_unk(4)) {
        ret = 1;
    }
    return ret;
}

// inferred: defined after its users so that the boost::function instances (invoker and manager)
// are emitted after vf60, as in the original
inline void HealPoint::StartEffect_unk() {
    PlayerTask* player = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0;
    if (player != 0 && mSkill != 0 && lbl_8087F9E8 != 0) {
        mEffect = fn_8059B66C(lbl_8087F9E8, mSkill, player, player, &m6C, &m78,
                              boost::bind(&HealPoint::fn_8040FAA4, this, _1), 1.0f);
    }
}

void HealPoint::fn_8040FAA4(SkillEffect* effect) {
    mEffect = 0;
    mWait = 5;
}

atn::Model* HealPoint::vf38() {
    return &mModel;
}

} // namespace gmk
