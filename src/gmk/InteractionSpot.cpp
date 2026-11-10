#include "gmk/InteractionSpot.h"
#include "game/GameScene.h"
#include "atn/Memory.h"
#include "chr/ControlBase.h"
#include "game/NpcManager.h"
#include "game/NpcTask.h"
#include "atn/RenderManager.h"

// inferred: route between two points (8 bytes; declared in game/CharaStatus.h)
struct RoutePath_unk {
    int m00;
    int m04;
};
// inferred: point of the field (0x30 bytes)
struct Spot_unk {
    int mId;
    atn::Vector3 mPos; // 0x04
    int m10;           // inferred: index (1-based) used by the links
    unsigned char m14[0x30 - 0x14];
};
// inferred: two linked points (indices into the point table)
struct SpotLink_unk {
    int mA;
    int mB;
};
// inferred: point tables of the field (lbl_8087F430->m10D8)
struct SpotTable_unk {
    Spot_unk* FindSpot_unk(int id) {
        for (unsigned int i = 0; i < mNumSpots; i++) {
            if (mSpots[i].mId == id) {
                return &mSpots[i];
            }
        }
        return 0;
    }
    Spot_unk* FindLinkA_unk(Spot_unk* spot) {
        for (unsigned int i = 0; i < mNumLinks; i++) {
            SpotLink_unk& link = mLinks[i];
            if (spot->m10 - 1 == link.mA) {
                return &mSpots[link.mB];
            }
        }
        return 0;
    }
    Spot_unk* FindLinkB_unk(Spot_unk* spot) {
        for (unsigned int i = 0; i < mNumLinks; i++) {
            SpotLink_unk& link = mLinks[i];
            if (spot->m10 - 1 == link.mB) {
                return &mSpots[link.mA];
            }
        }
        return 0;
    }

    unsigned char m00[0xA4];
    RoutePath_unk* mPaths;  // 0xA4: inferred: as RouteTable_unk::mPaths (game/CharaStatus.h)
    unsigned char mA8[0xC4 - 0xA8];
    unsigned int mNumSpots; // 0xC4
    Spot_unk* mSpots;       // 0xC8
    unsigned int mNumLinks; // 0xCC
    SpotLink_unk* mLinks;   // 0xD0
};
struct Field_unk {
    unsigned char m00[0x10D8];
    SpotTable_unk* m10D8;
};
extern "C" Field_unk* lbl_8087F430;
extern "C" unsigned short fn_803CD958(RoutePath_unk* path, int point); // inferred: link between two points
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);

// inferred: link between two route points (name invented; as in gmk/MeetingSpot.cpp)
static inline int GetLink_unk(SpotTable_unk* table, int a, int b) {
    return fn_803CD958(&table->mPaths[a - 1], b - 1);
}
// inferred: squared length (name invented; as in gmk/MeetingSpot.cpp)
static inline float SqLength_unk(const atn::Vector3& v) {
    return PSVECSquareMag(&v);
}

// inferred: index of the spot nearer to `chara` (name invented)
static inline int NearSpot_unk(Spot_unk* const* spots, CharaTask* chara) {
    float d0 = SqLength_unk(spots[0]->mPos - chara->mPos);
    float d1 = SqLength_unk(spots[1]->mPos - chara->mPos);
    return !(d0 <= d1);
}
// inferred: one of the four character ids of the descriptor matches (0 in the first one: any); as
// gmk::MeetingSpot::MatchId_unk (name invented)
static inline bool MatchId_unk(gmk::InteractionSpot* spot, CharaTask* chara) {
    unsigned int i;
    bool found = false;
    for (i = 4; i < 8; i++) {
        int id = (&spot->mDesc->m20)[i];
        if (id == chara->m50) {
            found = true;
            break;
        }
        if (id == 0 && i == 4) {
            found = true;
            break;
        }
    }
    return found;
}

namespace gmk {

GimmickCommon* InteractionSpot::fn_80428B0C(atn::Task* parent, GimmickDesc_unk* desc) {
    if (parent != 0) {
        return new ((MemoryTag)12, "", "", 0) InteractionSpot(parent, desc);
    }
    return 0;
}

int InteractionSpot::vf0C() {
    if (fn_803EC91C() == 0) {
        m54 = 2;
        return 1;
    }
    return 0;
}

// NONMATCHING: register allocation only (the target keeps the table in r7 and returns the spot in
// the register of the id). Tried: table/field/id locals, reference and reversed comparisons.
void InteractionSpot::Start() {
    m6C = mDesc->mPos;
    m78.y = mDesc->mRotY;
    SpotTable_unk* table = lbl_8087F430->m10D8;
    mSpot[0] = table->FindSpot_unk(mDesc->m20);
    mSpot[1] = table->FindLinkA_unk(mSpot[0]);
    if (mSpot[1] == 0) {
        mSpot[1] = table->FindLinkB_unk(mSpot[0]);
    }
    m10C = ((GameScene*)table)->fn_803C1560(&mSpot[0]->mPos, 0, 0, 0, 1, -1.0f);
    fn_8042952C(0);
}


void InteractionSpot::vf10() {
    int cancel = vf60() == 0;
    if (!cancel) {
        cancel = SqLength_unk(atn::RenderManager::instance_->mCamera.mPos - m6C) > 250000.0f;
    }
    if (cancel) {
        if (mState != 0) {
            fn_8042952C(11);
        }
        return;
    }
    switch (mState) {
    case 0:
        mTimer++;
        if (mDesc->m7C != 0 && mTimer > mDesc->m24) {
            fn_8042952C(1);
        }
        break;
    case 1:
        if (mChara[0] == 0) {
            mChara[0] = fn_8042917C(0);
        }
        if (mChara[0] != 0) {
            mSide = NearSpot_unk(mSpot, mChara[0]);
            mChara[0]->fn_80170F20(&mSpot[mSide]->mPos, 0x48, m78.y, 3.0f);
            mChara[0]->mC58.mHeld_unk = 1;
            fn_8042952C(2);
        }
        break;
    case 2: {
        CharaTask* chara = mChara[0];
        if (chara->m55C == 6) {
            fn_8042952C(11);
        } else if (chara->mRoute.m2C != 0) {
            mSide = !mSide;
            chara->fn_80170F20(&mSpot[mSide]->mPos, 0x48, m78.y, 3.0f);
            fn_8042952C(3);
        } else {
            int side = NearSpot_unk(mSpot, chara);
            if (mSide != side) {
                mSide = side;
                mChara[0]->fn_80170F20(&mSpot[mSide]->mPos, 0x48, m78.y, 3.0f);
            }
        }
        break;
    }
    case 3:
        if (mChara[0]->mRoute.m2C != 0 || mChara[0]->m55C == 6) {
            fn_8042952C(11);
        }
        break;
    }
}

// inferred: picks the farthest free NPC (between 50 and 150 units) that can walk to the nearer spot
// and faces it
CharaTask* InteractionSpot::fn_8042917C(CharaTask* other) {
    float bestDist = 2500.0f;
    CharaTask* best = 0;
    SpotTable_unk* table = lbl_8087F430->m10D8;
    for (NpcTask* npc = NpcManager::sInstance->mHead; npc != 0; npc = npc->mNext) {
        if (npc == other) {
            continue;
        }
        if (other != 0 && npc->m50 == other->m50) {
            continue;
        }
        if (npc == mPrev[0] || npc == mPrev[1]) {
            continue;
        }
        if (npc->mC58.mHeld_unk) {
            continue;
        }
        if (npc->m55C != 2) {
            continue;
        }
        unsigned int flags = npc->mFlags;
        bool active = false;
        if ((flags & 4) != 4 && (flags & 1) != 1) {
            active = true;
        }
        if (!active) {
            continue;
        }
        if ((flags & 0xDEAD0000) == 0xDEAD0000) {
            continue;
        }
        if (!MatchId_unk(this, npc)) {
            continue;
        }
        if (npc->mC58.m0C.mDestPoint > 0 &&
            GetLink_unk(table, npc->mC58.m0C.mDestPoint, m10C) > 0) {
            atn::Vector3 d = mSpot[NearSpot_unk(mSpot, npc)]->mPos - npc->mPos;
            float dist = SqLength_unk(d);
            if (dist < 22500.0f && dist > bestDist) {
                PSVECNormalize(&d, &d);
                atn::Vector3 front(0.0f, 0.0f, 1.0f);
                Mtx m;
                PSMTXRotRad(m, 'y', npc->mRot.y);
                PSMTXMultVec(m, &front, &front);
                if (PSVECDotProduct(&d, &front) > 0.70710677f) {
                    bestDist = dist;
                    best = npc;
                }
            }
        }
    }
    return best;
}

void InteractionSpot::fn_8042952C(int state) {
    mState = state;
    mTimer = 0;
    if (state == 11) {
        for (int i = 0; i < 2; i++) {
            if (mChara[i] != 0) {
                ((chr::Chara_unk*)mChara[i])->fn_8015495C(0, 1, 1, 1);
                if (((chr::Chara_unk*)mChara[i])->m55C == 6) {
                    if (mChara[i]->mType == 0) {
                        ((chr::Chara_unk*)mChara[i])->m564 = (CharaTask::CONTROL_MODE)1;
                    } else {
                        ((chr::Chara_unk*)mChara[i])->m564 = (CharaTask::CONTROL_MODE)2;
                    }
                } else {
                    mChara[i]->fn_8016EB48(0, 0, 0);
                }
                ((chr::Chara_unk*)mChara[i])->mC58.mHeld_unk = 0;
            }
            mPrev[i] = mChara[i];
            mChara[i] = 0;
        }
        fn_8042952C(0);
    }
}

void InteractionSpot::vf14() {}

int InteractionSpot::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    switch (msg->mType) {
    case 1:
    case 2:
        m54 = msg->mType;
        break;
    }
    return m54;
}

int InteractionSpot::vf60() {
    int ret = 0;
    if (m54 == 2 && mDesc->m7C != 0) {
        ret = 1;
    }
    return ret;
}

} // namespace gmk
