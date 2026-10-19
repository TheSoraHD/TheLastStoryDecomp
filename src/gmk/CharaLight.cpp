#include "gmk/CharaLight.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "game/EnemyManager.h"
#include "game/NpcManager.h"
#include "game/ResHandle.h"
#include <boost/function.hpp>
#include "game/MapManager.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" int sprintf(char* s, const char* format, ...);
extern "C" char lbl_8087D710[2]; // inferred: " " (field separators of the tab CSV)
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" int fn_800DC12C(const char* s);   // inferred: string to int
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
// inferred: name invented (the id is loaded before the manager)
inline PlayerTask* FindPlayer_unk(int id) {
    return lbl_8087F8A0->Find_unk(id);
}

// inferred: light settings of the camera (returned by fn_800C16B4)
struct LightTarget_unk {
    // inferred: name invented
    void SetLight_unk(CharaLightData_unk* light) {
        m74 = light;
        m70 = m74 != 0 || m70 != 0;
    }
    unsigned char m00[0x70];
    int m70;
    CharaLightData_unk* m74;
    unsigned char m78[0xC8 - 0x78];
    int mC8;
};
struct Camera_unk;
extern "C" Camera_unk* lbl_8087EFB4;
extern "C" LightTarget_unk* fn_800C16B4(Camera_unk* camera);

// inferred: sets the light of a map part (callback of fn_80491528). It is at 0x804225E0, just
// before the CharaLight split (the split will have to start there when the unit is linked).
void fn_804225E0(MapModel_unk* part, void* light) {
    part->mC0 = light;
    part->mBC = light != 0 || part->mBC != 0;
    part->m114 = -1;
}

namespace gmk {

GimmickCommon* CharaLight::fn_80422610(atn::Task* parent, GimmickDesc_unk* desc) {
    if (parent != 0) {
        return new (12, "", "", 0) CharaLight(parent, desc);
    }
    return 0;
}

CharaLight::CharaLight(atn::Task* parent, GimmickDesc_unk* desc)
    : GimmickCommon(parent, desc->mId, desc->mArg), mDesc(desc), mTarget(0), m14C(0), m150(0.0f),
      mStartSize(0.0f, 0.0f), mCurSize(0.0f, 0.0f) {
    mLight.m04 = atn::Vector2_unk(0.0f, 0.0f);
    mLight.mSize = atn::Vector2_unk(0.0f, 0.0f);
    m54 = 0;
    mLight.mDir = atn::Vector3(0.0f, -1.0f, 0.0f);
    mLight.m00 = 0;
}

CharaLight::~CharaLight() {
    fn_800C16B4(lbl_8087EFB4)->SetLight_unk(0);
    if (MapManager::sInstance != 0) {
        MapManager::sInstance->fn_80491528(&fn_804225E0, 0);
    }
}

int CharaLight::vf0C() {
    if (fn_803EC91C() != 0) {
        m54 = 0;
        return 0;
    }
    return 1;
}

void CharaLight::vf28() {
    fn_800C16B4(lbl_8087EFB4)->SetLight_unk(&mLight);
    LightTarget_unk* target = fn_800C16B4(lbl_8087EFB4);
    target->mC8 = -1;
    MapManager::sInstance->fn_80491528(&fn_804225E0, &mLight);
    switch (mDesc->m20) {
    case 0:
        mTarget = lbl_8087F8A0->mHead;
        break;
    case 1:
        mTarget = NpcManager::sInstance->Find_unk(mDesc->m24);
        break;
    case 2:
        mTarget = EnemyManager::sInstance->Find_unk(mDesc->m24);
        break;
    case 3:
        mTarget = FindPlayer_unk(mDesc->m24);
        break;
    }
    m6C = mDesc->mPos;
    m78 = atn::Vector3(0.0f, mDesc->mRotY, 0.0f);
    mLight.m04.x = m6C.x;
    mLight.m04.y = m6C.z;
    GimmickMsg_unk msg(1);
    vf40(&msg);
}

void CharaLight::vf14() {}

int CharaLight::vf1C() {
    return mLight.fn_800C1990();
}

// NONMATCHING: the target tests index >= 0 and index < 5 with two signed compares; this merges
// them into one unsigned compare. Tried: nested ifs, negated tests, if/else, inline helper.
void CharaLight::vf20() {
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    csv.fn_8005BCE8(lbl_8087D710);
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "TEXTURE") == 0) {
            mLight.fn_800C1814(csv.fn_8005B9CC());
        } else if (strcmp(field, "PARAM") == 0) {
            int index = fn_800DC12C(csv.fn_8005B9CC()) - 2;
            if (index >= 0 && index < 5) {
                mParams[index].m00 = fn_800DC288(csv.fn_8005B9CC());
                mParams[index].mSize.x = fn_800DC288(csv.fn_8005B9CC());
                mParams[index].mSize.y = fn_800DC288(csv.fn_8005B9CC());
            }
        }
    } while (csv.fn_8005BBF8() != 0);
}

int CharaLight::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    if (m54 == msg->mType) {
        return 0;
    }
    vf70(msg->mType);
    return m54;
}

void CharaLight::vf70(int state) {
    m14C = 0;
    mStartSize = mLight.mSize;
    if (state >= 2) {
        mCurSize = mParams[state - 2].mSize;
    }
    m54 = state;
}

int CharaLight::vf60() {
    int ret = 0;
    if (m54 >= 2 && mDesc->m7C != 0) {
        ret = 1;
    }
    return ret;
}

// NONMATCHING: scheduling only (the string pool address is built in r3 before `group` is moved).
void CharaLight::vf6C(void* menu) {
    char name[0x20];
    fn_803EC758(name);
    void* group;
    if (mF0.mRes == 0 && menu != 0 && lbl_8087EF10 != 0) {
        group = mF0.mRes = fn_8008937C(menu, name);
    } else {
        group = 0;
    }
    fn_800874C8(group, "state", &m54, 0, 99, 1, 0, 0);
    void* current = fn_8008937C(group, "CurrentParam");
    fn_8008771C(current, "SizeX", &mCurSize.x, 0, 0, 0.0f, 10000.0f, 1.0f);
    fn_8008771C(current, "SizeZ", &mCurSize.y, 0, 0, 0.0f, 10000.0f, 1.0f);
    for (int i = 0; i < 5; i++) {
        sprintf(name, "StateParam:%d", i + 2);
        void* sub = fn_8008937C(group, name);
        fn_8008771C(sub, "SizeX", &mParams[i].mSize.x, 0, 0, 0.0f, 10000.0f, 1.0f);
        fn_8008771C(sub, "SizeZ", &mParams[i].mSize.y, 0, 0, 0.0f, 10000.0f, 1.0f);
    }
}

} // namespace gmk
