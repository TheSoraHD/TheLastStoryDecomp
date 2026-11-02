// game/GameSaveData_unk.cpp (0x803B9804-0x803BEBAC): collection and restoration of the save data
// (GameSaveData_unk, the 0x134A8-byte game state) and of the header of a save slot (GameSaveSlot_unk).
#include "game/GameSaveData.h"
#include "game/GameSaveLoadBackgroundCommon.h"
#include "game/GameField_unk.h"
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
#include "db/WeaponParam_unk.h"
#include "db/EquipParam_unk.h"

extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" void* memcpy(void* dst, const void* src, unsigned long n);
extern "C" unsigned int fn_800DC500(const void* data, unsigned long len); // inferred: checksum
extern "C" void fn_804A1C4C(GameMap_unk* map); // inferred: refreshes the map after a load
extern "C" int lbl_8087DD64;                   // inferred: format of the loaded save data
extern "C" GameConfig_unk* lbl_8087F0A8;
extern "C" PlayerManager* lbl_8087F8A0;
extern "C" int fn_8021150C(int code);                     // inferred: index of an item code in the bag
extern "C" int fn_80206BE4(WeaponParamEntry_unk* weapon);  // inferred: item code of a weapon
extern "C" int fn_8020EF80(EquipParamEntry_unk* equip);    // inferred: item code of a protector

void GameSaveFlags_unk::fn_803B9C98() {
    GameField_unk* field = lbl_8087F430;
    if (field) {
        fn_80370094(field, mState);
        fn_80370320(field, 0x11B, 0, 1);
        for (int i = 0; i < 0x1000; i++) {
            if (i == 0x1E) {
                fn_80370320(field, i, 1, 1);
            } else {
                fn_80370320(field, i, mFlags[i], 1);
            }
        }
        if (mFlags[0x11B] > 0) {
            lbl_8087F0A8->mCurrentLevel = 3;
        } else {
            lbl_8087F0A8->mCurrentLevel = 1;
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

GameSaveParty_unk::GameSaveParty_unk() {
    mMask = 0;
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
                Inventory::instance_->mPlayers[i].m2F0 = 0;
                Inventory::instance_->mPlayers[i].m380 = 0;
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
    if (lbl_8087F430 && fn_80373148(lbl_8087F430)) {
        fn_804A1C4C(fn_80373148(lbl_8087F430));
    }
}

// NONMATCHING: 91.5% - the original loads the saved id before the gimmick id in the search loop
void GameSaveData_unk::fn_803BE6B8() {
    m4F5C.fn_803BA620();
    mParty.fn_803BAD6C();
    mEnemies.fn_803BB0E0();
    for (GimmickCommon* gimmick = GimmickManager::instance_->mHead; gimmick; gimmick = gimmick->m5C) {
        int i = FindGimmick_unk(gimmick);
        if (i != 0x200 && (mGimmicks[i].mFlags & 1) == 1) {
            gimmick->vf70(mGimmicks[i].mState);
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
