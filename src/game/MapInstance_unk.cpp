// inferred file name (splits-auto): model instances placed by game/MapPartsLocatorSet (0x90 bytes,
// created with new(0xC)); names invented.
#include "gmk/Lemon.h"

extern "C" InstData_unk* fn_8009F788(const char* path); // inferred: instance data of a model file
extern "C" int fn_8009FFCC(InstData_unk* data);          // inferred: nonzero while loading
extern "C" unsigned int fn_800DC6B4(const char* str);    // inferred: hash of a name
extern "C" void PSMTXScale(Mtx m, float x, float y, float z);
extern "C" void PSMTXTrans(Mtx m, float x, float y, float z);
extern "C" void PSMTXRotRad(Mtx m, char axis, float rad);
extern "C" void PSMTXConcat(const Mtx a, const Mtx b, Mtx ab);

// inferred: LOD distances of a locator
struct MapLod_unk {
    unsigned char m00[0x04];
    float m04;
    float m08;
    float m0C;
};

// inferred: locator of a map model (name, position, rotation in degrees, scale)
struct MapLocator_unk {
    const char* mName;     // 0x00
    atn::Vector3 mPos;     // 0x04
    atn::Vector3 mRot;     // 0x10
    atn::Vector3 mScale;   // 0x1C
    unsigned char m28[0x04];
    MapLod_unk* mLod;      // 0x2C
};

// inferred: locator with a shear (m30: xy, m34: xz, m38: yz)
struct MapLocatorShear_unk : public MapLocator_unk {
    float m30;
    float m34;
    float m38;
};

class MapInstance_unk {
public:
    MapInstance_unk(const char* path, MapLocator_unk* loc);
    MapInstance_unk(const char* path, MapLocatorShear_unk* loc);
    int fn_80490DE8();
    void fn_80490E38();
    void fn_80490EA0(int arg);

    atn::ModelInstance mInst;  // 0x00
    unsigned int mHidden : 1;  // 0x88
    unsigned int mFrame : 15;
    unsigned int mId : 16;
    unsigned int mHash;        // 0x8C
};

static int sCount_unk;

// inferred: degrees to radians (name invented)
inline float DegToRad_unk(float deg) {
    return 0.017453292f * deg;
}

// inferred: *this = *this * rot(axis, rad) (see game/CharaTask.cpp)
inline void Rotate_unk(atn::Matrix3x4& mtx, char axis, float rad) {
    Mtx r;
    Mtx t;
    PSMTXRotRad(r, axis, rad);
    PSMTXConcat(mtx.m, r, t);
    __MtxCopy(mtx.m, t);
}

// inferred: *this = *this * rot_z * rot_y * rot_x (name invented)
inline void RotateEuler_unk(atn::Matrix3x4& m, const atn::Vector3& rot) {
    if (0.0f != rot.z) {
        Rotate_unk(m, 'z', rot.z);
    }
    if (0.0f != rot.y) {
        Rotate_unk(m, 'y', rot.y);
    }
    if (0.0f != rot.x) {
        Rotate_unk(m, 'x', rot.x);
    }
}

// NONMATCHING: 94.1% - instruction scheduling of the id/counter stores against the degree conversion
// and of the name load in the LOD block (tried id before/after the rotation, mem-initializers,
// Vector3 * float, DegToRad helper, inline Euler rotation).
MapInstance_unk::MapInstance_unk(const char* path, MapLocator_unk* loc) : mInst(fn_8009F788(path)) {
    atn::Vector3 rot(DegToRad_unk(loc->mRot.x), DegToRad_unk(loc->mRot.y), DegToRad_unk(loc->mRot.z));
    mId = sCount_unk++;
    mFrame = 0;
    mHidden = 0;
    atn::Matrix3x4 mtx;
    PSMTXScale(mtx.m, loc->mScale.x, loc->mScale.y, loc->mScale.z);
    atn::Matrix3x4 r;
    r.SetIdentity();
    RotateEuler_unk(r, rot);
    PSMTXConcat(r.m, mtx.m, mtx.m);
    Mtx t;
    PSMTXTrans(t, loc->mPos.x, loc->mPos.y, loc->mPos.z);
    PSMTXConcat(t, mtx.m, mtx.m);
    mInst.fn_8009EE30(mtx);
    MapLod_unk* lod = loc->mLod;
    mInst.m70 = lod->m0C;
    mInst.m74 = (-1.0f * lod->m04) / mInst.m70;
    mInst.m78 = 1.0f - (lod->m08 + mInst.m70) / mInst.m70;
    mHash = fn_800DC6B4(loc->mName);
}

// NONMATCHING: 93.1% - same scheduling differences as the other constructor.
MapInstance_unk::MapInstance_unk(const char* path, MapLocatorShear_unk* loc) : mInst(fn_8009F788(path)) {
    atn::Vector3 rot(DegToRad_unk(loc->mRot.x), DegToRad_unk(loc->mRot.y), DegToRad_unk(loc->mRot.z));
    mId = sCount_unk++;
    mFrame = 0;
    mHidden = 0;
    atn::Matrix3x4 mtx;
    PSMTXScale(mtx.m, loc->mScale.x, loc->mScale.y, loc->mScale.z);
    atn::Matrix3x4 shear(1.0f, loc->m30, loc->m34, 0.0f, 0.0f, 1.0f, loc->m38, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    PSMTXConcat(shear.m, mtx.m, mtx.m);
    atn::Matrix3x4 r;
    r.SetIdentity();
    RotateEuler_unk(r, rot);
    PSMTXConcat(r.m, mtx.m, mtx.m);
    Mtx t;
    PSMTXTrans(t, loc->mPos.x, loc->mPos.y, loc->mPos.z);
    PSMTXConcat(t, mtx.m, mtx.m);
    mInst.fn_8009EE30(mtx);
    MapLod_unk* lod = loc->mLod;
    mInst.m70 = lod->m0C;
    mInst.m74 = (-1.0f * lod->m04) / mInst.m70;
    mInst.m78 = 1.0f - (lod->m08 + mInst.m70) / mInst.m70;
    mHash = fn_800DC6B4(loc->mName);
}

// inferred: updates the matrix once loaded
int MapInstance_unk::fn_80490DE8() {
    if (fn_8009FFCC(mInst.mData) != 0) {
        return 0;
    }
    mInst.fn_8009EE30(mInst.mMtx);
    return 1;
}

// inferred: LOD selection every 8 frames (spread by the id)
void MapInstance_unk::fn_80490E38() {
    if ((int)(mId + mFrame) % 8 == 0) {
        mInst.fn_8009EB20();
    }
    mFrame++;
}

void MapInstance_unk::fn_80490EA0(int arg) {
    if (mHidden == 1) {
        return;
    }
    mInst.fn_8009E6EC(arg);
}
