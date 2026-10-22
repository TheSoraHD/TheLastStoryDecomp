#include "gmk/RotaryRoom.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "atn/Model_unk.h"
#include "game/GimmickManager.h"
#include "game/MapPartsBase.h"
#include "gmk/Lemon.h"

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

namespace atn {
// inferred: axis aligned box (see atn/Geometry_unk.cpp)
struct AABB_unk {
    AABB_unk() {}
    AABB_unk Merge_unk(const AABB_unk& o) const; // 0x80070C98: union of both boxes
    Vector3 mMin;
    Vector3 mMax;
};
} // namespace atn

// inferred: box pair of a locator (0x18 bytes); name invented
struct RotaryBox_unk {
    atn::Vector3 mMin;
    atn::Vector3 mMax;
};
// inferred: locator of a part of type 6; name invented
struct RotaryLocator_unk {
    unsigned char m00[0x24];
    unsigned int mHash;     // 0x24
    RotaryBox_unk* mBoxes;  // 0x28
    unsigned int mNumBoxes; // 0x2C
};
// inferred: part of a map group (as ArenaPart_unk in gmk/ArenaFloor.cpp); a MapPartsBase
struct RotaryPart_unk {
    unsigned char m00[0x48];
    int mType;
    RotaryPart_unk* mNext;
    unsigned char m50[0x58 - 0x50];
    RotaryLocator_unk** mLocators; // 0x58: inferred: type 6
    unsigned int mNumLocators;     // 0x5C
    unsigned char m60[0x344 - 0x60];
    int mObjCount; // 0x344: inferred: objects of a part of type 4
};
// inferred: map group (as ArenaGroup_unk in gmk/ArenaFloor.cpp)
struct RotaryGroup_unk {
    unsigned char m00[0x38];
    unsigned int mFlags; // 0x38: inferred: bit 1 set while loading
    unsigned char m3C[0x70 - 0x3C];
    RotaryPart_unk* mParts; // 0x70
    unsigned char m74[0x88 - 0x74];
    atn::Vector3 mMin; // 0x88: inferred: bounding box of the group
    atn::Vector3 mMax; // 0x94
    unsigned char mA0[0xF4 - 0xA0];
    int mF4;
};
extern "C" RotaryGroup_unk* fn_8049D68C(void* owner, const char* name);
extern "C" void fn_8049994C(RotaryPart_unk* part, const atn::Vector3* pos, const atn::Vector3* rot);
extern "C" void fn_80499B9C(RotaryPart_unk* part);
extern "C" void fn_80499C48(RotaryPart_unk* part);

// NONMATCHING (__sinit): 99.94% - only .sdata2 order (fn_80424AD4/fn_80424FB4/fn_80425498 not decompiled).
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

// inferred: sets the translation column (as in gmk/OnOff.cpp)
inline void __SetTrans_unk(atn::Matrix3x4& m, const atn::Vector3& v) {
    m.m[0][3] = v.x;
    m.m[1][3] = v.y;
    m.m[2][3] = v.z;
}
#define __MAX_unk(a, b) ((a) > (b) ? (a) : (b))

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

// NONMATCHING: 96.1% - saved registers of the counters and the stack slots of the temporaries
// differ; the locator loop recomputes the box address; the collision flags are reloaded before each
// write in the original (as in vf10). Emits the weak MapPartsBase::vf24 and RotaryEntry_unk ctor.
int RotaryRoom::vf0C() {
    if (fn_803EC91C() == 0) {
        m54 = 1;
        if (mGroup != 0) {
            int count = 0;
            for (RotaryPart_unk* part = mGroup->mParts; part != 0; part = part->mNext) {
                switch (part->mType) {
                case 0:
                    count++;
                    break;
                case 4:
                    count += part->mObjCount;
                    break;
                case 5:
                    for (GimmickCommon* g = lbl_8087F4A0->mHead; g != 0; g = g->m5C) {
                        if (g->m20 == part) {
                            count++;
                        }
                    }
                    break;
                }
            }
            mEntries.Resize_unk(count, 0);
            fn_80424AD4();
            for (unsigned int i = 0; i < mEntries.mSize; i++) {
                RotaryEntry_unk& e = mEntries.mData[i];
                __SetTrans_unk(e.mMtx, e.mMtx.GetTrans_unk() + m6C);
                switch (e.mType) {
                case 0: {
                    Model_unk* model = (Model_unk*)e.mObject;
                    *(atn::Matrix3x4*)model->mMtx = e.mMtx;
                    atn::Vector3 az(e.mMtx.m[0][2], e.mMtx.m[1][2], e.mMtx.m[2][2]);
                    float sz = PSVECMag(&az);
                    atn::Vector3 ay(e.mMtx.m[0][1], e.mMtx.m[1][1], e.mMtx.m[2][1]);
                    float sy = PSVECMag(&ay);
                    atn::Vector3 ax(e.mMtx.m[0][0], e.mMtx.m[1][0], e.mMtx.m[2][0]);
                    float sx = PSVECMag(&ax);
                    atn::Vector3 sc(sx, sy, sz);
                    model->m54 = __MAX_unk(sc.x, __MAX_unk(sc.y, sc.z));
                    break;
                }
                case 4:
                    ((atn::ModelInstance*)e.mObject)->fn_8009EE30(e.mMtx);
                    break;
                case 5: {
                    GimmickCommon* g = (GimmickCommon*)e.mObject;
                    g->m6C = g->m6C + m6C;
                    g->m78 = g->m78 + m78;
                    break;
                }
                }
            }
            for (RotaryPart_unk* part = mGroup->mParts; part != 0; part = part->mNext) {
                if (part->mType == 6) {
                    for (unsigned int j = 0; j < part->mNumLocators; j++) {
                        RotaryLocator_unk* loc = part->mLocators[j];
                        for (unsigned int k = 0; k < loc->mNumBoxes; k++) {
                            loc->mBoxes[k].mMin += m6C;
                            loc->mBoxes[k].mMax += m6C;
                        }
                    }
                }
            }
            for (unsigned int i = 0; i < 25; i++) {
                if (mColli[i].mHash != 0) {
                    for (RotaryPart_unk* part = mGroup->mParts; part != 0; part = part->mNext) {
                        if (part->mType == 6) {
                            for (unsigned int j = 0; j < part->mNumLocators; j++) {
                                if (part->mLocators[j]->mHash == mColli[i].mHash) {
                                    mColli[i].m08 = (int)part;
                                    break;
                                }
                            }
                            if (mColli[i].m08 != 0) {
                                break;
                            }
                        }
                    }
                }
            }
            MapPartsBox_unk box;
            box.mMin = atn::Vector3(1e16f, 1e16f, 1e16f);
            box.mMax = atn::Vector3(-1e16f, -1e16f, -1e16f);
            for (RotaryPart_unk* part = mGroup->mParts; part != 0; part = part->mNext) {
                MapPartsBox_unk pbox = ((MapPartsBase*)part)->vf24(0);
                *(atn::AABB_unk*)&box = ((atn::AABB_unk*)&box)->Merge_unk(*(atn::AABB_unk*)&pbox);
            }
            mGroup->mMin = box.mMin;
            mGroup->mMax = box.mMax;
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
        }
        return 1;
    }
    return 0;
}

void RotaryRoom::vf28() {
    GimmickMsg_unk msg(m58 + 1);
    vf40(&msg);
}

// NONMATCHING: 95.59% - the original reloads the collision flags before each write and uses other
// registers for the loop and the frame counters; .sdata2 order.
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

// NONMATCHING: 99.93% - only .sdata2 order (fn_80424AD4/fn_80424FB4/fn_80425498 not decompiled).
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

// NONMATCHING: 99.95% - only .sdata2 order.
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
