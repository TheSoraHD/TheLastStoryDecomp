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
#include "atn/GraphicsDevice.h"
#include "game/NewCampPartsBase.h"
#include "atn/Csv.h"

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

// inferred: 0x64-byte record of camp_font_conf.csv (list lbl_807C912C, read by vf0C): colours of a font;
// names invented
struct NewCampElem_unk {
    int m00;
    char mName[0x20];   // 0x04: inferred: name of the debug menu folder
    int m24;
    int mBase[3];       // 0x28: R, G, B
    int mBaseShadow[4]; // 0x34: A, R, G, B
    int m44;
    int mAfter[3];       // 0x48: R, G, B
    int mAfterShadow[4]; // 0x54: A, R, G, B
};
static std::vector<NewCampElem_unk> sElems_unk; // inferred: cleared by the constructor
static NewCampPair_unk sNullPair; // inferred: returned by fn_80530384 for a missing id

extern "C" atn::GraphicsDevice_unk* lbl_8087EEE0;
extern "C" float fn_80076FF8(atn::GraphicsDevice_unk* device); // inferred: screen aspect ratio
inline atn::GraphicsDevice_unk* GetGraphics_unk() {
    return lbl_8087EEE0;
}
// inferred: setters (weak copies; C names: the method names are unknown)
extern "C" inline void fn_8052ACC8(atn::Camera* camera, float aspect) {
    camera->mAspect = aspect;
}
extern "C" inline void fn_8052ACD0(MapTask* map, int type) {
    map->mType = type;
}
extern "C" inline void fn_8037F688(atn::Camera* camera, float fovy);

extern "C" atn::Task* fn_8052A888(atn::Task* parent) {
    if (parent != 0) {
        return new ((MemoryTag)1, "", "", 0) NewCampTask(parent);
    }
    return 0;
}

NewCampTask::NewCampTask(atn::Task* parent)
    : atn::Task(parent), m5C(0), m60(0), m454(0), m458(0.0f), m45C(3.0f), m464(0), m11F0(0), m11F4(0), m11F8(0),
      m11FC(40.0f, 0.0f, 210.0f), m1208(40.0f, 0.0f, -20.0f), m1214(atn::DegToRad_unk(55.0f)), m121C(0), m1220(0),
      m12AC(0), m12D0(0.0f), m12D4(lbl_807C7030), m12E0(lbl_807C7030), m12EC(lbl_807C7030), m12F8(lbl_807C7030),
      m1304(1), m1308(0), m133C(0), m1350(0), m1354(0), m1358(0), m135C(60), m1364(0), m1368(3.0f), m136C(0),
      m1370(0.0f, 0.0f, 80.0f) {
    m134C.fn_80470364("/preload/camp.pk", 0, 0, 1);
    sElems_unk.clear();
    m1344.vf0C("database/camp/camp_font_conf.csv");
    fn_8052ACC8(&mCamera, fn_80076FF8(GetGraphics_unk()));
    if (fn_80076FF8(GetGraphics_unk()) < 1.5f) {
        m136C = 1;
        fn_8052ACC8(&mCamera, 1.3333334f);
    }
    atn::Vector3 offset(m136C != 0 ? m1370 : atn::Vector3(0.0f, 0.0f, 0.0f));
    mCamera.SetPosition(m11FC + offset);
    mCamera.SetTarget_unk(m1208 + offset);
    fn_8037F688(&mCamera, m1214);
    m460 = MapTask::SetTask(this, "database/map/ui_campmenu.map");
    fn_8052ACD0(m460, 6);
    m460->fn_800D246C(1);
    mTop = NewCampPartsTop::fn_80529118(this);
    mTop->fn_800D246C(1);
    mStatus = NewCampPartsStatus::fn_805268C8(this);
    mStatus->fn_800D246C(1);
    mMapSelect = NewCampPartsMapSelect::fn_8051B920(this);
    mMapSelect->fn_800D246C(1);
    mItem = NewCampPartsItem::fn_805169F8(this);
    mItem->fn_800D246C(1);
    mOption = NewCampPartsOption::fn_80521E1C(this);
    mOption->fn_800D246C(1);
    mProtector = CosEditProtector::fn_801CFD68(this);
    mProtector->fn_800D246C(1);
    mGmk = NewCampPartsGmk::fn_805128C0(this);
    mGmk->fn_800D246C(1);
    mHint = NewCampPartsHint::fn_80514720(this);
    mHint->fn_800D246C(1);
    m1334.vf0C("database/camp/camp_config.csv");
    m64 = atn::FadeColor::Create(this, 0, 0, 0);
    m1360 = -1;
    m12B0.Clear_unk();
}

NewCampTask::~NewCampTask() {}

namespace atn {
class Loader;
}
atn::Loader* ResMgr_unk(); // inferred: loader of the game (lbl_8087F518)
extern "C" void fn_8046ECDC(atn::Loader* loader, int show); // inferred: shows the loading screen
// inferred: debug menu (atn/MindowsCommon.h; arguments in the order of game/NewCampPartsBase.cpp)
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4); // AddBool
extern "C" void fn_80087E9C(void* group, const char* name, void* value, int arg3, int arg4, float min, float max,
                            float step); // x, y, z
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step); // AddFloat
extern "C" void fn_80087858(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step); // AddRadian
extern "C" void fn_800874C8(void* group, const char* name, int* value, int min, int max, int step, int arg6,
                            int arg7); // AddInt
// inferred: gamma settings of the render settings (weak copy)
extern "C" inline GammaParams_unk* fn_8052B7F0(RenderSetting_unk* settings) {
    return &settings->m3DC;
}
extern "C" RenderSetting_unk* fn_802F0990();

// inferred: loading of the camp: waits for its files, prepares the parts and the post effects and adds the debug
// menu items
// NONMATCHING: 99.5% - registers of the debug menu group (r27/r28); with NewCampView_unk::fn_8052E1A0 written
// (auto-inline candidate) the function is over the inlining budget (R3) as in the original and calls the reset,
// the gamma accessor, ResHandle::Load and the vector accessors out of line (it was 33% before)
int NewCampTask::vf0C() {
    fn_8046ECDC(ResMgr_unk(), 0);
    if (m60 == 0) {
        m60 = 1;
    } else if (m60 == 1) {
        m60 = 2;
    } else if (m60 == 2) {
        if (fn_800D3FA4() && !m1334.fn_80473F50() && !m1344.fn_80473F50()) {
            m460->fn_800D246C(0);
            mTop->vf1C((NewCampScene_unk*)m460);
            mTop->fn_800D246C(0);
            mStatus->vf1C((NewCampScene_unk*)m460);
            mStatus->fn_800D246C(0);
            mMapSelect->vf1C((NewCampScene_unk*)m460);
            mMapSelect->fn_800D246C(0);
            mItem->vf1C((NewCampScene_unk*)m460);
            mItem->fn_800D246C(0);
            mOption->vf1C((NewCampScene_unk*)m460);
            mOption->fn_800D246C(0);
            mProtector->vf1C((NewCampScene_unk*)m460);
            mProtector->fn_800D246C(0);
            mGmk->vf1C((NewCampScene_unk*)m460);
            mGmk->fn_800D246C(0);
            mHint->vf1C((NewCampScene_unk*)m460);
            mHint->fn_800D246C(0);
            fn_8052DEEC(0);
            fn_8052B7F0(fn_802F0990())->mEnable = 0;
            m468.fn_8052E1A0();
            fn_802F0990()->ResetPostFx_unk();
            fn_805303D0();
            fn_805305D0();
            void* group = m1218.Load("NewCamp");
            fn_80087994(group, "FreeCamera", &m121C, 0, 0);
            fn_80087E9C(group, "CameraOfs(Aspect4:3)", &m1370, 0, 0, -1000.0f, 1000.0f, 1.0f);
            fn_8008771C(group, "CameraSpeed", &m1368, 0, 0, 0.0f, 100.0f, 1.0f);
            fn_80087994(group, "DispLocId", &m1220, 0, 0);
            fn_800874C8(group, "InfoOnTime", &m135C, 0, 999999, 1, 0, 0);
            fn_80087858(group, "CamAng", &m1214, 0, 0, 0.0f, 360.0f, 1.0f);
            fn_80087994(group, "UnlockMapAll", &m1350, 0, 0);
            fn_80087994(group, "MapLinkDebug", &m1364, 0, 0);
            fn_800874C8(group, "BackType", &m1354, 0, 7, 1, 0, 0);
            mTop->vf40(group);
            mStatus->vf40(group);
            mMapSelect->vf40(group);
            mItem->vf40(group);
            mOption->vf40(group);
            mProtector->vf40(group);
            mGmk->vf40(group);
            mHint->vf40(group);
            for (unsigned int i = 0; i < sElems_unk.size(); i++) {
                NewCampElem_unk* elem = &sElems_unk[i];
                void* folder = fn_8008937C(group, elem->mName);
                void* sub = fn_8008937C(folder, "base");
                fn_800874C8(sub, "R", &elem->mBase[0], 0, 0xFF, 1, 0, 0);
                fn_800874C8(sub, "G", &elem->mBase[1], 0, 0xFF, 1, 0, 0);
                fn_800874C8(sub, "B", &elem->mBase[2], 0, 0xFF, 1, 0, 0);
                sub = fn_8008937C(folder, "base_sdw");
                fn_800874C8(sub, "A", &elem->mBaseShadow[0], 0, 0xFF, 1, 0, 0);
                fn_800874C8(sub, "R", &elem->mBaseShadow[1], 0, 0xFF, 1, 0, 0);
                fn_800874C8(sub, "G", &elem->mBaseShadow[2], 0, 0xFF, 1, 0, 0);
                fn_800874C8(sub, "B", &elem->mBaseShadow[3], 0, 0xFF, 1, 0, 0);
                sub = fn_8008937C(folder, "after");
                fn_800874C8(sub, "R", &elem->mAfter[0], 0, 0xFF, 1, 0, 0);
                fn_800874C8(sub, "G", &elem->mAfter[1], 0, 0xFF, 1, 0, 0);
                fn_800874C8(sub, "B", &elem->mAfter[2], 0, 0xFF, 1, 0, 0);
                sub = fn_8008937C(folder, "after_sdw");
                fn_800874C8(sub, "A", &elem->mAfterShadow[0], 0, 0xFF, 1, 0, 0);
                fn_800874C8(sub, "R", &elem->mAfterShadow[1], 0, 0xFF, 1, 0, 0);
                fn_800874C8(sub, "G", &elem->mAfterShadow[2], 0, 0xFF, 1, 0, 0);
                fn_800874C8(sub, "B", &elem->mAfterShadow[3], 0, 0xFF, 1, 0, 0);
            }
            return 1;
        }
    }
    return 0;
}

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
namespace atn {
// inferred: only the member used here (atn/PostFx.h clashes with atn/IRender.h)
class PostFxManager {
public:
    void fn_800A97C4();
};
} // namespace atn
extern "C" atn::PostFxManager* fn_804ABF20(); // inferred: post effect manager

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
// NONMATCHING: 99.96% - order of the copy of the frustum planes (as fn_80392A04 of game/GameCamera_unk.cpp)
extern "C" inline void fn_8052C6CC(atn::RenderManager* rm, const atn::Camera* camera) {
    rm->m558 = *camera;
}
// inferred: weak copy before fn_8052C108 (first used by vf10 in the original)
extern "C" inline int fn_8052C0E8(SetConfig* config) {
    return 0;
}

// inferred: opens the camp: lists the party members (with the status of the ones in the party), sets the
// camera of the camp and the parts
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
                if (fn_8052C64C((GameTask*)fn_80121F00()) <= 109000 &&
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
                fn_8052C664(Inventory::Order()->GetPlayerStatusInfo(pair.mId), chara->GetStatus_unk());
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
    mB1C.fn_8052E1A0();
    m468.fn_8052FA44();
    atn::Vector3 ofs(m136C != 0 ? m1370 : atn::Vector3(0.0f, 0.0f, 0.0f));
    XsfCurveSet_unk* curves = ((Xsca_unk*)fn_8052C680((NewCampPartsBase*)mStatus))->fn_805BDCBC(0);
    m11FC = fn_8052C688(curves, 0.0f);
    m1208 = fn_8052C6A4(curves, 0.0f);
    mCamera.SetPosition(m11FC + ofs);
    mCamera.SetTarget_unk(m1208 + ofs);
    fn_8037F688(&mCamera, m1214);
    mCamera.fn_8004B378();
    fn_8008B964()->SetCamera_unk(mCamera);
    fn_80392A04(&m260, &mCamera);
    fn_8052C6C0(fn_8008B964());
    fn_8052C6CC(fn_8008B964(), &m260);
    ((DamageEffectParam_unk*)&m130C[0x1310 - 0x130C])->mEnabled = 1;
    fn_802F0990()->SetDamage_unk(*(DamageEffectParam_unk*)&m130C[0x1310 - 0x130C]);
    if (mOpen.mMenu != 0) {
        fn_8052DEEC(mOpen.mMenu);
    } else {
        mGmk->fn_80512C90(m464);
    }
    mGmk->fn_80513020(m1354, m464);
    m1360 = fn_8052C0E8(GetSetConfig_unk());
    fn_80530DB4();
    fn_804ABF20()->fn_800A97C4();
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

extern "C" void fn_8036E624(GameTask* task); // inferred: defined in game/GameTask.cpp
extern "C" atn::PostFxManager* lbl_8087EF8C;  // inferred: post effect manager (returned by fn_804ABF20)
extern "C" int fn_800CB58C(atn::SoundRef* sound);                     // inferred: the sound is playing
extern "C" void fn_800CB5C8(atn::SoundRef* sound, int frames, int detach); // inferred: stops the sound

// inferred: motion blur of the render settings (+0x240)
extern "C" inline MotionBlurParam_unk* fn_8052D3C8(RenderSetting_unk* settings) {
    return &settings->m240;
}
extern "C" inline void fn_8052D3D0(RenderSetting_unk* settings, const MotionBlurParam_unk& blur) {
    settings->m240 = blur;
}
// inferred: closes the camp: restores the scene, the camera and the post effects saved when the camp opened
// (mB1C), closes the parts and turns off the effects of the camp
// NONMATCHING: 78% - the implicit atn::Scene::operator= is over the inlining budget in the original (R3/R5):
// the copies of Texture, GodRay, HazeParam_unk and ColorMatrixParam_unk are called (weak copies 0x8052E8D4,
// 0x8052BBEC, 0x8047C7FC, 0x80453DBC); here they are inlined (vector/CharaLightData_unk operator= are only
// declared, so they are no candidates); registers of the camera copy as fn_8052C6CC
void NewCampTask::fn_8052CA6C() {
    if (lbl_8087F430 != 0) {
        fn_8036E624((GameTask*)lbl_8087F430);
    }
    mB1C.fn_8052FA44();
    mTop->vf28();
    mStatus->vf28();
    mMapSelect->vf28();
    mItem->vf28();
    mOption->vf28();
    mProtector->vf28();
    mGmk->vf28();
    mHint->vf28();
    m1310.mEnabled = 0;
    fn_802F0990()->SetDamage_unk(m1310);
    MotionBlurParam_unk blur(*fn_8052D3C8(fn_802F0990()));
    blur.mEnabled = 0;
    fn_8052D3D0(fn_802F0990(), blur);
    atn::RenderManager::instance_->m554 = 0;
    lbl_8087EF8C->fn_800A97C4();
    if (fn_800CB58C(&m1340)) {
        fn_800CB5C8(&m1340, 10, 0);
    }
    m5C = 0;
}

// ---- fn_8052D468: camera of the camp (over the inlining budget: the accessors are weak copies) ----
extern "C" atn::Vector3 fn_80383728(const atn::Camera* camera); // inferred: direction the camera looks at
extern "C" float fn_800F8524(const float* a, const float* b, float t); // inferred: lerp of two floats
void* GetPad_unk();
// inferred: end of the lerp of the camera path (passed by address to fn_800F8524); name invented
static float sCamPathEnd_unk = 1.0f;
// inferred: offsets of the camera of the parts (m64: 4:3 screen, m70: 16:9)
extern "C" inline atn::Vector3 fn_8052DEA4(NewCampPartsBase* parts) {
    return parts->m64;
}
extern "C" inline atn::Vector3 fn_8052DEB8(NewCampPartsBase* parts) {
    return parts->m70;
}
extern "C" inline float fn_8052DECC(XsfCurveSet_unk* curves) {
    return curves->mLength;
}
extern "C" inline bool fn_8052DED4(NewCampTask* task) {
    return task->m11F8 != 1;
}

// inferred: camera of the camp: the camera of the parts (curves of the xsca, path m12B0 or free camera m121C)
// plus the offset of the parts; the second camera (m558 of the render manager) looks between both
void NewCampTask::fn_8052D468() {
    atn::Camera camera(m260);
    atn::Vector3 dir = fn_80383728(&m260);
    atn::Vector3 target = fn_80383728(&mCamera);
    target = target + dir;
    target.Normalize_unk();
    target = target + dir;
    target.Normalize_unk();
    target += *camera.GetPosition_unk();
    camera.SetTarget_unk(target);
    camera.fn_8004B378();
    fn_8052C6CC(fn_8008B964(), &camera);
    MotionBlurParam_unk blur(*fn_8052D3C8(fn_802F0990()));
    blur.mEnabled = 0;
    fn_8052D3D0(fn_802F0990(), blur);
    atn::Vector3 ofs(m136C != 0 ? m1370 : atn::Vector3(0.0f, 0.0f, 0.0f));
    GetPad_unk();
    if (m121C != 0) {
        mCamera.fn_8004BF64(3.0f, 3.0f);
        m11FC = *mCamera.GetPosition_unk();
        m1208 = *mCamera.GetTarget_unk();
    } else {
        if (m12AC == 1) {
            m12D0 = fn_800F8524(&m12D0, &sCamPathEnd_unk, 0.3f);
            m12EC = m12B0.GetPos_unk(m12D0);
            m12F8 = m12B0.GetPos_unk(m12D0);
        } else {
            m12EC = atn::Lerp_unk(m12EC, m12D4, 0.3f);
            m12F8 = atn::Lerp_unk(m12F8, m12E0, 0.3f);
        }
        if (m11F8 == 0) {
            atn::Vector3 parts((m136C != 0 && m11F0 != 0) ? fn_8052DEA4(m11F0) : lbl_807C7030);
            parts += (m136C == 0 && m11F0 != 0) ? fn_8052DEB8(m11F0) : lbl_807C7030;
            mCamera.SetPosition(m11FC + m12EC + ofs + parts);
            mCamera.SetTarget_unk(m1208 + m12F8 + ofs + parts);
        } else if (m11F8 == 1 && m454 != 0) {
            m458 += m45C;
            XsfCurveSet_unk* curves = ((Xsca_unk*)m454)->fn_805BDCBC(0);
            m11FC = fn_8052C688(curves, m458);
            m1208 = fn_8052C6A4(curves, m458);
            atn::Vector3 cur((m136C != 0 && m11F0 != 0) ? fn_8052DEA4(m11F0) : lbl_807C7030);
            atn::Vector3 prev((m136C != 0 && m11F4 != 0) ? fn_8052DEA4(m11F4) : lbl_807C7030);
            cur += (m136C == 0 && m11F0 != 0) ? fn_8052DEB8(m11F0) : lbl_807C7030;
            prev += (m136C == 0 && m11F4 != 0) ? fn_8052DEB8(m11F4) : lbl_807C7030;
            atn::Vector3 parts = atn::Lerp_unk(prev, cur, m458 / fn_8052DECC(curves));
            mCamera.SetPosition(m11FC + m12EC + ofs + parts);
            mCamera.SetTarget_unk(m1208 + m12F8 + ofs + parts);
            if (m458 > fn_8052DECC(curves) || m458 < 0.0f) {
                m11F8 = 0;
            }
            bool moving = false;
            if (m45C > 0.0f) {
                if (m458 < fn_8052DECC(curves) - 5.0f) {
                    moving = true;
                }
            } else if (m45C < 0.0f) {
                if (m458 > 5.0f) {
                    moving = true;
                }
            }
            if (moving) {
                MotionBlurParam_unk blur2(*fn_8052D3C8(fn_802F0990()));
                blur2.mEnabled = 0;
                fn_8052D3D0(fn_802F0990(), blur2);
            }
        }
    }
    if (fn_800CB58C(&m1340) != 0 && fn_8052DED4(this)) {
        fn_800CB5C8(&m1340, 20, 0);
    }
    fn_8037F688(&mCamera, m1214);
    mCamera.fn_8004B378();
    fn_8008B964()->SetCamera_unk(mCamera);
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

// inferred: saves the scene, the camera and the post effects of the game (done when the camp opens)
// NONMATCHING: 71% - the implicit atn::Scene::operator= is over the inlining budget in the original (R3/R5):
// the copies of Texture, GodRay, HazeParam_unk and ColorMatrixParam_unk are called (weak copies 0x8052E8D4,
// 0x8052BBEC, 0x8047C7FC, 0x80453DBC); here they are inlined (as fn_8052CA6C)
void NewCampView_unk::fn_8052E1A0() {
    mScene = *atn::RenderManager::instance_->fn_800C16B4();
    mCamera = atn::RenderManager::instance_->mCamera;
    mShadow = fn_802F0990()->m104;
    *(RsColor_unk*)m238 = fn_802F0990()->m03C;
    mBloom = fn_802F0990()->m054;
    mDof = fn_802F0990()->mDof;
    mColorMatrix = fn_802F0990()->m324;
    mGamma = *fn_8052B7F0(fn_802F0990());
}

// inferred: sets the scene, the camera and the post effects of the view
// NONMATCHING: 73% - the implicit atn::Scene::operator= is over the inlining budget in the original (R3/R5):
// the copies of Texture, GodRay, HazeParam_unk and ColorMatrixParam_unk are called (weak copies 0x8052E8D4,
// 0x8052BBEC, 0x8047C7FC, 0x80453DBC); here they are inlined (as fn_8052CA6C)
void NewCampView_unk::fn_8052FA44() {
    *atn::RenderManager::instance_->fn_800C16B4() = mScene;
    atn::RenderManager::instance_->SetCamera_unk(mCamera);
    fn_802F0990()->m03C = *(RsColor_unk*)m238; // inferred: clear color
    fn_802F0990()->mDof = mDof;
    fn_802F0990()->m054 = mBloom;
    fn_802F0990()->m104 = mShadow;
    fn_802F0990()->m324 = mColorMatrix;
    *fn_8052B7F0(fn_802F0990()) = mGamma;
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

extern "C" int strcmp(const char* a, const char* b);
extern "C" float fn_800DC288(const char* s); // inferred: string to float

// inferred: reads camp_config.csv: the damage effect of the camp starts from the one of the render settings
// ("angle", "color" r g b a, "scale", "zoom")
// NONMATCHING: 99.8% - offsets of the string pool (the vector error string of fn_805305D0 comes first)
void NewCampTask::fn_805303D0() {
    atn::Csv csv;
    m1310 = fn_802F0990()->m374;
    csv.SetData((const char*)m1334.fn_80470580(), m1334.fn_8047059C());
    do {
        const char* key = csv.fn_8005B9CC();
        if (strcmp(key, "angle") == 0) {
            m1310.mAngle = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(key, "color") == 0) {
            m1310.mColor.r = fn_800DC288(csv.fn_8005B9CC());
            m1310.mColor.g = fn_800DC288(csv.fn_8005B9CC());
            m1310.mColor.b = fn_800DC288(csv.fn_8005B9CC());
            m1310.mColor.a = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(key, "scale") == 0) {
            m1310.mScale = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(key, "zoom") == 0) {
            m1310.mZoom = fn_800DC288(csv.fn_8005B9CC());
        }
    } while (csv.fn_8005BBF8());
    m1334.fn_80473F88();
}

extern "C" char* strcpy(char* dst, const char* src);
extern "C" int fn_800DC12C(const char* s); // inferred: string to int

// inferred: reads camp_font_conf.csv into sElems_unk (lines starting with ';' are comments): name, id and the
// sixteen colour values
// NONMATCHING: 80% - the inlined push_back of sElems_unk: the original copies the record word by word and
// reports the allocation error with lbl_80793868 (another vector variant than std/vector.h)
void NewCampTask::fn_805305D0() {
    atn::Csv csv;
    csv.SetData((const char*)m1344.fn_80470580(), m1344.fn_8047059C());
    NewCampElem_unk elem;
    elem.m24 = 0;
    elem.mBase[0] = 0;
    elem.mBase[1] = 0;
    elem.mBase[2] = 0;
    elem.mBaseShadow[0] = 0;
    elem.mBaseShadow[1] = 0;
    elem.mBaseShadow[2] = 0;
    elem.mBaseShadow[3] = 0;
    elem.m44 = 0;
    elem.mAfter[0] = 0;
    elem.mAfter[1] = 0;
    elem.mAfter[2] = 0;
    elem.mAfterShadow[0] = 0;
    elem.mAfterShadow[1] = 0;
    elem.mAfterShadow[2] = 0;
    elem.mAfterShadow[3] = 0;
    do {
        const char* field = csv.fn_8005B9CC();
        if (*field == 0 || *field == ';') {
            continue;
        }
        strcpy(elem.mName, field);
        elem.m00 = fn_800DC12C(csv.fn_8005B9CC());
        elem.m24 = fn_800DC12C(csv.fn_8005B9CC());
        elem.mBase[0] = fn_800DC12C(csv.fn_8005B9CC());
        elem.mBase[1] = fn_800DC12C(csv.fn_8005B9CC());
        elem.mBase[2] = fn_800DC12C(csv.fn_8005B9CC());
        elem.mBaseShadow[0] = fn_800DC12C(csv.fn_8005B9CC());
        elem.mBaseShadow[1] = fn_800DC12C(csv.fn_8005B9CC());
        elem.mBaseShadow[2] = fn_800DC12C(csv.fn_8005B9CC());
        elem.mBaseShadow[3] = fn_800DC12C(csv.fn_8005B9CC());
        elem.m44 = fn_800DC12C(csv.fn_8005B9CC());
        elem.mAfter[0] = fn_800DC12C(csv.fn_8005B9CC());
        elem.mAfter[1] = fn_800DC12C(csv.fn_8005B9CC());
        elem.mAfter[2] = fn_800DC12C(csv.fn_8005B9CC());
        elem.mAfterShadow[0] = fn_800DC12C(csv.fn_8005B9CC());
        elem.mAfterShadow[1] = fn_800DC12C(csv.fn_8005B9CC());
        elem.mAfterShadow[2] = fn_800DC12C(csv.fn_8005B9CC());
        elem.mAfterShadow[3] = fn_800DC12C(csv.fn_8005B9CC());
        sElems_unk.push_back(elem);
    } while (csv.fn_8005BBF8());
    m1344.fn_80473F88();
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

// inferred: the early return written as !(a) || !(b) keeps the dead blr after the tail call of the original
void NewCampTask::fn_80530D88(int set, atn::ModelBase* model) {
    if (model == 0) {
        return;
    }
    if (!(0 <= set) || !(set < 3)) {
        return;
    }
    ((atn::Model*)model)->fn_8008C828(0);
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
