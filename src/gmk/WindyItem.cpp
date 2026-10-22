#include "gmk/WindyItem.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "game/CharaManager.h"
#include "game/CharaTask.h"

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
    mHalf.x = mSizeX * 0.5f;
    mHalf.y = mSizeY * 0.5f;
    mHalf.z = mSizeZ * 0.5f;
    mMtx.SetIdentity();
    {
        Mtx r;
        PSMTXRotRad(r, 'y', mRotY);
        Mtx t;
        PSMTXConcat(mMtx.m, r, t);
        __MtxCopy(mMtx.m, t);
    }
    atn::Vector3 center = mPos + mDir * mSizeZ * 0.5f + atn::Vector3(0.0f, mSizeY * 0.5f, 0.0f);
    mMtx.m[0][3] = center.x;
    mMtx.m[1][3] = center.y;
    mMtx.m[2][3] = center.z;
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

// NONMATCHING: 92.84% - registers of the hoisted constants (the original creates 1.0f first) and
// operand order of the half size products
void WindyItem::vf28() {
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
