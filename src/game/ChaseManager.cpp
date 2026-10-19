#include "game/ChaseManager.h"
#include "game/CharaTask.h"
#include "game/CharaManager.h"
#include "game/PlayerTask.h"
#include "atn/Memory.h"

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
    unsigned char m0000[0x10D8];
    ChaseGoalTable_unk* m10D8;
    unsigned char m10DC[0x5660 - 0x10DC];
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
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length

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
        lbl_8087F098 = new (3, "", "", 0) ChaseManager(parent);
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
    mFC[0].fn_80237654("ef_ui009a.eff");
    m114[0].fn_80237654("ef_ui010b.eff");
    m12C[0].fn_80237654("ef_ui010c.eff");
    mFC[1].fn_80237654("ef_ui009a.eff");
    m114[1].fn_80237654("ef_ui009b.eff");
    m12C[1].fn_80237654("ef_ui010c.eff");
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
        target->fn_801598B4((int)guard, lbl_8087F0A8->m524, lbl_8087F0A8->m528);
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
    if (mFC[0].fn_802376D0() == 0 && m114[0].fn_802376D0() == 0 && mFC[1].fn_802376D0() == 0 &&
        m114[1].fn_802376D0() == 0 && m12C[0].fn_802376D0() == 0 && m12C[1].fn_802376D0() == 0) {
        ret = 1;
    }
    return ret;
}

// NONMATCHING: vf10 (0x801829B4, 0xBD0 bytes: chase update and effects) is not written yet

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

// NONMATCHING: 99.4% - registers of the inlined goal search loop differ (table/offset/index); 5 variants tried
void ChaseManager::fn_80183618(int id) {
    RemoveMarker_unk();
    ChaseGoal_unk* goal = lbl_8087F430->m10D8->Find_unk(id);
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
        float dist = fn_805F9920(&d);
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
