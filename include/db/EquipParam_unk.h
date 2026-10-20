#ifndef DB_EQUIPPARAM_UNK_H
#define DB_EQUIPPARAM_UNK_H

#include "atn/Memory.h"
#include "gmk/SeTable.h"

extern "C" void* memset(void* dst, int c, unsigned long n);

// inferred: two lists of up to 7 values (":"-separated columns), as in db/CharaParam_unk.cpp
struct EquipParamLists_unk {
    EquipParamLists_unk() {
        for (int i = 0; i < 7; i++) {
            mA[i] = 0.0f;
            mB[i] = 0.0f;
        }
    }
    // inferred: copies each list as a block of its own (the upgraded copies at 0x80212840 and
    // 0x80212C34 do not merge the two lists)
    EquipParamLists_unk& operator=(const EquipParamLists_unk& other) {
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
struct EquipParamFive_unk {
    EquipParamFive_unk() : m00(0), m04(0), m08(0), m0C(0), m10(0) {}
    int m00;
    int m04;
    int m08;
    int m0C;
    int m10;
};

// inferred: one row of /boot/equip_param.csv (0x10C bytes)
struct EquipParamEntry_unk {
    EquipParamEntry_unk()
        : m00(0.0f), m04(0.0f), m08(0.0f), m0C(0.0f), m10(1.0f), m14(1.0f), m18(0.0f), m1C(0.0f), m20(0), m24(0),
          m28(0.0f), m2C(0.0f), m30(0), m34(0), mKind(5), mId(0), mBC(0), mE0(0) {
        memset(mAC, 0, sizeof(mAC));
        mName = 0;
        mText = 0;
    }
    ~EquipParamEntry_unk() {}

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
    EquipParamLists_unk m38;
    int m70[2];
    int mKind; // inferred: index of the id prefix (sKinds)
    int mId;   // inferred: number after the prefix
    const wchar_t* mName;
    const wchar_t* mText;
    atn::FixedString_unk<0x20> mModel; // inferred: "pc..." model names
    int mA8;   // inferred: from the 10th letter of a "pc" model name
    int mAC[4];
    int mBC;
    atn::FixedString_unk<0x10> mC0;
    atn::FixedString_unk<0x10> mD0;
    int mE0;
    EquipParamFive_unk mE4[2];
};

// inferred: accessors of db/EquipParam_unk.cpp
extern "C" EquipParamEntry_unk* fn_8020ED84(int kind, int id); // inferred: entry of a kind and id
extern "C" int fn_8020EE58();                                  // inferred: number of entries
extern "C" EquipParamEntry_unk* fn_8020EE60(int index);         // inferred: entry by index
extern "C" bool fn_8020EF04(int code);                          // inferred: whether an item code is equipment
extern "C" bool fn_8020EF4C(EquipParamEntry_unk* entry);
extern "C" EquipParamEntry_unk* fn_8020EFEC(int code);          // inferred: entry of an item code

#endif
