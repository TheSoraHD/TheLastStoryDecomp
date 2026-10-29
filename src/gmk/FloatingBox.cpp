#include "gmk/FloatingBox.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "game/CharaManager.h"
#include "game/CharaTask.h"
#include "game/GimmickManager.h"
#include "game/PlayerTask.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" double sin(double x);
extern "C" char lbl_8087D710[2]; // inferred: " " (field separators of the tab CSV)
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" void fn_8008AD4C(atn::Model* model, const char* path, const char* texture); // inferred: loads a model
extern "C" void fn_80092F1C(atn::Model* model, const char* path, int arg, float lod); // inferred: adds a LOD model
extern "C" int fn_802180A8(const char* s); // inferred: motion id from its name
extern "C" void fn_80097A88(atn::Model* model, int id, const char* path); // inferred: loads a motion
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" int fn_80051CD8(const CharaTask::Sphere_unk* sphere, const void* box); // inferred: sphere in box
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_80087E9C(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void fn_80088724(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
// inferred: game settings
struct Settings_unk {
    unsigned char m00[0x18];
    int m18;
};
extern "C" Settings_unk* lbl_8087F0A8;

// inferred: bounding box of the model resource (atn::Model::m16C); names invented
struct ModelBounds_unk {
    unsigned char m00[0x10];
    atn::Vector3 mMin; // 0x10
    atn::Vector3 mMax; // 0x1C
};
struct Box_unk {
    Box_unk(atn::Vector3 a, atn::Vector3 b) : mMin(a), mMax(b) {}
    atn::Vector3 mMin;
    atn::Vector3 mMax;
};
inline float AbsF_unk(float x) {
    if (x < 0.0f) {
        x = -x;
    }
    return x;
}
// inferred: link of the character lists of the managers (CharaTask + 0x14AC)
struct CharaListNode_unk {
    unsigned char m0000[0x14AC];
    CharaTask* mNext;
};

namespace gmk {

GimmickCommon* FloatingBox::fn_8042C21C(atn::Task* parent, int id, int arg) {
    if (parent != 0) {
        return new (12, "", "", 0) FloatingBox(parent, id, arg);
    }
    return 0;
}

FloatingBox::~FloatingBox() {}

// NONMATCHING: the original copies the corners of the bounds twice straight from the resource (one
// load feeding both stores, see vf10 too); here the second copy reads the first. Tried: by-value and
// by-reference box constructors, plain structs, temporaries.
int FloatingBox::vf0C() {
    if (fn_803EC91C() == 0) {
        ModelBounds_unk* res = *(ModelBounds_unk**)((char*)&mModel + 0x16C);
        Box_unk box(res->mMin, res->mMax);
        atn::Vector3 size(AbsF_unk(box.mMax.x - box.mMin.x) * 0.5f, AbsF_unk(box.mMax.y - box.mMin.y) * 0.5f,
                          AbsF_unk(box.mMax.z - box.mMin.z) * 0.5f);
        fn_803EDB18(&mModel);
        mColli.m0C = (int)this;
        mColli.fn_800575BC(&m6C, &m78, &size);
        return 1;
    }
    return 0;
}

void FloatingBox::Start() {
    vf2C();
    if (vf38() != 0) {
        fn_803ED0D4(vf38(), 1);
    }
    mColli.mFlags |= 1;
    mModel.fn_80097C08(0, 0, 0, 1, 1, 0.0f, 0.1f);
    mModel.mTracks[0].m0C = 0.0f;
    mModel.mTracks[0].m08 = 0.0f;
    mBasePos = m6C;
    GimmickMsg_unk msg(1);
    vf40(&msg);
}

// NONMATCHING: same bounds copies as vf0C, which also changes the register of this/chara.
void FloatingBox::vf10() {
    if (m54 == 0) {
        Start();
    }
    if (lbl_8087F8A0 == 0) {
        return;
    }
    ModelBounds_unk* res = *(ModelBounds_unk**)((char*)&mModel + 0x16C);
    Box_unk box(res->mMin, res->mMax);
    atn::Vector3 size(1.1f * (AbsF_unk(box.mMax.x - box.mMin.x) * 0.5f), AbsF_unk(box.mMax.y - box.mMin.y) * 0.5f,
                      1.1f * (AbsF_unk(box.mMax.z - box.mMin.z) * 0.5f));
    if (mCurAmp > 0.0f) {
        mCurAmp -= 0.01f;
    }
    mPhase -= 0.17453292f;
    for (CharaTask* chara = lbl_8087F8A0->mHead; chara != 0; chara = ((CharaListNode_unk*)chara)->mNext) {
        int inside = 0;
        atn::Vector3 d = m6C - chara->mPos;
        if (fn_805F9920(&d) < 1.5f * (size.z * size.z)) {
            CharaTask::Sphere_unk sphere = chara->fn_80176548(chara->mPos);
            sphere.mCenter.y -= 5.0f;
            if (fn_80051CD8(&sphere, &mColli.m3C) != 0) {
                inside = 1;
            }
        }
        int found = 0;
        for (CharaTask** it = mRiders.begin(); it != mRiders.end(); ++it) {
            if (*it == chara) {
                found = 1;
                if (!inside) {
                    mRiders.erase(it);
                }
                break;
            }
        }
        if (inside) {
            if (!found) {
                mCurAmp = mAmp;
                mRiders.push_back(chara);
            }
            if (chara->m570 > 0.5f) {
                mCurAmp = mAmp;
            }
        }
    }
    if (mCurAmp <= 0.0f) {
        mPhase = 0.0f;
    }
    m6C = mBasePos;
    m6C.y += mCurAmp * (float)sin(mPhase);
    mColli.fn_800575BC(&m6C, &m78, &size);
    if (vf38() != 0) {
        fn_803ED5D0(vf38());
    }
    if (vf38() != 0) {
        fn_803ED0D4(vf38(), 1);
    }
    if (m878 != 0.0f) {
        for (GimmickCommon* g = GimmickManager::instance_->mHead; g != 0; g = g->m5C) {
        }
    }
}

void FloatingBox::vf14() {
    if (lbl_8087F0A8->m18 != 0 && vf38() != 0) {
        fn_803ED774(vf38());
    }
}

int FloatingBox::vf1C() {
    return mModel.fn_8008B140() != 0;
}

void FloatingBox::vf20() {
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    char path[0x100];
    csv.fn_8005BCE8(lbl_8087D710);
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "MODEL") == 0) {
            fn_8008AD4C(&mModel, csv.fn_8005B9CC(), 0);
            fn_803EC7A0(&mModel, &csv);
        } else if (strcmp(field, "MODEL_LOD") == 0) {
            strcpy(path, csv.fn_8005B9CC());
            fn_80092F1C(&mModel, path, 0, fn_800DC288(csv.fn_8005B9CC()));
        } else if (strcmp(field, "MOTION") == 0) {
            strcpy(path, csv.fn_8005B9CC());
            fn_80097A88(&mModel, fn_802180A8(csv.fn_8005B9CC()), path);
        }
    } while (csv.fn_8005BBF8() != 0);
}

int FloatingBox::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    m54 = msg->mType;
    if ((unsigned int)m54 > 2) {
        m54 = 0;
    }
    return m54;
}

void FloatingBox::vf70(int state) {
    GimmickMsg_unk msg(state);
    vf40(&msg);
}

int FloatingBox::vf60() {
    return m54 == 2;
}

// inferred: the debug menu is only built once
static int sMenuDone_unk;

void FloatingBox::vf6C(void* menu) {
    if (sMenuDone_unk != 0) {
        return;
    }
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
    m878 = 1.0f;
    sMenuDone_unk = 1;
}

atn::Model* FloatingBox::vf38() {
    return &mModel;
}

} // namespace gmk
