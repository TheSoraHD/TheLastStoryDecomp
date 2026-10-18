#include "gmk/OpenBoard.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "chr/ControlBase.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" char lbl_8087D710[2]; // inferred: " " (field separators of the tab CSV)
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" void fn_8008AD4C(Model_unk* model, const char* path, const char* texture); // inferred: loads a model
extern "C" void fn_80092F1C(Model_unk* model, const char* path, int arg, float lod); // inferred: adds a LOD model
extern "C" int fn_802180A8(const char* s); // inferred: motion id from its name
extern "C" void fn_80097A88(Model_unk* model, int id, const char* path); // inferred: loads a motion
// inferred: float variables in .sdata used only here (values 0)
extern "C" float lbl_8087DFB8;
extern "C" float lbl_8087DFBC;
// inferred: game settings
struct Settings_unk {
    unsigned char m00[0x18];
    int m18;
};
extern "C" Settings_unk* lbl_8087F0A8;

namespace gmk {

GimmickCommon* OpenBoard::fn_8042B56C(atn::Task* parent, int id, int arg) {
    if (parent != 0) {
        return new (12, "", "", 0) OpenBoard(parent, id, arg);
    }
    return 0;
}

OpenBoard::OpenBoard(atn::Task* parent, int id, int arg)
    : GimmickCommon(parent, id, arg), mModel(8, 32), mOpenDistance(50.0f), m55C(0.0f), m560(0.0f), m564(1.0f),
      m568(false), m56C(lbl_8087DFB8), m57C(0.0f) {
    m54 = 0;
}

int OpenBoard::vf0C() {
    if (fn_803EC91C() == 0) {
        fn_803EDB18(&mModel);
        mColli.m0C = (int)this;
        return 1;
    }
    return 0;
}

void OpenBoard::vf28() {
    vf2C();
    if (vf38() != 0) {
        fn_803ED0D4(vf38(), 1);
    }
    atn::Matrix3x4 m;
    PSMTXRotRad(m.m, 'y', m78.y);
    m.m[0][3] = m6C.x;
    m.m[1][3] = m6C.y;
    m.m[2][3] = m6C.z;
    mColli.SetMtx_unk(m);
    mColli.mFlags |= 1;
    ((chr::Motion_unk*)&mModel)->fn_80097C08(0, 0, 0, 1, 1, 0.0f, 1.0f);
    mModel.m238 = 0.0f;
    mModel.mFrame = 0.0f;
    m55C = 0.0f;
    m560 = 0.0f;
    m564 = 1.0f;
    m568 = false;
    m56C.Set_unk(lbl_8087DFBC);
    GimmickMsg_unk msg(m58 + 1);
    vf40(&msg);
}

void OpenBoard::vf14() {
    if (lbl_8087F0A8->m18 != 0 && vf38() != 0) {
        fn_803ED774(vf38());
    }
}

int OpenBoard::vf1C() {
    if (mModel.fn_8008B140() != 0) {
        return 1;
    }
    if (mColli.fn_800580BC() != 0) {
        return 1;
    }
    return mEfp.fn_80237874() != 0;
}

void OpenBoard::vf20() {
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
            fn_803EC7A0(&mModel, &csv);
        } else if (strcmp(field, "MODEL_LOD") == 0) {
            strcpy(path, csv.fn_8005B9CC());
            fn_80092F1C(&mModel, path, 0, fn_800DC288(csv.fn_8005B9CC()));
        } else if (strcmp(field, "MOTION") == 0) {
            strcpy(path, csv.fn_8005B9CC());
            fn_80097A88(&mModel, fn_802180A8(csv.fn_8005B9CC()), path);
        } else if (strcmp(field, "COLLI_OBJECT") == 0) {
            mColli.fn_80058078(csv.fn_8005B9CC());
        } else if (strcmp(field, "EFP") == 0) {
            mEfp.fn_8023780C(csv.fn_8005B9CC());
        } else if (strcmp(field, "OPEN_DISTANCE") == 0) {
            mOpenDistance = fn_800DC288(csv.fn_8005B9CC());
        }
    } while (csv.fn_8005BBF8() != 0);
}

int OpenBoard::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    m54 = msg->mType;
    if ((unsigned int)m54 > 2) {
        m54 = 0;
    }
    return m54;
}

void OpenBoard::vf70(int state) {
    GimmickMsg_unk msg(state);
    vf40(&msg);
}

int OpenBoard::vf60() {
    return m54 == 1;
}

void OpenBoard::vf6C(void* menu) {}

Model_unk* OpenBoard::vf38() {
    return &mModel;
}

} // namespace gmk
