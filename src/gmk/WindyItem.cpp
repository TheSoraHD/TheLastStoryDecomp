#include "gmk/WindyItem.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "game/CharaManager.h"
#include "game/CharaTask.h"
#include "game/GimmickManager.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" int atoi(const char* s);
extern "C" char lbl_8087D710[2]; // inferred: field separators of the tab CSV
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" void PSMTXRotRad(Mtx m, char axis, float rad);
extern "C" void PSMTXConcat(const Mtx a, const Mtx b, Mtx ab);
extern "C" void PSMTXMultVec(const Mtx m, const atn::Vector3* src, atn::Vector3* dst);

// inferred: direction, half size and box matrix of an area (name invented)
inline void gmk::WindyArea_unk::Setup_unk() {
    mDir.Set_unk(0.0f, 0.0f, 1.0f);
    Mtx rot;
    PSMTXRotRad(rot, 'y', mRotY);
    PSMTXMultVec(rot, &mDir, &mDir);
    mHalf.Set_unk(mSizeX * 0.5f, mSizeY * 0.5f, mSizeZ * 0.5f);
    atn::Matrix3x4& m = mMtx;
    m.SetIdentity();
    {
        Mtx r;
        PSMTXRotRad(r, 'y', mRotY);
        Mtx t;
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    atn::Vector3 center = mPos + mDir * mSizeZ * 0.5f + atn::Vector3(0.0f, mSizeY * 0.5f, 0.0f);
    m.m[0][3] = center.x;
    m.m[1][3] = center.y;
    m.m[2][3] = center.z;
}

namespace gmk {

GimmickCommon* WindyItem::fn_80436E34(atn::Task* parent, int id, int arg) {
    if (parent != 0) {
        return new (12, "", "", 0) WindyItem(parent, id, arg);
    }
    return 0;
}

WindyItem::WindyItem(atn::Task* parent, int id, int arg)
    : GimmickCommon(parent, id, arg), mActive(0), mECC(5), mED0(2), mED4(-1), mED8(0), mEDC(300), mEE0(90),
      mEE4(0), mEE8(5), mGust(0), mGustLength(1500), mEF4(10.0f), mArea(0), mWait(0), mWaitBase(300),
      mWaitRand(10), mF08(0) {
    m54 = 0;
    for (int i = 0; i < 8; i++) {
        mSlots[i].mAnime.fn_8044D9BC(this);
    }
}

WindyItem::~WindyItem() {}

int WindyItem::vf0C() {
    return fn_803EC91C() == 0;
}

// NONMATCHING: 98.94% - registers of the hoisted constants 0.0f/1.0f/0.5f (rotated by one). The
// half size through Set_unk and the matrix through a reference fixed the rest.
void WindyItem::Start() {
    m54 = 1;
    ResetWait_unk();
    int half = mWaitBase / 2;
    mWait = half + rand() % (half + 1);
    for (unsigned int i = 0; i < mAreas.size(); i++) {
        mAreas[i].Setup_unk();
    }
}

// vf10 (0x804375C0) and vf14 (0x80438814) are not decompiled yet (wind gusts, item paths and debug
// drawing).

int WindyItem::vf1C() {
    if (mEfp.fn_80237874() != 0 || mEff.fn_802376D0() != 0) {
        return 1;
    }
    WindySlot_unk* slot = mSlots;
    for (int i = 0; i < 8; i++, slot++) {
        if (slot->mAnime.fn_8044E1BC() != 0) {
            return 1;
        }
    }
    return 0;
}

// NONMATCHING: 99.98% - the 0.017453292f constant is at another .sdata2 offset (the constants of
// vf10/vf14, which are not decompiled, come before it)
void WindyItem::vf20() {
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    csv.fn_8005BCE8(lbl_8087D710);
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "LOC") == 0) {
            WindyArea_unk area;
            area.mPos.x = fn_800DC288(csv.fn_8005B9CC());
            area.mPos.y = fn_800DC288(csv.fn_8005B9CC());
            area.mPos.z = fn_800DC288(csv.fn_8005B9CC());
            area.mRotY = 0.017453292f * fn_800DC288(csv.fn_8005B9CC());
            area.mSizeX = fn_800DC288(csv.fn_8005B9CC());
            area.mSizeY = fn_800DC288(csv.fn_8005B9CC());
            area.mSizeZ = fn_800DC288(csv.fn_8005B9CC());
            mAreas.push_back(area);
        } else if (strcmp(field, "PARAM") == 0) {
            mECC = atoi(csv.fn_8005B9CC());
            mEF4 = fn_800DC288(csv.fn_8005B9CC());
            mWaitBase = atoi(csv.fn_8005B9CC());
            mWaitRand = atoi(csv.fn_8005B9CC());
            mEDC = atoi(csv.fn_8005B9CC());
            mEE0 = atoi(csv.fn_8005B9CC());
            mGustLength = atoi(csv.fn_8005B9CC());
            mEE8 = atoi(csv.fn_8005B9CC());
        } else if (strcmp(field, "EFP") == 0) {
            mEfp.fn_8023780C(csv.fn_8005B9CC());
        } else if (strcmp(field, "EFF") == 0) {
            mEff.fn_80237654(csv.fn_8005B9CC());
        }
    } while (csv.fn_8005BBF8() != 0);
}

int WindyItem::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    return m54 = msg->mType;
}

void WindyItem::vf70(int state) {
    GimmickMsg_unk msg(state);
    vf40(&msg);
}

} // namespace gmk

extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
extern "C" void* lbl_8087EE98;                     // inferred: collision world
extern "C" int fn_8004ECC0(void* world, atn::Vector3* hit, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg); // inferred: ray test

// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct WindyEffectMgr_unk {
    unsigned char m00[0xB8];
    int mB8;
};
struct WindyColor_unk {
    WindyColor_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r, g, b, a;
};
extern "C" WindyEffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_8023A8B4(WindyEffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const WindyColor_unk& color,
                            int arg7, int arg8, int arg9, float scale);
extern "C" void fn_80239DAC(WindyEffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_80232B7C(void* owner, int arg);
extern "C" int fn_80239D58(WindyEffectMgr_unk* mgr, void* owner, int id); // inferred: the effect is playing
extern "C" void fn_8023A680(WindyEffectMgr_unk* mgr, ResLoader_unk* res, int arg2, int arg3, atn::Model* model,
                            const atn::Vector3* pos, const atn::Vector3* rot, int arg7, int arg8, int arg9,
                            int arg10, float scale);
extern "C" void fn_800CB688(atn::SoundRef* sound, int arg, float pitch); // inferred: sets the pitch
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume); // inferred: 2D sound
extern "C" void fn_800CB518(atn::SoundRef* sound, int frames); // inferred: fades the sound in
extern "C" const char* lbl_80754260[4]; // inferred: sound effects of the wind
// inferred: camera (only the position)
struct WindyCamera_unk {
    unsigned char m00[0x10C];
    atn::Vector3 mPos; // 0x10C
};
extern "C" WindyCamera_unk* lbl_8087EFB4;
struct WindySphere_unk {
    atn::Vector3 mCenter;
    float mRadius;
};
// inferred: gimmicks that react to the gust (scale and length set from the strength)
struct WindyTarget_unk {
    unsigned char m00[0x124];
    float m124;
    int m128;
};

// inferred: starts a gust in the area of the player (name unknown)
// NONMATCHING: 96.7% - the address of the sphere is computed once before the branch in the original.
void gmk::WindyItem::fn_804392C0() {
    if (mArea == 0) {
        return;
    }
    mED8 = mEDC + mEE0 * mEE4;
    if (lbl_8087F3C0 != 0) {
        CharaTask* player = lbl_8087F8A0 != 0 ? (CharaTask*)lbl_8087F8A0->mHead : 0;
        WindySphere_unk sphere;
        if (player != 0) {
            sphere.mCenter = player->m614;
            sphere.mRadius = player->m620;
        } else {
            sphere.mCenter = lbl_8087EFB4->mPos;
            sphere.mRadius = 5.0f;
        }
        atn::Vector3 pos = sphere.mCenter;
        pos.y = mArea->mPos.y;
        atn::Vector3 rot(0.0f, 3.1415927f + mArea->mRotY, 0.0f);
        fn_80239DAC(lbl_8087F3C0, this, 0, 1);
        fn_80232B7C(this, 0);
        fn_8023A8B4(lbl_8087F3C0, &mEfp, 0, 0, pos, rot, WindyColor_unk(1.0f, 1.0f, 1.0f, 1.0f), -1, -1, 1,
                    1.0f);
    }
    for (int i = 1; i <= 16; i++) {
        WindyTarget_unk* g = lbl_8087F4A0 != 0 ? (WindyTarget_unk*)lbl_8087F4A0->fn_803EEE10(11001, i) : 0;
        if (g != 0) {
            int len = (int)(0.8f * (float)mED8);
            g->m124 = 7.0f;
            g->m128 = len;
        }
    }
    atn::SoundRef sound = fn_800C31F4(lbl_80754260[2], 0, -1, 1.0f);
    fn_800CB518(&sound, 30);
    int n = 1;
    int free = mECC - mActive - mEE4;
    if (free > 1) {
        n = free;
    }
    WindySlot_unk* slot = mSlots;
    for (int i = 0; i < 8; i++, slot++) {
        if (slot->mState == 0) {
            fn_80439574(slot);
            if (--n <= 0) {
                break;
            }
        }
    }
}

extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector

// inferred: point of a path at the parameter `t` (0..1); names invented
inline atn::Vector3 Eval_unk(const gmk::WindyPath_unk& path, float t) {
    if (t <= 0.0f) {
        return path.mStart;
    }
    if (t >= 1.0f) {
        return path.mEnd;
    }
    float d = path.mScale * t;
    for (int i = 0; i < path.mCount - 1; i++) {
        float len = path.mLength.mData[i].v;
        if (d <= len) {
            float u = 0.0f;
            if (0.0f != d) {
                u = d / len;
            }
            if (i < 0) {
                return path.mStart;
            }
            if (i >= path.mCount - 1) {
                return path.mEnd;
            }
            if (path.mCount < 2) {
                return lbl_807C7030;
            }
            const gmk::WindyCoef_unk& x = path.mX.mData[i];
            const gmk::WindyCoef_unk& y = path.mY.mData[i];
            const gmk::WindyCoef_unk& z = path.mZ.mData[i];
            atn::Vector3 r(x.a + u * (x.b + u * (x.c + x.d * u)), y.a + u * (y.b + u * (y.c + y.d * u)),
                           z.a + u * (z.b + u * (z.c + z.d * u)));
            return r;
        }
        d -= len;
    }
    return path.mStart;
}

// inferred: update of a blown item (name unknown)
// NONMATCHING: 93.7% - registers of the inlined path evaluation (Eval_unk) and the shape of the
// zero test of the segment parameter.
void gmk::WindyItem::fn_8043A08C(WindySlot_unk* slot) {
    switch (slot->mState) {
    case 1:
        if (slot->mPath.m58 > 0) {
            slot->mPath.m58--;
        }
        if (slot->mPath.m58 <= 0) {
            slot->mState = 2;
            slot->mPath.m58 = 0;
            if (lbl_8087F3C0 != 0) {
                fn_80232B7C(slot, 0);
                fn_8023A680(lbl_8087F3C0, &mEff, -1, 5, (atn::Model*)&slot->mMtx, 0, 0, 0, 0, -1, 1, 1.0f);
            }
        }
        break;
    case 2:
        if (slot->m184 != 0 && slot->mPath.mT < 1.0f) {
            slot->mPath.mT += slot->mPath.m54;
            slot->mMtx.SetIdentity();
            atn::Vector3 pos = Eval_unk(slot->mPath, slot->mPath.mT);
            slot->mMtx.m[0][3] = pos.x;
            slot->mMtx.m[1][3] = pos.y;
            slot->mMtx.m[2][3] = pos.z;
        } else {
            slot->mState = 4;
        }
        break;
    case 3:
        slot->mAnime.fn_8044F434();
        if (!slot->mAnime.IsBusy_unk()) {
            atn::SoundRef sound = fn_800C31F4(lbl_80754260[1], 0, -1, 1.0f);
            float pitch = 1.0f;
            for (int i = mEE4 - 1; i > 0; i--) {
                pitch *= 1.122462f;
            }
            fn_800CB688(&sound, 0, pitch < 2.0f ? pitch : 2.0f);
            slot->mAnime.fn_80450778();
            slot->mState = 4;
        }
        break;
    case 4:
        slot->mAnime.fn_8044F434();
        if (slot->mAnime.fn_8044EEC0() != 0 && slot->mState != 0) {
            slot->mState = 0;
            if (lbl_8087F3C0 != 0 && fn_80239D58(lbl_8087F3C0, slot, 0) != 0) {
                fn_80239DAC(lbl_8087F3C0, slot, 0, 0);
            }
            mActive--;
        }
        break;
    }
}

// inferred: number of blown items the player can see during a gust (called by GameScene; name
// unknown)
// NONMATCHING: 86.1% - stack layout of the ray temporaries (the original copies the eye into the
// start of the segment inside the loop, probably through an inline helper).
extern "C" int fn_8043A530(gmk::WindyItem* item) {
    if (item->mActive <= 0 || item->mGust <= 0) {
        return 0;
    }
    CharaTask* player = lbl_8087F8A0 != 0 ? (CharaTask*)lbl_8087F8A0->mHead : 0;
    if (player == 0) {
        return 0;
    }
    atn::Vector3 eye = player->fn_801781B0();
    int count = 0;
    for (int i = 0; i < 8; i++) {
        gmk::WindySlot_unk& slot = item->mSlots[i];
        if ((slot.mState == 1 && slot.mPath.m58 < 90) || slot.mState == 2) {
            atn::Vector3 pos(slot.mMtx.m[0][3], slot.mMtx.m[1][3], slot.mMtx.m[2][3]);
            atn::Vector3 from = eye;
            atn::Vector3 to = pos;
            float r = item->mEF4;
            float len = (to - from).Length_unk();
            if (!(atn::Abs_unk(len) < 1.1920929e-7f)) {
                to -= from;
                to.Normalize_unk();
                float k = len - r;
                to.x = to.x * k + from.x;
                to.y = to.y * k + from.y;
                to.z = to.z * k + from.z;
            }
            if (fn_8004ECC0(lbl_8087EE98, 0, &from, &to, 0x80000010, 0, 0) == 0) {
                count++;
            }
        }
    }
    return count;
}
