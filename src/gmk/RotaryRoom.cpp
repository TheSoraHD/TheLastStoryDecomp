#include "gmk/RotaryRoom.h"
#include "atn/Csv.h"
#include "atn/Memory.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" int atoi(const char* s);
extern "C" double cos(double x);
extern "C" char lbl_8087D710[2]; // inferred: " " (field separators of the tab CSV)
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
extern "C" atn::Vector3 lbl_807C7030;              // inferred: zero vector
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_80087E9C(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void fn_80088724(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);

// inferred: part of a map group (as ArenaPart_unk in gmk/ArenaFloor.cpp)
struct RotaryPart_unk {
    unsigned char m00[0x48];
    int mType;
    RotaryPart_unk* mNext;
};
// inferred: map group (as ArenaGroup_unk in gmk/ArenaFloor.cpp)
struct RotaryGroup_unk {
    unsigned char m00[0x38];
    unsigned int mFlags; // 0x38: inferred: bit 1 set while loading
    unsigned char m3C[0x70 - 0x3C];
    RotaryPart_unk* mParts; // 0x70
    unsigned char m74[0xF4 - 0x74];
    int mF4;
};
extern "C" RotaryGroup_unk* fn_8049D68C(void* owner, const char* name);
extern "C" void fn_8049994C(RotaryPart_unk* part, const atn::Vector3* pos, const atn::Vector3* rot);
extern "C" void fn_80499B9C(RotaryPart_unk* part);
extern "C" void fn_80499C48(RotaryPart_unk* part);

// NONMATCHING (__sinit): 99.71% - only .sdata2 order.
// inferred: far away position and the 24 orientations of the room (x, y, z rotations)
static atn::Vector3 sFar(5000.0f, 5000.0f, 5000.0f);
static atn::Vector3 sRots[24] = {
    atn::Vector3(0.0f, 0.0f, 0.0f),
    atn::Vector3(0.0f, 1.5707964f, 0.0f),
    atn::Vector3(0.0f, 3.1415927f, 0.0f),
    atn::Vector3(0.0f, -1.5707964f, 0.0f),
    atn::Vector3(0.0f, 0.0f, 1.5707964f),
    atn::Vector3(1.5707964f, 1.5707964f, 0.0f),
    atn::Vector3(0.0f, 3.1415927f, -1.5707964f),
    atn::Vector3(1.5707964f, -1.5707964f, 0.0f),
    atn::Vector3(0.0f, 0.0f, 3.1415927f),
    atn::Vector3(-3.1415927f, 1.5707964f, 0.0f),
    atn::Vector3(-3.1415927f, 0.0f, 0.0f),
    atn::Vector3(-3.1415927f, -1.5707964f, 0.0f),
    atn::Vector3(0.0f, 0.0f, -1.5707964f),
    atn::Vector3(-1.5707964f, 1.5707964f, 0.0f),
    atn::Vector3(0.0f, 3.1415927f, 1.5707964f),
    atn::Vector3(1.5707964f, -1.5707964f, 0.0f),
    atn::Vector3(-1.5707964f, 0.0f, 0.0f),
    atn::Vector3(0.0f, 1.5707964f, 1.5707964f),
    atn::Vector3(1.5707964f, 3.1415927f, 0.0f),
    atn::Vector3(0.0f, -1.5707964f, -1.5707964f),
    atn::Vector3(1.5707964f, 0.0f, 0.0f),
    atn::Vector3(0.0f, 1.5707964f, -1.5707964f),
    atn::Vector3(-1.5707964f, 3.1415927f, 0.0f),
    atn::Vector3(0.0f, -1.5707964f, 1.5707964f),
};

// inferred: next orientation (1-24) after a turn (1000-1003) from each orientation
static const int sTurns_unk[24][4] = {
    {21, 5, 11, 9},
    {6, 18, 10, 12},
    {19, 15, 9, 11},
    {16, 24, 12, 10},
    {24, 9, 15, 13},
    {10, 19, 14, 16},
    {20, 3, 13, 15},
    {4, 23, 16, 14},
    {23, 13, 3, 1},
    {14, 20, 2, 4},
    {17, 7, 1, 3},
    {8, 22, 4, 2},
    {22, 1, 7, 5},
    {2, 17, 6, 8},
    {18, 11, 5, 7},
    {12, 21, 8, 6},
    {1, 8, 21, 23},
    {5, 12, 24, 22},
    {9, 16, 23, 21},
    {13, 4, 22, 24},
    {11, 6, 17, 19},
    {7, 2, 20, 18},
    {3, 14, 19, 17},
    {15, 10, 18, 20},
};

// inferred: the collision is enabled (name invented)
static inline bool IsOn_unk(atn::ColliTree* tree) {
    return (tree->mFlags & 1) == 1;
}

namespace gmk {

GimmickCommon* RotaryRoom::fn_80423F64(atn::Task* parent, int id, int arg) {
    if (parent != 0) {
        return new (12, "", "", 0) RotaryRoom(parent, id, arg);
    }
    return 0;
}

RotaryRoom::~RotaryRoom() {
    for (unsigned int i = 0; i < 25; i++) {
        if (mColli[i].mTree != 0) {
            delete mColli[i].mTree;
        }
    }
}

void RotaryRoom::vf28() {
    GimmickMsg_unk msg(m58 + 1);
    vf40(&msg);
}

// NONMATCHING: 95.59% - the original reloads the collision flags before each write and uses other
// registers for the loop and the frame counters; .sdata2 order (vf0C is not decompiled yet).
void RotaryRoom::vf10() {
    fn_80424FB4(0.5f * (1.0f - (float)cos(3.1415927f * ((float)mFrame / (float)mFrames))));
    RotaryColli_unk* colli = mColli;
    for (int i = 0; i < 25; i++, colli++) {
        if (colli->mTree != 0) {
            if (i == m54) {
                if (!IsOn_unk(colli->mTree)) {
                    colli->mTree->mFlags |= 1;
                }
            } else if (IsOn_unk(colli->mTree)) {
                colli->mTree->mFlags &= ~1;
            }
        }
    }
    fn_80425498();
    if (mFrame < mFrames) {
        if (++mFrame >= mFrames) {
            mFrame = mFrames;
            fn_80425878(1);
        }
    }
}

void RotaryRoom::fn_80425878(int on) {
    if (on) {
        for (RotaryPart_unk* part = mGroup->mParts; part != 0; part = part->mNext) {
            if (part->mType == 5) {
                fn_8049994C(part, &lbl_807C7030, &mRotOffset);
                fn_80499B9C(part);
            }
        }
    } else {
        for (RotaryPart_unk* part = mGroup->mParts; part != 0; part = part->mNext) {
            if (part->mType == 5) {
                fn_80499C48(part);
            }
        }
    }
}

void RotaryRoom::vf14() {}

int RotaryRoom::vf1C() {
    if (mGroup != 0 && (mGroup->mFlags & 2) != 2) {
        return 1;
    }
    RotaryColli_unk* colli = mColli;
    for (unsigned int i = 0; i < 25; i++, colli++) {
        if (colli->mTree != 0 && colli->mTree->fn_80058BBC() != 0) {
            return 1;
        }
    }
    return 0;
}

// NONMATCHING: 91.50% - the original keeps the constant 1 in a saved register (as in
// gmk::ArenaFloor::vf20): 16 bytes larger frame.
void RotaryRoom::vf20() {
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    csv.fn_8005BCE8(lbl_8087D710);
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "MAP") == 0) {
            void* owner = m20;
            if (owner != 0) {
                mGroup = fn_8049D68C(owner, csv.fn_8005B9CC());
                if (mGroup != 0) {
                    mGroup->mF4 = 1;
                }
            }
        } else if (strcmp(field, "STATE") == 0) {
            if (m20 != 0) {
                RotaryColli_unk& colli = mColli[atoi(csv.fn_8005B9CC())];
                colli.mTree = new (12, "", "", 0) atn::ColliTree;
                colli.mTree->fn_80058B78(csv.fn_8005B9CC());
                colli.mHash = fn_800DC6B4(csv.fn_8005B9CC());
            }
        }
    } while (csv.fn_8005BBF8() != 0);
}

// NONMATCHING: 99.83% - only .sdata2 order (vf0C is not decompiled yet).
int RotaryRoom::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    if (mFrame < mFrames) {
        fn_80424FB4(1.0f);
    }
    if (msg->mType >= 1 && 24 >= msg->mType) {
        fn_80424AD4();
        fn_80425878(0);
        m54 = msg->mType;
        mRotOffset = sRots[m54 - 1] - mRot;
        mRot = sRots[m54 - 1];
        mFrame = mFrames;
        fn_80425878(1);
    } else if (msg->mType >= 1000 && 1003 >= msg->mType) {
        fn_80424AD4();
        m54 = sTurns_unk[m54 - 1][msg->mType - 1000];
        switch (msg->mType) {
        case 1000:
            mRotOffset = atn::Vector3(1.5707964f, 0.0f, 0.0f);
            break;
        case 1001:
            mRotOffset = atn::Vector3(0.0f, 0.0f, 1.5707964f);
            break;
        case 1002:
            mRotOffset = atn::Vector3(3.1415927f, 0.0f, 0.0f);
            break;
        case 1003:
            mRotOffset = atn::Vector3(0.0f, 0.0f, 3.1415927f);
            break;
        }
        mRot = sRots[m54 - 1];
        mFrame = 0;
        fn_80425878(0);
    }
    return m54;
}

void RotaryRoom::vf70(int state) {
    GimmickMsg_unk msg(state);
    vf40(&msg);
    if (mE4 != 0) {
        mE4->m58 = 1;
    }
}

// NONMATCHING: 99.54% - only .sdata2 order.
void RotaryRoom::vf6C(void* menu) {
    char name[0x20];
    fn_803EC758(name);
    void* group;
    if (mF0.mRes == 0 && menu != 0 && lbl_8087EF10 != 0) {
        group = mF0.mRes = fn_8008937C(menu, name);
    } else {
        group = 0;
    }
    fn_800874C8(group, "state", &m54, 0, 99, 1, 0, 0);
    fn_80087E9C(group, "pos", &m6C, 0, 0, -9999.0f, 9999.0f, 0.25f);
    fn_80088724(group, "rot", &m78, 0, 0, -3.1415927f, 3.1415927f, 0.05235988f);
    fn_80087E9C(group, "scl", &m90, 0, 0, -9999.0f, 9999.0f, 0.25f);
    fn_80087994(group, "isShow", &m9C, 0, 0);
    fn_800874C8(group, "locData", &m58, 0, 99, 1, 0, 0);
    fn_800874C8(group, "rotOffsetFrame_", &mFrames, 0, 9999, 1, 0, 0);
    mB0.fn_803F11F8(group);
}

} // namespace gmk
