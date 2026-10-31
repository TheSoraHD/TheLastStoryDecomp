#ifndef GAME_TEAMAIMANAGER_H
#define GAME_TEAMAIMANAGER_H

#include "atn/Task.h"
#include "atn/Array_unk.h"
#include "atn/LH_Binary.h"
#include "game/ResHandle.h"
#include "math/Vec.h"

// inferred: team data (probably the TeamEntry_unk given to fn_805AE26C; names invented)
struct TeamData_unk {
    unsigned char m00[0x04];
    int mId; // 0x04
    unsigned char m08[0x8C - 0x08];
    void* mLeader; // 0x8C: inferred
};

struct TeamAI_unk;
// inferred: member of a team (a character)
struct TeamChara_unk {
    unsigned char m000[0x528];
    atn::Vector3 mPos; // 0x528
    unsigned char m534[0xD2C - 0x534];
    TeamAI_unk* mTeam; // 0xD2C
};

// inferred: node of the team tree (names invented)
struct TeamNode_unk {
    TeamNode_unk() : mParent(0), mChild(0), mPrev(0), mNext(0) {}
    void Detach_unk() {
        if (mParent != 0 && mParent->mChild == this) {
            mParent->mChild = mNext;
            mParent = 0;
        }
        if (mPrev != 0) {
            mPrev->mNext = mNext;
        }
        if (mNext != 0) {
            mNext->mPrev = mPrev;
        }
    }
    void Attach_unk(TeamNode_unk* parent) {
        mParent = parent;
        mPrev = 0;
        mNext = parent->mChild;
        if (parent->mChild != 0) {
            parent->mChild->mPrev = this;
        }
        parent->mChild = this;
    }

    void Add_unk(TeamNode_unk* child) {
        child->mParent = this;
        child->mPrev = 0;
        child->mNext = mChild;
        if (mChild != 0) {
            mChild->mPrev = child;
        }
        mChild = child;
    }

    TeamNode_unk* mParent; // 0x00
    TeamNode_unk* mChild;  // 0x04
    TeamNode_unk* mPrev;   // 0x08
    TeamNode_unk* mNext;   // 0x0C
};

// inferred: ring buffer of N entries {data, head, tail} (names invented)
template <class T, int N>
struct TeamRing_unk {
    TeamRing_unk() : mHead(0), mTail(N - 1) {}
    int Prev_unk(int i) const {
        i--;
        if (i < 0) {
            i += N;
        }
        return i;
    }
    T* Last_unk() {
        if (mTail == Prev_unk(mHead)) {
            return 0;
        }
        return &mData[Prev_unk(mHead)];
    }

    T mData[N];
    int mHead;
    int mTail;
};
struct TeamCmd_unk {
    unsigned char m00[0x10];
};

// inferred: AI of a team (0x37C bytes; class of game/TeamAI_unk.cpp, names invented)
struct TeamAI_unk : public TeamNode_unk {
    TeamAI_unk()
        : m218(0), m23C(0), mType(0xFF), m23E(0), mAIType(0), mBattleMode(0), mAreaID(0), mTargetMode(0),
          mTargetType(0), m245(2), mState(0), mNumMembers(0) {}
    int GetId_unk() const { return m218 != 0 ? m218->mId : 0; }
    void* GetLeader_unk() const { return m218 != 0 ? m218->mLeader : 0; }

    TeamRing_unk<TeamCmd_unk, 32> m10; // 0x010
    TeamData_unk* m218;                // 0x218
    unsigned char m21C[0x23C - 0x21C];
    unsigned char m23C;
    unsigned char mType;       // 0x23D: 1 NPC, 2 enemy, 3 friend
    unsigned char m23E;
    unsigned char mAIType;     // 0x23F
    unsigned char mBattleMode; // 0x240
    unsigned char mAreaID;     // 0x241
    unsigned char m242;
    unsigned char mTargetMode; // 0x243
    unsigned char mTargetType; // 0x244
    unsigned char m245;
    int mState;                // 0x248
    unsigned char mNumMembers; // 0x24C
    TeamChara_unk* mMembers[64]; // 0x250
    TeamRing_unk<unsigned char, 32> mTgtCtrlMode; // 0x350
    unsigned char m378[0x37C - 0x378];
};

// inferred: script message received by the manager (names invented)
struct TeamMsgIn_unk {
    unsigned char m00[0x08];
    int mType; // 0x08: team type
    int mId;   // 0x0C: team id
    int m10;
    int m14;
    int m18;
    int m1C;
    int m20;
};

class TeamAIManager;
extern "C" TeamAIManager* lbl_8087FA00; // inferred: TeamAIManager instance
extern "C" int lbl_8087FA04;           // inferred: only cleared by the constructor

// inferred from __vt__13TeamAIManager (0x80797790)
class TeamAIManager : public atn::Task {
public:
    TeamAIManager(atn::Task* parent);
    virtual ~TeamAIManager();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();

    static TeamAIManager* Create_unk(atn::Task* parent);
    void Rebuild_unk();
    int IsTop_unk(TeamNode_unk* node);
    void OnTarget_unk(const TeamMsgIn_unk* msg);
    void OnTarget2_unk(const TeamMsgIn_unk* msg);
    void OnMsg20_unk(const TeamMsgIn_unk* msg);
    void OnParent_unk(const TeamMsgIn_unk* msg);
    void OnMembers_unk(const TeamMsgIn_unk* msg);
    void OnMsg21_unk(const TeamMsgIn_unk* msg);
    void OnChara_unk(const TeamMsgIn_unk* msg);
    void OnMsg24_unk(const TeamMsgIn_unk* msg);
    TeamAI_unk* Find_unk(int type, int id);

    atn::SimpleArray<TeamAI_unk> mTeams; // 0x48
    TeamNode_unk mRoot;                 // 0x50
    int m60;
    ResHandle mMenu;      // 0x64
    int mDebug;           // 0x68
    int mSelected;        // 0x6C
    atn::LH_Binary m70;
    atn::LH_Binary m78;
    atn::LH_Binary m80[3];
};

#endif
