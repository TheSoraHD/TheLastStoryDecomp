#include "game/ChaseManager.h"
#include "game/CharaTask.h"
#include "game/CharaManager.h"
#include "game/PlayerTask.h"
#include "atn/Memory.h"
#include "game/EnemyManager.h"
#include "game/EnemyTask.h"
#include "game/ChaserGuard.h"

extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
extern "C" void* lbl_8087F418;
extern "C" void fn_80360780(void* obj, int a, int b, int c); // inferred: changes the BGM
struct ChaseEffectMgr_unk;
extern "C" ChaseEffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_80239DAC(ChaseEffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
// inferred: camera settings (only the members used here)
struct ChaseSettings_unk {
    unsigned char m000[0x524];
    int m524;
    int m528;
    atn::Vector3 m52C; // inferred: camera offset
    float m538;
};
extern "C" ChaseSettings_unk* lbl_8087F0A8;
// inferred: goal points of the field (0x28 bytes each)
struct ChaseGoal_unk {
    int mId;
    atn::Vector3 mPos;
    unsigned char m10[0x28 - 0x10];
};
struct ChaseGoalTable_unk {
    ChaseGoal_unk* Find_unk(int id);
    unsigned char m00[0x78];
    unsigned int mCount;
    ChaseGoal_unk* mGoals;
};
struct ChaseField_unk {
    unsigned char m0000[0x8A0];
    CharaTask* m8A0; // inferred: caught character (cleared every frame while chasing)
    unsigned char m08A4[0x10D8 - 0x8A4];
    ChaseGoalTable_unk* m10D8;
    unsigned char m10DC[0x54E4 - 0x10DC];
    int m54E4;
    unsigned char m54E8[0x5660 - 0x54E8];
    int m5660;
};
extern "C" ChaseField_unk* lbl_8087F430;
struct ChaseTown_unk {
    unsigned char m00[0x258];
    void* m258; // inferred: minimap
};
extern "C" ChaseTown_unk* lbl_8087FA20;
struct ChaseColor_unk {
    ChaseColor_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r, g, b, a;
};
extern "C" int fn_804AABA8(void* minimap, const atn::Vector3* pos, const ChaseColor_unk* color, float scale);
extern "C" void fn_804AAD54(void* minimap, int marker);
extern "C" float PSVECSquareMag(const atn::Vector3* v); // inferred: squared length

ChaseManager* lbl_8087F098;

// inferred: player of the player manager (name invented)
inline CharaTask* GetPlayer_unk() {
    if (lbl_8087F8A0 != 0) {
        return (CharaTask*)lbl_8087F8A0->mHead;
    }
    return 0;
}

// inferred: minimap of the town (name invented)
inline void* GetMiniMap_unk() {
    if (lbl_8087FA20 != 0) {
        return lbl_8087FA20->m258;
    }
    return 0;
}

ChaseManager* ChaseManager::fn_801823A8(atn::Task* parent) {
    if (parent != 0 && lbl_8087F098 == 0) {
        lbl_8087F098 = new ((MemoryTag)3, "", "", 0) ChaseManager(parent);
    }
    return lbl_8087F098;
}

inline void ChaseManager::SetEffect_unk(int mode) {
    if (mEffect != 0) {
        fn_80239DAC(lbl_8087F3C0, this, 0, 0);
    }
    mEffect = mode;
    switch (mEffect) {
    case 0:
        lbl_8087F0A8->m52C.Set_unk(0.0f, 11.5f, 8.0f);
        lbl_8087F0A8->m538 = 0.0f;
        break;
    case 1:
        lbl_8087F0A8->m52C.Set_unk(0.0f, 30.0f, 0.0f);
        lbl_8087F0A8->m538 = 0.0f;
        break;
    }
}

ChaseManager::ChaseManager(atn::Task* parent)
    : atn::Task(parent), mState(0), m4C(0), m50(0), m54(0), mFrames(0), mGoalId(0), mGoal(0.0f, 0.0f, 0.0f),
      mCatcher(0), mCaught(0), m1A4(1.0f), m1A8(1.0f), m1AC(1.0f), m1B0(1.0f), m1B4(-1.0f), mShowGoal(0),
      mEffect(0), mMarker(-1) {
    mFC[0].LoadRequest("ef_ui009a.eff");
    m114[0].LoadRequest("ef_ui010b.eff");
    m12C[0].LoadRequest("ef_ui010c.eff");
    mFC[1].LoadRequest("ef_ui009a.eff");
    m114[1].LoadRequest("ef_ui009b.eff");
    m12C[1].LoadRequest("ef_ui010c.eff");
    SetEffect_unk(0);
}

ChaseManager::~ChaseManager() {
    lbl_8087F098 = 0;
    if (lbl_8087F8A0 != 0 && lbl_8087F8A0->mHead != 0) {
        ((CharaTask*)lbl_8087F8A0->mHead)->fn_8014C228();
    }
    if (lbl_8087F418 != 0) {
        fn_80360780(lbl_8087F418, 1, 8, 0x3C);
    }
}

void ChaseManager::fn_801826E0() {
    fn_80182874(this);
    mFrames = 0;
    for (unsigned int i = 0; i < mCharas.mSize; i++) {
        if (mCharas.mData[i] != (CharaTask*)lbl_8087F8A0->mHead) {
            mCharas.mData[i]->fn_8015A184();
        }
    }
    if (lbl_8087F418 != 0) {
        fn_80360780(lbl_8087F418, 0, 8, 0x3C);
    }
}

extern "C" void fn_80182788(ChaseManager* mgr, ChaserGuard* guard, CharaTask* target) {
    if (mgr->mCatcher == 0) {
        mgr->mCatcher = guard;
        mgr->m6C = 0;
        mgr->mState = 2;
        mgr->mCaught = target;
        target->fn_801598B4(guard, lbl_8087F0A8->m524, lbl_8087F0A8->m528);
        lbl_8087F430->m5660 = 1;
        for (unsigned int i = 0; i < mgr->mCharas.mSize; i++) {
            if (mgr->mCharas.mData[i] != mgr->mCaught) {
                mgr->mCharas.mData[i]->fn_80159E9C();
            }
        }
    }
    if (mgr->mGuards.size() < 32) {
        mgr->mGuards.push_back(guard);
    }
}

extern "C" void fn_80182874(ChaseManager* mgr) {
    mgr->mCatcher = 0;
    mgr->mCaught = 0;
    mgr->mState = 1;
    mgr->mGuards.mSize = 0;
}

void ChaseManager::fn_80182890() {
    if (mCaught != 0) {
        mCaught->mF1C = 0;
    }
    mCatcher = 0;
    mCaught = 0;
    mState = 0;
    m6C = 0;
    mGuards.mSize = 0;
    mFrames = 0;
    mCharas.mSize = 0;
    if (lbl_8087F8A0 != 0 && lbl_8087F8A0->mHead != 0) {
        ((CharaTask*)lbl_8087F8A0->mHead)->fn_8014C228();
    }
    if (lbl_8087F418 != 0) {
        fn_80360780(lbl_8087F418, 1, 8, 0x3C);
    }
}

int ChaseManager::vf0C() {
    int ret = 0;
    if (mFC[0].IsLoading() == 0 && m114[0].IsLoading() == 0 && mFC[1].IsLoading() == 0 &&
        m114[1].IsLoading() == 0 && m12C[0].IsLoading() == 0 && m12C[1].IsLoading() == 0) {
        ret = 1;
    }
    return ret;
}

extern "C" int fn_80239D58(ChaseEffectMgr_unk* mgr, void* owner, int id); // inferred: the effect is playing
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
extern "C" void fn_8023A098(ChaseEffectMgr_unk* mgr, void* owner, int arg2, int arg3);
extern "C" void fn_8023A680(ChaseEffectMgr_unk* mgr, atn::EffFile* res, int a, int b, Mtx* mtx, int c, int d,
                            const float* color, float scale, int e, int f, int g); // inferred: starts an effect
extern "C" void PSMTXRotAxisRad(Mtx m, const atn::Vector3* axis, float rad);
extern "C" void PSMTXTrans(Mtx m, float x, float y, float z);
extern "C" void PSMTXMultVec(const Mtx m, const atn::Vector3* src, atn::Vector3* dst);
extern "C" double sin(double x);
// inferred: camera (only the members used here)
struct ChaseCamera_unk {
    unsigned char m000[0x10C];
    atn::Vector3 mEye;    // 0x10C
    atn::Vector3 mTarget; // 0x118
};
extern "C" ChaseCamera_unk* lbl_8087EFB4;

// inferred: rotate `m` around `axis` when the angle is not zero (the constant is compared first)
inline void ChaseRotate_unk(Mtx m, char axis, float rad) {
    if (0.0f != rad) {
        Mtx r;
        PSMTXRotRad(r, axis, rad);
        Mtx t;
        PSMTXConcat(m, r, t);
        __MtxCopy(m, t);
    }
}
// inferred: m = m * rotation (z, y, then x), skipping zero angles (name invented)
inline void __MtxRotateZYX_unk(Mtx m, const atn::Vector3& r) {
    ChaseRotate_unk(m, 'z', r.z);
    ChaseRotate_unk(m, 'y', r.y);
    ChaseRotate_unk(m, 'x', r.x);
}
// inferred: holding a ledge (name invented)
inline bool IsLedgeHold_unk(CharaTask* chara) {
    return chara->mFlag31 && chara->mC48 != 0;
}

// NONMATCHING: 97.4% - the switch on the guard state tests 1..2 with two signed compares in the original (a
// range check here), one more callee-saved register is used here, stack/load order of the locals.
// inferred: counts the guards by state, then places the goal marker (two effects) in front of the player
void ChaseManager::vf10() {
    if (mState != 0 && lbl_8087F8A0 != 0 && lbl_8087F8A0->mHead != 0) {
        mFrames++;
        lbl_8087F430->m8A0 = 0;
        if (lbl_8087F8A0->mHead->m484[3] != 0x1F) {
            lbl_8087F8A0->mHead->fn_8014C0B4(3, 0x1F);
            lbl_8087F8A0->mHead->fn_8014C0B4(2, 0x19);
        }
        m54 = 0;
        m50 = 0;
        m4C = 0;
        for (EnemyTask* enemy = EnemyManager::sInstance->mHead; enemy != 0; enemy = enemy->mNext) {
            if (enemy->CanAct_unk() && enemy->m60->m24 == 0x35) {
                switch (((ChaserGuard*)enemy)->m14B0) {
                case 3:
                    m54++;
                    m50++;
                    break;
                case 1:
                case 2:
                    m50++;
                    break;
                case 4:
                    m4C++;
                    break;
                }
            }
        }
    }
    if (mState == 2) {
        m6C++;
        lbl_8087F430->m8A0 = mCaught;
    }
    CharaTask* player = lbl_8087F8A0->mHead;
    if (mShowGoal != 0 && lbl_8087F430 != 0 && lbl_8087F430->m54E4 == 0 && mGoalId > 0 && !player->mFlag25 &&
        !IsLedgeHold_unk(player)) {
        atn::Vector3 pos = player->mPos;
        atn::Vector3 dir = mGoal - pos;
        float dist = PSVECMag(&dir);
        PSVECNormalize(&dir, &dir);
        if (mEffect == 1) {
            dir.y = 0.0f;
        }
        atn::Vector3 offset = lbl_8087F0A8->m52C;
        atn::Vector3 rot = dir.ToRotation_unk();
        Mtx rotY;
        PSMTXRotRad(rotY, 'y', rot.y);
        PSMTXMultVec(rotY, &offset, &offset);
        pos += offset;
        atn::Vector3 angle = dir.ToRotation_unk();
        if (mEffect == 0) {
            float s = sin(atn::DegToRad_unk(3.0f * m1B4));
            pos.y += 0.5f * s;
            pos.y += 5.0f * -(float)sin(angle.x);
            if (player->mFlag28) {
                pos.y += 5.0f;
            }
        }
        atn::Vector3 view;
        view = lbl_8087EFB4->mTarget - lbl_8087EFB4->mEye;
        PSVECNormalize(&view, &view);
        atn::Vector3 up(0.0f, 1.0f, 0.0f);
        atn::Vector3 axis;
        PSVECCrossProduct(&view, &up, &axis);
        Mtx tilt;
        PSMTXRotAxisRad(tilt, &axis, lbl_8087F0A8->m538);
        Mtx trans;
        PSMTXTrans(trans, pos.x, pos.y, pos.z);
        Mtx t;
        PSMTXConcat(trans, tilt, t);
        __MtxCopy(m144, t);
        atn::Matrix3x4 m;
        m.SetIdentity();
        __MtxRotateZYX_unk(m.m, angle);
        Mtx trans2;
        PSMTXTrans(trans2, pos.x, pos.y, pos.z);
        Mtx t2;
        PSMTXConcat(tilt, m.m, t2);
        Mtx t3;
        PSMTXConcat(trans2, t2, t3);
        __MtxCopy(m174, t3);
        fn_8023A098(lbl_8087F3C0, this, 0, 2);
        if (dist < 15.0f) {
            if (m1B4 > 0.5f) {
                if (fn_80239D58(lbl_8087F3C0, this, 0) != 0) {
                    fn_80239DAC(lbl_8087F3C0, this, 0, 0);
                }
                fn_80232B7C(0, 0);
                fn_8023A680(lbl_8087F3C0, &m12C[mEffect], -1, 5, &m174, 0, 0, 0, 1.0f, 0, -1, 1);
                m1B4 = 0.0f;
            }
        } else if (fn_80239D58(lbl_8087F3C0, this, 0) == 0) {
            fn_80232B7C(this, 0);
            if (mEffect != 0) {
                fn_8023A680(lbl_8087F3C0, &mFC[mEffect], -1, 5, &m144, 0, 0, 0, 1.0f, 0, -1, 1);
            }
            fn_8023A680(lbl_8087F3C0, &m114[mEffect], -1, 5, &m174, 0, 0, 0, 1.0f, 0, -1, 1);
            m1B4 = 1.0f;
        } else {
            m1B4 += 1.0f;
        }
    } else if (fn_80239D58(lbl_8087F3C0, this, 0) != 0) {
        fn_80239DAC(lbl_8087F3C0, this, 0, 0);
    }
}

void ChaseManager::fn_80183588(int id) {
    if (id == 0) {
        CharaTask* player = (CharaTask*)lbl_8087F8A0->mHead;
        mCharas.push_back(player);
    } else {
        CharaTask* chara = (CharaTask*)lbl_8087F8A0->Find_unk(id);
        mCharas.push_back(chara);
        chara->fn_8015A184();
    }
}

inline void ChaseManager::RemoveMarker_unk() {
    void* minimap = GetMiniMap_unk();
    if (minimap != 0 && mMarker >= 0) {
        fn_804AAD54(minimap, mMarker);
    }
    mMarker = -1;
}

inline void ChaseManager::AddMarker_unk() {
    void* minimap = GetMiniMap_unk();
    if (minimap != 0 && mGoalId > 0) {
        atn::Vector3 pos = mGoal;
        ChaseColor_unk color(1.0f, 0.7843138f, 0.0f, 1.0f);
        mMarker = fn_804AABA8(minimap, &pos, &color, 1.0f);
    }
}

// inferred: goal point `id` of the field (name invented)
inline ChaseGoal_unk* ChaseGoalTable_unk::Find_unk(int id) {
    for (unsigned int i = 0; i < mCount; i++) {
        if (id == mGoals[i].mId) {
            return &mGoals[i];
        }
    }
    return 0;
}

// NONMATCHING: 99.6% - registers of the inlined goal search loop differ (offset/table swapped); tried
// Find_unk forms (pointer sum, result variable, local data pointer), free helpers, declaration orders
void ChaseManager::fn_80183618(int id) {
    RemoveMarker_unk();
    ChaseGoalTable_unk* table = lbl_8087F430->m10D8;
    ChaseGoal_unk* goal = table->Find_unk(id);
    if (goal != 0) {
        mGoalId = id;
        mGoal = goal->mPos;
        if (mShowGoal != 0) {
            AddMarker_unk();
        }
    } else {
        mGoalId = 0;
    }
}

void ChaseManager::fn_80183774(int show) {
    mShowGoal = show;
    if (show != 0 && mGoalId > 0) {
        AddMarker_unk();
    } else {
        RemoveMarker_unk();
    }
}

extern "C" CharaTask* fn_80183860(ChaseManager* mgr, const atn::Vector3* pos) {
    if (mgr->mCaught != 0) {
        return mgr->mCaught;
    }
    float best = 1.0e10f;
    CharaTask* ret = 0;
    for (unsigned int i = 0; i < mgr->mCharas.mSize; i++) {
        CharaTask* chara = mgr->mCharas.mData[i];
        atn::Vector3 d(pos->x - chara->mPos.x, pos->y - chara->mPos.y, pos->z - chara->mPos.z);
        float dist = PSVECSquareMag(&d);
        if (dist < best) {
            ret = chara;
            best = dist;
        }
    }
    return ret;
}

void ChaseManager::fn_80183930() {
    if (mState != 0) {
        mState = 3;
    }
}

// NONMATCHING: 99.2% - the inlined fn_801826E0 puts 0/1 in r0/r4 swapped; tried a direct body, an early return and a
// shared inline helper
void ChaseManager::fn_80183948() {
    if (mState != 0) {
        fn_801826E0();
    }
}
