#include "game/SkillEffectManager.h"
#include "game/CharaTask.h"
#include "game/MapManager.h"

// Methods of Bullet_unk (the effects of SkillEffectManager). A file of its own: its float pool
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
extern "C" float PSVECSquareMag(const atn::Vector3* v);
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect

Bullet_unk::Bullet_unk() : m00(3), m04(0), m08(0), m10(0.0f, 0.0f, 0.0f), m1C(0.0f, 0.0f, 0.0f), m28(1.0f), m60(0),
      m7C(0) {
    m13C = 0;
}

Bullet_unk::~Bullet_unk() {}

void Bullet_unk::fn_80599C6C() {
    if (!IsActive_unk()) {
        return;
    }
}

int Bullet_unk::fn_80599C90(CharaTask* chara) {
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
int Bullet_unk::fn_80599D60(CharaTask* chara, int check) {
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
float Bullet_unk::fn_80599FB4(int keep) {
    float scale = 1.0f;
    if (!keep || (15.0f <= m5C && m5C <= 150.0f)) {
        scale = m5C / 15.0f;
    }
    return m28 * scale;
}

void Bullet_unk::fn_80599FFC(const atn::Vector3* pos) {
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
void Bullet_unk::fn_8059A0B4(atn::EffFile* eff) {
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

void Bullet_unk::fn_8059A1FC(int keep) {
    if (m7C) {
        lbl_8087F3C0->mD0 = 1;
        fn_80239DAC(lbl_8087F3C0, &m2C, 1, !keep);
        lbl_8087F3C0->mD0 = 0;
        m7C = 0;
    }
}

int Bullet_unk::fn_8059A264() {
    if (m08 != 0 && m04 != 0 && m13C != 0) {
        return 1;
    }
    return 0;
}
