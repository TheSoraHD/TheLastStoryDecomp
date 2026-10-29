#include "game/SkillEffectManager.h"
#include "atn/FixedArray.h"
#include "atn/Memory.h"
#include "game/CharaTask.h"
#include "game/PlayerTask.h"
#include "game/EnemyTask.h"
#include "game/EnemyManager.h"
#include "game/CharaManager.h"
#include <boost/function.hpp>

// inferred: the singleton (referenced as lbl_8087F9E8 by game/CharaTask.cpp and gmk/HealPoint.cpp)
extern "C" SkillEffectManager* lbl_8087F9E8;

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
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length

// inferred: methods of Bullet_unk in the auto split before this file
extern "C" void fn_805989A4(Bullet_unk* bullet, HitInfo_unk* skill, CharaTask* user, CharaTask* target,
                            const atn::Vector3* pos, const atn::Vector3* rot,
                            boost::function<void(Bullet_unk*)> callback, float rate); // inferred: starts it
extern "C" void fn_805991E0(Bullet_unk* bullet);                     // inferred: stops it
extern "C" void fn_80599454(Bullet_unk* bullet, int arg);            // inferred: update
extern "C" void fn_80599C6C(Bullet_unk* bullet);                     // inferred: draw
extern "C" void fn_80599FB4(Bullet_unk* bullet, int arg);
extern "C" void fn_8059A0B4(Bullet_unk* bullet, ResLoader_unk* res);
extern "C" void fn_8059A1FC(Bullet_unk* bullet, int arg);

// inferred: object behind lbl_8087F490; Get_unk is only declared here: the call goes to the weak
// copy at 0x80151210 (game/CharaTask.cpp)
struct Unk8087F490_unk;
extern "C" Unk8087F490_unk* lbl_8087F490;
struct Unk8087F490_unk {
    static Unk8087F490_unk* Get_unk();
};
extern "C" void fn_803E6890(Unk8087F490_unk* obj, CharaTask* chara, HitInfo_unk* hit);

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

SkillEffectManager* SkillEffectManager::fn_8059AB98(atn::Task* parent) {
    if (parent != 0 && lbl_8087F9E8 == 0) {
        lbl_8087F9E8 = new (1, "", "", 0) SkillEffectManager(parent);
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
        m284C.fn_8023780C(sEfpName0);
        m2858.fn_8023780C(sEfpName1);
        m2864.fn_8023780C(sEfpName2);
        m2870.fn_8023780C(sEfpName3);
        m287C.fn_8023780C(sEfpName4);
        m2888.fn_8023780C(sEfpName5);
        m2894.fn_8023780C(sEfpName6);
        m28A0.fn_80237654(sEffName7);
        m28AC.fn_8023780C(sEfpName8);
        m28C4.fn_80237654("data/efp/ef_we017_01.eff");
        m28D0.fn_80237654("data/efp/ef_we029_01.eff");
        m28DC.fn_80237654("data/efp/ef_we027_01.eff");
        m28E8.fn_80237654("data/efp/ef_we028_01.eff");
        m28F4.fn_8023780C(sEfpName9);
        m2900.fn_8023780C(sEfpName10);
        m290C.fn_8023780C(sEfpName11);
        m2918.fn_8023780C(sEfpName12);
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
        m28A0.fn_8023772C();
        m28AC.Destroy();
        m28B8.Destroy();
        m28C4.fn_8023772C();
        m28D0.fn_8023772C();
        m28DC.fn_8023772C();
        m28E8.fn_8023772C();
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
    if (m284C.fn_80237874() || m2858.fn_80237874() || m2864.fn_80237874() || m2870.fn_80237874() ||
        m287C.fn_80237874() || m2888.fn_80237874() || m2894.fn_80237874() || m28C4.fn_802376D0() ||
        m28D0.fn_802376D0() || m28DC.fn_802376D0() || m28E8.fn_802376D0() || m28F4.fn_80237874() ||
        m2900.fn_80237874() || m290C.fn_80237874() || m2918.fn_80237874()) {
        ret = 1;
    }
    if (m28A0.fn_802376D0() || m28AC.fn_80237874() || m28B8.fn_80237874()) {
        ret = 1;
    }
    return ret;
}

// NONMATCHING: 98.69% - register allocation only: the loop counters/pointers of the first and the
// unsigned loops get other registers (tried shared counters, declaration order).
void SkillEffectManager::vf10() {
    Bullet_unk* bullets = mBullets;
    for (int i = 0; i < 32; i++) {
        fn_80599454(&bullets[i], m2928);
    }
    PlayerTask* player = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0;
    if (player != 0 && player->mStatus.m04 < 0.3f * player->mStatus.mParamB.m84) {
        for (unsigned int i = 0; i < 32; i++) {
            Bullet_unk* bullet = lbl_8087F9E8->GetBullet_unk(i);
            if (bullet != 0 && bullet->IsActive_unk()) {
                int type = bullet->m08->mType;
                if (type == 3 || type == 0) {
                    if (fn_8021A984(!bullet->fn_8059A264() ? bullet->m04 : bullet->GetOwnHit_unk())) {
                        fn_8059A0B4(bullet, &m28A0);
                    }
                }
            }
        }
    } else if (lbl_8087F610 != 0) {
        for (int i = 0; i < 32; i++, bullets++) {
            if (bullets->IsActive_unk() && bullets->m60 > 90) {
                fn_8059A1FC(bullets, 0);
            }
        }
    } else {
        for (unsigned int i = 0; i < 32; i++) {
            Bullet_unk* bullet = lbl_8087F9E8->GetBullet_unk(i);
            if (bullet != 0 && bullet->IsActive_unk()) {
                fn_8059A1FC(bullet, 0);
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
    Bullet_unk* bullet = mBullets;
    for (int i = 0; i < 32; i++, bullet++) {
        if (bullet->IsActive_unk()) {
            fn_80599C6C(bullet);
        }
    }
}

// NONMATCHING: 99.01% - register allocation of the parameters (user gets r25 instead of r31) and
// the order of two increments in the unrolled search; tried member function, if/else layout.
// inferred: casts `skill`; returns the effect (null if none is free or the skill is of kind 7)
extern "C" Bullet_unk* fn_8059B66C(SkillEffectManager* self, HitInfo_unk* skill, CharaTask* user,
                                   CharaTask* target, const atn::Vector3* pos, const atn::Vector3* rot,
                                   boost::function<void(Bullet_unk*)> callback, float rate) {
    if (GetKind_unk(skill) == 7) {
        self->fn_8059B8A8(skill, user, pos, rot);
    } else {
        Bullet_unk* bullet = self->FindFree_unk();
        if (bullet != 0) {
            fn_805989A4(bullet, skill, user, target, pos, rot, callback, rate);
            if (lbl_8087F610 != 0 && fn_8021A984(skill)) {
                fn_8059A0B4(bullet, &self->m28A0);
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
    atn::FixedArray_unk<Pair_unk<Bullet_unk*, HitInfo_unk*>, 32> subs;
    for (unsigned int i = 0; i < 32; i++) {
        Bullet_unk* bullet = lbl_8087F9E8->GetBullet_unk(i);
        HitInfo_unk* hit = 0;
        if (bullet != 0) {
            hit = bullet->GetHit_unk();
        }
        if (bullet != 0 && bullet->IsActive_unk() && (hit->mAC & 0x40) != 0x40) {
            atn::Vector3 d = bullet->m10 - *pos;
            float r = skill->m58 * 0.3f + bullet->m5C * bullet->m28;
            if (fn_805F9920(&d) < r * r) {

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
                fn_805991E0(bullet);
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
            fn_803E6890(Unk8087F490_unk::Get_unk(), heals[i].second, heals[i].first);
        }
    }
    for (int i = 0; i < hits.size(); i++) {
        if (!user->mFlag2) {
            Effect_unk* mgr = lbl_8087F048;
            fn_800FBA30(mgr, hits[i].second, user, hits[i].first, fn_800F8548(mgr), pos, &user->mRot);
        }
        if (lbl_8087F490 != 0) {
            fn_803E6890(Unk8087F490_unk::Get_unk(), hits[i].second, hits[i].first);
        }
    }
    for (int i = 0; i < subs.size(); i++) {
        if (!user->mFlag2) {
            fn_80599FB4(subs[i].first, 1);
            Bullet_unk* bullet = subs[i].first;
            fn_800FB1BC(lbl_8087F048, bullet->m08, subs[i].second, &bullet->m10, &bullet->m1C, 0, 0);
        }
        if (lbl_8087F490 != 0) {
            fn_803E6890(Unk8087F490_unk::Get_unk(), subs[i].first->m08, subs[i].second);
        }
    }
}

// inferred: empty
extern "C" void fn_8059AB94() {}
