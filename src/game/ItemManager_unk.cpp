// game/ItemManager_unk.cpp (0x80442F10-0x8044D710): code of Inventory (the RSO names the class;
// the file name is unknown). Only the short functions at the end of the file are written so far.
#include "game/Inventory.h"
#include "game/CharaTask.h"
#include "game/MarketTask.h"
#include "db/ItemParam_unk.h"
#include "db/WeaponParam_unk.h"
#include "db/EquipParam_unk.h"
#include "atn/Memory.h"
#include "game/PlayerManager.h"

extern "C" const char* fn_8020BD3C(int index); // inferred: name of the group of a colour (db/PaletteTable_unk)
extern "C" const char* fn_8020BD78(int index); // inferred: name of an effect palette
extern "C" const char* fn_8020BDAC(int index); // inferred: name of an inverted palette

extern "C" int fn_8021150C(int code); // inferred: index of an item code in the bag
extern "C" int fn_80206BE4(WeaponParamEntry_unk* weapon); // inferred: item code of a weapon
extern "C" int fn_8020EF80(EquipParamEntry_unk* equip);   // inferred: item code of a protector

// inferred: texture rectangle of an item icon (16 x 16 icons per texture); callers declare their own
// copy of this struct
struct InventoryIconUV_unk {
    InventoryIconUV_unk(float u0_, float v0_, float u1_, float v1_) : u0(u0_), v0(v0_), u1(u1_), v1(v1_) {}
    float u0;
    float v0;
    float u1;
    float v1;
};

// inferred: field (lbl_8087F430); only the member used here
struct InventoryField_unk {
    unsigned char m0000[0x5694];
    int m5694; // inferred: read by fn_8044909C
};
extern "C" InventoryField_unk* lbl_8087F430;

// inferred: the bag of items, the money and the equipment of the party
Inventory* Inventory::instance_;

void Inventory::fn_80442F10() {
    if (instance_ == 0) {
        instance_ = new ((MemoryTag)3, "", "", 0) Inventory();
    }
}

void Inventory::fn_80442F68() {
    if (instance_ != 0) {
        delete instance_;
        instance_ = 0;
    }
}

int Inventory::fn_8044386C() {
    if (mLoaded != 0) {
        return 0;
    }
    if (mLoader.fn_80473F50() == 0) {
        mLoaded = 1;
        mLoader.fn_80473F88();
        return 0;
    }
    return 1;
}

void Inventory::fn_804438E0(int code) {
    Item_unk* item = &mItems[fn_8021150C(code)];
    if (item->m08 != 0) {
        for (int i = 0; i < 0x600; i++) {
            mItems[i].m08++;
        }
        item->m08 = 0;
    }
}

// inferred: removes `num` items `code`; returns the number removed (game shops, gimmicks)
extern "C" int fn_8044441C(Inventory* inventory, int code, int num) {
    int index = fn_8021150C(code);
    if (index < 0 || 0x600 <= index) {
        return 0;
    }
    Inventory::Item_unk* item = &inventory->mItems[index];
    int have = item->mCount;
    if (have < num) {
        item->mCount = 0;
    } else {
        item->mCount = have - num;
        have = num;
    }
    if (MarketTask::sInstance && MarketTask::sInstance->fn_804A24C4(code) && item->mCount <= 0) {
        MarketTask::sInstance->fn_804A28F8(code);
    }
    return have;
}

// inferred: number of items `code` in the bag (-1 if the code is not an item)
extern "C" int fn_804444E8(Inventory* inventory, int code) {
    int index = fn_8021150C(code);
    if (index < 0 || 0x600 <= index) {
        return -1;
    }
    return inventory->mItems[index].mCount;
}

// inferred: number of items of the slot `index`
extern "C" int fn_8044453C(Inventory* inventory, int index) {
    if (index < 0 || 0x600 <= index) {
        return -1;
    }
    return inventory->mItems[index].mCount;
}

// inferred: number of items `code` in the bag and on the party (game shops)
// NONMATCHING: the equipment pointer and its offset swap r29/r30
extern "C" int fn_80444564(Inventory* inventory, int code) {
    int count = fn_804444E8(inventory, code);
    Inventory::Equip_unk* equip;
    if (fn_80206B9C(code)) {
        for (unsigned int c = 0; c < 7; c++) {
            equip = &Inventory::Order()->mEquip[c];
            for (int w = 0; w < 2; w++) {
                WeaponParamEntry_unk* weapon = fn_80206B14(0, equip->mWeapons[w].mId);
                if (c == 0 && w == 1) {
                    weapon = fn_80206B14(1, equip->mWeapons[w].mId);
                }
                if (weapon && code == fn_80206BE4(weapon)) {
                    count++;
                }
            }
        }
    }
    if (fn_8020EF04(code)) {
        for (unsigned int c = 0; c < 7; c++) {
            equip = &Inventory::Order()->mEquip[c];
            for (int p = 0; p < 2; p++) {
                EquipParamEntry_unk* protector = fn_8020ED84(p, equip->mProtectors[p].mId);
                if (protector && code == fn_8020EF80(protector)) {
                    count++;
                }
            }
        }
    }
    return count;
}

// inferred: item id of the dye of a colour group (game/CosEditPalette_unk)
extern "C" int fn_804446BC(Inventory* inventory, int kind, int id) {
    for (int i = 0; i < fn_802114D8(); i++) {
        ItemParamEntry_unk* entry = fn_802114E0(i);
        if (entry && entry->mBC == kind && entry->mC4 == id) {
            return entry->mId;
        }
    }
    return 0;
}

Inventory::Item_unk* Inventory::fn_80444744(int code) {
    Item_unk* item = mItems;
    for (int i = 0; i < 0x600; item++, i++) {
        if (item->mCode < 0) {
            break;
        }
        if (item->mCode == code) {
            return item;
        }
    }
    return 0;
}

Inventory::Item_unk* Inventory::fn_80444804(int index) {
    if (0 <= index && index < 0x600) {
        return &mItems[index];
    }
    return 0;
}

// inferred: item whose key (m114) is the name of a colour group (game/CosEditPalette_unk)
extern "C" int fn_80444828(Inventory* inventory, int palette) {
    atn::FixedString_unk<8> name = fn_8020BD3C(palette);
    int i = 0;
    const char* key = name.c_str();
    for (; i < fn_802114D8(); i++) {
        ItemParamEntry_unk* entry = fn_802114E0(i);
        if (entry && entry->m114.Equals_unk(key)) {
            return entry->mId;
        }
    }
    return 0;
}

// inferred: same for an effect palette
extern "C" int fn_8044493C(Inventory* inventory, int part) {
    atn::FixedString_unk<8> name = fn_8020BD78(part);
    int i = 0;
    const char* key = name.c_str();
    for (; i < fn_802114D8(); i++) {
        ItemParamEntry_unk* entry = fn_802114E0(i);
        if (entry && entry->m114.Equals_unk(key)) {
            return entry->mId;
        }
    }
    return 0;
}

// inferred: same for an inverted palette
extern "C" int fn_80444A50(Inventory* inventory, int kind) {
    atn::FixedString_unk<8> name = fn_8020BDAC(kind);
    int i = 0;
    const char* key = name.c_str();
    for (; i < fn_802114D8(); i++) {
        ItemParamEntry_unk* entry = fn_802114E0(i);
        if (entry && entry->m114.Equals_unk(key)) {
            return entry->mId;
        }
    }
    return 0;
}

extern "C" int fn_80444BE8(Inventory* inventory, ItemParamEntry_unk* item);

// inferred: icon of the item `code`
extern "C" int fn_80444B64(Inventory* inventory, int code) {
    return fn_80444BE8(inventory, fn_80211480(code));
}

// inferred: icon of an item (texture cell, game shops)
extern "C" int fn_80444BE8(Inventory* inventory, ItemParamEntry_unk* item) {
    int icon = 0;
    if (item == 0) {
        return 0;
    }
    if (item->mBE > 0) {
        icon = item->mBE % 100 - 1 + (item->mBE / 100 - 1) * 16;
    }
    return icon;
}

// inferred: icon of the money (game/ShopIdentify, game/ShopBuySell)
extern "C" int fn_80444C48(Inventory* inventory) {
    return 0x58;
}

// inferred: texture rectangle of the icon `icon`
extern "C" InventoryIconUV_unk fn_80444C50(Inventory* inventory, int icon) {
    int x = icon % 16;
    int y = icon / 16;
    float size = 0.0625f;
    return InventoryIconUV_unk(x * size, y * size, (x + 1) * size, (y + 1) * size);
}

extern "C" int fn_80370174(InventoryField_unk* field, int index); // inferred: reads an event counter
// inferred: chance of finding a weapon by rank (the second one once the weapon was found)
static const int sWeaponRate0_unk[5] = {0, 0, 100, 100, 100};
static const int sWeaponRate1_unk[5] = {0, 100, 100, 100, 100};

int Inventory::fn_80448F9C(int code) {
    int rate = 100;
    if (fn_80206B9C(code)) {
        WeaponParamEntry_unk* weapon = fn_80206C50(code);
        int index = fn_8021150C(code);
        unsigned int flags;
        if (index < 0 || 0x600 <= index) {
            flags = 0;
        } else {
            flags = mItems[index].m0C;
        }
        if ((flags & 1) == 1) {
            rate = sWeaponRate1_unk[weapon->m118];
        } else {
            rate = sWeaponRate0_unk[weapon->m118];
        }
        if (lbl_8087F430 != 0) {
            if (fn_80370174(lbl_8087F430, 0x8A) == 1) {
                rate = 0;
            } else if (fn_80370174(lbl_8087F430, 0x8A) >= 2) {
                rate = 100;
            }
        }
    }
    return rate;
}

extern "C" PlayerManager* lbl_8087F8A0; // inferred: party
extern "C" int fn_80375184(InventoryField_unk* field); // inferred: saving is allowed

void Inventory::fn_804490E0(ItemParamEntry_unk* item, int member) {
    if (lbl_8087F8A0 != 0) {
        int type = item->mB4;
        bool group = 8 <= type && type <= 10;
        int stat = -1;
        if (type == 11) {
            stat = type;
        } else if (!group) {
            stat = type;
        } else {
            switch (type) {
            case 9:
                stat = 10;
                break;
            case 8:
                stat = 0x1C;
                break;
            }
        }
        CharaTask* chara = (CharaTask*)lbl_8087F8A0->fn_8054A33C(member);
        CharaStatus* status = chara != 0 ? chara->GetStatus_unk() : &mPlayers[member];
        status->fn_801342D0((StatusItem_unk*)item, stat);
        status->fn_8012B988();
        if (lbl_8087F430 == 0 || fn_80375184(lbl_8087F430) == 0) {
            status->fn_8012D8B8();
        }
    }
}

// inferred: rewards of a defeated enemy (outputs added to m6014/m6018); name invented
extern "C" void fn_804491F4(int* a, int* b, int level, int exp, int arg4, float rate);
// inferred: settings (lbl_8087F0A8); only the member used here
struct InventorySettings_unk {
    unsigned char m000[0x160];
    int m160; // inferred: debug: fixed protectors (fn_8044D208)
    unsigned char m164[0x18C - 0x164];
    int m18C; // inferred: debug: random protectors (fn_8044D208)
    unsigned char m190[0x280 - 0x190];
    int m280; // inferred: experience is shared (fn_80449A18)
};
extern "C" InventorySettings_unk* lbl_8087F0A8;
extern "C" void* lbl_8087F610; // inferred: multiplayer session

int Inventory::fn_8044909C() {
    int flags = 0;
    if ((lbl_8087F430 != 0 && lbl_8087F430->m5694 != 0) || (mDB14 != 0 && mDB24 != 0)) {
        flags |= 6;
    }
    return flags;
}

void Inventory::fn_80449A18(int killer, int level, int exp, int arg4, float rate) {
    if (lbl_8087F610 == 0) {
        int a = 0;
        int b = 0;
        fn_804491F4(&a, &b, level, exp, arg4, rate);
        m6014 += a;
        m6018 += b;
        if (lbl_8087F0A8->m280 != 0) {
            fn_80449ABC(killer, level);
        }
    }
}

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

extern "C" int rand();

int Inventory::fn_8044D208(int slot, int chara) {
    int index = fn_80219558(chara);
    if (index < 0 || index > 6) {
        return -1;
    }
    if (lbl_8087F0A8->m160 != 0) {
        switch (index) {
        case 0:
            return 0;
        case 2:
            return 20;
        case 3:
            return 40;
        case 4:
            return 60;
        case 5:
            return 80;
        case 6:
            return 100;
        case 1:
            return 120;
        }
    } else if (lbl_8087F0A8->m18C != 0 && slot == 2) {
        int a = rand() % 8 * 20 + rand() % 10;
        int b = rand() % 8 * 20 + rand() % 10;
        mEquip[index].mProtectors[0].mId = a;
        mEquip[index].mProtectors[1].mId = b;
        mEquip[index].mProtectors[2].mId = a;
        mEquip[index].mProtectors[3].mId = a;
    }
    return mEquip[index].mProtectors[slot].mId;
}

void Inventory::fn_8044D404(const int& code) {
    ItemParamEntry_unk* entry = fn_80211480(code);
    if (entry != 0 && entry->mBC == 2 && mIdentify.size() < 0x100) {
        mIdentify.push_back(code);
    }
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

// NONMATCHING: the original recomputes the erased element from `index` (r30) instead of reusing r31
// inferred: identifies the item `index` of the list: adds it to the bag and removes it from the list
// (game/ShopIdentify)
extern "C" void fn_8044D56C(Inventory* inventory, int index) {
    if (index < 0 || index >= (int)inventory->mIdentify.size()) {
        return;
    }
    int slot = fn_8021150C(inventory->mIdentify[index]);
    if (slot < 0 || 0x600 <= slot) {
        return;
    }
    inventory->AddItem(inventory->mIdentify[index], 1, 1, 0, 0, 1, 1);
    slot = fn_8021150C(inventory->mIdentify[index]);
    if (0 <= slot && slot < 0x600) {
        inventory->mItems[slot].m0C = 1;
    }
    inventory->mIdentify.erase(&inventory->mIdentify[index]);
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
