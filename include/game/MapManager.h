#ifndef GAME_MAPMANAGER_H
#define GAME_MAPMANAGER_H

#include "atn/Task.h"
#include "atn/FixedArray.h"
#include "atn/Sphere.h"
#include "std/vector.h"
#include <boost/function.hpp>

namespace atn {
class Scene; // the name is in the DOL (type_info 0x80756690)
}

// inferred: color returned by the material of a model (AreaEffConfig::fn_80043014)
struct MapColor_unk {
    MapColor_unk() {}
    MapColor_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    MapColor_unk(const MapColor_unk& c) : r(c.r), g(c.g), b(c.b), a(c.a) {}
    float r, g, b, a;
};
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
    unsigned char m4C[0xBC - 0x4C];
    int mBC;   // inferred: set by gmk::CharaLight (callback 0x804225E0)
    void* mC0; // inferred: light of gmk::CharaLight
    unsigned char mC4[0x114 - 0xC4];
    int m114;
    unsigned char m118[0x300 - 0x118];
    void* mMaterial; // 0x300
};
// inferred: group of models of a map part
struct MapGroup_unk {
    unsigned char m00[0x58];
    std::vector<MapModel_unk*> mModels; // 0x58
};
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
// inferred: camera callback: the map model hit by a sphere
typedef boost::function<atn::Scene*(atn::Sphere, void*)> MapHitFunc_unk;
typedef boost::function<void(MapModel_unk*, void*)> MapModelFunc_unk;

// inferred: list of the model groups; a class derived from the vector (the destructor of
// 0x80491324 tests the address four times)
struct MapGroupList_unk : public std::vector<MapGroup_unk*> {};

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

#endif
