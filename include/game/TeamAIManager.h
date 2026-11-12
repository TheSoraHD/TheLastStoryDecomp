#ifndef GAME_TEAMAIMANAGER_H
#define GAME_TEAMAIMANAGER_H

#include "atn/Task.h"
#include "atn/Array_unk.h"
#include "atn/LH_Binary.h"
#include "game/ResHandle.h"
#include "game/EventScriptCmd_unk.h"
#include "math/Vec.h"

// inferred: team data (probably the TeamEntry_unk given to fn_805AE26C; names invented)
struct TeamData_unk {
    unsigned char m00[0x04];
    int mId; // 0x04
    unsigned int mAIType; // 0x08
    struct TeamChara_unk* mCharas[0x20]; // 0x0C: inferred: members (null: none)
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
    // inferred: stores an entry at the tail, which moves back (name invented)
    void PushFront_unk(const T& value) {
        mData[mTail] = value;
        mTail--;
        if (mTail < 0) {
            mTail += N;
        }
    }
    // inferred: adds an entry unless the ring is full (name invented)
    void Push_unk(T value) {
        if (mHead != mTail) {
            mData[mHead] = value;
            mHead++;
            if (mHead >= N) {
                mHead -= N;
            }
        }
    }
    // inferred: drops the last entry (name invented)
    void Pop_unk() {
        mHead--;
        if (mHead < 0) {
            mHead += N;
        }
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
struct TeamCmdArg_unk {
    int m00;
    int m04;
};
struct TeamCmd_unk {
    unsigned short m00;
    unsigned short m02;
    TeamCmdArg_unk m04;
    int m0C;
};

// inferred: points the members of a team are sent to (route points, hide points, places); only the id
// and the position are used. Names invented.
struct TeamPoint_unk {
    int mId;
    atn::Vector3 mPos;
};
struct TeamRoutePoint_unk : public TeamPoint_unk {};
struct TeamHidePoint_unk : public TeamPoint_unk {};
struct TeamPlace_unk : public TeamPoint_unk {};
// inferred: list of points of a move order (the points start at 0x10)
template <class P>
struct TeamPointList_unk {
    unsigned char m00[0x10];
    P* mPoints[32];
};
// inferred: move order (names invented)
struct TeamMove_unk {
    void* mList;          // inferred: TeamPointList_unk of the kind
    unsigned int mFlags;  // 0x04: inferred: 4 leaves the leader out
    unsigned char mKind;  // 0x08: 0 route points, 1 hide points, 2 places
    unsigned char mMode;  // 0x09
    unsigned char m0A;    // inferred: 3 and 4 move the team as a whole
};

// inferred: operation on a field value (names invented)
struct TeamVarOp_unk {
    int mVar;            // 0x00: inferred: work index (mDst 0) or flag (mDst 1)
    int mValue;          // 0x04: inferred: constant, work index or flag (mSrc)
    unsigned char mDst;  // 0x08
    unsigned char mSrc;  // 0x09
    unsigned char mOp;   // 0x0A
};

// inferred: order that sets a team parameter (names invented)
struct TeamSetParam_unk {
    int mValue;          // 0x00: inferred: constant, work index or flag (mSrc)
    unsigned char mSrc;  // 0x04
    unsigned char mKind; // 0x05: 0 HP ratio, 3 AI type, 4 battle mode, 5 area
};

// inferred: order that stores a team parameter in a field value (names invented)
struct TeamGetParam_unk {
    int mVar;            // 0x00: inferred: work index (mDst 0) or flag (mDst 1)
    unsigned char mDst;  // 0x04
    unsigned char mKind; // 0x05: 0 HP, 1 fighters, 2 leader HP (percent), 3 AI type, 4 battle mode, 5 area
};

// inferred: gimmick order (names invented)
struct TeamGimmick_unk {
    int mGimmick;       // 0x00: inferred: gimmick id (gm%03d_%03d)
    int mArg;           // 0x04
    int mMode;          // 0x08: 0 all, 1 members that can act, 2 the one with data m122 == 2
};
// inferred: attack order (names invented)
struct TeamAttack_unk {
    class CharaTask* mTarget;
    unsigned char mMode; // 0x04: 0 all, 1 members that can act, 2 the nearest member
};

// inferred: AI of a team (0x37C bytes; class of game/TeamAI_unk.cpp, names invented)
struct TeamAI_unk : public TeamNode_unk {
    TeamAI_unk()
        : m218(0), m23C(0), mType(0xFF), m23E(0), mAIType(0), mBattleMode(0), mAreaID(0), mTargetMode(0),
          mTargetType(0), m245(2), mState(0), mNumMembers(0) {}
    int GetId_unk() const { return m218 != 0 ? m218->mId : 0; }
    void* GetLeader_unk() const { return m218 != 0 ? m218->mLeader : 0; }
    int GetAIType_unk() const { return m218 != 0 ? m218->mAIType : 0; } // inferred: name invented

    // game/TeamAI_unk.cpp (names invented)
    void fn_805AE26C(int type, TeamData_unk* data); // inferred: setup
    void fn_805AE3C4();                             // inferred: collects the members
    float fn_805AE454();                            // inferred: HP ratio
    float fn_805AE5E8();                            // inferred: member ratio
    float fn_805AE74C();                            // inferred: leader HP ratio
    int fn_805AE7B4();                              // inferred: members able to act
    void fn_805AE864(int ratio);                    // inferred: sets the HP of the members
    void SetAIType_unk(unsigned int type);         // 0x805AE9B8
    void SetBattleMode_unk(int mode);       // 0x805AE9D0
    void SetAreaID_unk(int id);             // 0x805AEA08
    void SetM242_unk(int value);            // 0x805AEA40
    void fn_805AEA70(TeamCmd_unk* cmd);       // inferred: queues a command
    void fn_805AEAC0();                             // inferred: resets the commands
    int fn_805AEB34(int mode);
    int fn_805AEC4C(TeamChara_unk** list, int count); // inferred: collects the members of the tree
    void fn_805AED74(TeamMove_unk* move);       // inferred: sends the members to points
    // inferred: point for the member `index` (mode 0: by index, 1: random, 2: nearest; at most an even
    // share of the members on each point). Instantiations 0x805AEF9C, 0x805AF1C4 and 0x805AF3EC.
    void fn_805B1918(TeamMove_unk* move); // inferred: sends the members to hide points
    void fn_805B1A9C(TeamVarOp_unk* op);  // inferred: operation on a field value
    void fn_805B1C84(TeamGimmick_unk* order);
    TeamChara_unk* FindGimmickUser_unk();
    void fn_805B1E5C(TeamAttack_unk* order);
    void fn_805B2028(TeamSetParam_unk* order);
    void fn_805B228C(TeamGetParam_unk* order);
    void fn_805B2560(); // inferred: update (runs the queued orders)
    void fn_805B27A0();
    template <class P>
    int SelectPoint_unk(int index, P** points, int mode);
    template <class P>
    void Move_unk(int kind, TeamPointList_unk<P>* list, unsigned int flags, int mode); // 0x805AF614, 0x805B01C0, 0x805B0D6C

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
    TeamChara_unk* mMembers[32]; // 0x250
    void* m2D0[32];
    TeamRing_unk<unsigned char, 32> mTgtCtrlMode; // 0x350
    int m378; // inferred: changed flag
};

// inferred: event script command received by the manager (names invented)
struct TeamMsgIn_unk : public ScriptCmd_unk {
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
