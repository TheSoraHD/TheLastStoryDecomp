#include "game/LastCocoonImago.h"
#include "chr/ControlBase.h"
#include "game/GameMessageList.h"
#include "game/GimmickManager.h"

// inferred: effect/bullet manager (lbl_8087F048); only the member used here (name invented)
struct Effect_unk {
    unsigned char m00000[0x3E2E8];
    void* m3E2E8; // inferred: sphere of the current target (compared with m18FC)
};
extern "C" Effect_unk* lbl_8087F048;
// inferred: field (lbl_8087F430); only the member used here
struct Field_unk {
    unsigned char m0000[0x54F4];
    unsigned char m54F4[4]; // inferred: name table (fn_803B3B38)
};
extern "C" Field_unk* lbl_8087F430;
extern "C" wchar_t* wcscpy(wchar_t* dst, const wchar_t* src);
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
extern "C" int fn_80370A78(Field_unk* field, int index);             // inferred: reads an event flag
extern "C" void fn_80370AE4(Field_unk* field, int index, int value); // inferred: sets an event flag
extern "C" atn::Vector3 lbl_807C7030;        // inferred: zero vector
extern "C" int fn_80051B14(const atn::ColliSphereShape_unk* s, const atn::ColliCapsuleShape_unk* c); // inferred: hit test

atn::Vector3 LastCocoonImago::vf64(int center) {
    atn::Vector3 result = mPos;
    if (center == 0) {
        result = mPos;
    } else if (m18D0 != 0 && m58C != 12) {
        result = m624.mData[m18C8].m04.mCenter;
    }
    return result;
}

inline void LastCocoonImago::SetMode_unk(int mode) {
    m14BC = 0;
    m14C0 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    vf48();
    fn_80178A6C();
    fn_8015495C(0, 1, 1, 1);
    fn_8016DA4C();
    m58C = mode;
    fn_8016E970(3);
}

// inferred: name of the character, unless the current target is its sphere
int LastCocoonImago::vfB8(wchar_t* name) {
    if (lbl_8087F048 != 0 && lbl_8087F048->m3E2E8 != &m18FC && m5C != 0 && m5C->mC4 > 0 && lbl_8087F430 != 0) {
        GameMessage* entry = ((GameMessageList*)lbl_8087F430->m54F4)->fn_803B3B38(m5C->mC4);
        if (entry != 0) {
            wcscpy(name, entry->mText);
            return 1;
        }
    }
    return 0;
}

void LastCocoonImago::vf24() {
    if (m55C == 6 && mControl && !mControl->vf3C()) {
        return;
    }
    if (m197C) {
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x3B, 1, 0, 1, 0.0f, 0.025f);
    } else if (m14C8 != 0) {
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x14, 1, 0, 1, 0.0f, 0.025f);
    } else {
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 2, 1, 0, 1, 0.0f, 0.025f);
    }
}

void LastCocoonImago::vf38(int enable) {
    for (int i = 0; i < 11; i++) {
        if (enable) {
            mCapsules[i].mFlags |= 1;
        } else {
            mCapsules[i].mFlags &= ~1;
        }
    }
}

void LastCocoonImago::fn_8045D224() {
    if (m18F8->m54 == 5 && m18F8->vf38() != 0) {
        // inferred: the model is read again before each access (a local per access keeps the constants
        // out of the saved registers)
        atn::Model* model = m18F8->vf38();
        model->mTracks[0].m0C = 1.0f;
        model = m18F8->vf38();
        if (model->mTracks[0].m08 < 201.0f) {
            model = m18F8->vf38();
            model->mTracks[0].m08 = 201.0f;
        }
    }
    if (m14C0 == 120) {
        LastCocoonImago::vf38(0);
        if (fn_80370A78(lbl_8087F430, 0xCD) == 0) {
            fn_80370AE4(lbl_8087F430, 0xCD, 1);
        }
    } else {
        mMotion.fn_80097D7C(0);
    }
}

void LastCocoonImago::fn_8045EB60() {
    int fromHang = m58C == 14;
    SetMode_unk(2);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.fn_80097C08(0, fromHang ? 0x31 : 0x37, 0, 0, 1, 0.0f, 0.025f);
    mMotion.mTracks[0].m0C = 0.5f;
    if (fromHang) {
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.mTracks[0].m08 = 200.0f;
        GimmickMsg_unk msg;
        msg.mType = 5;
        m18F8->vf40(&msg);
    }
    m574 = lbl_807C7030;
}

void LastCocoonImago::fn_8045ECC4() {
    SetMode_unk(12);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x31, 0, 0, 1, 0.0f, 0.1f);
    m574 = lbl_807C7030;
    GimmickMsg_unk msg;
    msg.mType = 5;
    m18F8->vf40(&msg);
}

// inferred: the sphere of a ColliSphere read as one shape (name invented)
inline const atn::ColliSphereShape_unk& GetShape_unk(const atn::ColliSphere& c) {
    return *(const atn::ColliSphereShape_unk*)&c.mSphere;
}
// inferred: the segment and radius of a capsule (m3C-m57) read as one shape (name invented)
inline const atn::ColliCapsuleShape_unk& GetShape_unk(const atn::ColliCapsule& c) {
    return *(const atn::ColliCapsuleShape_unk*)&c.m3C;
}

void LastCocoonImago::fn_8045F4CC() {
    for (GimmickCommon* g = GimmickManager::instance_->mHead; g != 0; g = g->m5C) {
        if (g->m50 == 1 && g->vf5C() != 0) {
            atn::ColliSphere* colli = (atn::ColliSphere*)g->vf50();
            if (colli != 0) {
                atn::ColliSphereShape_unk s = GetShape_unk(*colli);
                int hit = 0;
                atn::ColliCapsuleShape_unk c = GetShape_unk(mColli);
                if (fn_80051B14(&s, &c)) {
                    hit = 1;
                }
                c = GetShape_unk(mCapsules[6]);
                if (fn_80051B14(&s, &c)) {
                    hit = 1;
                }
                c = GetShape_unk(mCapsules[8]);
                if (fn_80051B14(&s, &c)) {
                    hit = 1;
                }
                if (hit) {
                    GimmickMsg_unk msg;
                    msg.mType = 4;
                    g->vf40(&msg);
                }
            }
        }
    }
}

// inferred: frames 80-105 of the motion of state 11
int LastCocoonImago::vfE4() {
    if (m58C == 11) {
        float frame = mMotion.mTracks[0].m08;
        if (80.0f <= frame && frame <= 105.0f) {
            return 1;
        }
    }
    return 0;
}

float LastCocoonImago::vfDC() {
    return 0.7f;
}

float LastCocoonImago::vfD4() {
    return 2.2f;
}

float LastCocoonImago::vf90() {
    return 5.0f;
}

LastCocoonImago::~LastCocoonImago() {}
