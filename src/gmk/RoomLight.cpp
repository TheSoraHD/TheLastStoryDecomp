#include "gmk/RoomLight.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include <boost/function.hpp>
#include "game/MapManager.h"
#include "atn/RenderView_unk.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" char lbl_8087D710[2]; // inferred: " " (field separators of the tab CSV)
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" float PSVECSquareMag(const atn::Vector3* v); // inferred: squared length
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
extern "C" void fn_800C18BC(CharaLightData_unk* light); // inferred: updates the light
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_80087E9C(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void fn_80088724(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
extern "C" void fn_80087BB4(void* group, const char* name, atn::Vector2_unk* value, int arg3, int arg4, float min,
                            float max, float step);

namespace atn {
struct Camera;
}
typedef atn::Camera Camera_unk;

// inferred: sets the light of a map part (callback of fn_80491528)
void fn_8041FED8(MapArea* part, void* light) {
    part->m074 = light;
    part->m070 = light != 0 || part->m070 != 0;
    part->m0C8 = ((RoomLightData_unk*)light)->m50;
}

namespace gmk {

GimmickCommon* RoomLight::fn_8041FF08(atn::Task* parent, int id, int arg) {
    if (parent != 0) {
        return new ((MemoryTag)12, "", "", 0) RoomLight(parent, id, arg);
    }
    return 0;
}

RoomLight::~RoomLight() {
    lbl_8087EFB4->fn_800C16B4()->SetLight_unk(0);
}

int RoomLight::vf0C() {
    if (fn_803EC91C() != 0) {
        m54 = 1;
        return 0;
    }
    return 1;
}

void RoomLight::Start() {
    lbl_8087EFB4->fn_800C16B4()->SetLight_unk(&mLight);
    lbl_8087EFB4->fn_800C16B4()->SetLightIndex_unk(mLight.m50);
    MapManager::sInstance->fn_80491528(&fn_8041FED8, &mLight);
    if (m58 == 0) {
        mFrame = 0;
    } else {
        mFrame = mFrames;
    }
    GimmickMsg_unk msg(m58 + 1);
    vf40(&msg);
}

void RoomLight::vf10() {
    if (m54 == 0) {
        Start();
    }
    float t = 0.0f;
    atn::Vector2_unk size(0.0f, 0.0f);
    if (m54 == 1) {
        if (mFrame > 0) {
            mFrame--;
        }
        t = mFrames > 0.0f ? mFrame / mFrames : 0.0f;

        size = mDefSize;
    } else if (m54 == 2) {
        if (mFrame < mFrames) {
            mFrame++;
        }
        if (mFrames > 0.0f) {
            t = mFrame / mFrames;
        } else {
            t = 1.0f;
        }
        size = mDefSize;
    }
    mLight.m04.x = m6C.x;
    mLight.m04.y = m6C.z;
    mLight.mSize = Lerp2_unk(size, mMinSize, t);
    fn_800C18BC(&mLight);
}

void RoomLight::vf14() {}

int RoomLight::vf1C() {
    return mLight.fn_800C1990();
}

void RoomLight::vf20() {
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    csv.fn_8005BCE8(lbl_8087D710);
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "TEXTURE") == 0) {
            mLight.fn_800C1814(csv.fn_8005B9CC());
            if (strcmp(csv.fn_8005B9CC(), "WRAP_RELEAT") == 0) {
                atn::Texture_unk* texture = mLight.mTextures.mData;
                texture->m2D = 1;
                texture->m2E = 1;
            }
        } else if (strcmp(field, "PARAM") == 0) {
            mFrames = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(field, "SIZE") == 0) {
            mMinSize.x = fn_800DC288(csv.fn_8005B9CC());
            mMinSize.y = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(field, "DEF_SIZE") == 0) {
            mDefSize.x = fn_800DC288(csv.fn_8005B9CC());
            mDefSize.y = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(field, "MUL") == 0) {
            mLight.m00 = 0;
        } else if (strcmp(field, "MUL2") == 0) {
            mLight.m00 = 1;
        } else if (strcmp(field, "MUL4") == 0) {
            mLight.m00 = 2;
        } else if (strcmp(field, "ADD") == 0) {
            mLight.m00 = 3;
        } else if (strcmp(field, "ADDBIAS") == 0) {
            mLight.m00 = 4;
        } else if (strcmp(field, "SUB") == 0) {
            mLight.m00 = 5;
        } else if (strcmp(field, "SUBBIAS") == 0) {
            mLight.m00 = 6;
        } else if (strcmp(field, "DIRECTION") == 0) {
            float x = fn_800DC288(csv.fn_8005B9CC());
            float y = fn_800DC288(csv.fn_8005B9CC());
            float z = fn_800DC288(csv.fn_8005B9CC());
            atn::Vector3 dir(x, y, z);
            if (PSVECSquareMag(&dir) > 0.0001f) {
                mLight.mDir = atn::Vector3(x, y, z).GetNormalize();
            } else {
                mLight.mDir = lbl_807C7030;
            }
        } else if (strcmp(field, "ROT") == 0) {
            mLight.m1C = fn_800DC288(csv.fn_8005B9CC());
            mLight.m20 = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(field, "SPEED") == 0) {
            float x = fn_800DC288(csv.fn_8005B9CC());
            mLight.m14 = atn::Vector2_unk(x, fn_800DC288(csv.fn_8005B9CC()));
        } else if (strcmp(field, "BACKGROUND") == 0) {
            mLight.m50 = 2;
        }
    } while (csv.fn_8005BBF8() != 0);
}

int RoomLight::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    vf70(msg->mType);
    return m54;
}

void RoomLight::vf6C(void* menu) {
    char name[0x20];
    fn_803EC758(name);
    void* group;
    if (mF0.mRes == 0 && menu != 0 && lbl_8087EF10 != 0) {
        group = mF0.mRes = fn_8008937C(menu, name);
    } else {
        group = 0;
    }
    fn_800874C8(group, "state", &m54, 0, 99, 1, 0, 0);
    fn_80087E9C(group, "pos", &m6C, 0, 0, -9999.0f, 9999.0f, 0.25f);
    fn_80088724(group, "rot", &m78, 0, 0, -3.1415927f, 3.1415927f, 0.05235988f);
    fn_80087E9C(group, "scl", &m90, 0, 0, -9999.0f, 9999.0f, 0.25f);
    fn_80087994(group, "isShow", &m9C, 0, 0);
    fn_800874C8(group, "locData", &m58, 0, 99, 1, 0, 0);
    mB0.fn_803F11F8(group);
    fn_80087BB4(group, "MinSize", &mMinSize, 0, 0, -9999.0f, 9999.0f, 0.25f);
    fn_80087BB4(group, "MaxSize", &mMinSize, 0, 0, -9999.0f, 9999.0f, 0.25f);
}

} // namespace gmk
