#include "atn/Task.h"
#include "atn/Memory.h"
#include "atn/FixedArray.h"
#include "math/Vec.h"
#include "std/vector.h"
#include <boost/function.hpp>

namespace atn {

// The names are in the DOL: the type_info of the camera callback is
// "atn::Scene * (*)(atn::Sphere, void *)" (0x80756690).
class Scene;

// inferred: bounding sphere (center and radius)
struct Sphere {
    // inferred: the center is copied with z loaded first (0x80491D50)
    Sphere(const Sphere& o) {
        mCenter.Assign_unk(o.mCenter);
        mRadius = o.mRadius;
    }
    Vector3 mCenter;
    float mRadius;
};

} // namespace atn

// inferred: color returned by the material of a model (fn_80043014)
struct MapColor_unk {
    MapColor_unk() {}
    MapColor_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    MapColor_unk(const MapColor_unk& c) : r(c.r), g(c.g), b(c.b), a(c.a) {}
    float r, g, b, a;
};
extern "C" MapColor_unk* fn_80043014(void* material, int type, int arg);

// inferred: segment of a model (0x18 bytes)
struct MapSegment_unk {
    atn::Vector3 mStart;
    atn::Vector3 mEnd;
};

// inferred: model of a map part; atn::Scene is its base at 0x4C (fn_804914BC)
struct MapModelBase_unk {
    unsigned char m00[0x28];
    MapSegment_unk* mSegments;  // 0x28
    unsigned int mSegmentNum;   // 0x2C
    unsigned char m30[0x4C - 0x30];
};
struct MapModel_unk : public MapModelBase_unk {
    unsigned char m4C[0x300 - 0x4C];
    void* mMaterial; // 0x300
};
// inferred: the atn::Scene part of a model (null-checked cast, 0x80491504)
inline atn::Scene* ToScene_unk(MapModel_unk* model) {
    return model != 0 ? (atn::Scene*)((char*)model + 0x4C) : 0;
}

// inferred: group of models of a map part
struct MapGroup_unk {
    unsigned char m00[0x58];
    std::vector<MapModel_unk*> mModels; // 0x58
};
extern "C" MapModel_unk* fn_804962B0(MapGroup_unk* group, const atn::Sphere* sphere); // inferred: model hit by the sphere

// inferred: map part (MapPartsBase)
struct MapPart_unk {
    // inferred: names invented
    bool IsValidIndex_unk(int i) const { return i >= 0 && i < mGroupNum; }
    MapGroup_unk* GetGroup_unk() {
        if (mGroupC4 != 0 && m108 != m10C) {
            return mGroupC4;
        }
        return IsValidIndex_unk(m108) ? mGroups[m108] : 0;
    }

    unsigned char m00[0x48];
    int mType;                   // 0x48
    unsigned char m4C[0x88 - 0x4C];
    unsigned char mBound[0x18];  // 0x88: inferred: bound tested by fn_800709F4
    int mGroupNum;               // 0xA0
    MapGroup_unk* mGroups[8];    // 0xA4
    MapGroup_unk* mGroupC4;      // 0xC4
    unsigned char mC8[0xF8 - 0xC8];
    int mF8;                     // 0xF8: inferred: nonzero when hidden
    unsigned char mFC[0x108 - 0xFC];
    int m108;
    int m10C;
};
extern "C" int fn_800709F4(void* bound, const atn::Sphere* sphere); // inferred: intersection test
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" int lbl_8087F610; // inferred: debug flag
// inferred: field data (lbl_8087F430) and its current map part
struct Field_unk;
extern "C" Field_unk* lbl_8087F430;
extern "C" MapPart_unk* fn_80373148(Field_unk* field);

// inferred: camera callback: the map model hit by a sphere
typedef boost::function<atn::Scene*(atn::Sphere, void*)> MapHitFunc_unk;
typedef boost::function<void(MapModel_unk*, void*)> MapModelFunc_unk;

// inferred: camera (lbl_8087EFB4); m300 is a callback with its argument at 0x314
struct Camera_unk {
    // inferred: weak copy at 0x8049103C (takes the function by value, so it is never inlined);
    // name invented
    void SetMapHit_unk(MapHitFunc_unk func, void* arg) {
        mMapHit = func;
        mMapHitArg = arg;
    }

    unsigned char m000[0x300];
    MapHitFunc_unk mMapHit; // 0x300
    void* mMapHitArg;       // 0x314
};
extern "C" Camera_unk* lbl_8087EFB4;

static atn::Scene* fn_804914BC(atn::Sphere sphere, void* manager);

// inferred: list of the model groups; a class derived from the vector (the destructor of
// 0x80491324 tests the address four times)
struct MapGroupList_unk : public std::vector<MapGroup_unk*> {};

// inferred: squared length of a - b (name invented)
inline float DistSq_unk(const atn::Vector3& a, const atn::Vector3& b) {
    atn::Vector3 d = a - b;
    return fn_805F9920(&d);
}

// inferred: removes element i of a fixed array (name invented)
template <class T, int N>
inline void Erase_unk(atn::FixedArray_unk<T, N>& a, unsigned int i) {
    for (; i < a.mSize - 1; i++) {
        a.mData[i] = a.mData[i + 1];
    }
    a.mSize = a.mSize - 1;
}

// inferred from __vt__10MapManager (0x807901E0): manager of the map parts (singleton)
class MapManager : public atn::Task {
public:
    MapManager(atn::Task* parent);
    static MapManager* fn_80490EB8(atn::Task* parent);
    virtual ~MapManager();
    virtual int vf0C();
    void fn_80491418(MapPart_unk* part);
    void fn_80491440(MapPart_unk* part);
    void fn_80491528(MapModelFunc_unk func, void* arg);
    MapModel_unk* fn_804918A4(atn::Sphere sphere);
    MapColor_unk fn_80491D50(atn::Sphere sphere);
    MapColor_unk fn_80491DF8(atn::Sphere sphere);
    // inferred: name invented (the callback copies the sphere twice, 0x804914BC)
    MapModel_unk* Find_unk(atn::Sphere sphere) { return fn_804918A4(sphere); }

    static MapManager* sInstance; // 0x8087F558

    atn::FixedArray_unk<MapPart_unk*, 64> mParts; // 0x48
    int mType;                                    // 0x14C: inferred: only parts of this type (0: all)
    MapGroupList_unk mGroups;                     // 0x150
    int m15C;
};

MapManager* MapManager::sInstance;

MapManager* MapManager::fn_80490EB8(atn::Task* parent) {
    if (parent != 0 && sInstance == 0) {
        sInstance = new (12, "", "", 0) MapManager(parent);
    }
    return sInstance;
}

// inferred: not inline; inlined into fn_80490EB8
MapManager::MapManager(atn::Task* parent) : atn::Task(parent), mType(0) {
    lbl_8087EFB4->SetMapHit_unk(&fn_804914BC, this);
}

MapManager::~MapManager() {
    sInstance = 0;
    lbl_8087EFB4->SetMapHit_unk(MapHitFunc_unk(), 0);
}

int MapManager::vf0C() {
    return fn_800D3FA4();
}

// inferred: adds a map part
void MapManager::fn_80491418(MapPart_unk* part) {
    mParts.push_back(part);
}

// inferred: removes a map part
void MapManager::fn_80491440(MapPart_unk* part) {
    MapPart_unk** end = &mParts.mData[mParts.mSize];
    MapPart_unk** it = mParts.mData;
    while (it != end && *it != part) {
        it++;
    }
    if (it == end) {
        return;
    }
    Erase_unk(mParts, it - mParts.mData);
}

// inferred: the camera callback (the argument is the manager)
static atn::Scene* fn_804914BC(atn::Sphere sphere, void* manager) {
    return ToScene_unk(((MapManager*)manager)->Find_unk(sphere));
}

// inferred: calls `func` on the models of the parts and of the groups
void MapManager::fn_80491528(MapModelFunc_unk func, void* arg) {
    MapModel_unk** model;
    for (MapPart_unk** it = mParts.mData; it != &mParts.mData[mParts.mSize]; it++) {
        MapPart_unk* part = *it;
        if (mType == 0 && part->mType == 6) {
            continue;
        }
        if (mType != 0 && mType != part->mType) {
            continue;
        }
        if (part->mF8 != 0) {
            continue;
        }
        if (lbl_8087F610 != 0 && part->mType == 0) {
            continue;
        }
        MapGroup_unk* group = part->GetGroup_unk();
        if (group != 0) {
            for (model = group->mModels.begin(); model != group->mModels.end(); model++) {
                func(*model, arg);
            }
        }
    }
    for (unsigned int i = 0; i < mGroups.size(); i++) {
        MapGroup_unk* group = mGroups[i];
        if (group != 0) {
            for (model = group->mModels.begin(); model != group->mModels.end(); model++) {
                func(*model, arg);
            }
        }
    }
}

// inferred: model hit by a sphere
// NONMATCHING: 97.6% - register allocation of the loops, and the original reloads m108 of the
// field part inside GetGroup_unk
MapModel_unk* MapManager::fn_804918A4(atn::Sphere sphere) {
    if (mType != 0) {
        MapPart_unk** end = &mParts.mData[mParts.mSize];
        for (MapPart_unk** it = mParts.mData; it != end; it++) {
            MapPart_unk* part = *it;
            if (mType == part->mType) {
                MapGroup_unk* group = part->GetGroup_unk();
                if (group != 0) {
                    return group->mModels[0];
                }
            }
        }
    } else {
        for (MapPart_unk** it = mParts.mData; it != &mParts.mData[mParts.mSize]; it++) {
            MapPart_unk* part = *it;
            if (part->mType == 6) {
                continue;
            }
            if (part->mF8 != 0) {
                continue;
            }
            if (lbl_8087F610 != 0 && part->mType == 0) {
                continue;
            }
            if (fn_800709F4(part->mBound, &sphere) == 0) {
                continue;
            }
            MapGroup_unk* group = part->GetGroup_unk();
            if (group != 0) {
                MapModel_unk* model = fn_804962B0(group, &sphere);
                if (model != 0) {
                    return model;
                }
            }
        }
        if (lbl_8087F430 != 0) {
            MapPart_unk* part = fn_80373148(lbl_8087F430);
            if (part != 0 && part->m108 != 0) {
                MapGroup_unk* group = part->GetGroup_unk();
                if (group != 0 && group->mModels.size() != 0 && part->mF8 == 0) {
                    return group->mModels[0];
                }
                MapPart_unk** end = &mParts.mData[mParts.mSize];
                for (MapPart_unk** it = mParts.mData; it != end; it++) {
                    MapPart_unk* p = *it;
                    if (p->mType == 6) {
                        continue;
                    }
                    if (p->mF8 != 0) {
                        continue;
                    }
                    MapGroup_unk* g = p->GetGroup_unk();
                    if (g != 0 && g->mModels.size() != 0) {
                        return g->mModels[0];
                    }
                }
            }
        }
    }
    for (unsigned int i = 0; i < mGroups.size(); i++) {
        MapGroup_unk* group = mGroups[i];
        if (group != 0) {
            MapModel_unk* model = fn_804962B0(group, &sphere);
            if (model != 0) {
                return model;
            }
        }
    }
    float radiusSq = sphere.mRadius * sphere.mRadius;
    for (unsigned int i = 0; i < mGroups.size(); i++) {
        MapGroup_unk* group = mGroups[i];
        if (group != 0) {
            for (MapModel_unk** model = group->mModels.begin(); model != group->mModels.end(); model++) {
                for (unsigned int j = 0; j < (*model)->mSegmentNum; j++) {
                    MapSegment_unk* seg = &(*model)->mSegments[j];
                    float d0 = DistSq_unk(seg->mStart, sphere.mCenter);
                    float d1 = DistSq_unk(seg->mEnd, sphere.mCenter);
                    if (d0 < radiusSq && d1 < radiusSq) {
                        return *model;
                    }
                }
            }
        }
    }
    return 0;
}

// inferred: color 2 of the material of the model hit by a sphere
// NONMATCHING: 71% - scheduling: the original loads the sphere before saving r31 (the implicit copy
// of the sphere gives 78%, but the callback 0x804914BC needs the copy with z first)
MapColor_unk MapManager::fn_80491D50(atn::Sphere sphere) {
    MapModel_unk* model = fn_804918A4(sphere);
    if (model != 0 && model->mMaterial != 0) {
        return *fn_80043014(model->mMaterial, 2, 0);
    }
    return MapColor_unk(0.0f, 0.0f, 0.0f, 0.0f);
}

// inferred: color 3 of the material of the model hit by a sphere
// NONMATCHING: 71% - as fn_80491D50
MapColor_unk MapManager::fn_80491DF8(atn::Sphere sphere) {
    MapModel_unk* model = fn_804918A4(sphere);
    if (model != 0 && model->mMaterial != 0) {
        return *fn_80043014(model->mMaterial, 3, 0);
    }
    return MapColor_unk(0.0f, 0.0f, 0.0f, 0.0f);
}
