#include "gmk/PassBoard.h"
#include "game/GameTask.h"
#include "atn/Csv.h"
#include "atn/Material_unk.h"
#include "atn/Memory.h"
#include "atn/SoundRef.h"
#include "game/MapTask.h"
#include "game/MapInstance_unk.h"
#include "game/MapPartsLocatorSet.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" char lbl_8087D710[2]; // inferred: " " (field separators of the tab CSV)
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash

// inferred: names of the objects of each board, indexed by GimmickCommon::m4C - 1 (0x80753960)
struct PassBoardNames_unk {
    int mCount;
    char mNames[5][0x40];
};
extern "C" PassBoardNames_unk lbl_80753960[];
extern "C" GameTask* lbl_8087F430;
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
// inferred: sound of the board (the name is its own object, pointed to from .sdata2)
static const char sSeName_unk[] = "SE_DG040_008";
static const char* const sSe_unk = sSeName_unk;

// inferred: rotation matrix z * y * x (same as GimmickCommon.cpp)
inline void __MakeRotation_unk(atn::Matrix3x4& m, const atn::Vector3& r) {
    m.SetIdentity();
    if (r.z != 0.0f) {
        Mtx t;
        Mtx a;
        PSMTXRotRad(a, 'z', r.z);
        PSMTXConcat(m.m, a, t);
        __MtxCopy(m.m, t);
    }
    if (r.y != 0.0f) {
        Mtx t;
        Mtx a;
        PSMTXRotRad(a, 'y', r.y);
        PSMTXConcat(m.m, a, t);
        __MtxCopy(m.m, t);
    }
    if (r.x != 0.0f) {
        Mtx t;
        Mtx a;
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
        return new ((MemoryTag)12, "", "", 0) PassBoard(parent, id, arg);
    }
    return 0;
}

// NONMATCHING: 98.67% - registers only: the original colours the pair cursor (this + 8 * i, stores at
// 0x864/0x868) and the name cursor after count/object/hash (here they come first). Tried: pair/name as
// named cursors, `&mPairs[i]` in a local, names->mNames[i], extra declarations.
// The pairs are indexed (the stores fold the 0x864 of mPairs into the offset, as in the original) and
// mColli.m0C is set even without a map (the original jumps there from both null checks).
int PassBoard::vf0C() {
    int count;
    MapInstance_unk* object;
    unsigned int hash;
    char* name;
    PassBoardNames_unk* names;
    MapTask* group;
    MapPartsBase* part;
    int i;
    int j;
    if (fn_803EC91C() == 0) {
        if (lbl_8087F430 != 0) {
            names = &lbl_80753960[m4C - 1];
            group = lbl_8087F430->GetCurrentMap();
            if (group != 0) {
                for (part = group->mParts; part != 0; part = part->mNext) {
                    if (part->mType == 4) {
                        count = names->mCount;
                        name = names->mNames[0];
                        for (i = 0; i < count; i++) {
                            for (j = 0; j < ((MapPartsLocatorSet*)part)->mInstances.mSize; j++) {
                                object = ((MapPartsLocatorSet*)part)->mInstances[j];
                                hash = object->mHash;
                                if (hash == fn_800DC6B4(name)) {
                                    mPairs[i].mB = object;
                                    mPairs[i].mA = object;
                                }
                            }
                            name += 0x40;
                        }
                    }
                }
            }
        }
        mColli.m0C = (int)this;
        return 1;
    }
    return 0;
}

void PassBoard::Start() {
    vf2C();
    GimmickMsg_unk msg(1);
    vf40(&msg);
}

// inferred: sound of the board when the pair passes (name invented)
static inline void PlaySe_unk(atn::Vector3* pos) {
    fn_800C344C(sSe_unk, pos, 0, -1, 1.0f);
}

// inferred: collision box of the board, placed at a far point along its rotation (name invented)
static inline void SetColliBox_unk(PassBoard* self) {
    atn::Vector3 pos(1.0e16f, 0.0f, 0.0f);
    atn::Matrix3x4 mtx;
    __MakeRotation_unk(mtx, self->m78);
    PSMTXMultVec(mtx.m, &pos, &pos);
    self->mColli.fn_800575BC(&pos, &self->m78, (atn::Vector3*)&self->mBox[6]);
}

// inferred: fades the materials and enables the collision box by the distance of the nearest pair
// The stack of the original: the second rotation (collision box) and the sound are one inline level
// deeper than the first rotation (SetColliBox_unk, PlaySe_unk), t above a in __MakeRotation_unk; the
// float registers: the position of the pair as GetTrans_unk of its instance matrix, the two absolute
// values and the numerator/denominator of the rate as named locals.
void PassBoard::vf10() {
    if (m54 == 0) {
        Start();
    }
    float dist = -60.0f;
    for (int i = 0; i < 5; i++) {
        if (mPairs[i].mB != 0 && mPairs[i].mA != 0) {
            atn::Vector3 pos = ((MapInstance_unk*)mPairs[i].mA)->mInst.mMtx.GetTrans_unk();
            PSMTXMultVec(((MapInstance_unk*)mPairs[i].mB)->mInst.mMtx.m, &pos, &pos);
            float d = (float)__fabs(dist);
            float y = (float)__fabs(pos.y);
            if (y < d) {
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
                PlaySe_unk(&pos);
            }
            break;
        }
    }
    float rate = 0.0f;
    if (m860 == 1) {
        float den = mDistance[1] - mDistance[0];
        float num = dist - mDistance[0];
        rate = num / den;
    } else if (m860 == 2) {
        rate = 1.0f;
    } else if (m860 == 3) {
        float den = mDistance[3] - mDistance[2];
        float num = mDistance[3] - dist;
        rate = num / den;
    }
    rate = (rate > 0.0f ? rate : 0.0f) < 1.0f ? (rate > 0.0f ? rate : 0.0f) : 1.0f;
    for (int i = 0; i < 2; i++) {
        Material_unk* material = mModel.fn_80092954(mMaterials[i]);
        if (material != 0) {
            PassColor_unk color(material->mColor);
            color.a = rate;
            material->mColor = color.ToGX_unk();
            material->fn_8007FAF0(3);
        }
    }
    mModel.mFlags |= 0x10;
    if (m860 == 2) {
        SetColliBox_unk(this);
        mColli.mFlags |= 1;
        mColli.m20 = 0x4000;
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
            mModel.fn_8008AD4C(csv.fn_8005B9CC(), 0);
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

atn::Model* PassBoard::vf38() {
    return &mModel;
}

} // namespace gmk
