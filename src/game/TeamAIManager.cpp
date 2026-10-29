#include "atn/Task.h"
#include "atn/Array_unk.h"
#include "atn/LH_Binary.h"
#include "atn/Memory.h"
#include "game/CharaManager.h"
#include "game/EnemyManager.h"
#include "game/NpcManager.h"
#include "game/ResHandle.h"
#include "math/Vec.h"

extern "C" int sprintf(char* buf, const char* fmt, ...);

// inferred: debug drawing (as in game/GameSaveTask.cpp)
extern "C" void* lbl_8087EEB0;
extern "C" void fn_80060D58(void* draw, float x, float y, float z, float w, float h,
                            unsigned int color); // inferred: rectangle
extern "C" void fn_800616C0(void* draw, float x, float y, float z, float w, float h, const char* text,
                            unsigned int color, float f6, int a, int b, int c); // inferred: text
// inferred: line from `from` to `to`
extern "C" void fn_800638B0(void* draw, float a, float b, const atn::Vector3* from, const atn::Vector3* to,
                            unsigned int color);
extern "C" void* lbl_8087EF70; // inferred: pad
extern "C" int fn_800A555C(void* pad, int port, int button); // inferred: button triggered
extern "C" void fn_80087994(void* menu, const char* name, int* flag, int arg3, int arg4);

extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager

// inferred: entries of the field tables read here (names invented)
struct TeamEntry_unk {
    unsigned char m00[0x90];
};
struct TeamTarget_unk {
    int mId;
    unsigned char m04[0x94 - 0x04];
};
// inferred: {count, entries} with a lookup by id (names invented)
struct TeamTargets_unk {
    TeamTarget_unk* Find_unk(int id) {
        for (int i = 0; i < mCount; i++) {
            TeamTarget_unk* p = &mData[i];
            if (id == p->mId) {
                return p;
            }
        }
        return 0;
    }

    int mCount;
    TeamTarget_unk* mData;
};
// inferred: team tables of the field (lbl_8087F430->m10D8, see game/CharaStatus.h); only the
// arrays used here
struct TeamTable_unk {
    unsigned char m000[0x148];
    int mNumEnemies;              // 0x148
    TeamEntry_unk* mEnemies;      // 0x14C
    int mNumFriends;              // 0x150
    TeamEntry_unk* mFriends;      // 0x154
    TeamTargets_unk mTargets[3];  // 0x158
};
struct TeamField_unk {
    unsigned char m00[0x10D8];
    TeamTable_unk* m10D8;
};
extern "C" TeamField_unk* lbl_8087F430;

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

// inferred: functions of game/TeamAI_unk.cpp
extern "C" void fn_805AE26C(TeamAI_unk* team, int type, TeamEntry_unk* entry); // inferred: setup
extern "C" void fn_805AE3C4(TeamAI_unk* team);
extern "C" float fn_805AE454(TeamAI_unk* team); // inferred: HP ratio
extern "C" float fn_805AE5E8(TeamAI_unk* team); // inferred: member ratio
extern "C" float fn_805AE74C(TeamAI_unk* team); // inferred: leader HP ratio
extern "C" int fn_805AE7B4(TeamAI_unk* team);
extern "C" void fn_805AEA70(TeamAI_unk* team, const void* msg); // inferred: sends a message
extern "C" void fn_805AEAC0(TeamAI_unk* team);
extern "C" void fn_805B2028(TeamAI_unk* team, const void* arg);
extern "C" void fn_805B2560(TeamNode_unk* team); // inferred: update

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
// inferred: messages sent to a team (16 bytes, names invented)
struct TeamMsgTarget_unk {
    unsigned short mType;
    void* mTarget;
    int m08;
    unsigned char m0C;
    unsigned char m0D;
    unsigned char m0E;
};
struct TeamMsgInt_unk {
    unsigned short mType;
    int m04;
    int m08;
    unsigned char m0C;
    unsigned char m0D;
    unsigned char m0E;
};
struct TeamMsg3_unk {
    unsigned short mType;
    int m04;
    int m08;
    int m0C;
};
struct TeamMsgChara_unk {
    unsigned short mType;
    void* mChara;
    unsigned char m08;
};
struct TeamMsgBody_unk {
    int m00;
    unsigned char m04;
    unsigned char m05;
};
struct TeamMsg24_unk {
    unsigned short mType;
    TeamMsgBody_unk mBody;
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

int TeamAIManager::vf0C() {
    return 1;
}

TeamAIManager::~TeamAIManager() {
    lbl_8087FA00 = 0;
}

// inferred: not inline; auto-inlined in Create_unk and stripped by the linker (its literals are
// first in the pool)
TeamAIManager::TeamAIManager(atn::Task* parent) : atn::Task(parent), m60(0), mDebug(0), mSelected(0) {
    lbl_8087FA04 = 0;
    fn_80087994(mMenu.Load("TeamAI"), "DebugMode", &mDebug, 0, 0);
}

TeamAIManager* TeamAIManager::Create_unk(atn::Task* parent) {
    if (lbl_8087FA00 == 0) {
        lbl_8087FA00 = new ((MemoryTag)3, "", "", 0) TeamAIManager(parent);
    }
    return lbl_8087FA00;
}

// inferred: removed by the linker; only their literals are left in the pool, each followed by an
// extra zero byte as in gmk/SwingObject.cpp (names invented)
void TeamDefault_unk(void* menu, int* flag) {
    fn_80087994(menu, "team_default\0", flag, 0, 0);
}
void TeamName_unk(char* buf, int id) {
    sprintf(buf, "ai_team%03d\0", id);
}

// NONMATCHING: 98.87% - register allocation of the linking loop and order of the induction variables
// inferred: rebuilds the teams from the field tables (enemies, then friends)
void TeamAIManager::Rebuild_unk() {
    TeamTable_unk* table = lbl_8087F430->m10D8;
    mTeams.Free_unk();
    mRoot.mParent = mRoot.mChild = mRoot.mPrev = mRoot.mNext = 0;
    int count = table->mNumEnemies + table->mNumFriends;
    if (count != 0) {
        mTeams.Resize_unk(count, 3);
        TeamNode_unk* root = &mRoot;
        for (int i = 0; i < count; i++) {
            root->Add_unk(&mTeams.mData[i]);
        }
        // inferred: `count` is reused for the number of enemy teams
        for (int i = 0; i < (count = table->mNumEnemies); i++) {
            fn_805AE26C(&mTeams.mData[i], 3, &table->mEnemies[i]);
        }
        for (int i = 0; i < table->mNumFriends; i++) {
            fn_805AE26C(&mTeams.mData[count + i], 2, &table->mFriends[i]);
        }
    }
}

void TeamAIManager::vf10() {
    for (TeamNode_unk* node = mRoot.mChild; node != 0; node = node->mNext) {
        fn_805B2560(node);
    }
}

int TeamAIManager::IsTop_unk(TeamNode_unk* node) {
    return node->mParent == &mRoot;
}

// NONMATCHING: 91.42% - register allocation of the inlined Find_unk (index, type and id take
// other registers) and of the target lookups
void TeamAIManager::OnTarget_unk(const TeamMsgIn_unk* in) {
    int id = in->mId;
    int type = in->mType;
    TeamAI_unk* team = Find_unk(type, id);
    if (team != 0) {
        TeamMsgTarget_unk msg;
        msg.mType = 0x1C;
        msg.m0C = in->m10;
        msg.m0D = in->m18;
        msg.m08 = in->m20;
        msg.m0E = in->m1C;
        switch (in->m10) {
        case 0:
            msg.mTarget = lbl_8087F430->m10D8->mTargets[0].Find_unk(in->m14);
            break;
        case 1:
            msg.mTarget = lbl_8087F430->m10D8->mTargets[1].Find_unk(in->m14);
            break;
        case 2:
            msg.mTarget = lbl_8087F430->m10D8->mTargets[2].Find_unk(in->m14);
            break;
        }
        fn_805AEA70(team, &msg);
    }
}

// NONMATCHING: 91.42% - register allocation of the inlined Find_unk (index, type and id take
// other registers) and of the target lookups
void TeamAIManager::OnTarget2_unk(const TeamMsgIn_unk* in) {
    int id = in->mId;
    int type = in->mType;
    TeamAI_unk* team = Find_unk(type, id);
    if (team != 0) {
        TeamMsgTarget_unk msg;
        msg.mType = 0x1D;
        msg.m0C = in->m10;
        msg.m0D = in->m18;
        msg.m08 = in->m20;
        msg.m0E = in->m1C;
        switch (in->m10) {
        case 0:
            msg.mTarget = lbl_8087F430->m10D8->mTargets[0].Find_unk(in->m14);
            break;
        case 1:
            msg.mTarget = lbl_8087F430->m10D8->mTargets[1].Find_unk(in->m14);
            break;
        case 2:
            msg.mTarget = lbl_8087F430->m10D8->mTargets[2].Find_unk(in->m14);
            break;
        }
        fn_805AEA70(team, &msg);
    }
}

// NONMATCHING: 98.85% - register allocation of the inlined Find_unk (index, type and id take
// other registers)
void TeamAIManager::OnMsg20_unk(const TeamMsgIn_unk* in) {
    TeamAI_unk* team = Find_unk(in->mType, in->mId);
    if (team != 0) {
        TeamMsgInt_unk msg;
        msg.mType = 0x20;
        msg.m0C = in->m10;
        msg.m04 = in->m14;
        msg.m0D = in->m18;
        msg.m08 = in->m1C;
        msg.m0E = in->m20;
        fn_805AEA70(team, &msg);
    }
}

// NONMATCHING: 96.89% - register allocation of the inlined Find_unk (index, type and id take
// other registers)
// inferred: moves a team under another one
void TeamAIManager::OnParent_unk(const TeamMsgIn_unk* in) {
    TeamAI_unk* team = Find_unk(in->mType, in->mId);
    if (team != 0) {
        TeamAI_unk* parent = Find_unk(in->m10, in->m14);
        if (parent != 0) {
            team->Detach_unk();
            TeamNode_unk* node = parent;
            if (node == 0) {
                node = &mRoot;
            }
            team->Attach_unk(node);
            fn_805AE3C4(team);
            fn_805AE3C4(parent);
        }
    }
}

// NONMATCHING: 99.47% - register allocation of the inlined Find_unk (index, type and id take
// other registers)
void TeamAIManager::OnMembers_unk(const TeamMsgIn_unk* in) {
    int id = in->mId;
    int type = in->mType;
    TeamAI_unk* team = Find_unk(type, id);
    if (team != 0) {
        fn_805AEAC0(team);
        for (int i = 0; i < team->mNumMembers; i++) {
            team->mMembers[i]->mTeam = team;
        }
    }
}

// NONMATCHING: 98.98% - register allocation of the inlined Find_unk (index, type and id take
// other registers)
void TeamAIManager::OnMsg21_unk(const TeamMsgIn_unk* in) {
    int id = in->mId;
    int type = in->mType;
    TeamAI_unk* team = Find_unk(type, id);
    if (team != 0) {
        TeamMsg3_unk msg;
        msg.mType = 0x21;
        msg.m04 = in->m10;
        msg.m08 = in->m14;
        msg.m0C = in->m18;
        fn_805AEA70(team, &msg);
    }
}

// NONMATCHING: 99.38% - register allocation of the inlined Find_unk (index, type and id take
// other registers)
void TeamAIManager::OnChara_unk(const TeamMsgIn_unk* in) {
    int id = in->mId;
    int type = in->mType;
    TeamAI_unk* team = Find_unk(type, id);
    if (team != 0) {
        TeamMsgChara_unk msg;
        msg.mType = 0x22;
        switch (in->m10) {
        case 0:
            msg.mChara = lbl_8087F8A0->mHead;
            break;
        case 1:
            msg.mChara = NpcManager::sInstance->Find_unk(in->m14);
            break;
        case 2:
            msg.mChara = EnemyManager::sInstance->Find_unk(in->m14);
            break;
        case 3:
            msg.mChara = lbl_8087F8A0->Find_unk(in->m14);
            break;
        }
        msg.m08 = in->m18;
        fn_805AEA70(team, &msg);
    }
}

// NONMATCHING: 98.98% - register allocation of the inlined Find_unk (index, type and id take
// other registers)
void TeamAIManager::OnMsg24_unk(const TeamMsgIn_unk* in) {
    int id = in->mId;
    int type = in->mType;
    TeamAI_unk* team = Find_unk(type, id);
    if (team != 0) {
        TeamMsg24_unk msg;
        msg.mType = 0x24;
        msg.mBody.m05 = in->m10;
        msg.mBody.m00 = in->m18;
        msg.mBody.m04 = in->m14;
        fn_805B2028(team, &msg.mBody);
    }
}

// inferred: state names (Shift-JIS: shoki, taiki, batoru: tsuujou / kougeki / shubi)
// inferred: the literal of the second state comes first in .rodata; an earlier use (this unused
// table, never emitted) gives that order
static const char* const sWaitName_unk[] = {"\x91\xD2\x8B\x40"};
static const char* const sStateNames[] = {
    "\x8F\x89\x8A\xFA",
    "\x91\xD2\x8B\x40",
    "\x83\x6F\x83\x67\x83\x8B\x81\x46\x92\xCA\x8F\xED",
    "\x83\x6F\x83\x67\x83\x8B\x81\x46\x8D\x55\x8C\x82",
    "\x83\x6F\x83\x67\x83\x8B\x81\x46\x8E\xE7\x94\xF5",
    0,
};

// NONMATCHING: 98.39% - x/z float registers swapped and operand order of the ring buffer compare
// inferred: debug display of the selected team
void TeamAIManager::vf14() {
    if (mDebug != 0) {
        if (fn_800A555C(lbl_8087EF70, 0, 2) != 0) {
            if (--mSelected < 0) {
                mSelected = mTeams.mSize - 1;
            }
        } else if (fn_800A555C(lbl_8087EF70, 0, 3) != 0) {
            int n = mTeams.mSize;
            if (++mSelected >= n) {
                mSelected = 0;
            }
        }
    }
    if (mDebug != 0) {
        void* draw = lbl_8087EEB0;
        if (draw != 0) {
            char buf[0x100];
            float z = -100.0f;
            float x = 10.0f;
            float y = 10.0f;
            float w = 250.0f;
            float h = 200.0f;
            float line = 15.0f;
            float size = 12.0f;
            fn_80060D58(draw, x, y, z, w, h, 0x30000000);
            float label1 = 5.0f + x;
            float mid = 0.5f * w + x;
            float label2 = 5.0f + mid;
            float value1 = 82.0f + x;
            float value2 = 82.0f + mid;
            TeamAI_unk* team = &mTeams.mData[mSelected];
            y = line;
            fn_800616C0(draw, label1, y, z, size, size, "[Team]", 0xFFFFFFFF, 0.0f, 1, 1, 0);
            y += line;
            fn_800616C0(draw, label1, y, z, size, size, "Index", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            sprintf(buf, "%d", mSelected);
            fn_800616C0(draw, value1, y, z, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            y += line;
            fn_800616C0(draw, label1, y, z, size, size, "Type", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            int type = team->mType;
            if (type == 1) {
                fn_800616C0(draw, value1, y, z, size, size, "NPC", 0xD0FFFFFF, 0.0f, 1, 1, 0);
            } else if (type == 2) {
                fn_800616C0(draw, value1, y, z, size, size, "Enemy", 0xD0FFFFFF, 0.0f, 1, 1, 0);
            } else if (type == 3) {
                fn_800616C0(draw, value1, y, z, size, size, "Friend", 0xD0FFFFFF, 0.0f, 1, 1, 0);
            }
            fn_800616C0(draw, label2, y, z, size, size, "ID", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            sprintf(buf, "%d", team->GetId_unk());
            fn_800616C0(draw, value2, y, z, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            y += line;
            fn_800616C0(draw, label1, y, z, size, size, "State", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            sprintf(buf, "%s", sStateNames[team->mState]);
            fn_800616C0(draw, value1, y, z, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            y += line;
            fn_800616C0(draw, label1, y, z, size, size, "AIType", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            sprintf(buf, "%d", team->mAIType);
            fn_800616C0(draw, value1, y, z, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            y += line;
            fn_800616C0(draw, label1, y, z, size, size, "MemberNum", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            sprintf(buf, "%d", team->mNumMembers);
            fn_800616C0(draw, value1, y, z, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            y += line;
            fn_800616C0(draw, label1, y, z, size, size, "BattleMode", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            sprintf(buf, "%d", team->mBattleMode);
            fn_800616C0(draw, value1, y, z, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            fn_800616C0(draw, label2, y, z, size, size, "AreaID", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            sprintf(buf, "%d", team->mAreaID);
            fn_800616C0(draw, value2, y, z, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            y += line;
            fn_800616C0(draw, label1, y, z, size, size, "TgtCtrlMode", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            sprintf(buf, "%d", *team->mTgtCtrlMode.Last_unk());
            fn_800616C0(draw, value1, y, z, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            y += line;
            fn_800616C0(draw, label1, y, z, size, size, "TargetMode", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            sprintf(buf, "%d", team->mTargetMode);
            fn_800616C0(draw, value1, y, z, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            fn_800616C0(draw, label2, y, z, size, size, "TargetType", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            sprintf(buf, "%d", team->mTargetType);
            fn_800616C0(draw, value2, y, z, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            y += line;
            y += 5.0f;
            fn_800616C0(draw, label1, y, z, size, size, "HPRatio", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            sprintf(buf, "%.2f", fn_805AE454(team));
            fn_800616C0(draw, value1, y, z, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            y += line;
            fn_800616C0(draw, label1, y, z, size, size, "LdrHPRatio", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            sprintf(buf, "%.2f", fn_805AE74C(team));
            fn_800616C0(draw, value1, y, z, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            y += line;
            fn_800616C0(draw, label1, y, z, size, size, "MbrRatio", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            sprintf(buf, "%.2f", fn_805AE5E8(team));
            fn_800616C0(draw, value1, y, z, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);

            float height = 17.0f;
            int i;
            TeamChara_unk** members = team->mMembers;
            for (i = 0; i < team->mNumMembers; i++) {
                TeamChara_unk* chara = members[i];
                atn::Vector3 pos = chara->mPos;
                pos.y += height;
                if (chara == team->GetLeader_unk()) {
                    fn_800638B0(draw, 0.0f, 7.0f, &(pos + atn::Vector3(0.0f, 15.0f, 0.0f)), &pos, 0xFFFFFF00);
                } else {
                    fn_800638B0(draw, 0.0f, 7.0f, &(pos + atn::Vector3(0.0f, 15.0f, 0.0f)), &pos, 0xFF00FFFF);
                }
            }
            if (fn_805AE7B4(team) > 0) {
                TeamAI_unk* other;
                for (int i = 0; i < mTeams.mSize; i++) {
                    other = &mTeams.mData[i];
                    if ((int)team->mType != (int)other->mType && fn_805AE7B4(other) > 0) {
                        TeamChara_unk** members = other->mMembers;
                        for (int j = 0; j < other->mNumMembers; j++) {
                            atn::Vector3 pos = members[j]->mPos;
                            pos.y += height;
                            fn_800638B0(draw, 0.0f, 7.0f, &(pos + atn::Vector3(0.0f, 15.0f, 0.0f)), &pos,
                                        0xFFFF00FF);
                        }
                    }
                }
            }
        }
    }
}

// inferred: team of a type and id, null if missing (auto-inlined in the handlers above)
TeamAI_unk* TeamAIManager::Find_unk(int type, int id) {
    for (int i = 0; i < mTeams.mSize; i++) {
        TeamAI_unk* team = &mTeams.mData[i];
        TeamData_unk* data;
        if (type == team->mType && id == ((data = team->m218) != 0 ? data->mId : 0)) {
            return team;
        }
    }
    return 0;
}

TeamAIManager* lbl_8087FA00;
int lbl_8087FA04;
