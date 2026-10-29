#include "gmk/AutomaticDoor.h"
#include "atn/Csv.h"
#include "atn/LoadHandle.h"
#include "atn/Memory.h"
#include <boost/scoped_ptr.hpp>

extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int atoi(const char* s);
extern "C" char lbl_8087D710[2]; // inferred: " " (field separators of the tab CSV)
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" int fn_800DC12C(const char* s);   // inferred: string to int
extern "C" void fn_8008AD4C(atn::Model* model, const char* path, const char* texture); // inferred: loads a model
extern "C" int fn_802180A8(const char* s); // inferred: motion id from its name
extern "C" void fn_80097A88(atn::Model* model, int id, const char* path); // inferred: loads a motion
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" void PSMTXInverse(const Mtx src, Mtx inv);
extern "C" void PSMTXTranspose(const Mtx src, Mtx dst);
extern "C" void fn_800D5908(atn::Texture_unk* texture, const char* path); // inferred: loads a texture

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

// inferred: camera (layout as in game/CharaTask.cpp)
struct DoorView_unk {
    unsigned char m00[8];
    atn::Vector3 mEye;    // 0x08
    atn::Vector3 mTarget; // 0x14
};
struct Camera_unk {
    Camera_unk(int a = 0, int b = 0);
    ~Camera_unk();
    void fn_8004B378(); // inferred: updates the matrices
    int mType;            // 0x00
    int mFovType;         // 0x04
    atn::Vector3 mPos;    // 0x08
    atn::Vector3 mTarget; // 0x14
    atn::Vector3 mUp;     // 0x20
    atn::Vector3 mDir;    // 0x2C
    float m38;
    float m3C; // inferred: projection bounds
    float m40;
    float m44;
    float m48;
    unsigned char m4C[0x104 - 0x4C];
    DoorView_unk m104;
    unsigned char m124[0x1F4 - 0x124];
};
extern "C" Camera_unk* lbl_8087EFB4;
// inferred: camera owned by a model (as in game/CharaTask.cpp)
struct ModelCamera_unk {
    ModelCamera_unk() : m00(0) {}
    int m00;
    void* m04;
    Camera_unk mCamera; // 0x08
};
// inferred: inlined (its literals are @STRING@ objects in .sdata, shared with game/CharaTask.cpp)
inline void CreateCamera_unk(atn::Model* model) {
    model->m208.reset(new ((MemoryTag)6, "", "", 0) ModelCamera_unk());
}
// inferred: screen data of a sphere (result of fn_800C122C); names invented
struct ScreenInfo_unk {
    unsigned char m000[0x120];
    int m120;
};
extern "C" ScreenInfo_unk* fn_800C122C(Camera_unk* camera, const atn::Model::Sphere_unk* sphere);
extern "C" void fn_800C2C20(ScreenInfo_unk* info, const Plane_unk* plane); // inferred: sets the reflection plane
extern "C" int lbl_8087DF50; // inferred: reflections enabled
extern "C" atn::Vector3 fn_800BFAC8(Camera_unk* camera, const atn::Vector3* pos); // inferred: to screen
// inferred: field and HUD (classes unknown)
struct DoorField_unk {
    unsigned char m000[0x268];
    atn::Vector3 mCamPos;    // 0x268
    atn::Vector3 mCamTarget; // 0x274
    atn::Vector3 mCamUp;     // 0x280
    unsigned char m28C[0x868 - 0x28C];
    int m868; // inferred: mode (3 and 4 project the texture)
    int GetMode_unk() const { return m868; } // inferred: name invented
};
extern "C" DoorField_unk* lbl_8087F430;
struct DoorHud_unk {
    unsigned char m0000[0x10FC];
    int m10FC;
    float m1100;
};
extern "C" DoorHud_unk* lbl_8087F490;
// inferred: projection passed to fn_80095F10 (names invented)
struct DoorProject_unk {
    DoorProject_unk() : mType(0) {}
    int mType;
    atn::Texture_unk* mTexture;
    Camera_unk mCamera;
};
extern "C" void fn_80095F10(atn::Model* model, DoorProject_unk* project);

// inferred: normalizes unless the vector is (almost) zero (as in gmk::WaterLevel); name invented
inline void SafeNormalize_unk(atn::Vector3* v) {
    if ((float)__fabs(fn_805F9920(v)) < 1.1920929e-7f) {
        return;
    }
    PSVECNormalize(v, v);
}
inline void PlayEfp_unk(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model) {
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

namespace gmk {

GimmickCommon* AutomaticDoor::fn_803F471C(atn::Task* parent, int id, int arg) {
    if (parent != 0) {
        return new ((MemoryTag)12, "", "", 0) AutomaticDoor(parent, id, arg);
    }
    return 0;
}

AutomaticDoor::AutomaticDoor(atn::Task* parent, int id, int arg)
    : GimmickCommon(parent, id, arg), mModel(8, 32), mCloseFrame(0.0f), mOpenFrame(0.0f), mHasClose(0), mHasOpen(0),
      mPlaying(0), mKnob(0), mReflect(0), mReflectNode(-1), mImmediate(0), mImmediateNow(0), mProject(0) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            mParams[i].mMotions[j] = -1;
        }
    }
}

AutomaticDoor::~AutomaticDoor() {
    if (lbl_8087F3C0 != 0) {
        fn_80239DAC(lbl_8087F3C0, this, 1, 1);
    }
}

// NONMATCHING: 99.81% - the ready flag gets r30 instead of r31 (tried int/bool and an inline test)
int AutomaticDoor::vf0C() {
    if (fn_803EC91C() == 0) {
        fn_803EDB18(&mModel);
        int ready = 0;
        if (mTexture.mImage != 0 ||
            (((GimmickUvOwner_unk*)&mTexture)->fn_800D59B8() == 0 &&
             ((atn::LoadHandle*)&mTexture.m20)->fn_80473EFC() == 3)) {
            ready = 1;
        }
        if (ready != 0) {
            CreateCamera_unk(&mModel);
        }
        if (strlen(mBoundNode.mStr) != 0 && mModel.mB0 < 0) {
            mModel.fn_80095D44(mModel.GetNodeNoByName(mBoundNode.mStr, 0), -1);
        }
        return 1;
    }
    return 0;
}

void AutomaticDoor::Start() {
    vf2C();
    if (vf38() != 0) {
        fn_803ED0D4(vf38(), 1);
    }
    mColliClose.SetMtx_unk(*(atn::Matrix3x4*)mModel.mMtx);
    mColliOpen.SetMtx_unk(*(atn::Matrix3x4*)mModel.mMtx);
    if (mHasClose != 0) {
        mColliClose.mFlags |= 1;
    } else {
        mColliClose.mFlags &= ~1;
    }
    mColliOpen.mFlags &= ~1;
    if (m58 == 0) {
        m54 = 1;
        GimmickMsg_unk msg(m54);
        vf40(&msg);
    } else if (m58 == 1) {
        m54 = 3;
        GimmickMsg_unk msg(m54);
        vf40(&msg);
    } else if (m58 == 2) {
        m54 = 4;
        GimmickMsg_unk msg(m54);
        vf40(&msg);
    }
}

void AutomaticDoor::vf10() {
    if (m54 == 0) {
        Start();
    } else if (m54 == 2) {
        if (mParams[0].mMotions[1] >= 0) {
            GimmickMsg_unk msg(3);
            m54 = 1;
            vf40(&msg);
        } else if (mParams[2].mMotions[1] >= 0) {
            GimmickMsg_unk msg(3);
            m54 = 4;
            vf40(&msg);
        }
    } else if (m54 == 3) {
        if ((mModel.mFlags & 4) == 4) {
            float frame = mModel.mTracks[0].m08;
            if (frame >= mModel.fn_80097D7C(0)) {
                mModel.mFlags &= ~4;
            }
        }
    }
    if (vf38() != 0) {
        fn_803ED0D4(vf38(), 1);
    }
    mColliClose.SetMtx_unk(*(atn::Matrix3x4*)mModel.mMtx);
    mColliOpen.SetMtx_unk(*(atn::Matrix3x4*)mModel.mMtx);
    if (mPlaying != 0 && vf38() != 0) {
        fn_803ED5D0(vf38());
    }
    fn_803F53E0();
    if (mImmediateNow != 0) {
        if (m54 == 1) {
            mColliClose.mFlags |= 1;
            mColliOpen.mFlags &= ~1;
            mOpen = 0;
        } else {
            mColliClose.mFlags &= ~1;
            mColliOpen.mFlags |= 1;
            mOpen = 1;
        }
    } else if (m54 == 1) {
        if (mModel.mTracks[0].m0C > 0.0f) {
            float frame = mModel.mTracks[0].m08;
            if (frame < mModel.fn_80097D7C(0) - mCloseFrame) {
                mColliClose.mFlags &= ~1;
            } else {
                if (mHasClose != 0) {
                    mColliClose.mFlags |= 1;
                } else {
                    mColliClose.mFlags &= ~1;
                }
                mOpen = 0;
            }
            frame = mModel.mTracks[0].m08;
            if (frame < mModel.fn_80097D7C(0) - mOpenFrame) {
                if (mHasOpen != 0) {
                    mColliOpen.mFlags |= 1;
                } else {
                    mColliOpen.mFlags &= ~1;
                }
            } else {
                mColliOpen.mFlags &= ~1;
            }
        } else {
            if (mModel.mTracks[0].m08 < mCloseFrame) {
                if (mHasClose != 0) {
                    mColliClose.mFlags |= 1;
                } else {
                    mColliClose.mFlags &= ~1;
                }
                mOpen = 0;
            } else {
                mColliClose.mFlags &= ~1;
            }
            if (mModel.mTracks[0].m08 < mOpenFrame) {
                mColliOpen.mFlags &= ~1;
            } else if (mHasOpen != 0) {
                mColliOpen.mFlags |= 1;
            } else {
                mColliOpen.mFlags &= ~1;
            }
        }
    } else if (m54 == 3) {
        if (mModel.mTracks[0].m08 < mCloseFrame) {
            if (mHasClose != 0) {
                mColliClose.mFlags |= 1;
            } else {
                mColliClose.mFlags &= ~1;
            }
        } else {
            mColliClose.mFlags &= ~1;
            mOpen = 1;
        }
        if (mModel.mTracks[0].m08 < mOpenFrame) {
            mColliOpen.mFlags &= ~1;
        } else if (mHasOpen != 0) {
            mColliOpen.mFlags |= 1;
        } else {
            mColliOpen.mFlags &= ~1;
        }
    }
    if (mReflect != 0 && mReflectNode != -1) {
        Camera_unk* camera = lbl_8087EFB4;
        ScreenInfo_unk* info = fn_800C122C(camera, &mModel.fn_80094F98());
        if (info != 0 && lbl_8087DF50 != 0) {
            Plane_unk plane = mPlane;
            MtxPtr mtx = mModel.GetNodeMtx_unk(mReflectNode);
            atn::Matrix3x4 m(*(atn::Matrix3x4*)mtx);
            PSMTXInverse(m.m, m.m);
            PSMTXTranspose(m.m, m.m);
            SafeNormalize_unk(&plane.mNormal);
            PSMTXMultVec(m.m, &plane.mNormal, &plane.mNormal);
            SafeNormalize_unk(&plane.mNormal);
            atn::Vector3 trans = ((atn::Matrix3x4*)mtx)->GetTrans_unk();
            plane.mDist = -PSVECDotProduct(&plane.mNormal, &trans);
            info->m120 = 1;
            fn_800C2C20(info, &plane);
        }
    }
}

// NONMATCHING: approximate - projection of the texture with a camera placed in front of the door
// (stack layout of the cameras and vectors, register allocation)
void AutomaticDoor::fn_803F53E0() {
    if (mProject == 0 || lbl_8087F430 == 0) {
        return;
    }
    if (lbl_8087F490 == 0) {
        return;
    }
    Camera_unk camera(0, 1);
    DoorField_unk* field = lbl_8087F430;
    if (lbl_8087F490->m10FC != 0 && (field->GetMode_unk() == 4 || field->GetMode_unk() == 3)) {
        float left = -1.0f;
        float bottom = left;
        float right = 1.0f;
        float top = right;
        atn::Vector3 dir;
        dir = lbl_8087EFB4->m104.mTarget - lbl_8087EFB4->m104.mEye;
        PSVECNormalize(&dir, &dir);
        atn::Vector3 pos;
        atn::Vector3 front(0.0f, 0.0f, 1.0f);
        pos = m6C;
        Mtx mtx;
        PSMTXRotRad(mtx, 'y', m78.y);
        PSMTXMultVec(mtx, &front, &front);
        pos += front * 11.5f;
        float d = -PSVECDotProduct(&front, &dir);
        if (d < 0.2f) {
            mModel.mFlags &= ~0x80000;
            return;
        }
        atn::Vector3 eye(lbl_8087EFB4->m104.mEye);
        float t = (front.x * (pos.x - eye.x) + front.y * (pos.y - eye.y) + front.z * (pos.z - eye.z)) / d;
        atn::Vector3 hit = eye - dir * t;
        atn::Vector3 p0;
        p0 = fn_800BFAC8(lbl_8087EFB4, &hit);
        atn::Vector3 up(0.0f, 1.0f, 0.0f);
        atn::Vector3 p1;
        atn::Vector3 top_pos = hit + up;
        p1 = fn_800BFAC8(lbl_8087EFB4, &top_pos);
        if (p0.z < 0.0f || 1.0f < p0.z || p1.z < 0.0f || 1.0f < p1.z) {
            mModel.mFlags &= ~0x80000;
            return;
        }
        float scale = 1e-05f;
        float dy = __fabs(p1.y - p0.y);
        if (dy > 0.0f) {
            scale = lbl_8087F490->m1100 / dy;
        }
        left *= scale;
        right *= scale;
        bottom *= scale;
        top *= scale;
        camera.mUp = field->mCamUp;
        camera.mPos = field->mCamPos;
        camera.mTarget = field->mCamTarget;
        camera.m3C = top;
        camera.m40 = bottom;
        camera.m44 = left;
        camera.m48 = right;
        camera.fn_8004B378();
        DoorProject_unk project;
        project.mCamera = camera;
        project.mType = 3;
        project.mTexture = &mTexture;
        fn_80095F10(&mModel, &project);
    } else {
        mModel.mFlags &= ~0x80000;
    }
}

void AutomaticDoor::vf14() {
    if (lbl_8087F0A8->m18 != 0 && vf38() != 0) {
        fn_803ED774(vf38());
    }
}

int AutomaticDoor::vf1C() {
    if (mModel.fn_8008B140() != 0) {
        return 1;
    }
    if (mColliClose.fn_800580BC() != 0 || mColliOpen.fn_800580BC() != 0) {
        return 1;
    }
    if (((GimmickUvOwner_unk*)&mTexture)->fn_800D59B8() != 0) {
        return 1;
    }
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (mParams[i].mEfp[j].IsLoading() != 0) {
                return 1;
            }
        }
    }
    return 0;
}

void AutomaticDoor::vf20() {
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
        } else if (strcmp(field, "MOTION") == 0) {
            strcpy(path, csv.fn_8005B9CC());
            fn_80097A88(&mModel, fn_802180A8(csv.fn_8005B9CC()), path);
        } else if (strcmp(field, "COLLISION_BEFORE") == 0) {
            mCloseFrame = fn_800DC288(csv.fn_8005B9CC());
            mColliClose.fn_80058078(csv.fn_8005B9CC());
            mHasClose = 1;
        } else if (strcmp(field, "COLLISION_AFTER") == 0) {
            mOpenFrame = fn_800DC288(csv.fn_8005B9CC());
            mColliOpen.fn_80058078(csv.fn_8005B9CC());
            mHasOpen = 1;
        } else if (strncmp(field, "PARAM_CLOSE", strlen("PARAM_CLOSE")) == 0) {
            field += strlen("PARAM_CLOSE");
            if (strcmp(field, "_MOTION") == 0) {
                for (int i = 0; i < 3; i++) {
                    mParams[0].mMotions[i] = fn_802180A8(csv.fn_8005B9CC());
                }
            } else if (strcmp(field, "_EFF") == 0) {
                for (int i = 0; i < 3; i++) {
                    if (strlen(csv.fn_8005B9CC()) != 0) {
                        mParams[0].mEfp[i].LoadRequest(csv.mField);
                    }
                }
            }
        } else if (strncmp(field, "PARAM_OPENHALF", strlen("PARAM_OPENHALF")) == 0) {
            field += strlen("PARAM_OPENHALF");
            if (strcmp(field, "_MOTION") == 0) {
                for (int i = 0; i < 3; i++) {
                    mParams[2].mMotions[i] = fn_802180A8(csv.fn_8005B9CC());
                }
            } else if (strcmp(field, "_EFF") == 0) {
                for (int i = 0; i < 3; i++) {
                    if (strlen(csv.fn_8005B9CC()) != 0) {
                        mParams[2].mEfp[i].LoadRequest(csv.mField);
                    }
                }
            }
        } else if (strncmp(field, "PARAM_OPEN", strlen("PARAM_OPEN")) == 0) {
            field += strlen("PARAM_OPEN");
            if (strcmp(field, "_MOTION") == 0) {
                for (int i = 0; i < 3; i++) {
                    mParams[1].mMotions[i] = fn_802180A8(csv.fn_8005B9CC());
                }
            } else if (strcmp(field, "_EFF") == 0) {
                for (int i = 0; i < 3; i++) {
                    if (strlen(csv.fn_8005B9CC()) != 0) {
                        mParams[1].mEfp[i].LoadRequest(csv.mField);
                    }
                }
            }
        } else if (strcmp(field, "COLLI_IMEDIATE") == 0) {
            mImmediate = fn_800DC12C(csv.fn_8005B9CC());
        } else if (strcmp(field, "PROJECT_EFFECT") == 0) {
            fn_800D5908(&mTexture, csv.fn_8005B9CC());
            mProject = 1;
        } else if (strcmp(field, "BOUND_CENTER_NODE") == 0) {
            mBoundNode.Set_unk(csv.fn_8005B9CC());
        }
    } while (csv.fn_8005BBF8() != 0);
}

void AutomaticDoor::vf24() {
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    csv.fn_8005BCE8(lbl_8087D710);
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "REFLECT_PLANE") == 0) {
            mReflect = 1;
            mPlane.mNormal.x = fn_800DC288(csv.fn_8005B9CC());
            mPlane.mNormal.y = fn_800DC288(csv.fn_8005B9CC());
            mPlane.mNormal.z = fn_800DC288(csv.fn_8005B9CC());
            mPlane.mDist = fn_800DC288(csv.fn_8005B9CC());
            mReflectNode = mModel.GetNodeNoByName(csv.fn_8005B9CC(), 0);
        } else if (strcmp(field, "KNOB_OFFSET") == 0) {
            mKnob = atoi(csv.fn_8005B9CC());
        }
    } while (csv.fn_8005BBF8() != 0);
}

int AutomaticDoor::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    switch (msg->mType) {
    case 1:
        if (m54 == 1) {
            fn_803F6448(0, 0, 1, 1.0f, 0.0f);
        } else if (m54 == 4) {
            fn_803F6448(2, 0, 0, 1.0f, 0.0f);
        } else {
            fn_803F6448(1, 0, 0, 1.0f, 0.0f);
        }
        break;
    case 3:
        mImmediateNow = mImmediate == msg->mType;
        if (m54 == 3) {
            fn_803F6448(1, 1, 1, 1.0f, 0.0f);
        } else if (m54 == 4) {
            fn_803F6448(2, 1, 0, 1.0f, 0.0f);
        } else {
            fn_803F6448(0, 1, 0, 1.0f, 0.0f);
        }
        break;
    case 4:
        mImmediateNow = mImmediate == msg->mType;
        if (m54 == 3) {
            fn_803F6448(1, 2, 0, 1.0f, 0.0f);
        } else if (m54 == 4) {
            fn_803F6448(2, 2, 1, 1.0f, 0.0f);
        } else {
            fn_803F6448(0, 2, 0, 1.0f, 0.0f);
        }
        break;
    }
    return m54 = msg->mType;
}

void AutomaticDoor::fn_803F6448(int from, int to, int loop, float speed, float frame) {
    DoorParam_unk* param = &mParams[from];
    if (param->mMotions[to] != -1) {
        mModel.fn_80097C08(0, param->mMotions[to], loop, 0, 1, 0.0f, 0.1f);
        mModel.mTracks[0].m08 = frame;
        mModel.mTracks[0].m0C = speed;
        mPlaying = 1;
    } else if (from == to) {
        switch (from) {
        case 0: {
            int motion = mParams[0].mMotions[2];
            if (motion == -1) {
                motion = mParams[0].mMotions[1];
            }
            if (motion != -1) {
                mModel.fn_80097C08(0, motion, 0, 0, 1, 1.0f, 0.1f);
                mModel.mTracks[0].m08 = 0.0f;
                mModel.mTracks[0].m0C = 0.0f;
                if (vf38() != 0) {
                    fn_803ED5D0(vf38());
                }
                mPlaying = 0;
            }
            mOpen = 0;
            break;
        }
        case 1: {
            int motion = mParams[0].mMotions[1];
            if (motion == -1) {
                motion = mParams[2].mMotions[1];
            }
            if (motion != -1) {
                mModel.fn_80097C08(0, motion, 0, 0, 1, 1.0f, 0.1f);
                mModel.mTracks[0].m08 = mModel.fn_80097D7C(0);
                mModel.mTracks[0].m0C = 1.0f;
                if (vf38() != 0) {
                    fn_803ED5D0(vf38());
                }
                mPlaying = 0;
            }
            mOpen = 1;
            break;
        }
        case 2: {
            int motion = mParams[0].mMotions[2];
            if (motion != -1) {
                mModel.fn_80097C08(0, motion, 0, 0, 1, 1.0f, 0.1f);
                mModel.mTracks[0].m08 = mModel.fn_80097D7C(0);
                mModel.mTracks[0].m0C = 1.0f;
                if (vf38() != 0) {
                    fn_803ED5D0(vf38());
                }
                mPlaying = 0;
            }
            mOpen = 0;
            break;
        }
        }
    } else if (from == 1 && to == 0) {
        int motion = mParams[0].mMotions[1];
        if (motion == -1) {
            motion = mParams[0].mMotions[2];
        }
        if (motion != -1) {
            mModel.fn_80097C08(0, motion, 0, 0, 1, 0.0f, 0.1f);
            mModel.mTracks[0].m08 = mModel.fn_80097D7C(0);
            mModel.mTracks[0].m0C = -1.0f;
            mPlaying = 1;
        }
    } else if (from == 2 && to == 0) {
        int motion = mParams[0].mMotions[2];
        if (motion != -1) {
            mModel.fn_80097C08(0, motion, 0, 0, 1, 0.0f, 0.1f);
            mModel.mTracks[0].m08 = mModel.fn_80097D7C(0);
            mModel.mTracks[0].m0C = -1.0f;
            mPlaying = 1;
        }
    } else if (from == 0 && to == 1) {
        mModel.mTracks[0].m08 = mModel.fn_80097D7C(0);
        mModel.mTracks[0].m0C = 1.0f;
        mPlaying = 1;
    }
    if (to == 0 && from == to) {
        lbl_8087F3C0->mB8 = 1;
    }
    fn_80239DAC(lbl_8087F3C0, this, 1, 1);
    fn_80232B7C(this, 1);
    PlayEfp_unk(lbl_8087F3C0, &param->mEfp[to], &mModel);
    if (to == 0 && from == to) {
        lbl_8087F3C0->mB8 = 0;
    }
}

void AutomaticDoor::vf70(int state) {
    GimmickMsg_unk msg;
    switch (state) {
    case 0:
    case 1:
    case 2:
        msg.mType = m54 = 1;
        vf40(&msg);
        break;
    case 3:
        msg.mType = m54 = 3;
        vf40(&msg);
        break;
    case 4:
        msg.mType = m54 = 4;
        vf40(&msg);
        break;
    }
    if (mE4 != 0) {
        mE4->m58 = 1;
    }
}

int AutomaticDoor::vf64(int arg) {
    switch (arg) {
    case 0:
        return mOpen;
    case 9:
        return mKnob;
    }
    return 0;
}

void AutomaticDoor::vf3C(void* lens) {
    mModel.m1DC = (Lens_unk*)lens;
    mModel.mFlags |= 0x10000;
}

atn::Model* AutomaticDoor::vf38() {
    return &mModel;
}

} // namespace gmk
