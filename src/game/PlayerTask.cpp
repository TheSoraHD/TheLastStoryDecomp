#include "game/PlayerTask.h"
#include "atn/Memory.h"
#include "game/CharaManager.h"
#include "game/FriendArganan.h"

extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
extern "C" CharaInfo_unk* fn_8020924C(int id);     // inferred: character table entry

PlayerTask* PlayerTask::fn_8054A3DC(atn::Task* parent, int type, void* data) {
    if (parent != 0) {
        CharaInfo_unk* info = fn_8020924C((int)data);
        if (info != 0 && info->m24 == 0x42) {
            return new (3, "", "", 0) FriendArganan(parent, type, data);
        }
        return new (3, "", "", 0) PlayerTask(parent, type, data);
    }
    return 0;
}

PlayerTask::PlayerTask(atn::Task* parent, int type, void* data) : BattleCharaTask(parent, type, data), mNext(0) {
    mId = lbl_8087F8A0->NextId_unk();
    lbl_8087F8A0->Add_unk(this);
    mF18 = mF14;
    mF14 = 0;
}

PlayerTask::~PlayerTask() {
    if (lbl_8087F8A0 != 0) {
        lbl_8087F8A0->Remove_unk(this);
    }
}

void PlayerTask::fn_8054A640(PlayerTask* player) {
    PlayerTask* cur = this;
    while (cur->mNext != 0) {
        cur = cur->mNext;
    }
    cur->mNext = player;
}

void PlayerTask::fn_8054A65C(PlayerTask* player) {
    PlayerTask* prev = 0;
    PlayerTask* cur = this;
    while (cur != player) {
        prev = cur;
        cur = cur->mNext;
    }
    if (cur == player && prev != 0) {
        prev->mNext = cur->mNext;
    }
}

PlayerTask* PlayerTask::fn_8054A68C() {
    CharaManager<PlayerTask>* manager = lbl_8087F8A0;
    if (manager != 0) {
        PlayerTask* player = manager->mHead;
        if (player != 0) {
            if (this == player) {
                while (player->mNext != 0) {
                    player = player->mNext;
                }
            } else {
                while (player->mNext != this) {
                    player = player->mNext;
                }
            }
            return player;
        }
    }
    return 0;
}
