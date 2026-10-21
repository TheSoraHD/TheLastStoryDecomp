#include "game/MapPartsLocatorSet.h"
#include "atn/Loader_unk.h"
#include "atn/Memory.h"
#include "game/GimmickCommon.h"
#include "std/string.h"
#include "std/string_members.h"
#include <boost/function.hpp>

extern "C" int strcmp(const char* a, const char* b);
extern "C" char* strstr(const char* s, const char* sub);
extern "C" int sprintf(char* s, const char* format, ...);
extern "C" float fn_800DC288(const char* s);         // inferred: string to float
extern "C" unsigned int fn_800DC6B4(const char* s);  // inferred: string hash
extern "C" int fn_8006AA20(const char* path);        // inferred: the file exists
extern "C" std::string fn_8006B174(const std::string& path); // inferred: file name of a path
extern "C" std::string fn_8006B2D8(const std::string& path); // inferred: name without extension
std::string fn_8006B0C8(const std::string& path);            // inferred: directory of a path
extern "C" void fn_8008AD4C(Model_unk* model, const char* path, const char* texture); // inferred: loads a model
extern "C" void fn_800D5908(atn::Texture_unk* texture, const char* path); // inferred: loads a texture
extern "C" void* fn_804962B0(void* tree, const atn::Sphere* sphere); // inferred: light of a place
extern "C" int fn_800502A8(void* frustum, const MapPartsBox_unk* box); // inferred: box visible

extern "C" void fn_805F9420(const Mtx m, const atn::Vector3* src, atn::Vector3* dst,
                            unsigned long n); // inferred: PSMTXMultVecArray

namespace atn {
// inferred: axis aligned box (see atn/Geometry_unk.cpp)
struct AABB_unk {
    AABB_unk() {}
    AABB_unk(const Vector3& min, const Vector3& max) : mMin(min), mMax(max) {}
    AABB_unk Merge_unk(const AABB_unk& o) const; // 0x80070C98: union of both boxes
    Vector3 mMin;
    Vector3 mMax;
};
} // namespace atn

// inferred: resource data of an instance (bounding box at 0x10)
struct LocatorInstRes_unk {
    unsigned char m00[0x10];
    atn::Vector3 mMin;
    atn::Vector3 mMax;
};
struct LocatorInstData_unk {
    unsigned char m000[0x17C];
    LocatorInstRes_unk* m17C;
};
extern "C" atn::AABB_unk fn_80095300(Model_unk* model); // inferred: bounding box of a model

// inferred: box of the 8 corners of `box` transformed by `m` (same shape as ProjectBox_unk of
// atn/Geometry_unk.cpp, with a 3x4 matrix)
inline atn::AABB_unk TransformBox_unk(const Mtx m, const atn::AABB_unk* box) {
    float maxZ = box->mMax.z;
    float maxY = box->mMax.y;
    float maxX = box->mMax.x;
    float minX = box->mMin.x;
    float minY = box->mMin.y;
    float minZ = box->mMin.z;
    atn::Vector3 corners[8];
    corners[0] = atn::Vector3(maxX, maxY, maxZ);
    corners[1] = atn::Vector3(minX, maxY, maxZ);
    corners[2] = atn::Vector3(maxX, minY, maxZ);
    corners[3] = atn::Vector3(minX, minY, maxZ);
    corners[4] = atn::Vector3(maxX, maxY, minZ);
    corners[5] = atn::Vector3(minX, maxY, minZ);
    corners[6] = atn::Vector3(maxX, minY, minZ);
    corners[7] = atn::Vector3(minX, minY, minZ);
    fn_805F9420(m, corners, corners, 8);
    atn::Vector3 max(corners[0]);
    atn::Vector3 min(corners[0]);
    __VecMinMax(&min.x, &max.x, &min.x, &max.x, &corners[1].x);
    __VecMinMax(&min.x, &max.x, &min.x, &max.x, &corners[2].x);
    __VecMinMax(&min.x, &max.x, &min.x, &max.x, &corners[3].x);
    __VecMinMax(&min.x, &max.x, &min.x, &max.x, &corners[4].x);
    __VecMinMax(&min.x, &max.x, &min.x, &max.x, &corners[5].x);
    __VecMinMax(&min.x, &max.x, &min.x, &max.x, &corners[6].x);
    __VecMinMax(&min.x, &max.x, &min.x, &max.x, &corners[7].x);
    return atn::AABB_unk(min, max);
}

// inferred: settings (only the members used here)
struct LocatorConfig_unk {
    unsigned char m000[0x18];
    int mDrawMap; // 0x18
    unsigned char m01C[0x5B0 - 0x1C];
    int m5B0; // inferred: use the merged occlusion models
    int m5B4; // inferred: use the merged locator models
};
extern "C" LocatorConfig_unk* lbl_8087F0A8;

// inferred: camera (frustum at 0x204)
struct LocatorCamera_unk {
    unsigned char m000[0x204];
    unsigned char m204[0x10];
};
extern "C" LocatorCamera_unk* lbl_8087EFB4;
extern "C" void* fn_800C16B4(LocatorCamera_unk* camera); // inferred: default light of the camera

struct LocatorDebug_unk {
    unsigned char m00[0x38];
    unsigned int m38; // inferred: debug flags (4: no culling)
};
extern "C" LocatorDebug_unk* lbl_8087FA20;

// inferred: ".building" file (names invented)
struct BuildingFile_unk {
    unsigned char m00[0x10];
    unsigned int mKind;   // 0x10: 'locs' = locators with shear (0x3C bytes), else 0x30 bytes
    unsigned char m14[0x20 - 0x14];
    const char* mTexture; // 0x20: inferred: light map
    int mCount;           // 0x24
    char* mLocators;      // 0x28
};

// inferred: creator (declared extern "C" by game/MapPartsFactory.cpp)
extern "C" MapPartsBase* fn_8049A018(atn::Task* parent, void* arg, atn::CsvTab* csv) {
    if (parent != 0) {
        return new (12, "", "", 0) MapPartsLocatorSet(parent, arg, csv);
    }
    return 0;
}

// NONMATCHING: 99.91% - r27/r28 (length and string) swapped in the second merged-model block
MapPartsLocatorSet::MapPartsLocatorSet(atn::Task* parent, void* arg, atn::CsvTab* csv)
    : MapPartsBase(parent, arg), mHideY(0.0f), mTexName(""), mFlags(9), mModel(9), mLoaded(0), mLinked(0),
      mHide(0), mDraw(1), mHasModel(0), mHasMerged(0) {
    const char* path = csv->fn_8005B9CC();
    mLoader.vf0C(path);
    mName = mLoader.fn_80473F18();
    if (lbl_8087F0A8->m5B0 != 0) {
        char name[0x100];
        sprintf(name, "merge_%s_occ.model", fn_8006B2D8(fn_8006B174(std::string(path))).c_str());
        if (fn_8006AA20(name)) {
            mHasModel = 1;
            fn_8008AD4C(mModel.GetModel_unk(), name, 0);
        }
    }
    if (lbl_8087F0A8->m5B4 != 0) {
        char name[0x100];
        sprintf(name, "merge_%s_loc.model", fn_8006B2D8(fn_8006B174(std::string(path))).c_str());
        if (fn_8006AA20(name)) {
            mHasMerged = 1;
            mMerged.reset(new (12, "", "", 0) EffectModel_unk(9));
            fn_8008AD4C(mMerged->GetModel_unk(), name, 0);
        }
    }
    const char* field = csv->fn_8005B9CC();
    do {
        if (strcmp(field, "HIDE_BIRDVIEW") == 0) {
            mHide = 1;
            mHideY = 1e16f;
        } else if (strcmp(field, "HIDE_BIRDVIEW_Y") == 0) {
            mHide = 1;
            mHideY = fn_800DC288(csv->fn_8005B9CC());
        } else if (strcmp(field, "LMAP") == 0) {
            mTexName = csv->fn_8005B9CC();
        } else if (strcmp(field, "REFLECT") == 0) {
            mFlags |= 0x400;
        } else if (strcmp(field, "REFRACT") == 0) {
            mFlags |= 0x800;
        } else if (strcmp(field, "FORCE_REFLECT") == 0) {
            mFlags |= 0x1400;
        } else if (strcmp(field, "FORCE_REFRACT") == 0) {
            mFlags |= 0x1800;
        } else if (strcmp(field, "NO_SHADOW") == 0) {
            mFlags &= ~0x8;
        } else if (strcmp(field, "SHADOW_RECEIVER") == 0) {
            mFlags |= 0x8;
        } else if (strcmp(field, "NO_CLOUD_MAP") == 0) {
            mFlags |= 0x200000;
        } else if (strcmp(field, "CLOUD_MAP") == 0) {
            mFlags |= 0x100000;
        } else if (strcmp(field, "PROJECTED_SHADOW") == 0) {
            mFlags |= 0x8000;
        }
    } while ((field = csv->fn_8005B9CC()) != 0 && strcmp("", field) != 0);
    if (!mHide && strstr(mName.mStr, "_hide.") != 0) {
        mHide = 1;
        mHideY = 1e16f;
    }
}

MapPartsLocatorSet::~MapPartsLocatorSet() {
    for (unsigned int i = 0; i < (unsigned int)mInstances.mSize; i++) {
        delete mInstances.mData[i];
    }
}

int MapPartsLocatorSet::vf0C() {
    if (mLoader.fn_80473F50()) {
        return 0;
    }
    if (!mLoaded) {
        mLoaded = 1;
        fn_8049B1F0();
        mLoader.fn_80473F88();
    }
    // inferred: the texture is seen through the declarations of GimmickUvOwner_unk
    if (((GimmickUvOwner_unk*)&mTexture)->fn_800D59B8()) {
        return 0;
    }
    if (!mHasMerged && !mLinked) {
        mLinked = 1;
        int ok = 0;
        if (mTexture.mImage != 0 ||
            (!((GimmickUvOwner_unk*)&mTexture)->fn_800D59B8() && mTexture.m20.fn_80473EFC() == 3)) {
            ok = 1;
        }
        if (ok) {
            for (unsigned int i = 0; i < (unsigned int)mInstances.mSize; i++) {
                mInstances.mData[i]->mInst.m7C = &mTexture;
            }
        }
    }
    int ready = 1;
    if (mHasMerged) {
        int loaded = mMerged->GetModel_unk()->fn_8008B140() == 0;
        ready = 0;
        if (mHasMerged && loaded) {
            ready = 1;
        }
    } else {
        for (unsigned int i = 0; i < (unsigned int)mInstances.mSize; i++) {
            ready = ready && mInstances.mData[i]->fn_80490DE8();
        }
    }
    if (mHasModel) {
        ready = ready && mModel.GetModel_unk()->fn_8008B140() == 0;
    }
    if (ready && fn_800D3FA4()) {
        mBox = vf24(0);
        return 1;
    }
    return 0;
}

void MapPartsLocatorSet::vf10() {
    if (mDraw) {
        if (mHasMerged) {
            mMerged->GetModel_unk()->fn_80092A4C();
            fn_800902C0(mMerged->GetModel_unk(), 0, boost::function<void()>());
        } else {
            for (unsigned int i = 0; i < (unsigned int)mInstances.mSize; i++) {
                mInstances.mData[i]->fn_80490E38();
            }
        }
    }
}

void MapPartsLocatorSet::vf14() {
    if (mDraw) {
        int shadow = 1;
        if (mHasModel && lbl_8087F0A8->m5B0 != 0) {
            shadow = 0;
            mModel.GetModel_unk()->fn_8008CD60();
        }
        if (mHasMerged) {
            mMerged->GetModel_unk()->fn_8008CD60();
            return;
        }
        int visible = 1;
        if (lbl_8087FA20 != 0 && (lbl_8087FA20->m38 & 4) != 4 && !fn_800502A8(lbl_8087EFB4->m204, &mBox)) {
            visible = 0;
        }
        if (visible && lbl_8087F0A8->mDrawMap != 0) {
            for (unsigned int i = 0; i < (unsigned int)mInstances.mSize; i++) {
                mInstances.mData[i]->fn_80490EA0(shadow);
            }
        }
    }
}

// NONMATCHING: 87.9% - the original reads the box of each instance twice (two copies of min and max,
// z through frsp) and lays out the corner temporaries differently
MapPartsBox_unk MapPartsLocatorSet::vf24(MapPartsTree_unk* tree) {
    MapPartsBox_unk box = MapPartsBase::vf24(tree);
    for (unsigned int i = 0; i < (unsigned int)mInstances.mSize; i++) {
        LocatorInstRes_unk* res = ((LocatorInstData_unk*)mInstances.mData[i]->mInst.mData)->m17C;
        if (res != 0) {
            atn::AABB_unk local(res->mMin, res->mMax);
            *(atn::AABB_unk*)&box = ((atn::AABB_unk*)&box)->Merge_unk(TransformBox_unk(mInstances.mData[i]->mInst.mMtx.m, &local));
        }
    }
    if (mHasMerged) {
        atn::AABB_unk merged = fn_80095300(mMerged->GetModel_unk());
        *(atn::AABB_unk*)&box = ((atn::AABB_unk*)&box)->Merge_unk(merged);
    }
    return box;
}

// NONMATCHING: 97.3% - the original computes i * 4 for the instance slot in each iteration (here it
// is strength reduced into one more saved register)
void MapPartsLocatorSet::fn_8049B1F0() {
    if (mLoader.fn_80473EFC() != 3) {
        return;
    }
    Loader_unk loader;
    fn_804714A4(&loader);
    fn_804714B0(&loader, mLoader.fn_80470580(), (void*)mLoader.fn_8047059C(), LoaderCallback_unk(), 0);
    BuildingFile_unk* file = (BuildingFile_unk*)mLoader.fn_80470580();
    std::string dir = fn_8006B0C8(std::string(mLoader.fn_80473F18()));
    if (mTexName.Equals_unk("") && strcmp(file->mTexture, "") != 0) {
        std::string tex(dir);
        tex += "/textures/";
        tex += file->mTexture;
        mTexName = tex.c_str();
    }
    if (!mTexName.Equals_unk("")) {
        fn_800D5908(&mTexture, mTexName.mStr);
    }
    if (mHasMerged) {
        return;
    }
    mInstances.Resize_unk(file->mCount, 6);
    for (int i = 0; i < file->mCount; i++) {
        MapLocator_unk* loc;
        if (file->mKind == 'locs') {
            loc = (MapLocator_unk*)(file->mLocators + i * 0x3C);
        } else {
            loc = (MapLocator_unk*)(file->mLocators + i * 0x30);
        }
        char path[0x100];
        sprintf(path, "data/building/%s.building", loc->mLod->mName);
        if (file->mKind == 'locs') {
            MapLocatorShear_unk* shear = (MapLocatorShear_unk*)(file->mLocators + i * 0x3C);
            mInstances[i] = new (12, "", "", 0) MapInstance_unk(path, shear);
        } else {
            mInstances[i] = new (12, "", "", 0) MapInstance_unk(path, loc);
        }
        mInstances[i]->mInst.m6C = mFlags;
    }
}

void MapPartsLocatorSet::fn_8049B72C(unsigned int flags) {
    if (mHasMerged) {
        mMerged->GetModel_unk()->mFlags |= flags;
    }
    for (unsigned int i = 0; i < (unsigned int)mInstances.mSize; i++) {
        mInstances.mData[i]->mInst.m6C = flags;
    }
    mFlags = flags;
}

void MapPartsLocatorSet::fn_8049B780(void* lens) {
    if (mHasMerged) {
        mMerged->GetModel_unk()->m1DC = (Lens_unk*)lens;
        return;
    }
    for (unsigned int i = 0; i < (unsigned int)mInstances.mSize; i++) {
        mInstances.mData[i]->mInst.m64 = lens;
    }
}

void MapPartsLocatorSet::fn_8049B7CC(void* tree) {
    for (unsigned int i = 0; i < (unsigned int)mInstances.mSize; i++) {
        const atn::Sphere& bound = mInstances.mData[i]->mInst.mBound;
        atn::Sphere sphere;
        sphere.mCenter = bound.mCenter;
        sphere.mRadius = bound.mRadius;
        void* light = fn_804962B0(tree, &sphere);
        if (light != 0) {
            mInstances.mData[i]->mInst.m64 = (char*)light + 0x4C;
        } else {
            mInstances.mData[i]->mInst.m64 = fn_800C16B4(lbl_8087EFB4);
        }
    }
}

// NONMATCHING: 99.45% - registers of the name pointer and its length (r29/r30)
void MapPartsLocatorSet::vf2C(unsigned int hash, int draw) {
    std::string name = fn_8006B174(std::string(mName.c_str()));
    if (hash == fn_800DC6B4(name.c_str()) || hash == fn_800DC6B4("<all>")) {
        mDraw = draw;
    }
}

bool MapPartsLocatorSet::vf20(const atn::Vector3& pos) {
    bool hidden = false;
    if (mHide && mHideY > 10.0f + pos.y) {
        hidden = true;
    }
    return hidden;
}
