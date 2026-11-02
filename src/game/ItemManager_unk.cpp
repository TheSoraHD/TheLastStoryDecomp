// game/ItemManager_unk.cpp (0x80442F10-0x8044D710): code of Inventory (the RSO names the class;
// the file name is unknown). Only the short functions at the end of the file are written so far.
#include "game/Inventory.h"
#include "game/CharaTask.h"

// inferred: the bag of items, the money and the equipment of the party
Inventory* Inventory::instance_;

void Inventory::fn_8044D028() {
    mExpEntries.clear();
}

// inferred: upgrade data of an equipment piece (game/CharaTask, game/CosEditBase)
extern "C" void* fn_8044D034(Inventory* inventory, int index) {
    if (index < 0 || 28 <= index) {
        return 0;
    }
    return &inventory->mUpgrades[index].m00C;
}

void Inventory::fn_8044D060(int slot, int item, int unused, int chara) {
    int index = fn_80219558(chara);
    if (0 <= index && index < 7) {
        mEquip[index].mProtectors[slot].mId = item;
        mEquip[index].mProtectors[slot].m04 = slot + index * 4;
    }
}

void Inventory::fn_8044D0DC(int slot, int item, int unused, int index) {
    mEquip[index].mProtectors[slot].mId = item;
    mEquip[index].mProtectors[slot].m04 = slot + index * 4;
}

// inferred: sets a weapon of a party member (game/Em201ZanGurgFinal)
extern "C" void fn_8044D104(Inventory* inventory, int slot, int item, int upgrade, int chara) {
    int index = fn_80219558(chara);
    if (0 <= index && index < 7) {
        inventory->mEquip[index].mWeapons[slot].mId = item;
        inventory->mEquip[index].mWeapons[slot].m04 = upgrade;
    }
}

// inferred: sets a weapon of the party member `index` (game/CosEditBase)
extern "C" void fn_8044D184(Inventory* inventory, int slot, int item, int upgrade, int index) {
    inventory->mEquip[index].mWeapons[slot].mId = item;
    inventory->mEquip[index].mWeapons[slot].m04 = upgrade;
}

// inferred: upgrade index of a protector of a party member (game/CharaTask)
extern "C" int fn_8044D1A4(Inventory* inventory, int slot, int chara) {
    int index = fn_80219558(chara);
    if (index >= 0 && index < 7) {
        return inventory->mEquip[index].mProtectors[slot].m04;
    }
    return -1;
}

// inferred: weapon of a party member (game/CharaTask)
extern "C" int fn_8044D3A0(Inventory* inventory, int slot, int chara) {
    int index = fn_80219558(chara);
    if (index >= 0 && index < 7) {
        return inventory->mEquip[index].mWeapons[slot].mId;
    }
    return -1;
}

// inferred: removes an item to identify (game/CosEditBase)
extern "C" void fn_8044D490(Inventory* inventory, int index) {
    if (index < 0 || index >= (int)inventory->mIdentify.size()) {
        return;
    }
    inventory->mIdentify.erase(&inventory->mIdentify[index]);
}

void Inventory::fn_8044D4F0() {
    mIdentify.clear();
}

// inferred: item to identify (game/ShopIdentify)
extern "C" int fn_8044D500(Inventory* inventory, int index) {
    if (index < 0 || index >= (int)inventory->mIdentify.size()) {
        return -1;
    }
    return inventory->mIdentify[index];
}

int Inventory::fn_8044D530(int index) {
    if (index < 0 || index >= (int)mIdentify.size()) {
        return -1;
    }
    return mIdentify[index];
}

// inferred: number of items to identify (game/ShopIdentify)
extern "C" int fn_8044D560(Inventory* inventory) {
    return inventory->mIdentify.size();
}

void Inventory::AddMoney(int n) {
    mMoney += n;
    if (mMoney < 0) {
        mMoney = 0;
    } else if (mMoney > 999999) {
        mMoney = 999999;
    }
}

// inferred: pays money (gmk/RandomMerchant, game/ShopIdentify)
extern "C" void fn_8044D6AC(Inventory* inventory, int money) {
    inventory->mMoney -= money;
    if (inventory->mMoney < 0) {
        inventory->mMoney = 0;
    } else if (inventory->mMoney > 999999) {
        inventory->mMoney = 999999;
    }
}

void Inventory::fn_8044D6E0(int money) {
    mMoney = money;
    if (mMoney < 0) {
        mMoney = 0;
    } else if (mMoney > 999999) {
        mMoney = 999999;
    }
}
