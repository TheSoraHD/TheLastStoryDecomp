#include "game/EnemyTask.h"
#include "atn/Memory.h"
#include "game/EnemyManager.h"
#include "game/GimmickCommon.h"
#include "game/GimmickManager.h"

extern "C" int rand();

extern "C" EnemyTask* fn_805A5374(atn::Task* parent, EnemyInfo_unk* info); // inferred: multiplayer enemy
extern "C" GimmickCommon* fn_803EC16C(atn::Task* parent, int item, int index); // inferred: creates a gimmick
extern "C" void* lbl_8087F610; // inferred: multiplayer session

// inferred: game settings (lbl_8087F0A8), only the field read here
struct EnemySettings_unk {
    unsigned char m000[0x330];
    int m330; // inferred: debug switch that always offers the finishing move
};
extern "C" EnemySettings_unk* lbl_8087F0A8;

extern "C" HitInfo_unk* fn_80219E6C(int id); // inferred: bullet/skill data by id
extern "C" char lbl_807C6B90[0x18];          // inferred: work area of the damage computation
// inferred: damage of `hit` from `attacker` to `target` (game/DamageCalc_unk.cpp)
extern "C" int fn_8003F538(HitInfo_unk* hit, CharaStatus* attacker, CharaStatus* target, int flags, void* work);

// inferred: reaction control created by vf6C (class unknown, 0x24 bytes)
struct EnemyReaction_unk {
    EnemyReaction_unk(CharaTask* chara, const atn::Vector3* dir, int kind); // 0x801AFAC0
    unsigned char m00[0x24];
};

EnemyTask* EnemyTask::fn_8035B534(atn::Task* parent, EnemyInfo_unk* info, int noCheck) {
    if (parent == 0) {
        return 0;
    }
    if (noCheck == 0) {
        EnemyTask* enemy = fn_805A5374(parent, info);
        if (enemy != 0) {
            return enemy;
        }
    }
    return new ((MemoryTag)3, "", "", 0) EnemyTask(parent, info->m20);
}

EnemyTask::EnemyTask(atn::Task* parent, void* data) : BattleCharaTask(parent, 2, data), mNext(0) {
    mId = EnemyManager::sInstance->NextId_unk();
    EnemyManager::sInstance->Add_unk(this);
    mFlag16 = 0;
    mF18 = mF14;
    mF14 = 1;
    if (m5C->mD0 > 0) {
        m1438 = fn_803EC16C(GimmickManager::instance_, 0x4650, mId);
        ((GimmickCommon*)m1438)->vf68(m5C->mD0, 1);
        m1438->fn_800D246C(1);
    }
}

EnemyTask::~EnemyTask() {
    if (EnemyManager::sInstance != 0) {
        EnemyManager::sInstance->Remove_unk(this);
    }
}

void EnemyTask::fn_8035B808(EnemyTask* enemy) {
    EnemyTask* cur = this;
    while (cur->mNext != 0) {
        cur = cur->mNext;
    }
    cur->mNext = enemy;
}

void EnemyTask::fn_8035B824(EnemyTask* enemy) {
    EnemyTask* prev = 0;
    EnemyTask* cur = this;
    while (cur != enemy) {
        prev = cur;
        cur = cur->mNext;
    }
    if (cur == enemy && prev != 0) {
        prev->mNext = cur->mNext;
    }
}

EnemyTask* EnemyTask::fn_8035B854() {
    EnemyManager* manager = EnemyManager::sInstance;
    if (manager != 0) {
        EnemyTask* enemy = manager->mHead;
        if (enemy != 0) {
            if (this == enemy) {
                while (enemy->mNext != 0) {
                    enemy = enemy->mNext;
                }
            } else {
                while (enemy->mNext != this) {
                    enemy = enemy->mNext;
                }
            }
            return enemy;
        }
    }
    return 0;
}

int EnemyTask::vf68(CharaTask* attacker) {
    if ((m14A8 >> 24) & 1) {
        if (lbl_8087F610 == 0) {
            int ok = 0;
            int count = 0;
            EnemyTask* last = 0;
            for (EnemyTask* e = EnemyManager::sInstance->mHead; e != 0; e = e->mNext) {
                if (e->CanAct_unk() && e->mC58.mC0 == 1) {
                    last = e;
                    count++;
                }
            }
            if (lbl_8087F0A8->m330 != 0 && CanAct_unk()) {
                count = 1;
                last = this;
            }
            if (count == 1 && last == this) {
                ok = 1;
                if (m55C == 6) {
                    switch (m560) {
                    case 23:
                    case 27:
                    case 28:
                    case 35:
                    case 36:
                    case 59:
                    case 113:
                    case 123:
                    case 124:
                    case 125:
                    case 126:
                    case 127:
                    case 142:
                        ok = 0;
                        break;
                    }
                }
                if ((unsigned int)m58C > 1 && m58C != 5) {
                    ok = 0;
                }
                if (mStatus.IsState_unk(0x40) || mStatus.IsState_unk(0x80)) {
                    ok = 0;
                }
                if (last->mStatus.m224 >= 1) {
                    ok = 0;
                }
            }
            if (ok) {
                HitInfo_unk* hit = fn_80219E6C(200);
                int damage = fn_8003F538(hit, &attacker->mStatus, &mStatus, 0x80, lbl_807C6B90);
                if (mStatus.m04 - damage <= 0.0f || lbl_8087F0A8->m330 != 0) {
                    return rand() % 3 + 9;
                }
            }
        }
    }
    return 0;
}

void EnemyTask::vf6C(int kind, CharaTask* other) {
    atn::Vector3 dir(mPos.x - other->mPos.x, mPos.y, mPos.z - other->mPos.z);
    dir.y = 0.0f;
    PSVECNormalize(&dir, &dir);
    if (kind >= 9 && kind <= 12) {
        fn_80178208((chr::ControlBase*)new ((MemoryTag)3, "", "", 0) EnemyReaction_unk(this, &dir, kind));
    }
}
