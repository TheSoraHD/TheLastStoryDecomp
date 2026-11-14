#include "game/SkillEffectManager.h"
#include "game/CharaTask.h"
#include "game/MapManager.h"
#include "game/GameTask.h"
#include "game/MapTask.h"
#include "gmk/Lemon.h"

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
extern "C" const char* fn_800EFE3C(int id); // inferred: start sound of a skill effect
extern "C" const char* fn_800EFEA0(int id); // inferred: loop sound of a skill effect
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" void fn_800CB6E4(atn::SoundRef* sound, const atn::Vector3* pos); // inferred: moves the sound
extern "C" void* lbl_8087EE98; // inferred: collision world
extern "C" int fn_8004ECC0(void* world, atn::Vector3* hit, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg); // inferred: ray test
extern "C" int fn_8059A298(atn::Vector3* pos, atn::Vector3* rot, int arg); // inferred: places a ground effect
extern "C" int fn_8004ED34(void* colli, GmkCollResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7); // inferred: ray test
extern "C" float PSVECSquareMag(const atn::Vector3* v);
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" double cos(double x);
extern "C" GameTask* lbl_8087F430; // inferred: field task
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

// NONMATCHING: 94.6% - stack slots of the sphere/light temporaries, argument scheduling of the effect
// calls, the bool test of fn_8059A298 and the matrices of RotateZYX_unk (as fn_805989A4)
void SkillEffect::fn_80599454(int pause) {
    if (m00 == 0) {
        m00 = 1;
        atn::EfpFile* efp = fn_800EFC64(m04->mB0);
        float scale = fn_80599FB4(1);
        switch ((char)m04->m02) {
        case 0:
        case 1:
        case 2:
        case 6:
        case 8:
        case 9:
            if (efp != 0) {
                MapColor_unk light = MapManager::instance_->fn_80491D50(atn::Sphere(m0C->mPos, 10.0f));
                fn_80232B7C(this, m04->m04);
                fn_8023A8B4(lbl_8087F3C0, efp, m0C->GetModel_unk(), 0, lbl_807C7030, lbl_807C7030, light, -1, -1, 1,
                            scale);
            }
            break;
        case 4:
            if (efp != 0) {
                MapColor_unk light = MapManager::instance_->fn_80491D50(atn::Sphere(m0C->mPos, 10.0f));
                fn_80232B7C(this, m04->m04);
                fn_8023A8B4(lbl_8087F3C0, efp, m0C->GetModel_unk(), 0, lbl_807C7030, lbl_807C7030, light, -1, -1, 1,
                            scale);
            }
            break;
        case 3:
        case 5:
            if (!fn_8059A298(&m10, &m1C, (char)m04->m02 == 5)) {
                fn_805991E0();
                return;
            }
            if (efp != 0) {
                MapColor_unk light = MapManager::instance_->fn_80491DF8(atn::Sphere(m10, 10.0f));
                fn_80232B7C(this, m04->m04);
                fn_8023A8B4(lbl_8087F3C0, efp, 0, 0, m10, m1C, light, -1, -1, 1, scale);
            }
            break;
        case 7:
            break;
        }
        const char* start = fn_800EFE3C(m04->mB0);
        const char* loop = fn_800EFEA0(m04->mB0);
        fn_800C344C(start, &m10, 0, -1, 1.0f);
        m64 = fn_800C344C(loop, &m10, 0, -1, 1.0f);
    } else if (m00 == 1) {
        if (pause == 0) {
            int life = m04->m5C;
            if (m13C) {
                life = m80.m5C;
            }
            if (life >= 0) {
                if (m60 >= life) {
                    m00 = 2;
                } else {
                    m60++;
                }
            }
            if (m04->m04 == 6004 || m04->m04 == 6047) {
                if (m08 != 0 && (m08->mFlags & 4) == 4) {
                    m00 = 2;
                }
                if (m0C != 0 && (m0C->mFlags & 4) == 4) {
                    m00 = 2;
                }
            } else {
                if (m08 != 0 && !m08->CanAct_unk()) {
                    m00 = 2;
                }
                if (m0C != 0 && !m0C->CanAct_unk()) {
                    m00 = 2;
                }
            }
            char kind = m04->m02;
            if (kind == 5 || kind == 3) {
                atn::Vector3 from = m10 + atn::Vector3(0.0f, 10.0f, 0.0f);
                atn::Vector3 to = m10 + atn::Vector3(0.0f, -12.0f, 0.0f);
                if (!fn_8004ECC0(lbl_8087EE98, 0, &from, &to, 0x80000000, 0, 0)) {
                    m00 = 2;
                }
            }
            if (m0C != 0 && (char)m04->m02 == 0) {
                m10 = m0C->mPos;
                m1C = m0C->mRot;
                fn_800CB6E4(&m64, &m10);
            }
            if (m08 != 0 && m04->m02 == 4) {
                m10 = m08->mPos;
                m1C = m08->mRot;
                fn_800CB6E4(&m64, &m10);
            }
        }
    } else if (m00 == 2) {
        fn_805991E0();
        return;
    }
    m2C.SetIdentity();
    m2C.RotateZYX_unk(m1C);
    m2C.fn_801479E4(m10);
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
    return scale * m28;
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
        MapColor_unk light = MapManager::instance_->fn_80491DF8(atn::Sphere(m2C.GetTrans_unk(), 2.0f));
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

// inferred: rotation matrix from euler angles, z then y then x (weak copy 0x800109E0 of
// Event/Puppeteer_CharaTask.cpp; inlined here)
extern "C" inline atn::Matrix3x4 fn_800109E0(const atn::Vector3& rot) {
    atn::Matrix3x4 m;
    m.SetIdentity();
    if (0.0f != rot.z) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'z', rot.z);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    if (0.0f != rot.y) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'y', rot.y);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    if (0.0f != rot.x) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'x', rot.x);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    return m;
}

// inferred: dot product (name invented)
inline float Dot_unk(const atn::Vector3& a, const atn::Vector3& b) {
    return PSVECDotProduct(&a, &b);
}

// inferred: material of the polygon hit (GmkCollResult_unk::m34); 0x14 is water (name invented)
inline bool IsWater_unk(const GmkCollResult_unk& res) {
    return res.m34 != 0 && *(unsigned int*)res.m34 == 0x14;
}

// inferred: puts a ground effect on the floor under `pos` and turns `rot` to the slope; with `steep`
// the effect is refused on slopes over 30 degrees. Returns 0 when there is no floor (names invented)
// NONMATCHING: 99.79% - the original keeps the address of the rotation matrix in r26 for PSMTXMultVec
// and two pairs of saved registers of the floor search are swapped (r25/r28, r22/r23)
extern "C" int fn_8059A298(atn::Vector3* pos, atn::Vector3* rot, int steep) {
    int fail = 0;
    atn::Matrix3x4 m = fn_800109E0(*rot);
    atn::Vector3 dir;
    dir = atn::Vector3(0.0f, 0.0f, 1.0f);
    PSMTXMultVec(m.m, &dir, &dir);
    atn::Vector3 start = *pos;
    atn::Vector3 from = *pos;
    from += dir * 0.001f;
    from.y += 3.0f;
    atn::Vector3 to = *pos;
    to += dir * 0.001f;
    to.y -= 350.0f;
    atn::Vector3 c;
    atn::Vector3 d;
    unsigned int mask = 0x80080000;
    GmkCollResult_unk res;
    if (fn_8004ED34(lbl_8087EE98, &res, &from, &to, mask, 0, 0)) {
        if (steep != 0 && Dot_unk(res.m28, atn::Vector3(0.0f, 1.0f, 0.0f)) < 0.8660254f) {
            fail = 1;
        } else {
            if (steep == 0) {
                float limit = cos(0.8726646304130554);
                if (Dot_unk(res.m28, atn::Vector3(0.0f, 1.0f, 0.0f)) < limit) {
                    res.m28 = atn::Vector3(0.0f, 1.0f, 0.0f);
                    res.m10.y += 2.0f;
                }
            }
            bool water = IsWater_unk(res);
            *pos = res.m10;
            *rot = res.m28.ToRotation_unk();
            rot->x += 1.5707964f;
            float flat = cos(0.0872664600610733);
            if (Dot_unk(res.m28, atn::Vector3(0.0f, 1.0f, 0.0f)) >= flat) {
                float best = 1e16f;
                atn::Vector3 offset(0.0f, 0.0f, 15.0f);
                for (int i = 0; i < 4; i++) {
                    atn::Vector3 a = *pos + atn::Vector3(0.0f, 1.0f, 0.0f);
                    atn::Vector3 b = *pos + offset + atn::Vector3(0.0f, 1.0f, 0.0f);
                    if (fn_8004ED34(lbl_8087EE98, &res, &a, &b, mask, 0, 0)) {
                        b = res.m10;
                        b += res.m28 * 2.0f;
                    }
                    c = b;
                    d = b + atn::Vector3(0.0f, -25.0f, 0.0f);
                    if (fn_8004ED34(lbl_8087EE98, &res, &c, &d, mask, 0, 0)) {
                        float limit = cos(0.0872664600610733);
                        if (Dot_unk(res.m28, atn::Vector3(0.0f, 1.0f, 0.0f)) >= limit && res.m10.y < best) {
                            best = res.m10.y;
                            if (best < pos->y) {
                                water = IsWater_unk(res);
                            }
                        }
                    }
                    Mtx ry;
                    PSMTXRotRad(ry, 'y', 1.5707964f);
                    PSMTXMultVec(ry, &offset, &offset);
                }
                if (best < pos->y) {
                    pos->y = best;
                }
            } else {
                pos->y += 2.0f;
            }
            if (water) {
                MapTask* map = lbl_8087F430 != 0 ? lbl_8087F430->GetCurrentMap() : 0;
                if (map != 0 && map->mType == 2 &&
                    ((map->mId == 5 && map->mSubId == 3) || (map->mId == 14 && map->mSubId == 3) ||
                     (map->mId == 24 && map->mSubId == 3) || (map->mId == 35 && map->mSubId == 3))) {
                    pos->y += 2.0f;
                }
            }
        }
    } else {
        fail = 1;
    }
    return !fail;
}
