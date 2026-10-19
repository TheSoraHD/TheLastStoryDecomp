#include "gmk/PassBoard.h"
#include "atn/Csv.h"
#include "atn/Memory.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" char lbl_8087D710[2]; // inferred: " " (field separators of the tab CSV)
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" void fn_8008AD4C(Model_unk* model, const char* path, const char* texture); // inferred: loads a model
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash

// inferred: names of the objects of each board, indexed by GimmickCommon::m4C - 1 (0x80753960)
struct PassBoardNames_unk {
    int mCount;
    char mNames[5][0x40];
};
extern "C" PassBoardNames_unk lbl_80753960[];
// inferred: map object (hash of its name at 0x8C)
struct PassObject_unk {
    unsigned char m00[0x8C];
    unsigned int mHash;
};
// inferred: map part with a list of objects
struct PassPart_unk {
    unsigned char m00[0x48];
    int mType;
    PassPart_unk* mNext; // 0x4C
    unsigned char m50[0x344 - 0x50];
    int mNumObjects;            // 0x344
    PassObject_unk** mObjects;  // 0x348
};
struct PassGroup_unk {
    unsigned char m00[0x70];
    PassPart_unk* mParts; // 0x70
};
struct Field_unk;
extern "C" Field_unk* lbl_8087F430;
extern "C" PassGroup_unk* fn_80373148(Field_unk* field);

namespace gmk {

GimmickCommon* PassBoard::fn_8042D628(atn::Task* parent, int id, int arg) {
    if (parent != 0) {
        return new (12, "", "", 0) PassBoard(parent, id, arg);
    }
    return 0;
}

// NONMATCHING: the target addresses the pairs from `this` (stw 0x868(r25) with r25 = this + 8 * i)
// where this keeps a pointer to the pair. Tried: indexed pairs (registers permute), locals declared
// at the top in several orders.
int PassBoard::vf0C() {
    int count;
    PassObject_unk* object;
    unsigned int hash;
    char* name;
    Pair_unk* pair;
    PassBoardNames_unk* names;
    PassGroup_unk* group;
    PassPart_unk* part;
    int i;
    int j;
    if (fn_803EC91C() == 0) {
        if (lbl_8087F430 != 0) {
            names = &lbl_80753960[m4C - 1];
            group = fn_80373148(lbl_8087F430);
            if (group != 0) {
                for (part = group->mParts; part != 0; part = part->mNext) {
                    if (part->mType == 4) {
                        count = names->mCount;
                        name = names->mNames[0];
                        pair = mPairs;
                        for (i = 0; i < count; i++) {
                            for (j = 0; j < part->mNumObjects; j++) {
                                object = part->mObjects[j];
                                hash = object->mHash;
                                if (hash == fn_800DC6B4(name)) {
                                    pair->mB = object;
                                    pair->mA = object;
                                }
                            }
                            name += 0x40;
                            pair++;
                        }
                    }
                }
                mColli.m0C = (int)this;
            }
        }
        return 1;
    }
    return 0;
}

void PassBoard::vf28() {
    vf2C();
    GimmickMsg_unk msg(1);
    vf40(&msg);
}

void PassBoard::vf14() {
    if (vf38() != 0) {
        fn_803ED774(vf38());
    }
}

int PassBoard::vf1C() {
    return mModel.fn_8008B140() != 0;
}

void PassBoard::vf20() {
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    csv.fn_8005BCE8(lbl_8087D710);
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "MODEL") == 0) {
            fn_8008AD4C(&mModel, csv.fn_8005B9CC(), 0);
            fn_803EC7A0(&mModel, &csv);
        } else if (strcmp(field, "COLLI_BOX") == 0) {
            mBox[0] = fn_800DC288(csv.fn_8005B9CC());
            mBox[1] = fn_800DC288(csv.fn_8005B9CC());
            mBox[2] = fn_800DC288(csv.fn_8005B9CC());
            mBox[3] = fn_800DC288(csv.fn_8005B9CC());
            mBox[4] = fn_800DC288(csv.fn_8005B9CC());
            mBox[5] = fn_800DC288(csv.fn_8005B9CC());
            mBox[6] = fn_800DC288(csv.fn_8005B9CC());
            mBox[7] = fn_800DC288(csv.fn_8005B9CC());
            mBox[8] = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(field, "MATERIAL_NAME") == 0) {
            for (int i = 0; i < 2; i++) {
                strcpy(mMaterials[i], csv.fn_8005B9CC());
            }
        } else if (strcmp(field, "DISTANCE") == 0) {
            for (int i = 0; i < 5; i++) {
                mDistance[i] = fn_800DC288(csv.fn_8005B9CC());
            }
        }
    } while (csv.fn_8005BBF8() != 0);
}

int PassBoard::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    m54 = msg->mType;
    if ((unsigned int)m54 > 1) {
        m54 = 0;
    }
    return m54;
}

void PassBoard::vf70(int state) {
    GimmickMsg_unk msg(state);
    vf40(&msg);
}

int PassBoard::vf60() {
    return m54 == 1;
}

void PassBoard::vf6C(void* menu) {}

Model_unk* PassBoard::vf38() {
    return &mModel;
}

} // namespace gmk
