#include "game/GameUI.h"
#include "game/SkillEffectManager.h"
#include "atn/FixedArray.h"
#include "atn/Memory.h"
#include "game/CharaTask.h"
#include "game/Arms.h"
#include "game/MapManager.h"
#include "game/PlayerTask.h"
#include "game/EnemyTask.h"
#include "game/EnemyManager.h"
#include "game/CharaManager.h"
#include <boost/function.hpp>


extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
extern "C" void* lbl_8087F610;                     // inferred: arena mode (see game/CharaTask.cpp)
struct Field_unk;
extern "C" Field_unk* lbl_8087F430;
extern "C" void fn_80370320(Field_unk* field, int id, int value, int arg); // inferred: sets an event flag
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048; // inferred: effect manager of the characters
extern "C" int fn_800F8548(Effect_unk* mgr);
extern "C" void fn_800FB4B0(Effect_unk* mgr, CharaTask* chara, HitInfo_unk* data, int id, const atn::Vector3* pos,
                            int a, int time, int b, int c);
extern "C" void fn_800FBA30(Effect_unk* mgr, CharaTask* chara, CharaTask* user, HitInfo_unk* data, int id,
                            const atn::Vector3* pos, const atn::Vector3* rot);
extern "C" void fn_800FB1BC(Effect_unk* mgr, CharaTask* chara, HitInfo_unk* data, const atn::Vector3* pos,
                            const atn::Vector3* rot, int a, int b);
struct Color4f_unk {
    Color4f_unk() {}
    Color4f_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r, g, b, a;
};
struct Motion_unk;
extern "C" void fn_801070C8(Effect_unk* effect, int id, int a, int b, Motion_unk* motion, const atn::Vector3* pos,
                            const atn::Vector3* rot, const Color4f_unk* color, float scale);
extern "C" void fn_80107208(Effect_unk* effect, int id, const atn::Vector3* pos, float scale);

extern "C" HitInfo_unk* fn_80219E6C(int id); // inferred: bullet/skill data by id
extern "C" int fn_8021A8D0(HitInfo_unk* hit);
extern "C" int fn_8021A918(HitInfo_unk* hit);
extern "C" int fn_8021A960(HitInfo_unk* hit);
extern "C" int fn_8021A984(HitInfo_unk* hit);
extern "C" int fn_8021A9CC(HitInfo_unk* hit);
extern "C" float PSVECSquareMag(const atn::Vector3* v); // inferred: squared length

// inferred: methods of SkillEffect in the auto split before this file
// inferred: starts it
extern "C" int fn_8059A298(atn::Vector3* pos, atn::Vector3* rot, int arg); // inferred: reflection test
struct EffectMgr_unk;
extern "C" EffectMgr_unk* lbl_8087F3C0; // inferred: effect manager of the efp files
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const MapColor_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_80232B7C(void* owner, int id); // inferred: names the next effect of `owner`
extern "C" void fn_8023A680(EffectMgr_unk* mgr, atn::EffFile* res, int arg2, int arg3, atn::Model* model,
                            const atn::Vector3* pos, int arg6, int arg7, int arg8, int arg9, int arg10, float scale);
extern "C" unsigned long strlen(const char* s);
// inferred: weapon of a character (CharaTask keeps it as chr::CharaPart_unk)
inline Arms* GetArms_unk(chr::CharaPart_unk* part) {
    return (Arms*)part;
}
// inferred: weapons 406-412 and 883-886 (of each thousand) carry their effects higher (name invented)
inline bool IsHighArms_unk(CharaTask* chara) {
    if (chara->m648 == 0 || GetArms_unk(chara->m648)->m274 == 0 || GetArms_unk(chara->m648)->m274->m78 != 0) {
        return false;
    }
    int n = GetArms_unk(chara->m648)->m274->m80 % 1000;
    if ((406 <= n && n <= 412) || (883 <= n && n <= 886)) {
        return true;
    }
    return false;
}

// inferred: an enemy outside the arena (names invented)
inline bool IsEnemyField_unk(CharaTask* chara) {
    bool result = false;
    if (chara->mType == 2 && lbl_8087F610 == 0) {
        result = true;
    }
    return result;
}
// inferred: plays `efp` on `model` at the origin (as PlayEfp_unk of chr/ControlStickByQuark.cpp)
inline void PlayEfp_unk(atn::EfpFile* efp, atn::Model* model, float scale) {
    atn::Vector3 pos;
    atn::Vector3 rot;
    MapColor_unk color;
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
    fn_8023A8B4(lbl_8087F3C0, efp, model, 0, pos, rot, color, -1, -1, 1, scale);
}

// inferred: pair of pointers (Pair_unk is not available; names invented)
template <class A, class B>
struct Pair_unk {
    Pair_unk() : first(0), second(0) {}
    Pair_unk(A a, B b) : first(a), second(b) {}
    A first;
    B second;
};
template <class A, class B>
inline Pair_unk<A, B> MakePair_unk(A a, B b) {
    return Pair_unk<A, B>(a, b);
}

// inferred: kind of the skill read as int (cmpwi after lbz)
inline int GetKind_unk(HitInfo_unk* hit) {
    return hit->m02;
}

// inferred: second bullet of a sub bullet (HitInfo_unk + 0xC8, past the known part of the struct)
inline int GetC8_unk(HitInfo_unk* hit) {
    return *(int*)((char*)hit + 0xC8);
}

static const char sEfp0[] = "effect/ef_sc001a.efp";
static const char sEfp1[] = "effect/ef_sc021a.efp";
static const char sEfp2[] = "effect/ef_sc006a.efp";
static const char sEfp3[] = "effect/ef_sc028a.efp";
static const char sEfp4[] = "effect/ef_sc020a.efp";
static const char sEfp5[] = "effect/ef_sc034.efp";
static const char sEfp6[] = "effect/ef_sc016a.efp";
static const char sEff7[] = "ef_mw003f_01.eff";
static const char sEfp8[] = "/effect/ef_ui002a.efp";
static const char sEfp9[] = "/effect/ef_sc033d.efp";
static const char sEfp10[] = "/effect/ef_ms003e.efp";
static const char sEfp11[] = "/effect/ef_sp013c.efp";
static const char sEfp12[] = "/effect/ef_mw003e.efp";
static const char* const sEfpName0 = sEfp0;
static const char* const sEfpName1 = sEfp1;
static const char* const sEfpName2 = sEfp2;
static const char* const sEfpName3 = sEfp3;
static const char* const sEfpName4 = sEfp4;
static const char* const sEfpName5 = sEfp5;
static const char* const sEfpName6 = sEfp6;
static const char* const sEffName7 = sEff7;
static const char* const sEfpName8 = sEfp8;
static const char* const sEfpName9 = sEfp9;
static const char* const sEfpName10 = sEfp10;
static const char* const sEfpName11 = sEfp11;
static const char* const sEfpName12 = sEfp12;
extern "C" SkillEffect* fn_8059B66C(SkillEffectManager* self, HitInfo_unk* skill, CharaTask* user, CharaTask* target, const atn::Vector3* pos, const atn::Vector3* rot, boost::function<void(SkillEffect*)> callback, float rate);

// inferred: empty
extern "C" void fn_8059AB94() {}

SkillEffectManager* SkillEffectManager::fn_8059AB98(atn::Task* parent) {
    if (parent != 0 && lbl_8087F9E8 == 0) {
        lbl_8087F9E8 = new ((MemoryTag)1, "", "", 0) SkillEffectManager(parent);
    }
    return lbl_8087F9E8;
}

SkillEffectManager::SkillEffectManager(atn::Task* parent) : atn::Task(parent), mLoaded(0), m2924(0), m2928(0) {}

SkillEffectManager::~SkillEffectManager() {
    lbl_8087F9E8 = 0;
}

int SkillEffectManager::vf0C() {
    int ret = 1;
    if (fn_8059B1B4()) {
        ret = 0;
    }
    if (!fn_800D3FA4()) {
        ret = 0;
    }
    if (ret) {
        return 1;
    }
    return 0;
}

void SkillEffectManager::fn_8059AFD8() {
    if (mLoaded == 0) {
        mLoaded = 1;
        m284C.LoadRequest(sEfpName0);
        m2858.LoadRequest(sEfpName1);
        m2864.LoadRequest(sEfpName2);
        m2870.LoadRequest(sEfpName3);
        m287C.LoadRequest(sEfpName4);
        m2888.LoadRequest(sEfpName5);
        m2894.LoadRequest(sEfpName6);
        m28A0.LoadRequest(sEffName7);
        m28AC.LoadRequest(sEfpName8);
        m28C4.LoadRequest("data/efp/ef_we017_01.eff");
        m28D0.LoadRequest("data/efp/ef_we029_01.eff");
        m28DC.LoadRequest("data/efp/ef_we027_01.eff");
        m28E8.LoadRequest("data/efp/ef_we028_01.eff");
        m28F4.LoadRequest(sEfpName9);
        m2900.LoadRequest(sEfpName10);
        m290C.LoadRequest(sEfpName11);
        m2918.LoadRequest(sEfpName12);
    }
}

void SkillEffectManager::fn_8059B0F0() {
    if (mLoaded != 0) {
        mLoaded = 0;
        m284C.Destroy();
        m2858.Destroy();
        m2864.Destroy();
        m2870.Destroy();
        m287C.Destroy();
        m2888.Destroy();
        m2894.Destroy();
        m28A0.Destroy();
        m28AC.Destroy();
        m28B8.Destroy();
        m28C4.Destroy();
        m28D0.Destroy();
        m28DC.Destroy();
        m28E8.Destroy();
        m28F4.Destroy();
        m2900.Destroy();
        m290C.Destroy();
    }
}

int SkillEffectManager::fn_8059B1B4() {
    if (mLoaded == 0) {
        return 0;
    }
    int ret = 0;
    if (m284C.IsLoading() || m2858.IsLoading() || m2864.IsLoading() || m2870.IsLoading() ||
        m287C.IsLoading() || m2888.IsLoading() || m2894.IsLoading() || m28C4.IsLoading() ||
        m28D0.IsLoading() || m28DC.IsLoading() || m28E8.IsLoading() || m28F4.IsLoading() ||
        m2900.IsLoading() || m290C.IsLoading() || m2918.IsLoading()) {
        ret = 1;
    }
    if (m28A0.IsLoading() || m28AC.IsLoading() || m28B8.IsLoading()) {
        ret = 1;
    }
    return ret;
}

// NONMATCHING: 98.69% - register allocation only: the loop counters/pointers of the first and the
// unsigned loops get other registers (tried shared counters, declaration order).
void SkillEffectManager::vf10() {
    SkillEffect* bullets = mBullets;
    for (int i = 0; i < 32; i++) {
        bullets[i].fn_80599454(m2928);
    }
    PlayerTask* player = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0;
    if (player != 0 && player->mStatus.m04 < 0.3f * player->mStatus.mParamB.m84) {
        for (unsigned int i = 0; i < 32; i++) {
            SkillEffect* bullet = lbl_8087F9E8->GetBullet_unk(i);
            if (bullet != 0 && bullet->IsActive_unk()) {
                int type = bullet->m08->mType;
                if (type == 3 || type == 0) {
                    if (fn_8021A984(!bullet->fn_8059A264() ? bullet->m04 : bullet->GetOwnHit_unk())) {
                        bullet->fn_8059A0B4(&m28A0);
                    }
                }
            }
        }
    } else if (lbl_8087F610 != 0) {
        for (int i = 0; i < 32; i++, bullets++) {
            if (bullets->IsActive_unk() && bullets->m60 > 90) {
                bullets->fn_8059A1FC(0);
            }
        }
    } else {
        for (unsigned int i = 0; i < 32; i++) {
            SkillEffect* bullet = lbl_8087F9E8->GetBullet_unk(i);
            if (bullet != 0 && bullet->IsActive_unk()) {
                bullet->fn_8059A1FC(0);
            }
        }
    }
    if (lbl_8087F430 != 0 && player != 0) {
        int count = 0;
        for (int i = 0; i < 32; i++) {
            if (mBullets[i].m08 != 0 && mBullets[i].m08->fn_8016F3D0(player, 1)) {
                HitInfo_unk* hit = mBullets[i].m04;
                if (hit != 0) {
                    if (mBullets[i].fn_8059A264()) {
                        hit = mBullets[i].GetOwnHit_unk();
                    }
                    if (fn_8021A960(hit)) {
                        count++;
                    }
                }
            }
        }
        fn_80370320(lbl_8087F430, 199, count, 0);
    }
}

void SkillEffectManager::vf14() {
    SkillEffect* bullet = mBullets;
    for (int i = 0; i < 32; i++, bullet++) {
        if (bullet->IsActive_unk()) {
            bullet->fn_80599C6C();
        }
    }
}

// NONMATCHING: 99.01% - register allocation of the parameters (user gets r25 instead of r31) and
// the order of two increments in the unrolled search; tried member function, if/else layout.
// inferred: casts `skill`; returns the effect (null if none is free or the skill is of kind 7)
extern "C" SkillEffect* fn_8059B66C(SkillEffectManager* self, HitInfo_unk* skill, CharaTask* user,
                                   CharaTask* target, const atn::Vector3* pos, const atn::Vector3* rot,
                                   boost::function<void(SkillEffect*)> callback, float rate) {
    if (GetKind_unk(skill) == 7) {
        self->fn_8059B8A8(skill, user, pos, rot);
    } else {
        SkillEffect* bullet = self->FindFree_unk();
        if (bullet != 0) {
            bullet->fn_805989A4(skill, user, target, pos, rot, callback, rate);
            if (lbl_8087F610 != 0 && fn_8021A984(skill)) {
                bullet->fn_8059A0B4(&self->m28A0);
            }
        }
        return bullet;
    }
    return 0;
}

// NONMATCHING: 99.46% - scheduling of the load of skill->m58 inside the distance computation
// (float registers); tried 8 orders of the radius/distance expressions.
void SkillEffectManager::fn_8059B8A8(HitInfo_unk* skill, CharaTask* user, const atn::Vector3* pos,
                                     const atn::Vector3* rot) {
    atn::FixedArray_unk<Pair_unk<HitInfo_unk*, CharaTask*>, 32> hits;
    atn::FixedArray_unk<Pair_unk<HitInfo_unk*, CharaTask*>, 32> heals;
    atn::FixedArray_unk<Pair_unk<SkillEffect*, HitInfo_unk*>, 32> subs;
    for (unsigned int i = 0; i < 32; i++) {
        SkillEffect* bullet = lbl_8087F9E8->GetBullet_unk(i);
        HitInfo_unk* hit = 0;
        if (bullet != 0) {
            hit = bullet->GetHit_unk();
        }
        if (bullet != 0 && bullet->IsActive_unk() && (hit->mAC & 0x40) != 0x40) {
            atn::Vector3 d = bullet->m10 - *pos;
            float r = skill->m58 * 0.3f + bullet->m5C * bullet->m28;
            if (PSVECSquareMag(&d) < r * r) {

                int hitOwner = 1;
                if (bullet->m08->fn_8016F3D0(user, 1)) {
                    hitOwner = 0;
                    HitInfo_unk* sub = hit->m70 > 0 ? fn_80219E6C(hit->m70) : 0;
                    if (sub != 0) {
                        if ((skill->mAC & 0x20) != 0x20 && GetC8_unk(sub) > 0) {
                            sub = fn_80219E6C(GetC8_unk(sub));
                        }
                        if (fn_8021A8D0(sub) || fn_8021A918(sub)) {
                            heals.push_back(MakePair_unk(sub, lbl_8087F610 == 0 ? bullet->m08 : user));
                        } else {
                            hits.push_back(MakePair_unk(sub, lbl_8087F610 == 0 ? bullet->m08 : user));
                        }
                    }
                    if (user->mFlag2) {
                        hitOwner = 1;
                    }
                }
                if (hitOwner) {
                    CharaTask* owner;
                    PlayerTask* p;
                    EnemyTask* e;
                    HitInfo_unk* second = fn_80219E6C(hit->m6C);
                    owner = bullet->m08;
                    if (lbl_8087F8A0 != 0) {
                        for (p = lbl_8087F8A0->mHead; p != 0; p = p->mNext) {
                            p->fn_80178864(second, owner);
                        }
                    }
                    if (EnemyManager::sInstance != 0) {
                        for (e = EnemyManager::sInstance->mHead; e != 0; e = e->mNext) {
                            e->fn_80178864(second, owner);
                        }
                    }
                    HitInfo_unk* sub = hit->m70 > 0 ? fn_80219E6C(hit->m70) : 0;
                    if (sub != 0) {
                        subs.push_back(MakePair_unk(bullet, sub));
                    }
                }
                bullet->fn_805991E0();
            }
        }
    }
    for (int i = 0; i < heals.size(); i++) {
        if (!user->mFlag2 || fn_8021A9CC(heals[i].first)) {
            Effect_unk* mgr = lbl_8087F048;
            fn_800FB4B0(mgr, heals[i].second, heals[i].first, fn_800F8548(mgr), pos, 1, 30, -1, 0);
            if (heals[i].first != 0 && user != 0) {
                fn_801070C8(lbl_8087F048, heals[i].first->mB0, 0, 0, 0, &user->mPos, &user->mRot,
                            &Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f), 1.0f);
                fn_80107208(lbl_8087F048, heals[i].first->mB0, &user->mPos, 1.0f);
            }
        }
        if (lbl_8087F490 != 0) {
            GameUI::Get_unk()->fn_803E6890(heals[i].second, heals[i].first);
        }
    }
    for (int i = 0; i < hits.size(); i++) {
        if (!user->mFlag2) {
            Effect_unk* mgr = lbl_8087F048;
            fn_800FBA30(mgr, hits[i].second, user, hits[i].first, fn_800F8548(mgr), pos, &user->mRot);
        }
        if (lbl_8087F490 != 0) {
            GameUI::Get_unk()->fn_803E6890(hits[i].second, hits[i].first);
        }
    }
    for (int i = 0; i < subs.size(); i++) {
        if (!user->mFlag2) {
            subs[i].first->fn_80599FB4(1);
            SkillEffect* bullet = subs[i].first;
            fn_800FB1BC(lbl_8087F048, bullet->m08, subs[i].second, &bullet->m10, &bullet->m1C, 0, 0);
        }
        if (lbl_8087F490 != 0) {
            GameUI::Get_unk()->fn_803E6890(subs[i].first->m08, subs[i].second);
        }
    }
}

void SkillEffectManager::fn_8059C084(CharaTask* chara) {
    if ((chara->mFlags & 4) == 4 || chara->mStatus.IsState_unk(0x20)) {
        chara->fn_80178A6C();
        return;
    }
    int ok = 1;
    if (chara->m55C == 6 && chara->m560 >= 27 && chara->m560 <= 28) {
        ok = 0;
    }
    if (ok) {
        SkillEffect* bullet = mBullets;
        for (int i = 0; i < 32; i++, bullet++) {
            if (!bullet->IsActive_unk()) {
                continue;
            }
            if (bullet->fn_80599D60(chara, 1)) {
                if (fn_8021A960(bullet->m04)) {
                    if (chara->mFlag22 == 1 && !chara->mFlagB15) {
                        continue;
                    }
                    if (chara->mFlag21 == 1) {
                        continue;
                    }
                    if (chara->mStatus.IsState_unk(0x20)) {
                        continue;
                    }
                    if (chara->mFlagB1) {
                        continue;
                    }
                    if (chara->mType == 0 && chara->m55C == 6 && chara->m560 == 14) {
                        continue;
                    }
                }
                if (chara->mFlag22 == 1 && (chara->fn_80179730(bullet->m04, 0) & 0x100) == 0x100) {
                    continue;
                }
                chara->fn_801784BC(bullet, 0);
            } else if (fn_8021A960(bullet->m04) && bullet->fn_80599D60(chara, 0)) {
                HitInfo_unk* hit = bullet->m04;
                if (hit->m74.m00 > 0.0f || hit->m74.m04 > 0.0f || hit->m74.m10 > 0.0f || hit->m74.m0C > 0.0f) {
                    chara->fn_801784BC(bullet, 1);
                }
            }
        }
    }
}

void SkillEffectManager::fn_8059C2A8(CharaTask* target, HitInfo_unk* hit) {
    SkillEffect* bullet = mBullets;
    for (int i = 0; i < 32; i++, bullet++) {
        if (bullet->m0C == target && (hit == 0 || bullet->m04 == hit)) {
            bullet->fn_805991E0();
        }
    }
}

void SkillEffectManager::fn_8059C32C(int clear) {
    SkillEffect* bullet = mBullets;
    for (int i = 0; i < 32; i++, bullet++) {
        if (bullet->IsActive_unk()) {
            if (clear) {
                bullet->SetCallback_unk(boost::function<void(SkillEffect*)>());
            }
            bullet->fn_805991E0();
        }
    }
}

SkillEffect* SkillEffectManager::fn_8059C5DC(CharaTask* owner, int id, int flags) {
    SkillEffect* bullet = mBullets;
    for (int i = 0; i < 32; bullet++, i++) {
        if (bullet->IsActive_unk() && (owner == 0 || bullet->m08 == owner) && (id <= 0 || id == bullet->m04->m04) &&
            (flags <= 0 || (flags & bullet->m04->mAC) == flags)) {
            return bullet;
        }
    }
    return 0;
}

int SkillEffectManager::fn_8059C6DC(HitInfo_unk* hit, CharaTask* owner) {
    if (hit == 0) {
        return 0;
    }
    return fn_8059C5DC(owner, hit->m04, hit->mAC) != 0;
}

void SkillEffectManager::fn_8059C800(CharaTask* chara) {
    fn_80232B7C(chara, 10);
    atn::EfpFile* efp = IsEnemyField_unk(chara) ? &m2858 : &m284C;
    float scale = chara->m1258;
    atn::Vector3 pos;
    atn::Vector3 rot;
    MapColor_unk color;
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
    fn_8023A8B4(lbl_8087F3C0, efp, chara->GetModel_unk(), 0, pos, rot, color, -1, -1, 1, scale);
}

void SkillEffectManager::fn_8059C8D8(CharaTask* chara) {
    fn_80239DAC(lbl_8087F3C0, chara, 10, 1);
}

void SkillEffectManager::fn_8059C8E8(CharaTask* chara) {
    fn_80232B7C(chara, 11);
    PlayEfp_unk(&m2864, chara->GetModel_unk(), 1.0f);
}

void SkillEffectManager::fn_8059C98C(CharaTask* chara) {
    fn_80239DAC(lbl_8087F3C0, chara, 11, 1);
}

void SkillEffectManager::fn_8059C99C(CharaTask* chara) {
    fn_80232B7C(chara, 12);
    atn::EfpFile* efp = IsEnemyField_unk(chara) ? &m287C : &m2870;
    float scale = chara->m1258;
    atn::Vector3 pos;
    atn::Vector3 rot;
    MapColor_unk color;
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
    fn_8023A8B4(lbl_8087F3C0, efp, chara->GetModel_unk(), 0, pos, rot, color, -1, -1, 1, scale);
}

void SkillEffectManager::fn_8059CA74(CharaTask* chara) {
    fn_80239DAC(lbl_8087F3C0, chara, 12, 1);
}

void SkillEffectManager::fn_8059CA84(CharaTask* chara) {
    fn_80232B7C(chara, 13);
    atn::EfpFile* efp = &m2888;
    float scale = chara->m1258;
    atn::Vector3 pos;
    atn::Vector3 rot;
    MapColor_unk color;
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
    fn_8023A8B4(lbl_8087F3C0, efp, chara->GetModel_unk(), 0, pos, rot, color, -1, -1, 1, scale);
}

void SkillEffectManager::fn_8059CB2C(CharaTask* chara) {
    fn_80239DAC(lbl_8087F3C0, chara, 13, 1);
}

void SkillEffectManager::fn_8059CB3C(CharaTask* chara) {
    fn_80232B7C(chara, 14);
    atn::EfpFile* efp = &m2894;
    float scale = chara->m1258;
    atn::Vector3 pos;
    atn::Vector3 rot;
    MapColor_unk color;
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
    fn_8023A8B4(lbl_8087F3C0, efp, chara->GetModel_unk(), 0, pos, rot, color, -1, -1, 1, scale);
}

void SkillEffectManager::fn_8059CBE4(CharaTask* chara) {
    fn_80239DAC(lbl_8087F3C0, chara, 14, 1);
}

void SkillEffectManager::fn_8059CBF4(CharaTask* chara) {
    fn_80232B7C(chara, 15);
    atn::EfpFile* efp = &m28AC;
    float scale = chara->m1258;
    atn::Vector3 pos;
    atn::Vector3 rot;
    MapColor_unk color;
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
    fn_8023A8B4(lbl_8087F3C0, efp, chara->GetModel_unk(), 0, pos, rot, color, -1, -1, 1, scale);
}

void SkillEffectManager::fn_8059CC9C(CharaTask* chara) {
    fn_80239DAC(lbl_8087F3C0, chara, 15, 1);
}

// NONMATCHING: 96.2% - the original loads m648 before the other arguments of the first fn_8023A680
// and swaps two registers in the copy of the second position
void SkillEffectManager::fn_8059CCAC(CharaTask* chara, int id, atn::EffFile* eff, const atn::Vector3* offset) {
    if (chara->m648 == 0) {
        return;
    }
    ArmsData_unk* data = GetArms_unk(chara->m648)->m274;
    if (strlen(data->m8C) == 0) {
        return;
    }
    float scale = data->mB8 == 1 ? 0.5f : 1.0f;
    atn::Vector3 pos = *offset * scale;
    if (IsHighArms_unk(chara)) {
        pos.y += -9.0f * scale;
    }
    fn_80232B7C(chara, id);
    fn_8023A680(lbl_8087F3C0, eff, -1, 3, (atn::Model*)chara->m648->m010, &pos, 0, 0, 0, -1, 1, scale);
    if (chara->m64C != 0) {
        float scale2 = GetArms_unk(chara->m64C)->m274->mB8 == 1 ? 0.5f : 1.0f;
        pos = *offset * scale2;
        if (IsHighArms_unk(chara)) {
            pos.y += -9.0f * scale2;
        }
        fn_8023A680(lbl_8087F3C0, eff, -1, 3, (atn::Model*)chara->m64C->m010, &pos, 0, 0, 0, -1, 1, scale2);
    }
}

void SkillEffectManager::fn_8059CF70(CharaTask* chara) {
    fn_8059CCAC(chara, 16, &m28C4, &atn::Vector3(0.0f, 7.0f, 0.0f));
}

void SkillEffectManager::fn_8059CFB0(CharaTask* chara, int keep) {
    fn_80239DAC(lbl_8087F3C0, chara, 16, !keep);
}

void SkillEffectManager::fn_8059CFC4(CharaTask* chara) {
    fn_8059CCAC(chara, 17, &m28D0, &atn::Vector3(0.0f, 4.0f, 0.0f));
}

void SkillEffectManager::fn_8059D004(CharaTask* chara, int keep) {
    fn_80239DAC(lbl_8087F3C0, chara, 17, !keep);
}

void SkillEffectManager::fn_8059D018(CharaTask* chara) {
    fn_8059CCAC(chara, 18, &m28DC, &atn::Vector3(0.0f, 4.0f, 0.0f));
}

void SkillEffectManager::fn_8059D058(CharaTask* chara, int keep) {
    fn_80239DAC(lbl_8087F3C0, chara, 18, !keep);
}

void SkillEffectManager::fn_8059D06C(CharaTask* chara) {
    fn_8059CCAC(chara, 19, &m28E8, &atn::Vector3(0.0f, 4.0f, 0.0f));
}

void SkillEffectManager::fn_8059D0AC(CharaTask* chara, int keep) {
    fn_80239DAC(lbl_8087F3C0, chara, 19, !keep);
}

void SkillEffectManager::fn_8059D0C0(CharaTask* chara) {
    fn_80232B7C(chara, 20);
    atn::EfpFile* efp = &m28F4;
    float scale = chara->m1258;
    atn::Vector3 pos;
    atn::Vector3 rot;
    MapColor_unk color;
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
    fn_8023A8B4(lbl_8087F3C0, efp, chara->GetModel_unk(), 0, pos, rot, color, -1, -1, 1, scale);
}

void SkillEffectManager::fn_8059D168(CharaTask* chara) {
    fn_80239DAC(lbl_8087F3C0, chara, 20, 1);
}

void SkillEffectManager::fn_8059D178() {
    for (unsigned int i = 0; i < 32; i++) {
        SkillEffect* bullet = lbl_8087F9E8->GetBullet_unk(i);
        if (bullet != 0 && bullet->IsActive_unk() && (bullet->m08->mType == 3 || bullet->m08->mType == 0) &&
            fn_8021A984(!bullet->fn_8059A264() ? bullet->m04 : bullet->GetOwnHit_unk())) {
            bullet->fn_8059A0B4(&m28A0);
        }
    }
}

void SkillEffectManager::fn_8059D268(int arg) {
    for (unsigned int i = 0; i < 32; i++) {
        SkillEffect* bullet = lbl_8087F9E8->GetBullet_unk(i);
        if (bullet != 0 && bullet->IsActive_unk()) {
            bullet->fn_8059A1FC(arg);
        }
    }
}

int SkillEffectManager::fn_8059D30C(CharaTask* chara) {
    int count = 0;
    for (int i = 0; i < 32; i++) {
        if (mBullets[i].m08 != 0 && mBullets[i].m08->fn_8016F3D0(chara, 1)) {
            HitInfo_unk* hit = mBullets[i].m04;
            if (hit != 0) {
                if (mBullets[i].fn_8059A264()) {
                    hit = mBullets[i].GetOwnHit_unk();
                }
                if (fn_8021A984(hit)) {
                    count++;
                }
            }
        }
    }
    return count;
}

// NONMATCHING: 99.94% - stack slots of the sphere and light temporaries (0x40/0x60 against 0x60/0x50)
void SkillEffectManager::fn_8059D3B0(CharaTask* chara) {
    if (chara == 0) {
        return;
    }
    for (int i = 0; i < 32; i++) {
        SkillEffect* bullet = &mBullets[i];
        if (!bullet->IsActive_unk() || bullet->m08 == 0 || bullet->m08->fn_8016F3D0(chara, 1) ||
            bullet->fn_8059A264()) {
            continue;
        }
        HitInfo_unk* hit = bullet->m04;
        if (fn_8021A960(hit) && (hit->m90.mFlags & 1U) != 1 && (hit->m90.mFlags & 0x400000U) != 0x400000 &&
            (hit->m90.mFlags & 0x40U) != 0x40) {
            atn::Vector3 pos = chara->mPos;
            atn::Vector3 rot = chara->mRot;
            if (fn_8059A298(&pos, &rot, 0)) {
                bullet->m10 = pos;
                bullet->m1C = rot;
                if (bullet->GetOwnHit_unk()) {
                    bullet->m5C = bullet->GetOwnHit_unk()->m58;
                }
                bullet->m13C = 1;
                bullet->m60 = 0;
                bullet->m08 = chara;
                float scale = bullet->fn_80599FB4(0);
                MapColor_unk light = MapManager::sInstance->fn_80491DF8(atn::Sphere(bullet->m10, 10.0f));
                fn_80239DAC(lbl_8087F3C0, bullet, bullet->m04->m04, 1);
                fn_80232B7C(0, 0);
                fn_8023A8B4(lbl_8087F3C0, &m2900, 0, 0, bullet->m10, bullet->m1C, MapColor_unk(1.0f, 1.0f, 1.0f, 1.0f),
                            -1, -1, 1, scale);
                fn_80232B7C(bullet, bullet->m04->m04);
                if (fn_8021A960(hit)) {
                    fn_8023A8B4(lbl_8087F3C0, &m2918, 0, 0, bullet->m10, bullet->m1C, light, -1, -1, 1, scale);
                } else {
                    fn_8023A8B4(lbl_8087F3C0, &m290C, 0, 0, bullet->m10, bullet->m1C, light, -1, -1, 1, scale);
                }
            }
        } else if (fn_8021A960(hit) || fn_8021A984(hit)) {
            bullet->m13C = 1;
            bullet->m60 = 0;
            bullet->m08 = chara;
            float scale = bullet->fn_80599FB4(0);
            MapColor_unk light = MapManager::sInstance->fn_80491DF8(atn::Sphere(bullet->m10, 10.0f));
            fn_80239DAC(lbl_8087F3C0, bullet, bullet->m04->m04, 1);
            fn_80232B7C(0, 0);
            fn_8023A8B4(lbl_8087F3C0, &m2900, 0, 0, bullet->m10, bullet->m1C, MapColor_unk(1.0f, 1.0f, 1.0f, 1.0f), -1,
                        -1, 1, scale);
            fn_80232B7C(bullet, bullet->m04->m04);
            if (fn_8021A960(hit)) {
                fn_8023A8B4(lbl_8087F3C0, &m2918, 0, 0, bullet->m10, bullet->m1C, light, -1, -1, 1, scale);
            } else {
                fn_8023A8B4(lbl_8087F3C0, &m290C, 0, 0, bullet->m10, bullet->m1C, light, -1, -1, 1, scale);
            }
        }
    }
}
