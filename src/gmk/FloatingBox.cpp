#include "game/GameConfig_unk.h"
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
extern "C" int fn_802180A8(const char* s); // inferred: motion id from its name
extern "C" float PSVECSquareMag(const atn::Vector3* v); // inferred: squared length
extern "C" int fn_80051CD8(const CharaTask::Sphere_unk* sphere, const void* box); // inferred: sphere in box
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, void* func,
                            void* arg);
extern "C" void fn_80087E9C(void* group, const char* name, atn::Vector3* value, float min, float max, float step,
                            void* func, void* arg);
extern "C" void fn_80088724(void* group, const char* name, atn::Vector3* value, float min, float max, float step,
                            void* func, void* arg);
extern "C" void fn_80087994(void* group, const char* name, int* value, void* func, void* arg);
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
extern "C" GameConfig_unk* lbl_8087F0A8;

// inferred: bounding box of the model resource; names invented. The corners are returned by value from
// inline getters and assigned (temporary + copy, one load feeding both stores), as in gmk/OpenBoard.
struct Box_unk {
    Box_unk() {}
    atn::Vector3 mMin;
    atn::Vector3 mMax;
};
inline atn::Vector3 BoxMin_unk(const atn::Model::Res_unk* r) { return r->mMin; }
inline atn::Vector3 BoxMax_unk(const atn::Model::Res_unk* r) { return r->mMax; }
inline float AbsF_unk(float x) {
    if (x < 0.0f) {
        x = -x;
    }
    return x;
}
// inferred: position of a chara read through a reference (the original computes the address of the
// position before the return slot of fn_80176548; name invented)
static inline const atn::Vector3& PosOf_unk(CharaTask* c) {
    return c->mPos;
}

namespace gmk {

GimmickCommon* FloatingBox::fn_8042C21C(atn::Task* parent, int id, int arg) {
    if (parent != 0) {
        return new ((MemoryTag)12, "", "", 0) FloatingBox(parent, id, arg);
    }
    return 0;
}

FloatingBox::~FloatingBox() {}

// The half sizes are assigned one by one (each product right after its absolute value).
int FloatingBox::vf0C() {
    if (fn_803EC91C() == 0) {
        atn::Model::Res_unk* res = mModel.GetRes_unk();
        Box_unk box;
        box.mMin = BoxMin_unk(res);
        box.mMax = BoxMax_unk(res);
        atn::Vector3 size;
        size.x = AbsF_unk(box.mMax.x - box.mMin.x) / 2.0f;
        size.y = AbsF_unk(box.mMax.y - box.mMin.y) / 2.0f;
        size.z = AbsF_unk(box.mMax.z - box.mMin.z) / 2.0f;
        fn_803EDB18(&mModel);
        mColli.m0C = this;
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

void FloatingBox::vf10() {
    if (m54 == 0) {
        Start();
    }
    if (lbl_8087F8A0 == 0) {
        return;
    }
    atn::Model::Res_unk* res = mModel.GetRes_unk();
    Box_unk box;
    box.mMin = BoxMin_unk(res);
    box.mMax = BoxMax_unk(res);
    atn::Vector3 size;
    size.x = 1.1f * (AbsF_unk(box.mMax.x - box.mMin.x) / 2.0f);
    size.y = AbsF_unk(box.mMax.y - box.mMin.y) / 2.0f;
    size.z = 1.1f * (AbsF_unk(box.mMax.z - box.mMin.z) / 2.0f);
    if (mCurAmp > 0.0f) {
        mCurAmp -= 0.01f;
    }
    mPhase -= 0.17453292f;
    for (PlayerTask* chara = lbl_8087F8A0->mHead; chara != 0; chara = chara->mNext) {
        int inside = 0;
        atn::Vector3 d = m6C - chara->mPos;
        if (PSVECSquareMag(&d) < 1.5f * (size.z * size.z)) {
            CharaTask::Sphere_unk sphere = chara->fn_80176548(PosOf_unk(chara));
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
    float phase = mPhase;
    m6C = mBasePos;
    m6C.y += mCurAmp * (float)sin(phase);
    mColli.fn_800575BC(&m6C, &m78, &size);
    if (vf38() != 0) {
        fn_803ED5D0(vf38());
    }
    if (vf38() != 0) {
        fn_803ED0D4(vf38(), 1);
    }
    if (m878) {
        for (GimmickCommon* g = GimmickManager::instance_->mHead; g != 0; g = g->m5C) {
        }
    }
}

void FloatingBox::vf14() {
    if (lbl_8087F0A8->mDrawMap != 0 && vf38() != 0) {
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
            mModel.fn_8008AD4C(csv.fn_8005B9CC(), 0);
            fn_803EC7A0(&mModel, &csv);
        } else if (strcmp(field, "MODEL_LOD") == 0) {
            strcpy(path, csv.fn_8005B9CC());
            mModel.fn_80092F1C(path, 0, fn_800DC288(csv.fn_8005B9CC()));
        } else if (strcmp(field, "MOTION") == 0) {
            strcpy(path, csv.fn_8005B9CC());
            mModel.fn_80097A88(fn_802180A8(csv.fn_8005B9CC()), path);
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
    fn_80087E9C(group, "pos", &m6C, -9999.0f, 9999.0f, 0.25f, 0, 0);
    fn_80088724(group, "rot", &m78, -3.1415927f, 3.1415927f, 0.05235988f, 0, 0);
    fn_80087E9C(group, "scl", &m90, -9999.0f, 9999.0f, 0.25f, 0, 0);
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
