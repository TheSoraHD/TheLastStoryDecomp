#include "gmk/InteractionSpot.h"
#include "atn/Memory.h"
#include "chr/ControlBase.h"

// inferred: point of the field (0x30 bytes)
struct Spot_unk {
    int mId;
    atn::Vector3 mPos; // 0x04
    int m10;           // inferred: index (1-based) used by the links
    unsigned char m14[0x30 - 0x14];
};
// inferred: two linked points (indices into the point table)
struct SpotLink_unk {
    int mA;
    int mB;
};
// inferred: point tables of the field (lbl_8087F430->m10D8)
struct SpotTable_unk {
    Spot_unk* FindSpot_unk(int id) {
        for (unsigned int i = 0; i < mNumSpots; i++) {
            if (mSpots[i].mId == id) {
                return &mSpots[i];
            }
        }
        return 0;
    }
    Spot_unk* FindLinkA_unk(Spot_unk* spot) {
        for (unsigned int i = 0; i < mNumLinks; i++) {
            SpotLink_unk& link = mLinks[i];
            if (spot->m10 - 1 == link.mA) {
                return &mSpots[link.mB];
            }
        }
        return 0;
    }
    Spot_unk* FindLinkB_unk(Spot_unk* spot) {
        for (unsigned int i = 0; i < mNumLinks; i++) {
            SpotLink_unk& link = mLinks[i];
            if (spot->m10 - 1 == link.mB) {
                return &mSpots[link.mA];
            }
        }
        return 0;
    }

    unsigned char m00[0xC4];
    unsigned int mNumSpots; // 0xC4
    Spot_unk* mSpots;       // 0xC8
    unsigned int mNumLinks; // 0xCC
    SpotLink_unk* mLinks;   // 0xD0
};
struct Field_unk {
    unsigned char m00[0x10D8];
    SpotTable_unk* m10D8;
};
extern "C" Field_unk* lbl_8087F430;
extern "C" int fn_803C1560(SpotTable_unk* table, const atn::Vector3* pos, int arg2, int arg3, int arg4, int arg5,
                           float arg6);

namespace gmk {

GimmickCommon* InteractionSpot::fn_80428B0C(atn::Task* parent, GimmickDesc_unk* desc) {
    if (parent != 0) {
        return new (12, "", "", 0) InteractionSpot(parent, desc);
    }
    return 0;
}

int InteractionSpot::vf0C() {
    if (fn_803EC91C() == 0) {
        m54 = 2;
        return 1;
    }
    return 0;
}

// NONMATCHING: register allocation only (the target keeps the table in r7 and returns the spot in
// the register of the id). Tried: table/field/id locals, reference and reversed comparisons.
void InteractionSpot::vf28() {
    m6C = mDesc->mPos;
    m78.y = mDesc->mRotY;
    SpotTable_unk* table = lbl_8087F430->m10D8;
    mSpot[0] = table->FindSpot_unk(mDesc->m20);
    mSpot[1] = table->FindLinkA_unk(mSpot[0]);
    if (mSpot[1] == 0) {
        mSpot[1] = table->FindLinkB_unk(mSpot[0]);
    }
    m10C = fn_803C1560(table, &mSpot[0]->mPos, 0, 0, 0, 1, -1.0f);
    fn_8042952C(0);
}

void InteractionSpot::fn_8042952C(int state) {
    mState = state;
    mTimer = 0;
    if (state == 11) {
        for (int i = 0; i < 2; i++) {
            if (mChara[i] != 0) {
                ((chr::Chara_unk*)mChara[i])->fn_8015495C(0, 1, 1, 1);
                if (((chr::Chara_unk*)mChara[i])->m55C == 6) {
                    if (mChara[i]->mType == 0) {
                        ((chr::Chara_unk*)mChara[i])->m564 = 1;
                    } else {
                        ((chr::Chara_unk*)mChara[i])->m564 = 2;
                    }
                } else {
                    mChara[i]->fn_8016EB48(0, 0, 0);
                }
                ((chr::Chara_unk*)mChara[i])->mC58.mHeld_unk = 0;
            }
            mPrev[i] = mChara[i];
            mChara[i] = 0;
        }
        fn_8042952C(0);
    }
}

void InteractionSpot::vf14() {}

int InteractionSpot::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    switch (msg->mType) {
    case 1:
    case 2:
        m54 = msg->mType;
        break;
    }
    return m54;
}

int InteractionSpot::vf60() {
    int ret = 0;
    if (m54 == 2 && mDesc->m7C != 0) {
        ret = 1;
    }
    return ret;
}

} // namespace gmk
