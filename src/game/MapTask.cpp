#include "game/MapTask.h"
#include "game/GameTask.h"
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
#include "atn/RenderSetting_unk.h"
#include "game/MapArea.h"
#include "game/MapPartsArea.h"
#include "game/MapPartsRain.h"
#include "game/MapPartsGimmickLoc_unk.h"
#include "game/MapPartsModel.h"
#include "atn/Light_unk.h"
#include "atn/SceneColorMatrix_unk.h"
#include "game/MapPartsFactory.h"
#include "atn/Plane_unk.h"
#include "atn/Lerp_unk.h"

extern "C" GameTask* lbl_8087F430; // inferred: the field

extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
extern "C" void fn_8009FE24(); // inferred: atn/Motion_unk.cpp
extern "C" std::string fn_8006B174(const std::string& path); // inferred: file name of a path
extern "C" std::string fn_8006B2D8(const std::string& path); // inferred: name without extension

// inferred: debug menu callback of "Enable" (atn::MindowsFunc_unk: the map and the address of the
// pointer to the value): shows or hides all the parts
void fn_8049D264(void* arg, void* value) {
    ((MapTask*)arg)->fn_804A0E18("<all>", **(int**)value);
}
// inferred: maps drawn with the "No Depth" shadow setting (fn_8049ED90 stores the result in
// RenderSetting_unk::m134): towns, some dungeons and types 3/4
bool MapTask::fn_8049D2E4(int type, int id, int subId) {
    if (type == 1) {
        return true;
    }
    if (type == 2) {
        if (id == 4) {
            return subId == 1 || subId == 3;
        }
        if (id == 5) {
            return subId == 1 || subId == 3;
        }
        if (id == 2 && subId == 1) {
            return true;
        }
        if (id == 0x10 && subId == 2) {
            return true;
        }
        if (id == 7 && subId == 1) {
            return true;
        }
        if (id == 0x12 && subId == 1) {
            return true;
        }
        if (id == 0xC) {
            return true;
        }
        if (id == 0x13) {
            return true;
        }
        if (id == 0x18) {
            return subId == 2 || subId == 3;
        }
        if (id == 0x19) {
            return subId == 1 || subId == 2;
        }
        if (id == 0x23 && subId == 3) {
            return true;
        }
        if (id == 0x24) {
            return true;
        }
        if (id == 0x27 && subId == 3) {
            return true;
        }
        if (id == 0xA && subId == 1) {
            return true;
        }
        if (id == 0xE) {
            return subId == 2 || subId == 3 || subId == 6;
        }
        if (id == 0x29) {
            return subId == 2 || subId == 3 || subId == 6 || subId == 8;
        }
        if (id == 0x2A) {
            return true;
        }
        if (id == 0x28) {
            return subId == 5 || subId == 8;
        }
        if (id == 0x36) {
            return subId == 1;
        }
    }
    if (type == 3 || type == 4) {
        return true;
    }
    return false;
}
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
MapTask::~MapTask() {
    m74 = 0xDEADBEEF;
    if (MapManager::instance_ != 0) {
        MapManager::instance_->fn_80491440(this);
    }
    atn::RenderManager::instance_->fn_800BDB40(0);
    fn_8009FE24();
    m60.fn_80473F88();
}
// inferred: debug menu items (atn/MindowsCommon.h; the folder is the handle mC8)
extern "C" void fn_800874C8(void* folder, const char* name, int* value, int min, int max, int step, void* func,
                            void* arg);
extern "C" void fn_8008771C(void* folder, const char* name, float* value, float min, float max, float step,
                            void* func, void* arg);
extern "C" RenderSetting_unk* lbl_8087EFA8;

// inferred: limits of the rain drop count of the areas (only used here)
extern "C" {
int lbl_8087E0D8 = 0; // NONMATCHING: in .sdata in the original (zero data needs explicit_zero_data)
int lbl_8087E0DC = 0x1000;
}

template <class T> inline T Clamp_unk(T v, const T& lo, const T& hi) {
    return v < lo ? lo : (v > hi ? hi : v);
}

// NONMATCHING: 97.38% - `this` and the path of the map file swap r26/r27; GameTask is loaded once for the
// camera block (the original reloads it) and the rain part is loaded after the clamp limits
// inferred: loads the parts once the map file is ready, then waits for the camera files
int MapTask::vf0C() {
    if (m74 == 0) {
        if (mFile.fn_80473F50()) {
            return 0;
        }
        mCC = (int)mC8.Load(fn_8006B174(std::string(mFile.fn_80473F18())).c_str());
        fn_800874C8((void*)mCC, "Brightness", &m104, 0, 3, 1, 0, 0);
        fn_80087994((void*)mCC, "Enable", &m118, fn_8049D264, this);
        void* folder = fn_8008937C((void*)mCC, "AreaChange");
        fn_800874C8(folder, "Index", &mAreaIndex, 0, 8, 1, 0, 0);
        fn_8008771C(folder, "Blend", &m110, 0.0f, 1.0f, 0.01f, 0, 0);
        fn_8008771C(folder, "BlendSpeed", &m114, 0.0f, 1.0f, 0.01f, 0, 0);
        fn_8049ED90();
        m74 = 1;
        if (mF4 == 0) {
            RenderSetting_unk* setting = lbl_8087EFA8;
            setting->m03C.r = m78;
            setting->m03C.g = m7C;
            setting->m03C.b = m80;
            setting->m03C.a = m84;
        }
        mFile.fn_80473F88();
    }
    if (fn_800D3FA4() && !m60.fn_80473F50() && !m68.fn_80473F50()) {
        if (lbl_8087F430 != 0 && (lbl_8087FA20 == 0 || lbl_8087FA20 == this) && mType != 5 && mType != 6) {
            lbl_8087F430->mCamera.fn_8037DD00();
            fn_804A04AC(0, 0, -1, 0);
            float scale = m54 != 0 ? m54->m64 : 1.0f;
            lbl_8087F430->mCamera.m8F4 = m68.fn_804786F8();
            lbl_8087F430->mCamera.m8FC = scale;
        }
        MapPartsBase* part;
        for (part = mParts; part != 0; part = part->mNext) {
            part->fn_800D246C(0);
        }
        for (part = mParts; part != 0; part = part->mNext) {
            mBox = mBox.Merge_unk(part->vf24(0));
            part->vf1C((void*)mCC);
            if (part->mType == 5) {
                ((MapPartsGimmickLoc*)part)->SetAreaEffConfig();
            }
            if (part->vf30()) {
                mEC = 1;
            }
        }
        fn_8049E480();
        mF0 = lbl_8087EFA8->mDof.mNearEnd;
        int count = 0;
        for (unsigned int i = 0; i < mAreas.mSize; i++) {
            MapPartsArea* list = mAreas.mData[i];
            for (unsigned int j = 0; j < list->mAreas.size(); j++) {
                MapArea* area = list->mAreas[j];
                MapPartsRain* rain = area->mRainPart;
                if (rain != 0) {
                    if (rain->mOn != 0) {
                        count = count > rain->mCount ? count : rain->mCount;
                    } else {
                        rain->mCount = Clamp_unk(0, lbl_8087E0D8, lbl_8087E0DC);
                    }
                    area->mRainPart->fn_800D246C(0);
                    if (mNextArea != 0) {
                        area->mRainPart->FreezeTask();
                    }
                }
            }
        }
        if (mNextArea != 0 && count > 0) {
            for (unsigned int j = 0; j < mNextArea->mAreas.size(); j++) {
                MapArea* area = mNextArea->mAreas[j];
                if (area->mRainPart != 0) {
                    area->mRainPart->mOn = 1;
                    area->mRainPart->mCount = Clamp_unk(count, lbl_8087E0D8, lbl_8087E0DC);
                    area->mRainPart->fn_804948D8();
                    area->mRainPart->fn_800D246C(0);
                }
            }
            fn_804A0F80();
        }
        return 1;
    }
    return 0;
}

// NONMATCHING: 98.58% - register allocation: the string temporaries and the strlen results take other
// saved registers (r16-r18 in the original); ~10 variants (locals, switch, loop forms)
// inferred: gives the scenes of the current area list to the parts: the locator sets and gimmicks pick
// theirs by position, then the parts named by an area ("MODEL" names of MapArea::m40) get its scene
void MapTask::fn_8049E480() {
    MapPartsArea* list = GetArea_unk();
    if (list == 0) {
        return;
    }
    for (MapPartsBase* part = mParts; part != 0; part = part->mNext) {
        if (part->mType == 4) {
            ((MapPartsLocatorSet*)part)->fn_8049B7CC(list);
        } else if (part->mType == 5) {
            ((MapPartsGimmickLoc*)part)->fn_80499D60(list);
        }
    }
    int count = list->mAreas.size();
    for (int i = 0; i < count; i++) {
        MapArea* area = list->mAreas[i];
        for (unsigned int j = 0; j < list->mAreas[i]->mPaths.size(); j++) {
            std::string& name = area->mPaths[j];
            for (MapPartsBase* part = mParts; part != 0; part = part->mNext) {
                if (part->mType == 0) {
                    MapPartsModel* model = (MapPartsModel*)part;
                    std::string file = fn_8006B2D8(fn_8006B174(std::string(model->mModel.fn_8008B130())));
                    if (name == file) {
                        model->mModel.mFlags |= 0x10000;
                        model->mModel.m1DC = &area->mScene;
                    }
                } else if (part->mType == 4) {
                    MapPartsLocatorSet* set = (MapPartsLocatorSet*)part;
                    std::string file = fn_8006B2D8(fn_8006B174(std::string(set->mName.c_str())));
                    if (name == file) {
                        set->fn_8049B780(&area->mScene);
                    }
                } else if (part->mType == 5) {
                    std::string file =
                        fn_8006B2D8(fn_8006B174(std::string(((MapPartsGimmickLoc*)part)->mFile.fn_80473F18())));
                    if (name == file) {
                        ((MapPartsGimmickLoc*)part)->fn_80499CCC(&area->mScene);
                    }
                }
            }
        }
    }
}

// inferred: blends the area lists, gives the material colours of the current areas to the models and
// lights the characters with the light of the scene
void MapTask::vf10() {
    fn_804A0F80();
    MapPartsArea* list = GetArea_unk();
    if (list != 0) {
        for (unsigned int i = 0; i < list->mAreas.size(); i++) {
            MapArea* area = list->mAreas[i];
            for (unsigned int j = 0; j < area->mMatColors.size(); j++) {
                MapPartsModel* model = (MapPartsModel*)fn_804A0BCC(0, area->mMatColors[j].mHash);
                if (model != 0) {
                    atn::Color& color = area->mMatColors[j].mColor;
                    model->mModel.m40.m00 = color.r;
                    model->mModel.m40.m04 = color.g;
                    model->mModel.m40.m08 = color.b;
                    model->mModel.m40.m0C = color.a;
                }
            }
        }
    }
    if (mF4 == 0) {
        fn_8009FE24();
    }
    if (lbl_8087FA20 == 0 && atn::RenderManager::instance_->GetScene()->m074 == 0 &&
        atn::RenderManager::instance_->fn_800C16B4()->GetLight_unk() != 0) {
        atn::RenderManager::instance_->fn_800C16B4()->GetLight_unk()->fn_800C18BC();
    }
}

extern "C" GameConfig_unk* lbl_8087F0A8;
extern "C" void fn_8006FE08(const atn::AABB_unk* box, unsigned int color); // inferred: draws a box (debug)
void MapTask::vf14() {
    if (lbl_8087F0A8->mDrawMapBounds != 0) {
        fn_8006FE08(&mBox, 0xFF00FFFFu);
    }
}

// NONMATCHING: 93.20% - the post effect colour store is missing (lbl_8087EF8C + 0x2EC = zero, a member of the
// god ray of atn::PostFxManager, which has no header yet: it is defined in atn/LinearAllocator.cpp)
// inferred: resets the render settings to their defaults (the map file sets them again) and creates the
// parts of the map file
void MapTask::fn_8049ED90() {
    atn::CsvTab csv(mFile.fn_80470580(), mFile.fn_8047059C());
    if (mF4 == 0) {
        atn::RenderManager::instance_->fn_800C16B4()->SetLight_unk(0);
        atn::RenderManager::instance_->fn_800C16B4()->fn_800C289C(0);
        lbl_8087EFA8->ResetPostFx_unk();
        lbl_8087EFA8->m3DC.mEnable = 0;
        lbl_8087EFA8->ResetShadow_unk();
        lbl_8087EFA8->m03C = RsColor_unk(0.0f, 0.0f, 0.0f, 1.0f);
        lbl_8087EFA8->m134 = fn_8049D2E4(mType, mId, mSubId);
        int deferred;
        if (mType == 1) {
            deferred = 0;
        } else if (mType == 2) {
            if (mId == 4 && mSubId == 3) {
                deferred = 1;
            } else if (mId == 0x12 && mSubId == 1) {
                deferred = 0;
            } else {
                deferred = 1;
            }
        } else {
            deferred = 1;
        }
        lbl_8087EFA8->m398 = deferred;
        for (int i = 0; i < 0x14; i++) {
            if (atn::RenderManager::instance_->mLayers[i] != 0) {
                atn::RenderManager::instance_->mLayers[i]->m04 = 1;
            }
        }
    }
    CreateParts(csv, 0);
}
extern "C" float fn_800DC288(const char* text); // inferred: atof (atn/Util_unk.cpp)
extern "C" int fn_800DC12C(const char* text);    // inferred: atoi
extern "C" int strcmp(const char* a, const char* b);
extern "C" void fn_800D2338(atn::Task* task); // inferred: deletes the task (atn::Task::DeleteTask in the RSO?)

// inferred: adds a part at the end of the list
inline void MapTask::AddPart_unk(MapPartsBase* part) {
    if (part == 0) {
        return;
    }
    part->fn_800D246C(1);
    if (mParts == 0) {
        mParts = part;
    } else {
        mParts->fn_804963F8(part);
    }
}

// inferred: removes a part from the list and deletes it (the first one is only unlinked)
inline void MapTask::RemovePart_unk(MapPartsBase* part) {
    if (part == 0) {
        return;
    }
    if (part == mParts) {
        mParts = part->mNext;
        return;
    }
    for (MapPartsBase* prev = mParts; prev != 0; prev = prev->mNext) {
        if (prev->mNext == part) {
            prev->fn_80496414(part->mNext);
            part->fn_80496414(0);
            fn_800D2338(part);
            return;
        }
    }
}

// NONMATCHING: 99.84% - the part key and the replaced part swap r15/r16 in the default branch (~12 variants)
// inferred: reads the map csv: render settings of the map (unless mF4) and the parts; with `replace`, a part
// replaces the one of the same type and file, and CMD_DELETE removes one
void MapTask::CreateParts(atn::CsvTab& csv, int replace) {
    int noSettings = mF4;
    do {
        const char* key = csv.fn_8005B9CC();
        if (*key == '\0' || *key == '#') {
            continue;
        }
        if (strcmp(key, "AMBIENT_COLOR") == 0 && noSettings == 0) {
            atn::Scene* scene = atn::RenderManager::instance_->GetScene();
            scene->mAmbient.r = fn_800DC288(csv.fn_8005B9CC());
            scene->mAmbient.g = fn_800DC288(csv.fn_8005B9CC());
            scene->mAmbient.b = fn_800DC288(csv.fn_8005B9CC());
            scene->mAmbient.a = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(key, "CLEAR_COLOR") == 0 && noSettings == 0) {
            m78 = fn_800DC288(csv.fn_8005B9CC());
            m7C = fn_800DC288(csv.fn_8005B9CC());
            m80 = fn_800DC288(csv.fn_8005B9CC());
            RenderSetting_unk* setting = lbl_8087EFA8;
            setting->m03C.r = m78;
            setting->m03C.g = m7C;
            setting->m03C.b = m80;
            setting->m03C.a = m84;
        } else if (strcmp(key, "CLOUDMAP") == 0 && noSettings == 0) {
            atn::Scene* scene = atn::RenderManager::instance_->GetScene();
            CharaLightData_unk light;
            float width = fn_800DC288(csv.fn_8005B9CC());
            float height = fn_800DC288(csv.fn_8005B9CC());
            scene->fn_800C289C(csv.fn_8005B9CC());
            scene->GetLight_unk()->m04 = atn::Vector2_unk(0.0f, 0.0f);
            scene->GetLight_unk()->mSize = atn::Vector2_unk(width, height);
        } else if (strcmp(key, "CAMERA_CLIP") == 0 && noSettings == 0) {
            float nearClip = fn_800DC288(csv.fn_8005B9CC());
            lbl_8087EFA8->m1C4 = fn_800DC288(csv.fn_8005B9CC());
            lbl_8087EFA8->m1C0 = nearClip;
        } else if (strcmp(key, "DOF") == 0 && noSettings == 0) {
            int enabled = strcmp(csv.fn_8005B9CC(), "true") == 0;
            lbl_8087EFA8->mDof.mEnabled = enabled;
            if (enabled) {
                DofParam_unk dof;
                dof.mEnabled = enabled;
                dof.mStart = fn_800DC288(csv.fn_8005B9CC());
                dof.mEnd = fn_800DC288(csv.fn_8005B9CC());
                dof.mNearStart = fn_800DC288(csv.fn_8005B9CC());
                dof.mNearEnd = fn_800DC288(csv.fn_8005B9CC());
                const char* blur = csv.fn_8005B9CC();
                if (strcmp("", blur) != 0) {
                    dof.mBlurWidth = fn_800DC288(blur);
                    dof.mBlurHeight = fn_800DC288(csv.fn_8005B9CC());
                }
                const char* clear = csv.fn_8005B9CC();
                if (strcmp("", clear) != 0) {
                    dof.mClearFar = strcmp(clear, "CLEARFAR") == 0;
                }
                lbl_8087EFA8->mDof = dof;
            }
        } else if (strcmp(key, "FOG") == 0 && noSettings == 0) {
            atn::Scene* scene = atn::RenderManager::instance_->GetScene();
            int on = strcmp(csv.fn_8005B9CC(), "ON") == 0;
            if (on) {
                int mode = fn_800DC12C(csv.fn_8005B9CC());
                float start = fn_800DC288(csv.fn_8005B9CC());
                float end = fn_800DC288(csv.fn_8005B9CC());
                float r = fn_800DC288(csv.fn_8005B9CC());
                float g = fn_800DC288(csv.fn_8005B9CC());
                float b = fn_800DC288(csv.fn_8005B9CC());
                scene->mFog.mMode = mode;
                scene->mFog.mStart = start;
                scene->mFog.mEnd = end;
                scene->mFog.mColor = atn::Color(r, g, b, 1.0f);
            } else {
                scene->mFog.mMode = 0;
            }
        } else if (strcmp(key, "BLOOM") == 0 && noSettings == 0) {
            int enabled = strcmp(csv.fn_8005B9CC(), "true") == 0;
            lbl_8087EFA8->m054.mEnabled = enabled;
            if (enabled) {
                BloomParam_unk bloom = lbl_8087EFA8->m054;
                for (int i = 0; i < 3; i++) {
                    if (strcmp("", csv.fn_8005B9CC()) == 0) {
                        break;
                    }
                    bloom.mColor[i].r = fn_800DC288(csv.GetField_unk());
                    bloom.mColor[i].g = fn_800DC288(csv.fn_8005B9CC());
                    bloom.mColor[i].b = fn_800DC288(csv.fn_8005B9CC());
                    bloom.mBias[i] = fn_800DC12C(csv.fn_8005B9CC());
                    bloom.mScale[i] = fn_800DC12C(csv.fn_8005B9CC());
                }
                const char* blur = csv.fn_8005B9CC();
                if (strcmp("", blur) != 0) {
                    bloom.mBlurWidth = fn_800DC288(blur);
                    bloom.mBlurHeight = fn_800DC288(csv.fn_8005B9CC());
                    bloom.mBlurAngle = fn_800DC288(csv.fn_8005B9CC());
                }
                lbl_8087EFA8->m054 = bloom;
            }
        } else if (strcmp(key, "GODRAY") == 0 && noSettings == 0) {
            GodRayParam_unk godRay;
            godRay.fn_800BB6C0(&csv);
            lbl_8087EFA8->m264 = godRay;
        } else if (strcmp(key, "SHADOW") == 0 && noSettings == 0) {
            lbl_8087EFA8->m104.fn_800BBD90(&csv);
        } else if (strcmp(key, "GAMMA") == 0 && noSettings == 0) {
            lbl_8087EFA8->m3DC.fn_800BC328(&csv);
        } else if (strcmp(key, "REFLECT") == 0 && noSettings == 0) {
            atn::Scene* scene = atn::RenderManager::instance_->GetScene();
            scene->mReflectChara = 1;
            Plane_unk plane;
            plane.mNormal = atn::Vector3(fn_800DC288(csv.fn_8005B9CC()), fn_800DC288(csv.fn_8005B9CC()),
                                         fn_800DC288(csv.fn_8005B9CC()));
            plane.mDist = fn_800DC288(csv.fn_8005B9CC());
            if (strcmp(csv.fn_8005B9CC(), "OFF") == 0) {
                scene->mReflectChara = 0;
            }
            scene->fn_800C2C20(&plane);
        } else if (strcmp(key, "LIGHT") == 0 && noSettings == 0) {
            if (strcmp(csv.fn_8005B9CC(), "KEY") == 0) {
                float r = fn_800DC288(csv.fn_8005B9CC());
                float g = fn_800DC288(csv.fn_8005B9CC());
                float b = fn_800DC288(csv.fn_8005B9CC());
                float a = fn_800DC288(csv.fn_8005B9CC());
                float x = fn_800DC288(csv.fn_8005B9CC());
                float y = fn_800DC288(csv.fn_8005B9CC());
                float z = fn_800DC288(csv.fn_8005B9CC());
                atn::Scene* scene = atn::RenderManager::instance_->GetScene();
                atn::Vector3 dir(x, y, z);
                scene->GetLight(0)->SetDir_unk(dir);
                scene->GetLight(0)->SetColor(atn::Color(r, g, b, a));
            }
        } else if (strcmp(key, "COLOR_MATRIX") == 0 && noSettings == 0) {
            ColorMatrixParam_unk matrix = lbl_8087EFA8->m324;
            matrix.fn_800BBFE8(&csv);
            lbl_8087EFA8->m324 = matrix;
        } else if (replace != 0 && strcmp(key, "CMD_DELETE") == 0) {
            int type = fn_80496A28(csv.fn_8005B9CC());
            MapPartsBase* part = fn_804A06B4(type, csv.fn_8005B9CC());
            if (part != 0) {
                RemovePart_unk(part);
            }
        } else {
            MapPartsBase* old = 0;
            if (replace != 0) {
                int type = fn_80496A28(key);
                old = fn_804A06B4(type, csv.fn_8005B9CC());
                csv.Reset_unk();
                key = csv.fn_8005B9CC();
            }
            MapPartsBase* part = fn_80496B3C(this, key, &csv);
            if (part != 0) {
                AddPart_unk(part);
                if (part->mType == 6) {
                    if (mAreas.mSize != 0 && mNextArea == 0) {
                        mNextArea = MapPartsArea::fn_80491F94(this, 6, ((MapPartsArea*)part)->mFile.fn_80473F18());
                    }
                    mAreas.push_back((MapPartsArea*)part);
                }
                mFlags &= ~2;
            }
            if (replace != 0 && old != 0) {
                RemovePart_unk(old);
            }
        }
    } while (csv.fn_8005BBF8());
}
// inferred: sets the camera of the map (camera and sub camera of the camera csv m60, mode and box)
void MapTask::fn_804A04AC(int camera, int sub, int mode, gs::CameraBox* box) {
    mCamera = camera;
    if (sub >= 0) {
        mE4 = sub;
    } else {
        sub = mE4;
    }
    GameCamera* gameCamera = &lbl_8087F430->mCamera;
    if (m60.fn_80470580()) {
        gameCamera->fn_8037E12C(m60.fn_80470580(), m60.fn_8047059C(), camera, sub);
    }
    gameCamera->mC20 = box;
    if (mode == 1) {
        gameCamera->fn_80389884(10);
    } else if (mode == 2) {
        gameCamera->fn_80389884(11);
    } else {
        gameCamera->fn_80389884(0);
    }
}
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

// NONMATCHING: 99.91% - string and length registers of the type-0 std::string swapped (as fn_804A0BCC)
// inferred: part of type `type` (models and locator sets) whose file name is the one of `name`
MapPartsBase* MapTask::fn_804A06B4(int type, const char* name) {
    if (type == -1) {
        return 0;
    }
    std::string key = fn_8006B2D8(fn_8006B174(std::string(name)));
    for (MapPartsBase* part = mParts; part != 0; part = part->mNext) {
        if (type == part->mType) {
            if (type == 0) {
                std::string file = fn_8006B2D8(fn_8006B174(std::string(((MapPartsModel*)part)->mModel.fn_8008B130())));
                if (file == key) {
                    return part;
                }
            } else if (type == 4) {
                std::string file = fn_8006B2D8(fn_8006B174(std::string(((MapPartsLocatorSet*)part)->mName.c_str())));
                if (file == key) {
                    return part;
                }
            }
        }
    }
    return 0;
}
// NONMATCHING: 99.80% - string and length registers of the type-0 std::string swapped (as in the
// MapPartsLocatorSet constructor)
// inferred: part of type `type` (models and locator sets) whose file name has the hash `hash`
MapPartsBase* MapTask::fn_804A0BCC(int type, unsigned int hash) {
    if (type == -1) {
        return 0;
    }
    for (MapPartsBase* part = mParts; part != 0; part = part->mNext) {
        if (type == part->mType) {
            if (type == 0) {
                std::string file = fn_8006B2D8(fn_8006B174(std::string(((MapPartsModel*)part)->mModel.fn_8008B130())));
                if (hash == fn_800DC6B4(file.c_str())) {
                    return part;
                }
            } else if (type == 4) {
                std::string file = fn_8006B2D8(fn_8006B174(std::string(((MapPartsLocatorSet*)part)->mName.c_str())));
                if (hash == fn_800DC6B4(file.c_str())) {
                    return part;
                }
            }
        }
    }
    return 0;
}
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
void MapTask::fn_804A0EFC(MapPartsArea* area) {
    for (int i = 0; i < mAreas.mSize; i++) {
        if (mAreas.mData[i] == area) {
            fn_804A0F58(i, 1.0f, 0.0f);
            break;
        }
    }
}

// inferred: switches to the area list `index` with a blend (t, speed)
void MapTask::fn_804A0F58(int index, float t, float speed) {
    if (index < (int)mAreas.mSize) {
        m10C = mAreaIndex;
        mAreaIndex = index;
        m110 = t;
        m114 = speed;
        fn_804A0F80();
    }
}

// NONMATCHING: 5% - over the inlining budget (docs/inlining.md R3): every Lerp_unk, operator and accessor
// is called, while the original inlines them all (with the (b - a), * t and + a temporaries on the stack)
// inferred: blends the scenes of the area list in use (mNextArea) from the previous list (m10C)
// to the current one (mAreaIndex) by m110, then the rain of the areas
void MapTask::fn_804A0F80() {
    MapPartsArea* list = mNextArea;
    if (list == 0) {
        return;
    }
    if (mAreaIndex != m10C) {
        if (m110 >= 1.0f) {
            m110 = 1.0f;
            m10C = mAreaIndex;
        } else {
            for (unsigned int i = 0; i < list->mAreas.size(); i++) {
                const MapArea* from = mAreas.mData[m10C]->mAreas[i];
                const MapArea* to = mAreas.mData[mAreaIndex]->mAreas[i];
                MapArea* area = list->mAreas[i];
                area->mScene.mAmbient = atn::Lerp_unk(from->mScene.mAmbient, to->mScene.mAmbient, m110);
                area->mScene.mLS = atn::Lerp_unk(from->mScene.mLS, to->mScene.mLS, m110);
                area->mScene.GetLight(0)->mDir =
                    atn::Lerp_unk<atn::Vector3>(from->mScene.GetLight(0)->mDir, to->mScene.GetLight(0)->mDir, m110);
                area->mScene.GetLight(0)->SetColor(
                    atn::Lerp_unk(from->mScene.GetLight(0)->mColor, to->mScene.GetLight(0)->mColor, m110));
                area->mScene.mShadow = to->mScene.mShadow;
                area->mScene.mShadow.mDir =
                    atn::Lerp_unk<atn::Vector3>(from->mScene.mShadow.mDir, to->mScene.mShadow.mDir, m110);
                area->mScene.mShadow.mColor = atn::Lerp_unk(from->mScene.mShadow.mColor, to->mScene.mShadow.mColor, m110);
                area->mScene.mGodRay.mValue = to->mScene.mGodRay.mValue;
                area->mScene.mGodRay.mValue.mColor =
                    atn::Lerp_unk(from->mScene.mGodRay.mValue.mColor, to->mScene.mGodRay.mValue.mColor, m110);
                area->mScene.mGodRay.mValue.mPos =
                    atn::Lerp_unk<atn::Vector3>(from->mScene.mGodRay.mValue.mPos, to->mScene.mGodRay.mValue.mPos, m110);
                area->mScene.mGodRay.mValue.mRadius =
                    atn::Lerp_unk(from->mScene.mGodRay.mValue.mRadius, to->mScene.mGodRay.mValue.mRadius, m110);
                area->mScene.mFog = to->mScene.mFog;
                area->mScene.mFog.mStart = atn::Lerp_unk(from->mScene.mFog.mStart, to->mScene.mFog.mStart, m110);
                area->mScene.mFog.mEnd = atn::Lerp_unk(from->mScene.mFog.mEnd, to->mScene.mFog.mEnd, m110);
                area->mScene.mFog.mColor = atn::Lerp_unk(from->mScene.mFog.mColor, to->mScene.mFog.mColor, m110);
                for (int j = 0; j < 4; j++) {
                    area->mScene.fn_8047C7DC()->mRows[j] =
                        atn::Lerp_unk(((MapArea*)from)->mScene.fn_8047C7DC()->mRows[j],
                                      ((MapArea*)to)->mScene.fn_8047C7DC()->mRows[j], m110);
                }
                area->mScene.mRain.mValue.mEnabled =
                    to->mScene.mRain.mValue.mEnabled != 0 || from->mScene.mRain.mValue.mEnabled != 0;
                area->mScene.mRain.mValue.mRate =
                    atn::Lerp_unk(from->mScene.mRain.mValue.mRate, to->mScene.mRain.mValue.mRate, m110);
                area->mScene.mRain.mValue.mWarp =
                    atn::Lerp_unk(from->mScene.mRain.mValue.mWarp, to->mScene.mRain.mValue.mWarp, m110);
                area->mScene.mHaze.mValue.mEnabled =
                    to->mScene.mHaze.mValue.mEnabled != 0 || from->mScene.mHaze.mValue.mEnabled != 0;
                area->mScene.mHaze.mValue.mWarp =
                    atn::Lerp_unk(from->mScene.mHaze.mValue.mWarp, to->mScene.mHaze.mValue.mWarp, m110);
                area->mScene.mHaze.mValue.mScroll =
                    atn::Lerp_unk(from->mScene.mHaze.mValue.mScroll, to->mScene.mHaze.mValue.mScroll, m110);
                area->mScene.mHaze.mValue.mScale =
                    atn::Lerp_unk(from->mScene.mHaze.mValue.mScale, to->mScene.mHaze.mValue.mScale, m110);
                area->mScene.mHaze.mValue.mPos =
                    atn::Lerp_unk(from->mScene.mHaze.mValue.mPos, to->mScene.mHaze.mValue.mPos, m110);
                area->mScene.mHaze.mValue.mSize =
                    atn::Lerp_unk(from->mScene.mHaze.mValue.mSize, to->mScene.mHaze.mValue.mSize, m110);
                area->mScene.mHaze.mValue.mFade =
                    atn::Lerp_unk(from->mScene.mHaze.mValue.mFade, to->mScene.mHaze.mValue.mFade, m110);
                for (unsigned int k = 0; k < area->mMatColors.size(); k++) {
                    area->mMatColors[k].mColor =
                        atn::Lerp_unk(from->mMatColors[k].mColor, to->mMatColors[k].mColor, m110);
                }
            }
            m110 += m114;
        }
        fn_8049E480();
    }
    for (unsigned int i = 0; i < mNextArea->mAreas.size(); i++) {
        if (mNextArea->mAreas[i]->mRainPart != 0) {
            mNextArea->mAreas[i]->mRainPart->fn_8049D1A4(mAreas.mData[m10C]->mAreas[i]->mRainPart,
                                                         mAreas.mData[mAreaIndex]->mAreas[i]->mRainPart, m110);
        }
    }
}
// inferred: restores the area list, its blend and the reflection from the game flags (after a load)
void MapTask::fn_804A1C4C() {
    if (lbl_8087F430 != 0) {
        int index = lbl_8087F430->fn_80370174(0x387);
        int blend = lbl_8087F430->fn_80370174(0x388);
        float t = ((blend >> 16) & 0xFFFF) / 10000.0f;
        float speed = (blend & 0xFFFF) / 10000.0f;
        fn_804A0F58(index, t, speed);
        atn::RenderManager::instance_->mLayers[4]->m04 = lbl_8087F430->fn_80370174(0x38B);
    }
}

// inferred: stores the area list, its blend and the reflection in the game flags (before a save)
void MapTask::fn_804A1D2C() {
    if (lbl_8087F430 != 0) {
        unsigned int blend = ((unsigned int)(m110 * 10000.0f) << 16) | (int)(m114 * 10000.0f);
        lbl_8087F430->fn_80370320(0x387, mAreaIndex, 0);
        lbl_8087F430->fn_80370320(0x388, blend, 0);
        lbl_8087F430->fn_80370320(0x38B, atn::RenderManager::instance_->mLayers[4]->m04, 0);
    }
}
