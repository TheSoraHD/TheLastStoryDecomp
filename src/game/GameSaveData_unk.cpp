// game/GameSaveData_unk.cpp (0x803B9804-0x803BEBAC): collection and restoration of the save data
// (GameSaveData_unk, the 0x134A8-byte game state) and of the header of a save slot (GameSaveSlot_unk).
#include "game/GameSaveData.h"
#include "game/GameTask.h"
#include "game/GameSaveLoadBackgroundCommon.h"
#include "game/GameConfig_unk.h"
#include "game/PlayerManager.h"
#include "game/PlayerTask.h"
#include "game/CharaTask.h"
#include "game/Inventory.h"
#include "game/GameTimer.h"
#include "game/EnvSeManager.h"
#include "game/GimmickManager.h"
#include "game/GimmickCommon.h"
#include "game/GameBgm.h"
#include "game/EnemyManager.h"
#include "game/EnemyTask.h"
#include "db/WeaponParam_unk.h"
#include "db/EquipParam_unk.h"
#include "db/ItemParam_unk.h"
#include "game/MapTask.h"
#include "game/GameScene.h"
#include "game/GameEventScript.h"
#include "game/GameUI.h"
#include "game/GameChoiceWindow.h"
#include "game/TeamAIManager.h"
#include "game/ChaseManager.h"
#include "game/NpcManager.h"
#include "game/NpcTask.h"
#include "game/MapPartsLocatorSet.h"

extern "C" GameTask* lbl_8087F430; // inferred: the field

extern "C" int rand();
extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" void* memcpy(void* dst, const void* src, unsigned long n);
extern "C" unsigned int fn_800DC500(const void* data, unsigned long len); // inferred: checksum
extern "C" int lbl_8087DD64;                   // inferred: format of the loaded save data
extern "C" GameConfig_unk* lbl_8087F0A8;
extern "C" PlayerManager* lbl_8087F8A0;
extern "C" int fn_8021150C(int code);                     // inferred: index of an item code in the bag
extern "C" int fn_80206BE4(WeaponParamEntry_unk* weapon);  // inferred: item code of a weapon
extern "C" int fn_8020EF80(EquipParamEntry_unk* equip);    // inferred: item code of a protector

// inferred: inline members of include/game/GameSaveData.h (defined here so that the units that include
// the header keep their label numbering)
inline MP::SaveElemBPart_unk::SaveElemBPart_unk() : mId(-1), m02(0), m04(-1.0f), m08(1.0f), m0C(1.0f), m10(-1.0f) {}

inline GameSaveFlags_unk::GameSaveFlags_unk() : mState(0), mSpotCount(0) {
    memset(mFlags, 0, sizeof(mFlags));
    memset(mWork, 0, sizeof(mWork));
}

inline GameSaveEquipExtra_unk::GameSaveEquipExtra_unk() {
    for (int i = 0; i < 8; i++) {
        mIds[i] = -1;
    }
    memset(m20, 0, sizeof(m20));
}

inline GameSaveGimmick_unk::GameSaveGimmick_unk() : mFlags(0) {}

inline void GameSaveGimmick_unk::Set_unk(GimmickCommon* gimmick) {
    mFlags |= 1;
    mState = gimmick->m54;
    mId = gimmick->mId;
    m04 = gimmick->m4C;
}

inline int GameSaveGimmicks_unk::FindGimmick_unk(GimmickCommon* gimmick) {
    int i;
    for (i = 0; i < 0x200; i++) {
        if ((mEntries[i].mFlags & 1) != 1) {
            break;
        }
        if (mEntries[i].mId == gimmick->mId && mEntries[i].m04 == gimmick->m4C) {
            break;
        }
    }
    return i;
}

inline GameSavePlayer_unk::GameSavePlayer_unk() : mFlags(0) {}

inline GameSaveEnemy_unk::GameSaveEnemy_unk() : mFlags(0) {}

inline GameSaveSettings_unk::GameSaveSettings_unk() {
    memset(m8C, 0, sizeof(m8C));
}

// inferred: flags kept when a new game starts from a cleared save (fn_803BA120); in .data
static int sKeptFlags_unk[273] = {
    0x1E, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94,
    0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E,
    0x9F, 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8,
    0xA9, 0xAB, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4,
    0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE,
    0xBF, 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xDA, 0xDB,
    0xDC, 0xDD, 0xDE, 0xDF, 0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5,
    0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
    0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9,
    0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF, 0x100, 0x101, 0x102, 0x103,
    0x104, 0x105, 0x106, 0x107, 0x108, 0x109, 0x10A, 0x10B, 0x10C, 0x10D,
    0x10E, 0x10F, 0x110, 0x111, 0x112, 0x113, 0x114, 0x115, 0x116, 0x117,
    0x118, 0x12C, 0x12D, 0x12E, 0x12F, 0x130, 0x131, 0x132, 0x133, 0x134,
    0x135, 0x136, 0x137, 0x138, 0x139, 0x13A, 0x13B, 0x13C, 0x13D, 0x13E,
    0x13F, 0x140, 0x141, 0x142, 0x143, 0x144, 0x145, 0x146, 0x147, 0x148,
    0x149, 0x14A, 0x14B, 0x14C, 0x14D, 0x14E, 0x14F, 0x150, 0x151, 0x152,
    0x153, 0x154, 0x155, 0x156, 0x157, 0x158, 0x159, 0x15A, 0x15B, 0x15C,
    0x15D, 0x15E, 0x15F, 0x160, 0x161, 0x162, 0x163, 0x164, 0x165, 0x166,
    0x167, 0x168, 0x169, 0x16A, 0x16B, 0x16C, 0x16D, 0x16E, 0x16F, 0x170,
    0x171, 0x172, 0x173, 0x174, 0x175, 0x176, 0x177, 0x178, 0x179, 0x17A,
    0x17B, 0x17C, 0x190, 0x191, 0x192, 0x193, 0x194, 0x195, 0x196, 0x197,
    0x198, 0x199, 0x19A, 0x19B, 0x19C, 0x19D, 0x19E, 0x19F, 0x1A0, 0x1A1,
    0x1A2, 0x1A3, 0x1AE, 0x1AF, 0x1B0, 0x1B1, 0x1B2, 0x1B3, 0x1B4, 0x1B5,
    0x1B6, 0x1B7, 0x1B8, 0x1B9, 0x1BA, 0x1BB, 0x1BC, 0x1BD, 0x1BE, 0x1BF,
    0x1C0, 0x1C1, 0x1D6, 0x1D7, 0x1D8, 0x1D9, 0x1DA, 0x1DB, 0x1DC, 0x1DD,
    0x1DE, 0x1DF, 0x1E0, 0x1E1, 0x1E2, 0x1E3, 0x1E4, 0x1E5, 0x1E6, 0x1E7,
    0x1E8, 0x1E9, 0x1EA, 0x1EB, 0x1EC, 0x1ED, 0x1EE, 0x1EF, 0x1F0, 0x1F1,
    0x1F2, 0x1F3, 0x1F4,
};

// inferred: true if `flag` is in sKeptFlags_unk (name invented)
static inline bool IsKeptFlag_unk(int flag) {
    for (int i = 0; i < 273; i++) {
        if (flag == sKeptFlags_unk[i]) {
            return true;
        }
    }
    return false;
}

// inferred: MSL map::operator[] of the finished events (as in game/GameEventScript.cpp)
inline bool& GameEventScript::Done_unk(const int& key) {
    ScriptDoneNode_unk* pred;
    bool left;
    bool leftmost;
    ScriptDoneNode_unk* parent = mDone.FindInsertPos_unk(key, pred, left, leftmost);
    ScriptDonePair_unk* v;
    if (pred == 0 || (v = &pred->mValue, v->first < key)) {
        v = &mDone.InsertNode_unk(parent, left, leftmost, ScriptDonePair_unk(key, bool()))->mValue;
    }
    return v->second;
}

// inferred: next node of the finished events (MSL tree iterator increment; name invented)
static inline ScriptDoneNode_unk* NextDone_unk(ScriptDoneNode_unk* node) {
    ScriptDoneNode_unk* next = node->mRight;
    if (next != 0) {
        while (next->mLeft != 0) {
            next = next->mLeft;
        }
        node = next;
    } else {
        ScriptDoneNode_unk* parent;
        while (node != (parent = (ScriptDoneNode_unk*)(node->mParent & ~1))->mLeft) {
            node = parent;
        }
        node = parent;
    }
    return node;
}

// inferred: entry before `cmd` in the ring of orders of a team, 0 at the end (name invented)
static inline TeamCmd_unk* PrevCmd_unk(TeamRing_unk<TeamCmd_unk, 32>& ring, TeamCmd_unk* cmd) {
    unsigned int pos = ((char*)cmd - (char*)ring.mData) / sizeof(TeamCmd_unk);
    int i = pos > 31 ? -1 : (int)pos;
    if (i == -1) {
        return 0;
    }
    i = ring.Prev_unk(i);
    if (i == ring.mTail) {
        return 0;
    }
    return &ring.mData[i];
}

// NONMATCHING: 97.87% - register allocation (r25-r31) and the copy of the next node of the finished events
// inferred: collects the flags, the work values, the map, the open events, the team orders that write values,
// the chase, the characters of the scene and the draw flags of the map parts
void GameSaveFlags_unk::fn_803B9804() {
    GameTask* field = lbl_8087F430;
    mState = field->m10D0;
    for (int i = 0; i < 0x1000; i++) {
        mFlags[i] = field->fn_80370174(i);
    }
    for (int i = 0; i < 0x100; i++) {
        mWork[i] = field->fn_80370A78(i);
    }
    MapTask* map = field->GetCurrentMap();
    mMap = map->mType;
    mPoint = map->mId;
    mArea = map->mSubId;
    GameScene* scene = field->m10D8;
    GameEventScript* script = scene->m134;
    int count = 0;
    for (ScriptDoneNode_unk* node = script->mDone.mFirst; node != script->mDone.End_unk(); node = NextDone_unk(node)) {
        if (script->fn_803ACD50(node->mValue.first) == 0) {
            mSpots[count] = node->mValue.first;
            count++;
        }
    }
    mSpotCount = count;
    GameUI* ui = lbl_8087F490;
    if (ui != 0 && ui->m2640 != 0 && ui->m2640->m71 != 0) {
        int mode = ui->m2640->m74;
        if (mode == 1) {
            mFlags[ui->m2640->m78] = 1;
        } else if (mode == 0) {
            mWork[ui->m2640->m78] = 1;
        }
    }
    TeamAIManager* teams = lbl_8087FA00;
    if (teams != 0) {
        for (unsigned int i = 0; i < teams->mTeams.size(); i++) {
            TeamRing_unk<TeamCmd_unk, 32>& ring = teams->mTeams.mData[i].m10;
            for (TeamCmd_unk* cmd = ring.Last_unk(); cmd != 0; cmd = PrevCmd_unk(ring, cmd)) {
                if (cmd->m00 == 0x20) {
                    TeamVarOp_unk* op = (TeamVarOp_unk*)&cmd->m04;
                    int value = op->mValue;
                    if (op->mSrc == 2) {
                        value = mFlags[value];
                    } else if (op->mSrc == 1) {
                        value = mWork[value];
                    } else if (op->mSrc == 3) {
                        value = rand() % op->mValue + 1;
                    }
                    if (op->mDst == 1) {
                        mFlags[op->mVar] = value;
                    } else if (op->mDst == 0) {
                        mWork[op->mVar] = value;
                    }
                }
            }
        }
    }
    if (lbl_8087F098 != 0) {
        mFlags[0x397] = lbl_8087F098->mGoalId;
        mFlags[0x398] = lbl_8087F098->mShowGoal;
    }
    int n = 0;
    for (unsigned int i = 0; i < scene->m80.size(); i++) {
        if (n >= 0x100) {
            break;
        }
        CharaTask* chara = NpcManager::sInstance->Find_unk(scene->m80.mData[i].mId);
        if (chara != 0) {
            mCharas[n].mId = scene->m80.mData[i].mId;
            mCharas[n].mType = 1;
            mCharas[n].mFlag = chara->mFlagB20;
            n++;
        }
    }
    for (unsigned int i = 0; i < scene->m88.size(); i++) {
        if (n >= 0x100) {
            break;
        }
        CharaTask* chara = EnemyManager::sInstance->Find_unk(scene->m88.mData[i].mId);
        if (chara != 0) {
            mCharas[n].mId = scene->m88.mData[i].mId;
            mCharas[n].mType = 2;
            mCharas[n].mFlag = chara->mFlagB20;
            n++;
        }
    }
    for (unsigned int i = 0; i < scene->m90.size(); i++) {
        if (n >= 0x100) {
            break;
        }
        CharaTask* chara = lbl_8087F8A0->Find_unk(scene->m90.mData[i].mId);
        if (chara != 0) {
            mCharas[n].mId = scene->m90.mData[i].mId;
            mCharas[n].mType = 3;
            mCharas[n].mFlag = chara->mFlagB20;
            n++;
        }
    }
    mCharaCount = n;
    mLeader = lbl_8087F8A0->mHead->m58;
    memcpy(m4D18, script->m154, sizeof(m4D18));
}

void GameSaveFlags_unk::fn_803B9C98() {
    GameTask* field = lbl_8087F430;
    if (field) {
        field->fn_80370094(mState);
        field->fn_80370320(0x11B, 0, 1);
        for (int i = 0; i < 0x1000; i++) {
            if (i == 0x1E) {
                field->fn_80370320(i, 1, 1);
            } else {
                field->fn_80370320(i, mFlags[i], 1);
            }
        }
        if (mFlags[0x11B] > 0) {
            lbl_8087F0A8->mCurrentLevel = 3;
        } else {
            lbl_8087F0A8->mCurrentLevel = 1;
        }
    }
}

// NONMATCHING: 92.41% - the original guards the loop of the finished events with a signed test and keeps the
// map in a saved register; the chase manager is loaded into saved registers before the flag reads
// inferred: restores the work values, the finished events, the leader, the chase and the draw flags of the
// map parts
void GameSaveFlags_unk::fn_803B9D78() {
    GameTask* field = lbl_8087F430;
    for (int i = 0; i < 0x100; i++) {
        field->fn_80370AE4(i, mWork[i]);
    }
    GameScene* scene = field->m10D8;
    GameEventScript* script = scene->m134;
    ScriptTree_unk<ScriptDoneNode_unk>& done = script->mDone;
    if (done.mRoot != 0) {
        done.Destroy_unk(done.mRoot);
        done.mSize = 0;
        done.mRoot = 0;
        done.mFirst = done.End_unk();
    }
    if (mSpotCount > 0) {
        for (int i = 0; i < mSpotCount; i++) {
            script->Done_unk(mSpots[i]) = true;
        }
    }
    if (mLeader != -1 && lbl_8087F8A0->Find_unk(mLeader) != 0) {
        PlayerManager* players = lbl_8087F8A0;
        players->fn_8054A09C(players->Find_unk(mLeader), 0);
    }
    if (lbl_8087F098 != 0) {
        lbl_8087F098->fn_80183618(field->fn_80370174(0x397));
        lbl_8087F098->fn_80183774(field->fn_80370174(0x398));
    }
    memcpy(script->m154, m4D18, sizeof(m4D18));
    for (int i = 0; i < 0x40; i++) {
        if (m4D18[i].m00 == 0) {
            break;
        }
        if ((m4D18[i].m04 & 2) == 2) {
            scene->m64->fn_804A0E90(m4D18[i].m00, m4D18[i].m04 & 1);
        }
        if ((m4D18[i].m04 & 8) == 8) {
            MapPartsLocatorSet* set = (MapPartsLocatorSet*)scene->m64->fn_804A0BCC(4, m4D18[i].m00);
            if (set != 0) {
                unsigned int flags = set->mFlags;
                set->fn_8049B72C((m4D18[i].m04 & 4) == 4 ? flags | 0x400 : flags & ~0x400);
            }
        }
    }
}

// inferred: entry of the characters of the map (GameScene); type 1: npc, 2: enemy, 3: player
extern "C" GameSaveChara_unk* fn_803BA0CC(GameSaveFlags_unk* flags, int type, int id) {
    for (unsigned int i = 0; i < flags->mCharaCount; i++) {
        if (type == flags->mCharas[i].mType && id == flags->mCharas[i].mId) {
            return &flags->mCharas[i];
        }
    }
    return 0;
}

void GameSaveFlags_unk::fn_803BA120(int mode) {
    if (mode == 2) {
        int level = mFlags[0x11B];
        mState = 0x18A88;
        for (int i = 0; i < 0x1000; i++) {
            if (!IsKeptFlag_unk(i)) {
                mFlags[i] = 0;
            }
        }
        level++;
        mFlags[0x11B] = level > 1 ? 1 : level;
        mFlags[0x387] = 0;
        mFlags[0x388] = 0;
        mFlags[0x38B] = 1;
        mFlags[0x385] = 0;
        mFlags[0x386] = 0;
        mFlags[0x38F] = 0;
        mFlags[0x390] = 100;
        mFlags[0x397] = 0;
        mFlags[0x398] = 0;
        mFlags[0] = 1;
        mFlags[0x1E] = 1;
        mFlags[0x2C] = 1;
        memset(mWork, 0, sizeof(mWork));
        memset(mSpots, 0, sizeof(mSpots));
        mSpotCount = 0;
        mMap = 2;
        mPoint = 4;
        mArea = 1;
        for (int i = 0; i < 0x100; i++) {
            mCharas[i].mId = 0;
        }
        mCharaCount = 0;
        mLeader = -1;
        for (int i = 0; i < 64; i++) {
            m4D18[i].m00 = 0;
        }
    }
}

// NONMATCHING: register numbering only (the original keeps the entry offset in r4 and the player in r5)
void GameSavePlayers_unk::fn_803BA408() {
    for (int i = 0; i < 16; i++) {
        mEntries[i].mFlags = 0;
    }
    int n = 0;
    for (PlayerTask* player = lbl_8087F8A0->mHead; player; player = player->mNext, n++) {
        mEntries[n].mFlags |= 1;
        if (player->mStatus.IsState_unk(0x20)) {
            mEntries[n].mFlags |= 2;
        }
        if (player->mFlag31) {
            mEntries[n].mFlags |= 4;
        }
        if (player->IsFlag54C_unk(0x2000)) {
            mEntries[n].mFlags |= 8;
        }
        mEntries[n].m04 = player->m58;
        mEntries[n].mPos = player->mPos;
        mEntries[n].mRot = player->mRot;
        mEntries[n].m02 = player->mC58.mC0;
        mEntries[n].m20 = player->mC58.mB4;
        mEntries[n].m03 = player->m674;
        if (player->m55C == 7 || (player->m55C == 6 && player->m564 == 7)) {
            mEntries[n].mFlags |= 0x10;
            RouteTable_unk* table = lbl_8087F430 ? (RouteTable_unk*)lbl_8087F430->m10D8 : 0; // inferred: RouteTable_unk is a view of GameScene
            if (table) {
                RouteWalker_unk& route = player->mRoute;
                mEntries[n].mRouteMode = route.m44;
                mEntries[n].mRouteFlag = route.m48;
                switch (route.m44) {
                case 0:
                    mEntries[n].mRouteTarget = table->mPoints[route.mPointB - 1].m00;
                    break;
                case 1:
                    mEntries[n].mRouteTarget = table->mLinks[route.mLink - 1].m00;
                    break;
                case 3:
                    mEntries[n].mRouteTarget = route.mPlace;
                    break;
                case 6:
                    if (route.mTarget && (route.mTarget->mType == 0 || player->mType == 3)) {
                        mEntries[n].mRouteTarget = route.mTarget->m58;
                        mEntries[n].mRouteRadius = route.mRadius;
                    }
                    break;
                }
            }
        }
    }
}

// inferred: state of a player from its entry (inlined in fn_803BA620: its locals are on the stack in the order
// of an inline; name invented)
static inline void RestorePlayer_unk(const GameSavePlayer_unk* entry, PlayerTask* player) {
    player->mPos = entry->mPos;
    player->mRot = entry->mRot;
    player->mC58.mC0 = entry->m02;
    player->mC58.mB4 = entry->m20;
    player->m58C = 0;
    if ((entry->mFlags & 2) == 2) {
        player->vf44();
        atn::Vector3 dir(0.0f, 0.0f, 1.0f);
        Mtx rot;
        PSMTXRotRad(rot, 'y', player->mRot.y);
        PSMTXMultVec(rot, &dir, &dir);
        player->fn_8015E7A0(atn::Vector3(-dir.x, -dir.y, -dir.z), -1, 0);
    }
    signed char level = entry->m03;
    if (level >= 0) {
        player->fn_8014DEE4(player->m50 == 1 ? 0 : level, 0, 0);
    } else {
        player->fn_8014EEC4(0);
    }
    if ((entry->mFlags & 4) == 4) {
        player->fn_80145334();
        GameSceneRoute_unk ledge;
        if (lbl_8087F430->fn_8036DAF4(&ledge, player, 1, 0)) {
            player->fn_80153698(&ledge, 0);
        }
    }
    if ((entry->mFlags & 8) == 8) {
        player->m54C |= 0x2000;
    }
    if ((entry->mFlags & 0x10) == 0x10) {
        switch (entry->mRouteMode) {
        case 0:
            player->fn_8016F824(entry->mRouteTarget, entry->mRouteFlag);
            break;
        case 1:
            player->fn_8016FDCC(entry->mRouteTarget, entry->mRouteFlag);
            break;
        case 3:
            player->fn_8017039C(entry->mRouteTarget, entry->mRouteFlag);
            break;
        case 6:
            player->fn_80171420(lbl_8087F8A0->Find_unk(entry->mRouteTarget), entry->mRouteFlag,
                                (float)entry->mRouteRadius);
            break;
        }
    }
}

// NONMATCHING: 99.83% - the original loads the id of the entry before the one of the player in the search
// inferred: restores the players: each one takes the entry with its id (or a free entry with id 0 when it has
// none), its position, state, ledge and route
void GameSavePlayers_unk::fn_803BA620() {
    if (lbl_8087F8A0 == 0) {
        return;
    }
    for (PlayerTask* player = lbl_8087F8A0->mHead; player; player = player->mNext) {
        int index = -1;
        for (int i = 0; i < 16; i++) {
            if ((mEntries[i].mFlags & 1) == 1 && mEntries[i].m04 == player->m58) {
                index = i;
                break;
            }
        }
        if (index < 0) {
            for (int i = 0; i < 16; i++) {
                if ((mEntries[i].mFlags & 1) == 1 && mEntries[i].m04 == 0 && player->m58 == -1) {
                    index = i;
                    break;
                }
            }
        }
        if (index < 0) {
            continue;
        }
        RestorePlayer_unk(&mEntries[index], player);
    }
}


GameSaveParty_unk::GameSaveParty_unk() {
    mMask = 0;
}

void GameSaveParty_unk::fn_803BAAE0() {
    for (unsigned int i = 0; i < 7; i++) {
        // inferred: the header declares the argument as a name, but it is the member index here
        PlayerTask* player = lbl_8087F8A0->fn_8054A33C(i);
        if (player) {
            mStatus[i] = player->mStatus;
            mMask |= 1 << i;
        } else {
            mStatus[i] = *Inventory::instance_->GetPlayerStatusInfo(i);
            mMask |= 1 << i;
        }
    }
}

void GameSaveParty_unk::fn_803BAD6C() {
    if (lbl_8087F8A0) {
        for (PlayerTask* player = lbl_8087F8A0->mHead; player; player = player->mNext) {
            int index = fn_80219558(player->m50);
            if (index == 9 || index == 10 || index == 12) {
                index = 1;
            }
            if (0 <= index && index < 7 && (mMask & (1 << index)) == (1 << index)) {
                memcpy(&player->mStatus, &mStatus[index], sizeof(CharaStatusBase));
                player->mStatus.mChara = player;
                player->mStatus.fn_80133B30(0);
                player->mStatus.fn_8012B3E8();
                player->mStatus.fn_8012B988();
                player->mStatus.fn_8012D8B8();
                player->mStatus.m224 = player->mStatus.mParamB.m98;
            }
        }
    }
}

int GameSaveParty_unk::fn_803BAE54(GameSavePacketEquip_unk* equip, int chara, int bit) const {
    if (equip->mValid) {
        int index = fn_80219558(chara);
        if (0 <= index && index < 7) {
            const CharaStatusBase& status = mStatus[index];
            equip->mMarked = (status.m230 & (1 << bit)) == (1 << bit);
            equip->mColor = status.m231;
            return 1;
        }
    }
    return 0;
}

void GameSaveEnemies_unk::fn_803BAEF8() {
    for (int i = 0; i < 64; i++) {
        mEntries[i].mFlags = 0;
    }
    GameSaveEnemy_unk* entry = mEntries;
    for (EnemyTask* enemy = EnemyManager::sInstance->mHead; enemy; enemy = enemy->mNext) {
        entry->mFlags |= 1;
        if (enemy->mStatus.IsState_unk(0x20)) {
            entry->mFlags |= 2;
        }
        if (enemy->mFlag31) {
            entry->mFlags |= 4;
        }
        if (enemy->IsFlag54C_unk(0x2000)) {
            entry->mFlags |= 8;
        }
        if ((enemy->m137C & 0x80) == 0x80 && (enemy->mFlagB20 || enemy->mMotion.m104.fn_80089B5C())) {
            entry->mFlags |= 0x10;
        }
        if ((enemy->m137C & 0x80) != 0x80 && (enemy->mFlagB22 || !enemy->mMotion.m104.fn_80089B5C())) {
            entry->mFlags |= 0x20;
        }
        if (enemy->mFlag15) {
            entry->mFlags |= 0x40;
        }
        entry->m04 = enemy->m58;
        entry->m02 = enemy->mC58.mC0;
        entry->m08 = enemy->mC58.mB4;
        entry->m03 = enemy->m674;
        entry++;
    }
}

void GameSaveEnemies_unk::fn_803BB0E0() {
    GameSaveEnemy_unk* entry = mEntries;
    for (EnemyTask* enemy = EnemyManager::sInstance->mHead; enemy; enemy = enemy->mNext) {
        if ((entry->mFlags & 1) == 1) {
            enemy->mC58.mC0 = entry->m02;
            enemy->mC58.mB4 = entry->m08;
            if ((entry->mFlags & 2) == 2) {
                enemy->mStatus.fn_8012DD70();
                enemy->fn_801765D8();
            }
            enemy->fn_8014DEE4(entry->m03, 0, 0);
            if ((entry->mFlags & 4) == 4) {
                enemy->fn_80145334();
                GameSceneRoute_unk ledge;
                if (lbl_8087F430->fn_8036DAF4(&ledge, enemy, 1, 0)) {
                    enemy->fn_80153698(&ledge, 0);
                }
            }
            if ((entry->mFlags & 8) == 8) {
                enemy->m54C |= 0x2000;
            }
            if ((entry->mFlags & 0x10) == 0x10) {
                enemy->mFlagB20 = 1;
                enemy->mMotion.fn_8008B978(0);
            }
            if ((entry->mFlags & 0x20) == 0x20) {
                enemy->mFlagB22 = 1;
                enemy->mFlagB20 = 0;
            }
            if ((entry->mFlags & 0x40) == 0x40) {
                enemy->mFlag15 = 1;
            }
            entry++;
        }
    }
}

void GameSaveGimmicks_unk::fn_803BB248() {
    for (int i = 0; i < 0x200; i++) {
        mEntries[i].mFlags = 0;
    }
    int n = 0;
    for (GimmickCommon* gimmick = GimmickManager::instance_->mHead; gimmick && n < 0x200; gimmick = gimmick->m5C) {
        switch (gimmick->m50) {
        case 1:
        case 2:
        case 4:
        case 5:
        case 6:
        case 8:
        case 17:
        case 21:
        case 22:
        case 24:
        case 28:
        case 34:
        case 36:
        case 38:
        case 43:
        case 45:
        case 46:
        case 51:
            mEntries[n++].Set_unk(gimmick);
            break;
        case 7:
            if (gimmick->mId % 1000 == 1) {
                mEntries[n++].Set_unk(gimmick);
            }
            break;
        }
    }
}

// NONMATCHING: the original compares the array end once (cr0 reused after the first element)
MP::SaveElemB_unk::SaveElemB_unk() : m000(-1), m004(-1), m008(-1), m00C(-1) {
    m290 = 0;
    m294 = 0;
}

// inferred: the equipment sets of the inventory are copied one by one into the save data and back (struct
// assignments of 0x40 bytes); the save data types them as GameSaveEquipExtra_unk
static inline Inventory::Equip_unk& AsEquip_unk(GameSaveEquipExtra_unk& e) {
    return *(Inventory::Equip_unk*)&e;
}
// inferred: view of the parts of an upgraded equipment, copied whole by the assignment of MP::SaveElemB_unk
struct SaveBlock280_unk {
    MP::SaveElemBPart_unk m[32];
};

extern "C" unsigned long wcslen(const wchar_t* s);
extern "C" wchar_t* wmemcpy(wchar_t* dst, const wchar_t* src, unsigned long n);

// inferred: assignment of an upgraded equipment (MP::SaveElemB_unk::operator= in the original: the name at
// 0x294 is a static wide string, copied with wcslen + wmemcpy)
static inline void CopyUpgrade_unk(MP::SaveElemB_unk& dst, const MP::SaveElemB_unk& src) {
    dst.m000 = src.m000;
    dst.m004 = src.m004;
    dst.m008 = src.m008;
    dst.m00C = src.m00C;
    *(SaveBlock280_unk*)dst.m010 = *(const SaveBlock280_unk*)src.m010;
    dst.m290 = src.m290;
    const wchar_t* name = (const wchar_t*)&src.m294;
    wchar_t* to = (wchar_t*)&dst.m294;
    if (name != to) {
        wmemcpy(to, name, wcslen(name) + 1);
    }
}

// NONMATCHING: 94.37% - register allocation (two more saved registers: r22-r31 against r24-r31) and
// the addressing of the equipment copies
// inferred: collects the inventory: money, bag, costume sets, shown equipment, upgraded equipment,
// equipment, items to identify, party order and the play time
void GameSaveInventory_unk::fn_803BB584() {
    Inventory* inv = Inventory::instance_;
    m0000 = inv->mMoney;
    for (int i = 0; i < 0x600; i++) {
        Inventory::Item_unk* item = inv->fn_80444804(i);
        if (item != 0) {
            mItems[i].mCode = item->mCode;
            mItems[i].mCount = item->mCount;
            mItems[i].mAge = item->m08;
            mItems[i].m05 = item->m0C;
        }
    }
    for (int i = 0; i < 16; i++) {
        AsEquip_unk(m0004[i]) = inv->mEquipSetA[i];
    }
    for (int i = 0; i < 7; i++) {
        AsEquip_unk(mExtras[i]) = inv->mEquipShown[i];
    }
    for (int i = 0; i < 28; i++) {
        CopyUpgrade_unk(mUpgrades[i], inv->mUpgrades[i]);
    }
    for (int i = 0; i < 7; i++) {
        mEquips[i] = inv->mEquip[i];
    }
    for (int i = 0; i < 0x100; i++) {
        mIdentify[i] = inv->fn_8044D530(i) - 100000;
    }
    // inferred: the order is 19 words from Inventory::mOrder (it goes on into m64C8)
    const int* order = inv->mOrder;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 6; j++) {
            mOrder[i][j] = order[i * 6 + j];
        }
    }
    mOrder18 = order[18];
    if (lbl_8087F430 != 0) {
        m94C8 = lbl_8087F430->m5744;
    }
}

extern "C" void fn_80212790();                                     // inferred: clears the found items
extern "C" int fn_80211940(ItemParamEntry_unk* item, int count);   // inferred: adds an item, returns its code

// NONMATCHING: 87.31% - register allocation and addressing of the copies (the bag loops keep pointers
// in other registers)
// inferred: restores the inventory: money, bag (format 10: 8-byte items, older: 0xC-byte items), and from
// format 3 the costume sets, the shown equipment and the upgraded equipment; then the equipment (the
// slots of unknown protectors and weapons are given back as items), the items to identify, the party
// order and the play time
void GameSaveInventory_unk::fn_803BC2D4() {
    Inventory* inv = Inventory::instance_;
    if (inv == 0) {
        return;
    }
    inv->fn_8044D6E0(m0000);
    fn_80212790();
    for (int i = 0; i < 0x600; i++) {
        Inventory::Item_unk* item = Inventory::instance_->fn_80444804(i);
        if (item != 0 && item->mCode >= 1000000) {
            item->mCode = -1;
            item->mCount = 0;
            item->m08 = 0;
        }
    }
    if (lbl_8087DD64 < 10) {
        for (int i = 0; i < 0x500; i++) {
            if (i < 0x400) {
                const GameSaveItemOld_unk* old = &((const GameSaveItemOld_unk*)mItems)[i];
                Inventory::Item_unk* item = inv->fn_80444744(old->mCode);
                if (item != 0) {
                    item->mCount = old->mCount;
                    item->m08 = old->m06;
                    item->m0C = old->m08;
                }
            } else {
                const GameSaveItemOld_unk* old = &mOldItems[i - 0x400];
                Inventory::Item_unk* item = inv->fn_80444744(old->mCode);
                if (item != 0) {
                    item->mCount = old->mCount;
                    item->m08 = old->m06;
                    item->m0C = old->m08;
                }
            }
        }
    } else {
        for (int i = 0; i < 0x600; i++) {
            Inventory::Item_unk* item = inv->fn_80444744(mItems[i].mCode);
            if (item != 0) {
                item->mCount = mItems[i].mCount;
                item->m08 = mItems[i].mAge;
                item->m0C = mItems[i].m05;
            } else if (mItems[i].mCode >= 1000000) {
                int code = fn_80211940(fn_80211480(mItems[i].mCode % 1000000), mItems[i].mCode / 1000000);
                if (code > 0) {
                    Inventory::Item_unk* slot = inv->fn_80444804(fn_8021150C(code));
                    if (slot != 0) {
                        slot->mCode = mItems[i].mCode;
                        slot->mCount = mItems[i].mCount;
                        slot->m08 = mItems[i].mAge;
                        slot->m0C = mItems[i].m05;
                    }
                }
            }
        }
    }
    if (lbl_8087DD64 >= 3) {
        for (int i = 0; i < 16; i++) {
            inv->mEquipSetA[i] = AsEquip_unk(m0004[i]);
        }
        for (int i = 0; i < 7; i++) {
            inv->mEquipShown[i] = AsEquip_unk(mExtras[i]);
        }
        for (int i = 0; i < 28; i++) {
            CopyUpgrade_unk(inv->mUpgrades[i], mUpgrades[i]);
        }
        for (int i = 0; i < 28; i++) {
            CopyUpgrade_unk(inv->mUpgrades[i], mUpgrades[i]);
        }
    }
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 4; j++) {
            mEquips[i].mProtectors[j].m04 = i * 4 + j;
        }
        inv->mEquip[i] = mEquips[i];
        // inferred: four weapon slots (the last two in m30)
        const Inventory::EquipSlot_unk* weapons = mEquips[i].mWeapons;
        for (int j = 0; j < 4; j++) {
            int id = mEquips[i].mProtectors[j].mId;
            if (id >= 1000 && fn_8020ED84(j, id) == 0) {
                fn_80211940(fn_80211480(fn_8020EF80(fn_8020ED84(j, id % 1000))), mEquips[i].mProtectors[j].mId / 1000);
            }
            id = weapons[j].mId;
            if (id >= 1000 && fn_80206B14(0, id) == 0) {
                fn_80211940(fn_80211480(fn_80206BE4(fn_80206B14(0, id % 1000))), weapons[j].mId / 1000);
            }
        }
    }
    inv->fn_8044D4F0();
    for (int i = 0; i < 0x100; i++) {
        int code = mIdentify[i] + 100000;
        inv->fn_8044D404(code);
    }
    int* order = inv->mOrder;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 6; j++) {
            order[i * 6 + j] = mOrder[i][j];
        }
    }
    order[18] = mOrder18;
    if (lbl_8087F430 != 0) {
        lbl_8087F430->m5744 = m94C8;
    }
}

int GameSaveInventory_unk::fn_803BD0FC(GameSavePacketEquip_unk* equip, int chara, int slot) const {
    int index = fn_80219558(chara);
    if (0 <= index && index < 7) {
        const Inventory::Equip_unk& equipment = mEquips[index];
        equip->mSlot = slot;
        equip->mItem = equipment.mProtectors[slot].mId;
        equip->mValid = 0;
        int upgrade = equipment.mProtectors[slot].m04;
        if (upgrade >= 0) {
            equip->m0C = mUpgrades[upgrade].m00C;
            equip->mExtra = mExtras[index];
            equip->mValid = 1;
        }
        return 1;
    }
    return 0;
}

int GameSaveInventory_unk::fn_803BD22C(GameSavePacketEquip_unk* equip, int chara, int slot) const {
    int index = fn_80219558(chara);
    if (0 <= index && index < 7) {
        equip->mSlot = slot;
        equip->mItem = mEquips[index].mWeapons[slot].mId;
        return 1;
    }
    return 0;
}

int GameSaveInventory_unk::fn_803BD2B0(int code, int count) {
    int index = fn_8021150C(code);
    if (index < 0) {
        return 0;
    }
    if (fn_80206B9C(code) || fn_8020EF04(code)) {
        int have = fn_803BD49C(code);
        if (have + count >= 10) {
            count = 10 - have;
        }
    }
    if (count <= 0) {
        return 0;
    }
    if (index < 0x500) {
        int n = mItems[index].mCount + count;
        mItems[index].mCount = n < 99 ? n : 99;
        for (int i = 0; i < 0x600; i++) {
            mItems[i].mAge++;
        }
        mItems[index].mAge = 0;
    } else {
        int n = mOldItems[index - 0x500].mCount + count;
        mOldItems[index - 0x500].mCount = n < 99 ? n : 99;
    }
    return 1;
}

int GameSaveInventory_unk::fn_803BD49C(int code) const {
    int count = 0;
    for (int i = 0; i < 0x600; i++) {
        if (code == mItems[i].mCode % 1000000) {
            count += mItems[i].mCount;
        }
    }
    if (fn_80206B9C(code)) {
        const Inventory::Equip_unk* equips = mEquips;
        for (int c = 0; c < 7; c++) {
            for (int w = 0; w < 2; w++) {
                WeaponParamEntry_unk* weapon = fn_80206B14(0, equips[c].mWeapons[w].mId);
                if (c == 0 && w == 1) {
                    weapon = fn_80206B14(1, equips[c].mWeapons[w].mId);
                }
                if (weapon && code == fn_80206BE4(weapon) % 1000000) {
                    count++;
                }
            }
        }
    }
    if (fn_8020EF04(code)) {
        const Inventory::Equip_unk* equips = mEquips;
        for (int c = 0; c < 7; c++) {
            for (int p = 0; p < 2; p++) {
                EquipParamEntry_unk* equip = fn_8020ED84(p, equips[c].mProtectors[p].mId);
                if (equip && code == fn_8020EF80(equip)) {
                    count++;
                }
            }
        }
    }
    return count;
}

void GameSaveInventory_unk::fn_803BD6C0(int mode) {
    if (mode == 2) {
        for (int i = 0; i < 4; i++) {
            mEquips[2].mProtectors[i].mId = 0x1D;
        }
        mEquips[2].mWeapons[0].mId = 0x1F8;
        for (int i = 0; i < 0x600; i++) {
            ItemParamEntry_unk* item = fn_802114E0(i);
            if (item && item->mBC == 0 && item->mId != 0x1C2 && item->mId != 0x1C3 && item->mId != 0x14A &&
                item->mId != 0x19D) {
                if (i < 0x500) {
                    mItems[i].mCount = 0;
                } else {
                    mOldItems[i - 0x500].mCount = 0;
                }
            }
        }
    }
}

// NONMATCHING: the original copies the player settings of GameConfig_unk (0x244-0x334) into a local, sets
// mAlwaysBirdview in it and copies it back member by member (no type for that block yet); the small arrays of
// SetConfig go through word copies
// inferred: restores the settings (format 7 and later), then the defaults of the settings added by later
// formats
void GameSaveSettings_unk::fn_803BD79C() {
    if (lbl_8087DD64 >= 7) {
        lbl_8087F0A8->mAlwaysBirdview = mAlwaysBirdview & 1;
        SetConfig::sInstance->m24 = m08;
        SetConfig::sInstance->mAttackType = mAttackType;
        SetConfig::sInstance->mDiveType = mDiveType;
        SetConfig::sInstance->mInTacviewType = mInTacviewType;
        SetConfig::sInstance->mGatheringType = mGatheringType;
        memcpy(SetConfig::sInstance->m38, m1C, sizeof(m1C));
        SetConfig::sInstance->mAttackTurnSpeed = mAttackTurnSpeed;
        SetConfig::sInstance->mTurnSpeed = mTurnSpeed;
        SetConfig::sInstance->mRollbackSpeed = mRollbackSpeed;
        SetConfig::sInstance->mCameraRotSpeed = mCameraRotSpeed;
        SetConfig::sInstance->m50 = m34;
        SetConfig::sInstance->mRotDelay = mRotDelay;
        SetConfig::sInstance->mBattleDrawHateLine = mBattleDrawHateLine;
        SetConfig::sInstance->mDrawEnemyHP = mDrawEnemyHP;
        SetConfig::sInstance->mDrawEnemyPopup = mDrawEnemyPopup;
        SetConfig::sInstance->mDrawFriendPopup = mDrawFriendPopup;
        memcpy(SetConfig::sInstance->m68, m4C, sizeof(m4C));
        SetConfig::sInstance->mShowDialog = mShowDialog;
        SetConfig::sInstance->mShowTutorial = mShowTutorial;
        SetConfig::sInstance->mEventSkipType = mEventSkipType;
        SetConfig::sInstance->mEventCameraControl = mEventCameraControl;
        memcpy(SetConfig::sInstance->m88, m6C, sizeof(m6C));
        SetConfig::sInstance->fn_8057F1B4(&mSound);
        SetConfig::sInstance->fn_8057F198(m88);
    }
    if (lbl_8087DD64 < 8) {
        SetConfig::sInstance->mAttackTurnSpeed = 1.0f;
        SetConfig::sInstance->mTurnSpeed = 1.0f;
        SetConfig::sInstance->mRollbackSpeed = 0.8f;
        SetConfig::sInstance->mCameraRotSpeed = 1.0f;
        SetConfig::sInstance->mRotDelay = 31.0f;
        SetConfig::sInstance->m50 = 16;
    }
    if (lbl_8087DD64 < 14) {
        SetConfig::sInstance->mTurnSpeed = 1.0f;
        SetConfig::sInstance->mRotDelay = 1.0f;
    }
    if (lbl_8087DD64 < 15) {
        SetConfig::sInstance->mAttackTurnSpeed = 1.0f;
    }
}

GameSaveData_unk::GameSaveData_unk() : m0000(0) {}

// NONMATCHING: the original copies SetConfig 0x24..0xA8 in seven struct assignments (one load of
// SetConfig::sInstance each); SetConfig declares single members there (game/SetConfig.h)
void GameSaveData_unk::fn_803BDEE4() {
    m0000 = 1;
    if (GameTimer::sInstance) {
        GameTimer::sInstance->fn_803CE7E0();
    }
    if (GameBgm::sInstance) {
        GameBgm::sInstance->fn_8037D808();
    }
    if (lbl_8087F430 && lbl_8087F430->GetCurrentMap()) {
        lbl_8087F430->GetCurrentMap()->fn_804A1D2C();
    }
    if (lbl_8087F418) {
        lbl_8087F418->fn_803608A8();
    }
    mFlags.fn_803B9804();
    mPlayers.fn_803BA408();
    mParty.fn_803BAAE0();
    mEnemies.fn_803BAEF8();
    mGimmicks.fn_803BB248();
    mInventory.fn_803BB584();
    mSettings.mMagic = 0xDEADBEEF;
    mSettings.mAlwaysBirdview = 0;
    int mode = mFlags.mFlags[0x391];
    mSettings.mAlwaysBirdview = lbl_8087F0A8->mAlwaysBirdview != 0;
    mSettings.m08 = SetConfig::sInstance->m24;
    mSettings.mAttackType = SetConfig::sInstance->mAttackType;
    mSettings.mDiveType = SetConfig::sInstance->mDiveType;
    mSettings.mInTacviewType = SetConfig::sInstance->mInTacviewType;
    mSettings.mGatheringType = SetConfig::sInstance->mGatheringType;
    memcpy(mSettings.m1C, SetConfig::sInstance->m38, sizeof(mSettings.m1C));
    mSettings.mAttackTurnSpeed = SetConfig::sInstance->mAttackTurnSpeed;
    mSettings.mTurnSpeed = SetConfig::sInstance->mTurnSpeed;
    mSettings.mRollbackSpeed = SetConfig::sInstance->mRollbackSpeed;
    mSettings.mCameraRotSpeed = SetConfig::sInstance->mCameraRotSpeed;
    mSettings.m34 = SetConfig::sInstance->m50;
    mSettings.mRotDelay = SetConfig::sInstance->mRotDelay;
    mSettings.mBattleDrawHateLine = SetConfig::sInstance->mBattleDrawHateLine;
    mSettings.mDrawEnemyHP = SetConfig::sInstance->mDrawEnemyHP;
    mSettings.mDrawEnemyPopup = SetConfig::sInstance->mDrawEnemyPopup;
    mSettings.mDrawFriendPopup = SetConfig::sInstance->mDrawFriendPopup;
    memcpy(mSettings.m4C, SetConfig::sInstance->m68, sizeof(mSettings.m4C));
    mSettings.mShowDialog = SetConfig::sInstance->mShowDialog;
    mSettings.mShowTutorial = SetConfig::sInstance->mShowTutorial;
    mSettings.mEventSkipType = SetConfig::sInstance->mEventSkipType;
    mSettings.mEventCameraControl = SetConfig::sInstance->mEventCameraControl;
    memcpy(mSettings.m6C, SetConfig::sInstance->m88, sizeof(mSettings.m6C));
    mSettings.mSound = SetConfig::sInstance->mSound;
    mSettings.m88 = SetConfig::sInstance->mA4;
    mFlags.fn_803BA120(mode);
    if (mode == 2) {
        for (int i = 0; i < 16; i++) {
            mPlayers.mEntries[i].mFlags = 0;
        }
    }
    if (mode == 2) {
        mParty.mStatus[2].m1A8.m08 = mParty.mStatus[0].m1A8.m08;
        mParty.mStatus[2].mParamA.m78 = mParty.mStatus[0].mParamA.m78;
        mParty.mStatus[2].mParamB.m78 = mParty.mStatus[0].mParamB.m78;
        mParty.mStatus[2].m228 = 0.0f;
    }
    if (mode == 2) {
        for (int i = 0; i < 64; i++) {
            mEnemies.mEntries[i].mFlags = 0;
        }
    }
    if (mode == 2) {
        for (int i = 0; i < 0x200; i++) {
            mGimmicks.mEntries[i].mFlags = 0;
        }
    }
    mInventory.fn_803BD6C0(mode);
    if (mode == 2) {
        mSettings.mEventSkipType = 1;
        mSettings.mEventCameraControl = 1;
    }
    mChecksum = fn_800DC500(this, (char*)&mChecksum - (char*)this);
}

extern "C" void fn_803BE550(GameSaveData_unk* data) {
    data->mChecksum = fn_800DC500(data, (char*)&data->mChecksum - (char*)data);
}

// NONMATCHING: 92.8% - loop registers (the original keeps the stored zero in a copy of the counter)
void GameSaveData_unk::fn_803BE590() {
    mInventory.fn_803BC2D4();
    if (Inventory::instance_) {
        for (unsigned int i = 0; i < 7; i++) {
            if ((mParty.mMask & (1 << i)) == (1 << i)) {
                memcpy(&Inventory::instance_->mPlayers[i], &mParty.mStatus[i], sizeof(CharaStatusBase));
                Inventory::instance_->mPlayers[i].mChara = 0;
                Inventory::instance_->mPlayers[i].mRefs.mCount = 0;
            }
        }
    }
    mFlags.fn_803B9C98();
    mSettings.fn_803BD79C();
    if (GameTimer::sInstance) {
        GameTimer::sInstance->fn_803CE768();
    }
    if (lbl_8087F418) {
        lbl_8087F418->fn_803608A4();
    }
}

void GameSaveData_unk::fn_803BE670() {
    mFlags.fn_803B9D78();
    if (lbl_8087F430 && lbl_8087F430->GetCurrentMap()) {
        lbl_8087F430->GetCurrentMap()->fn_804A1C4C();
    }
}

// NONMATCHING: 91.5% - the original loads the saved id before the gimmick id in the search loop
void GameSaveData_unk::fn_803BE6B8() {
    mPlayers.fn_803BA620();
    mParty.fn_803BAD6C();
    mEnemies.fn_803BB0E0();
    for (GimmickCommon* gimmick = GimmickManager::instance_->mHead; gimmick; gimmick = gimmick->m5C) {
        int i = mGimmicks.FindGimmick_unk(gimmick);
        if (i != 0x200 && (mGimmicks.mEntries[i].mFlags & 1) == 1) {
            gimmick->vf70(mGimmicks.mEntries[i].mState);
        }
    }
    if (GameBgm::sInstance) {
        GameBgm::sInstance->fn_8037D758();
    }
}

// inferred: called after the save data is loaded; empty in the release build
extern "C" void fn_803BE850(GameSaveData_unk* data) {}

// inferred: nonzero if the checksum of a save data of format 12 or later is wrong
extern "C" int fn_803BE854(GameSaveData_unk* data) {
    if (lbl_8087DD64 >= 12 && data->mChecksum != fn_800DC500(data, (char*)&data->mChecksum - (char*)data)) {
        return 1;
    }
    return 0;
}

GameSaveSlot_unk::GameSaveSlot_unk() {
    m00 = 0;
}

void GameSaveSlot_unk::fn_803BE8C0(const GameSaveData_unk* data, const SaveTime_unk* time) {
    int i = 0;
    m00 = 1;
    m04 = 15;
    m08 = time->m00[0];
    m0C = time->m00[1];
    m10 = time->m00[2];
    m14 = time->m00[3];
    m18 = time->m00[4];
    m1C = time->m00[5];
    m20 = time->m00[6];
    m24 = time->m00[7];
    m28 = time->m00[8];
    m2C = time->m00[9];
    m30 = data->mFlags.mMap;
    m31 = data->mFlags.mArea;
    m32 = data->mFlags.mPoint;
    m34 = data->mFlags.m4D10;
    m70 = data->mInventory.m94C8;
    m74 = data->mFlags.mState;
    m78 = data->mFlags.mFlags[0x11B];
    m7C = data->mFlags.mFlags[0x11D];
    m80 = lbl_8087F0A8->m194 != 0;
    memset(&mParty, 0, sizeof(mParty));
    for (PlayerTask* player = lbl_8087F8A0->mHead; player; player = player->mNext) {
        mParty.mCharas[i].mId = player->m50;
        mParty.mCharas[i].mLevel = player->mStatus.mParamB.m78;
        if (++i >= 7) {
            break;
        }
    }
    mB4 = fn_800DC500(this, (char*)&mB4 - (char*)this);
}

// inferred: copies the party of a slot header into the header of the work buffer (MP/SaveLoad)
extern "C" void fn_803BEA08(GameSaveSlot_unk* dst, GameSaveSlot_unk* src) {
    for (int i = 0; i < 7; i++) {
        dst->mParty.mCharas[i].mId = src->mParty.mCharas[i].mId;
        dst->mParty.mCharas[i].mLevel = src->mParty.mCharas[i].mLevel;
    }
}

extern "C" void fn_803BEA7C(GameSaveSlot_unk* slot) {
    slot->mB4 = fn_800DC500(slot, (char*)&slot->mB4 - (char*)slot);
}

int GameSaveSlot_unk::fn_803BEAB4() {
    if (lbl_8087F0A8->m194) {
        return m00 && m04 >= 9 && m80;
    }
    if (m04 >= 9) {
        return m00 && m04 >= 6 && !m80;
    }
    return m00 && m04 >= 6;
}

// inferred: nonzero if the checksum of a slot header of format 13 or later is wrong
extern "C" int fn_803BEB54(GameSaveSlot_unk* slot) {
    if (slot->m04 >= 13 && slot->mB4 != fn_800DC500(slot, (char*)&slot->mB4 - (char*)slot)) {
        return 1;
    }
    return 0;
}
