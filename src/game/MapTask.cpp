#include "game/MapTask.h"
#include "game/GameConfig_unk.h"
#include "game/MapInstance_unk.h"
#include "game/MapPartsBase.h"
#include "game/MapPartsLocatorSet.h"
#include "game/MapManager.h"
#include "atn/RenderManager.h"
#include "atn/GraphicsDevice.h"
#include "atn/LayerReflect.h"
#include "atn/GXDecl.h"
#include "db/MapIdTable_unk.h"
#include "game/GimmickManager.h"
#include "game/TownTask.h"
#include "atn/Memory.h"

extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash

// inferred: debug menu callback of "Enable" (atn::MindowsFunc_unk: the map and the address of the
// pointer to the value): shows or hides all the parts
void fn_8049D264(void* arg, void* value) {
    ((MapTask*)arg)->fn_804A0E18("<all>", **(int**)value);
}
// NONMATCHING: stub - process map part state.
extern "C" void fn_8049D2E4() {}
int MapTask::GetMapNameFromId(char* name, int type, int id, int subId) {
    switch (type) {
    case 0:
        sprintf(name, "wd%03d_%02d\0", id, subId);
        break;
    case 1:
        sprintf(name, "tw%02d_%02d\0", id, subId);
        break;
    case 2:
        sprintf(name, "dg%03d_%02d\0", id, subId);
        break;
    case 3:
    case 4:
        sprintf(name, "mp%03d_%02d\0", id, subId);
        break;
    default:
        return 0;
    }
    return 1;
}

// inferred: name of the map without the sub id
int MapTask::fn_8049D5DC(char* name, int type, int id, int subId) {
    switch (type) {
    case 0:
        sprintf(name, "wd%03d\0", id);
        break;
    case 1:
        sprintf(name, "tw%02d\0", id);
        break;
    case 2:
        sprintf(name, "dg%03d\0", id);
        break;
    case 3:
    case 4:
        sprintf(name, "mp%03d\0", id);
        break;
    default:
        return 0;
    }
    return 1;
}
MapTask* MapTask::SetTask(atn::Task* parent, const char* path) {
    if (parent != 0) {
        return new ((MemoryTag)0xC, "", "", 0) MapTask(parent, path);
    }
    return 0;
}

// inferred: towns (type 1) are TownTask
MapTask* MapTask::fn_8049D704(atn::Task* parent, int type, int id, int subId) {
    if (parent != 0) {
        if (type == 1) {
            return new ((MemoryTag)0xC, "", "", 0) TownTask(parent, type, id, subId);
        }
        return new ((MemoryTag)0xC, "", "", 0) MapTask(parent, type, id, subId);
    }
    return 0;
}
MapTask::MapTask(atn::Task* parent, const char* path)
    : atn::Task(parent), mType(0), mId(0), mSubId(0), m54(0), mParts(0), m74(0), m78(0.0f), m7C(0.0f), m80(0.0f),
      m84(1.0f), mNextArea(0), mCC(0), mCamera(0), mE4(0), mDefaultCamera(0), mEC(0), mF0(0.0f), mF4(0), mF8(0),
      m100(0), m104(0), mAreaIndex(0), m10C(0), m110(0.0f), m114(0.01f), m118(1) {
    mBox.mMin = atn::Vector3(1e16f, 1e16f, 1e16f);
    mBox.mMax = atn::Vector3(-1e16f, -1e16f, -1e16f);
    if (MapManager::instance_ != 0) {
        MapManager::instance_->fn_80491418(this);
    }
    mFile.vf0C(path);
}

extern "C" atn::GraphicsDevice_unk* lbl_8087EEE0; // inferred: graphics device (screen size)

// inferred: path of the map file of an id (inlined in the constructor; name invented)
static inline int GetMapPath_unk(char* path, int type, int id, int subId) {
    char name[0x100];
    if (MapTask::GetMapNameFromId(name, type, id, subId)) {
        if (type == 1) {
            sprintf(path, "/database/map/%s.twn", name);
        } else {
            sprintf(path, "/database/map/%s.map", name);
        }
        return 1;
    }
    return 0;
}

MapTask::MapTask(atn::Task* parent, int type, int id, int subId)
    : atn::Task(parent), mType(type), mId(id), mSubId(subId), m54(0), mParts(0), m74(0), m78(0.0f), m7C(0.0f),
      m80(0.0f), m84(1.0f), mNextArea(0), mCC(0), mCamera(0), mE4(0), mDefaultCamera(0), mEC(0), mF0(0.0f), mF4(0),
      mF8(0), m100(0), m104(0), mAreaIndex(0), m10C(0), m110(0.0f), m114(0.01f), m118(1) {
    mBox.mMin = atn::Vector3(1e16f, 1e16f, 1e16f);
    mBox.mMax = atn::Vector3(-1e16f, -1e16f, -1e16f);
    if (MapManager::instance_ != 0) {
        MapManager::instance_->fn_80491418(this);
    }
    char path[0x100];
    if (GetMapPath_unk(path, type, id, subId)) {
        mFile.vf0C(path);
    }
    m54 = fn_8021771C(type, id, subId);
    if (m54 != 0) {
        char buf[0x100];
        if (*m54->m5C != 0) {
            sprintf(buf, "/database/camera/%s.csv", m54->m5C);
            m60.vf0C(buf);
        }
        if (*m54->m60 != 0) {
            sprintf(buf, "/camera/ambient/%s.xsca", m54->m60);
            m68.vf0C(buf);
        }
        atn::Texture_unk* tex;
        int texWidth;
        int width = lbl_8087EEE0->mWidth / 2;
        int height = lbl_8087EEE0->mHeight / 2;
        if (m54->m58 > 0) {
            width *= m54->m58;
            height *= m54->m58;
        }
        if (type == 1) {
            width = lbl_8087EEE0->mWidth / 4;
            height = lbl_8087EEE0->mHeight / 4;
        }
        tex = &static_cast<atn::LayerReflect*>(atn::RenderManager::instance_->mLayers[4])->mTexture;
        texWidth = GXGetTexObjWidth(&tex->mObj);
        int texHeight = GXGetTexObjHeight(&tex->mObj);
        if (texWidth != width || texHeight != height) {
            tex->fn_800D5B58(width, height, 4, 0, "ReflectionTexture");
            GXInitTexObjLOD(&tex->mObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        }
    }
    if (GimmickManager::instance_ != 0) {
        GimmickManager::instance_->fn_803EE5AC(mType, mId, mSubId);
    }
    if (id == 0x30 || id == 0x36) {
        m104 = 1;
    }
}
extern "C" void fn_8009FE24(); // inferred: atn/Motion_unk.cpp
MapTask::~MapTask() {
    m74 = 0xDEADBEEF;
    if (MapManager::instance_ != 0) {
        MapManager::instance_->fn_80491440(this);
    }
    atn::RenderManager::instance_->fn_800BDB40(0);
    fn_8009FE24();
    m60.fn_80473F88();
}
// NONMATCHING: stub - update the map models and the current map selection.
int MapTask::vf0C() { return 0; }
// NONMATCHING: stub - load map resources.
extern "C" void fn_8049E480() {}
// NONMATCHING: stub - update visible map components.
void MapTask::vf10() {}

extern "C" GameConfig_unk* lbl_8087F0A8;
extern "C" void fn_8006FE08(const atn::AABB_unk* box, unsigned int color); // inferred: draws a box (debug)
void MapTask::vf14() {
    if (lbl_8087F0A8->mDrawMapBounds != 0) {
        fn_8006FE08(&mBox, 0xFF00FFFFu);
    }
}

// NONMATCHING: stub - load and select a map model.
extern "C" void fn_8049ED90() {}
// NONMATCHING: stub - update map parts and resource state.
extern "C" void fn_8049F4F4() {}
// NONMATCHING: stub - sets the camera of the map.
void MapTask::fn_804A04AC(int camera, int a, int b, gs::CameraBox* box) {}
// inferred: shows all the parts, or hides those that contain `pos`
void MapTask::fn_804A0580(int show, const atn::Vector3* pos) {
    for (MapPartsBase* part = mParts; part != 0; part = part->mNext) {
        if (show != 0) {
            part->MeltTask();
        } else if (part->vf20(*pos)) {
            part->FreezeTask();
        }
    }
}

// inferred: instance of the locator sets with the name `name`
MapInstance_unk* MapTask::fn_804A0614(const std::string& name) {
    unsigned int hash = fn_800DC6B4(name.c_str());
    for (MapPartsBase* part = mParts; part != 0; part = part->mNext) {
        if (part->mType == 4) {
            MapPartsLocatorSet* set = (MapPartsLocatorSet*)part;
            for (int i = 0; i < set->mInstances.mSize; i++) {
                MapInstance_unk* inst = set->mInstances[i];
                if (hash == inst->mHash) {
                    return inst;
                }
            }
        }
    }
    return 0;
}

// NONMATCHING: stub - process a map transition.
extern "C" void fn_804A06B4() {}
// NONMATCHING: stub - process a map transition.
extern "C" void fn_804A0BCC() {}
// NONMATCHING: stub - update map resource loading.
void MapTask::fn_804A0E18(const char* name, int draw) {
    unsigned int hash = fn_800DC6B4(name);
    for (MapPartsBase* part = mParts; part != 0; part = part->mNext) {
        part->vf2C(hash, draw);
    }
}
// NONMATCHING: stub - update map resource loading.
void MapTask::fn_804A0E90(unsigned int hash, int draw) {
    for (MapPartsBase* part = mParts; part != 0; part = part->mNext) {
        part->vf2C(hash, draw);
    }
}
// inferred: switches to the area list `area` (no blend)
// NONMATCHING: the empty stub of fn_804A0F80 is inlined (the original ends with a tail call)
void MapTask::fn_804A0EFC(MapPartsArea* area) {
    for (int i = 0; i < mAreas.mSize; i++) {
        if (mAreas.mData[i] == area) {
            fn_804A0F58(i, 1.0f, 0.0f);
            return;
        }
    }
}

// inferred: switches to the area list `index` with a blend (t, speed)
// NONMATCHING: the empty stub of fn_804A0F80 is inlined (the original ends with a tail call)
void MapTask::fn_804A0F58(int index, float t, float speed) {
    if (index < (int)mAreas.mSize) {
        m10C = mAreaIndex;
        mAreaIndex = index;
        m110 = t;
        m114 = speed;
        fn_804A0F80();
    }
}

// NONMATCHING: stub - transition between the area lists.
void MapTask::fn_804A0F80() {}
// NONMATCHING: stub - refreshes the map after a load.
void MapTask::fn_804A1C4C() {}
// NONMATCHING: stub - stores the state of the map.
void MapTask::fn_804A1D2C() {}
