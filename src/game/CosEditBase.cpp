#include "game/CosEditBase.h"
#include "game/CostumeEditChara.h"
#include "game/CharaTask.h"
#include "game/CharaStatus.h"
#include "game/CharaManager.h"
#include "game/PlayerTask.h"
#include "game/NpcTask.h"
#include "game/NpcManager.h"
#include "game/EnemyTask.h"
#include "game/EnemyManager.h"
#include "game/Inventory.h"
#include "atn/D3Anime.h"
#include "atn/D3AnimeInstance.h"
#include "atn/SoundRef.h"
#include "atn/Memory.h"
#include "atn/Model_unk.h"
#include "std/string.h"
#include "string.h"

extern "C" int sprintf(char* s, const char* format, ...);
extern "C" char* strstr(const char* s, const char* sub);
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
extern "C" void* lbl_8087EF70;                     // inferred: pad
extern "C" int fn_800A555C(void* pad, int port, int button); // inferred: button triggered
extern "C" float fn_80076FF8(void* render);        // inferred: screen aspect ratio
extern "C" void* lbl_8087EEE0;
extern "C" void* lbl_8087F518;                     // inferred: loading screen
extern "C" void fn_8046ECDC(void* obj, int arg);
extern "C" void fn_8046EED8(void* obj);
extern "C" atn::SoundRef fn_80117228(int id);      // inferred: plays a system sound effect
extern "C" void* lbl_8087F048;
extern "C" void fn_8010C00C(void* mgr, CharaTask* chara);
extern "C" void fn_805659B4(chr::CharaEquip_unk* equip);
extern "C" void fn_800D2338(atn::Task* task); // inferred: removes the task
extern "C" int fn_8050FD20(NewCampPartsBase* self); // inferred: these parts are the current ones
extern "C" void fn_80511258(NewCampPartsBase* self, NewCampPartsEntry_unk* entry, atn::D3Anime* anime,
                            const atn::Vector3& pos, const atn::Vector3& scale, const char* name);
extern "C" void fn_805112A8(NewCampPartsBase* self, NewCampPartsEntry_unk* entry, atn::D3AnimeInstance* anime,
                            const atn::Vector3& pos, const atn::Vector3& scale, const char* name);
extern "C" void fn_80087E9C(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" int fn_80219558(int id); // inferred: party member of a character id (negative for others)
// inferred: item data returned by fn_80206C50 / fn_80206B14 (names invented)
struct CosEditItemData_unk {
    unsigned char m00[0x78];
    int m78; // inferred: passed to CharaTask::fn_8014F5D4
    int m7C;
    int m80; // inferred: equipped id
};
extern "C" CosEditItemData_unk* fn_80206C50(int item); // inferred: item data
extern "C" CosEditItemData_unk* fn_80206B14(int second, int item); // inferred: weapon data
extern "C" int fn_80206BE4(CosEditItemData_unk* item);            // inferred: item id
extern "C" void* fn_8020ED84(int slot, int item);  // inferred: armor data
extern "C" void* fn_8044D034(Inventory* inv, int index); // inferred: equipment data
extern "C" void fn_8044D184(Inventory* inv, int slot, int item, int a, int member);
extern "C" void fn_8044441C(Inventory* inv, int id, int num); // inferred: removes items
extern "C" void fn_8044D490(Inventory* inv, int index); // inferred: removes the item at `index`
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager

// inferred: 2D point of a d2anime pane (names invented, as in game/MenuCommonTask.cpp)
struct D2APoint_unk {
    D2APoint_unk() {}
    float x;
    float y;
    float mScaleX;
    float mScaleY;
    float m10;
};
extern "C" D2APoint_unk fn_801F4E8C(atn::Task* layout, unsigned int hash);
extern "C" void fn_801F6E78(atn::Task* task, const char* name, const void* pos);

// inferred: game state (lbl_8087EFA8): camera override of the costume menu at 0x240
struct CosEditCamera_unk {
    int mEnabled;
    int m04;
    int m08;
    float m0C;
    float m10;
    int m14;
    int m18;
    float m1C;
    float m20;
};
struct CosEditGame_unk {
    unsigned char m000[0x240];
    CosEditCamera_unk mCamera;
};
extern "C" CosEditGame_unk* lbl_8087EFA8;

// inferred: parent of the costume menus (lists of the party members)
struct CosEditParent_unk {
    unsigned char m0000[0x458];
    float m458;
    unsigned char m045C[0x1224 - 0x45C];
    int mCount0;                    // 0x1224
    CosEditMember_unk mMembers0[8]; // 0x1228
    int mCount1;                    // 0x1268
    CosEditMember_unk mMembers1[8]; // 0x126C
};

// inferred: lemon event (lbl_8087F098)
struct CosEditLemon_unk {
    unsigned char m00[0x48];
    int m48;
};
extern "C" CosEditLemon_unk* lbl_8087F098;

static atn::Vector3 sScale(17.0f, 17.0f, 17.0f);
static atn::Vector3 sCenter(1200.0f, -280.0f, -420.0f);
static atn::Vector3 sUpperOfs(-110.0f, -60.0f, 150.0f);
static atn::Vector3 sLowerOfs(-110.0f, 65.0f, 150.0f);
static atn::Vector3 sZoomOfs(-85.0f, 0.0f, 0.0f);
static atn::Vector3 sPrevPos = sCenter + atn::Vector3(-700.0f, 0.0f, 100.0f);
static atn::Vector3 sMainPos = sCenter;
static atn::Vector3 sNextPos = sCenter + atn::Vector3(700.0f, 0.0f, 100.0f);

// NONMATCHING: 96.1% - the original zeroes mAA0 in the body (its handle class has a destructor but no
// zeroing constructor, unlike ResHandle) and the pool lacks the leading "" (see fn_801CEA98)
CosEditBase::CosEditBase(atn::Task* parent)
    : NewCampPartsBase(parent), mChara(), mE0(0.0f), mE4(0.0f), mE8(0), mEC(0), mF0(0), mF4(0), mF8(0.0f), mFC(0), m100(0),
      m104(0), m1E0(0), m1E4(0), m1E8(0), m1EC(0), m1F0(0), m1F4(0), m220(0, 0) {
    m414 = fn_80076FF8(lbl_8087EEE0);
    m418 = 0.6981317f;
    m41C = 1;
    m424 = 0.23f;
    m430[0] = -90.0f;
    m430[1] = 210.0f;
    m430[2] = 450.0f;
    m430[3] = -90.0f;
    m430[4] = 210.0f;
    m430[5] = 400.0f;
    m430[6] = -110.0f;
    m430[7] = 190.0f;
    m430[8] = 450.0f;
    m430[9] = -110.0f;
    m430[10] = 190.0f;
    m430[11] = 350.0f;
    m46C.x = 0.0f;
    m46C.y = 0.0f;
    m46C.z = 0.0f;
    mLoadCount = 0;
    mA8C = -1;
    mA98 = 0;
    mA9C = 0.0f;
    mAA4 = 0.4f;
    mCharaUpperOfs = sUpperOfs;
    mCharaLowerOfs = sLowerOfs;
    mCharaZoomOfs = sZoomOfs;
    mLoadCount = 0;
    mA88 = 0;
    mA90 = 0;
    mA94 = 0;
    m1F8 = 0;
    m1FC = 0;
    m200 = 0;
    m204 = 0;
    for (int i = 0; i < 13; i++) {
        mLists[0].mItems[i] = atn::D3AnimeInstance::Create(this, "data/d2anime/equip_list.d2a", 1);
        mLists[0].mItems[i]->fn_800D246C(1);
        mLists[1].mItems[i] = atn::D3AnimeInstance::Create(this, "data/d2anime/equip_color_list.d2a", 1);
        mLists[1].mItems[i]->fn_800D246C(1);
        mLists[2].mItems[i] = atn::D3AnimeInstance::Create(this, "data/d2anime/equip_parts_list.d2a", 1);
        mLists[2].mItems[i]->fn_800D246C(1);
    }
    mLists[0].mBase = atn::D3Anime::Create(this, "data/d2anime/equip_list_base.d2a", 1);
    mLists[0].mBase->fn_800D246C(1);
    mLists[1].mBase = atn::D3Anime::Create(this, "data/d2anime/equip_color_list_base.d2a", 1);
    mLists[1].mBase->fn_800D246C(1);
    mLists[2].mBase = atn::D3Anime::Create(this, "data/d2anime/equip_parts_list_base.d2a", 1);
    mLists[2].mBase->fn_800D246C(1);
    for (int i = 0; i < 3; i++) {
        mLists[i].mScrollBar = atn::D3AnimeInstance::Create(this, "scroll_bar.d2a", 1);
        mLists[i].mScrollBar->fn_800D246C(1);
    }
    mLists[0].mRows = 10;
    mLists[1].mRows = 10;
}

CosEditBase::~CosEditBase() {}

// inferred: inline helper of vf10 (its temporaries are below the caller's on the stack)
inline void SetCharaRot_unk(CosEditBase* self) {
    if (self->mChara[0]) {
        self->mChara[0]->fn_801E9218(atn::Vector3(0.0f, self->mE0, 0.0f), sScale);
    }
    if (self->mChara[1]) {
        self->mChara[1]->fn_801E9218(atn::Vector3(0.0f, self->mE0, 0.0f), sScale);
    }
    if (self->mChara[2]) {
        self->mChara[2]->fn_801E9218(atn::Vector3(0.0f, self->mE0, 0.0f), sScale);
    }
}

void CosEditBase::vf10() {
    vf20();
    if (mLoadCount) {
        fn_8046ECDC(lbl_8087F518, 0);
        mA88 = 3;
        int loading = 0;
        for (unsigned int i = 0; i < mLoadCount; i++) {
            if (mLoads[i].mType == 0) {
                if (mLoads[i].mEquip->fn_8056405C()) {
                    loading = 1;
                    break;
                }
            } else if (mLoads[i].mType == 1) {
                if (mLoads[i].mChara->fn_8014FAD0()) {
                    loading = 1;
                    break;
                }
            }
        }
        if (!loading) {
            for (unsigned int i = 0; i < mLoadCount; i++) {
                if (mLoads[i].mType == 0) {
                    mLoads[i].mEquip->fn_80566010();
                    mLoads[i].mChara->fn_80150178(mLoads[i].mEquip->m000, mLoads[i].mEquip);
                    fn_805659B4(mLoads[i].mEquip);
                    if (lbl_8087F048) {
                        mLoads[i].mChara->mStatus.fn_8013407C();
                        fn_8010C00C(lbl_8087F048, mLoads[i].mChara);
                    }
                    mLoads[i].mChara->mStatus.fn_8012B988();
                    CharaTask* chara = mLoads[i].mChara;
                    if (chara->m674 < 0 && (!lbl_8087F098 || !lbl_8087F098->m48)) {
                        chara->mStatus.fn_8012D8B8();
                    }
                }
            }
            mLoadCount = 0;
            if (mA98 == 1) {
                mA98 = 2;
                mA9C = 15.0f;
            }
            fn_8046EED8(lbl_8087F518);
            if (mA8C >= 0) {
                fn_80117228(mA8C);
                mA8C = -1;
            }
        }
    } else {
        if (--mA88 < 0) {
            mA88 = 0;
        }
    }
    bool idle = mA98 == 1 ? false : mA98 != 2;
    if (!idle) {
        if (mA98 == 2) {
            if ((mA9C -= 1.0f) < 0.0f) {
                mA98 = 0;
            }
        }
    } else {
        if (mF0) {
            if ((!mChara[0] || mChara[0]->IsFlag_unk(2)) && (!mChara[1] || mChara[1]->IsFlag_unk(2)) &&
                (!mChara[2] || mChara[2]->IsFlag_unk(2))) {
                SetCharaRot_unk(this);
                mF0 = 0;
            }
        }
        fn_801CD648();
    }
}

void CosEditBase::vf20() {
    if (m96) {
        int ready = 1;
        for (int i = 0; i < m4C; i++) {
            if (m50[i].mAnime && ((atn::D3Anime*)m50[i].mAnime)->mState == 1) {
                ready = 0;
                break;
            }
        }
        if (ready) {
            vf48();
            m96 = 0;
        }
    }
}

void CosEditBase::vf68() {}

// NONMATCHING: 94.0% - the camera override block of lbl_8087EFA8 is copied with other registers and order
void CosEditBase::fn_801CD648() {
    if (mF8 > 0.0f) {
        float v = mF8 - 0.1f;
        mF8 = v > 0.0f ? v : 0.0f;
    } else if (mF8 < 0.0f) {
        float v = mF8 + 0.1f;
        mF8 = v < 0.0f ? v : 0.0f;
    }
    CosEditCamera_unk camera = lbl_8087EFA8->mCamera;
    camera.mEnabled = 0;
    if (mF4) {
        camera.mEnabled = 1;
        mE0 = 0.0f;
        mE4 = 0.0f;
        int ready = 1;
        if (mChara[0] && !mChara[0]->fn_801E91B4()) {
            ready = 0;
        }
        if (mChara[1] && !mChara[1]->fn_801E91B4()) {
            ready = 0;
        }
        if (mChara[2] && !mChara[2]->fn_801E91B4()) {
            ready = 0;
        }
        if (ready) {
            fn_801CE148(m104);
            camera.mEnabled = 0;
            mF4 = 0;
        }
        if (mChara[0]) {
            mChara[0]->mVisible = 1;
        }
        if (mChara[1]) {
            mChara[1]->mVisible = 1;
        }
        if (mChara[2]) {
            mChara[2]->mVisible = 1;
        }
    } else {
        if (mChara[0]) {
            if (fn_8050FD20(this) && ((CosEditParent_unk*)m48)->m458 >= 25.0f) {
                mChara[0]->mVisible = 1;
            } else {
                mChara[0]->mVisible = 0;
            }
        }
        if (mChara[1]) {
            mChara[1]->mVisible = 0;
        }
        if (mChara[2]) {
            mChara[2]->mVisible = 0;
        }
    }
    lbl_8087EFA8->mCamera = camera;
    if (mChara[0]) {
        if (fn_800A555C(lbl_8087EF70, 0, 9)) {
            mE0 -= 1.5707964f;
            if (mE0 <= -6.2831855f) {
                mE0 += 6.2831855f;
                mE4 += 6.2831855f;
            }
        }
        mE4 = 0.2f * (mE0 - mE4) + mE4;
        atn::Vector3 rot(0.0f, mE4, 0.0f);
        mChara[0]->fn_801E91A0(rot);
        mChara[0]->mOffsetTarget = atn::Vector3(m478);
        if (mChara[1]) {
            mChara[1]->fn_801E91A0(rot);
            mChara[1]->mOffsetTarget = atn::Vector3(m478);
        }
        if (mChara[2]) {
            mChara[2]->fn_801E91A0(rot);
            mChara[2]->mOffsetTarget = atn::Vector3(m478);
        }
    }
}

// NONMATCHING: 95.0% - frame 0x40 bytes larger and saved register order (d2anime point temporaries)
void CosEditBase::fn_801CDA00(CosEditList_unk* list) {
    if (list->mBase) {
        list->mBase->mVisible = 0;
    }
    if (list->mScrollBar) {
        list->mScrollBar->mVisible = 0;
    }
    for (int i = 0; i < 13; i++) {
        if (list->mItems[i]) {
            list->mItems[i]->mVisible = 0;
        }
    }
    if (list && list->m3C >= 0 && list->m40 >= 0) {
        atn::D3Anime* base = list->mBase;
        NewCampPartsEntry_unk* entry = &m50[list->m3C];
        if (base && base->GetTask()) {
            D2APoint_unk pos;
            pos.x = 0.0f;
            pos.y = 0.0f;
            pos.mScaleX = 0.0f;
            pos.mScaleY = 0.0f;
            pos.m10 = 0.0f;
            atn::D3AnimeInstance* scroll = list->mScrollBar;
            base->mVisible = 1;
            fn_80511258(this, entry, base, atn::Vector3(0.0f, 0.0f, 0.0f), atn::Vector3(1.0f, 1.0f, 1.0f),
                        "reference");
            if (scroll) {
                scroll->mVisible = 1;
                fn_805112A8(this, entry, scroll, atn::Vector3(0.0f, 0.0f, 0.0f), atn::Vector3(1.0f, 1.0f, 1.0f),
                            "reference");
                if (scroll->GetTask()) {
                    char name[0x40];
                    sprintf(name, "scroll_point");
                    pos = fn_801F4E8C(base->GetTask(), fn_800DC6B4(name));
                    fn_801F6E78(scroll->GetTask(), "basePos", &pos);
                }
            }
            for (int i = 0; i < 13; i++) {
                if (i >= list->mRows) {
                    break;
                }
                atn::D3AnimeInstance* item = list->mItems[i];
                item->mVisible = 1;
                fn_805112A8(this, entry, item, atn::Vector3(0.0f, 0.0f, 0.0f), atn::Vector3(1.0f, 1.0f, 1.0f),
                            "reference");
                if (item->GetTask()) {
                    char name[0x40];
                    sprintf(name, "camp_list%d", i + 1);
                    pos = fn_801F4E8C(base->GetTask(), fn_800DC6B4(name));
                    fn_801F6E78(item->GetTask(), "base_point", &pos);
                }
            }
        }
    }
}

void CosEditBase::vf14() {
    vf64();
    for (int i = 0; i < 3; i++) {
        fn_801CDA00(&mLists[i]);
    }
}

void CosEditBase::vf64() {}

__declspec(noinline) CosEditMember_unk* CosEditBase::fn_801CDD9C(int step) {
    if (!mChara[0]) {
        return 0;
    }
    int count;
    if (m1E0 == 0) {
        count = ((CosEditParent_unk*)m48)->mCount0;
    } else {
        count = ((CosEditParent_unk*)m48)->mCount1;
    }
    int index = m100 + step;
    if (index < 0) {
        index = count - 1;
    }
    if (index >= count) {
        index = 0;
    }
    if (m1E0 == 0) {
        return &((CosEditParent_unk*)m48)->mMembers0[index];
    }
    return &((CosEditParent_unk*)m48)->mMembers1[index];
}

// NONMATCHING: 98.8% - the original loads mFace before member->mId but compares member->mId first
void CosEditBase::fn_801CDE14(CosEditMember_unk* member, int dir) {
    if (!mChara[0]) {
        return;
    }
    m104 = member;
    if (dir > 0) {
        if (mChara[1] && mChara[1]->mFace == member->mId) {
            mChara[0]->fn_801E92D4(sMainPos, sPrevPos);
            mChara[1]->fn_801E92D4(sNextPos, sMainPos);
        } else if (mChara[2] && member->mId == mChara[2]->mFace) {
            CostumeEditChara* next = mChara[1];
            mChara[1] = mChara[2];
            mChara[2] = next;
            mChara[0]->fn_801E92D4(sMainPos, sPrevPos);
            mChara[1]->fn_801E92D4(sNextPos, sMainPos);
        }
        mF4 = 1;
        mF8 = 1.0f;
        vf6C();
    } else {
        if (mChara[1] && member->mId == mChara[1]->mFace) {
            CostumeEditChara* next = mChara[1];
            mChara[1] = mChara[2];
            mChara[2] = next;
            mChara[0]->fn_801E92D4(sMainPos, sNextPos);
            mChara[2]->fn_801E92D4(sPrevPos, sMainPos);
        } else if (mChara[2] && member->mId == mChara[2]->mFace) {
            mChara[0]->fn_801E92D4(sMainPos, sNextPos);
            mChara[2]->fn_801E92D4(sPrevPos, sMainPos);
        }
        mF4 = -1;
        mF8 = -1.0f;
        vf6C();
    }
    int count;
    if (m1E0 == 0) {
        count = ((CosEditParent_unk*)m48)->mCount0;
    } else {
        count = ((CosEditParent_unk*)m48)->mCount1;
    }
    if ((m100 += mF4) < 0) {
        m100 = count - 1;
    }
    if (m100 >= count) {
        m100 = 0;
    }
}

void CosEditBase::vf6C() {}

// NONMATCHING: 97.4% - the static block base gets r31 here and r26 in the original (register coloring)
void CosEditBase::fn_801CE148(CosEditMember_unk* member) {
    if (mLoadCount) {
        for (unsigned int i = 0; i < mLoadCount; i++) {
            if (mLoads[i].mEquip) {
                delete mLoads[i].mEquip;
            }
        }
        mLoadCount = 0;
        mA88 = 0;
    }
    if (!mChara[0]) {
        mChara[0] = CostumeEditChara::fn_801E7FF8(this, member->mChara, member->mId);
        mChara[0]->fn_800D246C(1);
        mChara[0]->fn_801E917C(sMainPos);
        mChara[0]->mVisible = 0;
        CosEditMember_unk* next = fn_801CDD9C(1);
        CosEditMember_unk* prev = fn_801CDD9C(-1);
        if (prev != member) {
            mChara[2] = CostumeEditChara::fn_801E7FF8(this, prev->mChara, prev->mId);
            mChara[2]->fn_800D246C(1);
            mChara[2]->fn_801E917C(sPrevPos);
            mChara[2]->mVisible = 0;
        }
        if (next != member && prev != next) {
            mChara[1] = CostumeEditChara::fn_801E7FF8(this, next->mChara, next->mId);
            mChara[1]->fn_800D246C(1);
            mChara[1]->fn_801E917C(sNextPos);
            mChara[1]->mVisible = 0;
        }
    } else if (mChara[1] && member->mId == mChara[1]->mFace) {
        if (mChara[2]) {
            mChara[2]->fn_801E9164();
            fn_800D2338(mChara[2]);
            mChara[2] = 0;
        }
        CostumeEditChara* main = mChara[0];
        mChara[0] = mChara[1];
        mChara[2] = main;
        CosEditMember_unk* next = fn_801CDD9C(1);
        if (!next) {
            next = ((CosEditParent_unk*)m48)->mMembers1;
        }
        if (next->mId == mChara[2]->mFace) {
            mChara[1] = 0;
        } else {
            mChara[1] = CostumeEditChara::fn_801E7FF8(this, next->mChara, next->mId);
            mChara[1]->fn_800D246C(1);
            mChara[1]->SetCur_unk(sNextPos);
            mChara[1]->fn_801E9168(sNextPos);
        }
        vf6C();
    } else if (mChara[2] && member->mId == mChara[2]->mFace) {
        if (mChara[1]) {
            mChara[1]->fn_801E9164();
            fn_800D2338(mChara[1]);
            mChara[1] = 0;
        }
        CostumeEditChara* main = mChara[0];
        mChara[0] = mChara[2];
        mChara[1] = main;
        CosEditMember_unk* prev = fn_801CDD9C(-1);
        if (prev->mId == mChara[1]->mFace) {
            mChara[2] = 0;
        } else {
            mChara[2] = CostumeEditChara::fn_801E7FF8(this, prev->mChara, prev->mId);
            mChara[2]->fn_800D246C(1);
            mChara[2]->SetCur_unk(sPrevPos);
            mChara[2]->fn_801E9168(sPrevPos);
        }
        vf6C();
    }
    mF0 = 1;
}

int CosEditBase::fn_801CE4CC() {
    if (mChara[0] && mChara[0]->mChara && mChara[0]->mChara->fn_8014FAD0()) {
        return 1;
    }
    return 0;
}

void CosEditBase::fn_801CE518() {
    mF4 = 0;
    if (mChara[0]) {
        mChara[0]->fn_801E9164();
        fn_800D2338(mChara[0]);
        mChara[0] = 0;
    }
    if (mChara[1]) {
        mChara[1]->fn_801E9164();
        fn_800D2338(mChara[1]);
        mChara[1] = 0;
    }
    if (mChara[2]) {
        mChara[2]->fn_801E9164();
        fn_800D2338(mChara[2]);
        mChara[2] = 0;
    }
}

void CosEditBase::fn_801CE5B0(int b, int c) {
    if (b != m1E8) {
        vf74(m1E4, b);
        if (mE8) {
            ((CostumeEditFace_unk*)mE8)->m50 = 0.0f;
        }
        if (mEC) {
            ((CostumeEditFace_unk*)mEC)->m50 = 0.0f;
        }
        m1F0 = c;
    }
}

void CosEditBase::vf74(int a, int b) {
    m1EC = m1E8;
    m1E4 = a;
    m1E8 = b;
}

void* CosEditBase::fn_801CE648(int slot) {
    if (!fn_801CF334() || !fn_801CF334()->mChara) {
        return 0;
    }
    return fn_801CF334()->mChara->m680[slot];
}

int CosEditBase::fn_801CE7E8(int slot) {
    return fn_801CF334()->mChara->m680[slot]->m430;
}

void* CosEditBase::fn_801CE87C(int slot) {
    if (!fn_801CF334() || !fn_801CF334()->mChara) {
        return 0;
    }
    int member = fn_80219558(fn_801CF334()->mChara->m50);
    return fn_8044D034(Inventory::instance_, slot + member * 4);
}

void CosEditBase::fn_801CEA50(int member, CharaTask* chara, int slot, int a, int b, int c) {
    if (chara) {
        fn_801CEA98(member, chara, slot, chara->m680[slot]->m004, chara->m680[slot]->m008, a, b, c);
    }
}

// inferred: shows item `item` in `slot` on `chara` and on every character using the same model
// NONMATCHING: 97.5% - register coloring of index/c/d, and the "" of the allocations is the first
// string of the original pool (@stringBase0 + 0) while ours lands after "base_point"
void CosEditBase::fn_801CEA98(int member, CharaTask* chara, int slot, int item, int a, int b, int c, int d) {
    int index = slot + member * 4;
    if (b) {
        if (m210 < 0 && m214 >= 0) {
            Inventory::Equip_unk& shown = Inventory::instance_->mEquipShown[member];
            if (m218) {
                shown = Inventory::instance_->mEquipSetB[m214];
            } else {
                shown = Inventory::instance_->mEquipSetA[m214];
            }
        } else {
            // inferred: the block is read as ints here
            ((int*)&Inventory::instance_->mEquipShown[member])[m210] = m21C;
        }
    }
    Inventory::instance_->GetPlayerStatusInfo(member)->fn_80134774(slot, c);
    if (b && c) {
        return;
    }
    if (chara) {
        chara->mStatus.fn_80134774(slot, c);
        void* data = fn_8044D034(Inventory::instance_, index);
        chr::CharaEquip_unk* equip =
            new ((MemoryTag)6, "", "", 0) chr::CharaEquip_unk(slot, item, index, chara, (int)data, 0, c, 1, 0);
        CosEditLoad_unk load;
        load.mType = 0;
        load.mEquip = equip;
        load.mChara = chara;
        AddLoad_unk(load);
        mA90 = 0;
    }
    mA8C = d;
    std::string prefix;
    switch (member) {
    case 0:
        prefix = "pc101_\0";
        break;
    case 1:
        prefix = "pc107_\0";
        break;
    case 2:
        prefix = "pc102_\0";
        break;
    case 3:
        prefix = "pc103_\0";
        break;
    case 4:
        prefix = "pc104_\0";
        break;
    case 5:
        prefix = "pc106_\0";
        break;
    case 6:
        prefix = "pc105_\0";
        break;
    }
    for (PlayerTask* p = lbl_8087F8A0->mHead; p; p = p->mNext) {
        if (member == 1 && fn_80219558(p->m50) < 0) {
            continue;
        }
        if (p->IsFlag_unk(2) && p != chara && strstr(p->GetModel_unk()->fn_8008B130(), prefix.c_str())) {
            void* data = fn_8044D034(Inventory::instance_, index);
            chr::CharaEquip_unk* equip =
                new ((MemoryTag)6, "", "", 0) chr::CharaEquip_unk(slot, item, index, p, (int)data, 0, c, 1, 0);
            CosEditLoad_unk load;
            load.mType = 0;
            load.mEquip = equip;
            load.mChara = p;
            AddLoad_unk(load);
        }
    }
    for (NpcTask* p = NpcManager::sInstance->mHead; p; p = p->mNext) {
        if (member == 1 && fn_80219558(p->m50) < 0) {
            continue;
        }
        if (p->IsFlag_unk(2) && p != chara && strstr(p->GetModel_unk()->fn_8008B130(), prefix.c_str())) {
            void* data = fn_8044D034(Inventory::instance_, index);
            chr::CharaEquip_unk* equip =
                new ((MemoryTag)6, "", "", 0) chr::CharaEquip_unk(slot, item, index, p, (int)data, 0, c, 1, 0);
            CosEditLoad_unk load;
            load.mType = 0;
            load.mEquip = equip;
            load.mChara = p;
            AddLoad_unk(load);
        }
    }
    for (EnemyTask* p = EnemyManager::sInstance->mHead; p; p = p->mNext) {
        if (p->IsFlag_unk(2) && p != chara && strstr(p->GetModel_unk()->fn_8008B130(), prefix.c_str())) {
            void* data = fn_8044D034(Inventory::instance_, index);
            chr::CharaEquip_unk* equip =
                new ((MemoryTag)6, "", "", 0) chr::CharaEquip_unk(slot, item, index, p, (int)data, 0, c, 1, 0);
            CosEditLoad_unk load;
            load.mType = 0;
            load.mEquip = equip;
            load.mChara = p;
            AddLoad_unk(load);
        }
    }
}

CostumeEditChara* CosEditBase::fn_801CF334() {
    if (mF4) {
        if (mChara[0] && m104->mId == mChara[0]->mFace) {
            return mChara[0];
        }
        if (mChara[1] && m104->mId == mChara[1]->mFace) {
            return mChara[1];
        }
        if (mChara[2] && m104->mId == mChara[2]->mFace) {
            return mChara[2];
        }
    }
    return mChara[0];
}

// inferred: equips weapon `item` in `slot` of `member` (and on every character using the same model)
int CosEditBase::fn_801CF3C0(int member, CharaTask* chara, CosEditItem_unk* item, int slot, int add, int remove) {
    CosEditItemData_unk* data = fn_80206C50(item->mId);
    if (!data) {
        return 0;
    }
    Inventory::Equip_unk* equip = &Inventory::instance_->mEquip[member];
    CosEditItemData_unk* cur;
    if (member == 0 && slot == 1) {
        cur = fn_80206B14(1, equip->mWeapons[slot].mId);
    } else {
        cur = fn_80206B14(0, equip->mWeapons[slot].mId);
    }
    if (cur && cur == data) {
        return 0;
    }
    if (add && cur) {
        int id = fn_80206BE4(cur);
        if (id > 0) {
            Inventory::instance_->AddItem(id, 1, 1, 0, 0, 0, 0);
        }
    }
    fn_8044D184(Inventory::instance_, slot, data->m80, -1, member);
    if (chara) {
        if (chara->mType == 0 || chara->mType == 3) {
            int part = chara->m674;
            chara->fn_8014EEC4(1);
            chara->fn_8014F5D4(slot, data->m78, data->m80);
            if (part >= 0) {
                chara->fn_8014DEE4(part, 0, 1);
            } else if (slot == 1) {
                chara->fn_8014EE48();
            }
            CosEditLoad_unk load;
            load.mType = 1;
            load.mEquip = 0;
            load.mChara = chara;
            AddLoad_unk(load);
        } else {
            chara->mStatus.fn_8012B988();
        }
    }
    if (remove) {
        if (item->mIndex < 0) {
            fn_8044441C(Inventory::instance_, item->mId, 1);
        } else {
            fn_8044D490(Inventory::instance_, item->mIndex);
        }
    }
    mA8C = 0xE;
    std::string prefix;
    switch (member) {
    case 0:
        prefix = "pc101_\0";
        break;
    case 1:
        prefix = "pc107_\0";
        break;
    case 2:
        prefix = "pc102_\0";
        break;
    case 3:
        prefix = "pc103_\0";
        break;
    case 4:
        prefix = "pc104_\0";
        break;
    case 5:
        prefix = "pc106_\0";
        break;
    case 6:
        prefix = "pc105_\0";
        break;
    }
    for (PlayerTask* p = lbl_8087F8A0->mHead; p; p = p->mNext) {
        if (p->IsFlag_unk(2) && p != chara && strstr(p->GetModel_unk()->fn_8008B130(), prefix.c_str())) {
            int part = p->m674;
            p->fn_8014EEC4(1);
            p->fn_8014F5D4(slot, data->m78, data->m80);
            if (part >= 0) {
                p->fn_8014DEE4(part, 0, 1);
            }
            CosEditLoad_unk load;
            load.mType = 1;
            load.mEquip = 0;
            load.mChara = p;
            AddLoad_unk(load);
        }
    }
    for (EnemyTask* p = EnemyManager::sInstance->mHead; p; p = p->mNext) {
        if (p->IsFlag_unk(2) && p != chara && strstr(p->GetModel_unk()->fn_8008B130(), prefix.c_str())) {
            int part = p->m674;
            p->fn_8014EEC4(1);
            p->fn_8014F5D4(slot, data->m78, data->m80);
            if (part >= 0) {
                p->fn_8014DEE4(part, 0, 1);
            }
            CosEditLoad_unk load;
            load.mType = 1;
            load.mEquip = 0;
            load.mChara = p;
            AddLoad_unk(load);
        }
    }
    return 1;
}

void* CosEditBase::fn_801CFA5C(int member, int slot) {
    Inventory::Equip_unk* equip = &Inventory::instance_->mEquip[member];
    if (member == 0 && slot == 1) {
        return fn_80206B14(1, equip->mWeapons[slot].mId);
    }
    return fn_80206B14(0, equip->mWeapons[slot].mId);
}

void* CosEditBase::fn_801CFAA8(int member, int slot) {
    return fn_8020ED84(slot, Inventory::instance_->mEquip[member].mProtectors[slot].mId);
}

void CosEditBase::vf40(void* group) {
    group = fn_8008937C(group, m98.c_str());
    fn_80087E9C(group, "OffsetPos", &mA4, 0, 0, -1000000.0f, 1000000.0f, 1.0f);
    fn_80087E9C(group, "OffsetScl", &mB0, 0, 0, -1000000.0f, 1000000.0f, 1.0f);
    fn_80087E9C(group, "LocatorScl", &mBC, 0, 0, -1000000.0f, 1000000.0f, 1.0f);
    fn_80087E9C(group, "BasePos", &mC8, 0, 0, -1000000.0f, 1000000.0f, 1.0f);
    fn_80087E9C(group, "CharaUpperOfs", &mCharaUpperOfs, 0, 0, -1000000.0f, 1000000.0f, 1.0f);
    fn_80087E9C(group, "CharaLowerOfs", &mCharaLowerOfs, 0, 0, -1000000.0f, 1000000.0f, 1.0f);
    fn_80087E9C(group, "CharaZoomOfs", &mCharaZoomOfs, 0, 0, -1000000.0f, 1000000.0f, 1.0f);
}
