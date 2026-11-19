#include "game/GameConfig_unk.h"
#include "gmk/Brazier.h"
#include "game/BattleTask.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "game/CharaManager.h"
#include "game/CharaTask.h"
#include "game/EnemyManager.h"
#include "game/EnemyTask.h"
#include "game/PlayerTask.h"
#include "gmk/Lemon.h"
#include "atn/DrawHelper.h"
#include "game/GameTask.h"

extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" int strcmp(const char* a, const char* b);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int atoi(const char* s);
extern "C" char lbl_8087D710[2]; // inferred: field separators of the tab CSV
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" float PSVECSquareMag(const atn::Vector3* v); // inferred: squared length
extern "C" int fn_802180A8(const char* s); // inferred: motion id from its name
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector

// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct BrazierEffectMgr_unk {
    unsigned char m00[0xB8];
    int mB8;
};
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" BrazierEffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_8023A8B4(BrazierEffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80239DAC(BrazierEffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_80232B7C(void* owner, int arg);
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" void fn_800CB5C8(atn::SoundRef* sound, int frames, int arg); // inferred: stops the sound
extern "C" void fn_800CB6E4(atn::SoundRef* sound, const atn::Vector3* pos); // inferred: moves the sound
// inferred: collision world (lbl_8087EE98)
extern "C" void* lbl_8087EE98;
extern "C" int fn_8004D314(void* world, int arg, const atn::Vector3* pos, const atn::Vector3* dir, unsigned int mask,
                           int arg5, int arg6, float radius); // inferred: sphere test
extern "C" int fn_8004D388(void* world, atn::ColliResult_unk* result, CharaTask::Sphere_unk* sphere, atn::Vector3* move,
                           unsigned int mask, void* ignore, int arg, float); // inferred: sweep test (last: radius)
extern "C" GameConfig_unk* lbl_8087F0A8;
// inferred: the field task (lbl_8087F430); the frustum of its camera tests the spawn points
extern "C" GameTask* lbl_8087F430;
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
struct HitInfo_unk;
extern "C" BattleTask* lbl_8087F048; // inferred: the battle manager
extern "C" HitInfo_unk* fn_80219E6C(int id); // inferred: bullet data by id
// inferred: arena characters (lbl_8087F610); 0xD5C bytes each
struct BrazierArenaEntry_unk {
    CharaTask* mChara;
    unsigned char m04[0xD0 - 0x04];
    unsigned int mD0_31 : 1; // inferred: in the battle
    unsigned int mD0_30_0 : 31;
    unsigned char mD4[0xD5C - 0xD4];
};
struct BrazierArena_unk {
    unsigned char m000[0x5E4];
    BrazierArenaEntry_unk* mEntries; // 0x5E4
    int mCount;                      // 0x5E8
    // inferred: name invented
    int GetCount_unk() const { return mCount; }
    BrazierArenaEntry_unk* Get_unk(int i) {
        if (i >= 0 && i < mCount) {
            return &mEntries[(unsigned int)i];
        }
        return 0;
    }
};
extern "C" BrazierArena_unk* lbl_8087F610;
// inferred: link of the character lists of the managers (CharaTask + 0x14AC)
struct BrazierCharaNode_unk {
    unsigned char m0000[0x14AC];
    CharaTask* mNext;
};
// inferred: name invented
inline BrazierArenaEntry_unk* GetArenaEntry_unk(int i) {
    BrazierArena_unk* arena = lbl_8087F610;
    if (i >= 0 && i < arena->mCount) {
        return &arena->mEntries[(unsigned int)i];
    }
    return 0;
}
// inferred: listeners (see gmk/Item.cpp)
inline CharaListeners_unk* fn_8014F8F4() {
    static CharaListeners_unk sListeners;
    return &sListeners;
}
// inferred: inlined copy of CharaListeners_unk::fn_8014F860 (as in gmk/SparkCube; name invented)
inline void NotifyListeners_unk(CharaListeners_unk* listeners, int id, void* chara, void* arg) {
    if (listeners->mEnabled != 0) {
        for (int i = 0; i < (int)listeners->mList.size(); i++) {
            CharaListener_unk& listener = listeners->mList[i];
            if (listener.mId == -1 || id == listener.mId) {
                listener.mFunc(id, (CharaTask*)chara, arg);
            }
        }
    }
}

static const char* const sSe[] = {"SE_DGCOM_004", "SE_DGCOM_005"};

// inferred: plays an effect at the model (name invented)
inline void PlayEfp_unk(BrazierEffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model) {
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
// inferred: the position is too near the character (name invented)
inline bool IsNear_unk(const atn::Vector3& pos, const CharaTask* chara, float r) {
    atn::Vector3 d = pos - chara->mPos;
    return PSVECSquareMag(&d) < r * r;
}

namespace gmk {

GimmickCommon* Brazier::fn_80406754(atn::Task* parent, int id, int arg) {
    if (parent != 0) {
        return new ((MemoryTag)12, "", "", 0) Brazier(parent, id, arg);
    }
    return 0;
}

GimmickCommon* Brazier::fn_804067D8(atn::Task* parent, GimmickDesc_unk* desc) {
    if (parent != 0) {
        Brazier* brazier = new ((MemoryTag)12, "", "", 0) Brazier(parent, desc->mId, desc->mArg);
        if (brazier != 0) {
            brazier->m6C = desc->mPos;
            brazier->m78 = atn::Vector3(0.0f, desc->mRotY, 0.0f);
            brazier->m90 = atn::Vector3(1.0f, 1.0f, 1.0f);
            brazier->mDesc = desc;
        }
        return brazier;
    }
    return 0;
}

Brazier::Brazier(atn::Task* parent, int id, int arg)
    : GimmickCommon(parent, id, arg), mModel(8, 32), mModelCrush(8, 32), mDir(0.0f, 0.0f, 0.0f), mSpeed(0.0f),
      mRadius(3.0f), mBulletId(0), mWait(0), mTimer(-1), mDesc(0), mEnabled(1) {
    m54 = 0;
    mColli.mFlags |= 8;
    mColli.m0C = (int)this;
    mE8 = 6;
    mEC = 0;
}

Brazier::~Brazier() {}

// inferred: the rolling brazier hits a wall or something below it (name invented)
inline int Brazier::IsBlocked_unk() {
    atn::Vector3 dir;
    __VecCopy(&dir.x, &mDir.x);
    dir.y = 0.0f;
    atn::Vector3 pos = m6C + dir * mRadius;
    pos.y += mRadius;
    if (fn_8004D314(lbl_8087EE98, 0, &pos, &lbl_807C7030, 0x80000000, 0, 0, 0.5f * mRadius) != 0) {
        return 1;
    }
    if (fn_8004D314(lbl_8087EE98, 0, &m6C, &lbl_807C7030, 0x40000000, 0, 0, 2.5f * mRadius) != 0) {
        return 1;
    }
    return 0;
}

int Brazier::vf0C() {
    return fn_803EC91C() == 0;
}

void Brazier::vf10() {
    if (mDesc != 0) {
        if (mEnabled != mDesc->m7C) {
            if (mEnabled != 0) {
                mColli.mFlags &= ~1;
                fn_80239DAC(lbl_8087F3C0, this, 1, 0);
            } else {
                Start();
            }
            mEnabled = mDesc->m7C;
        }
        if (mDesc->m7C == 0) {
            return;
        }
    }
    if (m54 == 0) {
        Start();
    } else if (m54 == 2) {
        GimmickHitMsg_unk msg;
        memset(&msg, 0, sizeof(msg));
        msg.mType = 0;
        msg.mDir = atn::Vector3(0.0f, 0.0f, 1.0f);
        vf44(&msg);
    }
    fn_80407D24();
    fn_803ED0D4(&mModel, 1);
    fn_803ED0D4(&mModelCrush, 1);
    switch (m54) {
    case 3:
    case 4: {
        if (vf38() != 0) {
            fn_803ED5D0(vf38());
        }
        if (mSound.mPtr != 0) {
            fn_800CB6E4(&mSound, &m6C);
        }
        if (m54 == 3) {
            float frame = mModel.mTracks[0].m08;
            if (frame >= mModel.fn_80097D7C(0)) {
                m54 = 4;
                mModel.fn_80097C08(0, 1, 1, 0, 1, 0.0f, 0.1f);
            }
        }
        int hit = IsBlocked_unk();
        if (hit != 0) {
            GimmickMsg_unk msg(5);
            vf40(&msg);
        }
        break;
    }
    case 5:
        if (vf38() != 0) {
            fn_803ED5D0(vf38());
        }
        if (fn_80407E8C() != 0) {
            Start();
        }
        break;
    }
    mColli.SetMtx_unk(*(atn::Matrix3x4*)mModel.mMtx.m);
}

void Brazier::vf14() {
    if (mDesc != 0 && mDesc->m7C == 0) {
        return;
    }
    if (vf38() != 0) {
        fn_803ED774(vf38());
    }
    if (lbl_8087F0A8->mDrawCollision != 0) {
        atn::Vector3 pos(m6C);
        pos.y += mRadius;
        atn::Vector3 dir;
        __VecCopy(&dir.x, &mDir.x);
        dir.y = 0.0f;
        lbl_8087EEB0->fn_80063D3C(pos, mRadius, 0.0f, 0xFFFFFF00);
        lbl_8087EEB0->fn_80063D3C(pos + dir * mRadius, 0.5f * mRadius, 0.0f, 0xFFFFFF00);
    }
}

int Brazier::vf1C() {
    int ret = 0;
    if (mModel.fn_8008B140() != 0 || mModelCrush.fn_8008B140() != 0) {
        ret = 1;
    }
    if (mColli.fn_800580BC() != 0) {
        ret = 1;
    }
    if (mEfp.IsLoading() != 0 || mEfpRoll.IsLoading() != 0 || mEfpCrush.IsLoading() != 0) {
        ret = 1;
    }
    return ret;
}

void Brazier::vf20() {
    char motion[256];
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    csv.fn_8005BCE8(lbl_8087D710);
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "MODEL") == 0) {
            mModel.fn_8008AD4C(csv.fn_8005B9CC(), 0);
        } else if (strcmp(field, "MODEL_CRUSH") == 0) {
            mModelCrush.fn_8008AD4C(csv.fn_8005B9CC(), 0);
        } else if (strcmp(field, "MOTION") == 0) {
            strcpy(motion, csv.fn_8005B9CC());
            int id = fn_802180A8(csv.fn_8005B9CC());
            if (id != 2) {
                mModel.fn_80097A88(id, motion);
            } else {
                mModelCrush.fn_80097A88(id, motion);
            }
        } else if (strcmp(field, "COLLI_OBJECT") == 0) {
            mColli.fn_80058078(csv.fn_8005B9CC());
        } else if (strcmp(field, "EFP") == 0) {
            mEfp.LoadRequest(csv.fn_8005B9CC());
        } else if (strcmp(field, "EFP_ROLL") == 0) {
            mEfpRoll.LoadRequest(csv.fn_8005B9CC());
        } else if (strcmp(field, "EFP_CRUSH") == 0) {
            mEfpCrush.LoadRequest(csv.fn_8005B9CC());
        } else if (strcmp(field, "PARAM") == 0) {
            mSpeed = fn_800DC288(csv.fn_8005B9CC());
            mRadius = fn_800DC288(csv.fn_8005B9CC());
            mBulletId = atoi(csv.fn_8005B9CC());
            mWait = atoi(csv.fn_8005B9CC());
        }
    } while (csv.fn_8005BBF8() != 0);
}

void Brazier::Start() {
    vf2C();
    GimmickMsg_unk msg(1);
    if (mDesc != 0 && mDesc->m20 != 0) {
        msg.mType = 6;
    }
    vf40(&msg);
}

// The listeners are notified through the inline copy of CharaListeners_unk::fn_8014F860 (with the loop
// written here the player/bullet manager registers of case 5 were swapped). The registration object of the
// listeners static (lbl_807C8878) is a local symbol here.
int Brazier::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    switch (msg->mType) {
    case 1:
    case 6: {
        const atn::Vector3* pos = mDesc != 0 ? &mDesc->mPos : &lbl_807C7030;
        m6C = *pos;
        float rotY = mDesc != 0 ? mDesc->mRotY : 0.0f;
        m78 = atn::Vector3(0.0f, rotY, 0.0f);
        mModel.fn_80097C08(0, 0, 0, 1, 1, 1.0f, 0.1f);
        mModelCrush.fn_80097C08(0, 2, 0, 1, 1, 1.0f, 0.1f);
        fn_803ED0D4(&mModel, 1);
        fn_803ED0D4(&mModelCrush, 1);
        fn_803ED5D0(&mModel);
        fn_803ED5D0(&mModelCrush);
        mModel.mTracks[0].m08 = 0.0f;
        mModelCrush.mTracks[0].m08 = 0.0f;
        mColli.SetMtx_unk(*(atn::Matrix3x4*)mModel.mMtx.m);
        mColli.mFlags |= 1;
        lbl_8087F3C0->mB8 = 1;
        fn_80239DAC(lbl_8087F3C0, this, 1, 0);
        fn_80232B7C(this, 1);
        PlayEfp_unk(lbl_8087F3C0, &mEfp, &mModel);
        lbl_8087F3C0->mB8 = 0;
        break;
    }
    case 3:
    case 4:
        mDir = msg->mVec;
        if (!(atn::Abs_unk(PSVECSquareMag(&mDir)) < 1.1920929e-7f)) {
            PSVECNormalize(&mDir, &mDir);
        } else {
            mDir.x = 0.0f;
            mDir.y = 0.0f;
            mDir.z = 1.0f;
        }
        m78.y = mDir.ToRotation_unk().y;
        if (msg->mType == 3) {
            mModel.fn_80097C08(0, 0, 0, 0, 1, 0.0f, 0.1f);
        } else {
            mModel.fn_80097C08(0, 1, 1, 0, 1, 0.0f, 0.1f);
        }
        mColli.mFlags &= ~1;
        fn_80239DAC(lbl_8087F3C0, this, 1, 1);
        fn_80232B7C(this, 1);
        PlayEfp_unk(lbl_8087F3C0, &mEfpRoll, &mModel);
        mSound = fn_800C344C(sSe[0], &m6C, 0, -1, 1.0f);
        break;
    case 5: {
        mModelCrush.fn_80097C08(0, 2, 0, 0, 1, 0.0f, 0.1f);
        mModelCrush.mTracks[0].m0C = 1.0f;
        fn_80239DAC(lbl_8087F3C0, this, 1, 1);
        fn_80232B7C(this, 1);
        PlayEfp_unk(lbl_8087F3C0, &mEfpCrush, &mModelCrush);
        fn_800CB5C8(&mSound, 0, 0);
        fn_800C344C(sSe[1], &m6C, 0, -1, 1.0f);
        BattleTask* mgr;
        CharaTask* player = 0;
        if (lbl_8087F8A0 != 0) {
            player = lbl_8087F8A0->mHead;
        }
        if (lbl_8087F048 != 0) {
            lbl_8087F048->m3E39C = 1;
            mgr = lbl_8087F048;
            mgr->fn_800FAB80(player, fn_80219E6C(mBulletId), mgr->fn_800F8548(), &m6C, &m78, 0, 30, -1, -1, 4.0f,
                        1.0f);
            lbl_8087F048->m3E39C = 0;
        }
        mTimer = mWait;
        break;
    }
    }
    m54 = msg->mType;
    NotifyListeners_unk(fn_8014F8F4(), 11, this, msg);
    return m54;
}

int Brazier::vf44(GimmickHitMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    if (mDesc != 0 && mDesc->m7C == 0) {
        return 0;
    }
    if (m54 == 6) {
        return 0;
    }
    atn::Vector3 dir(msg->mDir);
    dir.y = 0.0f;
    if (!(atn::Abs_unk(PSVECSquareMag(&dir)) < 1.1920929e-7f) && m54 < 5) {
        GimmickMsg_unk push(3);
        push.mVec = dir;
        vf40(&push);
        return 1;
    }
    return 0;
}

void Brazier::fn_80407D24() {
    if (m54 != 3 && m54 != 4) {
        return;
    }
    m6C += mDir * mSpeed;
    atn::ColliResult_unk res;
    CharaTask::Sphere_unk sphere;
    sphere.mCenter = m6C;
    sphere.mCenter.y += mRadius;
    sphere.mRadius = mRadius;
    if (fn_8004D388(lbl_8087EE98, &res, &sphere, &mDir, 0, 0, 0, sphere.mRadius) != 0) {
        mDir.y = 0.0f;
    } else {
        mDir.y += -98.1f / (float)(lbl_8087F0A8->m030 * lbl_8087F0A8->m030);
    }
    m6C = res.m10;
    m6C.y -= mRadius;
    if (mDesc == 0) {
        m6C.y = 0.0f;
    }
}

// The count read through GetCount_unk() reloads lbl_8087F610 for the loop test, and the unsigned
// subscript of GetArenaEntry_unk gives the entry offset/chara registers of the original
int Brazier::fn_80407E8C() {
    GameTask* field = lbl_8087F430;
    if (field == 0) {
        return 0;
    }
    if (mWait <= 0) {
        return 0;
    }
    if (mTimer > 0) {
        mTimer--;
        return 0;
    }
    atn::Sphere sphere;
    atn::Vector3 pos(m6C);
    atn::Vector3 home(mDesc->mPos);
    Frustum_unk* view = &field->mCamera.mCamera.mFrustum;
    float r = 10.0f;
    sphere.mCenter = pos;
    sphere.mRadius = r;
    if (fn_8004FF58(view, &sphere) != 0) {
        return 0;
    }
    sphere.mCenter = home;
    sphere.mRadius = r;
    if (fn_8004FF58(view, &sphere) != 0) {
        return 0;
    }
    if (lbl_8087F8A0 != 0) {
        for (CharaTask* chara = lbl_8087F8A0->mHead; chara != 0; chara = ((BrazierCharaNode_unk*)chara)->mNext) {
            if (!chara->IsFlag_unk(4)) {
                atn::Vector3 d1 = pos - chara->mPos;
                if (PSVECSquareMag(&d1) < r * r) {
                    return 0;
                }
                atn::Vector3 d2 = home - chara->mPos;
                if (PSVECSquareMag(&d2) < r * r) {
                    return 0;
                }
            }
        }
    }
    if (EnemyManager::sInstance != 0) {
        for (CharaTask* chara = EnemyManager::sInstance->mHead; chara != 0;
             chara = ((BrazierCharaNode_unk*)chara)->mNext) {
            if (chara->CanAct_unk()) {
                atn::Vector3 d1 = pos - chara->mPos;
                if (PSVECSquareMag(&d1) < r * r) {
                    return 0;
                }
                atn::Vector3 d2 = home - chara->mPos;
                if (PSVECSquareMag(&d2) < r * r) {
                    return 0;
                }
            }
        }
    }
    if (lbl_8087F610 != 0) {
        for (int i = 0; i < lbl_8087F610->GetCount_unk(); i++) {
            BrazierArenaEntry_unk* entry = GetArenaEntry_unk(i);

            if (entry != 0) {
                CharaTask* chara = entry->mChara;
                if (chara != 0 && entry->mD0_31) {
                    atn::Vector3 d1 = pos - chara->mPos;
                    if (PSVECSquareMag(&d1) < r * r) {
                        return 0;
                    }
                    atn::Vector3 d2 = home - chara->mPos;
                    if (PSVECSquareMag(&d2) < r * r) {
                        return 0;
                    }
                }
            }
        }
    }
    return 1;
}

int Brazier::vf60() {
    int ret = 0;
    int enabled = 1;
    GimmickDesc_unk* desc = mDesc;
    if (desc != 0 && desc->m7C == 0) {
        enabled = 0;
    }
    if (enabled != 0 && m54 == 1) {
        ret = 1;
    }
    return ret;
}

atn::Model* Brazier::vf38() {
    return m54 != 5 ? &mModel : &mModelCrush;
}

} // namespace gmk
