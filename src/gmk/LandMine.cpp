#include "gmk/LandMine.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "atn/SoundRef.h"
#include "game/CharaManager.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" char lbl_8087D710[2]; // inferred: " " (field separators of the tab CSV)
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" void fn_8008AD4C(atn::Model* model, const char* path, const char* texture); // inferred: loads a model
extern "C" float PSVECSquareMag(const atn::Vector3* v); // inferred: squared length
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);

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
// inferred: player character in the list of the player manager
struct MinePlayer_unk {
    unsigned char m000[0x528];
    atn::Vector3 mPos; // 0x528
    unsigned char m534[0x55C - 0x534];
    int m55C;
    int m560;
    unsigned char m564[0x14AC - 0x564];
    MinePlayer_unk* mNext; // 0x14AC
};
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
// inferred: bullets
struct BulletMgr_unk;
extern "C" BulletMgr_unk* lbl_8087F048;
extern "C" void* fn_80219E6C(int id); // inferred: bullet data by id
extern "C" int fn_800F8548(BulletMgr_unk* mgr);
extern "C" void fn_800FAB80(BulletMgr_unk* mgr, int arg1, void* bullet, int arg3, const atn::Vector3* pos,
                            const atn::Vector3* rot, int arg6, int arg7, int arg8, int arg9, float arg10,
                            float arg11);

namespace gmk {

GimmickCommon* LandMine::fn_80420ADC(atn::Task* parent, GimmickDesc_unk* desc) {
    if (parent != 0) {
        return new ((MemoryTag)12, "", "", 0) LandMine(parent, desc);
    }
    return 0;
}

int LandMine::vf1C() {
    if (mModel.fn_8008B140() != 0) {
        return 1;
    }
    if (mEfp.IsLoading() != 0 || mEfpWait.IsLoading() != 0) {
        return 1;
    }
    return 0;
}

void LandMine::vf20() {
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    csv.fn_8005BCE8(lbl_8087D710);
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "EFP") == 0) {
            mEfp.LoadRequest(csv.fn_8005B9CC());
        } else if (strcmp(field, "EFP_WAIT") == 0) {
            mEfpWait.LoadRequest(csv.fn_8005B9CC());
        } else if (strcmp(field, "MODEL") == 0) {
            fn_8008AD4C(&mModel, csv.fn_8005B9CC(), 0);
            fn_803EC7A0(&mModel, &csv);
            mHasModel = 1;
        } else if (strcmp(field, "COLLI_RADIUS") == 0) {
            mColliRadius = fn_800DC288(csv.fn_8005B9CC());
        }
    } while (csv.fn_8005BBF8() != 0);
}

int LandMine::vf0C() {
    if (fn_803EC91C() == 0) {
        m54 = 1;
        return 1;
    }
    return 0;
}

// NONMATCHING: float register allocation, and the target converts the z of the sphere with frsp
// (as if it went through a double). Tried: radius local, sphere local, Vector3 temporary, inline
// setter, (float)(double) casts of z, a center copy, component helper, direct member stores, the
// order/chaining of the m78 stores.
void LandMine::Start() {
    m6C = mDesc->mPos;
    m78.y = mDesc->mRotY;
    m78.x = 0.0f;
    m78.z = 0.0f;
    mRadius = mDesc->m40;
    mBulletId = mDesc->m24;
    mColli.mSphere = atn::ColliSphere::Sphere_unk(m6C, mColliRadius);
    mColli.mFlags |= 8;
    mColli.m0C = (int)this;
}

void LandMine::vf10() {
    if (m54 == 1) {
        mColli.mFlags &= ~1;
    } else {
        switch (m54) {
        case 2:
            mColli.mFlags |= 1;
            for (MinePlayer_unk* player = (MinePlayer_unk*)lbl_8087F8A0->mHead; player != 0;
                 player = player->mNext) {
                if (player->m55C == 6 && player->m560 == 14) {
                    continue;
                }
                atn::Vector3 d(player->mPos.x - m6C.x, player->mPos.y - m6C.y, player->mPos.z - m6C.z);
                if (PSVECSquareMag(&d) < mRadius * mRadius) {
                    GimmickHitMsg_unk msg;
                    memset(&msg, 0, sizeof(msg));
                    msg.mType = 3;
                    vf44(&msg);
                    break;
                }
            }
            break;
        case 3:
            mColli.mFlags &= ~1;
            if (++mTimer > mDesc->m20) {
                mTimer = 0;
                m54 = 2;
            }
            break;
        }
        if (mHasModel != 0 && vf38() != 0) {
            fn_803ED0D4(vf38(), 1);
        }
    }
}

void LandMine::vf14() {
    if (m54 == 2 && lbl_8087F0A8->m18 != 0 && mHasModel != 0 && vf38() != 0) {
        fn_803ED774(vf38());
    }
}

int LandMine::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    m54 = msg->mType;
    switch (m54) {
    case 1:
    case 3:
        if (mHasModel != 0 && mEfpWait.mResource != 0 && lbl_8087F3C0 != 0) {
            fn_80239DAC(lbl_8087F3C0, this, 0, 1);
        }
        break;
    case 2:
        if (mHasModel != 0 && mEfpWait.mResource != 0 && lbl_8087F3C0 != 0) {
            fn_80239DAC(lbl_8087F3C0, this, 0, 1);
            lbl_8087F3C0->mB8 = 1;
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
            fn_8023A8B4(lbl_8087F3C0, &mEfpWait, &mModel, 0, pos, rot, color, -1, -1, 1, 1.0f);
            lbl_8087F3C0->mB8 = 0;
        }
        break;
    }
    return m54;
}

int LandMine::vf44(GimmickHitMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    if (m54 == 2) {
        void* bullet = fn_80219E6C(mBulletId);
        if (bullet != 0) {
            fn_800FAB80(lbl_8087F048, 0, bullet, fn_800F8548(lbl_8087F048), &m6C, &m78, 0, 30, -1, 2, 6.0f, 1.0f);
            fn_800C344C("SE_DG011_004_2", &m6C, 0, -1, 1.0f);
            mTimer = 0;
            if (lbl_8087F3C0 != 0) {
                fn_80239DAC(lbl_8087F3C0, this, 0, 1);
            }
            m54 = 3;
        }
    }
    return 0;
}

int LandMine::vf60() {
    int ret = 0;
    if (m54 == 2 && mDesc->m7C != 0) {
        ret = 1;
    }
    return ret;
}

atn::Vector3 LandMine::vf54() {
    return mColli.mSphere.mCenter;
}

atn::Model* LandMine::vf38() {
    return &mModel;
}

} // namespace gmk
