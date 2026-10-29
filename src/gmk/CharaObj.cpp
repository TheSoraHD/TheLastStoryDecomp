#include "gmk/CharaObj.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "std/string_members.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" char lbl_8087D710[2]; // inferred: " " (field separators of the tab CSV)
extern "C" int fn_8006AA20(const char* path); // inferred: the file exists
extern "C" void fn_80097A88(atn::Model* model, int id, const char* path); // inferred: loads a motion
// inferred: plays a face motion on CharaTask::m1188
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_80087E9C(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void fn_80088724(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
// inferred: empty names passed to operator new (in .sdata of another file)
extern "C" char lbl_8087DFC0[4];
extern "C" char lbl_8087DFC4[4];
// inferred: game settings
struct Settings_unk {
    unsigned char m00[0x74];
    int m74; // inferred: debug display
};
extern "C" Settings_unk* lbl_8087F0A8;

namespace gmk {

GimmickCommon* CharaObj::fn_8042CE30(atn::Task* parent, int id, int arg) {
    if (parent != 0) {
        return new (12, "", "", 0) CharaObj(parent, id, arg);
    }
    return 0;
}

CharaObj::~CharaObj() {}

int CharaObj::vf0C() {
    if (fn_803EC91C() == 0) {
        if (mChara != 0) {
            if (mChara->IsFlag_unk(2)) {
                mChara->fn_800D246C(0);
                return 1;
            }
            return 0;
        }
        return 1;
    }
    return 0;
}

void CharaObj::vf28() {
    vf2C();
    GimmickMsg_unk msg(1);
    vf40(&msg);
    if (mChara != 0) {
        mChara->mPos = m6C;
        mChara->mRot = m78;
        mChara->mMotion.m34C = 1;
        mChara->mMotion.mTracks[0].m20 = 1.0f;
        mChara->mMotion.mTracks[0].m0C = 1.0f;
        mChara->mMotion.fn_80097C08(0, 0x13F, 1, 0, 1, 1.0f, 0.1f);
        if (mHasFace != 0) {
            mChara->m1188.fn_8011E81C(0x142, 1, 1, 1, 1.0f, 0.0f);
        }
    }
}

// NONMATCHING: 52.5% - same as gmk::TodomeCountUp::vf10: the target keeps a stack frame and calls
// vf28 with bctrl instead of a tail call. Tried: return, for/break, switch, else branches, locals.
void CharaObj::vf10() {
    if (m54 == 0) {
        vf28();
    }
}

void CharaObj::vf14() {
    if (m9C != 0 && lbl_8087F0A8->m74 != 0) {
        fn_803EDCF4();
    }
}

int CharaObj::vf1C() {
    return 0;
}

void CharaObj::vf20() {
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    char path[0x100];
    csv.fn_8005BCE8(lbl_8087D710);
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "CHARA") == 0) {
            strcpy(path, csv.fn_8005B9CC());
            if (fn_8006AA20(path) != 0) {
                {
                    std::string name(path);
                    mChara = this != 0 ? new (1, lbl_8087DFC4, lbl_8087DFC0, 0) GmkChara(this, name) : 0;
                }
                mChara->fn_800D246C(1);
            }
        } else if (strcmp(field, "MOTION") == 0) {
            if (mChara != 0) {
                strcpy(path, csv.fn_8005B9CC());
                fn_80097A88(&mChara->mMotion, 0x13F, path);
            }
        } else if (strcmp(field, "FACE_MOTION") == 0) {
            if (mChara != 0) {
                strcpy(path, csv.fn_8005B9CC());
                fn_80097A88(&mChara->mMotion, 0x142, path);
                mHasFace = 1;
            }
        }
    } while (csv.fn_8005BBF8() != 0);
}

int CharaObj::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    m54 = msg->mType;
    if ((unsigned int)m54 > 1) {
        m54 = 0;
    }
    return m54;
}

void CharaObj::vf70(int state) {
    GimmickMsg_unk msg(state);
    vf40(&msg);
}

int CharaObj::vf60() {
    return m54 == 1;
}

void CharaObj::vf6C(void* menu) {
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
}

} // namespace gmk
