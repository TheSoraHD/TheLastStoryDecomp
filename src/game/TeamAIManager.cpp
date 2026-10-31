#include "atn/Task.h"
#include "atn/Array_unk.h"
#include "atn/LH_Binary.h"
#include "atn/Memory.h"
#include "game/CharaManager.h"
#include "game/EnemyManager.h"
#include "game/NpcManager.h"
#include "game/ResHandle.h"
#include "math/Vec.h"
#include "atn/DrawHelper.h"
#include "game/TeamAIManager.h"

extern "C" int sprintf(char* buf, const char* fmt, ...);

// inferred: debug drawing (as in game/GameSaveTask.cpp)
// inferred: line from `from` to `to`
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
        atn::DrawHelper* draw = lbl_8087EEB0;
        if (draw != 0) {
            char buf[0x100];
            float z = -100.0f;
            float x = 10.0f;
            float y = 10.0f;
            float w = 250.0f;
            float h = 200.0f;
            float line = 15.0f;
            float size = 12.0f;
            draw->fn_80060D58(x, y, z, w, h, 0x30000000);
            float label1 = 5.0f + x;
            float mid = 0.5f * w + x;
            float label2 = 5.0f + mid;
            float value1 = 82.0f + x;
            float value2 = 82.0f + mid;
            TeamAI_unk* team = &mTeams.mData[mSelected];
            y = line;
            draw->fn_800616C0(label1, y, z, size, size, "[Team]", 0xFFFFFFFF, 0.0f, 1, 1, 0);
            y += line;
            draw->fn_800616C0(label1, y, z, size, size, "Index", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            sprintf(buf, "%d", mSelected);
            draw->fn_800616C0(value1, y, z, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            y += line;
            draw->fn_800616C0(label1, y, z, size, size, "Type", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            int type = team->mType;
            if (type == 1) {
                draw->fn_800616C0(value1, y, z, size, size, "NPC", 0xD0FFFFFF, 0.0f, 1, 1, 0);
            } else if (type == 2) {
                draw->fn_800616C0(value1, y, z, size, size, "Enemy", 0xD0FFFFFF, 0.0f, 1, 1, 0);
            } else if (type == 3) {
                draw->fn_800616C0(value1, y, z, size, size, "Friend", 0xD0FFFFFF, 0.0f, 1, 1, 0);
            }
            draw->fn_800616C0(label2, y, z, size, size, "ID", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            sprintf(buf, "%d", team->GetId_unk());
            draw->fn_800616C0(value2, y, z, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            y += line;
            draw->fn_800616C0(label1, y, z, size, size, "State", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            sprintf(buf, "%s", sStateNames[team->mState]);
            draw->fn_800616C0(value1, y, z, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            y += line;
            draw->fn_800616C0(label1, y, z, size, size, "AIType", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            sprintf(buf, "%d", team->mAIType);
            draw->fn_800616C0(value1, y, z, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            y += line;
            draw->fn_800616C0(label1, y, z, size, size, "MemberNum", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            sprintf(buf, "%d", team->mNumMembers);
            draw->fn_800616C0(value1, y, z, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            y += line;
            draw->fn_800616C0(label1, y, z, size, size, "BattleMode", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            sprintf(buf, "%d", team->mBattleMode);
            draw->fn_800616C0(value1, y, z, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            draw->fn_800616C0(label2, y, z, size, size, "AreaID", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            sprintf(buf, "%d", team->mAreaID);
            draw->fn_800616C0(value2, y, z, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            y += line;
            draw->fn_800616C0(label1, y, z, size, size, "TgtCtrlMode", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            sprintf(buf, "%d", *team->mTgtCtrlMode.Last_unk());
            draw->fn_800616C0(value1, y, z, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            y += line;
            draw->fn_800616C0(label1, y, z, size, size, "TargetMode", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            sprintf(buf, "%d", team->mTargetMode);
            draw->fn_800616C0(value1, y, z, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            draw->fn_800616C0(label2, y, z, size, size, "TargetType", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            sprintf(buf, "%d", team->mTargetType);
            draw->fn_800616C0(value2, y, z, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            y += line;
            y += 5.0f;
            draw->fn_800616C0(label1, y, z, size, size, "HPRatio", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            sprintf(buf, "%.2f", fn_805AE454(team));
            draw->fn_800616C0(value1, y, z, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            y += line;
            draw->fn_800616C0(label1, y, z, size, size, "LdrHPRatio", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            sprintf(buf, "%.2f", fn_805AE74C(team));
            draw->fn_800616C0(value1, y, z, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            y += line;
            draw->fn_800616C0(label1, y, z, size, size, "MbrRatio", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            sprintf(buf, "%.2f", fn_805AE5E8(team));
            draw->fn_800616C0(value1, y, z, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);

            float height = 17.0f;
            int i;
            TeamChara_unk** members = team->mMembers;
            for (i = 0; i < team->mNumMembers; i++) {
                TeamChara_unk* chara = members[i];
                atn::Vector3 pos = chara->mPos;
                pos.y += height;
                if (chara == team->GetLeader_unk()) {
                    draw->fn_800638B0(0.0f, 7.0f, (pos + atn::Vector3(0.0f, 15.0f, 0.0f)), pos, 0xFFFFFF00);
                } else {
                    draw->fn_800638B0(0.0f, 7.0f, (pos + atn::Vector3(0.0f, 15.0f, 0.0f)), pos, 0xFF00FFFF);
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
                            draw->fn_800638B0(0.0f, 7.0f, (pos + atn::Vector3(0.0f, 15.0f, 0.0f)), pos, 0xFFFF00FF);
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
