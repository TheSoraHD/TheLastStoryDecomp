#include "game/SkillEffectManager.h"
#include "game/CharaTask.h"
#include "game/MapManager.h"

// Methods of SkillEffect (class name from the RTTI of the boost::bind in gmk::HealPoint and TacView;
// file name inferred). A file of its own: its float pool
// (0x80888178-0x808881C0) repeats 0.0f, which SkillEffectManager.cpp also uses (0x808881FC).

// inferred: effect manager of the efp files (fields set around the stop calls)
struct EffectMgr_unk {
    unsigned char m00[0xB8];
    int mB8;
    unsigned char mBC[0xD0 - 0xBC];
    int mD0;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" int fn_80239D58(EffectMgr_unk* mgr, void* owner, int id); // inferred: the effect is playing
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const MapColor_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_8023A680(EffectMgr_unk* mgr, atn::EffFile* res, int arg2, int arg3, atn::Model* model,
                            const atn::Vector3* pos, int arg6, const MapColor_unk* color, int arg8, int arg9,
                            int arg10, float scale);
extern "C" void fn_80232B7C(void* owner, int id); // inferred: names the next effect of `owner`
extern "C" atn::EfpFile* fn_800EFC64(int id);     // inferred: efp of a skill (game/BattleEfp_unk.cpp)
extern "C" atn::Vector3 lbl_807C7030;             // inferred: zero vector
extern "C" void fn_800CB5C8(atn::SoundRef* sound, int frames, int arg); // inferred: stops the sound
extern "C" HitInfo_unk* fn_80219E6C(int id); // inferred: skill data by id
extern "C" int fn_8021A960(HitInfo_unk* hit);
extern "C" int fn_8021A984(HitInfo_unk* hit);
extern "C" void* memcpy(void* dst, const void* src, unsigned long n);
extern "C" float PSVECSquareMag(const atn::Vector3* v);
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect

SkillEffect::SkillEffect() : m00(3), m04(0), m08(0), m10(0.0f, 0.0f, 0.0f), m1C(0.0f, 0.0f, 0.0f), m28(1.0f), m60(0),
      m7C(0) {
    m13C = 0;
}

SkillEffect::~SkillEffect() {}

// NONMATCHING: 89% - the original copies the skill data word by word (0x14-0x74 in a loop), so the
// real struct has no float members there (ours: BulletParam_unk, typed copies); the local matrices of
// RotateZYX_unk are above the rotation temporaries in the original
void SkillEffect::fn_805989A4(HitInfo_unk* skill, CharaTask* user, CharaTask* target, const atn::Vector3* pos,
                              const atn::Vector3* rot, boost::function<void(SkillEffect*)> callback, float rate) {
    if (skill == 0) {
        return;
    }
    m00 = 0;
    m04 = skill;
    m08 = user;
    if (user != 0) {
        user->fn_8017639C((CharaAttach_unk*)this); // inferred: CharaAttach_unk is a view of SkillEffect
    }
    m0C = target;
    m5C = m04->m58;
    m28 = rate;
    m60 = 0;
    mCallback = callback;
    m7C = 0;
    m13C = 0;
    char kind = m04->m02;
    if (kind == 0) {
        m10 = m0C->mPos;
        m1C = m0C->mRot;
    } else if (kind == 4) {
        m10 = m08->mPos;
        m1C = m08->mRot;
    } else if (kind == 3) {
        m10 = *pos;
        m1C = *rot;
    } else {
        m10 = *pos;
        m1C = *rot;
    }
    m2C.SetIdentity();
    m2C.RotateZYX_unk(m1C);
    m2C.fn_801479E4(m10);
    m80 = *(BulletParam_unk*)skill;
    if (fn_8021A960(m04) || fn_8021A984(m04)) {
        m80.m5C = ((BulletParam_unk*)m04)->m5C;
        for (int i = 0; i < 4; i++) {
            int v = m80.m98[i];
            if (v > 0) {
                m80.m98[i] = -(v < 5 ? v : 5);
            } else {
                m80.m98[i] = -(v > -5 ? v : -5);
            }
        }
        if (m80.mKind == 5) {
            m80.mKind = 4;
            BulletParam_unk* data = (BulletParam_unk*)fn_80219E6C(0x694);
            if (data != 0) {
                m80.mId = data->mId;
                m80.mName = data->mName;
                m80.mText = data->mText;
                m80.m64 = data->m64;
                m80.m6C = data->m6C;
                m80.m70 = data->m70;
                m80.m02 = data->m02;
                m80.m90 = data->m90;
                m80.m94 = data->m94;
                m80.mAC = data->mAC & ~0x20;
                memcpy(m80.m74, data->m74, sizeof(m80.m74));
            }
        } else if (m80.mKind == 4) {
            m80.mKind = 5;
            BulletParam_unk* data = (BulletParam_unk*)fn_80219E6C(0x7EC);
            BulletParam_unk* name = (BulletParam_unk*)fn_80219E6C(0x4EF8);
            if (data != 0 && name != 0) {
                m80.mId = name->mId;
                m80.mName = name->mName;
                m80.mText = name->mText;
                m80.m64 = data->m64;
                m80.m6C = data->m6C;
                m80.m70 = data->m70;
                m80.m02 = data->m02;
                m80.m90 = data->m90;
                m80.m94 = data->m94;
                m80.mAC = data->mAC;
                memcpy(m80.m74, data->m74, sizeof(m80.m74));
                if ((skill->mAC & 0x20) == 0x20) {
                    m80.m58 = data->m58;
                    m80.m5C = data->m5C;
                }
            }
        }
    }
    if ((skill->mAC & 0x800000) == 0x800000) {
        lbl_8087F9E8->fn_8059D3B0(m08);
    }
}

void SkillEffect::fn_805991E0() {
    if (mCallback) {
        mCallback(this);
    }
    fn_80239DAC(lbl_8087F3C0, this, m04->m04, 1);
    fn_800CB5C8(&m64, 30, 0);
    fn_8059A1FC(0);
    m00 = 3;
    if (m08 != 0) {
        m08->fn_801763E0((CharaAttach_unk*)this); // inferred: CharaAttach_unk is a view of SkillEffect
        if (m04->m04 == 6004) {
            m08->fn_80178864(m04, m08);
        }
    }
    m08 = 0;
    m0C = 0;
    m04 = 0;
    m60 = 0;
    mCallback.clear();
}

void SkillEffect::fn_80599C6C() {
    if (!IsActive_unk()) {
        return;
    }
}

int SkillEffect::fn_80599C90(CharaTask* chara) {
    CharaTask* owner = m08;
    int kind = GetHit_unk()->m64;
    if (kind == 0) {
        if (chara != owner) {
            return 0;
        }
    } else if (kind >= 1 && kind <= 2) {
        if (!chara->fn_8016F3D0(owner, 0)) {
            return 0;
        }
    } else if (kind >= 3 && kind <= 4) {
        if (chara->fn_8016F3D0(owner, 1)) {
            return 0;
        }
    }
    return 1;

}

// NONMATCHING: 99.3% - m04/m08 registers swapped in the inlined fn_80599C90 and GetHit_unk; the kind
// range test subtracts 3 before the clrlwi
int SkillEffect::fn_80599D60(CharaTask* chara, int check) {
    if (m00 != 1) {
        return 0;
    }
    if (check && !fn_80599C90(chara)) {
        return 0;
    }
    CharaTask* owner = m08;
    HitInfo_unk* hit = GetHit_unk();
    if ((hit->mAC & 0x100000) == 0x100000) {
        CharaStatus* status = &owner->mStatus;
        if (status->IsState_unk(0x20) || status->IsState_unk(0x10) || status->IsState_unk(0x40) ||
            status->IsState_unk(0x80) || status->IsState_unk(0x100) || status->IsState_unk(0x4000) ||
            status->IsState_unk(0x10000)) {
            return 0;
        }
    }
    if ((hit->m02 >= 3 && hit->m02 <= 5) || hit->m02 == 0) {
        atn::Vector3 d = chara->mPos - m10;
        float r = m5C * m28;
        if (PSVECSquareMag(&d) <= r * r) {
            return 1;
        }
    }
    return 0;
}

// NONMATCHING: 98% - float registers; the pool order needs the functions before it
float SkillEffect::fn_80599FB4(int keep) {
    float scale = 1.0f;
    if (!keep || (15.0f <= m5C && m5C <= 150.0f)) {
        scale = m5C / 15.0f;
    }
    return m28 * scale;
}

void SkillEffect::fn_80599FFC(const atn::Vector3* pos) {
    if (m04 == 0) {
        return;
    }
    atn::EfpFile* efp = fn_800EFC64(m04->mB0);
    if (efp == 0) {
        return;
    }
    fn_80232B7C(this, m04->m04);
    fn_8023A8B4(lbl_8087F3C0, efp, 0, 0, *pos, lbl_807C7030, MapColor_unk(1.0f, 1.0f, 1.0f, 1.0f), -1, -1, 1, 1.0f);
}

// NONMATCHING: 99.15% - stack slots of the sphere and light temporaries (as SkillEffectManager::fn_8059D3B0)
void SkillEffect::fn_8059A0B4(atn::EffFile* eff) {
    if (m7C) {
        return;
    }
    lbl_8087F3C0->mD0 = 1;
    lbl_8087F3C0->mB8 = 5;
    if (!fn_80239D58(lbl_8087F3C0, &m2C, 1)) {
        atn::Vector3 offset(0.0f, 5.0f, 0.0f);
        MapColor_unk light = MapManager::sInstance->fn_80491DF8(atn::Sphere(m2C.GetTrans_unk(), 2.0f));
        fn_80232B7C(&m2C, 1);
        fn_8023A680(lbl_8087F3C0, eff, -1, 5, (atn::Model*)&m2C, &offset, 0, &light, 0, -1, 1,
                    (1.0f + m5C) / 15.0f * m28);
    }
    lbl_8087F3C0->mB8 = 0;
    lbl_8087F3C0->mD0 = 0;
    m7C = 1;
}

void SkillEffect::fn_8059A1FC(int keep) {
    if (m7C) {
        lbl_8087F3C0->mD0 = 1;
        fn_80239DAC(lbl_8087F3C0, &m2C, 1, !keep);
        lbl_8087F3C0->mD0 = 0;
        m7C = 0;
    }
}

int SkillEffect::fn_8059A264() {
    if (m08 != 0 && m04 != 0 && m13C != 0) {
        return 1;
    }
    return 0;
}
