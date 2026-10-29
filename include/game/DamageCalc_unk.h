#ifndef GAME_DAMAGECALC_UNK_H
#define GAME_DAMAGECALC_UNK_H

#include "math/Vec.h"

class CharaTask;
class CharaStatus;
struct HitInfo_unk;
struct CharaStatusPair_unk;

// inferred: result of the damage computation (0x20 bytes; the same fields are HitMsg_unk::m64-m80
// in game/CharaTask.h). Names invented.
struct DamageResult_unk {
    DamageResult_unk() : mType(0), mDamage(0), m08(0), m0C(0), m10(0), m14(-1) {
        mFlag31 = 0;
        mFlag30 = 0;
        mFlag29 = 0;
        mFlag28 = 0;
        m18 = -1;
    }
    // inferred: the hit landed (types 1, 4 and 5)
    bool IsHit_unk() const { return mType == 1 || mType == 4 || mType == 5; }

    int mType;   // 0x00: inferred: 1 hit, 4 resisted, 5 strongly resisted (fn_80040B44)
    int mDamage; // 0x04: inferred: negative for a heal
    int m08;
    int m0C;     // inferred: levels removed (negative)
    int m10;
    int m14;     // inferred: element of the attack (fn_80040664)
    int m18;     // inferred: element the target is weak to (fn_800407F4)
    unsigned int mFlag31 : 1; // inferred: the target got state 0x20 from this hit
    unsigned int mFlag30 : 1;
    unsigned int mFlag29 : 1; // inferred: the attacker's buffs ran out
    unsigned int mFlag28 : 1;
    unsigned int mFlags27_0 : 28;
};

// inferred: options of the damage computation (0x18 bytes); default instance lbl_807C6B90.
// Names invented.
struct DamageParam_unk {
    DamageParam_unk() : mCombo(1), mRate(1.0f), m08(1.0f), m0C(0), m10(1.0f), m14(0) {}

    int mCombo;  // inferred: combo count (raises the damage)
    float mRate; // inferred: power of the attack (0..1)
    float m08;
    int m0C;     // inferred: bonus damage, divided by 10
    float m10;   // inferred: final rate
    int m14;     // inferred: nonzero: status 1 does not cancel the defence
};

extern "C" DamageParam_unk lbl_807C6B90;

// inferred: the damage computation (free functions; names unknown)
extern "C" int fn_8003EA3C(DamageResult_unk* result, HitInfo_unk* data, CharaTask* attacker, CharaTask* target,
                           unsigned int flags, const DamageParam_unk* param, const atn::Vector3* pos,
                           const atn::Vector3* rot);
extern "C" int fn_8003F030(DamageResult_unk* result, HitInfo_unk* data, CharaTask* attacker, const atn::Vector3* pos,
                           const atn::Vector3* rot, unsigned int flags);
extern "C" int fn_8003F1E4(DamageResult_unk* result, HitInfo_unk* data, CharaTask* attacker, CharaTask* target,
                           unsigned int flags);
extern "C" bool fn_8003F440(CharaTask* chara, CharaStatusPair_unk* skills);
extern "C" int fn_8003F538(HitInfo_unk* data, CharaStatus* attacker, CharaStatus* target, unsigned int flags,
                           const DamageParam_unk* param);
extern "C" void fn_80040434(float* elements, CharaStatus* status, unsigned int flags);
extern "C" float fn_80040588(const float* elements, CharaStatus* status, int positiveOnly);
extern "C" int fn_80040664(HitInfo_unk* data, CharaStatus* attacker, CharaStatus* target, unsigned int flags);
extern "C" int fn_800407F4(HitInfo_unk* data, CharaStatus* attacker, CharaStatus* target, unsigned int flags);
extern "C" int fn_80040994(HitInfo_unk* data, CharaStatus* attacker, CharaStatus* target, unsigned int flags);
extern "C" int fn_80040B40(DamageResult_unk* result, HitInfo_unk* data, CharaStatus* attacker, CharaStatus* target,
                           unsigned int flags, const DamageParam_unk* param);
extern "C" int fn_80040B44(DamageResult_unk* result, HitInfo_unk* data, CharaStatus* attacker, CharaStatus* target,
                           unsigned int flags, const DamageParam_unk* param);
extern "C" int fn_800413EC(int kind, CharaTask* chara, CharaTask* target);
extern "C" int fn_800417E8(int kind, CharaTask* chara, CharaTask* target);
extern "C" int fn_80041A28(int kind, CharaTask* chara, CharaTask* target);

#endif
