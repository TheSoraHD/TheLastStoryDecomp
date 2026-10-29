#include "gmk/PassBoard.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "atn/SoundRef.h"

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
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
// inferred: sound of the board (the name is its own object, pointed to from .sdata2)
static const char sSeName_unk[] = "SE_DG040_008";
static const char* const sSe_unk = sSeName_unk;

// inferred: rotation matrix z * y * x (same as GimmickCommon.cpp)
inline void __MakeRotation_unk(atn::Matrix3x4& m, const atn::Vector3& r) {
    m.SetIdentity();
    if (r.z != 0.0f) {
        Mtx a;
        Mtx t;
        PSMTXRotRad(a, 'z', r.z);
        PSMTXConcat(m.m, a, t);
        __MtxCopy(m.m, t);
    }
    if (r.y != 0.0f) {
        Mtx a;
        Mtx t;
        PSMTXRotRad(a, 'y', r.y);
        PSMTXConcat(m.m, a, t);
        __MtxCopy(m.m, t);
    }
    if (r.x != 0.0f) {
        Mtx a;
        Mtx t;
        PSMTXRotRad(a, 'x', r.x);
        PSMTXConcat(m.m, a, t);
        __MtxCopy(m.m, t);
    }
}

// inferred: colour with float components converted to a GXColor (names invented)
struct PassColor_unk {
    PassColor_unk(GXColor c) : r(c.r / 255.0f), g(c.g / 255.0f), b(c.b / 255.0f), a(c.a / 255.0f) {}
    GXColor ToGX_unk() const {
        GXColor c;
        c.r = (unsigned char)(255.0f * r);
        c.g = (unsigned char)(255.0f * g);
        c.b = (unsigned char)(255.0f * b);
        c.a = (unsigned char)(255.0f * a);
        return c;
    }
    float r, g, b, a;
};

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

void PassBoard::Start() {
    vf2C();
    GimmickMsg_unk msg(1);
    vf40(&msg);
}

// inferred: fades the materials and enables the collision box by the distance of the nearest pair
// NONMATCHING: 99.5% - stack layout: the matrices of the two inlined __MakeRotation_unk are placed in
// mirror order in the original (first block high, second block low), plus float registers of the
// nearest-pair loop and of the rate. Tried: declaration orders of pos/mtx, order of the temporaries
// in __MakeRotation_unk, a Front_unk helper (worse), pair pointer vs indexed pairs.
void PassBoard::vf10() {
    if (m54 == 0) {
        Start();
    }
    float dist = -60.0f;
    for (int i = 0; i < 5; i++) {
        if (mPairs[i].mB != 0 && mPairs[i].mA != 0) {
            float* a = (float*)mPairs[i].mA;
            atn::Vector3 pos(a[0x3C / 4], a[0x4C / 4], a[0x5C / 4]);
            PSMTXMultVec((MtxPtr)((char*)mPairs[i].mB + 0x30), &pos, &pos);
            if ((float)__fabs(pos.y) < (float)__fabs(dist)) {
                dist = pos.y;
            }
        }
    }
    int old = m860;
    m860 = 4;
    for (int i = 0; i < 4; i++) {
        if (dist < mDistance[i]) {
            m860 = i;
            if (old != i && i == 2) {
                atn::Vector3 pos(1.0e16f, 0.0f, 0.0f);
                atn::Matrix3x4 mtx;
                __MakeRotation_unk(mtx, m78);
                PSMTXMultVec(mtx.m, &pos, &pos);
                fn_800C344C(sSe_unk, &pos, 0, -1, 1.0f);
            }
            break;
        }
    }
    float rate = 0.0f;
    if (m860 == 1) {
        rate = (dist - mDistance[0]) / (mDistance[1] - mDistance[0]);
    } else if (m860 == 2) {
        rate = 1.0f;
    } else if (m860 == 3) {
        rate = (mDistance[3] - dist) / (mDistance[3] - mDistance[2]);
    }
    rate = (rate > 0.0f ? rate : 0.0f) < 1.0f ? (rate > 0.0f ? rate : 0.0f) : 1.0f;
    for (int i = 0; i < 2; i++) {
        Material_unk* material = mModel.fn_80092954(mMaterials[i]);
        if (material != 0) {
            PassColor_unk color(material->mColor);
            color.a = rate;
            material->mColor = color.ToGX_unk();
            fn_8007FAF0(material, 3);
        }
    }
    mModel.mFlags |= 0x10;
    if (m860 == 2) {
        atn::Vector3 pos(1.0e16f, 0.0f, 0.0f);
        atn::Matrix3x4 mtx;
        __MakeRotation_unk(mtx, m78);
        PSMTXMultVec(mtx.m, &pos, &pos);
        mColli.fn_800575BC(&pos, &m78, (atn::Vector3*)&mBox[6]);
        mColli.m20 = 0x4000;
        mColli.mFlags |= 1;
    } else {
        mColli.mFlags &= ~1;
    }
    if (vf38() != 0) {
        fn_803ED0D4(vf38(), 1);
    }
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
