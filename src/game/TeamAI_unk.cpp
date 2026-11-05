// game/TeamAI_unk.cpp: AI of a team of characters (TeamAI_unk, include/game/TeamAIManager.h).
// File name invented. The members of a team are characters: TeamChara_unk is the view of
// include/game/TeamAIManager.h, used here as a CharaTask.
#include "game/TeamAIManager.h"
#include "game/CharaTask.h"
#include "game/GameField_unk.h"

extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" void PSMTXRotRad(Mtx m, char axis, float rad);
extern "C" int rand();
extern "C" void fn_80370AE4(GameField_unk* field, int index, int value); // inferred: sets a work value

struct BulletMgr_unk;
extern "C" BulletMgr_unk* lbl_8087F048; // inferred: battle task
extern "C" void fn_800FDE60(BulletMgr_unk* mgr, int arg, CharaTask* chara, const atn::Vector3* dir);

// inferred: the member as a character (TeamChara_unk is a partial view)
inline CharaTask* Chara_unk(TeamChara_unk* chara) {
    return (CharaTask*)chara;
}

void TeamAI_unk::fn_805AE26C(int type, TeamData_unk* data) {
    mType = type;
    m218 = data;
    if (data->mAIType == 0) {
        SetAIType_unk(GetAIType_unk());
        SetAIType_unk(0);
    } else {
        SetAIType_unk(data->mAIType);
    }
    mTargetMode = 0;
    mTargetType = 0;
    m378 = 1;
    mTgtCtrlMode.Push_unk(0);
    m378 = 1;
    memset(m2D0, 0, sizeof(m2D0));
    fn_805AE3C4();
}

void TeamAI_unk::fn_805AE3C4() {
    memset(mMembers, 0, sizeof(mMembers));
    mNumMembers = 0;
    if (lbl_8087FA00->IsTop_unk(this)) {
        mNumMembers = fn_805AEC4C(mMembers, 0);
    }
    for (int i = 0; i < mNumMembers; i++) {
        mMembers[i]->mTeam = this;
    }
}

// inferred: HP of the members over their maximum, in percent
float TeamAI_unk::fn_805AE454() {
    int max = 0;
    float hp = 0.0f;
    for (int i = 0; i < mNumMembers; i++) {
        CharaTask* chara = Chara_unk(mMembers[i]);
        hp += chara->mStatus.m04;
        max += chara->mStatus.mParamB.m84;
    }
    if (max == 0) {
        return 0.0f;
    }
    return (100.0f * hp) / max;
}

// inferred: members that can still fight, in percent
float TeamAI_unk::fn_805AE5E8() {
    int count = 0;
    if (mType == 2) {
        for (int i = 0; i < mNumMembers; i++) {
            if (Chara_unk(mMembers[i])->IsActive_unk()) {
                count++;
            }
        }
    } else {
        for (int i = 0; i < mNumMembers; i++) {
            CharaTask* chara = Chara_unk(mMembers[i]);
            if (!chara->IsFlag_unk(4)) {
                if (chara->CanAct_unk() || chara->mStatus.m224 > 0) {
                    count++;
                }
            }
        }
    }
    if (mNumMembers == 0) {
        return 0.0f;
    }
    return (100.0f * count) / mNumMembers;
}

// inferred: HP of the leader, in percent
float TeamAI_unk::fn_805AE74C() {
    CharaTask* leader = (CharaTask*)GetLeader_unk();
    if (leader == 0) {
        return 0.0f;
    }
    return (100.0f * leader->mStatus.m04) / leader->mStatus.mParamB.m84;
}

// inferred: members that can act
int TeamAI_unk::fn_805AE7B4() {
    int count = 0;
    for (int i = 0; i < mNumMembers; i++) {
        if (Chara_unk(mMembers[i])->CanAct_unk()) {
            count++;
        }
    }
    return count;
}

// inferred: sets the HP of the members to `ratio` percent; the ones at 0 are knocked back
void TeamAI_unk::fn_805AE864(int ratio) {
    for (int i = 0; i < mNumMembers; i++) {
        CharaTask* chara = Chara_unk(mMembers[i]);
        chara->mStatus.m04 = (float)(ratio * chara->mStatus.mParamB.m84) / 100.0f;
        chara->mStatus.fn_801333E4();
        if (ratio <= 0 && lbl_8087F048 != 0) {
            atn::Vector3 dir(0.0f, 0.0f, 1.0f);
            Mtx m;
            PSMTXRotRad(m, 'y', chara->mRot.y);
            PSMTXMultVec(m, &dir, &dir);
            atn::Vector3 back(-dir.x, -dir.y, -dir.z);
            fn_800FDE60(lbl_8087F048, 0, chara, &back);
        }
    }
}

void TeamAI_unk::SetAIType_unk(unsigned char type) {
    if (type <= 2) {
        mAIType = type;
        m378 = 1;
    }
}

void TeamAI_unk::SetBattleMode_unk(int mode) {
    mBattleMode = mode;
    m378 = 1;
    for (int i = 0; i < mNumMembers; i++) {
        Chara_unk(mMembers[i])->mC58.mC0 = mode;
    }
}

void TeamAI_unk::SetAreaID_unk(int id) {
    mAreaID = id;
    m378 = 1;
    for (int i = 0; i < mNumMembers; i++) {
        Chara_unk(mMembers[i])->mC58.mB4 = id;
    }
}

void TeamAI_unk::SetM242_unk(int value) {
    m242 = value;
    for (int i = 0; i < mNumMembers; i++) {
        Chara_unk(mMembers[i])->mC58.mB8 = value;
    }
}

// NONMATCHING: 73% - the original copies the command field by field (loads and stores interleaved)
void TeamAI_unk::fn_805AEA70(const TeamCmd_unk* cmd) {
    m10.PushFront_unk(*cmd);
}

void TeamAI_unk::fn_805AEAC0() {
    m10.mHead = 0;
    m10.mTail = 31;
    m23E = 0;
    for (int i = 0; i < mNumMembers; i++) {
        Chara_unk(mMembers[i])->fn_80171DB0();
    }
}

// inferred: the members have finished moving (mode 0/1: the ones that can act; 2: any one moving)
int TeamAI_unk::fn_805AEB34(unsigned int mode) {
    switch (mode) {
    case 0:
    case 1:
        for (int i = 0; i < mNumMembers; i++) {
            CharaTask* chara = Chara_unk(mMembers[i]);
            if ((chara->CanAct_unk() || mode == 0) && chara->mRoute.m2C == 0) {
                return 0;
            }
        }
        return 1;
    case 2:
        for (int i = 0; i < mNumMembers; i++) {
            if (Chara_unk(mMembers[i])->mRoute.m2C != 0) {
                return 1;
            }
        }
        return 0;
    }
    return 1;
}

// inferred: adds the characters of this team and of the teams below it to `list` (at most 32)
int TeamAI_unk::fn_805AEC4C(TeamChara_unk** list, int count) {
    if (count == -1) {
        return -1;
    }
    TeamChara_unk** out = &list[count];
    for (int i = 0; i < 32; i++) {
        TeamChara_unk* chara = m218->mCharas[i];
        if (chara != 0) {
            if (count >= 32) {
                return -1;
            }
            bool found = false;
            for (int j = 0; j < count; j++) {
                if (list[j] == chara) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                *out++ = chara;
                count++;
            }
        }
    }
    if (mChild != 0) {
        count = ((TeamAI_unk*)mChild)->fn_805AEC4C(list, count);
    }
    if (!lbl_8087FA00->IsTop_unk(this) && mNext != 0) {
        count = ((TeamAI_unk*)mNext)->fn_805AEC4C(list, count);
    }
    return count;
}

// NONMATCHING: 98.7% - this/move/character in r27/r28/r29 in the original (r28/r29/r27 here)
void TeamAI_unk::fn_805AED74(const TeamMove_unk* move) {
    if (!(m23E & 1)) {
        if (move->m0A == 3 || move->m0A == 4) {
            if (move->mKind == 0) {
                Move_unk(move->mKind, (TeamPointList_unk<TeamRoutePoint_unk>*)move->mList, move->mFlags);
            } else if (move->mKind == 1) {
                Move_unk(move->mKind, (TeamPointList_unk<TeamHidePoint_unk>*)move->mList, move->mFlags);
            } else if (move->mKind == 2) {
                Move_unk(move->mKind, (TeamPointList_unk<TeamPlace_unk>*)move->mList, move->mFlags);
            }
        } else {
            m23C = 0;
            memset(m21C, 0, sizeof(m21C));
            for (int i = 0; i < mNumMembers; i++) {
                CharaTask* chara = Chara_unk(mMembers[i]);
                if ((move->mFlags & 4) == 4 && chara == GetLeader_unk()) {
                    continue;
                }
                bool out = false;
                if (chara->IsFlag_unk(4) || (chara->mType == 2 && chara->mStatus.IsState_unk(0x20))) {
                    out = true;
                }
                if (out) {
                    continue;
                }
                switch (move->mKind) {
                case 0:
                    chara->fn_8016F824(
                        SelectPoint_unk(i, ((TeamPointList_unk<TeamRoutePoint_unk>*)move->mList)->mPoints, 2), 0);
                    break;
                case 1:
                    chara->fn_8016FDCC(
                        SelectPoint_unk(i, ((TeamPointList_unk<TeamHidePoint_unk>*)move->mList)->mPoints, 2), 0);
                    break;
                case 2:
                    chara->fn_8017039C(
                        SelectPoint_unk(i, ((TeamPointList_unk<TeamPlace_unk>*)move->mList)->mPoints, 2), 0);
                    break;
                default:
                    return;
                }
                if (move->mMode == 2 && m23C != 0) {
                    break;
                }
            }
        }
        m23E |= 1;
    } else if (fn_805AEB34(move->mMode)) {
        m23E = 0;
    }
}

// NONMATCHING: 96.5% - the original loads the point before the counters are increased and keeps it in
// r3; same code otherwise (three instantiations)
template <class P>
int TeamAI_unk::SelectPoint_unk(int index, P** points, int mode) {
    int count = 0;
    while (count < 32 && points[count] != 0) {
        count++;
    }
    switch (mode) {
    case 0:
        return points[index % count]->mId;
    case 1: {
        int start = count * (rand() / 32767.0f);
        for (int i = 0; i < count; i++) {
            int k = (start + i) % count;
            if (m21C[k] <= m23C / count) {
                m21C[k]++;
                P* point = points[k];
                m23C++;
                return point->mId;
            }
        }
        break;
    }
    case 2: {
        float best = 3.4028235e38f;
        int found = -1;
        for (int i = 0; i < count; i++) {
            if (m21C[i] <= m23C / count) {
                const atn::Vector3& pos = points[i]->mPos;
                const atn::Vector3& from = Chara_unk(mMembers[index])->mPos;
                atn::Vector3 d(pos.x - from.x, pos.y - from.y, pos.z - from.z);
                float dist = d.LengthSq_unk();
                if (dist < best) {
                    found = i;
                    best = dist;
                }
            }
        }
        m21C[found]++;
        P* point = points[found];
        m23C++;
        return point->mId;
    }
    }
    return -1;
}

// NONMATCHING: 95% - same registers as fn_805AED74
void TeamAI_unk::fn_805B1918(const TeamMove_unk* move) {
    if (!(m23E & 1)) {
        if (move->m0A == 3 || move->m0A == 4) {
            Move_unk(move->mKind, (TeamPointList_unk<TeamHidePoint_unk>*)move->mList, move->mFlags);
        } else {
            m23C = 0;
            memset(m21C, 0, sizeof(m21C));
            for (int i = 0; i < mNumMembers; i++) {
                CharaTask* chara = Chara_unk(mMembers[i]);
                if ((move->mFlags & 4) == 4 && chara == GetLeader_unk()) {
                    continue;
                }
                bool out = false;
                if (chara->IsFlag_unk(4) || (chara->mType == 2 && chara->mStatus.IsState_unk(0x20))) {
                    out = true;
                }
                if (out) {
                    continue;
                }
                if (move->mKind == 1) {
                    chara->fn_8016FDCC(
                        SelectPoint_unk(i, ((TeamPointList_unk<TeamHidePoint_unk>*)move->mList)->mPoints, 2), 0);
                    if (move->mMode == 2 && m23C != 0) {
                        break;
                    }
                } else {
                    return;
                }
            }
        }
        m23E |= 1;
    } else if (fn_805AEB34(move->mMode)) {
        m23E = 0;
    }
}

// NONMATCHING: 96.7% - the original saves r30 before it loads mDst (prologue scheduling)
void TeamAI_unk::fn_805B1A9C(const TeamVarOp_unk* op) {
    int a;
    switch (op->mDst) {
    case 0:
        a = fn_80370A78(lbl_8087F430, op->mVar);
        break;
    case 1:
        a = fn_80370174(lbl_8087F430, op->mVar);
        break;
    default:
        return;
    }
    int b;
    switch (op->mSrc) {
    case 0:
        b = op->mValue;
        break;
    case 1:
        b = fn_80370A78(lbl_8087F430, op->mValue);
        break;
    case 2:
        b = fn_80370174(lbl_8087F430, op->mValue);
        break;
    case 3:
        rand(); // inferred: a random source left unfinished
    default:
        return;
    }
    switch (op->mOp) {
    case 0:
        a = b;
        break;
    case 1:
        a = a + b;
        break;
    case 2:
        a = a - b;
        break;
    case 3:
        a = a * b;
        break;
    case 4:
        a = a / b;
        break;
    case 5:
        a = a == b;
        break;
    case 6:
        a = a != b;
        break;
    case 7:
        a = a < b;
        break;
    case 8:
        a = a <= b;
        break;
    case 9:
        a = a > b;
        break;
    case 10:
        a = a >= b;
        break;
    case 11:
        a = a & b;
        break;
    case 12:
        a = a | b;
        break;
    default:
        return;
    }
    switch (op->mDst) {
    case 0:
        fn_80370AE4(lbl_8087F430, op->mVar, a);
        break;
    case 1:
        fn_80370320(lbl_8087F430, op->mVar, a, 0);
        break;
    }
}
