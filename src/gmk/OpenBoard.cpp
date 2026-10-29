#include "gmk/OpenBoard.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "chr/ControlBase.h"
#include "game/CharaManager.h"
#include "game/CharaTask.h"
#include "game/PlayerTask.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" char lbl_8087D710[2]; // inferred: " " (field separators of the tab CSV)
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" void fn_8008AD4C(atn::Model* model, const char* path, const char* texture); // inferred: loads a model
extern "C" void fn_80092F1C(atn::Model* model, const char* path, int arg, float lod); // inferred: adds a LOD model
extern "C" int fn_802180A8(const char* s); // inferred: motion id from its name
extern "C" void fn_80097A88(atn::Model* model, int id, const char* path); // inferred: loads a motion
// inferred: float variables in .sdata used only here (values 0)
extern "C" float lbl_8087DFB8;
extern "C" float lbl_8087DFBC;
// inferred: game settings
struct Settings_unk {
    unsigned char m00[0x18];
    int m18;
};
extern "C" Settings_unk* lbl_8087F0A8;
extern "C" void PSMTXInverse(const Mtx src, Mtx inv);
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
// inferred: camera (lbl_8087EFB4)
struct BoardCamera_unk {
    unsigned char m00[0x10C];
    atn::Vector3 mPos; // 0x10C
};
extern "C" BoardCamera_unk* lbl_8087EFB4;
// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk;
struct Color4f_unk {
    Color4f_unk(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
    float r, g, b, a;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" int fn_80239D58(EffectMgr_unk* mgr, void* owner, int id); // inferred: the effect is playing
extern "C" void fn_80232B7C(void* owner, int arg);
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
// inferred: plays an effect on a model (names invented)
// inferred: plays an effect on a model (the vectors and the color are locals; names invented)
inline void PlayEfp3_unk(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, atn::Model* model) {
    atn::Vector3 pos(0.0f, 0.0f, 0.0f);
    atn::Vector3 rot(0.0f, 0.0f, 0.0f);
    Color4f_unk color(1.0f, 1.0f, 1.0f, 1.0f);
    fn_8023A8B4(mgr, efp, model, 0, pos, rot, color, -1, -1, 1, 1.0f);
}
// inferred: next character of the player list (0x14AC, as in gmk/FlamingArrow)
struct BoardListNode_unk {
    unsigned char m0000[0x14AC];
    CharaTask* mNext;
};
inline float BoardAbs_unk(float x) {
    return x < 0.0f ? -x : x;
}
// inferred: bounding box (names invented)
struct BoardBox_unk {
    BoardBox_unk(atn::Vector3 min, atn::Vector3 max) : mMin(min), mMax(max) {}
    atn::Vector3 mMin;
    atn::Vector3 mMax;
};

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
    ((chr::Motion_unk*)&mModel)->fn_80097C08(0, 0, 0, 1, 1, 0.0f, 0.1f);
    mModel.mTracks[0].m0C = 0.0f;
    mModel.mTracks[0].m08 = 0.0f;
    m55C = 0.0f;
    m560 = 0.0f;
    m564 = 1.0f;
    m568 = false;
    m56C.Set_unk(lbl_8087DFBC);
    GimmickMsg_unk msg(m58 + 1);
    vf40(&msg);
}

// inferred: opens the board (motion frame) by the distance of the nearest player in front of it
// NONMATCHING: 94.7% - (1) the bounding box: the original copies res->mMin/mMax straight into the
// box and into the by-value arguments (here three more instructions through the stack); (2) float
// registers of the interpolation (m56C) and the order of the zero stores of the effect vectors.
// Tried: box by reference/by value/assignments/Assign_unk/component constructors, min/max locals,
// a GetBox helper, operand orders of the half width, frame/length/rate locals, the effect call
// with temporaries/locals/helpers, Start_unk by value/by reference, Interp_unk forms.
void OpenBoard::vf10() {
    if (m54 == 0) {
        vf28();
    }
    if (m54 == 2) {
        return;
    }
    atn::Model::Res_unk* res = mModel.m16C;
    BoardBox_unk box(res->mMin, res->mMax);
    float half = BoardAbs_unk(box.mMax.x - box.mMin.x) * 0.5f;
    atn::Matrix3x4 inv(*(atn::Matrix3x4*)mModel.mMtx);
    PSMTXInverse(inv.m, inv.m);
    float dist = mOpenDistance;
    if (lbl_8087F8A0 != 0) {
        half = half * 1.5f;
        float width = half;
        for (PlayerTask* player = lbl_8087F8A0->mHead; player != 0;
             player = (PlayerTask*)((BoardListNode_unk*)player)->mNext) {
            atn::Vector3 pos = player->mPos;
            PSMTXMultVec(inv.m, &pos, &pos);
            if (pos.x < width) {
                float z = BoardAbs_unk(pos.z);
                if (z < dist) {
                    dist = z;
                }
            }
        }
    } else {
        atn::Vector3 pos = lbl_8087EFB4->mPos;
        PSMTXMultVec(inv.m, &pos, &pos);
        float z = (float)__fabs(pos.z);
        if (z < dist) {
            dist = z;
        }
    }
    float frame = (1.0f - dist / mOpenDistance) * mModel.fn_80097D7C(0);
    float cur = mModel.mTracks[0].m08;
    if ((float)__fabs(frame - cur) > 0.01f && lbl_8087F3C0 != 0 &&
        fn_80239D58(lbl_8087F3C0, this, 0) == 0 && mEfp.mResource != 0) {
        fn_80232B7C(this, 0);
        PlayEfp3_unk(lbl_8087F3C0, &mEfp, &mModel);
    }
    Start_unk(m56C.mCur, frame);
    Update_unk();
    mModel.mTracks[0].m08 = m56C.mCur;
    if (vf38() != 0) {
        fn_803ED5D0(vf38());
    }
    if (vf38() != 0) {
        fn_803ED0D4(vf38(), 1);
    }
    atn::Matrix3x4 m;
    PSMTXRotRad(m.m, 'y', m78.y);
    m.m[0][3] = m6C.x;
    m.m[1][3] = m6C.y;
    m.m[2][3] = m6C.z;
    mColli.SetMtx_unk(m);
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

atn::Model* OpenBoard::vf38() {
    return &mModel;
}

} // namespace gmk
