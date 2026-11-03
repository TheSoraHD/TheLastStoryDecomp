#ifndef DB_ITEMPARAM_UNK_H
#define DB_ITEMPARAM_UNK_H

#include "gmk/SeTable.h"
#include "db/ParseUtil_unk.h"

// inferred: five numbers
struct ItemParamFive_unk {
    ItemParamFive_unk() : m00(0), m04(0), m08(0), m0C(0), m10(0) {}
    int m00;
    int m04;
    int m08;
    int m0C;
    int m10;
};

// inferred: one row of /boot/item_param.csv (0x138 bytes); the last 0x100 rows are free slots for the
// upgraded weapons and equipment
struct ItemParamEntry_unk : public BulletParam_unk {
    ItemParamEntry_unk();

    short mBC;
    short mBE;
    unsigned char mItemKind; // inferred: index in sKinds
    unsigned char mUser;  // inferred: index in sUsers
    unsigned char mC2;
    unsigned char mC3;
    int mC4;
    int mC8; // inferred: price
    atn::FixedString_unk<0x10> mCC;
    atn::FixedString_unk<0x10> mDC;
    ItemParamFive_unk mEC[2];
    atn::FixedString_unk<0x8> m114;
    int m11C[4];
    unsigned char m12C[4];
    int m130;
    int m134;
};

// inferred: accessors of db/ItemParam_unk.cpp
extern "C" int fn_802114D8();                         // inferred: number of entries
extern "C" ItemParamEntry_unk* fn_802114E0(int index); // inferred: entry by index
extern "C" ItemParamEntry_unk* fn_80211480(int code);  // inferred: entry of an item code

#endif
