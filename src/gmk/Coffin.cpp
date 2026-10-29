#include "gmk/Coffin.h"
#include "atn/Memory.h"
#include "game/CharaManager.h"
#include "game/CharaTask.h"
#include "game/EnemyManager.h"
#include "game/EnemyTask.h"
#include "game/PlayerTask.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" char lbl_8087D710[2]; // inferred: field separators of the tab CSV
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" void fn_8008AD4C(atn::Model* model, const char* path, const char* texture); // inferred: loads a model
extern "C" void fn_80092F1C(atn::Model* model, const char* path, int arg, float lod); // inferred: adds a LOD model
extern "C" int fn_802180A8(const char* s); // inferred: motion id from its name
extern "C" void fn_80097A88(atn::Model* model, int id, const char* path); // inferred: loads a motion

// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk;
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80232B7C(void* owner, int arg);
// inferred: field counters (lbl_8087F430)
struct CoffinField_unk;
extern "C" CoffinField_unk* lbl_8087F430;
extern "C" int fn_80370A78(CoffinField_unk* field, int index);             // inferred: reads a counter
extern "C" void fn_80370AE4(CoffinField_unk* field, int index, int value); // inferred: sets a counter
extern "C" int fn_80219558(int id); // inferred: party member kind of a character id (negative for enemies)
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
// inferred: bullets
struct BulletMgr_unk;
extern "C" BulletMgr_unk* lbl_8087F048;
struct HitInfo_unk;
extern "C" HitInfo_unk* fn_80219E6C(int id); // inferred: bullet data by id
extern "C" int fn_800F8548(BulletMgr_unk* mgr);
extern "C" void fn_800FAB80(BulletMgr_unk* mgr, int arg1, HitInfo_unk* bullet, int arg3, const atn::Vector3* pos,
                            const atn::Vector3* rot, int arg6, int arg7, int arg8, int arg9, float arg10,
                            float arg11);
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);

// inferred: field counter of the character inside each coffin (by GimmickCommon::m4C)
struct CoffinSlot_unk {
    int mArg;
    int mCounter;
};
static const CoffinSlot_unk sSlots[] = {
    {0, 0}, {1, 0xC8}, {2, 0xC9}, {3, 0xCA}, {4, 0xCB}, {5, 0xCC}, {6, 0xCD}, {-1, -1},
};

// inferred: copy of a position raised by dy (name invented)
inline atn::Vector3 OffsetY_unk(const atn::Vector3& v, float dy) {
    atn::Vector3 r(v);
    r.y += dy;
    return r;
}

// inferred: plays an effect at the model (name invented)
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

// inferred: name invented
inline int FindCounter_unk(int arg) {
    for (int i = 0; sSlots[i].mArg >= 0; i++) {
        if (arg == sSlots[i].mArg) {
            return sSlots[i].mCounter;
        }
    }
    return -1;
}

namespace gmk {

inline void Coffin::GetUp_unk() {
    if (mChara != 0) {
        atn::Vector3 pos(m6C);
        pos.y += 3.0f;
        atn::Vector3 rot(0.0f, m78.y, 0.0f);
        if (mSide == 0) {
            rot.y += 3.1415927f;
        }
        mChara->fn_8016A504(pos, rot);
    }
}

GimmickCommon* Coffin::fn_80411440(atn::Task* parent, GimmickDesc_unk* desc) {
    if (parent != 0) {
        return new (12, "", "", 0) Coffin(parent, desc);
    }
    return 0;
}

Coffin::Coffin(atn::Task* parent, GimmickDesc_unk* desc)
    : GimmickCommon(parent, desc->mId, desc->mArg), mModel(8, 32), mDesc(desc), mTimer(0), mSide(0), mCount(0),
      mKind(0), mChara(0) {
    m54 = 0;
}

Coffin::~Coffin() {}

int Coffin::vf1C() {
    if (mModel.fn_8008B140() != 0 || mColli.fn_800580BC() != 0 || mColliAfter[0].fn_800580BC() != 0 ||
        mColliAfter[1].fn_800580BC() != 0 || mOpenEfp[0].fn_80237874() != 0 || mOpenEfp[1].fn_80237874() != 0 ||
        mEfp.fn_80237874() != 0) {
        return 1;
    }
    return 0;
}

void Coffin::vf20() {
    char path[256];
    char motion[256];
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
        } else if (strcmp(field, "MOTION") == 0) {
            strcpy(motion, csv.fn_8005B9CC());
            fn_80097A88(&mModel, fn_802180A8(csv.fn_8005B9CC()), motion);
        } else if (strcmp(field, "COLLI_BEFORE") == 0) {
            mColli.fn_80058078(csv.fn_8005B9CC());
        } else if (strcmp(field, "COLLI_AFTER_LEFT") == 0) {
            mColliAfter[0].fn_80058078(csv.fn_8005B9CC());
        } else if (strcmp(field, "COLLI_AFTER_RIGHT") == 0) {
            mColliAfter[1].fn_80058078(csv.fn_8005B9CC());
        } else if (strcmp(field, "EFP") == 0) {
            mEfp.fn_8023780C(csv.fn_8005B9CC());
        } else if (strcmp(field, "OPEN_EFP_LEFT") == 0) {
            mOpenEfp[0].fn_8023780C(csv.fn_8005B9CC());
        } else if (strcmp(field, "OPEN_EFP_RIGHT") == 0) {
            mOpenEfp[1].fn_8023780C(csv.fn_8005B9CC());
        }
    } while (csv.fn_8005BBF8() != 0);
}

void Coffin::vf24() {
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    csv.fn_8005BCE8(lbl_8087D710);
    do {
        csv.fn_8005B9CC();
    } while (csv.fn_8005BBF8() != 0);
}

int Coffin::vf0C() {
    if (fn_803EC91C() == 0) {
        fn_803EDB18(&mModel);
        return 1;
    }
    return 0;
}

void Coffin::Start() {
    m6C = mDesc->mPos;
    m78.y = mDesc->mRotY;
    m78.x = 0.0f;
    m78.z = 0.0f;
    if (vf38() != 0) {
        fn_803ED0D4(vf38(), 1);
    }
    GimmickMsg_unk msg(1);
    vf40(&msg);
}

void Coffin::vf10() {
    if (m54 == 0) {
        Start();
    } else if (m54 == 2) {
        GimmickMsg_unk msg(3);
        vf40(&msg);
        if (vf38() != 0) {
            fn_803ED0D4(vf38(), 1);
        }
    }
    if (vf38() != 0) {
        fn_803ED0D4(vf38(), 1);
    }
    mColli.mFlags &= ~1;
    mColliAfter[0].mFlags &= ~1;
    mColliAfter[1].mFlags &= ~1;
    if ((unsigned int)(m54 - 3) <= 3) {
        mColliAfter[mSide].mFlags |= 1;
        mColliAfter[mSide].SetMtx_unk(*(atn::Matrix3x4*)mModel.mMtx);
    } else {
        mColli.mFlags |= 1;
        mColli.SetMtx_unk(*(atn::Matrix3x4*)mModel.mMtx);
    }
    switch (m54) {
    case 3: {
        int n = fn_80370A78(lbl_8087F430, 3);
        if (--mCount < 0) {
            fn_80370AE4(lbl_8087F430, 3, n + 1);
            GimmickMsg_unk msg(4);
            vf40(&msg);
        }
        if (vf38() != 0) {
            fn_803ED5D0(vf38());
        }
        break;
    }
    case 4:
        if (vf38() != 0) {
            fn_803ED5D0(vf38());
        }
        if (mKind == 0 && mTimer > 0 && --mTimer == 0) {
            fn_80232B7C(0, 0);
            PlayEfp_unk(lbl_8087F3C0, &mEfp, &mModel);
        }
        {
            float frame = mModel.mTracks[0].m08;
            if (frame >= mModel.fn_80097D7C(0)) {
                int open = 0;
                if (mKind == 0) {
                    HitInfo_unk* bullet = fn_80219E6C(4004);
                    if (bullet != 0) {
                        fn_800C344C("SE_MB003_003", &m6C, 0, -1, 1.0f);
                        int id = fn_800F8548(lbl_8087F048);
                        atn::Vector3 pos(m6C);
                        pos.y += 20.0f;
                        fn_800FAB80(lbl_8087F048, 0, bullet, id, &pos, &m78, 0, 30, -1, 2, 3.0f, 1.0f);
                    }
                    open = 1;
                } else if (mChara != 0 && mChara->fn_8016A7EC()) {
                    open = 1;
                }
                if (open != 0) {
                    GimmickMsg_unk msg;
                    msg.mType = mSide == 0 ? 5 : 6;
                    vf40(&msg);
                    mChara = 0;
                }
            }
        }
        break;
    }
}

void Coffin::vf14() {
    mModel.fn_8008CD60();
}

int Coffin::vf40(GimmickMsg_unk* msg) {
    CharaTask* player;
    if (msg == 0) {
        return 0;
    }
    switch (msg->mType) {
    case 1:
        mModel.fn_80097C08(0, 0, 0, 1, 1, 1.0f, 0.1f);
        mModel.mTracks[0].m08 = 0.0f;
        mModel.mTracks[0].m0C = 0.0f;
        if (vf38() != 0) {
            fn_803ED5D0(vf38());
        }
        break;
    case 3: {
        CharaTask* chara = lbl_8087F8A0->mHead;
        if (chara == 0) {
            return 0;
        }
        atn::Vector3 dir;
        dir = chara->mPos - m6C;
        dir.Normalize_unk();
        float a = atn::WrapAngle_unk(dir.ToRotation_unk().y - m78.y);
        if (a < 0.0f) {
            mSide = 0;
        } else {
            mSide = 1;
        }
        atn::Vector3 off(0.0f, 0.0f, 17.0f);
        float rotY = m78.y;
        if (mSide == 0) {
            rotY -= 1.5707964f;
        } else {
            rotY += 1.5707964f;
        }
        Mtx m;
        PSMTXRotRad(m, 'y', rotY);
        PSMTXMultVec(m, &off, &off);
        chara->fn_801539E0();
        chara->fn_80169F04(m6C + off, rotY - 3.1415927f);
        mCount = 4;
        fn_80412968();
        break;
    }
    case 4: {
        player = lbl_8087F8A0->mHead;
        if (player == 0) {
            return 0;
        }
        atn::Vector3 off(0.0f, 0.0f, 17.0f);
        float rotY = m78.y;
        if (mSide == 0) {
            rotY -= 1.5707964f;
        } else {
            rotY += 1.5707964f;
        }
        Mtx m;
        PSMTXRotRad(m, 'y', rotY);
        PSMTXMultVec(m, &off, &off);
        player->fn_8016A20C(m6C + off, mKind == 1, rotY - 3.1415927f);
        if (mSide == 0) {
            mModel.fn_80097C08(0, 1, 0, 1, 1, 0.0f, 0.1f);
        } else {
            mModel.fn_80097C08(0, 2, 0, 1, 1, 0.0f, 0.1f);
        }
        mModel.mTracks[0].m0C = 1.0f;
        PlayEfp_unk(lbl_8087F3C0, &mOpenEfp[mSide], &mModel);
        switch (mKind) {
        case 1:
            GetUp_unk();
            break;
        case 2:
            GetUp_unk();
            break;
        }
        mTimer = 40;
        break;
    }
    case 5:
        if (m54 == 4) {
            break;
        }
        mSide = 0;
        mModel.fn_80097C08(0, 1, 0, 1, 1, 1.0f, 0.1f);
        mModel.mTracks[0].m08 = mModel.fn_80097D7C(0);
        mModel.mTracks[0].m0C = 1.0f;
        if (vf38() != 0) {
            fn_803ED5D0(vf38());
        }
        break;
    case 6:
        if (m54 == 4) {
            break;
        }
        mSide = 1;
        mModel.fn_80097C08(0, 2, 0, 1, 1, 1.0f, 0.1f);
        mModel.mTracks[0].m08 = mModel.fn_80097D7C(0);
        mModel.mTracks[0].m0C = 1.0f;
        if (vf38() != 0) {
            fn_803ED5D0(vf38());
        }
        break;
    }
    return m54 = msg->mType;
}

void Coffin::vf70(int state) {
    GimmickMsg_unk msg;
    switch (state) {
    case 0:
    case 1:
    case 2:
        msg.mType = 1;
        vf40(&msg);
        break;
    case 3:
    case 4:
    case 5:
        msg.mType = 5;
        vf40(&msg);
        break;
    case 6:
        msg.mType = 6;
        vf40(&msg);
        break;
    }
}

void Coffin::fn_80412968() {
    mChara = 0;
    int counter = FindCounter_unk(m4C);
    if (counter < 0) {
        return;
    }
    int id = fn_80370A78(lbl_8087F430, counter);
    int kind = fn_80219558(id);
    if (kind < 0) {
        mChara = EnemyManager::sInstance->Find_unk(id);
        if (mChara == 0) {
            mKind = 0;
        } else {
            mKind = 1;
            fn_80370AE4(lbl_8087F430, 0x6F, 1);
        }
    } else {
        mChara = lbl_8087F8A0->FindByChara_unk(id);
        if (mChara != 0) {
            mKind = 2;
            switch (kind) {
            case 2:
                fn_80370AE4(lbl_8087F430, 0x1E, 0);
                break;
            case 6:
                fn_80370AE4(lbl_8087F430, 0x1F, 0);
                break;
            case 4:
                fn_80370AE4(lbl_8087F430, 0x20, 0);
                break;
            case 11:
                fn_80370AE4(lbl_8087F430, 0x21, 0);
                break;
            }
        } else {
            mKind = 0;
        }
    }
}

atn::Model* Coffin::vf38() {
    return &mModel;
}

} // namespace gmk
