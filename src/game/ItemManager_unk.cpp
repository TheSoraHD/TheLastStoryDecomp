// game/ItemManager_unk.cpp (0x80442F10-0x8044D710): code of Inventory (the RSO names the class;
// the file name is unknown). Only the short functions at the end of the file are written so far.
#include "game/Inventory.h"
#include "game/GameTask.h"
#include "game/MapTask.h"
#include "game/CharaTask.h"
#include "game/MarketTask.h"
#include "db/ItemParam_unk.h"
#include "db/WeaponParam_unk.h"
#include "db/EquipParam_unk.h"
#include "atn/Memory.h"
#include "game/PlayerManager.h"
#include "game/PlayerTask.h"
#include "db/ChapterList_unk.h"
#include "game/DamageCalc_unk.h"
#include "game/HitInfo_unk.h"
#include "game/MslSort_unk.h"

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

// inferred: chance of finding a weapon by rank (the second one once the weapon was found)
static const int sWeaponRate0_unk[5] = {0, 0, 100, 100, 100};
static const int sWeaponRate1_unk[5] = {0, 100, 100, 100, 100};

extern "C" int rand();
extern "C" int fn_80445130(Inventory* inventory, int* code, int* count, int arg); // inferred: random item
extern "C" int fn_80444564(Inventory* inventory, int code);

// inferred: items of a weapon that is not got go to the list to identify (with flag 0xA1)
int Inventory::AddItem(int id, int num, int a2, int a3, int* out, int a5, int a6) {
    fn_80445130(this, &id, &num, 0);
    int index = fn_8021150C(id);
    if (index < 0 || 0x600 <= index) {
        return 0;
    }
    ItemParamEntry_unk* item = fn_80211480(id);
    if (item == 0) {
        return 0;
    }
    int got = 1;
    if (fn_80206B9C(id)) {
        int rate;
        if (a3 != 0) {
            if (a2 != 0) {
                rate = 100;
            } else {
                // inferred: the body of fn_80448F9C, inlined
                int c = id;
                rate = 100;
                if (fn_80206B9C(c)) {
                    WeaponParamEntry_unk* weapon = fn_80206C50(c);
                    int i = fn_8021150C(c);
                    unsigned int flags;
                    if (i < 0 || 0x600 <= i) {
                        flags = 0;
                    } else {
                        flags = mItems[i].m0C;
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
            }
        } else {
            rate = a2 != 0 ? 100 : 0;
        }
        if (rate <= rand() % 100) {
            got = 0;
        }
    }
    if (fn_80206B9C(id) || fn_8020EF04(id)) {
        if (a5 != 0 && got != 0 && num + fn_80444564(this, id) > 10) {
            num = 10 - fn_80444564(this, id);
        }
    } else if (item->mBC == 6) {
        if (item->m64 == 2) {
            for (int i = 0; i < 7; i++) {
                fn_804490E0(item, i);
            }
        } else {
            fn_804490E0(item, 0);
        }
        num = 0;
    } else if (num + mItems[index].mCount > 99) {
        num = 99 - mItems[index].mCount;
    }
    if (got != 0) {
        mItems[index].mCode = id;
        mItems[index].mCount += num;
        if (a6 != 0) {
            fn_804438E0(id);
        }
    } else {
        for (int i = 0; i < num; i++) {
            int c = id;
            ItemParamEntry_unk* entry = fn_80211480(c);
            if (entry != 0 && entry->mBC == 2 && mIdentify.size() < 0x100) {
                mIdentify.push_back(c);
            }
        }
        if (lbl_8087F430 != 0) {
            lbl_8087F430->fn_803750E4(0xA1);
        }
    }
    if (out != 0) {
        *out = got;
    }
    if (lbl_8087F430 != 0) {
        if (id == 0x74) {
            lbl_8087F430->fn_803750E4(0x91);
        }
        if (id == 0x143) {
            lbl_8087F430->fn_803750E4(0x1B8);
        }
        if ((fn_80206B9C(id) || fn_8020EF04(id)) && lbl_8087F430->m10D0 >= 108000) {
            lbl_8087F430->fn_803750E4(0x148);
        }
        if (item->mBC == 6 && item->mB4 == 5) {
            lbl_8087F430->fn_803750E4(0x146);
        }
        if (fn_80206B9C(id) && fn_80206C50(id)->mF0 != 0 && lbl_8087F430->m10D0 >= 108000) {
            lbl_8087F430->fn_803750E4(0x151);
        }
        if (fn_8020EF04(id) && ((fn_8020EFEC(id)->mKind == 0 && fn_8020EFEC(id)->mE0 >= 6) ||
                                  (fn_8020EFEC(id)->mKind == 1 && fn_8020EFEC(id)->mE0 >= 7))) {
            lbl_8087F430->fn_803750E4(0x1AF);
        }
        if (fn_80206B9C(id)) {
            WeaponParamEntry_unk* weapon = fn_80206C50(id);
            if ((weapon->mC4[0].m00 == 0x3F && weapon->mC4[0].m04 == 7) ||
                (weapon->mC4[1].m00 == 0x3F && weapon->mC4[1].m04 == 7) ||
                (weapon->mKind == 0 && weapon->mId >= 399 && 405 >= weapon->mId)) {
                lbl_8087F430->fn_803750E4(0x1B4);
            }
        }
    }
    return num;
}

// inferred: drop tables (db, as in gmk/TreasureBox.cpp; names invented)
struct ItemEntry_unk {
    int mId;
    int mCount;
    int m08;
};
struct ItemTable_unk {
    int m00;
    ItemEntry_unk* mEntries; // 0x04
};
extern "C" ItemTable_unk* fn_8021E444(int table);
extern "C" int fn_8021E5A4(ItemTable_unk* table, int flags); // inferred: draws an entry
extern "C" HitInfo_unk* fn_80219E6C(int id);                // inferred: bullet data by id
extern "C" void* lbl_8087F048;                              // BattleTask
extern "C" void* lbl_8087F610;                              // inferred: multiplayer session
// inferred: shows a damage on the battle UI (game/BattleTask)
extern "C" void fn_80108C10(void* battle, DamageResult_unk* damage, const atn::Vector3* pos, CharaTask* attacker,
                            CharaTask* target, int arg5, int arg6);
// inferred: message of an item got (game/BattleTask; 1 item, 2 weapon, 3 money)
extern "C" void fn_801092C8(void* battle, int kind, int arg, int code, int count);
extern "C" int fn_80444EFC(CharaTask* chara);

// inferred: gets the entry `index` (drawn when negative) of the drop table `table`: the items used at once
// (kind 3 or flag 0x100) are applied to the leader, the money is added and the other items go to the bag;
// `message` shows the message of the battle. Returns 0 without a table or an entry (game/BattleTask)
// NONMATCHING: 88.7% - the original keeps the constants 0, -1, 1 and 0xF0000 in r19-r23 for the whole
// function (stack frame 0x80) and shares the final `return 1`
extern "C" int fn_80444020(Inventory* inventory, int table, int message, int index, CharaTask* chara, int arg,
                           int* outCode, int* outCount, int* outAdded) {
    ItemTable_unk* t = fn_8021E444(table);
    if (t == 0) {
        return 0;
    }
    int i = index < 0 ? fn_8021E5A4(t, Inventory::Order() != 0 ? Inventory::Order()->fn_8044909C() : 0) : index;
    if (i < 0) {
        return 0;
    }
    ItemEntry_unk* entry = &t->mEntries[i];
    ItemParamEntry_unk* item = fn_80211480(entry->mId);
    if (item == 0) {
        inventory->AddItem(1, 1, 1, 0, 0, 1, 1);
        return 1;
    }
    int code = entry->mId;
    int count = entry->mCount;
    if ((int)item->mItemKind == 3 || (item->mAC & 0x100) == 0x100) {
        CharaTask* leader = lbl_8087F8A0->mHead;
        HitInfo_unk* hit = item->mC4 > 0 ? fn_80219E6C(item->mC4) : 0;
        if (leader != 0 && hit != 0) {
            DamageResult_unk result;
            fn_8003EA3C(&result, hit, leader, leader, 0, &lbl_807C6B90, 0, 0);
            void* battle = lbl_8087F048;
            if (battle != 0 && (hit->mAC & 0x80000) != 0x80000 &&
                (lbl_8087F610 == 0 || (hit->m04 != 6004 && hit->m04 != 20428))) {
                atn::Vector3 pos = leader->vfAC();
                fn_80108C10(battle, &result, &pos, leader, leader, 0, 0);
            }
        }
        if (outCode != 0) {
            *outCode = code;
        }
        if (outCount != 0) {
            *outCount = count;
        }
        if (outAdded != 0) {
            *outAdded = 1;
        }
    } else {
        fn_80445130(inventory, &code, &count, 0);
        if (code != 0x2714 && code != 0x2719) {
            int added = 1;
            Inventory::instance_->AddItem(code, count, arg, 1, &added, 1, 1);
            if (message != 0) {
                if (fn_80206B9C(code) && added == 0) {
                    fn_801092C8(lbl_8087F048, 2, 0, code, count);
                } else if (item->mBC != 6) {
                    fn_801092C8(lbl_8087F048, 1, 0, code, count);
                }
            }
            if (outCode != 0) {
                *outCode = code;
            }
            if (outCount != 0) {
                *outCount = count;
            }
            if (outAdded != 0) {
                *outAdded = added;
            }
        } else {
            int money = count;
            if (code == 0x2714) {
                money = fn_80444EFC(chara);
            }
            Inventory::instance_->AddMoney(money);
            if (message != 0) {
                fn_801092C8(lbl_8087F048, 3, 0, money, 0);
            }
            if (outCode != 0) {
                *outCode = code;
            }
            if (outCount != 0) {
                *outCount = money;
            }
            if (outAdded != 0) {
                *outAdded = 1;
            }
        }
    }
    return 1;
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

// inferred: settings of the drops (db/InitSetting_unk.cpp, InitSetting_unk; only the members used here)
struct DropSetting_unk {
    unsigned char m000[0x100];
    int mRate[10];  // 0x100: "DropItem", by item kind (mBC)
    float mRare[3]; // 0x128: "DropItemRare"
};
extern "C" DropSetting_unk* fn_80210AC4();
extern "C" bool fn_80213B78(int code, int type, int id, int sub); // db/ItemRare_unk: rare item of a place

// inferred: candidate of a random item (0xC bytes; names invented)
struct ItemCandidate_unk {
    ItemCandidate_unk() : mCode(0), mCount(1), mWeight(10), mItem(0) {}
    int mCode;
    short mCount;
    short mWeight;
    ItemParamEntry_unk* mItem;
};
typedef atn::FixedArray_unk<ItemCandidate_unk, 384> ItemCandidates_unk;
// inferred: fills `list` with the candidates of the random code `code` (0x80445F44)
extern "C" int fn_80445F44(Inventory* inventory, ItemCandidates_unk* list, int code, int count, int arg);

// inferred: the item is rare in the current place (the map of the field, else the place of the item being
// added); name invented
static inline bool IsRareHere_unk(int code) {
    int type = -1;
    int id = -1;
    int sub = -1;
    if (lbl_8087F430 != 0) {
        if (lbl_8087F430->GetCurrentMap() != 0) {
            MapTask* map = lbl_8087F430->GetCurrentMap();
            type = map->mType;
            id = map->mId;
            sub = map->mSubId;
        }
    } else if (Inventory::instance_ != 0 && Inventory::instance_->mDB14 != 0) {
        type = Inventory::instance_->mDB18;
        id = Inventory::instance_->mDB1C;
        sub = Inventory::instance_->mDB20;
    }
    return fn_80213B78(code, type, id, sub);
}

// inferred: keeps only the rare candidates of the place / removes them; names invented
static inline void KeepRare_unk(ItemCandidates_unk& list) {
    for (ItemCandidate_unk* p = list.begin(); p != list.end();) {
        if (!IsRareHere_unk(p->mCode)) {
            list.erase(p);
        } else {
            p++;
        }
    }
}
static inline void RemoveRare_unk(ItemCandidates_unk& list) {
    for (ItemCandidate_unk* p = list.begin(); p != list.end();) {
        if (IsRareHere_unk(p->mCode)) {
            list.erase(p);
        } else {
            p++;
        }
    }
}

// NONMATCHING: 98.5% - the original compares the result of the unused IsRareHere_unk of the count of the
// kinds (cmpwi without a branch)
// inferred: replaces the random codes 0x2711-0x271A by an item drawn from the candidates of fn_80445F44 (the
// rare ones of the place with the chance mRare); 0 for the code 0x2719 and the unknown ones
extern "C" int fn_80445130(Inventory* inventory, int* code, int* count, int arg) {
    ItemCandidates_unk list;
    int total = 0;
    DropSetting_unk* setting = fn_80210AC4();
    int rare = 0;
    int found = 0;
    int ret = 1;
    switch (*code) {
    case 0x2711:
    case 0x2715:
    case 0x2718:
    case 0x271A: {
        if (fn_80445F44(inventory, &list, *code, *count, arg) == 0) {
            break;
        }
        for (unsigned int i = 0; i < list.size(); i++) {
            if (IsRareHere_unk(list.mData[i].mCode)) {
                found++;
            }
        }
        float rate = setting->mRare[0];
        if (*code == 0x271A) {
            rate *= 5.0f;
        }
        if (rate > 100.0f * ((rand() % 1001) / 1000.0f)) {
            rare = 1;
        }
        if (rare) {
            if (found > 0) {
                KeepRare_unk(list);
            }
        } else {
            RemoveRare_unk(list);
        }
        for (unsigned int i = 0; i < list.size(); i++) {
            total += list.mData[i].mWeight;
        }
        break;
    }
    case 0x2712:
    case 0x2716:
        if (fn_80445F44(inventory, &list, *code, *count, arg) == 0) {
            break;
        }
        for (unsigned int i = 0; i < list.size(); i++) {
            total += list.mData[i].mWeight;
        }
        break;
    case 0x2713:
    case 0x2717: {
        if (fn_80445F44(inventory, &list, *code, *count, arg) == 0) {
            break;
        }
        for (unsigned int i = 0; i < list.size(); i++) {
            if (IsRareHere_unk(list.mData[i].mCode)) {
                found++;
            }
        }
        float rate = setting->mRare[1];
        if (*code == 0x2717) {
            rate = setting->mRare[2];
        }
        if (rate > 100.0f * ((rand() % 1001) / 1000.0f)) {
            rare = 1;
        }
        if (rare) {
            if (found > 0) {
                KeepRare_unk(list);
            }
        } else {
            RemoveRare_unk(list);
        }
        int kinds[10];
        memset(kinds, 0, sizeof(kinds));
        for (unsigned int i = 0; i < list.size(); i++) {
            ItemParamEntry_unk* item = list.mData[i].mItem;
            IsRareHere_unk(item->mId);
            kinds[item->mBC]++;
        }
        int rates[10];
        if (*code == 0x2713) {
            memset(rates, 0, sizeof(rates));
            for (int k = 0; k < 10; k++) {
                if (kinds[k] > 0) {
                    rates[k] = setting->mRate[k] * 100 / kinds[k];
                }
            }
        } else {
            for (int k = 0; k < 10; k++) {
                rates[k] = 10;
            }
        }
        for (unsigned int i = 0; i < list.size(); i++) {
            list.mData[i].mWeight = rates[fn_80211480(list.mData[i].mCode)->mBC];
            total += list.mData[i].mWeight;
        }
        break;
    }
    case 0x2714:
        if (fn_80445F44(inventory, &list, *code, *count, arg) == 0) {
            break;
        }
        for (unsigned int i = 0; i < list.size(); i++) {
            total += list.mData[i].mWeight;
        }
        break;
    default:
        ret = 0;
        break;
    }
    if (ret != 0) {
        if (total > 0) {
            int r = rand() % total;
            ItemCandidate_unk* p = list.mData;
            for (unsigned int i = 0; i < list.size(); i++, p++) {
                if (r < p->mWeight) {
                    *code = p->mCode;
                    *count = p->mCount;
                    break;
                }
                r -= p->mWeight;
            }
        } else {
            *code = 0x2714;
            *count = fn_80444EFC(0);
        }
    }
    if (inventory->mDB0C != 0) {
        if ((lbl_8087F430 != 0 && lbl_8087F430->fn_80370174(0x11B) > 0) ||
            (inventory->mDB14 != 0 && inventory->mDB28 > 0)) {
            if (*code == 0x137 || *code == 0x138) {
                *code = 0x139;
            }
        }
    }
    return ret;
}

extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" unsigned long strlen(const char* s);
extern "C" int atoi(const char* s);
extern "C" ChapterEntry_unk* fn_80208714(int index); // db/ChapterList_unk: chapter by index
extern "C" int fn_80208740();                       // db/ChapterList_unk: number of chapters

// inferred: the place `map` ("any", "dgNNN" or "dgNNN_PP") matches the current map (or the map of the item
// being added); `from` ("any", "dgNNN", "to_dgNNN") limits it to the chapters reached before (fn_80445F44)
extern "C" int fn_80448B80(Inventory* inventory, const char* map, const char* from) {
    if (map == 0 || *map == 0) {
        return 0;
    }
    if (from == 0 || *from == 0) {
        return 0;
    }
    MapTask* cur = lbl_8087F430 != 0 ? lbl_8087F430->GetCurrentMap() : 0;
    int type = cur != 0 ? cur->mType : -1;
    int id = cur != 0 ? cur->mId : 0;
    int sub = cur != 0 ? cur->mSubId : 0;
    if (type == 2 && id == 9) {
        if (sub == 12) {
            sub = 2;
        }
        if (sub == 15) {
            sub = 5;
        }
        if (sub == 16) {
            sub = 6;
        }
    }
    if (type != 2 || id == 999) {
        if (inventory->mDB14 == 0) {
            return 1;
        }
        type = inventory->mDB18;
        id = inventory->mDB1C;
        sub = inventory->mDB20;
    }
    int ret = 0;
    if (strcmp(map, "any") == 0) {
        ret = 1;
    } else if (strncmp(map, "dg", 2) == 0 && atoi(map + 2) == id) {
        if (strlen(map) >= 6) {
            if (atoi(map + 6) == sub) {
                ret = 1;
            } else {
                ret = 0;
            }
        } else {
            ret = 1;
        }
    }
    if (ret == 0) {
        if (strcmp(from, "any") == 0) {
            if (lbl_8087F430 != 0 && lbl_8087F430->fn_803752F4(map) != 0) {
                ret = 1;
            } else if (inventory->mDB14 != 0 && strncmp(map, "dg", 2) == 0 && inventory->mDB18 == 2) {
                ChapterEntry_unk* chapter = fn_802085E0(2, atoi(map + 2));
                ret = fn_802085E0(2, inventory->mDB1C) >= chapter;
            }
        } else if (strncmp(map, "dg", 2) == 0 && strncmp(from, "to_dg", 5) == 0) {
            int dst = atoi(map + 2);
            int dstSub = 0;
            if (strlen(map) >= 6) {
                dstSub = atoi(map + 6);
            }
            int src = atoi(from + 5);
            int srcSub = 0;
            if (strlen(from) >= 9) {
                srcSub = atoi(from + 9);
            }
            int found = 0;
            for (int i = 0; i < fn_80208740(); i++) {
                ChapterEntry_unk* c = fn_80208714(i);
                if (c->mType == 2 && c->mMap == dst && (dstSub <= 0 || c->mPoint == dstSub)) {
                    found = 1;
                    continue;
                }
                if (found && c->mType == type && c->mMap == id && (c->mPoint <= 0 || c->mPoint == sub)) {
                    ret = 1;
                    break;
                }
                if (c->mType == 2 && c->mMap == src && (srcSub <= 0 || c->mPoint == srcSub)) {
                    break;
                }
            }
        } else if (strncmp(from, "dg", 2) == 0 && atoi(from + 2) == id) {
            if (strlen(from) >= 6) {
                if (atoi(from + 6) == sub) {
                    ret = 1;
                } else {
                    ret = 0;
                }
            } else {
                ret = 1;
            }
        }
    }
    return ret;
}


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
    unsigned char m288[0x298 - 0x288];
    int m298; // inferred: the killer gets 1.2 times the experience (fn_80449ABC)
};
extern "C" InventorySettings_unk* lbl_8087F0A8;

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

// inferred: order of the party in the results: by the member kind of the character id (fn_80219558;
// negative for the guests, placed after the members and among them by id); name invented
struct ExpOrderLess_unk {
    bool operator()(const Inventory::ExpEntry_unk& a, const Inventory::ExpEntry_unk& b) const {
        int ka = fn_80219558(a.mChara->m50);
        int kb = fn_80219558(b.mChara->m50);
        if (ka < 0 && kb >= 0) {
            return false;
        }
        if (ka >= 0 && kb < 0) {
            return true;
        }
        if (ka < 0 && kb < 0) {
            return a.mChara->m50 < b.mChara->m50;
        }
        return ka < kb;
    }
};

// NONMATCHING: the original constructs the entries with a constructor (mChara, memset of mResult, the
// four counters) in the local array of 8 and in the temporary of the push_back; here they are cleared with
// memset (ExpEntry_unk has no constructor: a new inline in Inventory.h would renumber its 58 includers)
// inferred: shares the experience of the battle (m6014/m6018) among the party members that can act and
// adds it to the list of the last battle (sorted for game/BattleResult)
void Inventory::fn_80449ABC(int killer, int level) {
    atn::FixedArray_unk<ExpEntry_unk, 8> party;
    party.clear();
    ExpEntry_unk entry;
    memset(&entry, 0, sizeof(entry)); // inferred: the original zeroes it with a constructor
    for (PlayerTask* chara = lbl_8087F8A0->mHead; chara != 0; chara = chara->mNext) {
        if (chara->IsActive_unk() && (chara->m54C & 0x2000) != 0x2000 && chara->mC54 != 0 &&
            chara->fn_8016E484() && party.size() < 8) {
            entry.mChara = chara;
            party.push_back(entry);
        }
    }
    int expA = m6014;
    int expB = m6018;
    for (unsigned int i = 0; i < party.size(); i++) {
        party[i].mExpA = expA;
        party[i].mExpB = expB;
        if (lbl_8087F0A8->m298 != 0 && party[i].mChara == (CharaTask*)killer) {
            party[i].mExpA = (int)(1.2f * party[i].mExpA);
            party[i].mExpB = (int)(1.2f * party[i].mExpB);
        }
    }
    int top = lbl_8087F430->fn_80375D0C();
    if (level > top && lbl_8087F0A8->mD4 == 3) {
        top = level;
    }
    for (unsigned int i = 0; i < party.size(); i++) {
        ExpEntry_unk& e = party[i];
        fn_80449448(&e.mExpA, &e.mExpB, e.mChara->mStatus.mParamA.m78, top);
        e.mExp = e.mExpA + e.mExpB;
        e.mLevelUp = e.mChara->mStatus.fn_8013354C(e.mExp, &e.mResult);
    }
    unsigned int count = mExpEntries.size();
    for (unsigned int i = 0; i < party.size(); i++) {
        ExpEntry_unk& e = party[i];
        ExpEntry_unk* p = 0;
        for (unsigned int j = 0; j < mExpEntries.size(); j++) {
            if (mExpEntries[j].mChara == e.mChara) {
                p = &mExpEntries[j];
                break;
            }
        }
        if (p == 0 && mExpEntries.size() < 8) {
            ExpEntry_unk added;
            memset(&added, 0, sizeof(added));
            added.mChara = e.mChara;
            mExpEntries.push_back(added);
            p = &mExpEntries[mExpEntries.size() - 1];
        }
        if (p != 0) {
            p->mExpA += e.mExpA;
            p->mExpB += e.mExpB;
            p->mExp += e.mExp;
            p->mLevelUp += e.mLevelUp;
            p->mResult.m00 += e.mResult.m00;
            p->mResult.m04 += e.mResult.m04;
            p->mResult.m08 += e.mResult.m08;
            p->mResult.m0C += e.mResult.m0C;
            p->mResult.m10 += e.mResult.m10;
            p->mResult.m14 += e.mResult.m14;
            p->mResult.m18 += e.mResult.m18;
            p->mResult.m1C += e.mResult.m1C;
            p->mResult.m20 += e.mResult.m20;
            p->mResult.m44[0] |= e.mResult.m44[0];
            p->mResult.m44[1] |= e.mResult.m44[1];
            p->mResult.m44[2] |= e.mResult.m44[2];
            p->mResult.m44[3] |= e.mResult.m44[3];
        }
    }
    if (count != mExpEntries.size()) {
        MslSort_unk(mExpEntries.begin(), mExpEntries.end(), ExpOrderLess_unk());
    }
    m600C += m6014;
    m6010 += m6018;
    m6014 = 0;
    m6018 = 0;
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
