#include "chr/ControlCannonLift.h"
#include "game/CharaTask.h"
#include "game/GimmickCommon.h"
#include "game/EnemyManager.h"
#include "game/EnemyTask.h"
#include "gmk/Cannonball.h"
#include "atn/Memory.h"
#include <boost/bind.hpp>
#include "game/Input_unk.h"

// inferred: the cannon gimmick held in CharaTask::m1208 (class unknown, derived from GimmickCommon)
struct Cannon_unk : public GimmickCommon {
    unsigned char mF4[0x520 - 0xF4]; // inferred: the GimmickCommon members end at 0xF4 (its sizeof is padded to 8)
    unsigned char m520; // inferred: slot id (0xFF: none)
    int m524;           // inferred: arena slot request (-1 none, -2 local)
};

// inferred: arena mode object (lbl_8087F610, class unknown); slots of the cannons
struct ArenaSlot_unk {
    int m00;
    int m04;
};
struct Arena_unk;
extern "C" Arena_unk* lbl_8087F610;
extern "C" int fn_804F5E38(Arena_unk* arena, unsigned char id);
extern "C" ArenaSlot_unk* fn_804F5B08(Arena_unk* arena, unsigned char id);
extern "C" void fn_804F5C9C(Arena_unk* arena, unsigned char id);

// inferred: global object at lbl_8087F628 (class unknown); same accessor as in gmk/Item.cpp
extern "C" unsigned char fn_806B0E30();
extern "C" int fn_806A8E40();
struct ItemState_unk {
    unsigned char m000[0x1F8];
    unsigned int m1F8_31_20 : 12;
    unsigned int mDisabled : 1; // inferred
    unsigned int m1F8_18_0 : 19;
    unsigned char m1FC[0xC215 - 0x1FC];
    unsigned char mC215; // inferred: selects the source of the value

    // inferred: name invented
    unsigned char Get1_unk() {
        if (mC215 == 0) {
            return mDisabled ? 0 : fn_806B0E30();
        } else {
            return mDisabled ? 0 : fn_806A8E40();
        }
    }
};
extern "C" ItemState_unk* lbl_8087F628;

// inferred: pad state at +0x48C of the object behind lbl_8087F0A8 (class unknown)
struct Unk_8087F0A8 {
    unsigned char m000[0x48C];
    unsigned char mInput[1]; // 0x48C
};
extern "C" Unk_8087F0A8* lbl_8087F0A8;

// inferred: field object (lbl_8087F430) and its current map data (class unknown)
struct FieldMap_unk {
    unsigned char m00[0x4C];
    int m4C; // inferred: area
    int m50; // inferred: room
};
struct Field_unk;
extern "C" Field_unk* lbl_8087F430;
extern "C" FieldMap_unk* fn_80373148(Field_unk* field);

// inferred: the boss of the cannon battle (enemy id 4, chr Em208Queen); m1544 is set when a shot
// reaches it
struct Queen_unk : public EnemyTask {
    unsigned char m14B0[0x1544 - 0x14B0];
    int m1544;
};
extern "C" int fn_802CCA0C(EnemyTask* enemy); // inferred: nonzero while the queen can be hit


// inferred: offset of the point of the queen hit by the shots
static atn::Vector3 sQueenOffset_unk(3.882f, 98.679f, 10.846f);

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

// inferred: names invented
inline int GetEnabled_unk() {
    return fn_8014F8F4()->mEnabled;
}
inline void SetEnabled_unk(int enabled) {
    fn_8014F8F4()->mEnabled = enabled;
}

// inferred: position of the point hit by the shots (name invented)
inline atn::Vector3 GetQueenPoint_unk(Queen_unk* queen) {
    Mtx mtx;
    PSMTXRotRad(mtx, 'y', queen->mRot.y);
    atn::Vector3 offset(sQueenOffset_unk);
    PSMTXMultVec(mtx, &offset, &offset);
    return queen->mPos + offset;
}

// inferred: point `dist` in front of the character (name invented)
inline atn::Vector3 GetFrontDir_unk(const chr::ControlBase* control) {
    chr::Chara_unk* chara = control->mChara;
    atn::Vector3 dir(0.0f, 0.0f, 1.0f);
    Mtx mtx;
    PSMTXRotRad(mtx, 'y', chara->mRot.y);
    PSMTXMultVec(mtx, &dir, &dir);
    return dir;
}

// inferred: starts motion `id` on track 0 (name invented)
inline void PlayMotion_unk(chr::Motion_unk* m, int id, float speed) {
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->fn_80097C08(0, id, 0, 0, 1, 0.0f, 0.1f);
    m->mTracks[0].m0C = speed;
}

namespace chr {

ControlCannonLift::ControlCannonLift(Chara_unk* chara, GimmickCommon* cannon)
    : ControlBase(chara), mCannon(cannon), m0C(true), m0D(false), m0E(false) {
    chara->m560 = 0x20;
    Motion_unk* m = &mChara->mMotion;
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->fn_80097C08(0, 0x6C, 0, 0, 1, 0.0f, 0.1f);
    m->mTracks[0].m0C = 2.0f;
    mChara->fn_801446F0();
}

int ControlCannonLift::vf0C() {
    Motion_unk* motion = &mChara->mMotion;
    Chara_unk* chara = mChara;
    if (chara->m1208 == 0) {
        if (lbl_8087F610 != 0) {
            bool ok = false;
            bool fail = false;
            if (!m0D) {
                m0D = true;
                if (((Cannon_unk*)mCannon)->m520 == 0xFF) {
                    GimmickMsg_unk ev;
                    ev.mType = 2;
                    ev.m10 = (int)chara;
                    ev.m08 = -1;
                    NotifyListeners_unk(fn_8014F8F4(), 11, mCannon, &ev);
                    ((Cannon_unk*)mCannon)->m520 = lbl_8087F628->Get1_unk();
                } else {
                    fail = true;
                }
                m0E = true;
            } else {
                bool release = true;
                int id = ((Cannon_unk*)mCannon)->m524;
                if (id == -2) {
                    if (((Cannon_unk*)mCannon)->m520 == lbl_8087F628->Get1_unk()) {
                        ok = true;
                    } else {
                        fail = true;
                    }
                } else if (id != -1) {
                    int state = fn_804F5E38(lbl_8087F610, id);
                    if (state == 0) {
                        release = false;
                    } else if (state == 1) {
                        ArenaSlot_unk* slot = fn_804F5B08(lbl_8087F610, ((Cannon_unk*)mCannon)->m524);
                        if (slot != 0 && slot->m00 == mCannon->mId && slot->m04 == mCannon->m4C) {
                            ok = true;
                        } else {
                            fail = true;
                        }
                    } else {
                        fail = true;
                    }
                }
                if (release == true) {
                    fn_804F5C9C(lbl_8087F610, ((Cannon_unk*)mCannon)->m524);
                    m0E = false;
                }
            }
            if (fail == true) {
                m0C = false;
                mChara->m1208 = 0;
                return 1;
            }
            if (ok == true) {
                mChara->m1208 = (CharaObj1208_unk*)mCannon;
                GimmickMsg_unk msg;
                msg.mType = 2;
                msg.m10 = (int)mChara;
                mCannon->vf40(&msg);
            }
        } else if (motion->mTracks[0].m08 >= 15.0f && chara->m1208 == 0) {
            chara->m1208 = (CharaObj1208_unk*)mCannon;
            GimmickMsg_unk msg;
            msg.mType = 2;
            msg.m10 = (int)mChara;
            mCannon->vf40(&msg);
        }
    } else if (chara->mType == 0 && ((Input_unk*)lbl_8087F0A8->mInput)->fn_801240B4(0x20)) {
        m0C = true;
    } else {
        m0C = false;
    }
    float frame = motion->mTracks[0].m08;
    if (frame >= motion->fn_80097D7C(0) - 8.0f) {
        if (lbl_8087F610 != 0) {
            if (mChara->m1208 != 0) {
                return 1;
            }
        } else {
            return 1;
        }
    }
    return 0;
}

// NONMATCHING: 98.9% - register allocation of the inlined ControlCannonTarget constructor (the
// new object is in r28, ours r31) and the r3/r4 swap of the __VecCopy of m08.
boost::function<void()> ControlCannonLift::vf10() {
    if (m0C) {
        return boost::bind(&CharaTask::fn_80178208, (CharaTask*)mChara,
                           new ((MemoryTag)1, "", "", 0) ControlCannonTarget(mChara));
    }
    return ControlBase::vf10();
}

void ControlCannonLift::vf1C() {
    if (lbl_8087F610 != 0 && mChara->m1208 == 0) {
        if (m0E == true) {
            fn_804F5C9C(lbl_8087F610, ((Cannon_unk*)mCannon)->m524);
            m0E = false;
        }
        GimmickMsg_unk ev;
        ev.mType = 3;
        ev.m10 = (int)mChara;
        ev.m08 = -1;
        ev.mVec = mCannon->m6C;
        ev.mVec.y += 2.0f;
        int enabled = fn_8014F8F4()->mEnabled;
        fn_8014F8F4()->mEnabled = 1;
        NotifyListeners_unk(fn_8014F8F4(), 11, mCannon, &ev);
        fn_8014F8F4()->mEnabled = enabled;
    }
}

ControlCannonThrow::ControlCannonThrow(Chara_unk* chara, const atn::Vector3& dir, float speed)
    : ControlBase(chara), m08(dir), m14(speed) {
    chara->m560 = 0x22;
    Motion_unk* m = &mChara->mMotion;
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->fn_80097C08(0, 0x70, 0, 0, 1, 0.0f, 0.1f);
    m->mTracks[0].m0C = 1.0f;
    mChara->fn_801446F0();
}

// NONMATCHING: 99.9% - stack slots of the offset/point/difference vectors (the original puts the
// rotated offset above the point and the difference lowest).
int ControlCannonThrow::vf0C() {
    Motion_unk* motion = &mChara->mMotion;
    if (motion->mTracks[0].m08 >= 10.0f && mChara->m1208 != 0) {
        Queen_unk* queen;
        if (lbl_8087F430 != 0 && fn_80373148(lbl_8087F430) != 0 && fn_80373148(lbl_8087F430)->m4C == 11 &&
            fn_80373148(lbl_8087F430)->m50 == 3 && (queen = (Queen_unk*)EnemyManager::sInstance->Find_unk(4)) != 0 &&
            fn_802CCA0C(queen)) {
            Mtx mtx;
            PSMTXRotRad(mtx, 'y', queen->mRot.y);
            atn::Vector3 offset(sQueenOffset_unk);
            PSMTXMultVec(mtx, &offset, &offset);
            atn::Vector3 point = queen->mPos + offset;
            atn::Vector3 diff = point - m08;
            if (PSVECMag(&diff) < m14) {
                GimmickMsg_unk msg;
                msg.mType = 6;
                msg.mVec = point;
                msg.m10 = (int)mChara;
                msg.m04 = 0x32;
                ((GimmickCommon*)mChara->m1208)->vf40(&msg);
                mChara->m1208 = 0;
                return 0;
            }
        }
        GimmickMsg_unk msg;
        msg.mType = 5;
        msg.m10 = (int)mChara;
        msg.mVec = m08;
        ((Cannon_unk*)mChara->m1208)->m520 = 0xFF;
        ((GimmickCommon*)mChara->m1208)->vf40(&msg);
        mChara->m1208 = 0;
    } else {
        float frame = motion->mTracks[0].m08;
        if (frame >= motion->fn_80097D7C(0)) {
            return 1;
        }
    }
    Queen_unk* queen2;
    if (lbl_8087F430 != 0 && fn_80373148(lbl_8087F430) != 0 && fn_80373148(lbl_8087F430)->m4C == 11 &&
            fn_80373148(lbl_8087F430)->m50 == 3 && (queen2 = (Queen_unk*)EnemyManager::sInstance->Find_unk(4)) != 0 &&
        fn_802CCA0C(queen2)) {
        Mtx mtx;
        PSMTXRotRad(mtx, 'y', queen2->mRot.y);
        atn::Vector3 offset(sQueenOffset_unk);
        PSMTXMultVec(mtx, &offset, &offset);
        atn::Vector3 point = queen2->mPos + offset;
        atn::Vector3 diff = point - m08;
        if (PSVECMag(&diff) < m14) {
            queen2->m1544 = 1;
        }
    }
    return 0;
}

// NONMATCHING: 99.6% - r3/r4 swapped in the __VecCopy of the front point (documented __VecCopy
// blocker).
ControlCannonTarget::ControlCannonTarget(Chara_unk* chara) : ControlMoveCursor(chara), m2C(0) {
    Chara_unk* c = mChara;
    c->m560 = 0x21;
    PlayMotion_unk(&mChara->mMotion, 0x6F, 1.0f);
    m08 = mChara->mPos + GetFrontDir_unk(this) * 30.0f;
    m28 = ((gmk::Cannonball*)mChara->m1208)->fn_803FB830();
    SetAim_unk();
}

// NONMATCHING: 99.9% - stack slots of the offset/point/difference vectors of the second queen test.
int ControlCannonTarget::vf0C() {
    int ret = 0;
    if (mChara->mType == 0) {
        int found = 0;
        CursorTarget_unk target;
        Queen_unk* queen;
        if (lbl_8087F430 != 0 && fn_80373148(lbl_8087F430) != 0 && fn_80373148(lbl_8087F430)->m4C == 11 &&
            fn_80373148(lbl_8087F430)->m50 == 3 && (queen = (Queen_unk*)EnemyManager::sInstance->Find_unk(4)) != 0 &&
            fn_802CCA0C(queen)) {
            target.mPos = queen->mPos;
            target.mRadius = m28;
            target.mHeight = sQueenOffset_unk.y;
            found = true;
        }
        if (found) {
            fn_801C3458(&target, 30.0f, 450.0f, 6.2831855f);
        } else {
            fn_801C3458(0, 30.0f, 450.0f, 6.2831855f);
        }
        Queen_unk* queen2;
        if (lbl_8087F430 != 0 && fn_80373148(lbl_8087F430) != 0 && fn_80373148(lbl_8087F430)->m4C == 11 &&
            fn_80373148(lbl_8087F430)->m50 == 3 && (queen2 = (Queen_unk*)EnemyManager::sInstance->Find_unk(4)) != 0 &&
            fn_802CCA0C(queen2)) {
            Mtx mtx;
            PSMTXRotRad(mtx, 'y', queen2->mRot.y);
            atn::Vector3 offset(sQueenOffset_unk);
            PSMTXMultVec(mtx, &offset, &offset);
            atn::Vector3 point = queen2->mPos + offset;
            atn::Vector3 diff = point - m08;
            if (PSVECMag(&diff) < m28) {
                queen2->m1544 = 1;
            }
        }
        if (((Input_unk*)lbl_8087F0A8->mInput)->fn_801231D0(0x21)) {
            m2C = 1;
            ret = 1;
        } else if (((Input_unk*)lbl_8087F0A8->mInput)->fn_801231D0(0x1F)) {
            mChara->fn_801750FC();
            ret = 1;
        }
    }
    SetAim_unk();
    return ret;
}

boost::function<void()> ControlCannonTarget::vf10() {
    if (m2C != 0) {
        return boost::bind(&CharaTask::fn_8017518C, (CharaTask*)mChara);
    }
    return ControlBase::vf10();
}

} // namespace chr
