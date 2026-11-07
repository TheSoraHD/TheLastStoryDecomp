#include "game/MapManager.h"
#include "game/GameTask.h"
#include "game/MapPartsArea.h"
#include "game/MapTask.h"
#include "atn/Memory.h"
#include "game/AreaEffConfig.h"
#include "atn/RenderManager.h"



// inferred: the scene of an area (null-checked, 0x80491504)
inline atn::Scene* ToScene_unk(MapArea* area) {
    return area != 0 ? &area->mScene : 0;
}

extern "C" int fn_800709F4(void* bound, const atn::Sphere* sphere); // inferred: intersection test
extern "C" float PSVECSquareMag(const atn::Vector3* v); // inferred: squared length
extern "C" int lbl_8087F610; // inferred: debug flag
// inferred: field data (lbl_8087F430) and its current map part
struct Field_unk;
extern "C" Field_unk* lbl_8087F430;

// inferred: weak copy at 0x8049103C (takes the function by value, so it is never inlined)
inline void atn::RenderManager::SetMapHit_unk(MapHitFunc_unk func, void* arg) {
    mMapHit = func;
    mMapHitArg = arg;
}

static atn::Scene* fn_804914BC(atn::Sphere sphere, void* manager);

// inferred: squared length of a - b (name invented)
inline float DistSq_unk(const atn::Vector3& a, const atn::Vector3& b) {
    atn::Vector3 d = a - b;
    return PSVECSquareMag(&d);
}

// inferred: removes element i of a fixed array (name invented)
template <class T, int N>
inline void Erase_unk(atn::FixedArray_unk<T, N>& a, unsigned int i) {
    for (; i < a.mSize - 1; i++) {
        a.mData[i] = a.mData[i + 1];
    }
    a.mSize = a.mSize - 1;
}


MapManager* MapManager::instance_;

MapManager* MapManager::SetTask(atn::Task* parent) {
    if (parent != 0 && instance_ == 0) {
        instance_ = new ((MemoryTag)12, "", "", 0) MapManager(parent);
    }
    return instance_;
}

// inferred: not inline; inlined into SetTask
MapManager::MapManager(atn::Task* parent) : atn::Task(parent), mType(0) {
    atn::RenderManager::instance_->SetMapHit_unk(&fn_804914BC, this);
}

MapManager::~MapManager() {
    instance_ = 0;
    atn::RenderManager::instance_->SetMapHit_unk(MapHitFunc_unk(), 0);
}

int MapManager::vf0C() {
    return fn_800D3FA4();
}

// inferred: adds a map
void MapManager::fn_80491418(MapTask* map) {
    mMaps.push_back(map);
}

// inferred: removes a map
void MapManager::fn_80491440(MapTask* map) {
    MapTask** end = &mMaps.mData[mMaps.mSize];
    MapTask** it = mMaps.mData;
    while (it != end && *it != map) {
        it++;
    }
    if (it == end) {
        return;
    }
    Erase_unk(mMaps, it - mMaps.mData);
}

// inferred: the camera callback (the argument is the manager)
static atn::Scene* fn_804914BC(atn::Sphere sphere, void* manager) {
    return ToScene_unk(((MapManager*)manager)->Find_unk(sphere));
}

// inferred: calls `func` on the areas of the maps and on the areas without a map
void MapManager::fn_80491528(MapModelFunc_unk func, void* arg) {
    MapArea** model;
    for (MapTask** it = mMaps.mData; it != &mMaps.mData[mMaps.mSize]; it++) {
        MapTask* map = *it;
        if (mType == 0 && map->mType == 6) {
            continue;
        }
        if (mType != 0 && mType != map->mType) {
            continue;
        }
        if (map->mF8 != 0) {
            continue;
        }
        if (lbl_8087F610 != 0 && map->mType == 0) {
            continue;
        }
        MapPartsArea* area = map->GetArea_unk();
        if (area != 0) {
            for (model = area->mAreas.begin(); model != area->mAreas.end(); model++) {
                func(*model, arg);
            }
        }
    }
    for (unsigned int i = 0; i < mAreas.size(); i++) {
        MapPartsArea* area = mAreas[i];
        if (area != 0) {
            for (model = area->mAreas.begin(); model != area->mAreas.end(); model++) {
                func(*model, arg);
            }
        }
    }
}

// inferred: area hit by a sphere (FindArea in the RSO)
// NONMATCHING: 97.6% - register allocation of the loops, and the original reloads mAreaIndex of the
// current map inside GetArea_unk
MapArea* MapManager::FindArea(atn::Sphere sphere) {
    if (mType != 0) {
        MapTask** end = &mMaps.mData[mMaps.mSize];
        for (MapTask** it = mMaps.mData; it != end; it++) {
            MapTask* map = *it;
            if (mType == map->mType) {
                MapPartsArea* area = map->GetArea_unk();
                if (area != 0) {
                    return area->mAreas[0];
                }
            }
        }
    } else {
        for (MapTask** it = mMaps.mData; it != &mMaps.mData[mMaps.mSize]; it++) {
            MapTask* map = *it;
            if (map->mType == 6) {
                continue;
            }
            if (map->mF8 != 0) {
                continue;
            }
            if (lbl_8087F610 != 0 && map->mType == 0) {
                continue;
            }
            if (fn_800709F4(&map->mBox, &sphere) == 0) {
                continue;
            }
            MapPartsArea* area = map->GetArea_unk();
            if (area != 0) {
                MapArea* model = area->fn_804962B0(&sphere);
                if (model != 0) {
                    return model;
                }
            }
        }
        if (lbl_8087F430 != 0) {
            MapTask* map = ((GameTask*)lbl_8087F430)->GetCurrentMap();
            if (map != 0 && map->mAreaIndex != 0) {
                MapPartsArea* area = map->GetArea_unk();
                if (area != 0 && area->mAreas.size() != 0 && map->mF8 == 0) {
                    return area->mAreas[0];
                }
                MapTask** end = &mMaps.mData[mMaps.mSize];
                for (MapTask** it = mMaps.mData; it != end; it++) {
                    MapTask* m = *it;
                    if (m->mType == 6) {
                        continue;
                    }
                    if (m->mF8 != 0) {
                        continue;
                    }
                    MapPartsArea* a = m->GetArea_unk();
                    if (a != 0 && a->mAreas.size() != 0) {
                        return a->mAreas[0];
                    }
                }
            }
        }
    }
    for (unsigned int i = 0; i < mAreas.size(); i++) {
        MapPartsArea* area = mAreas[i];
        if (area != 0) {
            MapArea* model = area->fn_804962B0(&sphere);
            if (model != 0) {
                return model;
            }
        }
    }
    float radiusSq = sphere.mRadius * sphere.mRadius;
    for (unsigned int i = 0; i < mAreas.size(); i++) {
        MapPartsArea* area = mAreas[i];
        if (area != 0) {
            for (MapArea** model = area->mAreas.begin(); model != area->mAreas.end(); model++) {
                for (unsigned int j = 0; j < (*model)->mSegments.size(); j++) {
                    atn::AABB_unk* seg = &(*model)->mSegments[j];
                    float d0 = DistSq_unk(seg->mMin, sphere.mCenter);
                    float d1 = DistSq_unk(seg->mMax, sphere.mCenter);
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
    MapArea* model = FindArea(sphere);
    if (model != 0 && model->mEffConfig != 0) {
        return *(const MapColor_unk*)model->mEffConfig->fn_80043014(2, 0);
    }
    return MapColor_unk(0.0f, 0.0f, 0.0f, 0.0f);
}

// inferred: color 3 of the material of the model hit by a sphere
// NONMATCHING: 71% - as fn_80491D50
MapColor_unk MapManager::fn_80491DF8(atn::Sphere sphere) {
    MapArea* model = FindArea(sphere);
    if (model != 0 && model->mEffConfig != 0) {
        return *(const MapColor_unk*)model->mEffConfig->fn_80043014(3, 0);
    }
    return MapColor_unk(0.0f, 0.0f, 0.0f, 0.0f);
}
