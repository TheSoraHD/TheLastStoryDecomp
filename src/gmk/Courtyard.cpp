#include "gmk/Courtyard.h"
#include "atn/Csv.h"
#include "atn/Memory.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" char* strncpy(char* dst, const char* src, unsigned long n);
extern "C" int atoi(const char* s);
extern "C" char lbl_8087D710[2]; // inferred: " " (field separators of the tab CSV)
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" void fn_8008AD4C(Model_unk* model, const char* path, const char* texture); // inferred: loads a model
extern "C" void fn_80092F1C(Model_unk* model, const char* path, int arg, float lod); // inferred: adds a LOD model
extern "C" int fn_802180A8(const char* s); // inferred: motion id from its name
extern "C" void fn_80097A88(Model_unk* model, int id, const char* path); // inferred: loads a motion
// inferred: game settings
struct Settings_unk {
    unsigned char m00[0x18];
    int m18;
};
extern "C" Settings_unk* lbl_8087F0A8;

namespace gmk {

GimmickCommon* Courtyard::fn_8041C09C(atn::Task* parent, int id, int arg) {
    if (parent != 0) {
        return new (12, "", "", 0) Courtyard(parent, id, arg);
    }
    return 0;
}

Courtyard::~Courtyard() {}

int Courtyard::vf0C() {
    if (fn_803EC91C() == 0) {
        m54 = 1;
        fn_803EDB18(&mModel);
        return 1;
    }
    return 0;
}

void Courtyard::vf28() {
    vf2C();
    if (vf38() != 0) {
        fn_803ED0D4(vf38(), 1);
    }
    mColli0.SetMtx_unk(*(atn::Matrix3x4*)mModel.mMtx);
    mColli1.SetMtx_unk(*(atn::Matrix3x4*)mModel.mMtx);
    GimmickMsg_unk msg(1);
    if (m58 == 1) {
        msg.mType = 4;
    }
    vf40(&msg);
}

void Courtyard::vf10() {
    if (m54 == 0) {
        vf28();
    }
    if (m54 == 2) {
        GimmickMsg_unk msg(3);
        vf40(&msg);
    }
    if (vf38() != 0) {
        fn_803ED0D4(vf38(), 1);
    }
    mColli0.SetMtx_unk(*(atn::Matrix3x4*)mModel.mMtx);
    mColli1.SetMtx_unk(*(atn::Matrix3x4*)mModel.mMtx);
    switch (m54) {
    case 1:
        if (mModel.fn_80097D40(1) != 0 && vf38() != 0) {
            fn_803ED5D0(vf38());
        }
        break;
    case 3: {
        if (vf38() != 0) {
            fn_803ED5D0(vf38());
        }
        float frame = mModel.mTracks[0].m08;
        if (frame >= mModel.fn_80097D7C(0)) {
            GimmickMsg_unk msg(4);
            vf40(&msg);
        }
        break;
    }
    case 5:
        if (vf38() != 0) {
            fn_803ED5D0(vf38());
        }
        if (mModel.mTracks[0].m08 <= 0.0f) {
            GimmickMsg_unk msg(1);
            vf40(&msg);
        }
        break;
    }
}

void Courtyard::vf14() {
    if (lbl_8087F0A8->m18 != 0 && vf38() != 0) {
        fn_803ED774(vf38());
    }
}

int Courtyard::vf1C() {
    if (mModel.fn_8008B140() != 0) {
        return 1;
    }
    if (mColli0.fn_800580BC() != 0 || mColli1.fn_800580BC() != 0) {
        return 1;
    }
    return 0;
}

void Courtyard::vf20() {
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
        if (strcmp(field, "COLLI_OBJECT") == 0) {
            strncpy(path, csv.fn_8005B9CC(), 0x100);
            if (atoi(csv.fn_8005B9CC()) == 0) {
                mColli0.fn_80058078(path);
            } else {
                mColli1.fn_80058078(path);
            }
        }
    } while (csv.fn_8005BBF8() != 0);
}

int Courtyard::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    m54 = msg->mType;
    switch (m54) {
    case 1:
        if (mModel.fn_80097D40(1) != 0) {
            mModel.fn_80097C08(0, 1, 1, 0, 1, 1.0f, 0.1f);
            mModel.mTracks[0].m0C = 1.0f;
        } else {
            mModel.fn_80097C08(0, 0, 0, 1, 1, 1.0f, 0.1f);
            mModel.mTracks[0].m0C = 0.0f;
        }
        mColli0.mFlags |= 1;
        mColli1.mFlags &= ~1;
        if (vf38() != 0) {
            fn_803ED0D4(vf38(), 1);
        }
        if (vf38() != 0) {
            fn_803ED5D0(vf38());
        }
        break;
    case 2:
    case 3:
        mModel.fn_80097C08(0, 0, 0, 1, 1, 1.0f, 0.1f);
        mModel.mTracks[0].m0C = 1.0f;
        mColli0.mFlags |= 1;
        mColli1.mFlags &= ~1;
        break;
    case 4:
        mModel.fn_80097C08(0, 0, 0, 1, 1, 1.0f, 0.1f);
        mModel.mTracks[0].m08 = mModel.fn_80097D7C(0);
        mColli0.mFlags &= ~1;
        mColli1.mFlags |= 1;
        if (vf38() != 0) {
            fn_803ED0D4(vf38(), 1);
        }
        if (vf38() != 0) {
            fn_803ED5D0(vf38());
        }
        break;
    case 5:
        mModel.fn_80097C08(0, 0, 0, 1, 1, 1.0f, 0.1f);
        mModel.mTracks[0].m08 = mModel.fn_80097D7C(0);
        mModel.mTracks[0].m0C = -1.0f;
        mColli0.mFlags |= 1;
        mColli1.mFlags &= ~1;
        break;
    }
    return m54;
}

void Courtyard::vf70(int state) {
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

Model_unk* Courtyard::vf38() {
    return &mModel;
}

} // namespace gmk
