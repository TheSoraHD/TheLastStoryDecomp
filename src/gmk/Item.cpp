#include "gmk/Item.h"
#include "atn/Memory.h"
#include "game/CharaManager.h"
#include "game/CharaTask.h"
#include "game/PlayerTask.h"

extern "C" float PSVECSquareMag(const atn::Vector3* v); // inferred: squared length
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager

// inferred: global object at lbl_8087F628 (class unknown); the two tests are inline accessors
extern "C" unsigned char fn_806B0E30();
extern "C" int fn_806A8E40();
extern "C" unsigned char fn_806B1250();
struct ItemState_unk {
    unsigned char m000[0x1F8];
    unsigned int m1F8_31_20 : 12;
    unsigned int mDisabled : 1; // inferred
    unsigned int m1F8_18_0 : 19;
    unsigned char m1FC[0xC215 - 0x1FC];
    unsigned char mC215; // inferred: selects the source of the first value

    // inferred: names invented
    unsigned char Get1_unk() {
        if (mC215 == 0) {
            return mDisabled ? 0 : fn_806B0E30();
        } else {
            return mDisabled ? 0 : fn_806A8E40();
        }
    }
    unsigned char Get2_unk() {
        if (mC215 == 0) {
            return mDisabled ? 0 : fn_806B1250();
        }
        return 0;
    }
};
extern "C" ItemState_unk* lbl_8087F628;

// inferred: inlined here (the local static is shared with the out-of-line copy in CharaTask.cpp).
// All functions of this file match, but the unit cannot be linked: this object defines the local
// static and its guard (common symbols), while the original only references the ones of
// CharaTask.cpp, so .bss/.sbss move.
inline CharaListeners_unk* fn_8014F8F4() {
    static CharaListeners_unk sListeners;
    return &sListeners;
}

namespace gmk {

GimmickCommon* Item::fn_803FBFA8(atn::Task* parent, GimmickDesc_unk* desc) {
    if (parent != 0) {
        return new ((MemoryTag)12, "", "", 0) Item(parent, desc);
    }
    return 0;
}

int Item::vf0C() {
    if (fn_800D3FA4() != 0) {
        m54 = 2;
        mDrop->fn_800D246C(0);
        return 1;
    }
    return 0;
}

void Item::vf10() {
    if (vf38() != 0) {
        fn_803ED0D4(vf38(), 1);
    }
    int ok = 1;
    if (lbl_8087F628 != 0) {
        if (lbl_8087F628->Get1_unk() != lbl_8087F628->Get2_unk()) {
            ok = 0;
        }
    }
    if (m54 == 2) {
        if (mDesc->m7C != 0) {
            if (mDrop->m190 != 0 && mDrop->vf60() != 0) {
                mE8 = 6;
                mEC = 0;
            }
            if (ok) {
                if (mDrop->vf60() == 0) {
                    if (mDrop->IsFlag_unk(4)) {
                        mDrop->mFlags &= ~4;
                    }
                    GimmickMsg_unk msg(3);
                    mDrop->vf40(&msg);
                }
                if (mDrop->m54 == 5) {
                    GimmickMsg_unk msg(3);
                    vf40(&msg);
                }
            }
        } else if (ok) {
            if (mDrop->vf60() != 0) {
                GimmickMsg_unk msg(6);
                mDrop->vf40(&msg);
            }
        }
    } else if (ok) {
        if (mDrop->vf60() != 0 && mDrop->m54 < 5) {
            GimmickMsg_unk msg(6);
            mDrop->vf40(&msg);
        }
        if (mTimer > 0 && --mTimer <= 0) {
            GimmickMsg_unk msg(2);
            vf40(&msg);
        }
    }
}

void Item::vf14() {
    if (vf60() != 0 && vf38() != 0) {
        fn_803ED774(vf38());
    }
}

void Item::vf20() {}

int Item::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    switch (msg->mType) {
    case 1:
    case 2:
        m54 = msg->mType;
        break;
    case 3:
        m54 = msg->mType;
        mTimer = mDesc->m28;
        break;
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
    return msg->mType;
}

void Item::fn_803FC65C() {
    if (mDrop != 0) {
        mDrop->vf68(mDesc->m20, 0);
    }
    mTimer = mDesc->m28;
}

int Item::vf60() {
    int ret = 0;
    if (m54 == 2 && mDesc->m7C != 0) {
        ret = 1;
    }
    return ret;
}

int Item::vf48(const atn::Vector3* pos, const atn::Vector3* dir, int flag) {
    if (vf60() != 0 && flag == 0 && lbl_8087F8A0->mHead->m50 != 700023) {
        atn::Vector3 d = mDrop->m6C - *pos;
        float len = PSVECSquareMag(&d);
        if (len > 0.0001f && len < 225.0f) {
            PSVECNormalize(&d, &d);
            int ret = 1;
            if (!(PSVECDotProduct(dir, &d) >= 0.5f)) {
                bool near = len < 4.0f;
                if (!near) {
                    ret = 0;
                }
            }
            return ret;
        }
    }
    return 0;
}

void Item::vf4C(CharaTask* chara) {
    if (vf60() != 0) {
        mDrop->m194 = 1;
    }
}

void Item::fn_803FC870() {
    mDrop->m198 = 1;
}

} // namespace gmk
