#include "game/MapPartsModel.h"
#include "atn/Memory.h"
#include "std/string.h"
#include "std/string_members.h"
#include <boost/function.hpp>

extern "C" int strcmp(const char* a, const char* b);
extern "C" char* strstr(const char* s, const char* sub);
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
extern "C" std::string fn_8006B174(const std::string& path); // inferred: file name of a path
extern "C" void PSMTXTrans(Mtx m, float x, float y, float z);
extern "C" void PSMTXScale(Mtx m, float x, float y, float z);
extern "C" int fn_800709F4(const MapPartsBox_unk* box, const atn::Vector3* pos); // inferred: pos inside box

// inferred: inserts a box into the map tree (atn/AABBTree.cpp)
extern "C" Model_unk::TreeEntry_unk fn_80045A24(MapPartsTree_unk* tree, const MapPartsBox_unk& box);
extern "C" void fn_8008AD4C(Model_unk* model, const char* path, const char* texture); // inferred: loads a model

// inferred: camera (lbl_8087EFB4)
struct Camera_unk {
    unsigned char m00[0x118];
    atn::Vector3 mPos; // 0x118
};
extern "C" Camera_unk* lbl_8087EFB4;
// inferred: game settings
struct Settings_unk {
    unsigned char m00[0x18];
    int mDrawMap; // 0x18
};
extern "C" Settings_unk* lbl_8087F0A8;

#define __MAX_unk(a, b) ((a) > (b) ? (a) : (b))

inline void __Vec3Set_unk(atn::Vector3& v, float x, float y, float z) {
    v.x = x;
    v.y = y;
    v.z = z;
}

MapPartsModel* MapPartsModel::fn_8049B9B8(atn::Task* parent, void* arg, atn::CsvTab* csv) {
    if (parent != 0) {
        return new (12, "", "", 0) MapPartsModel(parent, arg, csv);
    }
    return 0;
}

MapPartsModel::MapPartsModel(atn::Task* parent, void* arg, atn::CsvTab* csv)
    : MapPartsBase(parent, arg), mModel(9, 0x20) {
    mHide = 0;
    mCenterCamera = 0;
    mHasBox = 0;
    mDraw = 1;
    fn_8008AD4C(&mModel, csv->fn_8005B9CC(), 0);
    vf28(csv);
}

// inferred: rotation matrix z * y * x from euler angles (name invented; the same shape as
// __MakeRotationYXZ_unk in game/CharaTask.cpp)
inline atn::Matrix3x4& __MakeRotationZYX_unk(atn::Matrix3x4& m, const atn::Vector3& r) {
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
    return m;
}

// inferred: m = m * rotation(r) (name invented)
inline void __MtxRotateZYX_unk(atn::Matrix3x4& m, const atn::Vector3& r) {
    Mtx t;
    atn::Matrix3x4 rot;
    PSMTXConcat(m.m, __MakeRotationZYX_unk(rot, r).m, t);
    __MtxCopy(m.m, t);
}

// inferred: scale matrix and matrix product by value (names invented)
inline atn::Matrix3x4 __ScaleMtx_unk(const atn::Vector3& s) {
    atn::Matrix3x4 r;
    PSMTXScale(r.m, s.x, s.y, s.z);
    return r;
}

inline atn::Matrix3x4 __MtxMulV_unk(const atn::Matrix3x4& a, const atn::Matrix3x4& b) {
    atn::Matrix3x4 r;
    PSMTXConcat(a.m, b.m, r.m);
    return r;
}

// inferred: m = m * scale(s) (name invented)
inline void __MtxScale_unk(atn::Matrix3x4& m, const atn::Vector3& s) {
    m = __MtxMulV_unk(m, __ScaleMtx_unk(s));
}

void MapPartsModel::vf28(atn::CsvTab* csv) {
    atn::Vector3 pos(0.0f, 0.0f, 0.0f);
    atn::Vector3 rot(0.0f, 0.0f, 0.0f);
    atn::Vector3 scale(1.0f, 1.0f, 1.0f);
    const char* field = csv->fn_8005B9CC();
    do {
        if (strcmp(field, "") == 0) {
            continue;
        } else if (strcmp(field, "INVERT") == 0) {
            mModel.mFlags |= 0x80;
        } else if (strcmp(field, "OCCLUSION") == 0) {
            mModel.mFlags |= 0x2;
        } else if (strcmp(field, "NO_SHADOW") == 0) {
            mModel.mFlags &= ~0x8;
        } else if (strcmp(field, "SHADOW_RECEIVER") == 0) {
            mModel.mFlags |= 0x8;
        } else if (strcmp(field, "NO_CLOUD_MAP") == 0) {
            mModel.mFlags |= 0x200000;
        } else if (strcmp(field, "CLOUD_MAP") == 0) {
            mModel.mFlags |= 0x100000;
        } else if (strcmp(field, "HIDE_BIRDVIEW") == 0) {
            mHide = 1;
            mHideY = 1e16f;
        } else if (strcmp(field, "HIDE_BIRDVIEW_Y") == 0) {
            mHide = 1;
            mHideY = fn_800DC288(csv->fn_8005B9CC());
        } else if (strcmp(field, "CENTER_CAMERA") == 0) {
            mCenterCamera = 1;
            mModel.m98 = -1000.0f;
        } else if (strcmp(field, "REFLECT") == 0) {
            mModel.mFlags |= 0x400;
        } else if (strcmp(field, "REFRACT") == 0) {
            mModel.mFlags |= 0x800;
        } else if (strcmp(field, "FORCE_REFLECT") == 0) {
            mModel.mFlags |= 0x1400;
        } else if (strcmp(field, "FORCE_REFRACT") == 0) {
            mModel.mFlags |= 0x1800;
        } else if (strcmp(field, "PROJECTED_SHADOW") == 0) {
            mModel.mFlags |= 0x8000;
        } else if (strcmp(field, "OFF_RENDER_3D_OBJECT") == 0) {
            mModel.mFlags |= 0x1000000;
        } else if (strcmp(field, "POSITION") == 0) {
            pos.x = fn_800DC288(csv->fn_8005B9CC());
            pos.y = fn_800DC288(csv->fn_8005B9CC());
            pos.z = fn_800DC288(csv->fn_8005B9CC());
        } else if (strcmp(field, "ROTATION") == 0) {
            rot.x = fn_800DC288(csv->fn_8005B9CC());
            rot.y = fn_800DC288(csv->fn_8005B9CC());
            rot.z = fn_800DC288(csv->fn_8005B9CC());
        } else if (strcmp(field, "SCALE") == 0) {
            scale.x = fn_800DC288(csv->fn_8005B9CC());
            scale.y = fn_800DC288(csv->fn_8005B9CC());
            scale.z = fn_800DC288(csv->fn_8005B9CC());
        } else if (strcmp(field, "PRIORITY") == 0) {
            mModel.m98 = fn_800DC288(csv->fn_8005B9CC());
        } else if (strcmp(field, "BOX") == 0) {
            MapPartsBox_unk box;
            box.mMin.x = fn_800DC288(csv->fn_8005B9CC());
            box.mMin.y = fn_800DC288(csv->fn_8005B9CC());
            box.mMin.z = fn_800DC288(csv->fn_8005B9CC());
            box.mMax.x = fn_800DC288(csv->fn_8005B9CC());
            box.mMax.y = fn_800DC288(csv->fn_8005B9CC());
            box.mMax.z = fn_800DC288(csv->fn_8005B9CC());
            mHasBox = 1;
            mBox = box;
        } else {
            break;
        }
    } while ((field = csv->fn_8005B9CC()) != 0 && strcmp("", field) != 0);
    if (mHide == 0 && strstr(mModel.fn_8008B130(), "_hide.") != 0) {
        mHide = 1;
        mHideY = 1e16f;
    }
    atn::Matrix3x4 mtx;
    PSMTXTrans(mtx.m, pos.x, pos.y, pos.z);
    __MtxRotateZYX_unk(mtx, rot);
    __MtxScale_unk(mtx, scale);
    {
        __MtxCopy(mModel.mMtx, mtx.m);
        atn::Vector3 ax;
        atn::Vector3 ay;
        atn::Vector3 az;
        __Vec3Set_unk(az, mtx.m[0][2], mtx.m[1][2], mtx.m[2][2]);
        float sz = PSVECMag(&az);
        __Vec3Set_unk(ay, mtx.m[0][1], mtx.m[1][1], mtx.m[2][1]);
        float sy = PSVECMag(&ay);
        __Vec3Set_unk(ax, mtx.m[0][0], mtx.m[1][0], mtx.m[2][0]);
        float sx = PSVECMag(&ax);
        atn::Vector3 s(sx, sy, sz);
        mModel.m54 = __MAX_unk(s.x, __MAX_unk(s.y, s.z));
        fn_8000D430(&mModel, boost::function<void()>());
}
}

MapPartsModel::~MapPartsModel() {}

int MapPartsModel::vf0C() {
    if (mModel.fn_8008B140() == 0) {
        fn_8000D430(&mModel, boost::function<void()>());
        return 1;
    }
    return 0;
}

void MapPartsModel::vf10() {}

void MapPartsModel::vf14() {
    if (mHasBox != 0) {
        atn::Sphere cam;
        cam.mCenter = lbl_8087EFB4->mPos;
        cam.mRadius = 1.0f;
        if (fn_800709F4(&mBox, &cam.mCenter) == 0) {
            return;
        }
    }
    if (mCenterCamera != 0) {
        atn::Matrix3x4 mtx(*(atn::Matrix3x4*)mModel.mMtx);
        const atn::Vector3& p = lbl_8087EFB4->mPos;
        mtx.m[0][3] = p.x;
        mtx.m[1][3] = p.y;
        mtx.m[2][3] = p.z;
        {
            __MtxCopy(mModel.mMtx, mtx.m);
            atn::Vector3 ax;
            atn::Vector3 ay;
            atn::Vector3 az;
            __Vec3Set_unk(az, mtx.m[0][2], mtx.m[1][2], mtx.m[2][2]);
            float sz = PSVECMag(&az);
            __Vec3Set_unk(ay, mtx.m[0][1], mtx.m[1][1], mtx.m[2][1]);
            float sy = PSVECMag(&ay);
            __Vec3Set_unk(ax, mtx.m[0][0], mtx.m[1][0], mtx.m[2][0]);
            float sx = PSVECMag(&ax);
            atn::Vector3 s(sx, sy, sz);
            mModel.m54 = __MAX_unk(s.x, __MAX_unk(s.y, s.z));
            fn_8000D430(&mModel, boost::function<void()>());
}
    }
    if (lbl_8087F0A8->mDrawMap != 0 && mDraw != 0) {
        mModel.fn_8008CD60();
    }
}

MapPartsBox_unk MapPartsModel::vf24(MapPartsTree_unk* tree) {
    MapPartsBox_unk box;
    Model_unk::Res_unk* res = mModel.m16C;
    if (res != 0) {
        box.mMin = atn::Vector3(res->mMin);
        box.mMax = atn::Vector3(res->mMax);
        if (tree != 0) {
            Model_unk::TreeEntry_unk e = Model_unk::TreeEntry_unk(fn_80045A24(tree, box));
            mModel.m200 = Model_unk::TreeEntry_unk(e);
        }
    } else {
        return MapPartsBase::vf24(tree);
    }
    return box;
}

void MapPartsModel::vf2C(unsigned int hash, int draw) {
    std::string name = fn_8006B174(std::string(mModel.fn_8008B130()));
    if (hash == fn_800DC6B4(name.data()) || hash == fn_800DC6B4("<all>")) {
        mDraw = draw;
    }
}

bool MapPartsModel::vf20(const atn::Vector3& pos) {
    return mHide != 0 && mHideY > pos.y + 10.0f;
}
