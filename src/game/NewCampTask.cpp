#include "game/NewCampTask.h"
#include "atn/FadeCommon.h"
#include "atn/Model_unk.h"
#include "atn/SoundRef.h"
#include "atn/EffectManager.h"
#include "game/NewCampPartsTop.h"
#include "game/NewCampPartsStatus.h"
#include "game/NewCampPartsMapSelect.h"
#include "game/NewCampPartsItem.h"
#include "game/NewCampPartsOption.h"
#include "game/CosEditProtector.h"
#include "game/NewCampPartsHint.h"
#include "game/NewCampPartsGmk.h"
#include "game/MapTask.h"
#include "game/MapPartsArea.h"
#include "game/MapArea.h"
#include "game/MenuCommonTask.h"
#include "game/GameConfig_unk.h"
#include "game/SetConfig.h"
#include "atn/SoundManager.h"

extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
extern "C" atn::SoundRef fn_80117228(int id); // inferred: plays a system sound effect
// inferred: field (lbl_8087F430); only the members used here
struct Field_unk {
    void fn_803750E4(int id); // inferred: (as in gmk/RandomMerchant.cpp)
    unsigned char m0000[0x563C];
    int m563C; // inferred: reason of the next map change
};
extern "C" Field_unk* lbl_8087F430;
extern "C" void* lbl_8087FA20; // inferred: town task
// inferred: xsca camera curves (as in game/Em309Ghoul.cpp)
struct XsfCurveSet_unk {
    void fn_805BF410(float time, float* out, int x, int y, int z);
    unsigned char m00[0x14];
    float mLength; // 0x14
};
struct Xsca_unk {
    XsfCurveSet_unk* fn_805BDCBC(unsigned int index);
};
extern "C" void fn_8037529C(Field_unk* field, int a, unsigned int mask, int b, int c, int d);

extern "C" void* lbl_8087EF70; // inferred: pad
extern "C" float fn_800A56A8(void* pad, int port, int axis, int arg); // inferred: stick axis
extern "C" GameConfig_unk* lbl_8087F0A8;

static NewCampPair_unk sNullPair; // inferred: returned by fn_80530384 for a missing id

// inferred: point of the start of the camera animation of `parts` (channels x, y, z; name invented)
static inline atn::Vector3 GetCurvePos_unk(NewCampPartsBase* parts, int x, int y, int z) {
    atn::Vector3 pos;
    ((Xsca_unk*)parts->m5C.fn_804786F8())->fn_805BDCBC(0)->fn_805BF410(0.0f, &pos.x, x, y, z);
    return pos;
}

void NewCampTask::vf10() {
    if (m133C != 0) {
        fn_8052C9C4();
        return;
    }
    if ((float)__fabs(fn_800A56A8(lbl_8087EF70, 0, 4, 0)) > 2.0f && m1308 == 0) {
        SetConfig::sInstance->mCampVariation = 1;
        m1308 = 15;
    }
    if (m1308 > 0) {
        m1308--;
    }
    if (m1360 != 0) {
        fn_80530DB4();
        m1360 = 0;
    }
    fn_8052D468();
    int idle = 1;
    MenuCommonTask* menu = MenuCommonTask::sInstance;
    if (menu != 0 && (menu->m4C != 0 || menu->m54 != 0 || menu->m64 != 0)) {
        idle = 0;
    }
    if (idle && m11F8 == 0) {
        if (m464 != 0) {
            ((NewCampPartsBase*)mTop)->vf3C(0);
        }
        switch (m464) {
        case 0:
            ((NewCampPartsBase*)mTop)->vf2C();
            if (m1304 == 0 && m11F4 != 0 && m11F4 != (NewCampPartsBase*)mTop) {
                m11F4->vf3C(0);
            }
            m1304 = 1;
            break;
        case 1:
            ((NewCampPartsBase*)mStatus)->vf2C();
            break;
        case 2:
            ((NewCampPartsBase*)mMapSelect)->vf2C();
            break;
        case 4:
            ((NewCampPartsBase*)mItem)->vf2C();
            break;
        case 5:
        case 7:
            ((NewCampPartsBase*)mProtector)->vf2C();
            break;
        case 6:
            ((NewCampPartsBase*)mOption)->vf2C();
            break;
        case 8:
            ((NewCampPartsBase*)mHint)->vf2C();
            break;
        }
    }
    if (lbl_8087F0A8->mInput.fn_801231D0(0x10) && m11F0->vf38() && !mProtector->fn_801D0BAC() &&
        !mTop->fn_805294E4() && mMapSelect->m1998 != 3 && mStatus->mTutorial == 0 && mHint->mTutorial == 0) {
        fn_80530174();
        fn_8052C9C4();
        if (lbl_8087EFE8 != 0) {
            lbl_8087EFE8->fn_800CFBA0(1, 0, 0x20, 30);
            lbl_8087EFE8->fn_800CFBA0(2, 0, 0x20, 15);
        }
    }
    if (lbl_8087F3C0 != 0) {
        lbl_8087F3C0->fn_8023A614(6);
    }
    if (m1358 != m1354) {
        mGmk->fn_80513020(m1354, m464);
    }
    m1358 = m1354;
}

void NewCampTask::vf14() {
    if (lbl_8087F3C0 != 0) {
        lbl_8087F3C0->fn_8023A664((void*)1);
    }
}

void NewCampTask::fn_8052C9C4() {
    if (m133C == 0) {
        if (lbl_8087F430 != 0) {
            lbl_8087F430->m563C = 0x12;
        }
        m133C = 1;
        fn_80117228(0x20);
    } else {
        fn_8052CA6C();
        m133C = 0;
        if (lbl_8087F430 != 0) {
            fn_8037529C(lbl_8087F430, -1, 0xFFFFFF, 0, 0, 10);
        }
    }
    m5C = 0;
}

void NewCampTask::fn_8052DEEC(int state) {
    int prev = m464;
    int top = 0;
    m11F4 = m11F0;
    m464 = state;
    switch (state) {
    case 0:
        m11F0 = (NewCampPartsBase*)mTop;
        top = 1;
        break;
    case 1:
        lbl_8087F430->fn_803750E4(0x146);
        m11F0 = (NewCampPartsBase*)mStatus;
        break;
    case 2:
        lbl_8087F430->fn_803750E4(0xEC);
        if (lbl_8087FA20 != 0) {
            lbl_8087F430->fn_803750E4(0x1BA);
        }
        m11F0 = (NewCampPartsBase*)mMapSelect;
        break;
    case 4:
        m11F0 = (NewCampPartsBase*)mItem;
        break;
    case 5:
        m11F0 = (NewCampPartsBase*)mProtector;
        mProtector->m1E0 = 0;
        break;
    case 7:
        m11F0 = (NewCampPartsBase*)mProtector;
        mProtector->m1E0 = 1;
        break;
    case 6:
        m11F0 = (NewCampPartsBase*)mOption;
        break;
    case 8:
        m11F0 = (NewCampPartsBase*)mHint;
        break;
    }
    m11F0->vf30();
    if (m464 != 6 && m464 != 1) {
        m11F0->vf3C(1);
    }
    if (prev == 0 && m464 != 0) {
        m1340 = fn_80117228(7);
    }
    if (top) {
        if (m11F4 != 0) {
            m11F8 = 1;
            m454 = m11F4->m5C.fn_804786F8();
            m458 = ((Xsca_unk*)m11F4->m5C.fn_804786F8())->fn_805BDCBC(0)->mLength;
            m45C = -m1368;
        } else {
            m11F8 = 0;
            m11FC = GetCurvePos_unk((NewCampPartsBase*)mStatus, 1, 2, 3);
            m1208 = GetCurvePos_unk((NewCampPartsBase*)mStatus, 7, 8, 9);
        }
    } else {
        m11F8 = 1;
        m454 = m11F0->m5C.fn_804786F8();
        m458 = 0.0f;
        m45C = m1368;
    }
    if (m5C != 0) {
        mGmk->fn_80512C90(m464);
    }
    m1304 = 0;
}

// NONMATCHING: 96% - order of the store of m11F8 and float registers of the camera sums
void NewCampTask::fn_80530174() {
    m11FC = atn::Vector3(40.0f, 0.0f, 210.0f);
    m1208 = atn::Vector3(40.0f, 0.0f, -20.0f);
    m1214 = 0.9599311f;
    m11F8 = 0;
    m458 = 0.0f;
    m45C = m1368;
    m121C = 0;
    m1220 = 0;
    m11F0 = 0;
    m12EC = lbl_807C7030;
    m12F8 = lbl_807C7030;
    m12D4 = lbl_807C7030;
    m12E0 = lbl_807C7030;
    atn::Vector3 offset = m136C ? m1370 : atn::Vector3(0.0f, 0.0f, 0.0f);
    mCamera.SetPosition(m11FC + offset);
    mCamera.SetTarget_unk(m1208 + offset);
    mCamera.mFovy = m1214;
    fn_8052DEEC(0);
}

// NONMATCHING: 90% - the original loads both vector sizes before clearing them (same as
// Event::EffectLists_unk::Clear_unk)
void NewCampTask::fn_8053032C(atn::Vector3 pos, atn::Vector3 rot) {
    m12AC = 0;
    m12B0.Clear_unk();
    m12D4 = pos;
    m12E0 = rot;
}

NewCampPair_unk& NewCampTask::fn_80530384(int id) {
    for (unsigned int i = 0; i < m1224.size(); i++) {
        if (id == m1224.mData[i].mId) {
            return m1224.mData[i];
        }
    }
    return sNullPair;
}

void NewCampTask::fn_80530CD0(int duration) {
    m64->mFrame = 0;
    m64->mDelay = 0;
    m64->mDuration = duration;
    m64->mWait = 999999;
    m64->mColor0 = 0;
    m64->mColor1 = 0xFF000000;
    m64->mZ = -100000.0f;
    m64->mActive = 1;
    m68 = 1;
}

void NewCampTask::fn_80530D30(int duration) {
    m64->mFrame = 0;
    m64->mDelay = 0;
    m64->mDuration = duration;
    m64->mWait = 0;
    m64->mColor0 = 0xFF000000;
    m64->mColor1 = 0;
    m64->mZ = -100000.0f;
    m64->mActive = 1;
    m68 = 0;
}

// NONMATCHING: 80% - the original keeps a dead blr after the tail call
void NewCampTask::fn_80530D88(int set, atn::ModelBase* model) {
    if (model == 0) {
        return;
    }
    if (0 <= set && set < 3) {
        ((atn::Model*)model)->fn_8008C828(0);
    }
}

// NONMATCHING: 97% - the original copies the alpha of the fog color again as a word after the assignment
void NewCampTask::fn_80530DB4() {
    m1354 = 0;
    m1358 = 0;
    mGmk->fn_80513020(0, m464);
    mTop->vf44();
    mStatus->vf44();
    mMapSelect->vf44();
    mItem->vf44();
    mOption->vf44();
    mProtector->vf44();
    mHint->vf44();
    mTop->vf48();
    mStatus->vf48();
    mMapSelect->vf48();
    mItem->vf48();
    mOption->vf48();
    mProtector->vf48();
    mHint->vf48();
    m460->GetArea_unk()->mAreas[0]->mScene.mFog.mColor = atn::Color(0.68235296f, 0.7254902f, 0.76862746f, 1.0f);
}
