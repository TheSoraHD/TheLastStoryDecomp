#ifndef GAME_SKILLEFFECTMANAGER_H
#define GAME_SKILLEFFECTMANAGER_H

#include "atn/Task.h"
#include "atn/EfpHandle_unk.h"
#include "atn/ResLoader_unk.h"
#include "game/HitInfo_unk.h"
#include "db/ParseUtil_unk.h"
#include "atn/SoundRef.h"
#include "math/Vec.h"
#include <boost/function.hpp>

class CharaTask;

// Effect of a skill/magic (0x140 bytes, 32 of them in SkillEffectManager); its methods are in
// game/SkillEffect.cpp (0x80598874-0x8059AB94, its own float pool). Class name from the RTTI of
// boost::_bi::bind_t<..., mf1<void, gmk::HealPoint, SkillEffect *>> (previously Bullet_unk).
struct SkillEffect {
    SkillEffect();  // 0x80598874
    ~SkillEffect(); // 0x80598900
    void fn_805991E0();        // inferred: stops it (runs the callback)
    void fn_80599C6C();        // inferred: draw (empty in this build)
    int fn_80599C90(CharaTask* chara); // inferred: the effect can reach `chara`
    int fn_80599D60(CharaTask* chara, int check); // inferred: the effect touches `chara`
    float fn_80599FB4(int keep);        // inferred: scale of the effect
    void fn_80599FFC(const atn::Vector3* pos); // inferred: plays the efp of the skill at `pos`
    void fn_8059A0B4(atn::EffFile* eff);       // inferred: starts its eff effect (lit by the map)
    void fn_8059A1FC(int keep);         // inferred: stops its efp effect
    int fn_8059A264(); // inferred: nonzero when the effect uses its own copy of the hit data (m80)

    // inferred: replaces the callback (weak copy 0x8059C3F4; name invented)
    void SetCallback_unk(boost::function<void(SkillEffect*)> callback) { mCallback = callback; }
    // inferred: state 3 = finished
    bool IsActive_unk() { return m04 != 0 && m00 != 3; }
    // inferred: own copy of the hit data
    HitInfo_unk* GetOwnHit_unk() { return (HitInfo_unk*)&m80; }
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
    atn::Matrix3x4 m2C; // inferred: matrix of its eff effect (also the owner key of the effect)
    float m5C;         // inferred: radius
    int m60;           // inferred: frame counter
    atn::SoundRef m64;
    boost::function<void(SkillEffect*)> mCallback; // 0x68: inferred: called when the effect ends
    int m7C;           // inferred: the efp effect is playing
    BulletParam_unk m80; // inferred: own copy of the hit data
    int m13C; // inferred: set when the effect is reflected (fn_8059D3B0)
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
    void fn_8059C084(CharaTask* chara); // inferred: hits `chara` with the effects that touch it
    void fn_8059C2A8(CharaTask* target, HitInfo_unk* hit); // inferred: stops the effects on target (of `hit`)
    void fn_8059C32C(int clear); // inferred: stops every effect (and drops its callback)
    // inferred: active effect of `owner` (any if null) with skill id `id` (any if <= 0) and all the
    // flags `flags` (any if <= 0)
    SkillEffect* fn_8059C5DC(CharaTask* owner, int id, int flags);
    int fn_8059C6DC(HitInfo_unk* hit, CharaTask* owner); // inferred: `owner` has an effect of `hit`
    // inferred: status effects shown on a character (start/stop pairs; ids 10-15 of fn_80232B7C)
    void fn_8059C800(CharaTask* chara);
    void fn_8059C8D8(CharaTask* chara);
    void fn_8059C8E8(CharaTask* chara);
    void fn_8059C98C(CharaTask* chara);
    void fn_8059C99C(CharaTask* chara);
    void fn_8059CA74(CharaTask* chara);
    void fn_8059CA84(CharaTask* chara);
    void fn_8059CB2C(CharaTask* chara);
    void fn_8059CB3C(CharaTask* chara);
    void fn_8059CBE4(CharaTask* chara);
    void fn_8059CBF4(CharaTask* chara);
    void fn_8059CC9C(CharaTask* chara);
    // inferred: plays `eff` on the weapons of `chara` at `offset` (effect id `id`)
    void fn_8059CCAC(CharaTask* chara, int id, atn::EffFile* eff, const atn::Vector3* offset);
    // inferred: weapon effects (start/stop pairs, ids 16-19) and status effect 20
    void fn_8059CF70(CharaTask* chara);
    void fn_8059CFB0(CharaTask* chara, int keep);
    void fn_8059CFC4(CharaTask* chara);
    void fn_8059D004(CharaTask* chara, int keep);
    void fn_8059D018(CharaTask* chara);
    void fn_8059D058(CharaTask* chara, int keep);
    void fn_8059D06C(CharaTask* chara);
    void fn_8059D0AC(CharaTask* chara, int keep);
    void fn_8059D0C0(CharaTask* chara);
    void fn_8059D168(CharaTask* chara);
    void fn_8059D178();         // inferred: refreshes the effects of the players and allies (fn_8021A984)
    void fn_8059D268(int arg);  // inferred: fn_8059A1FC on every active effect
    int fn_8059D30C(CharaTask* chara); // inferred: effects of the allies of `chara` with fn_8021A984
    void fn_8059D3B0(CharaTask* chara); // inferred: `chara` reflects the enemy effects

    // inferred: effect slot i (null out of range); called on the singleton
    SkillEffect* GetBullet_unk(unsigned int i) {
        if (i >= 32) {
            return 0;
        }
        return &mBullets[i];
    }
    // inferred: first free effect slot
    SkillEffect* FindFree_unk() {
        SkillEffect* bullet = mBullets;
        for (int i = 0; i < 32; i++, bullet++) {
            if (!bullet->IsActive_unk()) {
                return &mBullets[i];
            }
        }
        return 0;
    }

    SkillEffect mBullets[32];   // 0x48
    int mLoaded;               // 0x2848
    atn::EfpFile m284C;
    atn::EfpFile m2858;
    atn::EfpFile m2864;
    atn::EfpFile m2870;
    atn::EfpFile m287C;
    atn::EfpFile m2888;
    atn::EfpFile m2894;
    atn::EffFile m28A0;
    atn::EfpFile m28AC;
    atn::EfpFile m28B8;
    atn::EffFile m28C4;
    atn::EffFile m28D0;
    atn::EffFile m28DC;
    atn::EffFile m28E8;
    atn::EfpFile m28F4;
    atn::EfpFile m2900;
    atn::EfpFile m290C;
    atn::EfpFile m2918;
    int m2924;
    int m2928; // inferred: passed to every effect by vf10
    int m292C;
};

// inferred: the singleton (referenced by game/CharaTask.cpp and gmk/HealPoint.cpp)
extern "C" SkillEffectManager* lbl_8087F9E8;

#endif
