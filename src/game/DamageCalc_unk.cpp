#include "game/DamageCalc_unk.h"
#include "atn/Memory.h"
#include "game/CharaStatus.h"
#include "game/CharaTask.h"
#include "game/GameConfig_unk.h"
#include "game/HitInfo_unk.h"
#include "game/Inventory.h"
#include "macros.h"
#include "string.h"
#include <boost/function.hpp>

// Everything below is inferred from the damage computation (0x8003EA3C..0x80041A80); names invented.

extern "C" int rand();
extern "C" void PSVECNormalize(const atn::Vector3* src, atn::Vector3* dst);

extern "C" HitInfo_unk* fn_80219E6C(int id); // inferred: bullet data by id
extern "C" int fn_8021A684(HitInfo_unk* data); // inferred: the bullet is an attack (kind test)
extern "C" int fn_8021A9CC(HitInfo_unk* data); // inferred: kind test
extern "C" void* fn_800EFC64(int id);         // inferred: effect of a skill (game/BattleEfp_unk.cpp)

// inferred: skill effect manager (as in gmk/HealPoint.cpp)
class SkillEffect;
extern "C" void* lbl_8087F9E8;
extern "C" SkillEffect* fn_8059B66C(void* mgr, HitInfo_unk* skill, CharaTask* user, CharaTask* target,
                                    const atn::Vector3* pos, const atn::Vector3* rot,
                                    boost::function<void(SkillEffect*)> callback, float rate);

// inferred: effect manager of the characters (lbl_8087F048)
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" void fn_800FB1BC(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* data, const atn::Vector3* pos,
                            const atn::Vector3* rot, int a, int b, float scale); // inferred: hit effect
extern "C" void fn_80108F38(Effect_unk* mgr, const atn::Vector3& pos, int status); // inferred: status effect
extern "C" float fn_8010CA2C(Effect_unk* mgr);
extern "C" int fn_8010CD3C(Effect_unk* mgr, int kind, CharaTask* chara, CharaTask* target);
extern "C" void fn_801092C8(Effect_unk* mgr, int id, CharaTask* chara, int a, int b);

// inferred: field (lbl_8087F430)
struct Field_unk {
    void fn_803750E4(int id);
};
extern "C" Field_unk* lbl_8087F430;
extern "C" int fn_80370174(Field_unk* field, int id); // inferred: field flag

extern "C" GameConfig_unk* lbl_8087F0A8;

// inferred: collision world (lbl_8087EE98)
extern "C" void* lbl_8087EE98;
extern "C" int fn_8004ED34(void* world, void* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, int a, int b);

// inferred: multiplayer state (lbl_8087F628, see gmk/Item.cpp)
struct MpState_unk {
    unsigned char m000[0x1F8];
    unsigned int m1F8_31_20 : 12;
    unsigned int mDisabled : 1;
    unsigned int m1F8_18_0 : 19;
};
extern "C" MpState_unk* lbl_8087F628;

// inferred: player of a multiplayer session (fn_804EB7C4)
struct MpPlayer_unk {
    unsigned char m000[0xD0];
    unsigned int mD0_31 : 1; // inferred: present
    unsigned int mD0_30 : 1;
    unsigned int mD0_29_0 : 30;
    unsigned char mD4[0xD4C - 0xD4];
    unsigned int mD4C; // inferred: flags (0x400, 0x800: no damage)
};
// inferred: team of a multiplayer session (fn_804EB814)
struct MpTeam_unk {
    unsigned char m000[0xB0];
    unsigned int mB0_31 : 1;
    unsigned int mB0_30_0 : 31;
};
// inferred: multiplayer session (lbl_8087F610, see MP/D2AMpMenuCockpit.cpp)
struct MpSession_unk {
    unsigned char m0000[0x50C];
    int m50C;
    unsigned char m0510[0x540 - 0x510];
    int m540; // inferred: game mode
    unsigned char m0544[0x99BC - 0x544];
    bool m99BC; // inferred: the damage is applied locally
};
extern "C" MpSession_unk* lbl_8087F610;
extern "C" MpPlayer_unk* fn_804EB7C4(MpSession_unk* session, CharaTask* chara);
extern "C" MpTeam_unk* fn_804EB814(MpSession_unk* session, CharaTask* chara);
extern "C" int fn_804EB4AC(MpSession_unk* session, MpPlayer_unk* player);

// inferred: data of the listener event of a hit (names invented)
struct HitEvent_unk {
    CharaTask* mAttacker;
    CharaTask* mTarget;
    int mId;
    unsigned int mFlags;
    int mFlag31;
    float mRate;
};

// inferred: listeners (see gmk/Item.cpp)
inline CharaListeners_unk* fn_8014F8F4() {
    static CharaListeners_unk sListeners;
    return &sListeners;
}
// inferred: inlined copy of CharaListeners_unk::fn_8014F860 (as in gmk/ReinforcePoint.cpp)
inline void NotifyListeners_unk(CharaListeners_unk* listeners, int id, CharaTask* chara, void* arg) {
    if (listeners->mEnabled != 0) {
        for (int i = 0; i < (int)listeners->mList.size(); i++) {
            CharaListener_unk& listener = listeners->mList[i];
            if (listener.mId == -1 || id == listener.mId) {
                listener.mFunc(id, chara, arg);
            }
        }
    }
}

// inferred: status used when a hit has no attacker (created on first use)
CharaStatus* lbl_8087EE70;
static CharaStatus* GetDummyStatus_unk() {
    if (lbl_8087EE70 == 0) {
        lbl_8087EE70 = new ((MemoryTag)3, "", "", 0) CharaStatus;
    }
    return lbl_8087EE70;
}

// inferred: clears the attacker's buffs of a hit
inline void ClearBuffs_unk(CharaTask* attacker, DamageResult_unk* result, unsigned int& flags) {
    attacker->mStatus.fn_80133E24(0x23, 9);
    if (attacker->mStatus.fn_80133F6C(-1, -1) <= 0) {
        result->mFlag29 = 1;
        flags &= ~0x2000000;
    }
}

// NONMATCHING: 99.9% - the original sets up the target status (r6) before the attacker status (r5)
// for fn_80040B44
int fn_8003EA3C(DamageResult_unk* result, HitInfo_unk* data, CharaTask* attacker, CharaTask* target,
                unsigned int flags, const DamageParam_unk* param, const atn::Vector3* pos,
                const atn::Vector3* rot) {
    if (attacker != 0 && (flags & 8) != 8) {
        attacker->mStatus.fn_80133E24(0x40, 9);
        ClearBuffs_unk(attacker, result, flags);
        if ((flags & 0x8000) == 0x8000) {
            result->mFlag28 = 1;
            flags &= ~0x8000;
        }
    }
    if (attacker != 0) {
        fn_80040B44(result, data, &attacker->mStatus, &target->mStatus, flags, param);
    } else {
        fn_80040B44(result, data, GetDummyStatus_unk(), &target->mStatus, flags, param);
    }
    if (data != 0 && result != 0 && result->IsHit_unk()) {
        if (fn_800EFC64(data->mB0) != 0) {
            pos = pos ? pos : &target->mPos;
            atn::Vector3 p = *pos;
            rot = rot ? rot : &target->mRot;
            atn::Vector3 r = *rot;
            float rate = attacker ? attacker->fn_8017AC24() : 1.0f;
            fn_8059B66C(lbl_8087F9E8, data, attacker, target, &p, &r, boost::function<void(SkillEffect*)>(), rate);
        }
    }
    if (attacker != 0 && result->mDamage < 0) {
        ClearBuffs_unk(attacker, result, flags);
    }
    if (attacker != 0 && target != 0 && fn_8021A684(data)) {
        CharaStatus* from = &attacker->mStatus;
        CharaStatus* to = &target->mStatus;
        if (from->fn_80133EE8(0x1A)) {
            CharaStatusEntry_unk* entry = from->fn_80133EE8(0x1A);
            CharaStatusRef_unk ref(entry, (unsigned int)attacker->m648, attacker, 900, 0);
            to->fn_80133FBC(ref, 1);
        }
        if (from->fn_80133EE8(0x1C)) {
            CharaStatusEntry_unk* entry = from->fn_80133EE8(0x1C);
            CharaStatusRef_unk ref(entry, (unsigned int)attacker->m648, attacker, 900, 0);
            to->fn_80133FBC(ref, 1);
        }
        if (from->fn_80133EE8(0x1E)) {
            CharaStatusEntry_unk* entry = from->fn_80133EE8(0x1E);
            CharaStatusRef_unk ref(entry, (unsigned int)attacker->m648, attacker, 900, 0);
            to->fn_80133FBC(ref, 1);
        }
        if (from->fn_80133EE8(0x20)) {
            CharaStatusEntry_unk* entry = from->fn_80133EE8(0x20);
            CharaStatusRef_unk ref(entry, (unsigned int)attacker->m648, attacker, 300, 0);
            to->fn_80133FBC(ref, 0);
        }
        if (from->fn_80133EE8(0x34)) {
            CharaStatusEntry_unk* entry = from->fn_80133EE8(0x34);
            Inventory::Order()->fn_8044D6E0((float)result->mDamage * entry->mValue / 100.0f + Inventory::Order()->mMoney);
        }
    }
    HitEvent_unk event;
    event.mId = data->m04;
    event.mFlags = flags;
    event.mAttacker = attacker;
    event.mTarget = target;
    event.mRate = param->mRate;
    event.mFlag31 = result->mFlag31;
    NotifyListeners_unk(fn_8014F8F4(), 7, attacker, &event);
    return result->mType;
}

int fn_8003F030(DamageResult_unk* result, HitInfo_unk* data, CharaTask* attacker, const atn::Vector3* pos,
                const atn::Vector3* rot, unsigned int flags) {
    if (attacker != 0) {
        fn_80040B44(result, data, &attacker->mStatus, GetDummyStatus_unk(), flags, &lbl_807C6B90);
    } else {
        fn_80040B44(result, data, GetDummyStatus_unk(), GetDummyStatus_unk(), flags, &lbl_807C6B90);
    }
    if (data != 0 && result != 0 && result->IsHit_unk()) {
        fn_800FB1BC(lbl_8087F048, attacker, data, pos, rot, flags, 1, attacker ? attacker->fn_8017AC24() : 1.0f);
    }
    return result->mType;
}

// NONMATCHING: 99.8% - same argument order of fn_80040B44 as fn_8003EA3C
int fn_8003F1E4(DamageResult_unk* result, HitInfo_unk* data, CharaTask* attacker, CharaTask* target,
                unsigned int flags) {
    if (attacker != 0) {
        fn_80040B44(result, data, &attacker->mStatus, &target->mStatus, flags, &lbl_807C6B90);
    } else {
        fn_80040B44(result, data, GetDummyStatus_unk(), &target->mStatus, flags, &lbl_807C6B90);
    }
    if (data != 0 && result != 0 && result->IsHit_unk()) {
        if (fn_800EFC64(data->mB0) != 0) {
            atn::Vector3 p = target->mPos;
            atn::Vector3 r = target->mRot;
            fn_8059B66C(lbl_8087F9E8, data, attacker, target, &p, &r, boost::function<void(SkillEffect*)>(), 1.0f);
        }
    }
    HitEvent_unk event;
    event.mId = data->m04;
    event.mFlags = flags;
    event.mAttacker = attacker;
    event.mTarget = target;
    event.mFlag31 = result->mFlag31;
    NotifyListeners_unk(fn_8014F8F4(), 6, attacker, &event);
    return result->mType;
}

bool fn_8003F440(CharaTask* chara, CharaStatusPair_unk* skills) {
    float cost = 0.0f;
    bool none = true;
    for (int i = 0; i < 2; i++) {
        int* id = &skills->m[i];
        if (*id > 0) {
            HitInfo_unk* data = fn_80219E6C(*id);
            if (data != 0) {
                none = false;
                cost += data->mBC;
            }
        }
    }
    if (chara != 0 && chara->fn_8016E484()) {
        none = false;
        cost = 0.0f;
    }
    if (none) {
        return false;
    }
    return chara->mStatus.m08 >= cost;
}

// inferred: keeps the elements forced by the flags at least neutral
inline void ForceElements_unk(float* elements, unsigned int flags) {
    float neutral = 1.0f;
    if ((flags & 0x800000) == 0x800000) {
        elements[0] = MAX(elements[0], neutral);
    }
    if ((flags & 0x1000000) == 0x1000000) {
        elements[1] = MAX(elements[1], neutral);
    }
    if ((flags & 0x4000000) == 0x4000000) {
        elements[4] = MAX(elements[4], neutral);
    }
    if ((flags & 0x8000000) == 0x8000000) {
        elements[3] = MAX(elements[3], neutral);
    }
}

// inferred: damage added by the elements
inline int ElementDamage_unk(int damage, const float* elements, CharaStatus* status, int positiveOnly) {
    return (int)(0.5f * (-1.0f * damage * fn_80040588(elements, status, positiveOnly)));
}

// inferred: damage rate of a combo (name invented)
inline float ComboRate_unk(int combo) {
    float rate = 1.0f;
    float step = 1.2f;
    float cap = 2.0f;
    if (lbl_8087F610 != 0) {
        step = 1.07f;
        cap = 2.6f;
    } else if (lbl_8087F0A8->mCurrentLevel == 3) {
        cap = 3.0f;
    }
    for (int i = 1; i < combo; i++) {
        rate *= step;
    }
    return MIN(rate, cap);
}

// NONMATCHING: 99.5% - register allocation: power, magic and the guard flag get r30/r29/r31
// instead of r29/r31/r30; the magic product loads the bullet rate first
int fn_8003F538(HitInfo_unk* data, CharaStatus* attacker, CharaStatus* target, unsigned int flags,
                const DamageParam_unk* param) {
    int result = 0;
    if (data != 0) {
        int power = (int)(attacker->mParamB.m00 * data->m28);
        power += data->m14;
        power += data->m18 > 0 ? rand() % data->m18 : 0;
        if ((flags & 0x40) == 0x40 && attacker->mChara != 0) {
            float ratio = attacker->mChara->mType == 2 ? lbl_8087F0A8->mEnemyDamageRatio
                                                       : lbl_8087F0A8->mPlayerDamageRatio;
            power = (int)(power * ratio);
        }
        if (!fn_80040994(data, attacker, target, flags)) {
            power = (int)(power * ComboRate_unk(param->mCombo));
        }
        if ((flags & 0x20000) == 0x20000) {
            power = (int)(1.5f * power);
        }
        if (power > 0 && ((unsigned int)target->mParamB.m9C & 0x400) == 0x400 && (flags & 0x10000) != 0x10000) {
            power = 0;
        }
        float magicRate = 1.0f;
        int magic = (int)(data->m2C * attacker->mParamB.m04);
        magic += data->m1C;
        magic += data->m20 > 0 ? rand() % data->m20 : 0;
        if (magic > 0 && ((unsigned int)target->mParamB.m9C & 0x800) == 0x800 && (flags & 0x10000) != 0x10000) {
            magic = 0;
        }
        int defence = (int)(target->mParamB.m08 * data->m30);
        if ((target->m0C & 0x40000) == 0x40000) {
            defence = (int)(0.2f * defence);
        }
        if ((flags & 8) == 8) {
            defence = (int)(defence * target->mParamB.m10);
        }
        if (attacker->fn_80133EE8(0x30)) {
            defence = (int)(0.5f * defence);
        }
        if ((flags & 0x800) == 0x800 || (param->m14 == 0 && attacker->fn_80133EE8(1))) {
            defence = 0;
        }
        int magicDefence = (int)(target->mParamB.m0C * data->m34);
        if ((flags & 8) == 8) {
            magicDefence = (int)(magicDefence * target->mParamB.m14);
        }
        if ((flags & 0x800) == 0x800) {
            magicDefence = 0;
        }
        int magicDamage = MAX(0, (int)(magicRate * (magic - magicDefence) + 0.01f));
        GameConfig_unk* config = lbl_8087F0A8;
        int damage = MAX(0, power - defence) + magicDamage;
        damage = MAX(damage, (int)(config->mAttackMinDamage * (power + magic)));
        float rateScale = 0.8f * param->mRate + 0.2f;
        damage = (int)(damage * rateScale);
        damage = (int)(damage * data->m38);
        if (lbl_8087F610 == 0 && attacker->mChara != 0 && target->mChara != 0) {
            int type = config->mLvBonusType;
            if ((type == 0 && attacker->mChara->mType == 0 && target->mChara->mType == 2) ||
                (type == 1 && attacker->mChara->mType == 0 && target->mChara->mType == 2) ||
                (type == 1 && attacker->mChara->mType == 3 && target->mChara->mType == 2)) {
                int diff = attacker->mParamB.m78 - target->fn_80133BD8(0);
                damage = (int)(damage * lbl_8087F0A8->fn_80187CC0(MAX(-5, MIN(5, diff))));
            }
        }
        if ((flags & 8) == 8) {
            if ((flags & 0x80000) == 0x80000) {
                damage = (int)(damage * lbl_8087F0A8->mMagicNormalGuard);
            } else {
                damage /= 4;
                if (lbl_8087F0A8->mCurrentLevel == 3 && target->mChara != 0 && target->mChara->mType != 2) {
                    damage /= 2;
                }
            }
            if (target->fn_80134134(0x2D)) {
                damage = (int)(damage * (100 - MIN(target->fn_80134168(0x2D, -1), 100)) / 100.0f);
            }
        }
        if (target->fn_80134134(0x45)) {
            int cut = target->fn_80134168(0x45, -1);
            if ((flags & 8) != 8) {
                cut = target->fn_80134168(0x45, 9);
            }
            damage = (int)(damage * (100 - MIN(target->fn_80134168(0x45, -1), 100)) / 100.0f);
        }
        if ((flags & 0x80) == 0x80) {
            damage *= 2;
        }
        if ((flags & 0x10) == 0x10) {
            damage = (int)(1.1f * damage);
        }
        if ((flags & 0x80000000) == 0x80000000) {
            if (attacker->mChara != 0 && attacker->mChara->mType == 0 && data->m04 == 0x133) {
                damage = (int)(damage * config->mHeadShootDamage);
            } else {
                damage = (int)(damage * config->mHeadShootSubDamage);
            }
        }
        if ((flags & 0x1000) == 0x1000) {
            damage *= 2;
        }
        if ((flags & 0x400) == 0x400 || (flags & 1) == 1) {
            damage = (int)(damage * lbl_8087F0A8->mDiveDamageRatio);
        }
        if (fn_80040994(data, attacker, target, flags)) {
            float heal = -1.0f;
            if (lbl_8087F610 != 0) {
                heal = -0.1f;
            }
            damage = (int)(damage * heal);
        } else {
            if ((flags & 0x800000) == 0x800000 || (flags & 0x1000000) == 0x1000000 ||
                (flags & 0x4000000) == 0x4000000 || (flags & 0x8000000) == 0x8000000) {
                if ((flags & 4) == 4) {
                    damage = (int)(1.2f * damage);
                } else {
                    damage = (int)(1.1f * damage);
                }
            }
            if ((flags & 0x100000) != 0x100000) {
                float elements[7];
                damage += ElementDamage_unk(damage, &data->m74.m00, target, 1);
                memset(elements, 0, sizeof(elements));
                fn_80040434(elements, attacker, flags);
                damage += ElementDamage_unk(damage, elements, target, 1);
                memset(elements, 0, sizeof(elements));
                ForceElements_unk(elements, flags);
                damage += ElementDamage_unk(damage, elements, target, 1);
            }
        }
        if (attacker->fn_80133EE8(3)) {
            damage = (int)((damage * attacker->fn_80133F18(3, -1)) / 10.0f);
        }
        if (attacker->fn_80133EE8(0x41)) {
            damage += attacker->fn_80133F18(0x41, -1);
        }
        if (attacker->fn_80133EE8(0x42) && (data->mAC & 0x2000000) == 0x2000000) {
            int bonus = attacker->fn_80133F18(0x42, -1);
            damage = (int)(damage * (1.0f + bonus / 100.0f));
        }
        if (attacker->fn_80133EE8(0x3B)) {
            damage = (int)((target->mParamB.m84 * attacker->fn_80133F18(0x3B, -1)) / 100.0f);
        }
        if (attacker->fn_80133EE8(0xB) && damage >= target->m04) {
            damage -= damage + 1 - (int)target->m04;
        }
        damage += param->m0C / 10;
        int final = (int)(damage * param->m10);
        if ((flags & 0x2000) == 0x2000) {
            final = 0;
        }
        if ((flags & 0x8000) == 0x8000) {
            final = 0;
        }
        if (attacker->fn_80133EE8(0xE)) {
            final = 0;
        }
        result = MIN(final, 99999);
    }
    if (lbl_8087F610 != 0) {
        if (lbl_8087F610->m50C != 0) {
            result = 0;
        } else if (target->mChara != 0) {
            MpPlayer_unk* player = fn_804EB7C4(lbl_8087F610, target->mChara);
            if (player != 0 && ((player->mD4C & 0x400) || (player->mD4C & 0x800))) {
                result = 0;
            }
        }
    }
    return result;
}

void fn_80040434(float* elements, CharaStatus* status, unsigned int flags) {
    if (status->fn_80133EE8(0x3F)) {
        CharaStatusEntry_unk* entry = status->mEntries.mData;
        for (unsigned int i = 0; i < status->mEntries.mCount; entry++, i++) {
            if (entry->mId == 0x3F) {
                switch (entry->mValue) {
                case 1:
                    elements[1] = MAX(elements[1], 1.0f);
                    break;
                case 2:
                    elements[0] = MAX(elements[0], 1.0f);
                    break;
                case 4:
                    elements[3] = MAX(elements[3], 1.0f);
                    break;
                case 5:
                    elements[4] = MAX(elements[4], 1.0f);
                    break;
                case 6:
                    elements[5] = MAX(elements[5], 1.0f);
                    break;
                case 7:
                    elements[6] = MAX(elements[6], 1.0f);
                    break;
                }
            }
        }
    }
}

float fn_80040588(const float* elements, CharaStatus* status, int positiveOnly) {
    float best = 0.0f;
    int index = -1;
    for (int i = 0; i < 7; i++) {
        float v = elements[i] * status->mParamB.m54[i];
        float a = atn::Abs_unk(v);
        if (a < 1.1920929e-7f) {
            continue;
        }
        if (atn::Abs_unk(best) < a) {
            best = v;
            index = i;
        }
    }
    if (index < 0) {
        return 0.0f;
    }
    if (positiveOnly != 0 && best >= 0.0f) {
        return 0.0f;
    }
    if (lbl_8087F430 != 0 && fn_80370174(lbl_8087F430, 0x11B) > 0) {
        best *= 2.0f;
    }
    return best;
}

int fn_80040664(HitInfo_unk* data, CharaStatus* attacker, CharaStatus* target, unsigned int flags) {
    if ((flags & 0x100000) == 0x100000) {
        return -1;
    }
    int index = -1;
    if ((flags & 0x800000) == 0x800000) {
        index = 0;
        if (target->mParamB.m54[0] < 0.0f) {
            return 0;
        }
    }
    if ((flags & 0x1000000) == 0x1000000) {
        index = 1;
        if (target->mParamB.m54[1] < 0.0f) {
            return 1;
        }
    }
    if ((flags & 0x4000000) == 0x4000000) {
        index = 4;
        if (target->mParamB.m54[4] < 0.0f) {
            return 4;
        }
    }
    if ((flags & 0x8000000) == 0x8000000) {
        index = 3;
        if (target->mParamB.m54[3] < 0.0f) {
            return 3;
        }
    }
    float elements[7];
    memcpy(elements, &data->m74, sizeof(elements));
    fn_80040434(elements, attacker, flags);
    float best = 0.0f;
    for (int i = 0; i < 7; i++) {
        float v = elements[i] * target->mParamB.m54[i];
        if (atn::Abs_unk(v) < 1.1920929e-7f) {
            continue;
        }
        if (best > v) {
            best = v;
            index = i;
        }
    }
    return index;
}

int fn_800407F4(HitInfo_unk* data, CharaStatus* attacker, CharaStatus* target, unsigned int flags) {
    int index = -1;
    if ((flags & 0x100000) == 0x100000) {
        return -1;
    }
    if ((flags & 0x800000) == 0x800000 && target->mParamB.m54[0] < 0.0f) {
        index = 0;
    }
    if ((flags & 0x1000000) == 0x1000000 && target->mParamB.m54[1] < 0.0f) {
        index = 1;
    }
    if ((flags & 0x4000000) == 0x4000000 && target->mParamB.m54[4] < 0.0f) {
        index = 4;
    }
    if ((flags & 0x8000000) == 0x8000000 && target->mParamB.m54[3] < 0.0f) {
        index = 3;
    }
    if (index >= 0) {
        return index;
    }
    float elements[7];
    memcpy(elements, &data->m74, sizeof(elements));
    fn_80040434(elements, attacker, flags);
    float best = 0.0f;
    for (int i = 0; i < 7; i++) {
        float v = elements[i] * target->mParamB.m54[i];
        float a = atn::Abs_unk(v);
        if (a < 1.1920929e-7f) {
            continue;
        }
        if (atn::Abs_unk(best) < a) {
            best = v;
            index = i;
        }
    }
    if (best >= 0.0f) {
        return -1;
    }
    return index;
}

// inferred: the resistance of an element is positive
inline bool IsPositive_unk(float v) {
    return v > 0.0f;
}

int fn_80040994(HitInfo_unk* data, CharaStatus* attacker, CharaStatus* target, unsigned int flags) {
    bool absorb = false;
    bool weak = false;
    if ((flags & 0x800000) == 0x800000) {
        bool positive = IsPositive_unk(target->mParamB.m54[0]);
        if (positive) {
            absorb = true;
        }
        if (!positive) {
            weak = true;
        }
    }
    if ((flags & 0x1000000) == 0x1000000) {
        bool positive = IsPositive_unk(target->mParamB.m54[1]);
        if (positive) {
            absorb = true;
        }
        if (!positive) {
            weak = true;
        }
    }
    if ((flags & 0x4000000) == 0x4000000) {
        bool positive = IsPositive_unk(target->mParamB.m54[4]);
        if (positive) {
            absorb = true;
        }
        if (!positive) {
            weak = true;
        }
    }
    if ((flags & 0x8000000) == 0x8000000) {
        bool positive = IsPositive_unk(target->mParamB.m54[3]);
        if (positive) {
            absorb = true;
        }
        if (!positive) {
            weak = true;
        }
    }
    if (weak) {
        return 0;
    }
    if (absorb) {
        return 1;
    }
    float copy[7];
    memcpy(copy, &data->m74, sizeof(copy));
    float elements[7];
    memcpy(elements, copy, sizeof(elements));
    fn_80040434(elements, attacker, flags);
    return ElementDamage_unk(10, elements, target, 0) < 0;
}

int fn_80040B40(DamageResult_unk* result, HitInfo_unk* data, CharaStatus* attacker, CharaStatus* target,
                unsigned int flags, const DamageParam_unk* param) {
    return fn_80040B44(result, data, attacker, target, flags, param);
}

int fn_80040B44(DamageResult_unk* result, HitInfo_unk* data, CharaStatus* attacker, CharaStatus* target,
                unsigned int flags, const DamageParam_unk* param) {
    result->mType = 0;
    result->mDamage = 0;
    result->m08 = 0;
    result->mFlag31 = 0;
    bool had = target->IsState_unk(0x20);
    bool apply = rand() % 100 < data->m03;
    if ((flags & 0x20) == 0x20) {
        apply = false;
    }
    unsigned int status = 0;
    if (apply) {
        status = data->m90.mFlags;
    }
    if ((data->mAC & 0x80) == 0x80) {
        unsigned int bits[32];
        int count = 0;
        unsigned int* out = bits;
        unsigned int mask = data->m90.mFlags;
        unsigned int bit;
        for (int i = 0; i < 32; i++) {
            bit = 1 << i;
            if (bit == (bit & mask)) {
                *out++ = bit;
                count++;
            }
        }
        status = bits[rand() % count];
    }
    if (target->mChara != 0 && target->fn_8012E534() == 0) {
        if (attacker->fn_80133EE8(0x10)) {
            Effect_unk* effect = lbl_8087F048;
            status |= 0x20;
            if (effect != 0) {
                fn_80108F38(effect, target->mChara->fn_801781B0(), 0x20);
            }
        }
        if (attacker->fn_80133EE8(0x11)) {
            status |= 0x10000;
        }
        if (attacker->fn_80133EE8(0x12)) {
            status |= 0x400000;
        }
        if (attacker->fn_80133EE8(0x13)) {
            status |= 0x80;
        }
        if (attacker->fn_80133EE8(0x14)) {
            status |= 0x1;
        }
        if (attacker->fn_80133EE8(0x15)) {
            status |= 0x40;
        }
        if (attacker->fn_80133EE8(0x16)) {
            status |= 0x10;
        }
        if (attacker->fn_80133EE8(0x17)) {
            status |= 0x8000;
        }
        if (attacker->fn_80133EE8(0x19)) {
            status |= 0x4;
        }
        if (attacker->fn_80133EE8(0x18) && (flags & 8) != 8 && (flags & 0x400) != 0x400 && (flags & 1) != 1 &&
            (flags & 0x20) != 0x20) {
            status |= 0x4000;
        }
    }
    if (data->m01 == 2) {
        if (data->m24 != 0 && (data->mAC & 0x20) == 0x20) {
            target->m224 = MIN(target->m224 + data->m24, target->mParamB.m98);
        }
        int heal = data->m1C;
        heal += rand() % (data->m20 + 1) - data->m20 / 2;
        heal += (int)(attacker->mParamB.m04 * data->m2C);
        if (target->mChara != 0 && !target->mChara->mFlag2) {
            target->fn_8012DF7C(-heal, status, data);
        }
        result->mDamage = -heal;
        result->m0C = 0;
        result->mType = 1;
    } else if (data->m01 == 3) {
        result->mDamage = 0;
        result->mType = 1;
    } else if (data->m01 == 0 || data->m01 == 1) {
        int levels = 0;
        if (data->m24 != 0) {
            levels = MIN(data->m24, target->mParamB.m78 - 1);
            target->fn_80133B30(target->m1C - levels);
        }
        int damage = fn_8003F538(data, attacker, target, flags, param);
        result->m14 = fn_80040664(data, attacker, target, flags);
        result->m18 = fn_800407F4(data, attacker, target, flags);
        bool local = true;
        if (lbl_8087F628 != 0 && !lbl_8087F628->mDisabled && lbl_8087F610 != 0) {
            local = lbl_8087F610->m99BC;
            if (!local) {
                CharaTask* chara = target->mChara;
                if (chara != 0) {
                    CharaTask* other = attacker->mChara;
                    fn_804EB7C4(lbl_8087F610, other);
                    MpPlayer_unk* player = fn_804EB7C4(lbl_8087F610, chara);
                    int mode = lbl_8087F610->m540;
                    if (mode == 1 || !mode) {
                        if (player != 0 && player->mD0_31 == 1 &&
                            (player->mD0_30 == 1 || fn_804EB4AC(lbl_8087F610, player) != 0)) {
                            local = true;
                        }
                    } else {
                        fn_804EB814(lbl_8087F610, other);
                        MpTeam_unk* team = fn_804EB814(lbl_8087F610, chara);
                        if (other != 0) {
                            if ((player != 0 && player->mD0_31 == 1 &&
                                 (player->mD0_30 == 1 || fn_804EB4AC(lbl_8087F610, player) != 0)) ||
                                (team != 0 && team->mB0_31 == 1)) {
                                local = true;
                            }
                        } else {
                            if ((player != 0 && player->mD0_31 == 1 &&
                                 (player->mD0_30 == 1 || fn_804EB4AC(lbl_8087F610, player) != 0)) ||
                                (team != 0 && team->mB0_31 == 1)) {
                                local = true;
                            }
                        }
                    }
                }
            }
        }
        if (local == 1) {
            target->fn_8012DF7C(damage, status, data);
        }
        result->mDamage = damage;
        result->m0C = -levels;
        float elements[7];
        memcpy(elements, &data->m74, sizeof(elements));
        fn_80040434(elements, attacker, flags);
        ForceElements_unk(elements, flags);
        int element = ElementDamage_unk(4, elements, target, 0);
        if ((flags & 0x100000) == 0x100000) {
            element = 0;
        }
        if (element <= -2) {
            result->mType = 5;
            if (lbl_8087F430 != 0) {
                lbl_8087F430->fn_803750E4(0x15B);
            }
        } else if (element <= -1) {
            result->mType = 4;
        } else if (element >= 1) {
            result->mType = 1;
            if (attacker->mChara != 0 && attacker->mChara->mType == 0 && lbl_8087F430 != 0) {
                lbl_8087F430->fn_803750E4(0x15A);
            }
        } else {
            result->mType = 1;
        }
        unsigned int immune = target->m18 | target->m240;
        if ((status & 0x80000) == 0x80000 && (immune & 0x80000) == 0) {
            result->mFlag30 = 1;
        }
    } else if (fn_8021A9CC(data)) {
        result->mDamage = 0;
        result->mType = 1;
    }
    if (!had && target->IsState_unk(0x20)) {
        result->mFlag31 = 1;
    }
    return result->mType;
}

// NONMATCHING: 99.5% - float registers of the probe end point (from + dir * length)
int fn_800413EC(int kind, CharaTask* chara, CharaTask* target) {
    int result = 0;
    unsigned int size = chara->m5C->m11C;
    float rate = 0.1f * (chara->mStatus.mParamB.m78 - target->mStatus.fn_80133BD8(0));
    float chance = MIN(0.5f, MAX(0.0f, rate));
    if (kind == 0 && ((unsigned int)chara->mStatus.m14 & 0x2000) == 0x2000) {
        chance += fn_8010CA2C(lbl_8087F048);
    }
    if (kind == 1 && lbl_8087F0A8->mCounterBonus == 0) {
        chance = 0.0f;
    }
    if (kind == 1) {
        if (chara->mF58 < 12 && lbl_8087F0A8->mInput.m4C < 12) {
            chance = 1.0f;
        }
        if (chara->fn_801789D8(0x1789, 0)) {
            chance = 1.0f;
        }
        if (chara->mStatus.fn_80134134(0xC)) {
            CharaStatusRef_unk* ref = chara->mStatus.fn_80134134(0xC);
            if (ref->mEntry->m10 > rand() % 100) {
                chance = 1.0f;
            }
        }
    }
    int blocked = 0;
    if (kind == 0 && size <= 1) {
        atn::Vector3 dir = target->mPos - chara->mPos;
        dir.y = 0.0f;
        PSVECNormalize(&dir, &dir);
        atn::Vector3 from = target->mPos;
        from.y += 10.0f;
        atn::Vector3 to = from + dir * (8.0f + target->m5B0);
        void* world = lbl_8087EE98;
        if (fn_8004ED34(world, 0, &from, &to, target->fn_80179D44() | 0x80000000, 0, 0)) {
            blocked = chara->fn_80156120(target);
            chance += (target->mFlag22 == 1 || lbl_8087F610 != 0) ? 0.05f : 0.3f;
        }
    }
    if ((float)(rand() % 1001) / 1000.0f < chance) {
        switch (kind) {
        case 0:
            if (size <= 1) {
                if (blocked) {
                    result = 4;
                } else if (chara->fn_80156120(target)) {
                    if (target->fn_8016ADF4(chara) && rand() % 3 == 0 && lbl_8087F610 == 0) {
                        result = 5;
                    } else {
                        result = 2;
                    }
                } else {
                    result = 1;
                }
            } else {
                result = 1;
            }
            break;
        case 1:
            if (size <= 2) {
                result = 3;
            }
            break;
        case 2:
        case 3:
            result = 1;
            break;
        }
    }
    return result;
}

int fn_800417E8(int kind, CharaTask* chara, CharaTask* target) {
    int forced = chara->vfA8(kind);
    if (forced >= 0) {
        return forced;
    }
    chr::CharaData_unk* data = chara->m5C;
    float chance = 0.0f;
    int result = 0;
    unsigned int size = data->m11C;
    if (kind == 0) {
        int count = data->mParam.m80;
        if (count > 0) {
            chance = 0.3f * MIN(count, 2);
        }
        if (chara->mType == 3 && ((unsigned int)chara->mStatus.m14 & 0x2000) == 0x2000) {
            chance += fn_8010CA2C(lbl_8087F048);
        }
    }
    if (kind == 1) {
        if (lbl_8087F048 != 0 && fn_8010CD3C(lbl_8087F048, 3, chara, target)) {
            fn_801092C8(lbl_8087F048, 0xE, chara, 3, 0);
            chance = 1.0f;
        }
        if (chara->mStatus.fn_80134134(0xC)) {
            CharaStatusRef_unk* ref = chara->mStatus.fn_80134134(0xC);
            if (rand() % 100 < ref->mEntry->m10) {
                chance = 1.0f;
            }
        }
        if (chara->fn_801789D8(0x1789, 0)) {
            chance = 1.0f;
        }
    }
    if ((float)(rand() % 1001) / 1000.0f < chance) {
        switch (kind) {
        case 0:
            if (size <= 1) {
                result = 2;
            }
            break;
        case 1:
            if (size <= 2) {
                result = 3;
            }
            break;
        }
    }
    return result;
}

int CharaTask::vfA8(int kind) {
    return -1;
}

int fn_80041A28(int kind, CharaTask* chara, CharaTask* target) {
    if (target == 0) {
        return 0;
    }
    if (chara->mType == 0) {
        return fn_800413EC(kind, chara, target);
    }
    return fn_800417E8(kind, chara, target);
}

DamageParam_unk lbl_807C6B90;
