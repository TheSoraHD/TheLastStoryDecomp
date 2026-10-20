#include "gmk/MeetingSpot.h"
#include "atn/Memory.h"
#include "game/CharaManager.h"
#include "game/NpcManager.h"
#include "game/NpcTask.h"

// inferred: route between two points (8 bytes; declared in game/CharaStatus.h)
struct RoutePath_unk {
    int m00;
    int m04;
};
struct Field_unk {
    unsigned char m00[0x10D8];
    RouteTable_unk* m10D8;
};
extern "C" Field_unk* lbl_8087F430;
extern "C" int fn_803C1560(RouteTable_unk* table, const atn::Vector3* pos, int arg2, int arg3, int arg4, int arg5,
                           float arg6);
extern "C" unsigned short fn_803CD958(RoutePath_unk* path, int point); // inferred: link between two points
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
// inferred: camera (only the position)
struct CameraPos_unk {
    unsigned char m00[0x10C];
    atn::Vector3 mPos; // 0x10C
};
extern "C" CameraPos_unk* lbl_8087EFB4;
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager

// inferred: link between two route points (name invented)
static inline int GetLink_unk(int a, int b) {
    return fn_803CD958(&lbl_8087F430->m10D8->mPaths[a - 1], b - 1);
}

static inline float SqLength_unk(const atn::Vector3& v) {
    return fn_805F9920(&v);
}

// inferred: frame of motion track 0 (name invented)
static inline float Frame_unk(Model_unk* motion) {
    return motion->mTracks[0].m08;
}
// inferred: squared length on the xz plane (name invented)
static inline float LengthXZSq_unk(const atn::Vector3& v) {
    return v.x * v.x + v.z * v.z;
}
// inferred: sends the character to a position (name invented)
static inline void MoveTo_unk(CharaTask* chara, const atn::Vector3& pos, float rot) {
    chara->fn_80170F20(&pos, 0, rot, -1.0f);
}

namespace gmk {

// inferred: plays a motion on character i (name invented)
void MeetingSpot::PlayMotion_unk(int i, int motion, int loop, float blend) {
    mChara[i]->fn_8016E970(9);
    mChara[i]->mMotion.m34C = 1;
    mChara[i]->mMotion.mTracks[0].m20 = 1.0f;
    mChara[i]->mMotion.mTracks[0].m0C = 1.0f;
    mChara[i]->mMotion.fn_80097C08(0, motion, loop, 0, 1, blend, 0.1f);
}

GimmickCommon* MeetingSpot::fn_803FC8D8(atn::Task* parent, GimmickDesc_unk* desc) {
    if (parent != 0) {
        return new (12, "", "", 0) MeetingSpot(parent, desc);
    }
    return 0;
}

int MeetingSpot::vf0C() {
    if (fn_803EC91C() == 0) {
        m54 = 2;
        return 1;
    }
    return 0;
}

void MeetingSpot::vf28() {
    m6C = mDesc->mPos;
    m78.y = mDesc->mRotY;
    if (mDesc->m2C == 1 || mDesc->m2C == 4) {
        m78.y = atn::WrapAngle_unk(3.1415927f + m78.y);
    }
    m10C = fn_803C1560(lbl_8087F430->m10D8, &m6C, 0, 0, 0, 1, -1.0f);
    fn_803FD328(0);
}

void MeetingSpot::vf10() {
    int cancel = vf60() == 0;
    if (!cancel) {
        cancel = SqLength_unk(lbl_8087EFB4->mPos - m6C) > 250000.0f;
    }
    if (!cancel && mDesc->m2C == 1 && mState < 7) {
        CharaTask* player = (CharaTask*)lbl_8087F8A0->mHead;
        if (player->m55C == 6 && player->m560 == 0x3D) {
            atn::Vector3 pos = player->mPos;
            if (LengthXZSq_unk(pos - m6C) < 49.0f) {
                cancel = 1;
            }
        }
    }
    if (cancel) {
        if (mState != 0) {
            fn_803FD328(10);
        }
        return;
    }
    for (int i = 0; i < 2; i++) {
        if (mChara[i] != 0 && ((mChara[i]->m55C == 6 && mChara[i]->m560 == 0x44) || mChara[i]->fn_8016D3F8())) {
            switch (mState) {
            case 3:
            case 4:
                fn_803FD328(10);
                return;
            case 5:
                if (i == 0) {
                    fn_803FD328(10);
                }
                return;
            case 6:
            case 7:
                fn_803FD328(10);
                return;
            case 8:
            case 9:
                fn_803FD328(10);
                return;
            }
        }
    }
    switch (mState) {
    case 0:
        mTimer++;
        if (mDesc->m7C != 0 && mTimer > mDesc->m20) {
            fn_803FD328(1);
        }
        break;
    case 1:
        if (mChara[0] == 0) {
            mChara[0] = fn_803FD0A0(0);
        }
        if (mChara[0] != 0) {
            mChara[0]->fn_80170F20(&m6C, 0, m78.y, -1.0f);
            mChara[0]->mC58.mHeld_unk = 1;
            fn_803FD328(2);
        }
        break;
    case 2:
        if (mChara[0]->mRoute.m2C != 0) {
            fn_803FD328(3);
        }
        break;
    case 3: {
        Model_unk* motion = &mChara[0]->mMotion;
        float frame = Frame_unk(motion);
        if (frame >= motion->fn_80097D7C(0)) {
            fn_803FD328(4);
        }
        break;
    }
    case 4:
        if (mChara[1] == 0) {
            mChara[1] = fn_803FD0A0(mChara[0]);
        }
        if (mChara[1] != 0) {
            atn::Vector3 offset;
            float rot;
            switch (mDesc->m2C) {
            case 2:
            case 3:
                offset.Set_unk(15.0f, 0.0f, 0.0f);
                rot = m78.y;
                break;
            case 1:
                offset.Set_unk(-15.0f, 0.0f, 0.0f);
                rot = m78.y;
                break;
            case 4:
                offset.Set_unk(0.0f, 0.0f, -15.0f);
                rot = m78.y;
                break;
            default:
                offset.Set_unk(0.0f, 0.0f, 15.0f);
                rot = atn::WrapAngle_unk(3.1415927f + m78.y);
                break;
            }
            atn::Matrix3x4 m;
            PSMTXRotRad(m.m, 'y', m78.y);
            PSMTXMultVec(m.m, &offset, &offset);
            MoveTo_unk(mChara[1], m6C + offset, rot);
            mChara[1]->mC58.mHeld_unk = 1;
            fn_803FD328(5);
        } else {
            if (++mTimer > 900) {
                fn_803FD328(9);
            }
        }
        break;
    case 5:
        if (mChara[1]->mRoute.m2C != 0) {
            fn_803FD328(6);
        } else {
            if (++mTimer > 900) {
                fn_803FD328(9);
            }
        }
        break;
    case 6: {
        Model_unk* motion = &mChara[1]->mMotion;
        float frame = Frame_unk(motion);
        if (frame >= motion->fn_80097D7C(0)) {
            fn_803FD328(7);
        }
        break;
    }
    case 7:
        mTimer++;
        if (mTimer > mDesc->m24) {
            if (mDesc->m24 != 0 || mTimer > 300) {
                fn_803FD328(8);
            }
        }
        break;
    case 8: {
        Model_unk* motion = &mChara[1]->mMotion;
        float frame = Frame_unk(motion);
        if (frame >= motion->fn_80097D7C(0)) {
            fn_803FD328(9);
        }
        break;
    }
    case 9: {
        Model_unk* motion = &mChara[0]->mMotion;
        float frame = Frame_unk(motion);
        if (frame >= motion->fn_80097D7C(0)) {
            fn_803FD328(10);
        }
        break;
    }
    }
}

// inferred: the character is one of the four ids of the description (0 in the first one accepts
// any character)
bool MeetingSpot::MatchId_unk(CharaTask* chara) {
    unsigned int i;
    bool found = false;
    for (i = 4; i < 8; i++) {
        if ((&mDesc->m20)[i] == chara->m50) {
            found = true;
            break;
        }
        if ((&mDesc->m20)[i] == 0 && i == 4) {
            found = true;
            break;
        }
    }
    return found;
}

// NONMATCHING: 99.72% - the four id comparisons have their operands swapped (cmpw r0, r3) and
// the 6400.0f constant is emitted before the 1.0f/0.1f of fn_803FD328 (in the original it is
// the last of the pool).
CharaTask* MeetingSpot::fn_803FD0A0(CharaTask* other) {
    CharaTask* best = 0;
    float maxDist = 6400.0f;
    float bestDist = 0.0f;
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
        if (!MatchId_unk(npc)) {
            continue;
        }
        if (npc->mC58.m0C.mDestPoint > 0 && GetLink_unk(npc->mC58.m0C.mDestPoint, m10C) > 0) {
            float dist = SqLength_unk(npc->mPos - m6C);
            if (dist < maxDist && dist > bestDist) {
                bestDist = dist;
                best = npc;
            }
        }
    }
    return best;
}

// NONMATCHING: 95.29% - the motion picked by character in state 7 (0x76/0x77, 0x7B/0x7C) is
// chosen right before the call in the original (one saved register less), and the 1.0f/0.1f
// constants are placed before 6400.0f in the original pool.
void MeetingSpot::fn_803FD328(int state) {
    mState = state;
    mTimer = 0;
    switch (state) {
    case 3:
        switch (mDesc->m2C) {
        case 0:
        case 2:
            fn_803FD328(4);
            break;
        case 1:
            PlayMotion_unk(0, 0x78, 0);
            break;
        case 3:
            PlayMotion_unk(0, 0x7E, 0);
            break;
        case 4:
            PlayMotion_unk(0, 0x81, 0);
            break;
        }
        break;
    case 4:
        if (mDesc->m28 > 1) {
            switch (mDesc->m2C) {
            case 0:
                PlayMotion_unk(0, 2, 1);
                break;
            case 2:
                PlayMotion_unk(0, 2, 1);
                break;
            case 1: {
                PlayMotion_unk(0, 0x79, 1, 1.0f);
                atn::Vector3 rot = mChara[0]->mRot;
                rot.y = atn::WrapAngle_unk(3.1415927f + rot.y);
                mChara[0]->mRot = rot;
                break;
            }
            case 3:
                PlayMotion_unk(0, 0x7F, 1);
                break;
            case 4: {
                PlayMotion_unk(0, 0x82, 1, 1.0f);
                atn::Vector3 rot = mChara[0]->mRot;
                rot.y = atn::WrapAngle_unk(3.1415927f + rot.y);
                mChara[0]->mRot = rot;
                break;
            }
            }
        } else {
            fn_803FD328(7);
        }
        break;
    case 6:
        switch (mDesc->m2C) {
        case 0:
        case 2:
        case 4:
            fn_803FD328(7);
            break;
        case 1:
            PlayMotion_unk(1, 0x78, 0);
            break;
        case 3:
            PlayMotion_unk(1, 0x7E, 0);
            break;
        }
        break;
    case 7:
        for (int i = 0; i < 2; i++) {
            if (mChara[i] == 0) {
                continue;
            }
            switch (mDesc->m2C) {
            case 0:
                if (mDesc->m28 == 1) {
                    PlayMotion_unk(i, 0x75, 1);
                } else {
                    int motion = 0x77;
                    if (i == 0) {
                        motion = 0x76;
                    }
                    PlayMotion_unk(i, motion, 1);
                }
                break;
            case 2:
                PlayMotion_unk(i, 0x7D, 1);
                break;
            case 1:
                int motion = 0x7C;
                if (i == 0) {
                    motion = 0x7B;
                }
                PlayMotion_unk(i, motion, 0);
                if (i == 1) {
                    atn::Vector3 rot = mChara[0]->mRot;
                    rot.y = atn::WrapAngle_unk(3.1415927f + rot.y);
                    mChara[i]->mRot = rot;
                }
                break;
            case 3:
                PlayMotion_unk(i, 0x7F, 0);
                break;
            case 4:
                if (i == 1) {
                    PlayMotion_unk(i, 0x76, 0);
                }
                break;
            }
        }
        break;
    case 8:
        if (mChara[1] != 0) {
            switch (mDesc->m2C) {
            case 0:
            case 2:
            case 4:
                fn_803FD328(9);
                break;
            case 1:
                PlayMotion_unk(1, 0x7A, 0);
                break;
            case 3:
                PlayMotion_unk(1, 0x80, 0);
                break;
            }
        } else {
            fn_803FD328(9);
        }
        break;
    case 9:
        if (mChara[0] != 0) {
            switch (mDesc->m2C) {
            case 0:
            case 2:
                fn_803FD328(10);
                break;
            case 1:
                PlayMotion_unk(0, 0x7A, 0);
                break;
            case 3:
                PlayMotion_unk(0, 0x80, 0);
                break;
            case 4:
                PlayMotion_unk(0, 0x83, 0);
                break;
            }
        } else {
            fn_803FD328(10);
        }
        break;
    case 10:
        for (int i = 0; i < 2; i++) {
            if (mChara[i] != 0) {
                mChara[i]->fn_8015495C(0, 1, 1, 1);
                if ((mChara[i]->m55C == 6 && mChara[i]->m560 == 0x44) || mChara[i]->fn_8016D3F8()) {
                    if (mChara[i]->mType == 0) {
                        mChara[i]->m564 = 1;
                    } else {
                        mChara[i]->m564 = 2;
                    }
                } else {
                    mChara[i]->fn_8016EB48(0, 0, 0);
                }
                mChara[i]->mC58.mHeld_unk = 0;
            }
            mPrev[i] = mChara[i];
            mChara[i] = 0;
        }
        fn_803FD328(0);
        break;
    }
}

void MeetingSpot::vf14() {}

int MeetingSpot::vf40(GimmickMsg_unk* msg) {
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

int MeetingSpot::vf60() {
    int ret = 0;
    if (m54 == 2 && mDesc->m7C != 0) {
        ret = 1;
    }
    return ret;
}

} // namespace gmk
