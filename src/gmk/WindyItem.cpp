#include "gmk/WindyItem.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "game/CharaManager.h"
#include "game/CharaTask.h"
#include "game/GimmickManager.h"
#include "atn/RenderManager.h"
#include "atn/DrawHelper.h"
#include "game/GameTask.h"
#include "game/GameUI.h"
#include "game/Inventory.h"
#include "game/PopupTask.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" int atoi(const char* s);
extern "C" char lbl_8087D710[2]; // inferred: field separators of the tab CSV
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" void PSMTXRotRad(Mtx m, char axis, float rad);
extern "C" void PSMTXConcat(const Mtx a, const Mtx b, Mtx ab);
extern "C" void PSMTXMultVec(const Mtx m, const atn::Vector3* src, atn::Vector3* dst);
extern "C" GameTask* lbl_8087F430; // inferred: field task
extern "C" int rand();
extern "C" float PSVECMag(const atn::Vector3* v);
extern "C" void PSVECNormalize(const atn::Vector3* src, atn::Vector3* dst);
extern "C" void fn_80445130(void* mgr, int* id, int* count, int arg);
extern "C" int fn_8021E4E4(int* itemId, int* num, int id, unsigned int flags);
extern "C" int fn_80450B60(int item);
extern "C" void* fn_80211480(int id); // inferred: item data
extern "C" int fn_80216AFC(const char* name, int* type, int* map, int* point); // inferred: parses a map id
struct WindyTable_unk;
extern "C" WindyTable_unk* fn_8021D990(int level); // inferred: random table of the wind items
extern "C" bool fn_8021D9D4(WindyTable_unk* table, int* item, char* name, int index); // inferred: random item
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
extern "C" void fn_8023A680(WindyEffectMgr_unk* mgr, atn::EffFile* res, int arg2, int arg3, atn::Model* model,
                            const atn::Vector3* pos, const atn::Vector3* rot, int arg7, int arg8, int arg9,
                            int arg10, float scale);
extern "C" void fn_800CB688(atn::SoundRef* sound, int arg, float pitch); // inferred: sets the pitch
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume); // inferred: 2D sound
extern "C" void fn_800CB518(atn::SoundRef* sound, int frames); // inferred: fades the sound in
extern "C" const char* lbl_80754260[4]; // inferred: sound effects of the wind
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

extern "C" int fn_80051CD8(const WindySphere_unk* sphere, const atn::Vector3* box); // inferred: sphere in the
                                                                                    // box (half size, matrix)
extern "C" int fn_80051B70(const atn::Vector3* line, const WindySphere_unk* sphere); // inferred: segment hits sphere

extern "C" float PSVECSquareMag(const atn::Vector3* v);
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);

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
        return new ((MemoryTag)12, "", "", 0) WindyItem(parent, id, arg);
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
            float u;
            if (0.0f == d) {
                u = 0.0f;
            } else {
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
            atn::Vector3 r;
            r.x = x.a + u * (x.b + u * (x.c + x.d * u));
            r.y = y.a + u * (y.b + u * (y.c + y.d * u));
            r.z = z.a + u * (z.b + u * (z.c + z.d * u));
            return r;
        }
        d -= len;
    }
    return path.mStart;
}

// inferred: camera modes in which the player looks around (as in gmk/RandomItem.cpp)
inline bool IsLookMode_unk(int mode) {
    return mode == 3 || mode == 1 || mode == 4;
}

// inferred: the item can be seen from `eye` (as in gmk/RandomItem.cpp; name invented)
inline int CanSee_unk(const atn::Vector3& eye, const WindySphere_unk& sphere) {
    atn::Vector3 line[2];
    line[0] = eye;
    line[1] = sphere.mCenter;
    atn::Vector3 d = line[1] - line[0];
    float len = PSVECMag(&d);
    float radius = sphere.mRadius;
    if (!((float)__fabs(len) < 1.1920929e-7f)) {
        line[1] = line[1] - line[0];
        PSVECNormalize(&line[1], &line[1]);
        line[1] = line[1] * (len - radius) + line[0];
    }
    return fn_8004ECC0(lbl_8087EE98, 0, &line[0], &line[1], 0x80000010, 0, 0) == 0;
}

// inferred: source of the item added to the inventory (as in gmk/RandomItem.cpp)
inline void SetSource_unk(Inventory* inv, int set, int type, int map, int point) {
    inv->mDB14 = set;
    inv->mDB18 = type;
    inv->mDB1C = map;
    inv->mDB20 = point;
    inv->mDB24 = 0;
    inv->mDB28 = 0;
}

// inferred: update: finds the area of the player, runs the gusts and the blown items; looked at through
// the camera during a gust an item is caught (the player plays motion 100), then it is given with a popup
// NONMATCHING: 36.6% - over the inline budget here (R3 of docs/inlining.md): our copy calls Eval_unk,
// CanSee_unk, the Vector3 operators... with bl, the original inlines every candidate
void WindyItem::vf10() {
    if (m54 == 0) {
        if (mActive > 0) {
            for (int i = 0; i < 8; i++) {
                WindySlot_unk& slot = mSlots[i];
                if (slot.mState != 0) {
                    slot.mState = 0;
                    if (lbl_8087F3C0 != 0 && fn_80239D58(lbl_8087F3C0, &slot, 0) != 0) {
                        fn_80239DAC(lbl_8087F3C0, &slot, 0, 0);
                    }
                    mActive--;
                }
            }
        }
        if (mGust > 0) {
            ResetWait_unk();
        }
        return;
    }
    mArea = 0;
    CharaTask* player = lbl_8087F8A0 != 0 ? (CharaTask*)lbl_8087F8A0->mHead : 0;
    WindySphere_unk sphere;
    if (player != 0) {
        sphere.mCenter = player->m614;
        sphere.mRadius = player->m620;
    } else {
        sphere.mCenter = atn::RenderManager::instance_->mCamera.mPos;
        sphere.mRadius = 5.0f;
    }
    for (unsigned int i = 0; i < mAreas.size(); i++) {
        if (fn_80051CD8(&sphere, &mAreas[i].mHalf)) {
            mArea = &mAreas[i];
            break;
        }
    }
    if (mArea == 0 && mActive <= 0) {
        return;
    }
    if (mWait > 0 && --mWait <= 0) {
        mGust = mGustLength;
        mEE4 = 0;
        mED8 = 1;
    }
    if (mGust > 0) {
        if (mED8 > 0 && --mED8 <= 0) {
            fn_804392C0();
        }
        if (--mGust <= 0 || mEE4 >= mEE8) {
            ResetWait_unk();
        }
    }
    for (int i = 0; i < 8; i++) {
        fn_8043A08C(&mSlots[i]);
    }
    player = lbl_8087F8A0 != 0 ? (CharaTask*)lbl_8087F8A0->mHead : 0;
    GameTask* field = lbl_8087F430;
    if (field != 0 && player != 0 && IsLookMode_unk(field->mCamera.m7FC)) {
        float range = 600.0f;
        if (mArea != 0) {
            atn::Vector3 front(0.0f, 0.0f, 1.0f);
            Mtx rot;
            PSMTXRotRad(rot, 'y', player->mRot.y);
            PSMTXMultVec(rot, &front, &front);
            if (PSVECDotProduct(&mArea->mDir, &front) < 0.0f) {
                range = 200.0f;
            }
        }
        GameCamera& camera = field->mCamera;
        for (int i = 0; i < 8; i++) {
            WindySlot_unk& slot = mSlots[i];
            if (slot.mState != 2) {
                continue;
            }
            atn::Vector3 line[2];
            line[0] = camera.mPos;
            line[1] = camera.mTarget - line[0];
            PSVECNormalize(&line[1], &line[1]);
            line[1] = line[1] * range + line[0];
            WindySphere_unk ball;
            ball.mCenter = Eval_unk(slot.mPath, slot.mPath.mT);
            ball.mRadius = mEF4;
            WindySphere_unk item = ball;
            atn::Vector3 toItem = ball.mCenter - camera.mPos;
            float dist = PSVECMag(&toItem);
            atn::Vector3 lo(-item.mRadius, -item.mRadius, 0.0f);
            atn::Vector3 hi(item.mRadius, item.mRadius, 0.0f);
            atn::Matrix3x4 view = camera.mInvView;
            view.m[0][3] = 0.0f;
            view.m[1][3] = 0.0f;
            view.m[2][3] = 0.0f;
            PSMTXMultVec(view.m, &lo, &lo);
            PSMTXMultVec(view.m, &hi, &hi);
            atn::Vector3 p1 = item.mCenter + lo;
            p1 = atn::RenderManager::instance_->GetScreenPosition(p1);
            atn::Vector3 p2 = item.mCenter + hi;
            p2 = atn::RenderManager::instance_->GetScreenPosition(p2);
            atn::Vector2_unk size(0.5f * (float)__fabs(p1.x - p2.x) + 12.0f, 0.5f * (float)__fabs(p1.y - p2.y) + 12.0f);
            atn::Vector2_unk center(0.5f * (p1.x + p2.x), 0.5f * (p1.y + p2.y));
            if (mGust <= 0) {
                continue;
            }
            if (mED0 == 2 && fn_80051B70(line, &ball) != 0 && CanSee_unk(line[0], ball)) {
                mED0 = 0;
                mED4 = i;
                player->mMotion.fn_80097C08(3, 100, 0, 0, 1, 0.0f, 0.1f);
                player->mMotion.mTracks[3].m0C = 1.5f;
            }
            if (dist < 600.0f && CanSee_unk(line[0], ball)) {
                lbl_8087F490->fn_803E3050(item.mCenter, atn::Vector3(center.x, center.y, p1.z), size, dist);
            }
        }
    }
    if (mED0 == 2) {
        return;
    }
    atn::Model& motion = player->mMotion;
    if (mED0 == 0) {
        if (motion.mTracks[3].m08 >= 12.0f || motion.mTracks[3].m00 != 100) {
            mED0 = 1;
            mEE4++;
            WindySlot_unk* slot = &mSlots[mED4];
            if (lbl_8087F8A8 != 0) {
                PopupEntry_unk e;
                e.m10 = 9;
                e.m94 = (PopupAnime_unk*)lbl_8087F8A8->fn_8054E528((unsigned int)player, mEE4);
                if (mEE4 >= 5) {
                    e.m84 = 100;
                }
                e.m04 = e.m94->mTask->mLastFrame;
                e.m60 = Eval_unk(slot->mPath, slot->mPath.mT);
                e.m84 = mEE4;
                lbl_8087F8A8->fn_8054E0FC(&e);
            }
            WindyTable_unk* table = fn_8021D990(lbl_8087F430->m10D0);
            char name[0x10];
            memset(name, 0, sizeof(name));
            int item;
            int id;
            int count;
            if (table != 0 && fn_8021D9D4(table, &item, name, mEE4 - 1) && item > 0 &&
                fn_8021E4E4(&id, &count, item, 1)) {
                int first = fn_80450B60(id);
                if (Inventory::instance_ != 0) {
                    int type = -1;
                    int map = -1;
                    int point = -1;
                    if (fn_80216AFC(name, &type, &map, &point)) {
                        SetSource_unk(Inventory::instance_, 1, type, map, point);
                    }
                    fn_80445130(Inventory::instance_, &id, &count, 0);
                    SetSource_unk(Inventory::instance_, 0, -1, -1, -1);
                }
                void* data = fn_80211480(id);
                if (data != 0) {
                    int rare = 0;
                    if (Inventory::instance_->fn_80448F9C(id) > rand() % 100) {
                        rare = 1;
                    }
                    atn::Vector3 pos = Eval_unk(slot->mPath, slot->mPath.mT);
                    slot->mAnime.fn_8044E418(&pos, 25, 25, data, count, rare, 0);
                    if (slot->mAnime.fn_8044E610() != 0) {
                        if (first != 0) {
                            slot->mAnime.fn_8044E644();
                        } else {
                            slot->mAnime.fn_8044F2D4();
                        }
                    }
                    slot->mState = 3;
                    if (lbl_8087F3C0 != 0 && fn_80239D58(lbl_8087F3C0, slot, 0) != 0) {
                        fn_80239DAC(lbl_8087F3C0, slot, 0, 0);
                    }
                }
            }
        }
    } else if (mED0 == 1) {
        if (motion.mTracks[3].m08 >= motion.fn_80097D7C(3) || motion.mTracks[3].m00 != 100) {
            motion.mTracks[3].m0C = 1.0f;
            motion.fn_80097CCC(3, 0.0f);
            mED0 = 2;
        }
    }
}

// inferred: debug drawing of the areas and of the paths of the blown items
// NONMATCHING: 97.2% - registers of the hoisted constants of Setup_unk (as in Start) and of the
// segment loop of Eval_unk
void WindyItem::vf14() {
    if (mF08 == 0) {
        return;
    }
    for (unsigned int i = 0; i < mAreas.size(); i++) {
        mAreas[i].Setup_unk();
    }
    for (unsigned int i = 0; i < 8; i++) {
        WindySlot_unk& slot = mSlots[i];
        if (slot.mState != 2) {
            continue;
        }
        lbl_8087EEB0->fn_80063D3C(atn::Vector3(slot.mMtx.m[0][3], slot.mMtx.m[1][3], slot.mMtx.m[2][3]), 1.0f, 0.0f, 0xFFFF8800);
        for (float t = 0.0f; t < 1.0f; t += 0.01f) {
            atn::Vector3 a = Eval_unk(slot.mPath, t);
            atn::Vector3 b = Eval_unk(slot.mPath, t + 0.01f);
            lbl_8087EEB0->fn_80063764(a, b, 0.0f, 0x5500FFFF);
        }
    }
    for (unsigned int i = 0; i < mAreas.size(); i++) {
        lbl_8087EEB0->fn_800629F0(lbl_807C7030, mAreas[i].mHalf, 0.0f, 0xFFFFFF00, &mAreas[i].mMtx);
    }
}


int WindyItem::vf1C() {
    if (mEfp.IsLoading() != 0 || mEff.IsLoading() != 0) {
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
            mEfp.LoadRequest(csv.fn_8005B9CC());
        } else if (strcmp(field, "EFF") == 0) {
            mEff.LoadRequest(csv.fn_8005B9CC());
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
            sphere.mCenter = atn::RenderManager::instance_->mCamera.mPos;
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
        WindyTarget_unk* g = GimmickManager::instance_ != 0 ? (WindyTarget_unk*)GimmickManager::instance_->fn_803EEE10(11001, i) : 0;
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

// inferred: raw storage of a std::vector<float> (constructed and destroyed through the weak copies,
// as PathFloats_unk of Event/ActionActorPosition.cpp)
struct WindyFloats_unk {
    float* mData;
    unsigned int mSize;
    unsigned int mCap;
};
// inferred: helpers of the spline compiled in another file (weak copies, C names as in
// Event/ActionActorPosition.cpp and game/Em403WarLion.cpp)
extern "C" void fn_8032B314(std::vector<gmk::WindyCoef_unk>* v, unsigned int n);      // inferred: resize
extern "C" WindyFloats_unk* fn_8032B6F8(WindyFloats_unk* v, unsigned int n);          // inferred: vector(n)
extern "C" void fn_8000D844(WindyFloats_unk* v, int flag);                            // inferred: ~vector<float>
extern "C" void fn_8032B7F0(gmk::WindyPath_unk* path, const float* y, gmk::WindyCoef_unk* out); // inferred: solve
extern "C" void fn_8032BE88(gmk::WindyLengths_unk* v, unsigned int n);                // inferred: resize
extern "C" float fn_8032C1EC(gmk::WindyPath_unk* path, int i);                        // inferred: segment length

// inferred: builds the spline through the points (same code as Event::PathSpline_unk::Build_unk)
inline void gmk::WindyPath_unk::Build_unk(const atn::Vector3* pts, int n) {
    mCount = n;
    fn_8032B314(&mX, n - 1);
    fn_8032B314(&mY, n - 1);
    fn_8032B314(&mZ, n - 1);
    WindyFloats_unk zs;
    WindyFloats_unk ys;
    WindyFloats_unk xs;
    fn_8032B6F8(&xs, n);
    fn_8032B6F8(&ys, n);
    fn_8032B6F8(&zs, n);
    for (int i = 0; i < n; i++) {
        xs.mData[i] = pts[i].x;
        ys.mData[i] = pts[i].y;
        zs.mData[i] = pts[i].z;
    }
    fn_8032B7F0(this, xs.mData, mX.begin());
    fn_8032B7F0(this, ys.mData, mY.begin());
    fn_8032B7F0(this, zs.mData, mZ.begin());
    mStart = pts[0];
    mEnd = pts[n - 1];
    mScale = 0.0f;
    fn_8032BE88(&mLength, n - 1);
    for (int i = 0; i < n - 1; i++) {
        mLength[i].v = fn_8032C1EC(this, i);
        mScale += mLength[i].v;
    }
    fn_8000D844(&zs, -1);
    fn_8000D844(&ys, -1);
    fn_8000D844(&xs, -1);
}

// inferred: segment [a, b] (name invented)
struct WindySegment_unk {
    WindySegment_unk(const atn::Vector3& a, const atn::Vector3& b) : mA(a), mB(b) {}
    // inferred: point of the segment nearest to `p`
    atn::Vector3 Nearest_unk(const atn::Vector3& p) const {
        atn::Vector3 ab = mB - mA;
        float len = PSVECSquareMag(&ab);
        if (atn::Abs_unk(len) < 1.1920929e-7f) {
            return mA;
        }
        atn::Vector3 ap = mA - p;
        float t = -PSVECDotProduct(&ab, &ap) / len;
        t = (t > 0.0f ? t : 0.0f) < 1.0f ? (t > 0.0f ? t : 0.0f) : 1.0f;
        return mA + ab * t;
    }
    atn::Vector3 mA;
    atn::Vector3 mB;
};

// inferred: starts a blown item in the area of the player (name unknown)
// NONMATCHING: 94.63% - the original copies the start of the segment before computing its end (and
// rounds both z with frsp); registers of the inlined Build_unk/Eval_unk
void gmk::WindyItem::fn_80439574(WindySlot_unk* slot) {
    if (slot->mState != 0) {
        return;
    }
    Mtx rot;
    float rotY = mArea->mRotY;
    float sx = mArea->mSizeX;
    float sy = mArea->mSizeY;
    float sz = mArea->mSizeZ;
    PSMTXRotRad(rot, 'y', rotY);
    slot->mState = 1;
    slot->m184 = (int)mArea;
    slot->mPath.m58 = rand() % 5 * 30 - 75;
    CharaTask* player = lbl_8087F8A0 != 0 ? (CharaTask*)lbl_8087F8A0->mHead : 0;
    atn::Vector3 eye;
    if (player != 0) {
        eye = player->mPos;
    } else {
        eye = atn::RenderManager::instance_->mCamera.mPos;
    }
    WindySegment_unk seg(mArea->mPos, mArea->mPos + mArea->mDir * sz);
    atn::Vector3 nearest = seg.Nearest_unk(eye);
    atn::Vector3 base = nearest - mArea->mDir * 500.0f;
    int n = rand() % 6 + 6;
    atn::Vector3 pts[12];
    for (int i = 0; i < n; i++) {
        pts[i] = base + mArea->mDir * (1000.0f / n) * i;
        atn::Vector3 v(sx * (rand() % 1001 / 1000.0f) - 0.5f * sx, sy * (rand() % 1001 / 1000.0f) + 5.0f, 0.0f);
        PSMTXMultVec(rot, &v, &v);
        pts[i] += v;
    }
    slot->mPath.Build_unk(pts, n);
    slot->mMtx.SetIdentity();
    slot->mMtx.fn_801479E4(Eval_unk(slot->mPath, 0.0f));
    slot->mPath.m54 = 0.0025641026f;
    if (slot->mPath.m58 >= 0) {
        slot->mPath.mT = 0.0f;
    } else {
        slot->mPath.mT = 0.0025641026f * -slot->mPath.m58;
    }
    mActive++;
}

// inferred: update of a blown item (name unknown)
// NONMATCHING: 98.56% - registers of the segment loop of the inlined path evaluation (Eval_unk:
// the index and the byte offset are swapped)
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
