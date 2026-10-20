#ifndef DB_WEAPONPARAM_UNK_H
#define DB_WEAPONPARAM_UNK_H

#include "atn/Memory.h"
#include "gmk/SeTable.h"

extern "C" void* memset(void* dst, int c, unsigned long n);

// inferred: two lists of up to 7 values (":"-separated columns), as in db/EquipParam_unk.cpp
struct WeaponParamLists_unk {
    WeaponParamLists_unk() {
        for (int i = 0; i < 7; i++) {
            mA[i] = 0.0f;
            mB[i] = 0.0f;
        }
    }
    // inferred: copies each list as a block of its own (the upgraded copies at 0x80212840 and
    // 0x80212C34 do not merge the two lists)
    WeaponParamLists_unk& operator=(const WeaponParamLists_unk& other) {
        *(List_unk*)mA = *(const List_unk*)other.mA;
        *(List_unk*)mB = *(const List_unk*)other.mB;
        return *this;
    }

    struct List_unk {
        float mValues[7];
    };
    float mA[7];
    float mB[7];
};

// inferred: five numbers
struct WeaponParamFive_unk {
    WeaponParamFive_unk() : m00(0), m04(0), m08(0), m0C(0), m10(0) {}
    int m00;
    int m04;
    int m08;
    int m0C;
    int m10;
};

// inferred: one row of /boot/weapon_param.csv (0x130 bytes; same prefix as EquipParamEntry_unk)
struct WeaponParamEntry_unk {
    WeaponParamEntry_unk()
        : m00(0.0f), m04(0.0f), m08(0.0f), m0C(0.0f), m10(1.0f), m14(1.0f), m18(0.0f), m1C(0.0f), m20(0), m24(0),
          m28(0.0f), m2C(0.0f), m30(0), m34(0), mKind(3), mId(0), mFlags(0), mC0(0), mEC(0), mF0(0), m114(0),
          m118(0), m11C(0), m120(0), m124(0), m128(0), m12C(0) {
        m20 = 1;
        memset(mAC, 0, sizeof(mAC));
        mName = 0;
        mText = 0;
    }
    ~WeaponParamEntry_unk();

    float m00;
    float m04;
    float m08;
    float m0C;
    float m10;
    float m14;
    float m18;
    float m1C;
    int m20;
    int m24;
    float m28;
    float m2C;
    int m30;
    int m34;
    WeaponParamLists_unk m38;
    int m70[2];
    int mKind; // inferred: 0 "ws", 1 "wg", 2 other prefix
    int m7C;
    int mId;   // inferred: number after the prefix
    const wchar_t* mName;
    const wchar_t* mText;
    atn::FixedString_unk<0x20> mModel;
    int mAC[3];
    int mType;  // inferred: index in sTypes
    int mFlags; // inferred: characters that can use it (bit per member, 0x8000.. for guests)
    int mC0;
    WeaponParamFive_unk mC4[2];
    int mEC;
    int mF0;
    atn::FixedString_unk<0x10> mF4;
    atn::FixedString_unk<0x10> m104;
    int m114;
    int m118; // inferred: index in sRanks
    int m11C; // inferred: kind of the linked weapon
    int m120; // inferred: id of the linked weapon
    int m124;
    int m128;
    int m12C;
};

// inferred: accessors of db/WeaponParam_unk.cpp
extern "C" WeaponParamEntry_unk* fn_80206B14(int kind, int id); // inferred: entry of a kind and id
extern "C" int fn_80206B68();                                   // inferred: number of entries
extern "C" WeaponParamEntry_unk* fn_80206B70(int index);         // inferred: entry by index
extern "C" bool fn_80206B9C(int code);                           // inferred: whether an item code is a weapon
extern "C" WeaponParamEntry_unk* fn_80206C50(int code);          // inferred: entry of an item code

#endif
