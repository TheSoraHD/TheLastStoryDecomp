#include "gmk/BulletPoint.h"
#include "atn/Memory.h"
#include "game/Bullet.h"
#include "game/CharaManager.h"
#include "game/CharaStatus.h"
#include "game/CharaTask.h"
#include "game/EnemyManager.h"
#include "game/EnemyTask.h"
#include "game/PlayerTask.h"

extern "C" int rand();
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
// inferred: field object (class unknown)
struct BulletField_unk {
    unsigned char m00[0x10D8];
    RouteTable_unk* m10D8; // inferred: route tables
};
extern "C" BulletField_unk* lbl_8087F430;
// inferred: game settings
struct BulletSettings_unk {
    unsigned char m00[0x30];
    int mFps; // 0x30: inferred: frames per second
};
extern "C" BulletSettings_unk* lbl_8087F0A8;
// inferred: bullet pool (class unknown)
struct BulletPool_unk;
extern "C" BulletPool_unk* lbl_8087F048;
extern "C" Bullet* fn_801010A0(BulletPool_unk* pool); // inferred: a free bullet
extern "C" HitInfo_unk* fn_80219E6C(int id);          // inferred: bullet data by id
extern "C" int fn_800EFD04(int id);                   // inferred: effect resources of a bullet
extern "C" int fn_800EFDA0(int id);
// inferred: gravity per frame (names invented)
inline float Gravity_unk() {
    return -98.1f / (float)(lbl_8087F0A8->mFps * lbl_8087F0A8->mFps);
}
// inferred: scale of the shot (an inline function: its constant goes before 1.0f in the pool)
inline float ShotScale_unk() { return 0.1f; }
// inferred: next character of the player/enemy lists (0x14AC, as in gmk/FlamingArrow)
struct BulletListNode_unk {
    unsigned char m0000[0x14AC];
    CharaTask* mNext;
};
// inferred: names invented (same checks as gmk/FlamingArrow)
inline bool CanTarget_unk(CharaTask* chara) {
    bool ok = false;
    bool disabled = false;
    if (chara->IsFlag_unk(4) || chara->IsFlag_unk(1)) {
        disabled = true;
    }
    if (!disabled && (chara->mStatus.m0C & 0x20) != 0x20) {
        bool held = false;
        if (chara->m55C == 6 && chara->m560 == 28) {
            held = true;
        }
        if (!held) {
            ok = true;
        }
    }
    return ok;
}
// inferred: list of characters (16 entries; names invented)
struct BulletTargets_unk {
    BulletTargets_unk() { mCount = 0; }
    template <class T>
    void Collect_unk(CharaManager<T>* manager) {
        for (T* chara = manager->mHead; chara != 0; chara = (T*)((BulletListNode_unk*)chara)->mNext) {
            if (CanTarget_unk(chara)) {
                CharaTask** slot = &mData[mCount];
                if (slot != 0) {
                    *slot = chara;
                }
                if (++mCount >= 16) {
                    return;
                }
            }
        }
    }
    bool IsEmpty_unk() { return mCount == 0; }
    unsigned int Size_unk() { return mCount; }
    CharaTask** At_unk(unsigned int i) { return &mData[i]; }
    unsigned int mCount;
    CharaTask* mData[16];
};

namespace gmk {

GimmickCommon* BulletPoint::fn_8041B970(atn::Task* parent, GimmickDesc_unk* desc) {
    if (parent != 0) {
        return new ((MemoryTag)12, "", "", 0) BulletPoint(parent, desc);
    }
    return 0;
}

int BulletPoint::vf0C() {
    if (fn_803EC91C() == 0) {
        m54 = 2;
        return 1;
    }
    return 0;
}

void BulletPoint::Start() {}

// inferred: fires a bullet every desc->m28 frames at a place or at a random character
// NONMATCHING: 98.5% - (1) the original reloads the target count from the stack for the empty test
// and again into r26 (here one load in r26); (2) the 0.1f/1.0f constants of the shot are in the
// other order in .sdata2; (3) registers of the inlined Collect_unk (as in gmk/FlamingArrow).
// Tried: IsEmpty/Size/At helpers and free functions, count before/after the test, a bool
// temporary, the shot set up in a constructor/inline helper/with a scale local (before or after
// the other fields), CanTarget_unk with int/bool locals and an IsDisabled_unk helper. The table and
// id locals before FindPlace_unk fix most of the place loop registers.
void BulletPoint::vf10() {
    if (vf60() == 0) {
        return;
    }
    if (++mTimer <= mDesc->m28) {
        return;
    }
    mTimer = 0;
    Bullet* bullet = fn_801010A0(lbl_8087F048);
    if (bullet == 0) {
        return;
    }
    HitInfo_unk* skill = fn_80219E6C(mDesc->m20);
    RouteTable_unk* table = lbl_8087F430->m10D8;
    int id = mDesc->m24;
    RoutePlace_unk* place = table->FindPlace_unk(id);
    atn::Vector3 target;
    if (place != 0) {
        target = place->mPos;
    } else {
        BulletTargets_unk list;
        if (mDesc->m2C == 2) {
            list.Collect_unk(EnemyManager::sInstance);
        } else {
            list.Collect_unk(lbl_8087F8A0);
        }
        if (list.IsEmpty_unk()) {
            return;
        }
        unsigned int count = list.Size_unk();
        target = (*list.At_unk(rand() % count))->mPos;
    }
    atn::Vector3 offset(0.0f, 0.0f, mDesc->m40 * (rand() % 1001 / 1000.0f));
    atn::Matrix3x4 rot;
    rot.fn_8013A13C(6.2831855f * (rand() % 1001 / 1000.0f));
    PSMTXMultVec(rot.m, &offset, &offset);
    target += offset;
    atn::Vector3 dir;
    float speed = skill->m4C;
    if ((int)skill->m02 == 1) {
        dir = target - mDesc->mPos;
        dir.y = 0.0f;
        float time = dir.Length_unk() / speed;
        float height = mDesc->mPos.y - target.y;
        float vy = time * (-0.5f * Gravity_unk()) - height / time;
        dir.Normalize_unk();
        dir *= speed;
        dir.y = vy;
        speed = dir.Length_unk();
        dir.Normalize_unk();
    } else {
        dir = target - mDesc->mPos;
        dir.Normalize_unk();
    }
    BulletShot_unk shot;
    shot.m04 = 0;
    shot.m08 = 0;
    shot.m10 = 0;
    shot.m14 = 0.0f;
    shot.m18 = 1.0f;
    shot.m00 = ShotScale_unk();
    shot.m04 = fn_800EFD04(skill->mB0);
    shot.m08 = fn_800EFDA0(skill->mB0);
    shot.m0C = skill;
    bullet->fn_8010EFF0(&shot, &mDesc->mPos, &dir, mDesc->m2C, -1, 0, speed);
}

void BulletPoint::vf14() {}

int BulletPoint::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    return m54 = msg->mType;
}

int BulletPoint::vf60() {
    int ret = 0;
    if (m54 == 2 && mDesc->m7C != 0) {
        ret = 1;
    }
    return ret;
}
} // namespace gmk
