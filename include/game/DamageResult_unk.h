#ifndef GAME_DAMAGERESULT_UNK_H
#define GAME_DAMAGERESULT_UNK_H

// inferred: own header (from game/DamageCalc_unk.h) for the units that only show the damage
// (BattleTask::fn_80108C10) and declare the damage computation themselves
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

#endif
