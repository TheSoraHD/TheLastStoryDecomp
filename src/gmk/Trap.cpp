#include "gmk/Trap.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "game/CharaManager.h"
#include "game/CharaTask.h"
#include "game/PlayerTask.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" char lbl_8087D710[2]; // inferred: " " (field separators of the tab CSV)
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" void fn_8008AD4C(atn::Model* model, const char* path, const char* texture); // inferred: loads a model
extern "C" void fn_80092F1C(atn::Model* model, const char* path, int arg, float lod); // inferred: adds a LOD model
extern "C" int fn_802180A8(const char* s); // inferred: motion id from its name
extern "C" void fn_80097A88(atn::Model* model, int id, const char* path); // inferred: loads a motion
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
// inferred: game settings (the pad state is at 0x48C)
struct Input_unk {
    int fn_8012476C(int button); // inferred: nonzero while `button` is pressed
};
struct Settings_unk {
    unsigned char m00[0x48C];
    Input_unk mInput; // 0x48C
};
extern "C" Settings_unk* lbl_8087F0A8;
// inferred: global object (class unknown)
struct TrapFlag_unk {
    unsigned char m000[0x728];
    int m728; // inferred: set while a character is caught
};
extern "C" TrapFlag_unk* lbl_8087F490;

namespace gmk {

GimmickCommon* Trap::fn_80410A68(atn::Task* parent, GimmickDesc_unk* desc) {
    if (parent != 0) {
        return new (12, "", "", 0) Trap(parent, desc);
    }
    return 0;
}

Trap::~Trap() {}

int Trap::vf0C() {
    return fn_803EC91C() == 0;
}

int Trap::vf1C() {
    return mModel.fn_8008B140() != 0;
}

void Trap::vf20() {
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
        } else if (strcmp(field, "PARENT_NODE") == 0) {
            strcpy(mNode, csv.fn_8005B9CC());
        }
    } while (csv.fn_8005BBF8() != 0);
}

void Trap::Start() {
    m6C = mDesc->mPos;
    m78.y = mDesc->mRotY;
    m78.x = 0.0f;
    m78.z = 0.0f;
    mDamage = 0.01f * mDesc->m40;
    if (vf38() != 0) {
        fn_803ED0D4(vf38(), 1);
    }
    m54 = 1;
}

void Trap::vf10() {
    if (m54 == 0) {
        Start();
    }
    if (m54 == 0 || m54 == 1 || m54 == 6) {
        return;
    }
    if (mPlayer != 0) {
        MtxPtr mtx = mPlayer->mMotion.GetNodeMtx_unk(mNode);
        if (mtx != 0) {
            m6C = ((atn::Matrix3x4*)mtx)->GetTrans_unk();
        }
    }
    if (vf38() != 0) {
        fn_803ED0D4(vf38(), 1);
    }
    float frame;
    switch (m54) {
    case 2:
        if (vf38() != 0) {
            fn_803ED5D0(vf38());
        }
        frame = mModel.mTracks[0].m08;
        if (frame >= mModel.fn_80097D7C(0)) {
            GimmickMsg_unk msg(3);
            vf40(&msg);
        }
        break;
    case 3:
        lbl_8087F490->m728 = 1;
        if (vf38() != 0) {
            fn_803ED5D0(vf38());
        }
        if (lbl_8087F0A8->mInput.fn_8012476C(0x22)) {
            mCount--;
        }
        if (mPlayer != 0 && (mPlayer->mStatus.m0C & 0x20) == 0x20) {
            GimmickMsg_unk msg(5);
            vf40(&msg);
        } else if (mCount < 0) {
            GimmickMsg_unk msg(4);
            vf40(&msg);
        }
        break;
    case 4:
    case 5:
        if (vf38() != 0) {
            fn_803ED5D0(vf38());
        }
        frame = mModel.mTracks[0].m08;
        if (frame >= mModel.fn_80097D7C(0)) {
            GimmickMsg_unk msg(6);
            vf40(&msg);
        }
        break;
    }
}

void Trap::vf14() {
    if (m54 == 0 || m54 == 1 || m54 == 6) {
        return;
    }
    if (vf38() != 0) {
        fn_803ED774(vf38());
    }
}

int Trap::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    switch (msg->mType) {
    case 2:
        mPlayer = lbl_8087F8A0->mHead;
        if (mPlayer != 0) {
            mPlayer->fn_8016A814(mDamage);
        }
        mModel.fn_80097C08(0, 0, 0, 0, 1, 0.0f, 0.1f);
        mModel.mTracks[0].m0C = 1.0f;
        break;
    case 3:
        mCount = 30;
        mModel.fn_80097C08(0, 1, 1, 0, 1, 0.0f, 0.1f);
        mModel.mTracks[0].m0C = 1.0f;
        break;
    case 4:
        if (mPlayer != 0) {
            mPlayer->fn_8016EB48(0, 0, 0);
        }
        mModel.fn_80097C08(0, 2, 0, 0, 1, 0.0f, 0.1f);
        mModel.mTracks[0].m0C = 1.0f;
        break;
    case 5:
        mModel.fn_80097C08(0, 2, 0, 0, 1, 0.0f, 0.1f);
        mModel.mTracks[0].m0C = 1.0f;
        break;
    case 6:
        mPlayer = 0;
        break;
    }
    return m54 = msg->mType;
}

atn::Model* Trap::vf38() {
    return &mModel;
}

} // namespace gmk
