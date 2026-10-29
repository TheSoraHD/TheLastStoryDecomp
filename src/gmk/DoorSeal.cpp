#include "gmk/DoorSeal.h"
#include "atn/Csv.h"
#include "atn/Memory.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" char lbl_8087D710[2]; // inferred: " " (field separators of the tab CSV)
extern "C" void fn_8008AD4C(atn::Model* model, const char* path, const char* texture); // inferred: loads a model
extern "C" int fn_802180A8(const char* s); // inferred: motion id from its name
extern "C" void fn_80097A88(atn::Model* model, int id, const char* path); // inferred: loads a motion

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
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
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

namespace gmk {

GimmickCommon* DoorSeal::fn_8042335C(atn::Task* parent, int id, int arg) {
    if (parent != 0) {
        return new ((MemoryTag)12, "", "", 0) DoorSeal(parent, id, arg);
    }
    return 0;
}

DoorSeal::~DoorSeal() {}

int DoorSeal::vf0C() {
    if (fn_803EC91C() == 0) {
        m54 = 1;
        mBefore.mFlags &= ~0x100;
        mBefore.mFlags &= ~8;
        mBefore.mFlags |= 0x100000;
        mAfter.mFlags &= ~0x100;
        mAfter.mFlags &= ~8;
        mAfter.mFlags |= 0x100000;
        fn_803EDB18(&mBefore);
        fn_803EDB18(&mAfter);
        return 1;
    }
    return 0;
}

void DoorSeal::Start() {
    vf2C();
    if (vf38() != 0) {
        fn_803ED0D4(vf38(), 1);
    }
    mColli.SetMtx_unk(*(atn::Matrix3x4*)mBefore.mMtx);
    GimmickMsg_unk msg(1);
    if (m58 == 1) {
        msg.mType = 4;
    }
    vf40(&msg);
}

void DoorSeal::vf10() {
    if (m54 == 0) {
        Start();
    }
    if (m54 == 2) {
        GimmickMsg_unk msg(3);
        vf40(&msg);
    }
    if (vf38() != 0) {
        fn_803ED0D4(vf38(), 1);
    }
    mColli.SetMtx_unk(*(atn::Matrix3x4*)mBefore.mMtx);
    switch (m54) {
    case 3: {
        if (vf38() != 0) {
            fn_803ED5D0(vf38());
        }
        float frame = mAfter.mTracks[0].m08;
        if (frame >= mAfter.fn_80097D7C(0)) {
            GimmickMsg_unk msg(4);
            vf40(&msg);
        }
        break;
    }
    case 5:
        if (vf38() != 0) {
            fn_803ED5D0(vf38());
        }
        if (mAfter.mTracks[0].m08 <= 0.0f) {
            GimmickMsg_unk msg(1);
            vf40(&msg);
        }
        break;
    }
}

void DoorSeal::vf14() {
    if (lbl_8087F0A8->m18 != 0 && vf38() != 0) {
        fn_803ED774(vf38());
    }
}

int DoorSeal::vf1C() {
    if (mBefore.fn_8008B140() != 0 || mAfter.fn_8008B140() != 0) {
        return 1;
    }
    if (mColli.fn_800580BC() != 0) {
        return 1;
    }
    return mEfp.IsLoading() != 0;
}

void DoorSeal::vf20() {
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    char path[0x100];
    csv.fn_8005BCE8(lbl_8087D710);
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "MODEL_BEFORE") == 0) {
            fn_8008AD4C(&mBefore, csv.fn_8005B9CC(), 0);
            fn_803EC7A0(&mBefore, &csv);
        } else if (strcmp(field, "MODEL_AFTER") == 0) {
            fn_8008AD4C(&mAfter, csv.fn_8005B9CC(), 0);
            fn_803EC7A0(&mAfter, &csv);
        } else if (strcmp(field, "MOTION") == 0) {
            strcpy(path, csv.fn_8005B9CC());
            fn_80097A88(&mAfter, fn_802180A8(csv.fn_8005B9CC()), path);
        } else if (strcmp(field, "COLLI_OBJECT") == 0) {
            mColli.fn_80058078(csv.fn_8005B9CC());
        } else if (strcmp(field, "EFP") == 0) {
            mEfp.LoadRequest(csv.fn_8005B9CC());
        }
    } while (csv.fn_8005BBF8() != 0);
}

int DoorSeal::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    m54 = msg->mType;
    switch (m54) {
    case 1: {
        mAfter.fn_80097C08(0, 0, 0, 1, 1, 1.0f, 0.1f);
        fn_803ED0D4(&mBefore, 1);
        fn_803ED0D4(&mAfter, 1);
        mColli.mFlags |= 1;
        fn_80239DAC(lbl_8087F3C0, this, 0, 1);
        fn_80232B7C(this, 0);
        Color4f_unk color;
        atn::Vector3 rot;
        atn::Vector3 pos;
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
        fn_8023A8B4(lbl_8087F3C0, &mEfp, &mBefore, 0, pos, rot, color, -1, -1, 1, 1.0f);
        break;
    }
    case 2:
    case 3:
        mAfter.fn_80097C08(0, 0, 0, 1, 1, 1.0f, 0.1f);
        mAfter.mTracks[0].m0C = 1.0f;
        mColli.mFlags |= 1;
        fn_80239DAC(lbl_8087F3C0, this, 0, 1);
        break;
    case 4:
        mAfter.fn_80097C08(0, 0, 0, 1, 1, 1.0f, 0.1f);
        mAfter.mTracks[0].m08 = mAfter.fn_80097D7C(0);
        mColli.mFlags &= ~1;
        fn_80239DAC(lbl_8087F3C0, this, 0, 1);
        if (vf38() != 0) {
            fn_803ED0D4(vf38(), 1);
        }
        if (vf38() != 0) {
            fn_803ED5D0(vf38());
        }
        break;
    case 5:
        mAfter.fn_80097C08(0, 0, 0, 1, 1, 1.0f, 0.1f);
        mAfter.mTracks[0].m08 = mAfter.fn_80097D7C(0);
        mAfter.mTracks[0].m0C = -1.0f;
        mColli.mFlags |= 1;
        fn_80239DAC(lbl_8087F3C0, this, 0, 1);
        break;
    }
    return m54;
}

void DoorSeal::vf70(int state) {
    switch (state) {
    case 1:
    case 5:
        state = 1;
        break;
    case 2:
    case 3:
    case 4:
        state = 4;
        break;
    }
    GimmickMsg_unk msg(state);
    vf40(&msg);
    if (mE4 != 0) {
        mE4->m58 = 1;
    }
}

atn::Model* DoorSeal::vf38() {
    if (m54 == 1) {
        return &mBefore;
    }
    return &mAfter;
}

} // namespace gmk
