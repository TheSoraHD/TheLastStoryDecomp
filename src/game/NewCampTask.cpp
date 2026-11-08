#include "game/NewCampTask.h"
#include "game/GameTask.h"
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
#include "game/NpcManager.h"
#include "game/NpcTask.h"
#include "game/PlayerManager.h"
#include "game/Inventory.h"
#include "game/CharaStatus.h"
#include "game/GameUI.h"
#include "atn/RenderManager.h"
#include "atn/RenderSetting_unk.h"
#include "atn/LH_Xsca.h"
#include "game/NewCampPartsBase.h"

extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
extern "C" atn::SoundRef fn_80117228(int id); // inferred: plays a system sound effect
// inferred: field (lbl_8087F430); only the members used here
struct Field_unk {
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

// ---- fn_8052C108: opens the camp (over the inlining budget: the helpers below are weak copies at
// 0x8052C64C-0x8052C6C0) ----
// inferred: party members available at a progress (as game/ShopBuySell.cpp)
struct ShopMembers_unk {
    unsigned char m00[0x0C];
    unsigned char mIn[8];
};
extern "C" ShopMembers_unk* fn_80217D9C(int progress); // inferred: members available at a progress (as game/ShopBuySell.cpp)
extern "C" int fn_80219544(int member); // inferred: character id of a party member
extern "C" void* memcpy(void* dst, const void* src, unsigned long n);
extern "C" void fn_804ABF20();
namespace atn {
// inferred: only the member used here (atn/PostFx.h clashes with atn/IRender.h)
class PostFxManager {
public:
    void fn_800A97C4();
};
} // namespace atn
extern "C" atn::PostFxManager* lbl_8087EF8C;

// inferred: objects of the camp at 0x468 and 0xB1C (classes unknown; reset when the camp opens)
struct NewCamp468_unk {
    void fn_8052FA44();
};
struct NewCampB1C_unk {
    void fn_8052E1A0();
};

extern "C" PlayerManager* lbl_8087F8A0;
extern "C" RenderSetting_unk* lbl_8087EFA8; // render settings (atn::RenderConfig)
// inferred: global accessors (inline: weak copies of other files; they count for the inline budget)
extern "C" inline Field_unk* fn_80121F00() {
    return lbl_8087F430;
}
extern "C" inline PlayerManager* fn_8000D9E8() {
    return (PlayerManager*)lbl_8087F8A0;
}
extern "C" inline RenderSetting_unk* fn_802F0990() {
    return lbl_8087EFA8;
}
inline SetConfig* GetSetConfig_unk() {
    return SetConfig::sInstance;
}
extern "C" inline int fn_80244CA0(atn::Task* task) {
    return (task->mFlags >> 1) & 1;
}
// inferred: copies a camera (defined in game/GameCamera_unk.cpp)
extern "C" inline void fn_80392A04(atn::Camera* dst, const atn::Camera* src) {
    *dst = *src;
}
extern "C" inline void fn_8037F688(atn::Camera* camera, float fovy) {
    camera->mFovy = fovy;
}
inline void RenderSetting_unk::SetDamage_unk(DamageEffectParam_unk p) {
    m374 = p;
}
// inferred: weak copies of game/NewCampTask.cpp (C names: their classes are unknown)
extern "C" inline int fn_8052C64C(GameTask* task) {
    return task->m10D0;
}
extern "C" inline NpcManager* fn_8052C654() {
    return NpcManager::sInstance;
}
extern "C" inline NpcTask* fn_8052C65C(NpcTask* npc) {
    return npc->mNext;
}
// inferred: copies the base status of a character (the vtable too)
extern "C" inline void fn_8052C664(CharaStatus* dst, CharaStatus* src) {
    memcpy(dst, src, sizeof(CharaStatusBase));
}
extern "C" inline void fn_8052C66C(CharaStatus* status, CharaTask* chara) {
    status->mChara = chara;
}
extern "C" inline void fn_8052C674(CharaStatus* status) {
    status->mRefs.mCount = 0;
}
extern "C" inline NpcTask* fn_8052C990(NpcManager* mgr) {
    return mgr->mHead;
}
extern "C" inline void* fn_8052C680(NewCampPartsBase* parts) {
    return parts->m5C.fn_804786F8();
}
// inferred: point of the camera curves at `time` (channels x, y, z and the target channels)
extern "C" inline atn::Vector3 fn_8052C688(XsfCurveSet_unk* curves, float time) {
    atn::Vector3 v;
    curves->fn_805BF410(time, &v.x, 1, 2, 3);
    return v;
}
extern "C" inline atn::Vector3 fn_8052C6A4(XsfCurveSet_unk* curves, float time) {
    atn::Vector3 v;
    curves->fn_805BF410(time, &v.x, 7, 8, 9);
    return v;
}
extern "C" inline void fn_8052C6C0(atn::RenderManager* rm) {
    rm->m554 = 1;
}
// inferred: second camera of the render manager (m558)
extern "C" inline void fn_8052C6CC(atn::RenderManager* rm, const atn::Camera* camera) {
    rm->m558 = *camera;
}
// inferred: weak copy before fn_8052C108 (first used by vf10 in the original)
extern "C" inline int fn_8052C0E8(SetConfig* config) {
    return 0;
}

// NONMATCHING: draft - opens the camp: lists the party members, copies the status of the absent ones,
// sets the camera of the camp and the parts
void NewCampTask::fn_8052C108(NewCampOpen_unk* open) {
    m5C = 1;
    fn_80117228(0x1F);
    ((GameUICounter_unk*)&m1224)->fn_803CF734();
    ((GameUICounter_unk*)&m1268)->fn_803CF734();
    NewCampPair_unk pair;
    pair.mId = 0;
    pair.m04 = (int)fn_8000D9E8()->fn_8054A33C(pair.mId);
    m1224.push_back(pair);
    ((atn::FixedArray_unk<NewCampPair_unk, 8>*)&m1268)->push_back(pair);
    ShopMembers_unk* members = fn_80217D9C(fn_8052C64C((GameTask*)fn_80121F00()));
    if (members != 0) {
        for (unsigned int i = 1; i < 7; i++) {
            int left = 0;
            switch (i) {
            case 0:
                left = ((GameTask*)fn_80121F00())->fn_80370174(0x124);
                break;
            case 1:
                left = ((GameTask*)fn_80121F00())->fn_80370174(0x125);
                if (fn_8052C64C((GameTask*)fn_80121F00()) <= 0x29C8 &&
                    ((GameTask*)fn_80121F00())->fn_80370174(0x764) < 1) {
                    left = 1;
                }
                break;
            case 2:
                left = ((GameTask*)fn_80121F00())->fn_80370174(0x126);
                break;
            case 3:
                left = ((GameTask*)fn_80121F00())->fn_80370174(0x127);
                break;
            case 4:
                left = ((GameTask*)fn_80121F00())->fn_80370174(0x128);
                break;
            case 5:
                left = ((GameTask*)fn_80121F00())->fn_80370174(0x12A);
                break;
            case 6:
                left = ((GameTask*)fn_80121F00())->fn_80370174(0x129);
                break;
            }
            if (members->mIn[i] == 0 || left != 0) {
                continue;
            }
            pair.mId = i;
            CharaTask* chara = fn_8000D9E8()->FindByName_unk(i);
            if (chara == 0) {
                int id = fn_80219544(pair.mId);
                for (NpcTask* npc = fn_8052C990(fn_8052C654()); npc != 0; npc = fn_8052C65C(npc)) {
                    if (id == npc->GetCharaNo_unk() && fn_80244CA0(npc) && !npc->IsFrozen_unk()) {
                        chara = npc;
                        break;
                    }
                }
            } else {
                CharaStatus* status = chara->GetStatus_unk();
                fn_8052C664(Inventory::Order()->GetPlayerStatusInfo(pair.mId), status);
                fn_8052C66C(Inventory::Order()->GetPlayerStatusInfo(pair.mId), 0);
                fn_8052C674(Inventory::Order()->GetPlayerStatusInfo(pair.mId));
            }
            if (chara != 0 && (!fn_80244CA0(chara) || chara->IsFrozen_unk())) {
                chara = 0;
            }
            pair.m04 = (int)chara;
            m1224.push_back(pair);
            if (chara != 0) {
                ((atn::FixedArray_unk<NewCampPair_unk, 8>*)&m1268)->push_back(pair);
            }
        }
    }
    if (fn_80121F00() != 0) {
        ((GameTask*)fn_80121F00())->fn_8037529C(0xFFFFFFFF, 0, 0, 0, 10);
    }
    mOpen = *open;
    mTop->vf24();
    mStatus->vf24();
    mMapSelect->vf24();
    mItem->vf24();
    mOption->vf24();
    mProtector->vf24();
    mGmk->vf24();
    mHint->vf24();
    m1308 = 0;
    ((NewCampB1C_unk*)&m468[0xB1C - 0x468])->fn_8052E1A0();
    ((NewCamp468_unk*)m468)->fn_8052FA44();
    atn::Vector3 ofs(m136C != 0 ? m1370 : atn::Vector3(0.0f, 0.0f, 0.0f));
    XsfCurveSet_unk* curves = ((Xsca_unk*)fn_8052C680((NewCampPartsBase*)mStatus))->fn_805BDCBC(0);
    m11FC = fn_8052C688(curves, 0.0f);
    m1208 = fn_8052C6A4(curves, 0.0f);
    mCamera.SetPosition(m11FC + ofs);
    mCamera.SetTarget_unk(m1208 + ofs);
    fn_8037F688(&mCamera, m1214);
    mCamera.fn_8004B378();
    fn_8008B964()->SetCamera_unk(mCamera);
    fn_80392A04((atn::Camera*)m260, &mCamera);
    fn_8052C6C0(fn_8008B964());
    fn_8052C6CC(fn_8008B964(), (atn::Camera*)m260);
    DamageEffectParam_unk& damage = *(DamageEffectParam_unk*)&m130C[0x1310 - 0x130C];
    damage.mEnabled = 1;
    fn_802F0990()->SetDamage_unk(damage);
    if (mOpen.mMenu != 0) {
        fn_8052DEEC(mOpen.mMenu);
    } else {
        mGmk->fn_80512C90(m464);
    }
    mGmk->fn_80513020(m1354, m464);
    m1360 = fn_8052C0E8(GetSetConfig_unk());
    fn_80530DB4();
    fn_804ABF20();
    lbl_8087EF8C->fn_800A97C4();
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
            ((GameTask*)lbl_8087F430)->fn_8037529C(-1, 0xFFFFFF, 0, 0, 10);
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
        ((GameTask*)lbl_8087F430)->fn_803750E4(0x146);
        m11F0 = (NewCampPartsBase*)mStatus;
        break;
    case 2:
        ((GameTask*)lbl_8087F430)->fn_803750E4(0xEC);
        if (lbl_8087FA20 != 0) {
            ((GameTask*)lbl_8087F430)->fn_803750E4(0x1BA);
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
