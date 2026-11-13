#include "macros.h"
#include "gmk/ItemSlotAnime.h"
#include "atn/Task.h"
#include "game/GameTask.h"
#include "game/MapTask.h"
#include "atn/FixedArray.h"
#include "game/NewCampPartsOption.h"
#include "game/GimmickManager.h"
#include "atn/Memory.h"
#include "atn/SoundManager.h"
#include "game/CharaManager.h"
#include "game/PlayerTask.h"
#include "game/Inventory.h"
#include "game/GameUI.h"
#include "atn/RenderManager.h"
#include "MP/MpSession_unk.h"
#include <string.h>

extern "C" bool fn_80213E60(gmk::ItemSlotItem_unk* item, int kind, int sub, int num); // inferred: db/ItemRare_unk.cpp
extern "C" GameTask* lbl_8087F430; // inferred: the field task

// inferred: task of an item animation entry (layout task or effect; bytes at 0x4C/0x4D set by
// ItemSlotAnimeBuffer::vf0C); names invented
class ItemSlotTask_unk : public atn::Task {
public:
    unsigned char m48[0x4C - 0x48];
    unsigned char m4C;
    unsigned char m4D;
    unsigned char m4E[0x50 - 0x4E];
    float m50; // inferred: frame of the layout animation
    float m54; // inferred: speed of the layout animation
};

// inferred: entry of RecycleBuffer (0x84 bytes; empty destructor 0x8044E158, cleared by 0x8044E198); names
// invented
struct RecycleBufferEntry_unk {
    RecycleBufferEntry_unk() { Clear_unk(); } // 0x8044E198
    ~RecycleBufferEntry_unk() {}
    void Clear_unk() {
        m00 = 0;
        m04.mSize = 0;
        m14.mSize = 0;
        m24.mSize = 0;
        m30.mSize = 0;
        m3C = 0;
        mTasks.mSize = 0;
    }
    // inferred: every task of the entry finished its vf0C
    bool IsReady_unk() {
        for (ItemSlotTask_unk** it = mTasks.begin(); it != mTasks.end(); it++) {
            if (*it != 0 && !(*it)->IsFlag_unk(2)) {
                return false;
            }
        }
        return true;
    }
    ItemSlotTask_unk* m00;
    atn::FixedArray_unk<ItemSlotTask_unk*, 3> m04;
    atn::FixedArray_unk<ItemSlotTask_unk*, 3> m14;
    atn::FixedArray_unk<ItemSlotTask_unk*, 2> m24;
    atn::FixedArray_unk<ItemSlotTask_unk*, 2> m30;
    ItemSlotTask_unk* m3C;
    atn::FixedArray_unk<ItemSlotTask_unk*, 16> mTasks; // 0x40
};

// inferred: task keeping a pool of entries (RecycleBuffer<gmk::ItemSlotAnime>, vtable 0x8078F3C8); members
// invented
template <class T>
class RecycleBuffer : public atn::Task {
public:
    RecycleBuffer(atn::Task* parent) : atn::Task(parent), mNum(0), m4C(0), mEntries(0) {}

    // inferred: reallocates the pool for n entries (inlined in ItemSlotAnime::fn_8044D9BC); name invented
    void Resize_unk(unsigned int n) {
        if (mNum != n) {
            mNum = 0;
            if (m4C != 0) {
                delete[] m4C;
                m4C = 0;
            }
            if (mEntries != 0) {
                delete[] mEntries;
                mEntries = 0;
            }
            mNum = n;
            unsigned int bytes = mNum >> 3;
            m4C = new ((MemoryTag)1, "", "", 0) unsigned char[bytes > 1 ? bytes : 1];
            bytes = mNum >> 3;
            memset(m4C, 0, bytes > 1 ? bytes : 1);
            mEntries = new ((MemoryTag)1, "", "", 0) RecycleBufferEntry_unk[mNum];
        }
    }
    virtual ~RecycleBuffer() {
        mNum = 0;
        if (m4C != 0) {
            delete[] m4C;
            m4C = 0;
        }
        if (mEntries != 0) {
            delete[] mEntries;
            mEntries = 0;
        }
    }

    // inferred: clears every entry (inlined in the destructor of ItemSlotAnimeBuffer); name invented
    void ClearAll_unk() {
        for (unsigned int i = 0; i < mNum; i++) {
            mEntries[i].Clear_unk();
        }
    }

    inline void Free_unk(RecycleBufferEntry_unk* entry);

    // inferred: takes the first free entry (the last one when all are in use); name invented
    RecycleBufferEntry_unk* Alloc_unk() {
        if (mNum == 0) {
            return 0;
        }
        for (unsigned int i = 0; i < mNum; i++) {
            int bit = 1 << (i & 7);
            if ((m4C[i >> 3] & bit) != bit) {
                m4C[i >> 3] |= bit;
                return &mEntries[i];
            }
        }
        return &mEntries[mNum - 1];
    }

    unsigned int mNum;              // 0x48
    unsigned char* m4C;
    RecycleBufferEntry_unk* mEntries; // 0x50
};

namespace gmk {

// inferred: the pool of the item animations (vtable 0x8078F388, instance lbl_8087F4F8)
class ItemSlotAnimeBuffer : public RecycleBuffer<ItemSlotAnime> {
public:
    ItemSlotAnimeBuffer(atn::Task* parent) : RecycleBuffer<ItemSlotAnime>(parent) {}
    virtual ~ItemSlotAnimeBuffer();
    virtual int vf0C(); // inferred: 0x804523D0
    inline bool Start_unk(); // inferred: true while the tasks are loading; then shows them (name invented)
};

} // namespace gmk


extern "C" gmk::ItemSlotAnimeBuffer* lbl_8087F4F8; // inferred: instance of the buffer

// inferred: clears the bit of the entry in the use mask (name invented)
template <class T>
inline void RecycleBuffer<T>::Free_unk(RecycleBufferEntry_unk* entry) {
    for (unsigned int i = 0; i < mNum; i++) {
        if (&mEntries[i] == entry) {
            m4C[i >> 3] &= ~(1 << (i & 7));
            return;
        }
    }
}

namespace gmk {

inline void ItemSlotAnime::Release_unk() {
    if (m00 != 0) {
        if (lbl_8087F4F8 != 0) {
            atn::FixedArray_unk<ItemSlotTask_unk*, 16>& tasks = m00->mTasks;
            for (ItemSlotTask_unk** it = tasks.begin(); it != tasks.end(); it++) {
                if (*it != 0) {
                    (*it)->FreezeTask();
                }
            }
            lbl_8087F4F8->Free_unk(m00);
        }
        m00 = 0;
    }
}

ItemSlotAnime::ItemSlotAnime() : m00(0), mState(0), mPos(0.0f, 0.0f, 0.0f), mScale(1.0f), m18(0), m1C(0), m20(0), m24(0) {
    mFlag31 = 1;
    mFlag30 = 1;
    mFlag29 = 1;
    mFlag28 = 1;
    mFlag27 = 1;
    mFlag26 = 0;
    mFlag25 = 0;
    mFlag24 = 0;
    mF0 = 0;
    mState = 0;
    Release_unk();
}

ItemSlotAnime::~ItemSlotAnime() {
    mState = 0;
    Release_unk();
}

extern "C" atn::Task* fn_801F64D0(atn::Task* parent, const char* path); // inferred: d2anime task

// DRAFT: the creation of the ItemSlotTextCache (lbl_8087F4FC)
// NONMATCHING: draft - the creation of the ItemSlotTextCache (lbl_8087F4FC) is missing; written for the
// weak copy of the entry constructor (0x8044E198)
void ItemSlotAnime::fn_8044D9BC(void* owner) {
    if (lbl_8087F4F8 == 0) {
        GimmickManager* gm = GimmickManager::instance_;
        if (lbl_8087F4F8 == 0) {
            lbl_8087F4F8 = new ((MemoryTag)1, "", "", 0) ItemSlotAnimeBuffer(gm);
        }
        ItemSlotAnimeBuffer* buf = lbl_8087F4F8;
        buf->Resize_unk(12);
        buf = lbl_8087F4F8;
        for (unsigned int i = 0; i < buf->mNum; i++) {
            RecycleBufferEntry_unk& e = buf->mEntries[i];
            e.m00 = (ItemSlotTask_unk*)fn_801F64D0(buf, "/data/d2anime/ui_slot_base.d2b");
            for (int j = 0; j < 3; j++) {
                e.m04.push_back((ItemSlotTask_unk*)fn_801F64D0(buf, "/data/d2anime/ui_slot_h.d2b"));
                e.m14.push_back((ItemSlotTask_unk*)fn_801F64D0(buf, "/data/d2anime/ui_slot_l.d2b"));
            }
            e.m24.push_back((ItemSlotTask_unk*)fn_801F64D0(buf, "/data/d2anime/ui_slot_ok.d2b"));
            e.m24.push_back((ItemSlotTask_unk*)fn_801F64D0(buf, "/data/d2anime/ui_slot_ok_rare.d2b"));
            e.m30.push_back((ItemSlotTask_unk*)fn_801F64D0(buf, "/data/d2anime/ui_item_pop.d2b"));
            e.m30.push_back((ItemSlotTask_unk*)fn_801F64D0(buf, "/data/d2anime/ui_item_pop_rare.d2b"));
            e.m3C = (ItemSlotTask_unk*)fn_801F64D0(buf, "/data/d2anime/ui_slot_rare.d2b");
            for (unsigned int j = 0; j < e.m04.size(); j++) {
                e.mTasks.push_back(e.m04[j]);
                e.mTasks.push_back(e.m14[j]);
            }
            e.mTasks.push_back(e.m00);
            for (unsigned int j = 0; j < e.m24.size(); j++) {
                e.mTasks.push_back(e.m24[j]);
            }
            for (unsigned int j = 0; j < e.m30.size(); j++) {
                e.mTasks.push_back(e.m30[j]);
            }
            e.mTasks.push_back(e.m3C);
            for (ItemSlotTask_unk** it = e.mTasks.begin(); it != e.mTasks.end(); it++) {
                if (*it != 0) {
                    (*it)->fn_800D246C(1);
                }
            }
        }
    }
}

int ItemSlotAnime::fn_8044E1BC() {
    if (mState > 0) {
        return 0;
    }
    if (lbl_8087F4F8->Start_unk()) {
        return 1;
    }
    mState = 1;
    return 0;
}

// NONMATCHING: 99.17% - registers of the inlined Alloc_unk loop (r5/r7 vs r6/r9)
void ItemSlotAnime::fn_8044E418(const atn::Vector3* pos, int arg2, int arg3, void* item, int count, int arg6,
                                int arg7) {
    mPos = *pos;
    fn_8044F434();
    mState = 2;
    m18 = arg2;
    m1C = arg3;
    m20 = (ItemSlotItem_unk*)item;
    m24 = count;
    mFlag30 = arg6;
    m28 = arg7;
    if (!mFlag31) {
        mState = 1;
        if (m00 == 0) {
            m00 = lbl_8087F4F8->Alloc_unk();
            if (m00 != 0 && fn_80450A78()) {
                atn::FixedArray_unk<ItemSlotTask_unk*, 2>& tasks = m00->m24;
                for (ItemSlotTask_unk** it = tasks.begin(); it != tasks.end(); it++) {
                    if (*it != 0) {
                        (*it)->m50 = 0.0f;
                    }
                }
            }
        }
    }
}

int ItemSlotAnime::fn_8044E610() {
    return mState == 2 && m20 != 0 && m24 > 0;
}

extern "C" float fn_801F6C2C(atn::Task* layout);                                    // inferred: last frame
extern "C" void fn_801F791C(atn::Task* task, const char* name, int index, float value); // inferred: pane alpha

// inferred: panes of the slot layouts (names invented but sColorPane)
static const char* const sSlotTextPanes[7] = {"string0009(1)", "string0008(1)", "string0007(1)", "string0006(1)",
                                              "string0005(1)", "string0003(1)", "arrowName02"};
static const char* const sArrowPane = "arrowName02";
static const char* const sColorPane = "col0001_01";
static const char* const sOkTextPanes[3] = {"arrowName02", "string0006(1)", "string0003(1)"};
static const char* const sOkColorPanes[3] = {"col0001_01", "col0003", "col0006"};
static const char* const sPopTextPanes[3] = {"arrowName02", "string0006(1)", "string0003(1)"};
static const char* const sPosPane = "pos0001(1)";

extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);

// inferred: shows the rare item at once
// NONMATCHING: 97.57% - registers of the inlined Alloc_unk loop, this in r30, string pool offset
void ItemSlotAnime::fn_8044E644() {
    if (mFlag31) {
        mState = 3;
        mF0 = 0;
        if (m00 == 0) {
            m00 = lbl_8087F4F8->Alloc_unk();
        }
        m00->m00->m50 = 0.0f;
        m00->m00->m54 = 1.0f;
        for (int i = 0; i < 3; i++) {
            ItemSlotTask_unk* t = m00->m04[i];
            t->m50 = i * 3.0f + 2.0f;
            t->m54 = 1.0f;
        }
        for (int i = 0; i < 3; i++) {
            fn_80450590();
        }
        fn_8044F96C();
        if (lbl_8087EFE8 != 0) {
            lbl_8087EFE8->m34D8 = this;
            fn_800C344C("SE_UICOM_019", &mPos, 0, -1, 1.0f);
            lbl_8087EFE8->m34D8 = 0;
        }
        if (lbl_8087F430 != 0 && lbl_8087F430->m10D0 >= 105000 && lbl_8087F430->GetCurrentMap() != 0 &&
            lbl_8087F430->GetCurrentMap()->mType == 2) {
            lbl_8087F430->fn_803750E4(0xBA);
        }
    }
}

// inferred: shows the item at once (the slots stopped)
// NONMATCHING: 98.35% - registers of the inlined Alloc_unk loop
void ItemSlotAnime::fn_8044E88C() {
    if (mFlag31) {
        mState = 4;
        m18 = 0;
        if (m00 == 0) {
            m00 = lbl_8087F4F8->Alloc_unk();
        }
        ItemSlotTask_unk* base = m00->m00;
        base->m50 = fn_801F6C2C(base);
        for (int i = 0; i < 3; i++) {
            ItemSlotTask_unk* t = m00->m14[i];
            t->m50 = i * 3.0f + 2.0f;
            t->m54 = 1.0f;
            fn_801F791C(t, sColorPane, 0, 200.0f);
        }
        fn_8044F96C();
    }
}

// NONMATCHING: 69.7% - the original inlines fn_8044F14C at the end (defined later; here it stays a call)
// and the registers of the inlined Alloc_unk loop
// inferred: stops the slots on the item (`shown`: the item is already shown); with mFlag27 the item is taken
// at once (fn_8044F14C)
void ItemSlotAnime::fn_8044EA34(int shown) {
    if (mFlag31) {
        mState = 5;
        m18 = 0;
        if (m00 == 0) {
            m00 = lbl_8087F4F8->Alloc_unk();
        }
        m00->m00->m50 = 10.0f;
        m00->m00->m54 = 1.0f;
        for (int i = 0; i < 3; i++) {
            ItemSlotTask_unk* t = m00->m14[i];
            t->m50 = i * 3.0f + 2.0f;
            t->m54 = 0.0f;
        }
        if (shown == 0) {
            atn::FixedArray_unk<ItemSlotTask_unk*, 2>& oks = m00->m24;
            for (ItemSlotTask_unk** it = oks.begin(); it != oks.end(); it++) {
                if (*it != 0) {
                    (*it)->m50 = 5.0f;
                }
            }
            atn::FixedArray_unk<ItemSlotTask_unk*, 2>& oks2 = m00->m24;
            for (ItemSlotTask_unk** it = oks2.begin(); it != oks2.end(); it++) {
                if (*it != 0) {
                    (*it)->m54 = 0.35f;
                }
            }
            if (lbl_8087EFE8 != 0) {
                lbl_8087EFE8->fn_800CFD18(this, 0);
                fn_800C344C("SE_UICOM_020", &mPos, 0, -1, 1.0f);
            }
        } else {
            ItemSlotTask_unk* base = m00->m00;
            base->m50 = fn_801F6C2C(base);
            atn::FixedArray_unk<ItemSlotTask_unk*, 2>& oks = m00->m24;
            for (ItemSlotTask_unk** it = oks.begin(); it != oks.end(); it++) {
                if (*it != 0) {
                    (*it)->m50 = 10.0f;
                }
            }
            atn::FixedArray_unk<ItemSlotTask_unk*, 2>& oks2 = m00->m24;
            for (ItemSlotTask_unk** it = oks2.begin(); it != oks2.end(); it++) {
                if (*it != 0) {
                    (*it)->m54 = 0.35f;
                }
            }
            if (lbl_8087EFE8 != 0) {
                lbl_8087EFE8->fn_800CFD18(this, 0);
            }
        }
        fn_8044F96C();
        if (mFlag28) {
            fn_8044F14C();
        }
    }
}

// inferred: the layout animation of `task` reached its last frame (name invented)
static inline bool IsAnimeEnd_unk(ItemSlotTask_unk* task) {
    return task->m50 >= fn_801F6C2C(task);
}

// NONMATCHING: 96.4% - the state test of IsState56_unk (the original keeps mState in r4 from the start
// and sets the flag to 1 on the branches)
// inferred: the animation of the item can be skipped (loading, or the slots of states 5-6 finished)
int ItemSlotAnime::fn_8044EEC0() {
    if (mState == 1) {
        return 1;
    }
    if (IsState56_unk()) {
        ItemSlotTask_unk* a = fn_80450A78() ? m00->m24[1] : m00->m24[0];
        ItemSlotTask_unk* b = fn_80450A78() ? m00->m30[1] : m00->m30[0];
        if ((!a->IsFlag_unk(4) && IsAnimeEnd_unk(a)) || (!b->IsFlag_unk(4) && IsAnimeEnd_unk(b))) {
            return 1;
        }
        if (mState == 5 && !a->IsFlag_unk(4) && (float)__fabs(a->m50 - 10.0f) < 1.1920929e-7f &&
            IsAnimeEnd_unk(m00->m00)) {
            return 1;
        }
    }
    return 0;
}

// inferred: the item was taken
// NONMATCHING: 98.83% - registers of the inlined Alloc_unk loop
void ItemSlotAnime::fn_8044F14C() {
    mState = 6;
    if (m00 == 0) {
        m00 = lbl_8087F4F8->Alloc_unk();
    }
    if (mFlag31) {
        if (m00->m00->m50 < 20.0f) {
            m00->m00->m50 = 20.0f;
            m00->m00->m54 = 1.0f;
        }
    }
    atn::FixedArray_unk<ItemSlotTask_unk*, 2>& oks = m00->m24;
    for (ItemSlotTask_unk** it = oks.begin(); it != oks.end(); it++) {
        if (*it != 0) {
            (*it)->m50 = 10.0f;
        }
    }
    atn::FixedArray_unk<ItemSlotTask_unk*, 2>& oks2 = m00->m24;
    for (ItemSlotTask_unk** it = oks2.begin(); it != oks2.end(); it++) {
        if (*it != 0) {
            (*it)->m54 = 1.0f;
        }
    }
    fn_80450590();
    fn_8044F96C();
}

// inferred: the item could not be taken
// NONMATCHING: 98.75% - registers of the inlined Alloc_unk loop
void ItemSlotAnime::fn_8044F2D4() {
    if (mFlag31) {
        mState = 7;
        mF0 = 1;
        if (m00 == 0) {
            m00 = lbl_8087F4F8->Alloc_unk();
        }
        atn::FixedArray_unk<ItemSlotTask_unk*, 2>& pops = m00->m30;
        for (ItemSlotTask_unk** it = pops.begin(); it != pops.end(); it++) {
            if (*it != 0) {
                (*it)->m50 = 0.0f;
            }
        }
        atn::FixedArray_unk<ItemSlotTask_unk*, 2>& pops2 = m00->m30;
        for (ItemSlotTask_unk** it = pops2.begin(); it != pops2.end(); it++) {
            if (*it != 0) {
                (*it)->m54 = 1.0f;
            }
        }
        fn_80450590();
        fn_8044F96C();
    }
}

// NONMATCHING: 98% - the test of the states 3-5 (here a range check) and registers of the inlined loops
// inferred: per-frame update: steps of the slot animation (3: spin, 4: stop, 5-6: shown, 7: not taken)
void ItemSlotAnime::fn_8044F434() {
    if (lbl_8087F430 != 0 && lbl_8087F430->m54E4 == 8 && (mState == 3 || mState == 4 || mState == 5)) {
        if (!fn_8044EEC0()) {
            fn_8044EA34(1);
        }
    }
    if (lbl_8087F4F8 != 0 ? lbl_8087F4F8->IsFrozen_unk() : false) {
        return;
    }
    fn_80450590();
    fn_8044F96C();
    if (mState == 3) {
        if (--m18 <= 0) {
            fn_8044E88C();
        }
    } else if (mState == 4) {
        if (--m1C <= 0) {
            fn_8044EA34(0);
        }
    } else if (mState == 5) {
        if (fn_8044EEC0()) {
            mState = 1;
            if (m00 != 0 && !fn_80450A78()) {
                atn::FixedArray_unk<ItemSlotTask_unk*, 2>& oks = m00->m24;
                for (ItemSlotTask_unk** it = oks.begin(); it != oks.end(); it++) {
                    if (*it != 0) {
                        (*it)->FreezeTask();
                    }
                }
                lbl_8087F4F8->Free_unk(m00);
                m00 = 0;
            }
        }
    } else if (mState == 6) {
        if (fn_8044EEC0()) {
            mState = 1;
            if (m00 != 0) {
                atn::FixedArray_unk<ItemSlotTask_unk*, 16>& tasks = m00->mTasks;
                for (ItemSlotTask_unk** it = tasks.begin(); it != tasks.end(); it++) {
                    if (*it != 0) {
                        (*it)->FreezeTask();
                    }
                }
                lbl_8087F4F8->Free_unk(m00);
                m00 = 0;
            }
        }
    } else if (mState == 7) {
        ItemSlotTask_unk* pop = fn_80450A78() ? m00->m30[1] : m00->m30[0];
        if (pop->m50 >= 10.0f && mFlag31) {
            mState = 5;
            if (m00 == 0) {
                m00 = lbl_8087F4F8->Alloc_unk();
            }
            atn::FixedArray_unk<ItemSlotTask_unk*, 2>& pops = m00->m30;
            for (ItemSlotTask_unk** it = pops.begin(); it != pops.end(); it++) {
                if (*it != 0) {
                    (*it)->m50 = 10.0f;
                }
            }
            atn::FixedArray_unk<ItemSlotTask_unk*, 2>& pops2 = m00->m30;
            for (ItemSlotTask_unk** it = pops2.begin(); it != pops2.end(); it++) {
                if (*it != 0) {
                    (*it)->m54 = 1.0f;
                }
            }
            fn_8044F96C();
        }
    }
}

extern "C" void fn_801F6D7C(atn::Task* task, const char* name, int index, float value); // inferred: pane value
extern "C" void fn_801F837C(atn::Task* task, const char* name, const wchar_t* text);    // inferred: pane text

// inferred: the "take" layout shown for the item (name invented)
inline ItemSlotTask_unk* ItemSlotAnime::OkTask_unk() const {
    return fn_80450A78() ? m00->m24[1] : m00->m24[0];
}

// inferred: the popup layout shown for the item (name invented)
inline ItemSlotTask_unk* ItemSlotAnime::PopTask_unk() const {
    return fn_80450A78() ? m00->m30[1] : m00->m30[0];
}

// inferred: alpha rate of the "take" layout fading in (frames 5-10) and out (10-15); names invented
static inline float FadeIn_unk(float frame) {
    float rate = (10.0f - frame) / 5.0f;
    return rate > 0.0f ? rate : 0.0f;
}
static inline float FadeOut_unk(float frame) {
    float rate = (5.0f - (frame - 10.0f)) / 5.0f;
    return rate > 0.0f ? rate : 0.0f;
}

// inferred: clears a text pane (name invented)
inline void ItemSlotAnime::ClearText_unk(ItemSlotTask_unk* task, const char* pane) {
    fn_801F837C(task, pane, L"");
}

// NONMATCHING: 99.27% - registers of the iterator loops (iterator and end base swapped) and of the selected
// layouts (r27 vs r30)
// inferred: shows the layouts of the current step and writes their panes (position, texts, alpha)
void ItemSlotAnime::fn_8044F96C() {
    bool loading = mState == 1 || mState == 2;
    if (loading && !mFlag26 || m00 == 0) {
        if (m00 != 0) {
            ItemSlotTask_unk* frame = m00->m3C;
            if (!frame->IsFlag_unk(4)) {
                frame->FreezeTask();
            }
            if (mState == 1) {
                atn::FixedArray_unk<ItemSlotTask_unk*, 16>& tasks = m00->mTasks;
                for (ItemSlotTask_unk** it = tasks.begin(); it != tasks.end(); it++) {
                    if (*it != 0) {
                        (*it)->FreezeTask();
                    }
                }
            }
        }
        return;
    }
    atn::FixedArray_unk<ItemSlotTask_unk*, 16>& tasks = m00->mTasks;
    for (ItemSlotTask_unk** it = tasks.begin(); it != tasks.end(); it++) {
        if (*it != 0) {
            (*it)->FreezeTask();
        }
    }
    atn::Vector3 screen = atn::RenderManager::instance_->GetScreenPosition(mPos);
    if (0.0f <= screen.z && screen.z <= 1.0f) {
        float x = screen.x;
        atn::FixedArray_unk<ItemSlotTask_unk*, 16>& t0 = m00->mTasks;
        for (ItemSlotTask_unk** it = t0.begin(); it != t0.end(); it++) {
            if (*it != 0) {
                fn_801F6D7C(*it, sPosPane, 0, x);
            }
        }
        float y = screen.y;
        atn::FixedArray_unk<ItemSlotTask_unk*, 16>& t1 = m00->mTasks;
        for (ItemSlotTask_unk** it = t1.begin(); it != t1.end(); it++) {
            if (*it != 0) {
                fn_801F6D7C(*it, sPosPane, 1, y);
            }
        }
        float sx = mScale;
        atn::FixedArray_unk<ItemSlotTask_unk*, 16>& t2 = m00->mTasks;
        for (ItemSlotTask_unk** it = t2.begin(); it != t2.end(); it++) {
            if (*it != 0) {
                fn_801F6D7C(*it, sPosPane, 2, sx);
            }
        }
        float sy = mScale;
        atn::FixedArray_unk<ItemSlotTask_unk*, 16>& t3 = m00->mTasks;
        for (ItemSlotTask_unk** it = t3.begin(); it != t3.end(); it++) {
            if (*it != 0) {
                fn_801F6D7C(*it, sPosPane, 3, sy);
            }
        }
    }
    if (lbl_8087F610 != 0 && !lbl_8087F610->IsStarting_unk()) {
        return;
    }
    if (mFlag27 && !mFlag25 && fn_80450A78() && (mState == 5 || mState == 6 || mFlag26)) {
        if (mFlag29 || (OkTask_unk() != 0 && OkTask_unk()->m50 < 30.0f)) {
            m00->m3C->MeltTask();
        } else {
            m00->m3C->FreezeTask();
        }
    } else {
        m00->m3C->FreezeTask();
    }
    if (mState == 3) {
        m00->m00->MeltTask();
        if (m00->m00->m50 > 10.0f) {
            m00->m00->m50 = 10.0f;
        }
        for (int i = 0; i < 3; i++) {
            ItemSlotTask_unk* slot = m00->m04[i];
            if (m00->m00->m50 >= 10.0f) {
                slot->MeltTask();
                for (int j = 0; j < 7; j++) {
                    fn_801F837C(slot, sSlotTextPanes[j], mTexts[i].mStr);
                }
            }
        }
    } else if (mState == 4 || mState == 5) {
        if (mF0 == 0) {
            m00->m00->MeltTask();
            if (mState == 4 && m00->m00->m50 > 10.0f) {
                m00->m00->m50 = 10.0f;
            }
            for (int i = 0; i < 3; i++) {
                ItemSlotTask_unk* arrow = m00->m14[i];
                arrow->MeltTask();
                if (mState == 4 || i != 1) {
                    fn_801F837C(arrow, sArrowPane, mTexts[i].mStr);
                } else {
                    ClearText_unk(arrow, sArrowPane);
                }
            }
            if (mState == 5) {
                ItemSlotTask_unk* ok = OkTask_unk();
                ok->MeltTask();
                if (ok->m50 > 10.0f) {
                    ok->m50 = 10.0f;
                }
                float rate = FadeIn_unk(ok->m50) < 1.0f ? FadeIn_unk(ok->m50) : 1.0f;
                float alpha = 200.0f * rate;
                for (int i = 0; i < 3; i++) {
                    fn_801F791C(m00->m14[i], sColorPane, 0, alpha);
                }
                for (int i = 0; i < 3; i++) {
                    fn_801F791C(ok, sOkColorPanes[i], 0, alpha);
                    if (mFlag29 || i != 0) {
                fn_801F837C(ok, sOkTextPanes[i], mTexts[1].mStr);
            } else {
                ClearText_unk(ok, sOkTextPanes[i]);
            }
                }
            }
        } else {
            PopTask_unk()->MeltTask();
        }
    } else if (mState == 6) {
        if (mFlag31) {
            m00->m00->MeltTask();
        }
        ItemSlotTask_unk* ok = OkTask_unk();
        ok->MeltTask();
        float rate = FadeOut_unk(ok->m50) < 1.0f ? FadeOut_unk(ok->m50) : 1.0f;
        float alpha = 200.0f * rate;
        for (int i = 0; i < 3; i++) {
            fn_801F791C(m00->m14[i], sColorPane, 0, alpha);
        }
        for (int i = 0; i < 3; i++) {
            fn_801F791C(ok, sOkColorPanes[i], 0, 200.0f);
            if (mFlag29 || i != 0) {
                fn_801F837C(ok, sOkTextPanes[i], mTexts[1].mStr);
            } else {
                ClearText_unk(ok, sOkTextPanes[i]);
            }
        }
    } else if (mState == 7) {
        ItemSlotTask_unk* pop = PopTask_unk();
        pop->MeltTask();
        for (int i = 0; i < 3; i++) {
            fn_801F837C(pop, sPopTextPanes[i], mTexts[0].mStr);
        }
    }
}

} // namespace gmk

extern "C" void fn_80444CF8(wchar_t* text, int item, int count, int rare); // inferred: item name
extern "C" int fn_804515A8(gmk::ItemSlotAnime* anime, wchar_t* text);      // inferred: text of the current step

namespace gmk {

// inferred: writes the name of the current item to the line `line` (name invented)
static inline void SetItemLine_unk(ItemSlotAnime* anime, int line) {
    wchar_t text[64];
    memset(text, 0, sizeof(text));
    if (anime->m20 != 0) {
        fn_80444CF8(text, anime->m20->mId, anime->m24, anime->mFlag30);
    }
    anime->mTexts[line] = text;
}

// inferred: updates the texts of the step: states 3-4 push a new line, state 7 replaces the first line,
// the others the second
void ItemSlotAnime::fn_80450590() {
    bool loading = mState == 1 || mState == 2;
    if (!loading && m20 != 0 && mState >= 3) {
        wchar_t text[64];
        memset(text, 0, sizeof(text));
        if (mState != 7) {
            if ((unsigned int)(mState - 3) <= 1) {
                for (int i = 2; i > 0; i--) {
                    mTexts[i] = mTexts[i - 1];
                }
                fn_804515A8(this, text);
                mTexts[0] = text;
            } else {
                wchar_t line[64];
                memset(line, 0, sizeof(line));
                if (m20 != 0) {
                    fn_80444CF8(line, m20->mId, m24, mFlag30);
                }
                mTexts[1] = line;
            }
        } else {
            SetItemLine_unk(this, 0);
        }
    }
}

} // namespace gmk

// inferred: damage result of fn_8003EA3C (as TashaDamage_unk of game/EnemyTasha.cpp; names invented)
struct ItemDamage_unk {
    ItemDamage_unk() : m00(0), m04(0), m08(0), m0C(0), m10(0), m14(-1) {
        mFlags31_28 = 0;
        m18 = -1;
    }
    int m00;
    int m04;
    int m08;
    int m0C;
    int m10;
    int m14;
    int m18;
    unsigned int mFlags31_28 : 4;
    unsigned int mFlags27_0 : 28;
};
extern "C" void* lbl_8087F048;              // inferred: effect/bullet manager
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0;
extern "C" void fn_801092C8(void* mgr, int arg1, int arg2, int item, int num); // inferred: take message
extern "C" HitInfo_unk* fn_80219E6C(int id);                                     // inferred: skill data
extern "C" int fn_80206B9C(int code);                                            // inferred: weapon code
extern "C" void fn_8003EA3C(ItemDamage_unk* result, HitInfo_unk* data, CharaTask* attacker, CharaTask* owner,
                            unsigned int flags, const void* param, int arg, int arg2); // inferred: computes the damage
extern "C" unsigned char lbl_807C6B90[]; // inferred: default parameters of fn_8003EA3C
extern "C" void fn_80108C10(void* effect, ItemDamage_unk* damage, const atn::Vector3& pos, int a, CharaTask* chara,
                            int b, int c);

namespace gmk {

// NONMATCHING: 97.4% - this and the leader in r30/r31 swapped, the early exit when m20 is null, cmplwi of mC0
// inferred: gives the item to the party: money, a skill used on the leader, or an item of the inventory;
// then the messages
void ItemSlotAnime::fn_80450778() {
    Inventory* inventory = Inventory::instance_;
    if (inventory != 0) {
        if (m20 != 0) {
            if (m20->mId == 10004 || m20->mId == 10009) {
                inventory->AddMoney(m24);
                if (lbl_8087F048 != 0) {
                    fn_801092C8(lbl_8087F048, 3, 0, m24, 0);
                }
            } else if (m20 != 0 && (m20->mC0 == 3 || (m20->mAC & 0x100) == 0x100)) {
                PlayerTask* leader = lbl_8087F8A0->mHead;
                HitInfo_unk* skill = m20->mC4 > 0 ? fn_80219E6C(m20->mC4) : 0;
                if (leader != 0 && skill != 0 && (lbl_8087F610 == 0 || leader->CanAct_unk())) {
                    ItemDamage_unk result;
                    fn_8003EA3C(&result, skill, leader, leader, 0, lbl_807C6B90, 0, 0);
                    void* effect = lbl_8087F048;
                    if (effect != 0) {
                        fn_80108C10(effect, &result, leader->vfAC(), 0, 0, 0, 1);
                    }
                }
            } else {
                inventory->AddItem(m20->mId, m24, mFlag30, 0, 0, 1, 1);
                if (lbl_8087F048 != 0) {
                    if (fn_80206B9C(m20->mId) && !mFlag30) {
                        fn_801092C8(lbl_8087F048, 2, 0, m20->mId, m24);
                    } else if (m20->mBC != 6) {
                        fn_801092C8(lbl_8087F048, 1, 0, m20->mId, m24);
                    }
                }
            }
        }
        if (m20 != 0 && m20->mB4 != 0 && lbl_8087F490 != 0) {
            lbl_8087F490->fn_803E5E64(m20->mId, m24, 1);
        }
    }
}

int ItemSlotAnime::fn_80450A5C() {
    return m20 != 0 ? m20->mId : 0;
}

// inferred: the item may be taken here (301 and flagged items always; the others by the item table of the
// current map)
int ItemSlotAnime::fn_80450A78() const {
    if (m20 == 0) {
        return 0;
    }
    if (m20->mId == 301) {
        return 1;
    }
    if (m20 == 0) {
        return 0;
    }
    if (m20->mC2 != 0) {
        return 1;
    }
    if (lbl_8087F430 != 0 && lbl_8087F430->GetCurrentMap() != 0) {
        MapTask* map = lbl_8087F430->GetCurrentMap();
        return fn_80213E60(m20, map->mType, map->mId, map->mSubId);
    }
    return 0;
}

} // namespace gmk


extern "C" void fn_80450B28() {
    if (lbl_8087F4F8 != 0) {
        lbl_8087F4F8->mFlags |= 4;
    }
}

extern "C" void fn_80450B44() {
    if (lbl_8087F4F8 != 0) {
        lbl_8087F4F8->mFlags &= ~4;
    }
}

extern "C" int fn_80450B60(int value) {
    unsigned int index = value - 0x2711;
    if (index > 7) {
        if (value != 0x271A) {
            goto rejected;
        }
    }
    return 1;
rejected:
    return 0;
}

// inferred: an item that a treasure table can give (0xC bytes, filled by fn_80445F44; names invented)
struct ItemSlotDraw_unk {
    ItemSlotDraw_unk() : mId(0), m04(1), m06(10), m08(0) {}
    int mId;
    short m04;
    short m06; // inferred: weight of the item in the draw
    int m08;
};
typedef atn::FixedArray_unk<ItemSlotDraw_unk, 384> ItemSlotList_unk;
// inferred: items of the treasure table `mTable` for the item kind `mKind` (0x120C bytes)
struct ItemSlotRecord_unk {
    int mTable;
    int mKind;
    ItemSlotList_unk mList;
};
// inferred: treasure tables of the item boxes (db/TreasureBoxTable_unk.cpp)
struct ItemSlotBoxItem_unk {
    int mId;
    int mNum;
    float mRate;
};
struct ItemSlotBox_unk {
    int mId;
    ItemSlotBoxItem_unk* mItems;
    unsigned int mCount;
};
extern "C" ItemSlotBox_unk* fn_8021E444(int id); // inferred: treasure box of an id
extern "C" int fn_8040B394(int item);              // inferred: kind of an item (gmk/DropItem.cpp)
// inferred: items of a treasure entry (nonzero when there are some)
extern "C" int fn_80445F44(Inventory* inv, ItemSlotList_unk* out, int id, int num, int arg);

namespace gmk {

// inferred: cache of the items of the treasure tables (vtable 0x8078F348, instance lbl_8087F4FC):
// twelve records reused in turn, cleared when the map changes
class ItemSlotTextCache : public atn::Task {
public:
    virtual ~ItemSlotTextCache();

    ItemSlotRecord_unk mRecords[12]; // 0x48
    int mNext;                       // 0xD8D8: inferred: next record to reuse
    int mMapType;                    // 0xD8DC
    int mMapId;
    int mMapSubId;
};

} // namespace gmk

extern "C" gmk::ItemSlotTextCache* lbl_8087F4FC; // inferred: instance of gmk::ItemSlotTextCache

// inferred: the treasure table `table` gives more than one item (for the kind of `item` when it is
// positive); the list is kept in the cache
// NONMATCHING: 97.3% - register allocation of the record pointers and the early return of a missing box
extern "C" int fn_80450B84(int table, int item, int arg) {
    if (lbl_8087F610 != 0) {
        return 0;
    }
    ItemSlotBox_unk* box = fn_8021E444(table);
    if (box != 0) {
        ItemSlotList_unk list;
        gmk::ItemSlotTextCache* cache = lbl_8087F4FC;
        if (cache != 0) {
            if (lbl_8087F430 != 0 && lbl_8087F430->GetCurrentMap() != 0) {
                MapTask* map = lbl_8087F430->GetCurrentMap();
                if (cache->mMapType != map->mType || cache->mMapId != map->mId || cache->mMapSubId != map->mSubId) {
                    cache->mMapType = map->mType;
                    cache->mMapId = map->mId;
                    cache->mMapSubId = map->mSubId;
                    for (int i = 0; i < 12; i++) {
                        cache->mRecords[i].mTable = 0;
                        cache->mRecords[i].mKind = 0;
                        cache->mRecords[i].mList.clear();
                    }
                }
            }
            int index = -1;
            int kind;
            if (item > 0) {
                kind = fn_8040B394(item);
            } else {
                kind = -1;
            }
            for (int i = 0; i < 12; i++) {
                if (cache->mRecords[i].mTable == table && cache->mRecords[i].mKind == kind) {
                    index = i;
                    break;
                }
            }
            if (index < 0) {
                index = cache->mNext;
                cache->mNext = (index + 1) % 12;
                ItemSlotRecord_unk* rec = &cache->mRecords[index];
                rec->mTable = table;
                rec->mKind = kind;
                ItemSlotBox_unk* entries = fn_8021E444(table);
                for (unsigned int j = 0; j < entries->mCount; j++) {
                    ItemSlotBoxItem_unk* entry = &entries->mItems[j];
                    if (entry->mRate > 0.0f && entry->mId > 0) {
                        ItemSlotList_unk items;
                        if (fn_80445F44(Inventory::instance_, &items, entry->mId, entry->mNum, arg)) {
                            for (unsigned int k = 0; k < items.size(); k++) {
                                if (rec->mList.size() < 384) {
                                    rec->mList.push_back(items.mData[k]);
                                    (rec->mList.end() - 1)->m06 =
                                        MAX(entry->mRate / (float)items.size(), 1.0f);
                                }
                            }
                        }
                    }
                }
                if (item > 0) {
                    for (ItemSlotDraw_unk* p = rec->mList.begin(); p != rec->mList.end();) {
                        if (kind != fn_8040B394(p->mId)) {
                            rec->mList.erase(p);
                        } else {
                            p++;
                        }
                    }
                }
            }
            if (index >= 0) {
                ItemSlotList_unk& src = cache->mRecords[index].mList;
                list.clear();
                for (unsigned int k = 0; k < src.size(); k++) {
                    if (list.size() < 384) {
                        list.push_back(src.mData[k]);
                    }
                }
            }
        } else {
            for (unsigned int j = 0; j < box->mCount; j++) {
                ItemSlotBoxItem_unk* entry = &box->mItems[j];
                if (entry->mRate > 0.0f && entry->mId > 0) {
                    ItemSlotList_unk items;
                    if (fn_80445F44(Inventory::instance_, &items, entry->mId, entry->mNum, arg)) {
                        for (unsigned int k = 0; k < items.size(); k++) {
                            list.push_back(items.mData[k]);
                        }
                    }
                }
            }
        }
        if (list.size() > 1) {
            return 1;
        }
    }
    return 0;
}

extern "C" int fn_80206B68();                 // inferred: number of weapons
extern "C" void* fn_80206B70(int index);      // inferred: weapon by index
extern "C" int fn_80206BE4(void* weapon);     // inferred: item id of a weapon
extern "C" int fn_8020EF04(int id);           // inferred: the item is an equipment (db/EquipParam_unk)
extern "C" int fn_8020EE58();                 // inferred: number of equipments
// inferred: equipment entry (only the kind and the part are used here)
struct ItemSlotEquip_unk {
    unsigned char m00[0x78];
    int mKind; // 0x78: 2 and 3: parts of a costume
    int mPart; // 0x7C
};
extern "C" ItemSlotEquip_unk* fn_8020EE60(int index);        // inferred: equipment by index
extern "C" ItemSlotEquip_unk* fn_8020ED84(int slot, int part); // inferred: equipment of a costume part
extern "C" int fn_8020EF80(ItemSlotEquip_unk* entry);         // inferred: item id of an equipment
// inferred: item data (only the id is used here)
struct ItemSlotData_unk {
    int m00;
    int mId;
};
extern "C" int fn_802114D8();                     // inferred: number of items
extern "C" ItemSlotData_unk* fn_802114E0(int index); // inferred: item by index
extern "C" int rand();

// inferred: writes in `text` the name of the item drawn for the current step: from the treasure
// table m28 (weighted by the rates, through the cache), else a random weapon, equipment or item of
// the same kind as m20; 0 when there is no item
// NONMATCHING: 97.4% - register allocation (same code as fn_80450B84 for the list of the table)
extern "C" int fn_804515A8(gmk::ItemSlotAnime* anime, wchar_t* text) {
    if (anime->m20 == 0 || text == 0) {
        return 0;
    }
    if (anime->m28 > 0) {
        ItemSlotBox_unk* box = fn_8021E444(anime->m28);
        if (box != 0) {
            ItemSlotList_unk list;
            gmk::ItemSlotTextCache* cache = lbl_8087F4FC;
            if (cache != 0) {
                bool arg = !anime->mFlag24;
                int item;
                if (anime->mFlag24) {
                    item = anime->m20->mId;
                } else {
                    item = 0;
                }
                int table = anime->m28;
                if (lbl_8087F430 != 0 && lbl_8087F430->GetCurrentMap() != 0) {
                    MapTask* map = lbl_8087F430->GetCurrentMap();
                    if (cache->mMapType != map->mType || cache->mMapId != map->mId ||
                        cache->mMapSubId != map->mSubId) {
                        cache->mMapType = map->mType;
                        cache->mMapId = map->mId;
                        cache->mMapSubId = map->mSubId;
                        for (int i = 0; i < 12; i++) {
                            cache->mRecords[i].mTable = 0;
                            cache->mRecords[i].mKind = 0;
                            cache->mRecords[i].mList.clear();
                        }
                    }
                }
                int index = -1;
                int kind;
                if (item > 0) {
                    kind = fn_8040B394(item);
                } else {
                    kind = -1;
                }
                for (int i = 0; i < 12; i++) {
                    if (cache->mRecords[i].mTable == table && cache->mRecords[i].mKind == kind) {
                        index = i;
                        break;
                    }
                }
                if (index < 0) {
                    index = cache->mNext;
                    cache->mNext = (index + 1) % 12;
                    ItemSlotRecord_unk* rec = &cache->mRecords[index];
                    rec->mTable = table;
                    rec->mKind = kind;
                    ItemSlotBox_unk* entries = fn_8021E444(table);
                    for (unsigned int j = 0; j < entries->mCount; j++) {
                        ItemSlotBoxItem_unk* entry = &entries->mItems[j];
                        if (entry->mRate > 0.0f && entry->mId > 0) {
                            ItemSlotList_unk items;
                            if (fn_80445F44(Inventory::instance_, &items, entry->mId, entry->mNum, arg)) {
                                for (unsigned int k = 0; k < items.size(); k++) {
                                    if (rec->mList.size() < 384) {
                                        rec->mList.push_back(items.mData[k]);
                                        (rec->mList.end() - 1)->m06 =
                                            MAX(entry->mRate / (float)items.size(), 1.0f);
                                    }
                                }
                            }
                        }
                    }
                    if (item > 0) {
                        for (ItemSlotDraw_unk* p = rec->mList.begin(); p != rec->mList.end();) {
                            if (kind != fn_8040B394(p->mId)) {
                                rec->mList.erase(p);
                            } else {
                                p++;
                            }
                        }
                    }
                }
                if (index >= 0) {
                    ItemSlotList_unk& src = cache->mRecords[index].mList;
                    list.clear();
                    for (unsigned int k = 0; k < src.size(); k++) {
                        if (list.size() < 384) {
                            list.push_back(src.mData[k]);
                        }
                    }
                }
            } else {
                for (unsigned int j = 0; j < box->mCount; j++) {
                    ItemSlotBoxItem_unk* entry = &box->mItems[j];
                    if (entry->mRate > 0.0f && entry->mId > 0) {
                        ItemSlotList_unk items;
                        if (fn_80445F44(Inventory::instance_, &items, entry->mId, entry->mNum, 0)) {
                            for (unsigned int k = 0; k < items.size(); k++) {
                                list.push_back(items.mData[k]);
                                (list.end() - 1)->m06 = MAX(entry->mRate / (float)items.size(), 1.0f);
                            }
                        }
                    }
                }
            }
            if (list.size() != 0) {
                int total = 0;
                for (unsigned int k = 0; k < list.size(); k++) {
                    total += list.mData[k].m06;
                }
                int r = rand() % total;
                for (ItemSlotDraw_unk* p = list.begin(); p != list.end(); p++) {
                    if (r < p->m06) {
                        fn_80444CF8(text, p->mId, p->m04, anime->mFlag30);
                        break;
                    }
                    r -= p->m06;
                }
            } else {
                int saved = anime->m28;
                anime->m28 = 0;
                fn_804515A8(anime, text);
                anime->m28 = saved;
            }
        }
    } else if (fn_80206B9C(anime->m20->mId)) {
        void* weapon = fn_80206B70(rand() % fn_80206B68());
        if (!anime->mFlag30) {
            rand();
        }
        fn_80444CF8(text, fn_80206BE4(weapon), 1, anime->mFlag30);
    } else if (fn_8020EF04(anime->m20->mId)) {
        ItemSlotEquip_unk* entry = fn_8020EE60(rand() % fn_8020EE58());
        if (entry->mKind == 2) {
            entry = fn_8020ED84(0, entry->mPart);
        } else if (entry->mKind == 3) {
            entry = fn_8020ED84(1, entry->mPart);
        }
        fn_80444CF8(text, fn_8020EF80(entry), 1, 1);
    } else if (anime->m20->mId == 10004) {
        int count = MAX(anime->m24 * (rand() % 5 - 2), 1);
        fn_80444CF8(text, anime->m20->mId, rand() % count, 1);
    } else if (anime->m20->mId == 10009) {
        fn_80444CF8(text, anime->m20->mId, anime->m24, 1);
    } else {
        ItemSlotData_unk* data = fn_802114E0(rand() % fn_802114D8());
        fn_80444CF8(text, data->mId, rand() % (anime->m24 * 5), 1);
    }
    return 1;
}

namespace gmk {

ItemSlotTextCache::~ItemSlotTextCache() {
    lbl_8087F4FC = 0;
}

} // namespace gmk

namespace gmk {

// inferred: once the tasks of every entry are ready they are shown (and frozen until used)
inline bool ItemSlotAnimeBuffer::Start_unk() {
    for (unsigned int i = 0; i < mNum; i++) {
        if (!mEntries[i].IsReady_unk()) {
            return true;
        }
    }
    for (unsigned int i = 0; i < mNum; i++) {
        RecycleBufferEntry_unk* e = &mEntries[i];
        for (ItemSlotTask_unk** it = e->mTasks.begin(); it != e->mTasks.end(); it++) {
            if (*it != 0) {
                (*it)->fn_800D246C(0);
            }
        }
        for (ItemSlotTask_unk** it = e->mTasks.begin(); it != e->mTasks.end(); it++) {
            if (*it != 0) {
                (*it)->FreezeTask();
            }
        }
        for (ItemSlotTask_unk** it = e->mTasks.begin(); it != e->mTasks.end(); it++) {
            if (*it != 0) {
                (*it)->m4D = 0;
            }
        }
        for (ItemSlotTask_unk** it = e->mTasks.begin(); it != e->mTasks.end(); it++) {
            if (*it != 0) {
                (*it)->m4C = 1;
            }
        }
        for (ItemSlotTask_unk** it = e->m04.begin(); it != e->m04.end(); it++) {
            if (*it != 0) {
                (*it)->m4D = 1;
            }
        }
        for (ItemSlotTask_unk** it = e->m14.begin(); it != e->m14.end(); it++) {
            if (*it != 0) {
                (*it)->m4D = 1;
            }
        }
        e->m3C->m4D = 1;
    }
    return false;
}

int ItemSlotAnimeBuffer::vf0C() {
    return !Start_unk();
}

// inferred: clears the entries and the instance pointer
ItemSlotAnimeBuffer::~ItemSlotAnimeBuffer() {
    ClearAll_unk();
    lbl_8087F4F8 = 0;
}


} // namespace gmk
