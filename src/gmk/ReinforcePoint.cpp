#include "gmk/ReinforcePoint.h"
#include "atn/Memory.h"
#include "game/CharaManager.h"
#include "game/CharaStatus.h"
#include "game/CharaTask.h"
#include "game/EnemyManager.h"
#include "game/EnemyTask.h"
#include "game/PlayerTask.h"

extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
// inferred: field object (class unknown)
struct ReinforceField_unk {
    unsigned char m00[0x10D8];
    RouteTable_unk* m10D8;
    unsigned char m10DC[0x54E4 - 0x10DC];
    int m54E4; // inferred: 8 while the field is paused
};
extern "C" ReinforceField_unk* lbl_8087F430;
// inferred: game settings (names invented)
struct ReinforceLevel_unk {
    int m00; // inferred: bonus added to CharaStatusData_unk::m08
    unsigned char m04[0x20 - 0x04];
};
struct Settings_unk {
    unsigned char m00[0xD4];
    int mD4; // inferred: difficulty (index into mE4)
    unsigned char mD8[0xE4 - 0xD8];
    ReinforceLevel_unk mE4[4];
};
extern "C" Settings_unk* lbl_8087F0A8;
extern "C" void* lbl_8087F490;
extern "C" void fn_803E598C(void* obj, int arg1, int arg2, int arg3);

// inferred: inlined here (the local static is shared with the out-of-line copy in CharaTask.cpp)
inline CharaListeners_unk* fn_8014F8F4() {
    static CharaListeners_unk sListeners;
    return &sListeners;
}
// inferred: inlined copy of CharaListeners_unk::fn_8014F860 (name invented)
inline void NotifyListeners_unk(CharaListeners_unk* listeners, int id, void* chara, void* arg) {
    if (listeners->mEnabled != 0) {
        for (int i = 0; i < (int)listeners->mList.size(); i++) {
            CharaListener_unk& listener = listeners->mList[i];
            if (listener.mId == -1 || id == listener.mId) {
                listener.mFunc(id, (CharaTask*)chara, arg);
            }
        }
    }
}

// inferred: flag test through an accessor (the flag word is loaded again before it is changed);
// name invented
inline bool HasCharaFlag_unk(const CharaTask* chara, unsigned int flag) {
    return (chara->m54C & flag) == flag;
}

// inferred: the four values are built in a temporary (constructor); name invented
struct Float4Init_unk : public Model_unk::Float4_unk {
    Float4Init_unk(float v) {
        m00 = v;
        m04 = v;
        m08 = v;
        m0C = v;
    }
};
// inferred: bit 25 of the flag word at 0x12A4 through an accessor (reloaded before it is cleared)
inline unsigned int IsFlag25_unk(const CharaTask* chara) {
    return chara->mFlag25;
}

// inferred: setters through accessors (names invented)
inline void SetCharaFlag_unk(CharaTask* chara, unsigned int flag) {
    chara->m54C |= flag;
}
inline void ClearCharaFlag_unk(CharaTask* chara, unsigned int flag) {
    chara->m54C &= ~flag;
}

namespace gmk {

GimmickCommon* ReinforcePoint::fn_803F9420(atn::Task* parent, GimmickDesc_unk* desc) {
    if (parent != 0) {
        return new (12, "", "", 0) ReinforcePoint(parent, desc);
    }
    return 0;
}

int ReinforcePoint::vf0C() {
    if (fn_803EC91C() == 0) {
        int state = 1;
        if ((mDesc->m28 & 1) == 1) {
            state = 2;
        }
        GimmickMsg_unk msg(state);
        vf40(&msg);
        return 1;
    }
    return 0;
}

void ReinforcePoint::Start() {
    m6C = mDesc->mPos;
    m78.y = mDesc->mRotY;
    switch (mDesc->m20) {
    case 2:
        mChara = EnemyManager::sInstance->Find_unk(mDesc->m24);
        break;
    case 3:
        mChara = lbl_8087F8A0->Find_unk(mDesc->m24);
        break;
    }
    if (mChara != 0) {
        mChara->m54C |= 0x80000;
    }
    mRespawn = (mDesc->m28 >> 1) & 1;
}

// NONMATCHING: 99.8% - register allocation in the desc->m30 == 3 branch (the two position addresses,
// the global and the place search loop use other volatile registers). Tried: if/ternary/inline
// helper for the search, declaration orders (table before place fixes table/place), id local,
// copy constructors.
void ReinforcePoint::vf10() {
    if (mChara == 0 || mChara->mFlag2 || vf60() == 0) {
        if (mChara != 0 && HasCharaFlag_unk(mChara, 0x80000)) {
            mChara->m54C &= ~0x80000;
        }
        return;
    }
    if (lbl_8087F430 != 0 && lbl_8087F430->m54E4 == 8) {
        return;
    }
    if ((mChara->mStatus.m0C & 0x20) == 0x20 && mDown == 0) {
        mDown = 1;
        mCount++;
    }
    if (mDesc->m34 > 0 && mFrame >= mDesc->m34) {
        mFrame = 0;
        GimmickMsg_unk msg(1);
        vf40(&msg);
        return;
    }
    if (mChara->mFlag15) {
        mRespawn = 0;
        if (mTimer > 0 || mDesc->m2C <= 0) {
            if (--mTimer <= 0) {
                if (mChara->m1418 >= 30 || !mChara->IsFlag_unk(4)) {
                    mRespawn = 1;
                } else {
                    mTimer = 1;
                }
            }
        } else {
            mTimer = mDesc->m2C;
        }
    }
    if (mRespawn == 0) {
        return;
    }
    mRespawn = 0;
    int changed = 0;
    mDown = 0;
    mChara->mFlag15 = 0;
    mChara->mMotion.m40 = Float4Init_unk(1.0f);
    if (m54 == 3) {
        if (!HasCharaFlag_unk(mChara, 0x10000000)) {
            changed = 1;
            SetCharaFlag_unk(mChara, 0x10000000);
            mChara->mStatus.m1A8.m08 += 8.0f;
        }
    } else {
        if (HasCharaFlag_unk(mChara, 0x10000000)) {
            changed = 1;
            ClearCharaFlag_unk(mChara, 0x10000000);
            mChara->mStatus.m1A8.m08 -= 8.0f;
        }
    }
    if (lbl_8087F0A8->mD4 == 3 && mChara->fn_8017CB2C() != 0) {
        if (!HasCharaFlag_unk(mChara, 0x800000)) {
            mChara->m54C |= 0x800000;
            changed = 1;
            mChara->mStatus.m1A8.m08 += lbl_8087F0A8->mE4[lbl_8087F0A8->mD4].m00;
            mChara->mStatus.m1A8.m10 += 1000.0f;
            mChara->mStatus.m1A8.m14 += 1000.0f;
        }
    } else {
        if (HasCharaFlag_unk(mChara, 0x800000)) {
            mChara->m54C &= ~0x800000;
            changed = 1;
            mChara->mStatus.m1A8.m08 -= lbl_8087F0A8->mE4[lbl_8087F0A8->mD4].m00;
            mChara->mStatus.m1A8.m10 -= 1000.0f;
            mChara->mStatus.m1A8.m14 -= 1000.0f;
        }
    }
    if (changed) {
        mChara->mStatus.fn_8012B3E8();
        mChara->mStatus.fn_8012B988();
    }
    mChara->fn_80176ACC();
    mChara->fn_800D246C(0);
    if (IsFlag25_unk(mChara)) {
        mChara->mFlag25 = 0;
    }
    mChara->mStatus.fn_8012D8B8();
    mChara->mStatus.m224 = mChara->mStatus.mParamB.m98;
    mChara->m58C = 0;
    mChara->fn_8015495C(0, 1, 1, 1);
    mChara->mC58.fn_8011BE54();
    atn::Vector3 off(0.0f, 0.0f, mDesc->m40);
    Mtx rot;
    PSMTXRotRad(rot, 'y', mDesc->mRotY);
    PSMTXMultVec(rot, &off, &off);
    mChara->mPos = mDesc->mPos - off;
    mChara->mRot = atn::Vector3(0.0f, mDesc->mRotY, 0.0f);
    if (mDesc->m30 == 0) {
        mChara->vf80(m6C - off, off);
    } else if (mDesc->m30 == 2) {
        CharaTask* target;
        if (mDesc->m38 <= 0) {
            target = lbl_8087F8A0->mHead;
        } else {
            target = lbl_8087F8A0->Find_unk(mDesc->m38);
        }
        if (target != 0) {
            mChara->fn_801644D4(atn::Vector3(m6C), target);
        }
    } else if (mDesc->m30 == 3) {
        atn::Vector3 to = mDesc->mPos;
        atn::Vector3 from = mDesc->mPos;
        RouteTable_unk* table = lbl_8087F430->m10D8;
        RoutePlace_unk* place = 0;
        if (table != 0) {
            place = table->FindPlace_unk(mDesc->m38);
        }
        if (place != 0) {
            to = place->mPos;
        }
        off = from - to;
        mChara->vf80(to, off);
    } else {
        mChara->vf84(0x2D, 1.0f);
    }
    mChara->fn_80145334();
    mFrame++;
    if (lbl_8087F490 != 0 && m54 == 3) {
        fn_803E598C(lbl_8087F490, 1, 30, 0);
    }
    NotifyListeners_unk(fn_8014F8F4(), 10, this, mChara);
}

int ReinforcePoint::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    int type = msg->mType;
    switch (type) {
    case 1:
    case 2:
    case 3:
        m54 = type;
        if (mChara != 0) {
            if (type >= 2) {
                mChara->m54C |= 0x80000;
            } else {
                mChara->m54C &= ~0x80000;
            }
        }
        break;
    }
    return m54;
}

int ReinforcePoint::vf60() {
    int ret = 0;
    if (m54 >= 2 && mDesc->m7C != 0) {
        ret = 1;
    }
    return ret;
}

int ReinforcePoint::vf64(int arg) {
    switch (arg) {
    case 0:
        return mCount;
    case 9:
        return mChara != 0 ? mChara->m58 : 0;
    }
    return 0;
}

} // namespace gmk
