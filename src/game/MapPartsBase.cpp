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
#include "atn/Light_unk.h"
#include "game/AreaEffConfig.h"
#include "std/string.h"
#include "std/string_members.h"
#include "std/new.h"
#include "std/vector.h"

// inferred: copies of inline functions of other classes that the original emits here (their caller,
// fn_80494354, is not written yet: then they become the weak copies of inline functions again)
extern "C" float fn_800DC288(const char* text);
extern "C" int strcmp(const char* a, const char* b);
extern "C" char* strncpy(char* dst, const char* src, unsigned long n);
extern "C" std::string fn_8006B174(const std::string& path); // inferred: file name of a path
extern "C" std::string fn_8006B2D8(const std::string& path); // inferred: name without extension
inline void fn_804948F0(float* values, atn::CsvBase<char, 9, 13, 34>* csv);
namespace Event {
// inferred: key light of the render manager (weak copy 0x804848C4 in Event/Manager.cpp; GetKeyLight in the RSO?)
atn::Light* GetViewLight_unk(atn::RenderManager* view);
}

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

// inferred: reads the areas once the csv is loaded, then waits for their scenes
int MapPartsArea::vf0C() {
    if (m64 != 0) {
        int ready = 1;
        for (int i = 0; i < mAreas.size(); i++) {
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

// inferred: reads the areas of the csv: the scene settings go to the current area (ParseScene), "AREA"
// starts a new one with its first box and the fog, ambient color and key light of the current scene
// NONMATCHING: 94.02% - the original keeps the current area in r28 (stored to the stack for push_back and
// reloaded after it; here the new area is read back from the stack), other registers, and the offsets of
// the keys in the string pool (ParseScene and vf1C, which come first and last in the pool, are not written)
void MapPartsArea::fn_80494354() {
    atn::CsvTab csv(mFile.fn_80470580(), mFile.fn_8047059C());
    MapArea* area = 0;
    do {
        const char* key = csv.fn_8005B9CC();
        if (strcmp(key, "AMBIENT_COLOR") == 0 || strcmp(key, "COVERAGEMAP") == 0 || strcmp(key, "CLOUDMAP") == 0 ||
            strcmp(key, "FOG") == 0 || strcmp(key, "RAIN") == 0 || strcmp(key, "HAZE") == 0 ||
            strcmp(key, "LIGHT") == 0 || strcmp(key, "LIGHTSURROUND") == 0 || strcmp(key, "GODRAY") == 0 ||
            strcmp(key, "COLOR_MATRIX") == 0 || strcmp(key, "SHADOW") == 0 || strcmp(key, "REFLECT") == 0) {
            ParseScene(&area->mScene, csv);
        } else if (strcmp(key, "AREA") == 0) {
            MapArea* newArea = new ((MemoryTag)12, "", "", 0) MapArea;
            strncpy(newArea->mName, csv.fn_8005B9CC(), 0x20);
            newArea->mName[0x1F] = 0;
            newArea->mHash = fn_800DC6B4(newArea->mName);
            atn::AABB_unk box;
            box.mMin.x = fn_800DC288(csv.fn_8005B9CC());
            box.mMin.y = fn_800DC288(csv.fn_8005B9CC());
            box.mMin.z = fn_800DC288(csv.fn_8005B9CC());
            box.mMax.x = fn_800DC288(csv.fn_8005B9CC());
            box.mMax.y = fn_800DC288(csv.fn_8005B9CC());
            box.mMax.z = fn_800DC288(csv.fn_8005B9CC());
            newArea->mSegments.push_back(box);
            *newArea->mScene.GetFog() = *fn_8008B964()->GetScene()->GetFog();
            newArea->mScene.GetAmbientLight() = fn_8008B964()->GetScene()->GetAmbientLight();
            newArea->mScene.AddLight_unk(*Event::GetViewLight_unk(fn_8008B964()));
            newArea->mEffConfig = 0;
            newArea->mRainPart = 0;
            mAreas.push_back(newArea);
            area = newArea;
        } else if (strcmp(key, "AREA_BOX") == 0) {
            atn::AABB_unk box;
            box.mMin.x = fn_800DC288(csv.fn_8005B9CC());
            box.mMin.y = fn_800DC288(csv.fn_8005B9CC());
            box.mMin.z = fn_800DC288(csv.fn_8005B9CC());
            box.mMax.x = fn_800DC288(csv.fn_8005B9CC());
            box.mMax.y = fn_800DC288(csv.fn_8005B9CC());
            box.mMax.z = fn_800DC288(csv.fn_8005B9CC());
            area->mSegments.push_back(box);
        } else if (strcmp(key, "AREA_CYLINDER") == 0) {
            MapPartsAreaParam_unk cylinder;
            cylinder.mPos.x = fn_800DC288(csv.fn_8005B9CC());
            cylinder.mPos.y = fn_800DC288(csv.fn_8005B9CC());
            cylinder.mPos.z = fn_800DC288(csv.fn_8005B9CC());
            cylinder.m10 = fn_800DC288(csv.fn_8005B9CC());
            cylinder.mC = fn_800DC288(csv.fn_8005B9CC());
            area->mParams.push_back(cylinder);
        } else if (strcmp(key, "EFF_CONFIG") == 0) {
            if (area != 0) {
                area->mEffConfig = AreaEffConfig::fn_80041A80(this, csv.fn_8005B9CC());
            }
        } else if (strcmp(key, "SET_AREA") == 0) {
            if (area != 0) {
                area->mPaths.push_back(fn_8006B2D8(fn_8006B174(std::string(csv.fn_8005B9CC()))));
            }
        } else if (strcmp(key, "RAINDROPS") == 0) {
            if (area != 0) {
                area->mRainPart = MapPartsRain::fn_8049CDBC(this, 8, &csv);
                area->mRainPart->fn_804948D8();
                area->mRainPart->fn_800D246C(1);
            }
        } else if (strcmp(key, "MODEL") != 0 && strcmp(key, "MODEL_MAT_COLOR") == 0) {
            if (area != 0) {
                MapArea::MatColorInfo info;
                info.mHash = fn_800DC6B4(fn_8006B2D8(fn_8006B174(std::string(csv.fn_8005B9CC()))).c_str());
                fn_804948F0(&info.mColor.r, &csv);
                area->mMatColors.push_back(info);
            }
        }
    } while (csv.fn_8005BBF8());
}

// inferred: reads a color (four values) from the csv
inline void fn_804948F0(float* values, atn::CsvBase<char, 9, 13, 34>* csv) {
    values[0] = fn_800DC288(csv->fn_8005B9CC());
    values[1] = fn_800DC288(csv->fn_8005B9CC());
    values[2] = fn_800DC288(csv->fn_8005B9CC());
    values[3] = fn_800DC288(csv->fn_8005B9CC());
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
atn::AABB_unk MapPartsArea::vf24(atn::AABBTree* tree) {
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
// inferred: not inlined into the creators of MapPartsArea (called with bl at 0x80491F08)
__declspec(noinline) MapPartsBase::MapPartsBase(atn::Task* parent, int type) : atn::Task(parent), mType(type), mNext(0) {}

// inferred: appends `part` at the end of the list that starts here (MapTask::CreateParts)
void MapPartsBase::fn_804963F8(MapPartsBase* part) {
    MapPartsBase* last = this;
    while (last->mNext != 0) {
        last = last->mNext;
    }
    last->mNext = part;
}

// inferred: sets the next part (MapTask::CreateParts)
void MapPartsBase::fn_80496414(MapPartsBase* next) {
    mNext = next;
}

void MapPartsBase::vf1C(void*) {}
