#ifndef GAME_SKILLEFFECTMANAGER_H
#define GAME_SKILLEFFECTMANAGER_H

#include "atn/Task.h"
#include "atn/EfpHandle_unk.h"
#include "atn/ResLoader_unk.h"
#include "game/HitInfo_unk.h"
#include "math/Vec.h"

class CharaTask;

// inferred: effect of a skill/magic (0x140 bytes, 32 of them in SkillEffectManager); its methods
// are in the auto split before game/SkillEffectManager.cpp (0x80598874-0x8059AB98). The name
// Bullet_unk is the one used by CharaTask::fn_801784BC; gmk::HealPoint calls it SkillEffect.
struct Bullet_unk {
    Bullet_unk();  // 0x80598874
    ~Bullet_unk(); // 0x80598900
    int fn_8059A264(); // inferred: nonzero when the effect uses its own copy of the hit data (m80)

    // inferred: state 3 = finished
    bool IsActive_unk() { return m04 != 0 && m00 != 3; }
    // inferred: own copy of the hit data
    HitInfo_unk* GetOwnHit_unk() { return (HitInfo_unk*)m80; }
    // inferred: hit data of the effect
    HitInfo_unk* GetHit_unk() {
        HitInfo_unk* hit = m04;
        if (fn_8059A264()) {
            hit = GetOwnHit_unk();
        }
        return hit;
    }

    int m00;           // inferred: state
    HitInfo_unk* m04;  // inferred: hit data
    CharaTask* m08;    // inferred: owner
    CharaTask* m0C;
    atn::Vector3 m10;  // inferred: position
    atn::Vector3 m1C;  // inferred: rotation
    float m28;         // inferred: scale
    unsigned char m2C[0x5C - 0x2C];
    float m5C;         // inferred: radius
    int m60;           // inferred: frame counter
    unsigned char m64[0x80 - 0x64];
    unsigned char m80[0x140 - 0x80];
};

// inferred from __vt__18SkillEffectManager (0x80796B28): effects of skills and magic
class SkillEffectManager : public atn::Task {
public:
    SkillEffectManager(atn::Task* parent);
    virtual ~SkillEffectManager();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();

    // inferred: creates the singleton under `parent` if it does not exist yet
    static SkillEffectManager* fn_8059AB98(atn::Task* parent);
    void fn_8059AFD8(); // inferred: starts loading the effect files
    void fn_8059B0F0(); // inferred: releases the effect files
    int fn_8059B1B4();  // inferred: nonzero while loading
    void fn_8059B8A8(HitInfo_unk* skill, CharaTask* user, const atn::Vector3* pos, const atn::Vector3* rot);

    // inferred: effect slot i (null out of range); called on the singleton
    Bullet_unk* GetBullet_unk(unsigned int i) {
        if (i >= 32) {
            return 0;
        }
        return &mBullets[i];
    }
    // inferred: first free effect slot
    Bullet_unk* FindFree_unk() {
        Bullet_unk* bullet = mBullets;
        for (int i = 0; i < 32; i++, bullet++) {
            if (!bullet->IsActive_unk()) {
                return &mBullets[i];
            }
        }
        return 0;
    }

    Bullet_unk mBullets[32];   // 0x48
    int mLoaded;               // 0x2848
    atn::EfpFile m284C;
    atn::EfpFile m2858;
    atn::EfpFile m2864;
    atn::EfpFile m2870;
    atn::EfpFile m287C;
    atn::EfpFile m2888;
    atn::EfpFile m2894;
    ResLoader_unk m28A0;
    atn::EfpFile m28AC;
    atn::EfpFile m28B8;
    ResLoader_unk m28C4;
    ResLoader_unk m28D0;
    ResLoader_unk m28DC;
    ResLoader_unk m28E8;
    atn::EfpFile m28F4;
    atn::EfpFile m2900;
    atn::EfpFile m290C;
    atn::EfpFile m2918;
    int m2924;
    int m2928; // inferred: passed to every effect by vf10
    int m292C;
};

#endif
