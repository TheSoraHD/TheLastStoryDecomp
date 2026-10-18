#include "gmk/Banana.h"
#include "atn/Csv.h"
#include "atn/Memory.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" char lbl_8087D710[2]; // inferred: " " (field separators of the tab CSV)
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" void fn_8008AD4C(Model_unk* model, const char* path, const char* texture); // inferred: loads a model
extern "C" void fn_80092F1C(Model_unk* model, const char* path, int arg, float lod); // inferred: adds a LOD model
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length

// inferred: camera (lbl_8087EFB4)
struct Camera_unk {
    unsigned char m00[0x10C];
    atn::Vector3 mPos; // 0x10C
};
extern "C" Camera_unk* lbl_8087EFB4;
// inferred: game settings
struct Settings_unk {
    unsigned char m00[0x30];
    int mFps; // 0x30: inferred: frames per second
};
extern "C" Settings_unk* lbl_8087F0A8;

namespace gmk {

GimmickCommon* Banana::fn_804166B0(atn::Task* parent, GimmickDesc_unk* desc) {
    if (parent != 0) {
        return new (12, "", "", 0) Banana(parent, desc);
    }
    return 0;
}

int Banana::vf1C() {
    return mModel.fn_8008B140() != 0;
}

void Banana::vf20() {
    char path[256];
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    csv.fn_8005BCE8(lbl_8087D710);
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "MODEL") == 0) {
            fn_8008AD4C(&mModel, csv.fn_8005B9CC(), 0);
        } else if (strcmp(field, "MODEL_LOD") == 0) {
            strcpy(path, csv.fn_8005B9CC());
            fn_80092F1C(&mModel, path, 0, fn_800DC288(csv.fn_8005B9CC()));
        }
    } while (csv.fn_8005BBF8() != 0);
}

void Banana::vf24() {
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    csv.fn_8005BCE8(lbl_8087D710);
    do {
        csv.fn_8005B9CC();
    } while (csv.fn_8005BBF8() != 0);
}

int Banana::vf0C() {
    if (fn_803EC91C() == 0) {
        m54 = 2;
        return 1;
    }
    return 0;
}

void Banana::vf28() {
    m6C = mDesc->mPos;
    m78.y = mDesc->mRotY;
    m78.x = 0.0f;
    m78.z = 0.0f;
}

// NONMATCHING: only the mDesc->m7C test differs: the target branches over a jump to the end
// (bne; b) where this compiles to one beq (same in vf14).
void Banana::vf10() {
    if (m54 == 1) {
        return;
    }
    if (mDesc->m7C == 0) {
        return;
    }
    atn::Vector3 d(lbl_8087EFB4->mPos.x - m6C.x, lbl_8087EFB4->mPos.y - m6C.y, lbl_8087EFB4->mPos.z - m6C.z);
    if (fn_805F9920(&d) > 250000.0f && m54 == 2) {
        return;
    }
    switch (m54) {
    case 3:
        m6C += mVel;
        m78 += mRotVel;
        mVel.y += -98.1f / (float)(lbl_8087F0A8->mFps * lbl_8087F0A8->mFps);
        if (++mTimer > 45) {
            m54 = 4;
            mTimer = 0;
        }
        break;
    case 4:
        if (++mTimer > mDesc->m20) {
            m54 = 2;
            mTimer = 0;
            Banana::vf28();
        }
        break;
    }
    fn_803ED0D4(&mModel, 1);
}

// NONMATCHING: see vf10 (target: bne; blr). Tried: == 0 / != 0, else, goto, switch, bool local,
// inline predicate.
void Banana::vf14() {
    if (m54 == 1) {
        return;
    }
    if (m54 == 4) {
        return;
    }
    if (mDesc->m7C != 0) {
        mModel.fn_8008CD60();
    }
}

int Banana::vf60() {
    int ret = 0;
    if (m54 == 2 && mDesc->m7C != 0) {
        ret = 1;
    }
    return ret;
}

Model_unk* Banana::vf38() {
    return &mModel;
}

} // namespace gmk
