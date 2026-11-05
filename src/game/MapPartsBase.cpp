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
#include "atn/SceneAddLight_unk.h"
#include "atn/Lerp_unk.h"
#include "atn/Plane_unk.h"
#include "atn/SceneColorMatrix_unk.h"

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

extern "C" int fn_800DC12C(const char* text); // inferred: atoi
extern "C" char* strcpy(char* dst, const char* src);
extern "C" std::string fn_8006AC08(const std::string& path); // inferred: extension of a path
extern "C" void PSVECNormalize(const atn::Vector3* src, atn::Vector3* dst);
extern "C" float PSVECSquareMag(const atn::Vector3* v);

// NONMATCHING: ParseScene is only written for the order of the string pool (vf1C and fn_80494354 come after
// it): the inlined AddLight_unk (std::vector growth) does not match (known blocker)
// inferred: reads one scene setting of an area (the key is the current field of the csv): ambient colour, floor
// light (COVERAGEMAP), character light (CLOUDMAP), fog, lights, reflection, god ray, colour matrix, rain, haze
// and shadow of the area (RSO name)
void MapPartsArea::ParseScene(atn::Scene* scene, atn::CsvTab& csv) {
    const char* key = csv.GetField_unk();
    if (strcmp(key, "AMBIENT_COLOR") == 0) {
        scene->mAmbient.r = fn_800DC288(csv.fn_8005B9CC());
        scene->mAmbient.g = fn_800DC288(csv.fn_8005B9CC());
        scene->mAmbient.b = fn_800DC288(csv.fn_8005B9CC());
        scene->mAmbient.a = fn_800DC288(csv.fn_8005B9CC());
    } else if (strcmp(key, "COVERAGEMAP") == 0) {
        const char* value = csv.fn_8005B9CC();
        atn::Vector2_unk pos;
        atn::Vector2_unk size;
        if (strcmp(value, "AREA") != 0) {
            atn::Vector2_unk min;
            atn::Vector2_unk max;
            min.x = fn_800DC288(value);
            min.y = fn_800DC288(csv.fn_8005B9CC());
            max.x = fn_800DC288(csv.fn_8005B9CC());
            max.y = fn_800DC288(csv.fn_8005B9CC());
            pos = (min + max) * 0.5f;
            size = max - min;
        }
        std::string path(csv.fn_8005B9CC());
        if (fn_8006AC08(path) == "") {
            path += ".texture";
        }
        scene->fn_800C2B24(&pos, &size, path.c_str());
    } else if (strcmp(key, "CLOUDMAP") == 0) {
        atn::Vector2_unk speed(0.5f, 0.5f);
        atn::Vector3 pos(0.0f, 0.0f, 0.0f);
        const char* value = csv.fn_8005B9CC();
        if (strcmp(value, "USEMAP") == 0) {
            scene->SetLight_unk(atn::RenderManager::instance_->fn_800C16B4()->GetLight_unk());
        } else if (strcmp(value, "DISABLE") != 0) {
            scene->mLightOn = 1;
            float width = fn_800DC288(value);
            float height = fn_800DC288(csv.fn_8005B9CC());
            char texture[0x100];
            strcpy(texture, csv.fn_8005B9CC());
            for (const char* option = csv.fn_8005B9CC(); strcmp(option, "") != 0; option = csv.fn_8005B9CC()) {
                if (strcmp(option, "BACKGROUND") == 0) {
                    scene->mLightMask = 2;
                } else if (strcmp(option, "BACKGROUND_CHARACTER") == 0) {
                    scene->mLightMask = 3;
                } else if (strcmp(option, "CHARACTER") == 0) {
                    scene->mLightMask = 1;
                } else if (strcmp(option, "MUL") == 0) {
                    scene->GetLight_unk()->m00 = 0;
                } else if (strcmp(option, "MUL2") == 0) {
                    scene->GetLight_unk()->m00 = 1;
                } else if (strcmp(option, "MUL4") == 0) {
                    scene->GetLight_unk()->m00 = 2;
                } else if (strcmp(option, "ADD") == 0) {
                    scene->GetLight_unk()->m00 = 3;
                } else if (strcmp(option, "ADDBIAS") == 0) {
                    scene->GetLight_unk()->m00 = 4;
                } else if (strcmp(option, "SUB") == 0) {
                    scene->GetLight_unk()->m00 = 5;
                } else if (strcmp(option, "SUBBIAS") == 0) {
                    scene->GetLight_unk()->m00 = 6;
                } else if (strcmp(option, "DIRECTION") == 0) {
                    float x = fn_800DC288(csv.fn_8005B9CC());
                    float y = fn_800DC288(csv.fn_8005B9CC());
                    float z = fn_800DC288(csv.fn_8005B9CC());
                    atn::Vector3 dir(x, y, z);
                    if (PSVECSquareMag(&dir) > 0.0001f) {
                        scene->GetLight_unk()->mDir = atn::Vector3(x, y, z).GetNormalize();
                    } else {
                        scene->GetLight_unk()->mDir = atn::Vector3(0.0f, 0.0f, 0.0f);
                    }
                } else if (strcmp(option, "SPEED") == 0) {
                    float x = fn_800DC288(csv.fn_8005B9CC());
                    speed = atn::Vector2_unk(x, fn_800DC288(csv.fn_8005B9CC()));
                } else if (strcmp(option, "ROT") == 0) {
                    scene->GetLight_unk()->m1C = fn_800DC288(csv.fn_8005B9CC());
                    scene->GetLight_unk()->m20 = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(option, "POSITION") == 0) {
                    pos.x = fn_800DC288(csv.fn_8005B9CC());
                    pos.y = fn_800DC288(csv.fn_8005B9CC());
                    pos.z = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(option, "ANIMATION") == 0) {
                    int count = fn_800DC12C(csv.fn_8005B9CC());
                    int frames = fn_800DC12C(csv.fn_8005B9CC());
                    scene->fn_800C2634(count);
                    scene->GetLight_unk()->m48 = frames;
                    for (int i = 0; i < count; i++) {
                        const char* path = csv.fn_8005B9CC();
                        scene->GetLight_unk()->mTextures.mData[i].fn_800D5908(path);
                    }
                }
            }
            if (scene->GetLight_unk() != 0) {
                if (scene->GetLight_unk()->mTextures.mSize == 0) {
                    scene->fn_800C289C(texture);
                }
                scene->GetLight_unk()->m04 = atn::Vector2_unk(0.0f, 0.0f);
                scene->GetLight_unk()->mSize = atn::Vector2_unk(width, height);
                scene->GetLight_unk()->m14 = speed;
                scene->GetLight_unk()->mPos = pos;
            }
        }
    } else if (strcmp(key, "FOG") == 0) {
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
    } else if (strcmp(key, "LIGHT") == 0) {
        const char* type = csv.fn_8005B9CC();
        if (strcmp(type, "POINT") == 0) {
            float r = fn_800DC288(csv.fn_8005B9CC());
            float g = fn_800DC288(csv.fn_8005B9CC());
            float b = fn_800DC288(csv.fn_8005B9CC());
            float a = fn_800DC288(csv.fn_8005B9CC());
            float x = fn_800DC288(csv.fn_8005B9CC());
            float y = fn_800DC288(csv.fn_8005B9CC());
            float z = fn_800DC288(csv.fn_8005B9CC());
            float radius = fn_800DC288(csv.fn_8005B9CC());
            atn::Light light;
            light.fn_800791B4(atn::Vector3(x, y, z), atn::Color(r, g, b, a), radius);
            scene->AddLight_unk(light);
        } else if (strcmp(type, "SPOT") == 0) {
            float r = fn_800DC288(csv.fn_8005B9CC());
            float g = fn_800DC288(csv.fn_8005B9CC());
            float b = fn_800DC288(csv.fn_8005B9CC());
            float a = fn_800DC288(csv.fn_8005B9CC());
            float x = fn_800DC288(csv.fn_8005B9CC());
            float y = fn_800DC288(csv.fn_8005B9CC());
            float z = fn_800DC288(csv.fn_8005B9CC());
            float dx = fn_800DC288(csv.fn_8005B9CC());
            float dy = fn_800DC288(csv.fn_8005B9CC());
            float dz = fn_800DC288(csv.fn_8005B9CC());
            float radius = fn_800DC288(csv.fn_8005B9CC());
            float angle = fn_800DC288(csv.fn_8005B9CC());
            atn::Light light;
            light.fn_80079210(atn::Vector3(x, y, z), atn::Vector3(dx, dy, dz), atn::Color(r, g, b, a), radius, angle);
            scene->AddLight_unk(light);
        } else if (strcmp(type, "KEY") == 0) {
            float r = fn_800DC288(csv.fn_8005B9CC());
            float g = fn_800DC288(csv.fn_8005B9CC());
            float b = fn_800DC288(csv.fn_8005B9CC());
            float a = fn_800DC288(csv.fn_8005B9CC());
            float x = fn_800DC288(csv.fn_8005B9CC());
            float y = fn_800DC288(csv.fn_8005B9CC());
            float z = fn_800DC288(csv.fn_8005B9CC());
            atn::Vector3 dir(x, y, z);
            scene->GetLight(0)->SetDir_unk(dir);
            scene->GetLight(0)->SetColor(atn::Color(r, g, b, a));
        }
    } else if (strcmp(key, "LIGHTSURROUND") == 0) {
        scene->mLS.r = fn_800DC288(csv.fn_8005B9CC());
        scene->mLS.g = fn_800DC288(csv.fn_8005B9CC());
        scene->mLS.b = fn_800DC288(csv.fn_8005B9CC());
        scene->mLS.a = fn_800DC288(csv.fn_8005B9CC());
    } else if (strcmp(key, "REFLECT") == 0) {
        scene->mReflectChara = 1;
        float x = fn_800DC288(csv.fn_8005B9CC());
        float y = fn_800DC288(csv.fn_8005B9CC());
        float z = fn_800DC288(csv.fn_8005B9CC());
        Plane_unk plane;
        plane.mNormal = atn::Vector3(x, y, z);
        plane.mDist = fn_800DC288(csv.fn_8005B9CC());
        if (strcmp(csv.fn_8005B9CC(), "OFF") == 0) {
            scene->mReflectChara = 0;
        }
        scene->fn_800C2C20(&plane);
    } else if (strcmp(key, "GODRAY") == 0) {
        GodRayParam_unk godRay;
        godRay.fn_800BB6C0(&csv);
        scene->mGodRay.Set_unk(godRay);
    } else if (strcmp(key, "COLOR_MATRIX") == 0) {
        ColorMatrixParam_unk matrix;
        matrix.fn_800BBFE8(&csv);
        scene->mColorMatrix.Set_unk(matrix);
    } else if (strcmp(key, "RAIN") == 0) {
        RainParam_unk rain;
        rain.fn_800BB994(&csv);
        scene->mRain.Set_unk(rain);
    } else if (strcmp(key, "HAZE") == 0) {
        HazeParam_unk haze;
        haze.fn_800BBBB0(&csv);
        scene->mHaze.Set_unk(haze);
    } else if (strcmp(key, "SHADOW") == 0) {
        ShadowParam_unk shadow;
        shadow.fn_800BBD90(&csv);
        scene->mShadowSet = 1;
        scene->mShadow = shadow;
    }
}

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

// inferred: debug menu items (atn/MindowsCommon.h; folders are void* as in the other users)
extern "C" void* fn_8008937C(void* folder, const char* name);
extern "C" void fn_800874C8(void* folder, const char* name, int* value, int min, int max, int step, void* func,
                            void* arg);
extern "C" void fn_8008771C(void* folder, const char* name, float* value, float min, float max, float step,
                            void* func, void* arg);
extern "C" void fn_80087E9C(void* folder, const char* name, float* value, float min, float max, float step,
                            void* func, void* arg); // inferred: x, y, z
extern "C" int sprintf(char* buf, const char* format, ...);

// inferred: debug menu of the areas: a folder per area with its scene settings, boxes and lights
// NONMATCHING: 99.90% - mAreas[i] in the box/light loop conditions is added as offset+base (original base+offset).
void MapPartsArea::vf1C(void* menu) {
    for (int i = 0; i < mAreas.size(); i++) {
        void* folder = fn_8008937C(menu, mAreas[i]->mName);
        fn_8008771C(folder, "Ambient R", &mAreas[i]->mScene.mAmbient.r, 0.0f, 1.0f, 0.01f, 0, 0);
        fn_8008771C(folder, "Ambient G", &mAreas[i]->mScene.mAmbient.g, 0.0f, 1.0f, 0.01f, 0, 0);
        fn_8008771C(folder, "Ambient B", &mAreas[i]->mScene.mAmbient.b, 0.0f, 1.0f, 0.01f, 0, 0);
        fn_8008771C(folder, "LS R", &mAreas[i]->mScene.mLS.r, 0.0f, 1.0f, 0.01f, 0, 0);
        fn_8008771C(folder, "LS G", &mAreas[i]->mScene.mLS.g, 0.0f, 1.0f, 0.01f, 0, 0);
        fn_8008771C(folder, "LS B", &mAreas[i]->mScene.mLS.b, 0.0f, 1.0f, 0.01f, 0, 0);
        void* fog = fn_8008937C(folder, "Fog");
        fn_800874C8(fog, "Fog Mode", &mAreas[i]->mScene.mFog.mMode, 0, 6, 1, 0, 0);
        fn_8008771C(fog, "Fog Start", &mAreas[i]->mScene.mFog.mStart, 0.0f, 10000.0f, 1.0f, 0, 0);
        fn_8008771C(fog, "Fog End", &mAreas[i]->mScene.mFog.mEnd, 0.0f, 10000.0f, 1.0f, 0, 0);
        fn_8008771C(fog, "Fog R", &mAreas[i]->mScene.mFog.mColor.r, 0.0f, 1.0f, 0.01f, 0, 0);
        fn_8008771C(fog, "Fog G", &mAreas[i]->mScene.mFog.mColor.g, 0.0f, 1.0f, 0.01f, 0, 0);
        fn_8008771C(fog, "Fog B", &mAreas[i]->mScene.mFog.mColor.b, 0.0f, 1.0f, 0.01f, 0, 0);
        if (mAreas[i]->mScene.mGodRay.mSet != 0) {
            void* godRay = fn_8008937C(folder, "GodRay");
            mAreas[i]->mScene.fn_8047C780()->fn_800BB4FC(godRay);
        }
        if (mAreas[i]->mScene.mColorMatrix.mSet != 0) {
            void* matrix = fn_8008937C(folder, "ColorMatrix");
            mAreas[i]->mScene.fn_8047C7DC()->fn_800BC194(matrix);
        }
        MapArea* area = mAreas[i];
        if (area->mScene.mRain.mSet != 0) {
            area->mScene.mRain.mValue.fn_800BB8D8(fn_8008937C(folder, "Rain"));
        }
        if (mAreas[i]->mRainPart != 0) {
            mAreas[i]->mRainPart->vf1C(folder);
        }
        area = mAreas[i];
        if (area->mScene.mHaze.mSet != 0) {
            area->mScene.mHaze.mValue.fn_800BBA64(fn_8008937C(folder, "Haze"));
        }
        if (mAreas[i]->mScene.mShadowSet != 0) {
            char name[0x100];
            strcpy(name, "Shadows");
            void* shadows = fn_8008937C(folder, name);
            mAreas[i]->mScene.fn_8047C7D4()->fn_800BBEFC(shadows);
        }
        if (mAreas[i]->mScene.GetFloorLight_unk() != 0) {
            CharaLightData_unk* floor = mAreas[i]->mScene.GetFloorLight_unk();
            char name[0x100];
            strcpy(name, "Coverage");
            void* coverage = fn_8008937C(folder, name);
            fn_8008771C(coverage, "Size X", &floor->mSize.x, 1.0f, 10000.0f, 1.0f, 0, 0);
            fn_8008771C(coverage, "Size Y", &floor->mSize.y, 1.0f, 10000.0f, 1.0f, 0, 0);
            fn_8008771C(coverage, "Center X", &floor->m04.x, -100000.0f, 100000.0f, 1.0f, 0, 0);
            fn_8008771C(coverage, "Center Y", &floor->m04.y, -100000.0f, 100000.0f, 1.0f, 0, 0);
        }
        if (mAreas[i]->mScene.GetLight_unk() != 0 && mAreas[i]->mScene.m074 == 0) {
            void* clouds = fn_8008937C(folder, "Clouds");
            CharaLightData_unk* light = mAreas[i]->mScene.GetLight_unk();
            fn_8008771C(clouds, "Size X", &light->mSize.x, 1.0f, 10000.0f, 1.0f, 0, 0);
            fn_8008771C(clouds, "Size Y", &light->mSize.y, 1.0f, 10000.0f, 1.0f, 0, 0);
            fn_8008771C(clouds, "Speed X", &light->m14.x, -10000.0f, 10000.0f, 1.0f, 0, 0);
            fn_8008771C(clouds, "Speed  Y", &light->m14.y, -10000.0f, 10000.0f, 1.0f, 0, 0);
            fn_80087E9C(clouds, "Direction", &light->mDir.x, -10000.0f, 10000.0f, 0.01f, 0, 0);
            fn_800874C8(clouds, "BlendMode", &light->m00, 0, 6, 1, 0, 0);
            fn_800874C8(clouds, "Frame Rate", &light->m48, 0, 1000, 1, 0, 0);
        }
        for (unsigned int j = 0; j < mAreas[i]->mSegments.size(); j++) {
            atn::AABB_unk& box = mAreas[i]->mSegments[j];
            fn_80087E9C(folder, "Box Min", &box.mMin.x, -1000000.0f, 1000000.0f, 1.0f, 0, 0);
            fn_80087E9C(folder, "Box Max", &box.mMax.x, -1000000.0f, 1000000.0f, 1.0f, 0, 0);
        }
        for (int j = 0; j < mAreas[i]->mScene.fn_800C23E8(); j++) {
            char name[0x100];
            sprintf(name, "Light %d", j);
            void* lightFolder = fn_8008937C(folder, name);
            mAreas[i]->mScene.GetLight(j)->fn_80079BC8(lightFolder);
        }
        mAreas[i]->mScene.fn_800C2E30(folder);
    }
}

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
