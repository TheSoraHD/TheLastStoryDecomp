#include "game/MapPartsBase.h"
#include "game/MapArea.h"
#include "game/MapManager.h"
#include "game/MapPartsArea.h"
#include "game/MapPartsRain.h"
#include "atn/Memory.h"
#include "atn/Rain.h"
#include "atn/RenderManager.h"
#include "game/EnemyManager.h"
#include "game/EnemyTask.h"
#include "game/NpcManager.h"
#include "game/NpcTask.h"
#include "game/PlayerManager.h"
#include "game/PlayerTask.h"
#include "game/TownTask.h"
#include "game/GameConfig_unk.h"
#include "atn/DrawHelper.h"
#include "std/new.h"
#include "std/vector.h"

// inferred: copies of inline functions of other classes that the original emits here (their caller,
// fn_80494354, is not written yet: then they become the weak copies of inline functions again)
extern "C" float fn_800DC288(const char* text);
extern "C" void fn_804948F0(float* values, atn::CsvBase<char, 9, 13, 34>* csv);

MapPartsArea* MapPartsArea::SetTask(atn::Task* parent, int type, atn::CsvTab& csv) {
    return fn_80491F94(parent, type, csv.fn_8005B9CC());
}

MapPartsArea* MapPartsArea::fn_80491F94(atn::Task* parent, int type, const char* path) {
    if (parent != 0) {
        return new ((MemoryTag)12, "", "", 0) MapPartsArea(parent, type, path);
    }
    return 0;
}

extern "C" PlayerManager* lbl_8087F8A0; // inferred: the party
extern "C" RenderSetting_unk* lbl_8087EFA8; // inferred: render settings
extern "C" GameConfig_unk* lbl_8087F0A8;

// inferred: names invented (same inline in game/CharaTask.cpp and game/ColliAttrManager.cpp)
inline atn::Scene* CharaTask::GetLens_unk() {
    return (mMotion.m1DC == 0) ? atn::RenderManager::instance_->mScene : mMotion.m1DC;
}

// inferred: the characters drawn with `scene` go back to the scene of the render manager
template <class T>
inline void ResetLens_unk(CharaManager<T>* manager, atn::Scene* scene) {
    if (manager != 0) {
        for (T* chara = manager->mHead; chara != 0; chara = chara->mNext) {
            if (chara->GetLens_unk() == scene) {
                chara->mMotion.m1DC = 0;
            }
        }
    }
}

// NONMATCHING: 98.51% - registers: the original keeps this in r27, the delete flag in r28 and the area in r26
// (here r26, r27 and r28)
MapPartsArea::~MapPartsArea() {
    for (unsigned int i = 0; i < mAreas.size(); i++) {
        if (lbl_8087FA20 != 0) {
            ResetLens_unk(lbl_8087F8A0, &mAreas[i]->mScene);
            ResetLens_unk(EnemyManager::sInstance, &mAreas[i]->mScene);
            ResetLens_unk(NpcManager::sInstance, &mAreas[i]->mScene);
        }
        MapArea* area = mAreas[i];
        delete area;
    }
}

// inferred: weak copy of atn::RenderManager::GetScene (RSO name); defined here (not inline) until
// its caller fn_80494354 is written
atn::Scene* atn::RenderManager::GetScene() {
    return mScene;
}

// inferred: reads the areas once the csv is loaded, then waits for their scenes
// NONMATCHING: 95.89% - the empty stub of fn_80494354 is inlined (the call goes away), and the loop
// increments the offset before the index
int MapPartsArea::vf0C() {
    if (m64 != 0) {
        int ready = 1;
        for (unsigned int i = 0; i < mAreas.size(); i++) {
            if (mAreas[i]->mScene.fn_800C2108() != 0) {
                ready = 0;
            }
        }
        if (ready != 0 && fn_800D3FA4() != 0) {
            return 1;
        }
    } else if (mFile.fn_80473F50() == 0) {
        fn_80494354();
        mFile.fn_80473F88();
        m64 = 1;
    }
    return 0;
}

// NONMATCHING: 0.05% - map area resource loading is not reconstructed yet.
void MapPartsArea::ParseScene(atn::Scene* scene, atn::CsvTab& csv) {}

// NONMATCHING: 0.28% - map area CSV parsing and resource setup are not reconstructed yet.
void MapPartsArea::fn_80494354() {}

// inferred: weak copy (starts the rain when it is on); defined here until fn_80494354 is written
void MapPartsRain::fn_804948D8() {
    if (mOn != 0) {
        mRain.fn_800B2180();
    }
}

// inferred: reads a color (four values) from the csv
extern "C" void fn_804948F0(float* values, atn::CsvBase<char, 9, 13, 34>* csv) {
    values[0] = fn_800DC288(csv->fn_8005B9CC());
    values[1] = fn_800DC288(csv->fn_8005B9CC());
    values[2] = fn_800DC288(csv->fn_8005B9CC());
    values[3] = fn_800DC288(csv->fn_8005B9CC());
}

// inferred: the implicit constructor (weak copy); defined here until fn_80494354 is written
MapArea::MapArea() {}

// The four functions below are the push_back of the vectors of MapArea and MapPartsArea (weak copies
// of std::vector<T>::push_back called by fn_80494354); written as functions until fn_80494354 is.
// NONMATCHING: 92.69% - the in-place copy matches; the vector reallocation path differs from the
// original's hand-inlined 24-byte element growth sequence.
extern "C" void fn_804949D4(std::vector<atn::AABB_unk>* values, const atn::AABB_unk* value) {
    values->push_back(*value);
}

// NONMATCHING: 96.56% - the allocator slow path matches, but MWCC loads size/capacity in the
// opposite register order in the fast path (same standalone std::vector pointer specialization).
extern "C" void fn_80494D78(std::vector<MapArea*>* values, MapArea* const* value) {
    values->push_back(*value);
}

// inferred: weak copy; defined here until fn_80494354 is written
MapPartsAreaParam_unk::MapPartsAreaParam_unk() {}

// NONMATCHING: 92.69% - the in-place 0x14-byte record copy matches; the vector reallocation path differs.
extern "C" void fn_80495070(std::vector<MapPartsAreaParam_unk>* values, const MapPartsAreaParam_unk* value) {
    values->push_back(*value);
}

// inferred: weak copy; defined here until fn_80494354 is written
MapArea::MatColorInfo::MatColorInfo() {}

// NONMATCHING: 92.90% - the in-place field copy matches; the vector reallocation path differs.
extern "C" void fn_80495418(std::vector<MapArea::MatColorInfo>* values, const MapArea::MatColorInfo* value) {
    values->push_back(*value);
}

// NONMATCHING: 0.22% - map debug menu entries are not reconstructed yet.
void MapPartsArea::vf1C(void*) {}

// inferred: lights the characters with the light of each area
void MapPartsArea::vf10() {
    for (int i = 0; i < mAreas.size(); i++) {
        atn::Scene& scene = mAreas[i]->mScene;
        float speedY = lbl_8087EFA8->m304;
        float speedX = lbl_8087EFA8->m300;
        atn::Vector2_unk speed(speedX, speedY);
        if (scene.GetLight_unk() != 0 && scene.m074 == 0) {
            scene.GetLight_unk()->fn_800C18BC();
        }
    }
}

extern "C" void fn_8006FE08(const atn::AABB_unk* box, unsigned int color); // inferred: draws a box (debug)

// inferred: draws the boxes and cylinders of the areas and the lights of their scenes (debug)
// NONMATCHING: 95.65% - registers (the area in r31) and the order of the ors of Color::ToU32
void MapPartsArea::vf14() {
    if (lbl_8087F0A8->mDrawMapBounds != 0) {
        for (unsigned int i = 0; i < mAreas.size(); i++) {
            MapArea* area = mAreas[i];
            unsigned int j = 0;
            if (area->mSegments.size() != 0) {
                for (; j < area->mSegments.size(); j++) {
                    fn_8006FE08(&area->mSegments[j], area->mScene.mAmbient.ToU32());
                }
            }
            for (unsigned int k = 0; k < area->mParams.size(); k++) {
                MapPartsAreaParam_unk& cyl = area->mParams[k];
                lbl_8087EEB0->fn_80062C8C(0x10, 0xFFFFFFFF, cyl.mPos.x, cyl.mPos.y, cyl.mPos.z, 0.0f, cyl.mC, cyl.m10);
            }
            area->mScene.fn_800C2D88();
        }
    }
}

// inferred: bounding box of the boxes of the areas
atn::AABB_unk MapPartsArea::vf24(MapPartsTree_unk* tree) {
    atn::AABB_unk box;
    box.mMin = atn::Vector3(1e16f, 1e16f, 1e16f);
    box.mMax = atn::Vector3(-1e16f, -1e16f, -1e16f);
    for (int i = 0; i < mAreas.size(); i++) {
        MapArea* area = mAreas[i];
        for (int j = 0; j < area->mSegments.size(); j++) {
            box = area->mSegments[j].Merge_unk(box);
        }
    }
    return box;
}

extern "C" int fn_800709F4(const atn::AABB_unk* box, const atn::Sphere* sphere);              // inferred: box hit by the sphere
extern "C" int fn_80070D04(const MapPartsAreaParam_unk* cylinder, const atn::Sphere* sphere); // inferred: cylinder hit by the sphere

// inferred: area whose boxes or cylinders are hit by the sphere
// NONMATCHING: 98.55% - the original keeps the area iterator in r31 and the element offset in r30
MapArea* MapPartsArea::fn_804962B0(const atn::Sphere* sphere) {
    for (MapArea** it = mAreas.begin(); it != mAreas.end(); it++) {
        for (unsigned int i = 0; i < (*it)->mSegments.size(); i++) {
            if (fn_800709F4(&(*it)->mSegments[i], sphere) != 0) {
                return *it;
            }
        }
        for (unsigned int i = 0; i < (*it)->mParams.size(); i++) {
            if (fn_80070D04(&(*it)->mParams[i], sphere) != 0) {
                return *it;
            }
        }
    }
    return 0;
}

int MapPartsBase::vf30() {
    return 0;
}

void MapPartsBase::vf2C(unsigned int, int) {}

void MapPartsBase::vf28(atn::CsvTab*) {}

bool MapPartsBase::vf20(const atn::Vector3&) {
    return false;
}
extern "C" void fn_804963F8(MapPartsBase* self, MapPartsBase* next);
extern "C" void fn_80496414(MapPartsBase* self, MapPartsBase* next);

// inferred: not inlined into the creators of MapPartsArea (called with bl at 0x80491F08)
__declspec(noinline) MapPartsBase::MapPartsBase(atn::Task* parent, int type) : atn::Task(parent), mType(type), mNext(0) {}

extern "C" void fn_804963F8(MapPartsBase* self, MapPartsBase* next) {
    while (self->mNext != 0) {
        self = self->mNext;
    }
    self->mNext = next;
}

extern "C" void fn_80496414(MapPartsBase* self, MapPartsBase* next) {
    self->mNext = next;
}

void MapPartsBase::vf1C(void*) {}
