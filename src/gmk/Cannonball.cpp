#include "gmk/Cannonball.h"
#include "atn/ColliBase.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "game/CharaManager.h"
#include "game/CharaTask.h"
#include "game/EnemyManager.h"
#include "game/EnemyTask.h"
#include "game/HitInfo_unk.h"
#include "game/PlayerTask.h"
#include "gmk/Lemon.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int atoi(const char* s);
extern "C" char lbl_8087D710[2]; // inferred: field separators of the tab CSV
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" void fn_8008AD4C(atn::Model* model, const char* path, const char* texture); // inferred: loads a model
extern "C" void fn_80092F1C(atn::Model* model, const char* path, int arg, float lod); // inferred: adds a LOD model
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" float PSVECMag(const atn::Vector3* v);
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector

// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct CannonEffectMgr_unk;
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" CannonEffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_8023A8B4(CannonEffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80239DAC(CannonEffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_80232B7C(void* owner, int arg);
// inferred: sounds
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" void fn_800CB5C8(atn::SoundRef* sound, int frames, int arg); // inferred: stops the sound
extern "C" void fn_800CB6E4(atn::SoundRef* sound, const atn::Vector3* pos); // inferred: moves the sound
// inferred: field (lbl_8087F430) and its map information
struct MapInfo_unk {
    unsigned char m00[0x4C];
    int m4C; // inferred: area
    int m50; // inferred: room
};
struct Field_unk {
    void fn_803750E4(int id);
    unsigned char m0000[0x54E4];
    int m54E4; // inferred: state (8-11 while an event runs)
};
extern "C" Field_unk* lbl_8087F430;
extern "C" MapInfo_unk* fn_80373148(Field_unk* field);
// inferred: accessors (weak copies elsewhere; names keep the addresses)
extern "C" inline Field_unk* fn_80121F00() {
    return lbl_8087F430;
}
extern "C" inline int fn_803D6E24(Field_unk* field) {
    return field->m54E4;
}
extern "C" inline int fn_803EFDA4(MapInfo_unk* info) {
    return info->m4C;
}
extern "C" inline int fn_803EFDAC(MapInfo_unk* info) {
    return info->m50;
}
// inferred: arena (lbl_8087F610)
struct CannonArena_unk;
extern "C" CannonArena_unk* lbl_8087F610;
extern "C" int fn_804EB1AC(CannonArena_unk* arena); // inferred: nonzero while the battle runs
extern "C" inline CannonArena_unk* fn_800F7F90() {
    return lbl_8087F610;
}
// inferred: name invented (weak copy 0x803FAEB4)
inline int IsArenaBattle_unk() {
    int ret = 0;
    if (fn_800F7F90() != 0 && fn_804EB1AC(fn_800F7F90()) != 0) {
        ret = 1;
    }
    return ret;
}
// inferred: character managers and lists (accessors, names keep the addresses)
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
// inferred: link of the character lists of the managers (CharaTask + 0x14AC)
struct CannonCharaNode_unk {
    unsigned char m0000[0x14AC];
    CharaTask* mNext;
};
extern "C" inline CharaManager<PlayerTask>* fn_8000D9E8() {
    return lbl_8087F8A0;
}
extern "C" inline CharaTask* fn_8000DCF4(CharaManager<PlayerTask>* mgr) {
    return mgr->mHead;
}
extern "C" inline CharaTask* fn_802A36B0(CharaManager<PlayerTask>* mgr) {
    return mgr->mHead;
}
extern "C" inline CharaTask* fn_802A4094(CharaTask* chara) {
    return ((CannonCharaNode_unk*)chara)->mNext;
}
extern "C" inline EnemyManager* fn_801A03E0() {
    return EnemyManager::sInstance;
}
extern "C" inline CharaTask* fn_801A0408(EnemyManager* mgr) {
    return mgr->mHead;
}
extern "C" inline CharaTask* fn_801A03EC(CharaTask* chara) {
    return ((CannonCharaNode_unk*)chara)->mNext;
}
extern "C" inline atn::Model* fn_800F84C8(CharaTask* chara) {
    return (atn::Model*)&chara->mMotion;
}
extern "C" inline atn::ColliCapsule* fn_80198C00(CharaTask* chara) {
    return &chara->mColli;
}
// inferred: segment and radius of a capsule
struct CannonCapsule_unk {
    atn::Vector3 m00;
    atn::Vector3 m0C;
    float m18; // inferred: radius
};
extern "C" inline CannonCapsule_unk* fn_80315644(atn::ColliCapsule* colli) {
    return (CannonCapsule_unk*)&colli->m3C;
}
// inferred: settings
struct CannonSettings_unk {
    unsigned char m00[0x30];
    int mFps; // 0x30: inferred: frames per second
};
extern "C" CannonSettings_unk* lbl_8087F0A8;
extern "C" inline CannonSettings_unk* fn_800F52F8() {
    return lbl_8087F0A8;
}
// inferred: gravity per frame
extern "C" inline float fn_802F0998(CannonSettings_unk* settings) {
    return -98.1f / (float)(settings->mFps * settings->mFps);
}
// inferred: collision world and its tests
extern "C" void* lbl_8087EE98;
inline void* fn_801404F8() {
    return lbl_8087EE98;
}
extern "C" int fn_8004ECC0(void* world, atn::Vector3* hit, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg); // inferred: ray test
extern "C" int fn_8004ED34(void* world, CollResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg); // inferred: ray test
// inferred: dot product
extern "C" inline float fn_801A03E8(const atn::Vector3& a, const atn::Vector3& b) {
    return PSVECDotProduct(&a, &b);
}
// inferred: model matrix helpers
extern "C" inline atn::Matrix3x4* fn_80266154(atn::Model* model) {
    return (atn::Matrix3x4*)model->mMtx;
}
extern "C" void PSMTXTrans(Mtx m, float x, float y, float z);
extern "C" inline void fn_800F80A8(MtxPtr mtx, const atn::Vector3* pos) {
    PSMTXTrans(mtx, pos->x, pos->y, pos->z);
}
extern "C" void fn_800F80B8(MtxPtr mtx, const atn::Vector3* rot);
extern "C" void fn_80316E38(atn::Model* model, MtxPtr mtx);
// inferred: bullets
struct CannonBullets_unk {
    unsigned char m00000[0x3E39C];
    int m3E39C; // inferred: set while a gimmick fires
};
extern "C" CannonBullets_unk* lbl_8087F048;
extern "C" HitInfo_unk* fn_80219E6C(int id); // inferred: bullet data by id
extern "C" int fn_800F8548(CannonBullets_unk* mgr);
extern "C" void fn_800FAB80(CannonBullets_unk* mgr, CharaTask* owner, HitInfo_unk* bullet, int arg3,
                            const atn::Vector3* pos, const atn::Vector3* rot, int arg6, int arg7, int arg8, int arg9,
                            float arg10, float arg11);
// inferred: hit request of the explosion when the bullet is of kind 3 (0x20 bytes; names invented)
struct CannonHit_unk {
    CannonHit_unk() {
        m00 = 0;
        m04 = 0;
        m08 = 0;
        m0C = 0;
        m10 = 0;
        m14 = -1;
        m1C_31_28 = 0;
        m18 = -1;
    }
    int m00;
    int m04;
    int m08;
    int m0C;
    int m10;
    int m14;
    int m18;
    unsigned int m1C_31_28 : 4;
    unsigned int m1C_27_0 : 28;
};
extern "C" void fn_8003F030(CannonHit_unk* hit, HitInfo_unk* bullet, CharaTask* owner, const atn::Vector3* pos,
                            const atn::Vector3* dir, int arg);
// inferred: listeners (see gmk/Item.cpp)
inline CharaListeners_unk* fn_8014F8F4() {
    static CharaListeners_unk sListeners;
    return &sListeners;
}

static const char* const sSe[4] = {"SE_BTCOM_036", "SE_BTCOM_037", "SE_BTCOM_038"};

// inferred: plays an effect at the model (name invented)
inline void PlayEfp_unk(CannonEffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model) {
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
// inferred: plays an effect at a position (name invented)
inline void PlayEfpAt_unk(CannonEffectMgr_unk* mgr, atn::EfpFile* efp, const atn::Vector3& pos,
                          const atn::Vector3& rot) {
    Color4f_unk color;
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    color.a = 1.0f;
    fn_8023A8B4(mgr, efp, 0, 0, pos, rot, color, -1, -1, 1, 1.0f);
}
// inferred: sound helpers (weak copies 0x803FADF4 and 0x803FAE08); names invented
inline bool IsPlaying_unk(const atn::SoundRef& sound) {
    return sound.mPtr != 0;
}
inline atn::SoundRef& SetSound_unk(atn::SoundRef& dst, const atn::SoundRef& src) {
    dst = src;
    return dst;
}
// inferred: matrix of a node of the model (weak copy 0x803FAE38); name invented
inline MtxPtr GetNode_unk(atn::Model* model, const char* name) {
    MtxPtr mtx;
    int index = model->GetNodeNoByName(name, 0);
    if (index < 0) {
        mtx = 0;
    } else {
        mtx = model->mNodeMtx[index];
    }
    return mtx;
}
// inferred: inline in the original (weak copy 0x8013C3B4, defined out of line in game/CharaTask.cpp)
inline atn::Vector3 atn::Vector3::operator-() const {
    return atn::Vector3(-x, -y, -z);
}
// inferred: vector scaled by a factor (weak copy 0x803FAE8C, also used at 0x80551108)
inline atn::Vector3 operator*(float f, const atn::Vector3& v) {
    return atn::Vector3(v.x * f, v.y * f, v.z * f);
}

namespace gmk {

GimmickCommon* Cannonball::fn_803F9F34(atn::Task* parent, GimmickDesc_unk* desc) {
    if (parent != 0) {
        return new (12, "", "", 0) Cannonball(parent, desc);
    }
    return 0;
}

Cannonball::Cannonball(atn::Task* parent, GimmickDesc_unk* desc)
    : GimmickCommon(parent, desc->mId, desc->mArg), mModel(1, 32), mDesc(desc), mHolder(0), mIgnore(0), mTimer(0),
      mCanExplode(1), mExplodeTime(0), mBulletId(0), mResetTime(0), mDrop(0), m520(0xFF), m524(-1) {
    m54 = 0;
    mE8 = 6;
    if (desc->mId % 1000 == 1) {
        mEC = 0x13;
    } else {
        mEC = 3;
    }
    if (mDesc->m24 > 0) {
        mDrop = (DropItem*)fn_804093B4(this, 18000, m4C + 7000);
        mDrop->fn_800D246C(1);
        mDrop->SetPos_unk(mDesc->mPos);
        mDrop->SetRot_unk(atn::Vector3(0.0f, mDesc->mRotY, 0.0f));
        mDrop->vf68(mDesc->m24, 0);
    }
}

int Cannonball::vf0C() {
    if (fn_803EC91C() == 0 && mEfp.IsLoading() == 0 && mEfpFuse.IsLoading() == 0 && fn_800D3FA4() != 0) {
        m54 = 1;
        if (mDrop != 0) {
            mDrop->fn_800D246C(0);
            mDrop->mFlags |= 4;
        }
        return 1;
    }
    return 0;
}

int Cannonball::vf1C() {
    return mModel.fn_8008B140() != 0;
}

void Cannonball::vf20() {
    char lod[256];
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
            strcpy(lod, csv.fn_8005B9CC());
            fn_80092F1C(&mModel, lod, 0, fn_800DC288(csv.fn_8005B9CC()));
        } else if (strcmp(field, "EFP") == 0) {
            const char* path = csv.fn_8005B9CC();
            if (path[0] != 0) {
                mEfp.LoadRequest(path);
            }
            path = csv.fn_8005B9CC();
            if (path[0] != 0) {
                mEfpFuse.LoadRequest(path);
            }
        }
    } while (csv.fn_8005BBF8() != 0);
}

void Cannonball::vf24() {
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    csv.fn_8005BCE8(lbl_8087D710);
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "EXPLODE_PARAM") == 0) {
            mExplodeTime = atoi(csv.fn_8005B9CC());
            mBulletId = atoi(csv.fn_8005B9CC());
            if (lbl_8087F610 != 0 && mBulletId == 2000) {
                mBulletId = 22000;
            }
        } else if (strcmp(field, "RESET_PARAM") == 0) {
            mResetTime = atoi(csv.fn_8005B9CC());
        }
    } while (csv.fn_8005BBF8() != 0);
}

void Cannonball::Start() {
    GimmickMsg_unk msg(1);
    vf40(&msg);
}

// NONMATCHING: 99.92% - the m9C accessor is called as IsShown_unk (the original calls the copy
// named CharaObj1208_unk::fn_8013A18C), the boost::function destructor is fn_8000D3A8 in the
// original, and the radius sums of the case 7 loops use other registers. As in the original,
// nothing is inlined in this function (the inline budget is exhausted), so the accessors, the
// Vector3 operators and the helpers are called out of line.
void Cannonball::vf10() {
    if (mDesc->m7C == 0) {
        return;
    }
    if (fn_80121F00() != 0 && (unsigned int)(fn_803D6E24(fn_80121F00()) - 8) <= 3) {
        if (IsShown_unk() != 0) {
            GimmickMsg_unk msg(8);
            vf40(&msg);
            mTimer = 0;
        }
        return;
    }
    if (m54 == 8 && m9C == 0 && mDrop == 0) {
        mTimer--;
        if (fn_800F7F90() != 0 && fn_804EB1AC(fn_800F7F90()) == 0) {
            mTimer = mTimer > 2 ? mTimer : 2;
        }
        if (mTimer <= 0) {
            m9C = 1;
            Start();
        } else {
            return;
        }
    }
    if ((m54 >= 5 && m54 <= 6) || m54 == 2) {
        if (mExplodeTime > 0 || m54 == 6) {
            mTimer--;
        } else {
            mTimer = 999;
        }
        if (IsPlaying_unk(mSound)) {
            fn_800CB6E4(&mSound, &m6C);
        } else if (mTimer < 150) {
            SetSound_unk(mSound, fn_800C344C(sSe[1], &m6C, 0, -1, 1.0f));
        }
        if (fn_80121F00() != 0 && fn_80373148(fn_80121F00()) != 0 &&
            fn_803EFDA4(fn_80373148(fn_80121F00())) == 11 && fn_803EFDAC(fn_80373148(fn_80121F00())) == 3 &&
            m54 == 2 && mTimer <= 180) {
            mTimer = 180;
        }
        if (mHolder != 0 && mHolder->fn_80148334() != 0) {
            mTimer = mTimer > 2 ? mTimer : 2;
        }
        if (mTimer <= 0) {
            if (m54 == 2) {
                fn_803FB870(&m6C, fn_8000DCF4(fn_8000D9E8()), 1);
            } else if (m54 == 6) {
                fn_803FB870(&m6C, fn_8000DCF4(fn_8000D9E8()), 1);
            } else {
                fn_803FB870(&m6C, fn_8000DCF4(fn_8000D9E8()), 0);
            }
            return;
        }
    }
    if (mHolder != 0) {
        atn::Matrix3x4 mtx(GetNode_unk(fn_800F84C8(mHolder), "rightwepoint"));
        mtx.fn_80144ED4(-1.5707964f);
        fn_80316E38(&mModel, mtx.m);
        m6C = fn_80266154(&mModel)->GetTrans_unk();
    } else {
        if (m54 == 3) {
            mFallSpeed += fn_802F0998(fn_800F52F8());
            m6C.y += mFallSpeed;
            atn::Vector3 from(m6C);
            from.y += 10.0f;
            atn::Vector3 hit;
            if (fn_8004ECC0(fn_801404F8(), &hit, &from, &m6C, 0x80000020, 0, 0) != 0) {
                m54 = 4;
                SetM520_unk(0xFF);
                m6C = hit;
            }
        } else if (m54 == 5) {
            atn::Vector3 from(m6C);
            mVel.y += fn_802F0998(fn_800F52F8());
            from -= mVel;
            m6C += mVel;
            atn::Vector3 unused;
            unsigned int mask = 2;
            if (mDesc->m20 == 1) {
                mask |= 0x80000000;
            }
            CollResult_unk res;
            if (fn_8004ED34(fn_801404F8(), &res, &from, &m6C, mask, mIgnore, 0) != 0) {
                if (mDesc->m20 == 1) {
                    if (fn_801A03E8(res.m28, atn::Vector3(0.0f, 1.0f, 0.0f)) < 0.70710677f) {
                        atn::Vector3 n = fn_801A03E8(res.m28, mVel) * res.m28;
                        atn::Vector3 t = mVel - n;
                        mVel = -n * 0.5f + t * 0.2f;
                        m6C = res.m04 + res.m28;
                    } else if (mCanExplode != 0) {
                        fn_803FBC34(&res.m04);
                        return;
                    }
                } else if (mCanExplode != 0) {
                    fn_803FB870(&res.m04, fn_8000DCF4(fn_8000D9E8()), 0);
                    return;
                }
            }
            if (m6C.y - mDesc->mPos.y < -500.0f) {
                fn_803FB870(&m6C, fn_8000DCF4(fn_8000D9E8()), 0);
                return;
            }
            mIgnore = 0;
            m78 = mVel.GetNormalize().ToRotation_unk();
            m78.x += 1.5707964f;
        } else if (m54 == 6) {
            atn::Vector3 prev(m6C);
            m6C += mVel;
            mVel.y += 1.5f * fn_802F0998(fn_800F52F8());
            m78 = mVel.GetNormalize().ToRotation_unk();
            m78.x += 1.5707964f;
        } else if (m54 == 7) {
            if (fn_800F7F90() == 0 || IsArenaBattle_unk() != 0) {
                for (CharaTask* chara = fn_802A36B0(fn_8000D9E8()); chara != 0; chara = fn_802A4094(chara)) {
                    if (chara->CanAct_unk()) {
                        atn::Vector3 d = *chara->fn_8013C38C() - m6C;
                        float r = fn_80315644(fn_80198C00(chara))->m18 + 1.0f;
                        if (d.LengthSq_unk() < r * r) {
                            fn_803FB870(&m6C, 0, 0);
                        }
                    }
                }
                for (CharaTask* chara = fn_801A0408(fn_801A03E0()); chara != 0; chara = fn_801A03EC(chara)) {
                    if (chara->CanAct_unk()) {
                        atn::Vector3 d = *chara->fn_8013C38C() - m6C;
                        float r = fn_80315644(fn_80198C00(chara))->m18 + 8.0f;
                        if (d.LengthSq_unk() < r * r) {
                            fn_803FB870(&m6C, 0, 0);
                        }
                    }
                }
            }
        }
        Mtx mtx;
        fn_800F80A8(mtx, &m6C);
        fn_800F80B8(mtx, &m78);
        fn_80316E38(&mModel, mtx);
    }
    mModel.CalcMatrixStack(atn::ModelNodeCallback_unk(0));
}

void Cannonball::vf14() {
    if (mDesc->m7C == 0) {
        return;
    }
    if (fn_80121F00() != 0 && (unsigned int)(fn_803D6E24(fn_80121F00()) - 8) <= 3) {
        return;
    }
    if (vf38() != 0) {
        fn_803ED774(vf38());
    }
}

// NONMATCHING: 98.79% - .sdata2 has an extra 1.1920929e-7f (from the weak copy of
// Vector3::ToRotation_unk emitted after vf10), so 5.0f and -0.5f are at other addresses; in case 6
// the registers of 1.0f, frames and 1/frames differ
int Cannonball::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    if (msg->m0C == 1 && msg->mType == 5 && msg->mType == m54) {
        return 0;
    }
    if (fn_800F7F90() != 0 && fn_804EB1AC(fn_800F7F90()) == 0 && msg->mType == 1) {
        m9C = 1;
    }
    m54 = msg->mType;
    mHolder = (CharaTask*)msg->m10;
    switch (m54) {
    case 1:
        m9C = 1;
        mHolder = 0;
        m6C = mDesc->mPos;
        m78.Set_unk(0.0f, mDesc->mRotY, 0.0f);
        mCanExplode = 1;
        m524 = -1;
        SetM520_unk(0xFF);
        if (mDrop != 0 && mDrop->vf60() != 0) {
            mDrop->mFlags |= 4;
            fn_8014F8F4()->mEnabled = 0;
            GimmickMsg_unk drop(6);
            mDrop->vf40(&drop);
            fn_8014F8F4()->mEnabled = 1;
        }
        break;
    case 2: {
        int skip = 0;
        if (fn_80121F00() != 0 && fn_80373148(fn_80121F00()) != 0 &&
            fn_803EFDA4(fn_80373148(fn_80121F00())) == 11 && fn_803EFDAC(fn_80373148(fn_80121F00())) == 3) {
            skip = 1;
        }
        mTimer = mExplodeTime;
        if (mTimer > 0 && skip == 0) {
            fn_80239DAC(lbl_8087F3C0, this, 0, 1);
            fn_80232B7C(this, 0);
            PlayEfp_unk(lbl_8087F3C0, &mEfpFuse, &mModel);
        }
        if (mId % 1000 != 1) {
            fn_800C344C(sSe[0], &m6C, 0, -1, 1.0f);
        }
        int kind = mId % 1000;
        if (kind != 1 && lbl_8087F430 != 0) {
            if (kind == 3) {
                lbl_8087F430->fn_803750E4(0xF4);
            } else {
                lbl_8087F430->fn_803750E4(0x94);
            }
        }
        mIgnore = mHolder != 0 ? fn_80198C00(mHolder) : 0;
        break;
    }
    case 3:
        mHolder = 0;
        if (msg->m0C == 1) {
            if (msg->m10 != 0) {
                ((CharaTask*)msg->m10)->fn_801750FC();
            }
            m6C = msg->mVec;
        }
        mTimer = 0;
        fn_80239DAC(lbl_8087F3C0, this, 0, 1);
        fn_800CB5C8(&mSound, 10, 0);
        mIgnore = 0;
        m78.x = 0.0f;
        m78.z = 0.0f;
        SetM520_unk(0xFF);
        break;
    case 5: {
        mHolder = 0;
        mTarget = msg->mVec;
        atn::Vector3 d = mTarget - m6C;
        d.y = 0.0f;
        float mag = PSVECMag(&d);
        float speed = 5.0f;
        float t = mag / speed;
        float dy = m6C.y - mTarget.y;
        float vy = t * (-0.5f * fn_802F0998(fn_800F52F8())) - dy / t;
        PSVECNormalize(&d, &d);
        d.x *= speed;
        d.y = vy;
        d.z *= speed;
        mVel = d;
        fn_800C344C(sSe[2], &m6C, 0, -1, 1.0f);
        if (msg->m0C == 1) {
            mCanExplode = 0;
        }
        break;
    }
    case 6: {
        mHolder = 0;
        mTimer = msg->m04;
        mTarget = msg->mVec;
        float frames = (float)mTimer;
        float inv = 1.0f / frames;
        mVel = (mTarget - m6C) * inv;
        float g = fn_802F0998(fn_800F52F8());
        mVel.y -= 0.5f * (frames * (1.5f * g));
        fn_800C344C(sSe[2], &m6C, 0, -1, 1.0f);
        break;
    }
    case 7:
        m6C = msg->mVec;
        mCanExplode = 1;
        SetM520_unk(0xFF);
        fn_800CB5C8(&mSound, 10, 0);
        break;
    case 8:
        m9C = 0;
        mTimer = mResetTime;
        fn_80239DAC(lbl_8087F3C0, this, 0, 0);
        fn_800CB5C8(&mSound, 10, 0);
        break;
    }
    mFallSpeed = 0.0f;
    if (msg->m0C != 1 && m54 == 3) {
        msg->mVec = m6C;
    }
    CharaListeners_unk* listeners = fn_8014F8F4();
    if (listeners->mEnabled != 0) {
        for (int i = 0; i < (int)listeners->mList.size(); i++) {
            CharaListener_unk& listener = listeners->mList[i];
            if (listener.mId == -1 || listener.mId == 11) {
                listener.mFunc(11, (CharaTask*)this, msg);
            }
        }
    }
    return m54;
}

int Cannonball::vf60() {
    int ret = 0;
    if (mDesc->m7C != 0 && mHolder == 0 && m9C != 0) {
        ret = 1;
    }
    return ret;
}

void Cannonball::vf4C(CharaTask* chara) {
    if (chara == 0) {
        return;
    }
    if (chara->m1208 != 0) {
        chara->fn_801750FC();
    } else {
        chara->fn_80174E04(this);
    }
}

float Cannonball::fn_803FB830() {
    if (mBulletId > 0) {
        HitInfo_unk* bullet = fn_80219E6C(mBulletId);
        if (bullet != 0) {
            return bullet->m58;
        }
    }
    return 10.0f;
}

void Cannonball::fn_803FB870(const atn::Vector3* pos, CharaTask* owner, int flag) {
    if (mBulletId > 0) {
        fn_80239DAC(lbl_8087F3C0, this, 0, 0);
        fn_80232B7C(0, 0);
        atn::Vector3 rot(0.0f, 0.0f, 0.0f);
        PlayEfpAt_unk(lbl_8087F3C0, &mEfp, *pos, rot);
        HitInfo_unk* bullet = fn_80219E6C(mBulletId);
        if (mCanExplode != 0) {
            if ((int)bullet->m02 == 3) {
                if (owner != 0) {
                    CannonHit_unk hit;
                    atn::Vector3 at(*pos);
                    fn_8003F030(&hit, bullet, owner, &at, &lbl_807C7030, 0);
                }
            } else {
                lbl_8087F048->m3E39C = 1;
                CannonBullets_unk* mgr = lbl_8087F048;
                atn::Vector3 dir(0.0f, 0.0f, 0.0f);
                fn_800FAB80(mgr, owner, bullet, fn_800F8548(mgr), pos, &dir, flag, 30, -1, -1, 0.0f, 1.0f);
                lbl_8087F048->m3E39C = 0;
            }
        }
        GimmickMsg_unk msg(8);
        vf40(&msg);
    } else {
        GimmickMsg_unk msg(3);
        vf40(&msg);
    }
    m78.x = 0.0f;
    m78.z = 0.0f;
    fn_800CB5C8(&mSound, 10, 0);
    SetM520_unk(0xFF);
    if (mDrop != 0 && mDrop->IsFlag_unk(4)) {
        mDrop->mFlags &= ~4;
        mDrop->SetPos_unk(*pos - mVel);
        mDrop->SetRot_unk(m78);
        fn_8014F8F4()->mEnabled = 0;
        GimmickMsg_unk drop(3);
        mDrop->vf40(&drop);
        fn_8014F8F4()->mEnabled = 1;
    }
}

// NONMATCHING: 99.72% - r5/r6 swapped in the copy to msg.mVec
void Cannonball::fn_803FBC34(const atn::Vector3* pos) {
    if (mBulletId > 0) {
        GimmickMsg_unk msg;
        msg.m10 = (int)mHolder;
        msg.mType = 7;
        msg.mVec = *pos - mVel.GetNormalize() * 1.5f;
        vf40(&msg);
    }
    fn_800CB5C8(&mSound, 10, 0);
}

// NONMATCHING: 99.86% - 625.0f and 0.25881904f are shifted in .sdata2 (see vf40)
int Cannonball::vf48(const atn::Vector3* pos, const atn::Vector3* dir, int flag) {
    if (vf60() != 0 && flag == 0 && (m54 == 1 || m54 == 7 || m54 == 4)) {
        atn::Vector3 d = m6C - *pos;
        if (fn_805F9920(&d) > 625.0f) {
            return 0;
        }
        atn::Vector3 n(d);
        PSVECNormalize(&n, &n);
        return !(PSVECDotProduct(&n, dir) < 0.25881904f);
    }
    return 0;
}

void Cannonball::vf70(int state) {
    GimmickMsg_unk msg(state);
    if (state != 1 && state != 8) {
        msg.mType = 1;
    }
    vf40(&msg);
}

} // namespace gmk
