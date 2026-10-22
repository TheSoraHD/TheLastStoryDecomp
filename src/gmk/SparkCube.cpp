#include "gmk/SparkCube.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "game/CharaTask.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int atoi(const char* s);
extern "C" int rand();
extern "C" char lbl_8087D710[2]; // inferred: " " (field separators of the tab CSV)
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" void fn_8008AD4C(Model_unk* model, const char* path, const char* texture); // inferred: loads a model
extern "C" void fn_80092F1C(Model_unk* model, const char* path, int arg, float lod); // inferred: adds a LOD model
extern "C" int fn_802180A8(const char* s); // inferred: motion id from its name
extern "C" void fn_80097A88(Model_unk* model, int id, const char* path); // inferred: loads a motion
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_80087E9C(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void fn_80088724(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);

// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk {
    unsigned char m00[0xB8];
    int mB8;
};
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, Model_unk* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_80232B7C(void* owner, int arg);
// inferred: game settings
struct Settings_unk {
    unsigned char m00[0x18];
    int m18;
};
extern "C" Settings_unk* lbl_8087F0A8;
// inferred: frame parameters (as gmk::ArenaFloor)
struct Params_unk {
    unsigned char m000[0x3A4];
    float mTimeScale; // 0x3A4
};
extern "C" Params_unk* lbl_8087EFA8;
// inferred: bullets
struct BulletMgr_unk;
extern "C" BulletMgr_unk* lbl_8087F048;
extern "C" int fn_800F8548(BulletMgr_unk* mgr);
extern "C" void fn_800FAB80(BulletMgr_unk* mgr, int arg1, void* bullet, int arg3, const atn::Vector3* pos,
                            const atn::Vector3* rot, int arg6, int arg7, int arg8, int arg9, float arg10,
                            float arg11);
extern "C" void* lbl_8087F610; // inferred: arena mode

// inferred: resource data of the spark model (Model_unk::m16C): material names (names invented)
struct SparkRes_unk {
    unsigned char m00[0x30];
    int mCount;           // 0x30
    unsigned char m34[0x50 - 0x34];
    const char* mNames[1]; // 0x50
};
// inferred: views of the hit message (names invented)
struct SparkHitSrc_unk {
    unsigned char m00[0x48];
    int m48; // inferred: kind (2 hits the cube outside the arena)
};
struct SparkHitSkill_unk {
    unsigned char m00[0x04];
    int mId;
    unsigned char m08[0x3C - 0x08];
    int m3C; // inferred: level
    unsigned char m40[0xAC - 0x40];
    unsigned int mFlags; // 0xAC
};
struct SparkHitMsg_unk {
    unsigned char m00[0x28];
    SparkHitSrc_unk* mSrc;     // 0x28
    SparkHitSkill_unk* mSkill; // 0x2C
};

// inferred: inlined here (the local static is shared with the out-of-line copy in CharaTask.cpp)
inline CharaListeners_unk* fn_8014F8F4() {
    static CharaListeners_unk sListeners;
    return &sListeners;
}
// inferred: inlined copy of CharaListeners_unk::fn_8014F860 (name invented)
inline void NotifyListeners_unk(CharaListeners_unk* listeners, int id, void* chara, void* arg) {
    if (listeners->mEnabled != 0) {
        for (int i = 0; i < (int)listeners->mList.size(); i++) {
            CharaListener_unk& listener = listeners->mList[i];
            if (listener.mId == -1 || id == listener.mId) {
                listener.mFunc(id, (CharaTask*)chara, arg);
            }
        }
    }
}
inline void PlayEfp_unk(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, Model_unk* model) {
    atn::Vector3 pos;
    atn::Vector3 rot;
    Color4f_unk color;
    rot.x = 0.0f;
    rot.y = 0.0f;
    rot.z = 0.0f;
    pos.x = 0.0f;
    pos.y = 0.0f;
    pos.z = 0.0f;
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    color.a = 1.0f;
    fn_8023A8B4(mgr, efp, model, 0, pos, rot, color, -1, -1, 1, 1.0f);
}

// inferred: matrix of the named node, else the model matrix (name invented)
inline MtxPtr GetMtx_unk(Model_unk* model, const char* name) {
    MtxPtr mtx = model->GetNodeMtx_unk(name);
    if (mtx != 0) {
        return mtx;
    }
    return model->mMtx;
}

namespace gmk {

GimmickCommon* SparkCube::fn_80426108(atn::Task* parent, int id, int arg) {
    if (parent != 0) {
        return new (12, "", "", 0) SparkCube(parent, id, arg);
    }
    return 0;
}

SparkCube::SparkCube(atn::Task* parent, int id, int arg)
    : GimmickCommon(parent, id, arg), mModel(8, 32), mSpark(8, 32), mHasSpark(0), mSkill(0), mHideDebug(0) {
    m54 = 0;
    for (int i = 0; i < 5; i++) {
        mMin[i] = 0.0f;
        mRange[i] = 0.0f;
    }
}

SparkCube::~SparkCube() {}

// The int index compared as unsigned increments the counter before the element offset
int SparkCube::vf0C() {
    if (fn_803EC91C() == 0) {
        fn_803EDB18(&mModel);
        fn_803EDB18(&mSpark);
        mColli.m0C = (int)this;
        mColli.mFlags |= 8;
        if (mId == 48000) {
            mColli.m20 = 0xF8;
        } else {
            mFades.Resize_unk(((SparkRes_unk*)mSpark.m16C)->mCount, 0);
            for (int i = 0; i < ((SparkRes_unk*)mSpark.m16C)->mCount; i++) {
                mFades.mData[(unsigned int)i].fn_803EBD6C(&mSpark, ((SparkRes_unk*)mSpark.m16C)->mNames[i], 30, 30, 0.0f);
            }
        }
        return 1;
    }
    return 0;
}

void SparkCube::vf28() {
    vf2C();
    if (vf38() != 0) {
        fn_803ED0D4(vf38(), 1);
    }
    mColli.SetMtx_unk(*(atn::Matrix3x4*)GetMtx_unk(&mModel, "model01"));
    mColli.mFlags |= 1;
    GimmickMsg_unk msg(m58 + 1);
    vf40(&msg);
}

// The int index compared as unsigned increments the counter before the element offset
void SparkCube::vf10() {
    if (m54 == 0) {
        vf28();
    }
    int state = -1;
    float step = lbl_8087EFA8->mTimeScale;
    mTimer += step;
    switch (m54) {
    case 2:
    case 3:
        if (mTimer > mWait) {
            state = m54 + 1;
        }
        break;
    case 4: {
        Model_unk* model = GetModel_unk();
        float frame = model->mTracks[0].m08;
        if (frame >= model->fn_80097D7C(0)) {
            state = 1;
        }
        break;
    }
    }
    if (state >= 0) {
        vf70(state);
    }
    if (vf38() != 0) {
        fn_803ED5D0(vf38());
    }
    if (vf38() != 0) {
        fn_803ED0D4(vf38(), 1);
    }
    mColli.SetMtx_unk(*(atn::Matrix3x4*)GetMtx_unk(&mModel, "model01"));
    if (m54 >= 3) {
        for (int i = 0; i < (unsigned int)mFades.mSize; i++) {
            mFades[i].fn_803EBE74(&mSpark);
        }
        if (m54 == 3) {
            mHideDebug = mSpark.mTracks[0].m08 > 60.0f;
        } else {
            mHideDebug = mSpark.mTracks[0].m08 < 30.0f;
        }
    } else {
        mHideDebug = 0;
    }
}

void SparkCube::vf14() {
    if (lbl_8087F0A8->m18 != 0) {
        if (mHasSpark != 0 && m54 == 3) {
            float frame = mSpark.mTracks[0].m08;
            if (frame >= mSpark.fn_80097D7C(0)) {
                return;
            }
        }
        if (mHideDebug == 0 && vf38() != 0) {
            fn_803ED774(vf38());
        }
    }
}

int SparkCube::vf1C() {
    if (mModel.fn_8008B140() != 0 || mSpark.fn_8008B140() != 0) {
        return 1;
    }
    if (mColli.fn_800580BC() != 0) {
        return 1;
    }
    for (int i = 0; i < 5; i++) {
        if (mEfp[i].fn_80237874() != 0) {
            return 1;
        }
    }
    return 0;
}

void SparkCube::vf20() {
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
        } else if (strcmp(field, "MODEL_SPARK") == 0) {
            fn_8008AD4C(&mSpark, csv.fn_8005B9CC(), 0);
            fn_803EC7A0(&mSpark, &csv);
            mHasSpark = 1;
        } else if (strcmp(field, "MODEL_LOD") == 0) {
            strcpy(path, csv.fn_8005B9CC());
            fn_80092F1C(&mModel, path, 0, fn_800DC288(csv.fn_8005B9CC()));
        } else if (strcmp(field, "MOTION") == 0) {
            strcpy(path, csv.fn_8005B9CC());
            int id = fn_802180A8(csv.fn_8005B9CC());
            if (mHasSpark != 0) {
                fn_80097A88(&mSpark, id, path);
            } else {
                fn_80097A88(&mModel, id, path);
            }
        } else if (strcmp(field, "COLLI_OBJECT") == 0) {
            mColli.fn_80058078(csv.fn_8005B9CC());
        } else if (strcmp(field, "EFP") == 0) {
            strcpy(path, csv.fn_8005B9CC());
            mEfp[atoi(csv.fn_8005B9CC())].fn_8023780C(path);
        } else if (strcmp(field, "STATE_CHANGE_FRAME") == 0) {
            int state = atoi(csv.fn_8005B9CC());
            mMin[state] = fn_800DC288(csv.fn_8005B9CC());
            mRange[state] = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(field, "SKILL") == 0) {
            mSkill = atoi(csv.fn_8005B9CC());
        }
    } while (csv.fn_8005BBF8() != 0);
}

// The int index compared as unsigned increments the counter before the element offset
int SparkCube::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    mTimer = 0.0f;
    m54 = msg->mType;
    if ((unsigned int)m54 > 4) {
        m54 = 0;
    }
    fn_80239DAC(lbl_8087F3C0, this, 0, 0);
    m9C = 0;
    mModel.fn_80097C08(0, -1, 1, 0, 1, 0.0f, 0.1f);
    mSpark.fn_80097C08(0, -1, 1, 0, 1, 0.0f, 0.1f);
    switch (m54) {
    case 1:
        if (mEfp[m54].mResource != 0) {
            lbl_8087F3C0->mB8 = 2;
            fn_80232B7C(this, 0);
            PlayEfp_unk(lbl_8087F3C0, &mEfp[m54], &mModel);
            lbl_8087F3C0->mB8 = 0;
        }
        m9C = 1;
        mColli.mFlags |= 1;
        break;
    case 2:
        if (mEfp[m54].mResource != 0) {
            fn_80232B7C(this, 0);
            PlayEfp_unk(lbl_8087F3C0, &mEfp[m54], &mModel);
        }
        mWait = GetWait_unk(m54);
        m9C = 1;
        mColli.mFlags |= 1;
        break;
    case 3: {
        if (mEfp[m54].mResource != 0) {
            fn_80232B7C(0, 0);
            PlayEfp_unk(lbl_8087F3C0, &mEfp[m54], &mModel);
        }
        Model_unk* model = vf38();
        if (model != 0 && model->fn_80097D40(1) != 0) {
            model->fn_80097C08(0, 1, 0, 1, 1, 1.0f, 0.1f);
            m9C = 1;
            for (int i = 0; i < (unsigned int)mFades.mSize; i++) {
                mFades[i].mReverse = 0;
            }
        }
        BulletMgr_unk* mgr = lbl_8087F048;
        if (mgr != 0) {
            atn::Vector3 pos;
            atn::Vector3 rot;
            rot.x = 0.0f;
            rot.y = 0.0f;
            rot.z = 0.0f;
            float z = mModel.mMtx[2][3];
            float y = mModel.mMtx[1][3];
            float x = mModel.mMtx[0][3];
            pos.x = x;
            pos.y = y;
            pos.z = z;
            fn_800FAB80(mgr, 0, fn_80219E6C(mSkill), fn_800F8548(mgr), &pos, &rot, 0, 30, -1, -1, 0.0f, 1.0f);
        }
        mWait = GetWait_unk(m54);
        mColli.mFlags &= ~1;
        break;
    }
    case 4: {
        if (mEfp[m54].mResource != 0) {
            fn_80232B7C(this, 0);
            PlayEfp_unk(lbl_8087F3C0, &mEfp[m54], &mModel);
        }
        Model_unk* model = vf38();
        if (model != 0 && model->fn_80097D40(0) != 0) {
            model->fn_80097C08(0, 0, 0, 1, 1, 1.0f, 0.1f);
            model->mTracks[0].m08 = 1.0f;
            for (int i = 0; i < (unsigned int)mFades.mSize; i++) {
                mFades[i].mReverse = 1;
            }
        }
        m9C = 1;
        mColli.mFlags |= 1;
        break;
    }
    }
    NotifyListeners_unk(fn_8014F8F4(), 11, this, msg);
    return m54;
}

int SparkCube::vf44(GimmickHitMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    if (vf5C() == 0) {
        return 0;
    }
    if (mId != 48000) {
        SparkHitMsg_unk* hit = (SparkHitMsg_unk*)msg;
        if (hit->mSrc == 0 || (hit->mSrc->m48 != 2 && lbl_8087F610 == 0)) {
            return 0;
        }
        SparkHitSkill_unk* skill = hit->mSkill;
        if (skill != 0) {
            switch (skill->mId) {
            case 0x785:
            case 0x78D:
            case 0x78E:
            case 0x78F:
                return 0;
            }
            if ((skill->mFlags & 0x2000000) == 0x2000000) {
                return 0;
            }
            if (skill->m3C < 2) {
                return 0;
            }
        }
    }
    if (m54 == 1) {
        vf70(2);
    }
    return m54;
}

void SparkCube::vf70(int state) {
    GimmickMsg_unk msg(state);
    vf40(&msg);
}

Model_unk* SparkCube::vf38() {
    return GetModel_unk();
}

int SparkCube::vf60() {
    return m54 != 3;
}

int SparkCube::vf50() {
    return (int)&mColli;
}

atn::Vector3 SparkCube::vf54() {
    float z = mModel.mMtx[2][3];
    float y = mModel.mMtx[1][3];
    float x = mModel.mMtx[0][3];
    return atn::Vector3(x, y, z);
}

float SparkCube::vf58() {
    return mModel.fn_80094F98().mRadius;
}

int SparkCube::vf5C() {
    return m54 == 1;
}

void SparkCube::vf6C(void* menu) {
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
}

} // namespace gmk
