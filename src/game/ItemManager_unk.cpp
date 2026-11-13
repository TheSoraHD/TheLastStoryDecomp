// game/ItemManager_unk.cpp (0x80442F10-0x8044D710): code of Inventory (the RSO names the class;
// the file name is unknown). Only the short functions at the end of the file are written so far.
#include "game/Inventory.h"
#include "game/GameTask.h"
#include "game/CharaTask.h"
#include "game/MarketTask.h"
#include "db/ItemParam_unk.h"
#include "db/WeaponParam_unk.h"
#include "db/EquipParam_unk.h"
#include "atn/Memory.h"
#include "game/PlayerManager.h"
#include "db/ChapterList_unk.h"

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

extern "C" GameTask* lbl_8087F430;
extern "C" PlayerManager* lbl_8087F8A0; // inferred: party

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

extern "C" const wchar_t lbl_808813D0[2]; // inferred: empty text
// inferred: message table (lbl_8087F1E4), read as single texts or as pairs (as in game/CharaTask.cpp and
// game/ShopIdentify.cpp); names invented
struct InventoryMsgPair_unk {
    const wchar_t* mKey;
    const wchar_t* mText;
};
struct InventoryMsgTable_unk {
    const wchar_t* Get_unk(int id) {
        if (mText[id] != 0) {
            return mText[id];
        }
        return lbl_808813D0;
    }
    const wchar_t* GetPair_unk(int id) {
        return mPairs[id].mText != 0 ? mPairs[id].mText : lbl_808813D0;
    }
    union {
        const wchar_t* mText[1];
        InventoryMsgPair_unk mPairs[1];
    };
};
extern "C" InventoryMsgTable_unk* lbl_8087F1E4;
extern "C" int fn_800DD3FC(wchar_t* s, const wchar_t* format, ...); // inferred: wide sprintf

// inferred: text of `count` items `code` (game/GameUI, gmk/DropItem, game/BattleResult); 0 without a name
// NONMATCHING: 93.9% - registers of the parameters (the original keeps text/count/rare in r28-r30 and
// the entry in r31)
extern "C" int fn_80444CF8(wchar_t* text, int code, int count, int rare) {
    ItemParamEntry_unk* item = fn_80211480(code);
    if (item == 0 || text == 0 || lbl_8087F1E4 == 0 || item->mName == 0) {
        return 0;
    }
    if (fn_80206B9C(item->mId)) {
        if (rare) {
            fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0x283), item->mName);
        } else {
            WeaponParamEntry_unk* weapon = fn_80206C50(item->mId);
            if (weapon) {
                const wchar_t* rank = lbl_8087F1E4->GetPair_unk(weapon->mType + 0xEB);
                fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0x1D5), rank);
            }
        }
    } else if (fn_8020EF04(item->mId)) {
        fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0x283), item->mName);
    } else if (item->mId == 0x2714 || item->mId == 0x2719) {
        fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0x287), count);
    } else if (count == 1) {
        fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0x283), item->mName);
    } else {
        fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0x285), item->mName, count);
    }
    return 1;
}

extern "C" ChapterEntry_unk* fn_802085E0(int type, int map); // db/ChapterList_unk
extern "C" int rand();

// inferred: multiplier of the money dropped by the progress of the story (m10D0 / 100000)
const float lbl_80754730[8] = {1.0f, 1.0f, 1.1f, 1.5f, 2.0f, 3.0f, 3.0f, 0.0f};

// inferred: `base` raised by a random fraction of itself (0-100 %); name invented
static inline float RandScale_unk(float base) {
    return base + base * ((rand() % 1001) / 1000.0f);
}

// inferred: money dropped by an enemy (gmk/DropItem): grows with the level of `chara` (the leader of
// the party without one, else the level of the chapter) and with the progress of the story
extern "C" int fn_80444EFC(CharaTask* chara) {
    int level = 0;
    int progress = 0;
    if (chara == 0) {
        chara = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0;
    }
    if (chara != 0) {
        level = chara->mStatus.mParamA.m78;
    } else if (lbl_8087F430 != 0 && lbl_8087F430->m10D4 != 0) {
        level = lbl_8087F430->m10D4->m58;
    } else if (Inventory::instance_ != 0 && Inventory::instance_->mDB14 != 0 && Inventory::instance_->mDB18 == 2) {
        ChapterEntry_unk* chapter = fn_802085E0(2, Inventory::instance_->mDB1C);
        if (chapter != 0) {
            level = chapter->m58;
        } else {
            level = 0;
        }
    }
    float rate = 1.0f;
    if (lbl_8087F430 != 0) {
        int v = lbl_8087F430->m10D0;
        progress = 600000;
        if ((v > 0 ? v : 0) < progress) {
            progress = v > 0 ? v : 0;
        }
    } else if (Inventory::instance_->mDB14 != 0 && Inventory::instance_->mDB18 == 2) {
        ChapterEntry_unk* chapter = fn_802085E0(2, Inventory::instance_->mDB1C);
        if (chapter != 0) {
            int v = chapter->m04;
            progress = 600000;
            if ((v > 0 ? v : 0) < progress) {
                progress = v > 0 ? v : 0;
            }
        }
    }
    if (progress > 0) {
        rate *= lbl_80754730[progress / 100000];
    }
    return (int)(rate * (level * 3 * RandScale_unk(8.0f)));
}

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
            if (lbl_8087F430->fn_80370174(0x8A) == 1) {
                rate = 0;
            } else if (lbl_8087F430->fn_80370174(0x8A) >= 2) {
                rate = 100;
            }
        }
    }
    return rate;
}


// inferred: settings (lbl_8087F0A8); only the member used here
struct InventorySettings_unk {
    unsigned char m000[0xD4];
    int mD4; // inferred: mode of the level scaling of the rewards (3: by tenths; fn_80449448)
    unsigned char m0D8[0x160 - 0xD8];
    int m160; // inferred: debug: fixed protectors (fn_8044D208)
    unsigned char m164[0x18C - 0x164];
    int m18C; // inferred: debug: random protectors (fn_8044D208)
    unsigned char m190[0x280 - 0x190];
    int m280; // inferred: experience is shared (fn_80449A18)
    int m284; // inferred: the rewards are scaled by the difference of levels (fn_80449448)
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
        if (lbl_8087F430 == 0 || lbl_8087F430->fn_80375184() == 0) {
            status->fn_8012D8B8();
        }
    }
}

extern "C" double pow(double x, double y);

// inferred: rewards of a defeated enemy (outputs added to m6014/m6018 by fn_80449A18): the level is
// lowered by fn_80375B78 and the rate raised when the enemy is above fn_80375D0C; capped at 99999
extern "C" void fn_804491F4(int* a, int* b, int level, int exp, int arg4, float rate) {
    if (lbl_8087F430 != 0 && lbl_8087F430->fn_803754F0() == 0) {
        level -= lbl_8087F430->fn_80375B78(0);
        level = level < 1 ? 1 : level;
    }
    if (1.0f < rate && rate <= 3.5f && lbl_8087F430 != 0) {
        int top = lbl_8087F430->fn_80375D0C();
        if (level > top) {
            rate = 1.1f > rate + (top - level) ? 1.1f : rate + (top - level);
        }
    }
    int n = (level - 1 < 1 ? 1 : level - 1) > 69 ? 69 : (level - 1 < 1 ? 1 : level - 1);
    float p = pow((float)n, 1.8f);
    *a = (int)(60.0f * p / 10.0f) + arg4;
    *b = (int)(0.25f * (*a * exp));
    *a = (int)(*a * rate);
    *b = (int)(*b * rate);
    *a = *a < 99999 ? *a : 99999;
    *b = *b < 99999 ? *b : 99999;
}

// inferred: the original repeats the expressions of the clamps (macros, not inline functions); names
// invented
#define INV_MIN_unk(a, b) ((a) < (b) ? (a) : (b))
#define INV_MAX_unk(a, b) ((a) > (b) ? (a) : (b))

// inferred: scales the rewards `a`/`b` by the difference between the levels `level` and `other` (the
// experience is not reduced while the flag 0x123 is set and the difference is at most 10)
extern "C" void fn_80449448(int* a, int* b, int level, int other) {
    if (lbl_8087F0A8->m284 == 0) {
        return;
    }
    if (level <= other) {
        if (lbl_8087F0A8->mD4 == 3) {
            *a = (int)(*a * INV_MAX_unk(1.0f, INV_MIN_unk(3.0f, 0.1f * (other - level))));
            *b = (int)(*b * INV_MAX_unk(1.0f, INV_MIN_unk(3.0f, 0.1f * (other - level))));
        } else {
            *a = (int)(*a * INV_MAX_unk(0.1f, INV_MIN_unk(100.0f, 0.75f * (other - level) + 1.0f)));
            *b = (int)(*b * INV_MAX_unk(0.1f, INV_MIN_unk(100.0f, 0.75f * (other - level) + 1.0f)));
        }
    } else if ((lbl_8087F430 != 0 ? lbl_8087F430->fn_80370174(0x123) == 0 : 1) || level - other > 10) {
        *a = (int)(*a * INV_MAX_unk(0.05f, INV_MIN_unk(100.0f, 0.18f * (other - level) + 1.0f)));
        *b = (int)(*b * INV_MAX_unk(0.05f, INV_MIN_unk(100.0f, 0.18f * (other - level) + 1.0f)));
    }
    *a = INV_MAX_unk(1, INV_MIN_unk(*a, 99999));
    *b = INV_MAX_unk(0, INV_MIN_unk(*b, 99999 - *a));
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
extern "C" int fn_8044D3A0(Inventory* inventory, int slot, int chara);
extern "C" void fn_8044D490(Inventory* inventory, int index);


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

// inferred: weapon of a party member (game/CharaTask)
extern "C" int fn_8044D3A0(Inventory* inventory, int slot, int chara) {
    int index = fn_80219558(chara);
    if (index >= 0 && index < 7) {
        return inventory->mEquip[index].mWeapons[slot].mId;
    }
    return -1;
}

void Inventory::fn_8044D404(const int& code) {
    ItemParamEntry_unk* entry = fn_80211480(code);
    if (entry != 0 && entry->mBC == 2 && mIdentify.size() < 0x100) {
        mIdentify.push_back(code);
    }
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
