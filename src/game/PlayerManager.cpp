#include "game/GameScene.h"
#include "game/PlayerManager.h"
#include "game/GameTask.h"
#include "game/TeamAIManager.h"
#include "atn/Memory.h"

extern "C" PlayerManager* lbl_8087F8A0; // inferred: the singleton

// inferred: multiplayer session (lbl_8087F610); fn_804EB870 gives the local player
struct MpPlayer_unk {
    unsigned char m00[0xB0];
    int mB0; // inferred: character of the local player
};
extern "C" void* lbl_8087F610;
extern "C" MpPlayer_unk* fn_804EB870(void* session);

// inferred: weak getter 0x8013C504 of game/CharaTask.cpp, inlined here
inline GameScene* GameTask::fn_8013C504() {
    return m10D8;
}
extern "C" GameTask* lbl_8087F430;
extern "C" inline GameTask* fn_80121F00() {
    return lbl_8087F430;
}
class MapTask;

// inferred: list of objects of 0x37C bytes updated when the leader changes (lbl_8087FA00)

extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
extern "C" int fn_80219544(int member); // inferred: character id of a party member
extern "C" void fn_800D2338(atn::Task* task);

PlayerManager* PlayerManager::fn_80549D8C(atn::Task* parent) {
    if (parent != 0 && lbl_8087F8A0 == 0) {
        lbl_8087F8A0 = new ((MemoryTag)3, "", "", 0) PlayerManager(parent);
    }
    return lbl_8087F8A0;
}

PlayerManager::PlayerManager(atn::Task* parent) : CharaManager<PlayerTask>(parent), m54(0), m5C(0) {
    m5C = mMenu.Load("PlayerManager");
}

PlayerManager::~PlayerManager() {
    lbl_8087F8A0 = 0;
}

int PlayerManager::vf0C() {
    return fn_800D3FA4() != 0;
}

// inferred: accessors of the party id of a character (m58; names invented)
inline int GetId_unk(PlayerTask* p) {
    return p->m58;
}
inline void SetId_unk(PlayerTask* p, int id) {
    p->m58 = id;
}

// inferred: the id is declared at the top between head and player (the original reuses r30 for the id
// and the head); a separate local in the else branch fixes the player register there
void PlayerManager::fn_80549F14(int mode) {
    PlayerTask* head;
    int id;
    PlayerTask* player;
    if (mode >= 3 && mode <= 4) {
        id = 700000;
        MpPlayer_unk* mp = fn_804EB870(lbl_8087F610);
        if (mp != 0) {
            id += mp->mB0;
        }
        player = PlayerTask::fn_8054A3DC(this, 0, (void*)id);
        player->fn_800D246C(1);
        player->m58 = -1;
        while (player != (head = mHead)) {
            Remove_unk(head);
            head->mNext = 0;
            Add_unk(head);
        }
    } else {
        PlayerTask* p = PlayerTask::fn_8054A3DC(this, 0, (void*)1);
        p->fn_800D246C(1);
        p->m58 = -1;
    }
    m54 = mHead;
}

void PlayerManager::vf1C() {
    lbl_8087F430->GetCurrentMap();
    for (PlayerTask* player = mHead; player != 0; player = player->mNext) {
        if (mHead == player) {
            player->fn_8016DF3C(0);
        } else {
            player->fn_8016DF3C(3);
        }
        player->fn_8016EB48(0, 0, 0);
        player->fn_800D246C(0);
        player->fn_80176ACC();
    }
}

// inferred: the table of the field is read through its weak getters (fn_80121F00, GameTask::fn_8013C504),
// the ids through accessors and the counter of the team loop is declared before the manager (registers
// of the original)
void PlayerManager::fn_8054A09C(PlayerTask* player, int swap) {
    if (mHead != player) {
        mHead->fn_8016DF3C(3);
        player->fn_8016DF3C(0);
        if ((player->mFlags & 4) == 4) {
            player->fn_80176ACC();
        }
        if (lbl_8087F430 != 0) {
            lbl_8087F430->fn_80371320(player);
        }
        if (swap != 0) {
            int tmp = GetId_unk(mHead);
            SetId_unk(mHead, GetId_unk(player));
            SetId_unk(player, tmp);
            GameScene* table = fn_80121F00()->fn_8013C504();
            for (unsigned int i = 0; i < table->m148.mSize; i++) {
                if (table->m148.mData[i].mLeader == mHead) {
                    table->m148.mData[i].mLeader = player;
                } else if (table->m148.mData[i].mLeader == player) {
                    table->m148.mData[i].mLeader = mHead;
                }
                for (int j = 0; j < 32; j++) {
                    if (table->m148.mData[i].mCharas[j] == mHead) {
                        table->m148.mData[i].mCharas[j] = player;
                    } else if (table->m148.mData[i].mCharas[j] == player) {
                        table->m148.mData[i].mCharas[j] = mHead;
                    }
                }
            }
            int k;
            TeamAIManager* teams = lbl_8087FA00;
            for (k = 0; k < teams->mTeams.mSize; k++) {
                teams->mTeams[k].fn_805AE3C4();
            }
        }
        if (mHead->m50 == 1) {
            mHead->fn_801546F4(0, lbl_807C7030, 0.0f);
            if (swap == 0) {
                mHead->fn_801765D8();
            }
        }
        if (player->m50 == 1) {
            player->m58 = -1;
            if (swap == 0) {
                player->fn_80176ACC();
            }
        }
        Remove_unk(player);
        player->mNext = mHead;
        mHead = player;
        mCount++;
    }
}

PlayerTask* PlayerManager::fn_8054A33C(int member) {
    int id = fn_80219544(member);
    for (PlayerTask* player = mHead; player != 0; player = player->mNext) {
        if (id == player->m50) {
            return player;
        }
    }
    return 0;
}

void PlayerManager::fn_8054A398() {
    for (PlayerTask* player = mHead->mNext; player != 0; player = player->mNext) {
        fn_800D2338(player);
    }
}
